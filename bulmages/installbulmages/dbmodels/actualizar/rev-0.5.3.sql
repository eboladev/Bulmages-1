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

SELECT drop_if_exists_proc ('tr_borradoasiento','');
CREATE OR REPLACE FUNCTION tr_borradoasiento() RETURNS "trigger"
AS '
BEGIN
	UPDATE PREVCOBRO SET idasiento = NULL WHERE idasiento=OLD.idasiento;
	RETURN OLD;
END;
' LANGUAGE 'plpgsql';

CREATE TRIGGER restriccionesborradoasientotrigger
   BEFORE DELETE ON asiento
   FOR EACH ROW
   EXECUTE PROCEDURE tr_borradoasiento();


ALTER TABLE prevcobro ALTER COLUMN tipoprevcobro SET NOT NULL;
ALTER TABLE prevcobro ALTER COLUMN idregistroiva DROP NOT NULL;

ALTER TABLE prevcobro ADD COLUMN idctacliente integer REFERENCES cuenta(idcuenta);

\echo "Quitmaos la restricción que vincula gestión de cobros y pagos con facturas"

--
-- Agregamos nuevos parametros de configuraci�.
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	as RECORD;
BEGIN
	SELECT INTO as * FROM configuracion WHERE nombre=''DatabaseRevision'';
	IF FOUND THEN
		UPDATE CONFIGURACION SET valor=''0.5.3'' WHERE nombre=''DatabaseRevision'';
	ELSE
		INSERT INTO configuracion (idconfiguracion, nombre, valor) VALUES (20, ''DatabaseRevision'', ''0.5.3''); 		 
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