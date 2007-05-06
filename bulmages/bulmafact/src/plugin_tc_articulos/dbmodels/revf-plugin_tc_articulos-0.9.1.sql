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
                CREATE TABLE talla (
                        idtalla SERIAL PRIMARY KEY,
                        nomtalla CHARACTER VARYING(50)
                );
                CREATE TABLE color (
                        idcolor SERIAL PRIMARY KEY,
                        nomcolor CHARACTER VARYING(50),
			rgbcolor CHARACTER VARYING(13)
                );
                CREATE TABLE articulo_talla (
                        idtalla INTEGER NOT NULL,
                        idarticulo INTEGER NOT NULL,
			incprecio NUMERIC(12, 2),
			PRIMARY KEY (idtalla, idarticulo)
                );
                CREATE TABLE articulo_color (
                        idcolor INTEGER NOT NULL,
                        idarticulo INTEGER NOT NULL,
			incprecio NUMERIC(12, 2),
			PRIMARY KEY (idcolor, idarticulo)
                );
		CREATE TABLE tc_stock_almacen (
			idtc_stock_almacen SERIAL PRIMARY KEY,
			idarticulo INTEGER NOT NULL REFERENCES articulo(idarticulo),
			idalmacen INTEGER NOT NULL REFERENCES almacen(idalmacen),
			idtalla INTEGER,
			idcolor INTEGER,
			stock numeric(12, 2) DEFAULT 0
		);
        END IF;

	SELECT INTO rec attname, relname FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid=pg_class.oid WHERE attname=''idtalla'' AND relname=''lalbaran'';
	IF NOT FOUND THEN
		ALTER TABLE lalbaran ADD COLUMN idtalla INTEGER;
		ALTER TABLE lalbaran ADD COLUMN idcolor INTEGER;
		ALTER TABLE lalbaranp ADD COLUMN idtalla INTEGER;
		ALTER TABLE lalbaranp ADD COLUMN idcolor INTEGER;
	END IF;


        RETURN 0;
END;
' LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo ":: Agregamos las tablas de tallas, colores y stock... "



SELECT drop_if_exists_proc('tc_disminuyestock', '');
CREATE FUNCTION tc_disminuyestock() RETURNS "trigger"
AS '
DECLARE

BEGIN
    -- Hacemos el update del stock por almacenes
    UPDATE tc_stock_almacen SET stock = stock + OLD.cantlalbaran WHERE idarticulo = OLD.idarticulo AND idtalla = OLD.idtalla AND idcolor = OLD.idcolor AND idalmacen IN (SELECT idalmacen FROM albaran WHERE idalbaran = OLD.idalbaran);
    RETURN NEW;
END;
' LANGUAGE plpgsql;

\echo -n ':: Disparador que disminuye stock al borrar o actualizar el detalle de un albaran a cliente ... '
CREATE TRIGGER tc_disminuyestockt
    AFTER DELETE OR UPDATE ON lalbaran
    FOR EACH ROW
    EXECUTE PROCEDURE tc_disminuyestock();

SELECT drop_if_exists_proc('tc_aumentastock', '');

\echo -n ':: Funcion para aumentar stock ... '
CREATE FUNCTION tc_aumentastock() RETURNS "trigger"
AS '
DECLARE
	bs RECORD;
	almacen INTEGER;
BEGIN
    -- Cojo el almacen del albaran
    SELECT INTO bs idalmacen FROM albaran WHERE idalbaran = NEW.idalbaran;
    almacen := bs.idalmacen;

    -- Compruebo que exista el registro de stock para hacer o bien un insert o un update.
    SELECT INTO bs * from tc_stock_almacen WHERE idarticulo = NEW.idarticulo AND idtalla = NEW.idtalla AND idcolor = NEW.idcolor AND idalmacen = almacen;
    IF FOUND THEN
	-- Hacemos el update del stock por almacenes
	UPDATE tc_stock_almacen SET stock = stock - NEW.cantlalbaran WHERE idarticulo = NEW.idarticulo AND idtalla = NEW.idtalla AND idcolor = NEW.idcolor AND idalmacen = almacen;
    ELSE
	-- Hacemos el insert del stock por almacenes
	INSERT INTO tc_stock_almacen (idarticulo, idalmacen, idtalla, idcolor, stock) VALUES (NEW.idarticulo, almacen, NEW.idtalla, NEW.idcolor, -NEW.cantlalbaran);
    END IF;
    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador que aumenta stock al insertar o actualizar el detalle de un albaran a cliente ... '
CREATE TRIGGER tc_aumentastockt
    AFTER INSERT OR UPDATE ON lalbaran
    FOR EACH ROW
    EXECUTE PROCEDURE tc_aumentastock();    



-- ====================== PARTE DE ALBARANES DE PROVEEDOR

SELECT drop_if_exists_proc('tc_disminuyestockp', '');
CREATE FUNCTION tc_disminuyestockp() RETURNS "trigger"
AS '
DECLARE
BEGIN
    -- Hacemos el update del stock por almacenes
    UPDATE tc_stock_almacen SET stock = stock + OLD.cantlalbaranp WHERE idarticulo = OLD.idarticulo AND idtalla = OLD.idtalla AND idcolor = OLD.idcolor AND idalmacen IN (SELECT idalmacen FROM albaranp WHERE idalbaranp = OLD.idalbaranp);
    RETURN NEW;
END;
' LANGUAGE plpgsql;

\echo -n ':: Disparador que disminuye stock al borrar o actualizar el detalle de un albaran a cliente ... '
CREATE TRIGGER tc_disminuyestockpt
    AFTER DELETE OR UPDATE ON lalbaranp
    FOR EACH ROW
    EXECUTE PROCEDURE tc_disminuyestockp();

SELECT drop_if_exists_proc('tc_aumentastockp', '');
\echo -n ':: Funcion para aumentar stock ... '
CREATE FUNCTION tc_aumentastockp() RETURNS "trigger"
AS '
DECLARE
	bs RECORD;
	almacen INTEGER;
BEGIN
    -- Cojo el almacen del albaranp
    SELECT INTO bs idalmacen FROM albaranp WHERE idalbaranp = NEW.idalbaranp;
    almacen := bs.idalmacen;

    IF NEW.idtalla IS NULL THEN
	NEW.idtalla := 0;
    END IF;

    IF NEW.idcolor IS NULL THEN
	NEW.idcolor := 0;
    END IF;


    -- Compruebo que exista el registro de stock para hacer o bien un insert o un update.
    SELECT INTO bs * from tc_stock_almacen WHERE idarticulo = NEW.idarticulo AND idtalla = NEW.idtalla AND idcolor = NEW.idcolor AND idalmacen = almacen;
    IF FOUND THEN
	-- Hacemos el update del stock por almacenes
	RAISE NOTICE '' Entramos '';
	UPDATE tc_stock_almacen SET stock = stock - NEW.cantlalbaranp WHERE idarticulo = NEW.idarticulo AND idtalla = NEW.idtalla AND idcolor = NEW.idcolor AND idalmacen = almacen;
    ELSE
	-- Hacemos el insert del stock por almacenes
	INSERT INTO tc_stock_almacen (idarticulo, idalmacen, idtalla, idcolor, stock) VALUES (NEW.idarticulo, almacen, NEW.idtalla, NEW.idcolor, -NEW.cantlalbaranp);
    END IF;
    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador que aumenta stock al insertar o actualizar el detalle de un albaran a cliente ... '
CREATE TRIGGER tc_aumentastockpt
    AFTER INSERT OR UPDATE ON lalbaranp
    FOR EACH ROW
    EXECUTE PROCEDURE tc_aumentastockp();    


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
