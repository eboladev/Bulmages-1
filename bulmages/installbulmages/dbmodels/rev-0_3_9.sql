CREATE FUNCTION bcontrapartida(integer) RETURNS integer
AS '
DECLARE
    midapunte ALIAS FOR $1;
    apt RECORD;
    aptasien RECORD;
    cont RECORD;
    descuadre FLOAT;
    apmaxdebe INTEGER;
    apmaxhaber INTEGER;
    maxdebe FLOAT;
    maxhaber FLOAT;
    salida BOOLEAN;
    salidadebe BOOLEAN;
    salidahaber BOOLEAN;
BEGIN
-- Dado un apunte nos retorna el apunte que ejerce de contrapartida. (no la cuenta si no el apunte).
-- Esta funcion puede variar con la funcion ccontrapartida ya que en ccontrapartida el calculo es distinto y puede ser erroneo.
-- Esta funcion esta mucho mas perfeccionada con lo que es menos probable un error. Por tanto se sugiere la migracion a esta nueva funcion
    RAISE NOTICE ''Em pezamos'';
    SELECT INTO apt * FROM apunte WHERE idapunte=midapunte;
    IF apt.contrapartida ISNULL THEN
	-- Inicializamos las variables.
	descuadre:=0;
	maxdebe :=0;
	maxhaber := 0;
	apmaxdebe:=0;
	apmaxhaber:=0;
	salida := FALSE;
	salidadebe := FALSE;
	salidahaber := FALSE;
	
	FOR  cont IN SELECT  * FROM apunte WHERE idasiento = apt.idasiento ORDER BY orden LOOP
	    -- Si es la cuenta que estamos buscando lo hacemos constar.
	    IF cont.idapunte = midapunte THEN
		salida := TRUE;
		IF cont.debe > 0 THEN
		    salidadebe := TRUE;
		ELSE
		    salidahaber := TRUE;
		END IF;
	    END IF;
	    -- Si es el debe maximo lo hacemos constar.
	    IF cont.debe > maxdebe THEN
		maxdebe := cont.debe;
		apmaxdebe := cont.idapunte;
	    END IF;
	    -- Si es el haber mximo lo hacemos constar
	    IF cont.haber > maxhaber THEN 
		maxhaber := cont.haber;
		apmaxhaber := cont.idapunte;
	    END IF;
	    -- Calculamos el descuadre
	    descuadre := descuadre + cont.debe;
	    descuadre := descuadre - cont.haber;
	    -- Si es el descuadre inicializamos las variables.
            
            
            
	    IF (descuadre < 0.001) THEN
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
    ELSE
	SELECT INTO cont * FROM apunte WHERE idasiento = apt.idasiento AND idcuenta = apt.contrapartida;
	RETURN cont.idapunte;
    END IF;
END;
' LANGUAGE plpgsql;


CREATE FUNCTION bcontrapartidaborr(integer) RETURNS integer
AS '
DECLARE
    midapunte ALIAS FOR $1;
    apt RECORD;
    aptasien RECORD;
    cont RECORD;
    descuadre FLOAT;
    apmaxdebe INTEGER;
    apmaxhaber INTEGER;
    maxdebe FLOAT;
    maxhaber FLOAT;
    salida BOOLEAN;
    salidadebe BOOLEAN;
    salidahaber BOOLEAN;
BEGIN
-- Dado un apunte nos retorna el apunte que ejerce de contrapartida. (no la cuenta si no el apunte).
-- Esta funcion puede variar con la funcion ccontrapartida ya que en ccontrapartida el calculo es distinto y puede ser erroneo.
-- Esta funcion esta mucho mas perfeccionada con lo que es menos probable un error. Por tanto se sugiere la migracion a esta nueva funcion
    RAISE NOTICE ''Empezamos'';
    SELECT INTO apt * FROM borrador WHERE idborrador=midapunte;
    IF apt.contrapartida ISNULL THEN
	-- Inicializamos las variables.
	descuadre:=0;
	maxdebe :=0;
	maxhaber := 0;
	apmaxdebe:=0;
	apmaxhaber:=0;
	salida := FALSE;
	salidadebe := FALSE;
	salidahaber := FALSE;
	FOR  cont IN SELECT  * FROM borrador WHERE idasiento = apt.idasiento ORDER BY orden LOOP
	    -- Si es la cuenta que estamos buscando lo hacemos constar.
	    IF cont.idborrador = midapunte THEN
                RAISE NOTICE ''Este ciclo debemos salir'';
		salida := TRUE;
		IF cont.debe = 0 THEN
		    salidadebe := TRUE;
                    RAISE NOTICE ''Salida por debe %'', salidadebe;
		ELSE
		    salidahaber := TRUE;
                    RAISE NOTICE ''Salida por haber %'', salidahaber;
		END IF;
	    END IF;
	    -- Si es el debe maximo lo hacemos constar.
	    IF cont.debe > maxdebe THEN
		maxdebe := cont.debe;
		apmaxdebe := cont.idborrador;
                RAISE NOTICE ''Maxdebe: %, apmaxdebe: %'', maxdebe, apmaxdebe;
	    END IF;
	    -- Si es el haber mximo lo hacemos constar
	    IF cont.haber > maxhaber THEN 
		maxhaber := cont.haber;
		apmaxhaber := cont.idborrador;
                RAISE NOTICE ''Maxhaber: %, apmaxhaber: %'', maxhaber, apmaxhaber;                
	    END IF;
	    -- Calculamos el descuadre
	    descuadre := descuadre + cont.debe;
	    descuadre := descuadre - cont.haber;
	    -- Si es el descuadre inicializamos las variables.
           RAISE NOTICE ''Ciclo de iborrador %, debe %, haber %, descuadre %'', cont.idborrador, cont.debe, cont.haber, descuadre;            
	    IF (descuadre < 0.001) THEN
               RAISE NOTICE '' El descuadre es igual a cero'';
		IF (salidadebe) THEN
                   RAISE NOTICE '' Salida debe %'', apmaxdebe;
		    RETURN apmaxdebe;
		END IF;
		IF (salidahaber) THEN
                   RAISE NOTICE '' Salida haber %'', apmaxhaber;
		    RETURN apmaxhaber;
		END IF;
		maxdebe := 0;
		maxhaber := 0;
		apmaxdebe:=0;
		apmaxhaber := 0;
	    END IF;
	END LOOP;
    ELSE
	SELECT INTO cont * FROM borrador WHERE idasiento = apt.idasiento AND idcuenta = apt.contrapartida;
	RETURN cont.idapunte;
    END IF;
END;
' LANGUAGE plpgsql;
