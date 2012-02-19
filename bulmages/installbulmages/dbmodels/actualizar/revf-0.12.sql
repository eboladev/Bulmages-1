--
-- ACTUALIZACION DE LA BASE DE DATOS DE BULMAFACT
--


-- Ocultamos las noticias
SET client_min_messages TO warning;


BEGIN;

--
-- Estas primeras funciones cambiaran los tipos de columnas que estan como flotantes a NUMERIC.
-- Se trata de un parche que se desea aplicar para almacenar los tipos monetarios
-- ya que actualmente se encuantran almacenados como 'doubles' y es preferible
-- que se almacenen como tipo 'numeric'.
-- Todas devuelven como valor numerico el nmero de filas influenciadas por el cambio
-- NOTA: Si alguien sabe como pasar por parametro un nombre de tabla y campo a modificarse
-- hara mucho mas sencillito porque solo habra que implementar un funcion ya que siempre
-- hay que hacer lo mismo.
--

--
-- Funcion auxiliar para borrar funciones limpiamente
--
CREATE OR REPLACE FUNCTION drop_if_exists_table (text) RETURNS INTEGER AS '
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


CREATE OR REPLACE FUNCTION drop_if_exists_proc (text, text) RETURNS INTEGER AS '
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
	rs RECORD;
BEGIN
	SELECT INTO rs * FROM configuracion WHERE nombre=''DatabaseRevision'' AND ( valor LIKE ''0.12.1%'' OR valor = ''0.11.1-0004'');
	IF FOUND THEN
		RETURN 0;
	ELSE
		RAISE EXCEPTION '' Version de la base de datos invalida, aplique parches anteriores'';
		RETURN -1;
	END IF;
END;
'   LANGUAGE plpgsql;
SELECT compruebarevision();
DROP FUNCTION compruebarevision() CASCADE;
\echo "Comprobada la revision"

-- ========================  FIN DE LA COMPROBACION ============================


-- ================================== ACTUALIZACION  ===================================

\echo "::Funciones auxiliares ... BgValidDate"
-- Esta Funcion devuelve una fecha si la fecha es correcta o NULL si esta no es correcta.
-- Pensada para usar en conjuncion con COALESCE y no generar un error de SQL
CREATE OR REPLACE FUNCTION BgValidDate(CHAR) RETURNS date AS $$
 DECLARE
     result DATE;
      res BOOL;
     validFormat TEXT := 'DD/MM/YYYY';
 BEGIN
    result := NULL;
     SELECT TO_CHAR(TO_DATE($1,validFormat),validFormat) = $1
     INTO res;
     if (res = TRUE) THEN
	RETURN $1::DATE;
     END IF;
     RETURN result;
 END;
 $$ LANGUAGE plpgsql;


SELECT drop_if_exists_proc('disminuyestockp', '');
SELECT drop_if_exists_proc('aumentastockp', '');

SELECT drop_if_exists_proc('cambiastockp', '');
\echo -n ':: Funcion cambia stock por la parte de albaranes de proveedor ... '
CREATE FUNCTION cambiastockp() RETURNS "trigger"
AS '
DECLARE

BEGIN
    -- Hacemos el update del stock del articulo
    IF TG_OP = ''DELETE'' OR TG_OP = ''UPDATE'' THEN
      UPDATE articulo SET stockarticulo = stockarticulo - OLD.cantlalbaranp WHERE idarticulo= OLD.idarticulo;
    END IF;
    IF TG_OP = ''INSERT'' OR TG_OP = ''UPDATE'' THEN
      UPDATE articulo SET stockarticulo = stockarticulo + NEW.cantlalbaranp WHERE idarticulo= NEW.idarticulo;
    END IF;
    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador que disminuye stock al borrar o actualizar el detalle de un albaran de proveedor ... '
CREATE TRIGGER cambiastockpt
    AFTER INSERT OR DELETE OR UPDATE ON lalbaranp
    FOR EACH ROW
    EXECUTE PROCEDURE cambiastockp();



SELECT drop_if_exists_proc('disminuyestock', '');
SELECT drop_if_exists_proc('aumentastock', '');


