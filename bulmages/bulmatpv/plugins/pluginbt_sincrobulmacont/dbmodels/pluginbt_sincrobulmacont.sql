--
-- Modificación de campos y funciones de la BD para la adaptacion para el plugin de Trazabilidad
--
\echo "********* INICIADO FICHERO DE ESTRUCTURA DEL PLUGIN DE SINCRONIZACION DE TPV CON BULMACONT *********"

\echo ":: Establecemos los mensajes minimos a avisos y otros parametros ... "
\echo -n ":: "
SET client_min_messages TO WARNING;
SET log_min_messages TO WARNING;
-- SET log_error_verbosity TO TERSE;
BEGIN;

--
-- Estas primeras funciones cambiaran los tipos de columnas que est� como flotantes a NUMERIC.
-- Se trata de un parche que se desea aplicar para almacenar los tipos monetarios
-- ya que actualmente se encuantran almacenados como 'doubles' y es preferible
-- que se almacenen como tipo 'numeric'.
-- Todas devuelven como valor numeico el nmero de filas influenciadas por el cambio
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


CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS $$
DECLARE
	bs RECORD;
BEGIN
	SELECT INTO bs valor FROM configuracion WHERE nombre='DataBaseContabilidad';
	IF NOT FOUND THEN
		INSERT INTO configuracion (nombre, valor) VALUES ('DataBaseContabilidad', 'bulmacont');
	END IF;

	CREATE OR REPLACE view BC_Cuenta AS SELECT * FROM dblink( 'SELECT idcuenta, codigo, descripcion, tipocuenta FROM cuenta') AS t1 ( idcuenta integer, codigo varchar, descripcion varchar, tipocuenta integer);

	SELECT INTO bs * FROM pg_attribute WHERE attname='idasientoz';
	IF NOT FOUND THEN
		ALTER TABLE z ADD COLUMN idasientoz INTEGER;
	END IF;

	RETURN 0;
END;
$$   LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo "Agregamos los campos para almacenar claves foraneas en las tablas necesarias."



CREATE OR REPLACE FUNCTION conectabulmacont() RETURNS INTEGER
AS $$
DECLARE
	bs RECORD;
	quer TEXT;
BEGIN
	SELECT INTO bs * FROM configuracion WHERE nombre='DataBaseContabilidad';
	quer := 'dbname=' || bs.valor;
	PERFORM dblink_connect(quer);
	PERFORM dblink_exec( 'SET DATESTYLE TO SQL,European');
	RETURN 0;
END;
$$ LANGUAGE plpgsql;

-- =================================================================
--           TRATO DE LAS Z
-- =================================================================
SELECT drop_if_exists_proc ('syncbulmacontzd','');
CREATE FUNCTION syncbulmacontzd () RETURNS "trigger"
AS $$
DECLARE
	query TEXT;
BEGIN
	PERFORM conectabulmacont();
	-- Hacemos el update del stock del articulo
	IF OLD.idasientoz IS NOT NULL THEN
		query := 'DELETE FROM apunte WHERE idasiento= ' || OLD.idasientoz;
		PERFORM dblink_exec(query);
		query := 'DELETE FROM  borrador WHERE idasiento= ' || OLD.idasientoz;
		PERFORM dblink_exec(query);
		query := 'DELETE FROM  asiento WHERE idasiento = ' || OLD.idasientoz;
		PERFORM dblink_exec(query);
	END IF;

	PERFORM dblink_disconnect();
	RETURN OLD;
END;
$$ LANGUAGE plpgsql;


CREATE TRIGGER syncbulmacontztriggerd
    AFTER DELETE ON z
    FOR EACH ROW
    EXECUTE PROCEDURE syncbulmacontzd();
\echo "Creado el trigger que al borrar una factura borra su respectivo asiento en la contabilidad"



SELECT drop_if_exists_proc ('syncbulmacontzu','');
CREATE FUNCTION syncbulmacontzu () RETURNS "trigger"
AS $$
DECLARE
	cs RECORD;
	bs RECORD;
	ctaiva RECORD;
	idctacliente TEXT;
	query TEXT;
	subquery TEXT;
	idctapago INTEGER;
	cta TEXT;
	idcta INTEGER;
	flecha DATE;
	concepto TEXT;
	concepto1 TEXT;
	asientonuevo BOOLEAN;
	varaux TEXT;
