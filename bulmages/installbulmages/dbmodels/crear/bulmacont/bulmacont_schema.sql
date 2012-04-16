-- -------------------------------------------------------------------------------------------
-- (C) Tomeu Borras Riera. <tborras@conetxia.com>
--
--   This program is free software; you can redistribute it and/or modify
--   it under the terms of the GNU General Public License as published by
--   the Free Software Foundation; either version 2 of the License, or
--   (at your option) any later version.
--
--   This program is distributed in the hope that it will be useful,
--   but WITHOUT ANY WARRANTY; without even the implied warranty of
--   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--   GNU General Public License for more details.
--
--   You should have received a copy of the GNU General Public License
--   along with this program; if not, write to the
--   Free Software Foundation, Inc.,
--   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
-- -------------------------------------------------------------------------------------------
-- Usage:    psql database_name < bulmacont_schema.sql
-- -------------------------------------------------------------------------------------------

\echo '********* INICIADO FICHERO DE ESTRUCTURA DE LA BASE DE DATOS DE BULMACONT *********'

--
-- PostgreSQL database definition of BulmaCont
-- Creamos el lenguaje plpgsql y lo preparamos para ser usado con toda la base de datos.
\echo -n ':: '
BEGIN;

\echo ':: Establecemos los mensajes minimos a avisos y otros parametros ... '
\echo -n ':: '
SET client_min_messages TO warning;
\echo -n ':: '
SET client_encoding = 'UNICODE';
\echo -n ':: '
SET check_function_bodies = false;
\echo -n ':: '
SET search_path = public, pg_catalog;


\echo -n ':: plpgsql_call_handler ... '
CREATE OR REPLACE FUNCTION plpgsql_call_handler() RETURNS language_handler
    AS '$libdir/plpgsql', 'plpgsql_call_handler'
    LANGUAGE c;

\echo -n ':: Establecemos el lenguaje de procedimientos ... '

-- DROP PROCEDURAL LANGUAGE IF EXISTS plpgsql CASCADE;
-- CREATE TRUSTED PROCEDURAL LANGUAGE plpgsql HANDLER plpgsql_call_handler;

CREATE OR REPLACE FUNCTION create_language_plpgsql()
 RETURNS BOOLEAN AS $$
     CREATE LANGUAGE plpgsql;
     SELECT TRUE;
 $$ LANGUAGE SQL;

SELECT CASE WHEN NOT
     (
         SELECT  TRUE AS exists
         FROM    pg_language
         WHERE   lanname = 'plpgsql'
         UNION
         SELECT  FALSE AS exists
         ORDER BY exists DESC
         LIMIT 1
     )
 THEN
     create_language_plpgsql()
 ELSE
     FALSE
 END AS plpgsql_created;

DROP FUNCTION create_language_plpgsql();


\echo -n ':: '
REVOKE ALL ON SCHEMA public FROM PUBLIC;
\echo -n ':: '
GRANT ALL ON SCHEMA public TO PUBLIC;


-- La tabla de configuracion.
-- En esta tabla se guardan parametros que el programa va a utilizar.
-- Como por ejemplo el numero de digitos por defecto de las cuentas o el asiento inteligente que se enlaza con
-- facturacion.
-- Tiene tres campos:
-- idconfiguracion = El identificador (No tiene ningun uso especial).
-- nombre = El nombre del parametro de configuracion.
-- valor = El valor que toma dicho parametro.
\echo -n ':: Configuracion ... '
CREATE TABLE configuracion (
    nombre character varying(25) PRIMARY KEY,
    valor character varying(350)
);


-- ** pais **
-- cod2pais: Codigo internacional de dos digitos.
-- cod3pais: Codigo internacional de tres digitos.
-- descpais: Cescripcion del pais.
\echo -n ':: Pais ... '
CREATE TABLE pais (
    idpais serial PRIMARY KEY,
    cod2pais character varying(2),
    cod3pais character varying(3),
    descpais character varying(50)
);

-- -- 
-- ** provincia **
\echo -n ':: Provincia ... '
CREATE TABLE provincia (
    idprovincia SERIAL PRIMARY KEY,
    idpais integer REFERENCES pais(idpais),
    provincia character varying(500)
);

-- La tabla canal refleja los canales a los que puede pertenecer un apunte determinado.
\echo -n ':: Canal ... '
CREATE TABLE canal (
    idcanal serial PRIMARY KEY,
    descripcion character varying(100),
    nombre character varying(50)
);


\echo -n ':: Centro de coste ... '
CREATE TABLE c_coste (
    idc_coste serial PRIMARY KEY,
    descripcion character varying(100),
    nombre character varying(50) NOT NULL,
    codigo character(3),
    padre integer REFERENCES c_coste(idc_coste),
    imputacion boolean,
    debe numeric(12, 2) DEFAULT 0,
    haber numeric(12, 2) DEFAULT 0
);

\echo -n ':: Centro de coste distribuido ... '
CREATE TABLE c_costedist (
	idc_costedist serial PRIMARY KEY,
	idc_coste     integer NOT NULL REFERENCES c_coste(idc_coste),
	iddestc_coste integer NOT NULL REFERENCES c_coste(idc_coste),
	porcentc_costedist numeric(12,2) DEFAULT 0
);

