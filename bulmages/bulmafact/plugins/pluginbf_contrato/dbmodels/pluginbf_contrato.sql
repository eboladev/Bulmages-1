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
	SELECT INTO rs * FROM pg_attribute WHERE attname = ''refcontrato'';

	IF NOT FOUND THEN
		CREATE TABLE contrato (
			idcontrato SERIAL PRIMARY KEY,
			idcliente INTEGER NOT NULL REFERENCES cliente(idcliente),
			refcontrato VARCHAR,
			nomcontrato VARCHAR NOT NULL,
			fincontrato DATE,
			ffincontrato DATE,
			periodicidadcontrato INTERVAL,
			descontrato VARCHAR,
			loccontrato VARCHAR
		);
	END IF;

	SELECT INTO rs * FROM pg_attribute WHERE attname = ''idlcontrato'';

	IF NOT FOUND THEN
		CREATE TABLE lcontrato (
			idlcontrato SERIAL PRIMARY KEY,
			idcontrato INTEGER NOT NULL REFERENCES contrato(idcontrato),
			idarticulo INTEGER NOT NULL REFERENCES articulo(idarticulo),
			cantlcontrato NUMERIC(12,2) DEFAULT 1,
			pvplcontrato NUMERIC(12,2) DEFAULT 0,
			desclcontrato CHARACTER VARYING,
			ordenlcontrato INTEGER
		);
	END IF;

	RETURN 0;
END;
' LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo ':: Agregamos los campos y tablas para el plugin PluginContratos ... '


CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS
$BODY$
DECLARE
	rs RECORD;
	txt TEXT;
BEGIN
	SELECT INTO rs REGEXP_REPLACE(prosrc,'-- MODIFICACION PLUGINCONTRATOS.*-- END MODIFICACION PLUGINCONTRATOS','','g') AS prosrc FROM pg_proc WHERE proname='crearef';
	txt := E'CREATE OR REPLACE FUNCTION crearef() RETURNS character varying(15) AS $BB$ ' || rs.prosrc || E' $BB$ LANGUAGE \'plpgsql\' ;';
	txt := REPLACE(txt, '-- PLUGINS', E'-- MODIFICACION PLUGINCONTRATOS\n 	SELECT INTO asd idcontrato FROM contrato WHERE refcontrato = result;\n	IF FOUND THEN\n		efound := FALSE;\n	END IF;\n-- END MODIFICACION PLUGINCONTRATOS\n-- PLUGINS\n');
	RAISE NOTICE '%', txt;
	EXECUTE txt;
	RETURN 0;
END;
$BODY$
LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo "Parcheo la funcion crearef para que contemple la nueva referencia."


-- Function: restriccionesvale()

-- DROP FUNCTION restriccionesvale();
SELECT drop_if_exists_proc('restriccionescontrato', '');
\echo -n ':: Funcion que crea restricciones en los contratos ... '
CREATE OR REPLACE FUNCTION restriccionescontrato()
RETURNS "trigger" AS
$BODY$
DECLARE
rs RECORD;

BEGIN
IF NEW.refcontrato IS NULL OR NEW.refcontrato = '' THEN
	SELECT INTO rs crearef() AS m;
	IF FOUND THEN
	NEW.refcontrato := rs.m;
	END IF;
END IF;
RETURN NEW;
END;$BODY$
LANGUAGE 'plpgsql' VOLATILE;

CREATE TRIGGER restriccionescontratotrigger
BEFORE INSERT OR UPDATE
ON contrato
FOR EACH ROW
EXECUTE PROCEDURE restriccionescontrato();

-- ==============================================================================


-- Agregamos nuevos parametros de configuracion.
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	rs RECORD;
BEGIN
	SELECT INTO rs * FROM configuracion WHERE nombre=''PluginBf_Contrato'';

	IF FOUND THEN
		UPDATE CONFIGURACION SET valor=''0.11.1-0001'' WHERE nombre=''PluginBf_Contrato'';
	ELSE
		INSERT INTO configuracion (nombre, valor) VALUES (''PluginBf_Contrato'', ''0.11.1-0001'');
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

