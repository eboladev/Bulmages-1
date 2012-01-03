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

	IF FOUND THEN
	    DROP TABLE lcontrato;
	    DROP TABLE contrato;
	END IF;

	RETURN 0;
END;
' LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo ':: Eliminamos los campos y tablas para el plugin PluginContratos ... '


CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS
$BODY$
DECLARE
	rs RECORD;
	txt TEXT;
BEGIN
    IF (select count(*) from pg_proc where proname = 'crearef') THEN
	SELECT INTO rs REGEXP_REPLACE(prosrc,'-- MODIFICACION PLUGINCONTRATOS.*-- END MODIFICACION PLUGINCONTRATOS','','g') AS prosrc FROM pg_proc WHERE proname='crearef';
	txt := E'CREATE OR REPLACE FUNCTION crearef() RETURNS character varying(15) AS $BB$ ' || rs.prosrc || E' $BB$ LANGUAGE \'plpgsql\' ;';
	RAISE NOTICE '%', txt;
	EXECUTE txt;
    END IF;
    RETURN 0;
END;
$BODY$
LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo "Quito el parche de la funcion crearef para que no contemple las nueva referencia."


-- Function: restriccionesvale()

-- DROP FUNCTION restriccionesvale();
SELECT drop_if_exists_proc('restriccionescontrato', '');
-- ==============================================================================


-- Agregamos nuevos parametros de configuracion.
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	rs RECORD;
BEGIN
	SELECT INTO rs * FROM configuracion WHERE nombre=''PluginBf_Contrato'';

	IF FOUND THEN
		DELETE FROM CONFIGURACION WHERE nombre=''PluginBf_Contrato'';
	END IF;

	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT actualizarevision();
DROP FUNCTION actualizarevision() CASCADE;
\echo "Eliminada la revision de la base de datos"


DROP FUNCTION drop_if_exists_table(text) CASCADE;
DROP FUNCTION drop_if_exists_proc(text,text) CASCADE;


COMMIT;
