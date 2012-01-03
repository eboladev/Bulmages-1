--
-- Modificación de campos y funciones de la BD para la adaptacion para el plugin de Inventario
--
\echo "********* INICIADO FICHERO DE ESTRUCTURA DEL PLUGIN DE INVENTARIO *********"

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



-- ========================== CONTROL DE STOCK =======================



CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
	rs RECORD;

BEGIN
	SELECT INTO rs * FROM pg_tables  WHERE tablename=''inventario'';

	IF FOUND THEN
	    DROP TABLE controlstock;
	    DROP TABLE stock_almacen;
	    DROP TABLE inventario;
	END IF;

	SELECT INTO rs * FROM pg_tables WHERE tablename=''minimsalmacen'';

	IF FOUND THEN
	    DROP TABLE minimsalmacen;
	END IF;

	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;

SELECT drop_if_exists_proc('s_narticulo', '');


SELECT drop_if_exists_proc('s_darticulo', '');

SELECT drop_if_exists_proc('s_nalmacen', '');

SELECT drop_if_exists_proc('s_dalmacen', '');

SELECT drop_if_exists_proc('s_disminuyestock', '');

SELECT drop_if_exists_proc('s_aumentastock', '');

SELECT drop_if_exists_proc('s_disminuyestockp', '');

SELECT drop_if_exists_proc('s_aumentastockp', '');

SELECT drop_if_exists_proc('s_modificadostockalmacen', '');

SELECT drop_if_exists_proc('s_disminuyecontrolstock', '');

SELECT drop_if_exists_proc('s_disminuyecontrolstock1', '');

SELECT drop_if_exists_proc('s_disminuyecontrolstock2', '');


SELECT drop_if_exists_proc('s_aumentacontrolstock', '');


-- Cuando cambiamos el almacen en un albaran o albaranp hay problemas con el control de stock
-- que se descuadra.
SELECT drop_if_exists_proc('s_cambiaalbaran', '');

SELECT drop_if_exists_proc('s_cambiaalbaranp', '');


-- ==============================================================================


-- Agregamos nuevos parametros de configuraci�.
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	rs RECORD;

BEGIN
	SELECT INTO rs * FROM configuracion WHERE nombre=''PluginBf_Inventario'';

	IF FOUND THEN
		DELETE FROM CONFIGURACION WHERE nombre=''PluginBf_Inventario'';
	END IF;

	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT actualizarevision();
DROP FUNCTION actualizarevision() CASCADE;
\echo "Actualizada la revision de la base de datos a la version 0.10.1"


DROP FUNCTION drop_if_exists_table(text) CASCADE;
DROP FUNCTION drop_if_exists_proc(text,text) CASCADE;


COMMIT;
