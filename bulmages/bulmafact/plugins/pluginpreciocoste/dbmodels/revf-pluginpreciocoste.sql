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
-- Estas primeras funciones cambiar� los tipos de columnas que est� como flotantes a NUMERIC.
-- Se trata de un parche que se desea aplicar para almacenar los tipos monetarios
-- ya que actualmente se encuantran almacenados como 'doubles' y es preferible
-- que se almacenen como tipo 'numeric'.
-- Todas devuelven como valor num�ico el nmero de filas influenciadas por el cambio
-- NOTA: Si alguien sabe como pasar por par�etro un nombre de tabla y campo a modificar se
-- har� mucho m� sencillito porque s�o habr� que implementar un funci� ya que siempre
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
	as RECORD;
BEGIN
	SELECT INTO as * FROM pg_attribute  WHERE attname=''preciocostearticulo'';
	IF NOT FOUND THEN
		ALTER TABLE articulo ADD COLUMN preciocostearticulo NUMERIC(12,2) DEFAULT 0;
	END IF;

	SELECT INTO as * FROM pg_attribute  WHERE attname=''margenarticulo'';
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
BEGIN
    IF NEW.actualizarmargenarticulo  = TRUE THEN
	NEW.pvparticulo := NEW.preciocostearticulo + NEW.preciocostearticulo * NEW.margenarticulo / 100;
    END IF;
    RETURN NEW;
END;
' LANGUAGE plpgsql;

\echo "Agregamos el trigger que hace la actualizacion de precio de coste.

CREATE TRIGGER actpvparticulot
    BEFORE INSERT OR UPDATE ON articulo
    FOR EACH ROW
    EXECUTE PROCEDURE actpvparticulo();


-- ==============================================================================


-- Agregamos nuevos parametros de configuracion
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	as RECORD;
BEGIN
	SELECT INTO as * FROM configuracion WHERE nombre=''DBRev-PrecioCoste'';
	IF FOUND THEN
		UPDATE CONFIGURACION SET valor=''0.10.1-0002'' WHERE nombre=''DBRev-PrecioCoste'';
	ELSE
		INSERT INTO configuracion (nombre, valor) VALUES (''DBRev-PrecioCoste'', ''0.10.1-0002'');
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
