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




create or replace function is_number(varchar) returns boolean as
'select $1 ~ ''^[-+]?[0-9]+$''' strict immutable language sql;

CREATE OR REPLACE FUNCTION to_number(character varying) RETURNS INT8 AS '
DECLARE
BEGIN
	RAISE NOTICE ''to_number %'', $1;
        RETURN CAST(text($1) AS INT8);
END
' LANGUAGE 'plpgsql';


SELECT drop_if_exists_proc ('calculacodigocompletoarticulo','');
CREATE OR REPLACE FUNCTION calculacodigocompletoarticulo () RETURNS "trigger"
AS '
DECLARE
	as RECORD;
	codigocompleto character varying(100);
	codnumeric integer;
BEGIN
	-- Lo primero comprobamos el el código del articulo no esté vacio y de ser así lo llenamos.
	IF NEW.codarticulo = '''' THEN
		SELECT INTO as max(codarticulo) AS m FROM articulo WHERE idfamilia = NEW.idfamilia;
		IF FOUND THEN
			IF is_number(as.m) THEN
				codnumeric := to_number(as.m);
				codnumeric := codnumeric +1;
				NEW.codarticulo := CAST (codnumeric AS varchar);
				WHILE length(NEW.codarticulo) < 4 LOOP
					NEW.codarticulo := ''0'' || NEW.codarticulo;
				END LOOP;
			ELSE
				NEW.codarticulo := ''0000'';
			END IF;
		ELSE
			NEW.codarticulo = ''0000'';
		END IF;
	END IF;

	codigocompleto := NEW.codarticulo;
	SELECT INTO as codigocompletofamilia FROM familia WHERE idfamilia = NEW.idfamilia;
	IF FOUND THEN
		codigocompleto := as.codigocompletofamilia || codigocompleto;
	END IF;
        NEW.codigocompletoarticulo := codigocompleto;
	RETURN NEW;
END;
' LANGUAGE plpgsql;


CREATE TRIGGER calculacodigocompletoarticulotrigger
    BEFORE INSERT OR UPDATE ON articulo
    FOR EACH ROW
    EXECUTE PROCEDURE calculacodigocompletoarticulo();
\echo "Cambiamos el disparador de comprobaciones para articulos.."



CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
	as RECORD;
BEGIN
	IF (select count(*) from pg_tables where tablename=''dalbaranp'') THEN
		RETURN 1;
	END IF;

	CREATE TABLE dalbaranp (
	iddalbaranp serial PRIMARY KEY,
	conceptdalbaranp character varying(500),
	proporciondalbaranp numeric(12,2),
	idalbaranp integer NOT NULL REFERENCES albaran(idalbaran)
	);

	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo "Agregamos la tabla de descuentos de albaran proveedor."





CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
	as RECORD;
BEGIN
	IF (select count(*) from pg_tables where tablename=''dfacturap'') THEN
		RETURN 1;
	END IF;
	CREATE TABLE dfacturap (
	iddfacturap serial PRIMARY KEY,
	conceptdfacturap character varying(2000),
	proporciondfacturap numeric(12,2),
	idfacturap integer NOT NULL REFERENCES factura(idfactura)
	-- Falta poner el lugar donde se aplica el descuento, antes de la factura o despu� de �ta.
	); 
	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo "Agregamos la tabla de descuentos de factura proveedor."






-- Calculo de totales para presupuestos.
CREATE OR REPLACE FUNCTION calctotalpres(integer) RETURNS numeric(12,2)
AS '
DECLARE
idp ALIAS FOR $1;
total numeric(12,2);
res RECORD;
BEGIN
	total := 0;
	FOR  res IN SELECT cantlpresupuesto * pvplpresupuesto * (1 - descuentolpresupuesto/100) *(1+ ivalpresupuesto/100) AS subtotal1 FROM lpresupuesto WHERE idpresupuesto = idp LOOP
		total := total + res.subtotal1;
	END LOOP;
	FOR res IN SELECT proporciondpresupuesto FROM dpresupuesto WHERE idpresupuesto = idp LOOP
		total := total * (1 - res.proporciondpresupuesto/100);
	END LOOP;
	RETURN total;
END;
' language plpgsql;
\echo "Calculo de totales para presupuestos."


-- Cálculo de totales para pedido cliente.
CREATE OR REPLACE FUNCTION calctotalpedcli(integer) RETURNS numeric(12,2)
AS '
DECLARE
idp ALIAS FOR $1;
total numeric(12,2);
res RECORD;
BEGIN
	total := 0;
	FOR  res IN SELECT cantlpedidocliente * pvplpedidocliente * (1 - descuentolpedidocliente/100) *(1+ ivalpedidocliente/100) AS subtotal1 FROM lpedidocliente WHERE idpedidocliente = idp LOOP
		total := total + res.subtotal1;
	END LOOP;
	FOR res IN SELECT proporciondpedidocliente FROM dpedidocliente WHERE idpedidocliente = idp LOOP
		total := total * (1 - res.proporciondpedidocliente/100);
	END LOOP;
	RETURN total;
END;
' language plpgsql;
\echo "Cálculo de totales para pedidos de cliente."


-- Cálculo de totales para albaranes.
CREATE OR REPLACE FUNCTION calctotalalbaran(integer) RETURNS numeric(12,2)
AS '
DECLARE
idp ALIAS FOR $1;
total numeric(12,2);
res RECORD;
BEGIN
	total := 0;
	FOR  res IN SELECT cantlalbaran * pvplalbaran * (1 - descontlalbaran/100) *(1+ ivalalbaran/100) AS subtotal1 FROM lalbaran WHERE idalbaran = idp LOOP
		total := total + res.subtotal1;
	END LOOP;
	FOR res IN SELECT proporciondalbaran FROM dalbaran WHERE idalbaran = idp LOOP
		total := total * (1 - res.proporciondalbaran/100);
	END LOOP;
	RETURN total;
END;
' language plpgsql;
\echo "Cálculo de totales para albaranes."


-- Cálculo de totales para facturas
CREATE OR REPLACE FUNCTION calctotalfactura(integer) RETURNS numeric(12,2)
AS '
DECLARE
idp ALIAS FOR $1;
total numeric(12,2);
res RECORD;
BEGIN
	total := 0;
	FOR  res IN SELECT cantlfactura * pvplfactura * (1 - descuentolfactura/100) *(1+ ivalfactura/100) AS subtotal1 FROM lfactura WHERE idfactura = idp LOOP
		total := total + res.subtotal1;
	END LOOP;
	FOR res IN SELECT proporciondfactura FROM dfactura WHERE idfactura = idp LOOP
		total := total * (1 - res.proporciondfactura/100);
	END LOOP;
	RETURN total;
END;
' language plpgsql;
\echo "Cálculo de totales para albaranes."

-- Cálculo de totales para pedido proveedor
CREATE OR REPLACE FUNCTION calctotalpedpro(integer) RETURNS numeric(12,2)
AS '
DECLARE
idp ALIAS FOR $1;
total numeric(12,2);
res RECORD;
BEGIN
	total := 0;
	FOR  res IN SELECT cantlpedidoproveedor * pvplpedidoproveedor * (1 - descuentolpedidoproveedor/100) *(1+ ivalpedidoproveedor/100) AS subtotal1 FROM lpedidoproveedor WHERE idpedidoproveedor = idp LOOP
		total := total + res.subtotal1;
	END LOOP;
	FOR res IN SELECT proporciondpedidoproveedor FROM dpedidoproveedor WHERE idpedidoproveedor = idp LOOP
		total := total * (1 - res.proporciondpedidoproveedor/100);
	END LOOP;
	RETURN total;
END;
' language plpgsql;
\echo "Cálculo de totales para pedidos de proveedor"



-- Cálculo de totales para albaaran proveedor
CREATE OR REPLACE FUNCTION calctotalalbpro(integer) RETURNS numeric(12,2)
AS '
DECLARE
idp ALIAS FOR $1;
total numeric(12,2);
res RECORD;
BEGIN
	total := 0;
	FOR  res IN SELECT cantlalbaranp * pvplalbaranp * (1 - descontlalbaranp/100) *(1+ ivalalbaranp/100) AS subtotal1 FROM lalbaranp WHERE idalbaranp = idp LOOP
		total := total + res.subtotal1;
	END LOOP;
	FOR res IN SELECT proporciondalbaranp FROM dalbaranp WHERE idalbaranp = idp LOOP
		total := total * (1 - res.proporciondalbaranp/100);
	END LOOP;
	RETURN total;
END;
' language plpgsql;
\echo "Cálculo de totales para albaranes de proveedor"



-- Cálculo de totales para factura proveedor
CREATE OR REPLACE FUNCTION calctotalfacpro(integer) RETURNS numeric(12,2)
AS '
DECLARE
idp ALIAS FOR $1;
total numeric(12,2);
res RECORD;
BEGIN
	total := 0;
	FOR  res IN SELECT cantlfacturap * pvplfacturap * (1 - descuentolfacturap/100) *(1+ ivalfacturap/100) AS subtotal1 FROM lfacturap WHERE idfacturap = idp LOOP
		total := total + res.subtotal1;
	END LOOP;
	FOR res IN SELECT proporciondfacturap FROM dfacturap WHERE idfacturap = idp LOOP
		total := total * (1 - res.proporciondfacturap/100);
	END LOOP;
	RETURN total;
END;
' language plpgsql;
\echo "Cálculo de totales para albaranes de proveedor"




--
-- Agregamos el campo fractemitida que indica si es una factura emitida o recibida
--
CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
	as RECORD;
BEGIN
	SELECT INTO as * FROM pg_attribute  WHERE attname=''comentprivalbaran'';
	IF NOT FOUND THEN
		ALTER TABLE albaran ADD COLUMN comentprivalbaran character varying(3000);
	END IF;
	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo "Agregamos un segundo campo de comentarios para los albaranes."




-- Agregamos nuevos parametros de configuraci�.
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	as RECORD;
BEGIN
	SELECT INTO as * FROM configuracion WHERE nombre=''DatabaseRevision'';
	IF FOUND THEN
		UPDATE CONFIGURACION SET valor=''0.1.3'' WHERE nombre=''DatabaseRevision'';
	ELSE
		INSERT INTO configuracion (nombre, valor) VALUES (''DatabaseRevision'', ''0.1.3''); 		 
	END IF;
	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT actualizarevision();
DROP FUNCTION actualizarevision() CASCADE;
\echo "Actualizada la revisión de la base de datos a la versión 0.1.3"


DROP FUNCTION drop_if_exists_table(text) CASCADE;
DROP FUNCTION drop_if_exists_proc(text,text) CASCADE;


COMMIT;