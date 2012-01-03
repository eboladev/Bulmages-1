--
-- Modificación de campos y alta de funciones en la base de datos de BulmaFact
-- para la sincronización de datos con BulmaCont.
--
\echo "********* INICIADO DESINSTALADOR DE ESTRUCTURA DEL PLUGIN DE SINCRONIZACION CON BULMACONT *********"

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
--	SELECT INTO bs valor FROM configuracion WHERE nombre='DataBaseContabilidad';
--	IF NOT FOUND THEN
--		INSERT INTO configuracion (nombre, valor) VALUES ('DataBaseContabilidad', 'bulmacont');
--	END IF;

	DELETE FROM configuracion WHERE nombre='DataBaseContabilidad';


	SELECT INTO bs viewname FROM pg_views WHERE viewname='bc_cuenta';
	IF FOUND THEN
		DROP VIEW bc_cuenta;
	END IF;
--	CREATE OR REPLACE view BC_Cuenta AS SELECT * FROM dblink( 'SELECT idgrupo, idcuenta, codigo, descripcion, tipocuenta FROM cuenta') AS t1 (idgrupo integer, idcuenta integer, codigo varchar, descripcion varchar, tipocuenta integer);


	SELECT INTO bs viewname FROM pg_views WHERE viewname ='BC_Asiento';
	IF FOUND THEN
		DROP VIEW bc_Asiento;
--	CREATE OR REPLACE VIEW BC_Asiento AS SELECT * FROM dblink( 'SELECT idasiento FROM asiento') AS t1 (idasiento integer);

		DROP VIEW BC_Borrador;
--	CREATE OR REPLACE VIEW BC_Borrador AS SELECT * FROM dblink( 'SELECT idborrador FROM borrador') AS t1 (idborrador integer);

		DROP VIEW BC_Apunte;
--	CREATE OR REPLACE VIEW BC_Apunte AS SELECT * FROM dblink( 'SELECT idapunte FROM apunte') AS t1 (idapunte integer);
        END IF;

	SELECT INTO bs * FROM pg_attribute WHERE attname='idasientofactura';
	IF FOUND THEN
--		ALTER TABLE factura ADD COLUMN idasientofactura INTEGER;
		ALTER TABLE factura DROP COLUMN idasientofactura;
	END IF;

	SELECT INTO bs * FROM pg_attribute WHERE attname='idcuentacliente';
	IF FOUND THEN
--		ALTER TABLE cliente ADD COLUMN idcuentacliente INTEGER;
		ALTER TABLE cliente DROP COLUMN idcuentacliente;
	END IF;


	SELECT INTO bs * FROM pg_attribute WHERE attname='idasientofacturap';
	IF FOUND THEN
--		ALTER TABLE facturap ADD COLUMN idasientofacturap INTEGER;
		ALTER TABLE facturap DROP COLUMN idasientofacturap;
	END IF;

	SELECT INTO bs * FROM pg_attribute WHERE attname='idcuentaproveedor';
	IF FOUND THEN
--		ALTER TABLE proveedor ADD COLUMN idcuentaproveedor INTEGER;
		ALTER TABLE proveedor DROP COLUMN idcuentaproveedor;
	END IF;

	-- Creamos el espacio para almacenar el identificador de cuenta que corresponde al banco.
	SELECT INTO bs * FROM pg_attribute WHERE attname='idcuentabanco';
	IF FOUND THEN
--		ALTER TABLE banco ADD COLUMN idcuentabanco INTEGER;
		ALTER TABLE banco DROP COLUMN idcuentabanco;
	END IF;

	-- Creamos el espacio para almacenar el identificador de cuenta que corresponde al almacen
	SELECT INTO bs * FROM pg_attribute WHERE attname='idcuentaalmacen';
	IF FOUND THEN
--		ALTER TABLE almacen ADD COLUMN idcuentaalmacen INTEGER;
		ALTER TABLE almacen DROP COLUMN idcuentaalmacen;
	END IF;

	-- Creamos el espacio para almacenar el identificador de cuenta que corresponde al cobro
	SELECT INTO bs * FROM pg_attribute WHERE attname='idasientocobro';
	IF FOUND THEN
