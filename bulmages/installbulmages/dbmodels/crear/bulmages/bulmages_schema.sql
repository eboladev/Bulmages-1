-- La tabla de configuración.
-- En esta tabla se guardan parametros que el programa va a utilizar.
-- Como por ejemplo el numero de dígitos por defecto de las cuentas o el asiento inteligente que se enlaza con
-- facturacion.
-- Tiene tres campos
-- idconfiguracion: el identificador (No tiene ningún uso especial).
-- nombre: El nombre del parametro de configuracion.
-- valor: El valor que toma dicho parametro.
CREATE TABLE configuracion (
    idconfiguracion integer NOT NULL,
    nombre character varying(25),
    valor character varying(350)
);



CREATE TABLE grupo (
    idgrupo serial PRIMARY KEY,
    descripcion character varying(50)
);



-- La tabla cuenta es la que presenta el plan contable.
-- Principalmente define el arbol de cuentas, tiene varios campos de SOLO LECTURA que sirven para acumulados.
-- Los campos son:
-- idcuenta El identificador de la cuenta. Tipo Serial
-- codigo El codigo de la cuenta.
-- descripcion Descripcion de la cuenta
-- imputacion booleano que indica si la cuenta es de imputacion o no (aun no he descubierto para que sirve).
-- padre Este campo indica el idcuenta padre de la cuenta que tratamos. Es un apuntador al indice de la tabla lo que la convierte en arbol
-- bloqueada Este boleano indica si la cuenta esta bloqueada o no
-- idgrupo es el apuntador  a la tabla de grupos.
-- msg ?????
-- debe Este campo es de solo lectura e indica el acumulado en debe de la cuenta.
-- haber Este campo es de solo lectura e indica el acumulado en haber de la cuenta.
-- nodebe Este booleano indica si la cuenta puede tener inserciones en el debe o no.
-- nohaber Este booleano indica si la cuenta puede tener inserciones en el haber o no.
-- regularizacion Este booleano indica si la cuenta es de regularizacion o no.
-- activo: Este campo indica si la cuenta es de activo o de pasivo.
-- nombreent_cuenta: Si hay una entidad ligada a la cuenta aqui ponemos su nombre.
-- cifent_cuenta: Si hay una entidad ligada a la cuenta aqui ponemos su CIF
-- dirent_cuenta: Direccion de una entidad ligada con la cuenta
-- telent_cuenta: Telefono de una entidad ligada con la cuetna.
-- coment_cuenta: Comentarios de una entidad ligada con la cuenta.
-- bancoent_cuenta: Cuenta bancaria de una entidad ligada con la cuenta.
-- emailent_cuenta: Direccion de correo ligada con la entidad
-- webent_cuenta: Pagina web ligada con la entidad
-- tipocuenta: Campo de tipo integer que en realidad es un ENUM (para los valores que ahora no recuerdo y falta rellenar) ??????

CREATE TABLE cuenta (
    idcuenta serial PRIMARY KEY,
    codigo character varying(12) NOT NULL,
    descripcion character varying(100) NOT NULL,
    imputacion boolean NOT NULL DEFAULT TRUE,
    padre integer,
    bloqueada boolean NOT NULL DEFAULT FALSE,
    idgrupo integer NOT NULL REFERENCES grupo(idgrupo),
    msg character varying(500),
    debe double precision NOT NULL DEFAULT 0,
    haber double precision NOT NULL DEFAULT 0,
    nodebe boolean NOT NULL DEFAULT FALSE,
    nohaber boolean NOT NULL DEFAULT FALSE,
    regularizacion boolean,
    activo boolean,
    nombreent_cuenta character varying(100),
    cifent_cuenta character varying(12),
    dirent_cuenta character varying(80),
    telent_cuenta character varying(20),
    coment_cuenta character varying(500),
    bancoent_cuenta character varying(30),
    emailent_cuenta character varying(50),
    webent_cuenta character varying(70),
    tipocuenta integer
);





-- La tabla canal refleja los canales a los que puede pertenecer un apunte determinado.
CREATE TABLE canal (
    idcanal serial PRIMARY KEY,
    descripcion character varying(100),
    nombre character varying(50)
);



CREATE TABLE c_coste (
    idc_coste serial PRIMARY KEY,
    descripcion character varying(100),
    nombre character varying(50) NOT NULL,
    codigo character(3),
    padre integer,
    imputacion boolean,
    debe double precision DEFAULT 0,
    haber double precision DEFAULT 0
);


CREATE TABLE acumulado_c_coste (
    idacumulado_c_coste serial PRIMARY KEY,
    idcuenta integer NOT NULL REFERENCES cuenta(idcuenta),
    idc_coste integer NOT NULL REFERENCES c_coste(idc_coste),
    debe double precision DEFAULT 0,
    haber double precision DEFAULT 0
);


CREATE TABLE acumulado_canal (
    idacumulado_canal serial PRIMARY KEY,
    idcuenta integer NOT NULL REFERENCES cuenta(idcuenta),
    idcanal integer NOT NULL REFERENCES canal(idcanal),
    debe double precision DEFAULT 0,
    haber double precision DEFAULT 0
);


--
-- TOC entry 36 (OID 1345982)
-- Name: diario; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE diario (
    iddiario serial PRIMARY KEY,
    descripcion character varying(100)
);

\echo "La tabla diario está en desuso, aunque de momento se crea."



CREATE TABLE asiento (
    idasiento serial PRIMARY KEY,
    descripcion character varying(100),
    fecha timestamp with time zone,
    comentariosasiento character varying(2000),
    ordenasiento integer NOT NULL,
    clase smallint DEFAULT 0
);



CREATE TABLE adocumental (
    idadocumental serial PRIMARY KEY,
    idasiento integer REFERENCES asiento(idasiento),
    descripcionadocumental character varying(200),
    fechaintadocumental timestamp with time zone,
    fechaasadocumental timestamp with time zone,
    archivoadocumental character varying(300)
);



CREATE TABLE apunte (
    idapunte serial PRIMARY KEY,
    codigoborrador integer,
    idasiento integer NOT NULL REFERENCES asiento(idasiento),
    iddiario integer REFERENCES diario(iddiario),
    fecha timestamp with time zone,
    conceptocontable character varying(50),
    idcuenta integer NOT NULL REFERENCES cuenta(idcuenta),
    descripcion character varying(100),
    debe double precision NOT NULL DEFAULT 0,
    haber double precision NOT NULL DEFAULT 0,
    contrapartida integer REFERENCES cuenta(idcuenta),
    comentario character varying(100),
    idcanal integer REFERENCES canal(idcanal),
    marcaconciliacion character(12),
    idc_coste integer REFERENCES c_coste(idc_coste),
    idtipoiva integer,
    orden integer,
    punteo boolean DEFAULT false
);



CREATE TABLE borrador (
    idborrador serial PRIMARY KEY,
    codigoborrador integer,
    idasiento integer NOT NULL REFERENCES asiento(idasiento),
    iddiario integer REFERENCES diario(iddiario),
    fecha timestamp with time zone,
    conceptocontable character varying(50),
    idcuenta integer NOT NULL REFERENCES cuenta(idcuenta),
    descripcion character varying(100),
    debe double precision NOT NULL DEFAULT 0,
    haber double precision NOT NULL DEFAULT 0,
    contrapartida integer REFERENCES cuenta(idcuenta),
    comentario character varying(100),
    idcanal integer REFERENCES canal(idcanal),
    marcaconciliacion character(12),
    idc_coste integer REFERENCES c_coste(idc_coste),
    idapunte integer,
    idtipoiva integer,
    orden integer
);





