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
        IF NOT FOUND THEN
                CREATE TABLE tc_talla (
                        idtc_talla SERIAL PRIMARY KEY,
                        nomtc_talla CHARACTER VARYING(50)
                );
                CREATE TABLE tc_color (
                        idtc_color SERIAL PRIMARY KEY,
                        nomtc_color CHARACTER VARYING(50),
			rgbtc_color CHARACTER VARYING(13)
                );
                CREATE TABLE tc_articulo_talla (
                        idtc_talla INTEGER NOT NULL,
                        idarticulo INTEGER NOT NULL,
			incpreciotc_articulo_talla NUMERIC(12, 2),
			PRIMARY KEY (idtc_talla, idarticulo)
                );
                CREATE TABLE tc_articulo_color (
                        idtc_color INTEGER NOT NULL,
                        idarticulo INTEGER NOT NULL,
			incpreciotc_articulo_color NUMERIC(12, 2),
			PRIMARY KEY (idtc_color, idarticulo)
                );
		CREATE TABLE tc_articulo_alias (
			idarticulo INTEGER NOT NULL REFERENCES articulo(idarticulo),
			idtc_talla INTEGER REFERENCES tc_talla(idtc_talla),
			idtc_color INTEGER REFERENCES tc_color(idtc_color),
			aliastc_articulo_tallacolor CHARACTER VARYING(50),
			PRIMARY KEY (idarticulo, idtc_talla, idtc_color)
		);
		CREATE TABLE tc_stock_almacen (
			idtc_stock_almacen SERIAL PRIMARY KEY,
			idarticulo INTEGER NOT NULL REFERENCES articulo(idarticulo),
			idalmacen INTEGER NOT NULL REFERENCES almacen(idalmacen),
			canttc_stock_almacen numeric(12, 2) DEFAULT 0
		);
		CREATE TABLE tc_stock_tallacolor (
			idtc_stock_almacen INTEGER NOT NULL REFERENCES tc_stock_almacen(idtc_stock_almacen),
			idtc_talla INTEGER REFERENCES tc_talla(idtc_talla),
			idtc_color INTEGER REFERENCES tc_color(idtc_color),
			canttc_stock_tallacolor NUMERIC(12,2) DEFAULT 0
		);
        END IF;

	SELECT INTO rs attname, relname FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid = pg_class.oid WHERE attname = ''idtc_talla'' AND relname = ''lalbaran'';
	IF NOT FOUND THEN
		-- Alteramos la tabla de presupuestos.
		ALTER TABLE lpresupuesto ADD COLUMN idtc_talla INTEGER;
		ALTER TABLE lpresupuesto ADD COLUMN idtc_color INTEGER;
		-- Agregamos cambios a los pedidos.
		ALTER TABLE lpedidocliente ADD COLUMN idtc_talla INTEGER;
		ALTER TABLE lpedidocliente ADD COLUMN idtc_color INTEGER;
		-- Agregamos cambios a los pedidos de proveedor.
		ALTER TABLE lpedidoproveedor ADD COLUMN idtc_talla INTEGER;
		ALTER TABLE lpedidoproveedor ADD COLUMN idtc_color INTEGER;
		-- Agregamos campos a los albaranes.
		ALTER TABLE lalbaran ADD COLUMN idtc_talla INTEGER;
		ALTER TABLE lalbaran ADD COLUMN idtc_color INTEGER;
		-- Agregamos campos a los albaranes de proveedor.
		ALTER TABLE lalbaranp ADD COLUMN idtc_talla INTEGER;
		ALTER TABLE lalbaranp ADD COLUMN idtc_color INTEGER;
		-- Agregamos cambios a las factura.
		ALTER TABLE lfactura ADD COLUMN idtc_talla INTEGER;
		ALTER TABLE lfactura ADD COLUMN idtc_color INTEGER;
		-- Agregamos cambios a las facturap.
		ALTER TABLE lfacturap ADD COLUMN idtc_talla INTEGER;
		ALTER TABLE lfacturap ADD COLUMN idtc_color INTEGER;
	END IF;

        RETURN 0;