SELECT drop_if_exists_proc('cambiastock', '');
\echo -n ':: Funcion cambia stock por la parte de albaranes de cliente ... '
CREATE FUNCTION cambiastock() RETURNS "trigger"
AS '
DECLARE

BEGIN
    -- Hacemos el update del stock del articulo
    IF TG_OP = ''DELETE'' OR TG_OP = ''UPDATE'' THEN
      UPDATE articulo SET stockarticulo = stockarticulo + OLD.cantlalbaran WHERE idarticulo= OLD.idarticulo;
    END IF;
    IF TG_OP = ''INSERT'' OR TG_OP = ''UPDATE'' THEN
      UPDATE articulo SET stockarticulo = stockarticulo - NEW.cantlalbaran WHERE idarticulo= NEW.idarticulo;
    END IF;
    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador que disminuye stock al borrar o actualizar el detalle de un albaran de proveedor ... '
CREATE TRIGGER cambiastockt
    AFTER INSERT OR DELETE OR UPDATE ON lalbaran
    FOR EACH ROW
    EXECUTE PROCEDURE cambiastock();




-- para poder especificar la forma de pago en los cobros
\echo -n ':: Agrega el campo idforma_pago a la tabla de cobros '
CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
        rs RECORD;
        cs RECORD;
        ds RECORD;
        es RECORD;
BEGIN

   --SELECT INTO rs attname, relname FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid=pg_class.oid WHERE attname=''cobro'' AND relname=''idforma_pago'';
   SELECT INTO rs attname FROM pg_attribute WHERE attrelid = ''cobro''::regclass AND attname = ''idforma_pago'' AND attisdropped IS FALSE AND attnum >= 1 ORDER BY attnum;
   IF NOT FOUND THEN
           ALTER TABLE cobro ADD COLUMN idforma_pago integer REFERENCES forma_pago(idforma_pago) ;
-- para compatibilidad con las bd de la branch docsMonolitic que usaban idbanco en lugar de 
-- sufijo
--           UPDATE banco set sufijobanco = idbanco ;
   END IF;

   FOR rs IN SELECT * FROM banco LOOP
   
      SELECT INTO es idbanco FROM forma_pago WHERE idbanco = rs.idbanco;
      IF NOT FOUND THEN
   
          INSERT INTO forma_pago (descforma_pago, idbanco) VALUES (rs.nombanco, rs.idbanco);
	  SELECT INTO cs MAX(idforma_pago) AS idf FROM forma_pago LIMIT 1;
      
          SELECT INTO ds idbanco FROM cobro WHERE idcobro = rs.idbanco;
          IF FOUND THEN
		  UPDATE cobro set idforma_pago = cs.idf WHERE idbanco = rs.idbanco;
	  END IF;

      END IF;

   END LOOP;

   RETURN 0;
END;
' LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;




\echo -n ':: Funcion que propaga el codigo completo de la familia ... '
SELECT drop_if_exists_proc ('propagacodigocompletofamilia','');        
CREATE FUNCTION propagacodigocompletofamilia() RETURNS "trigger"
AS '
DECLARE

BEGIN
    UPDATE articulo SET codigocompletoarticulo = codigocompletoarticulo WHERE articulo.idfamilia = NEW.idfamilia;
    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador que propaga el codigo completo de la familia ... '
CREATE TRIGGER propagacodigocompletofamiliatrigger
    AFTER UPDATE ON familia
    FOR EACH ROW
    EXECUTE PROCEDURE propagacodigocompletofamilia();


\echo -n ':: Funcion que cambia el tipo de dato en documentos de proveedor ... '





CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
        rs RECORD;
BEGIN
	SELECT INTO rs * FROM pg_attribute WHERE attname=''numpedidoproveedor'';
	IF FOUND THEN
		ALTER TABLE pedidoproveedor ALTER COLUMN numpedidoproveedor TYPE CHARACTER VARYING(20);
	END IF;
	IF FOUND THEN
		ALTER TABLE albaranp ALTER COLUMN numalbaranp TYPE CHARACTER VARYING(20);
	END IF;
	RETURN 0;
END;
' LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;


