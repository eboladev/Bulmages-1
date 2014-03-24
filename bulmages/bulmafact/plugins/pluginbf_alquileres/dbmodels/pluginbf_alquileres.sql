--
-- Modificacion de campos y funciones de la BD para la adaptacion para el plugin de profesores
--
\echo "********* INICIADO FICHERO DE ESTRUCTURA DE ALQUILERES *********"

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

    SELECT INTO rs * FROM pg_tables  WHERE tablename=''vivienda'';
    IF NOT FOUND THEN

	CREATE TABLE vivienda
	(
	  idvivienda serial NOT NULL,
	  nomvivienda character varying(255) NOT NULL,
	  dirvivienda character varying(255) NOT NULL,
	  gesavivienda character varying(255) NOT NULL,
	  aguavivienda character varying(255) NOT NULL,
	  alquilervivienda integer,
	  metrosvivienda integer,
	  PRIMARY KEY (idvivienda)
	);

	CREATE TABLE ingvivienda
	(
	  idingvivienda serial NOT NULL,
	  fechaingvivienda date,
	  descingvivienda character varying,
	  cantingvivienda numeric(12,2),
	  idvivienda integer,
	  CONSTRAINT ingvivienda_idingvivienda_pkey PRIMARY KEY (idingvivienda ),
	  CONSTRAINT ingvivienda_idvivienda_fkey FOREIGN KEY (idvivienda)
	      REFERENCES vivienda (idvivienda) MATCH SIMPLE
	      ON UPDATE NO ACTION ON DELETE NO ACTION
	);

	CREATE TABLE gasvivienda
	(
	  idgasvivienda serial NOT NULL,
	  idvivienda integer,
	  descgasvivienda character varying,
	  fechagasvivienda date,
	  cantgasvivienda numeric(12,2),
	  CONSTRAINT gasvivienda_idgasvivienda_pkey PRIMARY KEY (idgasvivienda ),
	  CONSTRAINT gasvivienda_idvivienda_fkey FOREIGN KEY (idvivienda)
	      REFERENCES vivienda (idvivienda) MATCH SIMPLE
	      ON UPDATE NO ACTION ON DELETE NO ACTION
	);

	ALTER TABLE contrato ADD COLUMN idvivienda INTEGER;
	ALTER TABLE contrato ADD CONSTRAINT idviviendafk FOREIGN KEY (idvivienda) REFERENCES vivienda(idvivienda);
	ALTER TABLE contrato ADD COLUMN actividadcontrato VARCHAR;
	ALTER TABLE contrato ADD COLUMN representantecontrato VARCHAR;
	ALTER TABLE contrato ADD COLUMN cifrepresentantecontrato VARCHAR;
	ALTER TABLE contrato ADD COLUMN dirrepresentantecontrato VARCHAR;
	ALTER TABLE contrato ADD COLUMN representanteempcontrato VARCHAR;
	ALTER TABLE contrato ADD COLUMN cifrepresentanteempcontrato VARCHAR;
	ALTER TABLE contrato ADD COLUMN dirrepresentanteempcontrato VARCHAR;
	ALTER TABLE contrato ADD COLUMN textocantidadcontrato VARCHAR;
	ALTER TABLE contrato ADD COLUMN fianzacontrato VARCHAR;
    END IF;


    SELECT INTO rs * FROM pg_attribute WHERE attname = ''bancocontrato'';
    IF NOT FOUND THEN
	ALTER TABLE contrato ADD COLUMN bancocontrato VARCHAR;
    END IF;

    SELECT INTO rs * FROM pg_attribute WHERE attname = ''numpropvivienda'';
    IF NOT FOUND THEN
	ALTER TABLE vivienda ADD COLUMN numpropvivienda VARCHAR;
	ALTER TABLE vivienda ADD COLUMN depropvivienda VARCHAR;
	ALTER TABLE vivienda ADD COLUMN tomopropvivienda VARCHAR;
	ALTER TABLE vivienda ADD COLUMN libropropvivienda VARCHAR;
	ALTER TABLE vivienda ADD COLUMN foliopropvivienda VARCHAR;
	ALTER TABLE vivienda ADD COLUMN fincapropvivienda VARCHAR;
    END IF;

RETURN 0;



END;
'   LANGUAGE plpgsql;

SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo "Creamos las tablas para la gestion de viviendas."




-- ======================== Actualizada la revision de la base de datos a la version. =====================


-- Agregamos nuevos parametros de configuracion.
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	rs RECORD;

BEGIN
	SELECT INTO rs * FROM configuracion WHERE nombre=''PluginBf_Alquileres'';

	IF FOUND THEN
		UPDATE CONFIGURACION SET valor=''0.15.0-0001'' WHERE nombre=''PluginBf_Alquileres'';
	ELSE
		INSERT INTO configuracion (nombre, valor) VALUES (''PluginBf_Alquileres'', ''0.15.0-0001'');
	END IF;

	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT actualizarevision();
DROP FUNCTION actualizarevision() CASCADE;
\echo "Actualizada la revision de la base de datos a la version 0.15.0-0001"

DROP FUNCTION drop_if_exists_table(text) CASCADE;
DROP FUNCTION drop_if_exists_proc(text,text) CASCADE;

COMMIT;

