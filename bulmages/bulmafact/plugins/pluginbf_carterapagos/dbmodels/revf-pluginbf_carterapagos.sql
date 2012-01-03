--
-- Modificación de campos y funciones de la BD para la adaptacion para el plugin de Contratos
--
\echo "********* INICIADO FICHERO DE ESTRUCTURA DEL PLUGIN DE CONTRATOS *********"
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
--	DROP TABLE vencimientop;
--	DROP TABLE vencimientoproveedor;
	
	SELECT INTO rs * FROM pg_attribute WHERE attname = ''idvencimientop'';
	IF NOT FOUND THEN
		CREATE TABLE vencimientop (
			idvencimientop SERIAL PRIMARY KEY,
			idfacturap INTEGER REFERENCES facturap(idfacturap),
			fechavencimientop DATE,
			fechaefvencimientop DATE,
			cantvencimientop NUMERIC(12,2),
			refvencimientop VARCHAR,
			procesadovencimientop BOOLEAN DEFAULT FALSE,
			estadovencimientop VARCHAR NOT NULL DEFAULT ''Pendiente'',
			descvencimientop VARCHAR,
			comentvencimientop VARCHAR,
			idforma_pago INTEGER REFERENCES forma_pago(idforma_pago),
			idproveedor INTEGER NOT NULL REFERENCES proveedor(idproveedor)
		);
	END IF;


	SELECT INTO rs * FROM pg_attribute WHERE attname = ''idvencimientoproveedor'';
	IF NOT FOUND THEN
		CREATE TABLE vencimientoproveedor (
			idvencimientoproveedor SERIAL PRIMARY KEY,
			idproveedor INTEGER NOT NULL REFERENCES proveedor(idproveedor),
			diasvencimientoproveedor INTEGER,
			porcentajevencimientoproveedor NUMERIC(12,2),
			idforma_pago INTEGER REFERENCES forma_pago(idforma_pago)
		);
	END IF;


	RETURN 0;
END;
' LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo ':: Agregamos los campos y tablas para el plugin PluginContratos ... '


-- ==============================================================================


-- Agregamos nuevos parametros de configuracion.
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	rs RECORD;

BEGIN
	SELECT INTO rs * FROM configuracion WHERE nombre=''DBRev-CarteraPagos'';

	IF FOUND THEN
		UPDATE CONFIGURACION SET valor=''0.11.1-0001'' WHERE nombre=''DBRev-CarteraPagos'';
	ELSE
		INSERT INTO configuracion (nombre, valor) VALUES (''DBRev-CarteraPagos'', ''0.11.1-0001'');
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

