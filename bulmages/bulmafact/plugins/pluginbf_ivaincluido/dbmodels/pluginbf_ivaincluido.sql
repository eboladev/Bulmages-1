--
-- ModificaciÃ³n de campos y funciones de la BD para la adaptacion para el plugin de IVA Incluido
--
\echo "********* INICIADO FICHERO DE ESTRUCTURA DEL PLUGIN DE IVA INCLUIDO *********"

\echo ":: Establecemos los mensajes minimos a avisos y otros parametros ... "
\echo -n ":: "
SET client_min_messages TO WARNING;
SET log_min_messages TO WARNING;
-- SET log_error_verbosity TO TERSE;
BEGIN;

--
-- Estas primeras funciones cambiarï¿½ los tipos de columnas que estï¿½ como flotantes a NUMERIC.
-- Se trata de un parche que se desea aplicar para almacenar los tipos monetarios
-- ya que actualmente se encuantran almacenados como 'doubles' y es preferible
-- que se almacenen como tipo 'numeric'.
-- Todas devuelven como valor numï¿½ico el nmero de filas influenciadas por el cambio
-- NOTA: Si alguien sabe como pasar por parï¿½etro un nombre de tabla y campo a modificar se
-- harï¿½ mucho mï¿½ sencillito porque sï¿½o habrï¿½ que implementar un funciï¿½ ya que siempre
-- hay que hacer lo mismo.
--

--
-- FunciÃ³n auxiliar para borrar funciones limpiamente
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


CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
	res RECORD;
BEGIN
	SELECT INTO res * FROM pg_attribute  WHERE attname=''ivaincarticulo'';
	IF NOT FOUND THEN
		ALTER TABLE articulo ADD COLUMN ivaincarticulo BOOLEAN DEFAULT FALSE;
	END IF;

	SELECT INTO res * FROM pg_attribute  WHERE attname=''pvpivainclalbaran'';
	IF NOT FOUND THEN
		ALTER TABLE lalbaran ADD COLUMN pvpivainclalbaran NUMERIC(12,2) DEFAULT 0;
		ALTER TABLE lfactura ADD COLUMN pvpivainclfactura NUMERIC(12,2) DEFAULT 0;
		ALTER TABLE articulo ADD COLUMN pvpivaincarticulo NUMERIC(12,2) DEFAULT 0;
	END IF;

	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo "Agregamos el campo booleano ivaincarticulo a la tabla de articulos"





-- ==============================================================================


-- Agregamos nuevos parametros de configuracion
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	res RECORD;
BEGIN
	SELECT INTO res * FROM configuracion WHERE nombre=''PluginBf_IVAIncluido'';

	IF FOUND THEN
		UPDATE configuracion SET valor=''0.15.1-0000'' WHERE nombre=''PluginBf_IVAIncluido'';
	ELSE
		INSERT INTO configuracion (nombre, valor) VALUES (''PluginBf_IVAIncluido'', ''0.15.1-0000'');
	END IF;

	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT actualizarevision();
DROP FUNCTION actualizarevision() CASCADE;
\echo "Actualizada la revision de la base de datos"


DROP FUNCTION drop_if_exists_table(text) CASCADE;
DROP FUNCTION drop_if_exists_proc(text,text) CASCADE;


COMMIT;