--		ALTER TABLE cobro ADD COLUMN idasientocobro INTEGER;
		ALTER TABLE cobro DROP COLUMN idasientocobro;
	END IF;
	-- Creamos el espacio para almacenar el identificador de cuenta que corresponde al pago
	SELECT INTO bs * FROM pg_attribute WHERE attname='idasientopago';
	IF FOUND THEN
--		ALTER TABLE pago ADD COLUMN idasientopago INTEGER;
		ALTER TABLE pago DROP COLUMN idasientopago;
	END IF;

	SELECT INTO bs * FROM pg_attribute WHERE attname='prefcuentaventafamilia';
	IF FOUND THEN
--		ALTER TABLE familia ADD COLUMN prefcuentaventafamilia CHARACTER VARYING(12);
--		ALTER TABLE familia ADD COLUMN origenidcuentaventafamilia INTEGER;
--		ALTER TABLE familia ADD COLUMN idcuentaventafamilia INTEGER;
		ALTER TABLE familia DROP COLUMN idcuentaventafamilia;
		ALTER TABLE familia DROP COLUMN prefcuentaventafamilia;
		ALTER TABLE familia DROP COLUMN origenidcuentaventafamilia;
	END IF;

	SELECT INTO bs * FROM pg_attribute WHERE attname='prefcuentacomprafamilia';
	IF FOUND THEN
--		ALTER TABLE familia ADD COLUMN prefcuentacomprafamilia CHARACTER VARYING(12);
--		ALTER TABLE familia ADD COLUMN origenidcuentacomprafamilia INTEGER;
--		ALTER TABLE familia ADD COLUMN idcuentacomprafamilia INTEGER;
		ALTER TABLE familia DROP COLUMN idcuentacomprafamilia;
		ALTER TABLE familia DROP COLUMN prefcuentacomprafamilia;
		ALTER TABLE familia DROP COLUMN origenidcuentacomprafamilia;
	END IF;


	SELECT INTO bs * FROM pg_attribute WHERE attname='prefcuentaforma_pago';
	IF FOUND THEN
--		ALTER TABLE forma_pago ADD COLUMN prefcuentaforma_pago CHARACTER VARYING(12);
--		ALTER TABLE forma_pago ADD COLUMN origenidcuentaforma_pago INTEGER;
--		ALTER TABLE forma_pago ADD COLUMN idcuentaforma_pago INTEGER;
		ALTER TABLE forma_pago DROP COLUMN idcuentaforma_pago;
		ALTER TABLE forma_pago DROP COLUMN prefcuentaforma_pago;
		ALTER TABLE forma_pago DROP COLUMN origenidcuentaforma_pago;
	END IF;


	SELECT INTO bs * FROM pg_attribute WHERE attname='prefcuentabanco';
	IF FOUND THEN
--		ALTER TABLE banco ADD COLUMN prefcuentabanco CHARACTER VARYING(12);
--		ALTER TABLE banco ADD COLUMN origenidcuentabanco INTEGER;
		ALTER TABLE banco DROP COLUMN prefcuentabanco;
		ALTER TABLE banco DROP COLUMN origenidcuentabanco;
	END IF;


	SELECT INTO bs * FROM pg_attribute WHERE attname='prefcuentacliente';
	IF FOUND THEN
--		ALTER TABLE cliente ADD COLUMN prefcuentacliente CHARACTER VARYING(12);
--		ALTER TABLE cliente ADD COLUMN origenidcuentacliente INTEGER;
		ALTER TABLE cliente DROP COLUMN prefcuentacliente;
		ALTER TABLE cliente DROP COLUMN origenidcuentacliente;
	END IF;


	SELECT INTO bs * FROM pg_attribute WHERE attname='prefcuentaproveedor';
	IF FOUND THEN
--		ALTER TABLE proveedor ADD COLUMN prefcuentaproveedor CHARACTER VARYING(12);
--		ALTER TABLE proveedor ADD COLUMN origenidcuentaproveedor INTEGER;
		ALTER TABLE proveedor DROP COLUMN prefcuentaproveedor;
		ALTER TABLE proveedor DROP COLUMN origenidcuentaproveedor;
	END IF;


	RETURN 0;
END;
$$   LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo "Quitamos los campos para almacenar claves foraneas en las tablas necesarias."


