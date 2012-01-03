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
	IF NOT FOUND THEN

		CREATE TABLE inventario (
		idinventario SERIAL PRIMARY KEY,
		nominventario varchar NOT NULL,
		fechainventario date default now()
		);

		
		CREATE TABLE controlstock (
		idinventario integer NOT NULL REFERENCES inventario(idinventario),
		idalmacen integer NOT NULL REFERENCES almacen(idalmacen),
		idarticulo integer NOT NULL REFERENCES articulo(idarticulo),
		stockantcontrolstock numeric(12, 2) NOT NULL,
		stocknewcontrolstock numeric(12, 2) NOT NULL,
		punteocontrolstock boolean NOT NULL DEFAULT FALSE,
		PRIMARY KEY (idinventario, idalmacen, idarticulo)
		);

		
		CREATE TABLE stock_almacen (
		idarticulo integer NOT NULL REFERENCES articulo(idarticulo),
		idalmacen integer NOT NULL REFERENCES almacen(idalmacen),
		stock numeric(12, 2) DEFAULT 0,
		PRIMARY KEY (idarticulo, idalmacen)
		);
	END IF;

	SELECT INTO rs * FROM pg_tables WHERE tablename=''minimsalmacen'';
	IF NOT FOUND THEN
		CREATE TABLE minimsalmacen (
		idminimsalmacen SERIAL PRIMARY KEY,
		idalmacen integer NOT NULL REFERENCES almacen(idalmacen),
		idarticulo integer NOT NULL REFERENCES articulo(idarticulo),
		valminimsalmacen numeric(12,2) NOT NULL
		);
		
	END IF;

	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;

SELECT drop_if_exists_proc('s_narticulo', '');
\echo -n ':: Funcion para insertar un nuevo articulo en el stock ... '
CREATE FUNCTION s_narticulo() RETURNS "trigger"
AS '
DECLARE
    rs RECORD;

BEGIN
    FOR rs IN SELECT * FROM almacen LOOP
	INSERT INTO stock_almacen (idarticulo, idalmacen, stock) VALUES (NEW.idarticulo, rs.idalmacen, 0);
    END LOOP;

    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador para insertar un nuevo articulo en el stock ... '
CREATE TRIGGER s_narticulot
    AFTER INSERT ON articulo
    FOR EACH ROW
    EXECUTE PROCEDURE s_narticulo();



SELECT drop_if_exists_proc('s_darticulo', '');
\echo -n ':: Funcion para borrar un articulo en el stock ... '
CREATE FUNCTION s_darticulo() RETURNS "trigger"
AS '
DECLARE
BEGIN
    DELETE FROM stock_almacen WHERE idarticulo = OLD.idarticulo;
    RETURN OLD;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador para borrar un articulo en el stock ... '
CREATE TRIGGER s_darticulot
    BEFORE DELETE ON articulo
    FOR EACH ROW
    EXECUTE PROCEDURE s_darticulo();


SELECT drop_if_exists_proc('s_nalmacen', '');
\echo -n ':: Funcion para insertar stock en un almacen ... '
CREATE FUNCTION s_nalmacen() RETURNS "trigger"
AS '
DECLARE
    rs RECORD;

BEGIN
    FOR rs IN SELECT * FROM articulo LOOP
	INSERT INTO stock_almacen (idarticulo, idalmacen, stock) VALUES (rs.idarticulo, NEW.idalmacen, 0);
    END LOOP;

    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador al insertar stock en un almacen ... '
CREATE TRIGGER s_nalmacent
    AFTER INSERT ON almacen
    FOR EACH ROW
    EXECUTE PROCEDURE s_nalmacen();


SELECT drop_if_exists_proc('s_dalmacen', '');
\echo -n ':: Funcion que borra stock de un almacen ... '
CREATE FUNCTION s_dalmacen() RETURNS "trigger"
AS '
DECLARE
BEGIN
    DELETE FROM stock_almacen WHERE idalmacen = OLD.idalmacen;
    RETURN OLD;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador que borra stock de un almacen ... '