END;
' LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo ":: Agregamos las tablas de tallas, colores y stock... "


--SELECT drop_if_exists_proc('tc_insertlalbaran', '');
--CREATE FUNCTION tc_insertlalbaran() RETURNS "trigger"
--AS '
--DECLARE
--   rs RECORD;
--   stockalmacen RECORD;
--   m_idalmacen INTEGER;
--BEGIN
--    SELECT INTO rs * FROM albaran WHERE idalbaran = NEW.idalbaran;
--    m_idalmacen := rs.idalmacen;

    -- Tratamos el control de stock general.
--    SELECT INTO stockalmacen * FROM tc_stock_almacen WHERE idalmacen = m_idalmacen AND idarticulo = NEW.idarticulo;
--    IF NOT FOUND THEN
--	INSERT INTO tc_stock_almacen (idarticulo, idalmacen, cantstock_almacen) VALUES (NEW.idarticulo, m_idalmacen, -NEW.cantlalbaran);
--	SELECT INTO stockalmacen * FROM tc_stock_almacen WHERE idalmacen = m_idalmacen AND idarticulo = NEW.idarticulo;	
--    ELSE
--	UPDATE tc_stock_almacen SET cantstock_almacen = cantstock_almacen - NEW.cantlalbaran WHERE idarticulo=NEW.idarticulo AND idalmacen = m_idalmacen;
--    END IF;

    -- Tratamos el control de stock para tallas y colores.
--    IF NEW.idtalla IS NOT NULL OR NEW.idcolor IS NOT NULL THEN
--	SELECT INTO rs * FROM tc_stock_tallacolor WHERE idstock_almacen = stockalmacen.idstock_almacen AND idtalla = NEW.idtalla AND idcolor = NEW.idcolor;
--	IF NOT FOUND THEN
--		INSERT INTO tc_stock_tallacolor (idstock_almacen, idtalla, idcolor, cantstock_tallacolor) VALUES (stockalmacen.idstock_almacen, NEW.idtalla, NEW.idcolor, -NEW.cantlalbaran);
--	ELSE
--		UPDATE tc_stock_tallacolor SET cantstock_tallacolor = cantstock_tallacolor -NEW.cantlalbaran WHERE idstock_almacen = stockalmacen.idstock_almacen AND idtalla = NEW.idtalla AND idcolor = NEW.idcolor;
--	END IF;			
--    END IF;
    
    
--    RETURN NEW;
--END;
--' LANGUAGE plpgsql;

--\echo -n ':: Disparador que disminuye stock al insertar una linea en el detalle de un albaran a cliente ... '
--CREATE TRIGGER tc_insertlalbarant
--    AFTER INSERT ON lalbaran
--    FOR EACH ROW
--    EXECUTE PROCEDURE tc_insertlalbaran();





-- ====================================================================================HECHO



-- ==============================================================================


-- Agregamos nuevos parametros de configuracion.
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	rs RECORD;
BEGIN
	SELECT INTO rs * FROM configuracion WHERE nombre=''PluginBf_TallasColores'';
	IF FOUND THEN
		UPDATE CONFIGURACION SET valor=''0.11.1-0002'' WHERE nombre=''PluginBf_TallasColores'';
	ELSE
		INSERT INTO configuracion (nombre, valor) VALUES (''PluginBf_TallasColores'', ''0.11.1-0002'');
	END IF;
	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT actualizarevision();
DROP FUNCTION actualizarevision() CASCADE;
\echo "Actualizada la revision de la base de datos a la version 0.11.1-0002"


DROP FUNCTION drop_if_exists_table(text) CASCADE;
DROP FUNCTION drop_if_exists_proc(text,text) CASCADE;


COMMIT;

