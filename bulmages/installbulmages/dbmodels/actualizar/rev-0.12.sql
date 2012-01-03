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
	rs RECORD;
BEGIN
	SELECT INTO rs * FROM configuracion WHERE nombre=''DatabaseRevision'' AND ( valor LIKE ''0.12.1%'' OR valor = ''0.11.1-0003'');
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

SELECT drop_if_exists_proc('aumenta_valor', '');
SELECT drop_if_exists_proc('disminuye_valor', '');
SELECT drop_if_exists_proc('disminuye_valor1', '');
SELECT drop_if_exists_proc('trataacumulados', '');

\echo -n ':: Funcion aumenta valor ... '

CREATE FUNCTION trataacumulados() RETURNS "trigger"
    AS '
DECLARE
    cta int4;
    ccost int4;
    ctar RECORD;
    ccostr RECORD;
BEGIN
  -- Si se trata de una insercion hacemos los calculos con el campo NEW
  IF TG_OP = ''INSERT'' THEN
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
      RETURN NEW;
  END IF;

  -- Si se trata de un borrado se hacen los calculos con el campo OLD
  IF TG_OP = ''DELETE'' THEN
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
    RETURN OLD;
  END IF;

  IF TG_OP = ''UPDATE'' THEN
    IF NEW.debe <> OLD.debe OR NEW.haber <> OLD.haber THEN
	-- RAISE NOTICE ''disminuye_valor: debe antiguo %, debe nuevo %'', OLD.debe, NEW.debe;
	UPDATE cuenta SET debe = debe - OLD.debe, haber = haber - OLD.haber + NEW.haber WHERE idcuenta = OLD.idcuenta;
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


	-- RAISE NOTICE '' disminuye_valor: Finaliza el algoritmo. '';
    END IF;
    RETURN NEW;
  END IF;

END;
'
    LANGUAGE plpgsql;


\echo -n ':: Disparador nuevo apunte aumente valor ... '
CREATE TRIGGER trataacumuladost
    AFTER INSERT OR UPDATE OR DELETE ON apunte
    FOR EACH ROW
    EXECUTE PROCEDURE trataacumulados();


SELECT drop_if_exists_proc('cambiadoiva', '');
SELECT drop_if_exists_proc('cambiadoivad', '');

\echo -n ':: Funcion cambiado IVA ... '
CREATE OR REPLACE FUNCTION cambiadoiva() RETURNS "trigger"
    AS '
DECLARE
    mrecord RECORD;
BEGIN
  IF TG_OP = ''INSERT'' OR TG_OP =''UPDATE'' THEN
    FOR mrecord IN SELECT SUM(baseiva) AS suma, SUM(ivaiva) AS sumaiva FROM iva WHERE iva.idregistroiva = NEW.idregistroiva LOOP
    	UPDATE registroiva SET baseimp = mrecord.suma, iva = mrecord.sumaiva WHERE idregistroiva = NEW.idregistroiva;
    END LOOP;
    RETURN NEW;
  END IF;

  IF TG_OP = ''DELETE'' THEN
    FOR mrecord IN SELECT SUM(baseiva) AS suma, SUM(ivaiva) AS sumaiva FROM iva WHERE iva.idregistroiva = OLD.idregistroiva LOOP
    	UPDATE registroiva SET baseimp = mrecord.suma, iva = mrecord.sumaiva WHERE idregistroiva = OLD.idregistroiva;
    END LOOP;
    RETURN OLD;
  END IF;
END;
'    LANGUAGE plpgsql;


\echo -n ':: Disparador cambio tipo IVA ... '
CREATE TRIGGER cambiadoivat
   AFTER INSERT OR UPDATE OR DELETE ON iva
   FOR EACH ROW
   EXECUTE PROCEDURE cambiadoiva(); 



CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
        rs RECORD;
BEGIN
	SELECT INTO rs * FROM pg_attribute WHERE attname=''pais'';
	IF FOUND THEN
		ALTER TABLE cuenta RENAME COLUMN pais TO idpais;
		ALTER TABLE cuenta RENAME COLUMN provincia TO idprovincia;
	END IF;
	RETURN 0;
END;
' LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo -n ':: Modificada tabla cuenta ... '



-- ================================== FIN PARCHE. ACTUALIZACION  =======================
-- =====================================================================================

-- Agregamos nuevos parametros de configuracion
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	rs RECORD;
BEGIN
	SELECT INTO rs * FROM configuracion WHERE nombre = ''DatabaseRevision'';
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
\echo "Actualizada la revision de la base de datos a la version 0.12.1"

DROP FUNCTION drop_if_exists_table(text) CASCADE;
DROP FUNCTION drop_if_exists_proc(text, text) CASCADE;


COMMIT;

