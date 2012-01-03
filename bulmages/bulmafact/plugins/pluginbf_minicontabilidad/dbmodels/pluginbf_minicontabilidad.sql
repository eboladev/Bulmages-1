--
-- Modificación de campos y funciones de la BD para la adaptacion para el plugin de Inventario
--
\echo "********* INICIADO FICHERO DE ESTRUCTURA DEL PLUGIN DE MINICONTABILIDAD *********"

\echo ":: Establecemos los mensajes minimos a avisos y otros parametros ... "
\echo -n ":: "
SET client_min_messages TO WARNING;
SET log_min_messages TO WARNING;
-- SET log_error_verbosity TO TERSE;
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



-- ========================== PARTIDAS =======================



CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
	rs RECORD;
BEGIN

	SELECT INTO rs * FROM pg_tables  WHERE tablename=''partida'';

	IF NOT FOUND THEN
		CREATE TABLE partida (
		idpartida SERIAL PRIMARY KEY,
		codigopartida varchar NOT NULL,
		codigocompletopartida varchar NOT NULL,
		nombrepartida varchar NOT NULL,
		descpartida varchar,
		ingresopartida BOOLEAN DEFAULT TRUE NOT NULL,
		padre integer DEFAULT 0
		);

	END IF;

	SELECT INTO rs * FROM pg_tables  WHERE tablename=''acontable'';

	IF NOT FOUND THEN
		CREATE TABLE acontable (
		idacontable SERIAL PRIMARY KEY,
		fechaacontable DATE,
		conceptoacontable varchar NOT NULL,
		debeacontable NUMERIC(12,2),
		haberacontable NUMERIC(12,2),
		idpartida integer REFERENCES partida(idpartida)
		);
	END IF;


	SELECT INTO rs * FROM pg_tables  WHERE tablename=''presupuestocontable'';

	IF NOT FOUND THEN
		CREATE TABLE presupuestocontable (
		idpresupuestocontable SERIAL PRIMARY KEY,
		fechapresupuestocontable DATE,
		conceptopresupuestocontable varchar NOT NULL
		);
	END IF;

	SELECT INTO rs * FROM pg_tables  WHERE tablename=''lpresupuestocontable'';

	IF NOT FOUND THEN
		CREATE TABLE lpresupuestocontable (
		idlpresupuestocontable SERIAL PRIMARY KEY,
		idpresupuestocontable INTEGER REFERENCES presupuestocontable(idpresupuestocontable),
		idpartida integer REFERENCES partida(idpartida),
		conceptolpresupuestocontable varchar NOT NULL,
		saldolpresupuestocontable NUMERIC(12,2)
		);
	END IF;

	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;




\echo -n ':: Funcion que calcula el codigo completo de la partida ... '
SELECT drop_if_exists_proc ('calculacodigocompletopartida','');
CREATE FUNCTION calculacodigocompletopartida() RETURNS "trigger"
AS '
DECLARE
    rs RECORD;
    codigocompleto character varying(50);

BEGIN
    codigocompleto := NEW.codigopartida;
    SELECT INTO rs codigocompletopartida FROM partida WHERE idpartida = NEW.padre;
    IF FOUND THEN
	codigocompleto := rs || codigocompleto;
    END IF;
    NEW.codigocompletopartida := codigocompleto;
    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador que calcula el codigo completo de la partida ... '
CREATE TRIGGER calculacodigocompletopartidatrigger
    BEFORE INSERT OR UPDATE ON partida
    FOR EACH ROW
    EXECUTE PROCEDURE calculacodigocompletopartida();
    
    
\echo -n ':: Funcion que propaga el codigo completo de la partida ... '
SELECT drop_if_exists_proc ('propagacodigocompletopartida','');
CREATE FUNCTION propagacodigocompletopartida() RETURNS "trigger"
AS '
DECLARE

BEGIN
    UPDATE partida SET codigocompletopartida = codigocompletopartida WHERE padre = NEW.idpartida;
    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador que propaga el codigo completo de la partida ... '
CREATE TRIGGER propagacodigocompletopartidatrigger
    AFTER UPDATE ON partida
    FOR EACH ROW
    EXECUTE PROCEDURE propagacodigocompletopartida();




-- ==============================================================================


-- Agregamos nuevos parametros de configuraci�.
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	rs RECORD;

BEGIN
	SELECT INTO rs * FROM configuracion WHERE nombre=''PluginBf_MiniContabilidad'';

	IF FOUND THEN
		UPDATE CONFIGURACION SET valor=''0.12.1-0002'' WHERE nombre=''PluginBf_MiniContabilidad'';
	ELSE
		INSERT INTO configuracion (nombre, valor) VALUES (''PluginBf_MiniContabilidad'', ''0.12.1-0002'');
	END IF;

	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT actualizarevision();
DROP FUNCTION actualizarevision() CASCADE;
\echo "Actualizada la revision de la base de datos a la version 0.12.1"


DROP FUNCTION drop_if_exists_table(text) CASCADE;
DROP FUNCTION drop_if_exists_proc(text,text) CASCADE;


COMMIT;