\echo -n ':: Funcion con restricciones en pedidos a proveedor ... '
CREATE OR REPLACE FUNCTION restriccionespedidoproveedor() RETURNS "trigger"
AS '
DECLARE
    rs RECORD;

BEGIN
    IF NEW.fechapedidoproveedor IS NULL THEN
	NEW.fechapedidoproveedor := now();
    END IF;

    IF NEW.numpedidoproveedor IS NULL THEN
	NEW.numpedidoproveedor := NEW.idpedidoproveedor;
    END IF;

    IF NEW.refpedidoproveedor IS NULL OR NEW.refpedidoproveedor = '''' THEN
	SELECT INTO rs crearef() AS m;
	IF FOUND THEN
	    NEW.refpedidoproveedor := rs.m;
	END IF;
    END IF;

    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ':: Funcion con restricciones en los albaranes de proveedor ... '
CREATE OR REPLACE FUNCTION restriccionesalbaranp() RETURNS "trigger"
AS '
DECLARE
    rs RECORD;

BEGIN
    IF NEW.fechaalbaranp IS NULL THEN
	NEW.fechaalbaranp := now();
    END IF;

    IF NEW.numalbaranp IS NULL THEN
	NEW.numalbaranp := NEW.idalbaranp;
    END IF;

    IF NEW.refalbaranp IS NULL OR NEW.refalbaranp = '''' THEN
	SELECT INTO rs crearef() AS m;
	IF FOUND THEN
	    NEW.refalbaranp := rs.m;
	END IF;
    END IF;

    RETURN NEW;
END;
' LANGUAGE plpgsql;



\echo -n ':: Funcion que calcula el total de una factura de proveedor ... '
CREATE OR REPLACE FUNCTION calctotalfacpro(integer) RETURNS numeric(12, 2)
AS '
DECLARE
    idp ALIAS FOR $1;
    totalBImponibleLineas numeric(12, 2);
    totalIRPF numeric(12, 2);
    totalIVA numeric(12, 2);
    totalRE numeric(12, 2);
    totalTotal numeric(12, 2);
    res RECORD;
    res2 RECORD;

BEGIN
    totalBImponibleLineas := 0;
    totalIRPF := 0;
    totalIVA := 0;
    totalRE := 0;
    totalTotal := 0;

    FOR res IN SELECT cantlfacturap * pvplfacturap * (1 - descuentolfacturap / 100) AS subtotal1 FROM lfacturap WHERE idfacturap = idp LOOP
	totalBImponibleLineas := totalBImponibleLineas + res.subtotal1;
    END LOOP;

    SELECT INTO res2 idproveedor FROM facturap WHERE idfacturap = idp;

    SELECT INTO res irpfproveedor FROM proveedor WHERE idproveedor = res2.idproveedor;
    IF FOUND THEN
        totalIRPF := totalBImponibleLineas * (res.irpfproveedor / 100);
    END IF;

    FOR res IN SELECT cantlfacturap * pvplfacturap * (1 - descuentolfacturap / 100) * (ivalfacturap / 100) AS subtotal1 FROM lfacturap WHERE idfacturap = idp LOOP
	totalIVA := totalIVA + res.subtotal1;
    END LOOP;

    FOR res IN SELECT cantlfacturap * pvplfacturap * (1 - descuentolfacturap / 100) * (reqeqlfacturap / 100) AS subtotal1 FROM lfacturap WHERE idfacturap = idp LOOP
	totalRE := totalRE + res.subtotal1;
    END LOOP;

    FOR res IN SELECT proporciondfacturap FROM dfacturap WHERE idfacturap = idp LOOP
	totalBImponibleLineas := totalBImponibleLineas * (1 - res.proporciondfacturap / 100);
	totalIRPF := totalIRPF * (1 - res.proporciondfacturap / 100);
	totalIVA := totalIVA * (1 - res.proporciondfacturap / 100);
	totalRE := totalRE * (1 - res.proporciondfacturap / 100);
    END LOOP;

    totalTotal = totalBImponibleLineas - totalIRPF + totalIVA + totalRE;

    RETURN totalTotal;
END;
' LANGUAGE plpgsql;




