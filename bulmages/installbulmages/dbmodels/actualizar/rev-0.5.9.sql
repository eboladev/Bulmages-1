--
-- Modificación de campos y funciones de la BD para la adaptaci� al tipo de datos monetario
--

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


CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER
AS '
BEGIN
	ALTER TABLE borrador ALTER COLUMN orden DROP NOT NULL;
	ALTER TABLE registroiva ALTER COLUMN idborrador SET NOT NULL;
	ALTER TABLE registroiva ADD FOREIGN KEY (idborrador) REFERENCES borrador(idborrador);

	RETURN 0;
END;
' LANGUAGE 'plpgsql';
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo "Quita restricciones en el campo orden"


SELECT drop_if_exists_proc ('restriccionesborrador','');
CREATE FUNCTION restriccionesborrador () RETURNS "trigger"
AS '
DECLARE
        cta RECORD;
	ej RECORD;
	ord RECORD;
BEGIN
        SELECT INTO cta * FROM cuenta WHERE idcuenta = NEW.idcuenta;
	IF FOUND THEN
                IF cta.bloqueada THEN
                        RAISE EXCEPTION '' Cuenta bloqueada, no se puede utilizar esta cuenta '';
                END IF;

                IF cta.nodebe THEN
                        IF NEW.debe <> 0 THEN
                                RAISE EXCEPTION '' Cuenta bloqueada por debe, solo permite haber '';
                        END IF;
                END IF;
                IF cta.nohaber THEN
                        IF NEW.haber <> 0 THEN
                                RAISE EXCEPTION '' Cuenta bloqueada por haber, solo permite debe '';
                        END IF;
                END IF;
	ELSE
		RAISE EXCEPTION '' Cuenta inexistente '';
        END IF;
	
	SELECT INTO  ej  * FROM ejercicios WHERE ejercicio = EXTRACT (YEAR FROM NEW.fecha) AND periodo =0;
	IF FOUND THEN
		IF ej.bloqueado = TRUE THEN
			RAISE EXCEPTION '' Periodo bloqueado '';
		END IF;
	ELSE
		RAISE EXCEPTION '' Ejercicio Inexistente'';
	END IF;
	SELECT INTO  ej * FROM ejercicios WHERE ejercicio = EXTRACT (YEAR FROM NEW.fecha) AND periodo = EXTRACT (MONTH FROM NEW.fecha);
	IF ej.bloqueado = TRUE THEN
		RAISE EXCEPTION '' Periodo bloqueado '';
	END IF;
	
	
        RETURN NEW;
END;
' LANGUAGE plpgsql;


CREATE TRIGGER restriccionesborradortrigger
    BEFORE INSERT OR UPDATE ON borrador
    FOR EACH ROW
    EXECUTE PROCEDURE restriccionesborrador();
\echo "restricciones para los borradores modificadas"



--
-- Agregamos nuevos parametros de configuraci�.
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	as RECORD;
BEGIN
	SELECT INTO as * FROM configuracion WHERE nombre=''DatabaseRevision'';
	IF FOUND THEN
		UPDATE CONFIGURACION SET valor=''0.5.9'' WHERE nombre=''DatabaseRevision'';
	ELSE
		INSERT INTO configuracion (idconfiguracion, nombre, valor) VALUES (20, ''DatabaseRevision'', ''0.5.9''); 		 
	END IF;
	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT actualizarevision();
DROP FUNCTION actualizarevision() CASCADE;
\echo "Actualizada la revisión de la base de datos"


DROP FUNCTION drop_if_exists_table(text) CASCADE;
DROP FUNCTION drop_if_exists_proc(text,text) CASCADE;


COMMIT;