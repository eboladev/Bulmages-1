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
	SELECT INTO rs * FROM configuracion WHERE nombre=''DatabaseRevision'' AND ( valor LIKE ''0.13.1%'' OR valor = ''0.12.1-0004'');
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








\echo -n ':: Cambiamos la tabla borrador y apunte renombrando los campos orden'
CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '

BEGIN

   ALTER TABLE borrador RENAME COLUMN orden TO ordenborrador;
   ALTER TABLE apunte RENAME COLUMN orden TO ordenapunte;

   RETURN 0;
END;
' LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;



\echo -n ':: Funcion contrapartidas de un asiento ... '
CREATE OR REPLACE FUNCTION contraasiento(integer) RETURNS NUMERIC(12, 2)
   AS '
DECLARE
    midasiento ALIAS FOR $1;
    midapunte ALIAS FOR $1;
    apt RECORD;
    aptasien RECORD;
    cont RECORD;
    descuadre NUMERIC(12, 2);
    apmaxdebe INTEGER;
    apmaxhaber INTEGER;
    maxdebe NUMERIC(12, 2);
    maxhaber NUMERIC(12, 2);
    ctadebe INTEGER;
    ctahaber INTEGER;
    salida BOOLEAN;
    salidadebe BOOLEAN;
    salidahaber BOOLEAN;   
BEGIN
    maxdebe := 0;
    maxhaber := 0;
    apmaxdebe:=0;
    apmaxhaber := 0;
    ctadebe := 0;
    ctahaber := 0;
    descuadre := 0;
    FOR cont IN SELECT idcuenta,idapunte, debe, haber, ordenapunte FROM apunte WHERE idasiento = midasiento ORDER BY ordenapunte LOOP
	-- Si es el debe maximo lo hacemos constar.
	IF cont.debe >= maxdebe THEN
            maxdebe := cont.debe;
            apmaxdebe := cont.idapunte;
	    ctadebe := cont.idcuenta;
        END IF;
        -- Si es el haber maximo lo hacemos constar.
        IF cont.haber >= maxhaber THEN
            maxhaber := cont.haber;
            apmaxhaber := cont.idapunte;
	    ctahaber := cont.idcuenta;
        END IF;
        -- Calculamos el descuadre.
    	descuadre := descuadre + cont.debe;
        descuadre := descuadre - cont.haber;
        -- Si es el descuadre inicializamos las variables.
        IF descuadre = 0 AND ctadebe <> 0 AND ctahaber <> 0 THEN
            UPDATE apunte SET contrapartida= ctahaber WHERE haber=0 AND idasiento = midasiento AND ordenapunte <= cont.ordenapunte AND contrapartida ISNULL;
            UPDATE apunte SET contrapartida= ctadebe WHERE debe=0 AND idasiento = midasiento AND ordenapunte <= cont.ordenapunte AND contrapartida ISNULL;
            maxdebe := 0;
            maxhaber := 0;
            apmaxdebe:=0;
            apmaxhaber := 0;
	    ctadebe := 0;
	    ctahaber := 0;
        END IF;
    END LOOP;
    RETURN 0;
END;
   ' LANGUAGE plpgsql;  


\echo -n ':: Funcion buscar contrapartida ... '
CREATE OR REPLACE FUNCTION bcontrapartida(integer) RETURNS integer
    AS '
DECLARE
    midapunte ALIAS FOR $1;
    apt RECORD;
    aptasien RECORD;
    cont RECORD;
    descuadre NUMERIC(12, 2);
    apmaxdebe INTEGER;
    apmaxhaber INTEGER;
    maxdebe NUMERIC(12, 2);
    maxhaber NUMERIC(12, 2);
    salida BOOLEAN;
    salidadebe BOOLEAN;
    salidahaber BOOLEAN;
BEGIN
--  RAISE NOTICE ''Empezamos'';
    SELECT INTO apt contrapartida, idasiento FROM apunte WHERE idapunte = midapunte;
    IF apt.contrapartida ISNULL THEN
        -- Inicializamos las variables.
        descuadre := 0;
        maxdebe := 0;
        maxhaber := 0;
        apmaxdebe := 0;
        apmaxhaber := 0;
        salidadebe := FALSE;
        salidahaber := FALSE;
        FOR cont IN SELECT idapunte, debe, haber FROM apunte WHERE idasiento = apt.idasiento ORDER BY ordenapunte LOOP
            -- Si es la cuenta que estamos buscando lo hacemos constar.
            IF cont.idapunte = midapunte THEN
                IF cont.debe > 0 THEN
                    salidahaber := TRUE;
                ELSE
                    salidadebe := TRUE;
                END IF;
            END IF;
            -- Si es el debe maximo lo hacemos constar.
            IF cont.debe > maxdebe THEN
                maxdebe := cont.debe;
                apmaxdebe := cont.idapunte;
            END IF;
            -- Si es el haber maximo lo hacemos constar.
            IF cont.haber > maxhaber THEN
                maxhaber := cont.haber;
                apmaxhaber := cont.idapunte;
            END IF;
            -- Calculamos el descuadre.
            descuadre := descuadre + cont.debe;
            descuadre := descuadre - cont.haber;
            -- Si es el descuadre inicializamos las variables.
            IF (descuadre = 0) THEN  -- Asi nos aseguramos que valores positivos tambien entran.
                IF (salidadebe = TRUE) THEN
                    RETURN apmaxdebe;
                END IF;
                IF (salidahaber = TRUE) THEN
                    RETURN apmaxhaber;
                END IF;
                maxdebe := 0;
                maxhaber := 0;
                apmaxdebe:=0;
                apmaxhaber := 0;
            END IF;
        END LOOP;
        return 0;
    ELSE
        SELECT INTO cont idapunte FROM apunte WHERE idasiento = apt.idasiento AND idcuenta = apt.contrapartida;
        RETURN cont.idapunte;
    END IF;