\echo -n ':: Funcion que calcula el total de una factura ... '
CREATE OR REPLACE FUNCTION calctotalfactura(integer) RETURNS numeric(12, 2)
AS '
DECLARE
    idp ALIAS FOR $1;
    totalBImponibleLineas numeric(12, 2);
    totalIRPF numeric(12, 2);
    totalIVA numeric(12, 2);
    totalRE numeric(12, 2);
    totalTotal numeric(12, 2);
    res RECORD;

BEGIN
    totalBImponibleLineas := 0;
    totalIRPF := 0;
    totalIVA := 0;
    totalRE := 0;
    totalTotal := 0;

    FOR res IN SELECT cantlfactura * pvplfactura * (1 - descuentolfactura / 100) AS subtotal1 FROM lfactura WHERE idfactura = idp LOOP
	totalBImponibleLineas := totalBImponibleLineas + res.subtotal1;
    END LOOP;

    SELECT INTO res valor::numeric FROM configuracion WHERE LOWER(nombre) = ''irpf'';
    IF FOUND THEN
        totalIRPF := totalBImponibleLineas * (res.valor / 100);
    END IF;

    FOR res IN SELECT cantlfactura * pvplfactura * (1 - descuentolfactura / 100) * (ivalfactura / 100) AS subtotal1 FROM lfactura WHERE idfactura = idp LOOP
	totalIVA := totalIVA + res.subtotal1;
    END LOOP;

    FOR res IN SELECT cantlfactura * pvplfactura * (1 - descuentolfactura / 100) * (reqeqlfactura / 100) AS subtotal1 FROM lfactura WHERE idfactura = idp LOOP
	totalRE := totalRE + res.subtotal1;
    END LOOP;

    FOR res IN SELECT proporciondfactura FROM dfactura WHERE idfactura = idp LOOP
	totalBImponibleLineas := totalBImponibleLineas * (1 - res.proporciondfactura / 100);
	totalIRPF := totalIRPF * (1 - res.proporciondfactura / 100);
	totalIVA := totalIVA * (1 - res.proporciondfactura / 100);
	totalRE := totalRE * (1 - res.proporciondfactura / 100);
    END LOOP;

    totalTotal = totalBImponibleLineas - totalIRPF + totalIVA + totalRE;

    RETURN totalTotal;
END;
' LANGUAGE plpgsql;



\echo -n ':: Funcion que calcula el total del albaran de proveedor ... '
CREATE OR REPLACE FUNCTION calctotalalbpro(integer) RETURNS numeric(12, 2)
AS '
DECLARE
    idp ALIAS FOR $1;
    totalBImponibleLineas numeric(12, 2);
    totalIRPF numeric(12, 2);
    totalIVA numeric(12, 2);
    totalRE numeric(12, 2);
    totalTotal numeric(12, 2);
    res RECORD;
    res2 RECORD;

BEGIN
    totalBImponibleLineas := 0;
    totalIRPF := 0;
    totalIVA := 0;
    totalRE := 0;
    totalTotal := 0;

    FOR res IN SELECT cantlalbaranp * pvplalbaranp * (1 - descuentolalbaranp / 100) AS subtotal1 FROM lalbaranp WHERE idalbaranp = idp LOOP
	totalBImponibleLineas := totalBImponibleLineas + res.subtotal1;
    END LOOP;

    SELECT INTO res2 idproveedor FROM albaranp WHERE idalbaranp = idp;

    SELECT INTO res irpfproveedor FROM proveedor WHERE idproveedor = res2.idproveedor;
    IF FOUND THEN
        totalIRPF := totalBImponibleLineas * (res.irpfproveedor / 100);
    END IF;

    FOR res IN SELECT cantlalbaranp * pvplalbaranp * (1 - descuentolalbaranp / 100) * (ivalalbaranp / 100) AS subtotal1 FROM lalbaranp WHERE idalbaranp = idp LOOP
	totalIVA := totalIVA + res.subtotal1;
    END LOOP;

    FOR res IN SELECT cantlalbaranp * pvplalbaranp * (1 - descuentolalbaranp / 100) * (reqeqlalbaranp / 100) AS subtotal1 FROM lalbaranp WHERE idalbaranp = idp LOOP
	totalRE := totalRE + res.subtotal1;
    END LOOP;

    FOR res IN SELECT proporciondalbaranp FROM dalbaranp WHERE idalbaranp = idp LOOP
	totalBImponibleLineas := totalBImponibleLineas * (1 - res.proporciondalbaranp / 100);
	totalIRPF := totalIRPF * (1 - res.proporciondalbaranp / 100);
	totalIVA := totalIVA * (1 - res.proporciondalbaranp / 100);
	totalRE := totalRE * (1 - res.proporciondalbaranp / 100);
    END LOOP;

    totalTotal = totalBImponibleLineas - totalIRPF + totalIVA + totalRE;

    RETURN totalTotal;
