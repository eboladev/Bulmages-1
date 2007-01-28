--
-- ACTUALIZACION DE LA BASE DE DATOS DE BULMACONT
--


-- Ocultamos las noticias
SET client_min_messages TO warning;


BEGIN;

--
-- Estas primeras funciones cambiaran los tipos de columnas que estan como flotantes a NUMERIC.
-- Se trata de un parche que se desea aplicar para almacenar los tipos monetarios
-- ya que actualmente se encuantran almacenados como 'doubles' y es preferible
-- que se almacenen como tipo 'numeric'.
-- Todas devuelven como valor numerico el nmero de filas influenciadas por el cambio
-- NOTA: Si alguien sabe como pasar por parametro un nombre de tabla y campo a modificar se
-- hara mucho mas sencillito porque solo habran que implementar un funcion ya que siempre
-- hay que hacer lo mismo.
--

--
-- Función auxiliar para borrar funciones limpiamente
--
CREATE OR REPLACE FUNCTION drop_if_exists_table(text) RETURNS INTEGER AS '
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


CREATE OR REPLACE FUNCTION drop_if_exists_proc(text, text) RETURNS INTEGER AS '
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

-- SELECT drop_if_exists_proc ('calculacodigocompletoarticulo','');

-- ======================== COMPROBACION DE CUAL ES LA ULTIMA VERSION ==================================

CREATE OR REPLACE FUNCTION compruebarevision() RETURNS INTEGER AS '
DECLARE
	as RECORD;
BEGIN
	SELECT INTO as * FROM configuracion WHERE nombre=''DatabaseRevision'' AND ( valor LIKE ''0.9.1%'' OR valor = ''0.5.9-0002'');
	IF FOUND THEN
		RETURN 0;
	ELSE
		RETURN -1;		 
	END IF;
END;
'   LANGUAGE plpgsql;
SELECT compruebarevision();
DROP FUNCTION compruebarevision() CASCADE;
\echo "Comprobada la revision"



-- ================================= CAMBIO DE ABREASIENTO =============================
-- =====================================================================================

CREATE OR REPLACE FUNCTION abreasiento(integer) RETURNS integer
    AS '
DECLARE
    id_asiento ALIAS FOR $1;
BEGIN
    -- Para prevenir la apertura de un asiento en teoría bloqueado primero lo modificamos para que salte el trigger.
    -- Esto debería hacer las comprobaciones por si mismo pero asi es mas comodo.
    UPDATE asiento SET idasiento = idasiento WHERE idasiento = id_asiento;
    UPDATE apunte SET idasiento = idasiento WHERE idasiento = id_asiento;

    DELETE FROM apunte WHERE idasiento = id_asiento;
    RETURN 1;
END;
'
    LANGUAGE plpgsql;


SELECT drop_if_exists_proc('restriccionesasiento', '');
CREATE OR REPLACE FUNCTION restriccionesasiento() RETURNS "trigger"
    AS '
DECLARE
    ej RECORD;
BEGIN
    SELECT INTO  ej  * FROM ejercicios WHERE ejercicio = EXTRACT (YEAR FROM NEW.fecha) AND periodo = 0;
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
    IF NEW.ordenasiento ISNULL OR NEW.ordenasiento = 0 THEN
	SELECT INTO ej max(ordenasiento) + 1 AS max, count(idasiento) as cuenta FROM asiento WHERE EXTRACT (YEAR FROM NEW.fecha)= EXTRACT(YEAR FROM fecha);
	IF ej.cuenta > 0 THEN
	    NEW.ordenasiento = ej.max;
	ELSE
	    NEW.ordenasiento = 1;
	END IF;
    END IF;
    RETURN NEW;
END;
' LANGUAGE plpgsql;


CREATE TRIGGER restriccionesasientotrigger
   BEFORE INSERT OR UPDATE ON asiento
   FOR EACH ROW
   EXECUTE PROCEDURE restriccionesasiento();


-- ================================== ACTUALIZACION  ===================================
-- =====================================================================================

-- Agregamos nuevos parametros de configuracion
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	as RECORD;
BEGIN
	SELECT INTO as * FROM configuracion WHERE nombre = ''DatabaseRevision'';
	IF FOUND THEN
		UPDATE CONFIGURACION SET valor = ''0.9.1-0002'' WHERE nombre = ''DatabaseRevision'';
	ELSE
		INSERT INTO configuracion (nombre, valor) VALUES (''DatabaseRevision'', ''0.9.1-0002'');
	END IF;
	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT actualizarevision();
DROP FUNCTION actualizarevision() CASCADE;
\echo "Actualizada la revision de la base de datos a la version 0.9.1"

DROP FUNCTION drop_if_exists_table(text) CASCADE;
DROP FUNCTION drop_if_exists_proc(text, text) CASCADE;


COMMIT;
