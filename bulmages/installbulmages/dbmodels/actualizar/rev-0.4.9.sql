--
-- Modificación de campos y funciones de la BD para la adaptación al tipo de datos monetario
--

BEGIN;

--
-- Estas primeras funciones cambiarán los tipos de columnas que están como flotantes a NUMERIC.
-- Se trata de un parche que se desea aplicar para almacenar los tipos monetarios
-- ya que actualmente se encuantran almacenados como 'doubles' y es preferible
-- que se almacenen como tipo 'numeric'.
-- Todas devuelven como valor numérico el número de filas influenciadas por el cambio
-- NOTA: Si alguien sabe como pasar por parámetro un nombre de tabla y campo a modificar se
-- haría mucho más sencillito porque sólo habría que implementar un función ya que siempre
-- hay que hacer lo mismo.
--

--
-- Función para convertir los tipos de la tabla CUENTA
--
CREATE OR REPLACE FUNCTION convierteanumeric_cuenta() RETURNS INTEGER AS '
DECLARE
   fila RECORD;
   num INTEGER;
BEGIN
   ALTER TABLE cuenta ADD COLUMN temp_debe numeric(12,2);
   ALTER TABLE cuenta ADD COLUMN temp_haber numeric(12,2);
   RAISE INFO ''Convirtiendo la tabla cuenta... '';
   num := 0;
   FOR fila IN SELECT * FROM cuenta LOOP
      UPDATE cuenta SET temp_debe=fila.debe, temp_haber=fila.haber where idcuenta=fila.idcuenta;
      num := num+1;
   END LOOP;
   ALTER TABLE cuenta DROP COLUMN debe;
   ALTER TABLE cuenta DROP COLUMN haber;
   ALTER TABLE cuenta RENAME COLUMN temp_debe TO debe;
   ALTER TABLE cuenta RENAME COLUMN temp_haber TO haber;
   RAISE INFO ''convertida.\n'';
   RETURN num;
END;
' LANGUAGE 'plpgsql';

--
-- Función para convertir los tipos de la tabla C_COSTE
--
CREATE OR REPLACE FUNCTION convierteanumeric_c_coste() RETURNS INTEGER AS '
DECLARE
   fila RECORD;
   num INTEGER;
BEGIN
   ALTER TABLE c_coste ADD COLUMN temp_debe numeric(12,2);
   ALTER TABLE c_coste ADD COLUMN temp_haber numeric(12,2);
   RAISE INFO ''Convirtiendo la tabla c_coste... '';
   num := 0;
   FOR fila IN SELECT * FROM c_coste LOOP
      UPDATE c_coste SET temp_debe=fila.debe, temp_haber=fila.haber where idc_coste=fila.idc_coste;
      num := num+1;
   END LOOP;
   ALTER TABLE c_coste DROP COLUMN debe;
   ALTER TABLE c_coste DROP COLUMN haber;
   ALTER TABLE c_coste RENAME COLUMN temp_debe TO debe;
   ALTER TABLE c_coste RENAME COLUMN temp_haber TO haber;
   RAISE INFO ''convertida.\n'';
   RETURN num;
END;
' LANGUAGE 'plpgsql';

--
-- Función para convertir los tipos de la tabla ACUMULADO_C_COSTE
--
CREATE OR REPLACE FUNCTION convierteanumeric_acumulado_c_coste() RETURNS INTEGER AS '
DECLARE
   fila RECORD;
   num INTEGER;
BEGIN
   ALTER TABLE acumulado_c_coste ADD COLUMN temp_debe numeric(12,2);
   ALTER TABLE acumulado_c_coste ADD COLUMN temp_haber numeric(12,2);
   RAISE INFO ''Convirtiendo la tabla acumulado_c_coste... '';
   num := 0;
   FOR fila IN SELECT * FROM acumulado_c_coste LOOP
      UPDATE acumulado_c_coste SET temp_debe=fila.debe, temp_haber=fila.haber where idacumulado_c_coste=fila.idacumulado_c_coste;
      num := num+1;
   END LOOP;
   ALTER TABLE acumulado_c_coste DROP COLUMN debe;
   ALTER TABLE acumulado_c_coste DROP COLUMN haber;
   ALTER TABLE acumulado_c_coste RENAME COLUMN temp_debe TO debe;
   ALTER TABLE acumulado_c_coste RENAME COLUMN temp_haber TO haber;
   RAISE INFO ''convertida.\n'';
   RETURN num;