BEGIN
	-- conectamos con contabilidad, etc
	PERFORM conectabulmacont();

	concepto := '[A. Automatico] TPV ' || NEW.fechaz || NEW.horaz;
	concepto1 := 'Cierre de Caja ' || NEW.fechaz || NEW.horaz;


	-- Puede darse el caso de que el contable haya borrado el asiento. Y por eso comprobamos que realmente exista en la contabilidad.
	asientonuevo := TRUE;
	IF NEW.idasientoz IS NOT NULL THEN
		query := 'SELECT idasiento FROM asiento WHERE idasiento =' || NEW.idz;
		SELECT INTO cs * FROM dblink(query) AS t1 (idasiento integer);
		IF FOUND THEN
			asientonuevo := FALSE;
		END IF;
	END IF;


	-- Hacemos el update del stock del articulo
	IF asientonuevo IS FALSE THEN
		query := 'DELETE FROM apunte WHERE idasiento= ' || NEW.idasientoz;
		PERFORM dblink_exec(query);
		query := 'DELETE FROM borrador WHERE idasiento= ' || NEW.idasientoz;
		PERFORM dblink_exec(query);
		query := 'UPDATE asiento SET descripcion = ''' || concepto || ''', comentariosasiento = ''' || concepto || ''' WHERE idasiento = ' || NEW.idasientoz;
		PERFORM dblink_exec(query);
	ELSE
		-- Hacemos el update del stock del articulo
		query := 'INSERT INTO asiento (fecha, descripcion, comentariosasiento) VALUES ( ' || quote_literal(NEW.fechaz) || ', ''' || concepto || ''', ''' || concepto || ''')';
		PERFORM dblink_exec(query);

		SELECT INTO cs * FROM dblink('SELECT max(idasiento) AS id FROM asiento') AS t1 (id integer);
		NEW.idasientoz = cs.id;
	END IF;

	-- El apunte de contado
	SELECT INTO bs SUM (cantlalbaran * pvpivainclalbaran) AS total FROM lalbaran NATURAL LEFT JOIN albaran WHERE idz = NEW.idz AND idforma_pago = 1;
	IF bs.total IS NOT NULL THEN
		-- Apunte opr el proveedor
		query := 'INSERT INTO borrador (fecha, idcuenta, debe, idasiento, descripcion, conceptocontable) VALUES (''' || NEW.fechaz || ''', id_cuenta(''57000001''), ' || bs.total || ', '|| NEW.idasientoz ||', ''' || concepto1 || ''', ''Cierre Caja'')';
		PERFORM dblink_exec(query);
	END IF;


	-- El apunte de banco
	SELECT INTO bs SUM (cantlalbaran * pvpivainclalbaran) AS total FROM lalbaran NATURAL LEFT JOIN albaran WHERE idz = NEW.idz AND idforma_pago = 2;
	IF bs.total IS NOT NULL THEN
		-- Apunte opr el proveedor
		query := 'INSERT INTO borrador (fecha, idcuenta, debe, idasiento, descripcion, conceptocontable) VALUES (''' || NEW.fechaz || ''', id_cuenta(''57000002''), ' || bs.total || ', '|| NEW.idasientoz ||', ''' || concepto1 || ''', ''Cierre Caja'')';
		PERFORM dblink_exec(query);
	END IF;


	-- Tratamos los distintos tipos de IVAe
	FOR cs IN SELECT round(ivalalbaran) AS iva, SUM(cantlalbaran * pvpivainclalbaran / ( 1+ ivalalbaran/100)) AS totiva FROM lalbaran WHERE lalbaran.idalbaran IN ( SELECT  idalbaran FROM albaran WHERE idz = NEW.idz) GROUP BY ivalalbaran LOOP
		varaux := '47200%' || cs.iva;
		-- Hacemos la busqueda de la cuenta de IVA correspondiente.
		SELECT INTO ctaiva * FROM bc_cuenta WHERE codigo LIKE varaux;
		IF NOT FOUND THEN
			RAISE EXCEPTION 'No existe una cuenta de IVA para el tipo de IVA %', cs.iva;
		END IF;
		cta := ctaiva.codigo;
		idcta := ctaiva.idcuenta;
		
		-- Apunte opr el proveedor
		query := 'INSERT INTO borrador (fecha, idcuenta, haber, idasiento, descripcion, conceptocontable) VALUES (''' || NEW.fechaz || ''', ' || idcta || ', ' || cs.totiva || ', '|| NEW.idasientoz ||', ''' || concepto1 || ''', ''Cierre Caja'')';
		PERFORM dblink_exec(query);
	END LOOP;


	-- Tratamos la venta de mercaderias
	FOR cs IN SELECT SUM(cantlalbaran * pvpivainclalbaran * (1- 1 / ( 1 + ivalalbaran/100))) AS totalbimp FROM lalbaran WHERE lalbaran.idalbaran IN ( SELECT  idalbaran FROM albaran WHERE idz = NEW.idz) LOOP
		varaux := '70000%';
		-- Hacemos la busqueda de la cuenta de IVA correspondiente.
		SELECT INTO ctaiva * FROM bc_cuenta WHERE codigo LIKE varaux;
		IF NOT FOUND THEN
			RAISE EXCEPTION 'No existe una cuenta de Venta de Mercaderias %', cs.iva;
		END IF;
		cta := ctaiva.codigo;
		idcta := ctaiva.idcuenta;
		
		-- Apunte opr el proveedor
		query := 'INSERT INTO borrador (fecha, idcuenta, haber, idasiento, descripcion, conceptocontable) VALUES (''' || NEW.fechaz || ''', ' || idcta || ', ' || cs.totalbimp || ', '|| NEW.idasientoz ||', ''' || concepto1 || ''', ''Cierre Caja'')';
		PERFORM dblink_exec(query);
	END LOOP;

	PERFORM dblink_disconnect();
	RETURN NEW;
END;
$$ LANGUAGE plpgsql;


CREATE TRIGGER syncbulmacontztriggeru
    BEFORE UPDATE ON z
    FOR EACH ROW
    EXECUTE PROCEDURE syncbulmacontzu();

\echo "Creado el trigger que al modificar o insertar una z la facturacion mete el correspondiente asiento en la contabilidad"

-- ==============================================================================

-- Agregamos nuevos parametros de configuracion.
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	rs RECORD;

BEGIN
	SELECT INTO rs * FROM configuracion WHERE nombre=''PluginBt_SincroBulmaCont'';

	IF FOUND THEN
		UPDATE CONFIGURACION SET valor=''0.11.1-001'' WHERE nombre=''PluginBt_SincroBulmaCont'';
	ELSE
		INSERT INTO configuracion (nombre, valor) VALUES (''PluginBt_SincroBulmaCont'', ''0.11.1-001'');
	END IF;

	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT actualizarevision();
DROP FUNCTION actualizarevision() CASCADE;
\echo "Actualizada la revision de la base de datos a la version 0.11.1"


DROP FUNCTION drop_if_exists_table(text) CASCADE;
DROP FUNCTION drop_if_exists_proc(text,text) CASCADE;


COMMIT;