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
	SELECT INTO as * FROM configuracion WHERE nombre=''DatabaseRevision'' AND ( valor LIKE ''0.11.1%'' OR valor = ''0.10.1-0005'');
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

-- PLUGINS

    END LOOP;
    RETURN result;
END;
' LANGUAGE plpgsql;


-- =====================================================================================

-- corregeixo calcul que multiplicava factor d'iva per factor de recarrec d'equivalencia
-- cosa qe eliminava o reduia massa els impostos
CREATE OR REPLACE FUNCTION calcimpuestosfactura(integer) RETURNS numeric(12, 2)
AS '
DECLARE
    idp ALIAS FOR $1;
    total numeric(12, 2);
    res RECORD;

BEGIN
    total := 0;
   FOR res IN SELECT cantlfactura * pvplfactura * (1 - descuentolfactura / 100) * ((ivalfactura+reqeqlfactura) / 100)  AS subtotal1 FROM lfactura WHERE idfactura = idp LOOP
	total := total + res.subtotal1;
    END LOOP;
    FOR res IN SELECT proporciondfactura FROM dfactura WHERE idfactura = idp LOOP
	total := total * (1 - res.proporciondfactura / 100);
    END LOOP;
    RETURN total;
END;
' LANGUAGE plpgsql;

-- para remesas de recibos a bancos en fornato q19
\echo -n ':: Agrega el campo de sufijo en la ficha de banco ... '
CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
        bs RECORD;
BEGIN
	SELECT INTO bs * FROM pg_attribute WHERE attname=''sufijobanco'';
	IF NOT FOUND THEN
           ALTER TABLE banco  ADD COLUMN sufijobanco numeric(3,0) ;
-- para compatibilidad con las bd de la branch docsMonolitic que usaban idbanco en lugar de 
-- sufijo
           UPDATE banco set sufijobanco = idbanco ;
	END IF;

	RETURN 0;
END;
' LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;


-- =====================================================================================

-- Agregamos nuevos parametros de configuracion
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	as RECORD;
BEGIN
	SELECT INTO as * FROM configuracion WHERE nombre = ''DatabaseRevision'';
	IF FOUND THEN
		UPDATE CONFIGURACION SET valor = ''0.11.1-0003'' WHERE nombre = ''DatabaseRevision'';
	ELSE
		INSERT INTO configuracion (nombre, valor) VALUES (''DatabaseRevision'', ''0.11.1-0003'');
	END IF;
	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT actualizarevision();
DROP FUNCTION actualizarevision() CASCADE;
\echo "Actualizada la revision de la base de datos a la version 0.11.1"

DROP FUNCTION drop_if_exists_table(text) CASCADE;
DROP FUNCTION drop_if_exists_proc(text, text) CASCADE;


COMMIT;