-- La tabla fpago lleva las formas de pago existentes
-- idfpago Identificador de la forma de pago
-- nomfpago Nombre de la forma de pago
-- nplazosfpago Numero de plazos de la forma de pago
-- plazoprimerpagofpago El plazo hasta el primer cobro/pago.
-- tipoplazoprimerpagofpago Tipo del plazo de la forma de pago
--                      0 - dias
--                      1 - semanas
--                      2 - meses
--                      3 - años
-- plazoentrerecibofpago numero de plazos en los siguientes recibos
-- tipoplazoentrerecibofpago (Igual que tipoplazoprimerpagofpago)

CREATE TABLE fpago (
    idfpago serial PRIMARY KEY,
    nomfpago character varying(50),
    nplazosfpago integer,
    plazoprimerpagofpago integer,
    tipoplazoprimerpagofpago integer,
    plazoentrerecibofpago integer,
    tipoplazoentrerecibofpago integer
);




-- La tabla de tipos de iva indica los tipos de iva que se soportan
-- Sirve para saber si un iva determinado es correcto o no.
-- Esta en forma de tabla y no en otra forma debido a que los porcentajes de IVA podrian cambiar.
-- Los campos son:
-- idtipoiva: Identificador de la tabla.
-- nombretipoiva: da un nombre al tipo de iva.
-- Porcentajetipoiva: El porcentaje que corresponde con este tipo.
CREATE TABLE tipoiva (
    idtipoiva serial PRIMARY KEY,
    nombretipoiva character varying(25),
    porcentajetipoiva float,
    idcuenta integer NOT NULL REFERENCES cuenta(idcuenta)
);


--
-- TOC entry 40 (OID 1346006)
-- Name: registroiva; Type: TABLE; Schema: public; Owner: postgres
--
-- ffactura fecha de la factura.

CREATE TABLE registroiva (
    idregistroiva serial PRIMARY KEY,
    contrapartida integer REFERENCES cuenta(idcuenta),
    baseimp double precision,
    iva double precision,
    ffactura date,
    factura character varying(70),
    idborrador integer,
    incregistro boolean,
    regularizacion boolean,
    plan349 boolean,
    numorden character varying(50),
    cif character varying(25),
    idfpago integer REFERENCES fpago(idfpago),
    rectificaaregistroiva integer REFERENCES registroiva(idregistroiva)
);

-- La tabla prevcobro es prevision de cobros / pagos para facturas.
-- Esta tabla proporciona las formas de pago de cada factura que se pasa a un cliente o que recibimos de un proveedor.

-- tipoprevcobro indica si es un cobro (true) o un pago (false).
-- fpagoprevcobro es un identificador a la otra tabla de Formas de Pago.
-- idcuenta La cuenta sobre la que se hará el cobro / pago.
-- idasiento Asiento de cobro (Si está hecho).
-- cantidadprevistaprevcobro Cantidad Prevista del cobro.
-- cantidadprevcobro         Cantidad Cobrada.
-- fprevistaprevcobro        Fecha en que se prevee el cobro / pago.
-- fcobroprevcobro           Fecha en que se ha realizado el cobro / pago.
-- idregistroiva             Registro de IVA con el que se corresponde, o factura con la que se corresponde.
-- tipoprevcobro             Indica si es un cobro o un pago. TRUE --> COBRO FALSE -->PAGO


CREATE TABLE prevcobro (
    idprevcobro serial PRIMARY KEY,
    fprevistaprevcobro date,
    fcobroprevcobro date,
    idfpago integer REFERENCES fpago(idfpago),
    idcuenta integer REFERENCES cuenta(idcuenta),
    idasiento integer REFERENCES asiento(idasiento),
    cantidadprevistaprevcobro double precision,
    cantidadprevcobro  double precision,
    idregistroiva integer NOT NULL REFERENCES registroiva(idregistroiva),
    tipoprevcobro boolean,
    docprevcobro character varying(50)
);

\echo "Se ha creado la tabla prevcobro"

-- Esta tabla especifica para cada factura los IVAS correspondientes.
CREATE TABLE iva (
   idiva serial PRIMARY KEY,
   idtipoiva integer NOT NULL REFERENCES tipoiva (idtipoiva),
   idregistroiva integer NOT NULL REFERENCES registroiva(idregistroiva),
   baseiva double precision DEFAULT 0
);

\echo "Se ha creado la tabla iva"


CREATE TABLE ainteligente (
    idainteligente serial PRIMARY KEY,
    descripcion character varying(100),
    comentariosasiento character varying(2000)
);