END;
' LANGUAGE 'plpgsql';

--
-- Función para convertir los tipos de la tabla ACUMULADO_CANAL
--
CREATE OR REPLACE FUNCTION convierteanumeric_acumulado_canal() RETURNS INTEGER AS '
DECLARE
   fila RECORD;
   num INTEGER;
BEGIN
   ALTER TABLE acumulado_canal ADD COLUMN temp_debe numeric(12,2);
   ALTER TABLE acumulado_canal ADD COLUMN temp_haber numeric(12,2);
   RAISE INFO ''Convirtiendo la tabla acumulado_canal... '';
   num := 0;
   FOR fila IN SELECT * FROM acumulado_canal LOOP
      UPDATE acumulado_canal SET temp_debe=fila.debe, temp_haber=fila.haber where idacumulado_canal=fila.idacumulado_canal;
      num := num+1;
   END LOOP;
   ALTER TABLE acumulado_canal DROP COLUMN debe;
   ALTER TABLE acumulado_canal DROP COLUMN haber;
   ALTER TABLE acumulado_canal RENAME COLUMN temp_debe TO debe;
   ALTER TABLE acumulado_canal RENAME COLUMN temp_haber TO haber;
   RAISE INFO ''convertida.\n'';
   RETURN num;
END;
' LANGUAGE 'plpgsql';

--
-- Función para convertir los tipos de la tabla APUNTE
--
CREATE OR REPLACE FUNCTION convierteanumeric_apunte() RETURNS INTEGER AS '
DECLARE
   fila RECORD;
   num INTEGER;
BEGIN
   ALTER TABLE apunte ADD COLUMN temp_debe numeric(12,2);
   ALTER TABLE apunte ADD COLUMN temp_haber numeric(12,2);
   RAISE INFO ''Convirtiendo la tabla apunte... (paciencia para esta)'';
   num := 0;
   FOR fila IN SELECT * FROM apunte LOOP
      UPDATE apunte SET temp_debe=fila.debe, temp_haber=fila.haber where idapunte=fila.idapunte;
      num := num+1;
   END LOOP;
   ALTER TABLE apunte DROP COLUMN debe;
   ALTER TABLE apunte DROP COLUMN haber;
   ALTER TABLE apunte RENAME COLUMN temp_debe TO debe;
   ALTER TABLE apunte RENAME COLUMN temp_haber TO haber;
   RAISE INFO ''convertida. (por fin)\n'';
   RETURN num;
END;
' LANGUAGE 'plpgsql';

--
-- Función para convertir los tipos de la tabla BORRADOR
--
CREATE OR REPLACE FUNCTION convierteanumeric_borrador() RETURNS INTEGER AS '
DECLARE
   fila RECORD;
   num INTEGER;
BEGIN
   ALTER TABLE borrador ADD COLUMN temp_debe numeric(12,2);
   ALTER TABLE borrador ADD COLUMN temp_haber numeric(12,2);
   RAISE INFO ''Convirtiendo la tabla borrador... (esta también tarda...)'';
   num := 0;
   FOR fila IN SELECT * FROM borrador LOOP
      UPDATE borrador SET temp_debe=fila.debe, temp_haber=fila.haber where idborrador=fila.idborrador;
      num := num+1;
   END LOOP;
   ALTER TABLE borrador DROP COLUMN debe;
   ALTER TABLE borrador DROP COLUMN haber;
   ALTER TABLE borrador RENAME COLUMN temp_debe TO debe;
   ALTER TABLE borrador RENAME COLUMN temp_haber TO haber;
   RAISE INFO ''convertida.\n'';
   RETURN num;
END;
' LANGUAGE 'plpgsql';

--
-- Función para convertir los tipos de la tabla REGISTROIVA
--
CREATE OR REPLACE FUNCTION convierteanumeric_registroiva() RETURNS INTEGER AS '
DECLARE
   fila RECORD;
   num INTEGER;
