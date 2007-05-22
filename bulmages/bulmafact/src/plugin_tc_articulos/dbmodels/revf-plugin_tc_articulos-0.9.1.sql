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
--	as RECORD;
--BEGIN
--	SELECT INTO as * FROM configuracion WHERE nombre=''DBRev-Tallas-y-Colores'' AND ( valor LIKE ''0.9.1%'' OR valor = ''0.9.1-0001'');
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
        rec RECORD;

BEGIN
        SELECT INTO rec * FROM pg_attribute WHERE attname = ''idtalla'';
        IF NOT FOUND THEN
                CREATE TABLE tc_talla (
                        idtalla SERIAL PRIMARY KEY,
                        nomtalla CHARACTER VARYING(50)
                );
                CREATE TABLE tc_color (
                        idcolor SERIAL PRIMARY KEY,
                        nomcolor CHARACTER VARYING(50),
			rgbcolor CHARACTER VARYING(13)
                );
                CREATE TABLE tc_articulo_talla (
                        idtalla INTEGER NOT NULL,
                        idarticulo INTEGER NOT NULL,
			incprecioarticulo_talla NUMERIC(12, 2),
			PRIMARY KEY (idtalla, idarticulo)
                );
                CREATE TABLE tc_articulo_color (
                        idcolor INTEGER NOT NULL,
                        idarticulo INTEGER NOT NULL,
			incprecioarticulo_color NUMERIC(12, 2),
			PRIMARY KEY (idcolor, idarticulo)
                );
		CREATE TABLE tc_stock_almacen (
			idstock_almacen SERIAL PRIMARY KEY,
			idarticulo INTEGER NOT NULL REFERENCES articulo(idarticulo),
			idalmacen INTEGER NOT NULL REFERENCES almacen(idalmacen),
			cantstock_almacen numeric(12, 2) DEFAULT 0
		);
		CREATE TABLE tc_stock_tallacolor (
			idstock_almacen INTEGER NOT NULL REFERENCES tc_stock_almacen(idstock_almacen),
			idtalla INTEGER REFERENCES tc_talla(idtalla),
			idcolor INTEGER REFERENCES tc_color(idcolor),
			cantstock_tallacolor NUMERIC(12,2) DEFAULT 0
		);
        END IF;

	SELECT INTO rec attname, relname FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid = pg_class.oid WHERE attname = ''idtalla'' AND relname = ''lalbaran'';
	IF NOT FOUND THEN
		-- Alteramos la tabla de presupuestos.
		ALTER TABLE lpresupuesto ADD COLUMN idtalla INTEGER;
		ALTER TABLE lpresupuesto ADD COLUMN idcolor INTEGER;
		-- Agregamos cambios a los pedidos.
		ALTER TABLE lpedidocliente ADD COLUMN idtalla INTEGER;
		ALTER TABLE lpedidocliente ADD COLUMN idcolor INTEGER;
		-- Agregamos cambios a los pedidos de proveedor.
		ALTER TABLE lpedidoproveedor ADD COLUMN idtalla INTEGER;
		ALTER TABLE lpedidoproveedor ADD COLUMN idcolor INTEGER;
		-- Agregamos campos a los albaranes.
		ALTER TABLE lalbaran ADD COLUMN idtalla INTEGER;
		ALTER TABLE lalbaran ADD COLUMN idcolor INTEGER;
		-- Agregamos campos a los albaranes de proveedor.
		ALTER TABLE lalbaranp ADD COLUMN idtalla INTEGER;
		ALTER TABLE lalbaranp ADD COLUMN idcolor INTEGER;
		-- Agregamos cambios a las factura.
		ALTER TABLE lfactura ADD COLUMN idtalla INTEGER;
		ALTER TABLE lfactura ADD COLUMN idcolor INTEGER;
		-- Agregamos cambios a las facturap.
		ALTER TABLE lfacturap ADD COLUMN idtalla INTEGER;
		ALTER TABLE lfacturap ADD COLUMN idcolor INTEGER;
	END IF;

        RETURN 0;
END;
' LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo ":: Agregamos las tablas de tallas, colores y stock... "


SELECT drop_if_exists_proc('tc_insertlalbaran', '');
CREATE FUNCTION tc_insertlalbaran() RETURNS "trigger"
AS '
DECLARE
   bs RECORD;
   stockalmacen RECORD;
   m_idalmacen INTEGER;
BEGIN
    SELECT INTO bs * FROM albaran WHERE idalbaran = NEW.idalbaran;
    m_idalmacen := bs.idalmacen;

    -- Tratamos el control de stock general.
    SELECT INTO stockalmacen * FROM tc_stock_almacen WHERE idalmacen = m_idalmacen AND idarticulo = NEW.idarticulo;
    IF NOT FOUND THEN
	INSERT INTO tc_stock_almacen (idarticulo, idalmacen, cantstock_almacen) VALUES (NEW.idarticulo, m_idalmacen, -NEW.cantlalbaran);
	SELECT INTO stockalmacen * FROM tc_stock_almacen WHERE idalmacen = m_idalmacen AND idarticulo = NEW.idarticulo;	
    ELSE
	UPDATE tc_stock_almacen SET cantstock_almacen = cantstock_almacen - NEW.cantlalbaran WHERE idarticulo=NEW.idarticulo AND idalmacen = m_idalmacen;
    END IF;

    -- Tratamos el control de stock para tallas y colores.
    IF NEW.idtalla IS NOT NULL OR NEW.idcolor IS NOT NULL THEN
	SELECT INTO bs * FROM tc_stock_tallacolor WHERE idstock_almacen = stockalmacen.idstock_almacen AND idtalla = NEW.idtalla AND idcolor = NEW.idcolor;
	IF NOT FOUND THEN
		INSERT INTO tc_stock_tallacolor (idstock_almacen, idtalla, idcolor, cantstock_tallacolor) VALUES (stockalmacen.idstock_almacen, NEW.idtalla, NEW.idcolor, -NEW.cantlalbaran);
	ELSE
		UPDATE tc_stock_tallacolor SET cantstock_tallacolor = cantstock_tallacolor -NEW.cantlalbaran WHERE idstock_almacen = stockalmacen.idstock_almacen AND idtalla = NEW.idtalla AND idcolor = NEW.idcolor;
	END IF;			
    END IF;
    
    
    RETURN NEW;
END;
' LANGUAGE plpgsql;

\echo -n ':: Disparador que disminuye stock al insertar una linea en el detalle de un albaran a cliente ... '
CREATE TRIGGER tc_insertlalbarant
    AFTER INSERT ON lalbaran
    FOR EACH ROW
    EXECUTE PROCEDURE tc_insertlalbaran();





-- ====================================================================================HECHO



-- ==============================================================================


-- Agregamos nuevos parametros de configuracion.
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	bs RECORD;
BEGIN
	SELECT INTO bs * FROM configuracion WHERE nombre=''DBRev-Tallas-y-Colores'';
	IF FOUND THEN
		UPDATE CONFIGURACION SET valor=''0.9.1-0001'' WHERE nombre=''DBRev-Tallas-y-Colores'';
	ELSE
		INSERT INTO configuracion (nombre, valor) VALUES (''DBRev-Tallas-y-Colores'', ''0.9.1-0001'');
	END IF;
	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT actualizarevision();
DROP FUNCTION actualizarevision() CASCADE;
\echo "Actualizada la revision de la base de datos a la version 0.9.1"


DROP FUNCTION drop_if_exists_table(text) CASCADE;
DROP FUNCTION drop_if_exists_proc(text,text) CASCADE;


COMMIT;

