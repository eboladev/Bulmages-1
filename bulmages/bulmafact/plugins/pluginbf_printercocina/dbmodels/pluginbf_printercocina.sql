--
-- Modificacion de campos y funciones de la BD para la adaptacion para el plugin de profesores
--
\echo "********* INICIADO FICHERO DE ESTRUCTURA DE IMPRESORAS DE COCINA *********"

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
    SELECT INTO rs * FROM pg_tables  WHERE tablename=''printercocina'';

    IF NOT FOUND THEN
        CREATE TABLE printercocina (
            idprintercocina SERIAL PRIMARY KEY,
            nombreprintercocina VARCHAR NOT NULL,
            colaprintercocina VARCHAR NOT NULL
        );

    ALTER TABLE articulo ADD COLUMN idprintercocina INTEGER REFERENCES printercocina (idprintercocina);

    END IF;

    RETURN 0;
END;
'   LANGUAGE plpgsql;

SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo "Creamos la tabla printercocina"



-- ======================== Actualizada la revision de la base de datos a la version. =====================


-- Agregamos nuevos parametros de configuracion.
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	rs RECORD;

BEGIN
	SELECT INTO rs * FROM configuracion WHERE nombre=''PluginBf_PrinterCocina'';

	IF FOUND THEN
		UPDATE CONFIGURACION SET valor=''0.12.1-0001'' WHERE nombre=''PluginBf_PrinterCocina'';
	ELSE
		INSERT INTO configuracion (nombre, valor) VALUES (''PluginBf_PrinterCocina'', ''0.12.1-0001'');
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
