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
	SELECT INTO rs * FROM configuracion WHERE nombre=''DatabaseRevision'' AND (  valor LIKE ''1.5.%'' OR valor LIKE ''1.6.%'');
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


    SELECT INTO rs attname, relname FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid=pg_class.oid WHERE attname=''irpffacturap'' AND relname=''facturap'';
    IF NOT FOUND THEN
        ALTER TABLE facturap ADD COLUMN irpffacturap NUMERIC(12,2);
    END IF;

    SELECT INTO rs attname, relname FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid=pg_class.oid WHERE attname=''irpfalbaranp'' AND relname=''albaranp'';
    IF NOT FOUND THEN
        ALTER TABLE albaranp ADD COLUMN irpfalbaranp NUMERIC(12,2);
    END IF;
    
    SELECT INTO rs attname, relname FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid=pg_class.oid WHERE attname=''irpfpedidoproveedor'' AND relname=''pedidoproveedor'';
    IF NOT FOUND THEN
        ALTER TABLE pedidoproveedor ADD COLUMN irpfpedidoproveedor NUMERIC(12,2);
    END IF;
    
    
   RETURN 0;
END;
' LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;


CREATE OR REPLACE FUNCTION calctotalfacpro(integer) RETURNS numeric(12, 2)
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

    FOR rs IN SELECT cantlfacturap * pvplfacturap * (1 - descuentolfacturap / 100) AS subtotal1 FROM lfacturap WHERE idfacturap = idp LOOP
	totalBImponibleLineas := totalBImponibleLineas + rs.subtotal1;
    END LOOP;

    SELECT INTO rs irpffacturap FROM facturap WHERE idfacturap = idp;
    IF FOUND THEN
        totalIRPF := totalBImponibleLineas * (rs.irpffacturap / 100);
    END IF;

    FOR rs IN SELECT cantlfacturap * pvplfacturap * (1 - descuentolfacturap / 100) * (ivalfacturap / 100) AS subtotal1 FROM lfacturap WHERE idfacturap = idp LOOP
	totalIVA := totalIVA + rs.subtotal1;
    END LOOP;

    FOR rs IN SELECT cantlfacturap * pvplfacturap * (1 - descuentolfacturap / 100) * (reqeqlfacturap / 100) AS subtotal1 FROM lfacturap WHERE idfacturap = idp LOOP
	totalRE := totalRE + rs.subtotal1;
    END LOOP;

    FOR rs IN SELECT proporciondfacturap FROM dfacturap WHERE idfacturap = idp LOOP
	totalBImponibleLineas := totalBImponibleLineas * (1 - rs.proporciondfacturap / 100);
	totalIRPF := totalIRPF * (1 - rs.proporciondfacturap / 100);
	totalIVA := totalIVA * (1 - rs.proporciondfacturap / 100);
	totalRE := totalRE * (1 - rs.proporciondfacturap / 100);
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
    rs RECORD;
    rs2 RECORD;

BEGIN
    totalBImponibleLineas := 0;
    totalIRPF := 0;
    totalIVA := 0;
    totalRE := 0;
    totalTotal := 0;

    FOR rs IN SELECT cantlalbaranp * pvplalbaranp * (1 - descuentolalbaranp / 100) AS subtotal1 FROM lalbaranp WHERE idalbaranp = idp LOOP
	totalBImponibleLineas := totalBImponibleLineas + rs.subtotal1;
    END LOOP;

    SELECT INTO rs irpfalbaranp FROM albaranp WHERE idalbaranp = idp;
    IF FOUND THEN
        totalIRPF := totalBImponibleLineas * (rs.irpfalbaranp / 100);
    END IF;

    FOR rs IN SELECT cantlalbaranp * pvplalbaranp * (1 - descuentolalbaranp / 100) * (ivalalbaranp / 100) AS subtotal1 FROM lalbaranp WHERE idalbaranp = idp LOOP
	totalIVA := totalIVA + rs.subtotal1;
    END LOOP;

    FOR rs IN SELECT cantlalbaranp * pvplalbaranp * (1 - descuentolalbaranp / 100) * (reqeqlalbaranp / 100) AS subtotal1 FROM lalbaranp WHERE idalbaranp = idp LOOP
	totalRE := totalRE + rs.subtotal1;
    END LOOP;

    FOR rs IN SELECT proporciondalbaranp FROM dalbaranp WHERE idalbaranp = idp LOOP
	totalBImponibleLineas := totalBImponibleLineas * (1 - rs.proporciondalbaranp / 100);
	totalIRPF := totalIRPF * (1 - rs.proporciondalbaranp / 100);
	totalIVA := totalIVA * (1 - rs.proporciondalbaranp / 100);
	totalRE := totalRE * (1 - rs.proporciondalbaranp / 100);
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
    rs RECORD;
    rs2 RECORD;

