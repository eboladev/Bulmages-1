--
-- Modificación de campos y funciones de la BD para el uso de BULMATPV
--
\echo "********* INICIADO FICHERO DE ESTRUCTURA DEL PLUGIN DE BULMATPV *********"
\echo ":: Establecemos los mensajes minimos a avisos y otros parametros ... "
\echo -n ":: "
SET client_min_messages TO WARNING;
SET log_min_messages TO WARNING;
-- SET log_error_verbosity TO TERSE;
BEGIN;

--
-- Estas primeras funciones cambiaran los tipos de columnas que estan como flotantes a NUMERIC.
-- Se trata de un parche que se desea aplicar para almacenar los tipos monetarios
-- ya que actualmente se encuantran almacenados como 'doubles' y es preferible
-- que se almacenen como tipo 'numeric'.
-- Todas devuelven como valor numerico el numero de filas influenciadas por el cambio
-- NOTA: Si alguien sabe como pasar por parametro un nombre de tabla y campo a modificar se
-- haran mucho mas sencillito porque solo habra que implementar un funcion ya que siempre
-- hay que hacer lo mismo.
--

--
-- Funcion auxiliar para borrar funciones limpiamente.
--
CREATE OR REPLACE FUNCTION drop_if_exists_table(text) RETURNS INTEGER AS '
DECLARE
    tbl_name ALIAS FOR $1;

BEGIN
    IF (select count(*) from pg_tables where tablename = $1) THEN
	EXECUTE ''DROP TABLE '' || $1;
	RETURN 1;
    END IF;
    RETURN 0;
END;
' language 'plpgsql';


CREATE OR REPLACE FUNCTION drop_if_exists_proc(text, text) RETURNS INTEGER AS '
DECLARE
    proc_name ALIAS FOR $1;
    proc_params ALIAS FOR $2;

BEGIN
    IF (select count(*) from pg_proc where proname = $1) THEN
	EXECUTE ''DROP FUNCTION '' || $1 || ''(''||$2||'') CASCADE'';
	RETURN 1;
    END IF;
    RETURN 0;
END;
' language 'plpgsql';


CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
	rs RECORD;
BEGIN
	SELECT INTO rs * FROM pg_attribute WHERE attname = ''horaalbaran'';
	IF NOT FOUND THEN
	    ALTER TABLE albaran ADD COLUMN horaalbaran TIME DEFAULT now();
	    ALTER TABLE albaran ADD COLUMN anuladoalbaran boolean;
	    ALTER TABLE albaran ADD COLUMN recogidoalbaran boolean;
	    CREATE TABLE z (
		idz SERIAL PRIMARY KEY,
		fechaz DATE DEFAULT now(),
		horaz  TIME DEFAULT now(),
		totalz NUMERIC(12,2),
		idalmacen integer NOT NULL REFERENCES almacen(idalmacen),
		numtickets integer
	    );
	END IF;

	SELECT INTO rs * FROM pg_attribute WHERE attname = ''ticketalbaran'';
	IF NOT FOUND THEN
	    ALTER TABLE albaran ADD COLUMN ticketalbaran boolean;
	END IF;

	SELECT INTO rs * FROM pg_attribute WHERE attname = ''ticketfactura'';
	IF NOT FOUND THEN
	    ALTER TABLE factura ADD COLUMN ticketfactura boolean;
	END IF;

	SELECT INTO rs attname, relname FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid=pg_class.oid WHERE attname=''idz'' AND relname=''albaran'';
	IF NOT FOUND THEN
	    ALTER TABLE albaran ADD COLUMN idz INTEGER;
	    ALTER TABLE albaran ADD CONSTRAINT idzfk FOREIGN KEY (idz) REFERENCES z(idz);
	END IF;

	SELECT INTO rs attname, relname FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid=pg_class.oid WHERE attname=''cambiosalbaran'' AND relname=''albaran'';
	IF NOT FOUND THEN
	    ALTER TABLE albaran ADD COLUMN cambiosalbaran VARCHAR;
	    ALTER TABLE albaran ADD COLUMN cambiospostalbaran VARCHAR;
	END IF;

	RETURN 0;
END;
' LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo ':: Agregamos los campos y tablas para el BulmaTPV  ... '



-- ==============================================================================


-- Agregamos nuevos parametros de configuracion.
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	rs RECORD;
BEGIN
	SELECT INTO rs * FROM configuracion WHERE nombre=''DBRev-BulmaTPV'';
	IF FOUND THEN
		UPDATE CONFIGURACION SET valor=''0.14.1-0002'' WHERE nombre=''DBRev-BulmaTPV'';
	ELSE
		INSERT INTO configuracion (nombre, valor) VALUES (''DBRev-BulmaTPV'', ''0.14.1-0002'');
	END IF;
	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT actualizarevision();
DROP FUNCTION actualizarevision() CASCADE;
\echo "Actualizada la revision de la base de datos a la version 0.13.1"


DROP FUNCTION drop_if_exists_table(text) CASCADE;
DROP FUNCTION drop_if_exists_proc(text,text) CASCADE;


COMMIT;

