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
	SELECT INTO as * FROM configuracion WHERE nombre=''DatabaseRevision'' AND ( valor LIKE ''0.9.1%'' OR valor = ''0.5.9-0005'');
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
	SELECT INTO as * FROM pg_attribute  WHERE attname=''ordenlpresupuesto'';
	IF NOT FOUND THEN
		ALTER TABLE lpresupuesto ADD COLUMN ordenlpresupuesto INTEGER;
	END IF;
	SELECT INTO as * FROM pg_attribute  WHERE attname=''ordenlpedidocliente'';
	IF NOT FOUND THEN
		ALTER TABLE lpedidocliente ADD COLUMN ordenlpedidocliente INTEGER;
		ALTER TABLE lfactura ADD COLUMN ordenlfactura INTEGER;
		ALTER TABLE lfacturap ADD COLUMN ordenlfacturap INTEGER;
		ALTER TABLE lalbaranp ADD COLUMN ordenlalbaranp INTEGER;
		ALTER TABLE lpedidoproveedor ADD COLUMN ordenlpedidoproveedor INTEGER;
	END IF;
	SELECT INTO as * FROM pg_attribute  WHERE attname=''ordenlalbaran'';
	IF NOT FOUND THEN
		ALTER TABLE lalbaran ADD COLUMN ordenlalbaran INTEGER;
	END IF;

	SELECT INTO as * FROM pg_attribute WHERE attname=''porcentretasa_iva'';
	IF NOT FOUND THEN
		ALTER TABLE tasa_iva ADD COLUMN porcentretasa_iva NUMERIC(5,2);
		ALTER TABLE tasa_iva ALTER COLUMN porcentretasa_iva SET DEFAULT 0;
		ALTER TABLE cliente ADD COLUMN recargoeqcliente BOOLEAN;
		ALTER TABLE cliente ALTER COLUMN recargoeqcliente SET DEFAULT FALSE;
		ALTER TABLE cliente ADD COLUMN idforma_pago integer;
		ALTER TABLE cliente ADD CONSTRAINT idforma_pagofk FOREIGN KEY (idforma_pago) REFERENCES forma_pago(idforma_pago);
		ALTER TABLE lpedido ADD COLUMN reqeqlpedido NUMERIC(12,2);
		ALTER TABLE lpedido ALTER COLUMN reqeqlpedido SET DEFAULT 0;
		ALTER TABLE lpresupuesto ADD COLUMN reqeqlpresupuesto NUMERIC(12,2);
		ALTER TABLE lpresupuesto ALTER COLUMN reqeqlpresupuesto SET DEFAULT 0;
		ALTER TABLE lpedidocliente ADD COLUMN reqeqlpedidocliente NUMERIC(12,2);
		ALTER TABLE lpedidocliente ALTER COLUMN reqeqlpedidocliente SET DEFAULT 0;
		ALTER TABLE lfactura ADD COLUMN reqeqlfactura NUMERIC(12,2);
		ALTER TABLE lfactura ALTER COLUMN reqeqlfactura SET DEFAULT 0;
		ALTER TABLE lfacturap ADD COLUMN reqeqlfacturap NUMERIC(12,2);
		ALTER TABLE lfacturap ALTER COLUMN reqeqlfacturap SET DEFAULT 0;
		ALTER TABLE lalbaran ADD COLUMN reqeqlalbaran NUMERIC(12,2);
		ALTER TABLE lalbaran ALTER COLUMN reqeqlalbaran SET DEFAULT 0;
		ALTER TABLE lpedidoproveedor ADD COLUMN reqeqlpedidoproveedor NUMERIC(12,2);
		ALTER TABLE lpedidoproveedor ALTER COLUMN reqeqlpedidoproveedor SET DEFAULT 0;
		ALTER TABLE lalbaranp ADD COLUMN reqeqlalbaranp NUMERIC(12,2);
		ALTER TABLE lalbaranp ALTER COLUMN reqeqlalbaranp SET DEFAULT 0;
	END IF;

	SELECT INTO as * FROM pg_attribute WHERE attname=''regimenfiscalcliente'';
	IF NOT FOUND THEN
		ALTER TABLE cliente ADD COLUMN regimenfiscalcliente CHARACTER VARYING(50);
		UPDATE cliente SET regimenfiscalcliente = ''Normal'';
		ALTER TABLE cliente ALTER COLUMN regimenfiscalcliente SET NOT NULL;
	END IF;
	-- Cambiamos el nombre del campo descontlalbaran en la tabla lalbaran
	SELECT INTO as * FROM pg_attribute WHERE attname=''descontlalbaran'';
	IF FOUND THEN
		ALTER TABLE lalbaran RENAME COLUMN descontlalbaran TO descuentolalbaran;
	END IF;

	-- Cambiamos el nombre del campo descontlalbaran en la tabla lalbaran
	SELECT INTO as * FROM pg_attribute WHERE attname=''descontlalbaranp'';
	IF FOUND THEN
		ALTER TABLE lalbaranp RENAME COLUMN descontlalbaranp TO descuentolalbaranp;
	END IF;

	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo "Agregamos los campos de totales para los presupuestos."


\echo -n ':: Funcion que calcula el total del albaran de proveedor ... '
CREATE OR REPLACE FUNCTION calctotalalbpro(integer) RETURNS numeric(12, 2)
AS '
DECLARE
    idp ALIAS FOR $1;
    total numeric(12, 2);
    res RECORD;

