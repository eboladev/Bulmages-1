--
-- PostgreSQL database dump
--

SET client_encoding = 'UTF8';
SET standard_conforming_strings = off;
SET check_function_bodies = false;
SET client_min_messages = warning;
SET escape_string_warning = off;

--
-- Name: SCHEMA public; Type: COMMENT; Schema: -; Owner: postgres
--

COMMENT ON SCHEMA public IS 'Standard public schema';


--
-- Name: plpgsql; Type: PROCEDURAL LANGUAGE; Schema: -; Owner: postgres
--

CREATE PROCEDURAL LANGUAGE plpgsql;


SET search_path = public, pg_catalog;

--
-- Name: abreasiento(integer); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION abreasiento(integer) RETURNS integer
    AS $_$
DECLARE
    id_asiento ALIAS FOR $1;
BEGIN
    -- Para prevenir la apertura de un asiento en teoria bloqueado primero lo modificamos para que salte el trigger.
    -- Esto deberia hacer las comprobaciones por si mismo pero asi es mas comodo.
    UPDATE asiento SET idasiento = idasiento WHERE idasiento = id_asiento;
    UPDATE apunte SET idasiento = idasiento WHERE idasiento = id_asiento;

    DELETE FROM apunte WHERE idasiento = id_asiento;
    RETURN 1;
END;
$_$
    LANGUAGE plpgsql;


ALTER FUNCTION public.abreasiento(integer) OWNER TO postgres;

--
-- Name: abreasientos(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION abreasientos() RETURNS integer
    AS $$
--DECLARE bs RECORD;
--DECLARE res RECORD;
BEGIN
--    FOR bs IN SELECT * FROM asiento ORDER BY idasiento DESC LOOP
--      SELECT INTO res cambiaasiento(bs.idasiento, bs.idasiento * 3);
--    END LOOP;
--  Abrir los asientos es modificar el campo ordenasiento de los mismos para que se reorganicen
    UPDATE asiento SET ordenasiento = ordenasiento * 3;
    RETURN 0;
END;
$$
    LANGUAGE plpgsql;


ALTER FUNCTION public.abreasientos() OWNER TO postgres;

--
-- Name: acumulados_canal(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION acumulados_canal() RETURNS "trigger"
    AS $$
DECLARE
    incdebe  numeric(12, 2);
    inchaber numeric(12, 2);
    cuentar  RECORD;
BEGIN
    -- RAISE NOTICE ' Ha entrado el trigger acumulados_canal() ';
    -- RAISE NOTICE ' idcuenta % idcanal %', NEW.idcuenta, NEW.idcanal;
    incdebe = NEW.debe - OLD.debe;
    inchaber = NEW.haber - OLD.haber;
    SELECT INTO cuentar * FROM cuenta WHERE idcuenta = NEW.idcuenta;
    SELECT INTO cuentar * FROM cuenta WHERE idcuenta = cuentar.padre;
    IF FOUND THEN
	UPDATE acumulado_canal SET debe = debe + incdebe, haber = haber + inchaber WHERE idcuenta = cuentar.idcuenta AND idcanal = NEW.idcanal;
    END IF;
    RETURN NEW;
END;
$$
    LANGUAGE plpgsql;


ALTER FUNCTION public.acumulados_canal() OWNER TO postgres;

--
-- Name: aumenta_valor(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION aumenta_valor() RETURNS "trigger"
    AS $$
DECLARE
    cta int4;
    ccost int4;
    ctar RECORD;
    ccostr RECORD;
BEGIN
    UPDATE cuenta SET debe = debe + NEW.debe, haber = haber + NEW.haber WHERE idcuenta = NEW.idcuenta;
    UPDATE c_coste SET debe = debe + NEW.debe, haber = haber + NEW.haber WHERE idc_coste = NEW.idc_coste;
    IF NEW.idcuenta IS NOT NULL THEN
	UPDATE acumulado_canal SET debe = debe + NEW.debe, haber = haber + NEW.haber WHERE idcuenta = NEW.idcuenta AND idcanal = NEW.idcanal;
    END IF;
    cta := NEW.idcuenta;
    ccost := NEW.idc_coste;
    -- RAISE NOTICE ' Se ha lanzado la funcion aumenta_valor()';
    SELECT INTO ccostr * FROM c_coste WHERE idc_coste = ccost;
    WHILE FOUND LOOP
	SELECT INTO ctar * FROM cuenta WHERE idcuenta = cta;
	WHILE FOUND LOOP
	    -- RAISE NOTICE ' Cuenta % Centro Coste %', ctar.idcuenta, ccostr.idc_coste;
	    UPDATE acumulado_c_coste SET debe = debe + NEW.debe, haber = haber + NEW.haber WHERE idc_coste = ccostr.idc_coste AND idcuenta = ctar.idcuenta;
	    SELECT INTO ctar * FROM cuenta WHERE idcuenta = ctar.padre;
	END LOOP;
	SELECT INTO ccostr * FROM c_coste WHERE idc_coste = ccostr.padre;
    END LOOP;
    RETURN NEW;
END;
$$
    LANGUAGE plpgsql;


ALTER FUNCTION public.aumenta_valor() OWNER TO postgres;

--
-- Name: bcontrapartida(integer); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION bcontrapartida(integer) RETURNS integer
    AS $_$
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
--  RAISE NOTICE 'Empezamos';
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
        FOR cont IN SELECT idapunte, debe, haber FROM apunte WHERE idasiento = apt.idasiento ORDER BY orden LOOP
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
$_$
    LANGUAGE plpgsql;


ALTER FUNCTION public.bcontrapartida(integer) OWNER TO postgres;

--
-- Name: bcontrapartidaborr(integer); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION bcontrapartidaborr(integer) RETURNS integer
    AS $_$
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
--  RAISE NOTICE 'Empezamos';
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
        FOR cont IN SELECT * FROM borrador WHERE idasiento = apt.idasiento ORDER BY orden LOOP
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
END;$_$
    LANGUAGE plpgsql;


ALTER FUNCTION public.bcontrapartidaborr(integer) OWNER TO postgres;

--
-- Name: borracanal(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION borracanal() RETURNS "trigger"
    AS $$
BEGIN
    DELETE FROM acumulado_canal WHERE idcanal = OLD.idcanal;
    RETURN OLD;
END;
$$
    LANGUAGE plpgsql;


ALTER FUNCTION public.borracanal() OWNER TO postgres;

--
-- Name: borraccoste(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION borraccoste() RETURNS "trigger"
    AS $$
BEGIN
    DELETE FROM acumulado_c_coste WHERE idc_coste = OLD.idc_coste;
    RETURN OLD;
END;
$$
    LANGUAGE plpgsql;


ALTER FUNCTION public.borraccoste() OWNER TO postgres;

--
-- Name: borracuenta(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION borracuenta() RETURNS "trigger"
    AS $$
DECLARE
    mrecord RECORD;
BEGIN
    FOR mrecord IN SELECT * FROM canal LOOP
	DELETE FROM acumulado_canal WHERE idcuenta = OLD.idcuenta;
    END LOOP;
    FOR mrecord IN SELECT * FROM c_coste LOOP
	DELETE FROM acumulado_c_coste WHERE idcuenta = OLD.idcuenta;
    END LOOP;
    RETURN OLD;
END;
$$
    LANGUAGE plpgsql;


ALTER FUNCTION public.borracuenta() OWNER TO postgres;

--
-- Name: cambiaasiento(integer, integer); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION cambiaasiento(integer, integer) RETURNS integer
    AS $_$
DECLARE
    idinicial ALIAS FOR $1;
    idfinal ALIAS FOR $2;
    bs RECORD;
BEGIN
    -- Esta funcion cambia un asiento de sitio, el asiento inicial y lo pone en el asiento final
    -- El problema es que si el asiento final debe estar vacio.
    SELECT INTO bs * FROM asiento WHERE idasiento = idinicial;
    IF FOUND THEN
        INSERT INTO asiento (idasiento, fecha, descripcion, comentariosasiento) VALUES (idfinal, bs.fecha, bs.descripcion, bs.comentariosasiento);
        UPDATE borrador SET idasiento = idfinal WHERE idasiento = idinicial;
        UPDATE apunte SET idasiento = idfinal WHERE idasiento = idinicial;
        DELETE FROM asiento WHERE idasiento = idinicial;
    END IF;
    RETURN 0;
END;
$_$
    LANGUAGE plpgsql;


ALTER FUNCTION public.cambiaasiento(integer, integer) OWNER TO postgres;

--
-- Name: cambiadoiva(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION cambiadoiva() RETURNS "trigger"
    AS $$
DECLARE
    mrecord RECORD;
BEGIN
    FOR mrecord IN SELECT SUM(baseiva) AS suma, SUM(ivaiva) AS sumaiva FROM iva WHERE iva.idregistroiva = NEW.idregistroiva LOOP
    	UPDATE registroiva SET baseimp = mrecord.suma, iva = mrecord.sumaiva WHERE idregistroiva = NEW.idregistroiva;
    END LOOP;
    RETURN NEW;
END;
$$
    LANGUAGE plpgsql;


ALTER FUNCTION public.cambiadoiva() OWNER TO postgres;

--
-- Name: cambiadoivad(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION cambiadoivad() RETURNS "trigger"
    AS $$
DECLARE
    mrecord RECORD;
BEGIN
    FOR mrecord IN SELECT SUM(baseiva) AS suma, SUM(ivaiva) AS sumaiva FROM iva WHERE iva.idregistroiva = OLD.idregistroiva LOOP
    	UPDATE registroiva SET baseimp = mrecord.suma, iva = mrecord.sumaiva WHERE idregistroiva = OLD.idregistroiva;
    END LOOP;
    RETURN OLD;
END;
$$
    LANGUAGE plpgsql;


ALTER FUNCTION public.cambiadoivad() OWNER TO postgres;

--
-- Name: ccontrapartida(integer); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION ccontrapartida(integer) RETURNS integer
    AS $_$
DECLARE 
    midapunte ALIAS FOR $1;
    apt RECORD;
    aptasien RECORD;
BEGIN
    SELECT INTO apt * FROM apunte WHERE idapunte= midapunte;
    IF apt.contrapartida ISNULL THEN
	IF apt.debe = 0 THEN
	    SELECT INTO aptasien * FROM apunte WHERE idasiento = apt.idasiento AND debe <> 0 ORDER BY debe DESC;
	    IF FOUND THEN
		RETURN aptasien.idcuenta;
	    END IF;
	ELSE
	    SELECT INTO aptasien * FROM apunte WHERE idasiento = apt.idasiento AND haber <> 0 ORDER BY haber DESC;
	    IF FOUND THEN
		RETURN aptasien.idcuenta;
	    END IF;
	END IF;
    ELSE
	RETURN apt.contrapartida;
    END IF;
    RETURN NULL;
END;
$_$
    LANGUAGE plpgsql;


ALTER FUNCTION public.ccontrapartida(integer) OWNER TO postgres;

--
-- Name: cierraasiento(integer); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION cierraasiento(integer) RETURNS integer
    AS $_$
DECLARE
    id_asiento ALIAS FOR $1;
    mrecord RECORD;
BEGIN
    DELETE FROM apunte WHERE idasiento=id_asiento;
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
$_$
    LANGUAGE plpgsql;


ALTER FUNCTION public.cierraasiento(integer) OWNER TO postgres;

--
-- Name: contraasiento(integer); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION contraasiento(integer) RETURNS numeric
    AS $_$
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
    FOR cont IN SELECT idcuenta,idapunte, debe, haber, orden FROM apunte WHERE idasiento = midasiento ORDER BY orden LOOP
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
            UPDATE apunte SET contrapartida= ctahaber WHERE haber=0 AND idasiento = midasiento AND orden <= cont.orden AND contrapartida ISNULL;
            UPDATE apunte SET contrapartida= ctadebe WHERE debe=0 AND idasiento = midasiento AND orden <= cont.orden AND contrapartida ISNULL;
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
   $_$
    LANGUAGE plpgsql;


ALTER FUNCTION public.contraasiento(integer) OWNER TO postgres;

--
-- Name: creacanal(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION creacanal() RETURNS "trigger"
    AS $$
DECLARE
    mrecord RECORD;
BEGIN
    FOR mrecord IN SELECT * FROM cuenta LOOP
	INSERT INTO acumulado_canal (idcuenta, idcanal, debe, haber) VALUES(mrecord.idcuenta, NEW.idcanal, 0, 0);
    END LOOP;
    RETURN NEW;
END;
$$
    LANGUAGE plpgsql;


ALTER FUNCTION public.creacanal() OWNER TO postgres;

--
-- Name: creaccoste(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION creaccoste() RETURNS "trigger"
    AS $$
DECLARE
    mrecord RECORD;
BEGIN
    FOR mrecord IN SELECT * FROM cuenta LOOP
	INSERT INTO acumulado_c_coste (idcuenta, idc_coste, debe, haber) VALUES(mrecord.idcuenta, NEW.idc_coste, 0, 0);
    END LOOP;
    RETURN NEW;
END;
$$
    LANGUAGE plpgsql;


ALTER FUNCTION public.creaccoste() OWNER TO postgres;

--
-- Name: creacuenta(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION creacuenta() RETURNS "trigger"
    AS $$
DECLARE
    mrecord RECORD;
BEGIN
    FOR mrecord IN SELECT * FROM canal LOOP
	INSERT INTO acumulado_canal (idcuenta, idcanal,debe,haber) VALUES(NEW.idcuenta, mrecord.idcanal,0,0);
    END LOOP;
    FOR mrecord IN SELECT * FROM c_coste LOOP
	INSERT INTO acumulado_c_coste (idcuenta, idc_coste,debe,haber) VALUES(NEW.idcuenta, mrecord.idc_coste,0,0);
    END LOOP;
    RETURN NEW;
END;
$$
    LANGUAGE plpgsql;


ALTER FUNCTION public.creacuenta() OWNER TO postgres;

--
-- Name: debempatrimonial(integer, date, date); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION debempatrimonial(integer, date, date) RETURNS numeric
    AS $_$
DECLARE
    identmpatrimonial ALIAS FOR $1;
    fechain ALIAS FOR $2;
    fechafin ALIAS FOR $3;
    aptsum RECORD;
    aptrest RECORD;
    smpatrimonialsum RECORD;
    smpatrimonialres RECORD;
    debe NUMERIC(12,2);
BEGIN
    -- Esta funcion calcula el debe de una masa patrimonial entre dos fechas.
    debe := 0;
    FOR aptsum IN SELECT * FROM cuenta WHERE idcuenta IN (SELECT idcuenta FROM compmasap WHERE masaperteneciente = identmpatrimonial AND idcuenta IS NOT NULL  AND signo = true) LOOP
        debe := debe + debetotal(aptsum.idcuenta, fechain, fechafin);
    END LOOP;
    FOR aptrest IN SELECT * FROM cuenta WHERE idcuenta IN (SELECT idcuenta FROM compmasap WHERE masaperteneciente = identmpatrimonial AND idcuenta IS NOT NULL  AND signo = false) LOOP
        debe := debe - debetotal(aptsum.idcuenta, fechain, fechafin);
    END LOOP;
    FOR smpatrimonialsum IN SELECT * FROM mpatrimonial WHERE idmpatrimonial IN (SELECT idmpatrimonial FROM compmasap WHERE masaperteneciente = identmpatrimonial AND idmpatrimonial IS NOT NULL AND signo = true) LOOP
        debe := debe + debempatrimonial (smpatrimonialsum.idmpatrimonial, fechain, fechafin);
    END LOOP;
    FOR smpatrimonialsum IN SELECT * FROM mpatrimonial WHERE idmpatrimonial IN (SELECT idmpatrimonial FROM compmasap WHERE masaperteneciente = identmpatrimonial AND idmpatrimonial IS NOT NULL AND signo = false) LOOP
        debe := debe - debepatrimonial (smpatrimonialsum.idmpatrimonial, fechain, fechafin);
    END LOOP;
    RETURN debe;
END;
$_$
    LANGUAGE plpgsql;


ALTER FUNCTION public.debempatrimonial(integer, date, date) OWNER TO postgres;

--
-- Name: debetotal(integer, date, date); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION debetotal(integer, date, date) RETURNS numeric
    AS $_$
DECLARE
    mcuenta ALIAS FOR $1;
    fechain ALIAS FOR $2;
    fechafin ALIAS FOR $3;
    apt RECORD;
    apt1 RECORD;
    apt2 RECORD;
    cta RECORD;
    debet NUMERIC(12, 2);
BEGIN
--  SELECT INTO apt * FROM apunte WHERE cuenta = apunte.idcuenta;
--  RAISE NOTICE ' Calculando debetotal de % entre % y % ', cuenta, fechain, fechafin;
    debet := 0;
    SELECT INTO apt sum(debe) AS mdebe FROM apunte WHERE apunte.idcuenta = mcuenta AND fecha <= "fechafin" AND fecha >= "fechain";
    IF (apt.mdebe ISNULL) THEN
	debet:= 0;
    ELSE
	debet := apt.mdebe;
    END IF;
--  RAISE NOTICE ' debe total % valor adquirido %', cuenta, debet;
    FOR apt1 IN SELECT idcuenta FROM cuenta WHERE padre = mcuenta LOOP
	debet := debet + debetotal(apt1.idcuenta, fechain, fechafin);
    END LOOP;
    RETURN debet;
END;
$_$
    LANGUAGE plpgsql;


ALTER FUNCTION public.debetotal(integer, date, date) OWNER TO postgres;

--
-- Name: debetotal1(integer); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION debetotal1(integer) RETURNS numeric
    AS $_$
DECLARE
    mcuenta ALIAS FOR $1;
    apt RECORD;
    apt1 RECORD;
    apt2 RECORD;
    cta RECORD;
    debet NUMERIC(12,2);
BEGIN
--  SELECT INTO apt * FROM apunte WHERE id_cuenta(codcuenta) = apunte.idcuenta;
    debet := 0;
    SELECT INTO apt sum(debe) AS mdebe FROM apunte WHERE apunte.idcuenta = mcuenta;
    IF (apt.mdebe ISNULL) THEN
	debet:= 0;
    ELSE
	debet := apt.mdebe;
    END IF;
--  RAISE NOTICE ' debe total1 % valor adquirido %', codcuenta, debet;
    FOR apt1 IN SELECT idcuenta FROM cuenta WHERE padre = mcuenta LOOP
	debet := debet + debetotal1(apt1.idcuenta);
    END LOOP;
    RETURN debet;
END;
$_$
    LANGUAGE plpgsql;


ALTER FUNCTION public.debetotal1(integer) OWNER TO postgres;

--
-- Name: deletetipoiva(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION deletetipoiva() RETURNS "trigger"
    AS $$
DECLARE
    mrecord RECORD;
BEGIN
    DELETE FROM iva WHERE idtipoiva = OLD.idtipoiva;
    RETURN OLD;
END;
$$
    LANGUAGE plpgsql;


ALTER FUNCTION public.deletetipoiva() OWNER TO postgres;

--
-- Name: disminuye_valor(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION disminuye_valor() RETURNS "trigger"
    AS $$
DECLARE
    cta int4;
    ccost int4;
    ctar RECORD;
    ccostr RECORD;
BEGIN
    -- RAISE NOTICE 'disminuye_valor: debe antiguo %, debe nuevo %', OLD.debe, NEW.debe;
    UPDATE cuenta SET debe = debe - OLD.debe, haber = haber - OLD.haber WHERE idcuenta = OLD.idcuenta;
    UPDATE c_coste SET debe = debe - OLD.debe, haber = haber - OLD.haber WHERE idc_coste = OLD.idc_coste;
    IF OLD.idcuenta IS NOT NULL THEN
	UPDATE acumulado_canal SET debe= debe - OLD.debe, haber =haber - OLD.haber WHERE idcuenta = OLD.idcuenta AND idcanal = OLD.idcanal;
    END IF;
    cta := OLD.idcuenta;
    ccost := OLD.idc_coste;
    -- RAISE NOTICE ' Se ha lanzado la funcion disminuye_valor()';
    SELECT INTO ccostr * FROM c_coste WHERE idc_coste = ccost;
    WHILE FOUND LOOP
	SELECT INTO ctar * FROM cuenta WHERE idcuenta = cta;
	WHILE FOUND LOOP
	    -- RAISE NOTICE ' Cuenta % Centro Coste %', ctar.idcuenta, ccostr.idc_coste;
	    UPDATE acumulado_c_coste SET debe = debe - OLD.debe, haber = haber -OLD.haber WHERE idc_coste = ccostr.idc_coste AND idcuenta = ctar.idcuenta;
	    SELECT INTO ctar * FROM cuenta WHERE idcuenta = ctar.padre;
        END LOOP;
        SELECT INTO ccostr * FROM c_coste WHERE idc_coste = ccostr.padre;
    END LOOP;
    -- RAISE NOTICE ' disminuye_valor: Finaliza el algoritmo. ';
    RETURN NEW;
END;
$$
    LANGUAGE plpgsql;


ALTER FUNCTION public.disminuye_valor() OWNER TO postgres;

--
-- Name: disminuye_valor1(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION disminuye_valor1() RETURNS "trigger"
    AS $$
DECLARE
    cta int4;
    ccost int4;
    ctar RECORD;
    ccostr RECORD;
BEGIN
    UPDATE cuenta SET debe = debe - OLD.debe, haber = haber - OLD.haber WHERE idcuenta = OLD.idcuenta;
    UPDATE c_coste SET debe = debe - OLD.debe, haber = haber - OLD.haber WHERE idc_coste = OLD.idc_coste;
    IF OLD.idcuenta IS NOT NULL THEN
	UPDATE acumulado_canal SET debe= debe - OLD.debe, haber =haber - OLD.haber WHERE idcuenta = OLD.idcuenta AND idcanal = OLD.idcanal;
    END IF;
    cta := OLD.idcuenta;
    ccost := OLD.idc_coste;
    -- RAISE NOTICE ' Se ha lanzado la funcion disminuye_valor()';
    SELECT INTO ccostr * FROM c_coste WHERE idc_coste = ccost;
    WHILE FOUND LOOP
	SELECT INTO ctar * FROM cuenta WHERE idcuenta = cta;
	WHILE FOUND LOOP
	    -- RAISE NOTICE ' Cuenta % Centro Coste %', ctar.idcuenta, ccostr.idc_coste;
	    UPDATE acumulado_c_coste SET debe = debe - OLD.debe, haber = haber -OLD.haber WHERE idc_coste = ccostr.idc_coste AND idcuenta = ctar.idcuenta;
	    SELECT INTO ctar * FROM cuenta WHERE idcuenta = ctar.padre;
	END LOOP;
	SELECT INTO ccostr * FROM c_coste WHERE idc_coste = ccostr.padre;
    END LOOP;
    -- RAISE NOTICE ' disminuye_valor: Finaliza el algoritmo. ';
    RETURN OLD;
END;
$$
    LANGUAGE plpgsql;


ALTER FUNCTION public.disminuye_valor1() OWNER TO postgres;

--
-- Name: habermpatrimonial(integer, date, date); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION habermpatrimonial(integer, date, date) RETURNS numeric
    AS $_$
DECLARE
    identmpatrimonial ALIAS FOR $1;
    fechain ALIAS FOR $2;
    fechafin ALIAS FOR $3;
    aptsum RECORD;
    aptrest RECORD;
    smpatrimonialsum RECORD;
    smpatrimonialres RECORD;
    haber NUMERIC(12, 2);
BEGIN
    -- Esta funcion calcula el saldo de una masa patrimonial entre dos fechas.
    haber := 0;
    FOR aptsum IN SELECT * FROM cuenta WHERE idcuenta IN (SELECT idcuenta FROM compmasap WHERE masaperteneciente = identmpatrimonial AND idcuenta IS NOT NULL  AND signo = true) LOOP
        haber := haber + habertotal(aptsum.idcuenta, fechain, fechafin);
    END LOOP;
    FOR aptrest IN SELECT * FROM cuenta WHERE idcuenta IN (SELECT idcuenta FROM compmasap WHERE masaperteneciente = identmpatrimonial AND idcuenta IS NOT NULL  AND signo = false) LOOP
        haber := haber - habertotal(aptsum.idcuenta, fechain, fechafin);
    END LOOP;
    FOR smpatrimonialsum IN SELECT * FROM mpatrimonial WHERE idmpatrimonial IN (SELECT idmpatrimonial FROM compmasap WHERE masaperteneciente = identmpatrimonial AND idmpatrimonial IS NOT NULL AND signo = true) LOOP
        haber := haber + habermpatrimonial (smpatrimonialsum.idmpatrimonial, fechain, fechafin);
    END LOOP;
    FOR smpatrimonialsum IN SELECT * FROM mpatrimonial WHERE idmpatrimonial IN (SELECT idmpatrimonial FROM compmasap WHERE masaperteneciente = identmpatrimonial AND idmpatrimonial IS NOT NULL AND signo = false) LOOP
        haber := haber - habermpatrimonial (smpatrimonialsum.idmpatrimonial, fechain, fechafin);
    END LOOP;
    RETURN haber;
END;
$_$
    LANGUAGE plpgsql;


ALTER FUNCTION public.habermpatrimonial(integer, date, date) OWNER TO postgres;

--
-- Name: habertotal(integer, date, date); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION habertotal(integer, date, date) RETURNS numeric
    AS $_$
DECLARE
    mcuenta ALIAS FOR $1;
    fechain ALIAS FOR $2;
    fechafin ALIAS FOR $3;
    apt RECORD;
    apt1 RECORD;
    apt2 RECORD;
    cta RECORD;
    habert NUMERIC(12,2);
BEGIN
--  SELECT INTO apt * FROM apunte WHERE id_cuenta(codcuenta) = apunte.idcuenta;
    habert := 0;
    SELECT INTO apt sum(haber) AS thaber FROM apunte WHERE apunte.idcuenta = mcuenta AND fecha <= "fechafin" AND fecha >= "fechain";
    IF (apt.thaber ISNULL) THEN
        habert := 0;
    ELSE
        habert := apt.thaber;
    END IF;
--  RAISE NOTICE ' haber total % valor adquirido %', codcuenta, habert;
    FOR apt1 IN SELECT idcuenta FROM cuenta WHERE padre = mcuenta LOOP
	habert := habert + habertotal(apt1.idcuenta,fechain, fechafin);
    END LOOP;
    RETURN habert;
END;
$_$
    LANGUAGE plpgsql;


ALTER FUNCTION public.habertotal(integer, date, date) OWNER TO postgres;

--
-- Name: habertotal1(integer); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION habertotal1(integer) RETURNS numeric
    AS $_$
DECLARE
    mcuenta ALIAS FOR $1;
    apt RECORD;
    apt1 RECORD;
    apt2 RECORD;
    cta RECORD;
    habert NUMERIC(12, 2);
BEGIN
--  SELECT INTO apt * FROM apunte WHERE id_cuenta(codcuenta) = apunte.idcuenta;
    habert := 0;
    SELECT INTO apt sum(haber) AS thaber FROM apunte WHERE apunte.idcuenta = mcuenta;
    IF (apt.thaber ISNULL) THEN
	habert := 0;
    ELSE
        habert := apt.thaber;
    END IF;
--  RAISE NOTICE ' haber total % valor adquirido %', codcuenta, habert;
    FOR apt1 IN SELECT idcuenta FROM cuenta WHERE padre = mcuenta LOOP
	habert := habert + habertotal1(apt1.idcuenta);
    END LOOP;
    RETURN habert;
END;
$_$
    LANGUAGE plpgsql;


ALTER FUNCTION public.habertotal1(integer) OWNER TO postgres;

--
-- Name: id_cuenta(character varying); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION id_cuenta(character varying) RETURNS integer
    AS $_$
DECLARE
   codcuenta ALIAS FOR $1;
   cta RECORD;
BEGIN
   SELECT INTO cta idcuenta FROM cuenta WHERE codigo = "codcuenta";
   IF FOUND THEN
        RETURN cta.idcuenta;
   ELSE
        RETURN 0;
   END IF;
END;
$_$
    LANGUAGE plpgsql;


ALTER FUNCTION public.id_cuenta(character varying) OWNER TO postgres;

--
-- Name: inserttipoiva(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION inserttipoiva() RETURNS "trigger"
    AS $$
DECLARE
    mrecord RECORD;
BEGIN
    FOR mrecord IN SELECT * FROM registroiva LOOP
	INSERT INTO iva (idregistroiva, idtipoiva,baseiva) VALUES(mrecord.idregistroiva, NEW.idtipoiva, 0);
    END LOOP;
    RETURN NEW;
END;
$$
    LANGUAGE plpgsql;


ALTER FUNCTION public.inserttipoiva() OWNER TO postgres;

--
-- Name: nivel(character varying); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION nivel(character varying) RETURNS integer
    AS $_$
DECLARE
    codcuenta ALIAS FOR $1;
BEGIN
    RETURN LENGTH(codcuenta);
END;
$_$
    LANGUAGE plpgsql;


ALTER FUNCTION public.nivel(character varying) OWNER TO postgres;

--
-- Name: plpgsql_call_handler(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION plpgsql_call_handler() RETURNS language_handler
    AS '$libdir/plpgsql', 'plpgsql_call_handler'
    LANGUAGE c;


ALTER FUNCTION public.plpgsql_call_handler() OWNER TO postgres;

--
-- Name: propagaacumuladoccoste(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION propagaacumuladoccoste() RETURNS "trigger"
    AS $$
DECLARE
    incdebe numeric(12, 2);
    inchaber numeric(12, 2);
BEGIN
    incdebe = NEW.debe - OLD.debe;
    inchaber = NEW.haber - OLD.haber;
    IF incdebe <> 0 OR inchaber <> 0 THEN
	UPDATE c_coste SET debe = debe + incdebe, haber = haber + inchaber WHERE idc_coste = OLD.padre;
    END IF;
    RETURN NEW;
END;
$$
    LANGUAGE plpgsql;


ALTER FUNCTION public.propagaacumuladoccoste() OWNER TO postgres;

--
-- Name: propagaacumuladocuenta(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION propagaacumuladocuenta() RETURNS "trigger"
    AS $$
DECLARE
    incdebe numeric(12, 2);
    inchaber numeric(12, 2);
BEGIN
    incdebe = NEW.debe - OLD.debe;
    inchaber = NEW.haber - OLD.haber;
    -- RAISE NOTICE 'propagaacumuladocuenta %', NEW.codigo;
    IF incdebe <> 0 OR inchaber <> 0 THEN
	UPDATE cuenta SET debe = debe + incdebe, haber = haber + inchaber WHERE idcuenta = NEW.padre;
    END IF;
    RETURN NEW;
END;
$$
    LANGUAGE plpgsql;


ALTER FUNCTION public.propagaacumuladocuenta() OWNER TO postgres;

--
-- Name: propagaccosteborrador(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION propagaccosteborrador() RETURNS "trigger"
    AS $$
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
$$
    LANGUAGE plpgsql;


ALTER FUNCTION public.propagaccosteborrador() OWNER TO postgres;

--
-- Name: recalculasaldos(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION recalculasaldos() RETURNS numeric
    AS $$
DECLARE
    cta RECORD;
BEGIN
    -- El orden si es importante porque aparecen problemas con el trigger que se dispara en los updates.
    FOR cta IN SELECT * FROM cuenta WHERE padre IS NOT NULL ORDER BY padre LOOP
        UPDATE cuenta SET debe = debetotal1(cta.idcuenta), haber = habertotal1(cta.idcuenta) WHERE idcuenta = cta.idcuenta;
    END LOOP;
    FOR cta IN SELECT * FROM cuenta WHERE padre IS NULL ORDER BY padre LOOP
        UPDATE cuenta SET debe = debetotal1(cta.idcuenta), haber = habertotal1(cta.idcuenta) WHERE idcuenta = cta.idcuenta;
    END LOOP;
    RETURN 0;
END;
$$
    LANGUAGE plpgsql;


ALTER FUNCTION public.recalculasaldos() OWNER TO postgres;

--
-- Name: recalculasaldos2(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION recalculasaldos2() RETURNS integer
    AS $$
DECLARE
    niveles RECORD;
    cta RECORD;
BEGIN
    -- De momento, la haremos funcionar para un sistema de cuentas xxxxyyy.
    SELECT INTO niveles strpos(valor, 'y') - 1 AS numx FROM configuracion WHERE nombre = 'CodCuenta';
    IF niveles.numx <> 4 THEN
	RAISE NOTICE 'Lo siento, pero esta funcion solo funciona de momento con 4 niveles de cuentas.';
	RETURN -1;
    END IF;
    -- Creamos la tabla con el arbol de cuentas y sus valores (se ha considerado hasta nivel 4).
    CREATE TEMPORARY TABLE temp4 AS (SELECT n1.codigo AS cod1, n1.debe AS debe1, n1.haber AS haber1, n2.codigo AS cod2, n2.debe AS debe2, n2.haber AS haber2, n3.codigo AS cod3, n3.debe AS debe3, n3.haber AS haber3, n4.codigo AS cod4, n4.debe AS debe4, n4.haber AS haber4 FROM (SELECT idcuenta, codigo, debe, haber FROM cuenta WHERE padre IS NULL) AS n1 INNER JOIN (SELECT idcuenta, padre, codigo, debe, haber FROM cuenta) AS n2 ON n1.idcuenta=n2.padre INNER JOIN (SELECT idcuenta, padre, codigo, debe, haber FROM cuenta) AS n3 ON n2.idcuenta = n3.padre LEFT JOIN (SELECT padre, codigo, debe, haber FROM cuenta) AS n4 ON n3.idcuenta = n4.padre);
   
    -- Ahora iremos actualizando las ramas desde las hojas hasta las raizes.
    -- Primero, tendremos en cuenta aquellas cuentas que estan en un nivel 4, calculamos la suma de su nivel y
    -- subimos el dato al nivel 3.
    CREATE TEMPORARY TABLE temp3 AS (SELECT cod1, cod2, cod3, sum(debe4) AS debe3, sum(haber4) AS haber3 FROM temp4 WHERE debe4 IS NOT NULL group by cod1, cod2, cod3 order by cod3);

    -- Seguidamente, anyadimos las hojas del nivel 3 que descartamos en la accion anterior porque no tenian cuentas
    -- hijas en el nivel 4.
    INSERT INTO temp3 SELECT cod1, cod2, cod3, debe3, haber3 FROM temp4 WHERE debe4 IS NULL;

    -- Se calculan las sumas del nivel 3 y les pasamos el dato a las cuentas padre del nivel 2.
    CREATE TEMPORARY TABLE temp2 AS (SELECT cod1, cod2, sum(debe3) AS debe2, sum(haber3) AS haber2 FROM temp3 group by cod1, cod2 order by cod2);

    -- Y finalmente, hacemos lo mismo con el nivel 2 y subimos las sumas al nivel 1
    CREATE TEMPORARY TABLE temp1 AS (SELECT cod1,sum(debe2) AS debe1, sum(haber2) AS haber1 FROM temp2 group by cod1 order by cod1);

    -- Ahora vamos a eliminar de las tablas aquellas cuentas que no sera necesario actualizar por ya estar con los
    -- valores correctos.
    CREATE TEMPORARY TABLE nivel1 AS (SELECT t1.cod1, t1.debe1, t1.haber1 FROM (SELECT * FROM temp1) AS t1 INNER JOIN (SELECT codigo, debe, haber FROM cuenta) AS t2 ON t1.cod1 = t2.codigo WHERE t1.debe1 <> t2.debe OR t1.haber1 <> t2.haber);
    CREATE TEMPORARY TABLE nivel2 AS (SELECT t1.cod2, t1.debe2, t1.haber2 FROM (SELECT * FROM temp2) AS t1 INNER JOIN (SELECT codigo, debe, haber FROM cuenta) AS t2 ON t1.cod2 = t2.codigo WHERE t1.debe2 <> t2.debe OR t1.haber2 <> t2.haber);
    CREATE TEMPORARY TABLE nivel3 AS (SELECT t1.cod3, t1.debe3, t1.haber3 FROM (SELECT * FROM temp3) AS t1 INNER JOIN (SELECT codigo, debe, haber FROM cuenta) AS t2 ON t1.cod3 = t2.codigo WHERE t1.debe3 <> t2.debe OR t1.haber3 <> t2.haber);
   
    -- Como colofon, hay que introducir los valores actualizados en las cuentas padre.
    FOR cta IN SELECT * FROM nivel1 ORDER BY cod1 LOOP
	RAISE NOTICE 'Cuenta %	-> debe: %	haber: %', cta.cod1, cta.debe1, cta.haber1;
	UPDATE cuenta SET debe = cta.debe1, haber = cta.haber1 WHERE idcuenta IN (SELECT idcuenta FROM cuenta WHERE codigo = cta.cod1);
	RAISE NOTICE 'Cuenta % actualizada', cta.cod1;
    END LOOP;
    FOR cta IN SELECT * FROM nivel2 ORDER BY cod2 LOOP
        RAISE NOTICE 'Cuenta %	-> debe: %     haber: %', cta.cod2, cta.debe2, cta.haber2;
        UPDATE cuenta SET debe = cta.debe2, haber = cta.haber2 WHERE idcuenta IN (SELECT idcuenta FROM cuenta WHERE codigo = cta.cod2);
        RAISE NOTICE 'Cuenta % actualizada', cta.cod2;
    END LOOP;
    FOR cta IN SELECT * FROM nivel3 WHERE length(cod3)=niveles.numx ORDER BY cod3 LOOP
	RAISE NOTICE 'Cuenta %	-> debe: %     haber: %', cta.cod3, cta.debe3, cta.haber3;
	UPDATE cuenta SET debe = cta.debe3, haber = cta.haber3 WHERE idcuenta IN (SELECT idcuenta FROM cuenta WHERE codigo = cta.cod3);
	RAISE NOTICE 'Cuenta % actualizada', cta.cod3;
    END LOOP;
    RETURN 0;
END;
$$
    LANGUAGE plpgsql;


ALTER FUNCTION public.recalculasaldos2() OWNER TO postgres;

--
-- Name: reordenaasientos(integer); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION reordenaasientos(integer) RETURNS integer
    AS $_$
DECLARE
    ejercicio ALIAS FOR $1;
    bs RECORD;
    cont integer;
BEGIN
    cont := 1;
    FOR bs IN SELECT * FROM asiento WHERE EXTRACT(YEAR FROM fecha) = ejercicio ORDER BY fecha, ordenasiento LOOP
        IF (cont <> bs.ordenasiento) THEN
            UPDATE asiento SET ordenasiento = cont WHERE idasiento = bs.idasiento;
        END IF;
        cont := cont + 1;
    END LOOP;
    RETURN 0;
END;
$_$
    LANGUAGE plpgsql;


ALTER FUNCTION public.reordenaasientos(integer) OWNER TO postgres;

--
-- Name: reordenaasientosall(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION reordenaasientosall() RETURNS integer
    AS $$
DECLARE
    bs RECORD;
    ejercicio integer;
BEGIN
    FOR bs IN SELECT DISTINCT EXTRACT (YEAR FROM FECHA) AS ano FROM asiento ORDER BY ano LOOP
        ejercicio = bs.ano;
        PERFORM reordenaasientos(ejercicio);
    END LOOP;
    RETURN 0;
END;
$$
    LANGUAGE plpgsql;


ALTER FUNCTION public.reordenaasientosall() OWNER TO postgres;

--
-- Name: restriccionesasiento(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION restriccionesasiento() RETURNS "trigger"
    AS $$
DECLARE
    ej RECORD;
BEGIN
    SELECT INTO ej * FROM ejercicios WHERE ejercicio = EXTRACT (YEAR FROM NEW.fecha) AND periodo = 0;
    IF FOUND THEN
    	IF ej.bloqueado = TRUE THEN
	    RAISE EXCEPTION 'Periodo bloqueado %', NEW.fecha;
	END IF;
    ELSE
	RAISE EXCEPTION 'Ejercicio Inexistente %', ej.ejercicio;
    END IF;
    SELECT INTO ej * FROM ejercicios WHERE ejercicio = EXTRACT (YEAR FROM NEW.fecha) AND periodo = EXTRACT (MONTH FROM NEW.fecha);
    IF ej.bloqueado = TRUE THEN
	RAISE EXCEPTION 'Periodo bloqueado %', NEW.fecha;
    END IF;
    IF NEW.ordenasiento ISNULL OR NEW.ordenasiento = 0 THEN
	SELECT INTO ej max(ordenasiento) + 1 AS max, count(idasiento) AS cuenta FROM asiento WHERE EXTRACT (YEAR FROM NEW.fecha) = EXTRACT(YEAR FROM fecha);
	IF ej.cuenta > 0 THEN
	    NEW.ordenasiento = ej.max;
	ELSE
	    NEW.ordenasiento = 1;
	END IF;
    END IF;
    RETURN NEW;
END;
$$
    LANGUAGE plpgsql;


ALTER FUNCTION public.restriccionesasiento() OWNER TO postgres;

--
-- Name: restriccionesborradocuenta(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION restriccionesborradocuenta() RETURNS "trigger"
    AS $$
DECLARE
    cta RECORD;
BEGIN
    SELECT INTO cta * FROM cuenta WHERE padre = OLD.idcuenta;
    IF FOUND THEN
	RAISE EXCEPTION ' La cuenta tiene hijos. ';
    END IF;
    RETURN OLD;
END;
$$
    LANGUAGE plpgsql;


ALTER FUNCTION public.restriccionesborradocuenta() OWNER TO postgres;

--
-- Name: restriccionesborrador(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION restriccionesborrador() RETURNS "trigger"
    AS $$
DECLARE
    cta RECORD;
    ej RECORD;
    ord RECORD;
BEGIN
    SELECT INTO cta * FROM cuenta WHERE idcuenta = NEW.idcuenta;
    IF FOUND THEN
        IF cta.bloqueada THEN
	    RAISE EXCEPTION 'Cuenta bloqueada, no se puede utilizar esta cuenta %', NEW.idcuenta;
        END IF;
        IF cta.nodebe THEN
            IF NEW.debe <> 0 THEN
                RAISE EXCEPTION 'Cuenta bloqueada por debe, solo permite haber %', NEW.idcuenta;
            END IF;
        END IF;
        IF cta.nohaber THEN
            IF NEW.haber <> 0 THEN
                RAISE EXCEPTION 'Cuenta bloqueada por haber, solo permite debe %', NEW.idcuenta;
            END IF;
        END IF;
    ELSE
	RAISE EXCEPTION 'Cuenta inexistente %', NEW.idcuenta;
    END IF;
	
    SELECT INTO ej * FROM ejercicios WHERE ejercicio = EXTRACT (YEAR FROM NEW.fecha) AND periodo = 0;
    IF FOUND THEN
	IF ej.bloqueado = TRUE THEN
    	    RAISE EXCEPTION 'Periodo bloqueado %', NEW.fecha;
	END IF;
    ELSE
	RAISE EXCEPTION 'Ejercicio Inexistente %', ej.ejercicio;
    END IF;

    SELECT INTO ej * FROM ejercicios WHERE ejercicio = EXTRACT (YEAR FROM NEW.fecha) AND periodo = EXTRACT (MONTH FROM NEW.fecha);
    IF ej.bloqueado = TRUE THEN
	RAISE EXCEPTION 'Periodo bloqueado %', NEW.fecha;
    END IF;
    RETURN NEW;
END;
$$
    LANGUAGE plpgsql;


ALTER FUNCTION public.restriccionesborrador() OWNER TO postgres;

--
-- Name: restriccionescuenta(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION restriccionescuenta() RETURNS "trigger"
    AS $$
DECLARE
    cta RECORD;
BEGIN
    IF NEW.padre <> 0 THEN
	SELECT INTO cta * FROM cuenta WHERE idcuenta= NEW.padre;
	IF NOT FOUND THEN
	    RAISE EXCEPTION ' La cuenta padre no existe. ';
	END IF;
    END IF;
    IF NEW.codigo = '' THEN
	RAISE EXCEPTION ' No se puede dejar el codigo de cuenta vacio ';
    END IF;
    IF NEW.descripcion = '' THEN
	RAISE EXCEPTION ' Nombre de cuenta vacio ';
    END IF;
    IF NEW.nombreent_cuenta = '' THEN
	NEW.nombreent_cuenta = NEW.descripcion;
    END IF;
    RETURN NEW;
END;
$$
    LANGUAGE plpgsql;


ALTER FUNCTION public.restriccionescuenta() OWNER TO postgres;

--
-- Name: saldompatrimonial(integer, date, date); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION saldompatrimonial(integer, date, date) RETURNS numeric
    AS $_$
DECLARE
    identmpatrimonial ALIAS FOR $1;
    fechain ALIAS FOR $2;
    fechafin ALIAS FOR $3;
    aptsum RECORD;
    aptrest RECORD;
    rsaldo RECORD;
    rsaldo1 RECORD;
    mp RECORD;
    smpatrimonialsum RECORD;
    smpatrimonialrest RECORD;
    saldo NUMERIC(12, 2);
    saldoi NUMERIC(12, 2);
BEGIN
    saldo := 0;
    FOR rsaldo IN SELECT * FROM cuenta LEFT JOIN compmasap ON cuenta.idcuenta = compmasap.idcuenta WHERE masaperteneciente = identmpatrimonial AND compmasap.idcuenta IS NOT NULL AND signo = TRUE LOOP
	saldoi := saldototal(rsaldo.codigo, fechain, fechafin);
	IF (rsaldo.tipocompmasap = 0 OR (rsaldo.tipocompmasap = 1 AND saldoi > 0) OR (rsaldo.tipocompmasap = 2 AND saldoi < 0)) THEN
	    saldo := saldo + saldoi;
	END IF;
    END LOOP;
    FOR rsaldo1 IN SELECT  * FROM cuenta LEFT JOIN compmasap ON cuenta.idcuenta = compmasap.idcuenta WHERE masaperteneciente = identmpatrimonial AND compmasap.idcuenta IS NOT NULL AND signo = FALSE LOOP
	saldoi := saldototal(rsaldo1.codigo, fechain, fechafin);
	IF (rsaldo1.tipocompmasap = 0 OR (rsaldo1.tipocompmasap = 1 AND saldoi > 0) OR (rsaldo1.tipocompmasap = 2 AND saldoi < 0)) THEN
    	    saldo := saldo - saldoi;
	END IF;
    END LOOP;
    FOR smpatrimonialsum IN SELECT idmpatrimonial, tipocompmasap FROM compmasap WHERE masaperteneciente = identmpatrimonial AND idmpatrimonial IS NOT NULL AND signo = true LOOP
	saldoi := saldompatrimonial (smpatrimonialsum.idmpatrimonial, fechain, fechafin);
	IF (smpatrimonialsum.tipocompmasap = 0 OR (smpatrimonialsum.tipocompmasap = 1 AND saldoi > 0) OR (smpatrimonialsum.tipocompmasap = 2 AND saldoi < 0)) THEN
       	    saldo := saldo + saldoi;
	END IF;
    END LOOP;
    FOR smpatrimonialrest IN SELECT idmpatrimonial, tipocompmasap FROM compmasap WHERE masaperteneciente = identmpatrimonial AND idmpatrimonial IS NOT NULL AND signo = false LOOP
	saldoi := saldompatrimonial (smpatrimonialrest.idmpatrimonial, fechain, fechafin);
	IF (smpatrimonialrest.tipocompmasap = 0 OR (smpatrimonialrest.tipocompmasap = 1 AND saldoi > 0) OR (smpatrimonialrest.tipocompmasap = 2 AND saldoi < 0)) THEN
	    saldo := saldo - saldoi;
	END IF;
    END LOOP;
    SELECT INTO mp tipompatrimonial FROM mpatrimonial WHERE idmpatrimonial = identmpatrimonial;
    IF FOUND THEN
	IF mp.tipompatrimonial = 1 THEN
	    IF saldo > 0 THEN
		RETURN saldo;
	    ELSE
	    	RETURN 0;
	    END IF;
	END IF;
	IF mp.tipompatrimonial = 2 THEN
	    IF saldo < 0 THEN
		RETURN saldo;
	    ELSE
		RETURN 0;
	    END IF;
	END IF;
	IF mp.tipompatrimonial = 3 THEN
	    IF saldo > 0 THEN
		RETURN saldo;
	    ELSE
		RETURN -saldo;
	    END IF;
	END IF;
    END IF;
    RETURN saldo;
END;
$_$
    LANGUAGE plpgsql;


ALTER FUNCTION public.saldompatrimonial(integer, date, date) OWNER TO postgres;

--
-- Name: saldototal(character varying, date, date); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION saldototal(character varying, date, date) RETURNS numeric
    AS $_$
DECLARE
    codcuenta ALIAS FOR $1;
    fechain ALIAS FOR $2;
    fechafin ALIAS FOR $3;
    apt RECORD;
    apt1 RECORD;
    --apt2 RECORD;
    cta RECORD;
    saldo NUMERIC(12,2);
BEGIN
--  SELECT INTO apt * FROM apunte WHERE id_cuenta(codcuenta) = apunte.idcuenta;
    SELECT INTO cta id_cuenta(codcuenta) AS id;
    SELECT INTO apt sum(debe) AS tdebe, sum(haber) AS thaber FROM apunte WHERE apunte.idcuenta = cta.id AND fecha <= "fechafin" AND fecha >= "fechain";
    IF (apt.tdebe ISNULL) THEN
        saldo := 0;
    ELSE
        saldo := apt.tdebe - apt.thaber;
    END IF;
--  RAISE NOTICE ' saldo total % valor adquirido %', codcuenta, saldo;
    FOR apt1 IN SELECT codigo FROM cuenta WHERE padre = cta.id LOOP
        saldo := saldo + saldototal(apt1.codigo,fechain, fechafin);
    END LOOP;
    RETURN saldo;
END;
$_$
    LANGUAGE plpgsql;


ALTER FUNCTION public.saldototal(character varying, date, date) OWNER TO postgres;

--
-- Name: saldototalmpatrimonial(integer); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION saldototalmpatrimonial(integer) RETURNS numeric
    AS $_$
DECLARE
   identmpatrimonial ALIAS FOR $1;
   saldo NUMERIC(12, 2);
   saldoi NUMERIC(12, 2);
   rsaldo RECORD;
   rsaldo1 RECORD;
   smpatrimonialsum RECORD;
   smpatrimonialrest RECORD;
   mp RECORD;
BEGIN
    saldo := 0;
    FOR rsaldo IN SELECT (debe-haber) AS total, tipocompmasap FROM cuenta LEFT JOIN compmasap ON cuenta.idcuenta = compmasap.idcuenta WHERE masaperteneciente = identmpatrimonial AND compmasap.idcuenta IS NOT NULL AND signo = TRUE LOOP
	IF (rsaldo.total IS NOT NULL) THEN
	    IF (rsaldo.tipocompmasap = 0 OR (rsaldo.tipocompmasap = 1 AND rsaldo.total > 0) OR (rsaldo.tipocompmasap = 2 AND rsaldo.total < 0)) THEN
		saldo := saldo + rsaldo.total;
	    END IF;
	END IF;
    END LOOP;
    FOR rsaldo1 IN SELECT (debe-haber) AS total, tipocompmasap FROM cuenta LEFT JOIN compmasap ON cuenta.idcuenta = compmasap.idcuenta WHERE masaperteneciente = identmpatrimonial AND compmasap.idcuenta IS NOT NULL AND signo = FALSE LOOP
	IF (rsaldo1.total IS NOT NULL) THEN
	    IF (rsaldo1.tipocompmasap = 0 OR (rsaldo1.tipocompmasap = 1 AND rsaldo1.total > 0) OR (rsaldo1.tipocompmasap = 2 AND rsaldo1.total < 0)) THEN
		saldo := saldo - rsaldo1.total;
	    END IF;
	END IF;
    END LOOP;
    FOR smpatrimonialsum IN SELECT idmpatrimonial, tipocompmasap FROM compmasap WHERE masaperteneciente = identmpatrimonial AND idmpatrimonial IS NOT NULL AND signo = true LOOP
	saldoi := saldototalmpatrimonial (smpatrimonialsum.idmpatrimonial);
	IF (smpatrimonialsum.tipocompmasap = 0 OR (smpatrimonialsum.tipocompmasap = 1 AND saldoi > 0) OR (smpatrimonialsum.tipocompmasap = 2 AND saldoi < 0)) THEN
       	    saldo := saldo + saldoi;
	END IF;
    END LOOP;
    FOR smpatrimonialrest IN SELECT idmpatrimonial, tipocompmasap FROM compmasap WHERE masaperteneciente = identmpatrimonial AND idmpatrimonial IS NOT NULL AND signo = false LOOP
	saldoi := saldototalmpatrimonial (smpatrimonialrest.idmpatrimonial);
	IF (smpatrimonialrest.tipocompmasap = 0 OR (smpatrimonialrest.tipocompmasap = 1 AND saldoi > 0) OR (smpatrimonialrest.tipocompmasap = 2 AND saldoi < 0)) THEN
       	    saldo := saldo - saldoi;
	END IF;
    END LOOP;
    SELECT INTO mp tipompatrimonial FROM mpatrimonial WHERE idmpatrimonial = identmpatrimonial;
    IF FOUND THEN
	IF mp.tipompatrimonial = 1 THEN
	    IF saldo > 0 THEN
		RETURN saldo;
	    ELSE
		RETURN 0;
	    END IF;
	END IF;
	IF mp.tipompatrimonial = 2 THEN
	    IF saldo < 0 THEN
		RETURN saldo;
	    ELSE
		RETURN 0;
	    END IF;
	END IF;
	IF mp.tipompatrimonial = 3 THEN
	    IF saldo > 0 THEN
		RETURN saldo;
	    ELSE
		RETURN -saldo;
	    END IF;
	END IF;
    END IF;
    RETURN saldo;
END;
$_$
    LANGUAGE plpgsql;


ALTER FUNCTION public.saldototalmpatrimonial(integer) OWNER TO postgres;

--
-- Name: tr_borradoasiento(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION tr_borradoasiento() RETURNS "trigger"
    AS $$
BEGIN
    UPDATE prevcobro SET idasiento = NULL WHERE idasiento = OLD.idasiento;
    RETURN OLD;
END;
$$
    LANGUAGE plpgsql;


ALTER FUNCTION public.tr_borradoasiento() OWNER TO postgres;

SET default_tablespace = '';

SET default_with_oids = false;

--
-- Name: acumulado_c_coste; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE acumulado_c_coste (
    idacumulado_c_coste integer NOT NULL,
    idcuenta integer NOT NULL,
    idc_coste integer NOT NULL,
    debe numeric(12,2) DEFAULT 0,
    haber numeric(12,2) DEFAULT 0
);


ALTER TABLE public.acumulado_c_coste OWNER TO postgres;

--
-- Name: acumulado_c_coste_idacumulado_c_coste_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE acumulado_c_coste_idacumulado_c_coste_seq
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.acumulado_c_coste_idacumulado_c_coste_seq OWNER TO postgres;

--
-- Name: acumulado_c_coste_idacumulado_c_coste_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE acumulado_c_coste_idacumulado_c_coste_seq OWNED BY acumulado_c_coste.idacumulado_c_coste;


--
-- Name: acumulado_c_coste_idacumulado_c_coste_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('acumulado_c_coste_idacumulado_c_coste_seq', 25953, true);


--
-- Name: acumulado_canal; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE acumulado_canal (
    idacumulado_canal integer NOT NULL,
    idcuenta integer NOT NULL,
    idcanal integer NOT NULL,
    debe numeric(12,2) DEFAULT 0,
    haber numeric(12,2) DEFAULT 0
);


ALTER TABLE public.acumulado_canal OWNER TO postgres;

--
-- Name: acumulado_canal_idacumulado_canal_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE acumulado_canal_idacumulado_canal_seq
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.acumulado_canal_idacumulado_canal_seq OWNER TO postgres;

--
-- Name: acumulado_canal_idacumulado_canal_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE acumulado_canal_idacumulado_canal_seq OWNED BY acumulado_canal.idacumulado_canal;


--
-- Name: acumulado_canal_idacumulado_canal_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('acumulado_canal_idacumulado_canal_seq', 5338, true);


--
-- Name: adocumental; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE adocumental (
    idadocumental integer NOT NULL,
    idasiento integer,
    descripcionadocumental character varying(200),
    fechaintadocumental date,
    fechaasadocumental date,
    archivoadocumental character varying(300)
);


ALTER TABLE public.adocumental OWNER TO postgres;

--
-- Name: adocumental_idadocumental_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE adocumental_idadocumental_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.adocumental_idadocumental_seq OWNER TO postgres;

--
-- Name: adocumental_idadocumental_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE adocumental_idadocumental_seq OWNED BY adocumental.idadocumental;


--
-- Name: adocumental_idadocumental_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('adocumental_idadocumental_seq', 1, false);


--
-- Name: ainteligente; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE ainteligente (
    idainteligente integer NOT NULL,
    descripcion character varying(100),
    comentariosasiento character varying(2000)
);


ALTER TABLE public.ainteligente OWNER TO postgres;

--
-- Name: ainteligente_idainteligente_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE ainteligente_idainteligente_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.ainteligente_idainteligente_seq OWNER TO postgres;

--
-- Name: ainteligente_idainteligente_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE ainteligente_idainteligente_seq OWNED BY ainteligente.idainteligente;


--
-- Name: ainteligente_idainteligente_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('ainteligente_idainteligente_seq', 1, false);


--
-- Name: amortizacion; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE amortizacion (
    idamortizacion integer NOT NULL,
    idcuentaactivo integer,
    idcuentaamortizacion integer,
    descamortizacion character varying(2000),
    nomamortizacion character varying(200),
    fechacompra date,
    fecha1cuota date,
    valorcompra numeric(12,2),
    periodicidad integer,
    numcuotas integer,
    metodo integer,
    nifproveedor character varying(12),
    nomproveedor character varying(150),
    dirproveedor character varying(200),
    telproveedor character varying(20),
    agrupacion character varying(150)
);


ALTER TABLE public.amortizacion OWNER TO postgres;

--
-- Name: amortizacion_idamortizacion_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE amortizacion_idamortizacion_seq
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.amortizacion_idamortizacion_seq OWNER TO postgres;

--
-- Name: amortizacion_idamortizacion_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE amortizacion_idamortizacion_seq OWNED BY amortizacion.idamortizacion;


--
-- Name: amortizacion_idamortizacion_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('amortizacion_idamortizacion_seq', 1, true);


--
-- Name: apunte; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE apunte (
    idapunte integer NOT NULL,
    codigoborrador integer,
    idasiento integer NOT NULL,
    iddiario integer,
    fecha date,
    conceptocontable character varying(50),
    idcuenta integer NOT NULL,
    descripcion character varying(100),
    debe numeric(12,2) DEFAULT 0 NOT NULL,
    haber numeric(12,2) DEFAULT 0 NOT NULL,
    contrapartida integer,
    comentario character varying(100),
    idcanal integer,
    marcaconciliacion character(12),
    idc_coste integer,
    idtipoiva integer,
    orden integer,
    punteo boolean DEFAULT false
);


ALTER TABLE public.apunte OWNER TO postgres;

--
-- Name: apunte_idapunte_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE apunte_idapunte_seq
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.apunte_idapunte_seq OWNER TO postgres;

--
-- Name: apunte_idapunte_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE apunte_idapunte_seq OWNED BY apunte.idapunte;


--
-- Name: apunte_idapunte_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('apunte_idapunte_seq', 2756, true);


--
-- Name: asiento; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE asiento (
    idasiento integer NOT NULL,
    descripcion character varying(100),
    fecha date,
    comentariosasiento character varying(2000),
    ordenasiento integer,
    clase smallint DEFAULT 0
);


ALTER TABLE public.asiento OWNER TO postgres;

--
-- Name: asiento_idasiento_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE asiento_idasiento_seq
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.asiento_idasiento_seq OWNER TO postgres;

--
-- Name: asiento_idasiento_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE asiento_idasiento_seq OWNED BY asiento.idasiento;


--
-- Name: asiento_idasiento_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('asiento_idasiento_seq', 1320, true);


--
-- Name: balance; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE balance (
    idbalance integer NOT NULL,
    nombrebalance character varying(150)
);


ALTER TABLE public.balance OWNER TO postgres;

--
-- Name: balance_idbalance_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE balance_idbalance_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.balance_idbalance_seq OWNER TO postgres;

--
-- Name: balance_idbalance_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE balance_idbalance_seq OWNED BY balance.idbalance;


--
-- Name: balance_idbalance_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('balance_idbalance_seq', 1, false);


--
-- Name: binteligente; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE binteligente (
    idbinteligente integer NOT NULL,
    idainteligente integer,
    iddiario character varying(100),
    fecha character varying(100),
    conceptocontable character varying(100),
    codcuenta character varying(100),
    descripcion character varying(100),
    debe character varying(100),
    haber character varying(100),
    contrapartida character varying(100),
    comentario character varying(100),
    canal character varying(100),
    marcaconciliacion character varying(100),
    idc_coste character varying(100)
);


ALTER TABLE public.binteligente OWNER TO postgres;

--
-- Name: binteligente_idbinteligente_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE binteligente_idbinteligente_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.binteligente_idbinteligente_seq OWNER TO postgres;

--
-- Name: binteligente_idbinteligente_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE binteligente_idbinteligente_seq OWNED BY binteligente.idbinteligente;


--
-- Name: binteligente_idbinteligente_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('binteligente_idbinteligente_seq', 1, false);


--
-- Name: borrador; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE borrador (
    idborrador integer NOT NULL,
    codigoborrador integer,
    idasiento integer NOT NULL,
    iddiario integer,
    fecha date,
    conceptocontable character varying(50),
    idcuenta integer NOT NULL,
    descripcion character varying(100),
    debe numeric(12,2) DEFAULT 0 NOT NULL,
    haber numeric(12,2) DEFAULT 0 NOT NULL,
    contrapartida integer,
    comentario character varying(100),
    idcanal integer,
    marcaconciliacion character(12),
    idc_coste integer,
    idapunte integer,
    idtipoiva integer,
    orden integer
);


ALTER TABLE public.borrador OWNER TO postgres;

--
-- Name: borrador_idborrador_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE borrador_idborrador_seq
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.borrador_idborrador_seq OWNER TO postgres;

--
-- Name: borrador_idborrador_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE borrador_idborrador_seq OWNED BY borrador.idborrador;


--
-- Name: borrador_idborrador_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('borrador_idborrador_seq', 27080, true);


--
-- Name: c_coste; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE c_coste (
    idc_coste integer NOT NULL,
    descripcion character varying(100),
    nombre character varying(50) NOT NULL,
    codigo character(3),
    padre integer,
    imputacion boolean,
    debe numeric(12,2) DEFAULT 0,
    haber numeric(12,2) DEFAULT 0
);


ALTER TABLE public.c_coste OWNER TO postgres;

--
-- Name: c_coste_idc_coste_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE c_coste_idc_coste_seq
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.c_coste_idc_coste_seq OWNER TO postgres;

--
-- Name: c_coste_idc_coste_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE c_coste_idc_coste_seq OWNED BY c_coste.idc_coste;


--
-- Name: c_coste_idc_coste_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('c_coste_idc_coste_seq', 32, true);


--
-- Name: c_costedist; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE c_costedist (
    idc_costedist integer NOT NULL,
    idc_coste integer NOT NULL,
    iddestc_coste integer NOT NULL,
    porcentc_costedist numeric(12,2) DEFAULT 0
);


ALTER TABLE public.c_costedist OWNER TO postgres;

--
-- Name: c_costedist_idc_costedist_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE c_costedist_idc_costedist_seq
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.c_costedist_idc_costedist_seq OWNER TO postgres;

--
-- Name: c_costedist_idc_costedist_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE c_costedist_idc_costedist_seq OWNED BY c_costedist.idc_costedist;


--
-- Name: c_costedist_idc_costedist_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('c_costedist_idc_costedist_seq', 9, true);


--
-- Name: canal; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE canal (
    idcanal integer NOT NULL,
    descripcion character varying(100),
    nombre character varying(50)
);


ALTER TABLE public.canal OWNER TO postgres;

--
-- Name: canal_idcanal_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE canal_idcanal_seq
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.canal_idcanal_seq OWNER TO postgres;

--
-- Name: canal_idcanal_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE canal_idcanal_seq OWNED BY canal.idcanal;


--
-- Name: canal_idcanal_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('canal_idcanal_seq', 6, true);


--
-- Name: compbalance; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE compbalance (
    idcompbalance integer NOT NULL,
    idbalance integer NOT NULL,
    idmpatrimonial integer,
    concepto character varying(150),
    orden integer,
    tabulacion integer
);


ALTER TABLE public.compbalance OWNER TO postgres;

--
-- Name: compbalance_idcompbalance_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE compbalance_idcompbalance_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.compbalance_idcompbalance_seq OWNER TO postgres;

--
-- Name: compbalance_idcompbalance_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE compbalance_idcompbalance_seq OWNED BY compbalance.idcompbalance;


--
-- Name: compbalance_idcompbalance_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('compbalance_idcompbalance_seq', 1, false);


--
-- Name: compmasap; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE compmasap (
    idcompmasap integer NOT NULL,
    idcuenta integer,
    idmpatrimonial integer,
    masaperteneciente integer,
    saldo numeric(12,2),
    signo boolean,
    tipocompmasap integer DEFAULT 0,
    nombre character varying(150)
);


ALTER TABLE public.compmasap OWNER TO postgres;

--
-- Name: compmasap_idcompmasap_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE compmasap_idcompmasap_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.compmasap_idcompmasap_seq OWNER TO postgres;

--
-- Name: compmasap_idcompmasap_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE compmasap_idcompmasap_seq OWNED BY compmasap.idcompmasap;


--
-- Name: compmasap_idcompmasap_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('compmasap_idcompmasap_seq', 1, false);


--
-- Name: configuracion; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE configuracion (
    nombre character varying(25) NOT NULL,
    valor character varying(350)
);


ALTER TABLE public.configuracion OWNER TO postgres;

--
-- Name: cuenta; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE cuenta (
    idcuenta integer NOT NULL,
    codigo character varying(12) NOT NULL,
    descripcion character varying(100) NOT NULL,
    imputacion boolean DEFAULT true NOT NULL,
    padre integer,
    bloqueada boolean DEFAULT false NOT NULL,
    msg character varying(500),
    debe numeric(12,2) DEFAULT 0 NOT NULL,
    haber numeric(12,2) DEFAULT 0 NOT NULL,
    nodebe boolean DEFAULT false NOT NULL,
    nohaber boolean DEFAULT false NOT NULL,
    regularizacion boolean,
    activo boolean,
    nombreent_cuenta character varying(100),
    cifent_cuenta character varying(12),
    cpent_cuenta character varying(5),
    dirent_cuenta character varying(80),
    telent_cuenta character varying(20),
    coment_cuenta character varying(500),
    bancoent_cuenta character varying(30),
    emailent_cuenta character varying(50),
    webent_cuenta character varying(70),
    tipocuenta integer DEFAULT 1,
    idprovincia integer,
    poblacion character varying(150),
    idc_coste integer
);


ALTER TABLE public.cuenta OWNER TO postgres;

--
-- Name: cuenta_idcuenta_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE cuenta_idcuenta_seq
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.cuenta_idcuenta_seq OWNER TO postgres;

--
-- Name: cuenta_idcuenta_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE cuenta_idcuenta_seq OWNED BY cuenta.idcuenta;


--
-- Name: cuenta_idcuenta_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('cuenta_idcuenta_seq', 10943, true);


--
-- Name: diario; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE diario (
    iddiario integer NOT NULL,
    descripcion character varying(100)
);


ALTER TABLE public.diario OWNER TO postgres;

--
-- Name: diario_iddiario_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE diario_iddiario_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.diario_iddiario_seq OWNER TO postgres;

--
-- Name: diario_iddiario_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE diario_iddiario_seq OWNED BY diario.iddiario;


--
-- Name: diario_iddiario_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('diario_iddiario_seq', 1, false);


--
-- Name: ejercicios; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE ejercicios (
    ejercicio integer,
    periodo smallint,
    bloqueado boolean
);


ALTER TABLE public.ejercicios OWNER TO postgres;

--
-- Name: fpago; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE fpago (
    idfpago integer NOT NULL,
    nomfpago character varying(50),
    nplazosfpago integer,
    plazoprimerpagofpago integer,
    tipoplazoprimerpagofpago integer,
    plazoentrerecibofpago integer,
    tipoplazoentrerecibofpago integer
);


ALTER TABLE public.fpago OWNER TO postgres;

--
-- Name: fpago_idfpago_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE fpago_idfpago_seq
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.fpago_idfpago_seq OWNER TO postgres;

--
-- Name: fpago_idfpago_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE fpago_idfpago_seq OWNED BY fpago.idfpago;


--
-- Name: fpago_idfpago_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('fpago_idfpago_seq', 8, true);


--
-- Name: iva; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE iva (
    idiva integer NOT NULL,
    idtipoiva integer NOT NULL,
    idregistroiva integer NOT NULL,
    baseiva numeric(12,2) DEFAULT 0,
    ivaiva numeric(12,2) DEFAULT 0
);


ALTER TABLE public.iva OWNER TO postgres;

--
-- Name: iva_idiva_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE iva_idiva_seq
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.iva_idiva_seq OWNER TO postgres;

--
-- Name: iva_idiva_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE iva_idiva_seq OWNED BY iva.idiva;


--
-- Name: iva_idiva_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('iva_idiva_seq', 8, true);


--
-- Name: lgaspresupuestoc; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE lgaspresupuestoc (
    idlgaspresupuestoc integer NOT NULL,
    idpresupuestoc integer NOT NULL,
    ordenlgaspresupuestoc integer,
    idcuenta integer NOT NULL,
    cantlgaspresupuestoc numeric(12,2),
    precioundlgaspresupuestoc numeric(12,2),
    ejerciciolgaspresupuestoc integer,
    comentlgaspresupuestoc character varying,
    desclgaspresupuestoc character varying
);


ALTER TABLE public.lgaspresupuestoc OWNER TO postgres;

--
-- Name: lgaspresupuestoc_idlgaspresupuestoc_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE lgaspresupuestoc_idlgaspresupuestoc_seq
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.lgaspresupuestoc_idlgaspresupuestoc_seq OWNER TO postgres;

--
-- Name: lgaspresupuestoc_idlgaspresupuestoc_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE lgaspresupuestoc_idlgaspresupuestoc_seq OWNED BY lgaspresupuestoc.idlgaspresupuestoc;


--
-- Name: lgaspresupuestoc_idlgaspresupuestoc_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('lgaspresupuestoc_idlgaspresupuestoc_seq', 1, true);


--
-- Name: linamortizacion; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE linamortizacion (
    idlinamortizacion integer NOT NULL,
    idamortizacion integer,
    idasiento integer,
    ejercicio integer,
    fechaprevista date,
    cantidad numeric(12,2)
);


ALTER TABLE public.linamortizacion OWNER TO postgres;

--
-- Name: linamortizacion_idlinamortizacion_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE linamortizacion_idlinamortizacion_seq
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.linamortizacion_idlinamortizacion_seq OWNER TO postgres;

--
-- Name: linamortizacion_idlinamortizacion_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE linamortizacion_idlinamortizacion_seq OWNED BY linamortizacion.idlinamortizacion;


--
-- Name: linamortizacion_idlinamortizacion_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('linamortizacion_idlinamortizacion_seq', 5, true);


--
-- Name: lingpresupuestoc; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE lingpresupuestoc (
    idlingpresupuestoc integer NOT NULL,
    idpresupuestoc integer NOT NULL,
    ordenlingpresupuestoc integer,
    idcuenta integer NOT NULL,
    cantlingpresupuestoc numeric(12,2),
    precioundlingpresupuestoc numeric(12,2),
    ejerciciolingpresupuestoc integer,
    comentlingpresupuestoc character varying,
    desclingpresupuestoc character varying
);


ALTER TABLE public.lingpresupuestoc OWNER TO postgres;

--
-- Name: lingpresupuestoc_idlingpresupuestoc_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE lingpresupuestoc_idlingpresupuestoc_seq
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.lingpresupuestoc_idlingpresupuestoc_seq OWNER TO postgres;

--
-- Name: lingpresupuestoc_idlingpresupuestoc_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE lingpresupuestoc_idlingpresupuestoc_seq OWNED BY lingpresupuestoc.idlingpresupuestoc;


--
-- Name: lingpresupuestoc_idlingpresupuestoc_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('lingpresupuestoc_idlingpresupuestoc_seq', 3, true);


--
-- Name: mpatrimonial; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE mpatrimonial (
    idmpatrimonial integer NOT NULL,
    idbalance integer,
    descmpatrimonial character varying(150),
    orden integer,
    tabulacion integer,
    saldo numeric(12,2),
    opdesc integer,
    tipompatrimonial integer DEFAULT 0
);


ALTER TABLE public.mpatrimonial OWNER TO postgres;

--
-- Name: mpatrimonial_idmpatrimonial_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE mpatrimonial_idmpatrimonial_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.mpatrimonial_idmpatrimonial_seq OWNER TO postgres;

--
-- Name: mpatrimonial_idmpatrimonial_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE mpatrimonial_idmpatrimonial_seq OWNED BY mpatrimonial.idmpatrimonial;


--
-- Name: mpatrimonial_idmpatrimonial_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('mpatrimonial_idmpatrimonial_seq', 1, false);


--
-- Name: pais; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE pais (
    idpais integer NOT NULL,
    cod2pais character varying(2),
    cod3pais character varying(3),
    descpais character varying(50)
);


ALTER TABLE public.pais OWNER TO postgres;

--
-- Name: pais_idpais_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE pais_idpais_seq
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.pais_idpais_seq OWNER TO postgres;

--
-- Name: pais_idpais_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE pais_idpais_seq OWNED BY pais.idpais;


--
-- Name: pais_idpais_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('pais_idpais_seq', 2, true);


--
-- Name: presupuestoc; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE presupuestoc (
    idpresupuestoc integer NOT NULL,
    idc_coste integer NOT NULL,
    fechapresupuestoc date DEFAULT now() NOT NULL,
    nompresupuestoc character varying,
    comentpresupuestoc character varying,
    archpresupuestoc character varying
);


ALTER TABLE public.presupuestoc OWNER TO postgres;

--
-- Name: presupuestoc_idpresupuestoc_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE presupuestoc_idpresupuestoc_seq
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.presupuestoc_idpresupuestoc_seq OWNER TO postgres;

--
-- Name: presupuestoc_idpresupuestoc_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE presupuestoc_idpresupuestoc_seq OWNED BY presupuestoc.idpresupuestoc;


--
-- Name: presupuestoc_idpresupuestoc_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('presupuestoc_idpresupuestoc_seq', 2, true);


--
-- Name: prevcobro; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE prevcobro (
    idprevcobro integer NOT NULL,
    fprevistaprevcobro date,
    fcobroprevcobro date,
    idctacliente integer,
    idfpago integer,
    idcuenta integer,
    idasiento integer,
    cantidadprevistaprevcobro numeric(12,2) DEFAULT 0,
    cantidadprevcobro numeric(12,2) DEFAULT 0,
    idregistroiva integer,
    tipoprevcobro boolean DEFAULT false NOT NULL,
    docprevcobro character varying(50)
);


ALTER TABLE public.prevcobro OWNER TO postgres;

--
-- Name: prevcobro_idprevcobro_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE prevcobro_idprevcobro_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.prevcobro_idprevcobro_seq OWNER TO postgres;

--
-- Name: prevcobro_idprevcobro_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE prevcobro_idprevcobro_seq OWNED BY prevcobro.idprevcobro;


--
-- Name: prevcobro_idprevcobro_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('prevcobro_idprevcobro_seq', 1, false);


--
-- Name: provincia; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE provincia (
    idprovincia integer NOT NULL,
    idpais integer,
    provincia character varying(500)
);


ALTER TABLE public.provincia OWNER TO postgres;

--
-- Name: provincia_idprovincia_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE provincia_idprovincia_seq
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.provincia_idprovincia_seq OWNER TO postgres;

--
-- Name: provincia_idprovincia_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE provincia_idprovincia_seq OWNED BY provincia.idprovincia;


--
-- Name: provincia_idprovincia_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('provincia_idprovincia_seq', 12, true);


--
-- Name: registroiva; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE registroiva (
    idregistroiva integer NOT NULL,
    contrapartida integer,
    baseimp numeric(12,2),
    iva numeric(12,2),
    ffactura date,
    femisionregistroiva date,
    factura character varying(70),
    serieregistroiva character varying(40),
    idborrador integer NOT NULL,
    incregistro boolean,
    regularizacion boolean,
    plan349 boolean,
    numorden character varying(50),
    cif character varying(25),
    idfpago integer,
    factemitida boolean NOT NULL,
    rectificaaregistroiva integer
);


ALTER TABLE public.registroiva OWNER TO postgres;

--
-- Name: registroiva_idregistroiva_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE registroiva_idregistroiva_seq
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.registroiva_idregistroiva_seq OWNER TO postgres;

--
-- Name: registroiva_idregistroiva_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE registroiva_idregistroiva_seq OWNED BY registroiva.idregistroiva;


--
-- Name: registroiva_idregistroiva_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('registroiva_idregistroiva_seq', 4, true);


--
-- Name: tipoiva; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE tipoiva (
    idtipoiva integer NOT NULL,
    nombretipoiva character varying(25),
    porcentajetipoiva numeric(12,2),
    idcuenta integer NOT NULL
);


ALTER TABLE public.tipoiva OWNER TO postgres;

--
-- Name: tipoiva_idtipoiva_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE tipoiva_idtipoiva_seq
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.tipoiva_idtipoiva_seq OWNER TO postgres;

--
-- Name: tipoiva_idtipoiva_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE tipoiva_idtipoiva_seq OWNED BY tipoiva.idtipoiva;


--
-- Name: tipoiva_idtipoiva_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('tipoiva_idtipoiva_seq', 9, true);


--
-- Name: idacumulado_c_coste; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE acumulado_c_coste ALTER COLUMN idacumulado_c_coste SET DEFAULT nextval('acumulado_c_coste_idacumulado_c_coste_seq'::regclass);


--
-- Name: idacumulado_canal; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE acumulado_canal ALTER COLUMN idacumulado_canal SET DEFAULT nextval('acumulado_canal_idacumulado_canal_seq'::regclass);


--
-- Name: idadocumental; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE adocumental ALTER COLUMN idadocumental SET DEFAULT nextval('adocumental_idadocumental_seq'::regclass);


--
-- Name: idainteligente; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ainteligente ALTER COLUMN idainteligente SET DEFAULT nextval('ainteligente_idainteligente_seq'::regclass);


--
-- Name: idamortizacion; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE amortizacion ALTER COLUMN idamortizacion SET DEFAULT nextval('amortizacion_idamortizacion_seq'::regclass);


--
-- Name: idapunte; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE apunte ALTER COLUMN idapunte SET DEFAULT nextval('apunte_idapunte_seq'::regclass);


--
-- Name: idasiento; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE asiento ALTER COLUMN idasiento SET DEFAULT nextval('asiento_idasiento_seq'::regclass);


--
-- Name: idbalance; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE balance ALTER COLUMN idbalance SET DEFAULT nextval('balance_idbalance_seq'::regclass);


--
-- Name: idbinteligente; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE binteligente ALTER COLUMN idbinteligente SET DEFAULT nextval('binteligente_idbinteligente_seq'::regclass);


--
-- Name: idborrador; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE borrador ALTER COLUMN idborrador SET DEFAULT nextval('borrador_idborrador_seq'::regclass);


--
-- Name: idc_coste; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE c_coste ALTER COLUMN idc_coste SET DEFAULT nextval('c_coste_idc_coste_seq'::regclass);


--
-- Name: idc_costedist; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE c_costedist ALTER COLUMN idc_costedist SET DEFAULT nextval('c_costedist_idc_costedist_seq'::regclass);


--
-- Name: idcanal; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE canal ALTER COLUMN idcanal SET DEFAULT nextval('canal_idcanal_seq'::regclass);


--
-- Name: idcompbalance; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE compbalance ALTER COLUMN idcompbalance SET DEFAULT nextval('compbalance_idcompbalance_seq'::regclass);


--
-- Name: idcompmasap; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE compmasap ALTER COLUMN idcompmasap SET DEFAULT nextval('compmasap_idcompmasap_seq'::regclass);


--
-- Name: idcuenta; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE cuenta ALTER COLUMN idcuenta SET DEFAULT nextval('cuenta_idcuenta_seq'::regclass);


--
-- Name: iddiario; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE diario ALTER COLUMN iddiario SET DEFAULT nextval('diario_iddiario_seq'::regclass);


--
-- Name: idfpago; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE fpago ALTER COLUMN idfpago SET DEFAULT nextval('fpago_idfpago_seq'::regclass);


--
-- Name: idiva; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE iva ALTER COLUMN idiva SET DEFAULT nextval('iva_idiva_seq'::regclass);


--
-- Name: idlgaspresupuestoc; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE lgaspresupuestoc ALTER COLUMN idlgaspresupuestoc SET DEFAULT nextval('lgaspresupuestoc_idlgaspresupuestoc_seq'::regclass);


--
-- Name: idlinamortizacion; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE linamortizacion ALTER COLUMN idlinamortizacion SET DEFAULT nextval('linamortizacion_idlinamortizacion_seq'::regclass);


--
-- Name: idlingpresupuestoc; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE lingpresupuestoc ALTER COLUMN idlingpresupuestoc SET DEFAULT nextval('lingpresupuestoc_idlingpresupuestoc_seq'::regclass);


--
-- Name: idmpatrimonial; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE mpatrimonial ALTER COLUMN idmpatrimonial SET DEFAULT nextval('mpatrimonial_idmpatrimonial_seq'::regclass);


--
-- Name: idpais; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE pais ALTER COLUMN idpais SET DEFAULT nextval('pais_idpais_seq'::regclass);


--
-- Name: idpresupuestoc; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE presupuestoc ALTER COLUMN idpresupuestoc SET DEFAULT nextval('presupuestoc_idpresupuestoc_seq'::regclass);


--
-- Name: idprevcobro; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE prevcobro ALTER COLUMN idprevcobro SET DEFAULT nextval('prevcobro_idprevcobro_seq'::regclass);


--
-- Name: idprovincia; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE provincia ALTER COLUMN idprovincia SET DEFAULT nextval('provincia_idprovincia_seq'::regclass);


--
-- Name: idregistroiva; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE registroiva ALTER COLUMN idregistroiva SET DEFAULT nextval('registroiva_idregistroiva_seq'::regclass);


--
-- Name: idtipoiva; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE tipoiva ALTER COLUMN idtipoiva SET DEFAULT nextval('tipoiva_idtipoiva_seq'::regclass);


--
-- Data for Name: acumulado_c_coste; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY acumulado_c_coste (idacumulado_c_coste, idcuenta, idc_coste, debe, haber) FROM stdin;
18102	10078	24	0.00	0.00
18103	10320	24	0.00	0.00
18105	10003	24	0.00	0.00
18107	10002	24	0.00	0.00
18108	10004	24	0.00	0.00
18109	10005	24	0.00	0.00
18110	10006	24	0.00	0.00
18111	10007	24	0.00	0.00
18112	10008	24	0.00	0.00
18113	10009	24	0.00	0.00
18114	10010	24	0.00	0.00
18115	10011	24	0.00	0.00
18116	10012	24	0.00	0.00
18117	10013	24	0.00	0.00
18118	10014	24	0.00	0.00
18119	10015	24	0.00	0.00
18120	10016	24	0.00	0.00
18121	10017	24	0.00	0.00
18122	10018	24	0.00	0.00
18123	10019	24	0.00	0.00
18124	10020	24	0.00	0.00
18125	10021	24	0.00	0.00
18126	10022	24	0.00	0.00
18127	10023	24	0.00	0.00
18128	10024	24	0.00	0.00
18129	10025	24	0.00	0.00
18130	10026	24	0.00	0.00
18131	10027	24	0.00	0.00
18132	10028	24	0.00	0.00
18133	10030	24	0.00	0.00
18134	10032	24	0.00	0.00
18135	10033	24	0.00	0.00
18136	10035	24	0.00	0.00
18137	10036	24	0.00	0.00
18138	10037	24	0.00	0.00
18139	10038	24	0.00	0.00
18140	10039	24	0.00	0.00
18141	10040	24	0.00	0.00
18142	10041	24	0.00	0.00
18143	10042	24	0.00	0.00
18144	10043	24	0.00	0.00
18145	10045	24	0.00	0.00
18146	10046	24	0.00	0.00
18147	10048	24	0.00	0.00
18148	10049	24	0.00	0.00
18149	10050	24	0.00	0.00
18150	10051	24	0.00	0.00
18151	10052	24	0.00	0.00
18152	10053	24	0.00	0.00
18153	10054	24	0.00	0.00
18154	10055	24	0.00	0.00
18155	10056	24	0.00	0.00
18156	10058	24	0.00	0.00
18157	10059	24	0.00	0.00
18158	10060	24	0.00	0.00
18159	10061	24	0.00	0.00
18160	10062	24	0.00	0.00
18161	10063	24	0.00	0.00
18162	10064	24	0.00	0.00
18164	10066	24	0.00	0.00
18165	10067	24	0.00	0.00
18167	10069	24	0.00	0.00
18168	10070	24	0.00	0.00
18169	10071	24	0.00	0.00
18170	10072	24	0.00	0.00
18171	10073	24	0.00	0.00
18172	10074	24	0.00	0.00
18173	10075	24	0.00	0.00
18174	10076	24	0.00	0.00
18175	10077	24	0.00	0.00
18176	10079	24	0.00	0.00
18177	10080	24	0.00	0.00
18178	10081	24	0.00	0.00
18179	10082	24	0.00	0.00
18180	10083	24	0.00	0.00
18181	10084	24	0.00	0.00
18182	10085	24	0.00	0.00
18183	10086	24	0.00	0.00
18184	10087	24	0.00	0.00
18185	10088	24	0.00	0.00
18186	10089	24	0.00	0.00
18187	10090	24	0.00	0.00
18188	10091	24	0.00	0.00
18189	10092	24	0.00	0.00
18190	10093	24	0.00	0.00
18191	10094	24	0.00	0.00
18192	10095	24	0.00	0.00
18193	10096	24	0.00	0.00
18194	10097	24	0.00	0.00
18195	10098	24	0.00	0.00
18196	10099	24	0.00	0.00
18197	10100	24	0.00	0.00
18198	10101	24	0.00	0.00
18199	10102	24	0.00	0.00
18200	10103	24	0.00	0.00
18201	10104	24	0.00	0.00
18202	10105	24	0.00	0.00
18203	10106	24	0.00	0.00
18204	10107	24	0.00	0.00
18205	10108	24	0.00	0.00
18206	10109	24	0.00	0.00
18207	10110	24	0.00	0.00
18208	10111	24	0.00	0.00
18209	10112	24	0.00	0.00
18210	10113	24	0.00	0.00
18211	10114	24	0.00	0.00
18212	10115	24	0.00	0.00
18213	10116	24	0.00	0.00
18214	10117	24	0.00	0.00
18215	10118	24	0.00	0.00
18216	10119	24	0.00	0.00
18217	10120	24	0.00	0.00
18218	10121	24	0.00	0.00
18219	10122	24	0.00	0.00
18220	10123	24	0.00	0.00
18221	10124	24	0.00	0.00
18222	10125	24	0.00	0.00
18223	10126	24	0.00	0.00
18224	10127	24	0.00	0.00
18225	10128	24	0.00	0.00
18226	10129	24	0.00	0.00
18227	10130	24	0.00	0.00
18228	10131	24	0.00	0.00
18229	10132	24	0.00	0.00
18230	10133	24	0.00	0.00
18231	10134	24	0.00	0.00
18232	10135	24	0.00	0.00
18233	10136	24	0.00	0.00
18234	10137	24	0.00	0.00
18235	10138	24	0.00	0.00
18236	10139	24	0.00	0.00
18237	10140	24	0.00	0.00
18238	10141	24	0.00	0.00
18239	10142	24	0.00	0.00
18240	10143	24	0.00	0.00
18241	10144	24	0.00	0.00
18242	10145	24	0.00	0.00
18243	10146	24	0.00	0.00
18244	10147	24	0.00	0.00
18245	10148	24	0.00	0.00
18246	10149	24	0.00	0.00
18247	10150	24	0.00	0.00
18248	10151	24	0.00	0.00
18249	10152	24	0.00	0.00
18250	10153	24	0.00	0.00
18251	10154	24	0.00	0.00
18252	10155	24	0.00	0.00
18253	10156	24	0.00	0.00
18254	10157	24	0.00	0.00
18255	10158	24	0.00	0.00
18256	10159	24	0.00	0.00
18257	10160	24	0.00	0.00
18258	10161	24	0.00	0.00
18259	10162	24	0.00	0.00
18260	10163	24	0.00	0.00
18261	10164	24	0.00	0.00
18262	10165	24	0.00	0.00
18263	10166	24	0.00	0.00
18264	10167	24	0.00	0.00
18265	10168	24	0.00	0.00
18266	10169	24	0.00	0.00
18267	10170	24	0.00	0.00
18268	10171	24	0.00	0.00
18269	10172	24	0.00	0.00
18270	10173	24	0.00	0.00
18271	10174	24	0.00	0.00
18272	10175	24	0.00	0.00
18273	10176	24	0.00	0.00
18274	10177	24	0.00	0.00
18275	10178	24	0.00	0.00
18276	10179	24	0.00	0.00
18277	10180	24	0.00	0.00
18278	10181	24	0.00	0.00
18279	10182	24	0.00	0.00
18280	10183	24	0.00	0.00
18281	10184	24	0.00	0.00
18282	10185	24	0.00	0.00
18283	10186	24	0.00	0.00
18284	10187	24	0.00	0.00
18285	10188	24	0.00	0.00
18286	10189	24	0.00	0.00
18287	10190	24	0.00	0.00
18288	10191	24	0.00	0.00
18289	10192	24	0.00	0.00
18290	10193	24	0.00	0.00
18291	10194	24	0.00	0.00
18292	10195	24	0.00	0.00
18293	10196	24	0.00	0.00
18294	10197	24	0.00	0.00
18295	10198	24	0.00	0.00
18296	10199	24	0.00	0.00
18297	10200	24	0.00	0.00
18298	10201	24	0.00	0.00
18299	10202	24	0.00	0.00
18300	10203	24	0.00	0.00
18301	10204	24	0.00	0.00
18302	10205	24	0.00	0.00
18303	10206	24	0.00	0.00
18304	10207	24	0.00	0.00
18305	10208	24	0.00	0.00
18306	10209	24	0.00	0.00
18307	10210	24	0.00	0.00
18308	10211	24	0.00	0.00
18309	10212	24	0.00	0.00
18310	10213	24	0.00	0.00
18311	10214	24	0.00	0.00
18312	10215	24	0.00	0.00
18313	10216	24	0.00	0.00
18314	10217	24	0.00	0.00
18315	10218	24	0.00	0.00
18316	10219	24	0.00	0.00
18317	10220	24	0.00	0.00
18318	10221	24	0.00	0.00
18319	10222	24	0.00	0.00
18320	10223	24	0.00	0.00
18321	10224	24	0.00	0.00
18322	10225	24	0.00	0.00
18323	10226	24	0.00	0.00
18324	10227	24	0.00	0.00
18325	10228	24	0.00	0.00
18326	10229	24	0.00	0.00
18327	10230	24	0.00	0.00
18328	10231	24	0.00	0.00
18329	10232	24	0.00	0.00
18330	10233	24	0.00	0.00
18331	10234	24	0.00	0.00
18332	10235	24	0.00	0.00
18333	10236	24	0.00	0.00
18334	10237	24	0.00	0.00
18335	10238	24	0.00	0.00
18336	10239	24	0.00	0.00
18337	10240	24	0.00	0.00
18338	10241	24	0.00	0.00
18339	10242	24	0.00	0.00
18340	10244	24	0.00	0.00
18341	10245	24	0.00	0.00
18342	10246	24	0.00	0.00
18343	10247	24	0.00	0.00
18344	10248	24	0.00	0.00
18345	10250	24	0.00	0.00
18346	10251	24	0.00	0.00
18348	10253	24	0.00	0.00
18349	10254	24	0.00	0.00
18350	10255	24	0.00	0.00
18351	10256	24	0.00	0.00
18352	10257	24	0.00	0.00
18353	10258	24	0.00	0.00
18354	10259	24	0.00	0.00
18355	10260	24	0.00	0.00
18356	10261	24	0.00	0.00
18357	10262	24	0.00	0.00
18358	10263	24	0.00	0.00
18359	10264	24	0.00	0.00
18360	10265	24	0.00	0.00
18361	10266	24	0.00	0.00
18362	10267	24	0.00	0.00
18363	10268	24	0.00	0.00
18364	10269	24	0.00	0.00
18365	10272	24	0.00	0.00
18366	10273	24	0.00	0.00
18367	10274	24	0.00	0.00
18368	10275	24	0.00	0.00
18369	10276	24	0.00	0.00
18370	10277	24	0.00	0.00
18371	10278	24	0.00	0.00
18372	10279	24	0.00	0.00
18373	10280	24	0.00	0.00
18374	10281	24	0.00	0.00
18375	10282	24	0.00	0.00
18376	10283	24	0.00	0.00
18377	10284	24	0.00	0.00
18378	10285	24	0.00	0.00
18379	10286	24	0.00	0.00
18380	10287	24	0.00	0.00
18381	10288	24	0.00	0.00
18382	10289	24	0.00	0.00
18383	10290	24	0.00	0.00
18384	10291	24	0.00	0.00
18385	10292	24	0.00	0.00
18386	10293	24	0.00	0.00
18387	10294	24	0.00	0.00
18388	10295	24	0.00	0.00
18389	10296	24	0.00	0.00
18390	10297	24	0.00	0.00
18391	10299	24	0.00	0.00
18392	10300	24	0.00	0.00
18393	10301	24	0.00	0.00
18394	10303	24	0.00	0.00
18395	10304	24	0.00	0.00
18396	10305	24	0.00	0.00
18397	10306	24	0.00	0.00
18398	10307	24	0.00	0.00
18399	10309	24	0.00	0.00
18400	10310	24	0.00	0.00
18401	10311	24	0.00	0.00
18402	10312	24	0.00	0.00
18403	10313	24	0.00	0.00
18404	10314	24	0.00	0.00
18405	10315	24	0.00	0.00
18406	10316	24	0.00	0.00
18407	10317	24	0.00	0.00
18408	10319	24	0.00	0.00
18409	10321	24	0.00	0.00
18410	10322	24	0.00	0.00
18411	10323	24	0.00	0.00
18412	10324	24	0.00	0.00
18413	10325	24	0.00	0.00
18414	10326	24	0.00	0.00
18415	10327	24	0.00	0.00
18416	10328	24	0.00	0.00
18417	10329	24	0.00	0.00
18418	10330	24	0.00	0.00
18419	10331	24	0.00	0.00
18420	10332	24	0.00	0.00
18421	10333	24	0.00	0.00
18422	10334	24	0.00	0.00
18423	10335	24	0.00	0.00
18424	10336	24	0.00	0.00
18425	10337	24	0.00	0.00
18426	10338	24	0.00	0.00
18427	10339	24	0.00	0.00
18428	10340	24	0.00	0.00
18429	10341	24	0.00	0.00
18430	10342	24	0.00	0.00
18431	10343	24	0.00	0.00
18432	10344	24	0.00	0.00
18433	10345	24	0.00	0.00
18434	10346	24	0.00	0.00
18435	10347	24	0.00	0.00
18436	10348	24	0.00	0.00
18437	10349	24	0.00	0.00
18438	10350	24	0.00	0.00
18439	10351	24	0.00	0.00
18440	10352	24	0.00	0.00
18441	10354	24	0.00	0.00
18442	10353	24	0.00	0.00
18443	10355	24	0.00	0.00
18444	10356	24	0.00	0.00
18445	10357	24	0.00	0.00
18446	10358	24	0.00	0.00
18447	10359	24	0.00	0.00
18448	10360	24	0.00	0.00
18449	10361	24	0.00	0.00
18450	10362	24	0.00	0.00
18451	10363	24	0.00	0.00
18452	10364	24	0.00	0.00
18453	10365	24	0.00	0.00
18454	10366	24	0.00	0.00
18455	10367	24	0.00	0.00
18456	10368	24	0.00	0.00
18457	10369	24	0.00	0.00
18458	10370	24	0.00	0.00
18459	10371	24	0.00	0.00
18460	10372	24	0.00	0.00
18461	10373	24	0.00	0.00
18462	10374	24	0.00	0.00
18463	10375	24	0.00	0.00
18464	10376	24	0.00	0.00
18465	10377	24	0.00	0.00
18466	10378	24	0.00	0.00
18467	10379	24	0.00	0.00
18468	10380	24	0.00	0.00
18469	10381	24	0.00	0.00
18470	10382	24	0.00	0.00
18471	10383	24	0.00	0.00
18472	10384	24	0.00	0.00
18473	10385	24	0.00	0.00
18474	10386	24	0.00	0.00
18475	10387	24	0.00	0.00
18476	10388	24	0.00	0.00
18477	10389	24	0.00	0.00
18478	10390	24	0.00	0.00
18479	10391	24	0.00	0.00
18480	10392	24	0.00	0.00
18481	10393	24	0.00	0.00
18482	10394	24	0.00	0.00
18483	10395	24	0.00	0.00
18484	10397	24	0.00	0.00
18485	10399	24	0.00	0.00
18486	10400	24	0.00	0.00
18487	10401	24	0.00	0.00
18488	10402	24	0.00	0.00
18489	10403	24	0.00	0.00
18490	10404	24	0.00	0.00
18491	10405	24	0.00	0.00
18492	10406	24	0.00	0.00
18493	10407	24	0.00	0.00
18494	10408	24	0.00	0.00
18495	10409	24	0.00	0.00
18496	10410	24	0.00	0.00
18497	10411	24	0.00	0.00
18498	10412	24	0.00	0.00
18499	10413	24	0.00	0.00
18500	10415	24	0.00	0.00
18501	10416	24	0.00	0.00
18502	10417	24	0.00	0.00
18503	10418	24	0.00	0.00
18504	10419	24	0.00	0.00
18505	10420	24	0.00	0.00
18506	10421	24	0.00	0.00
18507	10422	24	0.00	0.00
18508	10423	24	0.00	0.00
18509	10424	24	0.00	0.00
18510	10425	24	0.00	0.00
18511	10426	24	0.00	0.00
18512	10427	24	0.00	0.00
18513	10428	24	0.00	0.00
18514	10429	24	0.00	0.00
18515	10430	24	0.00	0.00
18516	10431	24	0.00	0.00
18517	10432	24	0.00	0.00
18518	10433	24	0.00	0.00
18519	10434	24	0.00	0.00
18520	10435	24	0.00	0.00
18521	10436	24	0.00	0.00
18522	10437	24	0.00	0.00
18523	10438	24	0.00	0.00
18524	10439	24	0.00	0.00
18525	10440	24	0.00	0.00
18526	10441	24	0.00	0.00
18527	10442	24	0.00	0.00
18528	10443	24	0.00	0.00
18529	10444	24	0.00	0.00
18530	10445	24	0.00	0.00
18531	10446	24	0.00	0.00
18532	10447	24	0.00	0.00
18533	10448	24	0.00	0.00
18534	10449	24	0.00	0.00
18535	10450	24	0.00	0.00
18536	10451	24	0.00	0.00
18537	10452	24	0.00	0.00
18538	10453	24	0.00	0.00
18539	10454	24	0.00	0.00
18540	10455	24	0.00	0.00
18541	10456	24	0.00	0.00
18542	10457	24	0.00	0.00
18543	10458	24	0.00	0.00
18544	10459	24	0.00	0.00
18545	10460	24	0.00	0.00
18546	10461	24	0.00	0.00
18547	10462	24	0.00	0.00
18548	10463	24	0.00	0.00
18549	10464	24	0.00	0.00
18550	10465	24	0.00	0.00
18551	10466	24	0.00	0.00
18552	10467	24	0.00	0.00
18553	10468	24	0.00	0.00
18554	10469	24	0.00	0.00
18555	10470	24	0.00	0.00
18556	10471	24	0.00	0.00
18557	10472	24	0.00	0.00
18558	10473	24	0.00	0.00
18559	10474	24	0.00	0.00
18560	10475	24	0.00	0.00
18561	10476	24	0.00	0.00
18562	10477	24	0.00	0.00
18563	10478	24	0.00	0.00
18564	10479	24	0.00	0.00
18565	10480	24	0.00	0.00
18566	10481	24	0.00	0.00
18567	10482	24	0.00	0.00
18568	10483	24	0.00	0.00
18569	10484	24	0.00	0.00
18570	10485	24	0.00	0.00
18571	10486	24	0.00	0.00
18572	10487	24	0.00	0.00
18573	10488	24	0.00	0.00
18574	10489	24	0.00	0.00
18575	10490	24	0.00	0.00
18576	10491	24	0.00	0.00
18577	10492	24	0.00	0.00
18578	10493	24	0.00	0.00
18579	10494	24	0.00	0.00
18580	10495	24	0.00	0.00
18581	10496	24	0.00	0.00
18582	10497	24	0.00	0.00
18583	10498	24	0.00	0.00
18584	10499	24	0.00	0.00
18585	10500	24	0.00	0.00
18586	10501	24	0.00	0.00
18587	10502	24	0.00	0.00
18588	10503	24	0.00	0.00
18589	10504	24	0.00	0.00
18590	10505	24	0.00	0.00
18591	10506	24	0.00	0.00
18592	10507	24	0.00	0.00
18593	10508	24	0.00	0.00
18594	10509	24	0.00	0.00
18595	10510	24	0.00	0.00
18596	10511	24	0.00	0.00
18597	10512	24	0.00	0.00
18598	10513	24	0.00	0.00
18599	10514	24	0.00	0.00
18600	10515	24	0.00	0.00
18601	10516	24	0.00	0.00
18602	10517	24	0.00	0.00
18603	10518	24	0.00	0.00
18604	10519	24	0.00	0.00
18605	10520	24	0.00	0.00
18606	10521	24	0.00	0.00
18607	10522	24	0.00	0.00
18608	10523	24	0.00	0.00
18609	10524	24	0.00	0.00
18610	10525	24	0.00	0.00
18611	10526	24	0.00	0.00
18612	10527	24	0.00	0.00
18613	10528	24	0.00	0.00
18614	10529	24	0.00	0.00
18615	10530	24	0.00	0.00
18616	10531	24	0.00	0.00
18617	10532	24	0.00	0.00
18618	10533	24	0.00	0.00
18619	10535	24	0.00	0.00
18620	10536	24	0.00	0.00
18621	10537	24	0.00	0.00
18622	10538	24	0.00	0.00
18623	10539	24	0.00	0.00
18624	10540	24	0.00	0.00
18625	10541	24	0.00	0.00
18626	10542	24	0.00	0.00
18627	10543	24	0.00	0.00
18628	10544	24	0.00	0.00
18629	10545	24	0.00	0.00
18630	10546	24	0.00	0.00
18631	10547	24	0.00	0.00
18632	10548	24	0.00	0.00
18633	10549	24	0.00	0.00
18634	10550	24	0.00	0.00
18635	10551	24	0.00	0.00
18636	10552	24	0.00	0.00
18637	10553	24	0.00	0.00
18638	10554	24	0.00	0.00
18639	10555	24	0.00	0.00
18640	10556	24	0.00	0.00
18641	10557	24	0.00	0.00
18642	10558	24	0.00	0.00
18643	10559	24	0.00	0.00
18644	10560	24	0.00	0.00
18645	10561	24	0.00	0.00
18646	10562	24	0.00	0.00
18647	10563	24	0.00	0.00
18648	10564	24	0.00	0.00
18649	10565	24	0.00	0.00
18650	10566	24	0.00	0.00
18651	10567	24	0.00	0.00
18652	10568	24	0.00	0.00
18653	10569	24	0.00	0.00
18654	10570	24	0.00	0.00
18655	10571	24	0.00	0.00
18656	10572	24	0.00	0.00
18657	10573	24	0.00	0.00
18658	10574	24	0.00	0.00
18659	10575	24	0.00	0.00
18660	10576	24	0.00	0.00
18661	10577	24	0.00	0.00
18662	10578	24	0.00	0.00
18663	10579	24	0.00	0.00
18664	10580	24	0.00	0.00
18665	10581	24	0.00	0.00
18666	10582	24	0.00	0.00
18667	10583	24	0.00	0.00
18668	10584	24	0.00	0.00
18669	10585	24	0.00	0.00
18670	10586	24	0.00	0.00
18671	10587	24	0.00	0.00
18672	10588	24	0.00	0.00
18673	10589	24	0.00	0.00
18674	10590	24	0.00	0.00
18675	10591	24	0.00	0.00
18676	10592	24	0.00	0.00
18677	10593	24	0.00	0.00
18678	10594	24	0.00	0.00
18679	10595	24	0.00	0.00
18680	10596	24	0.00	0.00
18681	10597	24	0.00	0.00
18682	10598	24	0.00	0.00
18683	10599	24	0.00	0.00
18684	10600	24	0.00	0.00
18685	10601	24	0.00	0.00
18686	10602	24	0.00	0.00
18687	10603	24	0.00	0.00
18688	10604	24	0.00	0.00
18689	10605	24	0.00	0.00
18690	10606	24	0.00	0.00
18691	10607	24	0.00	0.00
18692	10608	24	0.00	0.00
18693	10609	24	0.00	0.00
18694	10610	24	0.00	0.00
18695	10611	24	0.00	0.00
18696	10612	24	0.00	0.00
18697	10613	24	0.00	0.00
18698	10614	24	0.00	0.00
18699	10615	24	0.00	0.00
18700	10616	24	0.00	0.00
18701	10617	24	0.00	0.00
18702	10618	24	0.00	0.00
18703	10619	24	0.00	0.00
18704	10620	24	0.00	0.00
18705	10621	24	0.00	0.00
18706	10622	24	0.00	0.00
18707	10623	24	0.00	0.00
18708	10624	24	0.00	0.00
18709	10625	24	0.00	0.00
18710	10626	24	0.00	0.00
18711	10627	24	0.00	0.00
18712	10628	24	0.00	0.00
18713	10629	24	0.00	0.00
18714	10630	24	0.00	0.00
18715	10631	24	0.00	0.00
18848	10414	24	0.00	0.00
18849	10047	24	0.00	0.00
18850	10308	24	0.00	0.00
18851	10302	24	0.00	0.00
18874	10396	24	0.00	0.00
18883	10249	24	0.00	0.00
18884	10243	24	0.00	0.00
18885	10029	24	0.00	0.00
18887	10398	24	0.00	0.00
18888	10044	24	0.00	0.00
12587	10249	16	0.00	0.00
12588	10243	16	0.00	0.00
12589	10029	16	0.00	0.00
12576	10396	16	0.00	0.00
21226	10534	27	0.00	0.00
21227	10057	27	0.00	0.00
21236	10318	27	0.00	0.00
21237	10298	27	0.00	0.00
21238	10034	27	0.00	0.00
21240	10271	27	0.00	0.00
21241	10270	27	0.00	0.00
21242	10031	27	0.00	0.00
12577	10318	16	0.00	0.00
12578	10298	16	0.00	0.00
12579	10034	16	0.00	0.00
12581	10271	16	0.00	0.00
12582	10270	16	0.00	0.00
12583	10031	16	0.00	0.00
12584	10534	16	0.00	0.00
12585	10057	16	0.00	0.00
20463	10078	27	0.00	0.00
20464	10320	27	0.00	0.00
20466	10003	27	0.00	0.00
20468	10002	27	0.00	0.00
20469	10004	27	0.00	0.00
20470	10005	27	0.00	0.00
20471	10006	27	0.00	0.00
20472	10007	27	0.00	0.00
20473	10008	27	0.00	0.00
20474	10009	27	0.00	0.00
20475	10010	27	0.00	0.00
20476	10011	27	0.00	0.00
20477	10012	27	0.00	0.00
20478	10013	27	0.00	0.00
20479	10014	27	0.00	0.00
20480	10015	27	0.00	0.00
20481	10016	27	0.00	0.00
20482	10017	27	0.00	0.00
20483	10018	27	0.00	0.00
20484	10019	27	0.00	0.00
20485	10020	27	0.00	0.00
20486	10021	27	0.00	0.00
20487	10022	27	0.00	0.00
20488	10023	27	0.00	0.00
20489	10024	27	0.00	0.00
20490	10025	27	0.00	0.00
20491	10026	27	0.00	0.00
20492	10027	27	0.00	0.00
20493	10028	27	0.00	0.00
20494	10030	27	0.00	0.00
20495	10032	27	0.00	0.00
20496	10033	27	0.00	0.00
20497	10035	27	0.00	0.00
20498	10036	27	0.00	0.00
20499	10037	27	0.00	0.00
20500	10038	27	0.00	0.00
20501	10039	27	0.00	0.00
20502	10040	27	0.00	0.00
20503	10041	27	0.00	0.00
20504	10042	27	0.00	0.00
20505	10043	27	0.00	0.00
20506	10045	27	0.00	0.00
20507	10046	27	0.00	0.00
20508	10048	27	0.00	0.00
20509	10049	27	0.00	0.00
20510	10050	27	0.00	0.00
20511	10051	27	0.00	0.00
20512	10052	27	0.00	0.00
20513	10053	27	0.00	0.00
20514	10054	27	0.00	0.00
20515	10055	27	0.00	0.00
20516	10056	27	0.00	0.00
20517	10058	27	0.00	0.00
20518	10059	27	0.00	0.00
20519	10060	27	0.00	0.00
20520	10061	27	0.00	0.00
20521	10062	27	0.00	0.00
20522	10063	27	0.00	0.00
20523	10064	27	0.00	0.00
20525	10066	27	0.00	0.00
20526	10067	27	0.00	0.00
20528	10069	27	0.00	0.00
20529	10070	27	0.00	0.00
20530	10071	27	0.00	0.00
20531	10072	27	0.00	0.00
20532	10073	27	0.00	0.00
20533	10074	27	0.00	0.00
20534	10075	27	0.00	0.00
20535	10076	27	0.00	0.00
20536	10077	27	0.00	0.00
20537	10079	27	0.00	0.00
20538	10080	27	0.00	0.00
20539	10081	27	0.00	0.00
20540	10082	27	0.00	0.00
20541	10083	27	0.00	0.00
20542	10084	27	0.00	0.00
20543	10085	27	0.00	0.00
20544	10086	27	0.00	0.00
20545	10087	27	0.00	0.00
20546	10088	27	0.00	0.00
20547	10089	27	0.00	0.00
20548	10090	27	0.00	0.00
20549	10091	27	0.00	0.00
20550	10092	27	0.00	0.00
20551	10093	27	0.00	0.00
20552	10094	27	0.00	0.00
20553	10095	27	0.00	0.00
20554	10096	27	0.00	0.00
20555	10097	27	0.00	0.00
20556	10098	27	0.00	0.00
20557	10099	27	0.00	0.00
20558	10100	27	0.00	0.00
20559	10101	27	0.00	0.00
20560	10102	27	0.00	0.00
20561	10103	27	0.00	0.00
20562	10104	27	0.00	0.00
20563	10105	27	0.00	0.00
20564	10106	27	0.00	0.00
20565	10107	27	0.00	0.00
20566	10108	27	0.00	0.00
20567	10109	27	0.00	0.00
20568	10110	27	0.00	0.00
20569	10111	27	0.00	0.00
20570	10112	27	0.00	0.00
20571	10113	27	0.00	0.00
20572	10114	27	0.00	0.00
20573	10115	27	0.00	0.00
20574	10116	27	0.00	0.00
20575	10117	27	0.00	0.00
20576	10118	27	0.00	0.00
20577	10119	27	0.00	0.00
20578	10120	27	0.00	0.00
20579	10121	27	0.00	0.00
20580	10122	27	0.00	0.00
20581	10123	27	0.00	0.00
20582	10124	27	0.00	0.00
20583	10125	27	0.00	0.00
20584	10126	27	0.00	0.00
20585	10127	27	0.00	0.00
20586	10128	27	0.00	0.00
20587	10129	27	0.00	0.00
20588	10130	27	0.00	0.00
20589	10131	27	0.00	0.00
20590	10132	27	0.00	0.00
20591	10133	27	0.00	0.00
20592	10134	27	0.00	0.00
20593	10135	27	0.00	0.00
20594	10136	27	0.00	0.00
20595	10137	27	0.00	0.00
20596	10138	27	0.00	0.00
20597	10139	27	0.00	0.00
20598	10140	27	0.00	0.00
20599	10141	27	0.00	0.00
20600	10142	27	0.00	0.00
20601	10143	27	0.00	0.00
20602	10144	27	0.00	0.00
20603	10145	27	0.00	0.00
20604	10146	27	0.00	0.00
20605	10147	27	0.00	0.00
20606	10148	27	0.00	0.00
20607	10149	27	0.00	0.00
20608	10150	27	0.00	0.00
20609	10151	27	0.00	0.00
20610	10152	27	0.00	0.00
20611	10153	27	0.00	0.00
20612	10154	27	0.00	0.00
20613	10155	27	0.00	0.00
20614	10156	27	0.00	0.00
20615	10157	27	0.00	0.00
20616	10158	27	0.00	0.00
20617	10159	27	0.00	0.00
20618	10160	27	0.00	0.00
20619	10161	27	0.00	0.00
20620	10162	27	0.00	0.00
20621	10163	27	0.00	0.00
20622	10164	27	0.00	0.00
20623	10165	27	0.00	0.00
20624	10166	27	0.00	0.00
20625	10167	27	0.00	0.00
20626	10168	27	0.00	0.00
20627	10169	27	0.00	0.00
20628	10170	27	0.00	0.00
20629	10171	27	0.00	0.00
20630	10172	27	0.00	0.00
20631	10173	27	0.00	0.00
20632	10174	27	0.00	0.00
20633	10175	27	0.00	0.00
20634	10176	27	0.00	0.00
20635	10177	27	0.00	0.00
20636	10178	27	0.00	0.00
20637	10179	27	0.00	0.00
20638	10180	27	0.00	0.00
20639	10181	27	0.00	0.00
20640	10182	27	0.00	0.00
20641	10183	27	0.00	0.00
20642	10184	27	0.00	0.00
20643	10185	27	0.00	0.00
20644	10186	27	0.00	0.00
20645	10187	27	0.00	0.00
20646	10188	27	0.00	0.00
20647	10189	27	0.00	0.00
20648	10190	27	0.00	0.00
20649	10191	27	0.00	0.00
20650	10192	27	0.00	0.00
20651	10193	27	0.00	0.00
20652	10194	27	0.00	0.00
20653	10195	27	0.00	0.00
20654	10196	27	0.00	0.00
20655	10197	27	0.00	0.00
20656	10198	27	0.00	0.00
20657	10199	27	0.00	0.00
20658	10200	27	0.00	0.00
20659	10201	27	0.00	0.00
20660	10202	27	0.00	0.00
20661	10203	27	0.00	0.00
20662	10204	27	0.00	0.00
20663	10205	27	0.00	0.00
20664	10206	27	0.00	0.00
20665	10207	27	0.00	0.00
20666	10208	27	0.00	0.00
20667	10209	27	0.00	0.00
20668	10210	27	0.00	0.00
20669	10211	27	0.00	0.00
20670	10212	27	0.00	0.00
20671	10213	27	0.00	0.00
20672	10214	27	0.00	0.00
20673	10215	27	0.00	0.00
20674	10216	27	0.00	0.00
20675	10217	27	0.00	0.00
20676	10218	27	0.00	0.00
20677	10219	27	0.00	0.00
20678	10220	27	0.00	0.00
20679	10221	27	0.00	0.00
20680	10222	27	0.00	0.00
20681	10223	27	0.00	0.00
20682	10224	27	0.00	0.00
20683	10225	27	0.00	0.00
20684	10226	27	0.00	0.00
20685	10227	27	0.00	0.00
20686	10228	27	0.00	0.00
20687	10229	27	0.00	0.00
20688	10230	27	0.00	0.00
20689	10231	27	0.00	0.00
20690	10232	27	0.00	0.00
20691	10233	27	0.00	0.00
20692	10234	27	0.00	0.00
20693	10235	27	0.00	0.00
20694	10236	27	0.00	0.00
20695	10237	27	0.00	0.00
20696	10238	27	0.00	0.00
20697	10239	27	0.00	0.00
20698	10240	27	0.00	0.00
20699	10241	27	0.00	0.00
20700	10242	27	0.00	0.00
20701	10244	27	0.00	0.00
20702	10245	27	0.00	0.00
20703	10246	27	0.00	0.00
20704	10247	27	0.00	0.00
20705	10248	27	0.00	0.00
20706	10250	27	0.00	0.00
20707	10251	27	0.00	0.00
20709	10253	27	0.00	0.00
20710	10254	27	0.00	0.00
20711	10255	27	0.00	0.00
20712	10256	27	0.00	0.00
20713	10257	27	0.00	0.00
20714	10258	27	0.00	0.00
20715	10259	27	0.00	0.00
20716	10260	27	0.00	0.00
20717	10261	27	0.00	0.00
20718	10262	27	0.00	0.00
20719	10263	27	0.00	0.00
20720	10264	27	0.00	0.00
20721	10265	27	0.00	0.00
20722	10266	27	0.00	0.00
20723	10267	27	0.00	0.00
20724	10268	27	0.00	0.00
20725	10269	27	0.00	0.00
20726	10272	27	0.00	0.00
20727	10273	27	0.00	0.00
20728	10274	27	0.00	0.00
20729	10275	27	0.00	0.00
20730	10276	27	0.00	0.00
20731	10277	27	0.00	0.00
20732	10278	27	0.00	0.00
20733	10279	27	0.00	0.00
20734	10280	27	0.00	0.00
20735	10281	27	0.00	0.00
20736	10282	27	0.00	0.00
20737	10283	27	0.00	0.00
20738	10284	27	0.00	0.00
20739	10285	27	0.00	0.00
20740	10286	27	0.00	0.00
20741	10287	27	0.00	0.00
20742	10288	27	0.00	0.00
20743	10289	27	0.00	0.00
20744	10290	27	0.00	0.00
20745	10291	27	0.00	0.00
20746	10292	27	0.00	0.00
20747	10293	27	0.00	0.00
20748	10294	27	0.00	0.00
20749	10295	27	0.00	0.00
20750	10296	27	0.00	0.00
20751	10297	27	0.00	0.00
20752	10299	27	0.00	0.00
20753	10300	27	0.00	0.00
20754	10301	27	0.00	0.00
20755	10303	27	0.00	0.00
20756	10304	27	0.00	0.00
20757	10305	27	0.00	0.00
20758	10306	27	0.00	0.00
20759	10307	27	0.00	0.00
20760	10309	27	0.00	0.00
20761	10310	27	0.00	0.00
20762	10311	27	0.00	0.00
20763	10312	27	0.00	0.00
20764	10313	27	0.00	0.00
20765	10314	27	0.00	0.00
20766	10315	27	0.00	0.00
20767	10316	27	0.00	0.00
20768	10317	27	0.00	0.00
20769	10319	27	0.00	0.00
20770	10321	27	0.00	0.00
20771	10322	27	0.00	0.00
20772	10323	27	0.00	0.00
20773	10324	27	0.00	0.00
20774	10325	27	0.00	0.00
20775	10326	27	0.00	0.00
20776	10327	27	0.00	0.00
20777	10328	27	0.00	0.00
20778	10329	27	0.00	0.00
20779	10330	27	0.00	0.00
20780	10331	27	0.00	0.00
20781	10332	27	0.00	0.00
20782	10333	27	0.00	0.00
20783	10334	27	0.00	0.00
20784	10335	27	0.00	0.00
20785	10336	27	0.00	0.00
20786	10337	27	0.00	0.00
20787	10338	27	0.00	0.00
20788	10339	27	0.00	0.00
20789	10340	27	0.00	0.00
20790	10341	27	0.00	0.00
20791	10342	27	0.00	0.00
20792	10343	27	0.00	0.00
20793	10344	27	0.00	0.00
20794	10345	27	0.00	0.00
20795	10346	27	0.00	0.00
20796	10347	27	0.00	0.00
20797	10348	27	0.00	0.00
20798	10349	27	0.00	0.00
20799	10350	27	0.00	0.00
20800	10351	27	0.00	0.00
20801	10352	27	0.00	0.00
20802	10354	27	0.00	0.00
20803	10353	27	0.00	0.00
20804	10355	27	0.00	0.00
20805	10356	27	0.00	0.00
20806	10357	27	0.00	0.00
20807	10358	27	0.00	0.00
20808	10359	27	0.00	0.00
20809	10360	27	0.00	0.00
20810	10361	27	0.00	0.00
20811	10362	27	0.00	0.00
20812	10363	27	0.00	0.00
20813	10364	27	0.00	0.00
20814	10365	27	0.00	0.00
20815	10366	27	0.00	0.00
20816	10367	27	0.00	0.00
20817	10368	27	0.00	0.00
20818	10369	27	0.00	0.00
20819	10370	27	0.00	0.00
20820	10371	27	0.00	0.00
20821	10372	27	0.00	0.00
20822	10373	27	0.00	0.00
20823	10374	27	0.00	0.00
20824	10375	27	0.00	0.00
20825	10376	27	0.00	0.00
20826	10377	27	0.00	0.00
20827	10378	27	0.00	0.00
20828	10379	27	0.00	0.00
20829	10380	27	0.00	0.00
20830	10381	27	0.00	0.00
20831	10382	27	0.00	0.00
20832	10383	27	0.00	0.00
20833	10384	27	0.00	0.00
20834	10385	27	0.00	0.00
20835	10386	27	0.00	0.00
20836	10387	27	0.00	0.00
20837	10388	27	0.00	0.00
20838	10389	27	0.00	0.00
20839	10390	27	0.00	0.00
20840	10391	27	0.00	0.00
20841	10392	27	0.00	0.00
20842	10393	27	0.00	0.00
20843	10394	27	0.00	0.00
20844	10395	27	0.00	0.00
20845	10397	27	0.00	0.00
20846	10399	27	0.00	0.00
20847	10400	27	0.00	0.00
20848	10401	27	0.00	0.00
20849	10402	27	0.00	0.00
20850	10403	27	0.00	0.00
20851	10404	27	0.00	0.00
20852	10405	27	0.00	0.00
20853	10406	27	0.00	0.00
20854	10407	27	0.00	0.00
20855	10408	27	0.00	0.00
20856	10409	27	0.00	0.00
20857	10410	27	0.00	0.00
20858	10411	27	0.00	0.00
20859	10412	27	0.00	0.00
20860	10413	27	0.00	0.00
20861	10415	27	0.00	0.00
20862	10416	27	0.00	0.00
20863	10417	27	0.00	0.00
20864	10418	27	0.00	0.00
20865	10419	27	0.00	0.00
20866	10420	27	0.00	0.00
20867	10421	27	0.00	0.00
20868	10422	27	0.00	0.00
20869	10423	27	0.00	0.00
20870	10424	27	0.00	0.00
20871	10425	27	0.00	0.00
20872	10426	27	0.00	0.00
20873	10427	27	0.00	0.00
20874	10428	27	0.00	0.00
20875	10429	27	0.00	0.00
20876	10430	27	0.00	0.00
20877	10431	27	0.00	0.00
20878	10432	27	0.00	0.00
20879	10433	27	0.00	0.00
20880	10434	27	0.00	0.00
20881	10435	27	0.00	0.00
20882	10436	27	0.00	0.00
20883	10437	27	0.00	0.00
20884	10438	27	0.00	0.00
20885	10439	27	0.00	0.00
20886	10440	27	0.00	0.00
20887	10441	27	0.00	0.00
20888	10442	27	0.00	0.00
20889	10443	27	0.00	0.00
20890	10444	27	0.00	0.00
20891	10445	27	0.00	0.00
20892	10446	27	0.00	0.00
20893	10447	27	0.00	0.00
20894	10448	27	0.00	0.00
20895	10449	27	0.00	0.00
20896	10450	27	0.00	0.00
20897	10451	27	0.00	0.00
20898	10452	27	0.00	0.00
20899	10453	27	0.00	0.00
20900	10454	27	0.00	0.00
20901	10455	27	0.00	0.00
20902	10456	27	0.00	0.00
20903	10457	27	0.00	0.00
20904	10458	27	0.00	0.00
20905	10459	27	0.00	0.00
20906	10460	27	0.00	0.00
20907	10461	27	0.00	0.00
20908	10462	27	0.00	0.00
20909	10463	27	0.00	0.00
20910	10464	27	0.00	0.00
20911	10465	27	0.00	0.00
20912	10466	27	0.00	0.00
20913	10467	27	0.00	0.00
20914	10468	27	0.00	0.00
20915	10469	27	0.00	0.00
20916	10470	27	0.00	0.00
20917	10471	27	0.00	0.00
20918	10472	27	0.00	0.00
20919	10473	27	0.00	0.00
20920	10474	27	0.00	0.00
20921	10475	27	0.00	0.00
20922	10476	27	0.00	0.00
20923	10477	27	0.00	0.00
20924	10478	27	0.00	0.00
20925	10479	27	0.00	0.00
20926	10480	27	0.00	0.00
20927	10481	27	0.00	0.00
20928	10482	27	0.00	0.00
20929	10483	27	0.00	0.00
20930	10484	27	0.00	0.00
20931	10485	27	0.00	0.00
20932	10486	27	0.00	0.00
20933	10487	27	0.00	0.00
20934	10488	27	0.00	0.00
20935	10489	27	0.00	0.00
20936	10490	27	0.00	0.00
20937	10491	27	0.00	0.00
20938	10492	27	0.00	0.00
20939	10493	27	0.00	0.00
20940	10494	27	0.00	0.00
20941	10495	27	0.00	0.00
20942	10496	27	0.00	0.00
20943	10497	27	0.00	0.00
20944	10498	27	0.00	0.00
20945	10499	27	0.00	0.00
20946	10500	27	0.00	0.00
20947	10501	27	0.00	0.00
20948	10502	27	0.00	0.00
20949	10503	27	0.00	0.00
20950	10504	27	0.00	0.00
20951	10505	27	0.00	0.00
20952	10506	27	0.00	0.00
20953	10507	27	0.00	0.00
20954	10508	27	0.00	0.00
20955	10509	27	0.00	0.00
20956	10510	27	0.00	0.00
20957	10511	27	0.00	0.00
20958	10512	27	0.00	0.00
20959	10513	27	0.00	0.00
20960	10514	27	0.00	0.00
20961	10515	27	0.00	0.00
20962	10516	27	0.00	0.00
20963	10517	27	0.00	0.00
20964	10518	27	0.00	0.00
20965	10519	27	0.00	0.00
20966	10520	27	0.00	0.00
20967	10521	27	0.00	0.00
20968	10522	27	0.00	0.00
20969	10523	27	0.00	0.00
20970	10524	27	0.00	0.00
20971	10525	27	0.00	0.00
20972	10526	27	0.00	0.00
20973	10527	27	0.00	0.00
20974	10528	27	0.00	0.00
20975	10529	27	0.00	0.00
20976	10530	27	0.00	0.00
20977	10531	27	0.00	0.00
20978	10532	27	0.00	0.00
20979	10533	27	0.00	0.00
20980	10535	27	0.00	0.00
20981	10536	27	0.00	0.00
20982	10537	27	0.00	0.00
20983	10538	27	0.00	0.00
20984	10539	27	0.00	0.00
20985	10540	27	0.00	0.00
20986	10541	27	0.00	0.00
20987	10542	27	0.00	0.00
20988	10543	27	0.00	0.00
20989	10544	27	0.00	0.00
20990	10545	27	0.00	0.00
20991	10546	27	0.00	0.00
20992	10547	27	0.00	0.00
20993	10548	27	0.00	0.00
20994	10549	27	0.00	0.00
20995	10550	27	0.00	0.00
20996	10551	27	0.00	0.00
20997	10552	27	0.00	0.00
20998	10553	27	0.00	0.00
20999	10554	27	0.00	0.00
21000	10555	27	0.00	0.00
21001	10556	27	0.00	0.00
21002	10557	27	0.00	0.00
21003	10558	27	0.00	0.00
21004	10559	27	0.00	0.00
21005	10560	27	0.00	0.00
21006	10561	27	0.00	0.00
21007	10562	27	0.00	0.00
21008	10563	27	0.00	0.00
21009	10564	27	0.00	0.00
21010	10565	27	0.00	0.00
21011	10566	27	0.00	0.00
21012	10567	27	0.00	0.00
21013	10568	27	0.00	0.00
21014	10569	27	0.00	0.00
21015	10570	27	0.00	0.00
21016	10571	27	0.00	0.00
21017	10572	27	0.00	0.00
21018	10573	27	0.00	0.00
21019	10574	27	0.00	0.00
21020	10575	27	0.00	0.00
21021	10576	27	0.00	0.00
21022	10577	27	0.00	0.00
21023	10578	27	0.00	0.00
21024	10579	27	0.00	0.00
21025	10580	27	0.00	0.00
21026	10581	27	0.00	0.00
21027	10582	27	0.00	0.00
21028	10583	27	0.00	0.00
21029	10584	27	0.00	0.00
21030	10585	27	0.00	0.00
21031	10586	27	0.00	0.00
21032	10587	27	0.00	0.00
21033	10588	27	0.00	0.00
21034	10589	27	0.00	0.00
21035	10590	27	0.00	0.00
21036	10591	27	0.00	0.00
21037	10592	27	0.00	0.00
21038	10593	27	0.00	0.00
21039	10594	27	0.00	0.00
21040	10595	27	0.00	0.00
21041	10596	27	0.00	0.00
21042	10597	27	0.00	0.00
21043	10598	27	0.00	0.00
21044	10599	27	0.00	0.00
21045	10600	27	0.00	0.00
21046	10601	27	0.00	0.00
21047	10602	27	0.00	0.00
21048	10603	27	0.00	0.00
21049	10604	27	0.00	0.00
21050	10605	27	0.00	0.00
21051	10606	27	0.00	0.00
21052	10607	27	0.00	0.00
21053	10608	27	0.00	0.00
21054	10609	27	0.00	0.00
21055	10610	27	0.00	0.00
21056	10611	27	0.00	0.00
21057	10612	27	0.00	0.00
21058	10613	27	0.00	0.00
21059	10614	27	0.00	0.00
21060	10615	27	0.00	0.00
21061	10616	27	0.00	0.00
21062	10617	27	0.00	0.00
21063	10618	27	0.00	0.00
21064	10619	27	0.00	0.00
21065	10620	27	0.00	0.00
21066	10621	27	0.00	0.00
21067	10622	27	0.00	0.00
21068	10623	27	0.00	0.00
21069	10624	27	0.00	0.00
21070	10625	27	0.00	0.00
21071	10626	27	0.00	0.00
21072	10627	27	0.00	0.00
21073	10628	27	0.00	0.00
21074	10629	27	0.00	0.00
21075	10630	27	0.00	0.00
21076	10631	27	0.00	0.00
21209	10414	27	0.00	0.00
21210	10047	27	0.00	0.00
21211	10308	27	0.00	0.00
21212	10302	27	0.00	0.00
21248	10398	27	0.00	0.00
21244	10249	27	0.00	0.00
21245	10243	27	0.00	0.00
21246	10029	27	0.00	0.00
21235	10396	27	0.00	0.00
21249	10044	27	0.00	0.00
18875	10318	24	0.00	0.00
18876	10298	24	0.00	0.00
18877	10034	24	0.00	0.00
15725	10318	20	0.00	0.00
15726	10298	20	0.00	0.00
15727	10034	20	0.00	0.00
18879	10271	24	0.00	0.00
18880	10270	24	0.00	0.00
18881	10031	24	0.00	0.00
15729	10271	20	0.00	0.00
15730	10270	20	0.00	0.00
15731	10031	20	0.00	0.00
18865	10534	24	0.00	0.00
18866	10057	24	0.00	0.00
15732	10534	20	0.00	0.00
15733	10057	20	0.00	0.00
11806	10320	16	0.00	0.00
11809	10002	16	0.00	0.00
11810	10003	16	0.00	0.00
11811	10004	16	0.00	0.00
11812	10005	16	0.00	0.00
11813	10006	16	0.00	0.00
11814	10007	16	0.00	0.00
11815	10008	16	0.00	0.00
11816	10009	16	0.00	0.00
11817	10010	16	0.00	0.00
11818	10011	16	0.00	0.00
11819	10012	16	0.00	0.00
11820	10013	16	0.00	0.00
11821	10014	16	0.00	0.00
11822	10015	16	0.00	0.00
11823	10016	16	0.00	0.00
11824	10017	16	0.00	0.00
11825	10018	16	0.00	0.00
11826	10019	16	0.00	0.00
11827	10020	16	0.00	0.00
11828	10021	16	0.00	0.00
11829	10022	16	0.00	0.00
11830	10023	16	0.00	0.00
11831	10024	16	0.00	0.00
11832	10025	16	0.00	0.00
11833	10026	16	0.00	0.00
11834	10027	16	0.00	0.00
11835	10028	16	0.00	0.00
11836	10030	16	0.00	0.00
11837	10032	16	0.00	0.00
11838	10033	16	0.00	0.00
11839	10035	16	0.00	0.00
11840	10036	16	0.00	0.00
11841	10037	16	0.00	0.00
11842	10038	16	0.00	0.00
11843	10039	16	0.00	0.00
11844	10040	16	0.00	0.00
11845	10041	16	0.00	0.00
11846	10042	16	0.00	0.00
11847	10043	16	0.00	0.00
11848	10045	16	0.00	0.00
11849	10046	16	0.00	0.00
11850	10048	16	0.00	0.00
11851	10049	16	0.00	0.00
11852	10050	16	0.00	0.00
11853	10051	16	0.00	0.00
11854	10052	16	0.00	0.00
11855	10053	16	0.00	0.00
11856	10054	16	0.00	0.00
11857	10055	16	0.00	0.00
11858	10056	16	0.00	0.00
11859	10058	16	0.00	0.00
11860	10059	16	0.00	0.00
11861	10060	16	0.00	0.00
11862	10061	16	0.00	0.00
11863	10062	16	0.00	0.00
11864	10063	16	0.00	0.00
11865	10064	16	0.00	0.00
11867	10066	16	0.00	0.00
11868	10067	16	0.00	0.00
11870	10069	16	0.00	0.00
11871	10070	16	0.00	0.00
11872	10071	16	0.00	0.00
11873	10072	16	0.00	0.00
11874	10073	16	0.00	0.00
11875	10074	16	0.00	0.00
11876	10075	16	0.00	0.00
11877	10076	16	0.00	0.00
11878	10077	16	0.00	0.00
11879	10078	16	0.00	0.00
11880	10079	16	0.00	0.00
11881	10080	16	0.00	0.00
11882	10081	16	0.00	0.00
11883	10082	16	0.00	0.00
11884	10083	16	0.00	0.00
11885	10084	16	0.00	0.00
11886	10085	16	0.00	0.00
11887	10086	16	0.00	0.00
11888	10087	16	0.00	0.00
11889	10088	16	0.00	0.00
11890	10089	16	0.00	0.00
11891	10090	16	0.00	0.00
11892	10091	16	0.00	0.00
11893	10092	16	0.00	0.00
11894	10093	16	0.00	0.00
11895	10094	16	0.00	0.00
11896	10095	16	0.00	0.00
11897	10096	16	0.00	0.00
11898	10097	16	0.00	0.00
11899	10098	16	0.00	0.00
11900	10099	16	0.00	0.00
11901	10100	16	0.00	0.00
11902	10101	16	0.00	0.00
11903	10102	16	0.00	0.00
11904	10103	16	0.00	0.00
11905	10104	16	0.00	0.00
11906	10105	16	0.00	0.00
11907	10106	16	0.00	0.00
11908	10107	16	0.00	0.00
11909	10108	16	0.00	0.00
11910	10109	16	0.00	0.00
11911	10110	16	0.00	0.00
11912	10111	16	0.00	0.00
11913	10112	16	0.00	0.00
11914	10113	16	0.00	0.00
11915	10114	16	0.00	0.00
11916	10115	16	0.00	0.00
11917	10116	16	0.00	0.00
11918	10117	16	0.00	0.00
11919	10118	16	0.00	0.00
11920	10119	16	0.00	0.00
11921	10120	16	0.00	0.00
11922	10121	16	0.00	0.00
11923	10122	16	0.00	0.00
11924	10123	16	0.00	0.00
11925	10124	16	0.00	0.00
11926	10125	16	0.00	0.00
11927	10126	16	0.00	0.00
11928	10127	16	0.00	0.00
11929	10128	16	0.00	0.00
11930	10129	16	0.00	0.00
11931	10130	16	0.00	0.00
11932	10131	16	0.00	0.00
11933	10132	16	0.00	0.00
11934	10133	16	0.00	0.00
11935	10134	16	0.00	0.00
11936	10135	16	0.00	0.00
11937	10136	16	0.00	0.00
11938	10137	16	0.00	0.00
11939	10138	16	0.00	0.00
11940	10139	16	0.00	0.00
11941	10140	16	0.00	0.00
11942	10141	16	0.00	0.00
11943	10142	16	0.00	0.00
11944	10143	16	0.00	0.00
11945	10144	16	0.00	0.00
11946	10145	16	0.00	0.00
11947	10146	16	0.00	0.00
11948	10147	16	0.00	0.00
11949	10148	16	0.00	0.00
11950	10149	16	0.00	0.00
11951	10150	16	0.00	0.00
11952	10151	16	0.00	0.00
11953	10152	16	0.00	0.00
11954	10153	16	0.00	0.00
11955	10154	16	0.00	0.00
11956	10155	16	0.00	0.00
11957	10156	16	0.00	0.00
11958	10157	16	0.00	0.00
11959	10158	16	0.00	0.00
11960	10159	16	0.00	0.00
11961	10160	16	0.00	0.00
11962	10161	16	0.00	0.00
11963	10162	16	0.00	0.00
11964	10163	16	0.00	0.00
11965	10164	16	0.00	0.00
11966	10165	16	0.00	0.00
11967	10166	16	0.00	0.00
11968	10167	16	0.00	0.00
11969	10168	16	0.00	0.00
11970	10169	16	0.00	0.00
11971	10170	16	0.00	0.00
11972	10171	16	0.00	0.00
11973	10172	16	0.00	0.00
11974	10173	16	0.00	0.00
11975	10174	16	0.00	0.00
11976	10175	16	0.00	0.00
11977	10176	16	0.00	0.00
11978	10177	16	0.00	0.00
11979	10178	16	0.00	0.00
11980	10179	16	0.00	0.00
11981	10180	16	0.00	0.00
11982	10181	16	0.00	0.00
11983	10182	16	0.00	0.00
11984	10183	16	0.00	0.00
11985	10184	16	0.00	0.00
11986	10185	16	0.00	0.00
11987	10186	16	0.00	0.00
11988	10187	16	0.00	0.00
11989	10188	16	0.00	0.00
11990	10189	16	0.00	0.00
11991	10190	16	0.00	0.00
11992	10191	16	0.00	0.00
11993	10192	16	0.00	0.00
11994	10193	16	0.00	0.00
11995	10194	16	0.00	0.00
11996	10195	16	0.00	0.00
11997	10196	16	0.00	0.00
11998	10197	16	0.00	0.00
11999	10198	16	0.00	0.00
12000	10199	16	0.00	0.00
12001	10200	16	0.00	0.00
12002	10201	16	0.00	0.00
12003	10202	16	0.00	0.00
12004	10203	16	0.00	0.00
12005	10204	16	0.00	0.00
12006	10205	16	0.00	0.00
12007	10206	16	0.00	0.00
12008	10207	16	0.00	0.00
12009	10208	16	0.00	0.00
12010	10209	16	0.00	0.00
12011	10210	16	0.00	0.00
12012	10211	16	0.00	0.00
12013	10212	16	0.00	0.00
12014	10213	16	0.00	0.00
12015	10214	16	0.00	0.00
12016	10215	16	0.00	0.00
12017	10216	16	0.00	0.00
12018	10217	16	0.00	0.00
12019	10218	16	0.00	0.00
12020	10219	16	0.00	0.00
12021	10220	16	0.00	0.00
12022	10221	16	0.00	0.00
12023	10222	16	0.00	0.00
12024	10223	16	0.00	0.00
12025	10224	16	0.00	0.00
12026	10225	16	0.00	0.00
12027	10226	16	0.00	0.00
12028	10227	16	0.00	0.00
12029	10228	16	0.00	0.00
12030	10229	16	0.00	0.00
12031	10230	16	0.00	0.00
12032	10231	16	0.00	0.00
12033	10232	16	0.00	0.00
12034	10233	16	0.00	0.00
12035	10234	16	0.00	0.00
12036	10235	16	0.00	0.00
12037	10236	16	0.00	0.00
12038	10237	16	0.00	0.00
12039	10238	16	0.00	0.00
12040	10239	16	0.00	0.00
12041	10240	16	0.00	0.00
12042	10241	16	0.00	0.00
12043	10242	16	0.00	0.00
12044	10244	16	0.00	0.00
12045	10245	16	0.00	0.00
12046	10246	16	0.00	0.00
12047	10247	16	0.00	0.00
12048	10248	16	0.00	0.00
12049	10250	16	0.00	0.00
12050	10251	16	0.00	0.00
12052	10253	16	0.00	0.00
12053	10254	16	0.00	0.00
12054	10255	16	0.00	0.00
12055	10256	16	0.00	0.00
12056	10257	16	0.00	0.00
12057	10258	16	0.00	0.00
12058	10259	16	0.00	0.00
12059	10260	16	0.00	0.00
12060	10261	16	0.00	0.00
12061	10262	16	0.00	0.00
12062	10263	16	0.00	0.00
12063	10264	16	0.00	0.00
12064	10265	16	0.00	0.00
12065	10266	16	0.00	0.00
12066	10267	16	0.00	0.00
12067	10268	16	0.00	0.00
12068	10269	16	0.00	0.00
12069	10272	16	0.00	0.00
12070	10273	16	0.00	0.00
12071	10274	16	0.00	0.00
12072	10275	16	0.00	0.00
12073	10276	16	0.00	0.00
12074	10277	16	0.00	0.00
12075	10278	16	0.00	0.00
12076	10279	16	0.00	0.00
12077	10280	16	0.00	0.00
12078	10281	16	0.00	0.00
12079	10282	16	0.00	0.00
12080	10283	16	0.00	0.00
12081	10284	16	0.00	0.00
12082	10285	16	0.00	0.00
12083	10286	16	0.00	0.00
12084	10287	16	0.00	0.00
12085	10288	16	0.00	0.00
12086	10289	16	0.00	0.00
12087	10290	16	0.00	0.00
12088	10291	16	0.00	0.00
12089	10292	16	0.00	0.00
12090	10293	16	0.00	0.00
12091	10294	16	0.00	0.00
12092	10295	16	0.00	0.00
12093	10296	16	0.00	0.00
12094	10297	16	0.00	0.00
12095	10299	16	0.00	0.00
12096	10300	16	0.00	0.00
12097	10301	16	0.00	0.00
12098	10303	16	0.00	0.00
12099	10304	16	0.00	0.00
12100	10305	16	0.00	0.00
12101	10306	16	0.00	0.00
12102	10307	16	0.00	0.00
12103	10309	16	0.00	0.00
12104	10310	16	0.00	0.00
12105	10311	16	0.00	0.00
12106	10312	16	0.00	0.00
12107	10313	16	0.00	0.00
12108	10314	16	0.00	0.00
12109	10315	16	0.00	0.00
12110	10316	16	0.00	0.00
12111	10317	16	0.00	0.00
12112	10319	16	0.00	0.00
12113	10321	16	0.00	0.00
12114	10322	16	0.00	0.00
12115	10323	16	0.00	0.00
12116	10324	16	0.00	0.00
12117	10325	16	0.00	0.00
12118	10326	16	0.00	0.00
12119	10327	16	0.00	0.00
12120	10328	16	0.00	0.00
12121	10329	16	0.00	0.00
12122	10330	16	0.00	0.00
12123	10331	16	0.00	0.00
12124	10332	16	0.00	0.00
12125	10333	16	0.00	0.00
12126	10334	16	0.00	0.00
12127	10335	16	0.00	0.00
12128	10336	16	0.00	0.00
12129	10337	16	0.00	0.00
12130	10338	16	0.00	0.00
12131	10339	16	0.00	0.00
12132	10340	16	0.00	0.00
12133	10341	16	0.00	0.00
12134	10342	16	0.00	0.00
12135	10343	16	0.00	0.00
12136	10344	16	0.00	0.00
12137	10345	16	0.00	0.00
12138	10346	16	0.00	0.00
12139	10347	16	0.00	0.00
12140	10348	16	0.00	0.00
12141	10349	16	0.00	0.00
12142	10350	16	0.00	0.00
12143	10351	16	0.00	0.00
12144	10352	16	0.00	0.00
12145	10354	16	0.00	0.00
12146	10353	16	0.00	0.00
12147	10355	16	0.00	0.00
12148	10356	16	0.00	0.00
12149	10357	16	0.00	0.00
12150	10358	16	0.00	0.00
12151	10359	16	0.00	0.00
12152	10360	16	0.00	0.00
12153	10361	16	0.00	0.00
12154	10362	16	0.00	0.00
12155	10363	16	0.00	0.00
12156	10364	16	0.00	0.00
12157	10365	16	0.00	0.00
12158	10366	16	0.00	0.00
12159	10367	16	0.00	0.00
12160	10368	16	0.00	0.00
12161	10369	16	0.00	0.00
12162	10370	16	0.00	0.00
12163	10371	16	0.00	0.00
12164	10372	16	0.00	0.00
12165	10373	16	0.00	0.00
12166	10374	16	0.00	0.00
12167	10375	16	0.00	0.00
12168	10376	16	0.00	0.00
12169	10377	16	0.00	0.00
12170	10378	16	0.00	0.00
12171	10379	16	0.00	0.00
12172	10380	16	0.00	0.00
12173	10381	16	0.00	0.00
12174	10382	16	0.00	0.00
12175	10383	16	0.00	0.00
12176	10384	16	0.00	0.00
12177	10385	16	0.00	0.00
12178	10386	16	0.00	0.00
12179	10387	16	0.00	0.00
12180	10388	16	0.00	0.00
12181	10389	16	0.00	0.00
12182	10390	16	0.00	0.00
12183	10391	16	0.00	0.00
12184	10392	16	0.00	0.00
12185	10393	16	0.00	0.00
12186	10394	16	0.00	0.00
12187	10395	16	0.00	0.00
12188	10397	16	0.00	0.00
12189	10399	16	0.00	0.00
12190	10400	16	0.00	0.00
12191	10401	16	0.00	0.00
12192	10402	16	0.00	0.00
12193	10403	16	0.00	0.00
12194	10404	16	0.00	0.00
12195	10405	16	0.00	0.00
12196	10406	16	0.00	0.00
12197	10407	16	0.00	0.00
12198	10408	16	0.00	0.00
12199	10409	16	0.00	0.00
12200	10410	16	0.00	0.00
12201	10411	16	0.00	0.00
12202	10412	16	0.00	0.00
12203	10413	16	0.00	0.00
12204	10415	16	0.00	0.00
12205	10416	16	0.00	0.00
12206	10417	16	0.00	0.00
12207	10418	16	0.00	0.00
12208	10419	16	0.00	0.00
12209	10420	16	0.00	0.00
12210	10421	16	0.00	0.00
12211	10422	16	0.00	0.00
12212	10423	16	0.00	0.00
12213	10424	16	0.00	0.00
12214	10425	16	0.00	0.00
12215	10426	16	0.00	0.00
12216	10427	16	0.00	0.00
12217	10428	16	0.00	0.00
12218	10429	16	0.00	0.00
12219	10430	16	0.00	0.00
12220	10431	16	0.00	0.00
12221	10432	16	0.00	0.00
12222	10433	16	0.00	0.00
12223	10434	16	0.00	0.00
12224	10435	16	0.00	0.00
12225	10436	16	0.00	0.00
12226	10437	16	0.00	0.00
12227	10438	16	0.00	0.00
12228	10439	16	0.00	0.00
12229	10440	16	0.00	0.00
12230	10441	16	0.00	0.00
12231	10442	16	0.00	0.00
12232	10443	16	0.00	0.00
12233	10444	16	0.00	0.00
12234	10445	16	0.00	0.00
12235	10446	16	0.00	0.00
12236	10447	16	0.00	0.00
12237	10448	16	0.00	0.00
12238	10449	16	0.00	0.00
12239	10450	16	0.00	0.00
12240	10451	16	0.00	0.00
12241	10452	16	0.00	0.00
12242	10453	16	0.00	0.00
12243	10454	16	0.00	0.00
12244	10455	16	0.00	0.00
12245	10456	16	0.00	0.00
12246	10457	16	0.00	0.00
12247	10458	16	0.00	0.00
12248	10459	16	0.00	0.00
12249	10460	16	0.00	0.00
12250	10461	16	0.00	0.00
12251	10462	16	0.00	0.00
12252	10463	16	0.00	0.00
12253	10464	16	0.00	0.00
12254	10465	16	0.00	0.00
12255	10466	16	0.00	0.00
12256	10467	16	0.00	0.00
12257	10468	16	0.00	0.00
12258	10469	16	0.00	0.00
12259	10470	16	0.00	0.00
12260	10471	16	0.00	0.00
12261	10472	16	0.00	0.00
12262	10473	16	0.00	0.00
12263	10474	16	0.00	0.00
12264	10475	16	0.00	0.00
12265	10476	16	0.00	0.00
12266	10477	16	0.00	0.00
12267	10478	16	0.00	0.00
12268	10479	16	0.00	0.00
12269	10480	16	0.00	0.00
12270	10481	16	0.00	0.00
12271	10482	16	0.00	0.00
12272	10483	16	0.00	0.00
12273	10484	16	0.00	0.00
12274	10485	16	0.00	0.00
12275	10486	16	0.00	0.00
12276	10487	16	0.00	0.00
12277	10488	16	0.00	0.00
12278	10489	16	0.00	0.00
12279	10490	16	0.00	0.00
12280	10491	16	0.00	0.00
12281	10492	16	0.00	0.00
12282	10493	16	0.00	0.00
12283	10494	16	0.00	0.00
12284	10495	16	0.00	0.00
12285	10496	16	0.00	0.00
12286	10497	16	0.00	0.00
12287	10498	16	0.00	0.00
12288	10499	16	0.00	0.00
12289	10500	16	0.00	0.00
12290	10501	16	0.00	0.00
12291	10502	16	0.00	0.00
12292	10503	16	0.00	0.00
12293	10504	16	0.00	0.00
12294	10505	16	0.00	0.00
12295	10506	16	0.00	0.00
12296	10507	16	0.00	0.00
12297	10508	16	0.00	0.00
12298	10509	16	0.00	0.00
12299	10510	16	0.00	0.00
12300	10511	16	0.00	0.00
12301	10512	16	0.00	0.00
12302	10513	16	0.00	0.00
12303	10514	16	0.00	0.00
12304	10515	16	0.00	0.00
12305	10516	16	0.00	0.00
12306	10517	16	0.00	0.00
12307	10518	16	0.00	0.00
12308	10519	16	0.00	0.00
12309	10520	16	0.00	0.00
12310	10521	16	0.00	0.00
12311	10522	16	0.00	0.00
12312	10523	16	0.00	0.00
12313	10524	16	0.00	0.00
12314	10525	16	0.00	0.00
12315	10526	16	0.00	0.00
12316	10527	16	0.00	0.00
12317	10528	16	0.00	0.00
12318	10529	16	0.00	0.00
12319	10530	16	0.00	0.00
12320	10531	16	0.00	0.00
12321	10532	16	0.00	0.00
12322	10533	16	0.00	0.00
12323	10535	16	0.00	0.00
12324	10536	16	0.00	0.00
12325	10537	16	0.00	0.00
12326	10538	16	0.00	0.00
12327	10539	16	0.00	0.00
12328	10540	16	0.00	0.00
12329	10541	16	0.00	0.00
12330	10542	16	0.00	0.00
12331	10543	16	0.00	0.00
12332	10544	16	0.00	0.00
12333	10545	16	0.00	0.00
12334	10546	16	0.00	0.00
12335	10547	16	0.00	0.00
12336	10548	16	0.00	0.00
12337	10549	16	0.00	0.00
12338	10550	16	0.00	0.00
12339	10551	16	0.00	0.00
12340	10552	16	0.00	0.00
12341	10553	16	0.00	0.00
12342	10554	16	0.00	0.00
12343	10555	16	0.00	0.00
12344	10556	16	0.00	0.00
12345	10557	16	0.00	0.00
12346	10558	16	0.00	0.00
12347	10559	16	0.00	0.00
12348	10560	16	0.00	0.00
12349	10561	16	0.00	0.00
12350	10562	16	0.00	0.00
12351	10563	16	0.00	0.00
12352	10564	16	0.00	0.00
12353	10565	16	0.00	0.00
12354	10566	16	0.00	0.00
12355	10567	16	0.00	0.00
12356	10568	16	0.00	0.00
12357	10569	16	0.00	0.00
12358	10570	16	0.00	0.00
12359	10571	16	0.00	0.00
12360	10572	16	0.00	0.00
12361	10573	16	0.00	0.00
12362	10574	16	0.00	0.00
12363	10575	16	0.00	0.00
12364	10576	16	0.00	0.00
12365	10577	16	0.00	0.00
12366	10578	16	0.00	0.00
12367	10579	16	0.00	0.00
12368	10580	16	0.00	0.00
12369	10581	16	0.00	0.00
12370	10582	16	0.00	0.00
12371	10583	16	0.00	0.00
12372	10584	16	0.00	0.00
12373	10585	16	0.00	0.00
12374	10586	16	0.00	0.00
12375	10587	16	0.00	0.00
12376	10588	16	0.00	0.00
12377	10589	16	0.00	0.00
12378	10590	16	0.00	0.00
12379	10591	16	0.00	0.00
12380	10592	16	0.00	0.00
12381	10593	16	0.00	0.00
12382	10594	16	0.00	0.00
12383	10595	16	0.00	0.00
12384	10596	16	0.00	0.00
12385	10597	16	0.00	0.00
12386	10598	16	0.00	0.00
12387	10599	16	0.00	0.00
12388	10600	16	0.00	0.00
12389	10601	16	0.00	0.00
12390	10602	16	0.00	0.00
12391	10603	16	0.00	0.00
12392	10604	16	0.00	0.00
12393	10605	16	0.00	0.00
12394	10606	16	0.00	0.00
12395	10607	16	0.00	0.00
12396	10608	16	0.00	0.00
12397	10609	16	0.00	0.00
12398	10610	16	0.00	0.00
12399	10611	16	0.00	0.00
12400	10612	16	0.00	0.00
12401	10613	16	0.00	0.00
12402	10614	16	0.00	0.00
12403	10615	16	0.00	0.00
12404	10616	16	0.00	0.00
12405	10617	16	0.00	0.00
12406	10618	16	0.00	0.00
12407	10619	16	0.00	0.00
12408	10620	16	0.00	0.00
12409	10621	16	0.00	0.00
12410	10622	16	0.00	0.00
12411	10623	16	0.00	0.00
12412	10624	16	0.00	0.00
12413	10625	16	0.00	0.00
12414	10626	16	0.00	0.00
12415	10627	16	0.00	0.00
12416	10628	16	0.00	0.00
12417	10629	16	0.00	0.00
12418	10630	16	0.00	0.00
12419	10631	16	0.00	0.00
12552	10308	16	0.00	0.00
12553	10302	16	0.00	0.00
12554	10414	16	0.00	0.00
12555	10047	16	0.00	0.00
12591	10398	16	0.00	0.00
12592	10044	16	0.00	0.00
14954	10320	20	0.00	0.00
14957	10002	20	0.00	0.00
14958	10003	20	0.00	0.00
14959	10004	20	0.00	0.00
14960	10005	20	0.00	0.00
14961	10006	20	0.00	0.00
14962	10007	20	0.00	0.00
14963	10008	20	0.00	0.00
14964	10009	20	0.00	0.00
14965	10010	20	0.00	0.00
14966	10011	20	0.00	0.00
14967	10012	20	0.00	0.00
14968	10013	20	0.00	0.00
14969	10014	20	0.00	0.00
14970	10015	20	0.00	0.00
14971	10016	20	0.00	0.00
14972	10017	20	0.00	0.00
14973	10018	20	0.00	0.00
14974	10019	20	0.00	0.00
14975	10020	20	0.00	0.00
14976	10021	20	0.00	0.00
14977	10022	20	0.00	0.00
14978	10023	20	0.00	0.00
14979	10024	20	0.00	0.00
14980	10025	20	0.00	0.00
14981	10026	20	0.00	0.00
14982	10027	20	0.00	0.00
14983	10028	20	0.00	0.00
14984	10030	20	0.00	0.00
14985	10032	20	0.00	0.00
14986	10033	20	0.00	0.00
14987	10035	20	0.00	0.00
14988	10036	20	0.00	0.00
14989	10037	20	0.00	0.00
14990	10038	20	0.00	0.00
14991	10039	20	0.00	0.00
14992	10040	20	0.00	0.00
14993	10041	20	0.00	0.00
14994	10042	20	0.00	0.00
14995	10043	20	0.00	0.00
14996	10045	20	0.00	0.00
14997	10046	20	0.00	0.00
14998	10048	20	0.00	0.00
14999	10049	20	0.00	0.00
15000	10050	20	0.00	0.00
15001	10051	20	0.00	0.00
15002	10052	20	0.00	0.00
15003	10053	20	0.00	0.00
15004	10054	20	0.00	0.00
15005	10055	20	0.00	0.00
15006	10056	20	0.00	0.00
15007	10058	20	0.00	0.00
15008	10059	20	0.00	0.00
15009	10060	20	0.00	0.00
15010	10061	20	0.00	0.00
15011	10062	20	0.00	0.00
15012	10063	20	0.00	0.00
15013	10064	20	0.00	0.00
15015	10066	20	0.00	0.00
15016	10067	20	0.00	0.00
15018	10069	20	0.00	0.00
15019	10070	20	0.00	0.00
15020	10071	20	0.00	0.00
15021	10072	20	0.00	0.00
15022	10073	20	0.00	0.00
15023	10074	20	0.00	0.00
15024	10075	20	0.00	0.00
15025	10076	20	0.00	0.00
15026	10077	20	0.00	0.00
15027	10078	20	0.00	0.00
15028	10079	20	0.00	0.00
15029	10080	20	0.00	0.00
15030	10081	20	0.00	0.00
15031	10082	20	0.00	0.00
15032	10083	20	0.00	0.00
15033	10084	20	0.00	0.00
15034	10085	20	0.00	0.00
15035	10086	20	0.00	0.00
15036	10087	20	0.00	0.00
15037	10088	20	0.00	0.00
15038	10089	20	0.00	0.00
15039	10090	20	0.00	0.00
15040	10091	20	0.00	0.00
15041	10092	20	0.00	0.00
15042	10093	20	0.00	0.00
15043	10094	20	0.00	0.00
15044	10095	20	0.00	0.00
15045	10096	20	0.00	0.00
15046	10097	20	0.00	0.00
15047	10098	20	0.00	0.00
15048	10099	20	0.00	0.00
15049	10100	20	0.00	0.00
15050	10101	20	0.00	0.00
15051	10102	20	0.00	0.00
15052	10103	20	0.00	0.00
15053	10104	20	0.00	0.00
15054	10105	20	0.00	0.00
15055	10106	20	0.00	0.00
15056	10107	20	0.00	0.00
15057	10108	20	0.00	0.00
15058	10109	20	0.00	0.00
15059	10110	20	0.00	0.00
15060	10111	20	0.00	0.00
15061	10112	20	0.00	0.00
15062	10113	20	0.00	0.00
15063	10114	20	0.00	0.00
15064	10115	20	0.00	0.00
15065	10116	20	0.00	0.00
15066	10117	20	0.00	0.00
15067	10118	20	0.00	0.00
15068	10119	20	0.00	0.00
15069	10120	20	0.00	0.00
15070	10121	20	0.00	0.00
15071	10122	20	0.00	0.00
15072	10123	20	0.00	0.00
15073	10124	20	0.00	0.00
15074	10125	20	0.00	0.00
15075	10126	20	0.00	0.00
15076	10127	20	0.00	0.00
15077	10128	20	0.00	0.00
15078	10129	20	0.00	0.00
15079	10130	20	0.00	0.00
15080	10131	20	0.00	0.00
15081	10132	20	0.00	0.00
15082	10133	20	0.00	0.00
15083	10134	20	0.00	0.00
15084	10135	20	0.00	0.00
15085	10136	20	0.00	0.00
15086	10137	20	0.00	0.00
15087	10138	20	0.00	0.00
15088	10139	20	0.00	0.00
15089	10140	20	0.00	0.00
15090	10141	20	0.00	0.00
15091	10142	20	0.00	0.00
15092	10143	20	0.00	0.00
15093	10144	20	0.00	0.00
15094	10145	20	0.00	0.00
15095	10146	20	0.00	0.00
15096	10147	20	0.00	0.00
15097	10148	20	0.00	0.00
15098	10149	20	0.00	0.00
15099	10150	20	0.00	0.00
15100	10151	20	0.00	0.00
15101	10152	20	0.00	0.00
15102	10153	20	0.00	0.00
15103	10154	20	0.00	0.00
15104	10155	20	0.00	0.00
15105	10156	20	0.00	0.00
15106	10157	20	0.00	0.00
15107	10158	20	0.00	0.00
15108	10159	20	0.00	0.00
15109	10160	20	0.00	0.00
15110	10161	20	0.00	0.00
15111	10162	20	0.00	0.00
15112	10163	20	0.00	0.00
15113	10164	20	0.00	0.00
15114	10165	20	0.00	0.00
15115	10166	20	0.00	0.00
15116	10167	20	0.00	0.00
15117	10168	20	0.00	0.00
15118	10169	20	0.00	0.00
15119	10170	20	0.00	0.00
15120	10171	20	0.00	0.00
15121	10172	20	0.00	0.00
15122	10173	20	0.00	0.00
15123	10174	20	0.00	0.00
15124	10175	20	0.00	0.00
15125	10176	20	0.00	0.00
15126	10177	20	0.00	0.00
15127	10178	20	0.00	0.00
15128	10179	20	0.00	0.00
15129	10180	20	0.00	0.00
15130	10181	20	0.00	0.00
15131	10182	20	0.00	0.00
15132	10183	20	0.00	0.00
15133	10184	20	0.00	0.00
15134	10185	20	0.00	0.00
15135	10186	20	0.00	0.00
15136	10187	20	0.00	0.00
15137	10188	20	0.00	0.00
15138	10189	20	0.00	0.00
15139	10190	20	0.00	0.00
15140	10191	20	0.00	0.00
15141	10192	20	0.00	0.00
15142	10193	20	0.00	0.00
15143	10194	20	0.00	0.00
15144	10195	20	0.00	0.00
15145	10196	20	0.00	0.00
15146	10197	20	0.00	0.00
15147	10198	20	0.00	0.00
15148	10199	20	0.00	0.00
15149	10200	20	0.00	0.00
15150	10201	20	0.00	0.00
15151	10202	20	0.00	0.00
15152	10203	20	0.00	0.00
15153	10204	20	0.00	0.00
15154	10205	20	0.00	0.00
15155	10206	20	0.00	0.00
15156	10207	20	0.00	0.00
15157	10208	20	0.00	0.00
15158	10209	20	0.00	0.00
15159	10210	20	0.00	0.00
15160	10211	20	0.00	0.00
15161	10212	20	0.00	0.00
15162	10213	20	0.00	0.00
15163	10214	20	0.00	0.00
15164	10215	20	0.00	0.00
15165	10216	20	0.00	0.00
15166	10217	20	0.00	0.00
15167	10218	20	0.00	0.00
15168	10219	20	0.00	0.00
15169	10220	20	0.00	0.00
15170	10221	20	0.00	0.00
15171	10222	20	0.00	0.00
15172	10223	20	0.00	0.00
15173	10224	20	0.00	0.00
15174	10225	20	0.00	0.00
15175	10226	20	0.00	0.00
15176	10227	20	0.00	0.00
15177	10228	20	0.00	0.00
15178	10229	20	0.00	0.00
15179	10230	20	0.00	0.00
15180	10231	20	0.00	0.00
15181	10232	20	0.00	0.00
15182	10233	20	0.00	0.00
15183	10234	20	0.00	0.00
15184	10235	20	0.00	0.00
15185	10236	20	0.00	0.00
15186	10237	20	0.00	0.00
15187	10238	20	0.00	0.00
15188	10239	20	0.00	0.00
15189	10240	20	0.00	0.00
15190	10241	20	0.00	0.00
15191	10242	20	0.00	0.00
15192	10244	20	0.00	0.00
15193	10245	20	0.00	0.00
15194	10246	20	0.00	0.00
15195	10247	20	0.00	0.00
15196	10248	20	0.00	0.00
15197	10250	20	0.00	0.00
15198	10251	20	0.00	0.00
15200	10253	20	0.00	0.00
15201	10254	20	0.00	0.00
15202	10255	20	0.00	0.00
15203	10256	20	0.00	0.00
15204	10257	20	0.00	0.00
15205	10258	20	0.00	0.00
15206	10259	20	0.00	0.00
15207	10260	20	0.00	0.00
15208	10261	20	0.00	0.00
15209	10262	20	0.00	0.00
15210	10263	20	0.00	0.00
15211	10264	20	0.00	0.00
15212	10265	20	0.00	0.00
15213	10266	20	0.00	0.00
15214	10267	20	0.00	0.00
15215	10268	20	0.00	0.00
15216	10269	20	0.00	0.00
15217	10272	20	0.00	0.00
15218	10273	20	0.00	0.00
15219	10274	20	0.00	0.00
15220	10275	20	0.00	0.00
15221	10276	20	0.00	0.00
15222	10277	20	0.00	0.00
15223	10278	20	0.00	0.00
15224	10279	20	0.00	0.00
15225	10280	20	0.00	0.00
15226	10281	20	0.00	0.00
15227	10282	20	0.00	0.00
15228	10283	20	0.00	0.00
15229	10284	20	0.00	0.00
15230	10285	20	0.00	0.00
15231	10286	20	0.00	0.00
15232	10287	20	0.00	0.00
15233	10288	20	0.00	0.00
15234	10289	20	0.00	0.00
15235	10290	20	0.00	0.00
15236	10291	20	0.00	0.00
15237	10292	20	0.00	0.00
15238	10293	20	0.00	0.00
15239	10294	20	0.00	0.00
15240	10295	20	0.00	0.00
15241	10296	20	0.00	0.00
15242	10297	20	0.00	0.00
15243	10299	20	0.00	0.00
15244	10300	20	0.00	0.00
15245	10301	20	0.00	0.00
15246	10303	20	0.00	0.00
15247	10304	20	0.00	0.00
15248	10305	20	0.00	0.00
15249	10306	20	0.00	0.00
15250	10307	20	0.00	0.00
15251	10309	20	0.00	0.00
15252	10310	20	0.00	0.00
15253	10311	20	0.00	0.00
15254	10312	20	0.00	0.00
15255	10313	20	0.00	0.00
15256	10314	20	0.00	0.00
15257	10315	20	0.00	0.00
15258	10316	20	0.00	0.00
15259	10317	20	0.00	0.00
15260	10319	20	0.00	0.00
15261	10321	20	0.00	0.00
15262	10322	20	0.00	0.00
15263	10323	20	0.00	0.00
15264	10324	20	0.00	0.00
15265	10325	20	0.00	0.00
15266	10326	20	0.00	0.00
15267	10327	20	0.00	0.00
15268	10328	20	0.00	0.00
15269	10329	20	0.00	0.00
15270	10330	20	0.00	0.00
15271	10331	20	0.00	0.00
15272	10332	20	0.00	0.00
15273	10333	20	0.00	0.00
15274	10334	20	0.00	0.00
15275	10335	20	0.00	0.00
15276	10336	20	0.00	0.00
15277	10337	20	0.00	0.00
15278	10338	20	0.00	0.00
15279	10339	20	0.00	0.00
15280	10340	20	0.00	0.00
15281	10341	20	0.00	0.00
15282	10342	20	0.00	0.00
15283	10343	20	0.00	0.00
15284	10344	20	0.00	0.00
15285	10345	20	0.00	0.00
15286	10346	20	0.00	0.00
15287	10347	20	0.00	0.00
15288	10348	20	0.00	0.00
15289	10349	20	0.00	0.00
15290	10350	20	0.00	0.00
15291	10351	20	0.00	0.00
15292	10352	20	0.00	0.00
15293	10354	20	0.00	0.00
15294	10353	20	0.00	0.00
15295	10355	20	0.00	0.00
15296	10356	20	0.00	0.00
15297	10357	20	0.00	0.00
15298	10358	20	0.00	0.00
15299	10359	20	0.00	0.00
15300	10360	20	0.00	0.00
15301	10361	20	0.00	0.00
15302	10362	20	0.00	0.00
15303	10363	20	0.00	0.00
15304	10364	20	0.00	0.00
15305	10365	20	0.00	0.00
15306	10366	20	0.00	0.00
15307	10367	20	0.00	0.00
15308	10368	20	0.00	0.00
15309	10369	20	0.00	0.00
15310	10370	20	0.00	0.00
15311	10371	20	0.00	0.00
15312	10372	20	0.00	0.00
15313	10373	20	0.00	0.00
15314	10374	20	0.00	0.00
15315	10375	20	0.00	0.00
15316	10376	20	0.00	0.00
15317	10377	20	0.00	0.00
15318	10378	20	0.00	0.00
15319	10379	20	0.00	0.00
15320	10380	20	0.00	0.00
15321	10381	20	0.00	0.00
15322	10382	20	0.00	0.00
15323	10383	20	0.00	0.00
15324	10384	20	0.00	0.00
15325	10385	20	0.00	0.00
15326	10386	20	0.00	0.00
15327	10387	20	0.00	0.00
15328	10388	20	0.00	0.00
15329	10389	20	0.00	0.00
15330	10390	20	0.00	0.00
15331	10391	20	0.00	0.00
15332	10392	20	0.00	0.00
15333	10393	20	0.00	0.00
15334	10394	20	0.00	0.00
15335	10395	20	0.00	0.00
15336	10397	20	0.00	0.00
15337	10399	20	0.00	0.00
15338	10400	20	0.00	0.00
15339	10401	20	0.00	0.00
15340	10402	20	0.00	0.00
15341	10403	20	0.00	0.00
15342	10404	20	0.00	0.00
15343	10405	20	0.00	0.00
15344	10406	20	0.00	0.00
15345	10407	20	0.00	0.00
15346	10408	20	0.00	0.00
15347	10409	20	0.00	0.00
15348	10410	20	0.00	0.00
15349	10411	20	0.00	0.00
15350	10412	20	0.00	0.00
15351	10413	20	0.00	0.00
15352	10415	20	0.00	0.00
15353	10416	20	0.00	0.00
15354	10417	20	0.00	0.00
15355	10418	20	0.00	0.00
15356	10419	20	0.00	0.00
15357	10420	20	0.00	0.00
15358	10421	20	0.00	0.00
15359	10422	20	0.00	0.00
15360	10423	20	0.00	0.00
15361	10424	20	0.00	0.00
15362	10425	20	0.00	0.00
15363	10426	20	0.00	0.00
15364	10427	20	0.00	0.00
15365	10428	20	0.00	0.00
15366	10429	20	0.00	0.00
15367	10430	20	0.00	0.00
15368	10431	20	0.00	0.00
15369	10432	20	0.00	0.00
15370	10433	20	0.00	0.00
15371	10434	20	0.00	0.00
15372	10435	20	0.00	0.00
15373	10436	20	0.00	0.00
15374	10437	20	0.00	0.00
15375	10438	20	0.00	0.00
15376	10439	20	0.00	0.00
15377	10440	20	0.00	0.00
15378	10441	20	0.00	0.00
15379	10442	20	0.00	0.00
15380	10443	20	0.00	0.00
15381	10444	20	0.00	0.00
15382	10445	20	0.00	0.00
15383	10446	20	0.00	0.00
15384	10447	20	0.00	0.00
15385	10448	20	0.00	0.00
15386	10449	20	0.00	0.00
15387	10450	20	0.00	0.00
15388	10451	20	0.00	0.00
15389	10452	20	0.00	0.00
15390	10453	20	0.00	0.00
15391	10454	20	0.00	0.00
15392	10455	20	0.00	0.00
15393	10456	20	0.00	0.00
15394	10457	20	0.00	0.00
15395	10458	20	0.00	0.00
15396	10459	20	0.00	0.00
15397	10460	20	0.00	0.00
15398	10461	20	0.00	0.00
15399	10462	20	0.00	0.00
15400	10463	20	0.00	0.00
15401	10464	20	0.00	0.00
15402	10465	20	0.00	0.00
15403	10466	20	0.00	0.00
15404	10467	20	0.00	0.00
15405	10468	20	0.00	0.00
15406	10469	20	0.00	0.00
15407	10470	20	0.00	0.00
15408	10471	20	0.00	0.00
15409	10472	20	0.00	0.00
15410	10473	20	0.00	0.00
15411	10474	20	0.00	0.00
15412	10475	20	0.00	0.00
15413	10476	20	0.00	0.00
15414	10477	20	0.00	0.00
15415	10478	20	0.00	0.00
15416	10479	20	0.00	0.00
15417	10480	20	0.00	0.00
15418	10481	20	0.00	0.00
15419	10482	20	0.00	0.00
15420	10483	20	0.00	0.00
15421	10484	20	0.00	0.00
15422	10485	20	0.00	0.00
15423	10486	20	0.00	0.00
15424	10487	20	0.00	0.00
15425	10488	20	0.00	0.00
15426	10489	20	0.00	0.00
15427	10490	20	0.00	0.00
15428	10491	20	0.00	0.00
15429	10492	20	0.00	0.00
15430	10493	20	0.00	0.00
15431	10494	20	0.00	0.00
15432	10495	20	0.00	0.00
15433	10496	20	0.00	0.00
15434	10497	20	0.00	0.00
15435	10498	20	0.00	0.00
15436	10499	20	0.00	0.00
15437	10500	20	0.00	0.00
15438	10501	20	0.00	0.00
15439	10502	20	0.00	0.00
15440	10503	20	0.00	0.00
15441	10504	20	0.00	0.00
15442	10505	20	0.00	0.00
15443	10506	20	0.00	0.00
15444	10507	20	0.00	0.00
15445	10508	20	0.00	0.00
15446	10509	20	0.00	0.00
15447	10510	20	0.00	0.00
15448	10511	20	0.00	0.00
15449	10512	20	0.00	0.00
15450	10513	20	0.00	0.00
15451	10514	20	0.00	0.00
15452	10515	20	0.00	0.00
15453	10516	20	0.00	0.00
15454	10517	20	0.00	0.00
15455	10518	20	0.00	0.00
15456	10519	20	0.00	0.00
15457	10520	20	0.00	0.00
15458	10521	20	0.00	0.00
15459	10522	20	0.00	0.00
15460	10523	20	0.00	0.00
15461	10524	20	0.00	0.00
15462	10525	20	0.00	0.00
15463	10526	20	0.00	0.00
15464	10527	20	0.00	0.00
15465	10528	20	0.00	0.00
15466	10529	20	0.00	0.00
15467	10530	20	0.00	0.00
15468	10531	20	0.00	0.00
15469	10532	20	0.00	0.00
15470	10533	20	0.00	0.00
15471	10535	20	0.00	0.00
15472	10536	20	0.00	0.00
15473	10537	20	0.00	0.00
15474	10538	20	0.00	0.00
15475	10539	20	0.00	0.00
15476	10540	20	0.00	0.00
15477	10541	20	0.00	0.00
15478	10542	20	0.00	0.00
15479	10543	20	0.00	0.00
15480	10544	20	0.00	0.00
15481	10545	20	0.00	0.00
15482	10546	20	0.00	0.00
15483	10547	20	0.00	0.00
15484	10548	20	0.00	0.00
15485	10549	20	0.00	0.00
15486	10550	20	0.00	0.00
15487	10551	20	0.00	0.00
15488	10552	20	0.00	0.00
15489	10553	20	0.00	0.00
15490	10554	20	0.00	0.00
15491	10555	20	0.00	0.00
15492	10556	20	0.00	0.00
15493	10557	20	0.00	0.00
15494	10558	20	0.00	0.00
15495	10559	20	0.00	0.00
15496	10560	20	0.00	0.00
15497	10561	20	0.00	0.00
15498	10562	20	0.00	0.00
15499	10563	20	0.00	0.00
15500	10564	20	0.00	0.00
15501	10565	20	0.00	0.00
15502	10566	20	0.00	0.00
15503	10567	20	0.00	0.00
15504	10568	20	0.00	0.00
15505	10569	20	0.00	0.00
15506	10570	20	0.00	0.00
15507	10571	20	0.00	0.00
15508	10572	20	0.00	0.00
15509	10573	20	0.00	0.00
15510	10574	20	0.00	0.00
15511	10575	20	0.00	0.00
15512	10576	20	0.00	0.00
15513	10577	20	0.00	0.00
15514	10578	20	0.00	0.00
15515	10579	20	0.00	0.00
15516	10580	20	0.00	0.00
15517	10581	20	0.00	0.00
15518	10582	20	0.00	0.00
15519	10583	20	0.00	0.00
15520	10584	20	0.00	0.00
15521	10585	20	0.00	0.00
15522	10586	20	0.00	0.00
15523	10587	20	0.00	0.00
15524	10588	20	0.00	0.00
15525	10589	20	0.00	0.00
15526	10590	20	0.00	0.00
15527	10591	20	0.00	0.00
15528	10592	20	0.00	0.00
15529	10593	20	0.00	0.00
15530	10594	20	0.00	0.00
15531	10595	20	0.00	0.00
15532	10596	20	0.00	0.00
15533	10597	20	0.00	0.00
15534	10598	20	0.00	0.00
15535	10599	20	0.00	0.00
15536	10600	20	0.00	0.00
15537	10601	20	0.00	0.00
15538	10602	20	0.00	0.00
15539	10603	20	0.00	0.00
15540	10604	20	0.00	0.00
15541	10605	20	0.00	0.00
15542	10606	20	0.00	0.00
15543	10607	20	0.00	0.00
15544	10608	20	0.00	0.00
15545	10609	20	0.00	0.00
15546	10610	20	0.00	0.00
15547	10611	20	0.00	0.00
15548	10612	20	0.00	0.00
15549	10613	20	0.00	0.00
15550	10614	20	0.00	0.00
15551	10615	20	0.00	0.00
15552	10616	20	0.00	0.00
15553	10617	20	0.00	0.00
15554	10618	20	0.00	0.00
15555	10619	20	0.00	0.00
15556	10620	20	0.00	0.00
15557	10621	20	0.00	0.00
15558	10622	20	0.00	0.00
15559	10623	20	0.00	0.00
15560	10624	20	0.00	0.00
15561	10625	20	0.00	0.00
15562	10626	20	0.00	0.00
15563	10627	20	0.00	0.00
15564	10628	20	0.00	0.00
15565	10629	20	0.00	0.00
15566	10630	20	0.00	0.00
15567	10631	20	0.00	0.00
15700	10308	20	0.00	0.00
15701	10302	20	0.00	0.00
15702	10414	20	0.00	0.00
15703	10047	20	0.00	0.00
15724	10396	20	0.00	0.00
24398	10788	24	0.00	0.00
24399	10788	20	0.00	0.00
24400	10788	27	0.00	0.00
24401	10788	16	0.00	0.00
15735	10249	20	0.00	0.00
15736	10243	20	0.00	0.00
15737	10029	20	0.00	0.00
15739	10398	20	0.00	0.00
15740	10044	20	0.00	0.00
24414	10308	32	0.00	0.00
24415	10302	32	0.00	0.00
24416	10034	32	0.00	0.00
24417	10414	32	0.00	0.00
24418	10047	32	0.00	0.00
24421	10320	32	0.00	0.00
24428	10398	32	0.00	0.00
24431	10078	32	0.00	0.00
24432	10003	32	0.00	0.00
24437	10002	32	0.00	0.00
24438	10004	32	0.00	0.00
24439	10005	32	0.00	0.00
24440	10006	32	0.00	0.00
24441	10007	32	0.00	0.00
24442	10008	32	0.00	0.00
24443	10009	32	0.00	0.00
24444	10010	32	0.00	0.00
24445	10011	32	0.00	0.00
24446	10012	32	0.00	0.00
24447	10013	32	0.00	0.00
24448	10014	32	0.00	0.00
24449	10015	32	0.00	0.00
24450	10016	32	0.00	0.00
24451	10017	32	0.00	0.00
24452	10018	32	0.00	0.00
24453	10019	32	0.00	0.00
24454	10020	32	0.00	0.00
24455	10021	32	0.00	0.00
24456	10022	32	0.00	0.00
24457	10023	32	0.00	0.00
24458	10024	32	0.00	0.00
24459	10025	32	0.00	0.00
24460	10026	32	0.00	0.00
24461	10027	32	0.00	0.00
24462	10028	32	0.00	0.00
24463	10030	32	0.00	0.00
24464	10032	32	0.00	0.00
24465	10033	32	0.00	0.00
24466	10035	32	0.00	0.00
24467	10036	32	0.00	0.00
24468	10037	32	0.00	0.00
24469	10038	32	0.00	0.00
24470	10039	32	0.00	0.00
24471	10040	32	0.00	0.00
24472	10041	32	0.00	0.00
24473	10042	32	0.00	0.00
24474	10043	32	0.00	0.00
24475	10045	32	0.00	0.00
24476	10046	32	0.00	0.00
24477	10048	32	0.00	0.00
24478	10049	32	0.00	0.00
24479	10050	32	0.00	0.00
24480	10051	32	0.00	0.00
24481	10052	32	0.00	0.00
24482	10053	32	0.00	0.00
24483	10054	32	0.00	0.00
24484	10055	32	0.00	0.00
24485	10056	32	0.00	0.00
24486	10058	32	0.00	0.00
24487	10059	32	0.00	0.00
24488	10060	32	0.00	0.00
24489	10061	32	0.00	0.00
24490	10062	32	0.00	0.00
24491	10063	32	0.00	0.00
24492	10064	32	0.00	0.00
24494	10066	32	0.00	0.00
24495	10067	32	0.00	0.00
24496	10069	32	0.00	0.00
24497	10070	32	0.00	0.00
24498	10071	32	0.00	0.00
24499	10072	32	0.00	0.00
24500	10073	32	0.00	0.00
24501	10074	32	0.00	0.00
24502	10075	32	0.00	0.00
24503	10076	32	0.00	0.00
24504	10077	32	0.00	0.00
24505	10079	32	0.00	0.00
24506	10080	32	0.00	0.00
24507	10081	32	0.00	0.00
24508	10082	32	0.00	0.00
24509	10083	32	0.00	0.00
24510	10084	32	0.00	0.00
24511	10085	32	0.00	0.00
24512	10086	32	0.00	0.00
24513	10087	32	0.00	0.00
24514	10088	32	0.00	0.00
24515	10089	32	0.00	0.00
24516	10090	32	0.00	0.00
24517	10091	32	0.00	0.00
24518	10092	32	0.00	0.00
24519	10093	32	0.00	0.00
24520	10094	32	0.00	0.00
24521	10095	32	0.00	0.00
24522	10096	32	0.00	0.00
24523	10097	32	0.00	0.00
24524	10098	32	0.00	0.00
24525	10099	32	0.00	0.00
24526	10100	32	0.00	0.00
24527	10101	32	0.00	0.00
24528	10102	32	0.00	0.00
24529	10103	32	0.00	0.00
24530	10104	32	0.00	0.00
24531	10105	32	0.00	0.00
24532	10106	32	0.00	0.00
24533	10107	32	0.00	0.00
24534	10108	32	0.00	0.00
24535	10109	32	0.00	0.00
24536	10110	32	0.00	0.00
24537	10111	32	0.00	0.00
24538	10112	32	0.00	0.00
24539	10113	32	0.00	0.00
24540	10114	32	0.00	0.00
24541	10115	32	0.00	0.00
24542	10116	32	0.00	0.00
24543	10117	32	0.00	0.00
24544	10118	32	0.00	0.00
24545	10119	32	0.00	0.00
24546	10120	32	0.00	0.00
24547	10121	32	0.00	0.00
24548	10122	32	0.00	0.00
24549	10123	32	0.00	0.00
24550	10124	32	0.00	0.00
24551	10125	32	0.00	0.00
24552	10126	32	0.00	0.00
24553	10127	32	0.00	0.00
24554	10128	32	0.00	0.00
24555	10129	32	0.00	0.00
24556	10130	32	0.00	0.00
24557	10131	32	0.00	0.00
24558	10132	32	0.00	0.00
24559	10133	32	0.00	0.00
24560	10134	32	0.00	0.00
24561	10135	32	0.00	0.00
24562	10136	32	0.00	0.00
24563	10137	32	0.00	0.00
24564	10138	32	0.00	0.00
24565	10139	32	0.00	0.00
24566	10140	32	0.00	0.00
24567	10141	32	0.00	0.00
24568	10142	32	0.00	0.00
24569	10143	32	0.00	0.00
24570	10144	32	0.00	0.00
24571	10145	32	0.00	0.00
24572	10146	32	0.00	0.00
24573	10147	32	0.00	0.00
24574	10148	32	0.00	0.00
24575	10149	32	0.00	0.00
24576	10150	32	0.00	0.00
24577	10151	32	0.00	0.00
24578	10152	32	0.00	0.00
24579	10153	32	0.00	0.00
24580	10154	32	0.00	0.00
24581	10155	32	0.00	0.00
24582	10156	32	0.00	0.00
24583	10157	32	0.00	0.00
24584	10158	32	0.00	0.00
24585	10159	32	0.00	0.00
24586	10160	32	0.00	0.00
24587	10161	32	0.00	0.00
24588	10162	32	0.00	0.00
24589	10163	32	0.00	0.00
24590	10164	32	0.00	0.00
24591	10165	32	0.00	0.00
24592	10166	32	0.00	0.00
24593	10167	32	0.00	0.00
24594	10168	32	0.00	0.00
24595	10169	32	0.00	0.00
24596	10170	32	0.00	0.00
24597	10171	32	0.00	0.00
24598	10172	32	0.00	0.00
24599	10173	32	0.00	0.00
24600	10174	32	0.00	0.00
24601	10175	32	0.00	0.00
24602	10176	32	0.00	0.00
24603	10177	32	0.00	0.00
24604	10178	32	0.00	0.00
24605	10179	32	0.00	0.00
24606	10180	32	0.00	0.00
24607	10181	32	0.00	0.00
24608	10182	32	0.00	0.00
24609	10183	32	0.00	0.00
24610	10184	32	0.00	0.00
24611	10185	32	0.00	0.00
24612	10186	32	0.00	0.00
24613	10187	32	0.00	0.00
24614	10188	32	0.00	0.00
24615	10189	32	0.00	0.00
24616	10190	32	0.00	0.00
24617	10191	32	0.00	0.00
24618	10192	32	0.00	0.00
24619	10193	32	0.00	0.00
24620	10194	32	0.00	0.00
24621	10195	32	0.00	0.00
24622	10196	32	0.00	0.00
24623	10197	32	0.00	0.00
24624	10198	32	0.00	0.00
24625	10199	32	0.00	0.00
24626	10200	32	0.00	0.00
24627	10201	32	0.00	0.00
24628	10202	32	0.00	0.00
24629	10203	32	0.00	0.00
24630	10204	32	0.00	0.00
24631	10205	32	0.00	0.00
24632	10206	32	0.00	0.00
24633	10207	32	0.00	0.00
24634	10208	32	0.00	0.00
24635	10209	32	0.00	0.00
24636	10210	32	0.00	0.00
24637	10211	32	0.00	0.00
24638	10212	32	0.00	0.00
24639	10213	32	0.00	0.00
24640	10214	32	0.00	0.00
24641	10215	32	0.00	0.00
24642	10216	32	0.00	0.00
24643	10217	32	0.00	0.00
24644	10218	32	0.00	0.00
24645	10219	32	0.00	0.00
24646	10220	32	0.00	0.00
24647	10221	32	0.00	0.00
24648	10222	32	0.00	0.00
24649	10223	32	0.00	0.00
24650	10224	32	0.00	0.00
24651	10225	32	0.00	0.00
24652	10226	32	0.00	0.00
24653	10227	32	0.00	0.00
24654	10228	32	0.00	0.00
24655	10229	32	0.00	0.00
24656	10230	32	0.00	0.00
24657	10231	32	0.00	0.00
24658	10232	32	0.00	0.00
24659	10233	32	0.00	0.00
24660	10234	32	0.00	0.00
24661	10235	32	0.00	0.00
24662	10236	32	0.00	0.00
24663	10237	32	0.00	0.00
24664	10238	32	0.00	0.00
24665	10239	32	0.00	0.00
24666	10240	32	0.00	0.00
24667	10241	32	0.00	0.00
24668	10242	32	0.00	0.00
24669	10244	32	0.00	0.00
24670	10245	32	0.00	0.00
24671	10246	32	0.00	0.00
24672	10247	32	0.00	0.00
24673	10248	32	0.00	0.00
24674	10250	32	0.00	0.00
24675	10251	32	0.00	0.00
24677	10253	32	0.00	0.00
24678	10254	32	0.00	0.00
24679	10255	32	0.00	0.00
24680	10256	32	0.00	0.00
24681	10257	32	0.00	0.00
24682	10258	32	0.00	0.00
24683	10259	32	0.00	0.00
24684	10260	32	0.00	0.00
24685	10261	32	0.00	0.00
24686	10262	32	0.00	0.00
24687	10263	32	0.00	0.00
24688	10264	32	0.00	0.00
24689	10265	32	0.00	0.00
24690	10266	32	0.00	0.00
24691	10267	32	0.00	0.00
24692	10268	32	0.00	0.00
24693	10269	32	0.00	0.00
24694	10272	32	0.00	0.00
24695	10273	32	0.00	0.00
24696	10274	32	0.00	0.00
24697	10275	32	0.00	0.00
24698	10276	32	0.00	0.00
24699	10277	32	0.00	0.00
24700	10278	32	0.00	0.00
24701	10279	32	0.00	0.00
24702	10280	32	0.00	0.00
24703	10281	32	0.00	0.00
24704	10282	32	0.00	0.00
24705	10283	32	0.00	0.00
24706	10284	32	0.00	0.00
24707	10285	32	0.00	0.00
24708	10286	32	0.00	0.00
24709	10287	32	0.00	0.00
24710	10288	32	0.00	0.00
24711	10289	32	0.00	0.00
24712	10290	32	0.00	0.00
24713	10291	32	0.00	0.00
24714	10292	32	0.00	0.00
24715	10293	32	0.00	0.00
24716	10294	32	0.00	0.00
24717	10295	32	0.00	0.00
24718	10296	32	0.00	0.00
24719	10297	32	0.00	0.00
24720	10299	32	0.00	0.00
24721	10300	32	0.00	0.00
24722	10301	32	0.00	0.00
24723	10303	32	0.00	0.00
24724	10304	32	0.00	0.00
24725	10305	32	0.00	0.00
24726	10306	32	0.00	0.00
24727	10307	32	0.00	0.00
24728	10309	32	0.00	0.00
24729	10310	32	0.00	0.00
24730	10311	32	0.00	0.00
24731	10312	32	0.00	0.00
24732	10313	32	0.00	0.00
24733	10314	32	0.00	0.00
24734	10315	32	0.00	0.00
24735	10316	32	0.00	0.00
24736	10317	32	0.00	0.00
24737	10319	32	0.00	0.00
24738	10321	32	0.00	0.00
24739	10322	32	0.00	0.00
24740	10323	32	0.00	0.00
24741	10324	32	0.00	0.00
24742	10325	32	0.00	0.00
24743	10326	32	0.00	0.00
24744	10327	32	0.00	0.00
24745	10328	32	0.00	0.00
24746	10329	32	0.00	0.00
24747	10330	32	0.00	0.00
24748	10331	32	0.00	0.00
24749	10332	32	0.00	0.00
24750	10333	32	0.00	0.00
24751	10334	32	0.00	0.00
24752	10335	32	0.00	0.00
24753	10336	32	0.00	0.00
24754	10337	32	0.00	0.00
24755	10338	32	0.00	0.00
24756	10339	32	0.00	0.00
24757	10340	32	0.00	0.00
24758	10341	32	0.00	0.00
24759	10342	32	0.00	0.00
24760	10343	32	0.00	0.00
24761	10344	32	0.00	0.00
24762	10345	32	0.00	0.00
24763	10346	32	0.00	0.00
24764	10347	32	0.00	0.00
24765	10348	32	0.00	0.00
24766	10349	32	0.00	0.00
24767	10350	32	0.00	0.00
24768	10351	32	0.00	0.00
24769	10352	32	0.00	0.00
24770	10354	32	0.00	0.00
24771	10353	32	0.00	0.00
24772	10355	32	0.00	0.00
24773	10356	32	0.00	0.00
24774	10357	32	0.00	0.00
24775	10358	32	0.00	0.00
24776	10359	32	0.00	0.00
24777	10360	32	0.00	0.00
24778	10361	32	0.00	0.00
24779	10362	32	0.00	0.00
24780	10363	32	0.00	0.00
24781	10364	32	0.00	0.00
24782	10365	32	0.00	0.00
24783	10366	32	0.00	0.00
24784	10367	32	0.00	0.00
24785	10368	32	0.00	0.00
24786	10369	32	0.00	0.00
24787	10370	32	0.00	0.00
24788	10371	32	0.00	0.00
24789	10372	32	0.00	0.00
24790	10373	32	0.00	0.00
24791	10374	32	0.00	0.00
24792	10375	32	0.00	0.00
24793	10376	32	0.00	0.00
24794	10377	32	0.00	0.00
24795	10378	32	0.00	0.00
24796	10379	32	0.00	0.00
24797	10380	32	0.00	0.00
24798	10381	32	0.00	0.00
24799	10382	32	0.00	0.00
24800	10383	32	0.00	0.00
24801	10384	32	0.00	0.00
24802	10385	32	0.00	0.00
24803	10386	32	0.00	0.00
24804	10387	32	0.00	0.00
24805	10388	32	0.00	0.00
24806	10389	32	0.00	0.00
24807	10390	32	0.00	0.00
24808	10391	32	0.00	0.00
24809	10392	32	0.00	0.00
24810	10393	32	0.00	0.00
24811	10394	32	0.00	0.00
24812	10395	32	0.00	0.00
24813	10397	32	0.00	0.00
24814	10399	32	0.00	0.00
24815	10400	32	0.00	0.00
24816	10401	32	0.00	0.00
24817	10402	32	0.00	0.00
24818	10403	32	0.00	0.00
24819	10404	32	0.00	0.00
24820	10405	32	0.00	0.00
24821	10406	32	0.00	0.00
24822	10407	32	0.00	0.00
24823	10408	32	0.00	0.00
24824	10409	32	0.00	0.00
24825	10410	32	0.00	0.00
24826	10411	32	0.00	0.00
24827	10412	32	0.00	0.00
24828	10413	32	0.00	0.00
24829	10415	32	0.00	0.00
24830	10416	32	0.00	0.00
24831	10417	32	0.00	0.00
24832	10418	32	0.00	0.00
24833	10419	32	0.00	0.00
24834	10420	32	0.00	0.00
24835	10421	32	0.00	0.00
24836	10422	32	0.00	0.00
24837	10423	32	0.00	0.00
24838	10424	32	0.00	0.00
24839	10425	32	0.00	0.00
24840	10426	32	0.00	0.00
24841	10427	32	0.00	0.00
24842	10428	32	0.00	0.00
24843	10429	32	0.00	0.00
24844	10430	32	0.00	0.00
24845	10431	32	0.00	0.00
24846	10432	32	0.00	0.00
24847	10433	32	0.00	0.00
24848	10434	32	0.00	0.00
24849	10435	32	0.00	0.00
24850	10436	32	0.00	0.00
24851	10437	32	0.00	0.00
24852	10438	32	0.00	0.00
24853	10439	32	0.00	0.00
24854	10440	32	0.00	0.00
24855	10441	32	0.00	0.00
24856	10442	32	0.00	0.00
24857	10443	32	0.00	0.00
24858	10444	32	0.00	0.00
24859	10445	32	0.00	0.00
24860	10446	32	0.00	0.00
24861	10447	32	0.00	0.00
24862	10448	32	0.00	0.00
24863	10449	32	0.00	0.00
24864	10450	32	0.00	0.00
24865	10451	32	0.00	0.00
24866	10452	32	0.00	0.00
24867	10453	32	0.00	0.00
24868	10454	32	0.00	0.00
24869	10455	32	0.00	0.00
24870	10456	32	0.00	0.00
24871	10457	32	0.00	0.00
24872	10458	32	0.00	0.00
24873	10459	32	0.00	0.00
24874	10460	32	0.00	0.00
24875	10461	32	0.00	0.00
24876	10462	32	0.00	0.00
24877	10463	32	0.00	0.00
24878	10464	32	0.00	0.00
24879	10465	32	0.00	0.00
24880	10466	32	0.00	0.00
24881	10467	32	0.00	0.00
24882	10468	32	0.00	0.00
24883	10469	32	0.00	0.00
24884	10470	32	0.00	0.00
24885	10471	32	0.00	0.00
24886	10472	32	0.00	0.00
24887	10473	32	0.00	0.00
24888	10474	32	0.00	0.00
24889	10475	32	0.00	0.00
24890	10476	32	0.00	0.00
24891	10477	32	0.00	0.00
24892	10478	32	0.00	0.00
24893	10479	32	0.00	0.00
24894	10480	32	0.00	0.00
24895	10481	32	0.00	0.00
24896	10482	32	0.00	0.00
24897	10483	32	0.00	0.00
24898	10484	32	0.00	0.00
24899	10485	32	0.00	0.00
24900	10486	32	0.00	0.00
24901	10487	32	0.00	0.00
24902	10488	32	0.00	0.00
24903	10489	32	0.00	0.00
24904	10490	32	0.00	0.00
24905	10491	32	0.00	0.00
24906	10492	32	0.00	0.00
24907	10493	32	0.00	0.00
24908	10494	32	0.00	0.00
24909	10495	32	0.00	0.00
24910	10496	32	0.00	0.00
24911	10497	32	0.00	0.00
24912	10498	32	0.00	0.00
24913	10499	32	0.00	0.00
24914	10500	32	0.00	0.00
24915	10501	32	0.00	0.00
24916	10502	32	0.00	0.00
24917	10503	32	0.00	0.00
24918	10504	32	0.00	0.00
24919	10505	32	0.00	0.00
24920	10506	32	0.00	0.00
24921	10507	32	0.00	0.00
24922	10508	32	0.00	0.00
24923	10509	32	0.00	0.00
24924	10510	32	0.00	0.00
24925	10511	32	0.00	0.00
24926	10512	32	0.00	0.00
24927	10513	32	0.00	0.00
24928	10514	32	0.00	0.00
24929	10515	32	0.00	0.00
24930	10516	32	0.00	0.00
24931	10517	32	0.00	0.00
24932	10518	32	0.00	0.00
24933	10519	32	0.00	0.00
24934	10520	32	0.00	0.00
24935	10521	32	0.00	0.00
24936	10522	32	0.00	0.00
24937	10523	32	0.00	0.00
24938	10524	32	0.00	0.00
24939	10525	32	0.00	0.00
24940	10526	32	0.00	0.00
24941	10527	32	0.00	0.00
24942	10528	32	0.00	0.00
24943	10529	32	0.00	0.00
24944	10530	32	0.00	0.00
24945	10531	32	0.00	0.00
24946	10532	32	0.00	0.00
24947	10533	32	0.00	0.00
24948	10535	32	0.00	0.00
24949	10536	32	0.00	0.00
24950	10537	32	0.00	0.00
24951	10538	32	0.00	0.00
24952	10539	32	0.00	0.00
24953	10540	32	0.00	0.00
24954	10541	32	0.00	0.00
24955	10542	32	0.00	0.00
24956	10543	32	0.00	0.00
24957	10544	32	0.00	0.00
24958	10545	32	0.00	0.00
24959	10546	32	0.00	0.00
24960	10547	32	0.00	0.00
24961	10548	32	0.00	0.00
24962	10549	32	0.00	0.00
24963	10550	32	0.00	0.00
24964	10551	32	0.00	0.00
24965	10552	32	0.00	0.00
24966	10553	32	0.00	0.00
24967	10554	32	0.00	0.00
24968	10555	32	0.00	0.00
24969	10556	32	0.00	0.00
24970	10557	32	0.00	0.00
24971	10558	32	0.00	0.00
24972	10559	32	0.00	0.00
24973	10560	32	0.00	0.00
24974	10561	32	0.00	0.00
24975	10562	32	0.00	0.00
24976	10563	32	0.00	0.00
24977	10564	32	0.00	0.00
24978	10565	32	0.00	0.00
24979	10566	32	0.00	0.00
24980	10567	32	0.00	0.00
24981	10568	32	0.00	0.00
24982	10569	32	0.00	0.00
24983	10570	32	0.00	0.00
24984	10571	32	0.00	0.00
24985	10572	32	0.00	0.00
24986	10573	32	0.00	0.00
24987	10574	32	0.00	0.00
24988	10575	32	0.00	0.00
24989	10576	32	0.00	0.00
24990	10577	32	0.00	0.00
24991	10578	32	0.00	0.00
24992	10579	32	0.00	0.00
24993	10580	32	0.00	0.00
24994	10581	32	0.00	0.00
24995	10582	32	0.00	0.00
24996	10583	32	0.00	0.00
24997	10584	32	0.00	0.00
24998	10585	32	0.00	0.00
24999	10586	32	0.00	0.00
25000	10587	32	0.00	0.00
25001	10588	32	0.00	0.00
25002	10589	32	0.00	0.00
25003	10590	32	0.00	0.00
25004	10591	32	0.00	0.00
25005	10592	32	0.00	0.00
25006	10593	32	0.00	0.00
25007	10594	32	0.00	0.00
25008	10595	32	0.00	0.00
25009	10596	32	0.00	0.00
25010	10597	32	0.00	0.00
25011	10598	32	0.00	0.00
25012	10599	32	0.00	0.00
25013	10600	32	0.00	0.00
25014	10601	32	0.00	0.00
25015	10602	32	0.00	0.00
25016	10603	32	0.00	0.00
25017	10604	32	0.00	0.00
25018	10605	32	0.00	0.00
25019	10606	32	0.00	0.00
25020	10607	32	0.00	0.00
25021	10608	32	0.00	0.00
25022	10609	32	0.00	0.00
25023	10610	32	0.00	0.00
25024	10611	32	0.00	0.00
25025	10612	32	0.00	0.00
25026	10613	32	0.00	0.00
25027	10614	32	0.00	0.00
25028	10615	32	0.00	0.00
25029	10616	32	0.00	0.00
25030	10617	32	0.00	0.00
25031	10618	32	0.00	0.00
25032	10619	32	0.00	0.00
25033	10620	32	0.00	0.00
25034	10621	32	0.00	0.00
25035	10622	32	0.00	0.00
25036	10623	32	0.00	0.00
25037	10624	32	0.00	0.00
25038	10625	32	0.00	0.00
25039	10626	32	0.00	0.00
25040	10627	32	0.00	0.00
25041	10628	32	0.00	0.00
25042	10629	32	0.00	0.00
25043	10630	32	0.00	0.00
25044	10631	32	0.00	0.00
25138	10396	32	0.00	0.00
25139	10044	32	0.00	0.00
25189	10318	32	0.00	0.00
25190	10298	32	0.00	0.00
25191	10271	32	0.00	0.00
25192	10270	32	0.00	0.00
25193	10031	32	0.00	0.00
25194	10534	32	0.00	0.00
25195	10057	32	0.00	0.00
25199	10788	32	0.00	0.00
25201	10249	32	0.00	0.00
25202	10243	32	0.00	0.00
25203	10029	32	0.00	0.00
24435	10068	32	0.00	0.00
24436	10001	32	0.00	0.00
20527	10068	27	0.00	0.00
20467	10001	27	0.00	0.00
18166	10068	24	0.00	0.00
18106	10001	24	0.00	0.00
15017	10068	20	0.00	0.00
14956	10001	20	0.00	0.00
11869	10068	16	0.00	0.00
11808	10001	16	0.00	0.00
25204	10794	27	0.00	0.00
25205	10794	32	0.00	0.00
25206	10794	24	0.00	0.00
25207	10794	20	0.00	0.00
25208	10794	16	0.00	0.00
25209	10795	27	0.00	0.00
25210	10795	32	0.00	0.00
25211	10795	24	0.00	0.00
25212	10795	20	0.00	0.00
25213	10795	16	0.00	0.00
25214	10796	27	0.00	0.00
25215	10796	32	0.00	0.00
25216	10796	24	0.00	0.00
25217	10796	20	0.00	0.00
25218	10796	16	0.00	0.00
25219	10797	27	0.00	0.00
25220	10797	32	0.00	0.00
25221	10797	24	0.00	0.00
25222	10797	20	0.00	0.00
25223	10797	16	0.00	0.00
25224	10798	27	0.00	0.00
25225	10798	32	0.00	0.00
25226	10798	24	0.00	0.00
25227	10798	20	0.00	0.00
25228	10798	16	0.00	0.00
25229	10799	27	0.00	0.00
25230	10799	32	0.00	0.00
25231	10799	24	0.00	0.00
25232	10799	20	0.00	0.00
25233	10799	16	0.00	0.00
25234	10800	27	0.00	0.00
25235	10800	32	0.00	0.00
25236	10800	24	0.00	0.00
25237	10800	20	0.00	0.00
25238	10800	16	0.00	0.00
25239	10801	27	0.00	0.00
25240	10801	32	0.00	0.00
25241	10801	24	0.00	0.00
25242	10801	20	0.00	0.00
25243	10801	16	0.00	0.00
25244	10802	27	0.00	0.00
25245	10802	32	0.00	0.00
25246	10802	24	0.00	0.00
25247	10802	20	0.00	0.00
25248	10802	16	0.00	0.00
25249	10803	27	0.00	0.00
25250	10803	32	0.00	0.00
25251	10803	24	0.00	0.00
25252	10803	20	0.00	0.00
25253	10803	16	0.00	0.00
25254	10804	27	0.00	0.00
25255	10804	32	0.00	0.00
25256	10804	24	0.00	0.00
25257	10804	20	0.00	0.00
25258	10804	16	0.00	0.00
25259	10805	27	0.00	0.00
25260	10805	32	0.00	0.00
25261	10805	24	0.00	0.00
25262	10805	20	0.00	0.00
25263	10805	16	0.00	0.00
25264	10806	27	0.00	0.00
25265	10806	32	0.00	0.00
25266	10806	24	0.00	0.00
25267	10806	20	0.00	0.00
25268	10806	16	0.00	0.00
25269	10807	27	0.00	0.00
25270	10807	32	0.00	0.00
25271	10807	24	0.00	0.00
25272	10807	20	0.00	0.00
25273	10807	16	0.00	0.00
25274	10808	27	0.00	0.00
25275	10808	32	0.00	0.00
25276	10808	24	0.00	0.00
25277	10808	20	0.00	0.00
25278	10808	16	0.00	0.00
25279	10809	27	0.00	0.00
25280	10809	32	0.00	0.00
25281	10809	24	0.00	0.00
25282	10809	20	0.00	0.00
25283	10809	16	0.00	0.00
25284	10810	27	0.00	0.00
25285	10810	32	0.00	0.00
25286	10810	24	0.00	0.00
25287	10810	20	0.00	0.00
25288	10810	16	0.00	0.00
25289	10811	27	0.00	0.00
25290	10811	32	0.00	0.00
25291	10811	24	0.00	0.00
25292	10811	20	0.00	0.00
25293	10811	16	0.00	0.00
25294	10812	27	0.00	0.00
25295	10812	32	0.00	0.00
25296	10812	24	0.00	0.00
25297	10812	20	0.00	0.00
25298	10812	16	0.00	0.00
25299	10813	27	0.00	0.00
25300	10813	32	0.00	0.00
25301	10813	24	0.00	0.00
25302	10813	20	0.00	0.00
25303	10813	16	0.00	0.00
25304	10814	27	0.00	0.00
25305	10814	32	0.00	0.00
25306	10814	24	0.00	0.00
25307	10814	20	0.00	0.00
25308	10814	16	0.00	0.00
25309	10815	27	0.00	0.00
25310	10815	32	0.00	0.00
25311	10815	24	0.00	0.00
25312	10815	20	0.00	0.00
25313	10815	16	0.00	0.00
25314	10816	27	0.00	0.00
25315	10816	32	0.00	0.00
25316	10816	24	0.00	0.00
25317	10816	20	0.00	0.00
25318	10816	16	0.00	0.00
25319	10817	27	0.00	0.00
25320	10817	32	0.00	0.00
25321	10817	24	0.00	0.00
25322	10817	20	0.00	0.00
25323	10817	16	0.00	0.00
25324	10818	27	0.00	0.00
25325	10818	32	0.00	0.00
25326	10818	24	0.00	0.00
25327	10818	20	0.00	0.00
25328	10818	16	0.00	0.00
25329	10819	27	0.00	0.00
25330	10819	32	0.00	0.00
25331	10819	24	0.00	0.00
25332	10819	20	0.00	0.00
25333	10819	16	0.00	0.00
25334	10820	27	0.00	0.00
25335	10820	32	0.00	0.00
25336	10820	24	0.00	0.00
25337	10820	20	0.00	0.00
25338	10820	16	0.00	0.00
25339	10821	27	0.00	0.00
25340	10821	32	0.00	0.00
25341	10821	24	0.00	0.00
25342	10821	20	0.00	0.00
25343	10821	16	0.00	0.00
25344	10822	27	0.00	0.00
25345	10822	32	0.00	0.00
25346	10822	24	0.00	0.00
25347	10822	20	0.00	0.00
25348	10822	16	0.00	0.00
25349	10823	27	0.00	0.00
25350	10823	32	0.00	0.00
25351	10823	24	0.00	0.00
25352	10823	20	0.00	0.00
25353	10823	16	0.00	0.00
25354	10824	27	0.00	0.00
25355	10824	32	0.00	0.00
25356	10824	24	0.00	0.00
25357	10824	20	0.00	0.00
25358	10824	16	0.00	0.00
25359	10825	27	0.00	0.00
25360	10825	32	0.00	0.00
25361	10825	24	0.00	0.00
25362	10825	20	0.00	0.00
25363	10825	16	0.00	0.00
25364	10826	27	0.00	0.00
25365	10826	32	0.00	0.00
25366	10826	24	0.00	0.00
25367	10826	20	0.00	0.00
25368	10826	16	0.00	0.00
25369	10827	27	0.00	0.00
25370	10827	32	0.00	0.00
25371	10827	24	0.00	0.00
25372	10827	20	0.00	0.00
25373	10827	16	0.00	0.00
25374	10828	27	0.00	0.00
25375	10828	32	0.00	0.00
25376	10828	24	0.00	0.00
25377	10828	20	0.00	0.00
25378	10828	16	0.00	0.00
25379	10829	27	0.00	0.00
25380	10829	32	0.00	0.00
25381	10829	24	0.00	0.00
25382	10829	20	0.00	0.00
25383	10829	16	0.00	0.00
25384	10830	27	0.00	0.00
25385	10830	32	0.00	0.00
25386	10830	24	0.00	0.00
25387	10830	20	0.00	0.00
25388	10830	16	0.00	0.00
25389	10831	27	0.00	0.00
25390	10831	32	0.00	0.00
25391	10831	24	0.00	0.00
25392	10831	20	0.00	0.00
25393	10831	16	0.00	0.00
25394	10832	27	0.00	0.00
25395	10832	32	0.00	0.00
25396	10832	24	0.00	0.00
25397	10832	20	0.00	0.00
25398	10832	16	0.00	0.00
25399	10833	27	0.00	0.00
25400	10833	32	0.00	0.00
25401	10833	24	0.00	0.00
25402	10833	20	0.00	0.00
25403	10833	16	0.00	0.00
25404	10834	27	0.00	0.00
25405	10834	32	0.00	0.00
25406	10834	24	0.00	0.00
25407	10834	20	0.00	0.00
25408	10834	16	0.00	0.00
25409	10835	27	0.00	0.00
25410	10835	32	0.00	0.00
25411	10835	24	0.00	0.00
25412	10835	20	0.00	0.00
25413	10835	16	0.00	0.00
25414	10836	27	0.00	0.00
25415	10836	32	0.00	0.00
25416	10836	24	0.00	0.00
25417	10836	20	0.00	0.00
25418	10836	16	0.00	0.00
25419	10837	27	0.00	0.00
25420	10837	32	0.00	0.00
25421	10837	24	0.00	0.00
25422	10837	20	0.00	0.00
25423	10837	16	0.00	0.00
25424	10838	27	0.00	0.00
25425	10838	32	0.00	0.00
25426	10838	24	0.00	0.00
25427	10838	20	0.00	0.00
25428	10838	16	0.00	0.00
25429	10839	27	0.00	0.00
25430	10839	32	0.00	0.00
25431	10839	24	0.00	0.00
25432	10839	20	0.00	0.00
25433	10839	16	0.00	0.00
25434	10840	27	0.00	0.00
25435	10840	32	0.00	0.00
25436	10840	24	0.00	0.00
25437	10840	20	0.00	0.00
25438	10840	16	0.00	0.00
25439	10841	27	0.00	0.00
25440	10841	32	0.00	0.00
25441	10841	24	0.00	0.00
25442	10841	20	0.00	0.00
25443	10841	16	0.00	0.00
25444	10842	27	0.00	0.00
25445	10842	32	0.00	0.00
25446	10842	24	0.00	0.00
25447	10842	20	0.00	0.00
25448	10842	16	0.00	0.00
25449	10843	27	0.00	0.00
25450	10843	32	0.00	0.00
25451	10843	24	0.00	0.00
25452	10843	20	0.00	0.00
25453	10843	16	0.00	0.00
25454	10844	27	0.00	0.00
25455	10844	32	0.00	0.00
25456	10844	24	0.00	0.00
25457	10844	20	0.00	0.00
25458	10844	16	0.00	0.00
25459	10845	27	0.00	0.00
25460	10845	32	0.00	0.00
25461	10845	24	0.00	0.00
25462	10845	20	0.00	0.00
25463	10845	16	0.00	0.00
25464	10846	27	0.00	0.00
25465	10846	32	0.00	0.00
25466	10846	24	0.00	0.00
25467	10846	20	0.00	0.00
25468	10846	16	0.00	0.00
25469	10847	27	0.00	0.00
25470	10847	32	0.00	0.00
25471	10847	24	0.00	0.00
25472	10847	20	0.00	0.00
25473	10847	16	0.00	0.00
25474	10848	27	0.00	0.00
25475	10848	32	0.00	0.00
25476	10848	24	0.00	0.00
25477	10848	20	0.00	0.00
25478	10848	16	0.00	0.00
25479	10849	27	0.00	0.00
25480	10849	32	0.00	0.00
25481	10849	24	0.00	0.00
25482	10849	20	0.00	0.00
25483	10849	16	0.00	0.00
25484	10850	27	0.00	0.00
25485	10850	32	0.00	0.00
25486	10850	24	0.00	0.00
25487	10850	20	0.00	0.00
25488	10850	16	0.00	0.00
25489	10851	27	0.00	0.00
25490	10851	32	0.00	0.00
25491	10851	24	0.00	0.00
25492	10851	20	0.00	0.00
25493	10851	16	0.00	0.00
25494	10852	27	0.00	0.00
25495	10852	32	0.00	0.00
25496	10852	24	0.00	0.00
25497	10852	20	0.00	0.00
25498	10852	16	0.00	0.00
25499	10853	27	0.00	0.00
25500	10853	32	0.00	0.00
25501	10853	24	0.00	0.00
25502	10853	20	0.00	0.00
25503	10853	16	0.00	0.00
25504	10854	27	0.00	0.00
25505	10854	32	0.00	0.00
25506	10854	24	0.00	0.00
25507	10854	20	0.00	0.00
25508	10854	16	0.00	0.00
25509	10855	27	0.00	0.00
25510	10855	32	0.00	0.00
25511	10855	24	0.00	0.00
25512	10855	20	0.00	0.00
25513	10855	16	0.00	0.00
25514	10856	27	0.00	0.00
25515	10856	32	0.00	0.00
25516	10856	24	0.00	0.00
25517	10856	20	0.00	0.00
25518	10856	16	0.00	0.00
25519	10857	27	0.00	0.00
25520	10857	32	0.00	0.00
25521	10857	24	0.00	0.00
25522	10857	20	0.00	0.00
25523	10857	16	0.00	0.00
25524	10858	27	0.00	0.00
25525	10858	32	0.00	0.00
25526	10858	24	0.00	0.00
25527	10858	20	0.00	0.00
25528	10858	16	0.00	0.00
25529	10859	27	0.00	0.00
25530	10859	32	0.00	0.00
25531	10859	24	0.00	0.00
25532	10859	20	0.00	0.00
25533	10859	16	0.00	0.00
25534	10860	27	0.00	0.00
25535	10860	32	0.00	0.00
25536	10860	24	0.00	0.00
25537	10860	20	0.00	0.00
25538	10860	16	0.00	0.00
25539	10861	27	0.00	0.00
25540	10861	32	0.00	0.00
25541	10861	24	0.00	0.00
25542	10861	20	0.00	0.00
25543	10861	16	0.00	0.00
25544	10862	27	0.00	0.00
25545	10862	32	0.00	0.00
25546	10862	24	0.00	0.00
25547	10862	20	0.00	0.00
25548	10862	16	0.00	0.00
25549	10863	27	0.00	0.00
25550	10863	32	0.00	0.00
25551	10863	24	0.00	0.00
25552	10863	20	0.00	0.00
25553	10863	16	0.00	0.00
25554	10864	27	0.00	0.00
25555	10864	32	0.00	0.00
25556	10864	24	0.00	0.00
25557	10864	20	0.00	0.00
25558	10864	16	0.00	0.00
25559	10865	27	0.00	0.00
25560	10865	32	0.00	0.00
25561	10865	24	0.00	0.00
25562	10865	20	0.00	0.00
25563	10865	16	0.00	0.00
25564	10866	27	0.00	0.00
25565	10866	32	0.00	0.00
25566	10866	24	0.00	0.00
25567	10866	20	0.00	0.00
25568	10866	16	0.00	0.00
25569	10867	27	0.00	0.00
25570	10867	32	0.00	0.00
25571	10867	24	0.00	0.00
25572	10867	20	0.00	0.00
25573	10867	16	0.00	0.00
25574	10868	27	0.00	0.00
25575	10868	32	0.00	0.00
25576	10868	24	0.00	0.00
25577	10868	20	0.00	0.00
25578	10868	16	0.00	0.00
25579	10869	27	0.00	0.00
25580	10869	32	0.00	0.00
25581	10869	24	0.00	0.00
25582	10869	20	0.00	0.00
25583	10869	16	0.00	0.00
25584	10870	27	0.00	0.00
25585	10870	32	0.00	0.00
25586	10870	24	0.00	0.00
25587	10870	20	0.00	0.00
25588	10870	16	0.00	0.00
25589	10871	27	0.00	0.00
25590	10871	32	0.00	0.00
25591	10871	24	0.00	0.00
25592	10871	20	0.00	0.00
25593	10871	16	0.00	0.00
25594	10872	27	0.00	0.00
25595	10872	32	0.00	0.00
25596	10872	24	0.00	0.00
25597	10872	20	0.00	0.00
25598	10872	16	0.00	0.00
25599	10873	27	0.00	0.00
25600	10873	32	0.00	0.00
25601	10873	24	0.00	0.00
25602	10873	20	0.00	0.00
25603	10873	16	0.00	0.00
25604	10874	27	0.00	0.00
25605	10874	32	0.00	0.00
25606	10874	24	0.00	0.00
25607	10874	20	0.00	0.00
25608	10874	16	0.00	0.00
25609	10875	27	0.00	0.00
25610	10875	32	0.00	0.00
25611	10875	24	0.00	0.00
25612	10875	20	0.00	0.00
25613	10875	16	0.00	0.00
25614	10876	27	0.00	0.00
25615	10876	32	0.00	0.00
25616	10876	24	0.00	0.00
25617	10876	20	0.00	0.00
25618	10876	16	0.00	0.00
25619	10877	27	0.00	0.00
25620	10877	32	0.00	0.00
25621	10877	24	0.00	0.00
25622	10877	20	0.00	0.00
25623	10877	16	0.00	0.00
25624	10878	27	0.00	0.00
25625	10878	32	0.00	0.00
25626	10878	24	0.00	0.00
25627	10878	20	0.00	0.00
25628	10878	16	0.00	0.00
25629	10879	27	0.00	0.00
25630	10879	32	0.00	0.00
25631	10879	24	0.00	0.00
25632	10879	20	0.00	0.00
25633	10879	16	0.00	0.00
25634	10880	27	0.00	0.00
25635	10880	32	0.00	0.00
25636	10880	24	0.00	0.00
25637	10880	20	0.00	0.00
25638	10880	16	0.00	0.00
25639	10881	27	0.00	0.00
25640	10881	32	0.00	0.00
25641	10881	24	0.00	0.00
25642	10881	20	0.00	0.00
25643	10881	16	0.00	0.00
25644	10882	27	0.00	0.00
25645	10882	32	0.00	0.00
25646	10882	24	0.00	0.00
25647	10882	20	0.00	0.00
25648	10882	16	0.00	0.00
25649	10883	27	0.00	0.00
25650	10883	32	0.00	0.00
25651	10883	24	0.00	0.00
25652	10883	20	0.00	0.00
25653	10883	16	0.00	0.00
25654	10884	27	0.00	0.00
25655	10884	32	0.00	0.00
25656	10884	24	0.00	0.00
25657	10884	20	0.00	0.00
25658	10884	16	0.00	0.00
25659	10885	27	0.00	0.00
25660	10885	32	0.00	0.00
25661	10885	24	0.00	0.00
25662	10885	20	0.00	0.00
25663	10885	16	0.00	0.00
25664	10886	27	0.00	0.00
25665	10886	32	0.00	0.00
25666	10886	24	0.00	0.00
25667	10886	20	0.00	0.00
25668	10886	16	0.00	0.00
25669	10887	27	0.00	0.00
25670	10887	32	0.00	0.00
25671	10887	24	0.00	0.00
25672	10887	20	0.00	0.00
25673	10887	16	0.00	0.00
25674	10888	27	0.00	0.00
25675	10888	32	0.00	0.00
25676	10888	24	0.00	0.00
25677	10888	20	0.00	0.00
25678	10888	16	0.00	0.00
25679	10889	27	0.00	0.00
25680	10889	32	0.00	0.00
25681	10889	24	0.00	0.00
25682	10889	20	0.00	0.00
25683	10889	16	0.00	0.00
25684	10890	27	0.00	0.00
25685	10890	32	0.00	0.00
25686	10890	24	0.00	0.00
25687	10890	20	0.00	0.00
25688	10890	16	0.00	0.00
25689	10891	27	0.00	0.00
25690	10891	32	0.00	0.00
25691	10891	24	0.00	0.00
25692	10891	20	0.00	0.00
25693	10891	16	0.00	0.00
25694	10892	27	0.00	0.00
25695	10892	32	0.00	0.00
25696	10892	24	0.00	0.00
25697	10892	20	0.00	0.00
25698	10892	16	0.00	0.00
25699	10893	27	0.00	0.00
25700	10893	32	0.00	0.00
25701	10893	24	0.00	0.00
25702	10893	20	0.00	0.00
25703	10893	16	0.00	0.00
25704	10894	27	0.00	0.00
25705	10894	32	0.00	0.00
25706	10894	24	0.00	0.00
25707	10894	20	0.00	0.00
25708	10894	16	0.00	0.00
25709	10895	27	0.00	0.00
25710	10895	32	0.00	0.00
25711	10895	24	0.00	0.00
25712	10895	20	0.00	0.00
25713	10895	16	0.00	0.00
25714	10896	27	0.00	0.00
25715	10896	32	0.00	0.00
25716	10896	24	0.00	0.00
25717	10896	20	0.00	0.00
25718	10896	16	0.00	0.00
25719	10897	27	0.00	0.00
25720	10897	32	0.00	0.00
25721	10897	24	0.00	0.00
25722	10897	20	0.00	0.00
25723	10897	16	0.00	0.00
25724	10898	27	0.00	0.00
25725	10898	32	0.00	0.00
25726	10898	24	0.00	0.00
25727	10898	20	0.00	0.00
25728	10898	16	0.00	0.00
25729	10899	27	0.00	0.00
25730	10899	32	0.00	0.00
25731	10899	24	0.00	0.00
25732	10899	20	0.00	0.00
25733	10899	16	0.00	0.00
25734	10900	27	0.00	0.00
25735	10900	32	0.00	0.00
25736	10900	24	0.00	0.00
25737	10900	20	0.00	0.00
25738	10900	16	0.00	0.00
25739	10901	27	0.00	0.00
25740	10901	32	0.00	0.00
25741	10901	24	0.00	0.00
25742	10901	20	0.00	0.00
25743	10901	16	0.00	0.00
25744	10902	27	0.00	0.00
25745	10902	32	0.00	0.00
25746	10902	24	0.00	0.00
25747	10902	20	0.00	0.00
25748	10902	16	0.00	0.00
25749	10903	27	0.00	0.00
25750	10903	32	0.00	0.00
25751	10903	24	0.00	0.00
25752	10903	20	0.00	0.00
25753	10903	16	0.00	0.00
25754	10904	27	0.00	0.00
25755	10904	32	0.00	0.00
25756	10904	24	0.00	0.00
25757	10904	20	0.00	0.00
25758	10904	16	0.00	0.00
25759	10905	27	0.00	0.00
25760	10905	32	0.00	0.00
25761	10905	24	0.00	0.00
25762	10905	20	0.00	0.00
25763	10905	16	0.00	0.00
25764	10906	27	0.00	0.00
25765	10906	32	0.00	0.00
25766	10906	24	0.00	0.00
25767	10906	20	0.00	0.00
25768	10906	16	0.00	0.00
25769	10907	27	0.00	0.00
25770	10907	32	0.00	0.00
25771	10907	24	0.00	0.00
25772	10907	20	0.00	0.00
25773	10907	16	0.00	0.00
25774	10908	27	0.00	0.00
25775	10908	32	0.00	0.00
25776	10908	24	0.00	0.00
25777	10908	20	0.00	0.00
25778	10908	16	0.00	0.00
25779	10909	27	0.00	0.00
25780	10909	32	0.00	0.00
25781	10909	24	0.00	0.00
25782	10909	20	0.00	0.00
25783	10909	16	0.00	0.00
25784	10910	27	0.00	0.00
25785	10910	32	0.00	0.00
25786	10910	24	0.00	0.00
25787	10910	20	0.00	0.00
25788	10910	16	0.00	0.00
25789	10911	27	0.00	0.00
25790	10911	32	0.00	0.00
25791	10911	24	0.00	0.00
25792	10911	20	0.00	0.00
25793	10911	16	0.00	0.00
25794	10912	27	0.00	0.00
25795	10912	32	0.00	0.00
25796	10912	24	0.00	0.00
25797	10912	20	0.00	0.00
25798	10912	16	0.00	0.00
25799	10913	27	0.00	0.00
25800	10913	32	0.00	0.00
25801	10913	24	0.00	0.00
25802	10913	20	0.00	0.00
25803	10913	16	0.00	0.00
25804	10914	27	0.00	0.00
25805	10914	32	0.00	0.00
25806	10914	24	0.00	0.00
25807	10914	20	0.00	0.00
25808	10914	16	0.00	0.00
25809	10915	27	0.00	0.00
25810	10915	32	0.00	0.00
25811	10915	24	0.00	0.00
25812	10915	20	0.00	0.00
25813	10915	16	0.00	0.00
25814	10916	27	0.00	0.00
25815	10916	32	0.00	0.00
25816	10916	24	0.00	0.00
25817	10916	20	0.00	0.00
25818	10916	16	0.00	0.00
25819	10917	27	0.00	0.00
25820	10917	32	0.00	0.00
25821	10917	24	0.00	0.00
25822	10917	20	0.00	0.00
25823	10917	16	0.00	0.00
25824	10918	27	0.00	0.00
25825	10918	32	0.00	0.00
25826	10918	24	0.00	0.00
25827	10918	20	0.00	0.00
25828	10918	16	0.00	0.00
25829	10919	27	0.00	0.00
25830	10919	32	0.00	0.00
25831	10919	24	0.00	0.00
25832	10919	20	0.00	0.00
25833	10919	16	0.00	0.00
25834	10920	27	0.00	0.00
25835	10920	32	0.00	0.00
25836	10920	24	0.00	0.00
25837	10920	20	0.00	0.00
25838	10920	16	0.00	0.00
25839	10921	27	0.00	0.00
25840	10921	32	0.00	0.00
25841	10921	24	0.00	0.00
25842	10921	20	0.00	0.00
25843	10921	16	0.00	0.00
25844	10922	27	0.00	0.00
25845	10922	32	0.00	0.00
25846	10922	24	0.00	0.00
25847	10922	20	0.00	0.00
25848	10922	16	0.00	0.00
25849	10923	27	0.00	0.00
25850	10923	32	0.00	0.00
25851	10923	24	0.00	0.00
25852	10923	20	0.00	0.00
25853	10923	16	0.00	0.00
25854	10924	27	0.00	0.00
25855	10924	32	0.00	0.00
25856	10924	24	0.00	0.00
25857	10924	20	0.00	0.00
25858	10924	16	0.00	0.00
25859	10925	27	0.00	0.00
25860	10925	32	0.00	0.00
25861	10925	24	0.00	0.00
25862	10925	20	0.00	0.00
25863	10925	16	0.00	0.00
25864	10926	27	0.00	0.00
25865	10926	32	0.00	0.00
25866	10926	24	0.00	0.00
25867	10926	20	0.00	0.00
25868	10926	16	0.00	0.00
25869	10927	27	0.00	0.00
25870	10927	32	0.00	0.00
25871	10927	24	0.00	0.00
25872	10927	20	0.00	0.00
25873	10927	16	0.00	0.00
25874	10928	27	0.00	0.00
25875	10928	32	0.00	0.00
25876	10928	24	0.00	0.00
25877	10928	20	0.00	0.00
25878	10928	16	0.00	0.00
25879	10929	27	0.00	0.00
25880	10929	32	0.00	0.00
25881	10929	24	0.00	0.00
25882	10929	20	0.00	0.00
25883	10929	16	0.00	0.00
25884	10930	27	0.00	0.00
25885	10930	32	0.00	0.00
25886	10930	24	0.00	0.00
25887	10930	20	0.00	0.00
25888	10930	16	0.00	0.00
25889	10931	27	0.00	0.00
25890	10931	32	0.00	0.00
25891	10931	24	0.00	0.00
25892	10931	20	0.00	0.00
25893	10931	16	0.00	0.00
25894	10932	27	0.00	0.00
25895	10932	32	0.00	0.00
25896	10932	24	0.00	0.00
25897	10932	20	0.00	0.00
25898	10932	16	0.00	0.00
25899	10933	27	0.00	0.00
25900	10933	32	0.00	0.00
25901	10933	24	0.00	0.00
25902	10933	20	0.00	0.00
25903	10933	16	0.00	0.00
25904	10934	27	0.00	0.00
25905	10934	32	0.00	0.00
25906	10934	24	0.00	0.00
25907	10934	20	0.00	0.00
25908	10934	16	0.00	0.00
25909	10935	27	0.00	0.00
25910	10935	32	0.00	0.00
25911	10935	24	0.00	0.00
25912	10935	20	0.00	0.00
25913	10935	16	0.00	0.00
25914	10936	27	0.00	0.00
25915	10936	32	0.00	0.00
25916	10936	24	0.00	0.00
25917	10936	20	0.00	0.00
25918	10936	16	0.00	0.00
25919	10937	27	0.00	0.00
25920	10937	32	0.00	0.00
25921	10937	24	0.00	0.00
25922	10937	20	0.00	0.00
25923	10937	16	0.00	0.00
25924	10938	27	0.00	0.00
25925	10938	32	0.00	0.00
25926	10938	24	0.00	0.00
25927	10938	20	0.00	0.00
25928	10938	16	0.00	0.00
25929	10939	27	0.00	0.00
25930	10939	32	0.00	0.00
25931	10939	24	0.00	0.00
25932	10939	20	0.00	0.00
25933	10939	16	0.00	0.00
25934	10940	27	0.00	0.00
25935	10940	32	0.00	0.00
25936	10940	24	0.00	0.00
25937	10940	20	0.00	0.00
25938	10940	16	0.00	0.00
25939	10941	27	0.00	0.00
25940	10941	32	0.00	0.00
25941	10941	24	0.00	0.00
25942	10941	20	0.00	0.00
25943	10941	16	0.00	0.00
25944	10942	27	0.00	0.00
25945	10942	32	0.00	0.00
25946	10942	24	0.00	0.00
25947	10942	20	0.00	0.00
25948	10942	16	0.00	0.00
25949	10943	27	0.00	0.00
25950	10943	32	0.00	0.00
25951	10943	24	0.00	0.00
25952	10943	20	0.00	0.00
25953	10943	16	0.00	0.00
\.


--
-- Data for Name: acumulado_canal; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY acumulado_canal (idacumulado_canal, idcuenta, idcanal, debe, haber) FROM stdin;
3149	10320	5	0.00	0.00
3165	10308	5	0.00	0.00
3166	10302	5	0.00	0.00
3170	10398	5	0.00	0.00
3172	10414	5	0.00	0.00
3173	10047	5	0.00	0.00
3174	10078	5	0.00	0.00
3175	10003	5	0.00	0.00
3178	10068	5	0.00	0.00
3183	10534	5	0.00	0.00
3184	10057	5	0.00	0.00
3185	10001	5	0.00	0.00
3186	10002	5	0.00	0.00
3187	10004	5	0.00	0.00
3188	10005	5	0.00	0.00
3189	10006	5	0.00	0.00
3190	10007	5	0.00	0.00
3191	10008	5	0.00	0.00
3192	10009	5	0.00	0.00
3193	10010	5	0.00	0.00
3194	10011	5	0.00	0.00
3195	10012	5	0.00	0.00
3196	10013	5	0.00	0.00
3197	10014	5	0.00	0.00
3198	10015	5	0.00	0.00
3199	10016	5	0.00	0.00
3200	10017	5	0.00	0.00
3201	10018	5	0.00	0.00
3202	10019	5	0.00	0.00
3203	10020	5	0.00	0.00
3204	10021	5	0.00	0.00
3205	10022	5	0.00	0.00
3206	10023	5	0.00	0.00
3207	10024	5	0.00	0.00
3208	10025	5	0.00	0.00
3209	10026	5	0.00	0.00
3210	10027	5	0.00	0.00
3211	10028	5	0.00	0.00
3212	10030	5	0.00	0.00
3213	10032	5	0.00	0.00
3214	10033	5	0.00	0.00
3215	10035	5	0.00	0.00
3216	10036	5	0.00	0.00
3217	10037	5	0.00	0.00
3218	10038	5	0.00	0.00
3219	10039	5	0.00	0.00
3220	10040	5	0.00	0.00
3221	10041	5	0.00	0.00
3222	10042	5	0.00	0.00
3223	10043	5	0.00	0.00
3224	10045	5	0.00	0.00
3225	10046	5	0.00	0.00
3226	10048	5	0.00	0.00
3227	10049	5	0.00	0.00
3228	10050	5	0.00	0.00
3229	10051	5	0.00	0.00
3230	10052	5	0.00	0.00
3231	10053	5	0.00	0.00
3232	10054	5	0.00	0.00
3233	10055	5	0.00	0.00
3234	10056	5	0.00	0.00
3235	10058	5	0.00	0.00
3236	10059	5	0.00	0.00
3237	10060	5	0.00	0.00
3238	10061	5	0.00	0.00
3239	10062	5	0.00	0.00
3240	10063	5	0.00	0.00
3241	10064	5	0.00	0.00
3243	10066	5	0.00	0.00
3244	10067	5	0.00	0.00
3245	10069	5	0.00	0.00
3246	10070	5	0.00	0.00
3247	10071	5	0.00	0.00
3248	10072	5	0.00	0.00
3249	10073	5	0.00	0.00
3250	10074	5	0.00	0.00
3251	10075	5	0.00	0.00
3252	10076	5	0.00	0.00
3253	10077	5	0.00	0.00
3254	10079	5	0.00	0.00
3255	10080	5	0.00	0.00
3256	10081	5	0.00	0.00
3257	10082	5	0.00	0.00
3258	10083	5	0.00	0.00
3259	10084	5	0.00	0.00
3260	10085	5	0.00	0.00
3261	10086	5	0.00	0.00
3262	10087	5	0.00	0.00
3263	10088	5	0.00	0.00
3264	10089	5	0.00	0.00
3265	10090	5	0.00	0.00
3266	10091	5	0.00	0.00
3267	10092	5	0.00	0.00
3268	10093	5	0.00	0.00
3269	10094	5	0.00	0.00
3270	10095	5	0.00	0.00
3271	10096	5	0.00	0.00
3272	10097	5	0.00	0.00
3273	10098	5	0.00	0.00
3274	10099	5	0.00	0.00
3275	10100	5	0.00	0.00
3276	10101	5	0.00	0.00
3277	10102	5	0.00	0.00
3278	10103	5	0.00	0.00
3279	10104	5	0.00	0.00
3280	10105	5	0.00	0.00
3281	10106	5	0.00	0.00
3282	10107	5	0.00	0.00
3283	10108	5	0.00	0.00
3284	10109	5	0.00	0.00
3285	10110	5	0.00	0.00
3286	10111	5	0.00	0.00
3287	10112	5	0.00	0.00
3288	10113	5	0.00	0.00
3289	10114	5	0.00	0.00
3290	10115	5	0.00	0.00
3291	10116	5	0.00	0.00
3292	10117	5	0.00	0.00
3293	10118	5	0.00	0.00
3294	10119	5	0.00	0.00
3295	10120	5	0.00	0.00
3296	10121	5	0.00	0.00
3297	10122	5	0.00	0.00
3298	10123	5	0.00	0.00
3299	10124	5	0.00	0.00
3300	10125	5	0.00	0.00
3301	10126	5	0.00	0.00
3302	10127	5	0.00	0.00
3303	10128	5	0.00	0.00
3304	10129	5	0.00	0.00
3305	10130	5	0.00	0.00
3306	10131	5	0.00	0.00
3307	10132	5	0.00	0.00
3308	10133	5	0.00	0.00
3309	10134	5	0.00	0.00
3310	10135	5	0.00	0.00
3311	10136	5	0.00	0.00
3312	10137	5	0.00	0.00
3313	10138	5	0.00	0.00
3314	10139	5	0.00	0.00
3315	10140	5	0.00	0.00
3316	10141	5	0.00	0.00
3317	10142	5	0.00	0.00
3318	10143	5	0.00	0.00
3319	10144	5	0.00	0.00
3320	10145	5	0.00	0.00
3321	10146	5	0.00	0.00
3322	10147	5	0.00	0.00
3323	10148	5	0.00	0.00
3324	10149	5	0.00	0.00
3325	10150	5	0.00	0.00
3326	10151	5	0.00	0.00
3327	10152	5	0.00	0.00
3328	10153	5	0.00	0.00
3329	10154	5	0.00	0.00
3330	10155	5	0.00	0.00
3331	10156	5	0.00	0.00
3332	10157	5	0.00	0.00
3333	10158	5	0.00	0.00
3334	10159	5	0.00	0.00
3335	10160	5	0.00	0.00
3336	10161	5	0.00	0.00
3337	10162	5	0.00	0.00
3338	10163	5	0.00	0.00
3339	10164	5	0.00	0.00
3340	10165	5	0.00	0.00
3341	10166	5	0.00	0.00
3342	10167	5	0.00	0.00
3343	10168	5	0.00	0.00
3344	10169	5	0.00	0.00
3345	10170	5	0.00	0.00
3346	10171	5	0.00	0.00
3347	10172	5	0.00	0.00
3348	10173	5	0.00	0.00
3349	10174	5	0.00	0.00
3350	10175	5	0.00	0.00
3351	10176	5	0.00	0.00
3352	10177	5	0.00	0.00
3353	10178	5	0.00	0.00
3354	10179	5	0.00	0.00
3355	10180	5	0.00	0.00
3356	10181	5	0.00	0.00
3357	10182	5	0.00	0.00
3358	10183	5	0.00	0.00
3359	10184	5	0.00	0.00
3360	10185	5	0.00	0.00
3361	10186	5	0.00	0.00
3362	10187	5	0.00	0.00
3363	10188	5	0.00	0.00
3364	10189	5	0.00	0.00
3365	10190	5	0.00	0.00
3366	10191	5	0.00	0.00
3367	10192	5	0.00	0.00
3368	10193	5	0.00	0.00
3369	10194	5	0.00	0.00
3370	10195	5	0.00	0.00
3371	10196	5	0.00	0.00
3372	10197	5	0.00	0.00
3373	10198	5	0.00	0.00
3374	10199	5	0.00	0.00
3375	10200	5	0.00	0.00
3376	10201	5	0.00	0.00
3377	10202	5	0.00	0.00
3378	10203	5	0.00	0.00
3379	10204	5	0.00	0.00
3380	10205	5	0.00	0.00
3381	10206	5	0.00	0.00
3382	10207	5	0.00	0.00
3383	10208	5	0.00	0.00
3384	10209	5	0.00	0.00
3385	10210	5	0.00	0.00
3386	10211	5	0.00	0.00
3387	10212	5	0.00	0.00
3388	10213	5	0.00	0.00
3389	10214	5	0.00	0.00
3390	10215	5	0.00	0.00
3391	10216	5	0.00	0.00
3392	10217	5	0.00	0.00
3393	10218	5	0.00	0.00
3394	10219	5	0.00	0.00
3395	10220	5	0.00	0.00
3396	10221	5	0.00	0.00
3397	10222	5	0.00	0.00
3398	10223	5	0.00	0.00
3399	10224	5	0.00	0.00
3400	10225	5	0.00	0.00
3401	10226	5	0.00	0.00
3402	10227	5	0.00	0.00
3403	10228	5	0.00	0.00
3404	10229	5	0.00	0.00
3405	10230	5	0.00	0.00
3406	10231	5	0.00	0.00
3407	10232	5	0.00	0.00
3408	10233	5	0.00	0.00
3409	10234	5	0.00	0.00
3410	10235	5	0.00	0.00
3411	10236	5	0.00	0.00
3412	10237	5	0.00	0.00
3413	10238	5	0.00	0.00
3414	10239	5	0.00	0.00
3415	10240	5	0.00	0.00
3416	10241	5	0.00	0.00
3417	10242	5	0.00	0.00
3418	10244	5	0.00	0.00
3419	10245	5	0.00	0.00
3420	10246	5	0.00	0.00
3421	10247	5	0.00	0.00
3422	10248	5	0.00	0.00
3423	10250	5	0.00	0.00
3424	10251	5	0.00	0.00
3426	10253	5	0.00	0.00
3427	10254	5	0.00	0.00
3428	10255	5	0.00	0.00
3429	10256	5	0.00	0.00
3430	10257	5	0.00	0.00
3431	10258	5	0.00	0.00
3432	10259	5	0.00	0.00
3433	10260	5	0.00	0.00
3434	10261	5	0.00	0.00
3435	10262	5	0.00	0.00
3436	10263	5	0.00	0.00
3437	10264	5	0.00	0.00
3438	10265	5	0.00	0.00
3439	10266	5	0.00	0.00
3440	10267	5	0.00	0.00
3441	10268	5	0.00	0.00
3442	10269	5	0.00	0.00
3443	10272	5	0.00	0.00
3444	10273	5	0.00	0.00
3445	10274	5	0.00	0.00
3446	10275	5	0.00	0.00
3447	10276	5	0.00	0.00
3448	10277	5	0.00	0.00
3449	10278	5	0.00	0.00
3450	10279	5	0.00	0.00
3451	10280	5	0.00	0.00
3452	10281	5	0.00	0.00
3453	10282	5	0.00	0.00
3454	10283	5	0.00	0.00
3455	10284	5	0.00	0.00
3456	10285	5	0.00	0.00
3457	10286	5	0.00	0.00
3458	10287	5	0.00	0.00
3459	10288	5	0.00	0.00
3460	10289	5	0.00	0.00
3461	10290	5	0.00	0.00
3462	10291	5	0.00	0.00
3463	10292	5	0.00	0.00
3464	10293	5	0.00	0.00
3465	10294	5	0.00	0.00
3466	10295	5	0.00	0.00
3467	10296	5	0.00	0.00
3468	10297	5	0.00	0.00
3469	10299	5	0.00	0.00
3470	10300	5	0.00	0.00
3471	10301	5	0.00	0.00
3472	10303	5	0.00	0.00
3473	10304	5	0.00	0.00
3474	10305	5	0.00	0.00
3475	10306	5	0.00	0.00
3476	10307	5	0.00	0.00
3477	10309	5	0.00	0.00
3478	10310	5	0.00	0.00
3479	10311	5	0.00	0.00
3480	10312	5	0.00	0.00
3481	10313	5	0.00	0.00
3482	10314	5	0.00	0.00
3483	10315	5	0.00	0.00
3484	10316	5	0.00	0.00
3485	10317	5	0.00	0.00
3486	10319	5	0.00	0.00
3487	10321	5	0.00	0.00
3488	10322	5	0.00	0.00
3489	10323	5	0.00	0.00
3490	10324	5	0.00	0.00
3491	10325	5	0.00	0.00
3492	10326	5	0.00	0.00
3493	10327	5	0.00	0.00
3494	10328	5	0.00	0.00
3495	10329	5	0.00	0.00
3496	10330	5	0.00	0.00
3497	10331	5	0.00	0.00
3498	10332	5	0.00	0.00
3499	10333	5	0.00	0.00
3500	10334	5	0.00	0.00
3501	10335	5	0.00	0.00
3502	10336	5	0.00	0.00
3503	10337	5	0.00	0.00
3504	10338	5	0.00	0.00
3505	10339	5	0.00	0.00
3506	10340	5	0.00	0.00
3507	10341	5	0.00	0.00
3508	10342	5	0.00	0.00
3509	10343	5	0.00	0.00
3510	10344	5	0.00	0.00
3511	10345	5	0.00	0.00
3512	10346	5	0.00	0.00
3513	10347	5	0.00	0.00
3514	10348	5	0.00	0.00
3515	10349	5	0.00	0.00
3516	10350	5	0.00	0.00
3517	10351	5	0.00	0.00
3518	10352	5	0.00	0.00
3519	10354	5	0.00	0.00
3520	10353	5	0.00	0.00
3521	10355	5	0.00	0.00
3522	10356	5	0.00	0.00
3523	10357	5	0.00	0.00
3524	10358	5	0.00	0.00
3525	10359	5	0.00	0.00
3526	10360	5	0.00	0.00
3527	10361	5	0.00	0.00
3528	10362	5	0.00	0.00
3529	10363	5	0.00	0.00
3530	10364	5	0.00	0.00
3531	10365	5	0.00	0.00
3532	10366	5	0.00	0.00
3533	10367	5	0.00	0.00
3534	10368	5	0.00	0.00
3535	10369	5	0.00	0.00
3536	10370	5	0.00	0.00
3537	10371	5	0.00	0.00
3538	10372	5	0.00	0.00
3539	10373	5	0.00	0.00
3540	10374	5	0.00	0.00
3541	10375	5	0.00	0.00
3542	10376	5	0.00	0.00
3543	10377	5	0.00	0.00
3544	10378	5	0.00	0.00
3545	10379	5	0.00	0.00
3546	10380	5	0.00	0.00
3547	10381	5	0.00	0.00
3548	10382	5	0.00	0.00
3549	10383	5	0.00	0.00
3550	10384	5	0.00	0.00
3551	10385	5	0.00	0.00
3552	10386	5	0.00	0.00
3553	10387	5	0.00	0.00
3554	10388	5	0.00	0.00
3555	10389	5	0.00	0.00
3556	10390	5	0.00	0.00
3557	10391	5	0.00	0.00
3558	10392	5	0.00	0.00
3559	10393	5	0.00	0.00
3560	10394	5	0.00	0.00
3561	10395	5	0.00	0.00
3562	10397	5	0.00	0.00
3563	10399	5	0.00	0.00
3564	10400	5	0.00	0.00
3565	10401	5	0.00	0.00
3566	10402	5	0.00	0.00
3567	10403	5	0.00	0.00
3568	10404	5	0.00	0.00
3569	10405	5	0.00	0.00
3570	10406	5	0.00	0.00
3571	10407	5	0.00	0.00
3572	10408	5	0.00	0.00
3573	10409	5	0.00	0.00
3574	10410	5	0.00	0.00
3575	10411	5	0.00	0.00
3576	10412	5	0.00	0.00
3577	10413	5	0.00	0.00
3578	10415	5	0.00	0.00
3579	10416	5	0.00	0.00
3580	10417	5	0.00	0.00
3581	10418	5	0.00	0.00
3582	10419	5	0.00	0.00
3583	10420	5	0.00	0.00
3584	10421	5	0.00	0.00
3585	10422	5	0.00	0.00
3586	10423	5	0.00	0.00
3587	10424	5	0.00	0.00
3588	10425	5	0.00	0.00
3589	10426	5	0.00	0.00
3590	10427	5	0.00	0.00
3591	10428	5	0.00	0.00
3592	10429	5	0.00	0.00
3593	10430	5	0.00	0.00
3594	10431	5	0.00	0.00
3595	10432	5	0.00	0.00
3596	10433	5	0.00	0.00
3597	10434	5	0.00	0.00
3598	10435	5	0.00	0.00
3599	10436	5	0.00	0.00
3600	10437	5	0.00	0.00
3601	10438	5	0.00	0.00
3602	10439	5	0.00	0.00
3603	10440	5	0.00	0.00
3604	10441	5	0.00	0.00
3605	10442	5	0.00	0.00
3606	10443	5	0.00	0.00
3607	10444	5	0.00	0.00
3608	10445	5	0.00	0.00
3609	10446	5	0.00	0.00
3610	10447	5	0.00	0.00
3611	10448	5	0.00	0.00
3612	10449	5	0.00	0.00
3613	10450	5	0.00	0.00
3614	10451	5	0.00	0.00
3615	10452	5	0.00	0.00
3616	10453	5	0.00	0.00
3617	10454	5	0.00	0.00
3618	10455	5	0.00	0.00
3619	10456	5	0.00	0.00
3620	10457	5	0.00	0.00
3621	10458	5	0.00	0.00
3622	10459	5	0.00	0.00
3623	10460	5	0.00	0.00
3624	10461	5	0.00	0.00
3625	10462	5	0.00	0.00
3626	10463	5	0.00	0.00
3627	10464	5	0.00	0.00
3628	10465	5	0.00	0.00
3629	10466	5	0.00	0.00
3630	10467	5	0.00	0.00
3631	10468	5	0.00	0.00
3632	10469	5	0.00	0.00
3633	10470	5	0.00	0.00
3634	10471	5	0.00	0.00
3635	10472	5	0.00	0.00
3636	10473	5	0.00	0.00
3637	10474	5	0.00	0.00
3638	10475	5	0.00	0.00
3639	10476	5	0.00	0.00
3640	10477	5	0.00	0.00
3641	10478	5	0.00	0.00
3642	10479	5	0.00	0.00
3643	10480	5	0.00	0.00
3644	10481	5	0.00	0.00
3645	10482	5	0.00	0.00
3646	10483	5	0.00	0.00
3647	10484	5	0.00	0.00
3648	10485	5	0.00	0.00
3649	10486	5	0.00	0.00
3650	10487	5	0.00	0.00
3651	10488	5	0.00	0.00
3652	10489	5	0.00	0.00
3653	10490	5	0.00	0.00
3654	10491	5	0.00	0.00
3655	10492	5	0.00	0.00
3656	10493	5	0.00	0.00
3657	10494	5	0.00	0.00
3658	10495	5	0.00	0.00
3659	10496	5	0.00	0.00
3660	10497	5	0.00	0.00
3661	10498	5	0.00	0.00
3662	10499	5	0.00	0.00
3663	10500	5	0.00	0.00
3664	10501	5	0.00	0.00
3665	10502	5	0.00	0.00
3666	10503	5	0.00	0.00
3667	10504	5	0.00	0.00
3668	10505	5	0.00	0.00
3669	10506	5	0.00	0.00
3670	10507	5	0.00	0.00
3671	10508	5	0.00	0.00
3672	10509	5	0.00	0.00
3673	10510	5	0.00	0.00
3674	10511	5	0.00	0.00
3675	10512	5	0.00	0.00
3676	10513	5	0.00	0.00
3677	10514	5	0.00	0.00
3678	10515	5	0.00	0.00
3679	10516	5	0.00	0.00
3680	10517	5	0.00	0.00
3681	10518	5	0.00	0.00
3682	10519	5	0.00	0.00
3683	10520	5	0.00	0.00
3684	10521	5	0.00	0.00
3685	10522	5	0.00	0.00
3686	10523	5	0.00	0.00
3687	10524	5	0.00	0.00
3688	10525	5	0.00	0.00
3689	10526	5	0.00	0.00
3690	10527	5	0.00	0.00
3691	10528	5	0.00	0.00
3692	10529	5	0.00	0.00
3693	10530	5	0.00	0.00
3694	10531	5	0.00	0.00
3695	10532	5	0.00	0.00
3696	10533	5	0.00	0.00
3697	10535	5	0.00	0.00
3698	10536	5	0.00	0.00
3699	10537	5	0.00	0.00
3700	10538	5	0.00	0.00
3701	10539	5	0.00	0.00
3702	10540	5	0.00	0.00
3703	10541	5	0.00	0.00
3704	10542	5	0.00	0.00
3705	10543	5	0.00	0.00
3706	10544	5	0.00	0.00
3707	10545	5	0.00	0.00
3708	10546	5	0.00	0.00
3709	10547	5	0.00	0.00
3710	10548	5	0.00	0.00
3711	10549	5	0.00	0.00
3712	10550	5	0.00	0.00
3713	10551	5	0.00	0.00
3714	10552	5	0.00	0.00
3715	10553	5	0.00	0.00
3716	10554	5	0.00	0.00
3717	10555	5	0.00	0.00
3718	10556	5	0.00	0.00
3719	10557	5	0.00	0.00
3720	10558	5	0.00	0.00
3721	10559	5	0.00	0.00
3722	10560	5	0.00	0.00
3723	10561	5	0.00	0.00
3724	10562	5	0.00	0.00
3725	10563	5	0.00	0.00
3726	10564	5	0.00	0.00
3727	10565	5	0.00	0.00
3728	10566	5	0.00	0.00
3729	10567	5	0.00	0.00
3730	10568	5	0.00	0.00
3731	10569	5	0.00	0.00
3732	10570	5	0.00	0.00
3733	10571	5	0.00	0.00
3734	10572	5	0.00	0.00
3735	10573	5	0.00	0.00
3736	10574	5	0.00	0.00
3737	10575	5	0.00	0.00
3738	10576	5	0.00	0.00
3739	10577	5	0.00	0.00
3740	10578	5	0.00	0.00
3741	10579	5	0.00	0.00
3742	10580	5	0.00	0.00
3743	10581	5	0.00	0.00
3744	10582	5	0.00	0.00
3745	10583	5	0.00	0.00
3746	10584	5	0.00	0.00
3747	10585	5	0.00	0.00
3748	10586	5	0.00	0.00
3749	10587	5	0.00	0.00
3750	10588	5	0.00	0.00
3751	10589	5	0.00	0.00
3752	10590	5	0.00	0.00
3753	10591	5	0.00	0.00
3754	10592	5	0.00	0.00
3755	10593	5	0.00	0.00
3756	10594	5	0.00	0.00
3757	10595	5	0.00	0.00
3758	10596	5	0.00	0.00
3759	10597	5	0.00	0.00
3760	10598	5	0.00	0.00
3761	10599	5	0.00	0.00
3762	10600	5	0.00	0.00
3763	10601	5	0.00	0.00
3764	10602	5	0.00	0.00
3765	10603	5	0.00	0.00
3766	10604	5	0.00	0.00
3767	10605	5	0.00	0.00
3768	10606	5	0.00	0.00
3769	10607	5	0.00	0.00
3770	10608	5	0.00	0.00
3771	10609	5	0.00	0.00
3772	10610	5	0.00	0.00
3773	10611	5	0.00	0.00
3774	10612	5	0.00	0.00
3775	10613	5	0.00	0.00
3776	10614	5	0.00	0.00
3777	10615	5	0.00	0.00
3778	10616	5	0.00	0.00
3779	10617	5	0.00	0.00
3780	10618	5	0.00	0.00
3781	10619	5	0.00	0.00
3782	10620	5	0.00	0.00
3783	10621	5	0.00	0.00
3784	10622	5	0.00	0.00
3785	10623	5	0.00	0.00
3786	10624	5	0.00	0.00
3787	10625	5	0.00	0.00
3788	10626	5	0.00	0.00
3789	10627	5	0.00	0.00
3790	10628	5	0.00	0.00
3791	10629	5	0.00	0.00
3792	10630	5	0.00	0.00
3793	10631	5	0.00	0.00
788	10320	2	0.00	0.00
790	10001	2	0.00	0.00
791	10002	2	0.00	0.00
792	10003	2	0.00	0.00
793	10004	2	0.00	0.00
794	10005	2	0.00	0.00
795	10006	2	0.00	0.00
796	10007	2	0.00	0.00
797	10008	2	0.00	0.00
798	10009	2	0.00	0.00
799	10010	2	0.00	0.00
800	10011	2	0.00	0.00
801	10012	2	0.00	0.00
802	10013	2	0.00	0.00
803	10014	2	0.00	0.00
804	10015	2	0.00	0.00
805	10016	2	0.00	0.00
806	10017	2	0.00	0.00
807	10018	2	0.00	0.00
808	10019	2	0.00	0.00
809	10020	2	0.00	0.00
810	10021	2	0.00	0.00
811	10022	2	0.00	0.00
812	10023	2	0.00	0.00
813	10024	2	0.00	0.00
814	10025	2	0.00	0.00
815	10026	2	0.00	0.00
816	10027	2	0.00	0.00
817	10028	2	0.00	0.00
818	10030	2	0.00	0.00
819	10032	2	0.00	0.00
820	10033	2	0.00	0.00
821	10035	2	0.00	0.00
822	10036	2	0.00	0.00
823	10037	2	0.00	0.00
824	10038	2	0.00	0.00
825	10039	2	0.00	0.00
826	10040	2	0.00	0.00
827	10041	2	0.00	0.00
828	10042	2	0.00	0.00
829	10043	2	0.00	0.00
830	10045	2	0.00	0.00
831	10046	2	0.00	0.00
832	10048	2	0.00	0.00
833	10049	2	0.00	0.00
834	10050	2	0.00	0.00
835	10051	2	0.00	0.00
836	10052	2	0.00	0.00
837	10053	2	0.00	0.00
838	10054	2	0.00	0.00
839	10055	2	0.00	0.00
840	10056	2	0.00	0.00
841	10058	2	0.00	0.00
842	10059	2	0.00	0.00
843	10060	2	0.00	0.00
844	10061	2	0.00	0.00
845	10062	2	0.00	0.00
846	10063	2	0.00	0.00
847	10064	2	0.00	0.00
849	10066	2	0.00	0.00
850	10067	2	0.00	0.00
851	10068	2	0.00	0.00
852	10069	2	0.00	0.00
853	10070	2	0.00	0.00
854	10071	2	0.00	0.00
855	10072	2	0.00	0.00
856	10073	2	0.00	0.00
857	10074	2	0.00	0.00
858	10075	2	0.00	0.00
859	10076	2	0.00	0.00
860	10077	2	0.00	0.00
861	10078	2	0.00	0.00
862	10079	2	0.00	0.00
863	10080	2	0.00	0.00
864	10081	2	0.00	0.00
865	10082	2	0.00	0.00
866	10083	2	0.00	0.00
867	10084	2	0.00	0.00
868	10085	2	0.00	0.00
869	10086	2	0.00	0.00
870	10087	2	0.00	0.00
871	10088	2	0.00	0.00
872	10089	2	0.00	0.00
873	10090	2	0.00	0.00
874	10091	2	0.00	0.00
875	10092	2	0.00	0.00
876	10093	2	0.00	0.00
877	10094	2	0.00	0.00
878	10095	2	0.00	0.00
879	10096	2	0.00	0.00
880	10097	2	0.00	0.00
881	10098	2	0.00	0.00
882	10099	2	0.00	0.00
883	10100	2	0.00	0.00
884	10101	2	0.00	0.00
885	10102	2	0.00	0.00
886	10103	2	0.00	0.00
887	10104	2	0.00	0.00
888	10105	2	0.00	0.00
889	10106	2	0.00	0.00
890	10107	2	0.00	0.00
891	10108	2	0.00	0.00
892	10109	2	0.00	0.00
893	10110	2	0.00	0.00
894	10111	2	0.00	0.00
895	10112	2	0.00	0.00
896	10113	2	0.00	0.00
897	10114	2	0.00	0.00
898	10115	2	0.00	0.00
899	10116	2	0.00	0.00
900	10117	2	0.00	0.00
901	10118	2	0.00	0.00
902	10119	2	0.00	0.00
903	10120	2	0.00	0.00
904	10121	2	0.00	0.00
905	10122	2	0.00	0.00
906	10123	2	0.00	0.00
907	10124	2	0.00	0.00
908	10125	2	0.00	0.00
909	10126	2	0.00	0.00
910	10127	2	0.00	0.00
911	10128	2	0.00	0.00
912	10129	2	0.00	0.00
913	10130	2	0.00	0.00
914	10131	2	0.00	0.00
915	10132	2	0.00	0.00
916	10133	2	0.00	0.00
917	10134	2	0.00	0.00
918	10135	2	0.00	0.00
919	10136	2	0.00	0.00
920	10137	2	0.00	0.00
921	10138	2	0.00	0.00
922	10139	2	0.00	0.00
923	10140	2	0.00	0.00
924	10141	2	0.00	0.00
925	10142	2	0.00	0.00
926	10143	2	0.00	0.00
927	10144	2	0.00	0.00
928	10145	2	0.00	0.00
929	10146	2	0.00	0.00
930	10147	2	0.00	0.00
931	10148	2	0.00	0.00
932	10149	2	0.00	0.00
933	10150	2	0.00	0.00
934	10151	2	0.00	0.00
935	10152	2	0.00	0.00
936	10153	2	0.00	0.00
937	10154	2	0.00	0.00
938	10155	2	0.00	0.00
939	10156	2	0.00	0.00
940	10157	2	0.00	0.00
941	10158	2	0.00	0.00
942	10159	2	0.00	0.00
943	10160	2	0.00	0.00
944	10161	2	0.00	0.00
945	10162	2	0.00	0.00
946	10163	2	0.00	0.00
947	10164	2	0.00	0.00
948	10165	2	0.00	0.00
949	10166	2	0.00	0.00
950	10167	2	0.00	0.00
951	10168	2	0.00	0.00
952	10169	2	0.00	0.00
953	10170	2	0.00	0.00
954	10171	2	0.00	0.00
955	10172	2	0.00	0.00
956	10173	2	0.00	0.00
957	10174	2	0.00	0.00
958	10175	2	0.00	0.00
959	10176	2	0.00	0.00
960	10177	2	0.00	0.00
961	10178	2	0.00	0.00
962	10179	2	0.00	0.00
963	10180	2	0.00	0.00
964	10181	2	0.00	0.00
965	10182	2	0.00	0.00
966	10183	2	0.00	0.00
967	10184	2	0.00	0.00
968	10185	2	0.00	0.00
969	10186	2	0.00	0.00
970	10187	2	0.00	0.00
971	10188	2	0.00	0.00
972	10189	2	0.00	0.00
973	10190	2	0.00	0.00
974	10191	2	0.00	0.00
975	10192	2	0.00	0.00
976	10193	2	0.00	0.00
977	10194	2	0.00	0.00
978	10195	2	0.00	0.00
979	10196	2	0.00	0.00
980	10197	2	0.00	0.00
981	10198	2	0.00	0.00
982	10199	2	0.00	0.00
983	10200	2	0.00	0.00
984	10201	2	0.00	0.00
985	10202	2	0.00	0.00
986	10203	2	0.00	0.00
987	10204	2	0.00	0.00
988	10205	2	0.00	0.00
989	10206	2	0.00	0.00
990	10207	2	0.00	0.00
991	10208	2	0.00	0.00
992	10209	2	0.00	0.00
993	10210	2	0.00	0.00
994	10211	2	0.00	0.00
995	10212	2	0.00	0.00
996	10213	2	0.00	0.00
997	10214	2	0.00	0.00
998	10215	2	0.00	0.00
999	10216	2	0.00	0.00
1000	10217	2	0.00	0.00
1001	10218	2	0.00	0.00
1002	10219	2	0.00	0.00
1003	10220	2	0.00	0.00
1004	10221	2	0.00	0.00
1005	10222	2	0.00	0.00
1006	10223	2	0.00	0.00
1007	10224	2	0.00	0.00
1008	10225	2	0.00	0.00
1009	10226	2	0.00	0.00
1010	10227	2	0.00	0.00
1011	10228	2	0.00	0.00
1012	10229	2	0.00	0.00
1013	10230	2	0.00	0.00
1014	10231	2	0.00	0.00
1015	10232	2	0.00	0.00
1016	10233	2	0.00	0.00
1017	10234	2	0.00	0.00
1018	10235	2	0.00	0.00
1019	10236	2	0.00	0.00
1020	10237	2	0.00	0.00
1021	10238	2	0.00	0.00
1022	10239	2	0.00	0.00
1023	10240	2	0.00	0.00
1024	10241	2	0.00	0.00
1025	10242	2	0.00	0.00
1026	10244	2	0.00	0.00
1027	10245	2	0.00	0.00
1028	10246	2	0.00	0.00
1029	10247	2	0.00	0.00
1030	10248	2	0.00	0.00
1031	10250	2	0.00	0.00
1032	10251	2	0.00	0.00
1034	10253	2	0.00	0.00
1035	10254	2	0.00	0.00
1036	10255	2	0.00	0.00
1037	10256	2	0.00	0.00
1038	10257	2	0.00	0.00
1039	10258	2	0.00	0.00
1040	10259	2	0.00	0.00
1041	10260	2	0.00	0.00
1042	10261	2	0.00	0.00
1043	10262	2	0.00	0.00
1044	10263	2	0.00	0.00
1045	10264	2	0.00	0.00
1046	10265	2	0.00	0.00
1047	10266	2	0.00	0.00
1048	10267	2	0.00	0.00
1049	10268	2	0.00	0.00
1050	10269	2	0.00	0.00
1051	10272	2	0.00	0.00
1052	10273	2	0.00	0.00
1053	10274	2	0.00	0.00
1054	10275	2	0.00	0.00
1055	10276	2	0.00	0.00
1056	10277	2	0.00	0.00
1057	10278	2	0.00	0.00
1058	10279	2	0.00	0.00
1059	10280	2	0.00	0.00
1060	10281	2	0.00	0.00
1061	10282	2	0.00	0.00
1062	10283	2	0.00	0.00
1063	10284	2	0.00	0.00
1064	10285	2	0.00	0.00
1065	10286	2	0.00	0.00
1066	10287	2	0.00	0.00
1067	10288	2	0.00	0.00
1068	10289	2	0.00	0.00
1069	10290	2	0.00	0.00
1070	10291	2	0.00	0.00
1071	10292	2	0.00	0.00
1072	10293	2	0.00	0.00
1073	10294	2	0.00	0.00
1074	10295	2	0.00	0.00
1075	10296	2	0.00	0.00
1076	10297	2	0.00	0.00
1077	10299	2	0.00	0.00
1078	10300	2	0.00	0.00
1079	10301	2	0.00	0.00
1080	10303	2	0.00	0.00
1081	10304	2	0.00	0.00
1082	10305	2	0.00	0.00
1083	10306	2	0.00	0.00
1084	10307	2	0.00	0.00
1085	10309	2	0.00	0.00
1086	10310	2	0.00	0.00
1087	10311	2	0.00	0.00
1088	10312	2	0.00	0.00
1089	10313	2	0.00	0.00
1090	10314	2	0.00	0.00
1091	10315	2	0.00	0.00
1092	10316	2	0.00	0.00
1093	10317	2	0.00	0.00
1094	10319	2	0.00	0.00
1095	10321	2	0.00	0.00
1096	10322	2	0.00	0.00
1097	10323	2	0.00	0.00
1098	10324	2	0.00	0.00
1099	10325	2	0.00	0.00
1100	10326	2	0.00	0.00
1101	10327	2	0.00	0.00
1102	10328	2	0.00	0.00
1103	10329	2	0.00	0.00
1104	10330	2	0.00	0.00
1105	10331	2	0.00	0.00
1106	10332	2	0.00	0.00
1107	10333	2	0.00	0.00
1108	10334	2	0.00	0.00
1109	10335	2	0.00	0.00
1110	10336	2	0.00	0.00
1111	10337	2	0.00	0.00
1112	10338	2	0.00	0.00
1113	10339	2	0.00	0.00
1114	10340	2	0.00	0.00
1115	10341	2	0.00	0.00
1116	10342	2	0.00	0.00
1117	10343	2	0.00	0.00
1118	10344	2	0.00	0.00
1119	10345	2	0.00	0.00
1120	10346	2	0.00	0.00
1121	10347	2	0.00	0.00
1122	10348	2	0.00	0.00
1123	10349	2	0.00	0.00
1124	10350	2	0.00	0.00
1125	10351	2	0.00	0.00
1126	10352	2	0.00	0.00
1127	10354	2	0.00	0.00
1128	10353	2	0.00	0.00
1129	10355	2	0.00	0.00
1130	10356	2	0.00	0.00
1131	10357	2	0.00	0.00
1132	10358	2	0.00	0.00
1133	10359	2	0.00	0.00
1134	10360	2	0.00	0.00
1135	10361	2	0.00	0.00
1136	10362	2	0.00	0.00
1137	10363	2	0.00	0.00
1138	10364	2	0.00	0.00
1139	10365	2	0.00	0.00
1140	10366	2	0.00	0.00
1141	10367	2	0.00	0.00
1142	10368	2	0.00	0.00
1143	10369	2	0.00	0.00
1144	10370	2	0.00	0.00
1145	10371	2	0.00	0.00
1146	10372	2	0.00	0.00
1147	10373	2	0.00	0.00
1148	10374	2	0.00	0.00
1149	10375	2	0.00	0.00
1150	10376	2	0.00	0.00
1151	10377	2	0.00	0.00
1152	10378	2	0.00	0.00
1153	10379	2	0.00	0.00
1154	10380	2	0.00	0.00
1155	10381	2	0.00	0.00
1156	10382	2	0.00	0.00
1157	10383	2	0.00	0.00
1158	10384	2	0.00	0.00
1159	10385	2	0.00	0.00
1160	10386	2	0.00	0.00
1161	10387	2	0.00	0.00
1162	10388	2	0.00	0.00
1163	10389	2	0.00	0.00
1164	10390	2	0.00	0.00
1165	10391	2	0.00	0.00
1166	10392	2	0.00	0.00
1167	10393	2	0.00	0.00
1168	10394	2	0.00	0.00
1169	10395	2	0.00	0.00
1170	10397	2	0.00	0.00
1171	10399	2	0.00	0.00
1172	10400	2	0.00	0.00
1173	10401	2	0.00	0.00
1174	10402	2	0.00	0.00
1175	10403	2	0.00	0.00
1176	10404	2	0.00	0.00
1177	10405	2	0.00	0.00
1178	10406	2	0.00	0.00
1179	10407	2	0.00	0.00
1180	10408	2	0.00	0.00
1181	10409	2	0.00	0.00
1182	10410	2	0.00	0.00
1183	10411	2	0.00	0.00
1184	10412	2	0.00	0.00
1185	10413	2	0.00	0.00
1186	10415	2	0.00	0.00
1187	10416	2	0.00	0.00
1188	10417	2	0.00	0.00
1189	10418	2	0.00	0.00
1190	10419	2	0.00	0.00
1191	10420	2	0.00	0.00
1192	10421	2	0.00	0.00
1193	10422	2	0.00	0.00
1194	10423	2	0.00	0.00
1195	10424	2	0.00	0.00
1196	10425	2	0.00	0.00
1197	10426	2	0.00	0.00
1198	10427	2	0.00	0.00
1199	10428	2	0.00	0.00
1200	10429	2	0.00	0.00
1201	10430	2	0.00	0.00
1202	10431	2	0.00	0.00
1203	10432	2	0.00	0.00
1204	10433	2	0.00	0.00
1205	10434	2	0.00	0.00
1206	10435	2	0.00	0.00
1207	10436	2	0.00	0.00
1208	10437	2	0.00	0.00
1209	10438	2	0.00	0.00
1210	10439	2	0.00	0.00
1211	10440	2	0.00	0.00
1212	10441	2	0.00	0.00
1213	10442	2	0.00	0.00
1214	10443	2	0.00	0.00
1215	10444	2	0.00	0.00
1216	10445	2	0.00	0.00
1217	10446	2	0.00	0.00
1218	10447	2	0.00	0.00
1219	10448	2	0.00	0.00
1220	10449	2	0.00	0.00
1221	10450	2	0.00	0.00
1222	10451	2	0.00	0.00
1223	10452	2	0.00	0.00
1224	10453	2	0.00	0.00
1225	10454	2	0.00	0.00
1226	10455	2	0.00	0.00
1227	10456	2	0.00	0.00
1228	10457	2	0.00	0.00
1229	10458	2	0.00	0.00
1230	10459	2	0.00	0.00
1231	10460	2	0.00	0.00
1232	10461	2	0.00	0.00
1233	10462	2	0.00	0.00
1234	10463	2	0.00	0.00
1235	10464	2	0.00	0.00
1236	10465	2	0.00	0.00
1237	10466	2	0.00	0.00
1238	10467	2	0.00	0.00
1239	10468	2	0.00	0.00
1240	10469	2	0.00	0.00
1241	10470	2	0.00	0.00
1242	10471	2	0.00	0.00
1243	10472	2	0.00	0.00
1244	10473	2	0.00	0.00
1245	10474	2	0.00	0.00
1246	10475	2	0.00	0.00
1247	10476	2	0.00	0.00
1248	10477	2	0.00	0.00
1249	10478	2	0.00	0.00
1250	10479	2	0.00	0.00
1251	10480	2	0.00	0.00
1252	10481	2	0.00	0.00
1253	10482	2	0.00	0.00
1254	10483	2	0.00	0.00
1255	10484	2	0.00	0.00
1256	10485	2	0.00	0.00
1257	10486	2	0.00	0.00
1258	10487	2	0.00	0.00
1259	10488	2	0.00	0.00
1260	10489	2	0.00	0.00
1261	10490	2	0.00	0.00
1262	10491	2	0.00	0.00
1263	10492	2	0.00	0.00
1264	10493	2	0.00	0.00
1265	10494	2	0.00	0.00
1266	10495	2	0.00	0.00
1267	10496	2	0.00	0.00
1268	10497	2	0.00	0.00
1269	10498	2	0.00	0.00
1270	10499	2	0.00	0.00
1271	10500	2	0.00	0.00
1272	10501	2	0.00	0.00
1273	10502	2	0.00	0.00
1274	10503	2	0.00	0.00
1275	10504	2	0.00	0.00
1276	10505	2	0.00	0.00
1277	10506	2	0.00	0.00
1278	10507	2	0.00	0.00
1279	10508	2	0.00	0.00
1280	10509	2	0.00	0.00
1281	10510	2	0.00	0.00
1282	10511	2	0.00	0.00
1283	10512	2	0.00	0.00
1284	10513	2	0.00	0.00
1285	10514	2	0.00	0.00
1286	10515	2	0.00	0.00
1287	10516	2	0.00	0.00
1288	10517	2	0.00	0.00
1289	10518	2	0.00	0.00
1290	10519	2	0.00	0.00
1291	10520	2	0.00	0.00
1292	10521	2	0.00	0.00
1293	10522	2	0.00	0.00
1294	10523	2	0.00	0.00
1295	10524	2	0.00	0.00
1296	10525	2	0.00	0.00
1297	10526	2	0.00	0.00
1298	10527	2	0.00	0.00
1299	10528	2	0.00	0.00
1300	10529	2	0.00	0.00
1301	10530	2	0.00	0.00
1302	10531	2	0.00	0.00
1303	10532	2	0.00	0.00
1304	10533	2	0.00	0.00
1305	10535	2	0.00	0.00
1306	10536	2	0.00	0.00
1307	10537	2	0.00	0.00
1308	10538	2	0.00	0.00
1309	10539	2	0.00	0.00
1310	10540	2	0.00	0.00
1311	10541	2	0.00	0.00
1312	10542	2	0.00	0.00
1313	10543	2	0.00	0.00
1314	10544	2	0.00	0.00
1315	10545	2	0.00	0.00
1316	10546	2	0.00	0.00
1317	10547	2	0.00	0.00
1318	10548	2	0.00	0.00
1319	10549	2	0.00	0.00
1320	10550	2	0.00	0.00
1321	10551	2	0.00	0.00
1322	10552	2	0.00	0.00
1323	10553	2	0.00	0.00
1324	10554	2	0.00	0.00
1325	10555	2	0.00	0.00
1326	10556	2	0.00	0.00
1327	10557	2	0.00	0.00
1328	10558	2	0.00	0.00
1329	10559	2	0.00	0.00
1330	10560	2	0.00	0.00
1331	10561	2	0.00	0.00
1332	10562	2	0.00	0.00
1333	10563	2	0.00	0.00
1334	10564	2	0.00	0.00
1335	10565	2	0.00	0.00
1336	10566	2	0.00	0.00
1337	10567	2	0.00	0.00
1338	10568	2	0.00	0.00
1339	10569	2	0.00	0.00
1340	10570	2	0.00	0.00
1341	10571	2	0.00	0.00
1342	10572	2	0.00	0.00
1343	10573	2	0.00	0.00
1344	10574	2	0.00	0.00
1345	10575	2	0.00	0.00
1346	10576	2	0.00	0.00
1347	10577	2	0.00	0.00
1348	10578	2	0.00	0.00
1349	10579	2	0.00	0.00
1350	10580	2	0.00	0.00
1351	10581	2	0.00	0.00
1352	10582	2	0.00	0.00
1353	10583	2	0.00	0.00
1354	10584	2	0.00	0.00
1355	10585	2	0.00	0.00
1356	10586	2	0.00	0.00
1357	10587	2	0.00	0.00
1358	10588	2	0.00	0.00
1359	10589	2	0.00	0.00
1360	10590	2	0.00	0.00
1361	10591	2	0.00	0.00
1362	10592	2	0.00	0.00
1363	10593	2	0.00	0.00
1364	10594	2	0.00	0.00
1365	10595	2	0.00	0.00
1366	10596	2	0.00	0.00
1367	10597	2	0.00	0.00
1368	10598	2	0.00	0.00
1369	10599	2	0.00	0.00
1370	10600	2	0.00	0.00
1371	10601	2	0.00	0.00
1372	10602	2	0.00	0.00
1373	10603	2	0.00	0.00
1374	10604	2	0.00	0.00
1375	10605	2	0.00	0.00
1376	10606	2	0.00	0.00
1377	10607	2	0.00	0.00
1378	10608	2	0.00	0.00
1379	10609	2	0.00	0.00
1380	10610	2	0.00	0.00
1381	10611	2	0.00	0.00
1382	10612	2	0.00	0.00
1383	10613	2	0.00	0.00
1384	10614	2	0.00	0.00
1385	10615	2	0.00	0.00
1386	10616	2	0.00	0.00
1387	10617	2	0.00	0.00
1388	10618	2	0.00	0.00
1389	10619	2	0.00	0.00
1390	10620	2	0.00	0.00
1391	10621	2	0.00	0.00
1392	10622	2	0.00	0.00
1393	10623	2	0.00	0.00
1394	10624	2	0.00	0.00
1395	10625	2	0.00	0.00
1396	10626	2	0.00	0.00
1397	10627	2	0.00	0.00
1398	10628	2	0.00	0.00
1399	10629	2	0.00	0.00
1400	10630	2	0.00	0.00
1401	10631	2	0.00	0.00
1534	10308	2	0.00	0.00
1535	10302	2	0.00	0.00
1536	10414	2	0.00	0.00
1537	10047	2	0.00	0.00
1558	10396	2	0.00	0.00
1569	10249	2	0.00	0.00
1570	10243	2	0.00	0.00
1571	10029	2	0.00	0.00
1573	10398	2	0.00	0.00
1574	10044	2	0.00	0.00
3936	10320	6	0.00	0.00
3938	10318	6	0.00	0.00
3940	10298	6	0.00	0.00
3949	10249	6	0.00	0.00
3950	10243	6	0.00	0.00
3951	10029	6	0.00	0.00
3952	10308	6	0.00	0.00
3953	10302	6	0.00	0.00
3955	10396	6	0.00	0.00
3957	10398	6	0.00	0.00
3958	10044	6	0.00	0.00
3959	10414	6	0.00	0.00
3960	10047	6	0.00	0.00
3961	10078	6	0.00	0.00
3962	10003	6	0.00	0.00
3965	10068	6	0.00	0.00
3966	10034	6	0.00	0.00
3967	10271	6	0.00	0.00
3968	10270	6	0.00	0.00
3969	10031	6	0.00	0.00
3970	10534	6	0.00	0.00
3971	10057	6	0.00	0.00
3972	10001	6	0.00	0.00
3973	10002	6	0.00	0.00
3974	10004	6	0.00	0.00
3975	10005	6	0.00	0.00
3976	10006	6	0.00	0.00
3977	10007	6	0.00	0.00
3978	10008	6	0.00	0.00
3979	10009	6	0.00	0.00
3980	10010	6	0.00	0.00
3981	10011	6	0.00	0.00
3982	10012	6	0.00	0.00
3983	10013	6	0.00	0.00
3984	10014	6	0.00	0.00
3985	10015	6	0.00	0.00
3986	10016	6	0.00	0.00
3987	10017	6	0.00	0.00
3988	10018	6	0.00	0.00
3989	10019	6	0.00	0.00
3990	10020	6	0.00	0.00
3991	10021	6	0.00	0.00
3992	10022	6	0.00	0.00
3993	10023	6	0.00	0.00
3994	10024	6	0.00	0.00
3995	10025	6	0.00	0.00
3996	10026	6	0.00	0.00
3997	10027	6	0.00	0.00
3998	10028	6	0.00	0.00
3999	10030	6	0.00	0.00
4000	10032	6	0.00	0.00
4001	10033	6	0.00	0.00
4002	10035	6	0.00	0.00
4003	10036	6	0.00	0.00
4004	10037	6	0.00	0.00
4005	10038	6	0.00	0.00
4006	10039	6	0.00	0.00
4007	10040	6	0.00	0.00
4008	10041	6	0.00	0.00
4009	10042	6	0.00	0.00
4010	10043	6	0.00	0.00
4011	10045	6	0.00	0.00
4012	10046	6	0.00	0.00
4013	10048	6	0.00	0.00
4014	10049	6	0.00	0.00
4015	10050	6	0.00	0.00
4016	10051	6	0.00	0.00
4017	10052	6	0.00	0.00
4018	10053	6	0.00	0.00
4019	10054	6	0.00	0.00
4020	10055	6	0.00	0.00
4021	10056	6	0.00	0.00
4022	10058	6	0.00	0.00
4023	10059	6	0.00	0.00
4024	10060	6	0.00	0.00
4025	10061	6	0.00	0.00
4026	10062	6	0.00	0.00
4027	10063	6	0.00	0.00
4028	10064	6	0.00	0.00
4030	10066	6	0.00	0.00
4031	10067	6	0.00	0.00
4032	10069	6	0.00	0.00
4033	10070	6	0.00	0.00
4034	10071	6	0.00	0.00
4035	10072	6	0.00	0.00
4036	10073	6	0.00	0.00
4037	10074	6	0.00	0.00
4038	10075	6	0.00	0.00
4039	10076	6	0.00	0.00
4040	10077	6	0.00	0.00
4041	10079	6	0.00	0.00
4042	10080	6	0.00	0.00
4043	10081	6	0.00	0.00
4044	10082	6	0.00	0.00
4045	10083	6	0.00	0.00
4046	10084	6	0.00	0.00
4047	10085	6	0.00	0.00
4048	10086	6	0.00	0.00
4049	10087	6	0.00	0.00
4050	10088	6	0.00	0.00
4051	10089	6	0.00	0.00
4052	10090	6	0.00	0.00
4053	10091	6	0.00	0.00
4054	10092	6	0.00	0.00
4055	10093	6	0.00	0.00
4056	10094	6	0.00	0.00
4057	10095	6	0.00	0.00
4058	10096	6	0.00	0.00
4059	10097	6	0.00	0.00
4060	10098	6	0.00	0.00
4061	10099	6	0.00	0.00
4062	10100	6	0.00	0.00
4063	10101	6	0.00	0.00
4064	10102	6	0.00	0.00
4065	10103	6	0.00	0.00
4066	10104	6	0.00	0.00
4067	10105	6	0.00	0.00
4068	10106	6	0.00	0.00
4069	10107	6	0.00	0.00
4070	10108	6	0.00	0.00
4071	10109	6	0.00	0.00
4072	10110	6	0.00	0.00
4073	10111	6	0.00	0.00
4074	10112	6	0.00	0.00
4075	10113	6	0.00	0.00
4076	10114	6	0.00	0.00
4077	10115	6	0.00	0.00
4078	10116	6	0.00	0.00
4079	10117	6	0.00	0.00
4080	10118	6	0.00	0.00
4081	10119	6	0.00	0.00
4082	10120	6	0.00	0.00
4083	10121	6	0.00	0.00
4084	10122	6	0.00	0.00
4085	10123	6	0.00	0.00
4086	10124	6	0.00	0.00
4087	10125	6	0.00	0.00
4088	10126	6	0.00	0.00
4089	10127	6	0.00	0.00
4090	10128	6	0.00	0.00
4091	10129	6	0.00	0.00
4092	10130	6	0.00	0.00
4093	10131	6	0.00	0.00
4094	10132	6	0.00	0.00
4095	10133	6	0.00	0.00
4096	10134	6	0.00	0.00
4097	10135	6	0.00	0.00
4098	10136	6	0.00	0.00
4099	10137	6	0.00	0.00
4100	10138	6	0.00	0.00
4101	10139	6	0.00	0.00
4102	10140	6	0.00	0.00
4103	10141	6	0.00	0.00
4104	10142	6	0.00	0.00
4105	10143	6	0.00	0.00
4106	10144	6	0.00	0.00
4107	10145	6	0.00	0.00
4108	10146	6	0.00	0.00
4109	10147	6	0.00	0.00
4110	10148	6	0.00	0.00
4111	10149	6	0.00	0.00
4112	10150	6	0.00	0.00
4113	10151	6	0.00	0.00
4114	10152	6	0.00	0.00
4115	10153	6	0.00	0.00
4116	10154	6	0.00	0.00
4117	10155	6	0.00	0.00
4118	10156	6	0.00	0.00
4119	10157	6	0.00	0.00
4120	10158	6	0.00	0.00
4121	10159	6	0.00	0.00
4122	10160	6	0.00	0.00
4123	10161	6	0.00	0.00
4124	10162	6	0.00	0.00
4125	10163	6	0.00	0.00
4126	10164	6	0.00	0.00
4127	10165	6	0.00	0.00
4128	10166	6	0.00	0.00
4129	10167	6	0.00	0.00
4130	10168	6	0.00	0.00
4131	10169	6	0.00	0.00
4132	10170	6	0.00	0.00
4133	10171	6	0.00	0.00
4134	10172	6	0.00	0.00
4135	10173	6	0.00	0.00
4136	10174	6	0.00	0.00
4137	10175	6	0.00	0.00
4138	10176	6	0.00	0.00
4139	10177	6	0.00	0.00
4140	10178	6	0.00	0.00
4141	10179	6	0.00	0.00
4142	10180	6	0.00	0.00
4143	10181	6	0.00	0.00
4144	10182	6	0.00	0.00
4145	10183	6	0.00	0.00
4146	10184	6	0.00	0.00
4147	10185	6	0.00	0.00
4148	10186	6	0.00	0.00
4149	10187	6	0.00	0.00
4150	10188	6	0.00	0.00
4151	10189	6	0.00	0.00
4152	10190	6	0.00	0.00
4153	10191	6	0.00	0.00
4154	10192	6	0.00	0.00
4155	10193	6	0.00	0.00
4156	10194	6	0.00	0.00
4157	10195	6	0.00	0.00
4158	10196	6	0.00	0.00
4159	10197	6	0.00	0.00
4160	10198	6	0.00	0.00
4161	10199	6	0.00	0.00
4162	10200	6	0.00	0.00
4163	10201	6	0.00	0.00
4164	10202	6	0.00	0.00
4165	10203	6	0.00	0.00
4166	10204	6	0.00	0.00
4167	10205	6	0.00	0.00
4168	10206	6	0.00	0.00
4169	10207	6	0.00	0.00
4170	10208	6	0.00	0.00
4171	10209	6	0.00	0.00
4172	10210	6	0.00	0.00
4173	10211	6	0.00	0.00
4174	10212	6	0.00	0.00
4175	10213	6	0.00	0.00
4176	10214	6	0.00	0.00
4177	10215	6	0.00	0.00
4178	10216	6	0.00	0.00
4179	10217	6	0.00	0.00
4180	10218	6	0.00	0.00
4181	10219	6	0.00	0.00
4182	10220	6	0.00	0.00
4183	10221	6	0.00	0.00
4184	10222	6	0.00	0.00
4185	10223	6	0.00	0.00
4186	10224	6	0.00	0.00
4187	10225	6	0.00	0.00
4188	10226	6	0.00	0.00
4189	10227	6	0.00	0.00
4190	10228	6	0.00	0.00
4191	10229	6	0.00	0.00
4192	10230	6	0.00	0.00
4193	10231	6	0.00	0.00
4194	10232	6	0.00	0.00
4195	10233	6	0.00	0.00
4196	10234	6	0.00	0.00
4197	10235	6	0.00	0.00
4198	10236	6	0.00	0.00
4199	10237	6	0.00	0.00
4200	10238	6	0.00	0.00
4201	10239	6	0.00	0.00
4202	10240	6	0.00	0.00
4203	10241	6	0.00	0.00
4204	10242	6	0.00	0.00
4205	10244	6	0.00	0.00
4206	10245	6	0.00	0.00
4207	10246	6	0.00	0.00
4208	10247	6	0.00	0.00
4209	10248	6	0.00	0.00
4210	10250	6	0.00	0.00
4211	10251	6	0.00	0.00
4213	10253	6	0.00	0.00
4214	10254	6	0.00	0.00
4215	10255	6	0.00	0.00
4216	10256	6	0.00	0.00
4217	10257	6	0.00	0.00
4218	10258	6	0.00	0.00
4219	10259	6	0.00	0.00
4220	10260	6	0.00	0.00
4221	10261	6	0.00	0.00
4222	10262	6	0.00	0.00
4223	10263	6	0.00	0.00
4224	10264	6	0.00	0.00
4225	10265	6	0.00	0.00
4226	10266	6	0.00	0.00
4227	10267	6	0.00	0.00
4228	10268	6	0.00	0.00
4229	10269	6	0.00	0.00
4230	10272	6	0.00	0.00
4231	10273	6	0.00	0.00
4232	10274	6	0.00	0.00
4233	10275	6	0.00	0.00
4234	10276	6	0.00	0.00
4235	10277	6	0.00	0.00
4236	10278	6	0.00	0.00
4237	10279	6	0.00	0.00
4238	10280	6	0.00	0.00
4239	10281	6	0.00	0.00
4240	10282	6	0.00	0.00
4241	10283	6	0.00	0.00
4242	10284	6	0.00	0.00
4243	10285	6	0.00	0.00
4244	10286	6	0.00	0.00
4245	10287	6	0.00	0.00
4246	10288	6	0.00	0.00
4247	10289	6	0.00	0.00
4248	10290	6	0.00	0.00
4249	10291	6	0.00	0.00
4250	10292	6	0.00	0.00
4251	10293	6	0.00	0.00
4252	10294	6	0.00	0.00
4253	10295	6	0.00	0.00
4254	10296	6	0.00	0.00
4255	10297	6	0.00	0.00
4256	10299	6	0.00	0.00
4257	10300	6	0.00	0.00
4258	10301	6	0.00	0.00
4259	10303	6	0.00	0.00
4260	10304	6	0.00	0.00
4261	10305	6	0.00	0.00
4262	10306	6	0.00	0.00
4263	10307	6	0.00	0.00
4264	10309	6	0.00	0.00
4265	10310	6	0.00	0.00
4266	10311	6	0.00	0.00
4267	10312	6	0.00	0.00
4268	10313	6	0.00	0.00
4269	10314	6	0.00	0.00
4270	10315	6	0.00	0.00
4271	10316	6	0.00	0.00
4272	10317	6	0.00	0.00
4273	10319	6	0.00	0.00
4274	10321	6	0.00	0.00
4275	10322	6	0.00	0.00
4276	10323	6	0.00	0.00
4277	10324	6	0.00	0.00
4278	10325	6	0.00	0.00
4279	10326	6	0.00	0.00
4280	10327	6	0.00	0.00
4281	10328	6	0.00	0.00
4282	10329	6	0.00	0.00
4283	10330	6	0.00	0.00
4284	10331	6	0.00	0.00
4285	10332	6	0.00	0.00
4286	10333	6	0.00	0.00
4287	10334	6	0.00	0.00
4288	10335	6	0.00	0.00
4289	10336	6	0.00	0.00
4290	10337	6	0.00	0.00
4291	10338	6	0.00	0.00
4292	10339	6	0.00	0.00
4293	10340	6	0.00	0.00
4294	10341	6	0.00	0.00
4295	10342	6	0.00	0.00
4296	10343	6	0.00	0.00
4297	10344	6	0.00	0.00
4298	10345	6	0.00	0.00
4299	10346	6	0.00	0.00
4300	10347	6	0.00	0.00
4301	10348	6	0.00	0.00
4302	10349	6	0.00	0.00
4303	10350	6	0.00	0.00
4304	10351	6	0.00	0.00
4305	10352	6	0.00	0.00
4306	10354	6	0.00	0.00
4307	10353	6	0.00	0.00
4308	10355	6	0.00	0.00
4309	10356	6	0.00	0.00
4310	10357	6	0.00	0.00
4311	10358	6	0.00	0.00
4312	10359	6	0.00	0.00
4313	10360	6	0.00	0.00
4314	10361	6	0.00	0.00
4315	10362	6	0.00	0.00
4316	10363	6	0.00	0.00
4317	10364	6	0.00	0.00
4318	10365	6	0.00	0.00
4319	10366	6	0.00	0.00
4320	10367	6	0.00	0.00
4321	10368	6	0.00	0.00
4322	10369	6	0.00	0.00
4323	10370	6	0.00	0.00
4324	10371	6	0.00	0.00
4325	10372	6	0.00	0.00
4326	10373	6	0.00	0.00
4327	10374	6	0.00	0.00
4328	10375	6	0.00	0.00
4329	10376	6	0.00	0.00
4330	10377	6	0.00	0.00
4331	10378	6	0.00	0.00
4332	10379	6	0.00	0.00
4333	10380	6	0.00	0.00
4334	10381	6	0.00	0.00
4335	10382	6	0.00	0.00
4336	10383	6	0.00	0.00
4337	10384	6	0.00	0.00
4338	10385	6	0.00	0.00
4339	10386	6	0.00	0.00
4340	10387	6	0.00	0.00
4341	10388	6	0.00	0.00
4342	10389	6	0.00	0.00
4343	10390	6	0.00	0.00
4344	10391	6	0.00	0.00
4345	10392	6	0.00	0.00
4346	10393	6	0.00	0.00
4347	10394	6	0.00	0.00
4348	10395	6	0.00	0.00
4349	10397	6	0.00	0.00
4350	10399	6	0.00	0.00
4351	10400	6	0.00	0.00
4352	10401	6	0.00	0.00
4353	10402	6	0.00	0.00
4354	10403	6	0.00	0.00
4355	10404	6	0.00	0.00
4356	10405	6	0.00	0.00
4357	10406	6	0.00	0.00
4358	10407	6	0.00	0.00
4359	10408	6	0.00	0.00
4360	10409	6	0.00	0.00
4361	10410	6	0.00	0.00
4362	10411	6	0.00	0.00
4363	10412	6	0.00	0.00
4364	10413	6	0.00	0.00
4365	10415	6	0.00	0.00
4366	10416	6	0.00	0.00
4367	10417	6	0.00	0.00
4368	10418	6	0.00	0.00
4369	10419	6	0.00	0.00
4370	10420	6	0.00	0.00
4371	10421	6	0.00	0.00
4372	10422	6	0.00	0.00
4373	10423	6	0.00	0.00
4374	10424	6	0.00	0.00
4375	10425	6	0.00	0.00
4376	10426	6	0.00	0.00
4377	10427	6	0.00	0.00
4378	10428	6	0.00	0.00
4379	10429	6	0.00	0.00
4380	10430	6	0.00	0.00
4381	10431	6	0.00	0.00
4382	10432	6	0.00	0.00
4383	10433	6	0.00	0.00
4384	10434	6	0.00	0.00
4385	10435	6	0.00	0.00
4386	10436	6	0.00	0.00
4387	10437	6	0.00	0.00
4388	10438	6	0.00	0.00
4389	10439	6	0.00	0.00
4390	10440	6	0.00	0.00
4391	10441	6	0.00	0.00
4392	10442	6	0.00	0.00
4393	10443	6	0.00	0.00
4394	10444	6	0.00	0.00
4395	10445	6	0.00	0.00
4396	10446	6	0.00	0.00
4397	10447	6	0.00	0.00
4398	10448	6	0.00	0.00
4399	10449	6	0.00	0.00
4400	10450	6	0.00	0.00
4401	10451	6	0.00	0.00
4402	10452	6	0.00	0.00
4403	10453	6	0.00	0.00
4404	10454	6	0.00	0.00
4405	10455	6	0.00	0.00
4406	10456	6	0.00	0.00
4407	10457	6	0.00	0.00
4408	10458	6	0.00	0.00
4409	10459	6	0.00	0.00
4410	10460	6	0.00	0.00
4411	10461	6	0.00	0.00
4412	10462	6	0.00	0.00
4413	10463	6	0.00	0.00
4414	10464	6	0.00	0.00
4415	10465	6	0.00	0.00
4416	10466	6	0.00	0.00
4417	10467	6	0.00	0.00
4418	10468	6	0.00	0.00
4419	10469	6	0.00	0.00
4420	10470	6	0.00	0.00
4421	10471	6	0.00	0.00
4422	10472	6	0.00	0.00
4423	10473	6	0.00	0.00
4424	10474	6	0.00	0.00
4425	10475	6	0.00	0.00
4426	10476	6	0.00	0.00
4427	10477	6	0.00	0.00
4428	10478	6	0.00	0.00
4429	10479	6	0.00	0.00
4430	10480	6	0.00	0.00
4431	10481	6	0.00	0.00
4432	10482	6	0.00	0.00
4433	10483	6	0.00	0.00
4434	10484	6	0.00	0.00
4435	10485	6	0.00	0.00
4436	10486	6	0.00	0.00
4437	10487	6	0.00	0.00
4438	10488	6	0.00	0.00
4439	10489	6	0.00	0.00
4440	10490	6	0.00	0.00
4441	10491	6	0.00	0.00
4442	10492	6	0.00	0.00
4443	10493	6	0.00	0.00
4444	10494	6	0.00	0.00
4445	10495	6	0.00	0.00
4446	10496	6	0.00	0.00
4447	10497	6	0.00	0.00
4448	10498	6	0.00	0.00
4449	10499	6	0.00	0.00
4450	10500	6	0.00	0.00
4451	10501	6	0.00	0.00
4452	10502	6	0.00	0.00
4453	10503	6	0.00	0.00
4454	10504	6	0.00	0.00
4455	10505	6	0.00	0.00
4456	10506	6	0.00	0.00
4457	10507	6	0.00	0.00
4458	10508	6	0.00	0.00
4459	10509	6	0.00	0.00
4460	10510	6	0.00	0.00
4461	10511	6	0.00	0.00
4462	10512	6	0.00	0.00
4463	10513	6	0.00	0.00
4464	10514	6	0.00	0.00
4465	10515	6	0.00	0.00
4466	10516	6	0.00	0.00
4467	10517	6	0.00	0.00
4468	10518	6	0.00	0.00
4469	10519	6	0.00	0.00
4470	10520	6	0.00	0.00
4471	10521	6	0.00	0.00
4472	10522	6	0.00	0.00
4473	10523	6	0.00	0.00
4474	10524	6	0.00	0.00
4475	10525	6	0.00	0.00
4476	10526	6	0.00	0.00
4477	10527	6	0.00	0.00
4478	10528	6	0.00	0.00
4479	10529	6	0.00	0.00
4480	10530	6	0.00	0.00
4481	10531	6	0.00	0.00
4482	10532	6	0.00	0.00
4483	10533	6	0.00	0.00
4484	10535	6	0.00	0.00
4485	10536	6	0.00	0.00
4486	10537	6	0.00	0.00
4487	10538	6	0.00	0.00
4488	10539	6	0.00	0.00
4489	10540	6	0.00	0.00
4490	10541	6	0.00	0.00
4491	10542	6	0.00	0.00
4492	10543	6	0.00	0.00
4493	10544	6	0.00	0.00
4494	10545	6	0.00	0.00
4495	10546	6	0.00	0.00
4496	10547	6	0.00	0.00
4497	10548	6	0.00	0.00
4498	10549	6	0.00	0.00
4499	10550	6	0.00	0.00
4500	10551	6	0.00	0.00
4501	10552	6	0.00	0.00
4502	10553	6	0.00	0.00
4503	10554	6	0.00	0.00
4504	10555	6	0.00	0.00
4505	10556	6	0.00	0.00
4506	10557	6	0.00	0.00
4507	10558	6	0.00	0.00
4508	10559	6	0.00	0.00
4509	10560	6	0.00	0.00
4510	10561	6	0.00	0.00
4511	10562	6	0.00	0.00
4512	10563	6	0.00	0.00
4513	10564	6	0.00	0.00
4514	10565	6	0.00	0.00
4515	10566	6	0.00	0.00
4516	10567	6	0.00	0.00
4517	10568	6	0.00	0.00
4518	10569	6	0.00	0.00
4519	10570	6	0.00	0.00
4520	10571	6	0.00	0.00
4521	10572	6	0.00	0.00
4522	10573	6	0.00	0.00
4523	10574	6	0.00	0.00
4524	10575	6	0.00	0.00
4525	10576	6	0.00	0.00
4526	10577	6	0.00	0.00
4527	10578	6	0.00	0.00
4528	10579	6	0.00	0.00
4529	10580	6	0.00	0.00
4530	10581	6	0.00	0.00
4531	10582	6	0.00	0.00
4532	10583	6	0.00	0.00
4533	10584	6	0.00	0.00
4534	10585	6	0.00	0.00
4535	10586	6	0.00	0.00
4536	10587	6	0.00	0.00
4537	10588	6	0.00	0.00
4538	10589	6	0.00	0.00
4539	10590	6	0.00	0.00
4540	10591	6	0.00	0.00
4541	10592	6	0.00	0.00
4542	10593	6	0.00	0.00
4543	10594	6	0.00	0.00
4544	10595	6	0.00	0.00
4545	10596	6	0.00	0.00
4546	10597	6	0.00	0.00
4547	10598	6	0.00	0.00
4548	10599	6	0.00	0.00
4549	10600	6	0.00	0.00
4550	10601	6	0.00	0.00
4551	10602	6	0.00	0.00
4552	10603	6	0.00	0.00
4553	10604	6	0.00	0.00
4554	10605	6	0.00	0.00
4555	10606	6	0.00	0.00
4556	10607	6	0.00	0.00
4557	10608	6	0.00	0.00
4558	10609	6	0.00	0.00
4559	10610	6	0.00	0.00
4560	10611	6	0.00	0.00
4561	10612	6	0.00	0.00
4562	10613	6	0.00	0.00
4563	10614	6	0.00	0.00
4564	10615	6	0.00	0.00
4565	10616	6	0.00	0.00
4566	10617	6	0.00	0.00
4567	10618	6	0.00	0.00
4568	10619	6	0.00	0.00
4569	10620	6	0.00	0.00
4570	10621	6	0.00	0.00
4571	10622	6	0.00	0.00
4572	10623	6	0.00	0.00
4573	10624	6	0.00	0.00
4574	10625	6	0.00	0.00
4575	10626	6	0.00	0.00
4576	10627	6	0.00	0.00
4577	10628	6	0.00	0.00
4578	10629	6	0.00	0.00
4579	10630	6	0.00	0.00
4580	10631	6	0.00	0.00
2362	10320	4	0.00	0.00
2365	10002	4	0.00	0.00
2366	10003	4	0.00	0.00
2367	10004	4	0.00	0.00
2368	10005	4	0.00	0.00
2369	10006	4	0.00	0.00
2370	10007	4	0.00	0.00
2371	10008	4	0.00	0.00
2372	10009	4	0.00	0.00
2373	10010	4	0.00	0.00
2374	10011	4	0.00	0.00
2375	10012	4	0.00	0.00
2376	10013	4	0.00	0.00
2377	10014	4	0.00	0.00
2378	10015	4	0.00	0.00
2379	10016	4	0.00	0.00
2380	10017	4	0.00	0.00
2381	10018	4	0.00	0.00
2382	10019	4	0.00	0.00
2383	10020	4	0.00	0.00
2384	10021	4	0.00	0.00
2385	10022	4	0.00	0.00
2386	10023	4	0.00	0.00
2387	10024	4	0.00	0.00
2388	10025	4	0.00	0.00
2389	10026	4	0.00	0.00
2390	10027	4	0.00	0.00
2391	10028	4	0.00	0.00
2392	10030	4	0.00	0.00
2393	10032	4	0.00	0.00
2394	10033	4	0.00	0.00
2395	10035	4	0.00	0.00
2396	10036	4	0.00	0.00
2397	10037	4	0.00	0.00
2398	10038	4	0.00	0.00
2399	10039	4	0.00	0.00
2400	10040	4	0.00	0.00
2401	10041	4	0.00	0.00
2402	10042	4	0.00	0.00
2403	10043	4	0.00	0.00
2404	10045	4	0.00	0.00
2405	10046	4	0.00	0.00
2406	10048	4	0.00	0.00
2407	10049	4	0.00	0.00
2408	10050	4	0.00	0.00
2409	10051	4	0.00	0.00
2410	10052	4	0.00	0.00
2411	10053	4	0.00	0.00
2412	10054	4	0.00	0.00
2413	10055	4	0.00	0.00
2414	10056	4	0.00	0.00
2415	10058	4	0.00	0.00
2416	10059	4	0.00	0.00
2417	10060	4	0.00	0.00
2418	10061	4	0.00	0.00
2419	10062	4	0.00	0.00
2420	10063	4	0.00	0.00
2421	10064	4	0.00	0.00
2423	10066	4	0.00	0.00
2424	10067	4	0.00	0.00
2426	10069	4	0.00	0.00
2427	10070	4	0.00	0.00
2428	10071	4	0.00	0.00
2429	10072	4	0.00	0.00
2430	10073	4	0.00	0.00
2431	10074	4	0.00	0.00
2432	10075	4	0.00	0.00
2433	10076	4	0.00	0.00
2434	10077	4	0.00	0.00
2435	10078	4	0.00	0.00
2436	10079	4	0.00	0.00
2437	10080	4	0.00	0.00
2438	10081	4	0.00	0.00
2439	10082	4	0.00	0.00
2440	10083	4	0.00	0.00
2441	10084	4	0.00	0.00
2442	10085	4	0.00	0.00
2443	10086	4	0.00	0.00
2444	10087	4	0.00	0.00
2445	10088	4	0.00	0.00
2446	10089	4	0.00	0.00
2447	10090	4	0.00	0.00
2448	10091	4	0.00	0.00
2449	10092	4	0.00	0.00
2450	10093	4	0.00	0.00
2451	10094	4	0.00	0.00
2452	10095	4	0.00	0.00
2453	10096	4	0.00	0.00
2454	10097	4	0.00	0.00
2455	10098	4	0.00	0.00
2456	10099	4	0.00	0.00
2457	10100	4	0.00	0.00
2458	10101	4	0.00	0.00
2459	10102	4	0.00	0.00
2460	10103	4	0.00	0.00
2461	10104	4	0.00	0.00
2462	10105	4	0.00	0.00
2463	10106	4	0.00	0.00
2464	10107	4	0.00	0.00
2465	10108	4	0.00	0.00
2466	10109	4	0.00	0.00
2467	10110	4	0.00	0.00
2468	10111	4	0.00	0.00
2469	10112	4	0.00	0.00
2470	10113	4	0.00	0.00
2471	10114	4	0.00	0.00
2472	10115	4	0.00	0.00
2473	10116	4	0.00	0.00
2474	10117	4	0.00	0.00
2475	10118	4	0.00	0.00
2476	10119	4	0.00	0.00
2477	10120	4	0.00	0.00
2478	10121	4	0.00	0.00
2479	10122	4	0.00	0.00
2480	10123	4	0.00	0.00
2481	10124	4	0.00	0.00
2482	10125	4	0.00	0.00
2483	10126	4	0.00	0.00
2484	10127	4	0.00	0.00
2485	10128	4	0.00	0.00
2486	10129	4	0.00	0.00
2487	10130	4	0.00	0.00
2488	10131	4	0.00	0.00
2489	10132	4	0.00	0.00
2490	10133	4	0.00	0.00
2491	10134	4	0.00	0.00
2492	10135	4	0.00	0.00
2493	10136	4	0.00	0.00
2494	10137	4	0.00	0.00
2495	10138	4	0.00	0.00
2496	10139	4	0.00	0.00
2497	10140	4	0.00	0.00
2498	10141	4	0.00	0.00
2499	10142	4	0.00	0.00
2500	10143	4	0.00	0.00
2501	10144	4	0.00	0.00
2502	10145	4	0.00	0.00
2503	10146	4	0.00	0.00
2504	10147	4	0.00	0.00
2505	10148	4	0.00	0.00
2506	10149	4	0.00	0.00
2507	10150	4	0.00	0.00
2508	10151	4	0.00	0.00
2509	10152	4	0.00	0.00
2510	10153	4	0.00	0.00
2511	10154	4	0.00	0.00
2512	10155	4	0.00	0.00
2513	10156	4	0.00	0.00
2514	10157	4	0.00	0.00
2515	10158	4	0.00	0.00
2516	10159	4	0.00	0.00
2517	10160	4	0.00	0.00
2518	10161	4	0.00	0.00
2519	10162	4	0.00	0.00
2520	10163	4	0.00	0.00
2521	10164	4	0.00	0.00
2522	10165	4	0.00	0.00
2523	10166	4	0.00	0.00
2524	10167	4	0.00	0.00
2525	10168	4	0.00	0.00
2526	10169	4	0.00	0.00
2527	10170	4	0.00	0.00
2528	10171	4	0.00	0.00
2529	10172	4	0.00	0.00
2530	10173	4	0.00	0.00
2531	10174	4	0.00	0.00
2532	10175	4	0.00	0.00
2533	10176	4	0.00	0.00
2534	10177	4	0.00	0.00
2535	10178	4	0.00	0.00
2536	10179	4	0.00	0.00
2537	10180	4	0.00	0.00
2538	10181	4	0.00	0.00
2539	10182	4	0.00	0.00
2540	10183	4	0.00	0.00
2541	10184	4	0.00	0.00
2542	10185	4	0.00	0.00
2543	10186	4	0.00	0.00
2544	10187	4	0.00	0.00
2545	10188	4	0.00	0.00
2546	10189	4	0.00	0.00
2547	10190	4	0.00	0.00
2548	10191	4	0.00	0.00
2549	10192	4	0.00	0.00
2550	10193	4	0.00	0.00
2551	10194	4	0.00	0.00
2552	10195	4	0.00	0.00
2553	10196	4	0.00	0.00
2554	10197	4	0.00	0.00
2555	10198	4	0.00	0.00
2556	10199	4	0.00	0.00
2557	10200	4	0.00	0.00
2558	10201	4	0.00	0.00
2559	10202	4	0.00	0.00
2560	10203	4	0.00	0.00
2561	10204	4	0.00	0.00
2562	10205	4	0.00	0.00
2563	10206	4	0.00	0.00
2564	10207	4	0.00	0.00
2565	10208	4	0.00	0.00
2566	10209	4	0.00	0.00
2567	10210	4	0.00	0.00
2568	10211	4	0.00	0.00
2569	10212	4	0.00	0.00
2570	10213	4	0.00	0.00
2571	10214	4	0.00	0.00
2572	10215	4	0.00	0.00
2573	10216	4	0.00	0.00
2574	10217	4	0.00	0.00
2575	10218	4	0.00	0.00
2576	10219	4	0.00	0.00
2577	10220	4	0.00	0.00
2578	10221	4	0.00	0.00
2579	10222	4	0.00	0.00
2580	10223	4	0.00	0.00
2581	10224	4	0.00	0.00
2582	10225	4	0.00	0.00
2583	10226	4	0.00	0.00
2584	10227	4	0.00	0.00
2585	10228	4	0.00	0.00
2586	10229	4	0.00	0.00
2587	10230	4	0.00	0.00
2588	10231	4	0.00	0.00
2589	10232	4	0.00	0.00
2590	10233	4	0.00	0.00
2591	10234	4	0.00	0.00
2592	10235	4	0.00	0.00
2593	10236	4	0.00	0.00
2594	10237	4	0.00	0.00
2595	10238	4	0.00	0.00
2596	10239	4	0.00	0.00
2597	10240	4	0.00	0.00
2598	10241	4	0.00	0.00
2599	10242	4	0.00	0.00
2600	10244	4	0.00	0.00
2601	10245	4	0.00	0.00
2602	10246	4	0.00	0.00
2603	10247	4	0.00	0.00
2604	10248	4	0.00	0.00
2605	10250	4	0.00	0.00
2606	10251	4	0.00	0.00
2608	10253	4	0.00	0.00
2609	10254	4	0.00	0.00
2610	10255	4	0.00	0.00
2611	10256	4	0.00	0.00
2612	10257	4	0.00	0.00
2613	10258	4	0.00	0.00
2614	10259	4	0.00	0.00
2615	10260	4	0.00	0.00
2616	10261	4	0.00	0.00
2617	10262	4	0.00	0.00
2618	10263	4	0.00	0.00
2619	10264	4	0.00	0.00
2620	10265	4	0.00	0.00
2621	10266	4	0.00	0.00
2622	10267	4	0.00	0.00
2623	10268	4	0.00	0.00
2624	10269	4	0.00	0.00
2625	10272	4	0.00	0.00
2626	10273	4	0.00	0.00
2627	10274	4	0.00	0.00
2628	10275	4	0.00	0.00
2629	10276	4	0.00	0.00
2630	10277	4	0.00	0.00
2631	10278	4	0.00	0.00
2632	10279	4	0.00	0.00
2633	10280	4	0.00	0.00
2634	10281	4	0.00	0.00
2635	10282	4	0.00	0.00
2636	10283	4	0.00	0.00
2637	10284	4	0.00	0.00
2638	10285	4	0.00	0.00
2639	10286	4	0.00	0.00
2640	10287	4	0.00	0.00
2641	10288	4	0.00	0.00
2642	10289	4	0.00	0.00
2643	10290	4	0.00	0.00
2644	10291	4	0.00	0.00
2645	10292	4	0.00	0.00
2646	10293	4	0.00	0.00
2647	10294	4	0.00	0.00
2648	10295	4	0.00	0.00
2649	10296	4	0.00	0.00
2650	10297	4	0.00	0.00
2651	10299	4	0.00	0.00
2652	10300	4	0.00	0.00
2653	10301	4	0.00	0.00
2654	10303	4	0.00	0.00
2655	10304	4	0.00	0.00
2656	10305	4	0.00	0.00
2657	10306	4	0.00	0.00
2658	10307	4	0.00	0.00
2659	10309	4	0.00	0.00
2660	10310	4	0.00	0.00
2661	10311	4	0.00	0.00
2662	10312	4	0.00	0.00
2663	10313	4	0.00	0.00
2664	10314	4	0.00	0.00
2665	10315	4	0.00	0.00
2666	10316	4	0.00	0.00
2667	10317	4	0.00	0.00
2668	10319	4	0.00	0.00
2669	10321	4	0.00	0.00
2670	10322	4	0.00	0.00
2671	10323	4	0.00	0.00
2672	10324	4	0.00	0.00
2673	10325	4	0.00	0.00
2674	10326	4	0.00	0.00
2675	10327	4	0.00	0.00
2676	10328	4	0.00	0.00
2677	10329	4	0.00	0.00
2678	10330	4	0.00	0.00
2679	10331	4	0.00	0.00
2680	10332	4	0.00	0.00
2681	10333	4	0.00	0.00
2682	10334	4	0.00	0.00
2683	10335	4	0.00	0.00
2684	10336	4	0.00	0.00
2685	10337	4	0.00	0.00
2686	10338	4	0.00	0.00
2687	10339	4	0.00	0.00
2688	10340	4	0.00	0.00
2689	10341	4	0.00	0.00
2690	10342	4	0.00	0.00
2691	10343	4	0.00	0.00
2692	10344	4	0.00	0.00
2693	10345	4	0.00	0.00
2694	10346	4	0.00	0.00
2695	10347	4	0.00	0.00
2696	10348	4	0.00	0.00
2697	10349	4	0.00	0.00
2698	10350	4	0.00	0.00
2699	10351	4	0.00	0.00
2700	10352	4	0.00	0.00
2701	10354	4	0.00	0.00
2702	10353	4	0.00	0.00
2703	10355	4	0.00	0.00
2704	10356	4	0.00	0.00
2705	10357	4	0.00	0.00
2706	10358	4	0.00	0.00
2707	10359	4	0.00	0.00
2708	10360	4	0.00	0.00
2709	10361	4	0.00	0.00
2710	10362	4	0.00	0.00
2711	10363	4	0.00	0.00
2712	10364	4	0.00	0.00
2713	10365	4	0.00	0.00
2714	10366	4	0.00	0.00
2715	10367	4	0.00	0.00
2716	10368	4	0.00	0.00
2717	10369	4	0.00	0.00
2718	10370	4	0.00	0.00
2719	10371	4	0.00	0.00
2720	10372	4	0.00	0.00
2721	10373	4	0.00	0.00
2722	10374	4	0.00	0.00
2723	10375	4	0.00	0.00
2724	10376	4	0.00	0.00
2725	10377	4	0.00	0.00
2726	10378	4	0.00	0.00
2727	10379	4	0.00	0.00
2728	10380	4	0.00	0.00
2729	10381	4	0.00	0.00
2730	10382	4	0.00	0.00
2731	10383	4	0.00	0.00
2732	10384	4	0.00	0.00
2733	10385	4	0.00	0.00
2734	10386	4	0.00	0.00
2735	10387	4	0.00	0.00
2736	10388	4	0.00	0.00
2737	10389	4	0.00	0.00
2738	10390	4	0.00	0.00
2739	10391	4	0.00	0.00
2740	10392	4	0.00	0.00
2741	10393	4	0.00	0.00
2742	10394	4	0.00	0.00
2743	10395	4	0.00	0.00
2744	10397	4	0.00	0.00
2745	10399	4	0.00	0.00
2746	10400	4	0.00	0.00
2747	10401	4	0.00	0.00
2748	10402	4	0.00	0.00
2749	10403	4	0.00	0.00
2750	10404	4	0.00	0.00
2751	10405	4	0.00	0.00
2752	10406	4	0.00	0.00
2753	10407	4	0.00	0.00
2754	10408	4	0.00	0.00
2755	10409	4	0.00	0.00
2756	10410	4	0.00	0.00
2757	10411	4	0.00	0.00
2758	10412	4	0.00	0.00
2759	10413	4	0.00	0.00
2760	10415	4	0.00	0.00
2761	10416	4	0.00	0.00
2762	10417	4	0.00	0.00
2763	10418	4	0.00	0.00
2764	10419	4	0.00	0.00
2765	10420	4	0.00	0.00
2766	10421	4	0.00	0.00
2767	10422	4	0.00	0.00
2768	10423	4	0.00	0.00
2769	10424	4	0.00	0.00
2770	10425	4	0.00	0.00
2771	10426	4	0.00	0.00
2772	10427	4	0.00	0.00
2773	10428	4	0.00	0.00
2774	10429	4	0.00	0.00
2775	10430	4	0.00	0.00
2776	10431	4	0.00	0.00
2777	10432	4	0.00	0.00
2778	10433	4	0.00	0.00
2779	10434	4	0.00	0.00
2780	10435	4	0.00	0.00
2781	10436	4	0.00	0.00
2782	10437	4	0.00	0.00
2783	10438	4	0.00	0.00
2784	10439	4	0.00	0.00
2785	10440	4	0.00	0.00
2786	10441	4	0.00	0.00
2787	10442	4	0.00	0.00
2788	10443	4	0.00	0.00
2789	10444	4	0.00	0.00
2790	10445	4	0.00	0.00
2791	10446	4	0.00	0.00
2792	10447	4	0.00	0.00
2793	10448	4	0.00	0.00
2794	10449	4	0.00	0.00
2795	10450	4	0.00	0.00
2796	10451	4	0.00	0.00
2797	10452	4	0.00	0.00
2798	10453	4	0.00	0.00
2799	10454	4	0.00	0.00
2800	10455	4	0.00	0.00
2801	10456	4	0.00	0.00
2802	10457	4	0.00	0.00
2803	10458	4	0.00	0.00
2804	10459	4	0.00	0.00
2805	10460	4	0.00	0.00
2806	10461	4	0.00	0.00
2807	10462	4	0.00	0.00
2808	10463	4	0.00	0.00
2809	10464	4	0.00	0.00
2810	10465	4	0.00	0.00
2811	10466	4	0.00	0.00
2812	10467	4	0.00	0.00
2813	10468	4	0.00	0.00
2814	10469	4	0.00	0.00
2815	10470	4	0.00	0.00
2816	10471	4	0.00	0.00
2817	10472	4	0.00	0.00
2818	10473	4	0.00	0.00
2819	10474	4	0.00	0.00
2820	10475	4	0.00	0.00
2821	10476	4	0.00	0.00
2822	10477	4	0.00	0.00
2823	10478	4	0.00	0.00
2824	10479	4	0.00	0.00
2825	10480	4	0.00	0.00
2826	10481	4	0.00	0.00
2827	10482	4	0.00	0.00
2828	10483	4	0.00	0.00
2829	10484	4	0.00	0.00
2830	10485	4	0.00	0.00
2831	10486	4	0.00	0.00
2832	10487	4	0.00	0.00
2833	10488	4	0.00	0.00
2834	10489	4	0.00	0.00
2835	10490	4	0.00	0.00
2836	10491	4	0.00	0.00
2837	10492	4	0.00	0.00
2838	10493	4	0.00	0.00
2839	10494	4	0.00	0.00
2840	10495	4	0.00	0.00
2841	10496	4	0.00	0.00
2842	10497	4	0.00	0.00
2843	10498	4	0.00	0.00
2844	10499	4	0.00	0.00
2845	10500	4	0.00	0.00
2846	10501	4	0.00	0.00
2847	10502	4	0.00	0.00
2848	10503	4	0.00	0.00
2849	10504	4	0.00	0.00
2850	10505	4	0.00	0.00
2851	10506	4	0.00	0.00
2852	10507	4	0.00	0.00
2853	10508	4	0.00	0.00
2854	10509	4	0.00	0.00
2855	10510	4	0.00	0.00
2856	10511	4	0.00	0.00
2857	10512	4	0.00	0.00
2858	10513	4	0.00	0.00
2859	10514	4	0.00	0.00
2860	10515	4	0.00	0.00
2861	10516	4	0.00	0.00
2862	10517	4	0.00	0.00
2863	10518	4	0.00	0.00
2864	10519	4	0.00	0.00
2865	10520	4	0.00	0.00
2866	10521	4	0.00	0.00
2867	10522	4	0.00	0.00
2868	10523	4	0.00	0.00
2869	10524	4	0.00	0.00
2870	10525	4	0.00	0.00
2871	10526	4	0.00	0.00
2872	10527	4	0.00	0.00
2873	10528	4	0.00	0.00
2874	10529	4	0.00	0.00
2875	10530	4	0.00	0.00
2876	10531	4	0.00	0.00
2877	10532	4	0.00	0.00
2878	10533	4	0.00	0.00
2879	10535	4	0.00	0.00
2880	10536	4	0.00	0.00
2881	10537	4	0.00	0.00
2882	10538	4	0.00	0.00
2883	10539	4	0.00	0.00
2884	10540	4	0.00	0.00
2885	10541	4	0.00	0.00
2886	10542	4	0.00	0.00
2887	10543	4	0.00	0.00
2888	10544	4	0.00	0.00
2889	10545	4	0.00	0.00
2890	10546	4	0.00	0.00
2891	10547	4	0.00	0.00
2892	10548	4	0.00	0.00
2893	10549	4	0.00	0.00
2894	10550	4	0.00	0.00
2895	10551	4	0.00	0.00
2896	10552	4	0.00	0.00
2897	10553	4	0.00	0.00
2898	10554	4	0.00	0.00
2899	10555	4	0.00	0.00
2900	10556	4	0.00	0.00
2901	10557	4	0.00	0.00
2902	10558	4	0.00	0.00
2903	10559	4	0.00	0.00
2904	10560	4	0.00	0.00
2905	10561	4	0.00	0.00
2906	10562	4	0.00	0.00
2907	10563	4	0.00	0.00
2908	10564	4	0.00	0.00
2909	10565	4	0.00	0.00
2910	10566	4	0.00	0.00
2911	10567	4	0.00	0.00
2912	10568	4	0.00	0.00
2913	10569	4	0.00	0.00
2914	10570	4	0.00	0.00
2915	10571	4	0.00	0.00
2916	10572	4	0.00	0.00
2917	10573	4	0.00	0.00
2918	10574	4	0.00	0.00
2919	10575	4	0.00	0.00
2920	10576	4	0.00	0.00
2921	10577	4	0.00	0.00
2922	10578	4	0.00	0.00
2923	10579	4	0.00	0.00
2924	10580	4	0.00	0.00
2925	10581	4	0.00	0.00
2926	10582	4	0.00	0.00
2927	10583	4	0.00	0.00
2928	10584	4	0.00	0.00
2929	10585	4	0.00	0.00
2930	10586	4	0.00	0.00
2931	10587	4	0.00	0.00
2932	10588	4	0.00	0.00
2933	10589	4	0.00	0.00
2934	10590	4	0.00	0.00
2935	10591	4	0.00	0.00
2936	10592	4	0.00	0.00
2937	10593	4	0.00	0.00
2938	10594	4	0.00	0.00
2939	10595	4	0.00	0.00
2940	10596	4	0.00	0.00
2941	10597	4	0.00	0.00
2942	10598	4	0.00	0.00
2943	10599	4	0.00	0.00
2944	10600	4	0.00	0.00
2945	10601	4	0.00	0.00
2946	10602	4	0.00	0.00
2947	10603	4	0.00	0.00
2948	10604	4	0.00	0.00
2949	10605	4	0.00	0.00
2950	10606	4	0.00	0.00
2951	10607	4	0.00	0.00
2952	10608	4	0.00	0.00
2953	10609	4	0.00	0.00
2954	10610	4	0.00	0.00
2955	10611	4	0.00	0.00
2956	10612	4	0.00	0.00
2957	10613	4	0.00	0.00
2958	10614	4	0.00	0.00
2959	10615	4	0.00	0.00
2960	10616	4	0.00	0.00
2961	10617	4	0.00	0.00
2962	10618	4	0.00	0.00
2963	10619	4	0.00	0.00
2964	10620	4	0.00	0.00
2965	10621	4	0.00	0.00
2966	10622	4	0.00	0.00
2967	10623	4	0.00	0.00
2968	10624	4	0.00	0.00
2969	10625	4	0.00	0.00
2970	10626	4	0.00	0.00
2971	10627	4	0.00	0.00
2972	10628	4	0.00	0.00
2973	10629	4	0.00	0.00
2974	10630	4	0.00	0.00
2975	10631	4	0.00	0.00
3108	10308	4	0.00	0.00
3109	10302	4	0.00	0.00
3110	10414	4	0.00	0.00
3111	10047	4	0.00	0.00
3132	10396	4	0.00	0.00
3133	10318	4	0.00	0.00
3134	10298	4	0.00	0.00
3135	10034	4	0.00	0.00
3137	10271	4	0.00	0.00
3138	10270	4	0.00	0.00
3139	10031	4	0.00	0.00
3143	10249	4	0.00	0.00
3144	10243	4	0.00	0.00
3145	10029	4	0.00	0.00
3147	10398	4	0.00	0.00
3148	10044	4	0.00	0.00
1559	10318	2	0.00	0.00
1560	10298	2	0.00	0.00
1561	10034	2	0.00	0.00
1563	10271	2	0.00	0.00
1564	10270	2	0.00	0.00
1565	10031	2	0.00	0.00
1566	10534	2	0.00	0.00
1567	10057	2	0.00	0.00
3151	10318	5	0.00	0.00
3153	10298	5	0.00	0.00
3179	10034	5	0.00	0.00
3180	10271	5	0.00	0.00
3181	10270	5	0.00	0.00
3182	10031	5	0.00	0.00
3140	10534	4	0.00	0.00
3141	10057	4	0.00	0.00
4723	10788	4	0.00	0.00
4724	10788	2	0.00	0.00
4725	10788	5	0.00	0.00
4726	10788	6	0.00	0.00
2425	10068	4	0.00	0.00
2364	10001	4	0.00	0.00
3168	10396	5	0.00	0.00
3171	10044	5	0.00	0.00
3162	10249	5	0.00	0.00
3163	10243	5	0.00	0.00
3164	10029	5	0.00	0.00
4739	10794	4	0.00	0.00
4740	10794	2	0.00	0.00
4741	10794	5	0.00	0.00
4742	10794	6	0.00	0.00
4743	10795	4	0.00	0.00
4744	10795	2	0.00	0.00
4745	10795	5	0.00	0.00
4746	10795	6	0.00	0.00
4747	10796	4	0.00	0.00
4748	10796	2	0.00	0.00
4749	10796	5	0.00	0.00
4750	10796	6	0.00	0.00
4751	10797	4	0.00	0.00
4752	10797	2	0.00	0.00
4753	10797	5	0.00	0.00
4754	10797	6	0.00	0.00
4755	10798	4	0.00	0.00
4756	10798	2	0.00	0.00
4757	10798	5	0.00	0.00
4758	10798	6	0.00	0.00
4759	10799	4	0.00	0.00
4760	10799	2	0.00	0.00
4761	10799	5	0.00	0.00
4762	10799	6	0.00	0.00
4763	10800	4	0.00	0.00
4764	10800	2	0.00	0.00
4765	10800	5	0.00	0.00
4766	10800	6	0.00	0.00
4767	10801	4	0.00	0.00
4768	10801	2	0.00	0.00
4769	10801	5	0.00	0.00
4770	10801	6	0.00	0.00
4771	10802	4	0.00	0.00
4772	10802	2	0.00	0.00
4773	10802	5	0.00	0.00
4774	10802	6	0.00	0.00
4775	10803	4	0.00	0.00
4776	10803	2	0.00	0.00
4777	10803	5	0.00	0.00
4778	10803	6	0.00	0.00
4779	10804	4	0.00	0.00
4780	10804	2	0.00	0.00
4781	10804	5	0.00	0.00
4782	10804	6	0.00	0.00
4783	10805	4	0.00	0.00
4784	10805	2	0.00	0.00
4785	10805	5	0.00	0.00
4786	10805	6	0.00	0.00
4787	10806	4	0.00	0.00
4788	10806	2	0.00	0.00
4789	10806	5	0.00	0.00
4790	10806	6	0.00	0.00
4791	10807	4	0.00	0.00
4792	10807	2	0.00	0.00
4793	10807	5	0.00	0.00
4794	10807	6	0.00	0.00
4795	10808	4	0.00	0.00
4796	10808	2	0.00	0.00
4797	10808	5	0.00	0.00
4798	10808	6	0.00	0.00
4799	10809	4	0.00	0.00
4800	10809	2	0.00	0.00
4801	10809	5	0.00	0.00
4802	10809	6	0.00	0.00
4803	10810	4	0.00	0.00
4804	10810	2	0.00	0.00
4805	10810	5	0.00	0.00
4806	10810	6	0.00	0.00
4807	10811	4	0.00	0.00
4808	10811	2	0.00	0.00
4809	10811	5	0.00	0.00
4810	10811	6	0.00	0.00
4811	10812	4	0.00	0.00
4812	10812	2	0.00	0.00
4813	10812	5	0.00	0.00
4814	10812	6	0.00	0.00
4815	10813	4	0.00	0.00
4816	10813	2	0.00	0.00
4817	10813	5	0.00	0.00
4818	10813	6	0.00	0.00
4819	10814	4	0.00	0.00
4820	10814	2	0.00	0.00
4821	10814	5	0.00	0.00
4822	10814	6	0.00	0.00
4823	10815	4	0.00	0.00
4824	10815	2	0.00	0.00
4825	10815	5	0.00	0.00
4826	10815	6	0.00	0.00
4827	10816	4	0.00	0.00
4828	10816	2	0.00	0.00
4829	10816	5	0.00	0.00
4830	10816	6	0.00	0.00
4831	10817	4	0.00	0.00
4832	10817	2	0.00	0.00
4833	10817	5	0.00	0.00
4834	10817	6	0.00	0.00
4835	10818	4	0.00	0.00
4836	10818	2	0.00	0.00
4837	10818	5	0.00	0.00
4838	10818	6	0.00	0.00
4839	10819	4	0.00	0.00
4840	10819	2	0.00	0.00
4841	10819	5	0.00	0.00
4842	10819	6	0.00	0.00
4843	10820	4	0.00	0.00
4844	10820	2	0.00	0.00
4845	10820	5	0.00	0.00
4846	10820	6	0.00	0.00
4847	10821	4	0.00	0.00
4848	10821	2	0.00	0.00
4849	10821	5	0.00	0.00
4850	10821	6	0.00	0.00
4851	10822	4	0.00	0.00
4852	10822	2	0.00	0.00
4853	10822	5	0.00	0.00
4854	10822	6	0.00	0.00
4855	10823	4	0.00	0.00
4856	10823	2	0.00	0.00
4857	10823	5	0.00	0.00
4858	10823	6	0.00	0.00
4859	10824	4	0.00	0.00
4860	10824	2	0.00	0.00
4861	10824	5	0.00	0.00
4862	10824	6	0.00	0.00
4863	10825	4	0.00	0.00
4864	10825	2	0.00	0.00
4865	10825	5	0.00	0.00
4866	10825	6	0.00	0.00
4867	10826	4	0.00	0.00
4868	10826	2	0.00	0.00
4869	10826	5	0.00	0.00
4870	10826	6	0.00	0.00
4871	10827	4	0.00	0.00
4872	10827	2	0.00	0.00
4873	10827	5	0.00	0.00
4874	10827	6	0.00	0.00
4875	10828	4	0.00	0.00
4876	10828	2	0.00	0.00
4877	10828	5	0.00	0.00
4878	10828	6	0.00	0.00
4879	10829	4	0.00	0.00
4880	10829	2	0.00	0.00
4881	10829	5	0.00	0.00
4882	10829	6	0.00	0.00
4883	10830	4	0.00	0.00
4884	10830	2	0.00	0.00
4885	10830	5	0.00	0.00
4886	10830	6	0.00	0.00
4887	10831	4	0.00	0.00
4888	10831	2	0.00	0.00
4889	10831	5	0.00	0.00
4890	10831	6	0.00	0.00
4891	10832	4	0.00	0.00
4892	10832	2	0.00	0.00
4893	10832	5	0.00	0.00
4894	10832	6	0.00	0.00
4895	10833	4	0.00	0.00
4896	10833	2	0.00	0.00
4897	10833	5	0.00	0.00
4898	10833	6	0.00	0.00
4899	10834	4	0.00	0.00
4900	10834	2	0.00	0.00
4901	10834	5	0.00	0.00
4902	10834	6	0.00	0.00
4903	10835	4	0.00	0.00
4904	10835	2	0.00	0.00
4905	10835	5	0.00	0.00
4906	10835	6	0.00	0.00
4907	10836	4	0.00	0.00
4908	10836	2	0.00	0.00
4909	10836	5	0.00	0.00
4910	10836	6	0.00	0.00
4911	10837	4	0.00	0.00
4912	10837	2	0.00	0.00
4913	10837	5	0.00	0.00
4914	10837	6	0.00	0.00
4915	10838	4	0.00	0.00
4916	10838	2	0.00	0.00
4917	10838	5	0.00	0.00
4918	10838	6	0.00	0.00
4919	10839	4	0.00	0.00
4920	10839	2	0.00	0.00
4921	10839	5	0.00	0.00
4922	10839	6	0.00	0.00
4923	10840	4	0.00	0.00
4924	10840	2	0.00	0.00
4925	10840	5	0.00	0.00
4926	10840	6	0.00	0.00
4927	10841	4	0.00	0.00
4928	10841	2	0.00	0.00
4929	10841	5	0.00	0.00
4930	10841	6	0.00	0.00
4931	10842	4	0.00	0.00
4932	10842	2	0.00	0.00
4933	10842	5	0.00	0.00
4934	10842	6	0.00	0.00
4935	10843	4	0.00	0.00
4936	10843	2	0.00	0.00
4937	10843	5	0.00	0.00
4938	10843	6	0.00	0.00
4939	10844	4	0.00	0.00
4940	10844	2	0.00	0.00
4941	10844	5	0.00	0.00
4942	10844	6	0.00	0.00
4943	10845	4	0.00	0.00
4944	10845	2	0.00	0.00
4945	10845	5	0.00	0.00
4946	10845	6	0.00	0.00
4947	10846	4	0.00	0.00
4948	10846	2	0.00	0.00
4949	10846	5	0.00	0.00
4950	10846	6	0.00	0.00
4951	10847	4	0.00	0.00
4952	10847	2	0.00	0.00
4953	10847	5	0.00	0.00
4954	10847	6	0.00	0.00
4955	10848	4	0.00	0.00
4956	10848	2	0.00	0.00
4957	10848	5	0.00	0.00
4958	10848	6	0.00	0.00
4959	10849	4	0.00	0.00
4960	10849	2	0.00	0.00
4961	10849	5	0.00	0.00
4962	10849	6	0.00	0.00
4963	10850	4	0.00	0.00
4964	10850	2	0.00	0.00
4965	10850	5	0.00	0.00
4966	10850	6	0.00	0.00
4967	10851	4	0.00	0.00
4968	10851	2	0.00	0.00
4969	10851	5	0.00	0.00
4970	10851	6	0.00	0.00
4971	10852	4	0.00	0.00
4972	10852	2	0.00	0.00
4973	10852	5	0.00	0.00
4974	10852	6	0.00	0.00
4975	10853	4	0.00	0.00
4976	10853	2	0.00	0.00
4977	10853	5	0.00	0.00
4978	10853	6	0.00	0.00
4979	10854	4	0.00	0.00
4980	10854	2	0.00	0.00
4981	10854	5	0.00	0.00
4982	10854	6	0.00	0.00
4983	10855	4	0.00	0.00
4984	10855	2	0.00	0.00
4985	10855	5	0.00	0.00
4986	10855	6	0.00	0.00
4987	10856	4	0.00	0.00
4988	10856	2	0.00	0.00
4989	10856	5	0.00	0.00
4990	10856	6	0.00	0.00
4991	10857	4	0.00	0.00
4992	10857	2	0.00	0.00
4993	10857	5	0.00	0.00
4994	10857	6	0.00	0.00
4995	10858	4	0.00	0.00
4996	10858	2	0.00	0.00
4997	10858	5	0.00	0.00
4998	10858	6	0.00	0.00
4999	10859	4	0.00	0.00
5000	10859	2	0.00	0.00
5001	10859	5	0.00	0.00
5002	10859	6	0.00	0.00
5003	10860	4	0.00	0.00
5004	10860	2	0.00	0.00
5005	10860	5	0.00	0.00
5006	10860	6	0.00	0.00
5007	10861	4	0.00	0.00
5008	10861	2	0.00	0.00
5009	10861	5	0.00	0.00
5010	10861	6	0.00	0.00
5011	10862	4	0.00	0.00
5012	10862	2	0.00	0.00
5013	10862	5	0.00	0.00
5014	10862	6	0.00	0.00
5015	10863	4	0.00	0.00
5016	10863	2	0.00	0.00
5017	10863	5	0.00	0.00
5018	10863	6	0.00	0.00
5019	10864	4	0.00	0.00
5020	10864	2	0.00	0.00
5021	10864	5	0.00	0.00
5022	10864	6	0.00	0.00
5023	10865	4	0.00	0.00
5024	10865	2	0.00	0.00
5025	10865	5	0.00	0.00
5026	10865	6	0.00	0.00
5027	10866	4	0.00	0.00
5028	10866	2	0.00	0.00
5029	10866	5	0.00	0.00
5030	10866	6	0.00	0.00
5031	10867	4	0.00	0.00
5032	10867	2	0.00	0.00
5033	10867	5	0.00	0.00
5034	10867	6	0.00	0.00
5035	10868	4	0.00	0.00
5036	10868	2	0.00	0.00
5037	10868	5	0.00	0.00
5038	10868	6	0.00	0.00
5039	10869	4	0.00	0.00
5040	10869	2	0.00	0.00
5041	10869	5	0.00	0.00
5042	10869	6	0.00	0.00
5043	10870	4	0.00	0.00
5044	10870	2	0.00	0.00
5045	10870	5	0.00	0.00
5046	10870	6	0.00	0.00
5047	10871	4	0.00	0.00
5048	10871	2	0.00	0.00
5049	10871	5	0.00	0.00
5050	10871	6	0.00	0.00
5051	10872	4	0.00	0.00
5052	10872	2	0.00	0.00
5053	10872	5	0.00	0.00
5054	10872	6	0.00	0.00
5055	10873	4	0.00	0.00
5056	10873	2	0.00	0.00
5057	10873	5	0.00	0.00
5058	10873	6	0.00	0.00
5059	10874	4	0.00	0.00
5060	10874	2	0.00	0.00
5061	10874	5	0.00	0.00
5062	10874	6	0.00	0.00
5063	10875	4	0.00	0.00
5064	10875	2	0.00	0.00
5065	10875	5	0.00	0.00
5066	10875	6	0.00	0.00
5067	10876	4	0.00	0.00
5068	10876	2	0.00	0.00
5069	10876	5	0.00	0.00
5070	10876	6	0.00	0.00
5071	10877	4	0.00	0.00
5072	10877	2	0.00	0.00
5073	10877	5	0.00	0.00
5074	10877	6	0.00	0.00
5075	10878	4	0.00	0.00
5076	10878	2	0.00	0.00
5077	10878	5	0.00	0.00
5078	10878	6	0.00	0.00
5079	10879	4	0.00	0.00
5080	10879	2	0.00	0.00
5081	10879	5	0.00	0.00
5082	10879	6	0.00	0.00
5083	10880	4	0.00	0.00
5084	10880	2	0.00	0.00
5085	10880	5	0.00	0.00
5086	10880	6	0.00	0.00
5087	10881	4	0.00	0.00
5088	10881	2	0.00	0.00
5089	10881	5	0.00	0.00
5090	10881	6	0.00	0.00
5091	10882	4	0.00	0.00
5092	10882	2	0.00	0.00
5093	10882	5	0.00	0.00
5094	10882	6	0.00	0.00
5095	10883	4	0.00	0.00
5096	10883	2	0.00	0.00
5097	10883	5	0.00	0.00
5098	10883	6	0.00	0.00
5099	10884	4	0.00	0.00
5100	10884	2	0.00	0.00
5101	10884	5	0.00	0.00
5102	10884	6	0.00	0.00
5103	10885	4	0.00	0.00
5104	10885	2	0.00	0.00
5105	10885	5	0.00	0.00
5106	10885	6	0.00	0.00
5107	10886	4	0.00	0.00
5108	10886	2	0.00	0.00
5109	10886	5	0.00	0.00
5110	10886	6	0.00	0.00
5111	10887	4	0.00	0.00
5112	10887	2	0.00	0.00
5113	10887	5	0.00	0.00
5114	10887	6	0.00	0.00
5115	10888	4	0.00	0.00
5116	10888	2	0.00	0.00
5117	10888	5	0.00	0.00
5118	10888	6	0.00	0.00
5119	10889	4	0.00	0.00
5120	10889	2	0.00	0.00
5121	10889	5	0.00	0.00
5122	10889	6	0.00	0.00
5123	10890	4	0.00	0.00
5124	10890	2	0.00	0.00
5125	10890	5	0.00	0.00
5126	10890	6	0.00	0.00
5127	10891	4	0.00	0.00
5128	10891	2	0.00	0.00
5129	10891	5	0.00	0.00
5130	10891	6	0.00	0.00
5131	10892	4	0.00	0.00
5132	10892	2	0.00	0.00
5133	10892	5	0.00	0.00
5134	10892	6	0.00	0.00
5135	10893	4	0.00	0.00
5136	10893	2	0.00	0.00
5137	10893	5	0.00	0.00
5138	10893	6	0.00	0.00
5139	10894	4	0.00	0.00
5140	10894	2	0.00	0.00
5141	10894	5	0.00	0.00
5142	10894	6	0.00	0.00
5143	10895	4	0.00	0.00
5144	10895	2	0.00	0.00
5145	10895	5	0.00	0.00
5146	10895	6	0.00	0.00
5147	10896	4	0.00	0.00
5148	10896	2	0.00	0.00
5149	10896	5	0.00	0.00
5150	10896	6	0.00	0.00
5151	10897	4	0.00	0.00
5152	10897	2	0.00	0.00
5153	10897	5	0.00	0.00
5154	10897	6	0.00	0.00
5155	10898	4	0.00	0.00
5156	10898	2	0.00	0.00
5157	10898	5	0.00	0.00
5158	10898	6	0.00	0.00
5159	10899	4	0.00	0.00
5160	10899	2	0.00	0.00
5161	10899	5	0.00	0.00
5162	10899	6	0.00	0.00
5163	10900	4	0.00	0.00
5164	10900	2	0.00	0.00
5165	10900	5	0.00	0.00
5166	10900	6	0.00	0.00
5167	10901	4	0.00	0.00
5168	10901	2	0.00	0.00
5169	10901	5	0.00	0.00
5170	10901	6	0.00	0.00
5171	10902	4	0.00	0.00
5172	10902	2	0.00	0.00
5173	10902	5	0.00	0.00
5174	10902	6	0.00	0.00
5175	10903	4	0.00	0.00
5176	10903	2	0.00	0.00
5177	10903	5	0.00	0.00
5178	10903	6	0.00	0.00
5179	10904	4	0.00	0.00
5180	10904	2	0.00	0.00
5181	10904	5	0.00	0.00
5182	10904	6	0.00	0.00
5183	10905	4	0.00	0.00
5184	10905	2	0.00	0.00
5185	10905	5	0.00	0.00
5186	10905	6	0.00	0.00
5187	10906	4	0.00	0.00
5188	10906	2	0.00	0.00
5189	10906	5	0.00	0.00
5190	10906	6	0.00	0.00
5191	10907	4	0.00	0.00
5192	10907	2	0.00	0.00
5193	10907	5	0.00	0.00
5194	10907	6	0.00	0.00
5195	10908	4	0.00	0.00
5196	10908	2	0.00	0.00
5197	10908	5	0.00	0.00
5198	10908	6	0.00	0.00
5199	10909	4	0.00	0.00
5200	10909	2	0.00	0.00
5201	10909	5	0.00	0.00
5202	10909	6	0.00	0.00
5203	10910	4	0.00	0.00
5204	10910	2	0.00	0.00
5205	10910	5	0.00	0.00
5206	10910	6	0.00	0.00
5207	10911	4	0.00	0.00
5208	10911	2	0.00	0.00
5209	10911	5	0.00	0.00
5210	10911	6	0.00	0.00
5211	10912	4	0.00	0.00
5212	10912	2	0.00	0.00
5213	10912	5	0.00	0.00
5214	10912	6	0.00	0.00
5215	10913	4	0.00	0.00
5216	10913	2	0.00	0.00
5217	10913	5	0.00	0.00
5218	10913	6	0.00	0.00
5219	10914	4	0.00	0.00
5220	10914	2	0.00	0.00
5221	10914	5	0.00	0.00
5222	10914	6	0.00	0.00
5223	10915	4	0.00	0.00
5224	10915	2	0.00	0.00
5225	10915	5	0.00	0.00
5226	10915	6	0.00	0.00
5227	10916	4	0.00	0.00
5228	10916	2	0.00	0.00
5229	10916	5	0.00	0.00
5230	10916	6	0.00	0.00
5231	10917	4	0.00	0.00
5232	10917	2	0.00	0.00
5233	10917	5	0.00	0.00
5234	10917	6	0.00	0.00
5235	10918	4	0.00	0.00
5236	10918	2	0.00	0.00
5237	10918	5	0.00	0.00
5238	10918	6	0.00	0.00
5239	10919	4	0.00	0.00
5240	10919	2	0.00	0.00
5241	10919	5	0.00	0.00
5242	10919	6	0.00	0.00
5243	10920	4	0.00	0.00
5244	10920	2	0.00	0.00
5245	10920	5	0.00	0.00
5246	10920	6	0.00	0.00
5247	10921	4	0.00	0.00
5248	10921	2	0.00	0.00
5249	10921	5	0.00	0.00
5250	10921	6	0.00	0.00
5251	10922	4	0.00	0.00
5252	10922	2	0.00	0.00
5253	10922	5	0.00	0.00
5254	10922	6	0.00	0.00
5255	10923	4	0.00	0.00
5256	10923	2	0.00	0.00
5257	10923	5	0.00	0.00
5258	10923	6	0.00	0.00
5259	10924	4	0.00	0.00
5260	10924	2	0.00	0.00
5261	10924	5	0.00	0.00
5262	10924	6	0.00	0.00
5263	10925	4	0.00	0.00
5264	10925	2	0.00	0.00
5265	10925	5	0.00	0.00
5266	10925	6	0.00	0.00
5267	10926	4	0.00	0.00
5268	10926	2	0.00	0.00
5269	10926	5	0.00	0.00
5270	10926	6	0.00	0.00
5271	10927	4	0.00	0.00
5272	10927	2	0.00	0.00
5273	10927	5	0.00	0.00
5274	10927	6	0.00	0.00
5275	10928	4	0.00	0.00
5276	10928	2	0.00	0.00
5277	10928	5	0.00	0.00
5278	10928	6	0.00	0.00
5279	10929	4	0.00	0.00
5280	10929	2	0.00	0.00
5281	10929	5	0.00	0.00
5282	10929	6	0.00	0.00
5283	10930	4	0.00	0.00
5284	10930	2	0.00	0.00
5285	10930	5	0.00	0.00
5286	10930	6	0.00	0.00
5287	10931	4	0.00	0.00
5288	10931	2	0.00	0.00
5289	10931	5	0.00	0.00
5290	10931	6	0.00	0.00
5291	10932	4	0.00	0.00
5292	10932	2	0.00	0.00
5293	10932	5	0.00	0.00
5294	10932	6	0.00	0.00
5295	10933	4	0.00	0.00
5296	10933	2	0.00	0.00
5297	10933	5	0.00	0.00
5298	10933	6	0.00	0.00
5299	10934	4	0.00	0.00
5300	10934	2	0.00	0.00
5301	10934	5	0.00	0.00
5302	10934	6	0.00	0.00
5303	10935	4	0.00	0.00
5304	10935	2	0.00	0.00
5305	10935	5	0.00	0.00
5306	10935	6	0.00	0.00
5307	10936	4	0.00	0.00
5308	10936	2	0.00	0.00
5309	10936	5	0.00	0.00
5310	10936	6	0.00	0.00
5311	10937	4	0.00	0.00
5312	10937	2	0.00	0.00
5313	10937	5	0.00	0.00
5314	10937	6	0.00	0.00
5315	10938	4	0.00	0.00
5316	10938	2	0.00	0.00
5317	10938	5	0.00	0.00
5318	10938	6	0.00	0.00
5319	10939	4	0.00	0.00
5320	10939	2	0.00	0.00
5321	10939	5	0.00	0.00
5322	10939	6	0.00	0.00
5323	10940	4	0.00	0.00
5324	10940	2	0.00	0.00
5325	10940	5	0.00	0.00
5326	10940	6	0.00	0.00
5327	10941	4	0.00	0.00
5328	10941	2	0.00	0.00
5329	10941	5	0.00	0.00
5330	10941	6	0.00	0.00
5331	10942	4	0.00	0.00
5332	10942	2	0.00	0.00
5333	10942	5	0.00	0.00
5334	10942	6	0.00	0.00
5335	10943	4	0.00	0.00
5336	10943	2	0.00	0.00
5337	10943	5	0.00	0.00
5338	10943	6	0.00	0.00
\.


--
-- Data for Name: adocumental; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY adocumental (idadocumental, idasiento, descripcionadocumental, fechaintadocumental, fechaasadocumental, archivoadocumental) FROM stdin;
\.


--
-- Data for Name: ainteligente; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY ainteligente (idainteligente, descripcion, comentariosasiento) FROM stdin;
\.


--
-- Data for Name: amortizacion; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY amortizacion (idamortizacion, idcuentaactivo, idcuentaamortizacion, descamortizacion, nomamortizacion, fechacompra, fecha1cuota, valorcompra, periodicidad, numcuotas, metodo, nifproveedor, nomproveedor, dirproveedor, telproveedor, agrupacion) FROM stdin;
1	10633	10745	\N	una amortizacion	2007-09-10	2007-09-10	6000.00	\N	5	\N	\N	\N	\N	\N	becarios
\.


--
-- Data for Name: apunte; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY apunte (idapunte, codigoborrador, idasiento, iddiario, fecha, conceptocontable, idcuenta, descripcion, debe, haber, contrapartida, comentario, idcanal, marcaconciliacion, idc_coste, idtipoiva, orden, punteo) FROM stdin;
1963	\N	1109	\N	2007-01-10	Pago	10935	Pago BTURJT	151.73	0.00	\N	\N	\N	\N	\N	\N	\N	f
1964	\N	1109	\N	2007-01-10	Pago	10943	Pago BTURJT	0.00	151.73	\N	\N	\N	\N	\N	\N	\N	f
1965	\N	1110	\N	2007-01-26	Pago	10912	Pago VSHKXW	37.50	0.00	\N	\N	\N	\N	\N	\N	\N	f
1966	\N	1110	\N	2007-01-26	Pago	10943	Pago VSHKXW	0.00	37.50	\N	\N	\N	\N	\N	\N	\N	f
1967	\N	1111	\N	2007-01-26	Pago	10912	Pago UVHYFI	28.19	0.00	\N	\N	\N	\N	\N	\N	\N	f
1968	\N	1111	\N	2007-01-26	Pago	10943	Pago UVHYFI	0.00	28.19	\N	\N	\N	\N	\N	\N	\N	f
1969	\N	1112	\N	2007-02-01	Pago	10912	Pago POVHXI	42.74	0.00	\N	\N	\N	\N	\N	\N	\N	f
1970	\N	1112	\N	2007-02-01	Pago	10943	Pago POVHXI	0.00	42.74	\N	\N	\N	\N	\N	\N	\N	f
1971	\N	1113	\N	2007-01-11	Pago	10914	Pago YTGOLI	1180.47	0.00	\N	\N	\N	\N	\N	\N	\N	f
1972	\N	1113	\N	2007-01-11	Pago	10943	Pago YTGOLI	0.00	1180.47	\N	\N	\N	\N	\N	\N	\N	f
1973	\N	1114	\N	2007-01-18	Pago	10931	Pago OZDCTQ	250.04	0.00	\N	\N	\N	\N	\N	\N	\N	f
1974	\N	1114	\N	2007-01-18	Pago	10943	Pago OZDCTQ	0.00	250.04	\N	\N	\N	\N	\N	\N	\N	f
1975	\N	1115	\N	2007-02-22	Pago	10935	Pago XFHYRL	412.88	0.00	\N	\N	\N	\N	\N	\N	\N	f
1976	\N	1115	\N	2007-02-22	Pago	10943	Pago XFHYRL	0.00	412.88	\N	\N	\N	\N	\N	\N	\N	f
1977	\N	1116	\N	2007-02-09	Pago	10935	Pago FBWSQS	50.53	0.00	\N	\N	\N	\N	\N	\N	\N	f
1978	\N	1116	\N	2007-02-09	Pago	10943	Pago FBWSQS	0.00	50.53	\N	\N	\N	\N	\N	\N	\N	f
1979	\N	1117	\N	2007-03-14	Pago	10935	Pago TKGFJV	66.91	0.00	\N	\N	\N	\N	\N	\N	\N	f
1980	\N	1117	\N	2007-03-14	Pago	10943	Pago TKGFJV	0.00	66.91	\N	\N	\N	\N	\N	\N	\N	f
1981	\N	1118	\N	2007-03-09	Pago	10912	Pago ZHMULP	413.18	0.00	\N	\N	\N	\N	\N	\N	\N	f
1982	\N	1118	\N	2007-03-09	Pago	10943	Pago ZHMULP	0.00	413.18	\N	\N	\N	\N	\N	\N	\N	f
1983	\N	1119	\N	2007-03-09	Pago	10912	Pago DMJYLP	1488.64	0.00	\N	\N	\N	\N	\N	\N	\N	f
1984	\N	1119	\N	2007-03-09	Pago	10943	Pago DMJYLP	0.00	1488.64	\N	\N	\N	\N	\N	\N	\N	f
1985	\N	1120	\N	2007-03-02	Pago	10935	Pago UVRHKP	79.26	0.00	\N	\N	\N	\N	\N	\N	\N	f
1986	\N	1120	\N	2007-03-02	Pago	10943	Pago UVRHKP	0.00	79.26	\N	\N	\N	\N	\N	\N	\N	f
1987	\N	1121	\N	2007-02-21	Pago	10912	Pago TBSQGT	464.46	0.00	\N	\N	\N	\N	\N	\N	\N	f
1988	\N	1121	\N	2007-02-21	Pago	10943	Pago TBSQGT	0.00	464.46	\N	\N	\N	\N	\N	\N	\N	f
1989	\N	1122	\N	2007-02-22	Pago	10912	Pago XZINVQ	575.91	0.00	\N	\N	\N	\N	\N	\N	\N	f
1990	\N	1122	\N	2007-02-22	Pago	10943	Pago XZINVQ	0.00	575.91	\N	\N	\N	\N	\N	\N	\N	f
1991	\N	1123	\N	2007-02-15	Pago	10912	Pago MYCVWN	2558.88	0.00	\N	\N	\N	\N	\N	\N	\N	f
1992	\N	1123	\N	2007-02-15	Pago	10943	Pago MYCVWN	0.00	2558.88	\N	\N	\N	\N	\N	\N	\N	f
1993	\N	1124	\N	2007-02-21	Pago	10912	Pago LGTMEC	3.54	0.00	\N	\N	\N	\N	\N	\N	\N	f
1994	\N	1124	\N	2007-02-21	Pago	10943	Pago LGTMEC	0.00	3.54	\N	\N	\N	\N	\N	\N	\N	f
1995	\N	1125	\N	2007-02-17	Pago	10912	Pago GYXXFH	198.34	0.00	\N	\N	\N	\N	\N	\N	\N	f
1996	\N	1125	\N	2007-02-17	Pago	10943	Pago GYXXFH	0.00	198.34	\N	\N	\N	\N	\N	\N	\N	f
1997	\N	1126	\N	2007-03-02	Pago	10912	Pago MYIEOO	20.80	0.00	\N	\N	\N	\N	\N	\N	\N	f
1998	\N	1126	\N	2007-03-02	Pago	10943	Pago MYIEOO	0.00	20.80	\N	\N	\N	\N	\N	\N	\N	f
1999	\N	1127	\N	2007-02-08	Pago	10912	Pago XLNFYI	191.95	0.00	\N	\N	\N	\N	\N	\N	\N	f
2000	\N	1127	\N	2007-02-08	Pago	10943	Pago XLNFYI	0.00	191.95	\N	\N	\N	\N	\N	\N	\N	f
2001	\N	1128	\N	2007-02-02	Pago	10934	Pago VLGXGD	49.08	0.00	\N	\N	\N	\N	\N	\N	\N	f
2002	\N	1128	\N	2007-02-02	Pago	10943	Pago VLGXGD	0.00	49.08	\N	\N	\N	\N	\N	\N	\N	f
2003	\N	1129	\N	2007-01-09	Pago	10935	Pago WSNBXT	407.62	0.00	\N	\N	\N	\N	\N	\N	\N	f
2004	\N	1129	\N	2007-01-09	Pago	10943	Pago WSNBXT	0.00	407.62	\N	\N	\N	\N	\N	\N	\N	f
2005	\N	1130	\N	2007-02-06	Pago	10912	Pago KRICCN	80.92	0.00	\N	\N	\N	\N	\N	\N	\N	f
2006	\N	1130	\N	2007-02-06	Pago	10943	Pago KRICCN	0.00	80.92	\N	\N	\N	\N	\N	\N	\N	f
2007	\N	1131	\N	2007-01-23	Pago	10923	Pago EJLBGQ	135.27	0.00	\N	\N	\N	\N	\N	\N	\N	f
2008	\N	1131	\N	2007-01-23	Pago	10943	Pago EJLBGQ	0.00	135.27	\N	\N	\N	\N	\N	\N	\N	f
2009	\N	1132	\N	2007-03-03	Pago	10912	Pago ISOPVD	407.05	0.00	\N	\N	\N	\N	\N	\N	\N	f
2010	\N	1132	\N	2007-03-03	Pago	10943	Pago ISOPVD	0.00	407.05	\N	\N	\N	\N	\N	\N	\N	f
2011	\N	1133	\N	2007-02-23	Pago	10923	Pago MKMQTS	156.76	0.00	\N	\N	\N	\N	\N	\N	\N	f
2012	\N	1133	\N	2007-02-23	Pago	10943	Pago MKMQTS	0.00	156.76	\N	\N	\N	\N	\N	\N	\N	f
2013	\N	1134	\N	2007-03-21	Pago	10912	Pago VIPUQY	296.14	0.00	\N	\N	\N	\N	\N	\N	\N	f
2014	\N	1134	\N	2007-03-21	Pago	10943	Pago VIPUQY	0.00	296.14	\N	\N	\N	\N	\N	\N	\N	f
2015	\N	1135	\N	2007-03-20	Pago	10935	Pago QSCZIJ	154.74	0.00	\N	\N	\N	\N	\N	\N	\N	f
2016	\N	1135	\N	2007-03-20	Pago	10943	Pago QSCZIJ	0.00	154.74	\N	\N	\N	\N	\N	\N	\N	f
2017	\N	1136	\N	2007-03-28	Pago	10912	Pago PUFJNS	36.21	0.00	\N	\N	\N	\N	\N	\N	\N	f
2018	\N	1136	\N	2007-03-28	Pago	10943	Pago PUFJNS	0.00	36.21	\N	\N	\N	\N	\N	\N	\N	f
2019	\N	1137	\N	2007-03-30	Pago	10912	Pago XRQMGX	214.30	0.00	\N	\N	\N	\N	\N	\N	\N	f
2020	\N	1137	\N	2007-03-30	Pago	10943	Pago XRQMGX	0.00	214.30	\N	\N	\N	\N	\N	\N	\N	f
2021	\N	1138	\N	2007-03-19	Pago	10933	Pago OVMUKG	606.00	0.00	\N	\N	\N	\N	\N	\N	\N	f
2022	\N	1138	\N	2007-03-19	Pago	10943	Pago OVMUKG	0.00	606.00	\N	\N	\N	\N	\N	\N	\N	f
2023	\N	1139	\N	2007-05-17	Pago	10912	Pago [WCFQE	220.98	0.00	\N	\N	\N	\N	\N	\N	\N	f
2024	\N	1139	\N	2007-05-17	Pago	10943	Pago [WCFQE	0.00	220.98	\N	\N	\N	\N	\N	\N	\N	f
2025	\N	1140	\N	2007-01-27	Pago	10935	Pago WSNBXT	332.69	0.00	\N	\N	\N	\N	\N	\N	\N	f
2026	\N	1140	\N	2007-01-27	Pago	10936	Pago WSNBXT	0.00	332.69	\N	\N	\N	\N	\N	\N	\N	f
2027	\N	1141	\N	2007-06-06	Pago	10935	Pago TXBSBZ	132.28	0.00	\N	\N	\N	\N	\N	\N	\N	f
2028	\N	1141	\N	2007-06-06	Pago	10936	Pago TXBSBZ	0.00	132.28	\N	\N	\N	\N	\N	\N	\N	f
2029	\N	1142	\N	2007-05-14	Pago	10935	Pago BYFKIO	363.08	0.00	\N	\N	\N	\N	\N	\N	\N	f
2030	\N	1142	\N	2007-05-14	Pago	10936	Pago BYFKIO	0.00	363.08	\N	\N	\N	\N	\N	\N	\N	f
2031	\N	1143	\N	2007-05-23	Pago	10935	Pago YX[YNK	187.12	0.00	\N	\N	\N	\N	\N	\N	\N	f
2032	\N	1143	\N	2007-05-23	Pago	10936	Pago YX[YNK	0.00	187.12	\N	\N	\N	\N	\N	\N	\N	f
2033	\N	1144	\N	2007-02-12	Pago	10935	Pago TKSLPF	317.45	0.00	\N	\N	\N	\N	\N	\N	\N	f
2034	\N	1144	\N	2007-02-12	Pago	10936	Pago TKSLPF	0.00	317.45	\N	\N	\N	\N	\N	\N	\N	f
2035	\N	1145	\N	2007-05-25	Pago	10935	Pago UXVMXB	616.17	0.00	\N	\N	\N	\N	\N	\N	\N	f
2036	\N	1145	\N	2007-05-25	Pago	10936	Pago UXVMXB	0.00	616.17	\N	\N	\N	\N	\N	\N	\N	f
2037	\N	1146	\N	2007-06-07	Pago	10935	Pago UQYVIZ	72.89	0.00	\N	\N	\N	\N	\N	\N	\N	f
2038	\N	1146	\N	2007-06-07	Pago	10936	Pago UQYVIZ	0.00	72.89	\N	\N	\N	\N	\N	\N	\N	f
2039	\N	1147	\N	2007-05-04	Pago	10935	Pago VJXZTF	162.11	0.00	\N	\N	\N	\N	\N	\N	\N	f
2040	\N	1147	\N	2007-05-04	Pago	10936	Pago VJXZTF	0.00	162.11	\N	\N	\N	\N	\N	\N	\N	f
2041	\N	1148	\N	2007-05-04	Pago	10935	Pago ORDNOQ	316.16	0.00	\N	\N	\N	\N	\N	\N	\N	f
2042	\N	1148	\N	2007-05-04	Pago	10936	Pago ORDNOQ	0.00	316.16	\N	\N	\N	\N	\N	\N	\N	f
2043	\N	1149	\N	2007-05-21	Pago	10935	Pago YXQVED	66.91	0.00	\N	\N	\N	\N	\N	\N	\N	f
2044	\N	1149	\N	2007-05-21	Pago	10936	Pago YXQVED	0.00	66.91	\N	\N	\N	\N	\N	\N	\N	f
2045	\N	1150	\N	2007-05-21	Pago	10935	Pago OYAIKY	66.91	0.00	\N	\N	\N	\N	\N	\N	\N	f
2046	\N	1150	\N	2007-05-21	Pago	10936	Pago OYAIKY	0.00	66.91	\N	\N	\N	\N	\N	\N	\N	f
2047	\N	1151	\N	2007-06-06	Pago	10926	Pago JEOHZW	92.94	0.00	\N	\N	\N	\N	\N	\N	\N	f
2048	\N	1151	\N	2007-06-06	Pago	10936	Pago JEOHZW	0.00	92.94	\N	\N	\N	\N	\N	\N	\N	f
2049	\N	1152	\N	2007-08-13	Pago	10918	Pago 111	1500.00	0.00	\N	\N	\N	\N	\N	\N	\N	f
2050	\N	1152	\N	2007-08-13	Pago	10943	Pago 111	0.00	1500.00	\N	\N	\N	\N	\N	\N	\N	f
2051	\N	1153	\N	2007-08-14	Pago	10912	Pago UMQIBH	122.00	0.00	\N	\N	\N	\N	\N	\N	\N	f
2052	\N	1153	\N	2007-08-14	Pago	10943	Pago UMQIBH	0.00	122.00	\N	\N	\N	\N	\N	\N	\N	f
2053	\N	1154	\N	2007-08-31	Pago	10931	Pago SBDNFG	456.00	0.00	\N	\N	\N	\N	\N	\N	\N	f
2054	\N	1154	\N	2007-08-31	Pago	10938	Pago SBDNFG	0.00	456.00	\N	\N	\N	\N	\N	\N	\N	f
2055	\N	1281	\N	2007-01-18	Factura Proveedor	10308	Factura Proveedor Num. 004	34.49	0.00	\N	\N	\N	\N	\N	\N	\N	f
2056	\N	1281	\N	2007-01-18	Factura Proveedor	10931	Factura Proveedor Num. 004	0.00	250.04	\N	\N	\N	\N	\N	\N	\N	f
2057	\N	1281	\N	2007-01-18	Factura Proveedor	10414	Factura Proveedor Num. 004	215.55	0.00	\N	\N	\N	\N	\N	\N	\N	f
2058	\N	1282	\N	2007-01-16	Factura Proveedor	10308	Factura Proveedor Num. 075-7872324686	7.53	0.00	\N	\N	\N	\N	\N	\N	\N	f
2059	\N	1282	\N	2007-01-16	Factura Proveedor	10932	Factura Proveedor Num. 075-7872324686	0.00	54.59	\N	\N	\N	\N	\N	\N	\N	f
2060	\N	1282	\N	2007-01-16	Factura Proveedor	10414	Factura Proveedor Num. 075-7872324686	47.06	0.00	\N	\N	\N	\N	\N	\N	\N	f
2061	\N	1283	\N	2007-02-07	Factura Proveedor	10308	Factura Proveedor Num. 07/00475	56.95	0.00	\N	\N	\N	\N	\N	\N	\N	f
2062	\N	1283	\N	2007-02-07	Factura Proveedor	10935	Factura Proveedor Num. 07/00475	0.00	412.88	\N	\N	\N	\N	\N	\N	\N	f
2063	\N	1283	\N	2007-02-07	Factura Proveedor	10414	Factura Proveedor Num. 07/00475	355.93	0.00	\N	\N	\N	\N	\N	\N	\N	f
2064	\N	1284	\N	2007-01-11	Factura Proveedor	10308	Factura Proveedor Num. 99959	5.17	0.00	\N	\N	\N	\N	\N	\N	\N	f
2065	\N	1284	\N	2007-01-11	Factura Proveedor	10912	Factura Proveedor Num. 99959	0.00	37.50	\N	\N	\N	\N	\N	\N	\N	f
2066	\N	1284	\N	2007-01-11	Factura Proveedor	10414	Factura Proveedor Num. 99959	32.33	0.00	\N	\N	\N	\N	\N	\N	\N	f
2067	\N	1285	\N	2007-02-27	Factura Proveedor	10308	Factura Proveedor Num. 00717	9.23	0.00	\N	\N	\N	\N	\N	\N	\N	f
2068	\N	1285	\N	2007-02-27	Factura Proveedor	10935	Factura Proveedor Num. 00717	0.00	66.91	\N	\N	\N	\N	\N	\N	\N	f
2069	\N	1285	\N	2007-02-27	Factura Proveedor	10414	Factura Proveedor Num. 00717	57.68	0.00	\N	\N	\N	\N	\N	\N	\N	f
2070	\N	1286	\N	2007-02-22	Factura Proveedor	10308	Factura Proveedor Num. 102266	205.41	0.00	\N	\N	\N	\N	\N	\N	\N	f
2071	\N	1286	\N	2007-02-22	Factura Proveedor	10912	Factura Proveedor Num. 102266	0.00	1489.25	\N	\N	\N	\N	\N	\N	\N	f
2072	\N	1286	\N	2007-02-22	Factura Proveedor	10414	Factura Proveedor Num. 102266	1283.84	0.00	\N	\N	\N	\N	\N	\N	\N	f
2073	\N	1287	\N	2007-02-15	Factura Proveedor	10308	Factura Proveedor Num. 00583	10.93	0.00	\N	\N	\N	\N	\N	\N	\N	f
2074	\N	1287	\N	2007-02-15	Factura Proveedor	10935	Factura Proveedor Num. 00583	0.00	79.26	\N	\N	\N	\N	\N	\N	\N	f
2075	\N	1287	\N	2007-02-15	Factura Proveedor	10414	Factura Proveedor Num. 00583	68.33	0.00	\N	\N	\N	\N	\N	\N	\N	f
2076	\N	1288	\N	2007-02-06	Factura Proveedor	10308	Factura Proveedor Num. 101360	64.09	0.00	\N	\N	\N	\N	\N	\N	\N	f
2077	\N	1288	\N	2007-02-06	Factura Proveedor	10912	Factura Proveedor Num. 101360	0.00	464.65	\N	\N	\N	\N	\N	\N	\N	f
2078	\N	1288	\N	2007-02-06	Factura Proveedor	10414	Factura Proveedor Num. 101360	400.56	0.00	\N	\N	\N	\N	\N	\N	\N	f
2079	\N	1289	\N	2007-02-07	Factura Proveedor	10308	Factura Proveedor Num. 101483	79.47	0.00	\N	\N	\N	\N	\N	\N	\N	f
2080	\N	1289	\N	2007-02-07	Factura Proveedor	10912	Factura Proveedor Num. 101483	0.00	576.15	\N	\N	\N	\N	\N	\N	\N	f
2081	\N	1289	\N	2007-02-07	Factura Proveedor	10414	Factura Proveedor Num. 101483	496.68	0.00	\N	\N	\N	\N	\N	\N	\N	f
2082	\N	1290	\N	2007-01-31	Factura Proveedor	10308	Factura Proveedor Num. 101049	353.09	0.00	\N	\N	\N	\N	\N	\N	\N	f
2083	\N	1290	\N	2007-01-31	Factura Proveedor	10912	Factura Proveedor Num. 101049	0.00	2559.93	\N	\N	\N	\N	\N	\N	\N	f
2084	\N	1290	\N	2007-01-31	Factura Proveedor	10414	Factura Proveedor Num. 101049	2206.84	0.00	\N	\N	\N	\N	\N	\N	\N	f
2085	\N	1291	\N	2007-02-06	Factura Proveedor	10308	Factura Proveedor Num. 101359	0.49	0.00	\N	\N	\N	\N	\N	\N	\N	f
2086	\N	1291	\N	2007-02-06	Factura Proveedor	10912	Factura Proveedor Num. 101359	0.00	3.54	\N	\N	\N	\N	\N	\N	\N	f
2087	\N	1291	\N	2007-02-06	Factura Proveedor	10414	Factura Proveedor Num. 101359	3.05	0.00	\N	\N	\N	\N	\N	\N	\N	f
2088	\N	1292	\N	2007-02-02	Factura Proveedor	10308	Factura Proveedor Num. GIB	27.37	0.00	\N	\N	\N	\N	\N	\N	\N	f
2089	\N	1292	\N	2007-02-02	Factura Proveedor	10912	Factura Proveedor Num. GIB	0.00	198.42	\N	\N	\N	\N	\N	\N	\N	f
2090	\N	1292	\N	2007-02-02	Factura Proveedor	10414	Factura Proveedor Num. GIB	171.05	0.00	\N	\N	\N	\N	\N	\N	\N	f
2091	\N	1293	\N	2007-02-15	Factura Proveedor	10308	Factura Proveedor Num. 101892	2.87	0.00	\N	\N	\N	\N	\N	\N	\N	f
2092	\N	1293	\N	2007-02-15	Factura Proveedor	10912	Factura Proveedor Num. 101892	0.00	20.81	\N	\N	\N	\N	\N	\N	\N	f
2093	\N	1293	\N	2007-02-15	Factura Proveedor	10414	Factura Proveedor Num. 101892	17.94	0.00	\N	\N	\N	\N	\N	\N	\N	f
2094	\N	1294	\N	2007-01-24	Factura Proveedor	10308	Factura Proveedor Num. 100689	26.49	0.00	\N	\N	\N	\N	\N	\N	\N	f
2095	\N	1294	\N	2007-01-24	Factura Proveedor	10912	Factura Proveedor Num. 100689	0.00	192.03	\N	\N	\N	\N	\N	\N	\N	f
2096	\N	1294	\N	2007-01-24	Factura Proveedor	10414	Factura Proveedor Num. 100689	165.54	0.00	\N	\N	\N	\N	\N	\N	\N	f
2097	\N	1295	\N	2007-02-02	Factura Proveedor	10308	Factura Proveedor Num. A40307549	6.77	0.00	\N	\N	\N	\N	\N	\N	\N	f
2098	\N	1295	\N	2007-02-02	Factura Proveedor	10934	Factura Proveedor Num. A40307549	0.00	49.08	\N	\N	\N	\N	\N	\N	\N	f
2099	\N	1295	\N	2007-02-02	Factura Proveedor	10414	Factura Proveedor Num. A40307549	42.31	0.00	\N	\N	\N	\N	\N	\N	\N	f
2100	\N	1296	\N	2007-01-08	Factura Proveedor	10308	Factura Proveedor Num. 0710	27.71	0.00	\N	\N	\N	\N	\N	\N	\N	f
2101	\N	1296	\N	2007-01-08	Factura Proveedor	10935	Factura Proveedor Num. 0710	0.00	200.91	\N	\N	\N	\N	\N	\N	\N	f
2102	\N	1296	\N	2007-01-08	Factura Proveedor	10414	Factura Proveedor Num. 0710	173.20	0.00	\N	\N	\N	\N	\N	\N	\N	f
2103	\N	1297	\N	2007-01-09	Factura Proveedor	10308	Factura Proveedor Num. 0726	56.22	0.00	\N	\N	\N	\N	\N	\N	\N	f
2104	\N	1297	\N	2007-01-09	Factura Proveedor	10935	Factura Proveedor Num. 0726	0.00	407.62	\N	\N	\N	\N	\N	\N	\N	f
2105	\N	1297	\N	2007-01-09	Factura Proveedor	10414	Factura Proveedor Num. 0726	351.40	0.00	\N	\N	\N	\N	\N	\N	\N	f
2106	\N	1298	\N	2007-01-22	Factura Proveedor	10308	Factura Proveedor Num. 100560	11.17	0.00	\N	\N	\N	\N	\N	\N	\N	f
2107	\N	1298	\N	2007-01-22	Factura Proveedor	10912	Factura Proveedor Num. 100560	0.00	80.96	\N	\N	\N	\N	\N	\N	\N	f
2108	\N	1298	\N	2007-01-22	Factura Proveedor	10414	Factura Proveedor Num. 100560	69.79	0.00	\N	\N	\N	\N	\N	\N	\N	f
2109	\N	1299	\N	2007-01-23	Factura Proveedor	10308	Factura Proveedor Num. 321393	18.66	0.00	\N	\N	\N	\N	\N	\N	\N	f
2110	\N	1299	\N	2007-01-23	Factura Proveedor	10923	Factura Proveedor Num. 321393	0.00	135.27	\N	\N	\N	\N	\N	\N	\N	f
2111	\N	1299	\N	2007-01-23	Factura Proveedor	10414	Factura Proveedor Num. 321393	116.61	0.00	\N	\N	\N	\N	\N	\N	\N	f
2112	\N	1300	\N	2007-02-16	Factura Proveedor	10308	Factura Proveedor Num. 102029	56.17	0.00	\N	\N	\N	\N	\N	\N	\N	f
2113	\N	1300	\N	2007-02-16	Factura Proveedor	10912	Factura Proveedor Num. 102029	0.00	407.22	\N	\N	\N	\N	\N	\N	\N	f
2114	\N	1300	\N	2007-02-16	Factura Proveedor	10414	Factura Proveedor Num. 102029	351.05	0.00	\N	\N	\N	\N	\N	\N	\N	f
2115	\N	1301	\N	2007-02-23	Factura Proveedor	10308	Factura Proveedor Num. 778021	21.62	0.00	\N	\N	\N	\N	\N	\N	\N	f
2116	\N	1301	\N	2007-02-23	Factura Proveedor	10923	Factura Proveedor Num. 778021	0.00	156.76	\N	\N	\N	\N	\N	\N	\N	f
2117	\N	1301	\N	2007-02-23	Factura Proveedor	10414	Factura Proveedor Num. 778021	135.14	0.00	\N	\N	\N	\N	\N	\N	\N	f
2118	\N	1302	\N	2007-03-06	Factura Proveedor	10308	Factura Proveedor Num. 102870	40.86	0.00	\N	\N	\N	\N	\N	\N	\N	f
2119	\N	1302	\N	2007-03-06	Factura Proveedor	10912	Factura Proveedor Num. 102870	0.00	296.26	\N	\N	\N	\N	\N	\N	\N	f
2120	\N	1302	\N	2007-03-06	Factura Proveedor	10414	Factura Proveedor Num. 102870	255.40	0.00	\N	\N	\N	\N	\N	\N	\N	f
2121	\N	1303	\N	2007-03-06	Factura Proveedor	10308	Factura Proveedor Num. 769	21.34	0.00	\N	\N	\N	\N	\N	\N	\N	f
2122	\N	1303	\N	2007-03-06	Factura Proveedor	10935	Factura Proveedor Num. 769	0.00	154.74	\N	\N	\N	\N	\N	\N	\N	f
2123	\N	1303	\N	2007-03-06	Factura Proveedor	10414	Factura Proveedor Num. 769	133.40	0.00	\N	\N	\N	\N	\N	\N	\N	f
2124	\N	1304	\N	2007-03-13	Factura Proveedor	10308	Factura Proveedor Num. 103279	5.00	0.00	\N	\N	\N	\N	\N	\N	\N	f
2125	\N	1304	\N	2007-03-13	Factura Proveedor	10912	Factura Proveedor Num. 103279	0.00	36.23	\N	\N	\N	\N	\N	\N	\N	f
2126	\N	1304	\N	2007-03-13	Factura Proveedor	10414	Factura Proveedor Num. 103279	31.23	0.00	\N	\N	\N	\N	\N	\N	\N	f
2127	\N	1305	\N	2007-02-02	Factura Proveedor	10308	Factura Proveedor Num. 2007001	96.00	0.00	\N	\N	\N	\N	\N	\N	\N	f
2128	\N	1305	\N	2007-02-02	Factura Proveedor	10933	Factura Proveedor Num. 2007001	0.00	606.00	\N	\N	\N	\N	\N	\N	\N	f
2129	\N	1305	\N	2007-02-02	Factura Proveedor	10414	Factura Proveedor Num. 2007001	600.00	0.00	\N	\N	\N	\N	\N	\N	\N	f
2130	\N	1306	\N	2007-01-11	Factura Proveedor	10308	Factura Proveedor Num. 008922	162.82	0.00	\N	\N	\N	\N	\N	\N	\N	f
2131	\N	1306	\N	2007-01-11	Factura Proveedor	10914	Factura Proveedor Num. 008922	0.00	1180.47	\N	\N	\N	\N	\N	\N	\N	f
2132	\N	1306	\N	2007-01-11	Factura Proveedor	10414	Factura Proveedor Num. 008922	1017.65	0.00	\N	\N	\N	\N	\N	\N	\N	f
2133	\N	1307	\N	2007-04-27	Factura Proveedor	10308	Factura Proveedor Num. 07/01394	50.08	0.00	\N	\N	\N	\N	\N	\N	\N	f
2134	\N	1307	\N	2007-04-27	Factura Proveedor	10935	Factura Proveedor Num. 07/01394	0.00	363.08	\N	\N	\N	\N	\N	\N	\N	f
2135	\N	1307	\N	2007-04-27	Factura Proveedor	10414	Factura Proveedor Num. 07/01394	313.00	0.00	\N	\N	\N	\N	\N	\N	\N	f
2136	\N	1308	\N	2007-01-12	Factura Proveedor	10308	Factura Proveedor Num. 07/00111	43.79	0.00	\N	\N	\N	\N	\N	\N	\N	f
2137	\N	1308	\N	2007-01-12	Factura Proveedor	10935	Factura Proveedor Num. 07/00111	0.00	317.45	\N	\N	\N	\N	\N	\N	\N	f
2138	\N	1308	\N	2007-01-12	Factura Proveedor	10414	Factura Proveedor Num. 07/00111	273.66	0.00	\N	\N	\N	\N	\N	\N	\N	f
2139	\N	1309	\N	2007-05-22	Factura Proveedor	10308	Factura Proveedor Num. 07/0173	18.24	0.00	\N	\N	\N	\N	\N	\N	\N	f
2140	\N	1309	\N	2007-05-22	Factura Proveedor	10935	Factura Proveedor Num. 07/0173	0.00	132.27	\N	\N	\N	\N	\N	\N	\N	f
2141	\N	1309	\N	2007-05-22	Factura Proveedor	10414	Factura Proveedor Num. 07/0173	114.03	0.00	\N	\N	\N	\N	\N	\N	\N	f
2142	\N	1310	\N	2007-05-10	Factura Proveedor	10308	Factura Proveedor Num. 01567	84.99	0.00	\N	\N	\N	\N	\N	\N	\N	f
2143	\N	1310	\N	2007-05-10	Factura Proveedor	10935	Factura Proveedor Num. 01567	0.00	616.17	\N	\N	\N	\N	\N	\N	\N	f
2144	\N	1310	\N	2007-05-10	Factura Proveedor	10414	Factura Proveedor Num. 01567	531.18	0.00	\N	\N	\N	\N	\N	\N	\N	f
2145	\N	1311	\N	2007-05-23	Factura Proveedor	10308	Factura Proveedor Num. 1719	10.05	0.00	\N	\N	\N	\N	\N	\N	\N	f
2146	\N	1311	\N	2007-05-23	Factura Proveedor	10935	Factura Proveedor Num. 1719	0.00	72.89	\N	\N	\N	\N	\N	\N	\N	f
2147	\N	1311	\N	2007-05-23	Factura Proveedor	10414	Factura Proveedor Num. 1719	62.84	0.00	\N	\N	\N	\N	\N	\N	\N	f
2148	\N	1312	\N	2007-01-17	Factura Proveedor	10308	Factura Proveedor Num. 100270	5.90	0.00	\N	\N	\N	\N	\N	\N	\N	f
2149	\N	1312	\N	2007-01-17	Factura Proveedor	10912	Factura Proveedor Num. 100270	0.00	42.76	\N	\N	\N	\N	\N	\N	\N	f
2150	\N	1312	\N	2007-01-17	Factura Proveedor	10414	Factura Proveedor Num. 100270	36.86	0.00	\N	\N	\N	\N	\N	\N	\N	f
2151	\N	1313	\N	2007-04-10	Factura Proveedor	10308	Factura Proveedor Num. 1187	22.36	0.00	\N	\N	\N	\N	\N	\N	\N	f
2152	\N	1313	\N	2007-04-10	Factura Proveedor	10935	Factura Proveedor Num. 1187	0.00	162.11	\N	\N	\N	\N	\N	\N	\N	f
2153	\N	1313	\N	2007-04-10	Factura Proveedor	10414	Factura Proveedor Num. 1187	139.75	0.00	\N	\N	\N	\N	\N	\N	\N	f
2154	\N	1314	\N	2007-04-21	Factura Proveedor	10308	Factura Proveedor Num. 1317	43.61	0.00	\N	\N	\N	\N	\N	\N	\N	f
2155	\N	1314	\N	2007-04-21	Factura Proveedor	10935	Factura Proveedor Num. 1317	0.00	316.16	\N	\N	\N	\N	\N	\N	\N	f
2156	\N	1314	\N	2007-04-21	Factura Proveedor	10414	Factura Proveedor Num. 1317	272.55	0.00	\N	\N	\N	\N	\N	\N	\N	f
2157	\N	1315	\N	2007-05-02	Factura Proveedor	10308	Factura Proveedor Num. 1448	9.23	0.00	\N	\N	\N	\N	\N	\N	\N	f
2158	\N	1315	\N	2007-05-02	Factura Proveedor	10935	Factura Proveedor Num. 1448	0.00	66.91	\N	\N	\N	\N	\N	\N	\N	f
2159	\N	1315	\N	2007-05-02	Factura Proveedor	10414	Factura Proveedor Num. 1448	57.68	0.00	\N	\N	\N	\N	\N	\N	\N	f
2160	\N	1316	\N	2007-05-22	Factura Proveedor	10308	Factura Proveedor Num. 07/01630	12.82	0.00	\N	\N	\N	\N	\N	\N	\N	f
2161	\N	1316	\N	2007-05-22	Factura Proveedor	10926	Factura Proveedor Num. 07/01630	0.00	92.94	\N	\N	\N	\N	\N	\N	\N	f
2162	\N	1316	\N	2007-05-22	Factura Proveedor	10414	Factura Proveedor Num. 07/01630	80.12	0.00	\N	\N	\N	\N	\N	\N	\N	f
2163	\N	1317	\N	2007-05-02	Factura Proveedor	10308	Factura Proveedor Num. 105920	30.48	0.00	\N	\N	\N	\N	\N	\N	\N	f
2164	\N	1317	\N	2007-05-02	Factura Proveedor	10912	Factura Proveedor Num. 105920	0.00	220.98	\N	\N	\N	\N	\N	\N	\N	f
2165	\N	1317	\N	2007-05-02	Factura Proveedor	10414	Factura Proveedor Num. 105920	190.50	0.00	\N	\N	\N	\N	\N	\N	\N	f
2166	\N	1318	\N	2007-04-30	Factura Proveedor	10308	Factura Proveedor Num. 07/01430	25.81	0.00	\N	\N	\N	\N	\N	\N	\N	f
2167	\N	1318	\N	2007-04-30	Factura Proveedor	10935	Factura Proveedor Num. 07/01430	0.00	187.12	\N	\N	\N	\N	\N	\N	\N	f
2168	\N	1318	\N	2007-04-30	Factura Proveedor	10414	Factura Proveedor Num. 07/01430	161.31	0.00	\N	\N	\N	\N	\N	\N	\N	f
2169	\N	1319	\N	2007-02-22	Factura Proveedor	10308	Factura Proveedor Num. 102265	57.01	0.00	\N	\N	\N	\N	\N	\N	\N	f
2170	\N	1319	\N	2007-02-22	Factura Proveedor	10912	Factura Proveedor Num. 102265	0.00	413.35	\N	\N	\N	\N	\N	\N	\N	f
2171	\N	1319	\N	2007-02-22	Factura Proveedor	10414	Factura Proveedor Num. 102265	356.34	0.00	\N	\N	\N	\N	\N	\N	\N	f
2172	\N	1320	\N	2007-03-15	Factura Proveedor	10308	Factura Proveedor Num. 103405	29.56	0.00	\N	\N	\N	\N	\N	\N	\N	f
2173	\N	1320	\N	2007-03-15	Factura Proveedor	10912	Factura Proveedor Num. 103405	0.00	214.28	\N	\N	\N	\N	\N	\N	\N	f
2174	\N	1320	\N	2007-03-15	Factura Proveedor	10414	Factura Proveedor Num. 103405	184.72	0.00	\N	\N	\N	\N	\N	\N	\N	f
2553	\N	1007	\N	2007-01-17	Cobro	10867	Cobro VBDXNN	0.00	4135.40	\N	\N	\N	\N	\N	\N	\N	f
2554	\N	1007	\N	2007-01-17	Cobro	10943	Cobro VBDXNN	4135.40	0.00	\N	\N	\N	\N	\N	\N	\N	f
2555	\N	1008	\N	2007-01-15	Cobro	10877	Cobro GQGKTW	0.00	1908.86	\N	\N	\N	\N	\N	\N	\N	f
2556	\N	1008	\N	2007-01-15	Cobro	10943	Cobro GQGKTW	1908.86	0.00	\N	\N	\N	\N	\N	\N	\N	f
2557	\N	1009	\N	2007-02-05	Cobro	10806	Cobro OARTCQ	0.00	2796.06	\N	\N	\N	\N	\N	\N	\N	f
2558	\N	1009	\N	2007-02-05	Cobro	10943	Cobro OARTCQ	2796.06	0.00	\N	\N	\N	\N	\N	\N	\N	f
2559	\N	1010	\N	2007-02-05	Cobro	10881	Cobro KPPHUK	0.00	1402.08	\N	\N	\N	\N	\N	\N	\N	f
2560	\N	1010	\N	2007-02-05	Cobro	10943	Cobro KPPHUK	1402.08	0.00	\N	\N	\N	\N	\N	\N	\N	f
2561	\N	1011	\N	2007-02-05	Cobro	10870	Cobro SLRASN	0.00	971.15	\N	\N	\N	\N	\N	\N	\N	f
2562	\N	1011	\N	2007-02-05	Cobro	10943	Cobro SLRASN	971.15	0.00	\N	\N	\N	\N	\N	\N	\N	f
2563	\N	1012	\N	2007-02-05	Cobro	10865	Cobro BPRRQJ	0.00	213.44	\N	\N	\N	\N	\N	\N	\N	f
2564	\N	1012	\N	2007-02-05	Cobro	10943	Cobro BPRRQJ	213.44	0.00	\N	\N	\N	\N	\N	\N	\N	f
2565	\N	1013	\N	2007-02-05	Cobro	10881	Cobro XLTDFR	0.00	1760.88	\N	\N	\N	\N	\N	\N	\N	f
2566	\N	1013	\N	2007-02-05	Cobro	10943	Cobro XLTDFR	1760.88	0.00	\N	\N	\N	\N	\N	\N	\N	f
2567	\N	1014	\N	2007-01-15	Cobro	10877	Cobro GQGKTW	0.00	1908.86	\N	\N	\N	\N	\N	\N	\N	f
2568	\N	1014	\N	2007-01-15	Cobro	10943	Cobro GQGKTW	1908.86	0.00	\N	\N	\N	\N	\N	\N	\N	f
2569	\N	1015	\N	2007-02-12	Cobro	10807	Cobro RKSCOE	0.00	6898.53	\N	\N	\N	\N	\N	\N	\N	f
2570	\N	1015	\N	2007-02-12	Cobro	10943	Cobro RKSCOE	6898.53	0.00	\N	\N	\N	\N	\N	\N	\N	f
2571	\N	1016	\N	2007-02-12	Cobro	10807	Cobro YKQTLE	0.00	1067.20	\N	\N	\N	\N	\N	\N	\N	f
2572	\N	1016	\N	2007-02-12	Cobro	10943	Cobro YKQTLE	1067.20	0.00	\N	\N	\N	\N	\N	\N	\N	f
2573	\N	1017	\N	2007-01-31	Cobro	10896	Cobro XWEVQD	0.00	143.75	\N	\N	\N	\N	\N	\N	\N	f
2574	\N	1017	\N	2007-01-31	Cobro	10943	Cobro XWEVQD	143.75	0.00	\N	\N	\N	\N	\N	\N	\N	f
2575	\N	1018	\N	2006-10-27	Cobro	10798	Cobro FJFSVG	0.00	2925.60	\N	\N	\N	\N	\N	\N	\N	f
2576	\N	1018	\N	2006-10-27	Cobro	10943	Cobro FJFSVG	2925.60	0.00	\N	\N	\N	\N	\N	\N	\N	f
2577	\N	1019	\N	2007-02-22	Cobro	10798	Cobro FJFSVG	0.00	8774.04	\N	\N	\N	\N	\N	\N	\N	f
2578	\N	1019	\N	2007-02-22	Cobro	10943	Cobro FJFSVG	8774.04	0.00	\N	\N	\N	\N	\N	\N	\N	f
2579	\N	1020	\N	2007-01-29	Cobro	10877	Cobro YZOBMY	0.00	173.60	\N	\N	\N	\N	\N	\N	\N	f
2580	\N	1020	\N	2007-01-29	Cobro	10943	Cobro YZOBMY	173.60	0.00	\N	\N	\N	\N	\N	\N	\N	f
2581	\N	1021	\N	2007-02-23	Cobro	10877	Cobro LDBTEN	0.00	1322.68	\N	\N	\N	\N	\N	\N	\N	f
2582	\N	1021	\N	2007-02-23	Cobro	10943	Cobro LDBTEN	1322.68	0.00	\N	\N	\N	\N	\N	\N	\N	f
2583	\N	1022	\N	2007-02-23	Cobro	10877	Cobro TJEQQB	0.00	2524.80	\N	\N	\N	\N	\N	\N	\N	f
2584	\N	1022	\N	2007-02-23	Cobro	10943	Cobro TJEQQB	2524.80	0.00	\N	\N	\N	\N	\N	\N	\N	f
2585	\N	1023	\N	2007-02-23	Cobro	10877	Cobro PTHRMX	0.00	334.28	\N	\N	\N	\N	\N	\N	\N	f
2586	\N	1023	\N	2007-02-23	Cobro	10943	Cobro PTHRMX	334.28	0.00	\N	\N	\N	\N	\N	\N	\N	f
2587	\N	1024	\N	2007-02-27	Cobro	10820	Cobro KBUUFV	0.00	2320.29	\N	\N	\N	\N	\N	\N	\N	f
2588	\N	1024	\N	2007-02-27	Cobro	10943	Cobro KBUUFV	2320.29	0.00	\N	\N	\N	\N	\N	\N	\N	f
2589	\N	1025	\N	2007-02-27	Cobro	10820	Cobro R[WFFL	0.00	6147.07	\N	\N	\N	\N	\N	\N	\N	f
2590	\N	1025	\N	2007-02-27	Cobro	10943	Cobro R[WFFL	6147.07	0.00	\N	\N	\N	\N	\N	\N	\N	f
2591	\N	1026	\N	2007-02-27	Cobro	10820	Cobro MGKWYW	0.00	3073.54	\N	\N	\N	\N	\N	\N	\N	f
2592	\N	1026	\N	2007-02-27	Cobro	10943	Cobro MGKWYW	3073.54	0.00	\N	\N	\N	\N	\N	\N	\N	f
2593	\N	1027	\N	2007-03-05	Cobro	10903	Cobro XSZHYS	0.00	874.87	\N	\N	\N	\N	\N	\N	\N	f
2594	\N	1027	\N	2007-03-05	Cobro	10943	Cobro XSZHYS	874.87	0.00	\N	\N	\N	\N	\N	\N	\N	f
2595	\N	1028	\N	2007-03-06	Cobro	10877	Cobro RFMSHL	0.00	221.12	\N	\N	\N	\N	\N	\N	\N	f
2596	\N	1028	\N	2007-03-06	Cobro	10943	Cobro RFMSHL	221.12	0.00	\N	\N	\N	\N	\N	\N	\N	f
2597	\N	1029	\N	2007-03-01	Cobro	10851	Cobro XMTMXG	0.00	240.12	\N	\N	\N	\N	\N	\N	\N	f
2598	\N	1029	\N	2007-03-01	Cobro	10943	Cobro XMTMXG	240.12	0.00	\N	\N	\N	\N	\N	\N	\N	f
2599	\N	1030	\N	2007-03-08	Cobro	10815	Cobro JIZCRY	0.00	48.90	\N	\N	\N	\N	\N	\N	\N	f
2600	\N	1030	\N	2007-03-08	Cobro	10943	Cobro JIZCRY	48.90	0.00	\N	\N	\N	\N	\N	\N	\N	f
2601	\N	1031	\N	2007-02-28	Cobro	10806	Cobro [MJXDT	0.00	2103.12	\N	\N	\N	\N	\N	\N	\N	f
2602	\N	1031	\N	2007-02-28	Cobro	10943	Cobro [MJXDT	2103.12	0.00	\N	\N	\N	\N	\N	\N	\N	f
2603	\N	1032	\N	2007-02-28	Cobro	10806	Cobro WKNSLS	0.00	1758.81	\N	\N	\N	\N	\N	\N	\N	f
2604	\N	1032	\N	2007-02-28	Cobro	10943	Cobro WKNSLS	1758.81	0.00	\N	\N	\N	\N	\N	\N	\N	f
2605	\N	1033	\N	2007-02-28	Cobro	10806	Cobro BHMMAZ	0.00	2420.52	\N	\N	\N	\N	\N	\N	\N	f
2606	\N	1033	\N	2007-02-28	Cobro	10943	Cobro BHMMAZ	2420.52	0.00	\N	\N	\N	\N	\N	\N	\N	f
2607	\N	1034	\N	2007-02-28	Cobro	10806	Cobro BWRKUG	0.00	2420.52	\N	\N	\N	\N	\N	\N	\N	f
2608	\N	1034	\N	2007-02-28	Cobro	10943	Cobro BWRKUG	2420.52	0.00	\N	\N	\N	\N	\N	\N	\N	f
2609	\N	1035	\N	2007-02-28	Cobro	10881	Cobro MFROGY	0.00	1760.88	\N	\N	\N	\N	\N	\N	\N	f
2610	\N	1035	\N	2007-02-28	Cobro	10943	Cobro MFROGY	1760.88	0.00	\N	\N	\N	\N	\N	\N	\N	f
2611	\N	1036	\N	2007-02-28	Cobro	10858	Cobro SWELET	0.00	960.48	\N	\N	\N	\N	\N	\N	\N	f
2612	\N	1036	\N	2007-02-28	Cobro	10943	Cobro SWELET	960.48	0.00	\N	\N	\N	\N	\N	\N	\N	f
2613	\N	1037	\N	2007-02-28	Cobro	10806	Cobro LVVDTM	0.00	1595.37	\N	\N	\N	\N	\N	\N	\N	f
2614	\N	1037	\N	2007-02-28	Cobro	10943	Cobro LVVDTM	1595.37	0.00	\N	\N	\N	\N	\N	\N	\N	f
2615	\N	1038	\N	2007-02-28	Cobro	10806	Cobro XOYIET	0.00	2637.36	\N	\N	\N	\N	\N	\N	\N	f
2616	\N	1038	\N	2007-02-28	Cobro	10943	Cobro XOYIET	2637.36	0.00	\N	\N	\N	\N	\N	\N	\N	f
2617	\N	1039	\N	2007-02-28	Cobro	10878	Cobro HCIVYT	0.00	1096.09	\N	\N	\N	\N	\N	\N	\N	f
2618	\N	1039	\N	2007-02-28	Cobro	10943	Cobro HCIVYT	1096.09	0.00	\N	\N	\N	\N	\N	\N	\N	f
2619	\N	1040	\N	2007-02-28	Cobro	10870	Cobro XHKBSU	0.00	971.15	\N	\N	\N	\N	\N	\N	\N	f
2620	\N	1040	\N	2007-02-28	Cobro	10943	Cobro XHKBSU	971.15	0.00	\N	\N	\N	\N	\N	\N	\N	f
2621	\N	1041	\N	2007-03-20	Cobro	10885	Cobro GFUNYR	0.00	426.88	\N	\N	\N	\N	\N	\N	\N	f
2622	\N	1041	\N	2007-03-20	Cobro	10943	Cobro GFUNYR	426.88	0.00	\N	\N	\N	\N	\N	\N	\N	f
2623	\N	1042	\N	2007-03-20	Cobro	10885	Cobro GITZDY	0.00	426.88	\N	\N	\N	\N	\N	\N	\N	f
2624	\N	1042	\N	2007-03-20	Cobro	10943	Cobro GITZDY	426.88	0.00	\N	\N	\N	\N	\N	\N	\N	f
2625	\N	1043	\N	2007-03-29	Cobro	10802	Cobro VVSOHG	0.00	369.79	\N	\N	\N	\N	\N	\N	\N	f
2626	\N	1043	\N	2007-03-29	Cobro	10943	Cobro VVSOHG	369.79	0.00	\N	\N	\N	\N	\N	\N	\N	f
2627	\N	1044	\N	2007-03-28	Cobro	10830	Cobro SKAKPN	0.00	160.08	\N	\N	\N	\N	\N	\N	\N	f
2628	\N	1044	\N	2007-03-28	Cobro	10943	Cobro SKAKPN	160.08	0.00	\N	\N	\N	\N	\N	\N	\N	f
2629	\N	1045	\N	2007-03-29	Cobro	10830	Cobro IQSXKB	0.00	160.08	\N	\N	\N	\N	\N	\N	\N	f
2630	\N	1045	\N	2007-03-29	Cobro	10943	Cobro IQSXKB	160.08	0.00	\N	\N	\N	\N	\N	\N	\N	f
2631	\N	1046	\N	2007-03-29	Cobro	10830	Cobro SIWRWI	0.00	160.08	\N	\N	\N	\N	\N	\N	\N	f
2632	\N	1046	\N	2007-03-29	Cobro	10943	Cobro SIWRWI	160.08	0.00	\N	\N	\N	\N	\N	\N	\N	f
2633	\N	1047	\N	2007-03-29	Cobro	10883	Cobro VDRMHJ2	0.00	1493.85	\N	\N	\N	\N	\N	\N	\N	f
2634	\N	1047	\N	2007-03-29	Cobro	10943	Cobro VDRMHJ2	1493.85	0.00	\N	\N	\N	\N	\N	\N	\N	f
2635	\N	1048	\N	2007-03-29	Cobro	10883	Cobro QENLSH1	0.00	2673.11	\N	\N	\N	\N	\N	\N	\N	f
2636	\N	1048	\N	2007-03-29	Cobro	10943	Cobro QENLSH1	2673.11	0.00	\N	\N	\N	\N	\N	\N	\N	f
2637	\N	1049	\N	2007-03-29	Cobro	10883	Cobro QENLSH	0.00	1493.85	\N	\N	\N	\N	\N	\N	\N	f
2638	\N	1049	\N	2007-03-29	Cobro	10943	Cobro QENLSH	1493.85	0.00	\N	\N	\N	\N	\N	\N	\N	f
2639	\N	1050	\N	2007-03-29	Cobro	10851	Cobro KZGVJG	0.00	240.12	\N	\N	\N	\N	\N	\N	\N	f
2640	\N	1050	\N	2007-03-29	Cobro	10943	Cobro KZGVJG	240.12	0.00	\N	\N	\N	\N	\N	\N	\N	f
2641	\N	1051	\N	2007-03-29	Cobro	10878	Cobro RJGEKI	0.00	640.32	\N	\N	\N	\N	\N	\N	\N	f
2642	\N	1051	\N	2007-03-29	Cobro	10943	Cobro RJGEKI	640.32	0.00	\N	\N	\N	\N	\N	\N	\N	f
2643	\N	1052	\N	2007-03-29	Cobro	10858	Cobro WVYAUN	0.00	400.20	\N	\N	\N	\N	\N	\N	\N	f
2644	\N	1052	\N	2007-03-29	Cobro	10943	Cobro WVYAUN	400.20	0.00	\N	\N	\N	\N	\N	\N	\N	f
2645	\N	1053	\N	2007-03-29	Cobro	10849	Cobro PVEZPK	0.00	640.32	\N	\N	\N	\N	\N	\N	\N	f
2646	\N	1053	\N	2007-03-29	Cobro	10943	Cobro PVEZPK	640.32	0.00	\N	\N	\N	\N	\N	\N	\N	f
2647	\N	1054	\N	2007-03-29	Cobro	10806	Cobro SGPNZS	0.00	1811.34	\N	\N	\N	\N	\N	\N	\N	f
2648	\N	1054	\N	2007-03-29	Cobro	10943	Cobro SGPNZS	1811.34	0.00	\N	\N	\N	\N	\N	\N	\N	f
2649	\N	1055	\N	2007-03-29	Cobro	10806	Cobro ECEHFZ	0.00	1008.50	\N	\N	\N	\N	\N	\N	\N	f
2650	\N	1055	\N	2007-03-29	Cobro	10943	Cobro ECEHFZ	1008.50	0.00	\N	\N	\N	\N	\N	\N	\N	f
2651	\N	1056	\N	2007-03-29	Cobro	10806	Cobro HJKUGG	0.00	1947.41	\N	\N	\N	\N	\N	\N	\N	f
2652	\N	1056	\N	2007-03-29	Cobro	10943	Cobro HJKUGG	1947.41	0.00	\N	\N	\N	\N	\N	\N	\N	f
2653	\N	1057	\N	2007-03-29	Cobro	10868	Cobro ZCPBFD	0.00	480.24	\N	\N	\N	\N	\N	\N	\N	f
2654	\N	1057	\N	2007-03-29	Cobro	10943	Cobro ZCPBFD	480.24	0.00	\N	\N	\N	\N	\N	\N	\N	f
2655	\N	1058	\N	2007-03-29	Cobro	10870	Cobro DQCDPV	0.00	1727.85	\N	\N	\N	\N	\N	\N	\N	f
2656	\N	1058	\N	2007-03-29	Cobro	10943	Cobro DQCDPV	1727.85	0.00	\N	\N	\N	\N	\N	\N	\N	f
2657	\N	1059	\N	2007-03-29	Cobro	10864	Cobro FTLK[Q	0.00	480.24	\N	\N	\N	\N	\N	\N	\N	f
2658	\N	1059	\N	2007-03-29	Cobro	10943	Cobro FTLK[Q	480.24	0.00	\N	\N	\N	\N	\N	\N	\N	f
2659	\N	1060	\N	2007-03-29	Cobro	10865	Cobro CMNXZZ	0.00	213.44	\N	\N	\N	\N	\N	\N	\N	f
2660	\N	1060	\N	2007-03-29	Cobro	10943	Cobro CMNXZZ	213.44	0.00	\N	\N	\N	\N	\N	\N	\N	f
2661	\N	1061	\N	2007-03-29	Cobro	10881	Cobro ESWOMB	0.00	1760.88	\N	\N	\N	\N	\N	\N	\N	f
2662	\N	1061	\N	2007-03-29	Cobro	10943	Cobro ESWOMB	1760.88	0.00	\N	\N	\N	\N	\N	\N	\N	f
2663	\N	1062	\N	2007-04-02	Cobro	10804	Cobro QYIBYU	0.00	976.81	\N	\N	\N	\N	\N	\N	\N	f
2664	\N	1062	\N	2007-04-02	Cobro	10943	Cobro QYIBYU	976.81	0.00	\N	\N	\N	\N	\N	\N	\N	f
2665	\N	1063	\N	2007-04-03	Cobro	10804	Cobro XQBOOR	0.00	1449.78	\N	\N	\N	\N	\N	\N	\N	f
2666	\N	1063	\N	2007-04-03	Cobro	10943	Cobro XQBOOR	1449.78	0.00	\N	\N	\N	\N	\N	\N	\N	f
2667	\N	1064	\N	2007-04-03	Cobro	10804	Cobro [VYFS[	0.00	10672.00	\N	\N	\N	\N	\N	\N	\N	f
2668	\N	1064	\N	2007-04-03	Cobro	10943	Cobro [VYFS[	10672.00	0.00	\N	\N	\N	\N	\N	\N	\N	f
2669	\N	1065	\N	2007-03-30	Cobro	10901	Cobro YQPCIE	0.00	156.40	\N	\N	\N	\N	\N	\N	\N	f
2670	\N	1065	\N	2007-03-30	Cobro	10943	Cobro YQPCIE	156.40	0.00	\N	\N	\N	\N	\N	\N	\N	f
2671	\N	1066	\N	2007-05-01	Cobro	10802	Cobro IVXVYD	0.00	156.31	\N	\N	\N	\N	\N	\N	\N	f
2672	\N	1066	\N	2007-05-01	Cobro	10943	Cobro IVXVYD	156.31	0.00	\N	\N	\N	\N	\N	\N	\N	f
2673	\N	1067	\N	2007-05-03	Cobro	10863	Cobro CLL[ZS	0.00	2241.12	\N	\N	\N	\N	\N	\N	\N	f
2674	\N	1067	\N	2007-05-03	Cobro	10943	Cobro CLL[ZS	2241.12	0.00	\N	\N	\N	\N	\N	\N	\N	f
2675	\N	1068	\N	2007-04-27	Cobro	10892	Cobro CNJXJO	0.00	6349.84	\N	\N	\N	\N	\N	\N	\N	f
2676	\N	1068	\N	2007-04-27	Cobro	10943	Cobro CNJXJO	6349.84	0.00	\N	\N	\N	\N	\N	\N	\N	f
2677	\N	1069	\N	2007-04-26	Cobro	10890	Cobro QBVUMK	0.00	368.00	\N	\N	\N	\N	\N	\N	\N	f
2678	\N	1069	\N	2007-04-26	Cobro	10936	Cobro QBVUMK	368.00	0.00	\N	\N	\N	\N	\N	\N	\N	f
2679	\N	1070	\N	2007-04-24	Cobro	10802	Cobro MYVGGU	0.00	15105.99	\N	\N	\N	\N	\N	\N	\N	f
2680	\N	1070	\N	2007-04-24	Cobro	10936	Cobro MYVGGU	15105.99	0.00	\N	\N	\N	\N	\N	\N	\N	f
2681	\N	1071	\N	2007-04-20	Cobro	10890	Cobro QBVUMK	0.00	2300.00	\N	\N	\N	\N	\N	\N	\N	f
2682	\N	1071	\N	2007-04-20	Cobro	10936	Cobro QBVUMK	2300.00	0.00	\N	\N	\N	\N	\N	\N	\N	f
2683	\N	1072	\N	2007-05-02	Cobro	10820	Cobro PNWNYA	0.00	1672.10	\N	\N	\N	\N	\N	\N	\N	f
2684	\N	1072	\N	2007-05-02	Cobro	10936	Cobro PNWNYA	1672.10	0.00	\N	\N	\N	\N	\N	\N	\N	f
2685	\N	1073	\N	2007-05-02	Cobro	10820	Cobro KBJKOH	0.00	3073.54	\N	\N	\N	\N	\N	\N	\N	f
2686	\N	1073	\N	2007-05-02	Cobro	10936	Cobro KBJKOH	3073.54	0.00	\N	\N	\N	\N	\N	\N	\N	f
2687	\N	1074	\N	2007-04-30	Cobro	10881	Cobro OIWXYB	0.00	2027.68	\N	\N	\N	\N	\N	\N	\N	f
2688	\N	1074	\N	2007-04-30	Cobro	10936	Cobro OIWXYB	2027.68	0.00	\N	\N	\N	\N	\N	\N	\N	f
2689	\N	1075	\N	2007-04-30	Cobro	10806	Cobro NUYUXO	0.00	1932.69	\N	\N	\N	\N	\N	\N	\N	f
2690	\N	1075	\N	2007-04-30	Cobro	10936	Cobro NUYUXO	1932.69	0.00	\N	\N	\N	\N	\N	\N	\N	f
2691	\N	1076	\N	2007-04-30	Cobro	10870	Cobro YTWDIY	0.00	971.15	\N	\N	\N	\N	\N	\N	\N	f
2692	\N	1076	\N	2007-04-30	Cobro	10936	Cobro YTWDIY	971.15	0.00	\N	\N	\N	\N	\N	\N	\N	f
2693	\N	1077	\N	2007-04-30	Cobro	10865	Cobro UAWJDP	0.00	213.44	\N	\N	\N	\N	\N	\N	\N	f
2694	\N	1077	\N	2007-04-30	Cobro	10936	Cobro UAWJDP	213.44	0.00	\N	\N	\N	\N	\N	\N	\N	f
2695	\N	1078	\N	2007-05-03	Cobro	10889	Cobro INLXZA	0.00	1920.96	\N	\N	\N	\N	\N	\N	\N	f
2696	\N	1078	\N	2007-05-03	Cobro	10936	Cobro INLXZA	1920.96	0.00	\N	\N	\N	\N	\N	\N	\N	f
2697	\N	1079	\N	2007-03-31	Cobro	10806	Cobro COAQPM	0.00	2587.96	\N	\N	\N	\N	\N	\N	\N	f
2698	\N	1079	\N	2007-03-31	Cobro	10936	Cobro COAQPM	2587.96	0.00	\N	\N	\N	\N	\N	\N	\N	f
2699	\N	1080	\N	2007-03-31	Cobro	10806	Cobro ZRDELL	0.00	968.35	\N	\N	\N	\N	\N	\N	\N	f
2700	\N	1080	\N	2007-03-31	Cobro	10936	Cobro ZRDELL	968.35	0.00	\N	\N	\N	\N	\N	\N	\N	f
2701	\N	1081	\N	2007-03-30	Cobro	10806	Cobro AXDFEF	0.00	747.04	\N	\N	\N	\N	\N	\N	\N	f
2702	\N	1081	\N	2007-03-30	Cobro	10936	Cobro AXDFEF	747.04	0.00	\N	\N	\N	\N	\N	\N	\N	f
2703	\N	1082	\N	2007-03-17	Cobro	10806	Cobro WHRNDO	0.00	860.71	\N	\N	\N	\N	\N	\N	\N	f
2704	\N	1082	\N	2007-03-17	Cobro	10936	Cobro WHRNDO	860.71	0.00	\N	\N	\N	\N	\N	\N	\N	f
2705	\N	1083	\N	2007-03-29	Cobro	10858	Cobro WVYAUN	0.00	400.20	\N	\N	\N	\N	\N	\N	\N	f
2706	\N	1083	\N	2007-03-29	Cobro	10936	Cobro WVYAUN	400.20	0.00	\N	\N	\N	\N	\N	\N	\N	f
2707	\N	1084	\N	2007-03-17	Cobro	10866	Cobro HGOYZQ	0.00	160.08	\N	\N	\N	\N	\N	\N	\N	f
2708	\N	1084	\N	2007-03-17	Cobro	10936	Cobro HGOYZQ	160.08	0.00	\N	\N	\N	\N	\N	\N	\N	f
2709	\N	1085	\N	2007-02-19	Cobro	10885	Cobro YIKBD[	0.00	1188.87	\N	\N	\N	\N	\N	\N	\N	f
2710	\N	1085	\N	2007-02-19	Cobro	10943	Cobro YIKBD[	1188.87	0.00	\N	\N	\N	\N	\N	\N	\N	f
2711	\N	1086	\N	2007-03-29	Cobro	10866	Cobro JXKSHV	0.00	480.24	\N	\N	\N	\N	\N	\N	\N	f
2712	\N	1086	\N	2007-03-29	Cobro	10936	Cobro JXKSHV	480.24	0.00	\N	\N	\N	\N	\N	\N	\N	f
2713	\N	1087	\N	2007-03-29	Cobro	10864	Cobro JXKSHV	0.00	480.24	\N	\N	\N	\N	\N	\N	\N	f
2714	\N	1087	\N	2007-03-29	Cobro	10943	Cobro JXKSHV	480.24	0.00	\N	\N	\N	\N	\N	\N	\N	f
2715	\N	1088	\N	2007-05-09	Cobro	10882	Cobro CJIKTG	0.00	160.08	\N	\N	\N	\N	\N	\N	\N	f
2716	\N	1088	\N	2007-05-09	Cobro	10936	Cobro CJIKTG	160.08	0.00	\N	\N	\N	\N	\N	\N	\N	f
2717	\N	1089	\N	2007-05-11	Cobro	10867	Cobro UNIIYN	0.00	290.44	\N	\N	\N	\N	\N	\N	\N	f
2718	\N	1089	\N	2007-05-11	Cobro	10937	Cobro UNIIYN	290.44	0.00	\N	\N	\N	\N	\N	\N	\N	f
2719	\N	1090	\N	2007-05-11	Cobro	10867	Cobro LRMBBX	0.00	501.58	\N	\N	\N	\N	\N	\N	\N	f
2720	\N	1090	\N	2007-05-11	Cobro	10937	Cobro LRMBBX	501.58	0.00	\N	\N	\N	\N	\N	\N	\N	f
2721	\N	1091	\N	2007-05-11	Cobro	10818	Cobro KQTYYO	0.00	186.81	\N	\N	\N	\N	\N	\N	\N	f
2722	\N	1091	\N	2007-05-11	Cobro	10938	Cobro KQTYYO	186.81	0.00	\N	\N	\N	\N	\N	\N	\N	f
2723	\N	1092	\N	2007-04-12	Cobro	10877	Cobro EFEGZF	0.00	234.19	\N	\N	\N	\N	\N	\N	\N	f
2724	\N	1092	\N	2007-04-12	Cobro	10937	Cobro EFEGZF	234.19	0.00	\N	\N	\N	\N	\N	\N	\N	f
2725	\N	1093	\N	2007-05-11	Cobro	10881	Cobro GWCFSO	0.00	2331.51	\N	\N	\N	\N	\N	\N	\N	f
2726	\N	1093	\N	2007-05-11	Cobro	10936	Cobro GWCFSO	2331.51	0.00	\N	\N	\N	\N	\N	\N	\N	f
2727	\N	1094	\N	2007-05-11	Cobro	10868	Cobro DOOEWQ	0.00	160.08	\N	\N	\N	\N	\N	\N	\N	f
2728	\N	1094	\N	2007-05-11	Cobro	10936	Cobro DOOEWQ	160.08	0.00	\N	\N	\N	\N	\N	\N	\N	f
2729	\N	1095	\N	2007-05-11	Cobro	10887	Cobro KUHKXO	0.00	640.32	\N	\N	\N	\N	\N	\N	\N	f
2730	\N	1095	\N	2007-05-11	Cobro	10936	Cobro KUHKXO	640.32	0.00	\N	\N	\N	\N	\N	\N	\N	f
2731	\N	1096	\N	2007-08-14	Cobro	10845	Cobro TYZ[VR	0.00	6900.00	\N	\N	\N	\N	\N	\N	\N	f
2732	\N	1096	\N	2007-08-14	Cobro	10943	Cobro TYZ[VR	6900.00	0.00	\N	\N	\N	\N	\N	\N	\N	f
2733	\N	1097	\N	2007-03-14	Cobro	10856	Cobro LQIDBZ	0.00	4652.99	\N	\N	\N	\N	\N	\N	\N	f
2734	\N	1097	\N	2007-03-14	Cobro	10943	Cobro LQIDBZ	4652.99	0.00	\N	\N	\N	\N	\N	\N	\N	f
2735	\N	1098	\N	2007-05-24	Cobro	10879	Cobro HGMRII	0.00	465.75	\N	\N	\N	\N	\N	\N	\N	f
2736	\N	1098	\N	2007-05-24	Cobro	10937	Cobro HGMRII	465.75	0.00	\N	\N	\N	\N	\N	\N	\N	f
2737	\N	1099	\N	2007-05-24	Cobro	10830	Cobro EIY[ED	0.00	160.08	\N	\N	\N	\N	\N	\N	\N	f
2738	\N	1099	\N	2007-05-24	Cobro	10936	Cobro EIY[ED	160.08	0.00	\N	\N	\N	\N	\N	\N	\N	f
2739	\N	1100	\N	2007-05-19	Cobro	10802	Cobro NGIXEL	0.00	1109.34	\N	\N	\N	\N	\N	\N	\N	f
2740	\N	1100	\N	2007-05-19	Cobro	10936	Cobro NGIXEL	1109.34	0.00	\N	\N	\N	\N	\N	\N	\N	f
2741	\N	1101	\N	2007-05-07	Cobro	10878	Cobro FGTPBN	0.00	640.32	\N	\N	\N	\N	\N	\N	\N	f
2742	\N	1101	\N	2007-05-07	Cobro	10936	Cobro FGTPBN	640.32	0.00	\N	\N	\N	\N	\N	\N	\N	f
2743	\N	1102	\N	2007-05-28	Cobro	10857	Cobro KNMZWF	0.00	920.00	\N	\N	\N	\N	\N	\N	\N	f
2744	\N	1102	\N	2007-05-28	Cobro	10937	Cobro KNMZWF	920.00	0.00	\N	\N	\N	\N	\N	\N	\N	f
2745	\N	1103	\N	2007-05-24	Cobro	10830	Cobro IFREQT	0.00	160.08	\N	\N	\N	\N	\N	\N	\N	f
2746	\N	1103	\N	2007-05-24	Cobro	10936	Cobro IFREQT	160.08	0.00	\N	\N	\N	\N	\N	\N	\N	f
2747	\N	1104	\N	2007-05-24	Cobro	10885	Cobro XWZFGT	0.00	426.88	\N	\N	\N	\N	\N	\N	\N	f
2748	\N	1104	\N	2007-05-24	Cobro	10936	Cobro XWZFGT	426.88	0.00	\N	\N	\N	\N	\N	\N	\N	f
2749	\N	1105	\N	2007-05-24	Cobro	10885	Cobro VYBAMY	0.00	426.88	\N	\N	\N	\N	\N	\N	\N	f
2750	\N	1105	\N	2007-05-24	Cobro	10936	Cobro VYBAMY	426.88	0.00	\N	\N	\N	\N	\N	\N	\N	f
2751	\N	1106	\N	2007-05-25	Cobro	10804	Cobro NNZNUN	0.00	8004.00	\N	\N	\N	\N	\N	\N	\N	f
2752	\N	1106	\N	2007-05-25	Cobro	10936	Cobro NNZNUN	8004.00	0.00	\N	\N	\N	\N	\N	\N	\N	f
2753	\N	1107	\N	2007-05-24	Cobro	10830	Cobro SZYMUR	0.00	226.96	\N	\N	\N	\N	\N	\N	\N	f
2754	\N	1107	\N	2007-05-24	Cobro	10936	Cobro SZYMUR	226.96	0.00	\N	\N	\N	\N	\N	\N	\N	f
2755	\N	1108	\N	2007-02-05	Cobro	10798	Cobro YCIHXH	0.00	1467.40	\N	\N	\N	\N	\N	\N	\N	f
2756	\N	1108	\N	2007-02-05	Cobro	10943	Cobro YCIHXH	1467.40	0.00	\N	\N	\N	\N	\N	\N	\N	f
\.


--
-- Data for Name: asiento; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY asiento (idasiento, descripcion, fecha, comentariosasiento, ordenasiento, clase) FROM stdin;
1109	[A. Automatico] Pago BTURJT	2007-01-10	[A. Automatico] Pago BTURJT	102	0
1110	[A. Automatico] Pago VSHKXW	2007-01-26	[A. Automatico] Pago VSHKXW	103	0
1111	[A. Automatico] Pago UVHYFI	2007-01-26	[A. Automatico] Pago UVHYFI	104	0
1112	[A. Automatico] Pago POVHXI	2007-02-01	[A. Automatico] Pago POVHXI	105	0
1113	[A. Automatico] Pago YTGOLI	2007-01-11	[A. Automatico] Pago YTGOLI	106	0
1114	[A. Automatico] Pago OZDCTQ	2007-01-18	[A. Automatico] Pago OZDCTQ	107	0
1115	[A. Automatico] Pago XFHYRL	2007-02-22	[A. Automatico] Pago XFHYRL	108	0
1116	[A. Automatico] Pago FBWSQS	2007-02-09	[A. Automatico] Pago FBWSQS	109	0
1117	[A. Automatico] Pago TKGFJV	2007-03-14	[A. Automatico] Pago TKGFJV	110	0
1118	[A. Automatico] Pago ZHMULP	2007-03-09	[A. Automatico] Pago ZHMULP	111	0
1119	[A. Automatico] Pago DMJYLP	2007-03-09	[A. Automatico] Pago DMJYLP	112	0
1120	[A. Automatico] Pago UVRHKP	2007-03-02	[A. Automatico] Pago UVRHKP	113	0
1121	[A. Automatico] Pago TBSQGT	2007-02-21	[A. Automatico] Pago TBSQGT	114	0
1122	[A. Automatico] Pago XZINVQ	2007-02-22	[A. Automatico] Pago XZINVQ	115	0
1123	[A. Automatico] Pago MYCVWN	2007-02-15	[A. Automatico] Pago MYCVWN	116	0
1124	[A. Automatico] Pago LGTMEC	2007-02-21	[A. Automatico] Pago LGTMEC	117	0
1125	[A. Automatico] Pago GYXXFH	2007-02-17	[A. Automatico] Pago GYXXFH	118	0
1126	[A. Automatico] Pago MYIEOO	2007-03-02	[A. Automatico] Pago MYIEOO	119	0
1127	[A. Automatico] Pago XLNFYI	2007-02-08	[A. Automatico] Pago XLNFYI	120	0
1128	[A. Automatico] Pago VLGXGD	2007-02-02	[A. Automatico] Pago VLGXGD	121	0
1129	[A. Automatico] Pago WSNBXT	2007-01-09	[A. Automatico] Pago WSNBXT	122	0
1130	[A. Automatico] Pago KRICCN	2007-02-06	[A. Automatico] Pago KRICCN	123	0
1131	[A. Automatico] Pago EJLBGQ	2007-01-23	[A. Automatico] Pago EJLBGQ	124	0
1132	[A. Automatico] Pago ISOPVD	2007-03-03	[A. Automatico] Pago ISOPVD	125	0
1133	[A. Automatico] Pago MKMQTS	2007-02-23	[A. Automatico] Pago MKMQTS	126	0
1134	[A. Automatico] Pago VIPUQY	2007-03-21	[A. Automatico] Pago VIPUQY	127	0
1135	[A. Automatico] Pago QSCZIJ	2007-03-20	[A. Automatico] Pago QSCZIJ	128	0
1136	[A. Automatico] Pago PUFJNS	2007-03-28	[A. Automatico] Pago PUFJNS	129	0
1137	[A. Automatico] Pago XRQMGX	2007-03-30	[A. Automatico] Pago XRQMGX	130	0
1138	[A. Automatico] Pago OVMUKG	2007-03-19	[A. Automatico] Pago OVMUKG	131	0
1139	[A. Automatico] Pago [WCFQE	2007-05-17	[A. Automatico] Pago [WCFQE	132	0
1140	[A. Automatico] Pago WSNBXT	2007-01-27	[A. Automatico] Pago WSNBXT	133	0
1141	[A. Automatico] Pago TXBSBZ	2007-06-06	[A. Automatico] Pago TXBSBZ	134	0
1142	[A. Automatico] Pago BYFKIO	2007-05-14	[A. Automatico] Pago BYFKIO	135	0
1143	[A. Automatico] Pago YX[YNK	2007-05-23	[A. Automatico] Pago YX[YNK	136	0
1144	[A. Automatico] Pago TKSLPF	2007-02-12	[A. Automatico] Pago TKSLPF	137	0
1145	[A. Automatico] Pago UXVMXB	2007-05-25	[A. Automatico] Pago UXVMXB	138	0
1146	[A. Automatico] Pago UQYVIZ	2007-06-07	[A. Automatico] Pago UQYVIZ	139	0
1147	[A. Automatico] Pago VJXZTF	2007-05-04	[A. Automatico] Pago VJXZTF	140	0
1148	[A. Automatico] Pago ORDNOQ	2007-05-04	[A. Automatico] Pago ORDNOQ	141	0
1149	[A. Automatico] Pago YXQVED	2007-05-21	[A. Automatico] Pago YXQVED	142	0
1150	[A. Automatico] Pago OYAIKY	2007-05-21	[A. Automatico] Pago OYAIKY	143	0
1151	[A. Automatico] Pago JEOHZW	2007-06-06	[A. Automatico] Pago JEOHZW	144	0
1152	[A. Automatico] Pago 111	2007-08-13	[A. Automatico] Pago 111	145	0
1153	[A. Automatico] Pago UMQIBH	2007-08-14	[A. Automatico] Pago UMQIBH	146	0
1154	[A. Automatico] Pago SBDNFG	2007-08-31	[A. Automatico] Pago SBDNFG	147	0
1155	[A. Automatico] Factura Cliente Num.12	2007-01-31	[A. Automatico] Factura Cliente Num.12	148	0
1281	[A.Automatico] Factura Proveedor Num:004	2007-01-18	[A.Automatico] Factura Proveedor Num:004	274	0
1282	[A.Automatico] Factura Proveedor Num:075-7872324686	2007-01-16	[A.Automatico] Factura Proveedor Num:075-7872324686	275	0
1283	[A.Automatico] Factura Proveedor Num:07/00475	2007-02-07	[A.Automatico] Factura Proveedor Num:07/00475	276	0
1284	[A.Automatico] Factura Proveedor Num:99959	2007-01-11	[A.Automatico] Factura Proveedor Num:99959	277	0
1285	[A.Automatico] Factura Proveedor Num:00717	2007-02-27	[A.Automatico] Factura Proveedor Num:00717	278	0
1286	[A.Automatico] Factura Proveedor Num:102266	2007-02-22	[A.Automatico] Factura Proveedor Num:102266	279	0
1287	[A.Automatico] Factura Proveedor Num:00583	2007-02-15	[A.Automatico] Factura Proveedor Num:00583	280	0
1288	[A.Automatico] Factura Proveedor Num:101360	2007-02-06	[A.Automatico] Factura Proveedor Num:101360	281	0
1289	[A.Automatico] Factura Proveedor Num:101483	2007-02-07	[A.Automatico] Factura Proveedor Num:101483	282	0
1290	[A.Automatico] Factura Proveedor Num:101049	2007-01-31	[A.Automatico] Factura Proveedor Num:101049	283	0
1291	[A.Automatico] Factura Proveedor Num:101359	2007-02-06	[A.Automatico] Factura Proveedor Num:101359	284	0
1292	[A.Automatico] Factura Proveedor Num:GIB	2007-02-02	[A.Automatico] Factura Proveedor Num:GIB	285	0
1293	[A.Automatico] Factura Proveedor Num:101892	2007-02-15	[A.Automatico] Factura Proveedor Num:101892	286	0
1294	[A.Automatico] Factura Proveedor Num:100689	2007-01-24	[A.Automatico] Factura Proveedor Num:100689	287	0
1295	[A.Automatico] Factura Proveedor Num:A40307549	2007-02-02	[A.Automatico] Factura Proveedor Num:A40307549	288	0
1296	[A.Automatico] Factura Proveedor Num:0710	2007-01-08	[A.Automatico] Factura Proveedor Num:0710	289	0
1297	[A.Automatico] Factura Proveedor Num:0726	2007-01-09	[A.Automatico] Factura Proveedor Num:0726	290	0
1298	[A.Automatico] Factura Proveedor Num:100560	2007-01-22	[A.Automatico] Factura Proveedor Num:100560	291	0
1299	[A.Automatico] Factura Proveedor Num:321393	2007-01-23	[A.Automatico] Factura Proveedor Num:321393	292	0
1300	[A.Automatico] Factura Proveedor Num:102029	2007-02-16	[A.Automatico] Factura Proveedor Num:102029	293	0
1301	[A.Automatico] Factura Proveedor Num:778021	2007-02-23	[A.Automatico] Factura Proveedor Num:778021	294	0
1302	[A.Automatico] Factura Proveedor Num:102870	2007-03-06	[A.Automatico] Factura Proveedor Num:102870	295	0
1303	[A.Automatico] Factura Proveedor Num:769	2007-03-06	[A.Automatico] Factura Proveedor Num:769	296	0
1304	[A.Automatico] Factura Proveedor Num:103279	2007-03-13	[A.Automatico] Factura Proveedor Num:103279	297	0
1305	[A.Automatico] Factura Proveedor Num:2007001	2007-02-02	[A.Automatico] Factura Proveedor Num:2007001	298	0
1306	[A.Automatico] Factura Proveedor Num:008922	2007-01-11	[A.Automatico] Factura Proveedor Num:008922	299	0
1307	[A.Automatico] Factura Proveedor Num:07/01394	2007-04-27	[A.Automatico] Factura Proveedor Num:07/01394	300	0
1308	[A.Automatico] Factura Proveedor Num:07/00111	2007-01-12	[A.Automatico] Factura Proveedor Num:07/00111	301	0
1309	[A.Automatico] Factura Proveedor Num:07/0173	2007-05-22	[A.Automatico] Factura Proveedor Num:07/0173	302	0
1310	[A.Automatico] Factura Proveedor Num:01567	2007-05-10	[A.Automatico] Factura Proveedor Num:01567	303	0
1311	[A.Automatico] Factura Proveedor Num:1719	2007-05-23	[A.Automatico] Factura Proveedor Num:1719	304	0
1312	[A.Automatico] Factura Proveedor Num:100270	2007-01-17	[A.Automatico] Factura Proveedor Num:100270	305	0
1313	[A.Automatico] Factura Proveedor Num:1187	2007-04-10	[A.Automatico] Factura Proveedor Num:1187	306	0
1314	[A.Automatico] Factura Proveedor Num:1317	2007-04-21	[A.Automatico] Factura Proveedor Num:1317	307	0
1315	[A.Automatico] Factura Proveedor Num:1448	2007-05-02	[A.Automatico] Factura Proveedor Num:1448	308	0
1316	[A.Automatico] Factura Proveedor Num:07/01630	2007-05-22	[A.Automatico] Factura Proveedor Num:07/01630	309	0
1317	[A.Automatico] Factura Proveedor Num:105920	2007-05-02	[A.Automatico] Factura Proveedor Num:105920	310	0
1318	[A.Automatico] Factura Proveedor Num:07/01430	2007-04-30	[A.Automatico] Factura Proveedor Num:07/01430	311	0
1319	[A.Automatico] Factura Proveedor Num:102265	2007-02-22	[A.Automatico] Factura Proveedor Num:102265	312	0
1320	[A.Automatico] Factura Proveedor Num:103405	2007-03-15	[A.Automatico] Factura Proveedor Num:103405	313	0
1185	[A. Automatico] Factura Cliente Num.3	2007-01-25	[A. Automatico] Factura Cliente Num.3	178	0
1186	[A. Automatico] Factura Cliente Num.41	2007-02-27	[A. Automatico] Factura Cliente Num.41	179	0
1187	[A. Automatico] Factura Cliente Num.47	2007-03-06	[A. Automatico] Factura Cliente Num.47	180	0
1188	[A. Automatico] Factura Cliente Num.46	2007-03-06	[A. Automatico] Factura Cliente Num.46	181	0
1189	[A. Automatico] Factura Cliente Num.50	2007-03-08	[A. Automatico] Factura Cliente Num.50	182	0
1190	[A. Automatico] Factura Cliente Num.44	2007-02-27	[A. Automatico] Factura Cliente Num.44	183	0
1192	[A. Automatico] Factura Cliente Num.39	2007-02-22	[A. Automatico] Factura Cliente Num.39	185	0
1193	[A. Automatico] Factura Cliente Num.38	2007-02-22	[A. Automatico] Factura Cliente Num.38	186	0
1194	[A. Automatico] Factura Cliente Num.45	2007-02-27	[A. Automatico] Factura Cliente Num.45	187	0
1195	[A. Automatico] Factura Cliente Num.42	2007-02-27	[A. Automatico] Factura Cliente Num.42	188	0
1196	[A. Automatico] Factura Cliente Num.43	2007-02-27	[A. Automatico] Factura Cliente Num.43	189	0
1156	[A. Automatico] Factura Cliente Num.35	2007-02-22	[A. Automatico] Factura Cliente Num.35	149	0
1245	[A. Automatico] Factura Cliente Num.27	2007-02-19	[A. Automatico] Factura Cliente Num.27	238	0
1246	[A. Automatico] Factura Cliente Num.37	2007-02-22	[A. Automatico] Factura Cliente Num.37	239	0
1197	[A. Automatico] Factura Cliente Num.57	2007-03-20	[A. Automatico] Factura Cliente Num.57	190	0
1198	[A. Automatico] Factura Cliente Num.49	2007-03-08	[A. Automatico] Factura Cliente Num.49	191	0
1199	[A. Automatico] Factura Cliente Num.53	2007-03-14	[A. Automatico] Factura Cliente Num.53	192	0
1200	[A. Automatico] Factura Cliente Num.52	2007-03-14	[A. Automatico] Factura Cliente Num.52	193	0
1201	[A. Automatico] Factura Cliente Num.55	2007-03-20	[A. Automatico] Factura Cliente Num.55	194	0
1202	[A. Automatico] Factura Cliente Num.54	2007-03-20	[A. Automatico] Factura Cliente Num.54	195	0
1247	[A. Automatico] Factura Cliente Num.74	2007-03-29	[A. Automatico] Factura Cliente Num.74	240	0
1248	[A. Automatico] Factura Cliente Num.73	2007-03-29	[A. Automatico] Factura Cliente Num.73	241	0
1249	[A. Automatico] Factura Cliente Num.48	2007-03-08	[A. Automatico] Factura Cliente Num.48	242	0
1250	[A. Automatico] Factura Cliente Num.61	2007-03-23	[A. Automatico] Factura Cliente Num.61	243	0
1157	[A. Automatico] Factura Cliente Num.59	2007-03-23	[A. Automatico] Factura Cliente Num.59	150	0
1158	[A. Automatico] Factura Cliente Num.72	2007-03-27	[A. Automatico] Factura Cliente Num.72	151	0
1159	[A. Automatico] Factura Cliente Num.71	2007-03-27	[A. Automatico] Factura Cliente Num.71	152	0
1160	[A. Automatico] Factura Cliente Num.64	2007-03-27	[A. Automatico] Factura Cliente Num.64	153	0
1161	[A. Automatico] Factura Cliente Num.58	2007-03-20	[A. Automatico] Factura Cliente Num.58	154	0
1203	[A. Automatico] Factura Cliente Num.75	2007-03-30	[A. Automatico] Factura Cliente Num.75	196	0
1204	[A. Automatico] Factura Cliente Num.51	2007-03-13	[A. Automatico] Factura Cliente Num.51	197	0
1205	[A. Automatico] Factura Cliente Num.77	2007-04-03	[A. Automatico] Factura Cliente Num.77	198	0
1206	[A. Automatico] Factura Cliente Num.79	2007-04-03	[A. Automatico] Factura Cliente Num.79	199	0
1207	[A. Automatico] Factura Cliente Num.80	2007-04-03	[A. Automatico] Factura Cliente Num.80	200	0
1208	[A. Automatico] Factura Cliente Num.76	2007-04-09	[A. Automatico] Factura Cliente Num.76	201	0
1209	[A. Automatico] Factura Cliente Num.78	2007-04-03	[A. Automatico] Factura Cliente Num.78	202	0
1210	[A. Automatico] Factura Cliente Num.84	2007-04-23	[A. Automatico] Factura Cliente Num.84	203	0
1211	[A. Automatico] Factura Cliente Num.81	2007-04-17	[A. Automatico] Factura Cliente Num.81	204	0
1251	[A. Automatico] Factura Cliente Num.2	2007-01-15	[A. Automatico] Factura Cliente Num.2	244	0
1252	[A. Automatico] Factura Cliente Num.86	2007-04-23	[A. Automatico] Factura Cliente Num.86	245	0
1253	[A. Automatico] Factura Cliente Num.89	2007-04-23	[A. Automatico] Factura Cliente Num.89	246	0
1162	[A. Automatico] Factura Cliente Num.85	2007-04-23	[A. Automatico] Factura Cliente Num.85	155	0
1254	[A. Automatico] Factura Cliente Num.83	2007-04-20	[A. Automatico] Factura Cliente Num.83	247	0
1212	[A. Automatico] Factura Cliente Num.90	2007-04-23	[A. Automatico] Factura Cliente Num.90	205	0
1213	[A. Automatico] Factura Cliente Num.87	2007-04-23	[A. Automatico] Factura Cliente Num.87	206	0
1214	[A. Automatico] Factura Cliente Num.68	2007-03-27	[A. Automatico] Factura Cliente Num.68	207	0
1215	[A. Automatico] Factura Cliente Num.91	2007-04-24	[A. Automatico] Factura Cliente Num.91	208	0
1216	[A. Automatico] Factura Cliente Num.92	2007-04-24	[A. Automatico] Factura Cliente Num.92	209	0
1217	[A. Automatico] Factura Cliente Num.82	2007-04-20	[A. Automatico] Factura Cliente Num.82	210	0
1218	[A. Automatico] Factura Cliente Num.31	2007-02-22	[A. Automatico] Factura Cliente Num.31	211	0
1219	[A. Automatico] Factura Cliente Num.30	2007-02-22	[A. Automatico] Factura Cliente Num.30	212	0
1220	[A. Automatico] Factura Cliente Num.94	2007-04-26	[A. Automatico] Factura Cliente Num.94	213	0
1255	[A. Automatico] Factura Cliente Num.101	2007-05-08	[A. Automatico] Factura Cliente Num.101	248	0
1256	[A. Automatico] Factura Cliente Num.112	2007-05-18	[A. Automatico] Factura Cliente Num.112	249	0
1257	[A. Automatico] Factura Cliente Num.115	2007-05-25	[A. Automatico] Factura Cliente Num.115	250	0
1258	[A. Automatico] Factura Cliente Num.116	2007-05-25	[A. Automatico] Factura Cliente Num.116	251	0
1259	[A. Automatico] Factura Cliente Num.32	2007-02-22	[A. Automatico] Factura Cliente Num.32	252	0
1260	[A. Automatico] Factura Cliente Num.103	2007-05-08	[A. Automatico] Factura Cliente Num.103	253	0
1221	[A. Automatico] Factura Cliente Num.33	2007-02-22	[A. Automatico] Factura Cliente Num.33	214	0
1222	[A. Automatico] Factura Cliente Num.34	2007-02-22	[A. Automatico] Factura Cliente Num.34	215	0
1223	[A. Automatico] Factura Cliente Num.106	2007-05-08	[A. Automatico] Factura Cliente Num.106	216	0
1224	[A. Automatico] Factura Cliente Num.111	2007-05-08	[A. Automatico] Factura Cliente Num.111	217	0
1225	[A. Automatico] Factura Cliente Num.105	2007-05-08	[A. Automatico] Factura Cliente Num.105	218	0
1226	[A. Automatico] Factura Cliente Num.104	2007-05-08	[A. Automatico] Factura Cliente Num.104	219	0
1227	[A. Automatico] Factura Cliente Num.96	2007-05-04	[A. Automatico] Factura Cliente Num.96	220	0
1228	[A. Automatico] Factura Cliente Num.97	2007-05-04	[A. Automatico] Factura Cliente Num.97	221	0
1229	[A. Automatico] Factura Cliente Num.108	2007-05-09	[A. Automatico] Factura Cliente Num.108	222	0
1261	[A. Automatico] Factura Cliente Num.36	2007-02-22	[A. Automatico] Factura Cliente Num.36	254	0
1262	[A. Automatico] Factura Cliente Num.109	2007-05-09	[A. Automatico] Factura Cliente Num.109	255	0
1230	[A. Automatico] Factura Cliente Num.113	2007-05-18	[A. Automatico] Factura Cliente Num.113	223	0
1231	[A. Automatico] Factura Cliente Num.114	2007-05-18	[A. Automatico] Factura Cliente Num.114	224	0
1232	[A. Automatico] Factura Cliente Num.118	2007-05-25	[A. Automatico] Factura Cliente Num.118	225	0
1233	[A. Automatico] Factura Cliente Num.117	2007-05-25	[A. Automatico] Factura Cliente Num.117	226	0
1234	[A. Automatico] Factura Cliente Num.119	2007-05-25	[A. Automatico] Factura Cliente Num.119	227	0
1235	[A. Automatico] Factura Cliente Num.120	2007-05-25	[A. Automatico] Factura Cliente Num.120	228	0
1236	[A. Automatico] Factura Cliente Num.121	2007-05-25	[A. Automatico] Factura Cliente Num.121	229	0
1237	[A. Automatico] Factura Cliente Num.66	2007-03-27	[A. Automatico] Factura Cliente Num.66	230	0
1263	[A. Automatico] Factura Cliente Num.63	2007-03-27	[A. Automatico] Factura Cliente Num.63	256	0
1264	[A. Automatico] Factura Cliente Num.60	2007-03-23	[A. Automatico] Factura Cliente Num.60	257	0
1265	[A. Automatico] Factura Cliente Num.62	2007-03-27	[A. Automatico] Factura Cliente Num.62	258	0
1240	[A. Automatico] Factura Cliente Num.70	2007-03-27	[A. Automatico] Factura Cliente Num.70	233	0
1241	[A. Automatico] Factura Cliente Num.65	2007-03-27	[A. Automatico] Factura Cliente Num.65	234	0
1242	[A. Automatico] Factura Cliente Num.67	2007-03-27	[A. Automatico] Factura Cliente Num.67	235	0
1243	[A. Automatico] Factura Cliente Num.69	2007-03-27	[A. Automatico] Factura Cliente Num.69	236	0
1244	[A. Automatico] Factura Cliente Num.88	2007-04-23	[A. Automatico] Factura Cliente Num.88	237	0
1266	[A. Automatico] Factura Cliente Num.93	2007-04-25	[A. Automatico] Factura Cliente Num.93	259	0
1267	[A. Automatico] Factura Cliente Num.95	2007-05-03	[A. Automatico] Factura Cliente Num.95	260	0
1163	[A. Automatico] Factura Cliente Num.110	2007-05-09	[A. Automatico] Factura Cliente Num.110	156	0
1268	[A. Automatico] Factura Cliente Num.100	2007-05-08	[A. Automatico] Factura Cliente Num.100	261	0
1269	[A. Automatico] Factura Cliente Num.123	2007-05-25	[A. Automatico] Factura Cliente Num.123	262	0
1270	[A. Automatico] Factura Cliente Num.124	2007-05-25	[A. Automatico] Factura Cliente Num.124	263	0
1271	[A. Automatico] Factura Cliente Num.98	2007-05-07	[A. Automatico] Factura Cliente Num.98	264	0
1164	[A. Automatico] Factura Cliente Num.40	2007-02-23	[A. Automatico] Factura Cliente Num.40	157	0
1272	[A. Automatico] Factura Cliente Num.128	2007-07-11	[A. Automatico] Factura Cliente Num.128	265	0
1165	[A. Automatico] Factura Cliente Num.129	2007-09-14	[A. Automatico] Factura Cliente Num.129	158	0
1166	[A. Automatico] Factura Cliente Num.122	2007-05-25	[A. Automatico] Factura Cliente Num.122	159	0
1191	[A. Automatico] Factura Cliente Num.29	2007-02-22	[A. Automatico] Factura Cliente Num.29	184	0
1238	[A. Automatico] Factura Cliente Num.99	2007-05-08	[A. Automatico] Factura Cliente Num.99	231	0
1239	[A. Automatico] Factura Cliente Num.102	2007-05-08	[A. Automatico] Factura Cliente Num.102	232	0
1273	[A. Automatico] Factura Cliente Num.56	2007-03-20	[A. Automatico] Factura Cliente Num.56	266	0
1274	[A. Automatico] Factura Cliente Num.5	2007-01-30	[A. Automatico] Factura Cliente Num.5	267	0
1167	[A. Automatico] Factura Cliente Num.107	2007-05-09	[A. Automatico] Factura Cliente Num.107	160	0
1168	[A. Automatico] Factura Cliente Num.1	2007-01-15	[A. Automatico] Factura Cliente Num.1	161	0
1169	[A. Automatico] Factura Cliente Num.4	2007-01-30	[A. Automatico] Factura Cliente Num.4	162	0
1171	[A. Automatico] Factura Cliente Num.7	2007-01-31	[A. Automatico] Factura Cliente Num.7	164	0
1172	[A. Automatico] Factura Cliente Num.15	2007-01-31	[A. Automatico] Factura Cliente Num.15	165	0
1173	[A. Automatico] Factura Cliente Num.18	2007-01-31	[A. Automatico] Factura Cliente Num.18	166	0
1174	[A. Automatico] Factura Cliente Num.13	2007-01-31	[A. Automatico] Factura Cliente Num.13	167	0
1175	[A. Automatico] Factura Cliente Num.17	2007-01-31	[A. Automatico] Factura Cliente Num.17	168	0
1176	[A. Automatico] Factura Cliente Num.16	2007-01-31	[A. Automatico] Factura Cliente Num.16	169	0
1177	[A. Automatico] Factura Cliente Num.8	2007-01-31	[A. Automatico] Factura Cliente Num.8	170	0
1178	[A. Automatico] Factura Cliente Num.14	2007-01-31	[A. Automatico] Factura Cliente Num.14	171	0
1179	[A. Automatico] Factura Cliente Num.19	2007-01-31	[A. Automatico] Factura Cliente Num.19	172	0
1170	[A. Automatico] Factura Cliente Num.6	2007-01-31	[A. Automatico] Factura Cliente Num.6	163	0
1275	[A. Automatico] Factura Cliente Num.23	2007-02-02	[A. Automatico] Factura Cliente Num.23	268	0
1276	[A. Automatico] Factura Cliente Num.24	2007-02-02	[A. Automatico] Factura Cliente Num.24	269	0
1277	[A. Automatico] Factura Cliente Num.26	2007-02-13	[A. Automatico] Factura Cliente Num.26	270	0
1180	[A. Automatico] Factura Cliente Num.20	2007-02-02	[A. Automatico] Factura Cliente Num.20	173	0
1181	[A. Automatico] Factura Cliente Num.21	2007-02-02	[A. Automatico] Factura Cliente Num.21	174	0
1182	[A. Automatico] Factura Cliente Num.25	2007-02-12	[A. Automatico] Factura Cliente Num.25	175	0
1183	[A. Automatico] Factura Cliente Num.28	2007-02-19	[A. Automatico] Factura Cliente Num.28	176	0
1184	[A. Automatico] Factura Cliente Num.9	2007-01-31	[A. Automatico] Factura Cliente Num.9	177	0
1278	[A. Automatico] Factura Cliente Num.10	2007-01-31	[A. Automatico] Factura Cliente Num.10	271	0
1279	[A. Automatico] Factura Cliente Num.11	2007-01-31	[A. Automatico] Factura Cliente Num.11	272	0
1280	[A. Automatico] Factura Cliente Num.125	2007-05-28	[A. Automatico] Factura Cliente Num.125	273	0
1007	[A. Automatico] Cobro VBDXNN	2007-01-17	[A. Automatico] Cobro VBDXNN	1	0
1008	[A. Automatico] Cobro GQGKTW	2007-01-15	[A. Automatico] Cobro GQGKTW	2	0
1009	[A. Automatico] Cobro OARTCQ	2007-02-05	[A. Automatico] Cobro OARTCQ	3	0
1010	[A. Automatico] Cobro KPPHUK	2007-02-05	[A. Automatico] Cobro KPPHUK	4	0
1011	[A. Automatico] Cobro SLRASN	2007-02-05	[A. Automatico] Cobro SLRASN	5	0
1012	[A. Automatico] Cobro BPRRQJ	2007-02-05	[A. Automatico] Cobro BPRRQJ	6	0
1013	[A. Automatico] Cobro XLTDFR	2007-02-05	[A. Automatico] Cobro XLTDFR	7	0
1014	[A. Automatico] Cobro GQGKTW	2007-01-15	[A. Automatico] Cobro GQGKTW	8	0
1015	[A. Automatico] Cobro RKSCOE	2007-02-12	[A. Automatico] Cobro RKSCOE	9	0
1016	[A. Automatico] Cobro YKQTLE	2007-02-12	[A. Automatico] Cobro YKQTLE	10	0
1017	[A. Automatico] Cobro XWEVQD	2007-01-31	[A. Automatico] Cobro XWEVQD	11	0
1018	[A. Automatico] Cobro FJFSVG	2006-10-27	[A. Automatico] Cobro FJFSVG	1	0
1019	[A. Automatico] Cobro FJFSVG	2007-02-22	[A. Automatico] Cobro FJFSVG	12	0
1020	[A. Automatico] Cobro YZOBMY	2007-01-29	[A. Automatico] Cobro YZOBMY	13	0
1021	[A. Automatico] Cobro LDBTEN	2007-02-23	[A. Automatico] Cobro LDBTEN	14	0
1022	[A. Automatico] Cobro TJEQQB	2007-02-23	[A. Automatico] Cobro TJEQQB	15	0
1023	[A. Automatico] Cobro PTHRMX	2007-02-23	[A. Automatico] Cobro PTHRMX	16	0
1024	[A. Automatico] Cobro KBUUFV	2007-02-27	[A. Automatico] Cobro KBUUFV	17	0
1025	[A. Automatico] Cobro R[WFFL	2007-02-27	[A. Automatico] Cobro R[WFFL	18	0
1026	[A. Automatico] Cobro MGKWYW	2007-02-27	[A. Automatico] Cobro MGKWYW	19	0
1027	[A. Automatico] Cobro XSZHYS	2007-03-05	[A. Automatico] Cobro XSZHYS	20	0
1028	[A. Automatico] Cobro RFMSHL	2007-03-06	[A. Automatico] Cobro RFMSHL	21	0
1029	[A. Automatico] Cobro XMTMXG	2007-03-01	[A. Automatico] Cobro XMTMXG	22	0
1030	[A. Automatico] Cobro JIZCRY	2007-03-08	[A. Automatico] Cobro JIZCRY	23	0
1031	[A. Automatico] Cobro [MJXDT	2007-02-28	[A. Automatico] Cobro [MJXDT	24	0
1032	[A. Automatico] Cobro WKNSLS	2007-02-28	[A. Automatico] Cobro WKNSLS	25	0
1033	[A. Automatico] Cobro BHMMAZ	2007-02-28	[A. Automatico] Cobro BHMMAZ	26	0
1034	[A. Automatico] Cobro BWRKUG	2007-02-28	[A. Automatico] Cobro BWRKUG	27	0
1035	[A. Automatico] Cobro MFROGY	2007-02-28	[A. Automatico] Cobro MFROGY	28	0
1036	[A. Automatico] Cobro SWELET	2007-02-28	[A. Automatico] Cobro SWELET	29	0
1037	[A. Automatico] Cobro LVVDTM	2007-02-28	[A. Automatico] Cobro LVVDTM	30	0
1038	[A. Automatico] Cobro XOYIET	2007-02-28	[A. Automatico] Cobro XOYIET	31	0
1039	[A. Automatico] Cobro HCIVYT	2007-02-28	[A. Automatico] Cobro HCIVYT	32	0
1040	[A. Automatico] Cobro XHKBSU	2007-02-28	[A. Automatico] Cobro XHKBSU	33	0
1041	[A. Automatico] Cobro GFUNYR	2007-03-20	[A. Automatico] Cobro GFUNYR	34	0
1042	[A. Automatico] Cobro GITZDY	2007-03-20	[A. Automatico] Cobro GITZDY	35	0
1043	[A. Automatico] Cobro VVSOHG	2007-03-29	[A. Automatico] Cobro VVSOHG	36	0
1044	[A. Automatico] Cobro SKAKPN	2007-03-28	[A. Automatico] Cobro SKAKPN	37	0
1045	[A. Automatico] Cobro IQSXKB	2007-03-29	[A. Automatico] Cobro IQSXKB	38	0
1046	[A. Automatico] Cobro SIWRWI	2007-03-29	[A. Automatico] Cobro SIWRWI	39	0
1047	[A. Automatico] Cobro VDRMHJ2	2007-03-29	[A. Automatico] Cobro VDRMHJ2	40	0
1048	[A. Automatico] Cobro QENLSH1	2007-03-29	[A. Automatico] Cobro QENLSH1	41	0
1049	[A. Automatico] Cobro QENLSH	2007-03-29	[A. Automatico] Cobro QENLSH	42	0
1050	[A. Automatico] Cobro KZGVJG	2007-03-29	[A. Automatico] Cobro KZGVJG	43	0
1051	[A. Automatico] Cobro RJGEKI	2007-03-29	[A. Automatico] Cobro RJGEKI	44	0
1052	[A. Automatico] Cobro WVYAUN	2007-03-29	[A. Automatico] Cobro WVYAUN	45	0
1053	[A. Automatico] Cobro PVEZPK	2007-03-29	[A. Automatico] Cobro PVEZPK	46	0
1054	[A. Automatico] Cobro SGPNZS	2007-03-29	[A. Automatico] Cobro SGPNZS	47	0
1055	[A. Automatico] Cobro ECEHFZ	2007-03-29	[A. Automatico] Cobro ECEHFZ	48	0
1056	[A. Automatico] Cobro HJKUGG	2007-03-29	[A. Automatico] Cobro HJKUGG	49	0
1057	[A. Automatico] Cobro ZCPBFD	2007-03-29	[A. Automatico] Cobro ZCPBFD	50	0
1058	[A. Automatico] Cobro DQCDPV	2007-03-29	[A. Automatico] Cobro DQCDPV	51	0
1059	[A. Automatico] Cobro FTLK[Q	2007-03-29	[A. Automatico] Cobro FTLK[Q	52	0
1060	[A. Automatico] Cobro CMNXZZ	2007-03-29	[A. Automatico] Cobro CMNXZZ	53	0
1061	[A. Automatico] Cobro ESWOMB	2007-03-29	[A. Automatico] Cobro ESWOMB	54	0
1062	[A. Automatico] Cobro QYIBYU	2007-04-02	[A. Automatico] Cobro QYIBYU	55	0
1063	[A. Automatico] Cobro XQBOOR	2007-04-03	[A. Automatico] Cobro XQBOOR	56	0
1064	[A. Automatico] Cobro [VYFS[	2007-04-03	[A. Automatico] Cobro [VYFS[	57	0
1065	[A. Automatico] Cobro YQPCIE	2007-03-30	[A. Automatico] Cobro YQPCIE	58	0
1066	[A. Automatico] Cobro IVXVYD	2007-05-01	[A. Automatico] Cobro IVXVYD	59	0
1067	[A. Automatico] Cobro CLL[ZS	2007-05-03	[A. Automatico] Cobro CLL[ZS	60	0
1068	[A. Automatico] Cobro CNJXJO	2007-04-27	[A. Automatico] Cobro CNJXJO	61	0
1069	[A. Automatico] Cobro QBVUMK	2007-04-26	[A. Automatico] Cobro QBVUMK	62	0
1070	[A. Automatico] Cobro MYVGGU	2007-04-24	[A. Automatico] Cobro MYVGGU	63	0
1071	[A. Automatico] Cobro QBVUMK	2007-04-20	[A. Automatico] Cobro QBVUMK	64	0
1072	[A. Automatico] Cobro PNWNYA	2007-05-02	[A. Automatico] Cobro PNWNYA	65	0
1073	[A. Automatico] Cobro KBJKOH	2007-05-02	[A. Automatico] Cobro KBJKOH	66	0
1074	[A. Automatico] Cobro OIWXYB	2007-04-30	[A. Automatico] Cobro OIWXYB	67	0
1075	[A. Automatico] Cobro NUYUXO	2007-04-30	[A. Automatico] Cobro NUYUXO	68	0
1076	[A. Automatico] Cobro YTWDIY	2007-04-30	[A. Automatico] Cobro YTWDIY	69	0
1077	[A. Automatico] Cobro UAWJDP	2007-04-30	[A. Automatico] Cobro UAWJDP	70	0
1078	[A. Automatico] Cobro INLXZA	2007-05-03	[A. Automatico] Cobro INLXZA	71	0
1079	[A. Automatico] Cobro COAQPM	2007-03-31	[A. Automatico] Cobro COAQPM	72	0
1080	[A. Automatico] Cobro ZRDELL	2007-03-31	[A. Automatico] Cobro ZRDELL	73	0
1081	[A. Automatico] Cobro AXDFEF	2007-03-30	[A. Automatico] Cobro AXDFEF	74	0
1082	[A. Automatico] Cobro WHRNDO	2007-03-17	[A. Automatico] Cobro WHRNDO	75	0
1083	[A. Automatico] Cobro WVYAUN	2007-03-29	[A. Automatico] Cobro WVYAUN	76	0
1084	[A. Automatico] Cobro HGOYZQ	2007-03-17	[A. Automatico] Cobro HGOYZQ	77	0
1085	[A. Automatico] Cobro YIKBD[	2007-02-19	[A. Automatico] Cobro YIKBD[	78	0
1086	[A. Automatico] Cobro JXKSHV	2007-03-29	[A. Automatico] Cobro JXKSHV	79	0
1087	[A. Automatico] Cobro JXKSHV	2007-03-29	[A. Automatico] Cobro JXKSHV	80	0
1088	[A. Automatico] Cobro CJIKTG	2007-05-09	[A. Automatico] Cobro CJIKTG	81	0
1089	[A. Automatico] Cobro UNIIYN	2007-05-11	[A. Automatico] Cobro UNIIYN	82	0
1090	[A. Automatico] Cobro LRMBBX	2007-05-11	[A. Automatico] Cobro LRMBBX	83	0
1091	[A. Automatico] Cobro KQTYYO	2007-05-11	[A. Automatico] Cobro KQTYYO	84	0
1092	[A. Automatico] Cobro EFEGZF	2007-04-12	[A. Automatico] Cobro EFEGZF	85	0
1093	[A. Automatico] Cobro GWCFSO	2007-05-11	[A. Automatico] Cobro GWCFSO	86	0
1094	[A. Automatico] Cobro DOOEWQ	2007-05-11	[A. Automatico] Cobro DOOEWQ	87	0
1095	[A. Automatico] Cobro KUHKXO	2007-05-11	[A. Automatico] Cobro KUHKXO	88	0
1096	[A. Automatico] Cobro TYZ[VR	2007-08-14	[A. Automatico] Cobro TYZ[VR	89	0
1097	[A. Automatico] Cobro LQIDBZ	2007-03-14	[A. Automatico] Cobro LQIDBZ	90	0
1098	[A. Automatico] Cobro HGMRII	2007-05-24	[A. Automatico] Cobro HGMRII	91	0
1099	[A. Automatico] Cobro EIY[ED	2007-05-24	[A. Automatico] Cobro EIY[ED	92	0
1100	[A. Automatico] Cobro NGIXEL	2007-05-19	[A. Automatico] Cobro NGIXEL	93	0
1101	[A. Automatico] Cobro FGTPBN	2007-05-07	[A. Automatico] Cobro FGTPBN	94	0
1102	[A. Automatico] Cobro KNMZWF	2007-05-28	[A. Automatico] Cobro KNMZWF	95	0
1103	[A. Automatico] Cobro IFREQT	2007-05-24	[A. Automatico] Cobro IFREQT	96	0
1104	[A. Automatico] Cobro XWZFGT	2007-05-24	[A. Automatico] Cobro XWZFGT	97	0
1105	[A. Automatico] Cobro VYBAMY	2007-05-24	[A. Automatico] Cobro VYBAMY	98	0
1106	[A. Automatico] Cobro NNZNUN	2007-05-25	[A. Automatico] Cobro NNZNUN	99	0
1107	[A. Automatico] Cobro SZYMUR	2007-05-24	[A. Automatico] Cobro SZYMUR	100	0
1108	[A. Automatico] Cobro YCIHXH	2007-02-05	[A. Automatico] Cobro YCIHXH	101	0
\.


--
-- Data for Name: balance; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY balance (idbalance, nombrebalance) FROM stdin;
\.


--
-- Data for Name: binteligente; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY binteligente (idbinteligente, idainteligente, iddiario, fecha, conceptocontable, codcuenta, descripcion, debe, haber, contrapartida, comentario, canal, marcaconciliacion, idc_coste) FROM stdin;
\.


--
-- Data for Name: borrador; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY borrador (idborrador, codigoborrador, idasiento, iddiario, fecha, conceptocontable, idcuenta, descripcion, debe, haber, contrapartida, comentario, idcanal, marcaconciliacion, idc_coste, idapunte, idtipoiva, orden) FROM stdin;
26733	\N	1197	\N	2007-03-20	Factura Cliente	10318	Factura Cliente Num. 57	0.00	22.81	\N	\N	\N	\N	\N	\N	\N	\N
26734	\N	1197	\N	2007-03-20	Factura Cliente	10877	Factura Cliente Num. 57	165.39	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26735	\N	1197	\N	2007-03-20	Factura Cliente	10534	Factura Cliente Num. 57	0.00	142.58	\N	\N	\N	\N	\N	\N	\N	\N
26835	\N	1217	\N	2007-04-20	Factura Cliente	10318	Factura Cliente Num. 82	0.00	123.49	\N	\N	\N	\N	\N	\N	\N	\N
26836	\N	1217	\N	2007-04-20	Factura Cliente	10820	Factura Cliente Num. 82	895.33	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26837	\N	1217	\N	2007-04-20	Factura Cliente	10534	Factura Cliente Num. 82	0.00	771.84	\N	\N	\N	\N	\N	\N	\N	\N
26937	\N	1242	\N	2007-03-27	Factura Cliente	10318	Factura Cliente Num. 67	0.00	19.30	\N	\N	\N	\N	\N	\N	\N	\N
26938	\N	1242	\N	2007-03-27	Factura Cliente	10864	Factura Cliente Num. 67	139.90	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26939	\N	1242	\N	2007-03-27	Factura Cliente	10534	Factura Cliente Num. 67	0.00	120.60	\N	\N	\N	\N	\N	\N	\N	\N
27039	\N	1277	\N	2007-02-13	Factura Cliente	10318	Factura Cliente Num. 26	0.00	14.86	\N	\N	\N	\N	\N	\N	\N	\N
27040	\N	1277	\N	2007-02-13	Factura Cliente	10802	Factura Cliente Num. 26	107.71	0.00	\N	\N	\N	\N	\N	\N	\N	\N
27041	\N	1277	\N	2007-02-13	Factura Cliente	10534	Factura Cliente Num. 26	0.00	92.85	\N	\N	\N	\N	\N	\N	\N	\N
26736	\N	1198	\N	2007-03-08	Factura Cliente	10318	Factura Cliente Num. 49	0.00	17.15	\N	\N	\N	\N	\N	\N	\N	\N
26737	\N	1198	\N	2007-03-08	Factura Cliente	10885	Factura Cliente Num. 49	124.35	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26738	\N	1198	\N	2007-03-08	Factura Cliente	10534	Factura Cliente Num. 49	0.00	107.20	\N	\N	\N	\N	\N	\N	\N	\N
26838	\N	1218	\N	2007-02-22	Factura Cliente	10318	Factura Cliente Num. 31	0.00	101.44	\N	\N	\N	\N	\N	\N	\N	\N
26839	\N	1218	\N	2007-02-22	Factura Cliente	10877	Factura Cliente Num. 31	735.47	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26840	\N	1218	\N	2007-02-22	Factura Cliente	10534	Factura Cliente Num. 31	0.00	634.03	\N	\N	\N	\N	\N	\N	\N	\N
26940	\N	1243	\N	2007-03-27	Factura Cliente	10318	Factura Cliente Num. 69	0.00	8.58	\N	\N	\N	\N	\N	\N	\N	\N
26941	\N	1243	\N	2007-03-27	Factura Cliente	10865	Factura Cliente Num. 69	62.18	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26942	\N	1243	\N	2007-03-27	Factura Cliente	10534	Factura Cliente Num. 69	0.00	53.60	\N	\N	\N	\N	\N	\N	\N	\N
27042	\N	1180	\N	2007-02-02	Factura Cliente	10318	Factura Cliente Num. 20	0.00	277.18	\N	\N	\N	\N	\N	\N	\N	\N
27043	\N	1180	\N	2007-02-02	Factura Cliente	10807	Factura Cliente Num. 20	2009.57	0.00	\N	\N	\N	\N	\N	\N	\N	\N
27044	\N	1180	\N	2007-02-02	Factura Cliente	10534	Factura Cliente Num. 20	0.00	1732.39	\N	\N	\N	\N	\N	\N	\N	\N
26739	\N	1199	\N	2007-03-14	Factura Cliente	10318	Factura Cliente Num. 53	0.00	30.02	\N	\N	\N	\N	\N	\N	\N	\N
26740	\N	1199	\N	2007-03-14	Factura Cliente	10806	Factura Cliente Num. 53	217.62	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26741	\N	1199	\N	2007-03-14	Factura Cliente	10534	Factura Cliente Num. 53	0.00	187.60	\N	\N	\N	\N	\N	\N	\N	\N
26841	\N	1219	\N	2007-02-22	Factura Cliente	10318	Factura Cliente Num. 30	0.00	13.43	\N	\N	\N	\N	\N	\N	\N	\N
26842	\N	1219	\N	2007-02-22	Factura Cliente	10877	Factura Cliente Num. 30	97.38	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26843	\N	1219	\N	2007-02-22	Factura Cliente	10534	Factura Cliente Num. 30	0.00	83.95	\N	\N	\N	\N	\N	\N	\N	\N
26943	\N	1244	\N	2007-04-23	Factura Cliente	10318	Factura Cliente Num. 88	0.00	16.17	\N	\N	\N	\N	\N	\N	\N	\N
26944	\N	1244	\N	2007-04-23	Factura Cliente	10877	Factura Cliente Num. 88	117.23	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26945	\N	1244	\N	2007-04-23	Factura Cliente	10534	Factura Cliente Num. 88	0.00	101.06	\N	\N	\N	\N	\N	\N	\N	\N
27045	\N	1181	\N	2007-02-02	Factura Cliente	10318	Factura Cliente Num. 21	0.00	42.88	\N	\N	\N	\N	\N	\N	\N	\N
27046	\N	1181	\N	2007-02-02	Factura Cliente	10807	Factura Cliente Num. 21	310.88	0.00	\N	\N	\N	\N	\N	\N	\N	\N
27047	\N	1181	\N	2007-02-02	Factura Cliente	10534	Factura Cliente Num. 21	0.00	268.00	\N	\N	\N	\N	\N	\N	\N	\N
23866	\N	1109	\N	2007-01-10	Pago	10935	Pago BTURJT	151.73	0.00	\N	\N	\N	\N	\N	\N	\N	\N
23867	\N	1109	\N	2007-01-10	Pago	10943	Pago BTURJT	0.00	151.73	\N	\N	\N	\N	\N	\N	\N	\N
23868	\N	1110	\N	2007-01-26	Pago	10912	Pago VSHKXW	37.50	0.00	\N	\N	\N	\N	\N	\N	\N	\N
23869	\N	1110	\N	2007-01-26	Pago	10943	Pago VSHKXW	0.00	37.50	\N	\N	\N	\N	\N	\N	\N	\N
23870	\N	1111	\N	2007-01-26	Pago	10912	Pago UVHYFI	28.19	0.00	\N	\N	\N	\N	\N	\N	\N	\N
23871	\N	1111	\N	2007-01-26	Pago	10943	Pago UVHYFI	0.00	28.19	\N	\N	\N	\N	\N	\N	\N	\N
23872	\N	1112	\N	2007-02-01	Pago	10912	Pago POVHXI	42.74	0.00	\N	\N	\N	\N	\N	\N	\N	\N
23873	\N	1112	\N	2007-02-01	Pago	10943	Pago POVHXI	0.00	42.74	\N	\N	\N	\N	\N	\N	\N	\N
23874	\N	1113	\N	2007-01-11	Pago	10914	Pago YTGOLI	1180.47	0.00	\N	\N	\N	\N	\N	\N	\N	\N
23875	\N	1113	\N	2007-01-11	Pago	10943	Pago YTGOLI	0.00	1180.47	\N	\N	\N	\N	\N	\N	\N	\N
23876	\N	1114	\N	2007-01-18	Pago	10931	Pago OZDCTQ	250.04	0.00	\N	\N	\N	\N	\N	\N	\N	\N
23877	\N	1114	\N	2007-01-18	Pago	10943	Pago OZDCTQ	0.00	250.04	\N	\N	\N	\N	\N	\N	\N	\N
23878	\N	1115	\N	2007-02-22	Pago	10935	Pago XFHYRL	412.88	0.00	\N	\N	\N	\N	\N	\N	\N	\N
23879	\N	1115	\N	2007-02-22	Pago	10943	Pago XFHYRL	0.00	412.88	\N	\N	\N	\N	\N	\N	\N	\N
23880	\N	1116	\N	2007-02-09	Pago	10935	Pago FBWSQS	50.53	0.00	\N	\N	\N	\N	\N	\N	\N	\N
23881	\N	1116	\N	2007-02-09	Pago	10943	Pago FBWSQS	0.00	50.53	\N	\N	\N	\N	\N	\N	\N	\N
23882	\N	1117	\N	2007-03-14	Pago	10935	Pago TKGFJV	66.91	0.00	\N	\N	\N	\N	\N	\N	\N	\N
23883	\N	1117	\N	2007-03-14	Pago	10943	Pago TKGFJV	0.00	66.91	\N	\N	\N	\N	\N	\N	\N	\N
23884	\N	1118	\N	2007-03-09	Pago	10912	Pago ZHMULP	413.18	0.00	\N	\N	\N	\N	\N	\N	\N	\N
23885	\N	1118	\N	2007-03-09	Pago	10943	Pago ZHMULP	0.00	413.18	\N	\N	\N	\N	\N	\N	\N	\N
23886	\N	1119	\N	2007-03-09	Pago	10912	Pago DMJYLP	1488.64	0.00	\N	\N	\N	\N	\N	\N	\N	\N
23887	\N	1119	\N	2007-03-09	Pago	10943	Pago DMJYLP	0.00	1488.64	\N	\N	\N	\N	\N	\N	\N	\N
23888	\N	1120	\N	2007-03-02	Pago	10935	Pago UVRHKP	79.26	0.00	\N	\N	\N	\N	\N	\N	\N	\N
23889	\N	1120	\N	2007-03-02	Pago	10943	Pago UVRHKP	0.00	79.26	\N	\N	\N	\N	\N	\N	\N	\N
23890	\N	1121	\N	2007-02-21	Pago	10912	Pago TBSQGT	464.46	0.00	\N	\N	\N	\N	\N	\N	\N	\N
23891	\N	1121	\N	2007-02-21	Pago	10943	Pago TBSQGT	0.00	464.46	\N	\N	\N	\N	\N	\N	\N	\N
23892	\N	1122	\N	2007-02-22	Pago	10912	Pago XZINVQ	575.91	0.00	\N	\N	\N	\N	\N	\N	\N	\N
23893	\N	1122	\N	2007-02-22	Pago	10943	Pago XZINVQ	0.00	575.91	\N	\N	\N	\N	\N	\N	\N	\N
23894	\N	1123	\N	2007-02-15	Pago	10912	Pago MYCVWN	2558.88	0.00	\N	\N	\N	\N	\N	\N	\N	\N
23895	\N	1123	\N	2007-02-15	Pago	10943	Pago MYCVWN	0.00	2558.88	\N	\N	\N	\N	\N	\N	\N	\N
23896	\N	1124	\N	2007-02-21	Pago	10912	Pago LGTMEC	3.54	0.00	\N	\N	\N	\N	\N	\N	\N	\N
23897	\N	1124	\N	2007-02-21	Pago	10943	Pago LGTMEC	0.00	3.54	\N	\N	\N	\N	\N	\N	\N	\N
23898	\N	1125	\N	2007-02-17	Pago	10912	Pago GYXXFH	198.34	0.00	\N	\N	\N	\N	\N	\N	\N	\N
23899	\N	1125	\N	2007-02-17	Pago	10943	Pago GYXXFH	0.00	198.34	\N	\N	\N	\N	\N	\N	\N	\N
23900	\N	1126	\N	2007-03-02	Pago	10912	Pago MYIEOO	20.80	0.00	\N	\N	\N	\N	\N	\N	\N	\N
23901	\N	1126	\N	2007-03-02	Pago	10943	Pago MYIEOO	0.00	20.80	\N	\N	\N	\N	\N	\N	\N	\N
23902	\N	1127	\N	2007-02-08	Pago	10912	Pago XLNFYI	191.95	0.00	\N	\N	\N	\N	\N	\N	\N	\N
23903	\N	1127	\N	2007-02-08	Pago	10943	Pago XLNFYI	0.00	191.95	\N	\N	\N	\N	\N	\N	\N	\N
23904	\N	1128	\N	2007-02-02	Pago	10934	Pago VLGXGD	49.08	0.00	\N	\N	\N	\N	\N	\N	\N	\N
23905	\N	1128	\N	2007-02-02	Pago	10943	Pago VLGXGD	0.00	49.08	\N	\N	\N	\N	\N	\N	\N	\N
23906	\N	1129	\N	2007-01-09	Pago	10935	Pago WSNBXT	407.62	0.00	\N	\N	\N	\N	\N	\N	\N	\N
23907	\N	1129	\N	2007-01-09	Pago	10943	Pago WSNBXT	0.00	407.62	\N	\N	\N	\N	\N	\N	\N	\N
23908	\N	1130	\N	2007-02-06	Pago	10912	Pago KRICCN	80.92	0.00	\N	\N	\N	\N	\N	\N	\N	\N
23909	\N	1130	\N	2007-02-06	Pago	10943	Pago KRICCN	0.00	80.92	\N	\N	\N	\N	\N	\N	\N	\N
23910	\N	1131	\N	2007-01-23	Pago	10923	Pago EJLBGQ	135.27	0.00	\N	\N	\N	\N	\N	\N	\N	\N
23911	\N	1131	\N	2007-01-23	Pago	10943	Pago EJLBGQ	0.00	135.27	\N	\N	\N	\N	\N	\N	\N	\N
23912	\N	1132	\N	2007-03-03	Pago	10912	Pago ISOPVD	407.05	0.00	\N	\N	\N	\N	\N	\N	\N	\N
23913	\N	1132	\N	2007-03-03	Pago	10943	Pago ISOPVD	0.00	407.05	\N	\N	\N	\N	\N	\N	\N	\N
23914	\N	1133	\N	2007-02-23	Pago	10923	Pago MKMQTS	156.76	0.00	\N	\N	\N	\N	\N	\N	\N	\N
23915	\N	1133	\N	2007-02-23	Pago	10943	Pago MKMQTS	0.00	156.76	\N	\N	\N	\N	\N	\N	\N	\N
23916	\N	1134	\N	2007-03-21	Pago	10912	Pago VIPUQY	296.14	0.00	\N	\N	\N	\N	\N	\N	\N	\N
23917	\N	1134	\N	2007-03-21	Pago	10943	Pago VIPUQY	0.00	296.14	\N	\N	\N	\N	\N	\N	\N	\N
23918	\N	1135	\N	2007-03-20	Pago	10935	Pago QSCZIJ	154.74	0.00	\N	\N	\N	\N	\N	\N	\N	\N
23919	\N	1135	\N	2007-03-20	Pago	10943	Pago QSCZIJ	0.00	154.74	\N	\N	\N	\N	\N	\N	\N	\N
23920	\N	1136	\N	2007-03-28	Pago	10912	Pago PUFJNS	36.21	0.00	\N	\N	\N	\N	\N	\N	\N	\N
23921	\N	1136	\N	2007-03-28	Pago	10943	Pago PUFJNS	0.00	36.21	\N	\N	\N	\N	\N	\N	\N	\N
23922	\N	1137	\N	2007-03-30	Pago	10912	Pago XRQMGX	214.30	0.00	\N	\N	\N	\N	\N	\N	\N	\N
23923	\N	1137	\N	2007-03-30	Pago	10943	Pago XRQMGX	0.00	214.30	\N	\N	\N	\N	\N	\N	\N	\N
23924	\N	1138	\N	2007-03-19	Pago	10933	Pago OVMUKG	606.00	0.00	\N	\N	\N	\N	\N	\N	\N	\N
23925	\N	1138	\N	2007-03-19	Pago	10943	Pago OVMUKG	0.00	606.00	\N	\N	\N	\N	\N	\N	\N	\N
23926	\N	1139	\N	2007-05-17	Pago	10912	Pago [WCFQE	220.98	0.00	\N	\N	\N	\N	\N	\N	\N	\N
23927	\N	1139	\N	2007-05-17	Pago	10943	Pago [WCFQE	0.00	220.98	\N	\N	\N	\N	\N	\N	\N	\N
23928	\N	1140	\N	2007-01-27	Pago	10935	Pago WSNBXT	332.69	0.00	\N	\N	\N	\N	\N	\N	\N	\N
23929	\N	1140	\N	2007-01-27	Pago	10936	Pago WSNBXT	0.00	332.69	\N	\N	\N	\N	\N	\N	\N	\N
23930	\N	1141	\N	2007-06-06	Pago	10935	Pago TXBSBZ	132.28	0.00	\N	\N	\N	\N	\N	\N	\N	\N
23931	\N	1141	\N	2007-06-06	Pago	10936	Pago TXBSBZ	0.00	132.28	\N	\N	\N	\N	\N	\N	\N	\N
23932	\N	1142	\N	2007-05-14	Pago	10935	Pago BYFKIO	363.08	0.00	\N	\N	\N	\N	\N	\N	\N	\N
23933	\N	1142	\N	2007-05-14	Pago	10936	Pago BYFKIO	0.00	363.08	\N	\N	\N	\N	\N	\N	\N	\N
23934	\N	1143	\N	2007-05-23	Pago	10935	Pago YX[YNK	187.12	0.00	\N	\N	\N	\N	\N	\N	\N	\N
23935	\N	1143	\N	2007-05-23	Pago	10936	Pago YX[YNK	0.00	187.12	\N	\N	\N	\N	\N	\N	\N	\N
23936	\N	1144	\N	2007-02-12	Pago	10935	Pago TKSLPF	317.45	0.00	\N	\N	\N	\N	\N	\N	\N	\N
23937	\N	1144	\N	2007-02-12	Pago	10936	Pago TKSLPF	0.00	317.45	\N	\N	\N	\N	\N	\N	\N	\N
23938	\N	1145	\N	2007-05-25	Pago	10935	Pago UXVMXB	616.17	0.00	\N	\N	\N	\N	\N	\N	\N	\N
23939	\N	1145	\N	2007-05-25	Pago	10936	Pago UXVMXB	0.00	616.17	\N	\N	\N	\N	\N	\N	\N	\N
23940	\N	1146	\N	2007-06-07	Pago	10935	Pago UQYVIZ	72.89	0.00	\N	\N	\N	\N	\N	\N	\N	\N
23941	\N	1146	\N	2007-06-07	Pago	10936	Pago UQYVIZ	0.00	72.89	\N	\N	\N	\N	\N	\N	\N	\N
23942	\N	1147	\N	2007-05-04	Pago	10935	Pago VJXZTF	162.11	0.00	\N	\N	\N	\N	\N	\N	\N	\N
23943	\N	1147	\N	2007-05-04	Pago	10936	Pago VJXZTF	0.00	162.11	\N	\N	\N	\N	\N	\N	\N	\N
23944	\N	1148	\N	2007-05-04	Pago	10935	Pago ORDNOQ	316.16	0.00	\N	\N	\N	\N	\N	\N	\N	\N
23945	\N	1148	\N	2007-05-04	Pago	10936	Pago ORDNOQ	0.00	316.16	\N	\N	\N	\N	\N	\N	\N	\N
23946	\N	1149	\N	2007-05-21	Pago	10935	Pago YXQVED	66.91	0.00	\N	\N	\N	\N	\N	\N	\N	\N
23947	\N	1149	\N	2007-05-21	Pago	10936	Pago YXQVED	0.00	66.91	\N	\N	\N	\N	\N	\N	\N	\N
23948	\N	1150	\N	2007-05-21	Pago	10935	Pago OYAIKY	66.91	0.00	\N	\N	\N	\N	\N	\N	\N	\N
23949	\N	1150	\N	2007-05-21	Pago	10936	Pago OYAIKY	0.00	66.91	\N	\N	\N	\N	\N	\N	\N	\N
23950	\N	1151	\N	2007-06-06	Pago	10926	Pago JEOHZW	92.94	0.00	\N	\N	\N	\N	\N	\N	\N	\N
23951	\N	1151	\N	2007-06-06	Pago	10936	Pago JEOHZW	0.00	92.94	\N	\N	\N	\N	\N	\N	\N	\N
23952	\N	1152	\N	2007-08-13	Pago	10918	Pago 111	1500.00	0.00	\N	\N	\N	\N	\N	\N	\N	\N
23953	\N	1152	\N	2007-08-13	Pago	10943	Pago 111	0.00	1500.00	\N	\N	\N	\N	\N	\N	\N	\N
23954	\N	1153	\N	2007-08-14	Pago	10912	Pago UMQIBH	122.00	0.00	\N	\N	\N	\N	\N	\N	\N	\N
23955	\N	1153	\N	2007-08-14	Pago	10943	Pago UMQIBH	0.00	122.00	\N	\N	\N	\N	\N	\N	\N	\N
23956	\N	1154	\N	2007-08-31	Pago	10931	Pago SBDNFG	456.00	0.00	\N	\N	\N	\N	\N	\N	\N	\N
23957	\N	1154	\N	2007-08-31	Pago	10938	Pago SBDNFG	0.00	456.00	\N	\N	\N	\N	\N	\N	\N	\N
26742	\N	1200	\N	2007-03-14	Factura Cliente	10318	Factura Cliente Num. 52	0.00	34.58	\N	\N	\N	\N	\N	\N	\N	\N
26743	\N	1200	\N	2007-03-14	Factura Cliente	10806	Factura Cliente Num. 52	250.72	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26744	\N	1200	\N	2007-03-14	Factura Cliente	10534	Factura Cliente Num. 52	0.00	216.14	\N	\N	\N	\N	\N	\N	\N	\N
26844	\N	1220	\N	2007-04-26	Factura Cliente	10318	Factura Cliente Num. 94	0.00	107.20	\N	\N	\N	\N	\N	\N	\N	\N
26845	\N	1220	\N	2007-04-26	Factura Cliente	10890	Factura Cliente Num. 94	777.20	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26846	\N	1220	\N	2007-04-26	Factura Cliente	10534	Factura Cliente Num. 94	0.00	670.00	\N	\N	\N	\N	\N	\N	\N	\N
26946	\N	1266	\N	2007-04-25	Factura Cliente	10318	Factura Cliente Num. 93	0.00	107.20	\N	\N	\N	\N	\N	\N	\N	\N
26947	\N	1266	\N	2007-04-25	Factura Cliente	10858	Factura Cliente Num. 93	777.20	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26948	\N	1266	\N	2007-04-25	Factura Cliente	10534	Factura Cliente Num. 93	0.00	670.00	\N	\N	\N	\N	\N	\N	\N	\N
27048	\N	1182	\N	2007-02-12	Factura Cliente	10318	Factura Cliente Num. 25	0.00	5.78	\N	\N	\N	\N	\N	\N	\N	\N
27049	\N	1182	\N	2007-02-12	Factura Cliente	10896	Factura Cliente Num. 25	41.88	0.00	\N	\N	\N	\N	\N	\N	\N	\N
27050	\N	1182	\N	2007-02-12	Factura Cliente	10534	Factura Cliente Num. 25	0.00	36.10	\N	\N	\N	\N	\N	\N	\N	\N
26745	\N	1201	\N	2007-03-20	Factura Cliente	10318	Factura Cliente Num. 55	0.00	67.18	\N	\N	\N	\N	\N	\N	\N	\N
26746	\N	1201	\N	2007-03-20	Factura Cliente	10820	Factura Cliente Num. 55	487.08	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26747	\N	1201	\N	2007-03-20	Factura Cliente	10534	Factura Cliente Num. 55	0.00	419.90	\N	\N	\N	\N	\N	\N	\N	\N
26847	\N	1255	\N	2007-05-08	Factura Cliente	10318	Factura Cliente Num. 101	0.00	6.43	\N	\N	\N	\N	\N	\N	\N	\N
26848	\N	1255	\N	2007-05-08	Factura Cliente	10868	Factura Cliente Num. 101	46.63	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26849	\N	1255	\N	2007-05-08	Factura Cliente	10534	Factura Cliente Num. 101	0.00	40.20	\N	\N	\N	\N	\N	\N	\N	\N
26949	\N	1267	\N	2007-05-03	Factura Cliente	10318	Factura Cliente Num. 95	0.00	77.18	\N	\N	\N	\N	\N	\N	\N	\N
26950	\N	1267	\N	2007-05-03	Factura Cliente	10889	Factura Cliente Num. 95	559.58	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26951	\N	1267	\N	2007-05-03	Factura Cliente	10534	Factura Cliente Num. 95	0.00	482.40	\N	\N	\N	\N	\N	\N	\N	\N
27051	\N	1183	\N	2007-02-19	Factura Cliente	10318	Factura Cliente Num. 28	0.00	123.49	\N	\N	\N	\N	\N	\N	\N	\N
27052	\N	1183	\N	2007-02-19	Factura Cliente	10820	Factura Cliente Num. 28	895.33	0.00	\N	\N	\N	\N	\N	\N	\N	\N
27053	\N	1183	\N	2007-02-19	Factura Cliente	10534	Factura Cliente Num. 28	0.00	771.84	\N	\N	\N	\N	\N	\N	\N	\N
26748	\N	1202	\N	2007-03-20	Factura Cliente	10318	Factura Cliente Num. 54	0.00	123.49	\N	\N	\N	\N	\N	\N	\N	\N
26749	\N	1202	\N	2007-03-20	Factura Cliente	10820	Factura Cliente Num. 54	895.33	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26750	\N	1202	\N	2007-03-20	Factura Cliente	10534	Factura Cliente Num. 54	0.00	771.84	\N	\N	\N	\N	\N	\N	\N	\N
26850	\N	1256	\N	2007-05-18	Factura Cliente	10318	Factura Cliente Num. 112	0.00	123.49	\N	\N	\N	\N	\N	\N	\N	\N
26851	\N	1256	\N	2007-05-18	Factura Cliente	10820	Factura Cliente Num. 112	895.33	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26852	\N	1256	\N	2007-05-18	Factura Cliente	10534	Factura Cliente Num. 112	0.00	771.84	\N	\N	\N	\N	\N	\N	\N	\N
26952	\N	1163	\N	2007-05-09	Factura Cliente	10318	Factura Cliente Num. 110	0.00	93.68	\N	\N	\N	\N	\N	\N	\N	\N
26953	\N	1163	\N	2007-05-09	Factura Cliente	10881	Factura Cliente Num. 110	679.17	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26954	\N	1163	\N	2007-05-09	Factura Cliente	10534	Factura Cliente Num. 110	0.00	585.49	\N	\N	\N	\N	\N	\N	\N	\N
27054	\N	1184	\N	2007-01-31	Factura Cliente	10318	Factura Cliente Num. 9	0.00	58.96	\N	\N	\N	\N	\N	\N	\N	\N
27055	\N	1184	\N	2007-01-31	Factura Cliente	10878	Factura Cliente Num. 9	427.46	0.00	\N	\N	\N	\N	\N	\N	\N	\N
27056	\N	1184	\N	2007-01-31	Factura Cliente	10534	Factura Cliente Num. 9	0.00	368.50	\N	\N	\N	\N	\N	\N	\N	\N
26751	\N	1247	\N	2007-03-29	Factura Cliente	10318	Factura Cliente Num. 74	0.00	38.91	\N	\N	\N	\N	\N	\N	\N	\N
26752	\N	1247	\N	2007-03-29	Factura Cliente	10806	Factura Cliente Num. 74	282.09	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26753	\N	1247	\N	2007-03-29	Factura Cliente	10534	Factura Cliente Num. 74	0.00	243.18	\N	\N	\N	\N	\N	\N	\N	\N
26853	\N	1257	\N	2007-05-25	Factura Cliente	10318	Factura Cliente Num. 115	0.00	18.71	\N	\N	\N	\N	\N	\N	\N	\N
26854	\N	1257	\N	2007-05-25	Factura Cliente	10879	Factura Cliente Num. 115	135.67	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26855	\N	1257	\N	2007-05-25	Factura Cliente	10534	Factura Cliente Num. 115	0.00	116.96	\N	\N	\N	\N	\N	\N	\N	\N
26955	\N	1268	\N	2007-05-08	Factura Cliente	10318	Factura Cliente Num. 100	0.00	32.16	\N	\N	\N	\N	\N	\N	\N	\N
26956	\N	1268	\N	2007-05-08	Factura Cliente	10888	Factura Cliente Num. 100	233.16	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26957	\N	1268	\N	2007-05-08	Factura Cliente	10534	Factura Cliente Num. 100	0.00	201.00	\N	\N	\N	\N	\N	\N	\N	\N
27057	\N	1278	\N	2007-01-31	Factura Cliente	10318	Factura Cliente Num. 10	0.00	112.35	\N	\N	\N	\N	\N	\N	\N	\N
27058	\N	1278	\N	2007-01-31	Factura Cliente	10806	Factura Cliente Num. 10	814.51	0.00	\N	\N	\N	\N	\N	\N	\N	\N
27059	\N	1278	\N	2007-01-31	Factura Cliente	10534	Factura Cliente Num. 10	0.00	702.16	\N	\N	\N	\N	\N	\N	\N	\N
26754	\N	1248	\N	2007-03-29	Factura Cliente	10318	Factura Cliente Num. 73	0.00	103.98	\N	\N	\N	\N	\N	\N	\N	\N
26755	\N	1248	\N	2007-03-29	Factura Cliente	10806	Factura Cliente Num. 73	753.88	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26756	\N	1248	\N	2007-03-29	Factura Cliente	10534	Factura Cliente Num. 73	0.00	649.90	\N	\N	\N	\N	\N	\N	\N	\N
26856	\N	1258	\N	2007-05-25	Factura Cliente	10318	Factura Cliente Num. 116	0.00	6.43	\N	\N	\N	\N	\N	\N	\N	\N
26857	\N	1258	\N	2007-05-25	Factura Cliente	10830	Factura Cliente Num. 116	46.63	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26858	\N	1258	\N	2007-05-25	Factura Cliente	10534	Factura Cliente Num. 116	0.00	40.20	\N	\N	\N	\N	\N	\N	\N	\N
26958	\N	1269	\N	2007-05-25	Factura Cliente	10318	Factura Cliente Num. 123	0.00	16.90	\N	\N	\N	\N	\N	\N	\N	\N
26959	\N	1269	\N	2007-05-25	Factura Cliente	10877	Factura Cliente Num. 123	122.53	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26960	\N	1269	\N	2007-05-25	Factura Cliente	10534	Factura Cliente Num. 123	0.00	105.63	\N	\N	\N	\N	\N	\N	\N	\N
27060	\N	1279	\N	2007-01-31	Factura Cliente	10318	Factura Cliente Num. 11	0.00	56.34	\N	\N	\N	\N	\N	\N	\N	\N
27061	\N	1279	\N	2007-01-31	Factura Cliente	10881	Factura Cliente Num. 11	408.44	0.00	\N	\N	\N	\N	\N	\N	\N	\N
27062	\N	1279	\N	2007-01-31	Factura Cliente	10534	Factura Cliente Num. 11	0.00	352.10	\N	\N	\N	\N	\N	\N	\N	\N
26757	\N	1249	\N	2007-03-08	Factura Cliente	10318	Factura Cliente Num. 48	0.00	6.43	\N	\N	\N	\N	\N	\N	\N	\N
26758	\N	1249	\N	2007-03-08	Factura Cliente	10830	Factura Cliente Num. 48	46.63	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26759	\N	1249	\N	2007-03-08	Factura Cliente	10534	Factura Cliente Num. 48	0.00	40.20	\N	\N	\N	\N	\N	\N	\N	\N
26859	\N	1259	\N	2007-02-22	Factura Cliente	10318	Factura Cliente Num. 32	0.00	44.04	\N	\N	\N	\N	\N	\N	\N	\N
26860	\N	1259	\N	2007-02-22	Factura Cliente	10878	Factura Cliente Num. 32	319.29	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26861	\N	1259	\N	2007-02-22	Factura Cliente	10534	Factura Cliente Num. 32	0.00	275.25	\N	\N	\N	\N	\N	\N	\N	\N
26961	\N	1270	\N	2007-05-25	Factura Cliente	10318	Factura Cliente Num. 124	0.00	6.33	\N	\N	\N	\N	\N	\N	\N	\N
26962	\N	1270	\N	2007-05-25	Factura Cliente	10900	Factura Cliente Num. 124	45.87	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26963	\N	1270	\N	2007-05-25	Factura Cliente	10534	Factura Cliente Num. 124	0.00	39.54	\N	\N	\N	\N	\N	\N	\N	\N
27063	\N	1280	\N	2007-05-28	Factura Cliente	10318	Factura Cliente Num. 125	0.00	295.82	\N	\N	\N	\N	\N	\N	\N	\N
27064	\N	1280	\N	2007-05-28	Factura Cliente	10881	Factura Cliente Num. 125	2144.70	0.00	\N	\N	\N	\N	\N	\N	\N	\N
27065	\N	1280	\N	2007-05-28	Factura Cliente	10534	Factura Cliente Num. 125	0.00	1848.88	\N	\N	\N	\N	\N	\N	\N	\N
24336	\N	1281	\N	2007-01-18	Factura Proveedor	10308	Factura Proveedor Num. 004	34.49	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24337	\N	1281	\N	2007-01-18	Factura Proveedor	10931	Factura Proveedor Num. 004	0.00	250.04	\N	\N	\N	\N	\N	\N	\N	\N
24338	\N	1281	\N	2007-01-18	Factura Proveedor	10414	Factura Proveedor Num. 004	215.55	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24339	\N	1282	\N	2007-01-16	Factura Proveedor	10308	Factura Proveedor Num. 075-7872324686	7.53	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24340	\N	1282	\N	2007-01-16	Factura Proveedor	10932	Factura Proveedor Num. 075-7872324686	0.00	54.59	\N	\N	\N	\N	\N	\N	\N	\N
24341	\N	1282	\N	2007-01-16	Factura Proveedor	10414	Factura Proveedor Num. 075-7872324686	47.06	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24342	\N	1283	\N	2007-02-07	Factura Proveedor	10308	Factura Proveedor Num. 07/00475	56.95	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24343	\N	1283	\N	2007-02-07	Factura Proveedor	10935	Factura Proveedor Num. 07/00475	0.00	412.88	\N	\N	\N	\N	\N	\N	\N	\N
24344	\N	1283	\N	2007-02-07	Factura Proveedor	10414	Factura Proveedor Num. 07/00475	355.93	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24345	\N	1284	\N	2007-01-11	Factura Proveedor	10308	Factura Proveedor Num. 99959	5.17	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24346	\N	1284	\N	2007-01-11	Factura Proveedor	10912	Factura Proveedor Num. 99959	0.00	37.50	\N	\N	\N	\N	\N	\N	\N	\N
24347	\N	1284	\N	2007-01-11	Factura Proveedor	10414	Factura Proveedor Num. 99959	32.33	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24348	\N	1285	\N	2007-02-27	Factura Proveedor	10308	Factura Proveedor Num. 00717	9.23	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24349	\N	1285	\N	2007-02-27	Factura Proveedor	10935	Factura Proveedor Num. 00717	0.00	66.91	\N	\N	\N	\N	\N	\N	\N	\N
24350	\N	1285	\N	2007-02-27	Factura Proveedor	10414	Factura Proveedor Num. 00717	57.68	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24351	\N	1286	\N	2007-02-22	Factura Proveedor	10308	Factura Proveedor Num. 102266	205.41	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24352	\N	1286	\N	2007-02-22	Factura Proveedor	10912	Factura Proveedor Num. 102266	0.00	1489.25	\N	\N	\N	\N	\N	\N	\N	\N
24353	\N	1286	\N	2007-02-22	Factura Proveedor	10414	Factura Proveedor Num. 102266	1283.84	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24354	\N	1287	\N	2007-02-15	Factura Proveedor	10308	Factura Proveedor Num. 00583	10.93	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24355	\N	1287	\N	2007-02-15	Factura Proveedor	10935	Factura Proveedor Num. 00583	0.00	79.26	\N	\N	\N	\N	\N	\N	\N	\N
24356	\N	1287	\N	2007-02-15	Factura Proveedor	10414	Factura Proveedor Num. 00583	68.33	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24357	\N	1288	\N	2007-02-06	Factura Proveedor	10308	Factura Proveedor Num. 101360	64.09	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24358	\N	1288	\N	2007-02-06	Factura Proveedor	10912	Factura Proveedor Num. 101360	0.00	464.65	\N	\N	\N	\N	\N	\N	\N	\N
24359	\N	1288	\N	2007-02-06	Factura Proveedor	10414	Factura Proveedor Num. 101360	400.56	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24360	\N	1289	\N	2007-02-07	Factura Proveedor	10308	Factura Proveedor Num. 101483	79.47	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24361	\N	1289	\N	2007-02-07	Factura Proveedor	10912	Factura Proveedor Num. 101483	0.00	576.15	\N	\N	\N	\N	\N	\N	\N	\N
24362	\N	1289	\N	2007-02-07	Factura Proveedor	10414	Factura Proveedor Num. 101483	496.68	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24363	\N	1290	\N	2007-01-31	Factura Proveedor	10308	Factura Proveedor Num. 101049	353.09	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24364	\N	1290	\N	2007-01-31	Factura Proveedor	10912	Factura Proveedor Num. 101049	0.00	2559.93	\N	\N	\N	\N	\N	\N	\N	\N
24365	\N	1290	\N	2007-01-31	Factura Proveedor	10414	Factura Proveedor Num. 101049	2206.84	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24366	\N	1291	\N	2007-02-06	Factura Proveedor	10308	Factura Proveedor Num. 101359	0.49	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24367	\N	1291	\N	2007-02-06	Factura Proveedor	10912	Factura Proveedor Num. 101359	0.00	3.54	\N	\N	\N	\N	\N	\N	\N	\N
24368	\N	1291	\N	2007-02-06	Factura Proveedor	10414	Factura Proveedor Num. 101359	3.05	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24369	\N	1292	\N	2007-02-02	Factura Proveedor	10308	Factura Proveedor Num. GIB	27.37	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24370	\N	1292	\N	2007-02-02	Factura Proveedor	10912	Factura Proveedor Num. GIB	0.00	198.42	\N	\N	\N	\N	\N	\N	\N	\N
24371	\N	1292	\N	2007-02-02	Factura Proveedor	10414	Factura Proveedor Num. GIB	171.05	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24372	\N	1293	\N	2007-02-15	Factura Proveedor	10308	Factura Proveedor Num. 101892	2.87	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24373	\N	1293	\N	2007-02-15	Factura Proveedor	10912	Factura Proveedor Num. 101892	0.00	20.81	\N	\N	\N	\N	\N	\N	\N	\N
24374	\N	1293	\N	2007-02-15	Factura Proveedor	10414	Factura Proveedor Num. 101892	17.94	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24375	\N	1294	\N	2007-01-24	Factura Proveedor	10308	Factura Proveedor Num. 100689	26.49	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24376	\N	1294	\N	2007-01-24	Factura Proveedor	10912	Factura Proveedor Num. 100689	0.00	192.03	\N	\N	\N	\N	\N	\N	\N	\N
24377	\N	1294	\N	2007-01-24	Factura Proveedor	10414	Factura Proveedor Num. 100689	165.54	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24378	\N	1295	\N	2007-02-02	Factura Proveedor	10308	Factura Proveedor Num. A40307549	6.77	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24379	\N	1295	\N	2007-02-02	Factura Proveedor	10934	Factura Proveedor Num. A40307549	0.00	49.08	\N	\N	\N	\N	\N	\N	\N	\N
24380	\N	1295	\N	2007-02-02	Factura Proveedor	10414	Factura Proveedor Num. A40307549	42.31	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24381	\N	1296	\N	2007-01-08	Factura Proveedor	10308	Factura Proveedor Num. 0710	27.71	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24382	\N	1296	\N	2007-01-08	Factura Proveedor	10935	Factura Proveedor Num. 0710	0.00	200.91	\N	\N	\N	\N	\N	\N	\N	\N
24383	\N	1296	\N	2007-01-08	Factura Proveedor	10414	Factura Proveedor Num. 0710	173.20	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24384	\N	1297	\N	2007-01-09	Factura Proveedor	10308	Factura Proveedor Num. 0726	56.22	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24385	\N	1297	\N	2007-01-09	Factura Proveedor	10935	Factura Proveedor Num. 0726	0.00	407.62	\N	\N	\N	\N	\N	\N	\N	\N
24386	\N	1297	\N	2007-01-09	Factura Proveedor	10414	Factura Proveedor Num. 0726	351.40	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24387	\N	1298	\N	2007-01-22	Factura Proveedor	10308	Factura Proveedor Num. 100560	11.17	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24388	\N	1298	\N	2007-01-22	Factura Proveedor	10912	Factura Proveedor Num. 100560	0.00	80.96	\N	\N	\N	\N	\N	\N	\N	\N
24389	\N	1298	\N	2007-01-22	Factura Proveedor	10414	Factura Proveedor Num. 100560	69.79	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24390	\N	1299	\N	2007-01-23	Factura Proveedor	10308	Factura Proveedor Num. 321393	18.66	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24391	\N	1299	\N	2007-01-23	Factura Proveedor	10923	Factura Proveedor Num. 321393	0.00	135.27	\N	\N	\N	\N	\N	\N	\N	\N
24392	\N	1299	\N	2007-01-23	Factura Proveedor	10414	Factura Proveedor Num. 321393	116.61	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24393	\N	1300	\N	2007-02-16	Factura Proveedor	10308	Factura Proveedor Num. 102029	56.17	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24394	\N	1300	\N	2007-02-16	Factura Proveedor	10912	Factura Proveedor Num. 102029	0.00	407.22	\N	\N	\N	\N	\N	\N	\N	\N
24395	\N	1300	\N	2007-02-16	Factura Proveedor	10414	Factura Proveedor Num. 102029	351.05	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24396	\N	1301	\N	2007-02-23	Factura Proveedor	10308	Factura Proveedor Num. 778021	21.62	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24397	\N	1301	\N	2007-02-23	Factura Proveedor	10923	Factura Proveedor Num. 778021	0.00	156.76	\N	\N	\N	\N	\N	\N	\N	\N
24398	\N	1301	\N	2007-02-23	Factura Proveedor	10414	Factura Proveedor Num. 778021	135.14	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24399	\N	1302	\N	2007-03-06	Factura Proveedor	10308	Factura Proveedor Num. 102870	40.86	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24400	\N	1302	\N	2007-03-06	Factura Proveedor	10912	Factura Proveedor Num. 102870	0.00	296.26	\N	\N	\N	\N	\N	\N	\N	\N
24401	\N	1302	\N	2007-03-06	Factura Proveedor	10414	Factura Proveedor Num. 102870	255.40	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24402	\N	1303	\N	2007-03-06	Factura Proveedor	10308	Factura Proveedor Num. 769	21.34	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24403	\N	1303	\N	2007-03-06	Factura Proveedor	10935	Factura Proveedor Num. 769	0.00	154.74	\N	\N	\N	\N	\N	\N	\N	\N
24404	\N	1303	\N	2007-03-06	Factura Proveedor	10414	Factura Proveedor Num. 769	133.40	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24405	\N	1304	\N	2007-03-13	Factura Proveedor	10308	Factura Proveedor Num. 103279	5.00	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24406	\N	1304	\N	2007-03-13	Factura Proveedor	10912	Factura Proveedor Num. 103279	0.00	36.23	\N	\N	\N	\N	\N	\N	\N	\N
24407	\N	1304	\N	2007-03-13	Factura Proveedor	10414	Factura Proveedor Num. 103279	31.23	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24408	\N	1305	\N	2007-02-02	Factura Proveedor	10308	Factura Proveedor Num. 2007001	96.00	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24409	\N	1305	\N	2007-02-02	Factura Proveedor	10933	Factura Proveedor Num. 2007001	0.00	606.00	\N	\N	\N	\N	\N	\N	\N	\N
24410	\N	1305	\N	2007-02-02	Factura Proveedor	10414	Factura Proveedor Num. 2007001	600.00	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24411	\N	1306	\N	2007-01-11	Factura Proveedor	10308	Factura Proveedor Num. 008922	162.82	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24412	\N	1306	\N	2007-01-11	Factura Proveedor	10914	Factura Proveedor Num. 008922	0.00	1180.47	\N	\N	\N	\N	\N	\N	\N	\N
24413	\N	1306	\N	2007-01-11	Factura Proveedor	10414	Factura Proveedor Num. 008922	1017.65	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24414	\N	1307	\N	2007-04-27	Factura Proveedor	10308	Factura Proveedor Num. 07/01394	50.08	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24415	\N	1307	\N	2007-04-27	Factura Proveedor	10935	Factura Proveedor Num. 07/01394	0.00	363.08	\N	\N	\N	\N	\N	\N	\N	\N
24416	\N	1307	\N	2007-04-27	Factura Proveedor	10414	Factura Proveedor Num. 07/01394	313.00	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24417	\N	1308	\N	2007-01-12	Factura Proveedor	10308	Factura Proveedor Num. 07/00111	43.79	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24418	\N	1308	\N	2007-01-12	Factura Proveedor	10935	Factura Proveedor Num. 07/00111	0.00	317.45	\N	\N	\N	\N	\N	\N	\N	\N
24419	\N	1308	\N	2007-01-12	Factura Proveedor	10414	Factura Proveedor Num. 07/00111	273.66	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24420	\N	1309	\N	2007-05-22	Factura Proveedor	10308	Factura Proveedor Num. 07/0173	18.24	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24421	\N	1309	\N	2007-05-22	Factura Proveedor	10935	Factura Proveedor Num. 07/0173	0.00	132.27	\N	\N	\N	\N	\N	\N	\N	\N
24422	\N	1309	\N	2007-05-22	Factura Proveedor	10414	Factura Proveedor Num. 07/0173	114.03	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24423	\N	1310	\N	2007-05-10	Factura Proveedor	10308	Factura Proveedor Num. 01567	84.99	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24424	\N	1310	\N	2007-05-10	Factura Proveedor	10935	Factura Proveedor Num. 01567	0.00	616.17	\N	\N	\N	\N	\N	\N	\N	\N
24425	\N	1310	\N	2007-05-10	Factura Proveedor	10414	Factura Proveedor Num. 01567	531.18	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24426	\N	1311	\N	2007-05-23	Factura Proveedor	10308	Factura Proveedor Num. 1719	10.05	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24427	\N	1311	\N	2007-05-23	Factura Proveedor	10935	Factura Proveedor Num. 1719	0.00	72.89	\N	\N	\N	\N	\N	\N	\N	\N
24428	\N	1311	\N	2007-05-23	Factura Proveedor	10414	Factura Proveedor Num. 1719	62.84	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24429	\N	1312	\N	2007-01-17	Factura Proveedor	10308	Factura Proveedor Num. 100270	5.90	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24430	\N	1312	\N	2007-01-17	Factura Proveedor	10912	Factura Proveedor Num. 100270	0.00	42.76	\N	\N	\N	\N	\N	\N	\N	\N
24431	\N	1312	\N	2007-01-17	Factura Proveedor	10414	Factura Proveedor Num. 100270	36.86	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24432	\N	1313	\N	2007-04-10	Factura Proveedor	10308	Factura Proveedor Num. 1187	22.36	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24433	\N	1313	\N	2007-04-10	Factura Proveedor	10935	Factura Proveedor Num. 1187	0.00	162.11	\N	\N	\N	\N	\N	\N	\N	\N
24434	\N	1313	\N	2007-04-10	Factura Proveedor	10414	Factura Proveedor Num. 1187	139.75	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24435	\N	1314	\N	2007-04-21	Factura Proveedor	10308	Factura Proveedor Num. 1317	43.61	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24436	\N	1314	\N	2007-04-21	Factura Proveedor	10935	Factura Proveedor Num. 1317	0.00	316.16	\N	\N	\N	\N	\N	\N	\N	\N
24437	\N	1314	\N	2007-04-21	Factura Proveedor	10414	Factura Proveedor Num. 1317	272.55	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24438	\N	1315	\N	2007-05-02	Factura Proveedor	10308	Factura Proveedor Num. 1448	9.23	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24439	\N	1315	\N	2007-05-02	Factura Proveedor	10935	Factura Proveedor Num. 1448	0.00	66.91	\N	\N	\N	\N	\N	\N	\N	\N
24440	\N	1315	\N	2007-05-02	Factura Proveedor	10414	Factura Proveedor Num. 1448	57.68	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24441	\N	1316	\N	2007-05-22	Factura Proveedor	10308	Factura Proveedor Num. 07/01630	12.82	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24442	\N	1316	\N	2007-05-22	Factura Proveedor	10926	Factura Proveedor Num. 07/01630	0.00	92.94	\N	\N	\N	\N	\N	\N	\N	\N
24443	\N	1316	\N	2007-05-22	Factura Proveedor	10414	Factura Proveedor Num. 07/01630	80.12	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24444	\N	1317	\N	2007-05-02	Factura Proveedor	10308	Factura Proveedor Num. 105920	30.48	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24445	\N	1317	\N	2007-05-02	Factura Proveedor	10912	Factura Proveedor Num. 105920	0.00	220.98	\N	\N	\N	\N	\N	\N	\N	\N
24446	\N	1317	\N	2007-05-02	Factura Proveedor	10414	Factura Proveedor Num. 105920	190.50	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24447	\N	1318	\N	2007-04-30	Factura Proveedor	10308	Factura Proveedor Num. 07/01430	25.81	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24448	\N	1318	\N	2007-04-30	Factura Proveedor	10935	Factura Proveedor Num. 07/01430	0.00	187.12	\N	\N	\N	\N	\N	\N	\N	\N
24449	\N	1318	\N	2007-04-30	Factura Proveedor	10414	Factura Proveedor Num. 07/01430	161.31	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24450	\N	1319	\N	2007-02-22	Factura Proveedor	10308	Factura Proveedor Num. 102265	57.01	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24451	\N	1319	\N	2007-02-22	Factura Proveedor	10912	Factura Proveedor Num. 102265	0.00	413.35	\N	\N	\N	\N	\N	\N	\N	\N
24452	\N	1319	\N	2007-02-22	Factura Proveedor	10414	Factura Proveedor Num. 102265	356.34	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24453	\N	1320	\N	2007-03-15	Factura Proveedor	10308	Factura Proveedor Num. 103405	29.56	0.00	\N	\N	\N	\N	\N	\N	\N	\N
24454	\N	1320	\N	2007-03-15	Factura Proveedor	10912	Factura Proveedor Num. 103405	0.00	214.28	\N	\N	\N	\N	\N	\N	\N	\N
24455	\N	1320	\N	2007-03-15	Factura Proveedor	10414	Factura Proveedor Num. 103405	184.72	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26760	\N	1250	\N	2007-03-23	Factura Cliente	10318	Factura Cliente Num. 61	0.00	60.02	\N	\N	\N	\N	\N	\N	\N	\N
26761	\N	1250	\N	2007-03-23	Factura Cliente	10883	Factura Cliente Num. 61	435.17	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26762	\N	1250	\N	2007-03-23	Factura Cliente	10534	Factura Cliente Num. 61	0.00	375.15	\N	\N	\N	\N	\N	\N	\N	\N
26862	\N	1260	\N	2007-05-08	Factura Cliente	10318	Factura Cliente Num. 103	0.00	43.74	\N	\N	\N	\N	\N	\N	\N	\N
26863	\N	1260	\N	2007-05-08	Factura Cliente	10890	Factura Cliente Num. 103	317.10	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26864	\N	1260	\N	2007-05-08	Factura Cliente	10534	Factura Cliente Num. 103	0.00	273.36	\N	\N	\N	\N	\N	\N	\N	\N
26964	\N	1271	\N	2007-05-07	Factura Cliente	10318	Factura Cliente Num. 98	0.00	6.28	\N	\N	\N	\N	\N	\N	\N	\N
26965	\N	1271	\N	2007-05-07	Factura Cliente	10900	Factura Cliente Num. 98	45.54	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26966	\N	1271	\N	2007-05-07	Factura Cliente	10534	Factura Cliente Num. 98	0.00	39.26	\N	\N	\N	\N	\N	\N	\N	\N
26763	\N	1157	\N	2007-03-23	Factura Cliente	10318	Factura Cliente Num. 59	0.00	60.02	\N	\N	\N	\N	\N	\N	\N	\N
26764	\N	1157	\N	2007-03-23	Factura Cliente	10883	Factura Cliente Num. 59	435.17	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26765	\N	1157	\N	2007-03-23	Factura Cliente	10534	Factura Cliente Num. 59	0.00	375.15	\N	\N	\N	\N	\N	\N	\N	\N
26865	\N	1221	\N	2007-02-22	Factura Cliente	10318	Factura Cliente Num. 33	0.00	17.15	\N	\N	\N	\N	\N	\N	\N	\N
26866	\N	1221	\N	2007-02-22	Factura Cliente	10885	Factura Cliente Num. 33	124.35	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26867	\N	1221	\N	2007-02-22	Factura Cliente	10534	Factura Cliente Num. 33	0.00	107.20	\N	\N	\N	\N	\N	\N	\N	\N
26967	\N	1164	\N	2007-02-23	Factura Cliente	10318	Factura Cliente Num. 40	0.00	24.50	\N	\N	\N	\N	\N	\N	\N	\N
26968	\N	1164	\N	2007-02-23	Factura Cliente	10897	Factura Cliente Num. 40	177.59	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26969	\N	1164	\N	2007-02-23	Factura Cliente	10534	Factura Cliente Num. 40	0.00	153.10	\N	\N	\N	\N	\N	\N	\N	\N
26766	\N	1158	\N	2007-03-27	Factura Cliente	10318	Factura Cliente Num. 72	0.00	9.65	\N	\N	\N	\N	\N	\N	\N	\N
26767	\N	1158	\N	2007-03-27	Factura Cliente	10851	Factura Cliente Num. 72	69.95	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26768	\N	1158	\N	2007-03-27	Factura Cliente	10534	Factura Cliente Num. 72	0.00	60.30	\N	\N	\N	\N	\N	\N	\N	\N
26868	\N	1222	\N	2007-02-22	Factura Cliente	10318	Factura Cliente Num. 34	0.00	6.43	\N	\N	\N	\N	\N	\N	\N	\N
26869	\N	1222	\N	2007-02-22	Factura Cliente	10830	Factura Cliente Num. 34	46.63	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26870	\N	1222	\N	2007-02-22	Factura Cliente	10534	Factura Cliente Num. 34	0.00	40.20	\N	\N	\N	\N	\N	\N	\N	\N
26970	\N	1272	\N	2007-07-11	Factura Cliente	10318	Factura Cliente Num. 128	0.00	231.55	\N	\N	\N	\N	\N	\N	\N	\N
26971	\N	1272	\N	2007-07-11	Factura Cliente	10907	Factura Cliente Num. 128	1678.75	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26972	\N	1272	\N	2007-07-11	Factura Cliente	10534	Factura Cliente Num. 128	0.00	1447.20	\N	\N	\N	\N	\N	\N	\N	\N
27078	\N	1170	\N	2007-01-31	Factura Cliente	10318	Factura Cliente Num. 6	0.00	246.99	\N	\N	\N	\N	\N	\N	\N	\N
27079	\N	1170	\N	2007-01-31	Factura Cliente	10820	Factura Cliente Num. 6	1790.67	0.00	\N	\N	\N	\N	\N	\N	\N	\N
27080	\N	1170	\N	2007-01-31	Factura Cliente	10534	Factura Cliente Num. 6	0.00	1543.68	\N	\N	\N	\N	\N	\N	\N	\N
26769	\N	1159	\N	2007-03-27	Factura Cliente	10318	Factura Cliente Num. 71	0.00	16.08	\N	\N	\N	\N	\N	\N	\N	\N
26770	\N	1159	\N	2007-03-27	Factura Cliente	10858	Factura Cliente Num. 71	116.58	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26771	\N	1159	\N	2007-03-27	Factura Cliente	10534	Factura Cliente Num. 71	0.00	100.50	\N	\N	\N	\N	\N	\N	\N	\N
26871	\N	1223	\N	2007-05-08	Factura Cliente	10318	Factura Cliente Num. 106	0.00	11.67	\N	\N	\N	\N	\N	\N	\N	\N
26872	\N	1223	\N	2007-05-08	Factura Cliente	10867	Factura Cliente Num. 106	84.61	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26873	\N	1223	\N	2007-05-08	Factura Cliente	10534	Factura Cliente Num. 106	0.00	72.94	\N	\N	\N	\N	\N	\N	\N	\N
26973	\N	1165	\N	2007-09-14	Factura Cliente	10318	Factura Cliente Num. 129	0.00	2.14	\N	\N	\N	\N	\N	\N	\N	\N
26974	\N	1165	\N	2007-09-14	Factura Cliente	10908	Factura Cliente Num. 129	15.54	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26975	\N	1165	\N	2007-09-14	Factura Cliente	10534	Factura Cliente Num. 129	0.00	13.40	\N	\N	\N	\N	\N	\N	\N	\N
26772	\N	1160	\N	2007-03-27	Factura Cliente	10318	Factura Cliente Num. 64	0.00	70.75	\N	\N	\N	\N	\N	\N	\N	\N
26773	\N	1160	\N	2007-03-27	Factura Cliente	10881	Factura Cliente Num. 64	512.95	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26774	\N	1160	\N	2007-03-27	Factura Cliente	10534	Factura Cliente Num. 64	0.00	442.20	\N	\N	\N	\N	\N	\N	\N	\N
26874	\N	1224	\N	2007-05-08	Factura Cliente	10318	Factura Cliente Num. 111	0.00	25.73	\N	\N	\N	\N	\N	\N	\N	\N
26875	\N	1224	\N	2007-05-08	Factura Cliente	10887	Factura Cliente Num. 111	186.53	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26876	\N	1224	\N	2007-05-08	Factura Cliente	10534	Factura Cliente Num. 111	0.00	160.80	\N	\N	\N	\N	\N	\N	\N	\N
26976	\N	1166	\N	2007-05-25	Factura Cliente	10318	Factura Cliente Num. 122	0.00	14.85	\N	\N	\N	\N	\N	\N	\N	\N
26977	\N	1166	\N	2007-05-25	Factura Cliente	10869	Factura Cliente Num. 122	107.65	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26978	\N	1166	\N	2007-05-25	Factura Cliente	10534	Factura Cliente Num. 122	0.00	92.80	\N	\N	\N	\N	\N	\N	\N	\N
26775	\N	1161	\N	2007-03-20	Factura Cliente	10318	Factura Cliente Num. 58	0.00	25.73	\N	\N	\N	\N	\N	\N	\N	\N
26776	\N	1161	\N	2007-03-20	Factura Cliente	10878	Factura Cliente Num. 58	186.53	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26777	\N	1161	\N	2007-03-20	Factura Cliente	10534	Factura Cliente Num. 58	0.00	160.80	\N	\N	\N	\N	\N	\N	\N	\N
26877	\N	1225	\N	2007-05-08	Factura Cliente	10318	Factura Cliente Num. 105	0.00	17.15	\N	\N	\N	\N	\N	\N	\N	\N
26878	\N	1225	\N	2007-05-08	Factura Cliente	10885	Factura Cliente Num. 105	124.35	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26879	\N	1225	\N	2007-05-08	Factura Cliente	10534	Factura Cliente Num. 105	0.00	107.20	\N	\N	\N	\N	\N	\N	\N	\N
26979	\N	1191	\N	2007-02-22	Factura Cliente	10318	Factura Cliente Num. 29	0.00	53.15	\N	\N	\N	\N	\N	\N	\N	\N
26980	\N	1191	\N	2007-02-22	Factura Cliente	10877	Factura Cliente Num. 29	385.31	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26981	\N	1191	\N	2007-02-22	Factura Cliente	10534	Factura Cliente Num. 29	0.00	332.16	\N	\N	\N	\N	\N	\N	\N	\N
26778	\N	1203	\N	2007-03-30	Factura Cliente	10318	Factura Cliente Num. 75	0.00	6.28	\N	\N	\N	\N	\N	\N	\N	\N
26779	\N	1203	\N	2007-03-30	Factura Cliente	10901	Factura Cliente Num. 75	45.56	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26780	\N	1203	\N	2007-03-30	Factura Cliente	10534	Factura Cliente Num. 75	0.00	39.28	\N	\N	\N	\N	\N	\N	\N	\N
26880	\N	1226	\N	2007-05-08	Factura Cliente	10318	Factura Cliente Num. 104	0.00	9.12	\N	\N	\N	\N	\N	\N	\N	\N
26881	\N	1226	\N	2007-05-08	Factura Cliente	10830	Factura Cliente Num. 104	66.11	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26882	\N	1226	\N	2007-05-08	Factura Cliente	10534	Factura Cliente Num. 104	0.00	56.99	\N	\N	\N	\N	\N	\N	\N	\N
26982	\N	1238	\N	2007-05-08	Factura Cliente	10318	Factura Cliente Num. 99	0.00	70.75	\N	\N	\N	\N	\N	\N	\N	\N
26983	\N	1238	\N	2007-05-08	Factura Cliente	10882	Factura Cliente Num. 99	512.95	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26984	\N	1238	\N	2007-05-08	Factura Cliente	10534	Factura Cliente Num. 99	0.00	442.20	\N	\N	\N	\N	\N	\N	\N	\N
26781	\N	1204	\N	2007-03-13	Factura Cliente	10318	Factura Cliente Num. 51	0.00	6.28	\N	\N	\N	\N	\N	\N	\N	\N
26782	\N	1204	\N	2007-03-13	Factura Cliente	10802	Factura Cliente Num. 51	45.53	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26783	\N	1204	\N	2007-03-13	Factura Cliente	10534	Factura Cliente Num. 51	0.00	39.25	\N	\N	\N	\N	\N	\N	\N	\N
26883	\N	1227	\N	2007-05-04	Factura Cliente	10318	Factura Cliente Num. 96	0.00	6.43	\N	\N	\N	\N	\N	\N	\N	\N
26884	\N	1227	\N	2007-05-04	Factura Cliente	10858	Factura Cliente Num. 96	46.63	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26885	\N	1227	\N	2007-05-04	Factura Cliente	10534	Factura Cliente Num. 96	0.00	40.20	\N	\N	\N	\N	\N	\N	\N	\N
26985	\N	1239	\N	2007-05-08	Factura Cliente	10318	Factura Cliente Num. 102	0.00	107.20	\N	\N	\N	\N	\N	\N	\N	\N
26986	\N	1239	\N	2007-05-08	Factura Cliente	10880	Factura Cliente Num. 102	777.20	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26987	\N	1239	\N	2007-05-08	Factura Cliente	10534	Factura Cliente Num. 102	0.00	670.00	\N	\N	\N	\N	\N	\N	\N	\N
26784	\N	1205	\N	2007-04-03	Factura Cliente	10318	Factura Cliente Num. 77	0.00	58.25	\N	\N	\N	\N	\N	\N	\N	\N
26785	\N	1205	\N	2007-04-03	Factura Cliente	10804	Factura Cliente Num. 77	422.33	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26786	\N	1205	\N	2007-04-03	Factura Cliente	10534	Factura Cliente Num. 77	0.00	364.08	\N	\N	\N	\N	\N	\N	\N	\N
26886	\N	1228	\N	2007-05-04	Factura Cliente	10318	Factura Cliente Num. 97	0.00	24.03	\N	\N	\N	\N	\N	\N	\N	\N
26887	\N	1228	\N	2007-05-04	Factura Cliente	10820	Factura Cliente Num. 97	174.21	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26888	\N	1228	\N	2007-05-04	Factura Cliente	10534	Factura Cliente Num. 97	0.00	150.18	\N	\N	\N	\N	\N	\N	\N	\N
26988	\N	1273	\N	2007-03-20	Factura Cliente	10318	Factura Cliente Num. 56	0.00	72.78	\N	\N	\N	\N	\N	\N	\N	\N
26989	\N	1273	\N	2007-03-20	Factura Cliente	10806	Factura Cliente Num. 56	527.66	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26990	\N	1273	\N	2007-03-20	Factura Cliente	10534	Factura Cliente Num. 56	0.00	454.88	\N	\N	\N	\N	\N	\N	\N	\N
26787	\N	1206	\N	2007-04-03	Factura Cliente	10318	Factura Cliente Num. 79	0.00	17.15	\N	\N	\N	\N	\N	\N	\N	\N
26788	\N	1206	\N	2007-04-03	Factura Cliente	10885	Factura Cliente Num. 79	124.35	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26789	\N	1206	\N	2007-04-03	Factura Cliente	10534	Factura Cliente Num. 79	0.00	107.20	\N	\N	\N	\N	\N	\N	\N	\N
26889	\N	1229	\N	2007-05-09	Factura Cliente	10318	Factura Cliente Num. 108	0.00	77.18	\N	\N	\N	\N	\N	\N	\N	\N
26890	\N	1229	\N	2007-05-09	Factura Cliente	10820	Factura Cliente Num. 108	559.58	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26891	\N	1229	\N	2007-05-09	Factura Cliente	10534	Factura Cliente Num. 108	0.00	482.40	\N	\N	\N	\N	\N	\N	\N	\N
26991	\N	1274	\N	2007-01-30	Factura Cliente	10318	Factura Cliente Num. 5	0.00	47.77	\N	\N	\N	\N	\N	\N	\N	\N
26992	\N	1274	\N	2007-01-30	Factura Cliente	10885	Factura Cliente Num. 5	346.33	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26993	\N	1274	\N	2007-01-30	Factura Cliente	10534	Factura Cliente Num. 5	0.00	298.56	\N	\N	\N	\N	\N	\N	\N	\N
26688	\N	1155	\N	2007-01-31	Factura Cliente	10318	Factura Cliente Num. 12	0.00	70.75	\N	\N	\N	\N	\N	\N	\N	\N
26689	\N	1155	\N	2007-01-31	Factura Cliente	10881	Factura Cliente Num. 12	512.95	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26690	\N	1155	\N	2007-01-31	Factura Cliente	10534	Factura Cliente Num. 12	0.00	442.20	\N	\N	\N	\N	\N	\N	\N	\N
26790	\N	1207	\N	2007-04-03	Factura Cliente	10318	Factura Cliente Num. 80	0.00	6.43	\N	\N	\N	\N	\N	\N	\N	\N
26791	\N	1207	\N	2007-04-03	Factura Cliente	10830	Factura Cliente Num. 80	46.63	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26792	\N	1207	\N	2007-04-03	Factura Cliente	10534	Factura Cliente Num. 80	0.00	40.20	\N	\N	\N	\N	\N	\N	\N	\N
26892	\N	1261	\N	2007-02-22	Factura Cliente	10318	Factura Cliente Num. 36	0.00	64.10	\N	\N	\N	\N	\N	\N	\N	\N
26893	\N	1261	\N	2007-02-22	Factura Cliente	10806	Factura Cliente Num. 36	464.74	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26894	\N	1261	\N	2007-02-22	Factura Cliente	10534	Factura Cliente Num. 36	0.00	400.64	\N	\N	\N	\N	\N	\N	\N	\N
26994	\N	1167	\N	2007-05-09	Factura Cliente	10318	Factura Cliente Num. 107	0.00	20.15	\N	\N	\N	\N	\N	\N	\N	\N
26995	\N	1167	\N	2007-05-09	Factura Cliente	10867	Factura Cliente Num. 107	146.11	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26996	\N	1167	\N	2007-05-09	Factura Cliente	10534	Factura Cliente Num. 107	0.00	125.96	\N	\N	\N	\N	\N	\N	\N	\N
26691	\N	1185	\N	2007-01-25	Factura Cliente	10318	Factura Cliente Num. 3	0.00	6.98	\N	\N	\N	\N	\N	\N	\N	\N
26692	\N	1185	\N	2007-01-25	Factura Cliente	10877	Factura Cliente Num. 3	50.58	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26693	\N	1185	\N	2007-01-25	Factura Cliente	10534	Factura Cliente Num. 3	0.00	43.60	\N	\N	\N	\N	\N	\N	\N	\N
26793	\N	1208	\N	2007-04-09	Factura Cliente	10318	Factura Cliente Num. 76	0.00	39.25	\N	\N	\N	\N	\N	\N	\N	\N
26794	\N	1208	\N	2007-04-09	Factura Cliente	10804	Factura Cliente Num. 76	284.55	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26795	\N	1208	\N	2007-04-09	Factura Cliente	10534	Factura Cliente Num. 76	0.00	245.30	\N	\N	\N	\N	\N	\N	\N	\N
26895	\N	1262	\N	2007-05-09	Factura Cliente	10318	Factura Cliente Num. 109	0.00	38.59	\N	\N	\N	\N	\N	\N	\N	\N
26896	\N	1262	\N	2007-05-09	Factura Cliente	10820	Factura Cliente Num. 109	279.79	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26897	\N	1262	\N	2007-05-09	Factura Cliente	10534	Factura Cliente Num. 109	0.00	241.20	\N	\N	\N	\N	\N	\N	\N	\N
26997	\N	1168	\N	2007-01-15	Factura Cliente	10318	Factura Cliente Num. 1	0.00	166.16	\N	\N	\N	\N	\N	\N	\N	\N
26998	\N	1168	\N	2007-01-15	Factura Cliente	10867	Factura Cliente Num. 1	1204.66	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26999	\N	1168	\N	2007-01-15	Factura Cliente	10534	Factura Cliente Num. 1	0.00	1038.50	\N	\N	\N	\N	\N	\N	\N	\N
26694	\N	1186	\N	2007-02-27	Factura Cliente	10318	Factura Cliente Num. 41	0.00	385.92	\N	\N	\N	\N	\N	\N	\N	\N
26695	\N	1186	\N	2007-02-27	Factura Cliente	10858	Factura Cliente Num. 41	2797.92	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26696	\N	1186	\N	2007-02-27	Factura Cliente	10534	Factura Cliente Num. 41	0.00	2412.00	\N	\N	\N	\N	\N	\N	\N	\N
26796	\N	1209	\N	2007-04-03	Factura Cliente	10318	Factura Cliente Num. 78	0.00	428.80	\N	\N	\N	\N	\N	\N	\N	\N
26797	\N	1209	\N	2007-04-03	Factura Cliente	10804	Factura Cliente Num. 78	3108.80	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26798	\N	1209	\N	2007-04-03	Factura Cliente	10534	Factura Cliente Num. 78	0.00	2680.00	\N	\N	\N	\N	\N	\N	\N	\N
26898	\N	1230	\N	2007-05-18	Factura Cliente	10318	Factura Cliente Num. 113	0.00	71.83	\N	\N	\N	\N	\N	\N	\N	\N
26899	\N	1230	\N	2007-05-18	Factura Cliente	10820	Factura Cliente Num. 113	520.79	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26900	\N	1230	\N	2007-05-18	Factura Cliente	10534	Factura Cliente Num. 113	0.00	448.96	\N	\N	\N	\N	\N	\N	\N	\N
27000	\N	1169	\N	2007-01-30	Factura Cliente	10318	Factura Cliente Num. 4	0.00	6.43	\N	\N	\N	\N	\N	\N	\N	\N
27001	\N	1169	\N	2007-01-30	Factura Cliente	10830	Factura Cliente Num. 4	46.63	0.00	\N	\N	\N	\N	\N	\N	\N	\N
27002	\N	1169	\N	2007-01-30	Factura Cliente	10534	Factura Cliente Num. 4	0.00	40.20	\N	\N	\N	\N	\N	\N	\N	\N
26697	\N	1187	\N	2007-03-06	Factura Cliente	10318	Factura Cliente Num. 47	0.00	44.57	\N	\N	\N	\N	\N	\N	\N	\N
26698	\N	1187	\N	2007-03-06	Factura Cliente	10802	Factura Cliente Num. 47	323.12	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26699	\N	1187	\N	2007-03-06	Factura Cliente	10534	Factura Cliente Num. 47	0.00	278.55	\N	\N	\N	\N	\N	\N	\N	\N
26799	\N	1210	\N	2007-04-23	Factura Cliente	10318	Factura Cliente Num. 84	0.00	255.14	\N	\N	\N	\N	\N	\N	\N	\N
26800	\N	1210	\N	2007-04-23	Factura Cliente	10892	Factura Cliente Num. 84	1849.74	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26801	\N	1210	\N	2007-04-23	Factura Cliente	10534	Factura Cliente Num. 84	0.00	1594.60	\N	\N	\N	\N	\N	\N	\N	\N
26901	\N	1231	\N	2007-05-18	Factura Cliente	10318	Factura Cliente Num. 114	0.00	25.73	\N	\N	\N	\N	\N	\N	\N	\N
26902	\N	1231	\N	2007-05-18	Factura Cliente	10878	Factura Cliente Num. 114	186.53	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26903	\N	1231	\N	2007-05-18	Factura Cliente	10534	Factura Cliente Num. 114	0.00	160.80	\N	\N	\N	\N	\N	\N	\N	\N
26700	\N	1188	\N	2007-03-06	Factura Cliente	10318	Factura Cliente Num. 46	0.00	8.89	\N	\N	\N	\N	\N	\N	\N	\N
26701	\N	1188	\N	2007-03-06	Factura Cliente	10877	Factura Cliente Num. 46	64.44	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26702	\N	1188	\N	2007-03-06	Factura Cliente	10534	Factura Cliente Num. 46	0.00	55.55	\N	\N	\N	\N	\N	\N	\N	\N
26802	\N	1211	\N	2007-04-17	Factura Cliente	10318	Factura Cliente Num. 81	0.00	9.41	\N	\N	\N	\N	\N	\N	\N	\N
26803	\N	1211	\N	2007-04-17	Factura Cliente	10877	Factura Cliente Num. 81	68.22	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26804	\N	1211	\N	2007-04-17	Factura Cliente	10534	Factura Cliente Num. 81	0.00	58.81	\N	\N	\N	\N	\N	\N	\N	\N
26904	\N	1232	\N	2007-05-25	Factura Cliente	10318	Factura Cliente Num. 118	0.00	39.02	\N	\N	\N	\N	\N	\N	\N	\N
26905	\N	1232	\N	2007-05-25	Factura Cliente	10870	Factura Cliente Num. 118	282.90	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26906	\N	1232	\N	2007-05-25	Factura Cliente	10534	Factura Cliente Num. 118	0.00	243.88	\N	\N	\N	\N	\N	\N	\N	\N
27006	\N	1171	\N	2007-01-31	Factura Cliente	10318	Factura Cliente Num. 7	0.00	607.10	\N	\N	\N	\N	\N	\N	\N	\N
27007	\N	1171	\N	2007-01-31	Factura Cliente	10802	Factura Cliente Num. 7	4401.50	0.00	\N	\N	\N	\N	\N	\N	\N	\N
27008	\N	1171	\N	2007-01-31	Factura Cliente	10534	Factura Cliente Num. 7	0.00	3794.40	\N	\N	\N	\N	\N	\N	\N	\N
26703	\N	1189	\N	2007-03-08	Factura Cliente	10318	Factura Cliente Num. 50	0.00	1.96	\N	\N	\N	\N	\N	\N	\N	\N
26704	\N	1189	\N	2007-03-08	Factura Cliente	10815	Factura Cliente Num. 50	14.23	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26705	\N	1189	\N	2007-03-08	Factura Cliente	10534	Factura Cliente Num. 50	0.00	12.27	\N	\N	\N	\N	\N	\N	\N	\N
26805	\N	1251	\N	2007-01-15	Factura Cliente	10318	Factura Cliente Num. 2	0.00	76.69	\N	\N	\N	\N	\N	\N	\N	\N
26806	\N	1251	\N	2007-01-15	Factura Cliente	10877	Factura Cliente Num. 2	556.00	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26807	\N	1251	\N	2007-01-15	Factura Cliente	10534	Factura Cliente Num. 2	0.00	479.31	\N	\N	\N	\N	\N	\N	\N	\N
26907	\N	1233	\N	2007-05-25	Factura Cliente	10318	Factura Cliente Num. 117	0.00	81.47	\N	\N	\N	\N	\N	\N	\N	\N
26908	\N	1233	\N	2007-05-25	Factura Cliente	10881	Factura Cliente Num. 117	590.67	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26909	\N	1233	\N	2007-05-25	Factura Cliente	10534	Factura Cliente Num. 117	0.00	509.20	\N	\N	\N	\N	\N	\N	\N	\N
27009	\N	1172	\N	2007-01-31	Factura Cliente	10318	Factura Cliente Num. 15	0.00	6.43	\N	\N	\N	\N	\N	\N	\N	\N
27010	\N	1172	\N	2007-01-31	Factura Cliente	10866	Factura Cliente Num. 15	46.63	0.00	\N	\N	\N	\N	\N	\N	\N	\N
27011	\N	1172	\N	2007-01-31	Factura Cliente	10534	Factura Cliente Num. 15	0.00	40.20	\N	\N	\N	\N	\N	\N	\N	\N
26706	\N	1190	\N	2007-02-27	Factura Cliente	10318	Factura Cliente Num. 44	0.00	70.67	\N	\N	\N	\N	\N	\N	\N	\N
26707	\N	1190	\N	2007-02-27	Factura Cliente	10806	Factura Cliente Num. 44	512.35	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26708	\N	1190	\N	2007-02-27	Factura Cliente	10534	Factura Cliente Num. 44	0.00	441.68	\N	\N	\N	\N	\N	\N	\N	\N
26808	\N	1252	\N	2007-04-23	Factura Cliente	10318	Factura Cliente Num. 86	0.00	39.02	\N	\N	\N	\N	\N	\N	\N	\N
26809	\N	1252	\N	2007-04-23	Factura Cliente	10870	Factura Cliente Num. 86	282.90	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26810	\N	1252	\N	2007-04-23	Factura Cliente	10534	Factura Cliente Num. 86	0.00	243.88	\N	\N	\N	\N	\N	\N	\N	\N
26910	\N	1234	\N	2007-05-25	Factura Cliente	10318	Factura Cliente Num. 119	0.00	8.58	\N	\N	\N	\N	\N	\N	\N	\N
26911	\N	1234	\N	2007-05-25	Factura Cliente	10865	Factura Cliente Num. 119	62.18	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26912	\N	1234	\N	2007-05-25	Factura Cliente	10534	Factura Cliente Num. 119	0.00	53.60	\N	\N	\N	\N	\N	\N	\N	\N
27012	\N	1173	\N	2007-01-31	Factura Cliente	10318	Factura Cliente Num. 18	0.00	9.65	\N	\N	\N	\N	\N	\N	\N	\N
27013	\N	1173	\N	2007-01-31	Factura Cliente	10851	Factura Cliente Num. 18	69.95	0.00	\N	\N	\N	\N	\N	\N	\N	\N
27014	\N	1173	\N	2007-01-31	Factura Cliente	10534	Factura Cliente Num. 18	0.00	60.30	\N	\N	\N	\N	\N	\N	\N	\N
26709	\N	1192	\N	2007-02-22	Factura Cliente	10318	Factura Cliente Num. 39	0.00	38.59	\N	\N	\N	\N	\N	\N	\N	\N
26710	\N	1192	\N	2007-02-22	Factura Cliente	10858	Factura Cliente Num. 39	279.79	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26711	\N	1192	\N	2007-02-22	Factura Cliente	10534	Factura Cliente Num. 39	0.00	241.20	\N	\N	\N	\N	\N	\N	\N	\N
26811	\N	1253	\N	2007-04-23	Factura Cliente	10318	Factura Cliente Num. 89	0.00	8.58	\N	\N	\N	\N	\N	\N	\N	\N
26812	\N	1253	\N	2007-04-23	Factura Cliente	10865	Factura Cliente Num. 89	62.18	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26813	\N	1253	\N	2007-04-23	Factura Cliente	10534	Factura Cliente Num. 89	0.00	53.60	\N	\N	\N	\N	\N	\N	\N	\N
26913	\N	1235	\N	2007-05-25	Factura Cliente	10318	Factura Cliente Num. 120	0.00	321.60	\N	\N	\N	\N	\N	\N	\N	\N
26914	\N	1235	\N	2007-05-25	Factura Cliente	10804	Factura Cliente Num. 120	2331.60	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26915	\N	1235	\N	2007-05-25	Factura Cliente	10534	Factura Cliente Num. 120	0.00	2010.00	\N	\N	\N	\N	\N	\N	\N	\N
27015	\N	1174	\N	2007-01-31	Factura Cliente	10318	Factura Cliente Num. 13	0.00	29.48	\N	\N	\N	\N	\N	\N	\N	\N
27016	\N	1174	\N	2007-01-31	Factura Cliente	10855	Factura Cliente Num. 13	213.74	0.00	\N	\N	\N	\N	\N	\N	\N	\N
27017	\N	1174	\N	2007-01-31	Factura Cliente	10534	Factura Cliente Num. 13	0.00	184.26	\N	\N	\N	\N	\N	\N	\N	\N
26712	\N	1193	\N	2007-02-22	Factura Cliente	10318	Factura Cliente Num. 38	0.00	39.02	\N	\N	\N	\N	\N	\N	\N	\N
26713	\N	1193	\N	2007-02-22	Factura Cliente	10870	Factura Cliente Num. 38	282.90	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26714	\N	1193	\N	2007-02-22	Factura Cliente	10534	Factura Cliente Num. 38	0.00	243.88	\N	\N	\N	\N	\N	\N	\N	\N
26814	\N	1162	\N	2007-04-23	Factura Cliente	10318	Factura Cliente Num. 85	0.00	50.67	\N	\N	\N	\N	\N	\N	\N	\N
26815	\N	1162	\N	2007-04-23	Factura Cliente	10902	Factura Cliente Num. 85	367.38	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26816	\N	1162	\N	2007-04-23	Factura Cliente	10534	Factura Cliente Num. 85	0.00	316.71	\N	\N	\N	\N	\N	\N	\N	\N
26916	\N	1236	\N	2007-05-25	Factura Cliente	10318	Factura Cliente Num. 121	0.00	90.05	\N	\N	\N	\N	\N	\N	\N	\N
26917	\N	1236	\N	2007-05-25	Factura Cliente	10863	Factura Cliente Num. 121	652.85	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26918	\N	1236	\N	2007-05-25	Factura Cliente	10534	Factura Cliente Num. 121	0.00	562.80	\N	\N	\N	\N	\N	\N	\N	\N
27018	\N	1175	\N	2007-01-31	Factura Cliente	10318	Factura Cliente Num. 17	0.00	19.30	\N	\N	\N	\N	\N	\N	\N	\N
27019	\N	1175	\N	2007-01-31	Factura Cliente	10852	Factura Cliente Num. 17	139.90	0.00	\N	\N	\N	\N	\N	\N	\N	\N
27020	\N	1175	\N	2007-01-31	Factura Cliente	10534	Factura Cliente Num. 17	0.00	120.60	\N	\N	\N	\N	\N	\N	\N	\N
26715	\N	1194	\N	2007-02-27	Factura Cliente	10318	Factura Cliente Num. 45	0.00	84.50	\N	\N	\N	\N	\N	\N	\N	\N
26716	\N	1194	\N	2007-02-27	Factura Cliente	10806	Factura Cliente Num. 45	612.65	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26717	\N	1194	\N	2007-02-27	Factura Cliente	10534	Factura Cliente Num. 45	0.00	528.15	\N	\N	\N	\N	\N	\N	\N	\N
26817	\N	1254	\N	2007-04-20	Factura Cliente	10318	Factura Cliente Num. 83	0.00	25.73	\N	\N	\N	\N	\N	\N	\N	\N
26818	\N	1254	\N	2007-04-20	Factura Cliente	10878	Factura Cliente Num. 83	186.53	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26819	\N	1254	\N	2007-04-20	Factura Cliente	10534	Factura Cliente Num. 83	0.00	160.80	\N	\N	\N	\N	\N	\N	\N	\N
26919	\N	1237	\N	2007-03-27	Factura Cliente	10318	Factura Cliente Num. 66	0.00	25.73	\N	\N	\N	\N	\N	\N	\N	\N
26920	\N	1237	\N	2007-03-27	Factura Cliente	10849	Factura Cliente Num. 66	186.53	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26921	\N	1237	\N	2007-03-27	Factura Cliente	10534	Factura Cliente Num. 66	0.00	160.80	\N	\N	\N	\N	\N	\N	\N	\N
27021	\N	1176	\N	2007-01-31	Factura Cliente	10318	Factura Cliente Num. 16	0.00	38.59	\N	\N	\N	\N	\N	\N	\N	\N
27022	\N	1176	\N	2007-01-31	Factura Cliente	10864	Factura Cliente Num. 16	279.79	0.00	\N	\N	\N	\N	\N	\N	\N	\N
27023	\N	1176	\N	2007-01-31	Factura Cliente	10534	Factura Cliente Num. 16	0.00	241.20	\N	\N	\N	\N	\N	\N	\N	\N
26718	\N	1195	\N	2007-02-27	Factura Cliente	10318	Factura Cliente Num. 42	0.00	97.26	\N	\N	\N	\N	\N	\N	\N	\N
26719	\N	1195	\N	2007-02-27	Factura Cliente	10806	Factura Cliente Num. 42	705.11	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26720	\N	1195	\N	2007-02-27	Factura Cliente	10534	Factura Cliente Num. 42	0.00	607.85	\N	\N	\N	\N	\N	\N	\N	\N
26820	\N	1212	\N	2007-04-23	Factura Cliente	10318	Factura Cliente Num. 90	0.00	90.05	\N	\N	\N	\N	\N	\N	\N	\N
26821	\N	1212	\N	2007-04-23	Factura Cliente	10863	Factura Cliente Num. 90	652.85	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26822	\N	1212	\N	2007-04-23	Factura Cliente	10534	Factura Cliente Num. 90	0.00	562.80	\N	\N	\N	\N	\N	\N	\N	\N
26922	\N	1263	\N	2007-03-27	Factura Cliente	10318	Factura Cliente Num. 63	0.00	40.52	\N	\N	\N	\N	\N	\N	\N	\N
26923	\N	1263	\N	2007-03-27	Factura Cliente	10806	Factura Cliente Num. 63	293.78	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26924	\N	1263	\N	2007-03-27	Factura Cliente	10534	Factura Cliente Num. 63	0.00	253.26	\N	\N	\N	\N	\N	\N	\N	\N
27024	\N	1177	\N	2007-01-31	Factura Cliente	10318	Factura Cliente Num. 8	0.00	193.89	\N	\N	\N	\N	\N	\N	\N	\N
27025	\N	1177	\N	2007-01-31	Factura Cliente	10848	Factura Cliente Num. 8	1405.70	0.00	\N	\N	\N	\N	\N	\N	\N	\N
27026	\N	1177	\N	2007-01-31	Factura Cliente	10534	Factura Cliente Num. 8	0.00	1211.81	\N	\N	\N	\N	\N	\N	\N	\N
26721	\N	1196	\N	2007-02-27	Factura Cliente	10318	Factura Cliente Num. 43	0.00	97.26	\N	\N	\N	\N	\N	\N	\N	\N
26722	\N	1196	\N	2007-02-27	Factura Cliente	10806	Factura Cliente Num. 43	705.11	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26723	\N	1196	\N	2007-02-27	Factura Cliente	10534	Factura Cliente Num. 43	0.00	607.85	\N	\N	\N	\N	\N	\N	\N	\N
26823	\N	1213	\N	2007-04-23	Factura Cliente	10318	Factura Cliente Num. 87	0.00	81.47	\N	\N	\N	\N	\N	\N	\N	\N
26824	\N	1213	\N	2007-04-23	Factura Cliente	10881	Factura Cliente Num. 87	590.67	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26825	\N	1213	\N	2007-04-23	Factura Cliente	10534	Factura Cliente Num. 87	0.00	509.20	\N	\N	\N	\N	\N	\N	\N	\N
26925	\N	1264	\N	2007-03-23	Factura Cliente	10318	Factura Cliente Num. 60	0.00	107.41	\N	\N	\N	\N	\N	\N	\N	\N
26926	\N	1264	\N	2007-03-23	Factura Cliente	10883	Factura Cliente Num. 60	778.70	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26927	\N	1264	\N	2007-03-23	Factura Cliente	10534	Factura Cliente Num. 60	0.00	671.29	\N	\N	\N	\N	\N	\N	\N	\N
27027	\N	1178	\N	2007-01-31	Factura Cliente	10318	Factura Cliente Num. 14	0.00	39.02	\N	\N	\N	\N	\N	\N	\N	\N
27028	\N	1178	\N	2007-01-31	Factura Cliente	10870	Factura Cliente Num. 14	282.90	0.00	\N	\N	\N	\N	\N	\N	\N	\N
27029	\N	1178	\N	2007-01-31	Factura Cliente	10534	Factura Cliente Num. 14	0.00	243.88	\N	\N	\N	\N	\N	\N	\N	\N
26724	\N	1156	\N	2007-02-22	Factura Cliente	10318	Factura Cliente Num. 35	0.00	105.97	\N	\N	\N	\N	\N	\N	\N	\N
26725	\N	1156	\N	2007-02-22	Factura Cliente	10806	Factura Cliente Num. 35	768.29	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26726	\N	1156	\N	2007-02-22	Factura Cliente	10534	Factura Cliente Num. 35	0.00	662.32	\N	\N	\N	\N	\N	\N	\N	\N
26826	\N	1214	\N	2007-03-27	Factura Cliente	10318	Factura Cliente Num. 68	0.00	19.30	\N	\N	\N	\N	\N	\N	\N	\N
26827	\N	1214	\N	2007-03-27	Factura Cliente	10866	Factura Cliente Num. 68	139.90	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26828	\N	1214	\N	2007-03-27	Factura Cliente	10534	Factura Cliente Num. 68	0.00	120.60	\N	\N	\N	\N	\N	\N	\N	\N
26928	\N	1265	\N	2007-03-27	Factura Cliente	10318	Factura Cliente Num. 62	0.00	78.25	\N	\N	\N	\N	\N	\N	\N	\N
26929	\N	1265	\N	2007-03-27	Factura Cliente	10806	Factura Cliente Num. 62	567.30	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26930	\N	1265	\N	2007-03-27	Factura Cliente	10534	Factura Cliente Num. 62	0.00	489.05	\N	\N	\N	\N	\N	\N	\N	\N
27030	\N	1179	\N	2007-01-31	Factura Cliente	10318	Factura Cliente Num. 19	0.00	8.58	\N	\N	\N	\N	\N	\N	\N	\N
27031	\N	1179	\N	2007-01-31	Factura Cliente	10865	Factura Cliente Num. 19	62.18	0.00	\N	\N	\N	\N	\N	\N	\N	\N
27032	\N	1179	\N	2007-01-31	Factura Cliente	10534	Factura Cliente Num. 19	0.00	53.60	\N	\N	\N	\N	\N	\N	\N	\N
26727	\N	1245	\N	2007-02-19	Factura Cliente	10318	Factura Cliente Num. 27	0.00	93.23	\N	\N	\N	\N	\N	\N	\N	\N
26728	\N	1245	\N	2007-02-19	Factura Cliente	10820	Factura Cliente Num. 27	675.93	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26729	\N	1245	\N	2007-02-19	Factura Cliente	10534	Factura Cliente Num. 27	0.00	582.70	\N	\N	\N	\N	\N	\N	\N	\N
26829	\N	1215	\N	2007-04-24	Factura Cliente	10318	Factura Cliente Num. 91	0.00	68.70	\N	\N	\N	\N	\N	\N	\N	\N
26830	\N	1215	\N	2007-04-24	Factura Cliente	10820	Factura Cliente Num. 91	498.07	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26831	\N	1215	\N	2007-04-24	Factura Cliente	10534	Factura Cliente Num. 91	0.00	429.37	\N	\N	\N	\N	\N	\N	\N	\N
26931	\N	1240	\N	2007-03-27	Factura Cliente	10318	Factura Cliente Num. 70	0.00	19.30	\N	\N	\N	\N	\N	\N	\N	\N
26932	\N	1240	\N	2007-03-27	Factura Cliente	10868	Factura Cliente Num. 70	139.90	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26933	\N	1240	\N	2007-03-27	Factura Cliente	10534	Factura Cliente Num. 70	0.00	120.60	\N	\N	\N	\N	\N	\N	\N	\N
27033	\N	1275	\N	2007-02-02	Factura Cliente	10318	Factura Cliente Num. 23	0.00	186.96	\N	\N	\N	\N	\N	\N	\N	\N
27034	\N	1275	\N	2007-02-02	Factura Cliente	10900	Factura Cliente Num. 23	1355.44	0.00	\N	\N	\N	\N	\N	\N	\N	\N
27035	\N	1275	\N	2007-02-02	Factura Cliente	10534	Factura Cliente Num. 23	0.00	1168.48	\N	\N	\N	\N	\N	\N	\N	\N
26730	\N	1246	\N	2007-02-22	Factura Cliente	10318	Factura Cliente Num. 37	0.00	70.75	\N	\N	\N	\N	\N	\N	\N	\N
26731	\N	1246	\N	2007-02-22	Factura Cliente	10881	Factura Cliente Num. 37	512.95	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26732	\N	1246	\N	2007-02-22	Factura Cliente	10534	Factura Cliente Num. 37	0.00	442.20	\N	\N	\N	\N	\N	\N	\N	\N
26832	\N	1216	\N	2007-04-24	Factura Cliente	10318	Factura Cliente Num. 92	0.00	77.66	\N	\N	\N	\N	\N	\N	\N	\N
26833	\N	1216	\N	2007-04-24	Factura Cliente	10806	Factura Cliente Num. 92	563.01	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26834	\N	1216	\N	2007-04-24	Factura Cliente	10534	Factura Cliente Num. 92	0.00	485.35	\N	\N	\N	\N	\N	\N	\N	\N
26934	\N	1241	\N	2007-03-27	Factura Cliente	10318	Factura Cliente Num. 65	0.00	69.43	\N	\N	\N	\N	\N	\N	\N	\N
26935	\N	1241	\N	2007-03-27	Factura Cliente	10870	Factura Cliente Num. 65	503.34	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26936	\N	1241	\N	2007-03-27	Factura Cliente	10534	Factura Cliente Num. 65	0.00	433.91	\N	\N	\N	\N	\N	\N	\N	\N
27036	\N	1276	\N	2007-02-02	Factura Cliente	10318	Factura Cliente Num. 24	0.00	12.98	\N	\N	\N	\N	\N	\N	\N	\N
27037	\N	1276	\N	2007-02-02	Factura Cliente	10843	Factura Cliente Num. 24	94.13	0.00	\N	\N	\N	\N	\N	\N	\N	\N
27038	\N	1276	\N	2007-02-02	Factura Cliente	10534	Factura Cliente Num. 24	0.00	81.15	\N	\N	\N	\N	\N	\N	\N	\N
26106	\N	1007	\N	2007-01-17	Cobro	10867	Cobro VBDXNN	0.00	4135.40	\N	\N	\N	\N	\N	\N	\N	\N
26107	\N	1007	\N	2007-01-17	Cobro	10943	Cobro VBDXNN	4135.40	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26108	\N	1008	\N	2007-01-15	Cobro	10877	Cobro GQGKTW	0.00	1908.86	\N	\N	\N	\N	\N	\N	\N	\N
26109	\N	1008	\N	2007-01-15	Cobro	10943	Cobro GQGKTW	1908.86	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26110	\N	1009	\N	2007-02-05	Cobro	10806	Cobro OARTCQ	0.00	2796.06	\N	\N	\N	\N	\N	\N	\N	\N
26111	\N	1009	\N	2007-02-05	Cobro	10943	Cobro OARTCQ	2796.06	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26112	\N	1010	\N	2007-02-05	Cobro	10881	Cobro KPPHUK	0.00	1402.08	\N	\N	\N	\N	\N	\N	\N	\N
26113	\N	1010	\N	2007-02-05	Cobro	10943	Cobro KPPHUK	1402.08	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26114	\N	1011	\N	2007-02-05	Cobro	10870	Cobro SLRASN	0.00	971.15	\N	\N	\N	\N	\N	\N	\N	\N
26115	\N	1011	\N	2007-02-05	Cobro	10943	Cobro SLRASN	971.15	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26116	\N	1012	\N	2007-02-05	Cobro	10865	Cobro BPRRQJ	0.00	213.44	\N	\N	\N	\N	\N	\N	\N	\N
26117	\N	1012	\N	2007-02-05	Cobro	10943	Cobro BPRRQJ	213.44	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26118	\N	1013	\N	2007-02-05	Cobro	10881	Cobro XLTDFR	0.00	1760.88	\N	\N	\N	\N	\N	\N	\N	\N
26119	\N	1013	\N	2007-02-05	Cobro	10943	Cobro XLTDFR	1760.88	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26120	\N	1014	\N	2007-01-15	Cobro	10877	Cobro GQGKTW	0.00	1908.86	\N	\N	\N	\N	\N	\N	\N	\N
26121	\N	1014	\N	2007-01-15	Cobro	10943	Cobro GQGKTW	1908.86	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26122	\N	1015	\N	2007-02-12	Cobro	10807	Cobro RKSCOE	0.00	6898.53	\N	\N	\N	\N	\N	\N	\N	\N
26123	\N	1015	\N	2007-02-12	Cobro	10943	Cobro RKSCOE	6898.53	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26124	\N	1016	\N	2007-02-12	Cobro	10807	Cobro YKQTLE	0.00	1067.20	\N	\N	\N	\N	\N	\N	\N	\N
26125	\N	1016	\N	2007-02-12	Cobro	10943	Cobro YKQTLE	1067.20	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26126	\N	1017	\N	2007-01-31	Cobro	10896	Cobro XWEVQD	0.00	143.75	\N	\N	\N	\N	\N	\N	\N	\N
26127	\N	1017	\N	2007-01-31	Cobro	10943	Cobro XWEVQD	143.75	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26128	\N	1018	\N	2006-10-27	Cobro	10798	Cobro FJFSVG	0.00	2925.60	\N	\N	\N	\N	\N	\N	\N	\N
26129	\N	1018	\N	2006-10-27	Cobro	10943	Cobro FJFSVG	2925.60	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26130	\N	1019	\N	2007-02-22	Cobro	10798	Cobro FJFSVG	0.00	8774.04	\N	\N	\N	\N	\N	\N	\N	\N
26131	\N	1019	\N	2007-02-22	Cobro	10943	Cobro FJFSVG	8774.04	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26132	\N	1020	\N	2007-01-29	Cobro	10877	Cobro YZOBMY	0.00	173.60	\N	\N	\N	\N	\N	\N	\N	\N
26133	\N	1020	\N	2007-01-29	Cobro	10943	Cobro YZOBMY	173.60	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26134	\N	1021	\N	2007-02-23	Cobro	10877	Cobro LDBTEN	0.00	1322.68	\N	\N	\N	\N	\N	\N	\N	\N
26135	\N	1021	\N	2007-02-23	Cobro	10943	Cobro LDBTEN	1322.68	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26136	\N	1022	\N	2007-02-23	Cobro	10877	Cobro TJEQQB	0.00	2524.80	\N	\N	\N	\N	\N	\N	\N	\N
26137	\N	1022	\N	2007-02-23	Cobro	10943	Cobro TJEQQB	2524.80	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26138	\N	1023	\N	2007-02-23	Cobro	10877	Cobro PTHRMX	0.00	334.28	\N	\N	\N	\N	\N	\N	\N	\N
26139	\N	1023	\N	2007-02-23	Cobro	10943	Cobro PTHRMX	334.28	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26140	\N	1024	\N	2007-02-27	Cobro	10820	Cobro KBUUFV	0.00	2320.29	\N	\N	\N	\N	\N	\N	\N	\N
26141	\N	1024	\N	2007-02-27	Cobro	10943	Cobro KBUUFV	2320.29	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26142	\N	1025	\N	2007-02-27	Cobro	10820	Cobro R[WFFL	0.00	6147.07	\N	\N	\N	\N	\N	\N	\N	\N
26143	\N	1025	\N	2007-02-27	Cobro	10943	Cobro R[WFFL	6147.07	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26144	\N	1026	\N	2007-02-27	Cobro	10820	Cobro MGKWYW	0.00	3073.54	\N	\N	\N	\N	\N	\N	\N	\N
26145	\N	1026	\N	2007-02-27	Cobro	10943	Cobro MGKWYW	3073.54	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26146	\N	1027	\N	2007-03-05	Cobro	10903	Cobro XSZHYS	0.00	874.87	\N	\N	\N	\N	\N	\N	\N	\N
26147	\N	1027	\N	2007-03-05	Cobro	10943	Cobro XSZHYS	874.87	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26148	\N	1028	\N	2007-03-06	Cobro	10877	Cobro RFMSHL	0.00	221.12	\N	\N	\N	\N	\N	\N	\N	\N
26149	\N	1028	\N	2007-03-06	Cobro	10943	Cobro RFMSHL	221.12	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26150	\N	1029	\N	2007-03-01	Cobro	10851	Cobro XMTMXG	0.00	240.12	\N	\N	\N	\N	\N	\N	\N	\N
26151	\N	1029	\N	2007-03-01	Cobro	10943	Cobro XMTMXG	240.12	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26152	\N	1030	\N	2007-03-08	Cobro	10815	Cobro JIZCRY	0.00	48.90	\N	\N	\N	\N	\N	\N	\N	\N
26153	\N	1030	\N	2007-03-08	Cobro	10943	Cobro JIZCRY	48.90	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26154	\N	1031	\N	2007-02-28	Cobro	10806	Cobro [MJXDT	0.00	2103.12	\N	\N	\N	\N	\N	\N	\N	\N
26155	\N	1031	\N	2007-02-28	Cobro	10943	Cobro [MJXDT	2103.12	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26156	\N	1032	\N	2007-02-28	Cobro	10806	Cobro WKNSLS	0.00	1758.81	\N	\N	\N	\N	\N	\N	\N	\N
26157	\N	1032	\N	2007-02-28	Cobro	10943	Cobro WKNSLS	1758.81	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26158	\N	1033	\N	2007-02-28	Cobro	10806	Cobro BHMMAZ	0.00	2420.52	\N	\N	\N	\N	\N	\N	\N	\N
26159	\N	1033	\N	2007-02-28	Cobro	10943	Cobro BHMMAZ	2420.52	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26160	\N	1034	\N	2007-02-28	Cobro	10806	Cobro BWRKUG	0.00	2420.52	\N	\N	\N	\N	\N	\N	\N	\N
26161	\N	1034	\N	2007-02-28	Cobro	10943	Cobro BWRKUG	2420.52	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26162	\N	1035	\N	2007-02-28	Cobro	10881	Cobro MFROGY	0.00	1760.88	\N	\N	\N	\N	\N	\N	\N	\N
26163	\N	1035	\N	2007-02-28	Cobro	10943	Cobro MFROGY	1760.88	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26164	\N	1036	\N	2007-02-28	Cobro	10858	Cobro SWELET	0.00	960.48	\N	\N	\N	\N	\N	\N	\N	\N
26165	\N	1036	\N	2007-02-28	Cobro	10943	Cobro SWELET	960.48	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26166	\N	1037	\N	2007-02-28	Cobro	10806	Cobro LVVDTM	0.00	1595.37	\N	\N	\N	\N	\N	\N	\N	\N
26167	\N	1037	\N	2007-02-28	Cobro	10943	Cobro LVVDTM	1595.37	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26168	\N	1038	\N	2007-02-28	Cobro	10806	Cobro XOYIET	0.00	2637.36	\N	\N	\N	\N	\N	\N	\N	\N
26169	\N	1038	\N	2007-02-28	Cobro	10943	Cobro XOYIET	2637.36	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26170	\N	1039	\N	2007-02-28	Cobro	10878	Cobro HCIVYT	0.00	1096.09	\N	\N	\N	\N	\N	\N	\N	\N
26171	\N	1039	\N	2007-02-28	Cobro	10943	Cobro HCIVYT	1096.09	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26172	\N	1040	\N	2007-02-28	Cobro	10870	Cobro XHKBSU	0.00	971.15	\N	\N	\N	\N	\N	\N	\N	\N
26173	\N	1040	\N	2007-02-28	Cobro	10943	Cobro XHKBSU	971.15	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26174	\N	1041	\N	2007-03-20	Cobro	10885	Cobro GFUNYR	0.00	426.88	\N	\N	\N	\N	\N	\N	\N	\N
26175	\N	1041	\N	2007-03-20	Cobro	10943	Cobro GFUNYR	426.88	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26176	\N	1042	\N	2007-03-20	Cobro	10885	Cobro GITZDY	0.00	426.88	\N	\N	\N	\N	\N	\N	\N	\N
26177	\N	1042	\N	2007-03-20	Cobro	10943	Cobro GITZDY	426.88	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26178	\N	1043	\N	2007-03-29	Cobro	10802	Cobro VVSOHG	0.00	369.79	\N	\N	\N	\N	\N	\N	\N	\N
26179	\N	1043	\N	2007-03-29	Cobro	10943	Cobro VVSOHG	369.79	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26180	\N	1044	\N	2007-03-28	Cobro	10830	Cobro SKAKPN	0.00	160.08	\N	\N	\N	\N	\N	\N	\N	\N
26181	\N	1044	\N	2007-03-28	Cobro	10943	Cobro SKAKPN	160.08	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26182	\N	1045	\N	2007-03-29	Cobro	10830	Cobro IQSXKB	0.00	160.08	\N	\N	\N	\N	\N	\N	\N	\N
26183	\N	1045	\N	2007-03-29	Cobro	10943	Cobro IQSXKB	160.08	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26184	\N	1046	\N	2007-03-29	Cobro	10830	Cobro SIWRWI	0.00	160.08	\N	\N	\N	\N	\N	\N	\N	\N
26185	\N	1046	\N	2007-03-29	Cobro	10943	Cobro SIWRWI	160.08	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26186	\N	1047	\N	2007-03-29	Cobro	10883	Cobro VDRMHJ2	0.00	1493.85	\N	\N	\N	\N	\N	\N	\N	\N
26187	\N	1047	\N	2007-03-29	Cobro	10943	Cobro VDRMHJ2	1493.85	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26188	\N	1048	\N	2007-03-29	Cobro	10883	Cobro QENLSH1	0.00	2673.11	\N	\N	\N	\N	\N	\N	\N	\N
26189	\N	1048	\N	2007-03-29	Cobro	10943	Cobro QENLSH1	2673.11	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26190	\N	1049	\N	2007-03-29	Cobro	10883	Cobro QENLSH	0.00	1493.85	\N	\N	\N	\N	\N	\N	\N	\N
26191	\N	1049	\N	2007-03-29	Cobro	10943	Cobro QENLSH	1493.85	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26192	\N	1050	\N	2007-03-29	Cobro	10851	Cobro KZGVJG	0.00	240.12	\N	\N	\N	\N	\N	\N	\N	\N
26193	\N	1050	\N	2007-03-29	Cobro	10943	Cobro KZGVJG	240.12	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26194	\N	1051	\N	2007-03-29	Cobro	10878	Cobro RJGEKI	0.00	640.32	\N	\N	\N	\N	\N	\N	\N	\N
26195	\N	1051	\N	2007-03-29	Cobro	10943	Cobro RJGEKI	640.32	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26196	\N	1052	\N	2007-03-29	Cobro	10858	Cobro WVYAUN	0.00	400.20	\N	\N	\N	\N	\N	\N	\N	\N
26197	\N	1052	\N	2007-03-29	Cobro	10943	Cobro WVYAUN	400.20	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26198	\N	1053	\N	2007-03-29	Cobro	10849	Cobro PVEZPK	0.00	640.32	\N	\N	\N	\N	\N	\N	\N	\N
26199	\N	1053	\N	2007-03-29	Cobro	10943	Cobro PVEZPK	640.32	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26200	\N	1054	\N	2007-03-29	Cobro	10806	Cobro SGPNZS	0.00	1811.34	\N	\N	\N	\N	\N	\N	\N	\N
26201	\N	1054	\N	2007-03-29	Cobro	10943	Cobro SGPNZS	1811.34	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26202	\N	1055	\N	2007-03-29	Cobro	10806	Cobro ECEHFZ	0.00	1008.50	\N	\N	\N	\N	\N	\N	\N	\N
26203	\N	1055	\N	2007-03-29	Cobro	10943	Cobro ECEHFZ	1008.50	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26204	\N	1056	\N	2007-03-29	Cobro	10806	Cobro HJKUGG	0.00	1947.41	\N	\N	\N	\N	\N	\N	\N	\N
26205	\N	1056	\N	2007-03-29	Cobro	10943	Cobro HJKUGG	1947.41	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26206	\N	1057	\N	2007-03-29	Cobro	10868	Cobro ZCPBFD	0.00	480.24	\N	\N	\N	\N	\N	\N	\N	\N
26207	\N	1057	\N	2007-03-29	Cobro	10943	Cobro ZCPBFD	480.24	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26208	\N	1058	\N	2007-03-29	Cobro	10870	Cobro DQCDPV	0.00	1727.85	\N	\N	\N	\N	\N	\N	\N	\N
26209	\N	1058	\N	2007-03-29	Cobro	10943	Cobro DQCDPV	1727.85	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26210	\N	1059	\N	2007-03-29	Cobro	10864	Cobro FTLK[Q	0.00	480.24	\N	\N	\N	\N	\N	\N	\N	\N
26211	\N	1059	\N	2007-03-29	Cobro	10943	Cobro FTLK[Q	480.24	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26212	\N	1060	\N	2007-03-29	Cobro	10865	Cobro CMNXZZ	0.00	213.44	\N	\N	\N	\N	\N	\N	\N	\N
26213	\N	1060	\N	2007-03-29	Cobro	10943	Cobro CMNXZZ	213.44	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26214	\N	1061	\N	2007-03-29	Cobro	10881	Cobro ESWOMB	0.00	1760.88	\N	\N	\N	\N	\N	\N	\N	\N
26215	\N	1061	\N	2007-03-29	Cobro	10943	Cobro ESWOMB	1760.88	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26216	\N	1062	\N	2007-04-02	Cobro	10804	Cobro QYIBYU	0.00	976.81	\N	\N	\N	\N	\N	\N	\N	\N
26217	\N	1062	\N	2007-04-02	Cobro	10943	Cobro QYIBYU	976.81	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26218	\N	1063	\N	2007-04-03	Cobro	10804	Cobro XQBOOR	0.00	1449.78	\N	\N	\N	\N	\N	\N	\N	\N
26219	\N	1063	\N	2007-04-03	Cobro	10943	Cobro XQBOOR	1449.78	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26220	\N	1064	\N	2007-04-03	Cobro	10804	Cobro [VYFS[	0.00	10672.00	\N	\N	\N	\N	\N	\N	\N	\N
26221	\N	1064	\N	2007-04-03	Cobro	10943	Cobro [VYFS[	10672.00	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26222	\N	1065	\N	2007-03-30	Cobro	10901	Cobro YQPCIE	0.00	156.40	\N	\N	\N	\N	\N	\N	\N	\N
26223	\N	1065	\N	2007-03-30	Cobro	10943	Cobro YQPCIE	156.40	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26224	\N	1066	\N	2007-05-01	Cobro	10802	Cobro IVXVYD	0.00	156.31	\N	\N	\N	\N	\N	\N	\N	\N
26225	\N	1066	\N	2007-05-01	Cobro	10943	Cobro IVXVYD	156.31	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26226	\N	1067	\N	2007-05-03	Cobro	10863	Cobro CLL[ZS	0.00	2241.12	\N	\N	\N	\N	\N	\N	\N	\N
26227	\N	1067	\N	2007-05-03	Cobro	10943	Cobro CLL[ZS	2241.12	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26228	\N	1068	\N	2007-04-27	Cobro	10892	Cobro CNJXJO	0.00	6349.84	\N	\N	\N	\N	\N	\N	\N	\N
26229	\N	1068	\N	2007-04-27	Cobro	10943	Cobro CNJXJO	6349.84	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26230	\N	1069	\N	2007-04-26	Cobro	10890	Cobro QBVUMK	0.00	368.00	\N	\N	\N	\N	\N	\N	\N	\N
26231	\N	1069	\N	2007-04-26	Cobro	10936	Cobro QBVUMK	368.00	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26232	\N	1070	\N	2007-04-24	Cobro	10802	Cobro MYVGGU	0.00	15105.99	\N	\N	\N	\N	\N	\N	\N	\N
26233	\N	1070	\N	2007-04-24	Cobro	10936	Cobro MYVGGU	15105.99	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26234	\N	1071	\N	2007-04-20	Cobro	10890	Cobro QBVUMK	0.00	2300.00	\N	\N	\N	\N	\N	\N	\N	\N
26235	\N	1071	\N	2007-04-20	Cobro	10936	Cobro QBVUMK	2300.00	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26236	\N	1072	\N	2007-05-02	Cobro	10820	Cobro PNWNYA	0.00	1672.10	\N	\N	\N	\N	\N	\N	\N	\N
26237	\N	1072	\N	2007-05-02	Cobro	10936	Cobro PNWNYA	1672.10	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26238	\N	1073	\N	2007-05-02	Cobro	10820	Cobro KBJKOH	0.00	3073.54	\N	\N	\N	\N	\N	\N	\N	\N
26239	\N	1073	\N	2007-05-02	Cobro	10936	Cobro KBJKOH	3073.54	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26240	\N	1074	\N	2007-04-30	Cobro	10881	Cobro OIWXYB	0.00	2027.68	\N	\N	\N	\N	\N	\N	\N	\N
26241	\N	1074	\N	2007-04-30	Cobro	10936	Cobro OIWXYB	2027.68	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26242	\N	1075	\N	2007-04-30	Cobro	10806	Cobro NUYUXO	0.00	1932.69	\N	\N	\N	\N	\N	\N	\N	\N
26243	\N	1075	\N	2007-04-30	Cobro	10936	Cobro NUYUXO	1932.69	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26244	\N	1076	\N	2007-04-30	Cobro	10870	Cobro YTWDIY	0.00	971.15	\N	\N	\N	\N	\N	\N	\N	\N
26245	\N	1076	\N	2007-04-30	Cobro	10936	Cobro YTWDIY	971.15	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26246	\N	1077	\N	2007-04-30	Cobro	10865	Cobro UAWJDP	0.00	213.44	\N	\N	\N	\N	\N	\N	\N	\N
26247	\N	1077	\N	2007-04-30	Cobro	10936	Cobro UAWJDP	213.44	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26248	\N	1078	\N	2007-05-03	Cobro	10889	Cobro INLXZA	0.00	1920.96	\N	\N	\N	\N	\N	\N	\N	\N
26249	\N	1078	\N	2007-05-03	Cobro	10936	Cobro INLXZA	1920.96	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26250	\N	1079	\N	2007-03-31	Cobro	10806	Cobro COAQPM	0.00	2587.96	\N	\N	\N	\N	\N	\N	\N	\N
26251	\N	1079	\N	2007-03-31	Cobro	10936	Cobro COAQPM	2587.96	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26252	\N	1080	\N	2007-03-31	Cobro	10806	Cobro ZRDELL	0.00	968.35	\N	\N	\N	\N	\N	\N	\N	\N
26253	\N	1080	\N	2007-03-31	Cobro	10936	Cobro ZRDELL	968.35	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26254	\N	1081	\N	2007-03-30	Cobro	10806	Cobro AXDFEF	0.00	747.04	\N	\N	\N	\N	\N	\N	\N	\N
26255	\N	1081	\N	2007-03-30	Cobro	10936	Cobro AXDFEF	747.04	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26256	\N	1082	\N	2007-03-17	Cobro	10806	Cobro WHRNDO	0.00	860.71	\N	\N	\N	\N	\N	\N	\N	\N
26257	\N	1082	\N	2007-03-17	Cobro	10936	Cobro WHRNDO	860.71	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26258	\N	1083	\N	2007-03-29	Cobro	10858	Cobro WVYAUN	0.00	400.20	\N	\N	\N	\N	\N	\N	\N	\N
26259	\N	1083	\N	2007-03-29	Cobro	10936	Cobro WVYAUN	400.20	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26260	\N	1084	\N	2007-03-17	Cobro	10866	Cobro HGOYZQ	0.00	160.08	\N	\N	\N	\N	\N	\N	\N	\N
26261	\N	1084	\N	2007-03-17	Cobro	10936	Cobro HGOYZQ	160.08	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26262	\N	1085	\N	2007-02-19	Cobro	10885	Cobro YIKBD[	0.00	1188.87	\N	\N	\N	\N	\N	\N	\N	\N
26263	\N	1085	\N	2007-02-19	Cobro	10943	Cobro YIKBD[	1188.87	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26264	\N	1086	\N	2007-03-29	Cobro	10866	Cobro JXKSHV	0.00	480.24	\N	\N	\N	\N	\N	\N	\N	\N
26265	\N	1086	\N	2007-03-29	Cobro	10936	Cobro JXKSHV	480.24	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26266	\N	1087	\N	2007-03-29	Cobro	10864	Cobro JXKSHV	0.00	480.24	\N	\N	\N	\N	\N	\N	\N	\N
26267	\N	1087	\N	2007-03-29	Cobro	10943	Cobro JXKSHV	480.24	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26268	\N	1088	\N	2007-05-09	Cobro	10882	Cobro CJIKTG	0.00	160.08	\N	\N	\N	\N	\N	\N	\N	\N
26269	\N	1088	\N	2007-05-09	Cobro	10936	Cobro CJIKTG	160.08	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26270	\N	1089	\N	2007-05-11	Cobro	10867	Cobro UNIIYN	0.00	290.44	\N	\N	\N	\N	\N	\N	\N	\N
26271	\N	1089	\N	2007-05-11	Cobro	10937	Cobro UNIIYN	290.44	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26272	\N	1090	\N	2007-05-11	Cobro	10867	Cobro LRMBBX	0.00	501.58	\N	\N	\N	\N	\N	\N	\N	\N
26273	\N	1090	\N	2007-05-11	Cobro	10937	Cobro LRMBBX	501.58	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26274	\N	1091	\N	2007-05-11	Cobro	10818	Cobro KQTYYO	0.00	186.81	\N	\N	\N	\N	\N	\N	\N	\N
26275	\N	1091	\N	2007-05-11	Cobro	10938	Cobro KQTYYO	186.81	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26276	\N	1092	\N	2007-04-12	Cobro	10877	Cobro EFEGZF	0.00	234.19	\N	\N	\N	\N	\N	\N	\N	\N
26277	\N	1092	\N	2007-04-12	Cobro	10937	Cobro EFEGZF	234.19	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26278	\N	1093	\N	2007-05-11	Cobro	10881	Cobro GWCFSO	0.00	2331.51	\N	\N	\N	\N	\N	\N	\N	\N
26279	\N	1093	\N	2007-05-11	Cobro	10936	Cobro GWCFSO	2331.51	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26280	\N	1094	\N	2007-05-11	Cobro	10868	Cobro DOOEWQ	0.00	160.08	\N	\N	\N	\N	\N	\N	\N	\N
26281	\N	1094	\N	2007-05-11	Cobro	10936	Cobro DOOEWQ	160.08	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26282	\N	1095	\N	2007-05-11	Cobro	10887	Cobro KUHKXO	0.00	640.32	\N	\N	\N	\N	\N	\N	\N	\N
26283	\N	1095	\N	2007-05-11	Cobro	10936	Cobro KUHKXO	640.32	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26284	\N	1096	\N	2007-08-14	Cobro	10845	Cobro TYZ[VR	0.00	6900.00	\N	\N	\N	\N	\N	\N	\N	\N
26285	\N	1096	\N	2007-08-14	Cobro	10943	Cobro TYZ[VR	6900.00	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26286	\N	1097	\N	2007-03-14	Cobro	10856	Cobro LQIDBZ	0.00	4652.99	\N	\N	\N	\N	\N	\N	\N	\N
26287	\N	1097	\N	2007-03-14	Cobro	10943	Cobro LQIDBZ	4652.99	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26288	\N	1098	\N	2007-05-24	Cobro	10879	Cobro HGMRII	0.00	465.75	\N	\N	\N	\N	\N	\N	\N	\N
26289	\N	1098	\N	2007-05-24	Cobro	10937	Cobro HGMRII	465.75	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26290	\N	1099	\N	2007-05-24	Cobro	10830	Cobro EIY[ED	0.00	160.08	\N	\N	\N	\N	\N	\N	\N	\N
26291	\N	1099	\N	2007-05-24	Cobro	10936	Cobro EIY[ED	160.08	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26292	\N	1100	\N	2007-05-19	Cobro	10802	Cobro NGIXEL	0.00	1109.34	\N	\N	\N	\N	\N	\N	\N	\N
26293	\N	1100	\N	2007-05-19	Cobro	10936	Cobro NGIXEL	1109.34	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26294	\N	1101	\N	2007-05-07	Cobro	10878	Cobro FGTPBN	0.00	640.32	\N	\N	\N	\N	\N	\N	\N	\N
26295	\N	1101	\N	2007-05-07	Cobro	10936	Cobro FGTPBN	640.32	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26296	\N	1102	\N	2007-05-28	Cobro	10857	Cobro KNMZWF	0.00	920.00	\N	\N	\N	\N	\N	\N	\N	\N
26297	\N	1102	\N	2007-05-28	Cobro	10937	Cobro KNMZWF	920.00	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26298	\N	1103	\N	2007-05-24	Cobro	10830	Cobro IFREQT	0.00	160.08	\N	\N	\N	\N	\N	\N	\N	\N
26299	\N	1103	\N	2007-05-24	Cobro	10936	Cobro IFREQT	160.08	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26300	\N	1104	\N	2007-05-24	Cobro	10885	Cobro XWZFGT	0.00	426.88	\N	\N	\N	\N	\N	\N	\N	\N
26301	\N	1104	\N	2007-05-24	Cobro	10936	Cobro XWZFGT	426.88	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26302	\N	1105	\N	2007-05-24	Cobro	10885	Cobro VYBAMY	0.00	426.88	\N	\N	\N	\N	\N	\N	\N	\N
26303	\N	1105	\N	2007-05-24	Cobro	10936	Cobro VYBAMY	426.88	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26304	\N	1106	\N	2007-05-25	Cobro	10804	Cobro NNZNUN	0.00	8004.00	\N	\N	\N	\N	\N	\N	\N	\N
26305	\N	1106	\N	2007-05-25	Cobro	10936	Cobro NNZNUN	8004.00	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26306	\N	1107	\N	2007-05-24	Cobro	10830	Cobro SZYMUR	0.00	226.96	\N	\N	\N	\N	\N	\N	\N	\N
26307	\N	1107	\N	2007-05-24	Cobro	10936	Cobro SZYMUR	226.96	0.00	\N	\N	\N	\N	\N	\N	\N	\N
26308	\N	1108	\N	2007-02-05	Cobro	10798	Cobro YCIHXH	0.00	1467.40	\N	\N	\N	\N	\N	\N	\N	\N
26309	\N	1108	\N	2007-02-05	Cobro	10943	Cobro YCIHXH	1467.40	0.00	\N	\N	\N	\N	\N	\N	\N	\N
\.


--
-- Data for Name: c_coste; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY c_coste (idc_coste, descripcion, nombre, codigo, padre, imputacion, debe, haber) FROM stdin;
27	Escriba su descripcion	El centro de coste	\N	\N	\N	0.00	0.00
32	Escriba su descripcion	SuperMan distribuido	\N	\N	\N	0.00	0.00
24	Escriba su descripcion	Otro seg ccoste	\N	20	\N	0.00	0.00
20	Escriba su descripcion	Nuevo centro de coste	\N	16	\N	0.00	0.00
16	Escriba su descripcion	Nro de coste	\N	\N	\N	0.00	0.00
\.


--
-- Data for Name: c_costedist; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY c_costedist (idc_costedist, idc_coste, iddestc_coste, porcentc_costedist) FROM stdin;
8	24	27	50.00
9	16	27	50.00
4	27	32	50.00
5	20	32	50.00
\.


--
-- Data for Name: canal; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY canal (idcanal, descripcion, nombre) FROM stdin;
4	Escriba su descripcion	aasdfasdf
2	Escribescripcion	sdfgsdf
5	Escriba su descripcion	Nuevo canal
6	Escriba su descripcion	Nuevo canal
\.


--
-- Data for Name: compbalance; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY compbalance (idcompbalance, idbalance, idmpatrimonial, concepto, orden, tabulacion) FROM stdin;
\.


--
-- Data for Name: compmasap; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY compmasap (idcompmasap, idcuenta, idmpatrimonial, masaperteneciente, saldo, signo, tipocompmasap, nombre) FROM stdin;
\.


--
-- Data for Name: configuracion; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY configuracion (nombre, valor) FROM stdin;
Amortizacion	Amortizacion
Cobro	Cobro
CIF	--
TipoIva	--
NombreVia	--
NumeroVia	--
Escalera	--
Piso	--
Puerta	--
CodPostal	--
Municipio	--
Provincia	--
Pais	--
Tipo	BulmaCont
Ejercicio	2007
RegistroEmitida	472
RegistroSoportada	477
CuentaRegularizacion	129
CuentasIngresos	60;62
CuentasGastos	70
CuentasDerechos	43;3
CuentasObligaciones	40;41
Pago	Pago
NombreEmpresa	Pruebas_Conetxia
CodCuenta	xxxxyyyy
DatabaseRevision	0.10.1-0005
DBRev-PresContables	0.10.1-0001
\.


--
-- Data for Name: cuenta; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY cuenta (idcuenta, codigo, descripcion, imputacion, padre, bloqueada, msg, debe, haber, nodebe, nohaber, regularizacion, activo, nombreent_cuenta, cifent_cuenta, cpent_cuenta, dirent_cuenta, telent_cuenta, coment_cuenta, bancoent_cuenta, emailent_cuenta, webent_cuenta, tipocuenta, idprovincia, poblacion, idc_coste) FROM stdin;
10320	480	Gastos anticipados	t	10035	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10078	129	PÃ©rdidas y ganancias	t	10003	f	\N	0.00	0.00	f	f	\N	t	PÃ©rdidas y ganancias									2	\N	\N	\N
10003	12	RESULTADOS PENDIENTES DE APLICACIÃN	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10002	11	RESERVAS	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10004	13	INGRESOS A DISTRIBUIR EN VARIOS EJERCICIOS	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10005	14	PROVISIONES PARA RIESGOS Y GASTOS	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10006	15	EMPRESITOS Y OTRAS EMISIONES ANÃLOGAS	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10007	16	DEUDAS A LARGO PLAZO CON EMPRESAS DEL GRUPO Y ASOCIADAS	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10008	17	DEUDAS A LARGO PLAZO POR PRESTAMOS RECIBIDOS Y OTROS CONCEPTOS	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10009	18	FIANZAS Y DEPÃSITOS RECIBIDOS A LARGO PLAZO	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10010	19	SITUACIONES TRANSITORIAS DE FINANCIACIÃN	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10011	20	GASTOS DE ESTABLECIMIENTO	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10012	21	INMOVILIZACIONES INMATERIALES	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10013	22	INMOVILIZACIONES MATERIALES	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10014	23	INMOVILIZACIONES MATERIALES EN CURSO	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10015	24	INVERSIONES FINANCIERAS EN EMPRESAS DEL GRUPO Y ASOCIADAS	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10016	25	OTRAS INVERSIONES FINANCIERAS	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10017	26	FIANZAS Y DEPOSITOS CONSTITUIDOS A LARGO PLAZO	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10018	27	GASTOS A DISTRIBUIR EN VARIOS EJERCICIOS	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10019	28	AMORTIZACIÃN ACUMULADA DEL INMOVILIZADO	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10020	29	PROVISIONES DE INMOVILIZADO	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10021	30	COMERCIALES	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10022	31	MATERIAS PRIMAS	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10023	32	OTROS APROVISIONAMIENTOS	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10024	33	PRODUCTOS EN CURSO	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10025	34	PRODUCTOS SEMITERMINADOS	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10026	35	PRODUCTOS TERMINADOS	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10027	36	PRODUCTOS, RESIDUOS Y MATERIALES RECUPERADOS	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10028	39	PROVISIONES POR DEPRECIACIÃN DE EXISTENCIAS	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10030	41	ACREEDORES VARIOS	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10032	44	DEUDORES VARIOS	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10033	46	PERSONAL	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10035	48	AJUSTES POR PERIODIFICACIÃN	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10036	49	PROVISIONES POR OPERACIONES DE TRÃFICO	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10037	50	EMPRESITOS Y OTRAS EMISIONES ANÃLOGAS A CORTO PLAZO	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10038	51	DEUDAS A CORTO PLAZO CON EMPRESAS DEL GRUPO Y ASOCIADAS	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10039	52	DEUDAS A CORTO PLAZO POR PRESTAMOS RECIBIDOS Y OTROS CONCEPTOS	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10040	53	INVERSIONES FINANCIERAS A CORTO PLAZO EN EMPRESAS DEL GRUPO Y ASOCIADAS	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10041	54	OTRAS INVERSIONES FINANCIERAS TEMPORALES	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10042	55	OTRAS CUENTAS NO BANCARIAS	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10043	56	FIANZAS Y DEPOSITOS RECIBIDOS Y CONSTITUIDOS A CORTO PLAZO	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10045	58	AJUSTES POR PERIODIFICACIÃN	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10046	59	PROVISIONES FINANCIERAS	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10048	61	VARIACIÃN DE EXISTENCIAS	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10049	62	SERVICIOS EXTERIORES	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10050	63	TRIBUTOS	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10051	64	GASTOS DE PERSONAL	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10052	65	OTROS GASTOS DE GESTIÃN	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10053	66	GASTOS FINANCIEROS	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10054	67	PÃRDIDAS PROCEDENTES DEL INMOVILIZADO Y GASTOS EXCEPCIONALES	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10055	68	DOTACIONES A LAS PROVISIONES	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10056	69	DOTACIONES A LAS PROVISIONES	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10058	71	VARIACIÃN DE EXISTENCIAS	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10059	73	TRABAJOS REALIZADOS PARA LA EMPRESA	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10060	74	SUBVENCIONES A LA EXPLOTACIÃN	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10061	75	OTROS INGRESOS DE GESTIÃN	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10062	76	INGRESOS FINANCIEROS	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10063	77	BENEFICIOS PROCEDENTES DEL INMOVILIZADO E INGRESOS EXCEPCIONALES	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10064	79	EXCESOS Y APLICACIONES DE PROVISIONES	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10066	102	Capital	t	10001	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	3	\N	\N	\N
10067	101	Fondo social	t	10001	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10069	117	Reservas voluntarias	t	10002	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10070	113	Reservas especiales	t	10002	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10071	115	Reservas para acciones propias	t	10002	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10072	112	Reserva legal	t	10002	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10073	118	Reservas por capital amortizado	t	10002	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10074	110	Prima de emisiÃ³n de acciones	t	10002	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10075	111	Reservas de revalorizaciÃ³n	t	10002	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10076	116	Reservas estatutarias	t	10002	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10077	114	Reservas para acciones de la sociedad dominante	t	10002	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10079	120	Remanente	t	10003	f	\N	0.00	0.00	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10080	122	Aportaciones de socios para compensaciÃ³n de perdidas	t	10003	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10081	121	Resultados negativos de ejercicios anteriores	t	10003	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10082	130	Subvenciones oficiales de capital	t	10004	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10083	131	Subvenciones de capital	t	10004	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10084	135	Ingresos por interes diferidos	t	10004	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10085	136	Diferencias positivas en moneda extranjera	t	10004	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10086	1300	Subvenciones del Estado	t	10082	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10087	1301	Subvenciones de Otras Administraciones Publicas	t	10082	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10088	141	ProvisiÃ³n para impuestos	t	10005	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10089	142	ProvisiÃ³n para responsabilidades	t	10005	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10090	143	ProvisiÃ³n para grandes reparaciones	t	10005	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10091	140	Provisiones para pensiones y obligaciones similares	t	10005	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10092	144	Fondo de reversiÃ³n	t	10005	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10093	150	Obligaciones y bonos	t	10006	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10094	151	Obligaciones y bonos convertibles	t	10006	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10095	155	Deudas representadas en otro valores negociables	t	10006	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10096	1502	Obligaciones y bonos subordinados	t	10093	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10097	1503	Obligaciones y bonos cupon cero	t	10093	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10098	1504	Obligaciones y bonos con opciÃ³n de adquisiciÃ³n de acciones	t	10093	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10099	1505	Obligaciones y bonos con participaciÃ³n de beneficios	t	10093	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10100	1501	Obligaciones y bonos garantizados	t	10093	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10101	1500	Obligaciones y bonos simples	t	10093	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10102	161	Deudas a largo plazo con empresas asociadas	t	10007	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10103	162	Deudas a largo plazo con entidades de credito del grupo	t	10007	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10104	163	Deudas a largo plazo con entidades de credito asociadas	t	10007	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10105	164	Proveedores de inmovilizado a largo plazo, empresas del grupo	t	10007	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10106	165	proveedores de inmovilizado a largo plazo, empresas asociadas	t	10007	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10107	160	Deudas a largo plazo con empresas del grupo	t	10007	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10108	1609	Otras deudas a largo plazo con empresas del grupo	t	10107	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10109	1600	Prestamos a largo plazo de empresas del grupo	t	10107	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10110	173	Proveedores de inmovilizado a largo plazo	t	10008	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10111	174	Efectos a pagar a largo plazo	t	10008	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10112	171	Deudas a largo plazo	t	10008	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10113	172	Deudas a largo plazo transformables en subvenciones	t	10008	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10114	170	Deudas a largo plazo con entidades de crÃ©dito	t	10008	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10115	1700	Prestamos a largo plazo de entidades de crÃ©dito	t	10114	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10116	1709	Otras deudas a largo plazo con entidades de crÃ©dito	t	10114	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10117	180	Fianzas recibidas a largo plazo	t	10009	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10118	185	Depositos recibidos a largo plazo	t	10009	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10119	198	Acciones propias en situaciones especiales	t	10010	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10120	199	Acciones propias para reduccion del capital	t	10010	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10121	192	Accionistas por desembolsos no exigidos, empresas asociadas	t	10010	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10122	190	Accionistas por desembolsos no exigidos	t	10010	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10123	193	Accionistas por aportaciones no dinerarias pendientes	t	10010	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10124	194	Accionistas por aportaciones no dinerarias pendientes, empresas del grupo	t	10010	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10125	195	Accionistas por aportaciones no dinerarias pendientes, empresas asociadas	t	10010	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10126	196	Socios, parte no desembolsada	t	10010	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10127	191	Accionistas por desembolsos no exigidos, empresas del grupo	t	10010	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10128	201	Gastos de primer establecimiento	t	10011	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	5	\N	\N	\N
10129	200	Gastos de constituciÃ³n	t	10011	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	5	\N	\N	\N
10130	202	Gastos de ampliacion de capital	t	10011	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	5	\N	\N	\N
10131	212	Propiedad industrial	t	10012	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10132	211	Concesiones administrativas	t	10012	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10133	217	Derechos sobre bienes en regimen de arrendamientos financieros	t	10012	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10134	219	Anticipos para inmovilizaciones inmateriales	t	10012	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10135	214	Derechos de traspase	t	10012	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10136	213	Fondo de comercio	t	10012	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10137	210	Gastos de investigaciÃ³n y desarrollo	t	10012	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	5	\N	\N	\N
10138	215	Aplicaciones informÃ¡ticas	t	10012	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	5	\N	\N	\N
10139	2100	Gastos de investigaciÃ³n y desarrollo en proyectos no terminados	t	10137	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	5	\N	\N	\N
10140	2101	Gastos de investigaciÃ³n y desarrollo en proyectos terminados	t	10137	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	5	\N	\N	\N
10141	222	Instalaciones tÃ©cnicas	t	10013	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10142	227	Equipos para proceso de informaciÃ³n	t	10013	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10143	224	Utillaje	t	10013	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10144	229	Otro inmovilizado material	t	10013	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10145	223	Maquinaria	t	10013	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10146	225	Otras instalaciones	t	10013	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10147	221	Construcciones	t	10013	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10148	220	Terrenos y bienes naturales	t	10013	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10149	228	Elementos de transporte	t	10013	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10150	226	Mobiliario	t	10013	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10151	230	Adaptacion de terrenos y de bienes naturales	t	10014	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10152	233	Maquinaria en montaje	t	10014	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10153	232	Instalaciones tÃ©cnicas en montaje	t	10014	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10154	231	Construcciones en curso	t	10014	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10155	239	Anticipos para inmovilizaciones materiales	t	10014	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10156	237	Equipos para procesos de informaciÃ³n en montaje	t	10014	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10157	242	Valores de renta fija de empresas del grupo	t	10015	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10158	240	Participaciones en empresas del grupo	t	10015	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10159	241	Participaciones en empresas asociadas	t	10015	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10160	243	Valores de renta fija de empresas asociadas	t	10015	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10161	244	CrÃ©ditos a largo plazo a empresas del grupo	t	10015	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10162	245	CrÃ©ditos a largo plazo a empresas asociadas	t	10015	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10163	246	Intereses a largo plazo de inversiones financieras en empresas del grupo	t	10015	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10164	247	Intereses a largo plazo de inversiones financieras en empresas asociadas	t	10015	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10165	248	Desembolsos pendientes sobre acciones de empresas del grupo	t	10015	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10166	249	Desembolsos pendientes sobre acciones de empresas asociadas	t	10015	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10167	251	Valores de renta fija	t	10016	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10168	259	Desembolsos pendientes sobre acciones	t	10016	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10169	258	Imposiciones a largo plazo	t	10016	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10170	256	Intereses a largo plazo de valores de renta fija	t	10016	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10171	254	CrÃ©ditos a largo plazo al personal	t	10016	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10172	253	CrÃ©ditos a largo plazo por enajenacion de inmovilizado	t	10016	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10173	252	CrÃ©ditos a largo plazo	t	10016	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10174	250	Inversiones financieras permanentes en capital	t	10016	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10175	2501	Inversiones financieras permanentes en acciones sin cotizaciÃ³n en un mercado secundario 	t	10174	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10176	2502	Otras inversiones financieras en capital	t	10174	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10177	2500	Inversiones financieras permanentes en acciones con cotizaciÃ³n en un mercado secundario 	t	10174	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10178	257	Intereses a largo plazo de crÃ©ditos	t	10170	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10179	260	Fianzas constituidas a largo plazo	t	10017	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10180	261	DepÃ³sitos constituidos a largo plazo	t	10017	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10181	270	Gastos de formalizaciÃ³n de deudas	t	10018	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10182	272	Gastos por intereses diferidos	t	10018	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10183	271	Gastos por intereses diferidos en valores negociables	t	10018	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10184	282	AmortizaciÃ³n acumulada del inmovilizado material	t	10019	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10185	281	AmortizaciÃ³n acumulada del inmovilizado inmaterial	t	10019	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10186	2815	AmortizaciÃ³n acumulada de aplicaciones informaticas	t	10185	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10187	2810	AmortizaciÃ³n acumulada de gastos de investigaciÃ³n y desarrollo	t	10185	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10188	2811	AmortizaciÃ³n acumulada de concesiones administrativas	t	10185	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10189	2812	AmortizaciÃ³n acumulada de propiedad industrial	t	10185	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10190	2813	AmortizaciÃ³n acumulada de fondo de comercio	t	10185	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10191	2814	AmortizaciÃ³n acumulada de derechos de traspaso	t	10185	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10192	2817	AmortizaciÃ³n acumulada de derechos sobre bienes en rÃ©gimen de arrendamiento financiero	t	10185	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10193	2828	AmortizaciÃ³n acumulada de elementos de transporte	t	10184	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10194	2821	AmortizaciÃ³n acumulada de construcciones	t	10184	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10195	2822	AmortizaciÃ³n acumulada de instalaciones tÃ©cnicas	t	10184	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10196	2823	AmortizaciÃ³n acumulada de maquinaria	t	10184	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10197	2824	AmortizaciÃ³n acumulada de utillaje	t	10184	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10198	2825	AmortizaciÃ³n acumulada de otras instalaciones	t	10184	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10199	2826	AmortizaciÃ³n acumulada de mobiliario	t	10184	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10200	2827	AmortizaciÃ³n acumulada de equipos para proceso de informaciÃ³n	t	10184	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10201	2829	AmortizaciÃ³n acumulada de otro inmovilizado material	t	10184	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10202	292	ProvisiÃ³n por depreciaciÃ³n del inmovilizado material	t	10020	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10203	291	ProvisiÃ³n por depreciacion del inmovilizado inmaterial	t	10020	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10204	293	ProvisiÃ³n por depreciacion de valores de renta fija a largo plazo de empresas del grupo	t	10020	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10205	294	ProvisiÃ³n por depreciacion de valores de renta fija a largo plazo de empresas asociadas	t	10020	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10206	297	ProvisiÃ³n para insolvencias de valores negociables a largo plazo	t	10020	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10207	296	ProvisiÃ³n para insolvencias de crÃ©ditos a largo plazo a empresas asociadas	t	10020	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10208	295	ProvisiÃ³n para insolvencias de crÃ©ditos a largo plazo a empresas del grupo	t	10020	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10209	298	ProvisiÃ³n para insolvencias de crÃ©ditos a largo plazo	t	10020	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10210	2935	ProvisiÃ³n por depreciaciÃ³n de valores negociables a largo plazo en empresas del grupo	t	10204	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10211	2946	ProvisiÃ³n por depreciaciÃ³n de valores negociables a largo plazo en empresas asociadas	t	10205	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10212	2941	ProvisiÃ³n por depreciaciÃ³n de participaciones en capital a largo plazo en empresas asociadas	t	10205	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10213	301	MercaderÃ­as B	t	10021	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10214	300	Mercaderias A	t	10021	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10215	311	Materias primas B	t	10022	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10216	310	Materias primas A	t	10022	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10217	325	Materiales diversos	t	10023	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10218	327	Envases	t	10023	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10219	326	Embalajes	t	10023	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10220	328	Material de oficina	t	10023	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10221	320	Elementos y conjuntos incorporables	t	10023	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10222	321	Combustibles	t	10023	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10223	322	Repuestos	t	10023	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10224	331	Productos en curso B	t	10024	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10225	330	Productos en curso A	t	10024	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10226	341	Productos semiterminados B	t	10025	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10227	340	Productos semiterminados A	t	10025	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10228	351	Productos terminados B	t	10026	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10229	350	Productos terminados A	t	10026	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10230	368	Materiales recuperados A	t	10027	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10231	366	Residuos B	t	10027	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10232	365	Residuos A	t	10027	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10233	361	Subproductos B	t	10027	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10234	360	Subproductos A	t	10027	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10235	369	Materiales recuperados B	t	10027	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10236	392	ProvisiÃ³n por depreciaciÃ³n de otros aprovisionamientos	t	10028	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10237	393	ProvisiÃ³n por depreciaciÃ³n de productos en curso	t	10028	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10238	391	ProvisiÃ³n por depreciaciÃ³n de materias primas	t	10028	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10239	394	ProvisiÃ³n por depreciaciÃ³n de productos semiterminados	t	10028	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10240	395	ProvisiÃ³n por depreciaciÃ³n de productos terminados	t	10028	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10241	396	ProvisiÃ³n por depreciaciÃ³n de subproductos, residuos y materiales recuperados	t	10028	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10242	390	ProvisiÃ³n por depreciaciÃ³n de mercaderias	t	10028	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10244	401	Proveedores, efectos comerciales a pagar	t	10029	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10245	406	Embases y embalajes a devolver a proveedores	t	10029	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10246	403	Proveedores, empresas asociadas	t	10029	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10247	402	Proveedores, empresas del grupo	t	10029	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10248	407	Anticipos a proveedores	t	10029	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10250	4009	Proveedores, facturas pendientes de recibir o de formalizar	t	10243	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10251	4004	Proveedores (moneda extranjera)	t	10243	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10253	4020	Proveedores, empresas del grupo	t	10247	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10254	4026	Envases y embalajes a devolver a proveedores, empresas del grupo	t	10247	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10255	4024	Proveedores, empresas del grupo (moneda extranjera)	t	10247	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10256	4029	Proveedores, empresas del grupo, facturas pendientes de recibir o de formalizar	t	10247	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10257	4021	Efectos comerciales a pagar, empresas del grupo	t	10247	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10258	410	Acreedores por prestaciones de servicios	t	10030	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10259	411	Acreedores, efectos comerciales a pagar	t	10030	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10260	419	Acreedores por operaciones en comun	t	10030	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10261	4104	Acreedores por prestaciones de servicios (moneda extranjera)	t	10258	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10262	4100	Acreedores por prestaciones de servicios	t	10258	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10263	4109	Acreedores por prestaciones de servicios, facturas pendientes de recibir o de formalizar	t	10258	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10264	437	Anticipos de clientes	t	10031	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10265	436	Envases y embalajes a devolver por clientes	t	10031	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10266	433	Clientes, empresas asociadas	t	10031	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10267	432	Clientes, empresas del grupo	t	10031	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10268	431	Clientes, efectos comerciales a pagar	t	10031	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10269	435	Clientes de dudoso cobro	t	10031	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10272	4304	Clientes (Moneda extranjera)	t	10270	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10273	4309	Clientes, facturas pendientes de formalizar	t	10270	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10274	4312	Efectos comerciales en gestiÃ³n de cobro	t	10268	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10275	4315	Efectos comerciales impagados	t	10268	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10276	4311	Efectos comerciales descontados	t	10268	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10277	4310	Efectos comerciales en cartera	t	10268	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10278	4324	Clientes, empresas del grupo (moneda extranjera)	t	10267	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10279	4329	Clientes, empresas del grupo, facturas	t	10267	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10280	4326	Envases y embalajes a devolver a clientes, empresas del grupo	t	10267	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10281	4321	Efectos comerciales a cobrar, empresas del grupo	t	10267	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10282	4320	Clientes, empresas del grupo	t	10267	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10283	441	Deudores, efectos comerciales a cobrar	t	10032	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10284	440	Deudores	t	10032	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10285	445	Deudores de dudoso cobro	t	10032	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10286	449	Deudores por operaciones en comÃºn	t	10032	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10287	4404	Deudores (moneda extranjera)	t	10284	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10288	4409	Deudores, facturas pendientes de formalizar	t	10284	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10289	4400	Deudores	t	10284	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10290	4415	Deudores, efectos comerciales impagados	t	10283	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10291	4412	Deudores, efectos comerciales en gestiÃ³n de cobro	t	10283	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10292	4410	Deudores, efectos comerciales en cartera	t	10283	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10293	465	Remuneraciones pendientes de pago	t	10033	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10294	460	Anticipos de remuneraciones	t	10033	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10295	474	Impuesto sobre beneficios anticipado y compensaciÃ³n de pÃ©rdidas	t	10034	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10296	471	Organismos de la Seguridad Social, deudores	t	10034	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10297	476	Organismos de la Seguridad Social, acreedores	t	10034	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10299	475	Hacienda PÃºblica, acreedor por conceptos fiscales	t	10034	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10300	479	Impuesto sobre beneficios diferido	t	10034	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10301	470	Hacienda PÃºblica, deudor por diversos conceptos	t	10034	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10303	473	Hacienda PÃºblica, retenciones y pagos a cuenta	t	10034	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10304	4708	Hacienda PÃºblica, deudor por subvenciones conseguidas	t	10301	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10305	4700	Hacienda PÃºblica, deudor por IVA	t	10301	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10306	4709	Hacienda PÃºblica, deudor por devoluciÃ³n de impuestos	t	10301	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10307	47200004	IVA SOPORTADO 4% COMPRAS	t	10302	f	\N	0.00	0.00	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10309	47200007	IVA SOPORTADO 7% COMPRAS	t	10302	f	\N	0.00	0.00	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10310	4746	CrÃ©dito por perdidas a compensar del ejercicio	t	10295	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10311	4740	Impuesto sobre beneficios anticipado	t	10295	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10312	4752	Hacienda PÃºblica, acreedor por impuestos sobre sociedades	t	10299	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10313	4751	Hacienda PÃºblica, acreedor por retenciones practicadas	t	10299	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10314	4758	Hacienda PÃºblica, acreedor por subvenciones a reintegrar	t	10299	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10315	47500000	Hacienda Publica, acreedor por IVA	t	10299	f	\N	0.00	0.00	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10316	47700007	IVA 7% VENTAS	t	10298	f	\N	0.00	0.00	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10317	47700004	IVA 4% VENTAS	t	10298	f	\N	0.00	0.00	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10319	485	Ingresos anticipados	t	10035	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10321	490	ProvisiÃ³n para insolvencias de trÃ¡fico	t	10036	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10322	499	ProvisiÃ³n para otras insolvencias de trÃ¡fico	t	10036	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10323	494	ProvisiÃ³n para insolvencias de trÃ¡fico de empresas asociadas	t	10036	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10324	491	ProvisiÃ³n para insolvencias de trÃ¡fico de empresas del grupo	t	10036	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10325	501	Obligaciones y bonos convertibles a corto plazo	t	10037	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10326	505	Deudas representadas en otros valores negociables a corto plazo	t	10037	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10327	509	Valores negociables amortizados	t	10037	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10328	506	Intereses de emprÃ©sitos y otras emisiones anÃ¡logas	t	10037	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10329	500	Obligaciones y bonos a corto plazo	t	10037	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10330	5091	Obligaciones y bonos convertibles amortizados	t	10327	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10331	5090	Obligaciones y bonos amortizados	t	10327	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10332	5095	Otros valores negociables amortizados	t	10327	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10333	512	Deudas a corto plazo con entidades de crÃ©dito del grupo	t	10038	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10334	516	Intereses a corto plazo de deudas con empresas del grupo	t	10038	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10335	515	Proveedores de inmovilizado a corto plazo, empresas asociadas	t	10038	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10336	514	Proveedores de inmovilizado a corto plazo, empresas del grupo	t	10038	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10337	513	Deudas a corto plazo con entidades de crÃ©dito asociadas	t	10038	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10338	511	Deudas a corto plazo con empresas asociadas	t	10038	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10339	510	Deudas a corto plazo con empresas del grupo	t	10038	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10340	517	Intereses a corto plazo de deudas con empresas asociadas	t	10038	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10341	5100	PrÃ©stamos a corto plazo de empresas del grupo	t	10339	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10342	5109	Otras deudas a corto plazo con empresas del grupo	t	10339	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10343	5128	Deudas por efectos descontados en entidades de crÃ©dito del grupo	t	10333	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10344	5120	PrÃ©stamos a corto plazo de entidades de crÃ©dito del grupo	t	10333	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10345	5129	Otras deudas a corto plazo con entidades de crÃ©dito del grupo	t	10333	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10346	525	Dividendo activo a pagar	t	10039	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10347	524	Efectos a pagar a corto plazo	t	10039	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10348	523	Proveedores de inmovilizado a corto plazo	t	10039	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10349	521	Deudas a corto plazo	t	10039	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10350	520	Deudas a corto plazo con entidades de crÃ©dito	t	10039	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10351	526	Intereses a corto plazo de deudas con entidades de crÃ©dito	t	10039	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10352	527	Intereses a corto plazo de deudas	t	10039	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10354	5208	Deudas por efectos descontados	t	10350	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10353	5201	Deudas a corto plazo por credito dispuesto	t	10350	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10355	532	Valores de renta fija a corto plazo de empresas del grupo	t	10040	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10356	533	Valores de renta fija a corto plazo de empresas asociadas	t	10040	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10357	535	CrÃ©ditos a corto plazo a empresas asociadas	t	10040	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10358	536	Intereses a corto plazo de inversiones financieras en empresas del grupo	t	10040	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10359	537	Intereses a corto plazo de inversiones financieras en empresas asociadas	t	10040	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10360	538	Desembolsos pendientes sobre acciones a corto plazo de empresas del grupo	t	10040	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10361	539	Desembolsos sobre acciones a corto plazo de empresas asociadas	t	10040	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10362	534	CrÃ©ditos a corto plazo a empresas del grupo	t	10040	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10363	530	Participaciones a corto plazo en empresas del grupo	t	10040	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10364	531	Participaciones a corto plazo en empresas asociadas	t	10040	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10365	5361	Intereses a corto plazo de creditos a empresas del grupo	t	10358	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10366	5360	Intereses a corto plazo de valores de renta fija de empresas del grupo	t	10358	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10367	548	Imposiciones a corto plazo	t	10041	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10368	540	Inversiones financieras temporales en capital	t	10041	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10369	542	CrÃ©ditos a corto plazo	t	10041	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10370	541	Valores de renta fija a corto plazo	t	10041	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10371	549	Desembolsos pendientes sobre acciones a corto plazo	t	10041	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10372	546	Intereses a corto plazo de valores de renta fija	t	10041	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10373	547	Intereses a corto plazo de crÃ©ditos	t	10041	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10374	543	CrÃ©ditos a corto plazo por enajenacion del mobiliario	t	10041	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10375	544	CrÃ©ditos a corto plazo al personal	t	10041	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10376	545	Dividendo a cobrar	t	10041	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10377	5409	Otras inversiones financieras temporales en capital	t	10368	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10378	5401	Inversiones financieras temporales en acciones sin cotizaciÃ³n en un mercado secundario	t	10368	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10379	5400	Inversiones financieras temporales en acciones con cotizaciÃ³n en un mercado secundario	t	10368	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10380	556	Desembolsos exigidos sobre acciones	t	10042	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10381	555	Partidas pendientes de aplicaciÃ³n	t	10042	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10382	557	Dividendo activo a pagar	t	10042	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10383	558	Accionista por desembolsos exigidos	t	10042	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10384	551	Cuenta corriente con empresas del grupo	t	10042	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10385	552	Cuenta corriente con empresas asociadas	t	10042	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10386	550	Titular de explotaciÃ³n	t	10042	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	3	\N	\N	\N
10387	553	Cuenta corriente con socios y administradores	t	10042	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10388	5561	Desembolsos exigidos sobre acciones de empresas asociadas	t	10380	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10389	5560	Desembolsos exigidos sobre acciones de empresas del grupo	t	10380	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10390	5562	Desembolsos exigidos sobre acciones de otras empresas	t	10380	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10391	561	DepÃ³sitos recibidos a corto plazo	t	10043	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10392	560	Fianzas recibidas a corto plazo	t	10043	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10393	565	Fianzas constituidas a corto plazo	t	10043	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10394	566	DepÃ³stios constituidos a corto plazo	t	10393	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10395	574	Bancos e instituciones de crÃ©dito cuentas de ahorro	t	10044	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10397	575	Bancos e instituciones de crÃ©dito, cuentas de ahorro, moneda extranjera	t	10044	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10399	57300001	SA NOSTRA	t	10396	f	\N	0.00	0.00	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	4	\N	\N	\N
10400	580	Intereses pagados por anticipado	t	10045	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10401	585	Intereses cobrados por anticipado	t	10045	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10402	593	ProvisiÃ³n por depreciaciÃ³n de valores negociables a corto plazo de empresas del grupo	t	10046	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10403	597	ProvisiÃ³n por depreciaciÃ³n de valores negociables a corto plazo	t	10046	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10404	598	ProvisiÃ³n para insolvencias de crÃ©ditos a corto plazo	t	10046	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10405	596	ProvisiÃ³n para insolvencias de crÃ©ditos a corto plazo a empresas asociadas	t	10046	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10406	595	ProvisiÃ³n para insolvencias de crÃ©ditos a corto plazo a empresas del grupo	t	10046	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10407	594	ProvisiÃ³n por depreciaciÃ³n de valores negociables a corto plazo de empresas asociadas	t	10046	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10408	602	Compras de otros aprovisionamientos	t	10047	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10409	608	Devoluciones de compras y operaciones similares	t	10047	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10410	607	Trabajos realizados por otras empresas	t	10047	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10411	601	Compras de materias primas	t	10047	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10412	609	Rappels por compras	t	10047	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10413	6082	Devoluciones de compras de otros aprovisionamientos	t	10047	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10415	6092	Rappels por compras de otros aprovisionamientos	t	10047	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10416	6091	Rappels por compras de materias primas	t	10047	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10417	6090	Rappels por compras de mercaderÃ­as	t	10047	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10418	6081	Devoluciones de compras de materias primas	t	10409	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10419	6080	Devoluciones de compras de mercaderÃ­as	t	10409	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10420	610	VariaciÃ³n de existencias de mercaderias	t	10048	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10421	611	VariaciÃ³n de existencias de materias primas	t	10048	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10422	612	VariaciÃ³n de existencias de otros aprovisionamientos	t	10048	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10423	62600000	Servicios bancarios y similares	t	10049	f	\N	0.00	0.00	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10424	621	Arrendamiento y canones	t	10049	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10425	624	Transportes	t	10049	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10426	628	Suministros	t	10049	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	5	\N	\N	\N
10427	620	Gastos en investigaciÃ³n y desarrollo del ejercicio	t	10049	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10428	623	Servicios de profesionales independientes	t	10049	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10429	625	Primas de seguros	t	10049	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10430	62700000	Publicidad, propaganda y relaciones publicas	t	10049	f	\N	0.00	0.00	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10431	622	Reparaciones y conservaciones	t	10049	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10432	629	Otros servicios	t	10049	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	5	\N	\N	\N
10433	62800005	Telefonica tef	t	10426	f	\N	0.00	0.00	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	5	\N	\N	\N
10434	62800004	SUMINISTRO ADSL	t	10426	f	\N	0.00	0.00	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	5	\N	\N	\N
10435	62960000	MATERIAL DE OFICINA	t	10432	f	\N	0.00	0.00	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	5	\N	\N	\N
10436	62920001	SERVICIOS DE ASESORIA	t	10432	f	\N	0.00	0.00	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	5	\N	\N	\N
10437	631	Otros tributos	t	10050	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10438	633	Ajustes negativos en la imposiciÃ³n sobre beneficios	t	10050	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10439	636	DevoluciÃ³n de impuestos	t	10050	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10440	634	Ajustes negativos en la imposiciÃ³n indirecta	t	10050	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10441	638	Ajustes positivos en la imposiciÃ³n sobre beneficios	t	10050	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10442	639	Ajustes positivos en la imposiciÃ³n indirecta	t	10050	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10443	630	Impuestos sobre beneficios	t	10050	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10444	6342	Ajustes negativos en IVA de inversiones	t	10440	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10445	6341	Ajustes negativos en IVA de circulante	t	10440	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10446	6391	Ajustes positivos en IVA de circulante	t	10442	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10447	6392	Ajustes positivos en IVA de inversiones	t	10442	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10448	643	Aportaciones a sistemas complementarios de pensiones	t	10051	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10449	649	Otros gastos sociales	t	10051	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10450	640	Sueldos	t	10051	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10451	641	Indemnizaciones	t	10051	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10452	642	Seguridad Social a cargo de la empresa	t	10051	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10453	650	PÃ©rdidas de crÃ©ditos comerciales incobrables	t	10052	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10454	659	Otras pÃ©rdidas en gestiÃ³n corriente	t	10052	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10455	651	Resultados de operaciones en comÃºn	t	10052	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10456	6511	PÃ©rdida soportada (participe o asociado no gestor)	t	10455	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10457	6510	Beneficio transferido (Gestor)	t	10455	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10458	664	Intereses por descuento de efectos	t	10053	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10459	667	PÃ©rdidas de crÃ©ditos	t	10053	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10460	668	Diferencias negativas de cambio	t	10053	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10461	661	Intereses de obligaciones y bonos	t	10053	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10462	665	Descuento sobre ventas por pronto pago	t	10053	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10463	666	PÃ©rdidas procedentes de valores negociables	t	10053	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10464	669	Otros gastos financieros	t	10053	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10465	662	Intereses de deudas a largo plazo	t	10053	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10466	663	Intereses de deudas a corto plazo	t	10053	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10467	6618	Intereses de obligaciones y bonos a corto plazo en otras empresas	t	10461	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10468	6615	Intereses de obligaciones y bonos a corto plazo en empresas del grupo	t	10461	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10469	6613	Intereses de obligaciones y bonos a largo plazo en otras empresas	t	10461	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10470	6610	Intereses de obligaciones y bonos a largo plazo en empresas del grupo	t	10461	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10471	6611	Intereses de obligaciones y bonos a largo plazo en empresas asociadas	t	10461	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10472	6616	Intereses de obligaciones y bonos a corto plazo en empresas asociadas	t	10461	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10473	6620	Intereses de deudas a largo plazo con empresas del grupo	t	10465	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10474	6621	Intereses de deudas a largo plazo con empresas asociadas	t	10465	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10475	6622	Intereses de deudas a largo plazo con entidades de crÃ©dito	t	10465	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10476	6623	Intereses de deudas a largo plazo con otras empresas	t	10465	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10477	6631	Intereses de deudas a corto plazo con empresas asociadas	t	10466	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10478	6630	Intereses de deudas a corto plazo con empresas del grupo	t	10466	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10479	6632	Intereses de deudas a corto plazo con entidades de crÃ©dito	t	10466	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10480	6633	Intereses de deudas a corto plazo con otras empresas	t	10466	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10481	6643	Intereses por descuento de efectos en otras entidades de crÃ©dito	t	10458	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10482	6640	Intereses por descuento de efectos en entidades de crÃ©dito del grupo	t	10458	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10483	6641	Intereses por descuento de efectos en entidades de crÃ©dito asociadas	t	10458	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10484	6650	Descuento sobre ventas por pronto pago a empresas del grupo	t	10462	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10485	6653	Descuento sobre ventas por pronto pago a otras empresas	t	10462	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10486	6651	Descuento sobre ventas por pronto pago a empresas asociadas	t	10462	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10487	6660	PÃ©rdidas en valores negociables a largo plazo de empresas del grupo	t	10463	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10488	6668	PÃ©rdidas en valores negociables a corto plazo de otras empresas	t	10463	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10489	6665	PÃ©rdidas en valores negociables a corto plazo de empresas del grupo	t	10463	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10490	6666	PÃ©rdidas en valores negociables a corto plazo de empresas asociadas	t	10463	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10491	6663	PÃ©rdidas en valores negociables a largo plazo de otras empresas	t	10463	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10492	6661	PÃ©rdidas en valores negociables a largo plazo de empresas asociadas	t	10463	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10493	6678	PÃ©rdidas de crÃ©ditos a corto plazo a otras empresas	t	10459	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10494	6670	PÃ©rdidas de crÃ©ditos a largo plazo a empresas del grupo	t	10459	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10495	6671	PÃ©rdidas de crÃ©ditos a largo plazo a empresas asociadas	t	10459	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10496	6673	PÃ©rdidas de crÃ©ditos a largo plazo a otras empresas	t	10459	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10497	6675	PÃ©rdidas de crÃ©ditos a corto plazo a empresas del grupo	t	10459	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10498	6676	PÃ©rdidas de crÃ©ditos a corto plazo a empresas asociadas	t	10459	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10499	679	Gastos y perdidas de ejercicios anteriores	t	10054	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10500	672	PÃ©rdidas procedentes de participaciones en capital a largo plazo en empresas del grupo	t	10054	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10501	670	PÃ©rdidas procedentes del inmovilizado inmaterial	t	10054	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10502	674	PÃ©rdidas por operaciones con acciones y obligaciones propias	t	10054	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10503	673	PÃ©rdidas procedentes de participaciones en capital a largo plazo en empresas asociadas	t	10054	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10504	671	PÃ©rdidas procedentes del inmovilizado material	t	10054	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10505	678	Gastos extraordinarios	t	10054	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10506	681	AmortizaciÃ³n del inmovilizado inmaterial	t	10055	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10507	682	AmortizaciÃ³n del inmovilizado material	t	10055	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10508	680	AmortizaciÃ³n de gastos de establecimiento	t	10055	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10509	691	DotaciÃ³n a la provisiÃ³n del inmovilizado inmaterial	t	10056	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10510	693	Dotacion a la provisiÃ³n de existencias	t	10056	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10511	694	DotaciÃ³n a la provisiÃ³n para insolvencias de trÃ¡fico	t	10056	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10512	698	DotaciÃ³n a la provisiÃ³n para valores negociables a corto plazo	t	10056	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10513	699	DotaciÃ³n a la provisiÃ³n para insolvencias de crÃ©ditos a corto plazo	t	10056	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10514	697	DotaciÃ³n a la provisiÃ³n para insolvencias de crÃ©ditos a largo plazo	t	10056	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10515	696	DotaciÃ³n a la provisiÃ³n para valores negociables a largo plazo	t	10056	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10516	695	DotaciÃ³n a la provisiÃ³n para otras operaciones de trafico	t	10056	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10517	692	DotaciÃ³n a la provisiÃ³n del inmovilizado material	t	10056	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10518	690	DotaciÃ³n al fondo de reversion	t	10056	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10519	6965	DotaciÃ³n a la provisiÃ³n para valores de renta fija a largo plazo en empresas del grupo	t	10515	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10520	6966	DotaciÃ³n a la provisiÃ³n para valores de renta fija a largo plazo en empresas asociadas	t	10515	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10521	6963	DotaciÃ³n a la provisiÃ³n para valores negociables a largo plazo en otras empresas	t	10515	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10522	6960	DotaciÃ³n a la provisiÃ³n para participaciones en capital a largo plazo en empresas del grupo	t	10515	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10523	6961	DotaciÃ³n a la provisiÃ³n para participaciones en capital a largo plazo en empresas asociadas	t	10515	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10524	6970	DotaciÃ³n a la provisiÃ³n para insolvencias de crÃ©ditos a largo plazo a empresas del grupo	t	10514	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10525	6973	DotaciÃ³n a la provisiÃ³n para insolvencias de crÃ©ditos a largo plazo a otras empresas	t	10514	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10526	6971	DotaciÃ³n a la provisiÃ³n para insolvencias de crÃ©ditos a largo plazo a empresas asociadas	t	10514	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10527	6980	DotaciÃ³n a la provisiÃ³n para valores negociables a corto plazo de empresas del grupo	t	10512	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10528	6981	DotaciÃ³n a la provisiÃ³n para valores negociables a corto plazo de empresas asociadas	t	10512	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10529	6983	DotaciÃ³n a la provisiÃ³n para valores negociables a corto plazo de otras empresas	t	10512	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10530	6993	DotaciÃ³n a la provisiÃ³n para insolvencias de crÃ©ditos a corto plazo a otras empresas	t	10513	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10531	6991	DotaciÃ³n a la provisiÃ³n para insolvencias de crÃ©ditos a corto plazo a empresas asociadas	t	10513	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10532	6990	DotaciÃ³n a la provisiÃ³n para insolvencias de crÃ©ditos a corto plazo a empresas del grupo	t	10513	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10533	709	Rappels sobre ventas	t	10057	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10535	703	Ventas de subproductos y residuos	t	10057	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10536	702	Ventas de productos semiterminados	t	10057	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10537	701	Ventas de productos terminados	t	10057	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10538	708	Devoluciones de ventas y operaciones similares	t	10057	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10539	705	Prestaciones de servicios	t	10057	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10540	704	Ventas de embases y embalajes	t	10057	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10541	7081	Devoluciones de ventas de productos terminados	t	10538	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10542	7080	Devoluciones de ventas de mercaderÃ­as	t	10538	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10543	7084	Devoluciones de ventas de envases y embalajes	t	10538	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10544	7083	Devoluciones de ventas de subproductos y residuos	t	10538	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10545	7082	Devoluciones de ventas de productos semiterminados	t	10538	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10546	7091	Rappels sobre ventas de productos terminados	t	10533	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10547	7090	Rappels sobre ventas de mercaderÃ­a	t	10533	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10548	7092	Rappels sobre ventas de productos semiterminados	t	10533	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10549	7093	Rappels sobre ventas de subproductos y residuos	t	10533	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10550	7094	Rappels sobre ventas de envases y embalajes	t	10533	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10551	710	VariaciÃ³n de existencias de productos en curso	t	10058	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10552	711	VariaciÃ³n de existencias de productos semiterminados	t	10058	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10553	712	VariaciÃ³n de existencias de productos terminados	t	10058	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10554	713	VariaciÃ³n de existencias de subproductos, residuos y materiales recuperados	t	10058	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10555	737	Incorporacion al activo de gastos de formalizacion de deudas	t	10059	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10556	731	Trabajos realizados para el inmovilizado inmaterial	t	10059	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10557	733	Trabajos realizados para el inmovilizado material en curso	t	10059	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10558	732	Trabajos realizados para el inmovilizado material	t	10059	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10559	730	IncorporaciÃ³n al activo de gastos de establecimiento	t	10059	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10560	741	Otras subvenciones a la explotaciÃ³n	t	10060	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10561	740	Subvenciones oficiales a la explotaciÃ³n	t	10060	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10562	755	Ingresos por servicios al personal	t	10061	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10563	752	Ingresos por arrendamientos	t	10061	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10564	753	Ingresos de propiedad industrial cedida en explotaciÃ³n	t	10061	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10565	754	Ingresos por comisiones	t	10061	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10566	751	Resultados de operaciones en comÃºn	t	10061	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10567	759	Ingresos por servicios diversos	t	10061	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10568	7511	Beneficio atribuido (participe o asociado no gestor)	t	10566	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10569	7510	PÃ©rdida transferida (gestor)	t	10566	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10570	760	Ingresos de participaciones en capital	t	10062	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10571	766	Beneficios en valores negociables	t	10062	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10572	768	Diferencias positivas de cambio	t	10062	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10573	769	Otros ingresos financieros	t	10062	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10574	765	Descuentos sobre compras por pronto pago	t	10062	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10575	762	Ingresos de crÃ©ditos a largo plazo	t	10062	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10576	763	Ingresos de crÃ©ditos a corto plazo	t	10062	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10577	761	Ingresos de valores de renta fija	t	10062	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10578	7603	Ingresos de participaciones en capital de otras empresas	t	10570	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10579	7600	Ingresos de participaciones en capital de empresas del grupo	t	10570	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10580	7601	Ingresos de participaciones en capital de empresas asociadas	t	10570	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10581	7613	Ingresos de crÃ©ditos de renta fija de otras empresas	t	10577	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10582	7611	Ingresos de crÃ©ditos de renta fija de empresas asociadas	t	10577	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10583	7610	Ingresos de crÃ©ditos de renta fija de empresas del grupo	t	10577	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10584	7623	Ingresos de crÃ©ditos a largo plazo a otras empresas	t	10575	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10585	7621	Ingresos de crÃ©ditos a largo plazo a empresas asociadas	t	10575	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10586	7620	Ingresos de crÃ©ditos a largo plazo a empresas del grupo	t	10575	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10587	7631	Ingresos de crÃ©ditos a corto plazo a empresas asociadas	t	10576	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10588	7630	Ingresos de crÃ©ditos a corto plazo a empresas del grupo	t	10576	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10589	7633	Ingresos de crÃ©ditos a corto plazo a otras empresas	t	10576	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10590	7650	Descuentos sobre compras por pronto pago de empresas del grupo	t	10574	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10591	7651	Descuentos sobre compras por pronto pago de empresas asociadas	t	10574	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10592	7653	Descuentos sobre compras por pronto pago de otras empresas	t	10574	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10593	7666	Beneficios en valores negociables a corto plazo de empresas asociadas	t	10571	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10594	7660	Beneficios en valores negociables a largo plazo de empresas del grupo	t	10571	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10595	7661	Beneficios en valores negociables a largo plazo de empresas asociadas	t	10571	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10596	7663	Beneficios en valores negociables a largo plazo de otras empresas	t	10571	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10597	7665	Beneficios en valores negociables a corto plazo de empresas del grupo	t	10571	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10598	7668	Beneficios en valores negociables a corto plazo de otras empresas	t	10571	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10599	770	Beneficios procedentes de inmovilizado inmaterial	t	10063	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10600	772	Beneficios procedentes de participaciones en capital a largo plazo en empresas del grupo	t	10063	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10601	779	Ingresos y beneficios de ejercicios anteriores	t	10063	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10602	771	Beneficios procedentes de inmovilizado material	t	10063	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10603	778	Ingresos extraordinarios	t	10063	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10604	775	Subvenciones de capital traspasadas al resultado del ejercicio 	t	10063	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10605	774	Beneficios por operaciones con acciones y obligaciones propias	t	10063	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10606	773	Beneficios procedentes de participaciones en capital a largo plazo en empresas asociadas	t	10063	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10607	793	ProvisiÃ³n de existencias aplicada	t	10064	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10608	797	Exceso de provisiÃ³n para insolvencias de creditos a largo plazo	t	10064	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10609	791	Exceso de provisiÃ³n del inmovilizado inmaterial	t	10064	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10610	792	Exceso de provisiÃ³n del inmovilizado material	t	10064	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10611	799	Exceso de provisiÃ³n para insolvencias de creditos a corto plazo	t	10064	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10612	796	Exceso de provisiÃ³n para valores negociables a largo plazo	t	10064	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10613	794	ProvisiÃ³n para insolvencias de trÃ¡fico aplicada	t	10064	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10614	798	Exceso de provisiÃ³n para valores negociables a corto plazo 	t	10064	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10615	795	Provision para otras operaciones de trÃ¡fico aplicada	t	10064	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10616	790	Exceso de provisiÃ³n para riesgos y gastos	t	10064	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10617	79400000	Provision para insolvencias de trafico aplicada.	t	10613	f	\N	0.00	0.00	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10618	7966	Exceso de provisiÃ³n para valores de renta fija a largo plazo de empresas asociadas	t	10612	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10619	7960	Exceso de provisiÃ³n para participaciones en capital a largo plazo en empresas del grupo	t	10612	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10620	7961	Exceso de provisiÃ³n para participaciones en capital a largo plazo en empresas asociadas	t	10612	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10621	7963	Exceso de provisiÃ³n para valores negociables a largo plazo en otras empresas	t	10612	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10622	7965	Exceso de provisiÃ³n para valores de renta fija a largo plazo de empresas del grupo	t	10612	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10623	7970	Exceso de provisiÃ³n para insolvencias de crÃ©ditos a largo plazo de empresas del grupo 	t	10608	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10624	7971	Exceso de provisiÃ³n para insolvencias de crÃ©ditos a largo plazo de empresas asociadas	t	10608	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10625	7973	Exceso de provisiÃ³n para insolvencias de crÃ©ditos a largo plazo de otras empresas	t	10608	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10626	7983	Exceso de provisiÃ³n para valores negociables a corto plazo de otras empresas	t	10614	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10627	7981	Exceso de provisiÃ³n para valores negociables a corto plazo de empresas asociadas	t	10614	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10628	7980	Exceso de provisiÃ³n para valores negociables a corto plazo de empresas del grupo 	t	10614	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10629	7993	Exceso de provisiÃ³n para insolvencias de crÃ©ditos a corto plazo de otras empresas	t	10611	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10630	7990	Exceso de provisiÃ³n para insolvencias de crÃ©ditos a corto plazo de empresas del grupo	t	10611	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10631	7991	Exceso de provisiÃ³n para insolvencias de crÃ©ditos a corto plazo de empresas asociadas	t	10611	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10788	57000006	Almacen estanco	t	10398	f	\N	0.00	0.00	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10805	43000010	Ajuntament de Pollensa	t	10271	f	\N	0.00	0.00	f	f	\N	\N	Ajuntament de Pollensa	130	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10796	43000001	Salvador Ribas Maura	t	10271	f	\N	0.00	0.00	f	f	\N	\N	G-RibasAzcona	116	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10797	43000002	Jorge Gutierrez	t	10271	f	\N	0.00	0.00	f	f	\N	\N	Jorge	1	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10068	100	Capital Social	t	10001	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	3	\N	\N	\N
10001	10	CAPITAL	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10816	43000021	Centre Profesional de Formacio Publica, s.l	t	10271	f	\N	0.00	0.00	f	f	\N	\N	Centre Profesional de Formacio Publica, s.l	100	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10794	40000000	Proveedores Varios	t	10249	f	\N	0.00	0.00	f	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10795	43000000	Clientes Varios	t	10271	f	\N	0.00	0.00	f	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10861	43000066	Línea Turística SL	t	10271	f	\N	0.00	0.00	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10862	43000067	Easy Sail S.L.	t	10271	f	\N	0.00	0.00	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10886	43000091	Logística Urbana Ambiental S.A.	t	10271	f	\N	0.00	0.00	f	f	\N	\N	Urbient	140	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10859	43000064	AUTOSERVICIO HOYGAN	t	10271	f	\N	0.00	0.00	f	f	\N	\N	AUTOSERVICIO HOYGAN	101	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10884	43000089	AIGUA CORRENT SL	t	10271	f	\N	0.00	0.00	f	f	\N	\N	AIGUA CORRENT SL	81	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10899	43000104	Daniel Rebollo	t	10271	f	\N	0.00	0.00	f	f	\N	\N	Colegio Luis Vives	105	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10904	43000109	Jose Luis Tejada	t	10271	f	\N	0.00	0.00	f	f	\N	\N	Jose Luis tejada Luna	137	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10905	43000110	Luis Fraile Romero	t	10271	f	\N	0.00	0.00	f	f	\N	\N	Luis Fraile Romero	139	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10871	43000076	Alimenblo S.L.	t	10271	f	\N	0.00	0.00	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10872	43000077	Corporación Ingemar, S.A.	t	10271	f	\N	0.00	0.00	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10873	43000078	Dolores Martínez	t	10271	f	\N	0.00	0.00	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10874	43000079	Formacion i Recursos Humans S.L.	t	10271	f	\N	0.00	0.00	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10875	43000080	Francisco Javier Gonzalez Domingo	t	10271	f	\N	0.00	0.00	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10876	43000081	Francisco Javier Matas Ferrer	t	10271	f	\N	0.00	0.00	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10893	43000098	ANTONI MATEU ESTRANY	t	10271	f	\N	0.00	0.00	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10894	43000099	COFRAME	t	10271	f	\N	0.00	0.00	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10898	43000103	Carlos Santana	t	10271	f	\N	0.00	0.00	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10906	43000111	CALZADOS BALLESTER S.A.	t	10271	f	\N	0.00	0.00	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10909	40000001	Cristina	t	10249	f	\N	0.00	0.00	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10910	40000002	Tomeu	t	10249	f	\N	0.00	0.00	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10911	40000003	Marco Aurelio	t	10249	f	\N	0.00	0.00	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10913	40000005	E.M.A.Y.A	t	10249	f	\N	0.00	0.00	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10919	40000011	GoDaddy	t	10249	f	\N	0.00	0.00	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10922	40000014	G.E.S.A.	t	10249	f	\N	0.00	0.00	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10925	40000017	MATEU BORRAS HOMAR	t	10249	f	\N	0.00	0.00	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10927	40000019	DAVID NAVARRO JIMENO	t	10249	f	\N	0.00	0.00	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10928	40000020	OFICENTER (ILLY)	t	10249	f	\N	0.00	0.00	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10929	40000021	SARUTEK	t	10249	f	\N	0.00	0.00	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10930	40000022	ALCAMPO SA	t	10249	f	\N	0.00	0.00	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10920	40000012	Infortisa S.L.	t	10249	f	\N	0.00	0.00	f	f	\N	\N	Infortisa	15	07004	Joaquin Turina, 1 Local 4	971 29 06 29	\N	\N	info@iglues.org	www.infortisa.com	1	\N	\N	\N
10921	40000013	Compañia Internacional de Ordenadores y Componentes Electronicos S.A.	t	10249	f	\N	0.00	0.00	f	f	\N	\N	CIOCE	10	07004	Joaquin Turina, 1 Local 4	971 29 06 29	\N	\N	info@iglues.org	www.cioce.es	1	\N	\N	\N
10924	40000016	GRISOFT LIMITED	t	10249	f	\N	0.00	0.00	f	f	\N	\N	GRISOFT LIMITED - AVG	16	07004	Joaquin Turina, 1 Local 4	971 29 06 29	\N	\N	info@iglues.org		1	\N	\N	\N
10877	43000082	Federación Balear de Tiro Olímpico	t	10271	f	\N	0.00	8628.39	f	f	\N	\N	Federación Balear de Tiro Olímpico	91	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10868	43000073	J.M.C. Representaciones Siglo XXI	t	10271	f	\N	0.00	640.32	f	f	\N	\N	J.M.C. Representaciones Siglo XXI 	41	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10879	43000084	Eugenio Fco. Navarro Leiva	t	10271	f	\N	0.00	465.75	f	f	\N	\N	Eugenio Fco. Navarro Leiva	28	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10830	43000035	CARPINTERIA DE ALUMINIO GORG BLAU 	t	10271	f	\N	0.00	1027.36	f	f	\N	\N	CARPINTERIA DE ALUMINIO GORG BLAU 	85	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10318	47700016	IVA 16% VENTAS	t	10298	f	\N	0.00	0.00	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10885	43000090	Ecologing	t	10271	f	\N	0.00	2896.39	f	f	\N	\N	Ecologing	97	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10867	43000072	Construcciones Metálicas PYLSA, S.L.	t	10271	f	\N	0.00	4927.42	f	f	\N	\N	Pedro y Lopez	23	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10887	43000092	Zaibatsu S.L.	t	10271	f	\N	0.00	640.32	f	f	\N	\N	Zaibatsu S.L.	135	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10878	43000083	AZUL YACHTS PALMA S.L.	t	10271	f	\N	0.00	2376.73	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10881	43000086	TecnoDraw	t	10271	f	\N	0.00	11043.91	f	f	\N	\N	TecnoDraw	26	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10804	43000009	IGLUES	t	10271	f	\N	0.00	21102.59	f	f	\N	\N	IGLUES	93	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10849	43000054	Marmolería Fullana	t	10271	f	\N	0.00	640.32	f	f	\N	\N	Marmolería Fullana	88	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10883	43000088	ESTOP. Topografia y Mediciones	t	10271	f	\N	0.00	5660.81	f	f	\N	\N	ESTOP. Topografia y Mediciones	59	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10824	43000029	Aceites Ripoll s.l.	t	10271	f	\N	0.00	0.00	f	f	\N	\N	Aceites Ripoll s.l.	67	07004	Joaquin Turina, 1 Local 4		\N		info@iglues.org		1	\N	\N	\N
10270	430	Clientes	t	10031	t	\N	0.00	178154.07	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10826	43000031	Gabriel Lluis Rechach Riera	t	10271	f	\N	0.00	0.00	f	f	\N	\N	Gabriel Lluis Rechach Riera	33	07004	Joaquin Turina, 1 Local 4		\N		info@iglues.org		1	\N	\N	\N
10865	43000070	P.C.I. Procesos Comerciales Insulares S.L.	t	10271	f	\N	0.00	640.32	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10827	43000032	Idus de Comunicación S.A	t	10271	f	\N	0.00	0.00	f	f	\N	\N	Idus de Comunicación S.A	38	07004	Joaquin Turina, 1 Local 4		\N		info@iglues.org		1	\N	\N	\N
10031	43	CLIENTES	t	\N	t	\N	0.00	178154.07	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10828	43000033	Raggi Damp S.L.	t	10271	f	\N	0.00	0.00	f	f	\N	\N	Raggi Damp S.L.	58	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10858	43000063	Borisgrafic S.L.	t	10271	f	\N	0.00	1760.88	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10829	43000034	Mancomunitat des Raiguer	t	10271	f	\N	0.00	0.00	f	f	\N	\N	Mancomunitat des Raiguer	45	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10889	43000094	Patronat Municipal de Turisme Pollença	t	10271	f	\N	0.00	1920.96	f	f	\N	\N	Patronat Municipal Turisme Pollença	132	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10831	43000036	PAMADI S.L.	t	10271	f	\N	0.00	0.00	f	f	\N	\N	PAMADI S.L.	51	07004	Joaquin Turina, 1 Local 4		\N		info@iglues.org		1	\N	\N	\N
10271	4300	Clientes	t	10270	t	\N	0.00	178154.07	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10832	43000037	QAMAR Palma, S.L.	t	10271	f	\N	0.00	0.00	f	f	\N	\N	QAMAR Palma, S.L.	55	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10057	70	VENTAS DE MERCADERIAS DE PRODUCCION PROPIA, DE SERVICIOS, ETC.	t	\N	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10833	43000038	José Marí Estopá Rey	t	10271	f	\N	0.00	0.00	f	f	\N	\N	José Marí Estopá Rey	42	07004	Joaquin Turina, 1 Local 4		\N		info@iglues.org		1	\N	\N	\N
10882	43000087	Leon Von Ondarza Fuster	t	10271	f	\N	0.00	160.08	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10806	43000011	Aiguas de Lluc, S.A.	t	10271	f	\N	0.00	27595.76	f	f	\N	\N	Aiguas de Lluc, S.A.	10	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10834	43000039	Pedro Molinera Ordoñez	t	10271	f	\N	0.00	0.00	f	f	\N	\N	Pedro Molinera Ordoñez	53	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10820	43000025	Gerencia Inmobiliaria Balear	t	10271	f	\N	0.00	16286.54	f	f	\N	\N	GIB	103	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10835	43000040	Jose Vallejo Gonzalez	t	10271	f	\N	0.00	0.00	f	f	\N	\N	Jose Vallejo Gonzalez	73	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10802	43000007	Universitat de les Illes	t	10271	f	\N	0.00	16741.43	f	f	\N	\N	UIB	108	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10836	43000041	TransUnion Mallorca	t	10271	f	\N	0.00	0.00	f	f	\N	\N	TransUnion Mallorca	66	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10866	43000071	Ibiza Sea Charter, S.L.	t	10271	f	\N	0.00	640.32	f	f	\N	\N	Ibiza Sea Charter, S.L.	99	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10837	43000042	Charter Boat S.L	t	10271	f	\N	0.00	0.00	f	f	\N	\N	Charter Boat S.L	21	07004	Joaquin Turina, 1 Local 4	971280394	\N		info@iglues.org		1	\N	\N	\N
10838	43000043	Juana Oliver Estilistas	t	10271	f	\N	0.00	0.00	f	f	\N	\N	Juana Oliver Estilistas	77	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10851	43000056	M Nieves Barber Pérez	t	10271	f	\N	0.00	480.24	f	f	\N	\N	M Nieves Barber Pérez	49	07004	Joaquin Turina, 1 Local 4	 \t971.29.06.29	\N		info@iglues.org		1	\N	\N	\N
10839	43000044	Apoteke Bar	t	10271	f	\N	0.00	0.00	f	f	\N	\N	Apoteke Bar	78	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10534	70000000	Ventas de mercaderias	t	10057	f	\N	0.00	0.00	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10800	43000005	Centro de Negocios Avenidas	t	10271	f	\N	0.00	0.00	f	f	\N	\N	Centro de Negocios Avenidas	20	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10840	43000045	SKY Helicopteros	t	10271	f	\N	0.00	0.00	f	f	\N	\N	SKY Helicopteros	61	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10298	477	Hacienda PÃºblica, IVA repercutido	t	10034	t	\N	0.00	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10864	43000069	Náutica Ripollsegar	t	10271	f	\N	0.00	960.48	f	f	\N	\N	Náutica Ripollsegar	98	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10841	43000046	Caladrogue	t	10271	f	\N	0.00	0.00	f	f	\N	\N	Caladrogue	17	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10801	43000006	CIAC	t	10271	f	\N	0.00	0.00	f	f	\N	\N	CIAC	22	07004	Joaquin Turina, 1 Local 4	 \t971.75.47.26	\N		info@iglues.org		1	\N	\N	\N
10803	43000008	La Agencia Q, S.L.	t	10271	f	\N	0.00	0.00	f	f	\N	\N	La Agencia Q, S.L.	43	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10842	43000047	Alex Fuentes - IB3	t	10271	f	\N	0.00	0.00	f	f	\N	\N	Alex Fuentes - IB3	83	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10870	43000075	Radical Boats - Escuela del Mar	t	10271	f	\N	0.00	4641.30	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10807	43000012	Viajes Majorica	t	10271	f	\N	0.00	7965.73	f	f	\N	\N	Viajes Majorica	8	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10809	43000014	Maria Angeles Prieto Mateo	t	10271	f	\N	0.00	0.00	f	f	\N	\N	Maria Angeles Prieto Mateo	46	07004	Joaquin Turina, 1 Local 4	 \t971.20.21.25	\N		info@iglues.org		1	\N	\N	\N
10034	47	ADMINISTRACIONES PUBLICAS	t	\N	t	\N	1851.85	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10918	40000010	Torrijas	t	10249	f	\N	1500.00	0.00	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10810	43000015	Isabel - Marina Estrella	t	10271	f	\N	0.00	0.00	f	f	\N	\N	Isabel - Marina Estrella	39	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10931	40000023	Redoble 74, SL	t	10249	f	\N	706.04	250.04	f	f	\N	\N	Redoble	33	07004	Joaquin Turina, 1 Local 4	971 29 06 29	\N	\N	info@iglues.org	www.redoble.net	1	\N	\N	\N
10932	40000024	IBERIA	t	10249	f	\N	0.00	54.59	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10811	43000016	SA BARCELLA DE SINEU RESTAURANTS HOTELS RURALS SL	t	10271	f	\N	0.00	0.00	f	f	\N	\N	SA BARCELLA DE SINEU RESTAURANTS HOTELS RURALS SL	76	07004	Joaquin Turina, 1 Local 4		\N		info@iglues.org		1	\N	\N	\N
10812	43000017	Hewlett-Packard International SARL	t	10271	f	\N	0.00	0.00	f	f	\N	\N	Hewlett-Packard International SARL	35	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10895	43000100	Miguel Serra Arquitectura S.L.U.	t	10271	f	\N	0.00	0.00	f	f	\N	\N	Miguel Serra Arquitectura S.L.U.	109	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10813	43000018	IBC PMI (Esther)	t	10271	f	\N	0.00	0.00	f	f	\N	\N	IBC PMI (Esther)	36	07004	Joaquin Turina, 1 Local 4		\N		info@iglues.org		1	\N	\N	\N
10934	40000026	Panda Software Spain,S.L.U	t	10249	f	\N	49.08	49.08	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10814	43000019	ICentre	t	10271	f	\N	0.00	0.00	f	f	\N	\N	ICentre	37	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10817	43000022	Mercurio Network S.L.	t	10271	f	\N	0.00	0.00	f	f	\N	\N	Mercurio Network S.L.	47	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10923	40000015	ONO	t	10249	f	\N	292.03	292.03	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10819	43000024	Radical Boats	t	10271	f	\N	0.00	0.00	f	f	\N	\N	Radical Boats	56	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10933	40000025	Daniel Company Oliver	t	10249	f	\N	606.00	606.00	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10914	40000006	Memory Set S.A.U	t	10249	f	\N	1180.47	1180.47	f	f	\N	\N	Memory	11	07004	Joaquin Turina, 1 Local 4	971 29 06 29	\N	\N	info@iglues.org	www.memoryset.com	1	\N	\N	\N
10821	43000026	SISTEMES I SEGURETAT COSTA BRAVA S.L.	t	10271	f	\N	0.00	0.00	f	f	\N	\N	SISTEMES I SEGURETAT COSTA BRAVA S.L.	60	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10825	43000030	Soluciones Tecnologicas 10DE10	t	10271	f	\N	0.00	0.00	f	f	\N	\N	Soluciones Tecnologicas 10DE10	62	07004	Joaquin Turina, 1 Local 4		\N		info@iglues.org		1	\N	\N	\N
10926	40000018	Sistemas Técnicos y Consulta, S.L.	t	10249	f	\N	92.94	92.94	f	f	\N	\N	STC	21	07004	Joaquin Turina, 1 Local 4	971 29 06 29	\N	\N	info@iglues.org	www.sistemasyrepro.com	1	\N	\N	\N
10799	43000004	Prevefoc Insular S.L.	t	10271	f	\N	0.00	0.00	f	f	\N	\N	Prevefoc Insular S.L.	75	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10912	40000004	Procesos Comerciales Insulares,S.L	t	10249	f	\N	7401.73	7254.32	f	f	\N	\N	P.C.I. Baleares	5	07004	Joaquin Turina, 1 Local 4	971 29 06 29	\N	\N	info@iglues.org	www.pci-baleares.com	1	\N	\N	\N
10935	40000027	Repro Copy Shop,S.L.	t	10249	f	\N	3957.44	3556.48	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10308	47200016	IVA SOPORTADO 16% COMPRAS	t	10302	f	\N	1851.85	0.00	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10302	472	Hacienda PÃºblica, IVA soportado	t	10034	t	\N	1851.85	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10249	4000	Proveedores	t	10243	t	\N	15785.73	13335.95	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10243	400	Proveedores	t	10029	t	\N	15785.73	13335.95	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	2	\N	\N	\N
10029	40	PROVEEDORES	t	\N	t	\N	15785.73	13335.95	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10414	60000000	Compras de mercaderias	t	10047	f	\N	11574.10	0.00	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10047	60	COMPRAS	t	\N	t	\N	11574.10	0.00	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10823	43000028	Transportes A. Bibiloni S.L.	t	10271	f	\N	0.00	0.00	f	f	\N	\N	Transportes A. Bibiloni S.L.	65	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10822	43000027	Alatielia Management	t	10271	f	\N	0.00	0.00	f	f	\N	\N	Alatielia Management	114	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10844	43000049	Bartolome Borras Riera	t	10271	f	\N	0.00	0.00	f	f	\N	\N	Bartolome Borras Riera	14	07004	Joaquin Turina, 1 Local 4	677774476	\N		info@iglues.org		1	\N	\N	\N
10850	43000055	Miguel Angel Abraham Oliver	t	10271	f	\N	0.00	0.00	f	f	\N	\N	Miguel Angel Abraham Oliver	87	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10853	43000058	Productos Ecológicos del Automóvil S.L.	t	10271	f	\N	0.00	0.00	f	f	\N	\N	Productos Ecológicos del Automóvil	92	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10854	43000059	Vinissimus S.L.	t	10271	f	\N	0.00	0.00	f	f	\N	\N	Vinisimus	107	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10902	43000107	Escola Turistica Balear	t	10271	f	\N	0.00	0.00	f	f	\N	\N	Escola Turistica Balear	124	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10860	43000065	Xinaris Construcción S.L.	t	10271	f	\N	0.00	0.00	f	f	\N	\N	Xinaris	119	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10891	43000096	Olah Cooking Group. S.L.	t	10271	f	\N	0.00	0.00	f	f	\N	\N	Laurus Restaurant	129	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10808	43000013	LLABRES FELIU	t	10271	f	\N	0.00	0.00	f	f	\N	\N	LLABRES FELIU	127	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10846	43000051	JUNCOSA. Muebles Cocinas y Alfombras	t	10271	f	\N	0.00	0.00	f	f	\N	\N	JUNCOSA. Muebles Cocinas y Alfombras	86	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10847	43000052	Gestion Plus Balear S.L.	t	10271	f	\N	0.00	0.00	f	f	\N	\N	Gestion Plus Balear S.L.	34	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10888	43000093	Click Fortun	t	10271	f	\N	0.00	0.00	f	f	\N	\N	Click Fortun	133	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10897	43000102	TOYTO 2003, S.L.	t	10271	f	\N	0.00	0.00	f	f	\N	\N	TOYTO 2003, S.L.	64	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10916	40000008	Leroy Merlin S.A.	t	10249	f	\N	0.00	0.00	f	f	\N	\N	Leroy Merlin	13	07004	Joaquin Turina, 1 Local 4	971 29 06 29	\N	\N	info@iglues.org		1	\N	\N	\N
10908	43000113	Anima Events S.L.	t	10271	f	\N	0.00	0.00	f	f	\N	\N	Anima Events S.L.	136	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10907	43000112	A. Aumaris S.L.	t	10271	f	\N	0.00	0.00	f	f	\N	\N	A. Aumaris S.L.	138	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10917	40000009	Distribuidora Rotger S.A.	t	10249	f	\N	0.00	0.00	f	f	\N	\N	Distribuidora Rotger S.A.	14	07004	Joaquin Turina, 1 Local 4	971 29 06 29	\N	\N	info@iglues.org		1	\N	\N	\N
10869	43000074	AFIBE Asesoria Integral de Empresas S.L.	t	10271	f	\N	0.00	0.00	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10880	43000085	Centro Balear Inmobiliario	t	10271	f	\N	0.00	0.00	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10915	40000007	Intplus,S.L	t	10249	f	\N	0.00	0.00	f	f	\N	\N	Intplus	12	07004	Joaquin Turina, 1 Local 4	971 29 06 29	\N	\N	info@iglues.org	www.superinventos.com	1	\N	\N	\N
10855	43000060	Margarita Indexiada	t	10271	f	\N	0.00	0.00	f	f	\N	\N	Margarita Indexiada	95	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10843	43000048	ADMAS BALEARES S.L.	t	10271	f	\N	0.00	0.00	f	f	\N	\N	Tiendas MasIntimo	84	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10848	43000053	SPAZZOLA, S.L.	t	10271	f	\N	0.00	0.00	f	f	\N	\N	SPAZZOLA, S.L.	63	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10900	43000105	DISGLOBAL GAMES S.L.L.	t	10271	f	\N	0.00	0.00	f	f	\N	\N	DISGLOBAL GAMES	126	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10939	57000007	Tijuana	t	10398	f	\N	0.00	0.00	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10852	43000057	Fungestur	t	10271	f	\N	0.00	0.00	f	f	\N	\N	Fungestur	32	07004	Joaquin Turina, 1 Local 4		\N		info@iglues.org		1	\N	\N	\N
10940	57000008	India	t	10398	f	\N	0.00	0.00	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10941	57000009	Magalli	t	10398	f	\N	0.00	0.00	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10896	43000101	Don Reloj S.A.	t	10271	f	\N	0.00	143.75	f	f	\N	\N	Don Reloj S.A.	120	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10942	57000010	Mogambi	t	10398	f	\N	0.00	0.00	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10815	43000020	ELECTROSAT S.L.	t	10271	f	\N	0.00	48.90	f	f	\N	\N	ELECTROSAT	123	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10903	43000108	SPORT INCA S.A.	t	10271	f	\N	0.00	874.87	f	f	\N	\N	SPORT INCA	122	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10863	43000068	Proyeccion y Sonido Proson	t	10271	f	\N	0.00	2241.12	f	f	\N	\N	Proyeccion y Sonido Proson	54	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10892	43000097	SEYMA IMPORTACIO S.L.	t	10271	f	\N	0.00	6349.84	f	f	\N	\N	SEYMA IMPORTACIO S.L.	128	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10901	43000106	Calzados Ballester. S.A.	t	10271	f	\N	0.00	156.40	f	f	\N	\N	Calzados Ballester. S.A.	125	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10890	43000095	APA Colegio Madre Alberta	t	10271	f	\N	0.00	2668.00	f	f	\N	\N	APA Colegio Madre Alberta	131	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10818	43000023	Cliente de Contado	t	10271	f	\N	0.00	186.81	f	f	\N	\N	----	40	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10845	43000050	Miquel Serra Arbol	t	10271	f	\N	0.00	6900.00	f	f	\N	\N	Miquel Serra Arbol	94	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10856	43000061	Comercial Video Games Electronic	t	10271	f	\N	0.00	4652.99	f	f	\N	\N	Comercial Video Games Electronic	115	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10938	57300007	Caja Bis	t	10396	f	\N	186.81	456.00	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10936	57300005	La Caixa	t	10396	f	\N	50236.66	2726.71	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10937	57300006	Caja	t	10396	f	\N	2411.96	0.00	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10857	43000062	Consulting 80	t	10271	f	\N	0.00	920.00	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10396	573	Bancos e instituciones de credito c/c vista, moneda extranjera	t	10044	t	\N	52835.43	3182.71	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10798	43000003	NautiSur	t	10271	f	\N	0.00	13167.04	f	f	\N	\N	NautiSur	106	07004	Joaquin Turina, 1 Local 4	902 88 11 66	\N		info@iglues.org	http://www.iglues.org	1	\N	\N	\N
10398	570	Caja	t	10044	t	\N	125318.64	12603.02	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
10044	57	TESORERÃA	t	\N	t	\N	178154.07	15785.73	t	t	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	0	\N	\N	\N
10943	57000011	Conetxia	t	10398	f	\N	125318.64	12603.02	f	f	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	\N	1	\N	\N	\N
\.


--
-- Data for Name: diario; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY diario (iddiario, descripcion) FROM stdin;
\.


--
-- Data for Name: ejercicios; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY ejercicios (ejercicio, periodo, bloqueado) FROM stdin;
2007	0	f
2007	1	f
2007	2	f
2007	3	f
2007	4	f
2007	5	f
2007	6	f
2007	7	f
2007	8	f
2007	9	f
2007	10	f
2007	11	f
2007	12	f
2008	0	f
2008	1	f
2008	2	f
2008	3	f
2008	4	f
2008	5	f
2008	6	f
2008	7	f
2008	8	f
2008	9	f
2008	10	f
2008	11	f
2008	12	f
2006	0	f
2006	1	f
2006	2	f
2006	3	f
2006	4	f
2006	5	f
2006	6	f
2006	7	f
2006	8	f
2006	9	f
2006	10	f
2006	11	f
2006	12	f
2000	0	f
2000	1	f
2000	2	f
2000	3	f
2000	4	f
2000	5	f
2000	6	f
2000	7	f
2000	8	f
2000	9	f
2000	10	f
2000	11	f
2000	12	f
2005	0	f
2005	1	f
2005	2	f
2005	3	f
2005	4	f
2005	5	f
2005	6	f
2005	7	f
2005	8	f
2005	9	f
2005	10	f
2005	11	f
2005	12	f
\.


--
-- Data for Name: fpago; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY fpago (idfpago, nomfpago, nplazosfpago, plazoprimerpagofpago, tipoplazoprimerpagofpago, plazoentrerecibofpago, tipoplazoentrerecibofpago) FROM stdin;
8	Nueva forma de pago	0	5	\N	0	\N
2	2 pagos - 30 y 60 dias	2	30	\N	20	\N
1	1 pago - 30 dias	1	30	\N	0	\N
3	3 pagos - 30 , 60 y 90 dias	3	30	\N	30	\N
\.


--
-- Data for Name: iva; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY iva (idiva, idtipoiva, idregistroiva, baseiva, ivaiva) FROM stdin;
\.


--
-- Data for Name: lgaspresupuestoc; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY lgaspresupuestoc (idlgaspresupuestoc, idpresupuestoc, ordenlgaspresupuestoc, idcuenta, cantlgaspresupuestoc, precioundlgaspresupuestoc, ejerciciolgaspresupuestoc, comentlgaspresupuestoc, desclgaspresupuestoc) FROM stdin;
\.


--
-- Data for Name: linamortizacion; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY linamortizacion (idlinamortizacion, idamortizacion, idasiento, ejercicio, fechaprevista, cantidad) FROM stdin;
3	1	\N	\N	2007-11-10	1200.00
4	1	\N	\N	2007-12-10	1200.00
5	1	\N	\N	2008-01-10	1200.00
2	1	\N	\N	2007-10-10	1200.00
1	1	\N	\N	2007-09-10	1200.00
\.


--
-- Data for Name: lingpresupuestoc; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY lingpresupuestoc (idlingpresupuestoc, idpresupuestoc, ordenlingpresupuestoc, idcuenta, cantlingpresupuestoc, precioundlingpresupuestoc, ejerciciolingpresupuestoc, comentlingpresupuestoc, desclingpresupuestoc) FROM stdin;
\.


--
-- Data for Name: mpatrimonial; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY mpatrimonial (idmpatrimonial, idbalance, descmpatrimonial, orden, tabulacion, saldo, opdesc, tipompatrimonial) FROM stdin;
\.


--
-- Data for Name: pais; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY pais (idpais, cod2pais, cod3pais, descpais) FROM stdin;
2	in	ind	indianapolis
1	es	esp	Espanya
\.


--
-- Data for Name: presupuestoc; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY presupuestoc (idpresupuestoc, idc_coste, fechapresupuestoc, nompresupuestoc, comentpresupuestoc, archpresupuestoc) FROM stdin;
\.


--
-- Data for Name: prevcobro; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY prevcobro (idprevcobro, fprevistaprevcobro, fcobroprevcobro, idctacliente, idfpago, idcuenta, idasiento, cantidadprevistaprevcobro, cantidadprevcobro, idregistroiva, tipoprevcobro, docprevcobro) FROM stdin;
\.


--
-- Data for Name: provincia; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY provincia (idprovincia, idpais, provincia) FROM stdin;
1	1	alava
2	1	albacete
3	1	navarra
12	1	asdf
\.


--
-- Data for Name: registroiva; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY registroiva (idregistroiva, contrapartida, baseimp, iva, ffactura, femisionregistroiva, factura, serieregistroiva, idborrador, incregistro, regularizacion, plan349, numorden, cif, idfpago, factemitida, rectificaaregistroiva) FROM stdin;
\.


--
-- Data for Name: tipoiva; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY tipoiva (idtipoiva, nombretipoiva, porcentajetipoiva, idcuenta) FROM stdin;
1	IVA 16 SOPORTADO	16.00	10308
2	IVA 16% REPERCUTIDO	16.00	10318
\.


--
-- Name: acumulado_c_coste_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY acumulado_c_coste
    ADD CONSTRAINT acumulado_c_coste_pkey PRIMARY KEY (idacumulado_c_coste);


--
-- Name: acumulado_canal_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY acumulado_canal
    ADD CONSTRAINT acumulado_canal_pkey PRIMARY KEY (idacumulado_canal);


--
-- Name: adocumental_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY adocumental
    ADD CONSTRAINT adocumental_pkey PRIMARY KEY (idadocumental);


--
-- Name: ainteligente_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY ainteligente
    ADD CONSTRAINT ainteligente_pkey PRIMARY KEY (idainteligente);


--
-- Name: apunte_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY apunte
    ADD CONSTRAINT apunte_pkey PRIMARY KEY (idapunte);


--
-- Name: asiento_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY asiento
    ADD CONSTRAINT asiento_pkey PRIMARY KEY (idasiento);


--
-- Name: balance_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY balance
    ADD CONSTRAINT balance_pkey PRIMARY KEY (idbalance);


--
-- Name: binteligente_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY binteligente
    ADD CONSTRAINT binteligente_pkey PRIMARY KEY (idbinteligente);


--
-- Name: borrador_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY borrador
    ADD CONSTRAINT borrador_pkey PRIMARY KEY (idborrador);


--
-- Name: c_coste_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY c_coste
    ADD CONSTRAINT c_coste_pkey PRIMARY KEY (idc_coste);


--
-- Name: c_costedist_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY c_costedist
    ADD CONSTRAINT c_costedist_pkey PRIMARY KEY (idc_costedist);


--
-- Name: canal_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY canal
    ADD CONSTRAINT canal_pkey PRIMARY KEY (idcanal);


--
-- Name: compbalance_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY compbalance
    ADD CONSTRAINT compbalance_pkey PRIMARY KEY (idcompbalance);


--
-- Name: compmasap_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY compmasap
    ADD CONSTRAINT compmasap_pkey PRIMARY KEY (idcompmasap);


--
-- Name: configuracion_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY configuracion
    ADD CONSTRAINT configuracion_pkey PRIMARY KEY (nombre);


--
-- Name: cuenta_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY cuenta
    ADD CONSTRAINT cuenta_pkey PRIMARY KEY (idcuenta);


--
-- Name: diario_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY diario
    ADD CONSTRAINT diario_pkey PRIMARY KEY (iddiario);


--
-- Name: fpago_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY fpago
    ADD CONSTRAINT fpago_pkey PRIMARY KEY (idfpago);


--
-- Name: iva_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY iva
    ADD CONSTRAINT iva_pkey PRIMARY KEY (idiva);


--
-- Name: lgaspresupuestoc_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY lgaspresupuestoc
    ADD CONSTRAINT lgaspresupuestoc_pkey PRIMARY KEY (idlgaspresupuestoc);


--
-- Name: lingpresupuestoc_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY lingpresupuestoc
    ADD CONSTRAINT lingpresupuestoc_pkey PRIMARY KEY (idlingpresupuestoc);


--
-- Name: mpatrimonial_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY mpatrimonial
    ADD CONSTRAINT mpatrimonial_pkey PRIMARY KEY (idmpatrimonial);


--
-- Name: pais_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY pais
    ADD CONSTRAINT pais_pkey PRIMARY KEY (idpais);


--
-- Name: presupuestoc_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY presupuestoc
    ADD CONSTRAINT presupuestoc_pkey PRIMARY KEY (idpresupuestoc);


--
-- Name: prevcobro_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY prevcobro
    ADD CONSTRAINT prevcobro_pkey PRIMARY KEY (idprevcobro);


--
-- Name: provincia_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY provincia
    ADD CONSTRAINT provincia_pkey PRIMARY KEY (idprovincia);


--
-- Name: registroiva_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY registroiva
    ADD CONSTRAINT registroiva_pkey PRIMARY KEY (idregistroiva);


--
-- Name: tipoiva_nombretipoiva_key; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY tipoiva
    ADD CONSTRAINT tipoiva_nombretipoiva_key UNIQUE (nombretipoiva);


--
-- Name: tipoiva_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY tipoiva
    ADD CONSTRAINT tipoiva_pkey PRIMARY KEY (idtipoiva);


--
-- Name: acumulados_canal_fk; Type: TRIGGER; Schema: public; Owner: postgres
--

CREATE TRIGGER acumulados_canal_fk
    AFTER UPDATE ON acumulado_canal
    FOR EACH ROW
    EXECUTE PROCEDURE acumulados_canal();


--
-- Name: borra_canal; Type: TRIGGER; Schema: public; Owner: postgres
--

CREATE TRIGGER borra_canal
    BEFORE DELETE ON canal
    FOR EACH ROW
    EXECUTE PROCEDURE borracanal();


--
-- Name: borra_ccoste; Type: TRIGGER; Schema: public; Owner: postgres
--

CREATE TRIGGER borra_ccoste
    BEFORE DELETE ON c_coste
    FOR EACH ROW
    EXECUTE PROCEDURE borraccoste();


--
-- Name: borra_cuenta; Type: TRIGGER; Schema: public; Owner: postgres
--

CREATE TRIGGER borra_cuenta
    BEFORE DELETE ON cuenta
    FOR EACH ROW
    EXECUTE PROCEDURE borracuenta();


--
-- Name: borratipoiva; Type: TRIGGER; Schema: public; Owner: postgres
--

CREATE TRIGGER borratipoiva
    BEFORE DELETE ON tipoiva
    FOR EACH ROW
    EXECUTE PROCEDURE deletetipoiva();


--
-- Name: civa; Type: TRIGGER; Schema: public; Owner: postgres
--

CREATE TRIGGER civa
    AFTER INSERT OR UPDATE ON iva
    FOR EACH ROW
    EXECUTE PROCEDURE cambiadoiva();


--
-- Name: civad; Type: TRIGGER; Schema: public; Owner: postgres
--

CREATE TRIGGER civad
    AFTER DELETE ON iva
    FOR EACH ROW
    EXECUTE PROCEDURE cambiadoivad();


--
-- Name: nueva_cuenta; Type: TRIGGER; Schema: public; Owner: postgres
--

CREATE TRIGGER nueva_cuenta
    AFTER INSERT ON cuenta
    FOR EACH ROW
    EXECUTE PROCEDURE creacuenta();


--
-- Name: nuevo_apunte; Type: TRIGGER; Schema: public; Owner: postgres
--

CREATE TRIGGER nuevo_apunte
    AFTER INSERT OR UPDATE ON apunte
    FOR EACH ROW
    EXECUTE PROCEDURE aumenta_valor();


--
-- Name: nuevo_apunte1; Type: TRIGGER; Schema: public; Owner: postgres
--

CREATE TRIGGER nuevo_apunte1
    AFTER UPDATE ON apunte
    FOR EACH ROW
    EXECUTE PROCEDURE disminuye_valor();


--
-- Name: nuevo_apunte2; Type: TRIGGER; Schema: public; Owner: postgres
--

CREATE TRIGGER nuevo_apunte2
    AFTER DELETE ON apunte
    FOR EACH ROW
    EXECUTE PROCEDURE disminuye_valor1();


--
-- Name: nuevo_canal; Type: TRIGGER; Schema: public; Owner: postgres
--

CREATE TRIGGER nuevo_canal
    AFTER INSERT ON canal
    FOR EACH ROW
    EXECUTE PROCEDURE creacanal();


--
-- Name: nuevo_ccoste; Type: TRIGGER; Schema: public; Owner: postgres
--

CREATE TRIGGER nuevo_ccoste
    AFTER INSERT ON c_coste
    FOR EACH ROW
    EXECUTE PROCEDURE creaccoste();


--
-- Name: nuevotipoiva; Type: TRIGGER; Schema: public; Owner: postgres
--

CREATE TRIGGER nuevotipoiva
    AFTER INSERT ON tipoiva
    FOR EACH ROW
    EXECUTE PROCEDURE inserttipoiva();


--
-- Name: propaga_acumulado_ccoste; Type: TRIGGER; Schema: public; Owner: postgres
--

CREATE TRIGGER propaga_acumulado_ccoste
    AFTER UPDATE ON c_coste
    FOR EACH ROW
    EXECUTE PROCEDURE propagaacumuladoccoste();


--
-- Name: propaga_acumulado_cuenta; Type: TRIGGER; Schema: public; Owner: postgres
--

CREATE TRIGGER propaga_acumulado_cuenta
    AFTER UPDATE ON cuenta
    FOR EACH ROW
    EXECUTE PROCEDURE propagaacumuladocuenta();


--
-- Name: propagaccosteborradortrigger; Type: TRIGGER; Schema: public; Owner: postgres
--

CREATE TRIGGER propagaccosteborradortrigger
    BEFORE INSERT OR UPDATE ON borrador
    FOR EACH ROW
    EXECUTE PROCEDURE propagaccosteborrador();


--
-- Name: restriccionesasientotrigger; Type: TRIGGER; Schema: public; Owner: postgres
--

CREATE TRIGGER restriccionesasientotrigger
    BEFORE INSERT OR UPDATE ON asiento
    FOR EACH ROW
    EXECUTE PROCEDURE restriccionesasiento();


--
-- Name: restriccionesborradoasientotrigger; Type: TRIGGER; Schema: public; Owner: postgres
--

CREATE TRIGGER restriccionesborradoasientotrigger
    BEFORE DELETE ON asiento
    FOR EACH ROW
    EXECUTE PROCEDURE tr_borradoasiento();


--
-- Name: restriccionesborradocuentatrigger; Type: TRIGGER; Schema: public; Owner: postgres
--

CREATE TRIGGER restriccionesborradocuentatrigger
    BEFORE DELETE ON cuenta
    FOR EACH ROW
    EXECUTE PROCEDURE restriccionesborradocuenta();


--
-- Name: restriccionesborradortrigger; Type: TRIGGER; Schema: public; Owner: postgres
--

CREATE TRIGGER restriccionesborradortrigger
    BEFORE INSERT OR UPDATE ON borrador
    FOR EACH ROW
    EXECUTE PROCEDURE restriccionesborrador();


--
-- Name: restriccionescuentatrigger; Type: TRIGGER; Schema: public; Owner: postgres
--

CREATE TRIGGER restriccionescuentatrigger
    BEFORE INSERT OR UPDATE ON cuenta
    FOR EACH ROW
    EXECUTE PROCEDURE restriccionescuenta();


--
-- Name: acumulado_c_coste_idc_coste_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY acumulado_c_coste
    ADD CONSTRAINT acumulado_c_coste_idc_coste_fkey FOREIGN KEY (idc_coste) REFERENCES c_coste(idc_coste);


--
-- Name: acumulado_c_coste_idcuenta_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY acumulado_c_coste
    ADD CONSTRAINT acumulado_c_coste_idcuenta_fkey FOREIGN KEY (idcuenta) REFERENCES cuenta(idcuenta);


--
-- Name: acumulado_canal_idcanal_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY acumulado_canal
    ADD CONSTRAINT acumulado_canal_idcanal_fkey FOREIGN KEY (idcanal) REFERENCES canal(idcanal);


--
-- Name: acumulado_canal_idcuenta_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY acumulado_canal
    ADD CONSTRAINT acumulado_canal_idcuenta_fkey FOREIGN KEY (idcuenta) REFERENCES cuenta(idcuenta);


--
-- Name: adocumental_idasiento_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY adocumental
    ADD CONSTRAINT adocumental_idasiento_fkey FOREIGN KEY (idasiento) REFERENCES asiento(idasiento);


--
-- Name: apunte_contrapartida_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY apunte
    ADD CONSTRAINT apunte_contrapartida_fkey FOREIGN KEY (contrapartida) REFERENCES cuenta(idcuenta);


--
-- Name: apunte_idasiento_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY apunte
    ADD CONSTRAINT apunte_idasiento_fkey FOREIGN KEY (idasiento) REFERENCES asiento(idasiento);


--
-- Name: apunte_idc_coste_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY apunte
    ADD CONSTRAINT apunte_idc_coste_fkey FOREIGN KEY (idc_coste) REFERENCES c_coste(idc_coste);


--
-- Name: apunte_idcanal_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY apunte
    ADD CONSTRAINT apunte_idcanal_fkey FOREIGN KEY (idcanal) REFERENCES canal(idcanal);


--
-- Name: apunte_idcuenta_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY apunte
    ADD CONSTRAINT apunte_idcuenta_fkey FOREIGN KEY (idcuenta) REFERENCES cuenta(idcuenta);


--
-- Name: apunte_iddiario_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY apunte
    ADD CONSTRAINT apunte_iddiario_fkey FOREIGN KEY (iddiario) REFERENCES diario(iddiario);


--
-- Name: borrador_contrapartida_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY borrador
    ADD CONSTRAINT borrador_contrapartida_fkey FOREIGN KEY (contrapartida) REFERENCES cuenta(idcuenta);


--
-- Name: borrador_idasiento_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY borrador
    ADD CONSTRAINT borrador_idasiento_fkey FOREIGN KEY (idasiento) REFERENCES asiento(idasiento);


--
-- Name: borrador_idc_coste_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY borrador
    ADD CONSTRAINT borrador_idc_coste_fkey FOREIGN KEY (idc_coste) REFERENCES c_coste(idc_coste);


--
-- Name: borrador_idcanal_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY borrador
    ADD CONSTRAINT borrador_idcanal_fkey FOREIGN KEY (idcanal) REFERENCES canal(idcanal);


--
-- Name: borrador_idcuenta_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY borrador
    ADD CONSTRAINT borrador_idcuenta_fkey FOREIGN KEY (idcuenta) REFERENCES cuenta(idcuenta);


--
-- Name: borrador_iddiario_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY borrador
    ADD CONSTRAINT borrador_iddiario_fkey FOREIGN KEY (iddiario) REFERENCES diario(iddiario);


--
-- Name: c_coste_padre_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY c_coste
    ADD CONSTRAINT c_coste_padre_fkey FOREIGN KEY (padre) REFERENCES c_coste(idc_coste);


--
-- Name: c_coste_padre_fkey1; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY c_coste
    ADD CONSTRAINT c_coste_padre_fkey1 FOREIGN KEY (padre) REFERENCES c_coste(idc_coste);


--
-- Name: c_coste_padre_fkey2; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY c_coste
    ADD CONSTRAINT c_coste_padre_fkey2 FOREIGN KEY (padre) REFERENCES c_coste(idc_coste);


--
-- Name: c_coste_padre_fkey3; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY c_coste
    ADD CONSTRAINT c_coste_padre_fkey3 FOREIGN KEY (padre) REFERENCES c_coste(idc_coste);


--
-- Name: c_costedist_idc_coste_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY c_costedist
    ADD CONSTRAINT c_costedist_idc_coste_fkey FOREIGN KEY (idc_coste) REFERENCES c_coste(idc_coste);


--
-- Name: c_costedist_iddestc_coste_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY c_costedist
    ADD CONSTRAINT c_costedist_iddestc_coste_fkey FOREIGN KEY (iddestc_coste) REFERENCES c_coste(idc_coste);


--
-- Name: compbalance_idbalance_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY compbalance
    ADD CONSTRAINT compbalance_idbalance_fkey FOREIGN KEY (idbalance) REFERENCES balance(idbalance);


--
-- Name: compmasap_idcuenta_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY compmasap
    ADD CONSTRAINT compmasap_idcuenta_fkey FOREIGN KEY (idcuenta) REFERENCES cuenta(idcuenta);


--
-- Name: cuenta_idprovincia_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY cuenta
    ADD CONSTRAINT cuenta_idprovincia_fkey FOREIGN KEY (idprovincia) REFERENCES provincia(idprovincia);


--
-- Name: idc_costefk; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY cuenta
    ADD CONSTRAINT idc_costefk FOREIGN KEY (idc_coste) REFERENCES c_coste(idc_coste);


--
-- Name: iva_idregistroiva_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY iva
    ADD CONSTRAINT iva_idregistroiva_fkey FOREIGN KEY (idregistroiva) REFERENCES registroiva(idregistroiva);


--
-- Name: iva_idtipoiva_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY iva
    ADD CONSTRAINT iva_idtipoiva_fkey FOREIGN KEY (idtipoiva) REFERENCES tipoiva(idtipoiva);


--
-- Name: lgaspresupuestoc_idcuenta_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY lgaspresupuestoc
    ADD CONSTRAINT lgaspresupuestoc_idcuenta_fkey FOREIGN KEY (idcuenta) REFERENCES cuenta(idcuenta);


--
-- Name: lgaspresupuestoc_idpresupuestoc_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY lgaspresupuestoc
    ADD CONSTRAINT lgaspresupuestoc_idpresupuestoc_fkey FOREIGN KEY (idpresupuestoc) REFERENCES presupuestoc(idpresupuestoc);


--
-- Name: linamortizacion_idasiento_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY linamortizacion
    ADD CONSTRAINT linamortizacion_idasiento_fkey FOREIGN KEY (idasiento) REFERENCES asiento(idasiento);


--
-- Name: lingpresupuestoc_idcuenta_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY lingpresupuestoc
    ADD CONSTRAINT lingpresupuestoc_idcuenta_fkey FOREIGN KEY (idcuenta) REFERENCES cuenta(idcuenta);


--
-- Name: lingpresupuestoc_idpresupuestoc_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY lingpresupuestoc
    ADD CONSTRAINT lingpresupuestoc_idpresupuestoc_fkey FOREIGN KEY (idpresupuestoc) REFERENCES presupuestoc(idpresupuestoc);


--
-- Name: mpatrimonial_idbalance_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY mpatrimonial
    ADD CONSTRAINT mpatrimonial_idbalance_fkey FOREIGN KEY (idbalance) REFERENCES balance(idbalance);


--
-- Name: presupuestoc_idc_coste_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY presupuestoc
    ADD CONSTRAINT presupuestoc_idc_coste_fkey FOREIGN KEY (idc_coste) REFERENCES c_coste(idc_coste);


--
-- Name: prevcobro_idasiento_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY prevcobro
    ADD CONSTRAINT prevcobro_idasiento_fkey FOREIGN KEY (idasiento) REFERENCES asiento(idasiento);


--
-- Name: prevcobro_idctacliente_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY prevcobro
    ADD CONSTRAINT prevcobro_idctacliente_fkey FOREIGN KEY (idctacliente) REFERENCES cuenta(idcuenta);


--
-- Name: prevcobro_idcuenta_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY prevcobro
    ADD CONSTRAINT prevcobro_idcuenta_fkey FOREIGN KEY (idcuenta) REFERENCES cuenta(idcuenta);


--
-- Name: prevcobro_idfpago_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY prevcobro
    ADD CONSTRAINT prevcobro_idfpago_fkey FOREIGN KEY (idfpago) REFERENCES fpago(idfpago);


--
-- Name: prevcobro_idregistroiva_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY prevcobro
    ADD CONSTRAINT prevcobro_idregistroiva_fkey FOREIGN KEY (idregistroiva) REFERENCES registroiva(idregistroiva);


--
-- Name: provincia_idpais_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY provincia
    ADD CONSTRAINT provincia_idpais_fkey FOREIGN KEY (idpais) REFERENCES pais(idpais);


--
-- Name: registroiva_contrapartida_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY registroiva
    ADD CONSTRAINT registroiva_contrapartida_fkey FOREIGN KEY (contrapartida) REFERENCES cuenta(idcuenta);


--
-- Name: registroiva_idborrador_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY registroiva
    ADD CONSTRAINT registroiva_idborrador_fkey FOREIGN KEY (idborrador) REFERENCES borrador(idborrador);


--
-- Name: registroiva_idfpago_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY registroiva
    ADD CONSTRAINT registroiva_idfpago_fkey FOREIGN KEY (idfpago) REFERENCES fpago(idfpago);


--
-- Name: registroiva_rectificaaregistroiva_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY registroiva
    ADD CONSTRAINT registroiva_rectificaaregistroiva_fkey FOREIGN KEY (rectificaaregistroiva) REFERENCES registroiva(idregistroiva);


--
-- Name: tipoiva_idcuenta_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY tipoiva
    ADD CONSTRAINT tipoiva_idcuenta_fkey FOREIGN KEY (idcuenta) REFERENCES cuenta(idcuenta);


--
-- Name: public; Type: ACL; Schema: -; Owner: postgres
--

REVOKE ALL ON SCHEMA public FROM PUBLIC;
REVOKE ALL ON SCHEMA public FROM postgres;
GRANT ALL ON SCHEMA public TO postgres;
GRANT ALL ON SCHEMA public TO PUBLIC;


--
-- PostgreSQL database dump complete
--