END;
' LANGUAGE plpgsql;


\echo -n ':: Funcion que calcula el total del pedido a proveedor ... '
CREATE OR REPLACE FUNCTION calctotalpedpro(integer) RETURNS numeric(12, 2)
AS '
DECLARE
    idp ALIAS FOR $1;
    totalBImponibleLineas numeric(12, 2);
    totalIRPF numeric(12, 2);
    totalIVA numeric(12, 2);
    totalRE numeric(12, 2);
    totalTotal numeric(12, 2);
    res RECORD;
    res2 RECORD;

BEGIN
    totalBImponibleLineas := 0;
    totalIRPF := 0;
    totalIVA := 0;
    totalRE := 0;
    totalTotal := 0;

    FOR res IN SELECT cantlpedidoproveedor * pvplpedidoproveedor * (1 - descuentolpedidoproveedor / 100) AS subtotal1 FROM lpedidoproveedor WHERE idpedidoproveedor = idp LOOP
	totalBImponibleLineas := totalBImponibleLineas + res.subtotal1;
    END LOOP;

    SELECT INTO res2 idproveedor FROM pedidoproveedor WHERE idpedidoproveedor = idp;

    SELECT INTO res irpfproveedor FROM proveedor WHERE idproveedor = res2.idproveedor;
    IF FOUND THEN
        totalIRPF := totalBImponibleLineas * (res.irpfproveedor / 100);
    END IF;

    FOR res IN SELECT cantlpedidoproveedor * pvplpedidoproveedor * (1 - descuentolpedidoproveedor / 100) * (ivalpedidoproveedor / 100) AS subtotal1 FROM lpedidoproveedor WHERE idpedidoproveedor = idp LOOP
	totalIVA := totalIVA + res.subtotal1;
    END LOOP;

    FOR res IN SELECT cantlpedidoproveedor * pvplpedidoproveedor * (1 - descuentolpedidoproveedor / 100) * (reqeqlpedidoproveedor / 100) AS subtotal1 FROM lpedidoproveedor WHERE idpedidoproveedor = idp LOOP
	totalRE := totalRE + res.subtotal1;
    END LOOP;

    FOR res IN SELECT proporciondpedidoproveedor FROM dpedidoproveedor WHERE idpedidoproveedor = idp LOOP
	totalBImponibleLineas := totalBImponibleLineas * (1 - res.proporciondpedidoproveedor / 100);
	totalIRPF := totalIRPF * (1 - res.proporciondpedidoproveedor / 100);
	totalIVA := totalIVA * (1 - res.proporciondpedidoproveedor / 100);
	totalRE := totalRE * (1 - res.proporciondpedidoproveedor / 100);
    END LOOP;

    totalTotal = totalBImponibleLineas - totalIRPF + totalIVA + totalRE;

    RETURN totalTotal;
END;
' LANGUAGE plpgsql;



\echo -n ':: Funcion que calcula el total de un presupuesto a cliente ... '
CREATE OR REPLACE FUNCTION calctotalpres(integer) RETURNS numeric(12, 2)
AS '
DECLARE
    idp ALIAS FOR $1;
    totalBImponibleLineas numeric(12, 2);
    totalIRPF numeric(12, 2);
    totalIVA numeric(12, 2);
    totalRE numeric(12, 2);
    totalTotal numeric(12, 2);
    res RECORD;
    res2 RECORD;

