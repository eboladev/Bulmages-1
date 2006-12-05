--
-- Modificación de campos y funciones de la BD para la adaptaci� al tipo de datos monetario
--


-- Ocultamos las noticias
SET client_min_messages TO warning;


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
	SELECT INTO as * FROM configuracion WHERE nombre=''DatabaseRevision'' AND ( valor LIKE ''0.5.9%'' OR valor = ''0.5.3'');
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

	ALTER TABLE pedidocliente ALTER COLUMN fechapedidocliente SET DEFAULT now();
	ALTER TABLE albaran ALTER COLUMN fechaalbaran SET DEFAULT now();
	ALTER TABLE presupuesto ALTER COLUMN fpresupuesto SET DEFAULT now();
	ALTER TABLE factura ALTER COLUMN ffactura SET DEFAULT now();

	SELECT INTO as * FROM pg_attribute WHERE attname=''codcliente'';
	IF NOT FOUND THEN
		ALTER TABLE cliente ADD COLUMN codcliente character varying(12);
		ALTER TABLE cliente ADD CONSTRAINT ucodclient UNIQUE(codcliente);
	END IF;

	SELECT INTO as * FROM pg_attribute WHERE attname=''corpcliente'';
	IF NOT FOUND THEN
		ALTER TABLE cliente ADD COLUMN corpcliente character varying(200);
	END IF;


	SELECT INTO as * FROM pg_attribute WHERE attname=''teltrabcliente'';
	IF NOT FOUND THEN
		ALTER TABLE cliente ADD COLUMN teltrabcliente character varying(25);
		ALTER TABLE cliente ADD COLUMN movilcliente character varying(25);
	END IF;

	SELECT INTO as * FROM pg_attribute WHERE attname=''codproveedor'';
	IF NOT FOUND THEN
		ALTER TABLE proveedor ADD COLUMN codproveedor character varying(12);
		ALTER TABLE proveedor ADD CONSTRAINT ucodprov UNIQUE(codproveedor);
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


SELECT drop_if_exists_proc ('restriccioneslfactura','');
CREATE OR REPLACE FUNCTION restriccioneslfactura() RETURNS "trigger"
AS '
DECLARE
asd RECORD;
reg RECORD;
BEGIN
	IF NEW.idarticulo IS NULL THEN
	RAISE EXCEPTION ''ARTICULO INVALIDO'';
	return OLD;
	END IF;

	FOR asd IN SELECT * FROM articulo WHERE idarticulo=NEW.idarticulo LOOP
        	IF NEW.desclfactura IS NULL THEN
			NEW.desclfactura := asd.nomarticulo;
		END IF;
		IF NEW.cantlfactura IS NULL THEN
			NEW.cantlfactura := 1;
		END IF;
		IF NEW.descuentolfactura IS NULL THEN
			NEW.descuentolfactura := 0;
		END IF;
		IF NEW.pvplfactura IS NULL THEN
			SELECT INTO reg pvparticulo(NEW.idarticulo) AS precio;
			NEW.pvplfactura := reg.precio;
		END IF;
		IF NEW.ivalfactura IS NULL then
			SELECT INTO reg ivaarticulo(NEW.idarticulo) AS iva;
			NEW.ivalfactura := reg.iva;
		END IF;
	END LOOP;	
        RETURN NEW;
END;
'
    LANGUAGE plpgsql;

CREATE TRIGGER restriccionesalfacturatrigger
    BEFORE INSERT OR UPDATE ON lfactura
    FOR EACH ROW
    EXECUTE PROCEDURE restriccioneslfactura();

\echo "Creamos restricciones para linea de factura"



