--
-- Modificación de campos y funciones de la BD para la adaptacion para el plugin de Inventario
--
\echo "********* PLUGIN DE MINICONTABILIDAD *********"

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



-- ========================== INSERCION DE PARTIDAS =======================



CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
	rs RECORD;
BEGIN
	SELECT INTO rs * FROM partida;

	IF NOT FOUND THEN
		INSERT INTO partida (codigocompletopartida,codigopartida, nombrepartida,ingresopartida, padre) VALUES (''0'',''0'',''Ingressos'',TRUE, NULL);
		INSERT INTO partida (codigocompletopartida,codigopartida, nombrepartida,ingresopartida, padre) VALUES (''1'',''1'',''Despeses'',TRUE, NULL);
		INSERT INTO partida (codigocompletopartida,codigopartida, nombrepartida,ingresopartida,padre) VALUES (''001'',''01'',''Quotes cobrades'',TRUE,1);
		INSERT INTO partida (codigocompletopartida,codigopartida, nombrepartida,ingresopartida,padre) VALUES (''002'',''02'',''Altres ingressos dels associats'',TRUE,1);
		INSERT INTO partida (codigocompletopartida,codigopartida, nombrepartida,ingresopartida,padre) VALUES (''003'',''03'',''Subvencions'',TRUE,1);
		INSERT INTO partida (codigocompletopartida,codigopartida, nombrepartida,ingresopartida,padre) VALUES (''101'',''01'',''Activitats pròpies del fi social'',FALSE,2);
		INSERT INTO partida (codigocompletopartida,codigopartida, nombrepartida,ingresopartida,padre) VALUES (''102'',''02'',''Donacions'',FALSE,2);
		INSERT INTO partida (codigocompletopartida,codigopartida, nombrepartida,ingresopartida,padre) VALUES (''103'',''03'',''Material oficina i trameses'',FALSE,2);
		INSERT INTO partida (codigocompletopartida,codigopartida, nombrepartida,ingresopartida,padre) VALUES (''104'',''04'',''Despeses banc'',FALSE,2);
		INSERT INTO partida (codigocompletopartida,codigopartida, nombrepartida,ingresopartida,padre) VALUES (''105'',''05'',''Serveis públics'',FALSE,2);
		INSERT INTO partida (codigocompletopartida,codigopartida, nombrepartida,ingresopartida,padre) VALUES (''106'',''06'',''Quota FaPaC'',FALSE,2);
	END IF;

	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;





DROP FUNCTION drop_if_exists_table(text) CASCADE;
DROP FUNCTION drop_if_exists_proc(text,text) CASCADE;


COMMIT;