END;
'
    LANGUAGE plpgsql;


\echo -n ':: Funcion buscar contrapartida borrador ... '
CREATE OR REPLACE FUNCTION bcontrapartidaborr(integer) RETURNS integer
    AS '
DECLARE
    midapunte ALIAS FOR $1;
    apt RECORD;
    aptasien RECORD;
    cont RECORD;
    descuadre numeric(12, 2);
    apmaxdebe INTEGER;
    apmaxhaber INTEGER;
    maxdebe numeric(12, 2);
    maxhaber numeric(12, 2);
    salida BOOLEAN;
    salidadebe BOOLEAN;
    salidahaber BOOLEAN;
BEGIN
--  RAISE NOTICE ''Empezamos'';
    SELECT INTO apt * FROM borrador WHERE idborrador = midapunte;
    IF apt.contrapartida ISNULL THEN
        -- Inicializamos las variables.
        descuadre := 0;
        maxdebe := 0;
        maxhaber := 0;
        apmaxdebe := 0;
        apmaxhaber := 0;
        salidadebe := FALSE;
        salidahaber := FALSE;
        FOR cont IN SELECT * FROM borrador WHERE idasiento = apt.idasiento ORDER BY ordenborrador LOOP
            -- Si es la cuenta que estamos buscando lo hacemos constar.
            IF cont.idborrador = midapunte THEN
                IF cont.debe > 0 THEN
                    salidahaber := TRUE;
                ELSE
                    salidadebe := TRUE;
                END IF;
            END IF;
            -- Si es el debe maximo lo hacemos constar.
            IF cont.debe > maxdebe THEN
                maxdebe := cont.debe;
                apmaxdebe := cont.idborrador;
            END IF;
            -- Si es el haber maximo lo hacemos constar.
            IF cont.haber > maxhaber THEN
                maxhaber := cont.haber;
                apmaxhaber := cont.idborrador;
            END IF;
            -- Calculamos el descuadre.
            descuadre := descuadre + cont.debe;
            descuadre := descuadre - cont.haber;
            -- Si es el descuadre inicializamos las variables.
            IF (descuadre * descuadre < 0.001) THEN  -- Asi nos aseguramos que valores positivos tambien entran.
                IF (salidadebe = TRUE) THEN
                    RETURN apmaxdebe;
                END IF;
                IF (salidahaber = TRUE) THEN
                    RETURN apmaxhaber;
                END IF;
                maxdebe := 0;
                maxhaber := 0;
                apmaxdebe := 0;
                apmaxhaber := 0;
            END IF;
        END LOOP;
        return 0;
    ELSE
        SELECT INTO cont * FROM borrador WHERE idasiento = apt.idasiento AND idcuenta = apt.contrapartida;
        RETURN cont.idborrador;
    END IF;
END;'
    LANGUAGE plpgsql;


\echo -n ':: Funcion cierra asiento ... '
CREATE OR REPLACE FUNCTION cierraasiento(integer) RETURNS integer
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
--	IF mrecord.tdebe = 0 THEN
--		RETURN -1;
--	END IF;
     ELSE
	RETURN -1;
     END IF;

     -- // Linia afegida per Josep B.
    FOR mrecord IN SELECT * from borrador WHERE idasiento = id_asiento LOOP
	INSERT INTO apunte (codigoborrador, idasiento, iddiario, fecha, conceptocontable, idcuenta, descripcion, debe,
		    haber, contrapartida, comentario, idcanal, marcaconciliacion, idc_coste, idtipoiva, ordenapunte) VALUES
		    (mrecord.codigoborrador, mrecord.idasiento, mrecord.iddiario, mrecord.fecha, mrecord.conceptocontable,
		    mrecord.idcuenta, mrecord.descripcion, mrecord.debe, mrecord.haber, mrecord.contrapartida,
		    mrecord.comentario, mrecord.idcanal, mrecord.marcaconciliacion, mrecord.idc_coste, mrecord.idtipoiva,
		    mrecord.ordenborrador);
    END LOOP;
    -- Cuando cerramos el asiento, tambien recalculamos todas las contrapartidas.	
    PERFORM contraasiento(id_asiento);
    RETURN 1;
END;
'
    LANGUAGE plpgsql;




\echo -n ':: Funcion distribucion centros de coste en borrador ... '
CREATE OR REPLACE FUNCTION propagaccosteborrador() RETURNS "trigger"
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
			INSERT INTO borrador (idasiento, fecha, conceptocontable, idcuenta, descripcion, debe, haber, contrapartida, comentario, idcanal, idc_coste, ordenborrador) VALUES(NEW.idasiento, NEW.fecha, NEW.conceptocontable, NEW.idcuenta, NEW.descripcion, tdebe * ccostes.porcentc_costedist / 100, thaber * ccostes.porcentc_costedist / 100, NEW.contrapartida, NEW.comentario, NEW.idcanal, ccostes.idc_coste, NEW.ordenborrador);
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






-- ================================= A PARTIR DE AQUI EL PARCHE=========================
-- =====================================================================================

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
		UPDATE CONFIGURACION SET valor = ''0.13.1-0002'' WHERE nombre = ''DatabaseRevision'';
	ELSE
		INSERT INTO configuracion (nombre, valor) VALUES (''DatabaseRevision'', ''0.13.1-0002'');
	END IF;
	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT actualizarevision();
DROP FUNCTION actualizarevision() CASCADE;
\echo "Actualizada la revision de la base de datos a la version 0.13.2"

DROP FUNCTION drop_if_exists_table(text) CASCADE;
DROP FUNCTION drop_if_exists_proc(text, text) CASCADE;


COMMIT;

