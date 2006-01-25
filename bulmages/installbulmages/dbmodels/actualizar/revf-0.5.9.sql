--
-- Modificación de campos y funciones de la BD para la adaptaci� al tipo de datos monetario
--

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

-- SELECT drop_if_exists_proc ('calculacodigocompletoarticulo','');

SELECT drop_if_exists_proc ('narticulo','');
CREATE FUNCTION narticulo () RETURNS "trigger"
AS '
DECLARE
	as RECORD;
BEGIN
		FOR as IN SELECT * FROM almacen LOOP
			INSERT INTO stock_almacen (idarticulo, idalmacen, stock) VALUES (NEW.idarticulo, as.idalmacen, 0);
		END LOOP;
	RETURN NEW;
END;
' LANGUAGE plpgsql;

CREATE TRIGGER narticulot
    AFTER INSERT ON articulo
    FOR EACH ROW
    EXECUTE PROCEDURE narticulo();

SELECT drop_if_exists_proc ('darticulo','');
CREATE FUNCTION darticulo () RETURNS "trigger"
AS '
DECLARE
BEGIN
	DELETE FROM stock_almacen WHERE idarticulo = OLD.idarticulo;
	RETURN OLD;
END;
' LANGUAGE plpgsql;

CREATE TRIGGER darticulot
    BEFORE DELETE ON articulo
    FOR EACH ROW
    EXECUTE PROCEDURE darticulo();

SELECT drop_if_exists_proc ('nalmacen','');
CREATE FUNCTION nalmacen () RETURNS "trigger"
AS '
DECLARE
	as RECORD;
BEGIN
		FOR as IN SELECT * FROM articulo LOOP
			INSERT INTO stock_almacen (idarticulo, idalmacen, stock) VALUES (as.idarticulo, NEW.idalmacen, 0);
		END LOOP;
	RETURN NEW;
END;
' LANGUAGE plpgsql;

CREATE TRIGGER nalmacent
    AFTER INSERT ON almacen
    FOR EACH ROW
    EXECUTE PROCEDURE nalmacen();

SELECT drop_if_exists_proc ('dalmacen','');
CREATE FUNCTION dalmacen () RETURNS "trigger"
AS '
DECLARE
BEGIN
	DELETE FROM stock_almacen WHERE idalmacen = OLD.idalmacen;
	RETURN OLD;
END;
' LANGUAGE plpgsql;

CREATE TRIGGER dalmacent
    BEFORE DELETE ON almacen
    FOR EACH ROW
    EXECUTE PROCEDURE dalmacen();

SELECT drop_if_exists_proc ('disminuyestock','');
CREATE FUNCTION disminuyestock () RETURNS "trigger"
AS '
DECLARE
BEGIN
	-- Hacemos el update del stock del articulo
	UPDATE articulo SET stockarticulo = stockarticulo + OLD.cantlalbaran WHERE idarticulo= OLD.idarticulo;
	-- Hacemos el update del stock por almacenes
	UPDATE stock_almacen SET stock = stock + OLD.cantlalbaran WHERE idarticulo = OLD.idarticulo AND idalmacen IN (SELECT idalmacen FROM albaran WHERE idalbaran=OLD.idalbaran);
	RETURN NEW;
END;
' LANGUAGE plpgsql;


CREATE TRIGGER disminuyestockt
    AFTER DELETE OR UPDATE ON lalbaran
    FOR EACH ROW
    EXECUTE PROCEDURE disminuyestock();

SELECT drop_if_exists_proc ('aumentastock','');
CREATE FUNCTION aumentastock () RETURNS "trigger"
AS '
DECLARE
BEGIN
	-- Hacemos el update del stock del articulo
	UPDATE articulo SET stockarticulo = stockarticulo - NEW.cantlalbaran WHERE idarticulo = NEW.idarticulo;
	-- Hacemos el update del stock por almacenes
	UPDATE stock_almacen SET stock = stock - NEW.cantlalbaran WHERE idarticulo = NEW.idarticulo AND idalmacen IN (SELECT idalmacen FROM albaran WHERE idalbaran=NEW.idalbaran);
	RETURN NEW;
END;
' LANGUAGE plpgsql;


CREATE TRIGGER aumentastockt
    AFTER INSERT OR UPDATE ON lalbaran
    FOR EACH ROW
    EXECUTE PROCEDURE aumentastock();    