SELECT drop_if_exists_proc ('restriccionespedidocliente','');
CREATE FUNCTION restriccionespedidocliente () RETURNS "trigger"
AS '
DECLARE
asd RECORD;
BEGIN
	IF NEW.fechapedidocliente IS NULL THEN
		NEW.fechapedidocliente := now();
	END IF;
        IF NEW.numpedidocliente IS NULL THEN
                SELECT INTO asd max(numpedidocliente) AS m FROM pedidocliente;
		IF asd.m IS NOT NULL THEN
			NEW.numpedidocliente := asd.m + 1;
		ELSE
			NEW.numpedidocliente := 1;
		END IF;
        END IF;
	IF NEW.refpedidocliente IS NULL OR NEW.refpedidocliente = '''' THEN
		SELECT INTO asd crearef() AS m;
		IF FOUND THEN
			NEW.refpedidocliente := asd.m;
		END IF;
	END IF;
        RETURN NEW;
END;
' LANGUAGE plpgsql;


CREATE TRIGGER restriccionespedidoclientetrigger
    BEFORE INSERT OR UPDATE ON pedidocliente
    FOR EACH ROW
    EXECUTE PROCEDURE restriccionespedidocliente();

\echo "Creamos restricciones para pedidos a cliente"


SELECT drop_if_exists_proc ('restriccionespresupuesto','');
CREATE FUNCTION restriccionespresupuesto () RETURNS "trigger"
AS '
DECLARE
asd RECORD;
BEGIN
	IF NEW.fpresupuesto IS NULL THEN
		NEW.fpresupuesto := now();
	END IF;
        IF NEW.numpresupuesto IS NULL THEN
                SELECT INTO asd max(numpresupuesto) AS m FROM presupuesto;
		IF asd.m IS NOT NULL THEN	
			NEW.numpresupuesto := asd.m + 1;
		ELSE

			NEW.numpresupuesto := 1;
		END IF;			
        END IF;
	IF NEW.refpresupuesto IS NULL OR NEW.refpresupuesto = '''' THEN
		SELECT INTO asd crearef() AS m;
		IF FOUND THEN
			NEW.refpresupuesto := asd.m;
		END IF;
	END IF;
        RETURN NEW;
END;
' LANGUAGE plpgsql;


CREATE TRIGGER restriccionespresupuestotrigger
    BEFORE INSERT OR UPDATE ON presupuesto
    FOR EACH ROW
    EXECUTE PROCEDURE restriccionespresupuesto();

\echo "Creamos restricciones para presupuestos a cliente"

SELECT drop_if_exists_proc ('restriccionesalbaran','');
CREATE FUNCTION restriccionesalbaran () RETURNS "trigger"
AS '
DECLARE
asd RECORD;
BEGIN
	IF NEW.fechaalbaran IS NULL THEN
		NEW.fechaalbaran := now();
	END IF;
        IF NEW.numalbaran IS NULL THEN
                SELECT INTO asd max(numalbaran) AS m FROM albaran;
		IF asd.m IS NOT NULL THEN
			NEW.numalbaran := asd.m + 1;
		ELSE
			NEW.numalbaran := 1;
		END IF;
        END IF;
	IF NEW.refalbaran IS NULL OR NEW.refalbaran = '''' THEN
		SELECT INTO asd crearef() AS m;
		IF FOUND THEN
			NEW.refalbaran := asd.m;
		END IF;
	END IF;
        RETURN NEW;
END;
' LANGUAGE plpgsql;


CREATE TRIGGER restriccionesalbarantrigger
    BEFORE INSERT OR UPDATE ON albaran
    FOR EACH ROW
    EXECUTE PROCEDURE restriccionesalbaran();
\echo "Creamos restricciones para albaranes a cliente"


SELECT drop_if_exists_proc ('restriccionesfactura','');
CREATE FUNCTION restriccionesfactura () RETURNS "trigger"
AS '
DECLARE
asd RECORD;
BEGIN
	IF NEW.ffactura IS NULL THEN
		NEW.ffactura := now();
	END IF;
        IF NEW.numfactura IS NULL THEN
                SELECT INTO asd max(numfactura) AS m FROM factura WHERE idserie_factura=NEW.idserie_factura AND idalmacen = NEW.idalmacen;
		IF asd.m IS NOT NULL THEN
			NEW.numfactura := asd.m + 1;
		ELSE
			NEW.numfactura := 1;
		END IF;
        END IF;
	IF NEW.reffactura IS NULL OR NEW.reffactura = '''' THEN
		SELECT INTO asd crearef() AS m;
		IF FOUND THEN
			NEW.reffactura := asd.m;
		END IF;
	END IF;
        RETURN NEW;
END;
' LANGUAGE plpgsql;
CREATE TRIGGER restriccionesfacturatrigger
    BEFORE INSERT OR UPDATE ON factura
    FOR EACH ROW
    EXECUTE PROCEDURE restriccionesfactura();
\echo "Creamos restricciones para facturas a cliente"


