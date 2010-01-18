--
-- ACTUALIZACION DE LA BASE DE DATOS DE BULMAFACT
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
-- NOTA: Si alguien sabe como pasar por parametro un nombre de tabla y campo a modificarse
-- hara mucho mas sencillito porque solo habra que implementar un funcion ya que siempre
-- hay que hacer lo mismo.
--

--
-- Funcion auxiliar para borrar funciones limpiamente
--
CREATE OR REPLACE FUNCTION drop_if_exists_table (text) RETURNS INTEGER AS '
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


CREATE OR REPLACE FUNCTION drop_if_exists_proc (text, text) RETURNS INTEGER AS '
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
	SELECT INTO as * FROM configuracion WHERE nombre=''DatabaseRevision'' AND ( valor LIKE ''0.12.1%'' OR valor = ''0.11.1-0004'');
	IF FOUND THEN
		RETURN 0;
	ELSE
		RAISE EXCEPTION '' Version de la base de datos invalida, aplique parches anteriores'';
		RETURN -1;		 
	END IF;
END;
'   LANGUAGE plpgsql;
SELECT compruebarevision();
DROP FUNCTION compruebarevision() CASCADE;
\echo "Comprobada la revision"

-- ========================  FIN DE LA COMPROBACION ============================


-- ================================== ACTUALIZACION  ===================================

SELECT drop_if_exists_proc('disminuyestockp', '');
SELECT drop_if_exists_proc('aumentastockp', '');

SELECT drop_if_exists_proc('cambiastockp', '');
\echo -n ':: Funcion cambia stock por la parte de albaranes de proveedor ... '
CREATE FUNCTION cambiastockp() RETURNS "trigger"
AS '
DECLARE

BEGIN
    -- Hacemos el update del stock del articulo
    IF TG_OP = ''DELETE'' OR TG_OP = ''UPDATE'' THEN
      UPDATE articulo SET stockarticulo = stockarticulo - OLD.cantlalbaranp WHERE idarticulo= OLD.idarticulo;
    END IF;
    IF TG_OP = ''INSERT'' OR TG_OP = ''UPDATE'' THEN
      UPDATE articulo SET stockarticulo = stockarticulo + NEW.cantlalbaranp WHERE idarticulo= NEW.idarticulo;
    END IF;
    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador que disminuye stock al borrar o actualizar el detalle de un albaran de proveedor ... '
CREATE TRIGGER cambiastockpt
    AFTER INSERT OR DELETE OR UPDATE ON lalbaranp
    FOR EACH ROW
    EXECUTE PROCEDURE cambiastockp();



SELECT drop_if_exists_proc('disminuyestock', '');
SELECT drop_if_exists_proc('aumentastock', '');


SELECT drop_if_exists_proc('cambiastock', '');
\echo -n ':: Funcion cambia stock por la parte de albaranes de cliente ... '
CREATE FUNCTION cambiastock() RETURNS "trigger"
AS '
DECLARE

BEGIN
    -- Hacemos el update del stock del articulo
    IF TG_OP = ''DELETE'' OR TG_OP = ''UPDATE'' THEN
      UPDATE articulo SET stockarticulo = stockarticulo + OLD.cantlalbaran WHERE idarticulo= OLD.idarticulo;
    END IF;
    IF TG_OP = ''INSERT'' OR TG_OP = ''UPDATE'' THEN
      UPDATE articulo SET stockarticulo = stockarticulo - NEW.cantlalbaran WHERE idarticulo= NEW.idarticulo;
    END IF;
    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador que disminuye stock al borrar o actualizar el detalle de un albaran de proveedor ... '
CREATE TRIGGER cambiastockt
    AFTER INSERT OR DELETE OR UPDATE ON lalbaran
    FOR EACH ROW
    EXECUTE PROCEDURE cambiastock();




-- para poder especificar la forma de pago en los cobros
\echo -n ':: Agrega el campo idforma_pago a la tabla de cobros '
CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
        bs RECORD;
        cs RECORD;
BEGIN


   SELECT INTO bs attname, relname FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid=pg_class.oid WHERE attname=''cobro'' AND relname=''idforma_pago'';
   IF NOT FOUND THEN
           ALTER TABLE cobro  ADD COLUMN idforma_pago integer REFERENCES forma_pago(idforma_pago) ;
-- para compatibilidad con las bd de la branch docsMonolitic que usaban idbanco en lugar de 
-- sufijo
--           UPDATE banco set sufijobanco = idbanco ;
   END IF;

   FOR bs IN SELECT * FROM banco LOOP
      INSERT INTO forma_pago (descforma_pago, idbanco) VALUES (bs.nombanco, bs.idbanco);
      SELECT INTO cs MAX(idforma_pago) AS idf FROM forma_pago LIMIT 1;
      UPDATE cobro set idforma_pago = cs.idf WHERE idbanco = bs.idbanco;
   END LOOP;
  


   RETURN 0;
END;
' LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;




-- =====================================================================================

-- Agregamos nuevos parametros de configuracion
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	as RECORD;
BEGIN
	SELECT INTO as * FROM configuracion WHERE nombre = ''DatabaseRevision'';
	IF FOUND THEN
		UPDATE CONFIGURACION SET valor = ''0.12.1-0004'' WHERE nombre = ''DatabaseRevision'';
	ELSE
		INSERT INTO configuracion (nombre, valor) VALUES (''DatabaseRevision'', ''0.12.1-0004'');
	END IF;
	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT actualizarevision();
DROP FUNCTION actualizarevision() CASCADE;
\echo "Actualizada la revision de la base de datos a la version 0.12.0003"

DROP FUNCTION drop_if_exists_table(text) CASCADE;
DROP FUNCTION drop_if_exists_proc(text, text) CASCADE;


COMMIT;

