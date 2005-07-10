--
-- Modificación de campos y funciones de la BD para la adaptación al tipo de datos monetario
--

BEGIN;

--
-- Estas primeras funciones cambiarán los tipos de columnas que están como flotantes a NUMERIC.
-- Se trata de un parche que se desea aplicar para almacenar los tipos monetarios
-- ya que actualmente se encuantran almacenados como 'doubles' y es preferible
-- que se almacenen como tipo 'numeric'.
-- Todas devuelven como valor numérico el número de filas influenciadas por el cambio
-- NOTA: Si alguien sabe como pasar por parámetro un nombre de tabla y campo a modificar se
-- haría mucho más sencillito porque sólo habría que implementar un función ya que siempre
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


\echo "Gestiones Iniciales"

CREATE OR REPLACE FUNCTION reordenaasientosall() RETURNS integer
    AS '
DECLARE
    bs RECORD;
    ejercicio integer;
BEGIN
    FOR bs IN SELECT DISTINCT EXTRACT (YEAR  FROM FECHA) AS ano FROM asiento ORDER BY ano LOOP
        ejercicio = bs.ano;
        PERFORM reordenaasientos(ejercicio);
    END LOOP;
    RETURN 0;
END;
'    LANGUAGE plpgsql;

\echo "Cambiamos la función de reordenar asientos pq daba problemas."
 
--
-- Agregamos nuevos parametros de configuración.
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	as RECORD;
BEGIN
	SELECT INTO as * FROM configuracion WHERE nombre=''DatabaseRevision'';
	IF FOUND THEN
		UPDATE CONFIGURACION SET valor=''0.5.1'' WHERE nombre=''DatabaseRevision'';
	ELSE
		INSERT INTO configuracion (idconfiguracion, nombre, valor) VALUES (20, ''DatabaseRevision'', ''0.5.1''); 
		INSERT INTO configuracion (idconfiguracion, nombre, valor) VALUES (24, ''CuentasDerechos'', ''40:41'');
		INSERT INTO configuracion (idconfiguracion, nombre, valor) VALUES (25, ''CuentasObligaciones'', ''43;3'');		 
	END IF;
	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT actualizarevision();
DROP FUNCTION actualizarevision() CASCADE;
\echo "Actualizada la revisión de la base de datos"


CREATE OR REPLACE FUNCTION restriccionesborradocuenta () RETURNS "trigger"
AS '
DECLARE
	cta RECORD;
BEGIN
        SELECT INTO cta * FROM cuenta WHERE padre = OLD.idcuenta;
	IF FOUND THEN
                RAISE EXCEPTION '' La cuenta tiene hijos. '';
        END IF;
        RETURN OLD;
END;
' LANGUAGE plpgsql;

CREATE TRIGGER restriccionescuentatrigger
    BEFORE DELETE ON cuenta
    FOR EACH ROW
    EXECUTE PROCEDURE restriccionesborradocuenta(); 

\echo "Agregadas restricciones sobre los padres"


DROP FUNCTION drop_if_exists_table(text) CASCADE;
DROP FUNCTION drop_if_exists_proc(text,text) CASCADE;


COMMIT;