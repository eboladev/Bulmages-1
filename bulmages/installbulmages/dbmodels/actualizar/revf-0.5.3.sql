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

-- ======================== COMPROBACION DE CUAL ES LA ULTIMA VERSION ==================================

CREATE OR REPLACE FUNCTION compruebarevision() RETURNS INTEGER AS '
DECLARE
	as RECORD;
BEGIN
	SELECT INTO as * FROM configuracion WHERE nombre=''DatabaseRevision'' AND ( valor LIKE ''0.5.3%'' OR valor = ''0.1.3'');
	IF FOUND THEN
		RETURN 0;
	ELSE
		RETURN -1;		 
	END IF;
END;
'   LANGUAGE plpgsql;
SELECT compruebarevision();
DROP FUNCTION compruebarevision() CASCADE;
\echo "Comprobada la revision"

-- ========================  FIN DE LA COMPROBACION ============================





CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
	as RECORD;
BEGIN
	IF (select count(*) from pg_tables where tablename=''inventario'') THEN
		RETURN 1;
	END IF;

	CREATE TABLE inventario (
		idinventario SERIAL PRIMARY KEY,
		nominventario varchar NOT NULL,
		fechainventario date default now()
	);
	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo "Agregamos la tabla de inventario."


CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
	as RECORD;
BEGIN
	IF (select count(*) from pg_tables where tablename=''controlstock'') THEN
		RETURN 1;
	END IF;

	-- stockantcontrolstock es un campo de solo lectura, es autorregulado internamente mediante triggers
	CREATE TABLE controlstock (
		idinventario integer NOT NULL REFERENCES inventario(idinventario),
		idalmacen  integer NOT NULL REFERENCES almacen(idalmacen),
		idarticulo   integer NOT NULL REFERENCES articulo(idarticulo),
		stockantcontrolstock numeric(12,2) DEFAULT 0,
		stocknewcontrolstock numeric(12,2) NOT NULL,
		punteocontrolstock boolean NOT NULL DEFAULT FALSE,
		PRIMARY KEY (idinventario, idalmacen, idarticulo)
	);

	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo "Agregamos la tabla de controlstock."





CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
	as RECORD;
	bs RECORD;
BEGIN
	IF (select count(*) from pg_tables where tablename=''stock_almacen'') THEN
		RETURN 1;
	END IF;

	-- Esta tabla es mantenida por el SGDB y sirve solo para hacer consultas.
	CREATE TABLE  stock_almacen (
		idarticulo integer NOT NULL REFERENCES articulo(idarticulo),
		idalmacen integer NOT NULL REFERENCES almacen(idalmacen),
		stock numeric(12,2) DEFAULT 0,
		PRIMARY KEY (idarticulo, idalmacen)
	);

	FOR as IN SELECT * FROM articulo LOOP
		FOR bs IN SELECT * FROM almacen LOOP
			INSERT INtO stock_almacen (idarticulo, idalmacen, stock) VALUES (as.idarticulo, bs.idalmacen,0);
		END LOOP;
	END LOOP;

	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo "Agregamos la tabla de stock_almacen"





SELECT drop_if_exists_proc ('narticulo','');
CREATE OR REPLACE FUNCTION narticulo () RETURNS "trigger"
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
\echo "Reemplazamos el trigger narticulot"


SELECT drop_if_exists_proc ('darticulo','');
CREATE OR REPLACE FUNCTION darticulo () RETURNS "trigger"
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
\echo "Reemplazamos el trigger darticulot"


SELECT drop_if_exists_proc ('nalmacen','');
CREATE OR REPLACE FUNCTION nalmacen () RETURNS "trigger"
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
\echo "Reemplazamos el trigger nalmacent"


SELECT drop_if_exists_proc ('dalmacen','');
CREATE OR REPLACE FUNCTION dalmacen () RETURNS "trigger"
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
\echo "Reemplazamos el trigger dalmacent"


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
\echo "Reemplazamos el trigger disminuyecontrolstockt"


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
\echo "Reemplazamos el trigger disminuyecontrolstockt1"


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
\echo "Reemplazamos el trigger disminuyecontrolstockt2"


SELECT drop_if_exists_proc ('aumentastock','');
CREATE OR REPLACE FUNCTION aumentastock () RETURNS "trigger"
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
\echo "Reemplazamos el trigger aumentastock"


SELECT drop_if_exists_proc ('disminuyestockp','');
CREATE OR REPLACE FUNCTION disminuyestockp () RETURNS "trigger"
AS '
DECLARE
BEGIN
	-- Hacemos el update del stock del articulo
	UPDATE articulo SET stockarticulo = stockarticulo - OLD.cantlalbaranp WHERE idarticulo= OLD.idarticulo;
	-- Hacemos el update del stock por almacenes
	UPDATE stock_almacen SET stock = stock - OLD.cantlalbaranp WHERE idarticulo = OLD.idarticulo AND idalmacen IN (SELECT idalmacen FROM albaranp WHERE idalbaranp=OLD.idalbaranp);
	RETURN OLD;
