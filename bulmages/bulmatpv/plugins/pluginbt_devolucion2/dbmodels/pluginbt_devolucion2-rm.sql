--
-- Modificación de campos y funciones de la BD para la adaptacion para el plugin de Inventario
--
\echo "********* INICIADO FICHERO DE ESTRUCTURA DEL PLUGIN DE TIPOS DE DEVOLUCION *********"

\echo ":: Establecemos los mensajes minimos a avisos y otros parametros ... "
\echo -n ":: "
SET client_min_messages TO WARNING;
SET log_min_messages TO WARNING;
SET log_error_verbosity TO TERSE;

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



-- ========================== CONTROL DE STOCK =======================


CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
	rs RECORD;
BEGIN

	SELECT INTO rs * FROM pg_tables  WHERE tablename=''devolucion'';
	IF FOUND THEN
      DROP TABLE vale;
      DROP TABLE devolucion;
	END IF;

	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo "Elimino las tablas de devolucion y vale"


CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS
$BODY$
DECLARE
	asf RECORD;
	txt TEXT;
BEGIN
	IF (select count(*) from pg_proc where proname='crearef') THEN
	  SELECT INTO asf REGEXP_REPLACE(prosrc,'-- MODIFICACION PLUGINDEVOLUCION2.*-- END MODIFICACION PLUGINDEVOLUCION2','','g') AS prosrc FROM pg_proc WHERE proname='crearef';
	  txt := E'CREATE OR REPLACE FUNCTION crearef() RETURNS character varying(15) AS $BB$ ' || asf.prosrc || E' $BB$ LANGUAGE \'plpgsql\' ;';
	  RAISE NOTICE '%', txt;
	  EXECUTE txt;
	END IF;
	RETURN 0;
END;
$BODY$
LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo "Parcheo la funcion crearef para que contemple la nueva referencia."


-- Function: restriccionesvale()

-- DROP FUNCTION restriccionesvale();
SELECT drop_if_exists_proc('restriccionesvale', '');

-- Agregamos nuevos parametros de configuraci�.
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	rs RECORD;

BEGIN
	SELECT INTO rs * FROM configuracion WHERE nombre=''PluginBt_Devolucion2'';

	IF FOUND THEN
		DELETE FROM CONFIGURACION WHERE nombre=''PluginBt_Devolucion2'';
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