SELECT drop_if_exists_proc ('restriccionespedidoproveedor','');
CREATE FUNCTION restriccionespedidoproveedor () RETURNS "trigger"
AS '
DECLARE
asd RECORD;
BEGIN
	IF NEW.fechapedidoproveedor IS NULL THEN
		NEW.fechapedidoproveedor := now();
	END IF;
        IF NEW.numpedidoproveedor IS NULL THEN
                SELECT INTO asd max(numpedidoproveedor) AS m FROM pedidoproveedor;
		IF asd.m IS NOT NULL THEN
			NEW.numpedidoproveedor := asd.m + 1;
		ELSE
			NEW.numpedidoproveedor := 1;
		END IF;
        END IF;
	IF NEW.refpedidoproveedor IS NULL OR NEW.refpedidoproveedor = '''' THEN
		SELECT INTO asd crearef() AS m;
		IF FOUND THEN
			NEW.refpedidoproveedor := asd.m;
		END IF;
	END IF;
        RETURN NEW;
END;
' LANGUAGE plpgsql;
CREATE TRIGGER restriccionespedidoproveedortrigger
    BEFORE INSERT OR UPDATE ON pedidoproveedor
    FOR EACH ROW
    EXECUTE PROCEDURE restriccionespedidoproveedor();
\echo "Creamos restricciones para pedidos a proveedores"



SELECT drop_if_exists_proc ('restriccionesalbaranp','');
CREATE FUNCTION restriccionesalbaranp () RETURNS "trigger"
AS '
DECLARE
asd RECORD;
BEGIN
	IF NEW.fechaalbaranp IS NULL THEN
		NEW.fechaalbaranp := now();
	END IF;
        IF NEW.numalbaranp IS NULL THEN
                SELECT INTO asd max(numalbaranp) AS m FROM albaranp;
		IF asd.m IS NOT NULL THEN
			NEW.numalbaranp := asd.m + 1;
		ELSE
			NEW.numalbaranp := 1;
		END IF;
        END IF;
	IF NEW.refalbaranp IS NULL OR NEW.refalbaranp = '''' THEN
		SELECT INTO asd crearef() AS m;
		IF FOUND THEN
			NEW.refalbaranp := asd.m;
		END IF;
	END IF;
        RETURN NEW;
END;
' LANGUAGE plpgsql;
CREATE TRIGGER restriccionesalbaranptrigger
    BEFORE INSERT OR UPDATE ON albaranp
    FOR EACH ROW
    EXECUTE PROCEDURE restriccionesalbaranp();
\echo "Creamos restricciones para albaranes a proveedores"


