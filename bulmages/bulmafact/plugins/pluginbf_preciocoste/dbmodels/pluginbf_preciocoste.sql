--
-- Modificación de campos y funciones de la BD para la adaptacion para el plugin de Trazabilidad
--
\echo "********* INICIADO FICHERO DE ESTRUCTURA DEL PLUGIN DE PRECIO COSTE *********"

\echo ":: Establecemos los mensajes minimos a avisos y otros parametros ... "
\echo -n ":: "
SET client_min_messages TO WARNING;
SET log_min_messages TO WARNING;
-- SET log_error_verbosity TO TERSE;
BEGIN;

--
-- Estas primeras funciones cambiaran los tipos de columnas que está como flotantes a NUMERIC.
-- Se trata de un parche que se desea aplicar para almacenar los tipos monetarios
-- ya que actualmente se encuantran almacenados como 'doubles' y es preferible
-- que se almacenen como tipo 'numeric'.
-- Todas devuelven como valor numérico el nmero de filas influenciadas por el cambio
-- NOTA: Si alguien sabe como pasar por parámetro un nombre de tabla y campo a modificar se
-- hará mucho más sencillito porque sólo habrá que implementar un función ya que siempre
-- hay que hacer lo mismo.
--

--
-- Función auxiliar para borrar funciones limpiamente
--
create or replace function drop_if_exists_table (text) returns INTEGER AS '
DECLARE
    tbl_name ALIAS FOR $1;

BEGIN
    IF (select count(*) from pg_tables where tablename=$1) THEN
	EXECUTE ''DROP TABLE '' || $1;
	RETURN 1;
    END IF;

    RETURN 0;
END;
'
language 'plpgsql';


create or replace function drop_if_exists_proc (text,text) returns INTEGER AS '
DECLARE
    proc_name ALIAS FOR $1;
    proc_params ALIAS FOR $2;

BEGIN
    IF (select count(*) from pg_proc where proname=$1) THEN
	EXECUTE ''DROP FUNCTION '' || $1 || ''(''||$2||'') CASCADE'';
	RETURN 1;
    END IF;

    RETURN 0;
END;
'
language 'plpgsql';


CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
	rs RECORD;

BEGIN
	SELECT INTO rs * FROM pg_attribute  WHERE attname=''preciocostearticulo'';
	IF NOT FOUND THEN
		ALTER TABLE articulo ADD COLUMN preciocostearticulo NUMERIC(12,2) DEFAULT 0;
	END IF;

	SELECT INTO rs * FROM pg_attribute  WHERE attname=''margenarticulo'';
	IF NOT FOUND THEN
		ALTER TABLE articulo ADD COLUMN margenarticulo NUMERIC(12,2) DEFAULT 0;
		ALTER TABLE articulo ADD COLUMN actualizarmargenarticulo BOOLEAN DEFAULT FALSE;
	END IF;

	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo "Agregamos el campo precio de coste a los articulos"


SELECT drop_if_exists_proc('actpvparticulo', '');
\echo -n ':: Funcion para actualizar precios de venta segun precios de coste ... '
CREATE OR REPLACE FUNCTION actpvparticulo() RETURNS "trigger" AS '
DECLARE
    rs RECORD;
    bs RECORD;

BEGIN
    IF NEW.actualizarmargenarticulo  = TRUE THEN
	NEW.pvparticulo := NEW.preciocostearticulo + NEW.preciocostearticulo * NEW.margenarticulo / 100;

	SELECT INTO rs * FROM pg_attribute WHERE attname =''pvpivaincarticulo'';
	IF FOUND THEN
	    SELECT INTO bs * FROM tasa_iva where idtipo_iva= NEW.idtipo_iva ORDER BY fechatasa_iva DESC LIMIT 1;
	    IF FOUND THEN
		NEW.pvpivaincarticulo := NEW.pvparticulo * (100 + bs.porcentasa_iva) / 100;
	    END IF;
	END IF;
    END IF;

    RETURN NEW;
END;
' LANGUAGE plpgsql;

\echo "Agregamos el trigger que hace la actualizacion de precio de coste"

CREATE TRIGGER actpvparticulot
    BEFORE INSERT OR UPDATE ON articulo
    FOR EACH ROW
    EXECUTE PROCEDURE actpvparticulo();




CREATE OR REPLACE FUNCTION calcstocks() RETURNS INTEGER AS '
DECLARE
	ms RECORD;
	bs RECORD;
	cs RECORD;
	salidas1 NUMERIC(12,2);
	entradas1 NUMERIC(12,2);
BEGIN

    FOR ms IN SELECT * FROM articulo  LOOP
	SELECT INTO bs sum(cantlalbaran) AS salidas FROM lalbaran WHERE idarticulo = ms.idarticulo;

	IF bs.salidas IS NOT NULL THEN
		salidas1 := bs.salidas;
	ELSE
		salidas1 := 0;
	END IF;

	SELECT INTO cs sum(cantlalbaranp) AS entradas FROM lalbaranp WHERE idarticulo = ms.idarticulo;

	IF cs.entradas IS NOT NULL THEN
		entradas1 := cs.entradas;
	ELSE
		entradas1 := 0;
	END IF;

	UPDATE articulo set stockarticulo = entradas1 - salidas1 WHERE idarticulo =ms.idarticulo;

	RAISE NOTICE '' Articulo % con entradas % y salidas %'', ms.idarticulo, entradas1, salidas1;
    END LOOP;

    RETURN 0;
END;
'   LANGUAGE plpgsql;

-- ==============================================================================


-- Agregamos nuevos parametros de configuracion
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	rs RECORD;

BEGIN
	SELECT INTO rs * FROM configuracion WHERE nombre=''PluginBf_PrecioCoste'';

	IF FOUND THEN
		UPDATE CONFIGURACION SET valor=''0.13.1-0001'' WHERE nombre=''PluginBf_PrecioCoste'';
	ELSE
		INSERT INTO configuracion (nombre, valor) VALUES (''PluginBf_PrecioCoste'', ''0.13.1-0001'');
	END IF;

	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT actualizarevision();
DROP FUNCTION actualizarevision() CASCADE;
\echo "Actualizada la revision de la base de datos"


DROP FUNCTION drop_if_exists_table(text) CASCADE;
DROP FUNCTION drop_if_exists_proc(text,text) CASCADE;


COMMIT;