CREATE TRIGGER s_dalmacent
    BEFORE DELETE ON almacen
    FOR EACH ROW
    EXECUTE PROCEDURE s_dalmacen();


SELECT drop_if_exists_proc('s_disminuyestock', '');
\echo -n ':: Funcion para disminuir stock ... '
CREATE FUNCTION s_disminuyestock() RETURNS "trigger"
AS '
DECLARE
BEGIN
    -- Hacemos el update del stock por almacenes
    UPDATE stock_almacen SET stock = stock + OLD.cantlalbaran WHERE idarticulo = OLD.idarticulo AND idalmacen IN (SELECT idalmacen FROM albaran WHERE idalbaran = OLD.idalbaran);

    RETURN NEW;
END;
' LANGUAGE plpgsql;

\echo -n ':: Disparador que disminuye stock al borrar o actualizar el detalle de un albaran a cliente ... '
CREATE TRIGGER s_disminuyestockt
    AFTER DELETE OR UPDATE ON lalbaran
    FOR EACH ROW
    EXECUTE PROCEDURE s_disminuyestock();

SELECT drop_if_exists_proc('s_aumentastock', '');
\echo -n ':: Funcion para aumentar stock ... '
CREATE FUNCTION s_aumentastock() RETURNS "trigger"
AS '
DECLARE
BEGIN
    -- Hacemos el update del stock por almacenes
    UPDATE stock_almacen SET stock = stock - NEW.cantlalbaran WHERE idarticulo = NEW.idarticulo AND idalmacen IN (SELECT idalmacen FROM albaran WHERE idalbaran=NEW.idalbaran);

    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador que aumenta stock al insertar o actualizar el detalle de un albaran a cliente ... '
CREATE TRIGGER s_aumentastockt
    AFTER INSERT OR UPDATE ON lalbaran
    FOR EACH ROW
    EXECUTE PROCEDURE s_aumentastock();


SELECT drop_if_exists_proc('s_disminuyestockp', '');
\echo -n ':: Funcion disminuye stockp ... '
CREATE FUNCTION s_disminuyestockp() RETURNS "trigger"
AS '
DECLARE
BEGIN
    -- Hacemos el update del stock por almacenes
    UPDATE stock_almacen SET stock = stock - OLD.cantlalbaranp WHERE idarticulo = OLD.idarticulo AND idalmacen IN (SELECT idalmacen FROM albaranp WHERE idalbaranp=OLD.idalbaranp);

    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador que disminuye stock al borrar o actualizar el detalle de un albaran de proveedor ... '
CREATE TRIGGER s_disminuyestockpt
    AFTER DELETE OR UPDATE ON lalbaranp
    FOR EACH ROW
    EXECUTE PROCEDURE s_disminuyestockp();


SELECT drop_if_exists_proc('s_aumentastockp', '');
\echo -n ':: Funcion que aumenta el stock al recibir mercancias de un proveedor ... '
CREATE FUNCTION s_aumentastockp() RETURNS "trigger"
AS '
DECLARE
BEGIN
    -- Hacemos el update del stock por almacenes
    UPDATE stock_almacen SET stock = stock + NEW.cantlalbaranp WHERE idarticulo = NEW.idarticulo AND idalmacen IN (SELECT idalmacen FROM albaranp WHERE idalbaranp=NEW.idalbaranp);

    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador que aumenta stock al insertar o actualizar el detalle de un albaran de proveedor ... '
CREATE TRIGGER s_aumentastockpt
    AFTER INSERT OR UPDATE ON lalbaranp
    FOR EACH ROW
    EXECUTE PROCEDURE s_aumentastockp(); 


SELECT drop_if_exists_proc('s_modificadostockalmacen', '');
\echo -n ':: Funcion que modificado el stock de un almacen ... '
CREATE OR REPLACE FUNCTION s_modificadostockalmacen() RETURNS "trigger"
AS '
DECLARE 
    cant numeric;
    rs RECORD;