BEGIN
    totalBImponibleLineas := 0;
    totalIRPF := 0;
    totalIVA := 0;
    totalRE := 0;
    totalTotal := 0;

    FOR res IN SELECT cantlpresupuesto * pvplpresupuesto * (1 - descuentolpresupuesto / 100) AS subtotal1 FROM lpresupuesto WHERE idpresupuesto = idp LOOP
	totalBImponibleLineas := totalBImponibleLineas + res.subtotal1;
    END LOOP;

    SELECT INTO res valor::numeric FROM configuracion WHERE LOWER(nombre) = ''irpf'';
    IF FOUND THEN
        totalIRPF := totalBImponibleLineas * (res.valor / 100);
    END IF;

    FOR res IN SELECT cantlpresupuesto * pvplpresupuesto * (1 - descuentolpresupuesto / 100) * (ivalpresupuesto / 100) AS subtotal1 FROM lpresupuesto WHERE idpresupuesto = idp LOOP
	totalIVA := totalIVA + res.subtotal1;
    END LOOP;

    FOR res IN SELECT cantlpresupuesto * pvplpresupuesto * (1 - descuentolpresupuesto / 100) * (reqeqlpresupuesto / 100) AS subtotal1 FROM lpresupuesto WHERE idpresupuesto = idp LOOP
	totalRE := totalRE + res.subtotal1;
    END LOOP;

    FOR res IN SELECT proporciondpresupuesto FROM dpresupuesto WHERE idpresupuesto = idp LOOP
	totalBImponibleLineas := totalBImponibleLineas * (1 - res.proporciondpresupuesto / 100);
	totalIRPF := totalIRPF * (1 - res.proporciondpresupuesto / 100);
	totalIVA := totalIVA * (1 - res.proporciondpresupuesto / 100);
	totalRE := totalRE * (1 - res.proporciondpresupuesto / 100);
    END LOOP;

    totalTotal = totalBImponibleLineas - totalIRPF + totalIVA + totalRE;

    RETURN totalTotal;
END;
' LANGUAGE plpgsql;



\echo -n ':: Funcion que calcula el total de un pedido de cliente ... '
CREATE OR REPLACE FUNCTION calctotalpedcli(integer) RETURNS numeric(12, 2)
AS '
DECLARE
    idp ALIAS FOR $1;
    totalBImponibleLineas numeric(12, 2);
    totalIRPF numeric(12, 2);
    totalIVA numeric(12, 2);
    totalRE numeric(12, 2);
    totalTotal numeric(12, 2);
    res RECORD;
    res2 RECORD;

BEGIN
    totalBImponibleLineas := 0;
    totalIRPF := 0;
    totalIVA := 0;
    totalRE := 0;
    totalTotal := 0;

    FOR res IN SELECT cantlpedidocliente * pvplpedidocliente * (1 - descuentolpedidocliente / 100) AS subtotal1 FROM lpedidocliente WHERE idpedidocliente = idp LOOP
	totalBImponibleLineas := totalBImponibleLineas + res.subtotal1;
    END LOOP;

    SELECT INTO res valor::numeric FROM configuracion WHERE LOWER(nombre) = ''irpf'';
    IF FOUND THEN
        totalIRPF := totalBImponibleLineas * (res.valor / 100);
    END IF;

    FOR res IN SELECT cantlpedidocliente * pvplpedidocliente * (1 - descuentolpedidocliente / 100) * (ivalpedidocliente / 100) AS subtotal1 FROM lpedidocliente WHERE idpedidocliente = idp LOOP
	totalIVA := totalIVA + res.subtotal1;
    END LOOP;

    FOR res IN SELECT cantlpedidocliente * pvplpedidocliente * (1 - descuentolpedidocliente / 100) * (reqeqlpedidocliente / 100) AS subtotal1 FROM lpedidocliente WHERE idpedidocliente = idp LOOP
	totalRE := totalRE + res.subtotal1;
    END LOOP;

    FOR res IN SELECT proporciondpedidocliente FROM dpedidocliente WHERE idpedidocliente = idp LOOP
	totalBImponibleLineas := totalBImponibleLineas * (1 - res.proporciondpedidocliente / 100);
	totalIRPF := totalIRPF * (1 - res.proporciondpedidocliente / 100);
	totalIVA := totalIVA * (1 - res.proporciondpedidocliente / 100);
	totalRE := totalRE * (1 - res.proporciondpedidocliente / 100);
    END LOOP;

    totalTotal = totalBImponibleLineas - totalIRPF + totalIVA + totalRE;

    RETURN totalTotal;