-- La tabla cuenta es la que presenta el plan contable.
-- Principalmente define el arbol de cuentas, tiene varios campos de SOLO LECTURA que sirven para acumulados.
-- Los campos son:
-- idcuenta = El identificador de la cuenta. Tipo Serial.
-- codigo = El codigo de la cuenta.
-- descripcion = Descripcion de la cuenta.
-- imputacion = Booleano que indica si la cuenta es de imputacion o no (aun no he descubierto para que sirve).
-- padre = Este campo indica el idcuenta padre de la cuenta que tratamos. Es un apuntador al indice de la tabla
--         lo que la convierte en arbol.
-- bloqueada = Este boleano indica si la cuenta esta bloqueada o no.
-- msg = ?????
-- debe = Este campo es de solo lectura e indica el acumulado en debe de la cuenta.
-- haber = Este campo es de solo lectura e indica el acumulado en haber de la cuenta.
-- nodebe = Este booleano indica si la cuenta puede tener inserciones en el debe o no.
-- nohaber = Este booleano indica si la cuenta puede tener inserciones en el haber o no.
-- regularizacion = Este booleano indica si la cuenta es de regularizacion o no.
-- activo = Este campo indica si la cuenta es de activo o de pasivo.
-- nombreent_cuenta = Si hay una entidad ligada a la cuenta aqui ponemos su nombre.
-- cifent_cuenta = Si hay una entidad ligada a la cuenta aqui ponemos su CIF.
-- dirent_cuenta = Direccion de una entidad ligada con la cuenta
-- cpent_cuenta = Codigo Postal de una entidad ligada con la cuenta
-- telent_cuenta = Telefono de una entidad ligada con la cuenta.
-- coment_cuenta = Comentarios de una entidad ligada con la cuenta.
-- bancoent_cuenta = Cuenta bancaria de una entidad ligada con la cuenta.
-- emailent_cuenta = Direccion de correo ligada con la entidad
-- webent_cuenta = Pagina web ligada con la entidad
-- tipocuenta = Campo de tipo integer que en realidad es un ENUM (para los valores que ahora no recuerdo y falta
--              rellenar) ??????
\echo -n ':: Cuenta ... '
CREATE TABLE cuenta (
    idcuenta serial PRIMARY KEY,
    codigo character varying(12) NOT NULL,
    descripcion character varying(500) NOT NULL,
    imputacion boolean NOT NULL DEFAULT TRUE,
    padre integer REFERENCES cuenta(idcuenta),
    bloqueada boolean NOT NULL DEFAULT FALSE,
    msg character varying,
    debe numeric(12, 2) NOT NULL DEFAULT 0,
    haber numeric(12, 2) NOT NULL DEFAULT 0,
    nodebe boolean NOT NULL DEFAULT FALSE,
    nohaber boolean NOT NULL DEFAULT FALSE,
    regularizacion boolean,
    activo boolean,
    nombreent_cuenta character varying(500),
    cifent_cuenta character varying(12),
    cpent_cuenta character varying(5),
    dirent_cuenta character varying(80),
    telent_cuenta character varying(20),
    coment_cuenta character varying,
    bancoent_cuenta character varying(30),
    emailent_cuenta character varying(50),
    webent_cuenta character varying(150),
    tipocuenta integer DEFAULT 1,
    idc_coste integer REFERENCES c_coste (idc_coste),
    idpais INTEGER REFERENCES pais (idpais),
    idprovincia INTEGER REFERENCES provincia (idprovincia),
    poblacion CHARACTER VARYING(150)
);




\echo -n ':: Acumulado centro de coste ... '
CREATE TABLE acumulado_c_coste (
    idacumulado_c_coste serial PRIMARY KEY,
    idcuenta integer NOT NULL REFERENCES cuenta(idcuenta),
    idc_coste integer NOT NULL REFERENCES c_coste(idc_coste),
    debe numeric(12, 2) DEFAULT 0,
    haber numeric(12, 2) DEFAULT 0
);

\echo -n ':: Acumulado canal ... '
CREATE TABLE acumulado_canal (
    idacumulado_canal serial PRIMARY KEY,
    idcuenta integer NOT NULL REFERENCES cuenta(idcuenta),
    idcanal integer NOT NULL REFERENCES canal(idcanal),
    debe numeric(12, 2) DEFAULT 0,
    haber numeric(12, 2) DEFAULT 0
);

\echo -n ':: Diario (La tabla diario esta en desuso, aunque de momento se crea.) ... '
CREATE TABLE diario (
    iddiario serial PRIMARY KEY,
    descripcion character varying(100)
);

-- El campo ordenasiento se puede dejar en nulo solo porque luego el trigger lo reasigna. Con un default
-- sera ma adecuado pero no se como se implementa un default tan complicado.
\echo -n ':: Asiento ... '
CREATE TABLE asiento (
    idasiento serial PRIMARY KEY,
    descripcion character varying(100),
    fecha date,
    comentariosasiento character varying(2000),
    ordenasiento integer,
    clase smallint DEFAULT 0
);

\echo -n ':: Archivo documental ... '
CREATE TABLE adocumental (
    idadocumental serial PRIMARY KEY,
    idasiento integer REFERENCES asiento(idasiento),
    descripcionadocumental character varying(200),
    fechaintadocumental date,
    fechaasadocumental date,
    archivoadocumental character varying(300)
);

\echo -n ':: Apunte ... '
CREATE TABLE apunte (
    idapunte serial PRIMARY KEY,
    codigoborrador integer,
    idasiento integer NOT NULL REFERENCES asiento(idasiento),
    iddiario integer REFERENCES diario(iddiario),
    fecha date,
    conceptocontable character varying(50),
    idcuenta integer NOT NULL REFERENCES cuenta(idcuenta),
    descripcion character varying(100),
    debe numeric(12,2) NOT NULL DEFAULT 0,
    haber numeric(12,2) NOT NULL DEFAULT 0,
    contrapartida integer REFERENCES cuenta(idcuenta),
    comentario character varying(100),
    idcanal integer REFERENCES canal(idcanal),
    marcaconciliacion character(12),
    idc_coste integer REFERENCES c_coste(idc_coste),
    idtipoiva integer,
    ordenapunte integer,
    punteo boolean DEFAULT false
);

\echo -n ':: Borrador ... '
CREATE TABLE borrador (
    idborrador serial PRIMARY KEY,
    codigoborrador integer,
    idasiento integer NOT NULL REFERENCES asiento(idasiento),
    iddiario integer REFERENCES diario(iddiario),
    fecha date,
    conceptocontable character varying(50),
    idcuenta integer NOT NULL REFERENCES cuenta(idcuenta),
    descripcion character varying(100),
    debe numeric(12,2) NOT NULL DEFAULT 0,
    haber numeric(12,2) NOT NULL DEFAULT 0,
    contrapartida integer REFERENCES cuenta(idcuenta),
    comentario character varying(100),
    idcanal integer REFERENCES canal(idcanal),
    marcaconciliacion character(12),
    idc_coste integer REFERENCES c_coste(idc_coste),
-- El campo idapunte no se utiliza, existe la combinacion idasiento, orden que lo sustituye.
    idapunte integer,
    idtipoiva integer,
    ordenborrador integer
);


