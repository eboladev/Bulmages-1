--
-- Modificación de campos y alta de funciones en la base de datos de BulmaFact
-- para la sincronización de datos con BulmaCont.
--
\echo "********* INICIADO FICHERO DE ESTRUCTURA DEL PLUGIN DE SINCRONIZACION CON BULMACONT *********"

\echo ":: Establecemos los mensajes minimos a avisos y otros parametros ... "
\echo -n ":: "
SET client_min_messages TO WARNING;
SET log_min_messages TO WARNING;
-- SET log_error_verbosity TO TERSE;
BEGIN;

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
	CREATE OR REPLACE view BC_Cuenta AS SELECT * FROM dblink('bulmafact2cont',  'SELECT idcuenta, codigo, descripcion, tipocuenta FROM cuenta') AS t1 ( idcuenta integer, codigo varchar, descripcion varchar, tipocuenta integer);

	CREATE OR REPLACE VIEW BC_Asiento AS SELECT * FROM dblink('bulmafact2cont',  'SELECT idasiento FROM asiento') AS t1 (idasiento integer);

	CREATE OR REPLACE VIEW BC_Borrador AS SELECT * FROM dblink('bulmafact2cont',  'SELECT idborrador FROM borrador') AS t1 (idborrador integer);

	CREATE OR REPLACE VIEW BC_Apunte AS SELECT * FROM dblink('bulmafact2cont',  'SELECT idapunte FROM apunte') AS t1 (idapunte integer);

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



	-- Creamos el espacio para almacenar el identificador de cuenta que corresponde a la VENTA familia.
	SELECT INTO bs * FROM pg_attribute WHERE attname='idcuentaventafamilia';
	IF NOT FOUND THEN
		ALTER TABLE familia ADD COLUMN idcuentaventafamilia INTEGER;
	END IF;

	-- Creamos el espacio para almacenar el identificador de cuenta que corresponde a la COMPRA familia.
	SELECT INTO bs * FROM pg_attribute WHERE attname='idcuentacomprafamilia';
	IF NOT FOUND THEN
		ALTER TABLE familia ADD COLUMN idcuentacomprafamilia INTEGER;
	END IF;

	-- Creamos el espacio para almacenar las cuentas definidas por el usuario
	-- para las familias que sobreescriben la configuracion por defecto.
	SELECT INTO bs * FROM pg_attribute WHERE attname='prefcuentaventafamilia';
	IF NOT FOUND THEN
		ALTER TABLE familia ADD COLUMN prefcuentaventafamilia CHARACTER VARYING(12);
		ALTER TABLE familia ADD COLUMN origenidcuentaventafamilia INTEGER;
	END IF;

	SELECT INTO bs * FROM pg_attribute WHERE attname='prefcuentacomprafamilia';
	IF NOT FOUND THEN
		ALTER TABLE familia ADD COLUMN prefcuentacomprafamilia CHARACTER VARYING(12);
		ALTER TABLE familia ADD COLUMN origenidcuentacomprafamilia INTEGER;
	END IF;



	-- Creamos el espacio para almacenar el identificador de cuenta que corresponde a la forma_pago.
	SELECT INTO bs * FROM pg_attribute WHERE attname='idcuentaforma_pago';
	IF NOT FOUND THEN
		ALTER TABLE forma_pago ADD COLUMN idcuentaforma_pago INTEGER;
	END IF;


	-- Creamos el espacio para almacenar las cuentas definidas por el usuario
	-- para los bancos que sobreescriben la configuracion por defecto.
	SELECT INTO bs * FROM pg_attribute WHERE attname='prefcuentaforma_pago';
	IF NOT FOUND THEN
		ALTER TABLE forma_pago ADD COLUMN prefcuentaforma_pago CHARACTER VARYING(12);
		ALTER TABLE forma_pago ADD COLUMN origenidcuentaforma_pago INTEGER;
	END IF;


	-- Creamos el espacio para almacenar las cuentas definidas por el usuario
	-- para los bancos que sobreescriben la configuracion por defecto.
	SELECT INTO bs * FROM pg_attribute WHERE attname='prefcuentabanco';
	IF NOT FOUND THEN
		ALTER TABLE banco ADD COLUMN prefcuentabanco CHARACTER VARYING(12);
		ALTER TABLE banco ADD COLUMN origenidcuentabanco INTEGER;
	END IF;


	-- Creamos el espacio para almacenar las cuentas definidas por el usuario
	-- para los clientes que sobreescriben la configuracion por defecto.
	SELECT INTO bs * FROM pg_attribute WHERE attname='prefcuentacliente';
	IF NOT FOUND THEN
		ALTER TABLE cliente ADD COLUMN prefcuentacliente CHARACTER VARYING(12);
		ALTER TABLE cliente ADD COLUMN origenidcuentacliente INTEGER;
	END IF;

	-- Creamos el espacio para almacenar las cuentas definidas por el usuario
	-- para los proveedores que sobreescriben la configuracion por defecto.
	SELECT INTO bs * FROM pg_attribute WHERE attname='prefcuentaproveedor';
	IF NOT FOUND THEN
		ALTER TABLE proveedor ADD COLUMN prefcuentaproveedor CHARACTER VARYING(12);
		ALTER TABLE proveedor ADD COLUMN origenidcuentaproveedor INTEGER;
	END IF;


	RETURN 0;
END;
$$   LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo "Agregamos los campos para almacenar claves foraneas en las tablas necesarias."



CREATE OR REPLACE FUNCTION dblink_exists(text)
RETURNS bool AS $$
  SELECT COALESCE($1 = ANY (dblink_get_connections()), false);
$$ LANGUAGE sql;


CREATE OR REPLACE FUNCTION conectabulmacont() RETURNS INTEGER
AS $$
DECLARE
	bs RECORD;
	quer TEXT;
	check_dblink_connection BOOLEAN;
BEGIN
	SELECT INTO bs * FROM configuracion WHERE nombre = 'DataBaseContabilidad';
	quer := 'dbname =' || bs.valor;

	check_dblink_connection = dblink_exists('bulmafact2cont');

	IF check_dblink_connection IS TRUE THEN
	    RAISE NOTICE 'ROLLBACK dblink';
	    PERFORM dblink_exec('bulmafact2cont', 'ROLLBACK WORK;');
	    PERFORM dblink_disconnect('bulmafact2cont');
	END IF;

	PERFORM dblink_connect('bulmafact2cont', quer);
	PERFORM dblink_exec('bulmafact2cont',  'SET DATESTYLE TO SQL,European');

	RETURN 0;
END;
$$ LANGUAGE plpgsql;

-- =================================================================
--           TRATO DE LOS PAGOS
-- =================================================================
SELECT drop_if_exists_proc ('syncbulmacontpagod','');
CREATE OR REPLACE FUNCTION syncbulmacontpagod () RETURNS "trigger"
AS $$
DECLARE
	query TEXT;
BEGIN
	PERFORM conectabulmacont();
	PERFORM dblink_exec('bulmafact2cont', 'BEGIN WORK;');
	
	IF OLD.idasientopago IS NOT NULL THEN
		query := 'DELETE FROM apunte WHERE idasiento= ' || OLD.idasientopago;
		PERFORM dblink_exec('bulmafact2cont', query);
		query := 'DELETE FROM  borrador WHERE idasiento= ' || OLD.idasientopago ;
		PERFORM dblink_exec('bulmafact2cont', query);
		query := 'DELETE FROM  asiento WHERE idasiento = ' || OLD.idasientopago ;
		PERFORM dblink_exec('bulmafact2cont', query);
	END IF;

	PERFORM dblink_exec('bulmafact2cont', 'COMMIT WORK;');
	PERFORM dblink_disconnect('bulmafact2cont');
	RETURN OLD;
END;
$$ LANGUAGE plpgsql;


CREATE TRIGGER syncbulmacontpagotriggerd
    AFTER DELETE ON pago
    FOR EACH ROW
    EXECUTE PROCEDURE syncbulmacontpagod();
\echo "Creado el trigger que al borrar una factura borra su respectivo asiento en la contabilidad"



SELECT drop_if_exists_proc ('syncbulmacontpagou','');
CREATE OR REPLACE FUNCTION syncbulmacontpagou () RETURNS "trigger"
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
	asientonuevo BOOLEAN;
