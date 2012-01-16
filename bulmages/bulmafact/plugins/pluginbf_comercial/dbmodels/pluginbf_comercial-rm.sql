
SET client_min_messages TO warning;
--
-- Modificacion de campos y funciones de la BD para la adaptacion para el plugin de ComercialBF.
--
\echo -n ':: Iniciamos transaccion ... '
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
	SELECT INTO rs * FROM pg_attribute WHERE attname = ''idzonacomercial'';

	IF FOUND THEN
	    ALTER TABLE trabajador  DROP CONSTRAINT idzonacomercialfk;
	    ALTER TABLE cliente DROP CONSTRAINT idzonacomercialfk;
	    ALTER TABLE trabajador  DROP COLUMN tipotrabajador;
	    ALTER TABLE trabajador  DROP COLUMN idzonacomercial;
	    ALTER TABLE cliente DROP COLUMN comentcomercialcliente;
	    ALTER TABLE cliente DROP COLUMN idzonacomercial;
	    ALTER TABLE cliente DROP COLUMN periodocomercialcliente;
	    ALTER TABLE cliente DROP COLUMN fechabasecomercialcliente;
	    ALTER TABLE cliente DROP COLUMN horarutacomercialcliente;

	    DROP TABLE incidenciacomercial;
	    DROP TABLE rutacomercial;
	    DROP TABLE zonacomercial;
	END IF;

	RETURN 0;
END;
' LANGUAGE plpgsql;

SELECT aux();

DROP FUNCTION aux() CASCADE;
\echo ':: Eliminamos los campos y tablas para el plugin ComercialBF ... '



DROP FUNCTION IF EXISTS crearutacomercial(date);

--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	rs RECORD;

BEGIN
	SELECT INTO rs * FROM configuracion WHERE nombre = ''PluginBf_Comercial'';

	IF FOUND THEN
		DELETE FROM CONFIGURACION WHERE nombre = ''PluginBf_Comercial'';
	END IF;

	RETURN 0;
END;
' LANGUAGE plpgsql;

SELECT actualizarevision();

DROP FUNCTION actualizarevision() CASCADE;
\echo ':: Eliminamos la revision de la base de datos '


DROP FUNCTION drop_if_exists_table(text) CASCADE;
DROP FUNCTION drop_if_exists_proc(text, text) CASCADE;


\echo -n ':: Finalizamos la transaccion ... '
COMMIT;

