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

SELECT drop_if_exists_proc ('tr_borradoasiento','');
CREATE OR REPLACE FUNCTION tr_borradoasiento() RETURNS "trigger"
AS '
BEGIN
	UPDATE PREVCOBRO SET idasiento = NULL WHERE idasiento=OLD.idasiento;
	RETURN OLD;
END;
' LANGUAGE 'plpgsql';

CREATE TRIGGER restriccionesborradoasientotrigger
   BEFORE DELETE ON asiento
   FOR EACH ROW
   EXECUTE PROCEDURE tr_borradoasiento();



--
-- Agregamos el campo fractemitida que indica si es una factura emitida o recibida
--
CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
	as RECORD;
BEGIN
	SELECT INTO as * FROM pg_attribute  WHERE attname=''idctacliente'';
	IF NOT FOUND THEN
		UPDATE prevcobro SET tipoprevcobro = TRUE WHERE tipoprevcobro IS NULL;
		ALTER TABLE prevcobro ALTER COLUMN tipoprevcobro SET NOT NULL;
		ALTER TABLE prevcobro ALTER COLUMN idregistroiva DROP NOT NULL;
		ALTER TABLE prevcobro ADD COLUMN idctacliente integer REFERENCES cuenta(idcuenta);
	END IF;
	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo "Quitmaos la restricción que vincula gestión de cobros y pagos con facturas"



--
-- Agregamos el campo fractemitida que indica si es una factura emitida o recibida
--
CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
	as RECORD;
BEGIN
	UPDATE cuenta SET padre=0 WHERE padre IS NULL;
	UPDATE prevcobro SET tipoprevcobro = TRUE WHERE tipoprevcobro IS NULL;
	ALTER TABLE cuenta ALTER COLUMN padre SET NOT NULL;
	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo "Hacemos como toca el campo padre en las cuentas."


--
-- Alteramos la tabla compmasap para que permita hacer presentacions sólo si el saldo es mayor o menor que cero
--
CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
	as RECORD;
BEGIN
	SELECT INTO as * FROM pg_attribute  WHERE attname=''tipocompmasap'';
	IF NOT FOUND THEN
		ALTER TABLE compmasap ADD COLUMN tipocompmasap integer;
 		ALTER TABLE compmasap ALTER COLUMN tipocompmasap SET DEFAULT 0;
	END IF;
	SELECT INTO as * FROM pg_attribute  WHERE attname=''tipompatrimonial'';
	IF NOT FOUND THEN
		ALTER TABLE mpatrimonial ADD COLUMN tipompatrimonial integer;
 		ALTER TABLE mpatrimonial ALTER COLUMN tipompatrimonial SET DEFAULT 0;
	END IF;
	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo "Agregamos tipocompmasap a compmasap"



--
-- Agregamos el campo fractemitida que indica si es una factura emitida o recibida
--
CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
	as RECORD;
BEGIN
	SELECT INTO as * FROM configuracion WHERE nombre=''Pago'';
	IF NOT FOUND THEN
		INSERT INTO configuracion (idconfiguracion, nombre, valor) VALUES (26, ''Pago'', ''Pago'');  
	END IF;
	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo "Agregada la fila en la configuraci� para Asientos de Pago"




CREATE OR REPLACE FUNCTION saldototalmpatrimonial(integer) RETURNS NUMERIC(12,2)
    AS '
DECLARE
   identmpatrimonial ALIAS FOR $1;
   saldo NUMERIC(12,2);
   saldoi NUMERIC(12,2);
   rsaldo RECORD;
   rsaldo1 RECORD;
   smpatrimonialsum RECORD;
   smpatrimonialrest RECORD;
   mp RECORD;