BEGIN
   ALTER TABLE registroiva ADD COLUMN temp_baseimp numeric(12,2);
   ALTER TABLE registroiva ADD COLUMN temp_iva numeric(12,2);
   RAISE INFO ''Convirtiendo la tabla registroiva... '';
   num := 0;
   FOR fila IN SELECT * FROM registroiva LOOP
      UPDATE registroiva SET temp_baseimp=fila.baseimp, temp_iva=fila.iva where idregistroiva=fila.idregistroiva;
      num := num+1;
   END LOOP;
   ALTER TABLE registroiva DROP COLUMN baseimp;
   ALTER TABLE registroiva DROP COLUMN iva;
   ALTER TABLE registroiva RENAME COLUMN temp_baseimp TO baseimp;
   ALTER TABLE registroiva RENAME COLUMN temp_iva TO iva;
   RAISE INFO ''convertida.\n'';
   RETURN num;
END;
' LANGUAGE 'plpgsql';

--
-- Función para convertir los tipos de la tabla PREVCOBRO
--
CREATE OR REPLACE FUNCTION convierteanumeric_prevcobro() RETURNS INTEGER AS '
DECLARE
   fila RECORD;
   num INTEGER;
BEGIN
   ALTER TABLE prevcobro ADD COLUMN temp_cppcobro numeric(12,2);
   ALTER TABLE prevcobro ADD COLUMN temp_cpcobro numeric(12,2);
   RAISE INFO ''Convirtiendo la tabla prevcobro... '';
   num := 0;
   FOR fila IN SELECT * FROM prevcobro LOOP
      UPDATE prevcobro SET temp_cppcobro=fila.cantidadprevistaprevcobro, temp_cpcobro=fila.cantidadprevcobro where idprevcobro=fila.idprevcobro;
      num := num+1;
   END LOOP;
   ALTER TABLE prevcobro DROP COLUMN cantidadprevistaprevcobro;
   ALTER TABLE prevcobro DROP COLUMN cantidadprevcobro;
   ALTER TABLE prevcobro RENAME COLUMN temp_cppcobro TO cantidadprevistaprevcobro;
   ALTER TABLE prevcobro RENAME COLUMN temp_cpcobro TO cantidadprevcobro;
   RAISE INFO ''convertida.\n'';
   RETURN num;
END;
' LANGUAGE 'plpgsql';

--
-- Función para convertir los tipos de la tabla IVA
--
CREATE OR REPLACE FUNCTION convierteanumeric_iva() RETURNS INTEGER AS '
DECLARE
   fila RECORD;
   num INTEGER;
BEGIN
   ALTER TABLE iva ADD COLUMN temp_iva numeric(12,2);
   RAISE INFO ''Convirtiendo la tabla iva... '';
   num := 0;
   FOR fila IN SELECT * FROM iva LOOP
      UPDATE iva SET temp_iva=fila.baseiva where idiva=fila.idiva;
      num := num+1;
   END LOOP;
   ALTER TABLE iva DROP COLUMN baseiva;
   ALTER TABLE iva RENAME COLUMN temp_iva TO baseiva;
   RAISE INFO ''convertida.\n'';
   RETURN num;
END;
' LANGUAGE 'plpgsql';

--
-- Función para convertir los tipos de la tabla MPATROMINIAL
--
CREATE OR REPLACE FUNCTION convierteanumeric_mpatrimonial() RETURNS INTEGER AS '
DECLARE
   fila RECORD;
   num INTEGER;
BEGIN
   ALTER TABLE mpatrimonial ADD COLUMN temp_saldo numeric(12,2);
   RAISE INFO ''Convirtiendo la tabla mpatrimonial... '';
   num := 0;
   FOR fila IN SELECT * FROM mpatrimonial LOOP
      UPDATE mpatrimonial SET temp_saldo=fila.saldo where idmpatrimonial=fila.idmpatrimonial;
      num := num+1;
   END LOOP;
   ALTER TABLE mpatrimonial DROP COLUMN saldo;
   ALTER TABLE mpatrimonial RENAME COLUMN temp_saldo TO saldo;
   RAISE INFO ''convertida.\n'';
   RETURN num;
END;
' LANGUAGE 'plpgsql';

