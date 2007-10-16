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
	SELECT INTO as * FROM configuracion WHERE nombre=''DatabaseRevision'' AND ( valor LIKE ''0.10.1%'' OR valor = ''0.9.3-0001'');
	IF FOUND THEN
		RETURN 0;
	ELSE
		RAISE EXCEPTION ''La base de datos no tiene la version adecuada para aplicar este parche'';
		RETURN -1;		 
	END IF;
END;
'   LANGUAGE plpgsql;
SELECT compruebarevision();
DROP FUNCTION compruebarevision() CASCADE;
\echo "Comprobada la revision"






-- ================================= A PARTIR DE AQUI EL PARCHE=========================
-- =====================================================================================
CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
	as RECORD;
BEGIN
	SELECT INTO as attname, relname FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid=pg_class.oid WHERE attname=''idgrupo'' AND relname=''cuenta'';
	IF FOUND THEN
		ALTER TABLE cuenta DROP COLUMN idgrupo;
	END IF;

	SELECT INTO as * FROM pg_tables WHERE tablename=''grupo'';
	IF FOUND THEN
		DROP TABLE grupo;
	END IF;

	SELECT INTO as attname, relname FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid=pg_class.oid WHERE attname=''idc_coste'' AND relname=''cuenta'';
	IF NOT FOUND THEN
		ALTER TABLE cuenta ADD COLUMN idc_coste integer;
		ALTER TABLE cuenta ADD CONSTRAINT idc_costefk FOREIGN KEY (idc_coste) REFERENCES c_coste(idc_coste);
	END IF;

	SELECT INTO as attname, relname FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid=pg_class.oid WHERE attname=''idc_costedist'' AND relname=''c_costedist'';
	IF NOT FOUND THEN
		CREATE TABLE c_costedist (
			idc_costedist serial PRIMARY KEY,
			idc_coste     integer NOT NULL REFERENCES c_coste(idc_coste),
			iddestc_coste integer NOT NULL REFERENCES c_coste(idc_coste),
			porcentc_costedist numeric(12,2) DEFAULT 0
		);
	END IF;


	RETURN 0;
END;
' LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo "Agregado el campo de Fecha Vencimiento de cobro"



\echo -n ':: Funcion distribucion centros de coste en borrador ... '
SELECT drop_if_exists_proc('propagaccosteborrador','');
CREATE FUNCTION propagaccosteborrador() RETURNS "trigger"
AS '
DECLARE
    ccostes RECORD;
    a integer;
    i integer;
    tdebe numeric(12,2);
    thaber numeric(12,2);
    descuadredebe numeric(12,2);
    descuadrehaber numeric(12,2);
BEGIN

    i := 0;
    WHILE i = 0 LOOP
	i := 1;

	IF NEW.idc_coste IS NULL THEN
		RETURN NEW;
	END IF;
	
	a := 0;
	descuadredebe := NEW.debe;
	descuadrehaber := NEW.haber;
	tdebe := NEW.debe;
	thaber := NEW.haber;
	FOR ccostes IN SELECT * FROM c_costedist WHERE iddestc_coste = NEW.idc_coste LOOP
		descuadredebe := descuadredebe - tdebe * ccostes.porcentc_costedist / 100;
		descuadrehaber := descuadrehaber - thaber * ccostes.porcentc_costedist / 100;
		IF a = 0 THEN
			NEW.debe := tdebe * ccostes.porcentc_costedist / 100;
			NEW.haber := thaber * ccostes.porcentc_costedist / 100;
			NEW.idc_coste := ccostes.idc_coste;
			a := 1;
			i := 0;
		ELSE
			INSERT INTO borrador (idasiento, fecha, conceptocontable, idcuenta, descripcion, debe, haber, contrapartida, comentario, idcanal, idc_coste, orden) VALUES(NEW.idasiento, NEW.fecha, NEW.conceptocontable, NEW.idcuenta, NEW.descripcion, tdebe * ccostes.porcentc_costedist / 100, thaber * ccostes.porcentc_costedist / 100, NEW.contrapartida, NEW.comentario, NEW.idcanal, ccostes.idc_coste, NEW.orden);
		END IF;
	END LOOP;
	
	IF a = 1 THEN 
		NEW.debe := NEW.debe - descuadredebe;
		NEW.haber := NEW.haber - descuadrehaber;
	END IF;

    END LOOP;

    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador restricciones borrador ... '
CREATE TRIGGER propagaccosteborradortrigger
    BEFORE INSERT OR UPDATE ON borrador
    FOR EACH ROW
    EXECUTE PROCEDURE propagaccosteborrador();

-- ================================== FIN PARCHE. ACTUALIZACION  =======================
-- =====================================================================================

-- Agregamos nuevos parametros de configuracion
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	as RECORD;
BEGIN
	SELECT INTO as * FROM configuracion WHERE nombre = ''DatabaseRevision'';
	IF FOUND THEN
		UPDATE CONFIGURACION SET valor = ''0.10.1-0005'' WHERE nombre = ''DatabaseRevision'';
	ELSE
		INSERT INTO configuracion (nombre, valor) VALUES (''DatabaseRevision'', ''0.10.1-0005'');
	END IF;
	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT actualizarevision();
DROP FUNCTION actualizarevision() CASCADE;
\echo "Actualizada la revision de la base de datos a la version 0.10.1"

DROP FUNCTION drop_if_exists_table(text) CASCADE;
DROP FUNCTION drop_if_exists_proc(text, text) CASCADE;


COMMIT;