-- La tabla fpago lleva las formas de pago existentes
-- idfpago = Identificador de la forma de pago.
-- nomfpago = Nombre de la forma de pago.
-- nplazosfpago = Numero de plazos de la forma de pago.
-- plazoprimerpagofpago = El plazo hasta el primer cobro/pago.
-- tipoplazoprimerpagofpago = Tipo del plazo de la forma de pago:
--                            0 - dias
--                            1 - semanas
--                            2 - meses
--                            3 - anyos
-- plazoentrerecibofpago = Numero de plazos en los siguientes recibos.
-- tipoplazoentrerecibofpago = (Igual que tipoplazoprimerpagofpago).
\echo -n ':: Forma de pago ... '
CREATE TABLE fpago (
    idfpago serial PRIMARY KEY,
    nomfpago character varying(50),
    nplazosfpago integer,
    plazoprimerpagofpago integer,
    tipoplazoprimerpagofpago integer,
    plazoentrerecibofpago integer,
    tipoplazoentrerecibofpago integer
);


-- La tabla de tipos de IVA indica los tipos de iva que se soportan.
-- Sirve para saber si un IVA determinado es correcto o no.
-- Esta en forma de tabla y no en otra forma debido a que los porcentajes de IVA podrian cambiar.
-- Los campos son:
-- idtipoiva = Identificador de la tabla.
-- nombretipoiva = Da un nombre al tipo de IVA.
-- Porcentajetipoiva = El porcentaje que corresponde con este tipo.
\echo -n ':: Tipo de IVA ... '
CREATE TABLE tipoiva (
    idtipoiva serial PRIMARY KEY,
    nombretipoiva character varying(25) UNIQUE,
    porcentajetipoiva numeric(12, 2),
    idcuenta integer NOT NULL REFERENCES cuenta(idcuenta)
);


-- ffactura = Fecha de la factura.
-- iva = Es un campo de solo lectura que se actualiza con los valores del IVA.
-- factemitida = Indica si es una factura emitida o recibida.
\echo -n ':: Registro de IVA ... '
CREATE TABLE registroiva (
    idregistroiva serial PRIMARY KEY,
    contrapartida integer REFERENCES cuenta(idcuenta),
    baseimp numeric(12, 2),
    iva numeric(12, 2), -- De solo lectura.
    ffactura date,
    femisionregistroiva date,
    factura character varying(70),
    serieregistroiva character varying(40),
    idborrador integer NOT NULL REFERENCES borrador(idborrador),
    incregistro boolean,
    regularizacion boolean,
    plan349 boolean,
    numorden character varying(50),
    cif character varying(25),
    idfpago integer REFERENCES fpago(idfpago),
    factemitida boolean NOT NULL,
    rectificaaregistroiva integer REFERENCES registroiva(idregistroiva)
);


-- La tabla prevcobro es prevision de cobros / pagos para facturas.
-- Esta tabla proporciona las formas de pago de cada factura que se pasa a un cliente o que recibimos de un proveedor.
-- tipoprevcobro = Indica si es un cobro (true) o un pago (false).
-- fpagoprevcobro = Es un identificador a la otra tabla de Formas de Pago.
-- idcuenta = La cuenta sobre la que se hara el cobro / pago.
-- idasiento = Asiento de cobro (Si esta hecho).
-- cantidadprevistaprevcobro = Cantidad prevista del cobro.
-- cantidadprevcobro = Cantidad cobrada.
-- fprevistaprevcobro = Fecha en que se prevee el cobro / pago.
-- fcobroprevcobro = Fecha en que se ha realizado el cobro / pago.
-- idregistroiva = Registro de IVA con el que se corresponde, o factura con la que se corresponde.
-- tipoprevcobro = Indica si es un cobro o un pago. TRUE --> COBRO FALSE -->PAGO.
\echo -n ':: Prevision de cobro ... '
CREATE TABLE prevcobro (
    idprevcobro serial PRIMARY KEY,
    fprevistaprevcobro date,
    fcobroprevcobro date,
    idctacliente integer REFERENCES cuenta(idcuenta),
    idfpago integer REFERENCES fpago(idfpago),
    idcuenta integer REFERENCES cuenta(idcuenta),
    idasiento integer REFERENCES asiento(idasiento),
    cantidadprevistaprevcobro numeric(12, 2) DEFAULT 0,
    cantidadprevcobro  numeric(12, 2) DEFAULT 0,
    idregistroiva integer REFERENCES registroiva(idregistroiva),
    tipoprevcobro boolean NOT NULL DEFAULT FALSE,
    docprevcobro character varying(50)
);

-- Esta tabla especifica para cada factura los IVAs correspondientes.
\echo -n ':: IVA ... '
CREATE TABLE iva (
   idiva serial PRIMARY KEY,
   idtipoiva integer NOT NULL REFERENCES tipoiva (idtipoiva),
   idregistroiva integer NOT NULL REFERENCES registroiva(idregistroiva),
   baseiva numeric(12, 2) DEFAULT 0,
   ivaiva numeric(12, 2) DEFAULT 0
);

\echo -n ':: Asiento inteligente ... '
CREATE TABLE ainteligente (
    idainteligente serial PRIMARY KEY,
    descripcion character varying(100),
    comentariosasiento character varying(2000)
);

\echo -n ':: Linea de asientos inteligentes (Pte. borrar)... '
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

\echo -n ':: Balance ... '
CREATE TABLE balance (
    idbalance serial PRIMARY KEY,
    nombrebalance character varying(150)
);


-- La casilla tipomasapatrimonial:
-- 0.- normal, presentar siempre
-- 1.- Solo presentar si el saldo es > 0,
-- 2.- Solo presentar si el saldo es < 0,
-- 3.- Presentar en valor absoluto.
\echo -n ':: Masa patrimonial ... '
CREATE TABLE mpatrimonial (
    idmpatrimonial serial PRIMARY KEY,
    idbalance integer REFERENCES balance(idbalance),
    descmpatrimonial character varying(150),
    orden integer,
    tabulacion integer,
    saldo numeric(12, 2),
    opdesc integer,
    tipompatrimonial integer DEFAULT 0
);


