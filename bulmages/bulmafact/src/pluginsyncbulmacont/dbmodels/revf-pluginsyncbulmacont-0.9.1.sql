--
-- Modificación de campos y funciones de la BD para la adaptacion para el plugin de Trazabilidad
--
\echo "********* INICIADO FICHERO DE ESTRUCTURA DEL PLUGIN DE SINCRONIZACION CON BULMACONT *********"

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

	SELECT INTO bs viewname FROM pg_views WHERE viewname='bc_cuenta';
	IF FOUND THEN
		DROP VIEW bc_cuenta;
	END IF;
	CREATE OR REPLACE view BC_Cuenta AS SELECT * FROM dblink( 'SELECT idgrupo, idcuenta, codigo, descripcion, tipocuenta FROM cuenta') AS t1 (idgrupo integer, idcuenta integer, codigo varchar, descripcion varchar, tipocuenta integer);

	CREATE OR REPLACE VIEW BC_Asiento AS SELECT * FROM dblink( 'SELECT idasiento FROM asiento') AS t1 (idasiento integer);

	CREATE OR REPLACE VIEW BC_Borrador AS SELECT * FROM dblink( 'SELECT idborrador FROM borrador') AS t1 (idborrador integer);

	CREATE OR REPLACE VIEW BC_Apunte AS SELECT * FROM dblink( 'SELECT idapunte FROM apunte') AS t1 (idapunte integer);

	SELECT INTO bs * FROM pg_attribute WHERE attname='idasientofactura';
	IF NOT FOUND THEN
		ALTER TABLE factura ADD COLUMN idasientofactura INTEGER;
	END IF;

	SELECT INTO bs * FROM pg_attribute WHERE attname='idcuentacliente';
	IF NOT FOUND THEN
		ALTER TABLE cliente ADD COLUMN idcuentacliente INTEGER;
	END IF;


	SELECT INTO bs * FROM pg_attribute WHERE attname='idasientofacturap';
	IF NOT FOUND THEN
		ALTER TABLE facturap ADD COLUMN idasientofacturap INTEGER;
	END IF;

	SELECT INTO bs * FROM pg_attribute WHERE attname='idcuentaproveedor';
	IF NOT FOUND THEN
		ALTER TABLE proveedor ADD COLUMN idcuentaproveedor INTEGER;
	END IF;

	-- Creamos el espacio para almacenar el identificador de cuenta que corresponde al banco.
	SELECT INTO bs * FROM pg_attribute WHERE attname='idcuentabanco';
	IF NOT FOUND THEN
		ALTER TABLE banco ADD COLUMN idcuentabanco INTEGER;
	END IF;

	-- Creamos el espacio para almacenar el identificador de cuenta que corresponde al almacen
	SELECT INTO bs * FROM pg_attribute WHERE attname='idcuentaalmacen';
	IF NOT FOUND THEN
		ALTER TABLE almacen ADD COLUMN idcuentaalmacen INTEGER;
	END IF;

	-- Creamos el espacio para almacenar el identificador de cuenta que corresponde al cobro
	SELECT INTO bs * FROM pg_attribute WHERE attname='idasientocobro';
	IF NOT FOUND THEN
		ALTER TABLE cobro ADD COLUMN idasientocobro INTEGER;
	END IF;
	-- Creamos el espacio para almacenar el identificador de cuenta que corresponde al pago
	SELECT INTO bs * FROM pg_attribute WHERE attname='idasientopago';
	IF NOT FOUND THEN
		ALTER TABLE pago ADD COLUMN idasientopago INTEGER;
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
--           TRATO DE LOS PAGOS
-- =================================================================
SELECT drop_if_exists_proc ('syncbulmacontpagod','');
CREATE FUNCTION syncbulmacontpagod () RETURNS "trigger"
AS $$
DECLARE
	query TEXT;
BEGIN
	PERFORM conectabulmacont();
	-- Hacemos el update del stock del articulo
	IF OLD.idasientopago IS NOT NULL THEN
		query := 'DELETE FROM apunte WHERE idasiento= ' || OLD.idasientopago;
		PERFORM dblink_exec(query);
		query := 'DELETE FROM  borrador WHERE idasiento= ' || OLD.idasientopago ;
		PERFORM dblink_exec(query);
		query := 'DELETE FROM  asiento WHERE idasiento = ' || OLD.idasientopago ;
		PERFORM dblink_exec(query);
	END IF;

	PERFORM dblink_disconnect();
	RETURN OLD;
END;
$$ LANGUAGE plpgsql;


CREATE TRIGGER syncbulmacontpagotriggerd
    AFTER DELETE ON pago
    FOR EACH ROW
    EXECUTE PROCEDURE syncbulmacontpagod();
\echo "Creado el trigger que al borrar una factura borra su respectivo asiento en la contabilidad"



SELECT drop_if_exists_proc ('syncbulmacontpagou','');
CREATE FUNCTION syncbulmacontpagou () RETURNS "trigger"
AS $$
DECLARE
	cs RECORD;
	client RECORD;
	qbanco RECORD;
	idctacliente TEXT;
	query TEXT;
	subquery TEXT;
	idctapago INTEGER;
	flecha DATE;
	concepto TEXT;
	concepto1 TEXT;
