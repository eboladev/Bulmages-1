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
	rs  RECORD;
BEGIN

    SELECT INTO rs * FROM pg_tables  WHERE tablename=''alumnotutor'';
    IF FOUND THEN
        DROP TABLE alumnotutor CASCADE;
    END IF;

    SELECT INTO rs * FROM pg_tables  WHERE tablename=''alumno'';
    IF FOUND THEN
        DROP TABLE alumno CASCADE;
    END IF;

    SELECT INTO rs * FROM pg_tables WHERE tablename=''cuota'';
    IF FOUND THEN
        DROP TABLE cuota CASCADE;
    END IF;
    
    SELECT INTO rs * FROM pg_tables WHERE tablename = ''socio'';
    IF FOUND THEN
        DROP TABLE socio CASCADE;
    END IF;

    SELECT INTO rs * FROM pg_tables  WHERE tablename=''tutor'';
    IF FOUND THEN
        DROP TABLE tutor CASCADE;
    END IF;
    
    SELECT INTO rs * FROM pg_tables  WHERE tablename=''faltaasistenciaalumnoactividad'';
    IF FOUND THEN
        DROP TABLE faltaasistenciaalumnoactividad CASCADE;
    END IF;
        
    SELECT INTO rs * FROM pg_tables  WHERE tablename=''sesionactividad'';
    IF FOUND THEN
        DROP TABLE sesionactividad CASCADE;
    END IF;
        
    SELECT INTO rs * FROM pg_tables  WHERE tablename=''actividad'';
    IF FOUND THEN
        DROP TABLE actividad CASCADE;
    END IF;

    SELECT INTO rs * FROM pg_tables  WHERE tablename=''profesor'';
    IF FOUND THEN
        DROP TABLE profesor CASCADE;
    END IF;

-- Quitamos restricciones para la tabla de clientes
-- y agregamos unrs  menos restrictivas
   SELECT INTO rs * FROM pg_constraint WHERE conname =''cliente_codcliente_key'';
   IF NOT FOUND THEN
      ALTER TABLE cliente ADD CONSTRAINT cliente_codcliente_key UNIQUE (codcliente);
    END IF;

   SELECT INTO rs * FROM pg_constraint WHERE conname =''cliente_nomcliente_key'';
   IF NOT FOUND THEN
      ALTER TABLE cliente ADD constraint cliente_nomcliente_key UNIQUE (nomcliente);
    END IF;

   SELECT INTO rs * FROM pg_constraint WHERE conname =''cliente_cifcliente_key'';
   IF NOT FOUND THEN
      ALTER TABLE cliente ADD CONSTRAINT cliente_cifcliente_key UNIQUE (cifcliente);
    END IF;

   SELECT INTO rs * FROM pg_constraint WHERE conname =''cliente_fapac_key'';
   IF FOUND THEN
      ALTER TABLE cliente DROP CONSTRAINT cliente_fapac_key;
   END IF;


    RETURN 0;
    
END;
'   LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo "Creamos la tabla profesor."

-- ======================== Actualizada la revision de la base de datos a la version. =====================


-- Agregamos nuevos parametros de configuracion.
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	rs RECORD;

BEGIN
	SELECT INTO rs * FROM configuracion WHERE nombre=''PluginBf_Profesor'';

	IF FOUND THEN
		DELETE FROM CONFIGURACION WHERE nombre=''PluginBf_Profesor'';
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
