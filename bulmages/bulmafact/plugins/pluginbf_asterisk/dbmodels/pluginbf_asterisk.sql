--
-- Modificación de campos y funciones de la BD para la adaptacion para el plugin de Trazabilidad
--
\echo "********* INICIADO FICHERO DE ESTRUCTURA DEL PLUGIN DE ASTERISK *********"

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
	SELECT INTO rs * FROM pg_attribute  WHERE attname=''idvalasterisk'';
	IF NOT FOUND THEN
		CREATE TABLE valasterisk (
			idvalasterisk serial PRIMARY KEY,
			fechavalasterisk date NOT NULL DEFAULT now(),
			idalmacen integer NOT NULL REFERENCES almacen(idalmacen),
			idtrabajador integer NOT NULL REFERENCES trabajador(idtrabajador),
			horavalasterisk time NOT NULL DEFAULT now(),
			archvalasterisk character varying
		);


		ALTER TABLE almacen ADD COLUMN extasteriskalmacen VARCHAR;
		ALTER TABLE trabajador ADD COLUMN passasterisktrabajador VARCHAR;
		ALTER TABLE almacen ADD CONSTRAINT extasteriskalmacenunk UNIQUE(extasteriskalmacen);
		ALTER TABLE trabajador ADD CONSTRAINT passasterisktrabajadorunk UNIQUE(passasterisktrabajador);

	END IF;

        SELECT INTO rs * FROM pg_attribute WHERE attname=''validasiempreasterisktrabajador'';
	IF NOT FOUND THEN
	    ALTER TABLE trabajador ADD COLUMN validasiempreasterisktrabajador BOOLEAN DEFAULT FALSE;
	END IF;

	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo "Agregamos las tablas necesarias para validaciones con Asterisk"



SELECT drop_if_exists_proc('validacionasterisk', 'varchar, varchar');
CREATE FUNCTION validacionasterisk(varchar, varchar) RETURNS "bool" AS '
DECLARE
    passtraba ALIAS FOR $1;
    extealm   ALIAS FOR $2;
    trab      RECORD;
    alm       RECORD;

BEGIN
   SELECT INTO trab * FROM trabajador WHERE passasterisktrabajador = passtraba;
   IF FOUND THEN 
	SELECT INTO alm * FROM almacen WHERE extasteriskalmacen = extealm;
	IF FOUND THEN
		INSERT INTO valasterisk (idtrabajador, idalmacen) VALUES (trab.idtrabajador, alm.idalmacen);
		RETURN TRUE;
	END IF;
   END IF;

   RETURN FALSE;
END;
'  LANGUAGE 'plpgsql';
\echo ":: Funcion validacionasterisk que genera una validacion para asterisk ... "


CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
	rs RECORD;
BEGIN
	SELECT INTO rs * FROM pg_attribute  WHERE attname=''arreglovalasterisk'';
	IF NOT FOUND THEN
		ALTER TABLE valasterisk ADD COLUMN arreglovalasterisk BOOLEAN DEFAULT FALSE;
	END IF;

	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo ":: Agregamos el campo de arreglos ... "

-- ==============================================================================


-- Agregamos nuevos parametros de configuracion
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	rs RECORD;
BEGIN
	SELECT INTO rs * FROM configuracion WHERE nombre=''PluginBf_Asterisk'';

	IF FOUND THEN
		UPDATE CONFIGURACION SET valor=''0.11-002'' WHERE nombre=''PluginBf_Asterisk'';
	ELSE
		INSERT INTO configuracion (nombre, valor) VALUES (''PluginBf_Asterisk'', ''0.11-002'');
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
