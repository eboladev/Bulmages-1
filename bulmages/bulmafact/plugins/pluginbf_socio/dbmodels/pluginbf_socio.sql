--
-- Modificacion de campos y funciones de la BD para la adaptacion para el plugin de socios
--
\echo "********* INICIADO FICHERO DE ESTRUCTURA DE SOCIOS *********"

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
-- Funcion que prepara la base de datos para trabajar con socios
-- 

CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
    rs RECORD;

BEGIN
    SELECT INTO rs * FROM pg_tables  WHERE tablename=''tiposocio'';

    IF NOT FOUND THEN
        CREATE TABLE tiposocio
	(
	  idtiposocio serial NOT NULL,
	  nombretiposocio character varying NOT NULL,
	  cuotatiposocio numeric(12,2) DEFAULT 0,
	  comentariotiposocio character varying,
	  CONSTRAINT tiposocio_pkey PRIMARY KEY (idtiposocio)
	);

    END IF;

    SELECT INTO rs attname, relname 
    	FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid=pg_class.oid 
    	WHERE attname=''fnacimientocliente'' AND relname=''cliente'';
    IF NOT FOUND THEN
	ALTER TABLE cliente ADD COLUMN fnacimientocliente date;
    END IF;

    SELECT INTO rs attname, relname 
	FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid=pg_class.oid 
	WHERE attname=''profesioncliente'' AND relname=''cliente'';
    IF NOT FOUND THEN
	ALTER TABLE cliente ADD COLUMN profesioncliente character varying(100);
    END IF;

    SELECT INTO rs attname, relname 
	FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid=pg_class.oid 
	WHERE attname=''idtiposocio'' AND relname=''cliente'';
    IF NOT FOUND THEN
	ALTER TABLE cliente ADD COLUMN idtiposocio integer;
	ALTER TABLE cliente
	  ADD CONSTRAINT cliente_idtiposocio_fkey FOREIGN KEY (idtiposocio)
	      REFERENCES tiposocio (idtiposocio) MATCH SIMPLE
	      ON UPDATE NO ACTION ON DELETE NO ACTION;
    END IF;

    SELECT INTO rs attname, relname 
	FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid=pg_class.oid 
	WHERE attname=''cuotafamiliarcliente'' AND relname=''cliente'';
    IF NOT FOUND THEN
	ALTER TABLE cliente ADD COLUMN cuotafamiliarcliente boolean;
    END IF;

    SELECT INTO rs attname, relname 
	FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid=pg_class.oid 
	WHERE attname=''nombretitularbancocliente'' AND relname=''cliente'';
    IF NOT FOUND THEN
	ALTER TABLE cliente ADD COLUMN nombretitularbancocliente character varying(100);
    END IF;

    SELECT INTO rs attname, relname 
	FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid=pg_class.oid 
	WHERE attname=''apellidotitularbancocliente'' AND relname=''cliente'';
    IF NOT FOUND THEN
	ALTER TABLE cliente ADD COLUMN apellidotitularbancocliente character varying(100);
    END IF;

    SELECT INTO rs attname, relname 
	FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid=pg_class.oid 
	WHERE attname=''entidadbancocliente'' AND relname=''cliente'';
    IF NOT FOUND THEN
	ALTER TABLE cliente ADD COLUMN entidadbancocliente character varying(100);
    END IF;


    RETURN 0;
    
END;
'   LANGUAGE plpgsql;

SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo "Creamos las tablas de socios."


-- ======================== Actualizada la revision de la base de datos a la version. =====================


-- Agregamos nuevos parametros de configuracion.
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	rs RECORD;

BEGIN
	SELECT INTO rs * FROM configuracion WHERE nombre=''PluginBf_Socio'';

	IF FOUND THEN
		UPDATE CONFIGURACION SET valor=''0.11.1-0001'' WHERE nombre=''PluginBf_Socio'';
	ELSE
		INSERT INTO configuracion (nombre, valor) VALUES (''PluginBf_Socio'', ''0.11.1-0001'');
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
