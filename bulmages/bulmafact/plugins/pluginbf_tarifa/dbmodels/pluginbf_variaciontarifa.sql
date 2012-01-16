--
-- Modificacion de campos y funciones de la BD para la adaptacion para el plugin de
-- VARIACION TARIFA POR CANTIDAD
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
	rs RECORD;

BEGIN
        SELECT INTO rs * FROM pg_tables  WHERE tablename=''variaciontarifa'';

        IF NOT FOUND THEN
                CREATE TABLE variaciontarifa (
                idvariaciontarifa SERIAL PRIMARY KEY,
		idarticulo integer NOT NULL REFERENCES articulo(idarticulo),
		idtarifa integer NOT NULL REFERENCES tarifa(idtarifa),
		idalmacen integer NOT NULL REFERENCES almacen(idalmacen),
		cantidadmayoroigualque numeric(12,2),
		porcentajevariacion numeric(12,2),
		UNIQUE(idtarifa, idalmacen, cantidadmayoroigualque)
                );

        END IF;

	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo "Creamos la tabla variaciontarifa."

-- ======================== Actualizada la revision de la base de datos a la version. =====================


-- Agregamos nuevos parametros de configuracion.
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	rs RECORD;

BEGIN
	SELECT INTO rs * FROM configuracion WHERE nombre=''PluginBf_VariacionTarifa'';

	IF FOUND THEN
		UPDATE configuracion SET valor=''0.11.1-0001'' WHERE nombre=''PluginBf_VariacionTarifa'';
	ELSE
		INSERT INTO configuracion (nombre, valor) VALUES (''PluginBf_VariacionTarifa'', ''0.11.1-0001'');
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
