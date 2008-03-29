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
	SELECT INTO as * FROM configuracion WHERE nombre=''DatabaseRevision'' AND ( valor LIKE ''0.11.1%'' OR valor = ''0.10.1-0005'');
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

\echo -n ':: Funcion cerrar asiento ... '
SELECT drop_if_exists_proc('cierraasiento','integer');
CREATE FUNCTION cierraasiento(integer) RETURNS integer
    AS '
DECLARE
    id_asiento ALIAS FOR $1;
    mrecord RECORD;
BEGIN
    DELETE FROM apunte WHERE idasiento=id_asiento;

     -- // Debemos comprobar que el asiento se puede cerrar.
     SELECT INTO mrecord SUM(debe) AS tdebe, SUM(haber) AS thaber FROM borrador WHERE idasiento = id_asiento;
     IF FOUND THEN
	IF mrecord.tdebe <> mrecord.thaber THEN
		RETURN -1;
	END IF;
	IF mrecord.tdebe = 0 THEN
		RETURN -1;
	END IF;
     ELSE
	RETURN -1;
     END IF;

     -- // Linia afegida per Josep B.
    FOR mrecord IN SELECT * from borrador WHERE idasiento = id_asiento LOOP
	INSERT INTO apunte (codigoborrador, idasiento, iddiario, fecha, conceptocontable, idcuenta, descripcion, debe,
		    haber, contrapartida, comentario, idcanal, marcaconciliacion, idc_coste, idtipoiva, orden) VALUES
		    (mrecord.codigoborrador, mrecord.idasiento, mrecord.iddiario, mrecord.fecha, mrecord.conceptocontable,
		    mrecord.idcuenta, mrecord.descripcion, mrecord.debe, mrecord.haber, mrecord.contrapartida,
		    mrecord.comentario, mrecord.idcanal, mrecord.marcaconciliacion, mrecord.idc_coste, mrecord.idtipoiva,
		    mrecord.orden);
    END LOOP;
    -- Cuando cerramos el asiento, tambien recalculamos todas las contrapartidas.	
    PERFORM contraasiento(id_asiento);
    RETURN 1;
END;
'
    LANGUAGE plpgsql;



\echo -n ':: Funcion aumenta valor ... '
SELECT drop_if_exists_proc('aumenta_valor','');
CREATE FUNCTION aumenta_valor() RETURNS "trigger"
    AS '
DECLARE
    cta int4;
    ccost int4;
    ctar RECORD;
    ccostr RECORD;
BEGIN
    IF NEW.debe <> OLD.debe OR NEW.haber <> OLD.haber THEN
	UPDATE cuenta SET debe = debe + NEW.debe, haber = haber + NEW.haber WHERE idcuenta = NEW.idcuenta;
	UPDATE c_coste SET debe = debe + NEW.debe, haber = haber + NEW.haber WHERE idc_coste = NEW.idc_coste;
	IF NEW.idcuenta IS NOT NULL THEN
		UPDATE acumulado_canal SET debe = debe + NEW.debe, haber = haber + NEW.haber WHERE idcuenta = NEW.idcuenta AND idcanal = NEW.idcanal;
	END IF;
	cta := NEW.idcuenta;
	ccost := NEW.idc_coste;
	-- RAISE NOTICE '' Se ha lanzado la funcion aumenta_valor()'';
	SELECT INTO ccostr * FROM c_coste WHERE idc_coste = ccost;
	WHILE FOUND LOOP
		SELECT INTO ctar * FROM cuenta WHERE idcuenta = cta;
		WHILE FOUND LOOP
		-- RAISE NOTICE '' Cuenta % Centro Coste %'', ctar.idcuenta, ccostr.idc_coste;
		UPDATE acumulado_c_coste SET debe = debe + NEW.debe, haber = haber + NEW.haber WHERE idc_coste = ccostr.idc_coste AND idcuenta = ctar.idcuenta;
		SELECT INTO ctar * FROM cuenta WHERE idcuenta = ctar.padre;
		END LOOP;
		SELECT INTO ccostr * FROM c_coste WHERE idc_coste = ccostr.padre;
	END LOOP;
    END IF;
    RETURN NEW;
END;
'
    LANGUAGE plpgsql;


\echo -n ':: Disparador nuevo apunte aumente valor ... '
CREATE TRIGGER nuevo_apunte
    AFTER INSERT OR UPDATE ON apunte
    FOR EACH ROW
    EXECUTE PROCEDURE aumenta_valor();




\echo -n ':: Funcion disminuye valor ... '
SELECT drop_if_exists_proc('disminuye_valor','');
CREATE FUNCTION disminuye_valor() RETURNS "trigger"
    AS '
DECLARE
    cta int4;
    ccost int4;
    ctar RECORD;
    ccostr RECORD;
BEGIN
    IF NEW.debe <> OLD.debe OR NEW.haber <> OLD.haber THEN
	-- RAISE NOTICE ''disminuye_valor: debe antiguo %, debe nuevo %'', OLD.debe, NEW.debe;
	UPDATE cuenta SET debe = debe - OLD.debe, haber = haber - OLD.haber WHERE idcuenta = OLD.idcuenta;
	UPDATE c_coste SET debe = debe - OLD.debe, haber = haber - OLD.haber WHERE idc_coste = OLD.idc_coste;
	IF OLD.idcuenta IS NOT NULL THEN
		UPDATE acumulado_canal SET debe= debe - OLD.debe, haber =haber - OLD.haber WHERE idcuenta = OLD.idcuenta AND idcanal = OLD.idcanal;
	END IF;
	cta := OLD.idcuenta;
	ccost := OLD.idc_coste;
	-- RAISE NOTICE '' Se ha lanzado la funcion disminuye_valor()'';
	SELECT INTO ccostr * FROM c_coste WHERE idc_coste = ccost;
	WHILE FOUND LOOP
		SELECT INTO ctar * FROM cuenta WHERE idcuenta = cta;
		WHILE FOUND LOOP
		-- RAISE NOTICE '' Cuenta % Centro Coste %'', ctar.idcuenta, ccostr.idc_coste;
		UPDATE acumulado_c_coste SET debe = debe - OLD.debe, haber = haber -OLD.haber WHERE idc_coste = ccostr.idc_coste AND idcuenta = ctar.idcuenta;
		SELECT INTO ctar * FROM cuenta WHERE idcuenta = ctar.padre;
		END LOOP;
		SELECT INTO ccostr * FROM c_coste WHERE idc_coste = ccostr.padre;
	END LOOP;
	-- RAISE NOTICE '' disminuye_valor: Finaliza el algoritmo. '';
    END IF;
    RETURN NEW;
END;
'
    LANGUAGE plpgsql;


\echo -n ':: Disparador nuevo apunte disminuye valor ... '
CREATE TRIGGER nuevo_apunte1
    AFTER UPDATE ON apunte
    FOR EACH ROW
    EXECUTE PROCEDURE disminuye_valor();



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
		UPDATE CONFIGURACION SET valor = ''0.11.1-0002'' WHERE nombre = ''DatabaseRevision'';
	ELSE
		INSERT INTO configuracion (nombre, valor) VALUES (''DatabaseRevision'', ''0.11.1-0002'');
	END IF;
	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT actualizarevision();
DROP FUNCTION actualizarevision() CASCADE;
\echo "Actualizada la revision de la base de datos a la version 0.11.1"

DROP FUNCTION drop_if_exists_table(text) CASCADE;
DROP FUNCTION drop_if_exists_proc(text, text) CASCADE;


COMMIT;

