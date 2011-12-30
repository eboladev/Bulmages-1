--
-- Modificacion de campos y funciones de la BD para la adaptacion para el plugin de profesores
--
\echo "********* INICIADO FICHERO DE ESTRUCTURA DEL PLUGIN DE VARIACION TARIFA POR CANTIDAD *********"

\echo ":: Establecemos los mensajes minimos a avisos y otros parametros ... "
\echo -n ":: "
SET client_min_messages TO WARNING;
SET log_min_messages TO WARNING;
-- SET log_error_verbosity TO TERSE;
BEGIN;

--
-- Funcion auxiliar para borrar funciones limpiamente
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

DROP FUNCTION IF EXISTS actualizacantrecibo_insert() CASCADE;
DROP FUNCTION IF EXISTS actualizacantrecibo_update() CASCADE;
DROP FUNCTION IF EXISTS actualizacantrecibo_delete() CASCADE;

-- ========================== VARIACION DE TARIFA =======================

CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
	rsa  RECORD;
BEGIN

    SELECT INTO rsa * FROM pg_tables  WHERE tablename=''zcomercial'';
    IF FOUND THEN
	ALTER TABLE cliente DROP COLUMN idzcomercial;

	DROP TABLE zcomercial;

	DROP TABLE prestamo;

	DROP TABLE libro;

	DROP TABLE socio;

	DROP TABLE autor;

	DROP TABLE categorialibro;
    END IF;


    RETURN 0;
    
END;
'   LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo "Creamos las tablas para la gestion de biblioteca"

-- ======================== Actualizada la revision de la base de datos a la version. =====================


-- Agregamos nuevos parametros de configuracion.
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	rsa  RECORD;
BEGIN
	SELECT INTO rsa * FROM configuracion WHERE nombre=''PluginBf_Biblioteca'';
	IF FOUND THEN
		DELETE FROM CONFIGURACION WHERE nombre=''PluginBf_Biblioteca'';
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