--
-- Función para convertir los tipos de la tabla COMPMASAP
--
CREATE OR REPLACE FUNCTION convierteanumeric_compmasap() RETURNS INTEGER AS '
DECLARE
   fila RECORD;
   num INTEGER;
BEGIN
   ALTER TABLE compmasap ADD COLUMN temp_saldo numeric(12,2);
   RAISE INFO ''Convirtiendo la tabla compmasap... '';
   num := 0;
   FOR fila IN SELECT * FROM compmasap LOOP
      UPDATE compmasap SET temp_saldo=fila.saldo where idcompmasap=fila.idcompmasap;
      num := num+1;
   END LOOP;
   ALTER TABLE compmasap DROP COLUMN saldo;
   ALTER TABLE compmasap RENAME COLUMN temp_saldo TO saldo;
   RAISE INFO ''convertida.\n'';
   RETURN num;
END;
' LANGUAGE 'plpgsql';

--
-- Función para convertir los tipos de la tabla AMORTIZACION
--
CREATE OR REPLACE FUNCTION convierteanumeric_amortizacion() RETURNS INTEGER AS '
DECLARE
   fila RECORD;
   num INTEGER;
BEGIN
   ALTER TABLE amortizacion ADD COLUMN temp_valorcompra numeric(12,2);
   RAISE INFO ''Convirtiendo la tabla amortizacion... '';
   num := 0;
   FOR fila IN SELECT * FROM amortizacion LOOP
      UPDATE amortizacion SET temp_valorcompra=fila.valorcompra where idamortizacion=fila.idamortizacion;
      num := num+1;
   END LOOP;
   ALTER TABLE amortizacion DROP COLUMN valorcompra;
   ALTER TABLE amortizacion RENAME COLUMN temp_valorcompra TO valorcompra;
   RAISE INFO ''convertida.\n'';
   RETURN num;
END;
' LANGUAGE 'plpgsql';

--
-- Función para convertir los tipos de la tabla LINAMORTIZACION
--
CREATE OR REPLACE FUNCTION convierteanumeric_linamortizacion() RETURNS INTEGER AS '
DECLARE
   fila RECORD;
   num INTEGER;
BEGIN
   ALTER TABLE linamortizacion ADD COLUMN temp_cantidad numeric(12,2);
   RAISE INFO ''Convirtiendo la tabla linamortizacion... '';
   num := 0;
   FOR fila IN SELECT * FROM linamortizacion LOOP
      UPDATE linamortizacion SET temp_cantidad=fila.cantidad where idlinamortizacion=fila.idlinamortizacion;
      num := num+1;
   END LOOP;
   ALTER TABLE linamortizacion DROP COLUMN cantidad;
   ALTER TABLE linamortizacion RENAME COLUMN temp_cantidad TO cantidad;
   RAISE INFO ''convertida.\n'';
   RETURN num;
END;
' LANGUAGE 'plpgsql';

--
-- Se ejecutan las funciones para cambiar los tipos
--
SELECT convierteanumeric_cuenta();
SELECT convierteanumeric_c_coste();
SELECT convierteanumeric_acumulado_c_coste();
SELECT convierteanumeric_acumulado_canal();
SELECT convierteanumeric_apunte();
SELECT convierteanumeric_borrador();
SELECT convierteanumeric_registroiva();
SELECT convierteanumeric_prevcobro();
SELECT convierteanumeric_iva();
SELECT convierteanumeric_mpatrimonial();
SELECT convierteanumeric_compmasap();
SELECT convierteanumeric_amortizacion();
SELECT convierteanumeric_linamortizacion();

--
-- Y ahora borramos las funciones, para qué las queremos ya
--
DROP FUNCTION convierteanumeric_cuenta();
DROP FUNCTION convierteanumeric_c_coste();
DROP FUNCTION convierteanumeric_acumulado_c_coste();
DROP FUNCTION convierteanumeric_acumulado_canal();
DROP FUNCTION convierteanumeric_apunte();
DROP FUNCTION convierteanumeric_borrador();
DROP FUNCTION convierteanumeric_registroiva();
DROP FUNCTION convierteanumeric_prevcobro();
DROP FUNCTION convierteanumeric_iva();
DROP FUNCTION convierteanumeric_mpatrimonial();
DROP FUNCTION convierteanumeric_compmasap();
DROP FUNCTION convierteanumeric_amortizacion();
DROP FUNCTION convierteanumeric_linamortizacion();