END;
' LANGUAGE plpgsql;


\echo -n ':: Funcion que calcula el total de un albaran a cliente ... '
CREATE OR REPLACE FUNCTION calctotalalbaran(integer) RETURNS numeric(12, 2)
AS '
DECLARE
    idp ALIAS FOR $1;
    totalBImponibleLineas numeric(12, 2);
    totalIRPF numeric(12, 2);
    totalIVA numeric(12, 2);
    totalRE numeric(12, 2);
    totalTotal numeric(12, 2);
    res RECORD;
    res2 RECORD;

BEGIN
    totalBImponibleLineas := 0;
    totalIRPF := 0;
    totalIVA := 0;
    totalRE := 0;
    totalTotal := 0;

    FOR res IN SELECT cantlalbaran * pvplalbaran * (1 - descuentolalbaran / 100) AS subtotal1 FROM lalbaran WHERE idalbaran = idp LOOP
	totalBImponibleLineas := totalBImponibleLineas + res.subtotal1;
    END LOOP;

    SELECT INTO res valor::numeric FROM configuracion WHERE LOWER(nombre) = ''irpf'';
    IF FOUND THEN
        totalIRPF := totalBImponibleLineas * (res.valor / 100);
    END IF;

    FOR res IN SELECT cantlalbaran * pvplalbaran * (1 - descuentolalbaran / 100) * (ivalalbaran / 100) AS subtotal1 FROM lalbaran WHERE idalbaran = idp LOOP
	totalIVA := totalIVA + res.subtotal1;
    END LOOP;

    FOR res IN SELECT cantlalbaran * pvplalbaran * (1 - descuentolalbaran / 100) * (reqeqlalbaran / 100) AS subtotal1 FROM lalbaran WHERE idalbaran = idp LOOP
	totalRE := totalRE + res.subtotal1;
    END LOOP;

    FOR res IN SELECT proporciondalbaran FROM dalbaran WHERE idalbaran = idp LOOP
	totalBImponibleLineas := totalBImponibleLineas * (1 - res.proporciondalbaran / 100);
	totalIRPF := totalIRPF * (1 - res.proporciondalbaran / 100);
	totalIVA := totalIVA * (1 - res.proporciondalbaran / 100);
	totalRE := totalRE * (1 - res.proporciondalbaran / 100);
    END LOOP;

    totalTotal = totalBImponibleLineas - totalIRPF + totalIVA + totalRE;

    RETURN totalTotal;
END;
' LANGUAGE plpgsql;



CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
        rs RECORD;
BEGIN
	SELECT INTO rs * FROM pg_tables WHERE tablename=''comparticulo'';
	IF FOUND THEN
		ALTER TABLE comparticulo ALTER COLUMN cantcomparticulo TYPE numeric(12, 2);
		ALTER TABLE comparticulo ALTER COLUMN cantcomparticulo SET NOT NULL;
		ALTER TABLE comparticulo ALTER COLUMN cantcomparticulo SET DEFAULT 1;
	END IF;
	RETURN 0;
END;
' LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo -n ':: Modificada tabla comparticulo ... '




-- =====================================================================================

-- Agregamos nuevos parametros de configuracion
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	rs RECORD;
BEGIN
	SELECT INTO rs * FROM configuracion WHERE nombre = ''DatabaseRevision'';
	IF FOUND THEN
		UPDATE CONFIGURACION SET valor = ''0.12.1-0008'' WHERE nombre = ''DatabaseRevision'';
	ELSE
		INSERT INTO configuracion (nombre, valor) VALUES (''DatabaseRevision'', ''0.12.1-0008'');
	END IF;
	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT actualizarevision();
DROP FUNCTION actualizarevision() CASCADE;
\echo "Actualizada la revision de la base de datos a la version 0.12.1"

DROP FUNCTION drop_if_exists_table(text) CASCADE;
DROP FUNCTION drop_if_exists_proc(text, text) CASCADE;


COMMIT;

