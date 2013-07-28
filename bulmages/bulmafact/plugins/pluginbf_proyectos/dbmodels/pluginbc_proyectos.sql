--
-- Modificación de campos y funciones de la BD para la adaptacion para el plugin de Trazabilidad
--
\echo "********* INICIADO FICHERO DE ESTRUCTURA DEL PLUGIN DE PRESUPUESTOS CONTABLES *********"

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
	rs RECORD;

BEGIN
	SELECT INTO rs * FROM pg_attribute  WHERE attname=''idpresupuestoc'';
	IF NOT FOUND THEN
		CREATE TABLE presupuestoc (
			idpresupuestoc serial PRIMARY KEY,
			idc_coste integer NOT NULL REFERENCES c_coste(idc_coste),
			fechapresupuestoc date NOT NULL DEFAULT now(),
			nompresupuestoc   varchar,
			comentpresupuestoc  varchar,
			archpresupuestoc  varchar
		);

		CREATE TABLE lingpresupuestoc (
			idlingpresupuestoc serial PRIMARY KEY,
			idpresupuestoc integer NOT NULL REFERENCES presupuestoc(idpresupuestoc),
			ordenlingpresupuestoc integer,
			idcuenta integer NOT NULL REFERENCES cuenta(idcuenta),
			cantlingpresupuestoc NUMERIC(12,2),
			precioundlingpresupuestoc NUMERIC(12,2),
			ejerciciolingpresupuestoc integer,
			comentlingpresupuestoc varchar,
			desclingpresupuestoc   varchar
		);

		CREATE TABLE lgaspresupuestoc (
			idlgaspresupuestoc serial PRIMARY KEY,
			idpresupuestoc integer NOT NULL REFERENCES presupuestoc(idpresupuestoc),
			ordenlgaspresupuestoc integer,
			idcuenta integer NOT NULL REFERENCES cuenta(idcuenta),
			cantlgaspresupuestoc NUMERIC(12,2),
			precioundlgaspresupuestoc NUMERIC(12,2),
			ejerciciolgaspresupuestoc integer,
			comentlgaspresupuestoc varchar,
			desclgaspresupuestoc   varchar
		);
	END IF;

	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo "Agregamos las tablas necesarias para presupuestos contables"




-- ==============================================================================


-- Agregamos nuevos parametros de configuraci�.
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	rs RECORD;

BEGIN
	SELECT INTO rs * FROM configuracion WHERE nombre=''PluginBc_Proyectos'';

	IF FOUND THEN
		UPDATE CONFIGURACION SET valor=''0.10.1-0001'' WHERE nombre=''PluginBc_Proyectos'';
	ELSE
		INSERT INTO configuracion (nombre, valor) VALUES (''PluginBc_Proyectos'', ''0.10.1-0001'');
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