--
-- Ahora vamos a definir funciones que ya deberían existir en la BD y que, debido a la actualización de los tipos,
-- deben ser también implementadas de nuevo.
-- Ver 'dbmodels/crear/bulmages/bulmages_schema.sql' para más información
--
DROP FUNCTION contraasiento(integer);
CREATE FUNCTION contraasiento(integer) RETURNS NUMERIC(12,2)
   AS '
DECLARE
   midasiento ALIAS FOR $1;
    midapunte ALIAS FOR $1;
    apt RECORD;
    aptasien RECORD;
    cont RECORD;
    descuadre NUMERIC(12,2);
    apmaxdebe INTEGER;
    apmaxhaber INTEGER;
    maxdebe NUMERIC(12,2);
    maxhaber NUMERIC(12,2);
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
        FOR  cont IN SELECT  idcuenta,idapunte, debe, haber, orden FROM apunte WHERE idasiento = midasiento ORDER BY orden LOOP
            -- Si es el debe maximo lo hacemos constar.
            IF cont.debe >= maxdebe THEN
                maxdebe := cont.debe;
                apmaxdebe := cont.idapunte;
                ctadebe := cont.idcuenta;
            END IF;
            -- Si es el haber mximo lo hacemos constar
            IF cont.haber >= maxhaber THEN
                maxhaber := cont.haber;
                apmaxhaber := cont.idapunte;
                ctahaber := cont.idcuenta;
            END IF;
            -- Calculamos el descuadre
            descuadre := descuadre + cont.debe;
            descuadre := descuadre - cont.haber;
            -- Si es el descuadre inicializamos las variables.
            IF descuadre = 0 AND ctadebe <> 0 AND ctahaber <> 0 THEN
                UPDATE apunte SET contrapartida= ctahaber WHERE  haber=0 AND idasiento = midasiento AND orden <= cont.orden AND contrapartida ISNULL;
                UPDATE apunte SET contrapartida= ctadebe WHERE  debe=0 AND idasiento = midasiento AND orden <= cont.orden AND contrapartida ISNULL;
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


DROP FUNCTION saldototalmpatrimonial(integer);
CREATE FUNCTION saldototalmpatrimonial(integer) RETURNS NUMERIC(12,2)
    AS '
DECLARE
   identmpatrimonial ALIAS FOR $1;
   saldo NUMERIC(12,2);
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
'    LANGUAGE plpgsql;

DROP FUNCTION saldompatrimonial(integer, timestamp without time zone, timestamp without time zone);
CREATE FUNCTION saldompatrimonial(integer, timestamp without time zone, timestamp without time zone) RETURNS numeric(12,2)
    AS '
DECLARE
    identmpatrimonial ALIAS FOR $1;
    fechain ALIAS FOR $2;
    fechafin ALIAS FOR $3;
    aptsum RECORD;
    aptrest RECORD;
    smpatrimonialsum RECORD;
    smpatrimonialres RECORD;
    saldo NUMERIC(12,2);
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
'    LANGUAGE plpgsql;


DROP FUNCTION debempatrimonial(integer, timestamp without time zone, timestamp without time zone);
CREATE FUNCTION debempatrimonial(integer, timestamp without time zone, timestamp without time zone) RETURNS numeric(12,2)
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
'    LANGUAGE plpgsql;

DROP FUNCTION habermpatrimonial(integer, timestamp without time zone, timestamp without time zone);
CREATE FUNCTION habermpatrimonial(integer, timestamp without time zone, timestamp without time zone) RETURNS numeric(12,2)
    AS '
DECLARE
    identmpatrimonial ALIAS FOR $1;
    fechain ALIAS FOR $2;
    fechafin ALIAS FOR $3;
    aptsum RECORD;
    aptrest RECORD;
    smpatrimonialsum RECORD;
    smpatrimonialres RECORD;
    haber NUMERIC(12,2);
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
'    LANGUAGE plpgsql;