SELECT drop_if_exists_proc ('disminuyestockp','');
CREATE FUNCTION disminuyestockp () RETURNS "trigger"
AS '
DECLARE
BEGIN
	-- Hacemos el update del stock del articulo
	UPDATE articulo SET stockarticulo = stockarticulo - OLD.cantlalbaranp WHERE idarticulo= OLD.idarticulo;
	-- Hacemos el update del stock por almacenes
	UPDATE stock_almacen SET stock = stock - OLD.cantlalbaranp WHERE idarticulo = OLD.idarticulo AND idalmacen IN (SELECT idalmacen FROM albaranp WHERE idalbaranp=OLD.idalbaranp);
	RETURN NEW;
END;
' LANGUAGE plpgsql;


CREATE TRIGGER disminuyestockpt
    AFTER DELETE OR UPDATE ON lalbaranp
    FOR EACH ROW
    EXECUTE PROCEDURE disminuyestockp();

SELECT drop_if_exists_proc ('aumentastockp','');
CREATE FUNCTION aumentastockp () RETURNS "trigger"
AS '
DECLARE
BEGIN
	UPDATE articulo SET stockarticulo = stockarticulo + NEW.cantlalbaranp WHERE idarticulo = NEW.idarticulo;
	-- Hacemos el update del stock por almacenes
	UPDATE stock_almacen SET stock = stock + NEW.cantlalbaranp WHERE idarticulo = NEW.idarticulo AND idalmacen IN (SELECT idalmacen FROM albaranp WHERE idalbaranp=NEW.idalbaranp);
	RETURN NEW;
END;
' LANGUAGE plpgsql;


CREATE TRIGGER aumentastockpt
    AFTER INSERT OR UPDATE ON lalbaranp
    FOR EACH ROW
    EXECUTE PROCEDURE aumentastockp(); 

SELECT drop_if_exists_proc ('modificadostock','');
CREATE OR REPLACE FUNCTION modificadostock () RETURNS "trigger"
AS '
DECLARE 
	cant numeric;
	as RECORD;
BEGIN
	IF NEW.stockarticulo <> OLD.stockarticulo THEN
		cant := NEW.stockarticulo - OLD.stockarticulo;
		FOR as IN SELECT * FROM comparticulo WHERE idarticulo = NEW.idarticulo LOOP
			UPDATE articulo SET stockarticulo = stockarticulo + cant * as.cantcomparticulo WHERE idarticulo = as.idcomponente;
		END LOOP;
	END IF;
	RETURN NEW;
END;
' LANGUAGE plpgsql;

CREATE TRIGGER modificastocktrigger
	AFTER UPDATE ON articulo
	FOR EACH ROW
	EXECUTE PROCEDURE modificadostock();

SELECT drop_if_exists_proc ('modificadostockalmacen','');
CREATE OR REPLACE FUNCTION modificadostockalmacen () RETURNS "trigger"
AS '
DECLARE 
	cant numeric;
	as RECORD;
BEGIN
	IF NEW.stock <> OLD.stock THEN
		cant := NEW.stock - OLD.stock;
		FOR as IN SELECT * FROM comparticulo WHERE idarticulo = NEW.idarticulo LOOP
			UPDATE stock_almacen SET stock = stock + cant * as.cantcomparticulo WHERE idarticulo = as.idcomponente AND idalmacen = NEW.idalmacen;
		END LOOP;
	END IF;
	RETURN NEW;
END;
' LANGUAGE plpgsql;

CREATE TRIGGER modificastocktrigger
	AFTER UPDATE ON stock_almacen
	FOR EACH ROW
	EXECUTE PROCEDURE modificadostockalmacen();


-- aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
SELECT drop_if_exists_proc ('disminuyecontrolstock','');
CREATE FUNCTION disminuyecontrolstock () RETURNS "trigger"
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


CREATE TRIGGER disminuyecontrolstockt
    BEFORE DELETE ON controlstock
    FOR EACH ROW
    EXECUTE PROCEDURE disminuyecontrolstock();

SELECT drop_if_exists_proc ('disminuyecontrolstock1','');
CREATE FUNCTION disminuyecontrolstock1 () RETURNS "trigger"
AS '
DECLARE
	rant RECORD;
BEGIN
	-- Cogemos el stock anterior.
	FOR  rant IN SELECT * FROM stock_almacen WHERE idarticulo = NEW.idarticulo AND idalmacen = NEW.idalmacen LOOP
		NEW.stockantcontrolstock := rant.stock;
	END LOOP;
	RETURN NEW;
