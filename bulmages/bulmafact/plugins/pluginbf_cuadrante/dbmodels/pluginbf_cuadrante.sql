--
-- Modificación de campos y funciones de la BD para la adaptacion para el plugin de Trazabilidad
--
\echo "********* INICIADO FICHERO DE ESTRUCTURA DEL PLUGIN DE CUADRANTE *********"

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


CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
	rs RECORD;
BEGIN
	SELECT INTO rs * FROM pg_attribute  WHERE attname=''idcuadrante'';

	IF NOT FOUND THEN
		CREATE TABLE cuadrante (
			idcuadrante serial PRIMARY KEY,
			fechacuadrante date NOT NULL,
			idalmacen integer NOT NULL REFERENCES almacen(idalmacen),
			comentcuadrante varchar,
			aperturacuadrante time,
			cierrecuadrante   time,
			apertura1cuadrante time,
			cierre1cuadrante time,
			fiestacuadrante boolean DEFAULT FALSE,
			UNIQUE(fechacuadrante, idalmacen)
		);

		CREATE TABLE horario (
			idhorario serial PRIMARY KEY,
			idtrabajador integer NOT NULL REFERENCES trabajador(idtrabajador),
			idcuadrante integer NOT NULL REFERENCES cuadrante(idcuadrante),
			horainhorario time,
			horafinhorario time
		);

		ALTER TABLE almacen ADD COLUMN aperturaalmacen TIME;
		ALTER TABLE almacen ADD COLUMN cierrealmacen TIME;
		ALTER TABLE almacen ADD COLUMN apertura1almacen TIME;
		ALTER TABLE almacen ADD COLUMN cierre1almacen TIME;
	END IF;

	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo "Agregamos las tablas necesarias para cuadrantes."



SELECT drop_if_exists_proc ('ncuadrante','');
CREATE OR REPLACE FUNCTION ncuadrante() returns TRIGGER
AS '
DECLARE
    rs RECORD;

BEGIN
    IF NEW.aperturacuadrante IS NULL THEN
	SELECT INTO rs * FROM almacen WHERE idalmacen = NEW.idalmacen;
	NEW.aperturacuadrante = rs.aperturaalmacen;
	NEW.cierrecuadrante = rs.cierrealmacen;
	NEW.apertura1cuadrante = rs.apertura1almacen;
	NEW.cierre1cuadrante = rs.cierre1almacen;
    END IF;

    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ":: Disparador que completa las inserciones en el cuadrante "
CREATE TRIGGER cuadrante_trigger
    BEFORE INSERT ON cuadrante
    FOR EACH ROW
    EXECUTE PROCEDURE ncuadrante();



CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
	rs RECORD;

BEGIN
	SELECT INTO rs * FROM pg_attribute  WHERE attname=''idausencia'';

	IF NOT FOUND THEN
		CREATE TABLE ausencia (
			idausencia serial PRIMARY KEY,
			idtrabajador integer NOT NULL REFERENCES trabajador(idtrabajador),
			fechainausencia date NOT NULL,
			fechafinausencia date NOT NULL,
			motivoausencia varchar,
			CHECK (fechainausencia <= fechafinausencia)
		);
	END IF;

	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo "Agregamos las tablas necesarias para control de ausencias"

-- ==============================================================================


-- Agregamos nuevos parametros de configuraci�.
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	rs RECORD;

BEGIN
	SELECT INTO rs * FROM configuracion WHERE nombre=''PluginBf_Cuadrante'';

	IF FOUND THEN
		UPDATE CONFIGURACION SET valor=''0.9.3-0002'' WHERE nombre=''PluginBf_Cuadrante'';
	ELSE
		INSERT INTO configuracion (nombre, valor) VALUES (''PluginBf_Cuadrante'', ''0.9.3-0002'');
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
