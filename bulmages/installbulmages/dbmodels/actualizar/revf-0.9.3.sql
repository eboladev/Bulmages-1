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
	as RECORD;
BEGIN
	SELECT INTO as * FROM configuracion WHERE nombre=''DatabaseRevision'' AND ( valor LIKE ''0.9.3%'' OR valor = ''0.9.1-0011'');
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

CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
BEGIN
--    ALTER TABLE	lpresupuesto ALTER COLUMN desclpresupuesto TYPE character varying;
--    ALTER TABLE lpedidocliente ALTER COLUMN desclpedidocliente TYPE character varying;
--    ALTER TABLE lpedidoproveedor ALTER COLUMN desclpedidoproveedor TYPE character varying;
--    ALTER TABLE	lalbaranp ALTER COLUMN desclalbaranp TYPE character varying;
--    ALTER TABLE	lalbaran ALTER COLUMN desclalbaran TYPE character varying;
--    ALTER TABLE	lfactura ALTER COLUMN desclfactura TYPE character varying;
--    ALTER TABLE	lfacturap ALTER COLUMN desclfacturap TYPE character varying;
    RETURN 0;
END;
' LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo "Ampliada la capacidad de los campos de descripcion en las lineas de detalle"


CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
	as RECORD;
BEGIN
	SELECT INTO as * FROM pg_attribute WHERE attname=''fechavenccobro'';
	IF NOT FOUND THEN

		ALTER TABLE cobro ADD COLUMN fechavenccobro date;
		ALTER TABLE cobro ALTER COLUMN fechavenccobro SET DEFAULT NOW();

	END IF;
	RETURN 0;
END;
' LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo "Agregado el campo de Fecha Vencimiento de cobro"


SELECT drop_if_exists_proc('restriccionescobro', '');
\echo -n ':: Funcion que crea restricciones en cobro ... '
CREATE FUNCTION restriccionescobro() RETURNS "trigger"
AS '
DECLARE
    asd RECORD;

BEGIN
    IF NEW.fechacobro IS NULL THEN
	NEW.fechacobro := now();
    END IF;
    IF NEW.refcobro IS NULL OR NEW.refcobro = '''' THEN
	SELECT INTO asd crearef() AS m;
	IF FOUND THEN
	    NEW.refcobro := asd.m;
	END IF;
    END IF;
    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador de restricciones en cobro ... '
CREATE TRIGGER restriccionescobrotrigger
    BEFORE INSERT OR UPDATE ON cobro
    FOR EACH ROW
    EXECUTE PROCEDURE restriccionescobro();


SELECT drop_if_exists_proc('restriccionespago', '');
\echo -n ':: Funcion que crea restricciones en pago ... '
CREATE FUNCTION restriccionespago() RETURNS "trigger"
AS '
DECLARE
    asd RECORD;

BEGIN
    IF NEW.fechapago IS NULL THEN
	NEW.fechapago := now();
    END IF;
    IF NEW.refpago IS NULL OR NEW.refpago = '''' THEN
	SELECT INTO asd crearef() AS m;
	IF FOUND THEN
	    NEW.refpago := asd.m;
	END IF;
    END IF;
    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador de restricciones en pago ... '
CREATE TRIGGER restriccionespagotrigger
    BEFORE INSERT OR UPDATE ON pago
    FOR EACH ROW
    EXECUTE PROCEDURE restriccionespago();


SELECT drop_if_exists_proc('random_string', 'int4');
\echo -n ':: Funcion random_string que genera una cadena aleatoria ... '
CREATE FUNCTION random_string(int4) RETURNS "varchar" AS '
DECLARE
    iLoop int4;
    result varchar;