END;
' LANGUAGE plpgsql;


CREATE TRIGGER disminuyecontrolstockt1
    BEFORE INSERT ON controlstock
    FOR EACH ROW
    EXECUTE PROCEDURE disminuyecontrolstock1();

SELECT drop_if_exists_proc ('disminuyecontrolstock2','');
CREATE FUNCTION disminuyecontrolstock2 () RETURNS "trigger"
AS '
DECLARE
	rant RECORD;
BEGIN
		-- Hacemos el update del stock del articulo
		UPDATE articulo SET stockarticulo = stockarticulo - OLD.stocknewcontrolstock + OLD.stockantcontrolstock WHERE idarticulo= OLD.idarticulo;
		-- Hacemos el update del stock por almacenes
		UPDATE stock_almacen SET stock = stock - OLD.stocknewcontrolstock + OLD.stockantcontrolstock WHERE idarticulo = OLD.idarticulo AND idalmacen = OLD.idalmacen;

	-- Cogemos el stock anterior.
	FOR  rant IN SELECT * FROM stock_almacen WHERE idarticulo = NEW.idarticulo AND idalmacen = NEW.idalmacen LOOP
		NEW.stockantcontrolstock := rant.stock;
	END LOOP;
	RETURN NEW;
END;
' LANGUAGE plpgsql;


CREATE TRIGGER disminuyecontrolstockt2
    BEFORE UPDATE ON controlstock
    FOR EACH ROW
    EXECUTE PROCEDURE disminuyecontrolstock2();




SELECT drop_if_exists_proc ('aumentacontrolstock','');
CREATE FUNCTION aumentacontrolstock () RETURNS "trigger"
AS '
DECLARE
BEGIN
	UPDATE articulo SET stockarticulo = stockarticulo + NEW.stocknewcontrolstock - NEW.stockantcontrolstock WHERE idarticulo = NEW.idarticulo;
	-- Hacemos el update del stock por almacenes
	UPDATE stock_almacen SET stock = NEW.stocknewcontrolstock WHERE idarticulo = NEW.idarticulo AND idalmacen = NEW.idalmacen;
	RETURN NEW;
END;
' LANGUAGE plpgsql;


CREATE TRIGGER aumentacontrolstockt
    AFTER INSERT OR UPDATE ON controlstock
    FOR EACH ROW
    EXECUTE PROCEDURE aumentacontrolstock(); 


-- Cuando cambiamos el almacen en un albaran o albaranp hay problemas con el control de stock que se descuadra.
SELECT drop_if_exists_proc ('cambiaalbaran','');
CREATE FUNCTION cambiaalbaran () RETURNS "trigger"
AS '
DECLARE
	as RECORD;
BEGIN
	IF NEW.idalmacen <> OLD.idalmacen THEN
		FOR as IN SELECT * FROM lalbaran WHERE idalbaran = NEW.idalbaran LOOP
			UPDATE stock_almacen SET stock = stock + as.cantlalbaran WHERE idarticulo = as.idarticulo AND idalmacen = OLD.idalmacen;
			UPDATE stock_almacen SET stock = stock - as.cantlalbaran WHERE idarticulo = as.idarticulo AND idalmacen = NEW.idalmacen;
		END LOOP;
	END IF;
	RETURN NEW;
END;
' LANGUAGE plpgsql;

CREATE TRIGGER cambiadoalbarant
	AFTER UPDATE ON albaran
	FOR EACH ROW
	EXECUTE PROCEDURE cambiaalbaran();

SELECT drop_if_exists_proc ('cambiaalbaranp','');
CREATE FUNCTION cambiaalbaranp () RETURNS "trigger"
AS '
DECLARE
	as RECORD;
BEGIN
	IF NEW.idalmacen <> OLD.idalmacen THEN
		FOR as IN SELECT * FROM lalbaranp WHERE idalbaranp = NEW.idalbaranp LOOP
			UPDATE stock_almacen SET stock = stock - as.cantlalbaranp WHERE idarticulo = as.idarticulo AND idalmacen = OLD.idalmacen;
			UPDATE stock_almacen SET stock = stock + as.cantlalbaranp WHERE idarticulo = as.idarticulo AND idalmacen = NEW.idalmacen;
		END LOOP;
	END IF;
	RETURN NEW;