-- La casilla tipocompmasap:
-- 0.- normal,
-- 1.- Solo presentar si el saldo es > 0,
-- 2.- Solo presentar si el saldo es < 0.
\echo -n ':: Componente de masa patrimonial ... '
CREATE TABLE compmasap (
    idcompmasap serial PRIMARY KEY,
    idcuenta integer REFERENCES cuenta(idcuenta),
    idmpatrimonial integer,
    masaperteneciente integer,
    saldo numeric(12, 2),
    signo boolean,
    tipocompmasap integer DEFAULT 0,
    nombre character varying(150)
);


\echo -n ':: Componente de balance ... '
CREATE TABLE compbalance (
    idcompbalance serial PRIMARY KEY,
    idbalance integer NOT NULL REFERENCES balance(idbalance),
    idmpatrimonial integer,
    concepto character varying(150),
    orden integer,
    tabulacion integer
);

\echo -n ':: Amortizacion ... '
CREATE TABLE amortizacion (
    idamortizacion serial NOT NULL,
    idcuentaactivo integer,
    idcuentaamortizacion integer,
    descamortizacion character varying(2000),
    nomamortizacion character varying(200),
    fechacompra date,
    fecha1cuota date,
    valorcompra numeric(12, 2),
    periodicidad integer,
    numcuotas integer,
    metodo integer,
    nifproveedor character varying(12),
    nomproveedor character varying(150),
    dirproveedor character varying(200),
    telproveedor character varying(20),
    agrupacion character varying(150)
);


\echo -n ':: Linea de amortizacion ... '
CREATE TABLE linamortizacion (
    idlinamortizacion serial NOT NULL,
    idamortizacion integer,
    idasiento integer REFERENCES asiento(idasiento),
    ejercicio integer,
    fechaprevista date,
    cantidad numeric(12, 2)
);


\echo -n ':: Ejercicios ... '
CREATE TABLE ejercicios (
    ejercicio integer,
    periodo smallint,
    bloqueado boolean
);


\echo -n ':: Funcion abre asientos ... '
CREATE FUNCTION abreasientos() RETURNS integer
    AS '
--DECLARE rs RECORD;
--DECLARE rs2 RECORD;
BEGIN
--    FOR rs IN SELECT * FROM asiento ORDER BY idasiento DESC LOOP
--      SELECT INTO rs2 cambiaasiento(rs.idasiento, rs.idasiento * 3);
--    END LOOP;
--  Abrir los asientos es modificar el campo ordenasiento de los mismos para que se reorganicen
    UPDATE asiento SET ordenasiento = ordenasiento * 3;
    RETURN 0;
END;
'
    LANGUAGE plpgsql;


\echo -n ':: Funcion cambia asiento ... '
CREATE FUNCTION cambiaasiento(integer, integer) RETURNS integer
    AS '
DECLARE
    idinicial ALIAS FOR $1;
    idfinal ALIAS FOR $2;
    rs RECORD;
BEGIN
    -- Esta funcion cambia un asiento de sitio, el asiento inicial y lo pone en el asiento final
    -- El problema es que si el asiento final debe estar vacio.
    SELECT INTO rs * FROM asiento WHERE idasiento = idinicial;
    IF FOUND THEN
        INSERT INTO asiento (idasiento, fecha, descripcion, comentariosasiento) VALUES (idfinal, rs.fecha, rs.descripcion, rs.comentariosasiento);
        UPDATE borrador SET idasiento = idfinal WHERE idasiento = idinicial;
        UPDATE apunte SET idasiento = idfinal WHERE idasiento = idinicial;
        DELETE FROM asiento WHERE idasiento = idinicial;
    END IF;
    RETURN 0;
END;
'
    LANGUAGE plpgsql;


-- La contrapartida de un apunte donde todo son ceros es la misma que el apunte.
\echo -n ':: Funcion calcular contrapartida ... '
CREATE FUNCTION ccontrapartida(integer)
  RETURNS integer AS '
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
'  LANGUAGE plpgsql;

    
-- Creo que haciendo una funcion que cierre todas las contrapartidas de un asiento dado tendria
-- mejores rendimientos que mediante las funciones particulares y al cerrar el asiento se pueden 
-- Recalcular todos los cierres.
--
-- Recorremos el asiento en una unica pasada y almacenamos contrapartidas de paso (maximas) al llegar a un punto de
-- descuadre cero asignamos contrapartidas e inicializamos marcadores.
-- Esta funcion es invocada desde 'cierraasiento' porque que al cerrar el asiento se calculan las contrapartidas de
-- todos los apuntes.
-- Esta es de momento la forma mas eficiente de calcular contrapartidas.
\echo -n ':: Funcion contrapartidas de un asiento ... '
CREATE FUNCTION contraasiento(integer) RETURNS NUMERIC(12, 2)
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


-- Dado un apunte nos retorna el apunte que ejerce de contrapartida. (no la cuenta si no el apunte).
-- Esta funcion puede variar con la funcion ccontrapartida ya que en ccontrapartida el calculo es distinto y puede
-- ser erroneo.
-- Esta funcion esta mucho mas perfeccionada con lo que es menos probable un error. Por tanto se sugiere la migracion
-- a esta nueva funcion.
\echo -n ':: Funcion buscar contrapartida ... '
CREATE FUNCTION bcontrapartida(integer) RETURNS integer
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


-- Dado un apunte nos retorna el apunte que ejerce de contrapartida. (no la cuenta si no el apunte).
-- Esta funcion puede variar con la funcion ccontrapartida ya que en ccontrapartida el calculo es distinto y puede ser
-- erroneo.
-- Esta funcion esta mucho mas perfeccionada con lo que es menos probable un error. Por tanto se sugiere la migracion
-- a esta nueva funcion.
\echo -n ':: Funcion buscar contrapartida borrador ... '
CREATE FUNCTION bcontrapartidaborr(integer) RETURNS integer
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


\echo -n ':: Funcion ID cuenta ... '
CREATE FUNCTION id_cuenta(character varying) RETURNS integer
    AS '
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
'
    LANGUAGE plpgsql;