DROP FUNCTION saldototal(character varying, timestamp without time zone, timestamp without time zone);
CREATE FUNCTION saldototal(character varying, timestamp without time zone, timestamp without time zone) RETURNS numeric(12,2)
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
--    SELECT INTO apt * FROM apunte WHERE id_cuenta(codcuenta) = apunte.idcuenta;
    SELECT INTO cta id_cuenta(codcuenta) AS id;
    SELECT INTO apt sum(debe) AS tdebe, sum(haber) AS thaber FROM apunte WHERE apunte.idcuenta = cta.id AND fecha <= "fechafin" AND fecha >= "fechain";
    IF (apt.tdebe ISNULL) THEN
        saldo := 0;
    ELSE
        saldo := apt.tdebe - apt.thaber;
    END IF;
--    RAISE NOTICE '' saldo total % valor adquirido %'', codcuenta, saldo;
    FOR apt1 IN SELECT codigo FROM cuenta WHERE padre = cta.id LOOP
        saldo := saldo + saldototal(apt1.codigo,fechain, fechafin);
    END LOOP;
    RETURN saldo;
END;
'    LANGUAGE plpgsql;

DROP FUNCTION debetotal(integer, timestamp without time zone, timestamp without time zone);
CREATE FUNCTION debetotal(integer, timestamp without time zone, timestamp without time zone) RETURNS numeric(12,2)
    AS '
DECLARE
    mcuenta ALIAS FOR $1;
    fechain ALIAS FOR $2;
    fechafin ALIAS FOR $3;
    apt RECORD;
    apt1 RECORD;
    apt2 RECORD;
         cta RECORD;
    debet NUMERIC(12,2);
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
'    LANGUAGE plpgsql;

DROP FUNCTION debetotal1(integer);
CREATE FUNCTION debetotal1(integer) RETURNS numeric(12,2)
    AS '
DECLARE
    mcuenta ALIAS FOR $1;
    apt RECORD;
    apt1 RECORD;
    apt2 RECORD;
         cta RECORD;
    debet NUMERIC(12,2);
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
'    LANGUAGE plpgsql;

DROP FUNCTION habertotal(integer, timestamp without time zone, timestamp without time zone);
CREATE FUNCTION habertotal(integer, timestamp without time zone, timestamp without time zone) RETURNS numeric(12,2)
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
'    LANGUAGE plpgsql;

DROP FUNCTION habertotal1(integer);
CREATE FUNCTION habertotal1(integer) RETURNS numeric(12,2)
    AS '
DECLARE
    mcuenta ALIAS FOR $1;
    apt RECORD;
    apt1 RECORD;
    apt2 RECORD;
         cta RECORD;
    habert NUMERIC(12,2);
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
'    LANGUAGE plpgsql;

DROP FUNCTION recalculasaldos();
CREATE FUNCTION recalculasaldos() RETURNS numeric(12,2)
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
'    LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION recalculasaldos2() RETURNS integer
    AS '
DECLARE
   niveles RECORD;
   cta RECORD;
