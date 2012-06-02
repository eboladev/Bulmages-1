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
	rs RECORD;
BEGIN
	SELECT INTO rs * FROM configuracion WHERE nombre=''DatabaseRevision'' AND ( valor LIKE ''0.13.1%'' OR valor = ''0.12.1-0008'');
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

\echo -n ':: Funcion crearef para crear codigos de referencia ... '
CREATE OR REPLACE FUNCTION crearef() RETURNS character varying(15)
AS $$
DECLARE
    rs RECORD;
    result character varying(15);
    efound boolean;

BEGIN
    efound := FALSE;

    WHILE efound = FALSE LOOP
	result := random_string(6);
	efound := TRUE;

	SELECT INTO rs idpresupuesto FROM presupuesto WHERE refpresupuesto = result;

	IF FOUND THEN
	    efound := FALSE;
	END IF;

	SELECT INTO rs idpedidocliente FROM pedidocliente WHERE refpedidocliente = result;

	IF FOUND THEN
	    efound := FALSE;
	END IF;

	SELECT INTO rs idalbaran FROM albaran WHERE refalbaran = result;

	IF FOUND THEN
	    efound := FALSE;
	END IF;

	SELECT INTO rs idfactura FROM factura WHERE reffactura = result;

	IF FOUND THEN
		efound := FALSE;
	END IF;

	SELECT INTO rs idcobro FROM cobro WHERE refcobro = result;

	IF FOUND THEN
		efound := FALSE;
	END IF;

	SELECT INTO rs idpedidoproveedor FROM pedidoproveedor WHERE refpedidoproveedor = result;

	IF FOUND THEN
		efound := FALSE;
	END IF;

	SELECT INTO rs idalbaranp FROM albaranp WHERE refalbaranp = result;

	IF FOUND THEN
		efound := FALSE;
	END IF;

	SELECT INTO rs idfacturap FROM facturap WHERE reffacturap = result;
	IF FOUND THEN
		efound := FALSE;
	END IF;

	SELECT INTO rs idpago FROM pago WHERE refpago = result;

	IF FOUND THEN
		efound := FALSE;
	END IF;

-- PLUGINS

    END LOOP;
    RETURN result;
END;
$$ LANGUAGE plpgsql;


\echo -n ':: Funcion que muestra cadena de texto sin acentos ... '
CREATE OR REPLACE FUNCTION sinacentos (text) RETURNS text AS $$
    SELECT translate($1,'áéíóúÁÉÍÓÚäëïöüÄËÏÖÜñÑ','aeiouAEIOUaeiouAEIOUnN');
$$ LANGUAGE sql;


SELECT drop_if_exists_proc('restriccionescomparticulo', '');

\echo -n ':: Funcion con restricciones en los componentes de articulo ... '
CREATE FUNCTION restriccionescomparticulo() RETURNS "trigger"
AS '
DECLARE
BEGIN
    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador de las restricciones antes de insertar o actualizar un componente de articulo ... '
CREATE TRIGGER restriccionescomparticulotrigger
    BEFORE INSERT OR UPDATE ON comparticulo
    FOR EACH ROW
    EXECUTE PROCEDURE restriccionescomparticulo();



SELECT drop_if_exists_proc('calculacodigocompletoarticulo', '');


\echo -n ':: Funcion que calcula el codigo completo del articulo ... '
CREATE OR REPLACE FUNCTION calculacodigocompletoarticulo() RETURNS "trigger"
AS $$
DECLARE
    rs RECORD;
    codigocompleto character varying(100);
    codnumeric integer;

BEGIN
    -- Lo primero comprobamos es que el codigo del articulo no este vacio y de ser asi lo llenamos.
    IF NEW.codarticulo = '' OR NEW.codarticulo ISNULL THEN
	SELECT INTO rs max(codarticulo::int)::varchar AS m FROM articulo WHERE idfamilia = NEW.idfamilia;

	IF FOUND THEN
	    IF is_number(rs.m) THEN
		codnumeric := to_number(rs.m);
		codnumeric := codnumeric +1;
		NEW.codarticulo := CAST (codnumeric AS varchar);
		WHILE length(NEW.codarticulo) < 4 LOOP
		    NEW.codarticulo := '0' || NEW.codarticulo;
		END LOOP;
	    ELSE
		NEW.codarticulo := '0000';
	    END IF;
	ELSE
	    NEW.codarticulo = '0000';
	END IF;
    END IF;

    codigocompleto := NEW.codarticulo;

    SELECT INTO rs codigocompletofamilia FROM familia WHERE idfamilia = NEW.idfamilia;

    IF FOUND THEN
	codigocompleto := rs.codigocompletofamilia || codigocompleto;
    END IF;

    NEW.codigocompletoarticulo := codigocompleto;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;


\echo -n ':: Disparador que calcula el codigo completo del articulo ... '
CREATE TRIGGER calculacodigocompletoarticulotrigger
    BEFORE INSERT OR UPDATE ON articulo
    FOR EACH ROW
    EXECUTE PROCEDURE calculacodigocompletoarticulo();




-- Cambiamos la tabla lpedidocliente para que tenga el default 0 en el recargo de equivalencia
\echo -n ':: Cambiamos la tabla lpedidocliente para que tenga el default 0 en el recargo de equivalencia '
CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '

BEGIN

   ALTER TABLE lpedidocliente ALTER COLUMN reqeqlpedidocliente SET DEFAULT 0;
   UPDATE lpedidocliente SET reqeqlpedidocliente = 0 WHERE reqeqlpedidocliente IS NULL;

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
	rs RECORD;
BEGIN
	SELECT INTO rs * FROM configuracion WHERE nombre = ''DatabaseRevision'';
	IF FOUND THEN
		UPDATE CONFIGURACION SET valor = ''0.13.1-0006'' WHERE nombre = ''DatabaseRevision'';
	ELSE
		INSERT INTO configuracion (nombre, valor) VALUES (''DatabaseRevision'', ''0.13.1-0006'');
	END IF;
	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT actualizarevision();
DROP FUNCTION actualizarevision() CASCADE;
\echo "Actualizada la revision de la base de datos a la version 0.13.1"

DROP FUNCTION drop_if_exists_table(text) CASCADE;
DROP FUNCTION drop_if_exists_proc(text, text) CASCADE;


COMMIT;