SELECT drop_if_exists_proc('conectabulmacont', '');
SELECT drop_if_exists_proc ('syncbulmacontpagod','');
\echo "Borrado el trigger que al borrar una factura borra su respectivo asiento en la contabilidad"
SELECT drop_if_exists_proc ('syncbulmacontpagou','');
\echo "Borrado el trigger que al modificar o insertar una factura en la facturacion mete el correspondiente asiento en la contabilidad"
SELECT drop_if_exists_proc ('syncbulmacontcobrod','');
\echo "Borrado el trigger que al borrar un cobro borra su respectivo asiento en la contabilidad"
SELECT drop_if_exists_proc ('syncbulmacontcobrou','');
\echo "Borradoo el trigger que al modificar o insertar una factura en la facturacion mete el correspondiente asiento en la contabilidad"
SELECT drop_if_exists_proc ('syncbulmacontfacturad','');
\echo "Borrado el trigger que al borrar una factura borra su respectivo asiento en la contabilidad"
SELECT drop_if_exists_proc ('syncbulmacontfacturau','');
\echo "Borrado el trigger que al modificar o insertar una factura en la facturacion mete el correspondiente asiento en la contabilidad"
SELECT drop_if_exists_proc ('syncbulmacontfacturapd','');
\echo "Borrado el Trigger que al borrar una factura de proveedor la borra de la contabilidad"
SELECT drop_if_exists_proc ('syncbulmacontfacturapu','');
\echo "Borrado el trigger que al modificar o insertar una factura  de proveedor en la facturacion mete el correspondiente asiento en la contabilidad"
SELECT drop_if_exists_proc ('syncbulmacontcliented','');
SELECT drop_if_exists_proc ('syncbulmacontclienteu','');
SELECT drop_if_exists_proc ('syncbulmacontproveedord','');
SELECT drop_if_exists_proc ('syncbulmacontproveedoru','');
SELECT drop_if_exists_proc ('syncbulmacontbancod','');
\echo "Borrado el trigger que al borrar un banco borra la respectiva cuenta en la contabilidad"
SELECT drop_if_exists_proc ('syncbulmacontbancou','');
\echo "Borrado el trigger que al modificar o insertar un banco en la facturacion mete el correspondiente asiento en la contabilidad"
SELECT drop_if_exists_proc ('syncbulmacontalmacend','');
\echo "Borrado el trigger que al borrar un banco borra la respectiva cuenta en la contabilidad"
SELECT drop_if_exists_proc ('syncbulmacontalmacenu','');
\echo "Borrado el trigger que al modificar o insertar un almacen en la facturacion mete el correspondiente asiento en la contabilidad"
SELECT drop_if_exists_proc ('syncbulmacontforma_pagod','');
SELECT drop_if_exists_proc ('syncbulmacontforma_pagou','');


SELECT drop_if_exists_proc ('syncbulmacontclienteup','');
SELECT drop_if_exists_proc ('syncbulmacontfamiliaup','');
SELECT drop_if_exists_proc ('syncbulmacontproveedorup','');
SELECT drop_if_exists_proc ('syncbulmacontcobroup','');
SELECT drop_if_exists_proc ('syncbulmacontpagoup','');
SELECT drop_if_exists_proc ('syncbulmacontbancoup','');
SELECT drop_if_exists_proc ('syncbulmacontalmacenup','');
SELECT drop_if_exists_proc ('syncbulmacontforma_pagoup','');


SELECT drop_if_exists_proc ('syncbulmacontclienteupre','');
SELECT drop_if_exists_proc ('syncbulmacontfamiliaupre','');
SELECT drop_if_exists_proc ('syncbulmacontproveedorupre','');





-- Agregamos nuevos parametros de configuracion.
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	rs RECORD;
BEGIN
	DELETE FROM configuracion WHERE nombre=''PluginBf_SincroBulmaCont'';

	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT actualizarevision();
DROP FUNCTION actualizarevision() CASCADE;
\echo "Actualizada la revision de la base de datos"


DROP FUNCTION drop_if_exists_table(text) CASCADE;
DROP FUNCTION drop_if_exists_proc(text,text) CASCADE;


COMMIT;