BEGIN
    IF NEW.stock <> OLD.stock THEN
	cant := NEW.stock - OLD.stock;
	FOR rs IN SELECT * FROM comparticulo WHERE idarticulo = NEW.idarticulo LOOP
	    UPDATE stock_almacen SET stock = stock + cant * rs.cantcomparticulo WHERE idarticulo = rs.idcomponente AND idalmacen = NEW.idalmacen;
	END LOOP;
    END IF;

    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador que modifica el stock de un almacen ... '
CREATE TRIGGER s_modificastocktrigger
    AFTER UPDATE ON stock_almacen
    FOR EACH ROW
    EXECUTE PROCEDURE s_modificadostockalmacen();


SELECT drop_if_exists_proc('s_disminuyecontrolstock', '');
\echo -n ':: Funcion que disminuye control de stock ... '
CREATE FUNCTION s_disminuyecontrolstock() RETURNS "trigger"
AS '
DECLARE
BEGIN
    -- Hacemos el update del stock del articulo
    UPDATE articulo SET stockarticulo = stockarticulo - OLD.stocknewcontrolstock + OLD.stockantcontrolstock WHERE idarticulo= OLD.idarticulo;

    -- Hacemos el update del stock por almacenes
    UPDATE stock_almacen SET stock = stock - OLD.stocknewcontrolstock + OLD.stockantcontrolstock WHERE idarticulo = OLD.idarticulo AND idalmacen = OLD.idalmacen;

    RETURN OLD;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador que disminuye stock al borrar en control de stock ... '
CREATE TRIGGER s_disminuyecontrolstockt
    BEFORE DELETE ON controlstock
    FOR EACH ROW
    EXECUTE PROCEDURE s_disminuyecontrolstock();


SELECT drop_if_exists_proc('s_disminuyecontrolstock1', '');
\echo -n ':: Funcion que disminuye el control de stock ... '
CREATE FUNCTION s_disminuyecontrolstock1() RETURNS "trigger"
AS '
DECLARE
    rs RECORD;

BEGIN
    -- Cogemos el stock anterior.
    FOR rs IN SELECT * FROM stock_almacen WHERE idarticulo = NEW.idarticulo AND idalmacen = NEW.idalmacen LOOP
	NEW.stockantcontrolstock := rs.stock;
    END LOOP;

    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador que disminuye control stock ... '
CREATE TRIGGER s_disminuyecontrolstockt1
    BEFORE INSERT ON controlstock
    FOR EACH ROW
    EXECUTE PROCEDURE s_disminuyecontrolstock1();


SELECT drop_if_exists_proc('s_disminuyecontrolstock2', '');
\echo -n ':: Funcion que disminuye control stock ... '
CREATE FUNCTION s_disminuyecontrolstock2() RETURNS "trigger"
AS '
DECLARE
    rs RECORD;

BEGIN
    -- Hacemos el update del stock del articulo
    UPDATE articulo SET stockarticulo = stockarticulo - OLD.stocknewcontrolstock + OLD.stockantcontrolstock WHERE idarticulo= OLD.idarticulo;

    -- Hacemos el update del stock por almacenes
    UPDATE stock_almacen SET stock = stock - OLD.stocknewcontrolstock + OLD.stockantcontrolstock WHERE idarticulo = OLD.idarticulo AND idalmacen = OLD.idalmacen;

    -- Cogemos el stock anterior.
    FOR rs IN SELECT * FROM stock_almacen WHERE idarticulo = NEW.idarticulo AND idalmacen = NEW.idalmacen LOOP
	NEW.stockantcontrolstock := rs.stock;
    END LOOP;

    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador que disminuye control de stock ... '
CREATE TRIGGER s_disminuyecontrolstockt2
    BEFORE UPDATE ON controlstock
    FOR EACH ROW
    EXECUTE PROCEDURE s_disminuyecontrolstock2();


