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



-- ========================== VARIACION DE TARIFA =======================


CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
	as RECORD;
BEGIN

        SELECT INTO as * FROM pg_tables  WHERE tablename=''profesor'';
        IF NOT FOUND THEN

        CREATE TABLE profesor (
            idprofesor SERIAL PRIMARY KEY,
	    nombreprofesor VARCHAR NOT NULL
        );

        END IF;


        SELECT INTO as * FROM pg_tables  WHERE tablename=''alumno'';
        IF NOT FOUND THEN

        CREATE TABLE alumno (
	    idalumno SERIAL PRIMARY KEY,
	    nombrealumno VARCHAR NOT NULL
        );

        END IF;

        SELECT INTO as * FROM pg_tables  WHERE tablename=''tutor'';
        IF NOT FOUND THEN

        CREATE TABLE tutor (
	    idtutor SERIAL PRIMARY KEY,
	    nombretutor VARCHAR NOT NULL
        );

        END IF;

	DROP TABLE sesionactividad;
	DROP TABLE actividad;
	
        SELECT INTO as * FROM pg_tables  WHERE tablename=''actividad'';
        IF NOT FOUND THEN

        CREATE TABLE actividad (
	    idactividad SERIAL PRIMARY KEY,
	    idprofesor  INTEGER NOT NULL REFERENCES profesor(idprofesor),
	    nombreactividad VARCHAR NOT NULL
        );

        END IF;


        SELECT INTO as * FROM pg_tables  WHERE tablename=''sesionactividad'';
        IF NOT FOUND THEN

        CREATE TABLE sesionactividad (
	    idsesionactividad SERIAL PRIMARY KEY,
	    idactividad INTEGER NOT NULL REFERENCES actividad(idactividad),
	    idprofesor INTEGER NOT NULL REFERENCES profesor(idprofesor),
	    fechainsesion timestamp DEFAULT now() NOT NULL,
	    fechafinsesion timestamp DEFAULT now() NOT NULL
        );

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
	as RECORD;
BEGIN
	SELECT INTO as * FROM configuracion WHERE nombre=''DBRev-Profesor'';
	IF FOUND THEN
		UPDATE CONFIGURACION SET valor=''0.11.1-0001'' WHERE nombre=''DBRev-Profesor'';
	ELSE
		INSERT INTO configuracion (nombre, valor) VALUES (''DBRev-Profesor'', ''0.11.1-0001'');
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