BEGIN
    saldo := 0;
    FOR rsaldo IN SELECT  (debe-haber) AS total, tipocompmasap FROM cuenta LEFT JOIN compmasap ON cuenta.idcuenta = compmasap.idcuenta WHERE masaperteneciente = identmpatrimonial AND compmasap.idcuenta IS NOT NULL AND signo = TRUE LOOP
       IF (rsaldo.total IS NOT NULL) THEN
		IF (rsaldo.tipocompmasap = 0 OR (rsaldo.tipocompmasap = 1 AND rsaldo.total > 0) OR (rsaldo.tipocompmasap = 2 AND rsaldo.total < 0)) THEN
           		saldo := saldo + rsaldo.total;
		END IF;
       END IF;
    END LOOP;
    FOR rsaldo1 IN SELECT  (debe-haber) AS total, tipocompmasap FROM cuenta LEFT JOIN compmasap ON cuenta.idcuenta = compmasap.idcuenta WHERE masaperteneciente = identmpatrimonial AND compmasap.idcuenta IS NOT NULL AND signo = FALSE LOOP
       IF (rsaldo1.total IS NOT NULL) THEN
		IF (rsaldo1.tipocompmasap = 0 OR (rsaldo1.tipocompmasap = 1 AND rsaldo1.total > 0) OR (rsaldo1.tipocompmasap = 2 AND rsaldo1.total < 0)) THEN
         		 saldo := saldo - rsaldo1.total;
		END IF;
       END IF;
    END LOOP;
    FOR smpatrimonialsum IN SELECT idmpatrimonial, tipocompmasap FROM compmasap WHERE masaperteneciente = identmpatrimonial AND idmpatrimonial IS NOT NULL AND signo = true LOOP
	saldoi := saldototalmpatrimonial (smpatrimonialsum.idmpatrimonial);
		IF (smpatrimonialsum.tipocompmasap = 0 OR (smpatrimonialsum.tipocompmasap = 1 AND saldoi > 0) OR (smpatrimonialsum.tipocompmasap = 2 AND saldoi < 0)) THEN
       			 saldo := saldo + saldoi;
		END IF;

    END LOOP;
    FOR smpatrimonialrest IN SELECT idmpatrimonial, tipocompmasap FROM compmasap WHERE masaperteneciente = identmpatrimonial AND idmpatrimonial IS NOT NULL AND signo = false LOOP

	saldoi := saldototalmpatrimonial (smpatrimonialrest.idmpatrimonial);
		IF (smpatrimonialrest.tipocompmasap = 0 OR (smpatrimonialrest.tipocompmasap = 1 AND saldoi > 0) OR (smpatrimonialrest.tipocompmasap = 2 AND saldoi < 0)) THEN
       			 saldo := saldo - saldoi;
		END IF;
    END LOOP;

    SELECT INTO mp tipompatrimonial FROM mpatrimonial WHERE idmpatrimonial = identmpatrimonial;
    IF FOUND THEN
	IF mp.tipompatrimonial = 1 THEN
		IF saldo > 0 THEN
			RETURN saldo;
		ELSE
			RETURN 0;
		END IF;
	END IF;
	IF mp.tipompatrimonial = 2 THEN
		IF saldo < 0 THEN
			RETURN saldo;
		ELSE
			RETURN 0;
		END IF;
	END IF;
	IF mp.tipompatrimonial = 3 THEN
		IF saldo > 0 THEN
			RETURN saldo;
		ELSE
			RETURN -saldo;
		END IF;
	END IF;
    END IF;
    RETURN saldo;
END;
' LANGUAGE plpgsql;


--
-- TOC entry 93 (OID 1346055)
-- Name: saldompatrimonial(integer, timestamp without time zone, timestamp without time zone); Type: FUNCTION; Schema: public; Owner: postgres
--
CREATE OR REPLACE FUNCTION saldompatrimonial(integer, timestamp without time zone, timestamp without time zone) RETURNS numeric(12,2)
    AS '
DECLARE
    identmpatrimonial ALIAS FOR $1;
    fechain ALIAS FOR $2;
    fechafin ALIAS FOR $3;
    aptsum RECORD;
    aptrest RECORD;
   rsaldo RECORD;
   rsaldo1 RECORD;
    smpatrimonialsum RECORD;
    smpatrimonialrest RECORD;
    saldo NUMERIC(12,2);
    saldoi NUMERIC(12,2);