\echo -n ':: Funcion nivel de codigo de cuenta ... '
\echo -n ':: Usamos la longitud del codigo para determinarlo ya que los sistemas contables usan esa notacion. Podria usarse el nivel en el arbol de cuentas y seria mas general y correcto'
CREATE FUNCTION nivel(character varying) RETURNS integer
    AS '
DECLARE
    codcuenta ALIAS FOR $1;
BEGIN
    RETURN LENGTH(codcuenta);
END;
'
    LANGUAGE plpgsql;


\echo -n ':: Funcion saldo total masa patrimonial ... '
CREATE OR REPLACE FUNCTION saldototalmpatrimonial(integer) RETURNS NUMERIC(12, 2)
    AS '
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
' LANGUAGE plpgsql;


\echo -n ':: Funcion saldo masa patrimonial ... '
CREATE OR REPLACE FUNCTION saldompatrimonial(integer, date, date) RETURNS numeric(12, 2)
    AS '
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
' LANGUAGE plpgsql;


\echo -n ':: Funcion debe de masa patrimonial ... '
CREATE FUNCTION debempatrimonial(integer, date, date) RETURNS numeric(12, 2)
    AS '
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
'
    LANGUAGE plpgsql;


\echo -n ':: Funcion haber de masa patrimonial ... '
CREATE FUNCTION habermpatrimonial(integer, date, date) RETURNS numeric(12, 2)
    AS '
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
'
    LANGUAGE plpgsql;


\echo -n ':: Funcion saldo total ... '
CREATE FUNCTION saldototal(character varying, date, date) RETURNS numeric(12, 2)
    AS '
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
--  RAISE NOTICE '' saldo total % valor adquirido %'', codcuenta, saldo;
    FOR apt1 IN SELECT codigo FROM cuenta WHERE padre = cta.id LOOP
        saldo := saldo + saldototal(apt1.codigo,fechain, fechafin);
    END LOOP;
    RETURN saldo;
END;
'    LANGUAGE plpgsql;


\echo -n ':: Funcion debe total entre fechas ... '
CREATE FUNCTION debetotal(integer, date, date) RETURNS numeric(12,2)
    AS '
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
--  RAISE NOTICE '' Calculando debetotal de % entre % y % '', cuenta, fechain, fechafin;
    debet := 0;
    SELECT INTO apt sum(debe) AS mdebe FROM apunte WHERE apunte.idcuenta = mcuenta AND fecha <= "fechafin" AND fecha >= "fechain";
    IF (apt.mdebe ISNULL) THEN
	debet:= 0;
    ELSE
	debet := apt.mdebe;
    END IF;
--  RAISE NOTICE '' debe total % valor adquirido %'', cuenta, debet;
    FOR apt1 IN SELECT idcuenta FROM cuenta WHERE padre = mcuenta LOOP
	debet := debet + debetotal(apt1.idcuenta, fechain, fechafin);
    END LOOP;
    RETURN debet;
END;
'
    LANGUAGE plpgsql;


\echo -n ':: Funcion debe total ... '
CREATE FUNCTION debetotal1(integer) RETURNS numeric(12, 2)
    AS '
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
--  RAISE NOTICE '' debe total1 % valor adquirido %'', codcuenta, debet;
    FOR apt1 IN SELECT idcuenta FROM cuenta WHERE padre = mcuenta LOOP
	debet := debet + debetotal1(apt1.idcuenta);
    END LOOP;
    RETURN debet;
END;
'
    LANGUAGE plpgsql;


\echo -n ':: Funcion haber total entre fechas ... '
CREATE FUNCTION habertotal(integer, date, date) RETURNS numeric(12, 2)
    AS '
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
--  RAISE NOTICE '' haber total % valor adquirido %'', codcuenta, habert;
    FOR apt1 IN SELECT idcuenta FROM cuenta WHERE padre = mcuenta LOOP
	habert := habert + habertotal(apt1.idcuenta,fechain, fechafin);
    END LOOP;
    RETURN habert;
END;
'
    LANGUAGE plpgsql;


\echo -n ':: Funcion haber total ... '
CREATE FUNCTION habertotal1(integer) RETURNS numeric(12, 2)
    AS '
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
--  RAISE NOTICE '' haber total % valor adquirido %'', codcuenta, habert;
    FOR apt1 IN SELECT idcuenta FROM cuenta WHERE padre = mcuenta LOOP
	habert := habert + habertotal1(apt1.idcuenta);
    END LOOP;
    RETURN habert;
END;
'
    LANGUAGE plpgsql;


\echo -n ':: Funcion recalcula saldos ... '
CREATE FUNCTION recalculasaldos() RETURNS numeric(12,2)
    AS '
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
'
    LANGUAGE plpgsql;

    
\echo -n ':: Funcion recalcula saldos 2 ... '
CREATE FUNCTION recalculasaldos2() RETURNS integer
    AS '
DECLARE
    niveles RECORD;
    cta RECORD;
