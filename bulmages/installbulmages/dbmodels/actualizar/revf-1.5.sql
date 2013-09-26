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
	SELECT INTO rs * FROM configuracion WHERE nombre=''DatabaseRevision'' AND ( valor LIKE ''0.14.1%'' OR valor = ''0.13.1-0006'' OR valor LIKE ''0.15.%'' OR valor LIKE ''1.5.%'');
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

-- Creamos la tabla de gestion de IRPF
\echo -n ':: Creamos la tabla de gestion de IRPF '
CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
    rs RECORD;
BEGIN

    SELECT INTO rs * FROM pg_tables WHERE tablename=''irpf'';
    IF NOT FOUND THEN
	CREATE TABLE irpf (
	    idirpf serial PRIMARY KEY,
	    fechairpf date NOT NULL,
	    tasairpf  numeric(12,2)
	);

    END IF;

    DELETE FROM configuracion WHERE nombre=''IRPF'';

    SELECT INTO rs attname, relname FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid=pg_class.oid WHERE attname=''irpfpresupuesto'' AND relname=''presupuesto'';
    IF NOT FOUND THEN
        ALTER TABLE presupuesto ADD COLUMN irpfpresupuesto NUMERIC(12,2);
    END IF;

    SELECT INTO rs attname, relname FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid=pg_class.oid WHERE attname=''irpfpedidocliente'' AND relname=''pedidocliente'';
    IF NOT FOUND THEN
        ALTER TABLE pedidocliente ADD COLUMN irpfpedidocliente NUMERIC(12,2);
        ALTER TABLE albaran ADD COLUMN irpfalbaran NUMERIC(12,2);
        ALTER TABLE factura ADD COLUMN irpffactura NUMERIC(12,2);
    END IF;


   RETURN 0;
END;
' LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;



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
    rs RECORD;

BEGIN
    totalBImponibleLineas := 0;
    totalIRPF := 0;
    totalIVA := 0;
    totalRE := 0;
    totalTotal := 0;

    FOR rs IN SELECT cantlfactura * pvplfactura * (1 - descuentolfactura / 100) AS subtotal1 FROM lfactura WHERE idfactura = idp LOOP
	totalBImponibleLineas := totalBImponibleLineas + rs.subtotal1;
    END LOOP;


    SELECT INTO rs irpffactura FROM factura WHERE idfactura = idp;
    IF FOUND THEN
        totalIRPF := totalBImponibleLineas * (rs.irpffactura / 100);
    END IF;

    FOR rs IN SELECT cantlfactura * pvplfactura * (1 - descuentolfactura / 100) * (ivalfactura / 100) AS subtotal1 FROM lfactura WHERE idfactura = idp LOOP
	totalIVA := totalIVA + rs.subtotal1;
    END LOOP;

    FOR rs IN SELECT cantlfactura * pvplfactura * (1 - descuentolfactura / 100) * (reqeqlfactura / 100) AS subtotal1 FROM lfactura WHERE idfactura = idp LOOP
	totalRE := totalRE + rs.subtotal1;
    END LOOP;

    FOR rs IN SELECT proporciondfactura FROM dfactura WHERE idfactura = idp LOOP
	totalBImponibleLineas := totalBImponibleLineas * (1 - rs.proporciondfactura / 100);
	totalIRPF := totalIRPF * (1 - rs.proporciondfactura / 100);
	totalIVA := totalIVA * (1 - rs.proporciondfactura / 100);
	totalRE := totalRE * (1 - rs.proporciondfactura / 100);
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
    rs RECORD;
    rs2 RECORD;

BEGIN
    totalBImponibleLineas := 0;
    totalIRPF := 0;
    totalIVA := 0;
    totalRE := 0;
    totalTotal := 0;

    FOR rs IN SELECT cantlpresupuesto * pvplpresupuesto * (1 - descuentolpresupuesto / 100) AS subtotal1 FROM lpresupuesto WHERE idpresupuesto = idp LOOP
	totalBImponibleLineas := totalBImponibleLineas + rs.subtotal1;
    END LOOP;

    SELECT INTO rs irpfpresupuesto FROM presupuesto WHERE idpresupuesto = idp;
    IF FOUND THEN
        totalIRPF := totalBImponibleLineas * (rs.irpfpresupuesto / 100);
    END IF;

    FOR rs IN SELECT cantlpresupuesto * pvplpresupuesto * (1 - descuentolpresupuesto / 100) * (ivalpresupuesto / 100) AS subtotal1 FROM lpresupuesto WHERE idpresupuesto = idp LOOP
	totalIVA := totalIVA + rs.subtotal1;
    END LOOP;

    FOR rs IN SELECT cantlpresupuesto * pvplpresupuesto * (1 - descuentolpresupuesto / 100) * (reqeqlpresupuesto / 100) AS subtotal1 FROM lpresupuesto WHERE idpresupuesto = idp LOOP
	totalRE := totalRE + rs.subtotal1;
    END LOOP;

    FOR rs IN SELECT proporciondpresupuesto FROM dpresupuesto WHERE idpresupuesto = idp LOOP
	totalBImponibleLineas := totalBImponibleLineas * (1 - rs.proporciondpresupuesto / 100);
	totalIRPF := totalIRPF * (1 - rs.proporciondpresupuesto / 100);
	totalIVA := totalIVA * (1 - rs.proporciondpresupuesto / 100);
	totalRE := totalRE * (1 - rs.proporciondpresupuesto / 100);
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
    rs RECORD;
    rs2 RECORD;