BEGIN
    saldo := 0;
    FOR rsaldo IN SELECT * FROM cuenta LEFT JOIN compmasap ON cuenta.idcuenta = compmasap.idcuenta WHERE masaperteneciente = identmpatrimonial AND compmasap.idcuenta IS NOT NULL AND signo = TRUE LOOP
	saldoi := saldototal(rsaldo.codigo, fechain, fechafin);
		IF (rsaldo.tipocompmasap = 0 OR (rsaldo.tipocompmasap = 1 AND saldoi > 0) OR (rsaldo.tipocompmasap = 2 AND saldoi < 0)) THEN
           		saldo := saldo + saldoi;
		END IF;
    END LOOP;
    FOR rsaldo1 IN SELECT  * FROM cuenta LEFT JOIN compmasap ON cuenta.idcuenta = compmasap.idcuenta WHERE masaperteneciente = identmpatrimonial AND compmasap.idcuenta IS NOT NULL AND signo = FALSE LOOP
		saldoi := saldototal(rsaldo1.codigo, fechain, fechafin);
		IF (rsaldo1.tipocompmasap = 0 OR (rsaldo1.tipocompmasap = 1 AND saldoi > 0) OR (rsaldo1.tipocompmasap = 2 AND saldoi < 0)) THEN
         		 saldo := saldo - saldoi;
       END IF;
    END LOOP;
    FOR smpatrimonialsum IN SELECT idmpatrimonial, tipocompmasap FROM compmasap WHERE masaperteneciente = identmpatrimonial AND idmpatrimonial IS NOT NULL AND signo = true LOOP
	saldoi := saldompatrimonial (smpatrimonialsum.idmpatrimonial, fechain, fechafin);
		IF (smpatrimonialsum.tipocompmasap = 0 OR (smpatrimonialsum.tipocompmasap = 1 AND saldoi > 0) OR (smpatrimonialsum.tipocompmasap = 2 AND saldoi < 0)) THEN
       			 saldo := saldo + saldoi;
		END IF;

    END LOOP;
    FOR smpatrimonialrest IN SELECT idmpatrimonial, tipocompmasap FROM compmasap WHERE masaperteneciente = identmpatrimonial AND idmpatrimonial IS NOT NULL AND signo = false LOOP

	saldoi := saldompatrimonial (smpatrimonialrest.idmpatrimonial, fechain, fechafin);
		IF (smpatrimonialrest.tipocompmasap = 0 OR (smpatrimonialrest.tipocompmasap = 1 AND saldoi > 0) OR (smpatrimonialrest.tipocompmasap = 2 AND saldoi < 0)) THEN
       			 saldo := saldo - saldoi;
		END IF;

    END LOOP;

    SELECT INTO mp tipompatrimonial FROM mpatrimonial WHERE idmpatrimonial = identmpatrimonial;
    IF FOUND THEN
	IF mp.tipompatrimonial = 1 THEN
		IF saldo > 0 THEN
			RETURN saldo;
		ELSE
			RETURN 0;
		END IF;
	END IF;
	IF mp.tipompatrimonial = 2 THEN
		IF saldo < 0 THEN
			RETURN saldo;
		ELSE
			RETURN 0;
		END IF;
	END IF;
	IF mp.tipompatrimonial = 3 THEN
		IF saldo > 0 THEN
			RETURN saldo;
		ELSE
			RETURN -saldo;
		END IF;
	END IF;
    END IF;


    RETURN saldo;
END;
' LANGUAGE plpgsql;


--
-- Agregamos nuevos parametros de configuraci�.
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	as RECORD;
BEGIN
	SELECT INTO as * FROM configuracion WHERE nombre=''DatabaseRevision'';
	IF FOUND THEN
		UPDATE CONFIGURACION SET valor=''0.5.3'' WHERE nombre=''DatabaseRevision'';
	ELSE
		INSERT INTO configuracion (idconfiguracion, nombre, valor) VALUES (20, ''DatabaseRevision'', ''0.5.3''); 		 
	END IF;
	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT actualizarevision();
DROP FUNCTION actualizarevision() CASCADE;
\echo "Actualizada la revisión de la base de datos"



SELECT drop_if_exists_proc ('restriccionescuenta','');
CREATE FUNCTION restriccionescuenta () RETURNS "trigger"
AS '
DECLARE
   cta RECORD;
BEGIN
	IF NEW.padre <> 0 THEN
		SELECT INTO cta * FROM cuenta WHERE idcuenta= NEW.padre;
		IF NOT FOUND THEN
			RAISE EXCEPTION '' La cuenta padre no existe. '';
		END IF;
	END IF;
        IF NEW.codigo = '''' THEN
                RAISE EXCEPTION '' No se puede dejar el código de cuenta vacio '';
        END IF;
	IF NEW.descripcion = '''' THEN
		RAISE EXCEPTION '' Nombre de cuenta vacio '';
	END IF;
	IF NEW.nombreent_cuenta = '''' THEN
		NEW.nombreent_cuenta = NEW.descripcion;
	END IF;
        RETURN NEW;
END;
' LANGUAGE plpgsql;

CREATE TRIGGER restriccionescuentatrigger
    BEFORE INSERT OR UPDATE ON cuenta
    FOR EACH ROW
    EXECUTE PROCEDURE restriccionescuenta();



DROP FUNCTION drop_if_exists_table(text) CASCADE;
DROP FUNCTION drop_if_exists_proc(text,text) CASCADE;


COMMIT;