BEGIN
	-- conectamos con contabilidad, etc
	PERFORM conectabulmacont();

	concepto := '[A. Automatico] Pago ' || NEW.refpago;
	concepto1 := 'Pago ' || NEW.refpago;

	-- Hacemos el update del stock del articulo
	IF NEW.idasientopago IS NOT NULL THEN
		query := 'DELETE FROM apunte WHERE idasiento= ' || NEW.idasientopago;
		PERFORM dblink_exec(query);
		query := 'DELETE FROM borrador WHERE idasiento= ' || NEW.idasientopago;
		PERFORM dblink_exec(query);
		query := 'UPDATE asiento SET descripcion = ''' || concepto || ''', comentariosasiento = ''' || concepto || ''' WHERE idasiento = ' || NEW.idasientopago;
		PERFORM dblink_exec(query);
	ELSE
		-- Hacemos el update del stock del articulo
		query := 'INSERT INTO asiento (fecha, descripcion, comentariosasiento) VALUES ( ' || quote_literal(NEW.fechapago) || ', ''' || concepto || ''', ''' || concepto || ''')';
		PERFORM dblink_exec(query);

		SELECT INTO cs * FROM dblink('SELECT max(idasiento) AS id FROM asiento') AS t1 (id integer);
		NEW.idasientopago = cs.id;
	END IF;


	-- Buscamos el cliente y su cuenta.
	SELECT INTO client idcuentaproveedor FROM proveedor WHERE idproveedor = NEW.idproveedor;
	IF NOT FOUND THEN
		RAISE EXCEPTION 'El cliente no tiene cuenta asociada en la contabilidad';
	END IF;
	idctacliente := client.idcuentaproveedor;


	-- Buscamos el banco
	IF NEW.idbanco IS NOT NULL THEN
		SELECT INTO qbanco idcuentabanco FROM banco WHERE idbanco = NEW.idbanco;
		IF NOT FOUND THEN
			RAISE EXCEPTION 'El banco no tiene cuenta asociada en la contabilidad';
		END IF;
		idctapago := qbanco.idcuentabanco;
	ELSE
		-- Buscamos la cuenta de servicio o de venta
		SELECT INTO cs MAX(idcuenta) AS id FROM bc_cuenta WHERE codigo LIKE '5700%';
		IF NOT FOUND THEN
			RAISE EXCEPTION 'No existe ls cuenta de Caja 5700...';
		END IF;
		idctapago := cs.id;
	END IF;



	-- Apunte opr el proveedor
	query := 'INSERT INTO borrador (fecha, idcuenta, debe, idasiento, descripcion, conceptocontable) VALUES (''' || NEW.fechapago || ''', ' || idctacliente || ', ' || NEW.cantpago || ', '|| NEW.idasientopago ||', ''' || concepto1 || ''', ''Pago'')';
	PERFORM dblink_exec(query);


	-- Apunte por caja o banco.
	query := 'INSERT INTO borrador (fecha, idcuenta, haber, idasiento, descripcion, conceptocontable) VALUES (''' || NEW.fechapago || ''', ' || idctapago || ', ' || NEW.cantpago || ', '|| NEW.idasientopago ||', ''' || concepto1 || ''', ''Pago'')';
	PERFORM dblink_exec(query);


	PERFORM dblink_disconnect();
	RETURN NEW;
END;
$$ LANGUAGE plpgsql;


CREATE TRIGGER syncbulmacontpagotriggeru
    BEFORE INSERT OR UPDATE ON pago
    FOR EACH ROW
    EXECUTE PROCEDURE syncbulmacontpagou();

\echo "Creado el trigger que al modificar o insertar una factura en la facturacion mete el correspondiente asiento en la contabilidad"

-- =================================================================
--           TRATO DE LOS COBROS
-- =================================================================
SELECT drop_if_exists_proc ('syncbulmacontcobrod','');
CREATE FUNCTION syncbulmacontcobrod () RETURNS "trigger"
AS $$
DECLARE
	query TEXT;
BEGIN
	PERFORM conectabulmacont();
	-- Hacemos el update del stock del articulo
	IF OLD.idasientocobro IS NOT NULL THEN
		query := 'DELETE FROM apunte WHERE idasiento= ' || OLD.idasientocobro;
		PERFORM dblink_exec(query);
		query := 'DELETE FROM  borrador WHERE idasiento= ' || OLD.idasientocobro ;
		PERFORM dblink_exec(query);
		query := 'DELETE FROM  asiento WHERE idasiento = ' || OLD.idasientocobro;
		PERFORM dblink_exec(query);
	END IF;

	PERFORM dblink_disconnect();
	RETURN OLD;
END;
$$ LANGUAGE plpgsql;


CREATE TRIGGER syncbulmacontcobrotriggerd
    AFTER DELETE ON cobro
    FOR EACH ROW
    EXECUTE PROCEDURE syncbulmacontcobrod();
\echo "Creado el trigger que al borrar un cobro borra su respectivo asiento en la contabilidad"



SELECT drop_if_exists_proc ('syncbulmacontcobrou','');
CREATE FUNCTION syncbulmacontcobrou () RETURNS "trigger"
AS $$
DECLARE
	cs RECORD;
	client RECORD;
	qbanco RECORD;
	idctacliente TEXT;
	query TEXT;
	subquery TEXT;
	idctacobro INTEGER;
	flecha DATE;
	concepto TEXT;
	concepto1 TEXT;
BEGIN
	-- conectamos con contabilidad, etc
	PERFORM conectabulmacont();

	concepto := '[A. Automatico] Cobro ' || NEW.refcobro;
	concepto1 := 'Cobro ' || NEW.refcobro;

	-- Hacemos el update del stock del articulo
	IF NEW.idasientocobro IS NOT NULL THEN
		query := 'DELETE FROM apunte WHERE idasiento= ' || NEW.idasientocobro;
		PERFORM dblink_exec(query);
		query := 'DELETE FROM borrador WHERE idasiento= ' || NEW.idasientocobro;
		PERFORM dblink_exec(query);
		query := 'UPDATE asiento SET descripcion = ''' || concepto || ''', comentariosasiento = ''' || concepto || ''' WHERE idasiento = ' || NEW.idasientocobro;
		PERFORM dblink_exec(query);
	ELSE
		-- Hacemos el update del stock del articulo
		query := 'INSERT INTO asiento (fecha, descripcion, comentariosasiento) VALUES ( ' || quote_literal(NEW.fechacobro) || ', ''' || concepto || ''', ''' || concepto || ''')';
		PERFORM dblink_exec(query);

		SELECT INTO cs * FROM dblink('SELECT max(idasiento) AS id FROM asiento') AS t1 (id integer);
		NEW.idasientocobro = cs.id;
	END IF;


	-- Buscamos el cliente y su cuenta.
	SELECT INTO client idcuentacliente FROM cliente WHERE idcliente = NEW.idcliente;
	IF NOT FOUND THEN
		RAISE EXCEPTION 'El cliente no tiene cuenta asociada en la contabilidad';
	END IF;
	idctacliente := client.idcuentacliente;


	-- Buscamos el banco
	IF NEW.idbanco IS NOT NULL THEN
		SELECT INTO qbanco idcuentabanco FROM banco WHERE idbanco = NEW.idbanco;
		IF NOT FOUND THEN
			RAISE EXCEPTION 'El banco no tiene cuenta asociada en la contabilidad';
		END IF;
		idctacobro := qbanco.idcuentabanco;
	ELSE
		-- Buscamos la cuenta de servicio o de venta
		SELECT INTO cs MAX(idcuenta) AS id FROM bc_cuenta WHERE codigo LIKE '5700%';
		IF NOT FOUND THEN
			RAISE EXCEPTION 'No existe ls cuenta de Caja 5700...';
		END IF;
		idctacobro := cs.id;
	END IF;



	-- Apunte opr el proveedor
	query := 'INSERT INTO borrador (fecha, idcuenta, haber, idasiento, descripcion, conceptocontable) VALUES (''' || NEW.fechacobro || ''', ' || idctacliente || ', ' || NEW.cantcobro || ', '|| NEW.idasientocobro ||', ''' || concepto1 || ''', ''Cobro'')';
	PERFORM dblink_exec(query);


	-- Apunte por caja o banco.
	query := 'INSERT INTO borrador (fecha, idcuenta, debe, idasiento, descripcion, conceptocontable) VALUES (''' || NEW.fechacobro || ''', ' || idctacobro || ', ' || NEW.cantcobro || ', '|| NEW.idasientocobro ||', ''' || concepto1 || ''', ''Cobro'')';
	PERFORM dblink_exec(query);


	PERFORM dblink_disconnect();
	RETURN NEW;
END;
$$ LANGUAGE plpgsql;


CREATE TRIGGER syncbulmacontcobrotriggeru
    BEFORE INSERT OR UPDATE ON cobro
    FOR EACH ROW
    EXECUTE PROCEDURE syncbulmacontcobrou();

\echo "Creado el trigger que al modificar o insertar una factura en la facturacion mete el correspondiente asiento en la contabilidad"
-- =================================================================
--           TRATO DE LAS FACTURAS
-- =================================================================
SELECT drop_if_exists_proc ('syncbulmacontfacturad','');
CREATE FUNCTION syncbulmacontfacturad () RETURNS "trigger"
AS $$
DECLARE
	query TEXT;
BEGIN
	PERFORM conectabulmacont();
	-- Hacemos el update del stock del articulo
	IF OLD.idasientofactura IS NOT NULL THEN
		query := 'DELETE FROM registroiva WHERE idborrador IN (SELECT idborrador FROM borrador WHERE idasiento = ' || OLD.idasientofactura || ')';
		PERFORM dblink_exec(query);
		query := 'DELETE FROM apunte WHERE idasiento= ' || OLD.idasientofactura;
		PERFORM dblink_exec(query);
		query := 'DELETE FROM  borrador WHERE idasiento= ' || OLD.idasientofactura ;
		PERFORM dblink_exec(query);
		query := 'DELETE FROM  asiento WHERE idasiento = ' || OLD.idasientofactura ;
		PERFORM dblink_exec(query);
	END IF;

	PERFORM dblink_disconnect();
	RETURN OLD;
END;
$$ LANGUAGE plpgsql;


CREATE TRIGGER syncbulmacontfacturatriggerd
    AFTER DELETE ON factura
    FOR EACH ROW
    EXECUTE PROCEDURE syncbulmacontfacturad();
\echo "Creado el trigger que al borrar una factura borra su respectivo asiento en la contabilidad"



SELECT drop_if_exists_proc ('syncbulmacontfacturau','');
CREATE FUNCTION syncbulmacontfacturau () RETURNS "trigger"
AS $$
DECLARE
	bs RECORD;
	cs RECORD;
	client RECORD;
	ctaiva RECORD;
	cta TEXT;
	idcta TEXT;
	idctacliente TEXT;
	flecha DATE;
	query TEXT;
	subquery TEXT;
	varaux TEXT;
	idctaserv TEXT;
	porirpf NUMERIC;
	totaldesc NUMERIC;
	idctairpf TEXT;
	totaliva NUMERIC;
	concepto TEXT;
	concepto1 TEXT;
BEGIN
	-- conectamos con contabilidad, etc
	PERFORM conectabulmacont();
	totaliva := 0;

	concepto := '[A. Automatico] Factura Cliente Num.' || NEW.numfactura;
	concepto1 := 'Factura Cliente Num. ' || NEW.numfactura;

	-- Hacemos el update del stock del articulo
	IF NEW.idasientofactura IS NOT NULL THEN
		query := 'DELETE FROM registroiva WHERE idborrador IN (SELECT idborrador FROM borrador WHERE idasiento = ' || NEW.idasientofactura || ')';
		PERFORM dblink_exec(query);
		query := 'DELETE FROM apunte WHERE idasiento= ' || NEW.idasientofactura;
		PERFORM dblink_exec(query);
		query := 'DELETE FROM borrador WHERE idasiento= ' || NEW.idasientofactura;
		PERFORM dblink_exec(query);
		query := 'UPDATE asiento SET descripcion =''' || concepto || ''', comentariosasiento = ''' || concepto || ''' WHERE idasiento = ' || NEW.idasientofactura;
		PERFORM dblink_exec(query);
	ELSE
		-- Hacemos el update del stock del articulo
		query := 'INSERT INTO asiento (fecha, descripcion, comentariosasiento) VALUES ( ' || quote_literal(NEW.ffactura) || ', ''' || concpeto || ''' , ''' || concepto || ''')';
		PERFORM dblink_exec(query);
		SELECT INTO bs * FROM dblink('SELECT max(idasiento) AS id FROM asiento') AS t1 (id integer);
		NEW.idasientofactura = bs.id;
	END IF;


	-- Buscamos el cliente y su cuenta.
	SELECT INTO client idcuentacliente FROM cliente WHERE idcliente=NEW.idcliente;
	IF NOT FOUND THEN
		RAISE EXCEPTION 'El cliente no tiene cuenta asociada en la contabilidad';
	END IF;
	idctacliente := client.idcuentacliente;

	-- Buscamos la cuenta de servicio o de venta
	SELECT INTO cs MAX(idcuenta) AS id FROM bc_cuenta WHERE codigo LIKE '7000%';
	IF NOT FOUND THEN
		RAISE EXCEPTION 'No existe ls cuenta de Ventas 7000...';
	END IF;
	idctaserv := cs.id;

	-- Buscamos la cuenta de IRPF
	SELECT INTO cs MAX(idcuenta) AS id FROM bc_cuenta WHERE codigo LIKE '4730%';
	IF NOT FOUND THEN
		RAISE EXCEPTION 'No existe ls cuenta de IRPF 4730...';
	END IF;
	idctairpf := cs.id;


	-- Buscamos el IRP que aplicamos
	SELECT INTO bs valor::NUMERIC / 100 as val FROM configuracion WHERE nombre ='IRPF';
	IF FOUND THEN
		porirpf := bs.val;
	ELSE
		porirpf := 0;
	END IF;

	-- Buscamos los decuentos generales a aplicar.
	SELECT INTO bs 1 - SUM(proporciondfactura) / 100 AS tdesc FROM dfactura WHERE idfactura = NEW.idfactura;
	IF FOUND THEN
		totaldesc := bs.tdesc;
	END IF;
	totaldesc := 1;

	-- Creamos los apuntes de IVA.
	FOR  bs IN SELECT SUM (cantlfactura*pvplfactura*(100-descuentolfactura)/100) AS base, round(ivalfactura) AS iva, ivalfactura, reqeqlfactura FROM lfactura  WHERE idfactura = NEW.idfactura GROUP BY ivalfactura, reqeqlfactura LOOP
		IF bs.iva <> 0 THEN
			varaux := '4770%' || bs.iva;
			-- Hacemos la busqueda de la cuenta de IVA correspondiente.
			SELECT INTO ctaiva * FROM bc_cuenta WHERE codigo LIKE varaux;
			IF NOT FOUND THEN
				RAISE EXCEPTION 'No existe una cuenta de IVA para el tipo de IVA %', bs.iva;
			END IF;
			cta := ctaiva.codigo;
			idcta := ctaiva.idcuenta;

			-- Hacemos la insercion del borrador del apunte.
			query := 'INSERT INTO borrador (fecha, idcuenta, haber, idasiento, descripcion, conceptocontable) VALUES (''' || NEW.ffactura || ''', ' || idcta || ', ' || bs.base * totaldesc * bs.iva / 100 || ', '|| NEW.idasientofactura ||', ''' || concepto1 || ''' ,  ''Factura Cliente'')';
			PERFORM dblink_exec(query);

			-- Vamos calculando el total de IVA
			totaliva := totaliva + bs.base * totaldesc * bs.iva / 100;
		END IF;
		IF bs.reqeqlfactura <> 0 THEN
			varaux := '47701%' || bs.iva;
			-- Hacemos la busqueda de la cuenta de Recargo correspondiente.
			SELECT INTO ctaiva * FROM bc_cuenta WHERE codigo LIKE varaux;
			IF NOT FOUND THEN
				RAISE EXCEPTION 'No existe una cuenta de Recargo para el tipo de Recargo %', bs.reqeqlfactura;
			END IF;
			cta := ctaiva.codigo;
			idcta := ctaiva.idcuenta;

			-- Hacemos la insercion del borrador del apunte.
			query := 'INSERT INTO borrador (fecha, idcuenta, haber, idasiento, descripcion, conceptocontable) VALUES (''' || NEW.ffactura || ''', ' || idcta || ', ' || bs.base * totaldesc * bs.reqeqlfactura / 100 || ', '|| NEW.idasientofactura ||', ''' || concepto1 || ''' ,  ''Factura Cliente'')';
			PERFORM dblink_exec(query);

			-- Vamos calculando el total de IVA
			totaliva := totaliva + bs.base * totaldesc * bs.reqeqlfactura / 100;
		END IF;
	END LOOP;



	-- Creamos el apunte de cliente.
	SELECT INTO bs SUM (cantlfactura*pvplfactura*(100-descuentolfactura)/100) AS base FROM lfactura WHERE idfactura = NEW.idfactura;
	-- Hacemos la insercion del borrador del apunte.
	query := 'INSERT INTO borrador (fecha, idcuenta, debe, idasiento, descripcion, conceptocontable) VALUES (''' || NEW.ffactura || ''', ' || idctacliente || ', ' || bs.base * totaldesc * (1-porirpf) + totaliva || ', '|| NEW.idasientofactura ||', ''' || concepto1 || ''' ,  ''Factura Cliente'')';
	PERFORM dblink_exec(query);


	-- Creamos el apunte de IRPF
	IF porirpf <> 0 THEN
		-- Hacemos la insercion del borrador del apunte.
		query := 'INSERT INTO borrador (fecha, idcuenta, debe, idasiento, descripcion, conceptocontable) VALUES (''' || NEW.ffactura || ''', ' || idctairpf || ', ' || bs.base * totaldesc * porirpf || ', '|| NEW.idasientofactura ||', ''' || concepto1 || ''' ,  ''Factura Cliente'')';
		PERFORM dblink_exec(query);
	END IF;


	-- Creamos el apunte de servicio.
	-- Hacemos la insercion del borrador del apunte.
	query := 'INSERT INTO borrador (fecha, idcuenta, haber, idasiento, descripcion, conceptocontable) VALUES (''' || NEW.ffactura || ''', ' || idctaserv || ', ' || bs.base * totaldesc || ', '|| NEW.idasientofactura ||', ''' || concepto1 || ''' ,  ''Factura Cliente'')';
	PERFORM dblink_exec(query);


	PERFORM dblink_disconnect();
	RETURN NEW;
END;
$$ LANGUAGE plpgsql;


CREATE TRIGGER syncbulmacontfacturatriggeru
    BEFORE INSERT OR UPDATE ON factura
    FOR EACH ROW
    EXECUTE PROCEDURE syncbulmacontfacturau();

\echo "Creado el trigger que al modificar o insertar una factura en la facturacion mete el correspondiente asiento en la contabilidad"


-- =================================================================
--           TRATO DE LAS FACTURAS DE PROVEEDOR
-- =================================================================

SELECT drop_if_exists_proc ('syncbulmacontfacturapd','');
CREATE FUNCTION syncbulmacontfacturapd () RETURNS "trigger"
AS $$
DECLARE
	query TEXT;
BEGIN
	PERFORM conectabulmacont();
	-- Hacemos el update del stock del articulo
	IF OLD.idasientofacturap IS NOT NULL THEN
		query := 'DELETE FROM registroiva WHERE idborrador IN (SELECT idborrador FROM borrador WHERE idasiento = ' || OLD.idasientofacturap || ')';
		PERFORM dblink_exec(query);
		query := 'DELETE FROM apunte WHERE idasiento= ' || OLD.idasientofacturap;
		PERFORM dblink_exec(query);
		query := 'DELETE FROM  borrador WHERE idasiento= ' || OLD.idasientofacturap ;
		PERFORM dblink_exec(query);
		query := 'DELETE FROM  asiento WHERE idasiento = ' || OLD.idasientofacturap ;
		PERFORM dblink_exec(query);
	END IF;

	PERFORM dblink_disconnect();
	RETURN OLD;
END;
$$ LANGUAGE plpgsql;


CREATE TRIGGER syncbulmacontfacturaptriggerd
    AFTER DELETE ON facturap
    FOR EACH ROW
    EXECUTE PROCEDURE syncbulmacontfacturapd();

\echo "Creado el Trigger que al borrar una factura de proveedor la borra de la contabilidad"



SELECT drop_if_exists_proc ('syncbulmacontfacturapu','');
CREATE FUNCTION syncbulmacontfacturapu () RETURNS "trigger"
AS $$
DECLARE
	bs RECORD;
	cs RECORD;
	client RECORD;
	ctaiva RECORD;
	cta TEXT;
	idcta TEXT;
	idctacliente TEXT;
	query TEXT;
	varaux TEXT;
	idctaserv TEXT;
	idctairpf TEXT;
	porirpf NUMERIC := 0;
	totaldesc NUMERIC := 1;
	totaliva NUMERIC := 0;
	total NUMERIC := 0;
	concepto TEXT;
	concepto1 TEXT;
BEGIN
	-- conectamos con contabilidad, etc
	PERFORM conectabulmacont();
	concepto := '[A.Automatico] Factura Proveedor Num:' || NEW.numfacturap;
	concepto1 := 'Factura Proveedor Num. ' || NEW.numfacturap;

	-- Hacemos el update del stock del articulo
	IF NEW.idasientofacturap IS NOT NULL THEN
		query := 'DELETE FROM registroiva WHERE idborrador IN (SELECT idborrador FROM borrador WHERE idasiento = ' || NEW.idasientofacturap || ')';
		PERFORM dblink_exec(query);
		query := 'DELETE FROM apunte WHERE idasiento= ' || NEW.idasientofacturap;
		PERFORM dblink_exec(query);
		query := 'DELETE FROM borrador WHERE idasiento= ' || NEW.idasientofacturap;
		PERFORM dblink_exec(query);
		query := 'UPDATE asiento SET descripcion =''' || concepto || ''', comentariosasiento = ''' || concepto || ''' WHERE idasiento = ' || NEW.idasientofacturap;
		PERFORM dblink_exec(query);
	ELSE
		-- Hacemos el update del stock del articulo
		query := 'INSERT INTO asiento (fecha, descripcion, comentariosasiento) VALUES ( ' || quote_literal(NEW.ffactura) || ', ''' || concepto ||  ''', ''' || concepto || ''')';
		PERFORM dblink_exec(query);
		SELECT INTO bs * FROM dblink('SELECT max(idasiento) AS id FROM asiento') AS t1 (id integer);
		NEW.idasientofacturap = bs.id;
	END IF;


	-- Buscamos el cliente y su cuenta.
	SELECT INTO client idcuentaproveedor FROM proveedor WHERE idproveedor=NEW.idproveedor;
	IF NOT FOUND THEN
		RAISE EXCEPTION 'El proveedor no tiene cuenta asociada en la contabilidad';
	END IF;
	idctacliente := client.idcuentaproveedor;

	-- Buscamos la cuenta de servicio o de venta
	SELECT INTO cs MAX(idcuenta) AS id FROM bc_cuenta WHERE codigo LIKE '6000%';
	IF NOT FOUND THEN
		RAISE EXCEPTION 'No existe ls cuenta de Compras 6000...';
	END IF;
	idctaserv := cs.id;

	-- Buscamos la cuenta de IRPF
	SELECT INTO cs MAX(idcuenta) AS id FROM bc_cuenta WHERE codigo LIKE '4730%';
	IF NOT FOUND THEN
		RAISE EXCEPTION 'No existe ls cuenta de IRPF 4730...';
	END IF;
	idctairpf := cs.id;


	-- Buscamos el IRP que aplicamos
	SELECT INTO bs (irpfproveedor / 100) as val FROM proveedor WHERE idproveedor = NEW.idproveedor;
	IF bs.val IS NOT NULL THEN
		porirpf := bs.val;
	END IF;


	-- Buscamos los decuentos generales a aplicar.
	SELECT INTO bs 1 - SUM(proporciondfacturap) / 100 AS tdesc, count(iddfacturap) AS numdesc FROM dfacturap WHERE idfacturap = NEW.idfacturap;
	IF bs.numdesc <> 0 THEN
		totaldesc := bs.tdesc;
	END IF;

	-- Creamos los apuntes de IVA.
	FOR  bs IN SELECT SUM (cantlfacturap*pvplfacturap*(100-descuentolfacturap)/100) AS base, round(ivalfacturap) AS iva, ivalfacturap, reqeqlfacturap FROM lfacturap  WHERE idfacturap = NEW.idfacturap GROUP BY ivalfacturap, reqeqlfacturap LOOP
		IF bs.iva <> 0 THEN
			varaux := '4720%' || bs.iva;
			-- Hacemos la busqueda de la cuenta de IVA correspondiente.
			SELECT INTO ctaiva * FROM bc_cuenta WHERE codigo LIKE varaux;
			IF NOT FOUND THEN
				RAISE EXCEPTION 'No existe una cuenta de IVA para el tipo de IVA %', bs.iva;
			END IF;
			cta := ctaiva.codigo;
			idcta := ctaiva.idcuenta;

			-- Hacemos la insercion del borrador del apunte.
			query := 'INSERT INTO borrador (fecha, idcuenta, debe, idasiento, descripcion, conceptocontable) VALUES (''' || NEW.ffacturap || ''', ' || idcta || ', ' || bs.base * totaldesc * bs.iva / 100 || ', '|| NEW.idasientofacturap ||', ''' || concepto1 || ''' ,  ''Factura Proveedor'')';
			PERFORM dblink_exec(query);

			-- Vamos calculando el total de IVA
			totaliva := totaliva + bs.base * totaldesc * bs.iva / 100;
		END IF;
		IF bs.reqeqlfacturap <> 0 THEN
			varaux := '47201%' || bs.iva;
			-- Hacemos la busqueda de la cuenta de Recargo correspondiente.
			SELECT INTO ctaiva * FROM bc_cuenta WHERE codigo LIKE varaux;
			IF NOT FOUND THEN
				RAISE EXCEPTION 'No existe una cuenta de Recargo para el tipo de Recargo %', bs.reqeqlfacturap;
			END IF;
			cta := ctaiva.codigo;
			idcta := ctaiva.idcuenta;

			-- Hacemos la insercion del borrador del apunte.
			query := 'INSERT INTO borrador (fecha, idcuenta, debe, idasiento, descripcion, conceptocontable) VALUES (''' || NEW.ffacturap || ''', ' || idcta || ', ' || bs.base * totaldesc * bs.reqeqlfacturap / 100 || ', '|| NEW.idasientofacturap ||', ''' || concepto1 || ''', ''Factura Proveedor'')';
			PERFORM dblink_exec(query);

			-- Vamos calculando el total de IVA
			totaliva := totaliva + bs.base * totaldesc * bs.reqeqlfacturap / 100;
		END IF;
	END LOOP;



	-- Calculamos el total de la factura.
	SELECT INTO bs SUM (cantlfacturap*pvplfacturap*(100-descuentolfacturap)/100) AS base FROM lfacturap WHERE idfacturap = NEW.idfacturap;


	-- Hacemos la insercion del borrador del apunte al proveedor.
	total := bs.base * totaldesc * (1 - porirpf) + totaliva;
	query := 'INSERT INTO borrador (fecha, idcuenta, haber, idasiento, descripcion, conceptocontable) VALUES (''' || NEW.ffacturap || ''', ' || idctacliente || ', ' || total || ', '|| NEW.idasientofacturap ||', ''' || concepto1 || ''' , ''Factura Proveedor'')';
	PERFORM dblink_exec(query);


	-- Creamos el apunte de IRPF
	IF porirpf <> 0 THEN
		-- Hacemos la insercion del borrador del apunte.
		query := 'INSERT INTO borrador (fecha, idcuenta, haber, idasiento, descripcion, conceptocontable) VALUES (''' || NEW.ffacturap || ''', ' || idctairpf || ', ' || bs.base * totaldesc * porirpf || ', '|| NEW.idasientofacturap ||', ''' || concepto1 || ''' , ''Factura Proveedor'')';
		PERFORM dblink_exec(query);
	END IF;


	-- Creamos el apunte de servicio.
	-- Hacemos la insercion del borrador del apunte.
	query := 'INSERT INTO borrador (fecha, idcuenta, debe, idasiento, descripcion, conceptocontable) VALUES (''' || NEW.ffacturap || ''', ' || idctaserv || ', ' || bs.base * totaldesc || ', '|| NEW.idasientofacturap ||', ''' || concepto1 || ''' , ''Factura Proveedor'')';
	PERFORM dblink_exec(query);


	PERFORM dblink_disconnect();
	RETURN NEW;
END;
$$ LANGUAGE plpgsql;


CREATE TRIGGER syncbulmacontfacturaptriggeru
    BEFORE INSERT OR UPDATE ON facturap
    FOR EACH ROW
    EXECUTE PROCEDURE syncbulmacontfacturapu();
\echo "Creado el trigger que al modificar o insertar una factura  de proveedor en la facturacion mete el correspondiente asiento en la contabilidad"

-- ==============================================================================
-- TRATO DE LOS CLIENTES
-- ==============================================================================



SELECT drop_if_exists_proc ('syncbulmacontcliented','');
CREATE FUNCTION syncbulmacontcliented () RETURNS "trigger"
AS $$
DECLARE
	query TEXT;
BEGIN
	PERFORM conectabulmacont();
	-- Hacemos el update del stock del articulo
	IF OLD.idcuentacliente IS NOT NULL THEN
		query := 'DELETE FROM  cuenta WHERE idcuenta = ' || OLD.idcuentacliente ;
		PERFORM dblink_exec(query);
	END IF;

	PERFORM dblink_disconnect();
	RETURN OLD;
END;
$$ LANGUAGE plpgsql;


CREATE TRIGGER syncbulmacontclientetriggerd
    AFTER DELETE ON cliente
    FOR EACH ROW
    EXECUTE PROCEDURE syncbulmacontcliented();




SELECT drop_if_exists_proc ('syncbulmacontclienteu','');
CREATE FUNCTION syncbulmacontclienteu () RETURNS "trigger"
AS $$
DECLARE
	bs RECORD;
	quer TEXT;
	subquery TEXT;
	codcta INTEGER;
	idpadre INTEGER;
	grupo INTEGER;
	descripcion TEXT;
	cp TEXT;
	cif TEXT;
BEGIN
	PERFORM conectabulmacont();
	-- Hacemos el update del stock del articulo

	-- Cogemos el nombre de la cuenta.
	descripcion := quote_literal(NEW.nomcliente);
	cp := substring(NEW.cpcliente, length(NEW.cpcliente)-4, 5);
	cif := substring(NEW.cifcliente, length(NEW.cifcliente)-11, 12);

	IF NEW.idcuentacliente IS NULL THEN
		-- Buscamos el codigo de cuenta que vaya a corresponderle
		SELECT INTO bs  max(codigo)::INTEGER as cod FROM bc_cuenta WHERE codigo LIKE '4300%' ;
		IF FOUND THEN
			codcta := bs.cod;
			codcta := codcta +1;
		ELSE
			codcta := '4300001';
		END IF;
	
		-- Buscamos la cuenta padre (la 4300)
		SELECT INTO bs idcuenta, idgrupo FROM bc_cuenta WHERE codigo ='4300';
		idpadre := bs.idcuenta;
		grupo := bs.idgrupo;
	


		-- Creamos el Query de insercion
		quer := 'INSERT INTO cuenta (descripcion, padre, codigo, idgrupo) VALUES ( ' || descripcion ||', ' || idpadre || ', ''' || codcta || ''' , ' || grupo ||')';

		PERFORM dblink_exec(quer);
	
	
		SELECT INTO bs  max(idcuenta) AS id FROM bc_cuenta;
	
		NEW.idcuentacliente = bs.id;
	ELSE
		quer := 'UPDATE cuenta SET descripcion = ' || descripcion || ', cifent_cuenta = ''' || cif || ''', nombreent_cuenta = ''' || NEW.nomaltcliente || ''', dirent_cuenta = ''' || NEW.dircliente || ''', cpent_cuenta = ''' || cp || ''', telent_cuenta = ''' || NEW.telcliente || ''', bancoent_cuenta = ''' || NEW.bancocliente || ''', emailent_cuenta = ''' || NEW.mailcliente || ''', webent_cuenta = ''' || NEW.urlcliente || ''' WHERE idcuenta= ' || NEW.idcuentacliente;
		PERFORM dblink_exec(quer);
	END IF;

	PERFORM dblink_disconnect();
	RETURN NEW;
END;
$$ LANGUAGE plpgsql;


CREATE TRIGGER syncbulmacontclientetriggeru
    BEFORE UPDATE OR INSERT ON cliente
    FOR EACH ROW
    EXECUTE PROCEDURE syncbulmacontclienteu();


-- ==============================================================================
-- TRATO DE LOS PROVEEDORES
-- ==============================================================================




SELECT drop_if_exists_proc ('syncbulmacontproveedord','');
CREATE FUNCTION syncbulmacontproveedord () RETURNS "trigger"
AS $$
DECLARE
	query TEXT;
BEGIN
	PERFORM conectabulmacont();
	-- Hacemos el update del stock del articulo
	IF OLD.idcuentacliente IS NOT NULL THEN
		query := 'DELETE FROM  cuenta WHERE idcuenta = ' || OLD.idcuentaproveedor ;
		PERFORM dblink_exec(query);
	END IF;

	PERFORM dblink_disconnect();
	RETURN OLD;
END;
$$ LANGUAGE plpgsql;


CREATE TRIGGER syncbulmacontproveedortriggerd
    AFTER DELETE ON proveedor
    FOR EACH ROW
    EXECUTE PROCEDURE syncbulmacontproveedord();




SELECT drop_if_exists_proc ('syncbulmacontproveedoru','');
CREATE FUNCTION syncbulmacontproveedoru () RETURNS "trigger"
AS $$
DECLARE
	bs RECORD;
	quer TEXT;
	subquery TEXT;
	flecha DATE;
	codcta INTEGER;
	idpadre INTEGER;
	grupo INTEGER;
	descripcion TEXT;
BEGIN
	PERFORM conectabulmacont();
	-- Hacemos el update del stock del articulo

	-- Cogemos el nombre de la cuenta.
	descripcion := quote_literal(NEW.nomproveedor);

	IF NEW.idcuentaproveedor IS NULL THEN
		-- Buscamos el codigo de cuenta que vaya a corresponderle
		SELECT INTO bs  max(codigo) as cod FROM bc_cuenta WHERE codigo LIKE '4000%' ;
		codcta := bs.cod;
		codcta := codcta +1;
	
		-- Buscamos la cuenta padre (la 4300)
		SELECT INTO bs idcuenta, idgrupo FROM bc_cuenta WHERE codigo ='4000';
		idpadre := bs.idcuenta;
		grupo := bs.idgrupo;
	
		-- Creamos el Query de insercion
		quer := 'INSERT INTO cuenta (descripcion, padre, codigo, idgrupo) VALUES ( ' || descripcion ||', ' || idpadre || ', ''' || codcta || ''' , ' || grupo ||')';
		PERFORM dblink_exec(quer);
	
	
		SELECT INTO bs  max(idcuenta) AS id FROM bc_cuenta;
	
		NEW.idcuentaproveedor = bs.id;
	ELSE
		-- NOTA: Si no estan todos los campos completos el update no funciona bien
		-- Hay que introducir una validacion de campos.
		quer := 'UPDATE cuenta SET descripcion = ' || descripcion || ', cifent_cuenta = ''' || NEW.cifproveedor || ''', nombreent_cuenta = ''' || NEW.nomaltproveedor || ''', dirent_cuenta = ''' || NEW.dirproveedor || ''', cpent_cuenta = ''' || NEW.cpproveedor || ''', telent_cuenta = ''' || NEW.telproveedor || ''', emailent_cuenta = ''' || NEW.emailproveedor || ''', webent_cuenta = ''' || NEW.urlproveedor || ''' WHERE idcuenta= ' || NEW.idcuentaproveedor;
--		RAISE EXCEPTION ' % ', quer;
		PERFORM dblink_exec(quer);
	END IF;

	PERFORM dblink_disconnect();
	RETURN NEW;
END;
$$ LANGUAGE plpgsql;


CREATE TRIGGER syncbulmacontproveedortriggeru
    BEFORE INSERT OR UPDATE ON proveedor
    FOR EACH ROW
    EXECUTE PROCEDURE syncbulmacontproveedoru();


-- =================================================================
--           TRATO DE LOS BANCOS
-- =================================================================

SELECT drop_if_exists_proc ('syncbulmacontbancod','');
CREATE FUNCTION syncbulmacontbancod () RETURNS "trigger"
AS $$
DECLARE
	query TEXT;
BEGIN
	PERFORM conectabulmacont();
	-- Hacemos el update del stock del articulo
	IF OLD.idasientobanco IS NOT NULL THEN
		query := 'DELETE FROM cuenta WHERE idcuenta = ' || OLD.idcuentabanco;
		PERFORM dblink_exec(query);

	END IF;

	PERFORM dblink_disconnect();
	RETURN OLD;
END;
$$ LANGUAGE plpgsql;


CREATE TRIGGER syncbulmacontbancotriggerd
    AFTER DELETE ON banco
    FOR EACH ROW
    EXECUTE PROCEDURE syncbulmacontbancod();
\echo "Creado el trigger que al borrar un banco borra la respectiva cuenta en la contabilidad"



SELECT drop_if_exists_proc ('syncbulmacontbancou','');
CREATE FUNCTION syncbulmacontbancou () RETURNS "trigger"
AS $$
DECLARE
	bs RECORD;
	codcta INTEGER;
	quer TEXT;
	idpadre INTEGER;
	grupo INTEGER;
	descripcion TEXT;
	tipocuenta1 INTEGER;
BEGIN
	-- conectamos con contabilidad, etc
	PERFORM conectabulmacont();

	-- Cogemos el nombre de la cuenta.
	descripcion := quote_literal(NEW.nombanco);

	-- Hacemos el update del stock del articulo
	IF NEW.idcuentabanco IS NULL THEN
		-- Buscamos el codigo de cuenta que vaya a corresponderle
		SELECT INTO bs  max(codigo)::INTEGER as cod FROM bc_cuenta WHERE codigo LIKE '5730%' ;
		IF FOUND THEN
			codcta := bs.cod;
			codcta := codcta +1;
		ELSE
			codcta := '5730001';
		END IF;
	
		-- Buscamos la cuenta padre (la 4300)
		SELECT INTO bs idcuenta, idgrupo, tipocuenta FROM bc_cuenta WHERE codigo ='573';
		idpadre := bs.idcuenta;
		grupo := bs.idgrupo;
		tipocuenta1 := bs.tipocuenta;
	
		-- Creamos el Query de insercion
		quer := 'INSERT INTO cuenta (descripcion, padre, codigo, idgrupo, tipocuenta) VALUES ( ' || descripcion ||', ' || idpadre || ', ''' || codcta || ''' , ' || grupo ||', '|| tipocuenta1 || ')';
		PERFORM dblink_exec(quer);
	
		SELECT INTO bs  max(idcuenta) AS id FROM bc_cuenta;
		NEW.idcuentabanco := bs.id;
	ELSE
		quer := 'UPDATE cuenta SET descripcion =' || descripcion || ' WHERE idcuenta =' || NEW.idcuentabanco;
		PERFORM dblink_exec(quer);
	END IF;


	PERFORM dblink_disconnect();
	RETURN NEW;
END;
$$ LANGUAGE plpgsql;


CREATE TRIGGER syncbulmacontbancotriggeru
    BEFORE INSERT OR UPDATE ON banco
    FOR EACH ROW
    EXECUTE PROCEDURE syncbulmacontbancou();

\echo "Creado el trigger que al modificar o insertar un banco en la facturacion mete el correspondiente asiento en la contabilidad"

-- =================================================================
--           TRATO DE LOS ALMACENES
-- =================================================================

SELECT drop_if_exists_proc ('syncbulmacontalmacend','');
CREATE FUNCTION syncbulmacontalmacend () RETURNS "trigger"
AS $$
DECLARE
	query TEXT;
BEGIN
	PERFORM conectabulmacont();
	-- Hacemos el update del stock del articulo
	IF OLD.idasientobanco IS NOT NULL THEN
		query := 'DELETE FROM cuenta WHERE idcuenta = ' || OLD.idcuentaalmacen;
		PERFORM dblink_exec(query);
	END IF;
	PERFORM dblink_disconnect();
	RETURN OLD;
END;
$$ LANGUAGE plpgsql;


CREATE TRIGGER syncbulmacontalmacentriggerd
    AFTER DELETE ON almacen
    FOR EACH ROW
    EXECUTE PROCEDURE syncbulmacontalmacend();
\echo "Creado el trigger que al borrar un banco borra la respectiva cuenta en la contabilidad"



SELECT drop_if_exists_proc ('syncbulmacontalmacenu','');
CREATE FUNCTION syncbulmacontalmacenu () RETURNS "trigger"
AS $$
DECLARE
	cs RECORD;
	bs RECORD;
	codcta INTEGER;
	quer TEXT;
	idpadre INTEGER;
	grupo INTEGER;
	descripcion TEXT;
	tipocuenta1 INTEGER;
BEGIN
	-- conectamos con contabilidad, etc
	PERFORM conectabulmacont();

	-- Cogemos el nombre de la cuenta.
	descripcion := quote_literal(NEW.nomalmacen);

	-- Hacemos el update del stock del articulo
	IF NEW.idcuentaalmacen IS NULL THEN
		-- Buscamos el codigo de cuenta que vaya a corresponderle
		SELECT INTO cs  max(codigo)::INTEGER as cod FROM bc_cuenta WHERE codigo LIKE '5700%' ;
		IF cs.cod IS NOT NULL THEN
			codcta := cs.cod;
			codcta := codcta +1;
		ELSE
			codcta := '5700001';
		END IF;
	
		-- Buscamos la cuenta padre (la 4300)
		SELECT INTO bs idcuenta, idgrupo, tipocuenta FROM bc_cuenta WHERE codigo ='570';
		idpadre := bs.idcuenta;
		grupo := bs.idgrupo;
		tipocuenta1 := bs.tipocuenta;
	

	
		-- Creamos el Query de insercion
		quer := 'INSERT INTO cuenta (descripcion, padre, codigo, idgrupo, tipocuenta) VALUES ( ' || descripcion ||', ' || idpadre || ', ''' || codcta || ''' , ' || grupo ||', '|| tipocuenta1 || ')';
		PERFORM dblink_exec(quer);
	
		SELECT INTO bs  max(idcuenta) AS id FROM bc_cuenta;
		NEW.idcuentaalmacen := bs.id;
	ELSE
		quer := 'UPDATE cuenta SET descripcion =' || descripcion || ' WHERE idcuenta =' || NEW.idcuentaalmacen;
		PERFORM dblink_exec(quer);
	END IF;


	PERFORM dblink_disconnect();
	RETURN NEW;
END;
$$ LANGUAGE plpgsql;


CREATE TRIGGER syncbulmacontalmacentriggeru
    BEFORE INSERT OR UPDATE ON almacen
    FOR EACH ROW
    EXECUTE PROCEDURE syncbulmacontalmacenu();

\echo "Creado el trigger que al modificar o insertar un almacen en la facturacion mete el correspondiente asiento en la contabilidad"


-- ==============================================================================

-- Agregamos nuevos parametros de configuracion.
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	as RECORD;
BEGIN
	SELECT INTO as * FROM configuracion WHERE nombre=''DBRev-SyncBulmaCont'';
	IF FOUND THEN
		UPDATE CONFIGURACION SET valor=''0.9.1-001'' WHERE nombre=''DBRev-SyncBulmaCont'';
	ELSE
		INSERT INTO configuracion (nombre, valor) VALUES (''DBRev-SyncBulmaCont'', ''0.9.1-001'');
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