CREATE TABLE binteligente (
    idbinteligente serial PRIMARY KEY,
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



--
-- TOC entry 47 (OID 1346044)
-- Name: balance; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE balance (
    idbalance serial PRIMARY KEY,
    nombrebalance character varying(150)
);

\echo "Se ha creado balance"

--
-- TOC entry 45 (OID 1346034)
-- Name: mpatrimonial; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE mpatrimonial (
    idmpatrimonial serial PRIMARY KEY,
    idbalance integer REFERENCES balance(idbalance),
    descmpatrimonial character varying(150),
    orden integer,
    tabulacion integer,
    saldo double precision,
    opdesc integer
);

\echo "Se ha creado la tabla mpatrimonial"

--
-- TOC entry 44 (OID 1346029)
-- Name: compmasap; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE compmasap (
    idcompmasap serial PRIMARY KEY,
    idcuenta integer REFERENCES cuenta(idcuenta),
    idmpatrimonial integer,
    masaperteneciente integer,
    saldo double precision,
    signo boolean,
    nombre character varying(150)
);

\echo "Se ha creado la tabla compmasap"

--
-- TOC entry 46 (OID 1346039)
-- Name: compbalance; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE compbalance (
    idcompbalance serial PRIMARY KEY,
    idbalance integer NOT NULL REFERENCES balance(idbalance),
    idmpatrimonial integer,
    concepto character varying(150),
    orden integer,
    tabulacion integer
);

\echo "Se ha creado la tabla compbalance"



--
-- TOC entry 85 (OID 1346047)
-- Name: abreasientos(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION abreasientos() RETURNS integer
    AS '
--DECLARE as RECORD;
--DECLARE res RECORD;
BEGIN
--    FOR as IN SELECT * FROM asiento ORDER BY idasiento DESC LOOP
--      SELECT INTO res cambiaasiento(as.idasiento, as.idasiento*3);
--    END LOOP;
--  Abrir los asientos es modificar el campo ordenasiento de los mismos para que se reorganicen
    UPDATE asiento SET ordenasiento= ordenasiento * 3;
    RETURN 0;
END;
'
    LANGUAGE plpgsql;


--
-- TOC entry 86 (OID 1346048)
-- Name: cambiaasiento(integer, integer); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION cambiaasiento(integer, integer) RETURNS integer
    AS '
DECLARE
    idinicial ALIAS FOR $1;
    idfinal ALIAS FOR $2;
    as RECORD;
BEGIN
    -- Esta funcion cambia un asiento de sitio, el asiento inicial y lo pone en el asiento final
    -- El problema es que si el asiento final debe estar vacio
    SELECT INTO as * FROM asiento WHERE idasiento = idinicial;
    IF FOUND THEN
        INSERT INTO asiento (idasiento, fecha, descripcion, comentariosasiento) VALUES (idfinal, as.fecha, as.descripcion, as.comentariosasiento);
        UPDATE borrador SET idasiento = idfinal WHERE idasiento = idinicial;
        UPDATE apunte SET idasiento = idfinal WHERE idasiento = idinicial;
        DELETE FROM asiento WHERE idasiento = idinicial;
    END IF;
    RETURN 0;
END;
'
    LANGUAGE plpgsql;


--
-- TOC entry 87 (OID 1346049)
-- Name: ccontrapartida(integer); Type: FUNCTION; Schema: public; Owner: postgres
--
CREATE FUNCTION ccontrapartida(integer) RETURNS integer
    AS '
DECLARE
    midapunte ALIAS FOR $1;
    apt RECORD;
    aptasien RECORD;
    contra RECORD;
BEGIN
    SELECT INTO apt * FROM apunte WHERE idapunte= midapunte;
    IF apt.contrapartida ISNULL THEN
        SELECT INTO contra * FROM apunte WHERE idapunte = bcontrapartida(midapunte);
        IF FOUND THEN
                RETURN contra.idcuenta;
        ELSE
                RETURN 0;
        END IF;
    ELSE
        RETURN apt.contrapartida;
    END IF;
END;
'
    LANGUAGE plpgsql;


--
-- TOC entry 88 (OID 1346050)
-- Name: bcontrapartida(integer); Type: FUNCTION; Schema: public; Owner: postgres
--

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
        salidadebe := FALSE;
        salidahaber := FALSE;

        FOR  cont IN SELECT  * FROM apunte WHERE idasiento = apt.idasiento ORDER BY orden LOOP
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
            -- Si es el haber mximo lo hacemos constar
            IF cont.haber > maxhaber THEN
                maxhaber := cont.haber;
                apmaxhaber := cont.idapunte;
            END IF;
            -- Calculamos el descuadre
            descuadre := descuadre + cont.debe;
            descuadre := descuadre - cont.haber;
            -- Si es el descuadre inicializamos las variables.
            IF (descuadre*descuadre < 0.001) THEN  -- Asi nos aseguramos que valores positivos también entran.
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
        SELECT INTO cont * FROM apunte WHERE idasiento = apt.idasiento AND idcuenta = apt.contrapartida;
        RETURN cont.idapunte;
    END IF;
END;
'
    LANGUAGE plpgsql;


--
-- TOC entry 89 (OID 1346051)
-- Name: bcontrapartidaborr(integer); Type: FUNCTION; Schema: public; Owner: postgres
--

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
    RAISE NOTICE ''Em pezamos'';
    SELECT INTO apt * FROM borrador WHERE idborrador=midapunte;
    IF apt.contrapartida ISNULL THEN
        -- Inicializamos las variables.
        descuadre:=0;
        maxdebe :=0;
        maxhaber := 0;
        apmaxdebe:=0;
        apmaxhaber:=0;
        salidadebe := FALSE;
        salidahaber := FALSE;

        FOR  cont IN SELECT  * FROM borrador WHERE idasiento = apt.idasiento ORDER BY orden LOOP
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
            -- Si es el haber mximo lo hacemos constar
            IF cont.haber > maxhaber THEN
                maxhaber := cont.haber;
                apmaxhaber := cont.idborrador;
            END IF;
            -- Calculamos el descuadre
            descuadre := descuadre + cont.debe;
            descuadre := descuadre - cont.haber;
            -- Si es el descuadre inicializamos las variables.
            IF (descuadre*descuadre < 0.001) THEN  -- Asi nos aseguramos que valores positivos también entran.
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
        SELECT INTO cont * FROM borrador WHERE idasiento = apt.idasiento AND idcuenta = apt.contrapartida;
        RETURN cont.idborrador;
    END IF;
END;'
    LANGUAGE plpgsql;


--
-- TOC entry 90 (OID 1346052)
-- Name: id_cuenta(character varying); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION id_cuenta(character varying) RETURNS integer
    AS '
DECLARE
   codcuenta ALIAS FOR $1;
   cta  RECORD;
BEGIN
   SELECT INTO cta idcuenta FROM cuenta  WHERE codigo = "codcuenta";
   IF FOUND THEN
        RETURN cta.idcuenta;
   ELSE
        RETURN 0;
   END IF;
END;
'
    LANGUAGE plpgsql;


--
-- TOC entry 91 (OID 1346053)
-- Name: nivel(character varying); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION nivel(character varying) RETURNS integer
    AS '
DECLARE
    codcuenta ALIAS FOR $1;
BEGIN
    RETURN LENGTH(codcuenta);
END;
'
    LANGUAGE plpgsql;


--
-- TOC entry 92 (OID 1346054)
-- Name: saldototalmpatrimonial(integer); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION saldototalmpatrimonial(integer) RETURNS double precision
    AS '
DECLARE
   identmpatrimonial ALIAS FOR $1;
   saldo DOUBLE PRECISION;
   rsaldo RECORD;
   rsaldo1 RECORD;
   smpatrimonialsum RECORD;
   smpatrimonialrest RECORD;
BEGIN
    saldo := 0;
    FOR rsaldo IN SELECT (sum(debe)-sum(haber)) AS total FROM cuenta WHERE idcuenta IN (SELECT  idcuenta FROM compmasap WHERE masaperteneciente = identmpatrimonial AND idcuenta IS NOT NULL AND signo=true) LOOP
       IF (rsaldo.total IS NOT NULL) THEN
           saldo := saldo + rsaldo.total;
       END IF;
    END LOOP;
    FOR rsaldo1 IN SELECT (sum(debe)-sum(haber)) AS total FROM cuenta WHERE idcuenta IN (SELECT  idcuenta FROM compmasap WHERE masaperteneciente = identmpatrimonial AND idcuenta IS NOT NULL AND signo=false) LOOP
       IF (rsaldo1.total IS NOT NULL) THEN
          saldo := saldo - rsaldo1.total;
       END IF;
    END LOOP;
    FOR smpatrimonialsum IN SELECT idmpatrimonial FROM compmasap WHERE masaperteneciente = identmpatrimonial AND idmpatrimonial IS NOT NULL AND signo = true LOOP
        saldo := saldo + saldototalmpatrimonial (smpatrimonialsum.idmpatrimonial);
    END LOOP;
    FOR smpatrimonialrest IN SELECT idmpatrimonial FROM compmasap WHERE masaperteneciente = identmpatrimonial AND idmpatrimonial IS NOT NULL AND signo = false LOOP
        saldo := saldo - saldototalmpatrimonial (smpatrimonialrest.idmpatrimonial);
    END LOOP;
    RETURN saldo;
END;
'
    LANGUAGE plpgsql;


--
-- TOC entry 93 (OID 1346055)
-- Name: saldompatrimonial(integer, timestamp without time zone, timestamp without time zone); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION saldompatrimonial(integer, timestamp without time zone, timestamp without time zone) RETURNS double precision
    AS '
DECLARE
    identmpatrimonial ALIAS FOR $1;
    fechain ALIAS FOR $2;
    fechafin ALIAS FOR $3;
    aptsum RECORD;
    aptrest RECORD;
    smpatrimonialsum RECORD;
    smpatrimonialres RECORD;
    saldo DOUBLE PRECISION;
BEGIN
    -- Esta funcion calcula el saldo de una masa patrimonial entre dos fechas.
    saldo := 0;
    FOR aptsum IN SELECT * FROM cuenta WHERE idcuenta IN (SELECT idcuenta FROM compmasap WHERE masaperteneciente = identmpatrimonial AND idcuenta IS NOT NULL  AND signo = true) LOOP
        saldo := saldo + saldototal(aptsum.codigo, fechain, fechafin);
    END LOOP;
    FOR aptrest IN SELECT * FROM cuenta WHERE idcuenta IN (SELECT idcuenta FROM compmasap WHERE masaperteneciente = identmpatrimonial AND idcuenta IS NOT NULL  AND signo = false) LOOP
        saldo := saldo - saldototal(aptsum.codigo, fechain, fechafin);
    END LOOP;
    FOR smpatrimonialsum IN SELECT * FROM mpatrimonial WHERE idmpatrimonial IN (SELECT idmpatrimonial FROM compmasap WHERE masaperteneciente = identmpatrimonial AND idmpatrimonial IS NOT NULL AND signo = true) LOOP
        saldo := saldo + saldompatrimonial (smpatrimonialsum.idmpatrimonial, fechain, fechafin);
    END LOOP;
    FOR smpatrimonialsum IN SELECT * FROM mpatrimonial WHERE idmpatrimonial IN (SELECT idmpatrimonial FROM compmasap WHERE masaperteneciente = identmpatrimonial AND idmpatrimonial IS NOT NULL AND signo = false) LOOP
        saldo := saldo - saldompatrimonial (smpatrimonialsum.idmpatrimonial, fechain, fechafin);
    END LOOP;
    RETURN saldo;
END;
'
    LANGUAGE plpgsql;


--
-- TOC entry 94 (OID 1346056)
-- Name: debempatrimonial(integer, timestamp without time zone, timestamp without time zone); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION debempatrimonial(integer, timestamp without time zone, timestamp without time zone) RETURNS double precision
    AS '
DECLARE
    identmpatrimonial ALIAS FOR $1;
    fechain ALIAS FOR $2;
    fechafin ALIAS FOR $3;
    aptsum RECORD;
    aptrest RECORD;
    smpatrimonialsum RECORD;
    smpatrimonialres RECORD;
    debe DOUBLE PRECISION;
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
'
    LANGUAGE plpgsql;


--
-- TOC entry 95 (OID 1346057)
-- Name: habermpatrimonial(integer, timestamp without time zone, timestamp without time zone); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION habermpatrimonial(integer, timestamp without time zone, timestamp without time zone) RETURNS double precision
    AS '
DECLARE
    identmpatrimonial ALIAS FOR $1;
    fechain ALIAS FOR $2;
    fechafin ALIAS FOR $3;
    aptsum RECORD;
    aptrest RECORD;
    smpatrimonialsum RECORD;
    smpatrimonialres RECORD;
    haber DOUBLE PRECISION;
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
'
    LANGUAGE plpgsql;


--
-- TOC entry 96 (OID 1346058)
-- Name: saldototal(character varying, timestamp without time zone, timestamp without time zone); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION saldototal(character varying, timestamp without time zone, timestamp without time zone) RETURNS double precision
    AS '
DECLARE
    codcuenta ALIAS FOR $1;
    fechain ALIAS FOR $2;
    fechafin ALIAS FOR $3;
    apt RECORD;
    apt1 RECORD;
    apt2 RECORD;
         cta RECORD;
    saldo DOUBLE PRECISION;
BEGIN
--    SELECT INTO apt * FROM apunte WHERE id_cuenta(codcuenta) = apunte.idcuenta;
         SELECT INTO cta * FROM  cuenta WHERE codigo = codcuenta;
    SELECT INTO apt sum(debe) AS tdebe, sum(haber) AS thaber FROM apunte WHERE apunte.idcuenta = id_cuenta(codcuenta) AND fecha <= "fechafin" AND fecha >= "fechain";
    IF (apt.tdebe ISNULL) THEN
        saldo := 0;
    ELSE
        saldo := apt.tdebe - apt.thaber;
    END IF;
--    RAISE NOTICE '' saldo total % valor adquirido %'', codcuenta, saldo;
    FOR apt1 IN SELECT codigo FROM cuenta WHERE padre = id_cuenta(codcuenta) LOOP
        saldo := saldo + saldototal(apt1.codigo,fechain, fechafin);
    END LOOP;
    RETURN saldo;
END;
'
    LANGUAGE plpgsql;


--
-- TOC entry 97 (OID 1346059)
-- Name: debetotal(integer, timestamp without time zone, timestamp without time zone); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION debetotal(integer, timestamp without time zone, timestamp without time zone) RETURNS double precision
    AS '
DECLARE
    mcuenta ALIAS FOR $1;
    fechain ALIAS FOR $2;
    fechafin ALIAS FOR $3;
    apt RECORD;
    apt1 RECORD;
    apt2 RECORD;
         cta RECORD;
    debet DOUBLE PRECISION;
BEGIN
--    SELECT INTO apt * FROM apunte WHERE cuenta = apunte.idcuenta;
--    RAISE NOTICE '' Calculando debetotal de % entre % y % '', cuenta, fechain, fechafin;
    debet := 0;
         SELECT INTO apt sum(debe) AS mdebe FROM apunte WHERE apunte.idcuenta = mcuenta AND fecha <= "fechafin" AND fecha >= "fechain";
         if (apt.mdebe ISNULL) THEN
                debet:= 0;
         ELSE
                debet := apt.mdebe;
        END IF;
--    RAISE NOTICE '' debe total % valor adquirido %'', cuenta, debet;
    FOR apt1 IN SELECT idcuenta FROM cuenta WHERE padre = mcuenta LOOP
        debet := debet + debetotal(apt1.idcuenta,fechain, fechafin);
    END LOOP;
    RETURN debet;
END;
'
    LANGUAGE plpgsql;


--
-- TOC entry 98 (OID 1346060)
-- Name: debetotal1(integer); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION debetotal1(integer) RETURNS double precision
    AS '
DECLARE
    mcuenta ALIAS FOR $1;
    apt RECORD;
    apt1 RECORD;
    apt2 RECORD;
         cta RECORD;
    debet DOUBLE PRECISION;
BEGIN
--    SELECT INTO apt * FROM apunte WHERE id_cuenta(codcuenta) = apunte.idcuenta;
    debet := 0;
         SELECT INTO apt sum(debe) AS mdebe FROM apunte WHERE apunte.idcuenta = mcuenta;
         if (apt.mdebe ISNULL) THEN
                debet:= 0;
         ELSE
                debet := apt.mdebe;
        END IF;
--    RAISE NOTICE '' debe total1 % valor adquirido %'', codcuenta, debet;
    FOR apt1 IN SELECT idcuenta FROM cuenta WHERE padre = mcuenta LOOP
        debet := debet + debetotal1(apt1.idcuenta);
    END LOOP;
    RETURN debet;
END;
'
    LANGUAGE plpgsql;


--
-- TOC entry 99 (OID 1346061)
-- Name: habertotal(integer, timestamp without time zone, timestamp without time zone); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION habertotal(integer, timestamp without time zone, timestamp without time zone) RETURNS double precision
    AS '
DECLARE
    mcuenta ALIAS FOR $1;
    fechain ALIAS FOR $2;
    fechafin ALIAS FOR $3;
    apt RECORD;
    apt1 RECORD;
    apt2 RECORD;
         cta RECORD;
    habert DOUBLE PRECISION;
BEGIN
--    SELECT INTO apt * FROM apunte WHERE id_cuenta(codcuenta) = apunte.idcuenta;
        habert := 0;
    SELECT INTO apt sum(haber) AS thaber FROM apunte WHERE apunte.idcuenta = mcuenta AND fecha <= "fechafin" AND fecha >= "fechain";
         IF (apt.thaber ISNULL) THEN
                habert := 0;
        ELSE
                habert := apt.thaber;
        END IF;
--    RAISE NOTICE '' haber total % valor adquirido %'', codcuenta, habert;
    FOR apt1 IN SELECT idcuenta FROM cuenta WHERE padre = mcuenta LOOP
                habert := habert + habertotal(apt1.idcuenta,fechain, fechafin);
    END LOOP;
    RETURN habert;
END;
'
    LANGUAGE plpgsql;


--
-- TOC entry 100 (OID 1346062)
-- Name: habertotal1(integer); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION habertotal1(integer) RETURNS double precision
    AS '
DECLARE
    mcuenta ALIAS FOR $1;
    apt RECORD;
    apt1 RECORD;
    apt2 RECORD;
         cta RECORD;
    habert DOUBLE PRECISION;
BEGIN
--    SELECT INTO apt * FROM apunte WHERE id_cuenta(codcuenta) = apunte.idcuenta;
        habert := 0;
    SELECT INTO apt sum(haber) AS thaber FROM apunte WHERE apunte.idcuenta = mcuenta;
         IF (apt.thaber ISNULL) THEN
                habert := 0;
        ELSE
                habert := apt.thaber;
        END IF;
--    RAISE NOTICE '' haber total % valor adquirido %'', codcuenta, habert;
    FOR apt1 IN SELECT idcuenta FROM cuenta WHERE padre = mcuenta LOOP
                habert := habert + habertotal1(apt1.idcuenta);
    END LOOP;
    RETURN habert;
END;
'
    LANGUAGE plpgsql;


--
-- TOC entry 101 (OID 1346063)
-- Name: recalculasaldos(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION recalculasaldos() RETURNS double precision
    AS '
DECLARE
    cta RECORD;
BEGIN
-- El orden si es importante ya que aparecen problemas con el trigger. que se dispara en los updates.
    FOR cta IN SELECT * FROM cuenta WHERE padre IS NOT NULL ORDER BY padre LOOP
        UPDATE cuenta SET debe = debetotal1(cta.idcuenta), haber=habertotal1(cta.idcuenta) WHERE idcuenta = cta.idcuenta;
    END LOOP;
    FOR cta IN SELECT * FROM cuenta WHERE padre IS NULL ORDER BY padre LOOP
        UPDATE cuenta SET debe = debetotal1(cta.idcuenta), haber=habertotal1(cta.idcuenta) WHERE idcuenta = cta.idcuenta;
    END LOOP;
    RETURN 0;
END;
'
    LANGUAGE plpgsql;


--
-- TOC entry 102 (OID 1346064)
-- Name: aumenta_valor(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION aumenta_valor() RETURNS "trigger"
    AS '
DECLARE
   cta    int4;
   ccost  int4;
   ctar   RECORD;
   ccostr RECORD;
BEGIN
        UPDATE cuenta SET debe = debe + NEW.debe, haber = haber + NEW.haber WHERE idcuenta = NEW.idcuenta;
        UPDATE c_coste SET debe = debe + NEW.debe, haber = haber + NEW.haber WHERE idc_coste = NEW.idc_coste;
        IF NEW.idcuenta IS NOT NULL THEN
           UPDATE acumulado_canal SET debe= debe + NEW.debe, haber = haber + NEW.haber WHERE idcuenta = NEW.idcuenta AND
idcanal = NEW.idcanal;
        END IF;
        cta := NEW.idcuenta;
        ccost := NEW.idc_coste;
--        RAISE NOTICE '' Se ha lanzado la funcion aumenta_valor()'';
        SELECT INTO ccostr * FROM c_coste WHERE idc_coste = ccost;
        WHILE FOUND LOOP
           SELECT INTO ctar * FROM cuenta WHERE idcuenta = cta;
           WHILE FOUND LOOP
--                RAISE NOTICE '' Cuenta % Centro Coste %'', ctar.idcuenta, ccostr.idc_coste;
                UPDATE acumulado_c_coste SET debe = debe + NEW.debe, haber = haber + NEW.haber WHERE idc_coste =
ccostr.idc_coste AND idcuenta = ctar.idcuenta;
              SELECT INTO ctar * FROM cuenta WHERE idcuenta = ctar.padre;
           END LOOP;
           SELECT INTO ccostr * FROM c_coste WHERE idc_coste = ccostr.padre;
        END LOOP;
        RETURN NEW;
END;
'
    LANGUAGE plpgsql;


--
-- TOC entry 103 (OID 1346065)
-- Name: disminuye_valor(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION disminuye_valor() RETURNS "trigger"
    AS '
DECLARE
   cta    int4;
   ccost  int4;
   ctar   RECORD;
   ccostr RECORD;
BEGIN
--      RAISE NOTICE ''disminuye_valor: debe antiguo %, debe nuevo %'', OLD.debe, NEW.debe;

        UPDATE cuenta SET debe = debe - OLD.debe, haber = haber - OLD.haber WHERE idcuenta = OLD.idcuenta;
        UPDATE c_coste SET debe = debe - OLD.debe, haber = haber - OLD.haber WHERE idc_coste = OLD.idc_coste;
        IF OLD.idcuenta IS NOT NULL THEN
           UPDATE acumulado_canal SET debe= debe - OLD.debe, haber =haber - OLD.haber WHERE idcuenta = OLD.idcuenta AND
idcanal = OLD.idcanal;
        END IF;
        cta := OLD.idcuenta;
        ccost := OLD.idc_coste;
--      RAISE NOTICE '' Se ha lanzado la funcion disminuye_valor()'';
        SELECT INTO ccostr * FROM c_coste WHERE idc_coste = ccost;
        WHILE FOUND LOOP
           SELECT INTO ctar * FROM cuenta WHERE idcuenta = cta;
           WHILE FOUND LOOP
--            RAISE NOTICE '' Cuenta % Centro Coste %'', ctar.idcuenta, ccostr.idc_coste;
              UPDATE acumulado_c_coste SET debe = debe - OLD.debe, haber = haber -OLD.haber WHERE idc_coste = ccostr.idc_coste
AND idcuenta = ctar.idcuenta;
              SELECT INTO ctar * FROM cuenta WHERE idcuenta = ctar.padre;
            END LOOP;
            SELECT INTO ccostr * FROM c_coste WHERE idc_coste = ccostr.padre;
        END LOOP;
--      RAISE NOTICE '' disminuye_valor: Finaliza el algoritmo. '';
        RETURN NEW;
END;
'
    LANGUAGE plpgsql;


--
-- TOC entry 104 (OID 1346066)
-- Name: disminuye_valor1(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION disminuye_valor1() RETURNS "trigger"
    AS '
DECLARE
   cta    int4;
   ccost  int4;
   ctar   RECORD;
   ccostr RECORD;
BEGIN

        UPDATE cuenta SET debe = debe - OLD.debe, haber = haber - OLD.haber WHERE idcuenta = OLD.idcuenta;
        UPDATE c_coste SET debe = debe - OLD.debe, haber = haber - OLD.haber WHERE idc_coste = OLD.idc_coste;
        IF OLD.idcuenta IS NOT NULL THEN
           UPDATE acumulado_canal SET debe= debe - OLD.debe, haber =haber - OLD.haber WHERE idcuenta = OLD.idcuenta AND
idcanal = OLD.idcanal;
        END IF;
        cta := OLD.idcuenta;
        ccost := OLD.idc_coste;
--      RAISE NOTICE '' Se ha lanzado la funcion disminuye_valor()'';
        SELECT INTO ccostr * FROM c_coste WHERE idc_coste = ccost;
        WHILE FOUND LOOP
           SELECT INTO ctar * FROM cuenta WHERE idcuenta = cta;
           WHILE FOUND LOOP
--            RAISE NOTICE '' Cuenta % Centro Coste %'', ctar.idcuenta, ccostr.idc_coste;
              UPDATE acumulado_c_coste SET debe = debe - OLD.debe, haber = haber -OLD.haber WHERE idc_coste = ccostr.idc_coste
AND idcuenta = ctar.idcuenta;
              SELECT INTO ctar * FROM cuenta WHERE idcuenta = ctar.padre;
            END LOOP;
            SELECT INTO ccostr * FROM c_coste WHERE idc_coste = ccostr.padre;
        END LOOP;
--      RAISE NOTICE '' disminuye_valor: Finaliza el algoritmo. '';
        RETURN OLD;
END;
'
    LANGUAGE plpgsql;


--
-- TOC entry 105 (OID 1346067)
-- Name: creacuenta(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION creacuenta() RETURNS "trigger"
    AS '
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
'
    LANGUAGE plpgsql;


--
-- TOC entry 106 (OID 1346068)
-- Name: borracuenta(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION borracuenta() RETURNS "trigger"
    AS '
DECLARE
   mrecord RECORD;
BEGIN
      FOR mrecord IN SELECT * FROM canal LOOP
        DELETE FROM acumulado_canal WHERE idcuenta = OLD.idcuenta;
      END LOOP;
      FOR mrecord IN SELECT * FROM c_coste LOOP
         DELETE FROM acumulado_c_coste WHERE idcuenta=OLD.idcuenta;
      END LOOP;
        RETURN OLD;
END;
'
    LANGUAGE plpgsql;


--
-- TOC entry 107 (OID 1346069)
-- Name: creacanal(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION creacanal() RETURNS "trigger"
    AS '
DECLARE
   mrecord RECORD;
BEGIN
      FOR mrecord IN SELECT * FROM cuenta LOOP
         INSERT INTO acumulado_canal (idcuenta, idcanal,debe,haber) VALUES(mrecord.idcuenta, NEW.idcanal,0,0);
      END LOOP;
        RETURN NEW;
END;
'
    LANGUAGE plpgsql;


--
-- TOC entry 108 (OID 1346070)
-- Name: borracanal(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION borracanal() RETURNS "trigger"
    AS '
BEGIN
    DELETE FROM acumulado_canal WHERE idcanal = OLD.idcanal;
    RETURN OLD;
END;
'
    LANGUAGE plpgsql;


--
-- TOC entry 109 (OID 1346071)
-- Name: creaccoste(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION creaccoste() RETURNS "trigger"
    AS '
DECLARE
   mrecord RECORD;
BEGIN
      FOR mrecord IN SELECT * FROM cuenta LOOP
         INSERT INTO acumulado_c_coste (idcuenta, idc_coste,debe,haber) VALUES(mrecord.idcuenta, NEW.idc_coste,0,0);
      END LOOP;
        RETURN NEW;
END;
'
    LANGUAGE plpgsql;


--
-- TOC entry 110 (OID 1346072)
-- Name: borraccoste(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION borraccoste() RETURNS "trigger"
    AS '
BEGIN
    DELETE FROM acumulado_c_coste WHERE idc_coste = OLD.idc_coste;
    RETURN OLD;
END;
'
    LANGUAGE plpgsql;


--
-- TOC entry 111 (OID 1346073)
-- Name: propagaacumuladocuenta(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION propagaacumuladocuenta() RETURNS "trigger"
    AS '
DECLARE
   incdebe FLOAT8;
   inchaber FLOAT8;
BEGIN
   incdebe = NEW.debe - OLD.debe;
   inchaber = NEW.haber - OLD.haber;
--   RAISE NOTICE ''propagaacumuladocuenta %'', NEW.codigo;
   IF incdebe <> 0 OR inchaber <> 0 THEN
           UPDATE cuenta SET debe = debe + incdebe, haber = haber + inchaber WHERE idcuenta = NEW.padre;
   END IF;
   RETURN NEW;
END;
'
    LANGUAGE plpgsql;


--
-- TOC entry 112 (OID 1346074)
-- Name: propagaacumuladoccoste(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION propagaacumuladoccoste() RETURNS "trigger"
    AS '
DECLARE
   incdebe FLOAT8;
   inchaber FLOAT8;
BEGIN
   incdebe = NEW.debe - OLD.debe;
   inchaber = NEW.haber - OLD.haber;
   IF incdebe <> 0 OR inchaber <> 0 THEN
           UPDATE c_coste SET debe = debe + incdebe, haber = haber + inchaber WHERE idc_coste = OLD.padre;
   END IF;
   RETURN NEW;
END;
'
    LANGUAGE plpgsql;


--
-- TOC entry 113 (OID 1346075)
-- Name: acumulados_canal(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION acumulados_canal() RETURNS "trigger"
    AS '
DECLARE
   incdebe  FLOAT8;
   inchaber FLOAT8;
   cuentar  RECORD;
BEGIN
--   RAISE NOTICE '' Ha entrado el trigger acumulados_canal() '';
--   RAISE NOTICE '' idcuenta % idcanal %'', NEW.idcuenta, NEW.idcanal;
   incdebe = NEW.debe - OLD.debe;
   inchaber = NEW.haber - OLD.haber;
   SELECT INTO cuentar * FROM cuenta WHERE idcuenta = NEW.idcuenta;
   SELECT INTO cuentar * FROM cuenta WHERE idcuenta = cuentar.padre;
   IF FOUND THEN
      UPDATE acumulado_canal SET debe = debe + incdebe, haber = haber + inchaber WHERE idcuenta = cuentar.idcuenta AND idcanal
= NEW.idcanal;
   END IF;
   RETURN NEW;
END;
'
    LANGUAGE plpgsql;


--
-- TOC entry 48 (OID 1346076)
-- Name: pga_graphs; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE pga_graphs (
    graphname character varying(64) NOT NULL,
    graphsource text,
    graphcode text
);


--
-- TOC entry 49 (OID 1346076)
-- Name: pga_graphs; Type: ACL; Schema: public; Owner: postgres
--

REVOKE ALL ON TABLE pga_graphs FROM PUBLIC;
GRANT ALL ON TABLE pga_graphs TO PUBLIC;


--
-- TOC entry 50 (OID 1346081)
-- Name: pga_images; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE pga_images (
    imagename character varying(64) NOT NULL,
    imagesource text
);


--
-- TOC entry 51 (OID 1346081)
-- Name: pga_images; Type: ACL; Schema: public; Owner: postgres
--

REVOKE ALL ON TABLE pga_images FROM PUBLIC;
GRANT ALL ON TABLE pga_images TO PUBLIC;


--
-- TOC entry 52 (OID 1346086)
-- Name: pga_diagrams; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE pga_diagrams (
    diagramname character varying(64) NOT NULL,
    diagramtables text,
    diagramlinks text
);


--
-- TOC entry 53 (OID 1346086)
-- Name: pga_diagrams; Type: ACL; Schema: public; Owner: postgres
--

REVOKE ALL ON TABLE pga_diagrams FROM PUBLIC;
GRANT ALL ON TABLE pga_diagrams TO PUBLIC;


--
-- TOC entry 54 (OID 1346091)
-- Name: pga_layout; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE pga_layout (
    tablename character varying(64) NOT NULL,
    nrcols smallint,
    colnames text,
    colwidth text
);


--
-- TOC entry 55 (OID 1346091)
-- Name: pga_layout; Type: ACL; Schema: public; Owner: postgres
--

REVOKE ALL ON TABLE pga_layout FROM PUBLIC;
GRANT ALL ON TABLE pga_layout TO PUBLIC;


--
-- TOC entry 114 (OID 1346096)
-- Name: abreasiento(integer); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION abreasiento(integer) RETURNS integer
    AS 'DECLARE
        id_asiento ALIAS FOR $1;
BEGIN
        DELETE FROM apunte WHERE idasiento=id_asiento;
        RETURN 1;
END;
'
    LANGUAGE plpgsql;


--
-- TOC entry 115 (OID 1346097)
-- Name: cierraasiento(integer); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION cierraasiento(integer) RETURNS integer
    AS 'DECLARE
        id_asiento ALIAS FOR $1;
        mrecord RECORD;
BEGIN
        DELETE FROM apunte WHERE idasiento=id_asiento;
         -- // Linia Afegida per Josep B.

        FOR mrecord IN SELECT * from borrador WHERE idasiento=id_asiento LOOP
                INSERT INTO apunte (codigoborrador, idasiento, iddiario, fecha, conceptocontable, idcuenta, descripcion, debe,
haber, contrapartida, comentario, idcanal, marcaconciliacion, idc_coste, idtipoiva, orden) VALUES (mrecord.codigoborrador,
mrecord.idasiento, mrecord.iddiario, mrecord.fecha, mrecord.conceptocontable, mrecord.idcuenta, mrecord.descripcion,
mrecord.debe, mrecord.haber, mrecord.contrapartida, mrecord.comentario, mrecord.idcanal, mrecord.marcaconciliacion,
mrecord.idc_coste, mrecord.idtipoiva, mrecord.orden);
        END LOOP;

-- Cuando cerramos el asiento, tambien recalculamos todas las contrapartidas.
-- Si hay una contrapartida puesta en el borrador, entonces esa no se tocara.
-- De esta forma agilizamos cálculos y siemrpe en la tabla de apuntes tenemos las contrapartidas puestas.
        UPDATE apunte set contrapartida = ccontrapartida(idapunte) WHERE idasiento= id_asiento;
        RETURN 1;
END;

'
    LANGUAGE plpgsql;


--
-- TOC entry 56 (OID 1346100)
-- Name: amortizacion; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE amortizacion (
    idamortizacion serial NOT NULL,
    idcuentaactivo integer,
    idcuentaamortizacion integer,
    descamortizacion character varying(2000),
    nomamortizacion character varying(200),
    fechacompra date,
    fecha1cuota date,
    valorcompra double precision,
    periodicidad integer,
    numcuotas integer,
    metodo integer,
    nifproveedor character varying(12),
    nomproveedor character varying(150),
    dirproveedor character varying(200),
    telproveedor character varying(20),
    agrupacion character varying(150)
);


--
-- TOC entry 57 (OID 1346108)
-- Name: linamortizacion; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE linamortizacion (
    idlinamortizacion serial NOT NULL,
    idamortizacion integer,
    idasiento integer REFERENCES asiento(idasiento),
    ejercicio integer,
    fechaprevista date,
    cantidad double precision
);


--
-- TOC entry 58 (OID 1346111)
-- Name: ejercicios; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE ejercicios (
    ejercicio integer,
    periodo smallint,
    bloqueado boolean
);


--
-- TOC entry 116 (OID 1346113)
-- Name: reordenaasientos(integer); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION reordenaasientos(integer) RETURNS integer
    AS '
DECLARE
    ejercicio ALIAS FOR $1;
    as RECORD;
    cont integer;
BEGIN
    cont := 1;
    FOR as IN SELECT * from asiento WHERE EXTRACT(YEAR FROM fecha)=ejercicio ORDER BY fecha,ordenasiento LOOP
        IF (cont <> as.ordenasiento) THEN
            UPDATE asiento SET ordenasiento = cont WHERE idasiento = as.idasiento;
        END IF;
        cont := cont + 1;
    END LOOP;
    RETURN 0;
END;
'    LANGUAGE plpgsql;


--
-- TOC entry 116 (OID 1346113)
-- Name: reordenaasientosall(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION reordenaasientosall() RETURNS integer
    AS '
DECLARE
    as RECORD;
    ejercicio integer;
BEGIN
    FOR as IN SELECT DISTINCT EXTRACT (YEAR  FROM FECHA) AS ano FROM asiento ORDER BY ano LOOP
        ejercicio := as.ano;
        PERFORM reordenaasientos(ejercicio);
    END LOOP;
    RETURN 0;
END;
'    LANGUAGE plpgsql;




-- **********************************************************************
-- APARTADO DE COMPROBACIONES DE INTEGRIDAD EXTRA Y DETECCIÓN DE ERRORES.
-- **********************************************************************
-- **********************************************************************

-- Las comprobaciones saltan como disparadores y abortan la ejecución si no se cumplen las restricciones.
--DROP TRIGGER restriccionescuentatrigger ON cuenta CASCADE;
--DROP FUNCTION restriccionescuenta();


CREATE FUNCTION restriccionescuenta () RETURNS "trigger"
AS '
DECLARE
BEGIN
        IF NEW.codigo = '''' THEN
                RAISE EXCEPTION '' No se puede dejar el código de cuenta vacio '';
        END IF;
	IF NEW.descripcion = '''' THEN
		RAISE EXCEPTION '' Nombre de cuenta vacio '';
	END IF;
	IF NEW.nombreent_cuenta = '''' THEN
		NEW.nombreent_cuenta = NEW.descripcion;
	END IF;
        RETURN NEW;
END;
' LANGUAGE plpgsql;


CREATE TRIGGER restriccionescuentatrigger
    BEFORE INSERT OR UPDATE ON cuenta
    FOR EACH ROW
    EXECUTE PROCEDURE restriccionescuenta();


CREATE FUNCTION restriccionesborrador () RETURNS "trigger"
AS '
DECLARE
        cta RECORD;
	ej RECORD;
BEGIN
        SELECT INTO cta * FROM cuenta WHERE idcuenta = NEW.idcuenta;
	IF FOUND THEN
                IF cta.bloqueada THEN
                        RAISE EXCEPTION '' Cuenta bloqueada, no se puede utilizar esta cuenta '';
                END IF;

                IF cta.nodebe THEN
                        IF NEW.debe <> 0 THEN
                                RAISE EXCEPTION '' Cuenta bloqueada por debe, solo permite haber '';
                        END IF;
                END IF;
                IF cta.nohaber THEN
                        IF NEW.haber <> 0 THEN
                                RAISE EXCEPTION '' Cuenta bloqueada por haber, solo permite debe '';
                        END IF;
                END IF;
	ELSE
		RAISE EXCEPTION '' Cuenta inexistente '';
        END IF;
	
	
	SELECT INTO  ej  * FROM ejercicios WHERE ejercicio = EXTRACT (YEAR FROM NEW.fecha) AND periodo =0;
	IF FOUND THEN
		IF ej.bloqueado = TRUE THEN
			RAISE EXCEPTION '' Periodo bloqueado '';
		END IF;
	ELSE
		RAISE EXCEPTION '' Ejercicio Inexistente'';
	END IF;
	SELECT INTO  ej * FROM ejercicios WHERE ejercicio = EXTRACT (YEAR FROM NEW.fecha) AND periodo = EXTRACT (MONTH FROM NEW.fecha);
	IF ej.bloqueado = TRUE THEN
		RAISE EXCEPTION '' Periodo bloqueado '';
	END IF;
	
	
        RETURN NEW;
END;
' LANGUAGE plpgsql;


CREATE TRIGGER restriccionesborradortrigger
    BEFORE INSERT OR UPDATE ON borrador
    FOR EACH ROW
    EXECUTE PROCEDURE restriccionesborrador();

    
    
CREATE FUNCTION restriccionesasiento () RETURNS "trigger"
AS '
DECLARE
	ej RECORD;
BEGIN
	SELECT INTO  ej  * FROM ejercicios WHERE ejercicio = EXTRACT (YEAR FROM NEW.fecha) AND periodo =0;
	IF FOUND THEN
		IF ej.bloqueado = TRUE THEN
			RAISE EXCEPTION '' Periodo bloqueado '';
		END IF;
	ELSE
		RAISE EXCEPTION '' Ejercicio Inexistente'';
	END IF;
	SELECT INTO  ej * FROM ejercicios WHERE ejercicio = EXTRACT (YEAR FROM NEW.fecha) AND periodo = EXTRACT (MONTH FROM NEW.fecha);
	IF ej.bloqueado = TRUE THEN
		RAISE EXCEPTION '' Periodo bloqueado '';
	END IF;
	RETURN NEW;
END;
' LANGUAGE plpgsql;

CREATE TRIGGER restriccionesasientotrigger
   BEFORE INSERT OR UPDATE ON asiento
   FOR EACH ROW
   EXECUTE PROCEDURE restriccionesasiento();
    
-- ******************************************************
-- FIN DEL APARTADO DE COMPROBACIONES.
-- ******************************************************
-- ******************************************************



--
-- TOC entry 186 (OID 1358703)
-- Name: nuevo_apunte; Type: TRIGGER; Schema: public; Owner: postgres
--

CREATE TRIGGER nuevo_apunte
    AFTER INSERT OR UPDATE ON apunte
    FOR EACH ROW
    EXECUTE PROCEDURE aumenta_valor();


--
-- TOC entry 187 (OID 1358704)
-- Name: nuevo_apunte1; Type: TRIGGER; Schema: public; Owner: postgres
--

CREATE TRIGGER nuevo_apunte1
    BEFORE UPDATE ON apunte
    FOR EACH ROW
    EXECUTE PROCEDURE disminuye_valor();


--
-- TOC entry 188 (OID 1358705)
-- Name: nuevo_apunte2; Type: TRIGGER; Schema: public; Owner: postgres
--

CREATE TRIGGER nuevo_apunte2
    BEFORE DELETE ON apunte
    FOR EACH ROW
    EXECUTE PROCEDURE disminuye_valor1();


--
-- TOC entry 144 (OID 1358706)
-- Name: nueva_cuenta; Type: TRIGGER; Schema: public; Owner: postgres
--

CREATE TRIGGER nueva_cuenta
    AFTER INSERT ON cuenta
    FOR EACH ROW
    EXECUTE PROCEDURE creacuenta();


--
-- TOC entry 143 (OID 1358707)
-- Name: borra_cuenta; Type: TRIGGER; Schema: public; Owner: postgres
--

CREATE TRIGGER borra_cuenta
    BEFORE DELETE ON cuenta
    FOR EACH ROW
    EXECUTE PROCEDURE borracuenta();


--
-- TOC entry 153 (OID 1358708)
-- Name: nuevo_canal; Type: TRIGGER; Schema: public; Owner: postgres
--

CREATE TRIGGER nuevo_canal
    AFTER INSERT ON canal
    FOR EACH ROW
    EXECUTE PROCEDURE creacanal();


--
-- TOC entry 152 (OID 1358709)
-- Name: borra_canal; Type: TRIGGER; Schema: public; Owner: postgres
--

CREATE TRIGGER borra_canal
    BEFORE DELETE ON canal
    FOR EACH ROW
    EXECUTE PROCEDURE borracanal();


--
-- TOC entry 164 (OID 1358710)
-- Name: nuevo_ccoste; Type: TRIGGER; Schema: public; Owner: postgres
--

CREATE TRIGGER nuevo_ccoste
    AFTER INSERT ON c_coste
    FOR EACH ROW
    EXECUTE PROCEDURE creaccoste();


--
-- TOC entry 163 (OID 1358711)
-- Name: borra_ccoste; Type: TRIGGER; Schema: public; Owner: postgres
--

CREATE TRIGGER borra_ccoste
    BEFORE DELETE ON c_coste
    FOR EACH ROW
    EXECUTE PROCEDURE borraccoste();


--
-- TOC entry 145 (OID 1358712)
-- Name: propaga_acumulado_cuenta; Type: TRIGGER; Schema: public; Owner: postgres
--

CREATE TRIGGER propaga_acumulado_cuenta
    AFTER UPDATE ON cuenta
    FOR EACH ROW
    EXECUTE PROCEDURE propagaacumuladocuenta();


--
-- TOC entry 165 (OID 1358713)
-- Name: propaga_acumulado_ccoste; Type: TRIGGER; Schema: public; Owner: postgres
--

CREATE TRIGGER propaga_acumulado_ccoste
    AFTER UPDATE ON c_coste
    FOR EACH ROW
    EXECUTE PROCEDURE propagaacumuladoccoste();


--
-- TOC entry 170 (OID 1358714)
-- Name: acumulados_canal_fk; Type: TRIGGER; Schema: public; Owner: postgres
--

CREATE TRIGGER acumulados_canal_fk
    AFTER UPDATE ON acumulado_canal
    FOR EACH ROW
    EXECUTE PROCEDURE acumulados_canal();


--
-- TOC entry 3 (OID 2200)
-- Name: SCHEMA public; Type: COMMENT; Schema: -; Owner: postgres
--

COMMENT ON SCHEMA public IS 'Standard public schema';