BEGIN
   -- De momento, la haremos funcionar para un sistema de cuentas xxxxyyy
   SELECT INTO niveles strpos(valor, ''y'')-1 AS numx FROM configuracion WHERE nombre=''CodCuenta'';
   IF niveles.numx <> 4 THEN
        RAISE NOTICE ''Lo siento, pero esta función sólo funciona de momento con 4 niveles de cuentas'';
        RETURN -1;
   END IF;

   -- Creamos la tabla con el árbol de cuentas y sus valores (se ha considerado hasta nivel 4)
   CREATE TEMPORARY TABLE temp4 AS (SELECT n1.codigo AS cod1, n1.debe AS debe1, n1.haber AS haber1, n2.codigo AS cod2, n2.debe AS debe2, n2.haber AS haber2, n3.codigo AS cod3, n3.debe AS debe3, n3.haber AS haber3, n4.codigo AS cod4, n4.debe AS debe4, n4.haber AS haber4 FROM (SELECT idcuenta, codigo, debe, haber FROM cuenta WHERE padre IS NULL) AS n1 INNER JOIN (SELECT idcuenta, padre, codigo, debe, haber FROM cuenta) AS n2 ON n1.idcuenta=n2.padre INNER JOIN (SELECT idcuenta, padre, codigo, debe, haber FROM cuenta) AS n3 ON n2.idcuenta=n3.padre LEFT JOIN (SELECT padre, codigo, debe, haber FROM cuenta) AS n4 ON n3.idcuenta=n4.padre);

   -- Ahora iremos actualizando las ramas desde las hojas hasta las raíces
   -- Primero, tendremos en cuenta aquellas cuentas que están en un nivel 4, calculamos la suma de su nivel y subimos el dato al nivel 3
   CREATE TEMPORARY TABLE temp3 AS (SELECT cod1,cod2,cod3,sum(debe4) AS debe3,sum(haber4) AS haber3 FROM temp4 WHERE debe4 IS NOT NULL group by cod1,cod2,cod3 order by cod3);
   -- Seguidamente, añadimos las hojas del nivel 3 que descartamos en la acción anterior porque no tenían cuentas hijas en el nivel 4
   INSERT INTO temp3 SELECT cod1,cod2,cod3,debe3,haber3 FROM temp4 WHERE debe4 IS NULL;
   -- Se calculan las sumas del nivel 3 y les pasamos el dato a las cuentas padre del nivel 2
   CREATE TEMPORARY TABLE temp2 AS (SELECT cod1,cod2,sum(debe3) AS debe2,sum(haber3) AS haber2 FROM temp3 group by cod1,cod2 order by cod2);
   -- Y finalmente, hacemos lo mismo con el nivel 2 y subimos las sumas al nivel 1
   CREATE TEMPORARY TABLE temp1 AS (SELECT cod1,sum(debe2) AS debe1,sum(haber2) AS haber1 FROM temp2 group by cod1 order by cod1);

   -- Ahora vamos a eliminar de las tablas aquellas cuentas que no será necesario actualizar por ya estar con los valores correctos
   CREATE TEMPORARY TABLE nivel1 AS (SELECT t1.cod1,t1.debe1,t1.haber1 FROM (SELECT * FROM temp1) AS t1 INNER JOIN (SELECT codigo,debe,haber FROM cuenta) AS t2 ON t1.cod1=t2.codigo WHERE t1.debe1<>t2.debe OR t1.haber1<>t2.haber);
   CREATE TEMPORARY TABLE nivel2 AS (SELECT t1.cod2,t1.debe2,t1.haber2 FROM (SELECT * FROM temp2) AS t1 INNER JOIN (SELECT codigo,debe,haber FROM cuenta) AS t2 ON t1.cod2=t2.codigo WHERE t1.debe2<>t2.debe OR t1.haber2<>t2.haber);
   CREATE TEMPORARY TABLE nivel3 AS (SELECT t1.cod3,t1.debe3,t1.haber3 FROM (SELECT * FROM temp3) AS t1 INNER JOIN (SELECT codigo,debe,haber FROM cuenta) AS t2 ON t1.cod3=t2.codigo WHERE t1.debe3<>t2.debe OR t1.haber3<>t2.haber);

   -- Como colofón, hay que introducir los valores actualizados en las cuentas padre.
   FOR cta IN SELECT * FROM nivel1 ORDER BY cod1 LOOP
        RAISE NOTICE ''Cuenta % -> debe: %      haber: %'',cta.cod1,cta.debe1,cta.haber1;
        UPDATE cuenta SET debe=cta.debe1, haber=cta.haber1 WHERE idcuenta IN (SELECT idcuenta FROM cuenta WHERE codigo=cta.cod1);
        RAISE NOTICE ''Cuenta % actualizada'',cta.cod1;
   END LOOP;
   FOR cta IN SELECT * FROM nivel2 ORDER BY cod2 LOOP
        RAISE NOTICE ''Cuenta % -> debe: %     haber: %'',cta.cod2,cta.debe2,cta.haber2;
        UPDATE cuenta SET debe=cta.debe2, haber=cta.haber2 WHERE idcuenta IN (SELECT idcuenta FROM cuenta WHERE codigo=cta.cod2);
        RAISE NOTICE ''Cuenta % actualizada'',cta.cod2;
   END LOOP;
   FOR cta IN SELECT * FROM nivel3 WHERE length(cod3)=niveles.numx ORDER BY cod3 LOOP
        RAISE NOTICE ''Cuenta % -> debe: %     haber: %'',cta.cod3,cta.debe3,cta.haber3;
        UPDATE cuenta SET debe=cta.debe3, haber=cta.haber3 WHERE idcuenta IN (SELECT idcuenta FROM cuenta WHERE codigo=cta.cod3);
        RAISE NOTICE ''Cuenta % actualizada'',cta.cod3;
   END LOOP;

   RETURN 0;
