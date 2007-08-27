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
    ALTER TABLE	lpresupuesto ALTER COLUMN desclpresupuesto TYPE character varying;
    ALTER TABLE lpedidocliente ALTER COLUMN desclpedidocliente TYPE character varying;
    ALTER TABLE lpedidoproveedor ALTER COLUMN desclpedidoproveedor TYPE character varying;
    ALTER TABLE	lalbaranp ALTER COLUMN desclalbaranp TYPE character varying;
    ALTER TABLE	lalbaran ALTER COLUMN desclalbaran TYPE character varying;
    ALTER TABLE	lfactura ALTER COLUMN desclfactura TYPE character varying;
    ALTER TABLE	lfacturap ALTER COLUMN desclfacturap TYPE character varying;
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
		UPDATE CONFIGURACION SET valor = ''0.9.3-0004'' WHERE nombre = ''DatabaseRevision'';
	ELSE
		INSERT INTO configuracion (nombre, valor) VALUES (''DatabaseRevision'', ''0.9.3-0004'');
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

