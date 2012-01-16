--
-- Modificación de campos y funciones de la BD para la adaptacion para el plugin de Inventario
--
\echo "********* Pone los stocks correctos y quita los inventarios. *********"

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



-- ========================== CONTROL DE STOCK =======================



CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
	cs RECORD;
	rs RECORD;
        nstock NUMERIC(12,2);
BEGIN
	UPDATE controlstock SET stockantcontrolstock= 0 , stocknewcontrolstock=0;
	UPDATE stock_almacen SET stock =0;

--	nstock := 0;
	FOR cs IN SELECT * FROM articulo LOOP
		nstock := 0;	
		SELECT INTO rs COALESCE (sum(cantlalbaranp), 0) AS totalin FROM lalbaranp WHERE idarticulo = cs.idarticulo;
		IF FOUND THEN
			nstock := nstock + rs.totalin;
		END IF;
		SELECT INTO rs COALESCE (sum(cantlalbaran), 0) AS totalin FROM lalbaran WHERE idarticulo = cs.idarticulo;
		IF FOUND THEN
			nstock := nstock - rs.totalin;
		END IF;
		UPDATE articulo set stockarticulo = nstock WHERE idarticulo = cs.idarticulo;		
	END LOOP;

	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;

\echo "Calculados los stocks iniciales sin inventario."


DROP FUNCTION drop_if_exists_table(text) CASCADE;
DROP FUNCTION drop_if_exists_proc(text,text) CASCADE;


COMMIT;
