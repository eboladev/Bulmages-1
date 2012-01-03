--
-- Modificación de campos y funciones de la BD para la adaptacion para el plugin de Trazabilidad
--
\echo "********* INICIADO FICHERO DE ESTRUCTURA DEL PLUGIN DE TRAZABILIDAD *********"

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
	SELECT INTO rs * FROM pg_attribute  WHERE attname=''lotelalbaranp'';

	IF FOUND THEN
		ALTER TABLE lalbaranp DROP COLUMN lotelalbaranp;
		ALTER TABLE lfacturap DROP COLUMN lotelfacturap;
		ALTER TABLE lalbaran DROP COLUMN lotelalbaran;
		ALTER TABLE lfactura DROP COLUMN lotelfactura;
	END IF;

	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo "Eliminamos los campos para Trazabilidad en las tablas necesarias."

CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
        rec RECORD;

BEGIN
--	DROP TABLE movimiento;
        SELECT INTO rec * FROM pg_attribute WHERE attname = ''idmovimiento'';
        IF FOUND THEN
            DROP TABLE movimiento;
        END IF;
        RETURN 0;
END;
' LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo ":: Eliminamos la tabla de movimientos ... "





\echo -n ":: Funcion que actualiza el total de una factura al borrar una linea de detalle ... "
SELECT drop_if_exists_proc ('trazabilidadalbaranpi','');

\echo -n ":: Funcion que actualiza el total de una factura al borrar una linea de detalle ... "
SELECT drop_if_exists_proc ('trazabilidadalbaranpu','');

\echo -n ":: Funcion que actualiza el total de una factura al borrar una linea de detalle ... "
SELECT drop_if_exists_proc ('trazabilidadalbaranpd','');


-- ==============================================================================

\echo -n ":: Funcion que actualiza el total de una factura al borrar una linea de detalle ... "
SELECT drop_if_exists_proc ('trazabilidadalbarani','');

\echo -n ":: Funcion que actualiza el total de una factura al borrar una linea de detalle ... "
SELECT drop_if_exists_proc ('trazabilidadalbaranu','');

\echo -n ":: Funcion que actualiza el total de una factura al borrar una linea de detalle ... "
SELECT drop_if_exists_proc ('trazabilidadalbarand','');

-- ==============================================================================


-- Agregamos nuevos parametros de configuraci�.
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	rs RECORD;

BEGIN
	SELECT INTO rs * FROM configuracion WHERE nombre=''PluginBf_Trazabilidad'';

	IF FOUND THEN
		DELETE FROM CONFIGURACION WHERE nombre=''PluginBf_Trazabilidad'';
	END IF;

	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT actualizarevision();
DROP FUNCTION actualizarevision() CASCADE;
\echo "Eliminada la revision de la base de datos"


DROP FUNCTION drop_if_exists_table(text) CASCADE;
DROP FUNCTION drop_if_exists_proc(text,text) CASCADE;


COMMIT;