END;
' LANGUAGE plpgsql;

CREATE TRIGGER disminuyestockpt
    AFTER DELETE OR UPDATE ON lalbaranp
    FOR EACH ROW
    EXECUTE PROCEDURE disminuyestockp();
\echo "Reemplazamos el trigger disminuyestockp"


SELECT drop_if_exists_proc ('aumentastockp','');
CREATE OR REPLACE FUNCTION aumentastockp () RETURNS "trigger"
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
\echo "Reemplazamos el trigger aumentastockpt"


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
\echo "Reemplazamos el trigger modificastocktrigger"


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

CREATE TRIGGER modificastocktriggeralmacen
	AFTER UPDATE ON stock_almacen
	FOR EACH ROW
	EXECUTE PROCEDURE modificadostockalmacen();
\echo "Reemplazamos el trigger modificastocktriggeralmacen"

-- aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
SELECT drop_if_exists_proc ('disminuyecontrolstock','');
CREATE OR REPLACE FUNCTION disminuyecontrolstock () RETURNS "trigger"
AS '
DECLARE
BEGIN
	-- Hacemos el update del stock del articulo
	UPDATE articulo SET stockarticulo = stockarticulo - OLD.stockantcontrolstock + OLD.stocknewcontrolstock WHERE idarticulo= OLD.idarticulo;
	-- Hacemos el update del stock por almacenes
	UPDATE stock_almacen SET stock = stock - OLD.stockantcontrolstock + OLD.stocknewcontrolstock WHERE idarticulo = OLD.idarticulo AND idalmacen = OLD.idalmacen;
	RETURN OLD;
END;
' LANGUAGE plpgsql;

CREATE TRIGGER disminuyestockpt
    AFTER DELETE OR UPDATE ON controlstock
    FOR EACH ROW
    EXECUTE PROCEDURE disminuyecontrolstock();
\echo "Reemplazamos el trigger disminuyestockpt"



SELECT drop_if_exists_proc ('aumentacontrolstock','');
CREATE OR REPLACE FUNCTION aumentacontrolstock () RETURNS "trigger"
AS '
DECLARE
	rant RECORD;
BEGIN
	-- Cogemos el stock anterior.
	FOR  rant IN SELECT * FROM stock_almacen WHERE idarticulo = NEW.idarticulo AND idalmacen = NEW.idalmacen LOOP
		NEW.stockantcontrolstock := rant.stock;
	END LOOP;
	UPDATE articulo SET stockarticulo = stockarticulo + NEW.stocknewcontrolstock - NEW.stockantcontrolstock WHERE idarticulo = NEW.idarticulo;
	-- Hacemos el update del stock por almacenes
	UPDATE stock_almacen SET stock = NEW.stocknewcontrolstock WHERE idarticulo = NEW.idarticulo AND idalmacen = NEW.idalmacen;
	RETURN NEW;
END;
' LANGUAGE plpgsql;

CREATE TRIGGER aumentastockpt
    AFTER INSERT OR UPDATE ON controlstock
    FOR EACH ROW
    EXECUTE PROCEDURE aumentacontrolstock(); 
\echo "Reemplazamos el trigger aumentastockpt"


-- Cuando cambiamos el almacen en un albaran o albaranp hay problemas con el control de stock que se descuadra.
SELECT drop_if_exists_proc ('cambiaalbaran','');
CREATE OR REPLACE FUNCTION cambiaalbaran () RETURNS "trigger"
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
\echo "Reemplazamos el trigger cambiadoalbarant"


SELECT drop_if_exists_proc ('cambiaalbaranp','');
CREATE OR REPLACE FUNCTION cambiaalbaranp () RETURNS "trigger"
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
\echo "Reemplazamos el trigger cambiadoalbaranpt"


-- Agregamos nuevos parametros de configuraci�.
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	as RECORD;
BEGIN
	SELECT INTO as * FROM configuracion WHERE nombre=''DatabaseRevision'';
	IF FOUND THEN
		UPDATE CONFIGURACION SET valor=''0.5.3'' WHERE nombre=''DatabaseRevision'';
	ELSE
		INSERT INTO configuracion (nombre, valor) VALUES (''DatabaseRevision'', ''0.5.3''); 		 
	END IF;
	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT actualizarevision();
DROP FUNCTION actualizarevision() CASCADE;
\echo "Actualizada la revision de la base de datos a la versión 0.5.3"


DROP FUNCTION drop_if_exists_table(text) CASCADE;
DROP FUNCTION drop_if_exists_proc(text,text) CASCADE;


COMMIT;