SELECT drop_if_exists_proc ('restriccionesfacturap','');
CREATE FUNCTION restriccionesfacturap () RETURNS "trigger"
AS '
DECLARE
asd RECORD;
BEGIN
	IF NEW.ffacturap IS NULL THEN
		NEW.ffacturap := now();
	END IF;
	IF NEW.reffacturap IS NULL OR NEW.reffacturap = '''' THEN
		SELECT INTO asd crearef() AS m;
		IF FOUND THEN
			NEW.reffacturap := asd.m;
		END IF;
	END IF;
        RETURN NEW;
END;
' LANGUAGE plpgsql;
CREATE TRIGGER restriccionesfacturaptrigger
    BEFORE INSERT OR UPDATE ON facturap
    FOR EACH ROW
    EXECUTE PROCEDURE restriccionesfacturap();
\echo "Creamos restricciones para facturas a proveedores"


-- ================================== MODULO DE TARIFAS ================================
-- =====================================================================================

SELECT drop_if_exists_proc ('restriccionestarifa','');
CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
	as RECORD;
BEGIN
	SELECT INTO as * FROM pg_tables  WHERE tablename=''tipo_tarifa'';
	IF FOUND THEN
		DROP TABLE tipo_tarifa CASCADE;
		DROP TABLE tarifa CASCADE;
	END IF;

	SELECT INTO as * FROM pg_tables  WHERE tablename=''tarifa'';
	IF NOT FOUND THEN
		CREATE TABLE tarifa (
			idtarifa serial PRIMARY KEY,
			nomtarifa varchar(60),
			finiciotarifa date,
			ffintarifa date
		);
		CREATE TABLE ltarifa (
			idltarifa serial PRIMARY KEY,
			idalmacen integer NOT NULL REFERENCES almacen(idalmacen),
			idarticulo integer NOT NULL REFERENCES articulo(idarticulo),
			idtarifa integer NOT NULL REFERENCES tarifa(idtarifa),
			pvpltarifa numeric(13, 4)
		);

		CREATE UNIQUE INDEX indice_ltarifa ON ltarifa (idalmacen, idarticulo, idtarifa);

		ALTER TABLE cliente ADD COLUMN idtarifa integer;
		ALTER TABLE cliente ADD CONSTRAINT idtarifafk FOREIGN KEY (idtarifa) REFERENCES tarifa(idtarifa);

	END IF;
	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;



CREATE OR REPLACE FUNCTION pvparticuloclial(integer, integer, integer) RETURNS numeric(12,2)
AS'
DECLARE
	idarticulo ALIAS FOR $1;
	idclient ALIAS FOR $2;
	idalmacen ALIAS FOR $3;
	as RECORD;
BEGIN
	SELECT INTO AS pvpltarifa FROM ltarifa WHERE ltarifa.idarticulo = idarticulo AND ltarifa.idalmacen = idalmacen AND idtarifa IN (SELECT idtarifa FROM cliente WHERE idcliente=idclient);
	IF FOUND THEN
		RETURN as.pvpltarifa;
	END IF;


	SELECT INTO AS pvparticulo FROM  articulo WHERE articulo.idarticulo = idarticulo;
	IF FOUND THEN
		RETURN as.pvparticulo;
	END IF;
	RETURN 0.0;
END;
' LANGUAGE plpgsql;
\echo "Aderezamos las tablas de tarifas."


-- ============= METEMOS LOS TOTALES EN PEDIDOS A CLIENTE  =============================
-- =====================================================================================

CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
	as RECORD;
BEGIN
	SELECT INTO as * FROM pg_attribute  WHERE attname=''totalpedidocliente'';
	IF NOT FOUND THEN
		ALTER TABLE pedidocliente ADD COLUMN totalpedidocliente NUMERIC(12,2);
		ALTER TABLE pedidocliente ADD COLUMN bimppedidocliente NUMERIC(12,2);
		ALTER TABLE pedidocliente ADD COLUMN imppedidocliente NUMERIC(12,2);
		ALTER TABLE pedidocliente ALTER COLUMN totalpedidocliente SET DEFAULT 0;
		ALTER TABLE pedidocliente ALTER COLUMN bimppedidocliente SET DEFAULT 0;
		ALTER TABLE pedidocliente ALTER COLUMN imppedidocliente SET DEFAULT 0;
	END IF;
	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo "Agregamos los campos de totales para los pedidos a cliente"

SELECT drop_if_exists_proc ('actualizatotpedcli','');
CREATE OR REPLACE FUNCTION actualizatotpedcli() returns TRIGGER
AS '
DECLARE
	tot NUMERIC(12,2);
	bimp NUMERIC(12,2);
	imp NUMERIC(12,2);
BEGIN
	tot := calctotalpedcli(NEW.idpedidocliente);
	bimp := calcbimppedcli(NEW.idpedidocliente);
	imp := calcimpuestospedcli(NEW.idpedidocliente);
	UPDATE pedidocliente SET totalpedidocliente = tot, bimppedidocliente = bimp, imppedidocliente = imp WHERE idpedidocliente = NEW.idpedidocliente;
	RETURN NEW;
END;
' LANGUAGE plpgsql;

CREATE TRIGGER totalesautomaticospedidoclientetrigger
    AFTER INSERT OR UPDATE ON lpedidocliente
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotpedcli();

CREATE TRIGGER totalesautomaticospedidoclientetrigger1
    AFTER INSERT OR UPDATE ON dpedidocliente
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotpedcli();

SELECT drop_if_exists_proc ('actualizatotpedclib','');
CREATE OR REPLACE FUNCTION actualizatotpedclib() returns TRIGGER
AS '
DECLARE
	tot NUMERIC(12,2);
	bimp NUMERIC(12,2);
	imp NUMERIC(12,2);
BEGIN
	tot := calctotalpedcli(OLD.idpedidocliente);
	bimp := calcbimppedcli(OLD.idpedidocliente);
	imp := calcimpuestospedcli(OLD.idpedidocliente);
	UPDATE pedidocliente SET totalpedidocliente = tot, bimppedidocliente = bimp, imppedidocliente = imp WHERE idpedidocliente = OLD.idpedidocliente;
	RETURN OLD;
END;
' LANGUAGE plpgsql;

CREATE TRIGGER totalesautomaticospedidoclientetriggerd
    AFTER DELETE OR UPDATE ON lpedidocliente
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotpedclib();

CREATE TRIGGER totalesautomaticospedidoclientetriggerd1
    AFTER DELETE OR UPDATE ON dpedidocliente
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotpedclib();


-- ============= METEMOS LOS TOTALES EN ALBARANES A CLIENTE  ===========================
-- =====================================================================================

CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
	as RECORD;
BEGIN
	SELECT INTO as * FROM pg_attribute  WHERE attname=''totalalbaran'';
	IF NOT FOUND THEN
		ALTER TABLE albaran ADD COLUMN totalalbaran NUMERIC(12,2);
		ALTER TABLE albaran ADD COLUMN bimpalbaran NUMERIC(12,2);
		ALTER TABLE albaran ADD COLUMN impalbaran NUMERIC(12,2);
		ALTER TABLE albaran ALTER COLUMN totalalbaran SET DEFAULT 0;
		ALTER TABLE albaran ALTER COLUMN bimpalbaran SET DEFAULT 0;
		ALTER TABLE albaran ALTER COLUMN impalbaran SET DEFAULT 0;
	END IF;
	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo "Agregamos los campos de totales para los albaranes"

SELECT drop_if_exists_proc ('actualizatotalbaran','');
CREATE OR REPLACE FUNCTION actualizatotalbaran() returns TRIGGER
AS '
DECLARE
	tot NUMERIC(12,2);
	bimp NUMERIC(12,2);
	imp NUMERIC(12,2);
BEGIN
	tot := calctotalalbaran(NEW.idalbaran);
	bimp := calcbimpalbaran(NEW.idalbaran);
	imp := calcimpuestosalbaran(NEW.idalbaran);
	UPDATE albaran SET totalalbaran = tot, bimpalbaran = bimp, impalbaran = imp WHERE idalbaran = NEW.idalbaran;
	RETURN NEW;
END;
' LANGUAGE plpgsql;

CREATE TRIGGER totalesautomaticosalbarantrigger
    AFTER INSERT OR UPDATE ON lalbaran
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotalbaran();

CREATE TRIGGER totalesautomaticosalbarantrigger1
    AFTER INSERT OR UPDATE ON dalbaran
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotalbaran();

SELECT drop_if_exists_proc ('actualizatotalbaranb','');
CREATE OR REPLACE FUNCTION actualizatotalbaranb() returns TRIGGER
AS '
DECLARE
	tot NUMERIC(12,2);
	bimp NUMERIC(12,2);
	imp NUMERIC(12,2);
BEGIN
	tot := calctotalalbaran(OLD.idalbaran);
	bimp := calcbimpalbaran(OLD.idalbaran);
	imp := calcimpuestosalbaran(OLD.idalbaran);
	UPDATE albaran SET totalalbaran = tot, bimpalbaran = bimp, impalbaran = imp WHERE idalbaran = OLD.idalbaran;
	RETURN OLD;
END;
' LANGUAGE plpgsql;

CREATE TRIGGER totalesautomaticosalbarantriggerd
    AFTER DELETE OR UPDATE ON lalbaran
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotalbaranb();

CREATE TRIGGER totalesautomaticosalbarantriggerd1
    AFTER DELETE OR UPDATE ON dalbaran
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotalbaranb();


-- ============= METEMOS LOS TOTALES EN FACTURAS A CLIENTE  ===========================
-- =====================================================================================

CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
	as RECORD;
BEGIN
	SELECT INTO as * FROM pg_attribute  WHERE attname=''totalfactura'';
	IF NOT FOUND THEN
		ALTER TABLE factura ADD COLUMN totalfactura NUMERIC(12,2);
		ALTER TABLE factura ADD COLUMN bimpfactura NUMERIC(12,2);
		ALTER TABLE factura ADD COLUMN impfactura NUMERIC(12,2);
		ALTER TABLE factura ALTER COLUMN totalfactura SET DEFAULT 0;
		ALTER TABLE factura ALTER COLUMN bimpfactura SET DEFAULT 0;
		ALTER TABLE factura ALTER COLUMN impfactura SET DEFAULT 0;
	END IF;
	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo "Agregamos los campos de totales para las facturas a cliente"

SELECT drop_if_exists_proc ('actualizatotfactura','');
CREATE OR REPLACE FUNCTION actualizatotfactura() returns TRIGGER
AS '
DECLARE
	tot NUMERIC(12,2);
	bimp NUMERIC(12,2);
	imp NUMERIC(12,2);
BEGIN
	tot := calctotalfactura(NEW.idfactura);
	bimp := calcbimpfactura(NEW.idfactura);
	imp := calcimpuestosfactura(NEW.idfactura);
	UPDATE factura SET totalfactura = tot, bimpfactura = bimp, impfactura = imp WHERE idfactura = NEW.idfactura;
	RETURN NEW;
END;
' LANGUAGE plpgsql;

CREATE TRIGGER totalesautomaticosfacturatrigger
    AFTER INSERT OR UPDATE ON lfactura
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotfactura();

CREATE TRIGGER totalesautomaticosfacturatrigger1
    AFTER INSERT OR UPDATE ON dfactura
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotfactura();

SELECT drop_if_exists_proc ('actualizatotfacturab','');
CREATE OR REPLACE FUNCTION actualizatotfacturab() returns TRIGGER
AS '
DECLARE
	tot NUMERIC(12,2);
	bimp NUMERIC(12,2);
	imp NUMERIC(12,2);
BEGIN
	tot := calctotalfactura(OLD.idfactura);
	bimp := calcbimpfactura(OLD.idfactura);
	imp := calcimpuestosfactura(OLD.idfactura);
	UPDATE factura SET totalfactura = tot, bimpfactura = bimp, impfactura = imp WHERE idfactura = OLD.idfactura;
	RETURN OLD;
END;
' LANGUAGE plpgsql;

CREATE TRIGGER totalesautomaticosfacturatriggerd
    AFTER DELETE OR UPDATE ON lfactura
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotfacturab();

CREATE TRIGGER totalesautomaticosfacturatriggerd1
    AFTER DELETE OR UPDATE ON dfactura
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotfacturab();

-- ============= METEMOS LOS TOTALES EN PEDIDOS A PROVEEDOR  ===========================
-- =====================================================================================

CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
	as RECORD;
BEGIN
	SELECT INTO as * FROM pg_attribute  WHERE attname=''totalpedidoproveedor'';
	IF NOT FOUND THEN
		ALTER TABLE pedidoproveedor ADD COLUMN totalpedidoproveedor NUMERIC(12,2);
		ALTER TABLE pedidoproveedor ADD COLUMN bimppedidoproveedor NUMERIC(12,2);
		ALTER TABLE pedidoproveedor ADD COLUMN imppedidoproveedor NUMERIC(12,2);
		ALTER TABLE pedidoproveedor ALTER COLUMN totalpedidoproveedor SET DEFAULT 0;
		ALTER TABLE pedidoproveedor ALTER COLUMN bimppedidoproveedor SET DEFAULT 0;
		ALTER TABLE pedidoproveedor ALTER COLUMN imppedidoproveedor SET DEFAULT 0;
	END IF;
	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo "Agregamos los campos de totales para los pedidos a proveedor"

SELECT drop_if_exists_proc ('actualizatotpedidoproveedor','');
CREATE OR REPLACE FUNCTION actualizatotpedidoproveedor() returns TRIGGER
AS '
DECLARE
	tot NUMERIC(12,2);
	bimp NUMERIC(12,2);
	imp NUMERIC(12,2);
BEGIN
	tot := calctotalpedpro(NEW.idpedidoproveedor);
	bimp := calcbimppedpro(NEW.idpedidoproveedor);
	imp := calcimpuestospedpro(NEW.idpedidoproveedor);
	UPDATE pedidoproveedor SET totalpedidoproveedor = tot, bimppedidoproveedor = bimp, imppedidoproveedor = imp WHERE idpedidoproveedor = NEW.idpedidoproveedor;
	RETURN NEW;
END;
' LANGUAGE plpgsql;

CREATE TRIGGER totalesautomaticospedidoproveedortrigger
    AFTER INSERT OR UPDATE ON lpedidoproveedor
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotpedidoproveedor();

CREATE TRIGGER totalesautomaticospedidoproveedortrigger1
    AFTER INSERT OR UPDATE ON dpedidoproveedor
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotpedidoproveedor();

SELECT drop_if_exists_proc ('actualizatotpedidoproveedorb','');
CREATE OR REPLACE FUNCTION actualizatotpedidoproveedorb() returns TRIGGER
AS '
DECLARE
	tot NUMERIC(12,2);
	bimp NUMERIC(12,2);
	imp NUMERIC(12,2);
BEGIN
	tot := calctotalpedpro(OLD.idpedidoproveedor);
	bimp := calcbimppedpro(OLD.idpedidoproveedor);
	imp := calcimpuestospedpro(OLD.idpedidoproveedor);
	UPDATE pedidoproveedor SET totalpedidoproveedor = tot, bimppedidoproveedor = bimp, imppedidoproveedor = imp WHERE idpedidoproveedor = OLD.idpedidoproveedor;
	RETURN OLD;
END;
' LANGUAGE plpgsql;

CREATE TRIGGER totalesautomaticospedidoproveedortriggerd
    AFTER DELETE OR UPDATE ON lpedidoproveedor
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotpedidoproveedorb();

CREATE TRIGGER totalesautomaticospedidoproveedortriggerd1
    AFTER DELETE OR UPDATE ON dpedidoproveedor
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotpedidoproveedorb();



-- ============= METEMOS LOS TOTALES EN ALBARANES A PROVEEDOR  =========================
-- =====================================================================================

CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
	as RECORD;
BEGIN
	SELECT INTO as * FROM pg_attribute  WHERE attname=''totalalbaranp'';
	IF NOT FOUND THEN
		ALTER TABLE albaranp ADD COLUMN totalalbaranp NUMERIC(12,2);
		ALTER TABLE albaranp ADD COLUMN bimpalbaranp NUMERIC(12,2);
		ALTER TABLE albaranp ADD COLUMN impalbaranp NUMERIC(12,2);
		ALTER TABLE albaranp ALTER COLUMN totalalbaranp SET DEFAULT 0;
		ALTER TABLE albaranp ALTER COLUMN bimpalbaranp SET DEFAULT 0;
		ALTER TABLE albaranp ALTER COLUMN impalbaranp SET DEFAULT 0;
	END IF;
	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo "Agregamos los campos de totales para los albaranes a proveedor"

SELECT drop_if_exists_proc ('actualizatotalbaranp','');
CREATE OR REPLACE FUNCTION actualizatotalbaranp() returns TRIGGER
AS '
DECLARE
	tot NUMERIC(12,2);
	bimp NUMERIC(12,2);
	imp NUMERIC(12,2);
BEGIN
	tot := calctotalalbpro(NEW.idalbaranp);
	bimp := calcbimpalbpro(NEW.idalbaranp);
	imp := calcimpuestosalbpro(NEW.idalbaranp);
	UPDATE albaranp SET totalalbaranp = tot, bimpalbaranp = bimp, impalbaranp = imp WHERE idalbaranp = NEW.idalbaranp;
	RETURN NEW;
END;
' LANGUAGE plpgsql;

CREATE TRIGGER totalesautomaticosalbaranptrigger
    AFTER INSERT OR UPDATE ON lalbaranp
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotalbaranp();

CREATE TRIGGER totalesautomaticosalbaranptrigger1
    AFTER INSERT OR UPDATE ON dalbaranp
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotalbaranp();

SELECT drop_if_exists_proc ('actualizatotalbaranpb','');
CREATE OR REPLACE FUNCTION actualizatotalbaranpb() returns TRIGGER
AS '
DECLARE
	tot NUMERIC(12,2);
	bimp NUMERIC(12,2);
	imp NUMERIC(12,2);
BEGIN
	tot := calctotalalbpro(OLD.idalbaranp);
	bimp := calcbimpalbpro(OLD.idalbaranp);
	imp := calcimpuestosalbpro(OLD.idalbaranp);
	UPDATE albaranp SET totalalbaranp = tot, bimpalbaranp = bimp, impalbaranp = imp WHERE idalbaranp = OLD.idalbaranp;
	RETURN OLD;
END;
' LANGUAGE plpgsql;

CREATE TRIGGER totalesautomaticosalbaranptriggerd
    AFTER DELETE OR UPDATE ON lalbaranp
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotalbaranpb();

CREATE TRIGGER totalesautomaticosalbaranptriggerd1
    AFTER DELETE OR UPDATE ON dalbaranp
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotalbaranpb();



-- ============= METEMOS LOS TOTALES EN FACTURAS A PROVEEDOR  ==========================
-- =====================================================================================

CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
	as RECORD;
BEGIN
	SELECT INTO as * FROM pg_attribute  WHERE attname=''totalfacturap'';
	IF NOT FOUND THEN
		ALTER TABLE facturap ADD COLUMN totalfacturap NUMERIC(12,2);
		ALTER TABLE facturap ADD COLUMN bimpfacturap NUMERIC(12,2);
		ALTER TABLE facturap ADD COLUMN impfacturap NUMERIC(12,2);
		ALTER TABLE facturap ALTER COLUMN totalfacturap SET DEFAULT 0;
		ALTER TABLE facturap ALTER COLUMN bimpfacturap SET DEFAULT 0;
		ALTER TABLE facturap ALTER COLUMN impfacturap SET DEFAULT 0;
	END IF;
	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo "Agregamos los campos de totales para las facturas a proveedor"

SELECT drop_if_exists_proc ('actualizatotfacturap','');
CREATE OR REPLACE FUNCTION actualizatotfacturap() returns TRIGGER
AS '
DECLARE
	tot NUMERIC(12,2);
	bimp NUMERIC(12,2);
	imp NUMERIC(12,2);
BEGIN
	tot := calctotalfacpro(NEW.idfacturap);
	bimp := calcbimpfacpro(NEW.idfacturap);
	imp := calcimpuestosfacpro(NEW.idfacturap);
	UPDATE facturap SET totalfacturap = tot, bimpfacturap = bimp, impfacturap = imp WHERE idfacturap = NEW.idfacturap;
	RETURN NEW;
END;
' LANGUAGE plpgsql;

CREATE TRIGGER totalesautomaticosfacturaptrigger
    AFTER INSERT OR UPDATE ON lfacturap
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotfacturap();

CREATE TRIGGER totalesautomaticosfacturaptrigger1
    AFTER INSERT OR UPDATE ON dfacturap
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotfacturap();

SELECT drop_if_exists_proc ('actualizatotfacturapb','');
CREATE OR REPLACE FUNCTION actualizatotfacturapb() returns TRIGGER
AS '
DECLARE
	tot NUMERIC(12,2);
	bimp NUMERIC(12,2);
	imp NUMERIC(12,2);
BEGIN
	tot := calctotalfacpro(OLD.idfacturap);
	bimp := calcbimpfacpro(OLD.idfacturap);
	imp := calcimpuestosfacpro(OLD.idfacturap);
	UPDATE facturap SET totalfacturap = tot, bimpfacturap = bimp, impfacturap = imp WHERE idfacturap = OLD.idfacturap;
	RETURN OLD;
END;
' LANGUAGE plpgsql;

CREATE TRIGGER totalesautomaticosfacturaptriggerd
    AFTER DELETE OR UPDATE ON lfacturap
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotfacturapb();

CREATE TRIGGER totalesautomaticosfacturaptriggerd1
    AFTER DELETE OR UPDATE ON dfacturap
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotfacturapb();


-- ================================== DISPARAMOS TRIGGERS  =============================
-- =====================================================================================


CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
BEGIN
	UPDATE lpresupuesto SET cantlpresupuesto = cantlpresupuesto;
	UPDATE lpedidocliente SET cantlpedidocliente = cantlpedidocliente;
	UPDATE lalbaran SET cantlalbaran = cantlalbaran;
	UPDATE lfactura SET cantlfactura = cantlfactura;
	UPDATE lpedidoproveedor SET cantlpedidoproveedor = cantlpedidoproveedor;
	UPDATE lalbaranp SET cantlalbaranp = cantlalbaranp;
	UPDATE lfacturap SET cantlfacturap = cantlfacturap;
	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo "Disparamos los triggers para que se actualicen los totales."

--
-- Ponemos valores necesarios en la tabla configuracion para hacer uso del plugin efacturabf.
--

CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
	as RECORD;
BEGIN
	SELECT INTO as * FROM configuracion WHERE nombre=''DireccionCompleta'';
	IF NOT FOUND THEN
		INSERT INTO configuracion (nombre, valor) VALUES (''DireccionCompleta'', ''*CAMBIAME*''); 		 
		INSERT INTO configuracion (nombre, valor) VALUES (''Ciudad'', ''*CAMBIAME*''); 		 
		INSERT INTO configuracion (nombre, valor) VALUES (''CodArticuloGenerico'', ''*CAMBIAME*'');
	END IF;
	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;

--
-- Correccion de bug en el schema de la BD. En la tabla dfacturap, la clave extranjera a facturap
-- apuntaba a la tabla factura.
--

CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
BEGIN
	ALTER TABLE dfacturap DROP CONSTRAINT dfacturap_idfacturap_fkey;
	ALTER TABLE dfacturap ADD FOREIGN KEY(idfacturap) REFERENCES facturap(idfacturap);
	
	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;





-- ================================== ACTUALIZACION  ===================================
-- =====================================================================================

-- Agregamos nuevos parametros de configuraci�.
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	as RECORD;
BEGIN
	SELECT INTO as * FROM configuracion WHERE nombre=''DatabaseRevision'';
	IF FOUND THEN
		UPDATE CONFIGURACION SET valor=''0.5.9-0004'' WHERE nombre=''DatabaseRevision'';
	ELSE
		INSERT INTO configuracion (nombre, valor) VALUES (''DatabaseRevision'', ''0.5.9-0004''); 		 
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