BEGIN
    totalBImponibleLineas := 0;
    totalIRPF := 0;
    totalIVA := 0;
    totalRE := 0;
    totalTotal := 0;

    FOR rs IN SELECT cantlpedidocliente * pvplpedidocliente * (1 - descuentolpedidocliente / 100) AS subtotal1 FROM lpedidocliente WHERE idpedidocliente = idp LOOP
	totalBImponibleLineas := totalBImponibleLineas + rs.subtotal1;
    END LOOP;

    SELECT INTO rs irpfpedidocliente FROM pedidocliente WHERE idpedidocliente = idp;
    IF FOUND THEN
        totalIRPF := totalBImponibleLineas * (rs.irpfpedidocliente / 100);
    END IF;

    FOR rs IN SELECT cantlpedidocliente * pvplpedidocliente * (1 - descuentolpedidocliente / 100) * (ivalpedidocliente / 100) AS subtotal1 FROM lpedidocliente WHERE idpedidocliente = idp LOOP
	totalIVA := totalIVA + rs.subtotal1;
    END LOOP;

    FOR rs IN SELECT cantlpedidocliente * pvplpedidocliente * (1 - descuentolpedidocliente / 100) * (reqeqlpedidocliente / 100) AS subtotal1 FROM lpedidocliente WHERE idpedidocliente = idp LOOP
	totalRE := totalRE + rs.subtotal1;
    END LOOP;

    FOR rs IN SELECT proporciondpedidocliente FROM dpedidocliente WHERE idpedidocliente = idp LOOP
	totalBImponibleLineas := totalBImponibleLineas * (1 - rs.proporciondpedidocliente / 100);
	totalIRPF := totalIRPF * (1 - rs.proporciondpedidocliente / 100);
	totalIVA := totalIVA * (1 - rs.proporciondpedidocliente / 100);
	totalRE := totalRE * (1 - rs.proporciondpedidocliente / 100);
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
    rs RECORD;
    rs2 RECORD;

BEGIN
    totalBImponibleLineas := 0;
    totalIRPF := 0;
    totalIVA := 0;
    totalRE := 0;
    totalTotal := 0;

    FOR rs IN SELECT cantlalbaran * pvplalbaran * (1 - descuentolalbaran / 100) AS subtotal1 FROM lalbaran WHERE idalbaran = idp LOOP
	totalBImponibleLineas := totalBImponibleLineas + rs.subtotal1;
    END LOOP;

    SELECT INTO rs irpfalbaran FROM albaran WHERE idalbaran = idp;
    IF FOUND THEN
        totalIRPF := totalBImponibleLineas * (rs.irpfalbaran / 100);
    END IF;

    FOR rs IN SELECT cantlalbaran * pvplalbaran * (1 - descuentolalbaran / 100) * (ivalalbaran / 100) AS subtotal1 FROM lalbaran WHERE idalbaran = idp LOOP
	totalIVA := totalIVA + rs.subtotal1;
    END LOOP;

    FOR rs IN SELECT cantlalbaran * pvplalbaran * (1 - descuentolalbaran / 100) * (reqeqlalbaran / 100) AS subtotal1 FROM lalbaran WHERE idalbaran = idp LOOP
	totalRE := totalRE + rs.subtotal1;
    END LOOP;

    FOR rs IN SELECT proporciondalbaran FROM dalbaran WHERE idalbaran = idp LOOP
	totalBImponibleLineas := totalBImponibleLineas * (1 - rs.proporciondalbaran / 100);
	totalIRPF := totalIRPF * (1 - rs.proporciondalbaran / 100);
	totalIVA := totalIVA * (1 - rs.proporciondalbaran / 100);
	totalRE := totalRE * (1 - rs.proporciondalbaran / 100);
    END LOOP;

    totalTotal = totalBImponibleLineas - totalIRPF + totalIVA + totalRE;

    RETURN totalTotal;
END;
' LANGUAGE plpgsql;


-- =====================================================================================

-- Agregamos nuevos parametros de configuracion
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	rs RECORD;
BEGIN
	SELECT INTO rs * FROM configuracion WHERE nombre = ''DatabaseRevision'';
	IF FOUND THEN
		UPDATE CONFIGURACION SET valor = ''1.5.1-0002'' WHERE nombre = ''DatabaseRevision'';
	ELSE
		INSERT INTO configuracion (nombre, valor) VALUES (''DatabaseRevision'', ''1.5.1-0002'');
	END IF;
	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT actualizarevision();
DROP FUNCTION actualizarevision() CASCADE;
\echo "Actualizada la revision de la base de datos a la version 1.5.2"

DROP FUNCTION drop_if_exists_table(text) CASCADE;
DROP FUNCTION drop_if_exists_proc(text, text) CASCADE;


COMMIT;