BEGIN
    totalBImponibleLineas := 0;
    totalIRPF := 0;
    totalIVA := 0;
    totalRE := 0;
    totalTotal := 0;

    FOR rs IN SELECT cantlpedidoproveedor * pvplpedidoproveedor * (1 - descuentolpedidoproveedor / 100) AS subtotal1 FROM lpedidoproveedor WHERE idpedidoproveedor = idp LOOP
	totalBImponibleLineas := totalBImponibleLineas + rs.subtotal1;
    END LOOP;


    SELECT INTO rs irpfpedidoproveedor FROM pedidoproveedor WHERE idpedidoproveedor = idp;
    IF FOUND THEN
        totalIRPF := totalBImponibleLineas * (rs.irpfpedidoproveedor / 100);
    END IF;

    FOR rs IN SELECT cantlpedidoproveedor * pvplpedidoproveedor * (1 - descuentolpedidoproveedor / 100) * (ivalpedidoproveedor / 100) AS subtotal1 FROM lpedidoproveedor WHERE idpedidoproveedor = idp LOOP
	totalIVA := totalIVA + rs.subtotal1;
    END LOOP;

    FOR rs IN SELECT cantlpedidoproveedor * pvplpedidoproveedor * (1 - descuentolpedidoproveedor / 100) * (reqeqlpedidoproveedor / 100) AS subtotal1 FROM lpedidoproveedor WHERE idpedidoproveedor = idp LOOP
	totalRE := totalRE + rs.subtotal1;
    END LOOP;

    FOR rs IN SELECT proporciondpedidoproveedor FROM dpedidoproveedor WHERE idpedidoproveedor = idp LOOP
	totalBImponibleLineas := totalBImponibleLineas * (1 - rs.proporciondpedidoproveedor / 100);
	totalIRPF := totalIRPF * (1 - rs.proporciondpedidoproveedor / 100);
	totalIVA := totalIVA * (1 - rs.proporciondpedidoproveedor / 100);
	totalRE := totalRE * (1 - rs.proporciondpedidoproveedor / 100);
    END LOOP;

    totalTotal = totalBImponibleLineas - totalIRPF + totalIVA + totalRE;

    RETURN totalTotal;
END;
' LANGUAGE plpgsql;


-- FUNCIONES VARIAS DE SOPORTE.
\echo -n ':: Funcion que calcula el IVA de un articulo ... '
CREATE OR REPLACE FUNCTION ivaarticulo(integer) RETURNS numeric(12, 2)
AS'
DECLARE
    idarticulo1 ALIAS FOR $1;
    rs RECORD;

BEGIN
    SELECT INTO rs * FROM tipo_iva, tasa_iva, articulo WHERE tasa_iva.idtipo_iva = tipo_iva.idtipo_iva AND tipo_iva.idtipo_iva = articulo.idtipo_iva AND articulo.idarticulo = idarticulo1 ORDER BY fechatasa_iva;

    IF FOUND THEN
	RETURN rs.porcentasa_iva;
    END IF;

    RETURN 0.0;
END;
' LANGUAGE plpgsql;


\echo -n ':: Funcion que calcula el PVP de un articulo ... '
CREATE OR REPLACE FUNCTION pvparticulo(integer) RETURNS numeric(12, 2)
AS'
DECLARE
    idarticulo1 ALIAS FOR $1;
    rs RECORD;

BEGIN
    SELECT INTO rs pvparticulo FROM articulo1 WHERE articulo.idarticulo = idarticulo;

    IF FOUND THEN
	RETURN rs.pvparticulo;
    END IF;

    RETURN 0.0;
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
		UPDATE CONFIGURACION SET valor = ''1.6.0-0001'' WHERE nombre = ''DatabaseRevision'';
	ELSE
		INSERT INTO configuracion (nombre, valor) VALUES (''DatabaseRevision'', ''1.6.0-0001'');
	END IF;
	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT actualizarevision();
DROP FUNCTION actualizarevision() CASCADE;
\echo "Actualizada la revision de la base de datos a la version 1.6.0"

DROP FUNCTION drop_if_exists_table(text) CASCADE;
DROP FUNCTION drop_if_exists_proc(text, text) CASCADE;


COMMIT;

