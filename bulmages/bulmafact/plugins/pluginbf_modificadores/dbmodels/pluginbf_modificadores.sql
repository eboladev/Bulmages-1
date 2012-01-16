--
-- Modificacion de campos y funciones de la BD para la adaptacion para el plugin de
-- MODIFICADORES
--
\echo "********* INICIADO FICHERO DE ESTRUCTURA DEL PLUGIN DE MODIFICADORES *********"
\echo ":: Establecemos los mensajes minimos a avisos y otros parametros ... "
\echo -n ":: "
SET client_min_messages TO WARNING;
SET log_min_messages TO WARNING;
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
	rs RECORD;

BEGIN
        SELECT INTO rs * FROM pg_tables  WHERE tablename=''modificador'';

        IF NOT FOUND THEN
                CREATE TABLE modificador (
                idmodificador SERIAL PRIMARY KEY,
		varpreciomodificador numeric(12,2) DEFAULT 0,
		nombremodificador VARCHAR NOT NULL,
		idarticulo INTEGER NOT NULL REFERENCES articulo (idarticulo)
                );
        END IF;

	SELECT INTO rs attname, relname FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid=pg_class.oid WHERE attname=''textolibremodificador'' AND relname=''lalbaran'';

	IF NOT FOUND THEN
	    ALTER TABLE lalbaran ADD COLUMN textolibremodificador VARCHAR;
	    ALTER TABLE lalbaran ADD COLUMN imgmodificador VARCHAR;
	    ALTER TABLE lalbaran ADD COLUMN idmodificador1 INTEGER DEFAULT NULL REFERENCES modificador(idmodificador);
	    ALTER TABLE lalbaran ADD COLUMN idmodificador2 INTEGER DEFAULT NULL REFERENCES modificador(idmodificador);
	    ALTER TABLE lalbaran ADD COLUMN idmodificador3 INTEGER DEFAULT NULL REFERENCES modificador(idmodificador);
	    ALTER TABLE lalbaran ADD COLUMN idmodificador4 INTEGER DEFAULT NULL REFERENCES modificador(idmodificador);
	    ALTER TABLE lalbaran ADD COLUMN idmodificador5 INTEGER DEFAULT NULL REFERENCES modificador(idmodificador);
	    ALTER TABLE lalbaran ADD COLUMN idmodificador6 INTEGER DEFAULT NULL REFERENCES modificador(idmodificador);
	    ALTER TABLE lalbaran ADD COLUMN idmodificador7 INTEGER DEFAULT NULL REFERENCES modificador(idmodificador);
	    ALTER TABLE lalbaran ADD COLUMN idmodificador8 INTEGER DEFAULT NULL REFERENCES modificador(idmodificador);
	    ALTER TABLE lalbaran ADD COLUMN idmodificador9 INTEGER DEFAULT NULL REFERENCES modificador(idmodificador);
	END IF;

	SELECT INTO rs attname, relname FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid=pg_class.oid WHERE attname=''imglalbaran'' AND relname=''lalbaran'';

        IF NOT FOUND THEN
	    ALTER TABLE lalbaran ADD COLUMN imglalbaran VARCHAR;
        END IF;

	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo "Creamos la tabla modificador."

-- ======================== Actualizada la revision de la base de datos a la version. =====================


-- Agregamos nuevos parametros de configuracion.
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	rs RECORD;

BEGIN
	SELECT INTO rs * FROM configuracion WHERE nombre=''PluginBf_Modificadores'';

	IF FOUND THEN
		UPDATE CONFIGURACION SET valor=''0.12.1-0002'' WHERE nombre=''PluginBf_Modificadores'';
	ELSE
		INSERT INTO configuracion (nombre, valor) VALUES (''PluginBf_Modificadores'', ''0.12.1-0002'');
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