BEGIN
    result = '''';
    IF ($1 > 0) AND ($1 < 255) THEN
	FOR iLoop in 1 .. $1 LOOP
	    result = result || chr(int4(random()*25)+65);
	END LOOP;
	RETURN result;
    ELSE
	RETURN ''f'';
    END IF;
END;
'  LANGUAGE 'plpgsql';


SELECT drop_if_exists_proc('crearef', '');
\echo -n ':: Funcion crearef para crear codigos de referencia ... '
CREATE FUNCTION crearef() RETURNS character varying(15)
AS '
DECLARE
    asd RECORD;
    result character varying(15);
    efound boolean;

BEGIN
    efound := FALSE;
    WHILE efound = FALSE LOOP
	result := random_string(6);
	efound := TRUE;
	SELECT INTO asd idpresupuesto FROM presupuesto WHERE refpresupuesto = result;
	IF FOUND THEN
	    efound := FALSE;
	END IF;
	SELECT INTO asd idpedidocliente FROM pedidocliente WHERE refpedidocliente = result;
	IF FOUND THEN
	    efound := FALSE;
	END IF;
	SELECT INTO asd idalbaran FROM albaran WHERE refalbaran = result;
	IF FOUND THEN
	    efound := FALSE;
	END IF;	
	SELECT INTO asd idfactura FROM factura WHERE reffactura = result;
	IF FOUND THEN
		efound := FALSE;
	END IF;
	SELECT INTO asd idcobro FROM cobro WHERE refcobro = result;
	IF FOUND THEN
		efound := FALSE;
	END IF;
	SELECT INTO asd idpedidoproveedor FROM pedidoproveedor WHERE refpedidoproveedor = result;
	IF FOUND THEN
		efound := FALSE;
	END IF;
	SELECT INTO asd idalbaranp FROM albaranp WHERE refalbaranp = result;
	IF FOUND THEN
		efound := FALSE;
	END IF;
	SELECT INTO asd idfacturap FROM facturap WHERE reffacturap = result;
	IF FOUND THEN
		efound := FALSE;
	END IF;
	SELECT INTO asd idpago FROM pago WHERE refpago = result;
	IF FOUND THEN
		efound := FALSE;
	END IF;
    END LOOP;
    RETURN result;
END;
' LANGUAGE plpgsql;


CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
	bs RECORD;
BEGIN
	SELECT INTO bs * FROM pg_tables WHERE tablename=''pais'';
	IF NOT FOUND THEN
		DROP TABLE provincia;
                ALTER TABLE cliente DROP COLUMN provcliente;
                ALTER TABLE proveedor DROP COLUMN provproveedor;
		CREATE TABLE pais (
			idpais SERIAL PRIMARY KEY,
			cod2pais CHARACTER varying(2),
			cod3pais CHARACTER varying(3),
			descpais CHARACTER varying(50)
		);
		CREATE TABLE provincia (
			idprovincia SERIAL PRIMARY KEY,
			idpais integer REFERENCES pais(idpais),
			provincia CHARACTER VARYING(500)
		);
		ALTER TABLE cliente ADD COLUMN idprovincia INTEGER REFERENCES provincia (idprovincia);
		ALTER TABLE proveedor ADD COLUMN idprovincia INTEGER REFERENCES provincia (idprovincia);
	END IF;
	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo "Creamos tablas 'pais' y 'provincia'. Se modifica 'cuenta'."



\echo -n ':: Funcion con restricciones en factura ... '
SELECT drop_if_exists_proc('restriccionesfactura', '');
CREATE FUNCTION restriccionesfactura() RETURNS "trigger"
AS '
DECLARE
    asd RECORD;

BEGIN
    IF NEW.ffactura IS NULL THEN
	NEW.ffactura := now();
    END IF;
    IF NEW.numfactura IS NULL THEN
	SELECT INTO asd max(numfactura) AS m FROM factura WHERE codigoserie_factura = NEW.codigoserie_factura AND idalmacen = NEW.idalmacen;
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


\echo -n ':: Disparador antes de insertar o actualizar una factura ... '
CREATE TRIGGER restriccionesfacturatrigger
    BEFORE INSERT OR UPDATE ON factura
    FOR EACH ROW
    EXECUTE PROCEDURE restriccionesfactura();


SELECT drop_if_exists_proc('calctotalfactura', 'integer');
\echo -n ':: Funcion que calcula el total de una factura ... '
CREATE OR REPLACE FUNCTION calctotalfactura(integer) RETURNS numeric(12, 2)
AS '
DECLARE
    idp ALIAS FOR $1;
    totalBImponibleLineas numeric(12, 4);
    totalIRPF numeric(12, 4);
    totalIVA numeric(12, 4);
    totalRE numeric(12, 4);
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



SELECT drop_if_exists_proc('calctotalfacpro', 'integer');
\echo -n ':: Funcion que calcula el total de una factura de proveedor ... '
CREATE OR REPLACE FUNCTION calctotalfacpro(integer) RETURNS numeric(12, 2)
AS '
DECLARE
    idp ALIAS FOR $1;
    totalBImponibleLineas numeric(12, 4);
    totalIRPF numeric(12, 4);
    totalIVA numeric(12, 4);
    totalRE numeric(12, 4);
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



SELECT drop_if_exists_proc('calctotalalbpro', 'integer');
\echo -n ':: Funcion que calcula el total del albaran de proveedor ... '
CREATE OR REPLACE FUNCTION calctotalalbpro(integer) RETURNS numeric(12, 2)
AS '
DECLARE
    idp ALIAS FOR $1;
    totalBImponibleLineas numeric(12, 4);
    totalIRPF numeric(12, 4);
    totalIVA numeric(12, 4);
    totalRE numeric(12, 4);
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



\echo -n ':: Se cambia el nombre a algunos campos ... '
CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER
AS '
DECLARE
	bs RECORD;
BEGIN
	SELECT INTO bs * FROM pg_attribute WHERE attname=''numlpedidoproveedor'';
	IF FOUND THEN
		ALTER TABLE lpedidoproveedor RENAME numlpedidoproveedor TO idlpedidoproveedor;
	END IF;

	RETURN 0;
END;
' LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;



SELECT drop_if_exists_proc('calctotalpedpro', 'integer');
\echo -n ':: Funcion que calcula el total del pedido a proveedor ... '
CREATE OR REPLACE FUNCTION calctotalpedpro(integer) RETURNS numeric(12, 2)
AS '
DECLARE
    idp ALIAS FOR $1;
    totalBImponibleLineas numeric(12, 4);
    totalIRPF numeric(12, 4);
    totalIVA numeric(12, 4);
    totalRE numeric(12, 4);
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


\echo -n ':: Funcion que calcula el total de un pedido de cliente ... '
CREATE OR REPLACE FUNCTION calctotalpedcli(integer) RETURNS numeric(12, 2)
AS '
DECLARE
    idp ALIAS FOR $1;
    totalBImponibleLineas numeric(12, 4);
    totalIRPF numeric(12, 4);
    totalIVA numeric(12, 4);
    totalRE numeric(12, 4);
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



\echo -n ':: Funcion que calcula el total de un presupuesto a cliente ... '
CREATE OR REPLACE FUNCTION calctotalpres(integer) RETURNS numeric(12, 2)
AS '
DECLARE
    idp ALIAS FOR $1;
    totalBImponibleLineas numeric(12, 4);
    totalIRPF numeric(12, 4);
    totalIVA numeric(12, 4);
    totalRE numeric(12, 4);
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


\echo -n ':: Funcion que calcula el total de un albaran a cliente ... '
CREATE OR REPLACE FUNCTION calctotalalbaran(integer) RETURNS numeric(12, 2)
AS '
DECLARE
    idp ALIAS FOR $1;
    totalBImponibleLineas numeric(12, 4);
    totalIRPF numeric(12, 4);
    totalIVA numeric(12, 4);
    totalRE numeric(12, 4);
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




\echo -n ':: Se recalculan los totales de todas las lineas de los documentos ... '
CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER
AS '
BEGIN
	UPDATE lpresupuesto SET idlpresupuesto = idlpresupuesto;
	UPDATE lpedidocliente SET numlpedidocliente = numlpedidocliente;
	UPDATE lalbaran SET numlalbaran = numlalbaran;
	UPDATE lfactura SET idlfactura = idlfactura;
	
	UPDATE lpedidoproveedor SET idlpedidoproveedor = idlpedidoproveedor;
	UPDATE lalbaranp SET numlalbaranp = numlalbaranp;
	UPDATE lfacturap SET idlfacturap = idlfacturap;

	RETURN 0;
END;
' LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;




-- ================================== ACTUALIZACION  ===================================
-- =====================================================================================

-- Agregamos nuevos parametros de configuracion
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	as RECORD;
BEGIN
	SELECT INTO as * FROM configuracion WHERE nombre = ''DatabaseRevision'';
	IF FOUND THEN
		UPDATE CONFIGURACION SET valor = ''0.9.3-0009'' WHERE nombre = ''DatabaseRevision'';
	ELSE
		INSERT INTO configuracion (nombre, valor) VALUES (''DatabaseRevision'', ''0.9.3-0009'');
	END IF;
	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT actualizarevision();
DROP FUNCTION actualizarevision() CASCADE;
\echo "Actualizada la revision de la base de datos a la version 0.9.3"

DROP FUNCTION drop_if_exists_table(text) CASCADE;
DROP FUNCTION drop_if_exists_proc(text, text) CASCADE;


COMMIT;