END;
'    LANGUAGE plpgsql;

DROP FUNCTION propagaacumuladocuenta() CASCADE;
CREATE FUNCTION propagaacumuladocuenta() RETURNS "trigger"
    AS '
DECLARE
   incdebe numeric(12,2);
   inchaber numeric(12,2);
BEGIN
   incdebe = NEW.debe - OLD.debe;
   inchaber = NEW.haber - OLD.haber;
--   RAISE NOTICE ''propagaacumuladocuenta %'', NEW.codigo;
   IF incdebe <> 0 OR inchaber <> 0 THEN
           UPDATE cuenta SET debe = debe + incdebe, haber = haber + inchaber WHERE idcuenta = NEW.padre;
   END IF;
   RETURN NEW;
END;
'    LANGUAGE plpgsql;

CREATE TRIGGER propaga_acumulado_cuenta
    AFTER UPDATE ON cuenta
    FOR EACH ROW
    EXECUTE PROCEDURE propagaacumuladocuenta();

DROP FUNCTION propagaacumuladoccoste() CASCADE;
CREATE FUNCTION propagaacumuladoccoste() RETURNS "trigger"
    AS '
DECLARE
   incdebe numeric(12,2);
   inchaber numeric(12,2);
BEGIN
   incdebe = NEW.debe - OLD.debe;
   inchaber = NEW.haber - OLD.haber;
   IF incdebe <> 0 OR inchaber <> 0 THEN
           UPDATE c_coste SET debe = debe + incdebe, haber = haber + inchaber WHERE idc_coste = OLD.padre;
   END IF;
   RETURN NEW;
END;
'    LANGUAGE plpgsql;

CREATE TRIGGER propaga_acumulado_ccoste
    AFTER UPDATE ON c_coste
    FOR EACH ROW
    EXECUTE PROCEDURE propagaacumuladoccoste();

DROP FUNCTION acumulados_canal() CASCADE;
CREATE FUNCTION acumulados_canal() RETURNS "trigger"
    AS '
DECLARE
   incdebe  numeric(12,2);
   inchaber numeric(12,2);
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
'    LANGUAGE plpgsql;

CREATE TRIGGER acumulados_canal_fk
    AFTER UPDATE ON acumulado_canal
    FOR EACH ROW
    EXECUTE PROCEDURE acumulados_canal();


DROP FUNCTION inserttipoiva() CASCADE;
CREATE OR REPLACE FUNCTION inserttipoiva () RETURNS "trigger"
AS '
DECLARE
   mrecord RECORD;
BEGIN
      FOR mrecord IN SELECT * FROM registroiva LOOP
         INSERT INTO iva (idregistroiva, idtipoiva,baseiva) VALUES(mrecord.idregistroiva, NEW.idtipoiva,0);
      END LOOP;
      RETURN NEW;
END;
' LANGUAGE plpgsql;

CREATE TRIGGER nuevotipoiva
   AFTER INSERT ON tipoiva
   FOR EACH ROW
   EXECUTE PROCEDURE inserttipoiva();

DROP FUNCTION deletetipoiva() CASCADE;
CREATE OR REPLACE FUNCTION deletetipoiva() RETURNS "trigger"
    AS '
DECLARE
   mrecord RECORD;
BEGIN
        DELETE FROM iva WHERE idtipoiva=OLD.idtipoiva;
        RETURN OLD;
END;
'    LANGUAGE plpgsql;

CREATE TRIGGER borratipoiva
   BEFORE DELETE ON tipoiva
   FOR EACH ROW
   EXECUTE PROCEDURE deletetipoiva();

--
-- Se añade el campo Código Postal a las cuenta porque se necesita ese dato en los listados para el modelo 347.
--
ALTER TABLE cuenta ADD COLUMN cpent_cuenta character varying(5);

COMMIT;