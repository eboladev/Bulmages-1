--
-- Modificacion de campos y funciones de la BD para la adaptacion para tallas y colores.
--
\echo "********* INICIADO FICHERO DE ESTRUCTURA DEL PLUGIN DE TALLAS Y COLORES *********"

\echo ":: Establecemos los mensajes minimos a avisos y otros parametros ... "
\echo -n ":: "
SET client_min_messages TO WARNING;
SET log_min_messages TO WARNING;
-- SET log_error_verbosity TO TERSE;
BEGIN;

--
-- Estas primeras funciones cambiaran los tipos de columnas que estan como flotantes a NUMERIC.
-- Se trata de un parche que se desea aplicar para almacenar los tipos monetarios
-- ya que actualmente se encuantran almacenados como 'doubles' y es preferible
-- que se almacenen como tipo 'numeric'.
-- Todas devuelven como valor numerico el nmero de filas influenciadas por el cambio
-- NOTA: Si alguien sabe como pasar por parametro un nombre de tabla y campo a modificar se
-- hara mucho mas sencillito porque solo habra que implementar un funcion ya que siempre
-- hay que hacer lo mismo.
--

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

-- ======================== COMPROBACION DE CUAL ES LA ULTIMA VERSION ==================================
-- DESCOMENTAR EN VERSION 1.0


--CREATE OR REPLACE FUNCTION compruebarevision() RETURNS INTEGER AS '
--DECLARE
--	rs RECORD;
--BEGIN
--	SELECT INTO rs * FROM configuracion WHERE nombre=''DBRev-Tallas-y-Colores'' AND ( valor LIKE ''0.9.1%'' OR valor = ''0.9.1-0001'');
--	IF FOUND THEN
--		RETURN 0;
--	ELSE
--		RETURN -1;
--	END IF;
--END;
--'   LANGUAGE plpgsql;
--SELECT compruebarevision();
--DROP FUNCTION compruebarevision() CASCADE;
--\echo "Comprobada la revision"

-- ========================  FIN DE LA COMPROBACION ============================


CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
        rs RECORD;

BEGIN
        SELECT INTO rs * FROM pg_attribute WHERE attname = ''idtc_talla'';
        IF FOUND THEN
        
                DROP TABLE tc_articulo_talla;
                DROP TABLE tc_articulo_color;
                DROP TABLE tc_articulo_alias;
                DROP TABLE tc_stock_tallacolor;
                DROP TABLE tc_stock_almacen;
                DROP TABLE tc_color;
                DROP TABLE tc_talla;
        END IF;

	SELECT INTO rs attname, relname FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid = pg_class.oid WHERE attname = ''idtc_talla'' AND relname = ''lalbaran'';
	IF FOUND THEN
		-- Alteramos la tabla de presupuestos.
		ALTER TABLE lpresupuesto DROP COLUMN idtc_talla;
		ALTER TABLE lpresupuesto DROP COLUMN idtc_color;
		-- Agregamos cambios a los pedidos.
		ALTER TABLE lpedidocliente DROP COLUMN idtc_talla;
		ALTER TABLE lpedidocliente DROP COLUMN idtc_color;
		-- Agregamos cambios a los pedidos de proveedor.
		ALTER TABLE lpedidoproveedor DROP COLUMN idtc_talla;
		ALTER TABLE lpedidoproveedor DROP COLUMN idtc_color;
		-- Agregamos campos a los albaranes.
		ALTER TABLE lalbaran DROP COLUMN idtc_talla;
		ALTER TABLE lalbaran DROP COLUMN idtc_color;
		-- Agregamos campos a los albaranes de proveedor.
		ALTER TABLE lalbaranp DROP COLUMN idtc_talla;
		ALTER TABLE lalbaranp DROP COLUMN idtc_color;
		-- Agregamos cambios a las factura.
		ALTER TABLE lfactura DROP COLUMN idtc_talla;
		ALTER TABLE lfactura DROP COLUMN idtc_color;
		-- Agregamos cambios a las facturap.
		ALTER TABLE lfacturap DROP COLUMN idtc_talla;
		ALTER TABLE lfacturap DROP COLUMN idtc_color;
	END IF;

        RETURN 0;
END;
' LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo ":: Eliminamos las tablas de tallas, colores y stock... "



-- ==============================================================================


-- Agregamos nuevos parametros de configuracion.
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	rs RECORD;
BEGIN
	SELECT INTO rs * FROM configuracion WHERE nombre=''PluginBf_TallasColores'';

	IF FOUND THEN
		DELETE FROM CONFIGURACION WHERE nombre=''PluginBf_TallasColores'';
	END IF;

	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT actualizarevision();
DROP FUNCTION actualizarevision() CASCADE;
\echo "Eliminada la revision de la base de datos a la version 0.11.1"


DROP FUNCTION drop_if_exists_table(text) CASCADE;
DROP FUNCTION drop_if_exists_proc(text,text) CASCADE;


COMMIT;