BEGIN
	-- conectamos con contabilidad, etc
	PERFORM conectabulmacont();
	PERFORM dblink_exec('bulmafact2cont', 'BEGIN WORK;');

	concepto := '[A. Automatico] Pago ' || NEW.refpago;
	concepto1 := 'Pago ' || NEW.refpago;


	-- Puede darse el caso de que el contable haya borrado el asiento. Y por eso comprobamos que realmente exista en la contabilidad.
	asientonuevo := TRUE;
	IF NEW.idasientopago IS NOT NULL THEN
		query := 'SELECT idasiento FROM asiento WHERE idasiento =' || NEW.idasientopago;
		SELECT INTO cs * FROM dblink('bulmafact2cont', query) AS t1 (idasiento integer);
		IF FOUND THEN
			asientonuevo := FALSE;
		END IF;
	END IF;


	IF asientonuevo IS FALSE THEN
		query := 'DELETE FROM apunte WHERE idasiento= ' || NEW.idasientopago;
		PERFORM dblink_exec('bulmafact2cont', query);
		query := 'DELETE FROM borrador WHERE idasiento= ' || NEW.idasientopago;
		PERFORM dblink_exec('bulmafact2cont', query);
		query := 'UPDATE asiento SET descripcion = ''' || concepto || ''', comentariosasiento = ''' || concepto || ''' WHERE idasiento = ' || NEW.idasientopago;
		PERFORM dblink_exec('bulmafact2cont', query);
	ELSE
		query := 'INSERT INTO asiento (fecha, descripcion, comentariosasiento) VALUES ( ' || quote_literal(NEW.fechapago) || ', ''' || concepto || ''', ''' || concepto || ''')';
		PERFORM dblink_exec('bulmafact2cont', query);

		SELECT INTO cs * FROM dblink('bulmafact2cont', 'SELECT max(idasiento) AS id FROM asiento') AS t1 (id integer);
		NEW.idasientopago = cs.id;
	END IF;


	-- Buscamos el proveedor y su cuenta.
	SELECT INTO client idcuentaproveedor FROM proveedor WHERE idproveedor = NEW.idproveedor;
	IF NOT FOUND THEN
		PERFORM dblink_exec('bulmafact2cont', 'ROLLBACK WORK;');
		RAISE EXCEPTION 'El proveedor no tiene cuenta asociada en la contabilidad';
	END IF;
	idctacliente := client.idcuentaproveedor;

	-- Buscamos el banco
	IF NEW.idbanco IS NOT NULL THEN
		SELECT INTO qbanco idcuentabanco FROM banco WHERE idbanco = NEW.idbanco;

		IF NOT FOUND THEN
			PERFORM dblink_exec('bulmafact2cont', 'ROLLBACK WORK;');
			RAISE EXCEPTION 'El banco no tiene cuenta asociada en la contabilidad';
		END IF;
		idctapago := qbanco.idcuentabanco;
	ELSE
		-- Buscamos la cuenta de servicio o de venta
		SELECT INTO cs MAX(idcuenta) AS id FROM bc_cuenta WHERE codigo LIKE '5700%';
		IF cs.id IS NULL THEN
			PERFORM dblink_exec('bulmafact2cont', 'ROLLBACK WORK;');
			RAISE EXCEPTION 'No existe la cuenta de Caja 5700...';
		END IF;
		idctapago := cs.id;
	END IF;

	-- Apunte por el proveedor
	query := 'INSERT INTO borrador (fecha, idcuenta, debe, idasiento, descripcion, conceptocontable) VALUES (''' || NEW.fechapago || ''', ' || idctacliente || ', ' || NEW.cantpago || ', '|| NEW.idasientopago ||', ''' || concepto1 || ''', ''Pago'')';
	PERFORM dblink_exec('bulmafact2cont', query);


	-- Apunte por caja o banco.
	query := 'INSERT INTO borrador (fecha, idcuenta, haber, idasiento, descripcion, conceptocontable) VALUES (''' || NEW.fechapago || ''', ' || idctapago || ', ' || NEW.cantpago || ', '|| NEW.idasientopago ||', ''' || concepto1 || ''', ''Pago'')';
	PERFORM dblink_exec('bulmafact2cont', query);

	RETURN NEW;
END;
$$ LANGUAGE plpgsql;


CREATE TRIGGER syncbulmacontpagotriggeru
    BEFORE INSERT OR UPDATE ON pago
    FOR EACH ROW
    EXECUTE PROCEDURE syncbulmacontpagou();

\echo "Creado el trigger que al modificar o insertar una factura en la facturacion mete el correspondiente asiento en la contabilidad"


SELECT drop_if_exists_proc ('syncbulmacontpagoup','');
CREATE OR REPLACE FUNCTION syncbulmacontpagoup () RETURNS "trigger"
AS $$
DECLARE
      check_dblink_connection BOOLEAN;
BEGIN

      check_dblink_connection = dblink_exists('bulmafact2cont');
      
      IF check_dblink_connection IS TRUE THEN
	  -- Hace un COMMIT de la conexion dblink si todo ha ido bien.
	  PERFORM dblink_exec('bulmafact2cont', 'COMMIT WORK;');
	  PERFORM dblink_disconnect('bulmafact2cont');
      ELSE
	  RAISE EXCEPTION 'Se ha perdido la conexion dblink en syncbulmacontpagoup.';
      END IF;

      RETURN NULL;

END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER syncbulmacontpagotriggerup
    AFTER UPDATE OR INSERT ON pago
    FOR EACH STATEMENT
    EXECUTE PROCEDURE syncbulmacontpagoup();




-- =================================================================
--           TRATO DE LOS COBROS
-- =================================================================
SELECT drop_if_exists_proc ('syncbulmacontcobrod','');
CREATE OR REPLACE FUNCTION syncbulmacontcobrod () RETURNS "trigger"
AS $$
DECLARE
	query TEXT;
BEGIN
	PERFORM conectabulmacont();
	PERFORM dblink_exec('bulmafact2cont', 'BEGIN WORK;');

	IF OLD.idasientocobro IS NOT NULL THEN
		query := 'DELETE FROM apunte WHERE idasiento = ' || OLD.idasientocobro;
		PERFORM dblink_exec('bulmafact2cont', query);
		query := 'DELETE FROM borrador WHERE idasiento = ' || OLD.idasientocobro ;
		PERFORM dblink_exec('bulmafact2cont', query);
		query := 'DELETE FROM asiento WHERE idasiento = ' || OLD.idasientocobro;
		PERFORM dblink_exec('bulmafact2cont', query);
	END IF;

	PERFORM dblink_exec('bulmafact2cont', 'COMMIT WORK;');
	PERFORM dblink_disconnect('bulmafact2cont');
	RETURN OLD;
END;
$$ LANGUAGE plpgsql;


CREATE TRIGGER syncbulmacontcobrotriggerd
    AFTER DELETE ON cobro
    FOR EACH ROW
    EXECUTE PROCEDURE syncbulmacontcobrod();
\echo "Creado el trigger que al borrar un cobro borra su respectivo asiento en la contabilidad"



SELECT drop_if_exists_proc ('syncbulmacontcobrou','');
CREATE OR REPLACE FUNCTION syncbulmacontcobrou () RETURNS "trigger"
AS $$
DECLARE
	cs RECORD;
	client RECORD;
	qbanco RECORD;
	qforma_pago RECORD;
	idctacliente TEXT;
	query TEXT;
	subquery TEXT;
	idctacobro INTEGER;
	flecha DATE;
	concepto TEXT;
	concepto1 TEXT;
	asientonuevo BOOLEAN;
BEGIN
	-- conectamos con contabilidad, etc
	PERFORM conectabulmacont();
	PERFORM dblink_exec('bulmafact2cont', 'BEGIN WORK;');

	concepto := '[A. Automatico] Cobro ' || NEW.refcobro;
	concepto1 := 'Cobro ' || NEW.refcobro;

	
	-- Puede darse el caso de que el contable haya borrado el asiento. Y por eso comprobamos que realmente exista en la contabilidad.
	asientonuevo := TRUE;
	IF NEW.idasientocobro IS NOT NULL THEN
		query := 'SELECT idasiento FROM asiento WHERE idasiento = ' || NEW.idasientocobro;
		SELECT INTO cs * FROM dblink('bulmafact2cont', query) AS t1 (idasiento integer);
		IF FOUND THEN
			asientonuevo := FALSE;
		END IF;
	END IF;

	IF asientonuevo IS FALSE THEN
		query := 'DELETE FROM apunte WHERE idasiento= ' || NEW.idasientocobro;
		PERFORM dblink_exec('bulmafact2cont', query);
		query := 'DELETE FROM borrador WHERE idasiento= ' || NEW.idasientocobro;
		PERFORM dblink_exec('bulmafact2cont', query);
		query := 'UPDATE asiento SET descripcion = ''' || concepto || ''', comentariosasiento = ''' || concepto || ''' WHERE idasiento = ' || NEW.idasientocobro;
		PERFORM dblink_exec('bulmafact2cont', query);
	ELSE
		query := 'INSERT INTO asiento (fecha, descripcion, comentariosasiento) VALUES ( ' || quote_literal(NEW.fechacobro) || ', ''' || concepto || ''', ''' || concepto || ''')';
		PERFORM dblink_exec('bulmafact2cont', query);

		SELECT INTO cs * FROM dblink('bulmafact2cont', 'SELECT max(idasiento) AS id FROM asiento') AS t1 (id integer);
		NEW.idasientocobro = cs.id;
	END IF;


	-- Buscamos el cliente y su cuenta.
	SELECT INTO client idcuentacliente FROM cliente WHERE idcliente = NEW.idcliente;
	IF NOT FOUND THEN
		PERFORM dblink_exec('bulmafact2cont', 'ROLLBACK WORK;');
		RAISE EXCEPTION 'El cliente no tiene cuenta asociada en la contabilidad';
	END IF;
	idctacliente := client.idcuentacliente;


	IF NEW.idforma_pago IS NOT NULL THEN

		SELECT INTO qforma_pago idcuentaforma_pago, idbanco FROM forma_pago WHERE idforma_pago = NEW.idforma_pago;
		IF NOT FOUND THEN
			PERFORM dblink_exec('bulmafact2cont', 'ROLLBACK WORK;');
			RAISE EXCEPTION 'La forma de pago no tiene cuenta asociada en la contabilidad';
		END IF;

		-- Si existe idbanco, entonces es un banco y se tiene que buscar la cuenta contable que le corresponde
		-- a ese banco.
		IF qforma_pago.idbanco IS NOT NULL THEN

		    SELECT INTO qbanco idcuentabanco FROM banco WHERE idbanco = qforma_pago.idbanco;
		    IF NOT FOUND THEN
			    PERFORM dblink_exec('bulmafact2cont', 'ROLLBACK WORK;');
			    RAISE EXCEPTION 'El banco no tiene cuenta asociada en la contabilidad';
		    END IF;
		    idctacobro := qbanco.idcuentabanco;

		ELSE

		    idctacobro := qforma_pago.idcuentaforma_pago;

		END IF;

	ELSE
		-- Buscamos la cuenta de caja (efectivo)
		SELECT INTO cs MAX(idcuenta) AS id FROM bc_cuenta WHERE codigo LIKE '5700%';
		IF cs.id IS NULL THEN
			PERFORM dblink_exec('bulmafact2cont', 'ROLLBACK WORK;');
			RAISE EXCEPTION 'No existe ls cuenta de Caja 5700...';
		END IF;
		idctacobro := cs.id;
	END IF;

	-- Apunte por el cliente
	query := 'INSERT INTO borrador (fecha, idcuenta, haber, idasiento, descripcion, conceptocontable) VALUES (''' || NEW.fechacobro || ''', ' || idctacliente || ', ' || NEW.cantcobro || ', '|| NEW.idasientocobro ||', ''' || concepto1 || ''', ''Cobro'')';
	PERFORM dblink_exec('bulmafact2cont', query);

	-- Apunte por caja o banco.
	query := 'INSERT INTO borrador (fecha, idcuenta, debe, idasiento, descripcion, conceptocontable) VALUES (''' || NEW.fechacobro || ''', ' || idctacobro || ', ' || NEW.cantcobro || ', '|| NEW.idasientocobro ||', ''' || concepto  || ''', ''Cobro'')';
	PERFORM dblink_exec('bulmafact2cont', query);

	RETURN NEW;
END;
$$ LANGUAGE plpgsql;


CREATE TRIGGER syncbulmacontcobrotriggeru
    BEFORE INSERT OR UPDATE ON cobro
    FOR EACH ROW
    EXECUTE PROCEDURE syncbulmacontcobrou();



SELECT drop_if_exists_proc ('syncbulmacontcobroup','');
CREATE OR REPLACE FUNCTION syncbulmacontcobroup () RETURNS "trigger"
AS $$
DECLARE
      check_dblink_connection BOOLEAN;
BEGIN

      check_dblink_connection = dblink_exists('bulmafact2cont');
      
      IF check_dblink_connection IS TRUE THEN
	  -- Hace un COMMIT de la conexion dblink si todo ha ido bien.
	  PERFORM dblink_exec('bulmafact2cont', 'COMMIT WORK;');
	  PERFORM dblink_disconnect('bulmafact2cont');
      ELSE
	  RAISE EXCEPTION 'Se ha perdido la conexion dblink en syncbulmacontcobroup.';
      END IF;

      RETURN NULL;

END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER syncbulmacontcobrotriggerup
    AFTER UPDATE OR INSERT ON cobro
    FOR EACH STATEMENT
    EXECUTE PROCEDURE syncbulmacontcobroup();




\echo "Creado el trigger que al modificar o insertar una factura en la facturacion mete el correspondiente asiento en la contabilidad"
-- =================================================================
--           TRATO DE LAS FACTURAS A CLIENTE
-- =================================================================
SELECT drop_if_exists_proc ('syncbulmacontfacturad','');
CREATE OR REPLACE FUNCTION syncbulmacontfacturad () RETURNS "trigger"
AS $$
DECLARE
	query TEXT;
BEGIN
	PERFORM conectabulmacont();
	-- Hacemos el update del stock del articulo
	IF OLD.idasientofactura IS NOT NULL THEN
		query := 'DELETE FROM registroiva WHERE idborrador IN (SELECT idborrador FROM borrador WHERE idasiento = ' || OLD.idasientofactura || ')';
		PERFORM dblink_exec('bulmafact2cont', query);
		query := 'DELETE FROM apunte WHERE idasiento = ' || OLD.idasientofactura;
		PERFORM dblink_exec('bulmafact2cont', query);
		query := 'DELETE FROM borrador WHERE idasiento = ' || OLD.idasientofactura ;
		PERFORM dblink_exec('bulmafact2cont', query);
		query := 'DELETE FROM asiento WHERE idasiento = ' || OLD.idasientofactura ;
		PERFORM dblink_exec('bulmafact2cont', query);
	END IF;

	PERFORM dblink_disconnect('bulmafact2cont');
	RETURN OLD;
END;
$$ LANGUAGE plpgsql;


CREATE TRIGGER syncbulmacontfacturatriggerd
    AFTER DELETE ON factura
    FOR EACH ROW
    EXECUTE PROCEDURE syncbulmacontfacturad();
\echo "Creado el trigger que al borrar una factura borra su respectivo asiento en la contabilidad"



SELECT drop_if_exists_proc ('syncbulmacontfacturau','');
CREATE OR REPLACE FUNCTION syncbulmacontfacturau () RETURNS "trigger"
AS $$
DECLARE
	bs RECORD;
	cs RECORD;
	client RECORD;
	ctaiva RECORD;
	ctareq RECORD;
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
	asientonuevo BOOLEAN;
	redondeo INTEGER := 2;
BEGIN
	-- conectamos con contabilidad, etc
	PERFORM conectabulmacont();
	-- Es necesario controlar la transaccion que se hace en la base de datos enlazada porque no actua
	-- dentro de la transaccion de esta funcion y si hay un error no desaria los cambios provocando problemas
	-- de integridad de los datos.
	-- Hay que hacer un ROLLBACK WORK explicito ANTES de cada RAISE EXCEPTION.
	PERFORM dblink_exec('bulmafact2cont', 'BEGIN WORK;');
	totaliva := 0;

	concepto := '[A. Automatico] Factura Cliente Num: '  || NEW.codigoserie_factura || ' / ' || NEW.numfactura;
	concepto1 := 'Factura Cliente Num: ' || NEW.codigoserie_factura || ' / ' || NEW.numfactura;

	asientonuevo := TRUE;

	-- Puede darse el caso de que el contable haya borrado el asiento. Y por eso comprobamos que realmente exista en la contabilidad.
	-- Si idasientofactura es NULL entonces es porque es un INSERT de una nueva factura. En caso contrario
	-- es un UPDATE de un registro.
	IF NEW.idasientofactura IS NOT NULL THEN
		query := 'SELECT idasiento FROM asiento WHERE idasiento =' || NEW.idasientofactura;
		SELECT INTO bs * FROM dblink('bulmafact2cont', query) AS t1 (idasiento integer);
		IF FOUND THEN
			asientonuevo := FALSE;
		END IF;
	END IF;

	IF asientonuevo IS FALSE THEN
		query := 'DELETE FROM registroiva WHERE idborrador IN (SELECT idborrador FROM borrador WHERE idasiento = ' || NEW.idasientofactura || ')';
		PERFORM dblink_exec('bulmafact2cont', query);
		query := 'DELETE FROM apunte WHERE idasiento= ' || NEW.idasientofactura;
		PERFORM dblink_exec('bulmafact2cont', query);
		query := 'DELETE FROM borrador WHERE idasiento= ' || NEW.idasientofactura;
		PERFORM dblink_exec('bulmafact2cont', query);
		query := 'UPDATE asiento SET descripcion =''' || concepto || ''', comentariosasiento = ''' || concepto || ''' WHERE idasiento = ' || NEW.idasientofactura;
		PERFORM dblink_exec('bulmafact2cont', query);
	ELSE
		-- Crea el asiento en la contabilidad y recupera el numero de asiento para actualizar luego
		-- el campo idasientofactura del registro factura.
		query := 'INSERT INTO asiento (fecha, descripcion, comentariosasiento) VALUES ( ' || quote_literal(NEW.ffactura) || ', ''' || concepto || ''' , ''' || concepto || ''')';
		PERFORM dblink_exec('bulmafact2cont', query);
		SELECT INTO bs * FROM dblink('bulmafact2cont', 'SELECT max(idasiento) AS id FROM asiento') AS t1 (id integer);
		NEW.idasientofactura = bs.id;
	END IF;


	-- Buscamos el cliente y su cuenta.
	SELECT INTO client idcuentacliente FROM cliente WHERE idcliente=NEW.idcliente;
	IF NOT FOUND THEN
		PERFORM dblink_exec('bulmafact2cont', 'ROLLBACK WORK;');
		RAISE EXCEPTION 'El cliente no tiene cuenta asociada en la contabilidad.';
	END IF;
	idctacliente := client.idcuentacliente;

	-- Buscamos la cuenta de servicio o de venta
	SELECT INTO cs MAX(idcuenta) AS id FROM bc_cuenta WHERE codigo LIKE '7000%';
	IF NOT FOUND THEN
		PERFORM dblink_exec('bulmafact2cont', 'ROLLBACK WORK;');
		RAISE EXCEPTION 'No existe la cuenta de Ventas 7000...';
	END IF;
	idctaserv := cs.id;


	-- Buscamos la cuenta de IRPF
	SELECT INTO cs MAX(idcuenta) AS id FROM bc_cuenta WHERE codigo LIKE '4730%';
	IF cs.id IS NULL THEN
	      PERFORM dblink_exec('bulmafact2cont', 'ROLLBACK WORK;');
	      RAISE EXCEPTION 'No existe la cuenta de IRPF 4730...';
	ELSE
	      idctairpf := cs.id;
	END IF;

	-- Buscamos el IRPF que aplicamos
	SELECT INTO bs valor::NUMERIC / 100 as val FROM configuracion WHERE nombre ='IRPF';
	IF bs.val IS NULL THEN
		porirpf := 0;
	ELSE
		porirpf := bs.val;
	END IF;

	-- Buscamos los decuentos generales a aplicar.
	SELECT INTO bs 1 - SUM(proporciondfactura) / 100 AS tdesc FROM dfactura WHERE idfactura = NEW.idfactura;
	IF bs.tdesc IS NULL THEN
		totaldesc := 1;
	ELSE
		totaldesc := bs.tdesc;
	END IF;


	-- IVA for para diferentes tipos de IVA
	FOR bs IN SELECT SUM (cantlfactura * pvplfactura * (100 - descuentolfactura) / 100) AS base, round(ivalfactura) AS iva, ivalfactura FROM lfactura WHERE idfactura = NEW.idfactura GROUP BY ivalfactura LOOP

	      IF bs.iva <> 0 THEN
		      varaux := '4770%' || bs.iva;
		      -- Hacemos la busqueda de la cuenta de IVA correspondiente.
		      SELECT INTO ctaiva * FROM bc_cuenta WHERE codigo LIKE varaux;
		      IF NOT FOUND THEN
			      PERFORM dblink_exec('bulmafact2cont', 'ROLLBACK WORK;');
			      RAISE EXCEPTION 'No existe una cuenta de IVA para el tipo de IVA %', bs.iva;
		      END IF;
		      cta := ctaiva.codigo;
		      idcta := ctaiva.idcuenta;

		      -- Hacemos la insercion del borrador del apunte.
		      query := 'INSERT INTO borrador (fecha, idcuenta, haber, idasiento, descripcion, conceptocontable) VALUES (''' || NEW.ffactura || ''', ' || idcta || ', ' || round(bs.base * totaldesc * bs.iva / 100, redondeo) || ', '|| NEW.idasientofactura ||', ''' || concepto1 || ''' ,  ''Factura Cliente'')';
		      PERFORM dblink_exec('bulmafact2cont', query);

		      -- Vamos calculando el total de IVA
		      totaliva := totaliva + round(bs.base * totaldesc * bs.iva / 100, redondeo);
	      END IF;

	END LOOP;

	-- Recargo de equivalencia
	FOR bs IN SELECT SUM (cantlfactura * pvplfactura * (100 - descuentolfactura) / 100) AS base, round(reqeqlfactura) AS req, reqeqlfactura FROM lfactura WHERE idfactura = NEW.idfactura GROUP BY reqeqlfactura LOOP

	      IF bs.reqeqlfactura <> 0 THEN
		      varaux := '47710%' || bs.req;

		      -- Hacemos la busqueda de la cuenta de Recargo correspondiente.
		      SELECT INTO ctareq * FROM bc_cuenta WHERE codigo LIKE varaux;
		      IF NOT FOUND THEN
			      PERFORM dblink_exec('bulmafact2cont', 'ROLLBACK WORK;');
			      RAISE EXCEPTION 'No existe una cuenta (4771xxx) de Recargo para el tipo de Recargo %', bs.reqeqlfactura;
		      END IF;
		      cta := ctareq.codigo;
		      idcta := ctareq.idcuenta;

		      -- Hacemos la insercion del borrador del apunte.
		      query := 'INSERT INTO borrador (fecha, idcuenta, haber, idasiento, descripcion, conceptocontable) VALUES (''' || NEW.ffactura || ''', ' || idcta || ', ' || round(bs.base * totaldesc * bs.reqeqlfactura / 100, redondeo) || ', '|| NEW.idasientofactura ||', ''' || concepto1 || ''' ,  ''Factura Cliente'')';
		      PERFORM dblink_exec('bulmafact2cont', query);

		      -- Vamos calculando el total de IVA
		      totaliva := totaliva + round(bs.base * totaldesc * bs.reqeqlfactura / 100, redondeo);
	      END IF;

	END LOOP;


	-- Creamos el apunte de cliente.
	SELECT INTO bs SUM (cantlfactura * pvplfactura * (100 - descuentolfactura) / 100) AS base FROM lfactura WHERE idfactura = NEW.idfactura;
	-- Hacemos la insercion del borrador del apunte.
	query := 'INSERT INTO borrador (fecha, idcuenta, debe, idasiento, descripcion, conceptocontable) VALUES (''' || NEW.ffactura || ''', ' || idctacliente || ', ' || round(bs.base * totaldesc * (1-porirpf) + totaliva, redondeo) || ', '|| NEW.idasientofactura ||', ''' || concepto1 || ''' ,  ''Factura Cliente'')';
	PERFORM dblink_exec('bulmafact2cont', query);

	-- Creamos el apunte de IRPF
	IF porirpf <> 0 THEN
		-- Hacemos la insercion del borrador del apunte.
		query := 'INSERT INTO borrador (fecha, idcuenta, debe, idasiento, descripcion, conceptocontable) VALUES (''' || NEW.ffactura || ''', ' || idctairpf || ', ' || round(bs.base * totaldesc * porirpf, redondeo) || ', '|| NEW.idasientofactura ||', ''' || concepto1 || ''' ,  ''Factura Cliente'')';
		PERFORM dblink_exec('bulmafact2cont', query);
	END IF;

	-- Hacemos la insercion del borrador del apunte segun familia.
	FOR cs IN SELECT familia.idcuentaventafamilia, SUM (cantlfactura * pvplfactura * (100 - descuentolfactura) / 100) AS base FROM familia LEFT JOIN articulo ON familia.idfamilia = articulo.idfamilia LEFT JOIN lfactura ON lfactura.idarticulo = articulo.idarticulo WHERE lfactura.idfactura = NEW.idfactura GROUP BY familia.idcuentaventafamilia LOOP

	    query := 'INSERT INTO borrador (fecha, idcuenta, haber, idasiento, descripcion, conceptocontable) VALUES (''' || NEW.ffactura || ''', ' || cs.idcuentaventafamilia || ', ' || round(cs.base * totaldesc, redondeo) || ', '|| NEW.idasientofactura ||', ''' || concepto1 || ''' ,  ''Factura Cliente'')';
	    PERFORM dblink_exec('bulmafact2cont', query);

	END LOOP;

	PERFORM dblink_exec('bulmafact2cont', 'COMMIT WORK;');
	PERFORM dblink_disconnect('bulmafact2cont');
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
CREATE OR REPLACE FUNCTION syncbulmacontfacturapd () RETURNS "trigger"
AS $$
DECLARE
	query TEXT;
BEGIN
	PERFORM conectabulmacont();
	-- Hacemos el update del stock del articulo
	IF OLD.idasientofacturap IS NOT NULL THEN
		query := 'DELETE FROM registroiva WHERE idborrador IN (SELECT idborrador FROM borrador WHERE idasiento = ' || OLD.idasientofacturap || ')';
		PERFORM dblink_exec('bulmafact2cont', query);
		query := 'DELETE FROM apunte WHERE idasiento= ' || OLD.idasientofacturap;
		PERFORM dblink_exec('bulmafact2cont', query);
		query := 'DELETE FROM  borrador WHERE idasiento= ' || OLD.idasientofacturap ;
		PERFORM dblink_exec('bulmafact2cont', query);
		query := 'DELETE FROM  asiento WHERE idasiento = ' || OLD.idasientofacturap ;
		PERFORM dblink_exec('bulmafact2cont', query);
	END IF;

	PERFORM dblink_disconnect('bulmafact2cont');
	RETURN OLD;
END;
$$ LANGUAGE plpgsql;


CREATE TRIGGER syncbulmacontfacturaptriggerd
    AFTER DELETE ON facturap
    FOR EACH ROW
    EXECUTE PROCEDURE syncbulmacontfacturapd();

\echo "Creado el Trigger que al borrar una factura de proveedor la borra de la contabilidad"



SELECT drop_if_exists_proc ('syncbulmacontfacturapu','');
CREATE OR REPLACE FUNCTION syncbulmacontfacturapu () RETURNS "trigger"
AS $$
DECLARE
	bs RECORD;
	cs RECORD;
	provider RECORD;
	ctaiva RECORD;
	cta TEXT;
	idcta TEXT;
	idctaproveedor TEXT;
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
	asientonuevo BOOLEAN;
	redondeo INTEGER := 2;
BEGIN
	-- conectamos con contabilidad, etc
	PERFORM conectabulmacont();
	PERFORM dblink_exec('bulmafact2cont', 'BEGIN WORK;');

	concepto := '[A.Automatico] Factura Proveedor Num: ' || NEW.numfacturap;
	concepto1 := 'Factura Proveedor Num: ' || NEW.numfacturap;

	-- Puede darse el caso de que el contable haya borrado el asiento. Y por eso comprobamos que realmente exista en la contabilidad.
	asientonuevo := TRUE;
	IF NEW.idasientofacturap IS NOT NULL THEN
		query := 'SELECT idasiento FROM asiento WHERE idasiento =' || NEW.idasientofacturap;
		SELECT INTO cs * FROM dblink('bulmafact2cont', query) AS t1 (idasiento integer);
		IF FOUND THEN
			asientonuevo := FALSE;
		END IF;
	END IF;

	-- Hacemos el update del stock del articulo
	IF asientonuevo IS FALSE THEN
		query := 'DELETE FROM registroiva WHERE idborrador IN (SELECT idborrador FROM borrador WHERE idasiento = ' || NEW.idasientofacturap || ')';
		PERFORM dblink_exec('bulmafact2cont', query);
		query := 'DELETE FROM apunte WHERE idasiento= ' || NEW.idasientofacturap;
		PERFORM dblink_exec('bulmafact2cont', query);
		query := 'DELETE FROM borrador WHERE idasiento= ' || NEW.idasientofacturap;
		PERFORM dblink_exec('bulmafact2cont', query);
		query := 'UPDATE asiento SET descripcion =''' || concepto || ''', comentariosasiento = ''' || concepto || ''' WHERE idasiento = ' || NEW.idasientofacturap;
		PERFORM dblink_exec('bulmafact2cont', query);
	ELSE
		-- Hacemos el update del stock del articulo
		query := 'INSERT INTO asiento (fecha, descripcion, comentariosasiento) VALUES ( ' || quote_literal(NEW.ffacturap) || ', ''' || concepto ||  ''', ''' || concepto || ''')';
		PERFORM dblink_exec('bulmafact2cont', query);
		SELECT INTO bs * FROM dblink('bulmafact2cont', 'SELECT max(idasiento) AS id FROM asiento') AS t1 (id integer);
		NEW.idasientofacturap = bs.id;
	END IF;


	-- Buscamos el proveedor y su cuenta.
	SELECT INTO provider idcuentaproveedor FROM proveedor WHERE idproveedor=NEW.idproveedor;
	IF NOT FOUND THEN
		RAISE EXCEPTION 'El proveedor no tiene cuenta asociada en la contabilidad';
	END IF;
	idctaproveedor := provider.idcuentaproveedor;

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


	-- Buscamos el IRPF que aplicamos
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
	FOR bs IN SELECT SUM (cantlfacturap*pvplfacturap*(100-descuentolfacturap)/100) AS base, round(ivalfacturap) AS iva, ivalfacturap, reqeqlfacturap FROM lfacturap  WHERE idfacturap = NEW.idfacturap GROUP BY ivalfacturap, reqeqlfacturap LOOP
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
			query := 'INSERT INTO borrador (fecha, idcuenta, debe, idasiento, descripcion, conceptocontable) VALUES (''' || NEW.ffacturap || ''', ' || idcta || ', ' || round(bs.base * totaldesc * bs.iva / 100, redondeo) || ', '|| NEW.idasientofacturap ||', ''' || concepto1 || ''' ,  ''Factura Proveedor'')';
			PERFORM dblink_exec('bulmafact2cont', query);

			-- Vamos calculando el total de IVA
			totaliva := totaliva + round(bs.base * totaldesc * bs.iva / 100, redondeo);
		END IF;
		IF bs.reqeqlfacturap <> 0 THEN
			varaux := '47201%' || bs.iva;
			-- Hacemos la busqueda de la cuenta de Recargo correspondiente.
			SELECT INTO ctaiva * FROM bc_cuenta WHERE codigo LIKE varaux;
			IF NOT FOUND THEN
				RAISE EXCEPTION 'No existe una cuenta (47201xx) de Recargo para el tipo de Recargo %', bs.reqeqlfacturap;
			END IF;
			cta := ctaiva.codigo;
			idcta := ctaiva.idcuenta;

			-- Hacemos la insercion del borrador del apunte.
			query := 'INSERT INTO borrador (fecha, idcuenta, debe, idasiento, descripcion, conceptocontable) VALUES (''' || NEW.ffacturap || ''', ' || idcta || ', ' || round(bs.base * totaldesc * bs.reqeqlfacturap / 100, redondeo) || ', '|| NEW.idasientofacturap ||', ''' || concepto1 || ''', ''Factura Proveedor'')';
			PERFORM dblink_exec('bulmafact2cont', query);

			-- Vamos calculando el total de IVA
			totaliva := totaliva + round(bs.base * totaldesc * bs.reqeqlfacturap / 100, redondeo);
		END IF;
	END LOOP;

	-- Calculamos el total de la factura.
	SELECT INTO bs SUM (cantlfacturap*pvplfacturap*(100-descuentolfacturap)/100) AS base FROM lfacturap WHERE idfacturap = NEW.idfacturap;

	-- Hacemos la insercion del borrador del apunte al proveedor.
	total := bs.base * totaldesc * (1 - porirpf) + totaliva;
	query := 'INSERT INTO borrador (fecha, idcuenta, haber, idasiento, descripcion, conceptocontable) VALUES (''' || NEW.ffacturap || ''', ' || idctaproveedor || ', ' || total || ', '|| NEW.idasientofacturap ||', ''' || concepto1 || ''' , ''Factura Proveedor'')';
	PERFORM dblink_exec('bulmafact2cont', query);


	-- Creamos el apunte de IRPF
	IF porirpf <> 0 THEN
		-- Hacemos la insercion del borrador del apunte.
		query := 'INSERT INTO borrador (fecha, idcuenta, haber, idasiento, descripcion, conceptocontable) VALUES (''' || NEW.ffacturap || ''', ' || idctairpf || ', ' || round(bs.base * totaldesc * porirpf, redondeo) || ', '|| NEW.idasientofacturap ||', ''' || concepto1 || ''' , ''Factura Proveedor'')';
		PERFORM dblink_exec('bulmafact2cont', query);
	END IF;


	-- Hacemos la insercion del borrador del apunte segun familia.
	FOR cs IN SELECT familia.idcuentacomprafamilia, SUM (cantlfacturap * pvplfacturap * (100 - descuentolfacturap) / 100) AS base FROM familia LEFT JOIN articulo ON familia.idfamilia = articulo.idfamilia LEFT JOIN lfacturap ON lfacturap.idarticulo = articulo.idarticulo WHERE lfacturap.idfacturap = NEW.idfacturap GROUP BY familia.idcuentacomprafamilia LOOP

	    query := 'INSERT INTO borrador (fecha, idcuenta, debe, idasiento, descripcion, conceptocontable) VALUES (''' || NEW.ffacturap || ''', ' || cs.idcuentacomprafamilia || ', ' || round(cs.base * totaldesc, redondeo) || ', '|| NEW.idasientofacturap ||', ''' || concepto1 || ''' , ''Factura Proveedor'')';
	    PERFORM dblink_exec('bulmafact2cont', query);

	END LOOP;


	PERFORM dblink_exec('bulmafact2cont', 'COMMIT WORK;');
	PERFORM dblink_disconnect('bulmafact2cont');
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
CREATE OR REPLACE FUNCTION syncbulmacontcliented () RETURNS "trigger"
AS $$
DECLARE
	query TEXT;
BEGIN
	PERFORM conectabulmacont();
	PERFORM dblink_exec('bulmafact2cont', 'BEGIN WORK;');

	IF OLD.idcuentacliente IS NOT NULL THEN
		query := 'DELETE FROM cuenta WHERE idcuenta = ' || OLD.idcuentacliente;
		PERFORM dblink_exec('bulmafact2cont', query);
	END IF;

	PERFORM dblink_exec('bulmafact2cont', 'COMMIT WORK;');
	PERFORM dblink_disconnect('bulmafact2cont');
	RETURN OLD;
END;
$$ LANGUAGE plpgsql;


CREATE TRIGGER syncbulmacontclientetriggerd
    AFTER DELETE ON cliente
    FOR EACH ROW
    EXECUTE PROCEDURE syncbulmacontcliented();





SELECT drop_if_exists_proc ('syncbulmacontclienteupre','');
CREATE OR REPLACE FUNCTION syncbulmacontclienteupre () RETURNS "trigger"
AS $$
DECLARE
BEGIN

      PERFORM conectabulmacont();

      -- Es necesario controlar la transaccion que se hace en la base de datos enlazada porque no actua
      -- dentro de la transaccion de esta funcion y si hay un error no desaria los cambios provocando problemas
      -- de integridad de los datos.
      -- Hay que hacer un ROLLBACK WORK explicito ANTES de cada RAISE EXCEPTION.
      PERFORM dblink_exec('bulmafact2cont', 'BEGIN WORK;');

      RETURN NULL;

END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER syncbulmacontclientetriggerupre
    BEFORE UPDATE OR INSERT ON cliente
    FOR EACH STATEMENT
    EXECUTE PROCEDURE syncbulmacontclienteupre();





SELECT drop_if_exists_proc ('syncbulmacontclienteu','');
CREATE OR REPLACE FUNCTION syncbulmacontclienteu () RETURNS "trigger"
AS $$
DECLARE
	bs RECORD;
	cs RECORD;
	quer TEXT;
	subquery TEXT;
	codcta TEXT;
	idpadre INTEGER;
	descripcion TEXT;
	cp TEXT;
	cif TEXT;
	cuentasiguiente TEXT;

BEGIN

	-- Cogemos el nombre de la cuenta.
	descripcion := quote_literal(NEW.nomcliente);
	cp := substring(NEW.cpcliente, length(NEW.cpcliente)-4, 5);
	cif := substring(NEW.cifcliente, length(NEW.cifcliente)-11, 12);


	SELECT INTO cs count(idcuenta) AS c_idcuenta FROM bc_cuenta WHERE idcuenta = NEW.idcuentacliente;

	IF (NEW.idcuentacliente IS NULL) OR (cs.c_idcuenta = 0) THEN
		-- Buscamos el codigo de cuenta que vaya a corresponderle
		-- El codigo esta formado por una parte fija y otra que se calcula automaticamente.
		-- Se evita de esta manera la limitacion a 100 cuentas.
		SELECT INTO bs MAX(codigo::BIGINT) as cod FROM bc_cuenta WHERE codigo LIKE '43000%' ;

		IF bs.cod IS NOT NULL THEN
			codcta := bs.cod + 1;
		ELSE
			codcta := '4300001';
		END IF;
	
		-- Buscamos la cuenta padre (la 4300)
		SELECT INTO bs idcuenta FROM bc_cuenta WHERE codigo ='4300';
		idpadre := bs.idcuenta;

		-- Creamos el Query de insercion
		quer := 'INSERT INTO cuenta (descripcion, padre, codigo) VALUES ( ' || descripcion ||', ' || idpadre || ', ''' || codcta || ''' )';
		PERFORM dblink_exec('bulmafact2cont', quer);
	
		SELECT INTO bs max(idcuenta) AS id FROM bc_cuenta;
		NEW.idcuentacliente = bs.id;

	ELSE
		quer := 'UPDATE cuenta SET descripcion = ' || descripcion || ', cifent_cuenta = ''' || COALESCE(cif, '') || ''', nombreent_cuenta = ''' || COALESCE(NEW.nomaltcliente, '') || ''', dirent_cuenta = ''' || COALESCE(NEW.dircliente, '') || ''', cpent_cuenta = ''' || COALESCE(cp, '') || ''', telent_cuenta = ''' || COALESCE(NEW.telcliente, '') || ''', bancoent_cuenta = ''' || COALESCE(NEW.bancocliente, '') || ''', emailent_cuenta = ''' || COALESCE(NEW.mailcliente, '') || ''', webent_cuenta = ''' || COALESCE(NEW.urlcliente, '') || ''' WHERE idcuenta = ' || NEW.idcuentacliente;
		PERFORM dblink_exec('bulmafact2cont', quer);
	END IF;

	RETURN NEW;
END;
$$ LANGUAGE plpgsql;


CREATE TRIGGER syncbulmacontclientetriggeru
    BEFORE UPDATE OR INSERT ON cliente
    FOR EACH ROW
    EXECUTE PROCEDURE syncbulmacontclienteu();


SELECT drop_if_exists_proc ('syncbulmacontclienteup','');
CREATE OR REPLACE FUNCTION syncbulmacontclienteup () RETURNS "trigger"
AS $$
DECLARE
      check_dblink_connection BOOLEAN;
BEGIN

      check_dblink_connection = dblink_exists('bulmafact2cont');
      
      IF check_dblink_connection IS TRUE THEN
	  -- Hace un COMMIT de la conexion dblink si todo ha ido bien.

	  PERFORM dblink_exec('bulmafact2cont', 'COMMIT WORK;');
	  PERFORM dblink_disconnect('bulmafact2cont');
      ELSE
	  RAISE EXCEPTION 'Se ha perdido la conexion dblink en syncbulmacontclienteup.';
      END IF;

      RETURN NULL;

END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER syncbulmacontclientetriggerup
    AFTER UPDATE OR INSERT ON cliente
    FOR EACH STATEMENT
    EXECUTE PROCEDURE syncbulmacontclienteup();



-- ==============================================================================
-- TRATO DE LOS PROVEEDORES
-- ==============================================================================


SELECT drop_if_exists_proc ('syncbulmacontproveedord','');
CREATE OR REPLACE FUNCTION syncbulmacontproveedord () RETURNS "trigger"
AS $$
DECLARE
	query TEXT;
BEGIN
	PERFORM conectabulmacont();
	PERFORM dblink_exec('bulmafact2cont', 'BEGIN WORK;');

	IF OLD.idcuentaproveedor IS NOT NULL THEN
		query := 'DELETE FROM cuenta WHERE idcuenta = ' || OLD.idcuentaproveedor ;
		PERFORM dblink_exec('bulmafact2cont', query);
	END IF;

	PERFORM dblink_exec('bulmafact2cont', 'COMMIT WORK;');
	PERFORM dblink_disconnect('bulmafact2cont');
	RETURN OLD;
END;
$$ LANGUAGE plpgsql;


CREATE TRIGGER syncbulmacontproveedortriggerd
    AFTER DELETE ON proveedor
    FOR EACH ROW
    EXECUTE PROCEDURE syncbulmacontproveedord();




SELECT drop_if_exists_proc ('syncbulmacontproveedorupre','');
CREATE OR REPLACE FUNCTION syncbulmacontproveedorupre () RETURNS "trigger"
AS $$
DECLARE
BEGIN

      PERFORM conectabulmacont();

      -- Es necesario controlar la transaccion que se hace en la base de datos enlazada porque no actua
      -- dentro de la transaccion de esta funcion y si hay un error no desaria los cambios provocando problemas
      -- de integridad de los datos.
      -- Hay que hacer un ROLLBACK WORK explicito ANTES de cada RAISE EXCEPTION.
      PERFORM dblink_exec('bulmafact2cont', 'BEGIN WORK;');

      RETURN NULL;

END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER syncbulmacontproveedortriggerupre
    BEFORE UPDATE OR INSERT ON proveedor
    FOR EACH STATEMENT
    EXECUTE PROCEDURE syncbulmacontproveedorupre();



SELECT drop_if_exists_proc ('syncbulmacontproveedoru','');
CREATE OR REPLACE FUNCTION syncbulmacontproveedoru () RETURNS "trigger"
AS $$
DECLARE
	bs RECORD;
	cs RECORD;
	quer TEXT;
	subquery TEXT;
	flecha DATE;
	codcta INTEGER;
	idpadre INTEGER;
	grupo INTEGER;
	descripcion TEXT;
BEGIN
	-- Cogemos el nombre de la cuenta.
	descripcion := quote_literal(NEW.nomproveedor);


	SELECT INTO cs count(idcuenta) AS p_idcuenta FROM bc_cuenta WHERE idcuenta = NEW.idcuentaproveedor;

	IF (NEW.idcuentaproveedor IS NULL) OR (cs.p_idcuenta = 0) THEN
		-- Buscamos el codigo de cuenta que vaya a corresponderle
		SELECT INTO bs MAX(codigo::BIGINT) as cod FROM bc_cuenta WHERE codigo LIKE '40000%' ;

		IF bs.cod IS NOT NULL THEN
			codcta := bs.cod + 1;
		ELSE
			codcta := '4000001';
		END IF;
	
		-- Buscamos la cuenta padre (la 4000)
		SELECT INTO bs idcuenta FROM bc_cuenta WHERE codigo = '4000';
		idpadre := bs.idcuenta;
	
		-- Creamos el Query de insercion
		quer := 'INSERT INTO cuenta (descripcion, padre, codigo) VALUES ( ' || descripcion ||', ' || idpadre || ', ''' || codcta || ''' )';
		PERFORM dblink_exec('bulmafact2cont', quer);
	
		SELECT INTO bs MAX(idcuenta) AS id FROM bc_cuenta;
		NEW.idcuentaproveedor = bs.id;
	ELSE

		quer := 'UPDATE cuenta SET descripcion = ' || descripcion || ', cifent_cuenta = ''' || NEW.cifproveedor || ''', nombreent_cuenta = ''' || COALESCE(NEW.nomaltproveedor, '') || ''', dirent_cuenta = ''' || COALESCE(NEW.dirproveedor, '') || ''', cpent_cuenta = ''' || COALESCE(NEW.cpproveedor, '') || ''', telent_cuenta = ''' || COALESCE(NEW.telproveedor, '') || ''', emailent_cuenta = ''' || COALESCE(NEW.emailproveedor, '') || ''', webent_cuenta = ''' || COALESCE(NEW.urlproveedor, '') || ''' WHERE idcuenta = ' || NEW.idcuentaproveedor;
		PERFORM dblink_exec('bulmafact2cont', quer);

	END IF;

	RETURN NEW;
END;
$$ LANGUAGE plpgsql;


CREATE TRIGGER syncbulmacontproveedortriggeru
    BEFORE INSERT OR UPDATE ON proveedor
    FOR EACH ROW
    EXECUTE PROCEDURE syncbulmacontproveedoru();



SELECT drop_if_exists_proc ('syncbulmacontproveedorup','');
CREATE OR REPLACE FUNCTION syncbulmacontproveedorup () RETURNS "trigger"
AS $$
DECLARE
      check_dblink_connection BOOLEAN;
BEGIN

      check_dblink_connection = dblink_exists('bulmafact2cont');
      
      IF check_dblink_connection IS TRUE THEN
	  -- Hace un COMMIT de la conexion dblink si todo ha ido bien.
	  PERFORM dblink_exec('bulmafact2cont', 'COMMIT WORK;');
	  PERFORM dblink_disconnect('bulmafact2cont');
      ELSE
	  RAISE EXCEPTION 'Se ha perdido la conexion dblink en syncbulmacontproveedorup.';
      END IF;

      RETURN NULL;

END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER syncbulmacontproveedortriggerup
    AFTER UPDATE OR INSERT ON proveedor
    FOR EACH STATEMENT
    EXECUTE PROCEDURE syncbulmacontproveedorup();



-- =================================================================
--           TRATO DE LOS BANCOS
-- =================================================================

SELECT drop_if_exists_proc ('syncbulmacontbancod','');
CREATE OR REPLACE FUNCTION syncbulmacontbancod () RETURNS "trigger"
AS $$
DECLARE
	query TEXT;
BEGIN
	PERFORM conectabulmacont();
	PERFORM dblink_exec('bulmafact2cont', 'BEGIN WORK;');

	-- Hacemos el DELETE de la cuenta de banco.
	IF OLD.idcuentabanco IS NOT NULL THEN
		query := 'DELETE FROM cuenta WHERE idcuenta = ' || OLD.idcuentabanco;
		PERFORM dblink_exec('bulmafact2cont', query);
	END IF;

	PERFORM dblink_exec('bulmafact2cont', 'COMMIT WORK;');
	PERFORM dblink_disconnect('bulmafact2cont');
	RETURN OLD;
END;
$$ LANGUAGE plpgsql;



CREATE TRIGGER syncbulmacontbancotriggerd
    AFTER DELETE ON banco
    FOR EACH ROW
    EXECUTE PROCEDURE syncbulmacontbancod();
\echo "Creado el trigger que al borrar un banco borra la respectiva cuenta en la contabilidad"




SELECT drop_if_exists_proc ('syncbulmacontbancou','');
CREATE OR REPLACE FUNCTION syncbulmacontbancou () RETURNS "trigger"
AS $$
DECLARE
	cs RECORD;
	bs RECORD;
	codcta INTEGER;
	quer TEXT;
	idpadre INTEGER;
	descripcion TEXT;
	tipocuenta1 INTEGER;
	query TEXT;
	check_dblink_connection BOOLEAN;
	altacuentabanco BOOLEAN;

BEGIN
	-- conectamos con contabilidad, etc
	PERFORM conectabulmacont();
	PERFORM dblink_exec('bulmafact2cont', 'BEGIN WORK;');

	-- Cogemos el nombre de la cuenta.
	descripcion := quote_literal(NEW.nombanco);

	altacuentabanco := FALSE;

	-- Entra si es un INSERT o cuando la cuenta 'idcuentabanco' apunta a la contabilidad
	-- pero en realidad no existe. Asi ejecutamos su creacion.
	SELECT INTO cs idcuenta FROM bc_cuenta WHERE idcuenta = NEW.idcuentabanco;
	IF NOT FOUND THEN
	    altacuentabanco := TRUE;
	END IF;


	IF (TG_OP = 'INSERT') OR (altacuentabanco IS TRUE) THEN
	    -- Buscamos el codigo de cuenta que vaya a corresponderle.
	    SELECT INTO cs MAX(codigo::INTEGER) AS cod FROM bc_cuenta WHERE codigo LIKE '5720%';
	    IF cs.cod IS NOT NULL THEN
		    codcta := cs.cod + 1;
	    ELSE
		    codcta := '5720001';
	    END IF;

	    -- Buscamos la cuenta padre (la 573)
	    SELECT INTO bs idcuenta, tipocuenta FROM bc_cuenta WHERE codigo ='572';
	    idpadre := bs.idcuenta;
	    tipocuenta1 := bs.tipocuenta;

	    -- Creamos el Query de insercion
	    quer := 'INSERT INTO cuenta (descripcion, padre, codigo, tipocuenta) VALUES ( ' || descripcion ||', ' || idpadre || ', ''' || codcta || ''' , '|| tipocuenta1 || ')';
	    PERFORM dblink_exec('bulmafact2cont', quer);

	    SELECT INTO bs MAX(idcuenta) AS id FROM bc_cuenta;
	    NEW.idcuentabanco := bs.id;
	END IF;


	IF (TG_OP = 'UPDATE') THEN

		-- No se ha cambiado. Solo actualiza datos.
		quer := 'UPDATE cuenta SET descripcion = ' || descripcion || ' WHERE idcuenta = ' || NEW.idcuentabanco;
		PERFORM dblink_exec('bulmafact2cont', quer);

	END IF;

	RETURN NEW;
END;
$$ LANGUAGE plpgsql;


CREATE TRIGGER syncbulmacontbancotriggeru
    BEFORE INSERT OR UPDATE ON banco
    FOR EACH ROW
    EXECUTE PROCEDURE syncbulmacontbancou();

\echo "Creado el trigger que al modificar o insertar un banco en la facturacion mete el correspondiente asiento en la contabilidad"




SELECT drop_if_exists_proc ('syncbulmacontbancoup','');
CREATE OR REPLACE FUNCTION syncbulmacontbancoup () RETURNS "trigger"
AS $$
DECLARE
      check_dblink_connection BOOLEAN;
BEGIN

      check_dblink_connection = dblink_exists('bulmafact2cont');
      
      IF check_dblink_connection IS TRUE THEN
	  -- Hace un COMMIT de la conexion dblink si todo ha ido bien.
	  PERFORM dblink_exec('bulmafact2cont', 'COMMIT WORK;');
	  PERFORM dblink_disconnect('bulmafact2cont');
      ELSE
	  RAISE EXCEPTION 'Se ha perdido la conexion dblink en syncbulmacontbancoup.';
      END IF;

      RETURN NULL;

END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER syncbulmacontbancotriggerup
    AFTER UPDATE OR INSERT ON banco
    FOR EACH STATEMENT
    EXECUTE PROCEDURE syncbulmacontbancoup();




-- =================================================================
--           TRATO DE LAS FORMAS DE PAGO
-- =================================================================

SELECT drop_if_exists_proc ('syncbulmacontforma_pagod','');
CREATE OR REPLACE FUNCTION syncbulmacontforma_pagod () RETURNS "trigger"
AS $$
DECLARE
	query TEXT;
BEGIN
	PERFORM conectabulmacont();
	PERFORM dblink_exec('bulmafact2cont', 'BEGIN WORK;');

	-- Hacemos el DELETE de la cuenta de forma_pago.
	IF OLD.idcuentaforma_pago IS NOT NULL THEN
		query := 'DELETE FROM cuenta WHERE idcuenta = ' || OLD.idcuentaforma_pago;
		PERFORM dblink_exec('bulmafact2cont', query);
	END IF;

	PERFORM dblink_exec('bulmafact2cont', 'COMMIT WORK;');
	PERFORM dblink_disconnect('bulmafact2cont');
	RETURN OLD;
END;
$$ LANGUAGE plpgsql;



CREATE TRIGGER syncbulmacontforma_pagotriggerd
    AFTER DELETE ON forma_pago
    FOR EACH ROW
    EXECUTE PROCEDURE syncbulmacontforma_pagod();
\echo "Creado el trigger que al borrar una forma de pago borra la respectiva cuenta en la contabilidad"




SELECT drop_if_exists_proc ('syncbulmacontforma_pagou','');
CREATE OR REPLACE FUNCTION syncbulmacontforma_pagou () RETURNS "trigger"
AS $$
DECLARE
	cs RECORD;
	bs RECORD;
	codcta INTEGER;
	quer TEXT;
	idpadre INTEGER;
	descripcion TEXT;
	tipocuenta1 INTEGER;
	query TEXT;
	check_dblink_connection BOOLEAN;
	altacuentaforma_pago BOOLEAN;

BEGIN
	-- conectamos con contabilidad, etc
	PERFORM conectabulmacont();
	PERFORM dblink_exec('bulmafact2cont', 'BEGIN WORK;');

	-- Cogemos el nombre de la cuenta.
	descripcion := quote_literal(NEW.descforma_pago);

	altacuentaforma_pago := FALSE;

	-- Entra si es un INSERT o cuando la cuenta 'idcuentaforma_pago' apunta a la contabilidad
	-- pero en realidad no existe. Asi ejecutamos su creacion.
	SELECT INTO cs idcuenta FROM bc_cuenta WHERE idcuenta = NEW.idcuentaforma_pago;
	IF NOT FOUND THEN
	    altacuentaforma_pago := TRUE;
	END IF;


	IF (TG_OP = 'INSERT') OR (altacuentaforma_pago IS TRUE) THEN
	    -- Buscamos el codigo de cuenta que vaya a corresponderle.
	    SELECT INTO cs MAX(codigo::INTEGER) AS cod FROM bc_cuenta WHERE codigo LIKE '5700%';
	    IF cs.cod IS NOT NULL THEN
		    codcta := cs.cod + 1;
	    ELSE
		    codcta := '5700001';
	    END IF;

	    -- Buscamos la cuenta padre (la 570)
	    SELECT INTO bs idcuenta, tipocuenta FROM bc_cuenta WHERE codigo ='570';
	    idpadre := bs.idcuenta;
	    tipocuenta1 := bs.tipocuenta;

	    -- Creamos el Query de insercion
	    quer := 'INSERT INTO cuenta (descripcion, padre, codigo, tipocuenta) VALUES ( ' || descripcion ||', ' || idpadre || ', ''' || codcta || ''' , '|| tipocuenta1 || ')';
	    PERFORM dblink_exec('bulmafact2cont', quer);

	    SELECT INTO bs MAX(idcuenta) AS id FROM bc_cuenta;
	    NEW.idcuentaforma_pago := bs.id;
	END IF;


	IF (TG_OP = 'UPDATE') THEN

		-- No se ha cambiado. Solo actualiza datos.
		quer := 'UPDATE cuenta SET descripcion = ' || descripcion || ' WHERE idcuenta = ' || NEW.idcuentaforma_pago;
		PERFORM dblink_exec('bulmafact2cont', quer);

	END IF;

	RETURN NEW;
END;
$$ LANGUAGE plpgsql;


CREATE TRIGGER syncbulmacontforma_pagotriggeru
    BEFORE INSERT OR UPDATE ON forma_pago
    FOR EACH ROW
    EXECUTE PROCEDURE syncbulmacontforma_pagou();

\echo "Creado el trigger que al modificar o insertar una forma de pago en la facturacion mete el correspondiente asiento en la contabilidad"




SELECT drop_if_exists_proc ('syncbulmacontforma_pagoup','');
CREATE OR REPLACE FUNCTION syncbulmacontforma_pagoup () RETURNS "trigger"
AS $$
DECLARE
      check_dblink_connection BOOLEAN;
BEGIN

      check_dblink_connection = dblink_exists('bulmafact2cont');
      
      IF check_dblink_connection IS TRUE THEN
	  -- Hace un COMMIT de la conexion dblink si todo ha ido bien.
	  PERFORM dblink_exec('bulmafact2cont', 'COMMIT WORK;');
	  PERFORM dblink_disconnect('bulmafact2cont');
      ELSE
	  RAISE EXCEPTION 'Se ha perdido la conexion dblink en syncbulmacontforma_pagoup.';
      END IF;

      RETURN NULL;

END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER syncbulmacontforma_pagotriggerup
    AFTER UPDATE OR INSERT ON forma_pago
    FOR EACH STATEMENT
    EXECUTE PROCEDURE syncbulmacontforma_pagoup();




-- =================================================================
--           TRATO DE LOS ALMACENES
-- =================================================================

SELECT drop_if_exists_proc ('syncbulmacontalmacend','');
CREATE OR REPLACE FUNCTION syncbulmacontalmacend () RETURNS "trigger"
AS $$
DECLARE
	query TEXT;
BEGIN
	PERFORM conectabulmacont();
	PERFORM dblink_exec('bulmafact2cont', 'BEGIN WORK;');

	IF OLD.idcuentaalmacen IS NOT NULL THEN
		query := 'DELETE FROM cuenta WHERE idcuenta = ' || OLD.idcuentaalmacen;
		PERFORM dblink_exec('bulmafact2cont', query);
	END IF;

	PERFORM dblink_exec('bulmafact2cont', 'COMMIT WORK;');
	PERFORM dblink_disconnect('bulmafact2cont');
	RETURN OLD;
END;
$$ LANGUAGE plpgsql;


CREATE TRIGGER syncbulmacontalmacentriggerd
    AFTER DELETE ON almacen
    FOR EACH ROW
    EXECUTE PROCEDURE syncbulmacontalmacend();
\echo "Creado el trigger que al borrar un banco borra la respectiva cuenta en la contabilidad"



SELECT drop_if_exists_proc ('syncbulmacontalmacenu','');
CREATE OR REPLACE FUNCTION syncbulmacontalmacenu () RETURNS "trigger"
AS $$
DECLARE
	cs RECORD;
	bs RECORD;
	codcta INTEGER;
	quer TEXT;
	idpadre INTEGER;
	descripcion TEXT;
	tipocuenta1 INTEGER;
BEGIN
	-- conectamos con contabilidad, etc
	PERFORM conectabulmacont();
	PERFORM dblink_exec('bulmafact2cont', 'BEGIN WORK;');

	-- Cogemos el nombre de la cuenta.
	descripcion := quote_literal(NEW.nomalmacen);

	IF NEW.idcuentaalmacen IS NULL THEN
		-- Buscamos el codigo de cuenta que vaya a corresponderle
		SELECT INTO cs MAX(codigo::INTEGER) AS cod FROM bc_cuenta WHERE codigo LIKE '5700%';

		IF cs.cod IS NOT NULL THEN
			codcta := cs.cod + 1;
		ELSE
			codcta := '5700001';
		END IF;
	
		-- Buscamos la cuenta padre (la 5700)
		SELECT INTO bs idcuenta, tipocuenta FROM bc_cuenta WHERE codigo ='570';
		idpadre := bs.idcuenta;
		tipocuenta1 := bs.tipocuenta;
	
		-- Creamos el Query de insercion
		quer := 'INSERT INTO cuenta (descripcion, padre, codigo, tipocuenta) VALUES ( ' || descripcion ||', ' || idpadre || ', ''' || codcta || ''' , '|| tipocuenta1 || ')';
		PERFORM dblink_exec('bulmafact2cont', quer);
	
		SELECT INTO bs MAX(idcuenta) AS id FROM bc_cuenta;
		NEW.idcuentaalmacen := bs.id;
	ELSE
		quer := 'UPDATE cuenta SET descripcion = ' || descripcion || ' WHERE idcuenta = ' || NEW.idcuentaalmacen;
		PERFORM dblink_exec('bulmafact2cont', quer);
	END IF;

	RETURN NEW;
END;
$$ LANGUAGE plpgsql;


CREATE TRIGGER syncbulmacontalmacentriggeru
    BEFORE INSERT OR UPDATE ON almacen
    FOR EACH ROW
    EXECUTE PROCEDURE syncbulmacontalmacenu();

\echo "Creado el trigger que al modificar o insertar un almacen en la facturacion mete el correspondiente asiento en la contabilidad"



SELECT drop_if_exists_proc ('syncbulmacontalmacenup','');
CREATE OR REPLACE FUNCTION syncbulmacontalmacenup () RETURNS "trigger"
AS $$
DECLARE
      check_dblink_connection BOOLEAN;
BEGIN

      check_dblink_connection = dblink_exists('bulmafact2cont');
      
      IF check_dblink_connection IS TRUE THEN
	  -- Hace un COMMIT de la conexion dblink si todo ha ido bien.
	  PERFORM dblink_exec('bulmafact2cont', 'COMMIT WORK;');
	  PERFORM dblink_disconnect('bulmafact2cont');
      ELSE
	  RAISE EXCEPTION 'Se ha perdido la conexion dblink en syncbulmacontalmacenup.';
      END IF;

      RETURN NULL;

END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER syncbulmacontfamiliatriggerup
    AFTER UPDATE OR INSERT ON almacen
    FOR EACH STATEMENT
    EXECUTE PROCEDURE syncbulmacontalmacenup();



-- =================================================================
--           TRATO DE FAMILIAS
-- =================================================================

SELECT drop_if_exists_proc ('syncbulmacontfamiliad','');
CREATE OR REPLACE FUNCTION syncbulmacontfamiliad () RETURNS "trigger"
AS $$
DECLARE
	query TEXT;
BEGIN
	PERFORM conectabulmacont();
	PERFORM dblink_exec('bulmafact2cont', 'BEGIN WORK;');

	-- Hacemos el DELETE de la cuenta de VENTA familia.
	IF OLD.idcuentaventafamilia IS NOT NULL THEN
		query := 'DELETE FROM cuenta WHERE idcuenta = ' || OLD.idcuentaventafamilia;
		PERFORM dblink_exec('bulmafact2cont', query);
	END IF;

	-- Hacemos el DELETE de la cuenta de COMPRA familia.
	IF OLD.idcuentacomprafamilia IS NOT NULL THEN
		query := 'DELETE FROM cuenta WHERE idcuenta = ' || OLD.idcuentacomprafamilia;
		PERFORM dblink_exec('bulmafact2cont', query);
	END IF;


	PERFORM dblink_exec('bulmafact2cont', 'COMMIT WORK;');
	PERFORM dblink_disconnect('bulmafact2cont');
	RETURN OLD;
END;
$$ LANGUAGE plpgsql;


CREATE TRIGGER syncbulmacontfamiliatriggerd
    AFTER DELETE ON familia
    FOR EACH ROW
    EXECUTE PROCEDURE syncbulmacontfamiliad();
\echo "Creado el trigger que al borrar una familia borra la respectiva cuenta en la contabilidad"



SELECT drop_if_exists_proc ('syncbulmacontfamiliaupre','');
CREATE OR REPLACE FUNCTION syncbulmacontfamiliaupre () RETURNS "trigger"
AS $$
DECLARE
BEGIN

      PERFORM conectabulmacont();

      -- Es necesario controlar la transaccion que se hace en la base de datos enlazada porque no actua
      -- dentro de la transaccion de esta funcion y si hay un error no desaria los cambios provocando problemas
      -- de integridad de los datos.
      -- Hay que hacer un ROLLBACK WORK explicito ANTES de cada RAISE EXCEPTION.
      PERFORM dblink_exec('bulmafact2cont', 'BEGIN WORK;');

      RETURN NULL;

END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER syncbulmacontfamiliatriggerupre
    BEFORE UPDATE OR INSERT ON familia
    FOR EACH STATEMENT
    EXECUTE PROCEDURE syncbulmacontfamiliaupre();






--si se crea una familia hay que mirar si es de servicios o producto para saber bajo que cuenta padre hay que crearla.
--si se actualiza y se ha cambiado de servicio a producto o al revés habrá que hacer un delete de la cuenta
-- padre antigua y un insert en la cuenta padre nueva.
--
-- Las cuentas de familias tienen que gestionar las cuentas de VENTA y de COMPRA.
--
--hay que cambiar como las facturas se contabilizan para separar los servicios y los productos en su cuenta adecuada.

SELECT drop_if_exists_proc ('syncbulmacontfamiliau','');
CREATE OR REPLACE FUNCTION syncbulmacontfamiliau () RETURNS "trigger"
AS $$
DECLARE
	cs RECORD;
	bs RECORD;
	codcta INTEGER;
	quer TEXT;
	idpadre INTEGER;
	descripcion TEXT;
	tipocuenta1 INTEGER;
	query TEXT;
	check_dblink_connection BOOLEAN;
	altacuentaventa BOOLEAN;
	altacuentacompra BOOLEAN;
BEGIN
	-- Cogemos el nombre de la cuenta.
	descripcion := quote_literal(NEW.nombrefamilia);

	altacuentaventa := FALSE;
	altacuentacompra := FALSE;

	-- Entra si es un INSERT o cuando una de las cuentas 'idcuentaventafamilia' o 'idcuentacomprafamilia' apunta a la contabilidad
	-- pero en realidad no existe. Asi ejecutamos su creacion.
	SELECT INTO cs idcuenta FROM bc_cuenta WHERE idcuenta = NEW.idcuentaventafamilia;
	IF NOT FOUND THEN
	    altacuentaventa := TRUE;
	END IF;

	SELECT INTO cs idcuenta FROM bc_cuenta WHERE idcuenta = NEW.idcuentacomprafamilia;
	IF NOT FOUND THEN
	    altacuentacompra := TRUE;
	END IF;


	IF (TG_OP = 'INSERT') OR (altacuentaventa IS TRUE) OR (altacuentacompra IS TRUE) THEN

		-- Discrimina entre producto o servicio para hacer un INSERT.
		IF NEW.productofisicofamilia IS TRUE THEN
		    -- Producto VENTA
			IF (TG_OP = 'INSERT') OR (altacuentaventa IS TRUE) THEN
			    -- Buscamos el codigo de cuenta que vaya a corresponderle.
			    SELECT INTO cs MAX(codigo::INTEGER) AS cod FROM bc_cuenta WHERE codigo LIKE '7000%';
			    IF cs.cod IS NOT NULL THEN
				    codcta := cs.cod + 1;
			    ELSE
				    codcta := '7000001';
			    END IF;

			    -- Buscamos la cuenta padre (la 700)
			    SELECT INTO bs idcuenta, tipocuenta FROM bc_cuenta WHERE codigo ='700';
			    idpadre := bs.idcuenta;
			    tipocuenta1 := bs.tipocuenta;

			    -- Creamos el Query de insercion
			    quer := 'INSERT INTO cuenta (descripcion, padre, codigo, tipocuenta) VALUES ( ' || descripcion ||', ' || idpadre || ', ''' || codcta || ''' , '|| tipocuenta1 || ')';
			    PERFORM dblink_exec('bulmafact2cont', quer);

			    SELECT INTO bs MAX(idcuenta) AS id FROM bc_cuenta;
			    NEW.idcuentaventafamilia := bs.id;
			 END IF;

		    -- Producto COMPRA
			IF (TG_OP = 'INSERT') OR (altacuentacompra IS TRUE) THEN
			    -- Buscamos el codigo de cuenta que vaya a corresponderle.
			    SELECT INTO cs MAX(codigo::INTEGER) AS cod FROM bc_cuenta WHERE codigo LIKE '6000%';
			    IF cs.cod IS NOT NULL THEN
				    codcta := cs.cod + 1;
			    ELSE
				    codcta := '6000001';
			    END IF;

			    -- Buscamos la cuenta padre (la 600)
			    SELECT INTO bs idcuenta, tipocuenta FROM bc_cuenta WHERE codigo ='600';
			    idpadre := bs.idcuenta;
			    tipocuenta1 := bs.tipocuenta;

			    -- Creamos el Query de insercion
			    quer := 'INSERT INTO cuenta (descripcion, padre, codigo, tipocuenta) VALUES ( ' || descripcion ||', ' || idpadre || ', ''' || codcta || ''' , '|| tipocuenta1 || ')';
			    PERFORM dblink_exec('bulmafact2cont', quer);

			    SELECT INTO bs MAX(idcuenta) AS id FROM bc_cuenta;
			    NEW.idcuentacomprafamilia := bs.id;
			END IF;

		ELSE
		    -- Servicio VENTA
			IF (TG_OP = 'INSERT') OR (altacuentaventa IS TRUE) THEN
			    -- Buscamos el codigo de cuenta que vaya a corresponderle.
			    SELECT INTO cs MAX(codigo::INTEGER) AS cod FROM bc_cuenta WHERE codigo LIKE '7050%';
			    IF cs.cod IS NOT NULL THEN
				    codcta := cs.cod + 1;
			    ELSE
				    codcta := '7050001';
			    END IF;

			    -- Buscamos la cuenta padre (la 705)
			    SELECT INTO bs idcuenta, tipocuenta FROM bc_cuenta WHERE codigo ='705';
			    idpadre := bs.idcuenta;
			    tipocuenta1 := bs.tipocuenta;

			    -- Creamos el Query de insercion
			    quer := 'INSERT INTO cuenta (descripcion, padre, codigo, tipocuenta) VALUES ( ' || descripcion ||', ' || idpadre || ', ''' || codcta || ''' , '|| tipocuenta1 || ')';
			    PERFORM dblink_exec('bulmafact2cont', quer);

			    SELECT INTO bs MAX(idcuenta) AS id FROM bc_cuenta;
			    NEW.idcuentaventafamilia := bs.id;
			 END IF;

		    -- Servicio COMPRA. No se crea porque cada caso es diferente.
			IF (TG_OP = 'INSERT') OR (altacuentacompra IS TRUE) THEN
			    -- Buscamos el codigo de cuenta que vaya a corresponderle.
			    SELECT INTO cs MAX(codigo::INTEGER) AS cod FROM bc_cuenta WHERE codigo LIKE '6070%';
			    IF cs.cod IS NOT NULL THEN
				    codcta := cs.cod + 1;
			    ELSE
				    codcta := '6070001';
			    END IF;

			    -- Buscamos la cuenta padre (la 607)
			    SELECT INTO bs idcuenta, tipocuenta FROM bc_cuenta WHERE codigo ='607';
			    idpadre := bs.idcuenta;
			    tipocuenta1 := bs.tipocuenta;

			    -- Creamos el Query de insercion
			    quer := 'INSERT INTO cuenta (descripcion, padre, codigo, tipocuenta) VALUES ( ' || descripcion ||', ' || idpadre || ', ''' || codcta || ''' , '|| tipocuenta1 || ')';
			    PERFORM dblink_exec('bulmafact2cont', quer);

			    SELECT INTO bs MAX(idcuenta) AS id FROM bc_cuenta;
			    NEW.idcuentacomprafamilia := bs.id;
			END IF;


		END IF;

	END IF;

	IF (TG_OP = 'UPDATE') THEN

		-- Al hacer el UPDATE hay que ver que no se haya cambiado de PRODUCTO a SERVICIO o viceversa
		-- y actuar en consecuencia.
		IF NEW.productofisicofamilia = OLD.productofisicofamilia THEN

		    -- No se ha cambiado. Solo actualiza datos.
		    quer := 'UPDATE cuenta SET descripcion = ' || descripcion || ' WHERE idcuenta = ' || NEW.idcuentaventafamilia;
		    PERFORM dblink_exec('bulmafact2cont', quer);

		    quer := 'UPDATE cuenta SET descripcion = ' || descripcion || ' WHERE idcuenta = ' || NEW.idcuentacomprafamilia;
		    PERFORM dblink_exec('bulmafact2cont', quer);

		ELSE

		    -- Se ha cambiado, por tanto hay que borrar la cuenta de la rama actual y crearlo
		    -- en la nueva rama. Si la familia tiene datos entonces fallara el borrado impidiendo el cambio de tipo de cuenta.
		    query := 'DELETE FROM cuenta WHERE idcuenta = ' || OLD.idcuentaventafamilia;
		    PERFORM dblink_exec('bulmafact2cont', query);

		    query := 'DELETE FROM cuenta WHERE idcuenta = ' || OLD.idcuentacomprafamilia;
		    PERFORM dblink_exec('bulmafact2cont', query);

		    -- Discrimina entre producto o servicio para hacer un INSERT.
		    IF NEW.productofisicofamilia IS TRUE THEN
			-- Producto VENTA
			    -- Buscamos el codigo de cuenta que vaya a corresponderle.
			    SELECT INTO cs MAX(codigo::INTEGER) AS cod FROM bc_cuenta WHERE codigo LIKE '7000%';
			    IF cs.cod IS NOT NULL THEN
				    codcta := cs.cod + 1;
			    ELSE
				    codcta := '7000001';
			    END IF;

			    -- Buscamos la cuenta padre (la 700)
			    SELECT INTO bs idcuenta, tipocuenta FROM bc_cuenta WHERE codigo ='700';
			    idpadre := bs.idcuenta;
			    tipocuenta1 := bs.tipocuenta;

			    -- Creamos el Query de insercion
			    quer := 'INSERT INTO cuenta (descripcion, padre, codigo, tipocuenta) VALUES ( ' || descripcion ||', ' || idpadre || ', ''' || codcta || ''' , '|| tipocuenta1 || ')';
			    PERFORM dblink_exec('bulmafact2cont', quer);

			    SELECT INTO bs MAX(idcuenta) AS id FROM bc_cuenta;
			    NEW.idcuentaventafamilia := bs.id;

			-- Producto COMPRA
			    -- Buscamos el codigo de cuenta que vaya a corresponderle.
			    SELECT INTO cs MAX(codigo::INTEGER) AS cod FROM bc_cuenta WHERE codigo LIKE '6000%';
			    IF cs.cod IS NOT NULL THEN
				    codcta := cs.cod + 1;
			    ELSE
				    codcta := '6000001';
			    END IF;

			    -- Buscamos la cuenta padre (la 600)
			    SELECT INTO bs idcuenta, tipocuenta FROM bc_cuenta WHERE codigo ='600';
			    idpadre := bs.idcuenta;
			    tipocuenta1 := bs.tipocuenta;

			    -- Creamos el Query de insercion
			    quer := 'INSERT INTO cuenta (descripcion, padre, codigo, tipocuenta) VALUES ( ' || descripcion ||', ' || idpadre || ', ''' || codcta || ''' , '|| tipocuenta1 || ')';
			    PERFORM dblink_exec('bulmafact2cont', quer);

			    SELECT INTO bs MAX(idcuenta) AS id FROM bc_cuenta;
			    NEW.idcuentacomprafamilia := bs.id;

		    ELSE
			-- Servicio VENTA
			    -- Buscamos el codigo de cuenta que vaya a corresponderle.
			    SELECT INTO cs MAX(codigo::INTEGER) AS cod FROM bc_cuenta WHERE codigo LIKE '7050%';
			    IF cs.cod IS NOT NULL THEN
				    codcta := cs.cod + 1;
			    ELSE
				    codcta := '7050001';
			    END IF;

			    -- Buscamos la cuenta padre (la 705)
			    SELECT INTO bs idcuenta, tipocuenta FROM bc_cuenta WHERE codigo = '705';
			    idpadre := bs.idcuenta;
			    tipocuenta1 := bs.tipocuenta;

			    -- Creamos el Query de insercion
			    quer := 'INSERT INTO cuenta (descripcion, padre, codigo, tipocuenta) VALUES ( ' || descripcion ||', ' || idpadre || ', ''' || codcta || ''' , '|| tipocuenta1 || ')';
			    PERFORM dblink_exec('bulmafact2cont', quer);

			    SELECT INTO bs MAX(idcuenta) AS id FROM bc_cuenta;
			    NEW.idcuentaventafamilia := bs.id;

			-- Servicio COMPRA. No se crea porque cada caso es diferente.
			    -- Buscamos el codigo de cuenta que vaya a corresponderle.
			    SELECT INTO cs MAX(codigo::INTEGER) AS cod FROM bc_cuenta WHERE codigo LIKE '6070%';
			    IF cs.cod IS NOT NULL THEN
				    codcta := cs.cod + 1;
			    ELSE
				    codcta := '6070001';
			    END IF;

			    -- Buscamos la cuenta padre (la 607)
			    SELECT INTO bs idcuenta, tipocuenta FROM bc_cuenta WHERE codigo ='607';
			    idpadre := bs.idcuenta;
			    tipocuenta1 := bs.tipocuenta;

			    -- Creamos el Query de insercion
			    quer := 'INSERT INTO cuenta (descripcion, padre, codigo, tipocuenta) VALUES ( ' || descripcion ||', ' || idpadre || ', ''' || codcta || ''' , '|| tipocuenta1 || ')';
			    PERFORM dblink_exec('bulmafact2cont', quer);

			    SELECT INTO bs MAX(idcuenta) AS id FROM bc_cuenta;
			    NEW.idcuentacomprafamilia := bs.id;

		    END IF;

		END IF;

	END IF;

	RETURN NEW;
END;
$$ LANGUAGE plpgsql;


CREATE TRIGGER syncbulmacontfamiliatriggeru
    BEFORE INSERT OR UPDATE ON familia
    FOR EACH ROW
    EXECUTE PROCEDURE syncbulmacontfamiliau();

\echo "Creado el trigger que al modificar o insertar una familia en la facturacion mete el correspondiente asiento en la contabilidad"


SELECT drop_if_exists_proc ('syncbulmacontfamiliaup','');
CREATE OR REPLACE FUNCTION syncbulmacontfamiliaup () RETURNS "trigger"
AS $$
DECLARE
  check_dblink_connection BOOLEAN;

BEGIN
	check_dblink_connection = dblink_exists('bulmafact2cont');
	
	IF check_dblink_connection IS TRUE THEN
	    -- Hace un COMMIT de la conexion dblink si todo ha ido bien.
	    PERFORM dblink_exec('bulmafact2cont', 'COMMIT WORK;');
	    PERFORM dblink_disconnect('bulmafact2cont');
	ELSE
	    RAISE EXCEPTION 'Se ha perdido la conexion dblink en syncbulmacontfamiliaup.';
	END IF;

      RETURN NULL;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER syncbulmacontfamiliatriggerup
    AFTER UPDATE OR INSERT ON familia
    FOR EACH STATEMENT
    EXECUTE PROCEDURE syncbulmacontfamiliaup();




-- ==============================================================================

-- Agregamos nuevos parametros de configuracion.
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	rs RECORD;

BEGIN
	SELECT INTO rs * FROM configuracion WHERE nombre=''PluginBf_SincroBulmaCont'';

	IF FOUND THEN
		UPDATE CONFIGURACION SET valor=''0.9.1-003'' WHERE nombre=''PluginBf_SincroBulmaCont'';
	ELSE
		INSERT INTO configuracion (nombre, valor) VALUES (''PluginBf_SincroBulmaCont'', ''0.9.1-003'');
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