BEGIN
    -- De momento, la haremos funcionar para un sistema de cuentas xxxxyyy.
    SELECT INTO niveles strpos(valor, ''y'') - 1 AS numx FROM configuracion WHERE nombre = ''CodCuenta'';
    IF niveles.numx <> 4 THEN
	RAISE NOTICE ''Lo siento, pero esta funcion solo funciona de momento con 4 niveles de cuentas.'';
	RETURN -1;
    END IF;
    -- Creamos la tabla con el arbol de cuentas y sus valores (se ha considerado hasta nivel 4).
    CREATE TEMPORARY TABLE temp4 AS (SELECT n1.codigo AS cod1, n1.debe AS debe1, n1.haber AS haber1, n2.codigo AS cod2, n2.debe AS debe2, n2.haber AS haber2, n3.codigo AS cod3, n3.debe AS debe3, n3.haber AS haber3, n4.codigo AS cod4, n4.debe AS debe4, n4.haber AS haber4 FROM (SELECT idcuenta, codigo, debe, haber FROM cuenta WHERE padre IS NULL) AS n1 INNER JOIN (SELECT idcuenta, padre, codigo, debe, haber FROM cuenta) AS n2 ON n1.idcuenta=n2.padre INNER JOIN (SELECT idcuenta, padre, codigo, debe, haber FROM cuenta) AS n3 ON n2.idcuenta = n3.padre LEFT JOIN (SELECT padre, codigo, debe, haber FROM cuenta) AS n4 ON n3.idcuenta = n4.padre);
   
    -- Ahora iremos actualizando las ramas desde las hojas hasta las raices.
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
	RAISE NOTICE ''Cuenta %	-> debe: %	haber: %'', cta.cod1, cta.debe1, cta.haber1;
	UPDATE cuenta SET debe = cta.debe1, haber = cta.haber1 WHERE idcuenta IN (SELECT idcuenta FROM cuenta WHERE codigo = cta.cod1);
	RAISE NOTICE ''Cuenta % actualizada'', cta.cod1;
    END LOOP;
    FOR cta IN SELECT * FROM nivel2 ORDER BY cod2 LOOP
        RAISE NOTICE ''Cuenta %	-> debe: %     haber: %'', cta.cod2, cta.debe2, cta.haber2;
        UPDATE cuenta SET debe = cta.debe2, haber = cta.haber2 WHERE idcuenta IN (SELECT idcuenta FROM cuenta WHERE codigo = cta.cod2);
        RAISE NOTICE ''Cuenta % actualizada'', cta.cod2;
    END LOOP;
    FOR cta IN SELECT * FROM nivel3 WHERE length(cod3)=niveles.numx ORDER BY cod3 LOOP
	RAISE NOTICE ''Cuenta %	-> debe: %     haber: %'', cta.cod3, cta.debe3, cta.haber3;
	UPDATE cuenta SET debe = cta.debe3, haber = cta.haber3 WHERE idcuenta IN (SELECT idcuenta FROM cuenta WHERE codigo = cta.cod3);
	RAISE NOTICE ''Cuenta % actualizada'', cta.cod3;
    END LOOP;
    RETURN 0;
END;
'    LANGUAGE plpgsql;

\echo -n ':: Funcion tratamiento de acumulados ... '
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

\echo -n ':: Funcion crea cuenta ... '
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


\echo -n ':: Disparador nueva cuenta ... '
CREATE TRIGGER nueva_cuenta
    AFTER INSERT ON cuenta
    FOR EACH ROW
    EXECUTE PROCEDURE creacuenta();


\echo -n ':: Funcion borra cuenta ... '
CREATE FUNCTION borracuenta() RETURNS "trigger"
    AS '
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
'
    LANGUAGE plpgsql;


\echo -n ':: Disparador borra cuenta ... '
CREATE TRIGGER borra_cuenta
    BEFORE DELETE ON cuenta
    FOR EACH ROW
    EXECUTE PROCEDURE borracuenta();


\echo -n ':: Funcion crea canal ... '
CREATE FUNCTION creacanal() RETURNS "trigger"
    AS '
DECLARE
    mrecord RECORD;
BEGIN
    FOR mrecord IN SELECT * FROM cuenta LOOP
	INSERT INTO acumulado_canal (idcuenta, idcanal, debe, haber) VALUES(mrecord.idcuenta, NEW.idcanal, 0, 0);
    END LOOP;
    RETURN NEW;
END;
'
    LANGUAGE plpgsql;


\echo -n ':: Disparador nuevo canal ... '
CREATE TRIGGER nuevo_canal
    AFTER INSERT ON canal
    FOR EACH ROW
    EXECUTE PROCEDURE creacanal();
    

\echo -n ':: Funcion borra canal ... '
CREATE FUNCTION borracanal() RETURNS "trigger"
    AS '
BEGIN
    DELETE FROM acumulado_canal WHERE idcanal = OLD.idcanal;
    RETURN OLD;
END;
'
    LANGUAGE plpgsql;


\echo -n ':: Disparador borra canal ... '
CREATE TRIGGER borra_canal
    BEFORE DELETE ON canal
    FOR EACH ROW
    EXECUTE PROCEDURE borracanal();
    

\echo -n ':: Funcion crea centro de coste ... '
CREATE FUNCTION creaccoste() RETURNS "trigger"
    AS '
DECLARE
    mrecord RECORD;
BEGIN
    FOR mrecord IN SELECT * FROM cuenta LOOP
	INSERT INTO acumulado_c_coste (idcuenta, idc_coste, debe, haber) VALUES(mrecord.idcuenta, NEW.idc_coste, 0, 0);
    END LOOP;
    RETURN NEW;
END;
'
    LANGUAGE plpgsql;


\echo -n ':: Disparador nuevo centro de coste ... '
CREATE TRIGGER nuevo_ccoste
    AFTER INSERT ON c_coste
    FOR EACH ROW
    EXECUTE PROCEDURE creaccoste();


\echo -n ':: Funcion borra centro de coste ... '
CREATE FUNCTION borraccoste() RETURNS "trigger"
    AS '
BEGIN
    DELETE FROM acumulado_c_coste WHERE idc_coste = OLD.idc_coste;
    RETURN OLD;
END;
'
    LANGUAGE plpgsql;


\echo -n ':: Disparador borra centro de coste ... '
CREATE TRIGGER borra_ccoste
    BEFORE DELETE ON c_coste
    FOR EACH ROW
    EXECUTE PROCEDURE borraccoste();


\echo -n ':: Funcion propaga acumulado cuenta ... '
CREATE FUNCTION propagaacumuladocuenta() RETURNS "trigger"
    AS '
DECLARE
    incdebe numeric(12, 2);
    inchaber numeric(12, 2);
BEGIN
    incdebe = NEW.debe - OLD.debe;
    inchaber = NEW.haber - OLD.haber;
    -- RAISE NOTICE ''propagaacumuladocuenta %'', NEW.codigo;
    IF incdebe <> 0 OR inchaber <> 0 THEN
	UPDATE cuenta SET debe = debe + incdebe, haber = haber + inchaber WHERE idcuenta = NEW.padre;
    END IF;
    RETURN NEW;
END;
'
    LANGUAGE plpgsql;


\echo -n ':: Disparador propaga acumulado cuenta ... '
CREATE TRIGGER propaga_acumulado_cuenta
    AFTER UPDATE ON cuenta
    FOR EACH ROW
    EXECUTE PROCEDURE propagaacumuladocuenta();