BEGIN
    total := 0;
    FOR res IN SELECT cantlalbaranp * pvplalbaranp * (1 - descuentolalbaranp / 100) * (1 + ivalalbaranp / 100) AS subtotal1 FROM lalbaranp WHERE idalbaranp = idp LOOP
	total := total + res.subtotal1;
    END LOOP;
    FOR res IN SELECT proporciondalbaranp FROM dalbaranp WHERE idalbaranp = idp LOOP
	total := total * (1 - res.proporciondalbaranp / 100);
    END LOOP;
    RETURN total;
END;
' LANGUAGE plpgsql;


\echo -n ':: Funcion que calcula la Base Imponible total de un albaran de proveedor ... '
CREATE OR REPLACE FUNCTION calcbimpalbpro(integer) RETURNS numeric(12, 2)
AS '
DECLARE
    idp ALIAS FOR $1;
    total numeric(12, 2);
    res RECORD;

BEGIN
    total := 0;
    FOR res IN SELECT cantlalbaranp * pvplalbaranp * (1 - descuentolalbaranp / 100) AS subtotal1 FROM lalbaranp WHERE idalbaranp = idp LOOP
	total := total + res.subtotal1;
    END LOOP;
    FOR res IN SELECT proporciondalbaranp FROM dalbaranp WHERE idalbaranp = idp LOOP
	total := total * (1 - res.proporciondalbaranp / 100);
    END LOOP;
    RETURN total;
END;
' LANGUAGE plpgsql;

\echo -n ':: Funcion que calcula el total de impuestos de un albaran de proveedor ... '
CREATE OR REPLACE FUNCTION calcimpuestosalbpro(integer) RETURNS numeric(12, 2)
AS '
DECLARE
    idp ALIAS FOR $1;
    total numeric(12, 2);
    res RECORD;

BEGIN
    total := 0;
    FOR res IN SELECT cantlalbaranp * pvplalbaranp * (1 - descuentolalbaranp / 100) * (ivalalbaranp / 100) AS subtotal1 FROM lalbaranp WHERE idalbaranp = idp LOOP
	total := total + res.subtotal1;
    END LOOP;
    FOR res IN SELECT proporciondalbaranp FROM dalbaranp WHERE idalbaranp = idp LOOP
	total := total * (1 - res.proporciondalbaranp / 100);
    END LOOP;
    RETURN total;
END;
' LANGUAGE plpgsql;


\echo -n ':: Funcion que calcula el total de un albaran a cliente ... '
CREATE OR REPLACE FUNCTION calctotalalbaran(integer) RETURNS numeric(12, 2)
AS '
DECLARE
    idp ALIAS FOR $1;
    total numeric(12, 2);
    res RECORD;

BEGIN
    total := 0;
    FOR res IN SELECT cantlalbaran * pvplalbaran * (1 - descuentolalbaran / 100) * (1 + ivalalbaran / 100) AS subtotal1 FROM lalbaran WHERE idalbaran = idp LOOP
	total := total + res.subtotal1;
    END LOOP;
    FOR res IN SELECT proporciondalbaran FROM dalbaran WHERE idalbaran = idp LOOP
    	total := total * (1 - res.proporciondalbaran / 100);
    END LOOP;
    RETURN total;
END;
' LANGUAGE plpgsql;


\echo -n ':: Funcion que calcula la Base Imponible total de un albaran a cliente ... '
CREATE OR REPLACE FUNCTION calcbimpalbaran(integer) RETURNS numeric(12, 2)
AS '
DECLARE
    idp ALIAS FOR $1;
    total numeric(12, 2);
    res RECORD;

BEGIN
    total := 0;
    FOR res IN SELECT cantlalbaran * pvplalbaran * (1 - descuentolalbaran / 100) AS subtotal1 FROM lalbaran WHERE idalbaran = idp LOOP
	total := total + res.subtotal1;
    END LOOP;
    FOR res IN SELECT proporciondalbaran FROM dalbaran WHERE idalbaran = idp LOOP
	total := total * (1 - res.proporciondalbaran / 100);
    END LOOP;
    RETURN total;
END;
' LANGUAGE plpgsql;


\echo -n ':: Funcion que calcula los impuestos totales de un albaran a cliente ... '
CREATE OR REPLACE FUNCTION calcimpuestosalbaran(integer) RETURNS numeric(12, 2)
AS '
DECLARE
    idp ALIAS FOR $1;
    total numeric(12, 2);
    res RECORD;

BEGIN
    total := 0;
    FOR res IN SELECT cantlalbaran * pvplalbaran * (1 - descuentolalbaran / 100) * (ivalalbaran / 100) AS subtotal1 FROM lalbaran WHERE idalbaran = idp LOOP
    	total := total + res.subtotal1;
    END LOOP;
    FOR res IN SELECT proporciondalbaran FROM dalbaran WHERE idalbaran = idp LOOP
    	total := total * (1 - res.proporciondalbaran / 100);
    END LOOP;
    RETURN total;
END;
' LANGUAGE plpgsql;


CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
	as RECORD;
BEGIN
	SELECT INTO as * FROM configuracion WHERE nombre=''IRPF'';
	IF NOT FOUND THEN
		INSERT INTO configuracion (nombre, valor) VALUES (''IRPF'', ''0''); 		 
	END IF;
	RETURN 0;
END;
'   LANGUAGE plpgsql;
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
	SELECT INTO as * FROM configuracion WHERE nombre=''DatabaseRevision'';
	IF FOUND THEN
		UPDATE CONFIGURACION SET valor=''0.9.1-0004'' WHERE nombre=''DatabaseRevision'';
	ELSE
		INSERT INTO configuracion (nombre, valor) VALUES (''DatabaseRevision'', ''0.9.1-0004''); 		 
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
