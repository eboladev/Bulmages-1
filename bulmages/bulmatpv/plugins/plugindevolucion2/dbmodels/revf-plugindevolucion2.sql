--
-- Modificación de campos y funciones de la BD para la adaptacion para el plugin de Inventario
--
\echo "********* INICIADO FICHERO DE ESTRUCTURA DEL PLUGIN DE TIPOS DE DEVOLUCION *********"

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



-- ========================== CONTROL DE STOCK =======================


CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
	as RECORD;
BEGIN

	SELECT INTO as * FROM pg_tables  WHERE tablename=''devolucion'';
	IF NOT FOUND THEN
		CREATE TABLE devolucion
		(
		iddevolucion serial PRIMARY KEY,
		idalbaran_origen integer NOT NULL REFERENCES albaran(idalbaran),
		idalbaran_devolucion integer NOT NULL REFERENCES albaran(idalbaran)
		);

		
		CREATE TABLE vale
		(
		idvale serial PRIMARY KEY,
		refvale character varying(12) NOT NULL,
		fechavale date DEFAULT now(),
		impvale numeric(12,2) DEFAULT 0,
		descvale character varying(150),
		idtrabajador integer REFERENCES trabajador (idtrabajador),
		anuladovale boolean,
		iddevolucion integer REFERENCES devolucion (iddevolucion),
		idalbaran_canjeo integer REFERENCES albaran (idalbaran)
		)


	END IF;

	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo "Creo las tablas de devolucion y vale"




-- Function: restriccionesvale()

-- DROP FUNCTION restriccionesvale();
SELECT drop_if_exists_proc('restriccionesvale', '');
\echo -n ':: Funcion que crea restricciones en los vales ... '
CREATE OR REPLACE FUNCTION restriccionesvale()
RETURNS "trigger" AS
$BODY$DECLARE
asd RECORD;

BEGIN
IF NEW.fechavale IS NULL THEN
	NEW.fechavale := now();
END IF;
IF NEW.refavale IS NULL OR NEW.refvale = '' THEN
	SELECT INTO asd crearef() AS m;
	IF FOUND THEN
	NEW.refvale := asd.m;
	END IF;
END IF;
RETURN NEW;
END;$BODY$
LANGUAGE 'plpgsql' VOLATILE;
ALTER FUNCTION restriccionesvale() OWNER TO postgres;

-- Function: crearef()

-- DROP FUNCTION crearef();
SELECT drop_if_exists_proc('crearef', '');
\echo -n ':: Funcion que crea restricciones en vales ... '
CREATE OR REPLACE FUNCTION crearef()
RETURNS character varying AS
$BODY$
DECLARE
asd RECORD;
result character varying(15);
efound boolean;

BEGIN
efound := FALSE;
WHILE efound = FALSE LOOP
	result := random_string(6);
	efound := TRUE;
	SELECT INTO asd idpresupuesto FROM presupuesto WHERE refpresupuesto = result;
	IF FOUND THEN
	efound := FALSE;
	END IF;
	SELECT INTO asd idpedidocliente FROM pedidocliente WHERE
refpedidocliente = result;
	IF FOUND THEN
	efound := FALSE;
	END IF;
	SELECT INTO asd idalbaran FROM albaran WHERE refalbaran = result;
	IF FOUND THEN
	efound := FALSE;
	END IF; 
	SELECT INTO asd idfactura FROM factura WHERE reffactura = result;
	IF FOUND THEN
		efound := FALSE;
	END IF;
	SELECT INTO asd idcobro FROM cobro WHERE refcobro = result;
	IF FOUND THEN
		efound := FALSE;
	END IF;
	SELECT INTO asd idpedidoproveedor FROM pedidoproveedor WHERE
refpedidoproveedor = result;
	IF FOUND THEN
		efound := FALSE;
	END IF;
	SELECT INTO asd idalbaranp FROM albaranp WHERE refalbaranp = result;
	IF FOUND THEN
		efound := FALSE;
	END IF;
	SELECT INTO asd idfacturap FROM facturap WHERE reffacturap = result;
	IF FOUND THEN
		efound := FALSE;
	END IF;
	SELECT INTO asd idpago FROM pago WHERE refpago = result;
	IF FOUND THEN
		efound := FALSE;
	END IF;
	SELECT INTO asd idvale FROM vale WHERE refvale = result;
	IF FOUND THEN
		efound := FALSE;
	END IF;
END LOOP;
RETURN result;
END;
$BODY$
LANGUAGE 'plpgsql' VOLATILE;
ALTER FUNCTION crearef() OWNER TO postgres;


CREATE TRIGGER restriccionesvaletrigger
BEFORE INSERT OR UPDATE
ON vale
FOR EACH ROW
EXECUTE PROCEDURE restriccionesvale();

-- Agregamos nuevos parametros de configuraci�.
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	as RECORD;
BEGIN
	SELECT INTO as * FROM configuracion WHERE nombre=''DBRev-Devolucion2'';
	IF FOUND THEN
		UPDATE CONFIGURACION SET valor=''0.11.1-0001'' WHERE nombre=''DBRev-Devolucion2'';
	ELSE
		INSERT INTO configuracion (nombre, valor) VALUES (''DBRev-Devolucion2'', ''0.11.1-0001'');
	END IF;
	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT actualizarevision();
DROP FUNCTION actualizarevision() CASCADE;
\echo "Actualizada la revision de la base de datos a la version"


DROP FUNCTION drop_if_exists_table(text) CASCADE;
DROP FUNCTION drop_if_exists_proc(text,text) CASCADE;


COMMIT;