\echo -n ':: Funcion propaga acumulado centro de coste ... '
CREATE FUNCTION propagaacumuladoccoste() RETURNS "trigger"
    AS '
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
'
    LANGUAGE plpgsql;


\echo -n ':: Disparador propaga acumulado centro de coste ... '
CREATE TRIGGER propaga_acumulado_ccoste
    AFTER UPDATE ON c_coste
    FOR EACH ROW
    EXECUTE PROCEDURE propagaacumuladoccoste();
    

\echo -n ':: Funcion acumulados canal ... '
CREATE FUNCTION acumulados_canal() RETURNS "trigger"
    AS '
DECLARE
    incdebe  numeric(12, 2);
    inchaber numeric(12, 2);
    cuentar  RECORD;
BEGIN
    -- RAISE NOTICE '' Ha entrado el trigger acumulados_canal() '';
    -- RAISE NOTICE '' idcuenta % idcanal %'', NEW.idcuenta, NEW.idcanal;
    incdebe = NEW.debe - OLD.debe;
    inchaber = NEW.haber - OLD.haber;
    SELECT INTO cuentar * FROM cuenta WHERE idcuenta = NEW.idcuenta;
    SELECT INTO cuentar * FROM cuenta WHERE idcuenta = cuentar.padre;
    IF FOUND THEN
	UPDATE acumulado_canal SET debe = debe + incdebe, haber = haber + inchaber WHERE idcuenta = cuentar.idcuenta AND idcanal = NEW.idcanal;
    END IF;
    RETURN NEW;
END;
'
    LANGUAGE plpgsql;


\echo -n ':: Disparador acumulados canal ... '
CREATE TRIGGER acumulados_canal_fk
    AFTER UPDATE ON acumulado_canal
    FOR EACH ROW
    EXECUTE PROCEDURE acumulados_canal();


\echo -n ':: Funcion abre asiento ... '
CREATE FUNCTION abreasiento(integer) RETURNS integer
    AS '
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
'
    LANGUAGE plpgsql;


\echo -n ':: Funcion cierra asiento ... '
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


\echo -n ':: Funcion reordena asientos por ejercicio ... '
CREATE FUNCTION reordenaasientos(integer) RETURNS integer
    AS '
DECLARE
    ejercicio ALIAS FOR $1;
    rs RECORD;
    cont integer;
BEGIN
    cont := 1;
    FOR rs IN SELECT * FROM asiento WHERE EXTRACT(YEAR FROM fecha) = ejercicio ORDER BY fecha, ordenasiento LOOP
        IF (cont <> rs.ordenasiento) THEN
            UPDATE asiento SET ordenasiento = cont WHERE idasiento = rs.idasiento;
        END IF;
        cont := cont + 1;
    END LOOP;
    RETURN 0;
END;
'    LANGUAGE plpgsql;


\echo -n ':: Funcion reordena todos los asientos ... '
CREATE OR REPLACE FUNCTION reordenaasientosall() RETURNS integer
    AS '
DECLARE
    rs RECORD;
    ejercicio integer;
BEGIN
    FOR rs IN SELECT DISTINCT EXTRACT (YEAR FROM FECHA) AS ano FROM asiento ORDER BY ano LOOP
        ejercicio = rs.ano;
        PERFORM reordenaasientos(ejercicio);
    END LOOP;
    RETURN 0;
END;
'    LANGUAGE plpgsql;


-- **********************************************************************
-- APARTADO DE COMPROBACIONES DE INTEGRIDAD EXTRA Y DETECCION DE ERRORES.
-- **********************************************************************
-- **********************************************************************
--
-- Las comprobaciones saltan como disparadores y abortan la ejecucion si no se cumplen las restricciones.
-- DROP TRIGGER restriccionescuentatrigger ON cuenta CASCADE;
-- DROP FUNCTION restriccionescuenta();
--
\echo -n ':: Funcion restricciones de cuenta ... '
CREATE FUNCTION restriccionescuenta() RETURNS "trigger"
    AS '
DECLARE
    cta RECORD;
