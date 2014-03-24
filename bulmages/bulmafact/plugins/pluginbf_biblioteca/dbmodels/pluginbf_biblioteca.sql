--
-- Modificacion de campos y funciones de la BD para la adaptacion para el plugin de profesores
--
\echo "********* INICIADO FICHERO DE ESTRUCTURA DE BIBLIOTECA *********"

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

-- 
-- Funcion que prepara la base de datos para funcionar como una AMPA
-- 

CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
	rs RECORD;
BEGIN

    SELECT INTO rs * FROM pg_tables  WHERE tablename=''prestamolibro'';
    IF NOT FOUND THEN


	      CREATE TABLE categorialibro
	      (
		idcategorialibro serial NOT NULL,
		nomcategorialibro character varying(255) NOT NULL,
		PRIMARY KEY (idcategorialibro)
	      );

	      CREATE TABLE autor
	      (
		idautor serial NOT NULL,
		nomautor character varying(255) NOT NULL,
		idpais integer,
		fechanacimientoautor date,
		PRIMARY KEY (idautor),
		CONSTRAINT idpais FOREIGN KEY (idpais) REFERENCES pais (idpais)
	      );

	      CREATE TABLE asociado
	      (
		idasociado serial NOT NULL,
		nomasociado character varying(50),
		apellido1asociado character varying(50),
		apellido2asociado character varying(50),
		fechanacimientoasociado date,
		telasociado character varying(20),
		mailasociado character varying(50),
		dirasociado character varying(100),
		idprovincia integer,
		CONSTRAINT idasociadoprimarykey PRIMARY KEY (idasociado),
		CONSTRAINT asociadoprovincia FOREIGN KEY (idprovincia) REFERENCES provincia (idprovincia) ON UPDATE NO ACTION ON DELETE NO ACTION
	      );

	      CREATE TABLE libro
	      (
		idlibro serial NOT NULL,
		isbnlibro character varying(40) NOT NULL,
		titulolibro character varying(200) NOT NULL,
		idautor integer,
		resumenlibro text,
		idcategorialibro integer,
		CONSTRAINT libroidliborprimarykey PRIMARY KEY (idlibro),
		CONSTRAINT idcategorialibro_libro_externkey FOREIGN KEY (idcategorialibro) REFERENCES categorialibro (idcategorialibro) ON UPDATE NO ACTION ON DELETE NO ACTION,
		CONSTRAINT autor_idautor_fkey FOREIGN KEY (idautor) REFERENCES autor (idautor) ON UPDATE NO ACTION ON DELETE NO ACTION
	      );


	      CREATE TABLE prestamolibro
	      (
		idprestamolibro serial,
		fechainprestamolibro date NOT NULL DEFAULT now(),
		fechafinprestamolibro date NOT NULL DEFAULT now() +'' @ 1 week'',
		idasociado integer NOT NULL,
		idlibro integer NOT NULL,
		devueltoprestamolibro boolean DEFAULT FALSE,
		CONSTRAINT prestamolibro_idprestamolibro_p_key PRIMARY KEY (idprestamolibro),
		CONSTRAINT asociado_idasociado_f_key FOREIGN KEY (idasociado) REFERENCES asociado (idasociado) ON UPDATE NO ACTION ON DELETE NO ACTION,
		CONSTRAINT libro_idlibro_f_key FOREIGN KEY (idlibro) REFERENCES libro (idlibro) ON UPDATE NO ACTION ON DELETE NO ACTION
	      );

END IF;
RETURN 0;



END;
'   LANGUAGE plpgsql;

SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo "Creamos las tablas para la gestion de bibliotecas."




-- ======================== Actualizada la revision de la base de datos a la version. =====================


-- Agregamos nuevos parametros de configuracion.
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	rs RECORD;

BEGIN
	SELECT INTO rs * FROM configuracion WHERE nombre=''PluginBf_Biblioteca'';

	IF FOUND THEN
		UPDATE CONFIGURACION SET valor=''1.6.0-0001'' WHERE nombre=''PluginBf_Biblioteca'';
	ELSE
		INSERT INTO configuracion (nombre, valor) VALUES (''PluginBf_Biblioteca'', ''1.6.0-0001'');
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