SELECT drop_if_exists_proc('s_aumentacontrolstock', '');
\echo -n ':: Funcion que aumenta control de stock ... '
CREATE FUNCTION s_aumentacontrolstock() RETURNS "trigger"
AS '
DECLARE

BEGIN
    UPDATE articulo SET stockarticulo = stockarticulo + NEW.stocknewcontrolstock - NEW.stockantcontrolstock WHERE idarticulo = NEW.idarticulo;

    -- Hacemos el update del stock por almacenes
    UPDATE stock_almacen SET stock = NEW.stocknewcontrolstock WHERE idarticulo = NEW.idarticulo AND idalmacen = NEW.idalmacen;

    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador que controla el control de stock ... '
CREATE TRIGGER s_aumentacontrolstockt
    AFTER INSERT OR UPDATE ON controlstock
    FOR EACH ROW
    EXECUTE PROCEDURE s_aumentacontrolstock(); 



-- Cuando cambiamos el almacen en un albaran o albaranp hay problemas con el control de stock
-- que se descuadra.
SELECT drop_if_exists_proc('s_cambiaalbaran', '');
\echo -n ':: Funcion que actualiza el stock al cambiar un albaran a cliente ... '
CREATE FUNCTION s_cambiaalbaran() RETURNS "trigger"
AS '
DECLARE
    rs RECORD;

BEGIN
    IF NEW.idalmacen <> OLD.idalmacen THEN
	FOR rs IN SELECT * FROM lalbaran WHERE idalbaran = NEW.idalbaran LOOP
    	    UPDATE stock_almacen SET stock = stock + rs.cantlalbaran WHERE idarticulo = rs.idarticulo AND idalmacen = OLD.idalmacen;
	    UPDATE stock_almacen SET stock = stock - rs.cantlalbaran WHERE idarticulo = rs.idarticulo AND idalmacen = NEW.idalmacen;
	END LOOP;
    END IF;

    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador que controla cuando se actualiza un albaran a un cliente ... '
CREATE TRIGGER s_cambiadoalbarant
    AFTER UPDATE ON albaran
    FOR EACH ROW
    EXECUTE PROCEDURE s_cambiaalbaran();


SELECT drop_if_exists_proc('s_cambiaalbaranp', '');
\echo -n ':: Funcion que actualiza el stock al cambiar un albaran de proveedor ... '
CREATE FUNCTION s_cambiaalbaranp() RETURNS "trigger"
AS '
DECLARE
    rs RECORD;

BEGIN
    IF NEW.idalmacen <> OLD.idalmacen THEN
	FOR rs IN SELECT * FROM lalbaranp WHERE idalbaranp = NEW.idalbaranp LOOP
	    UPDATE stock_almacen SET stock = stock - rs.cantlalbaranp WHERE idarticulo = rs.idarticulo AND idalmacen = OLD.idalmacen;
	    UPDATE stock_almacen SET stock = stock + rs.cantlalbaranp WHERE idarticulo = rs.idarticulo AND idalmacen = NEW.idalmacen;
	END LOOP;
    END IF;

    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador que controla cuando se actualiza un albaran de proveedor ... '
CREATE TRIGGER s_cambiadoalbaranpt
    AFTER UPDATE ON albaranp
    FOR EACH ROW
    EXECUTE PROCEDURE s_cambiaalbaranp();




-- ==============================================================================


-- Agregamos nuevos parametros de configuraci�.
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	rs RECORD;
BEGIN
	SELECT INTO rs * FROM configuracion WHERE nombre=''PluginBf_Inventario'';
	IF FOUND THEN
		UPDATE CONFIGURACION SET valor=''0.10.1-0002'' WHERE nombre=''PluginBf_Inventario'';
	ELSE
		INSERT INTO configuracion (nombre, valor) VALUES (''PluginBf_Inventario'', ''0.10.1-0002'');
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