BEGIN
    IF NEW.padre <> 0 THEN
	SELECT INTO cta * FROM cuenta WHERE idcuenta= NEW.padre;
	IF NOT FOUND THEN
	    RAISE EXCEPTION '' La cuenta padre no existe. '';
	END IF;
    END IF;
    IF NEW.codigo = '''' THEN
	RAISE EXCEPTION '' No se puede dejar el codigo de cuenta vacio '';
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


\echo -n ':: Disparador restricciones de cuenta ... '
CREATE TRIGGER restriccionescuentatrigger
    BEFORE INSERT OR UPDATE ON cuenta
    FOR EACH ROW
    EXECUTE PROCEDURE restriccionescuenta();

    
\echo -n ':: Funcion restricciones al borrado de cuenta ... '
CREATE OR REPLACE FUNCTION restriccionesborradocuenta() RETURNS "trigger"
    AS '
DECLARE
    cta RECORD;
BEGIN
    SELECT INTO cta * FROM cuenta WHERE padre = OLD.idcuenta;
    IF FOUND THEN
	RAISE EXCEPTION '' La cuenta tiene hijos. '';
    END IF;
    RETURN OLD;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador restricciones al borrado de cuenta ... '
CREATE TRIGGER restriccionesborradocuentatrigger
    BEFORE DELETE ON cuenta
    FOR EACH ROW
    EXECUTE PROCEDURE restriccionesborradocuenta();    
    

\echo -n ':: Funcion restricciones borrador ... '
CREATE FUNCTION restriccionesborrador() RETURNS "trigger"
AS '
DECLARE
    cta RECORD;
    ej RECORD;
    ord RECORD;
BEGIN
    SELECT INTO cta * FROM cuenta WHERE idcuenta = NEW.idcuenta;
    IF FOUND THEN
        IF cta.bloqueada THEN
	    RAISE EXCEPTION ''Cuenta bloqueada, no se puede utilizar esta cuenta %'', NEW.idcuenta;
        END IF;
        IF cta.nodebe THEN
            IF NEW.debe <> 0 THEN
                RAISE EXCEPTION ''Cuenta bloqueada por debe, solo permite haber %'', NEW.idcuenta;
            END IF;
        END IF;
        IF cta.nohaber THEN
            IF NEW.haber <> 0 THEN
                RAISE EXCEPTION ''Cuenta bloqueada por haber, solo permite debe %'', NEW.idcuenta;
            END IF;
        END IF;
    ELSE
	RAISE EXCEPTION ''Cuenta inexistente %'', NEW.idcuenta;
    END IF;
	
    SELECT INTO ej * FROM ejercicios WHERE ejercicio = EXTRACT (YEAR FROM NEW.fecha) AND periodo = 0;
    IF FOUND THEN
	IF ej.bloqueado = TRUE THEN
    	    RAISE EXCEPTION ''Periodo bloqueado %'', NEW.fecha;
	END IF;
    ELSE
	RAISE EXCEPTION ''Ejercicio Inexistente %'', ej.ejercicio;
    END IF;

    SELECT INTO ej * FROM ejercicios WHERE ejercicio = EXTRACT (YEAR FROM NEW.fecha) AND periodo = EXTRACT (MONTH FROM NEW.fecha);
    IF ej.bloqueado = TRUE THEN
	RAISE EXCEPTION ''Periodo bloqueado %'', NEW.fecha;
    END IF;
    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador restricciones borrador ... '
CREATE TRIGGER restriccionesborradortrigger
    BEFORE INSERT OR UPDATE ON borrador
    FOR EACH ROW
    EXECUTE PROCEDURE restriccionesborrador();


\echo -n ':: Funcion distribucion centros de coste en borrador ... '
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


\echo -n ':: Disparador restricciones borrador ... '
CREATE TRIGGER propagaccosteborradortrigger
    BEFORE INSERT OR UPDATE ON borrador
    FOR EACH ROW
    EXECUTE PROCEDURE propagaccosteborrador();



    
\echo -n ':: Funcion restricciones asiento ... '
CREATE FUNCTION restriccionesasiento() RETURNS "trigger"
    AS '
DECLARE
    ej RECORD;
BEGIN
    SELECT INTO ej * FROM ejercicios WHERE ejercicio = EXTRACT (YEAR FROM NEW.fecha) AND periodo = 0;
    IF FOUND THEN
    	IF ej.bloqueado = TRUE THEN
	    RAISE EXCEPTION ''Periodo bloqueado %'', NEW.fecha;
	END IF;
    ELSE
	RAISE EXCEPTION ''Ejercicio Inexistente %'', ej.ejercicio;
    END IF;
    SELECT INTO ej * FROM ejercicios WHERE ejercicio = EXTRACT (YEAR FROM NEW.fecha) AND periodo = EXTRACT (MONTH FROM NEW.fecha);
    IF ej.bloqueado = TRUE THEN
	RAISE EXCEPTION ''Periodo bloqueado %'', NEW.fecha;
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
' LANGUAGE plpgsql;


\echo -n ':: Disparador restricciones asiento ... '
CREATE TRIGGER restriccionesasientotrigger
   BEFORE INSERT OR UPDATE ON asiento
   FOR EACH ROW
   EXECUTE PROCEDURE restriccionesasiento();


\echo -n ':: Funcion borrado asiento desde un disparador ... '
CREATE FUNCTION tr_borradoasiento() RETURNS "trigger"
    AS '
BEGIN
    UPDATE prevcobro SET idasiento = NULL WHERE idasiento = OLD.idasiento;
    RETURN OLD;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador restricciones borrado de asiento ... '
CREATE TRIGGER restriccionesborradoasientotrigger
   BEFORE DELETE ON asiento
   FOR EACH ROW
   EXECUTE PROCEDURE tr_borradoasiento();

   
-- Propaga los acumulados de IVA.
\echo -n ':: Funcion inserta tipo de IVA ... '
CREATE FUNCTION inserttipoiva() RETURNS "trigger"
    AS '
DECLARE
    mrecord RECORD;
BEGIN
    FOR mrecord IN SELECT * FROM registroiva LOOP
	INSERT INTO iva (idregistroiva, idtipoiva,baseiva) VALUES(mrecord.idregistroiva, NEW.idtipoiva, 0);
    END LOOP;
    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador nuevo tipo de IVA ... '
CREATE TRIGGER nuevotipoiva
   AFTER INSERT ON tipoiva
   FOR EACH ROW
   EXECUTE PROCEDURE inserttipoiva();


\echo -n ':: Funcion borra tipo de IVA ... '
CREATE FUNCTION deletetipoiva() RETURNS "trigger"
    AS '
DECLARE
    mrecord RECORD;
BEGIN
    DELETE FROM iva WHERE idtipoiva = OLD.idtipoiva;
    RETURN OLD;
END;
'    LANGUAGE plpgsql;


\echo -n ':: Disparador borra tipo de IVA ... '
CREATE TRIGGER borratipoiva
   BEFORE DELETE ON tipoiva
   FOR EACH ROW
   EXECUTE PROCEDURE deletetipoiva();


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
   
   

-- ******************************************************
-- FIN DEL APARTADO DE COMPROBACIONES.
-- ******************************************************
-- ******************************************************

\echo '********* FIN FICHERO DE ESTRUCTURA DE LA BASE DE DATOS DE BULMACONT *********'

-- ================================== ACTUALIZACION  ===================================
-- =====================================================================================

-- Agregamos nuevos parametros de configuracion
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	rs RECORD;
BEGIN
	SELECT INTO rs * FROM configuracion WHERE nombre = ''DatabaseRevision'';
	IF FOUND THEN
		UPDATE CONFIGURACION SET valor = ''0.14.1-0001'' WHERE nombre = ''DatabaseRevision'';
	ELSE
		INSERT INTO configuracion (nombre, valor) VALUES (''DatabaseRevision'', ''0.14.1-0001'');
	END IF;
	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT actualizarevision();
DROP FUNCTION actualizarevision() CASCADE;
\echo "Actualizada la revision de la base de datos a la version 0.14.1"

\echo -n ':: '
COMMENT ON SCHEMA public IS 'Standard public schema';

\echo -n ':: '
COMMIT;