END;
' LANGUAGE plpgsql;

CREATE TRIGGER cambiadoalbaranpt
	AFTER UPDATE ON albaranp
	FOR EACH ROW
	EXECUTE PROCEDURE cambiaalbaranp();




CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
	as RECORD;
BEGIN
	SELECT INTO as * FROM pg_attribute  WHERE attname=''totalpresupuesto'';
	IF NOT FOUND THEN
		ALTER TABLE presupuesto ADD COLUMN totalpresupuesto NUMERIC(12,2);
		ALTER TABLE presupuesto ADD COLUMN bimppresupuesto NUMERIC(12,2);
		ALTER TABLE presupuesto ADD COLUMN imppresupuesto NUMERIC(12,2);
		ALTER TABLE presupuesto ALTER COLUMN totalpresupuesto SET DEFAULT 0;
		ALTER TABLE presupuesto ALTER COLUMN bimppresupuesto SET DEFAULT 0;
		ALTER TABLE presupuesto ALTER COLUMN imppresupuesto SET DEFAULT 0;
	END IF;

	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo "Agregamos los campos de totales para los presupuestos."

SELECT drop_if_exists_proc ('actualizatotpres','');
CREATE OR REPLACE FUNCTION actualizatotpres() returns TRIGGER
AS '
DECLARE
	tot NUMERIC(12,2);
	bimp NUMERIC(12,2);
	imp NUMERIC(12,2);
BEGIN
	tot := calctotalpres(NEW.idpresupuesto);
	bimp := calcbimppres(NEW.idpresupuesto);
	imp := calcimpuestospres(NEW.idpresupuesto);
	UPDATE presupuesto SET totalpresupuesto = tot, bimppresupuesto = bimp, imppresupuesto = imp WHERE idpresupuesto = NEW.idpresupuesto;
	RETURN NEW;
END;
' LANGUAGE plpgsql;

CREATE TRIGGER restriccionespedidoclientetrigger
    AFTER INSERT OR UPDATE ON lpresupuesto
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotpres();

CREATE TRIGGER restriccionespedidoclientetrigger1
    AFTER INSERT OR UPDATE ON dpresupuesto
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotpres();

SELECT drop_if_exists_proc ('actualizatotpresb','');
CREATE OR REPLACE FUNCTION actualizatotpresb() returns TRIGGER
AS '
DECLARE
	tot NUMERIC(12,2);
	bimp NUMERIC(12,2);
	imp NUMERIC(12,2);
BEGIN
	tot := calctotalpres(OLD.idpresupuesto);
	bimp := calcbimppres(OLD.idpresupuesto);
	imp := calcimpuestospres(OLD.idpresupuesto);
	UPDATE presupuesto SET totalpresupuesto = tot, bimppresupuesto = bimp, imppresupuesto = imp WHERE idpresupuesto = OLD.idpresupuesto;
	RETURN OLD;
END;
' LANGUAGE plpgsql;

CREATE TRIGGER restriccionespedidoclientetriggerd
    AFTER DELETE OR UPDATE ON lpresupuesto
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotpresb();

CREATE TRIGGER restriccionespedidoclientetriggerd1
    AFTER DELETE OR UPDATE ON dpresupuesto
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotpresb();


CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
BEGIN
	UPDATE lpresupuesto SET idlpresupuesto=idlpresupuesto;
	UPDATE dpresupuesto SET iddpresupuesto=iddpresupuesto;
	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;



\echo "Creamos la funcionalidad para guardar acumulados en los presupuestos"

-- Agregamos nuevos parametros de configuraci�.
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	as RECORD;
BEGIN
	SELECT INTO as * FROM configuracion WHERE nombre=''DatabaseRevision'';
	IF FOUND THEN
		UPDATE CONFIGURACION SET valor=''0.5.9'' WHERE nombre=''DatabaseRevision'';
	ELSE
		INSERT INTO configuracion (nombre, valor) VALUES (''DatabaseRevision'', ''0.5.9''); 		 
	END IF;
	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT actualizarevision();
DROP FUNCTION actualizarevision() CASCADE;
\echo "Actualizada la revision de la base de datos a la version 0.5.9"


DROP FUNCTION drop_if_exists_table(text) CASCADE;
DROP FUNCTION drop_if_exists_proc(text,text) CASCADE;


COMMIT;