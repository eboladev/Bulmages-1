-- 
-- Revision de las bases de datos de empresa. Para BulmaGes 0.3.7
--
--            (C)2004, Tomeu Borras tborras@conetxia.com
--
-- Este archivo debe aplicarse mediante el comando:
--
--     psql xxxx < rev-0_3_7.sql
--
-- a todas las empresas del programa BulmaGes.
--
-- De esta forma se consigue  actualizar las bases de datos
-- de la version 0.3.5 a la version 0.3.7
--
--
\connect - postgres


DROP TABLE linamortizacion;
DROP TABLE amortizacion;
DROP SEQUENCE amortizacion_idamortizacion_seq;
DROP SEQUENCE linamortizacion_idlinamortizacion_seq;

CREATE TABLE amortizacion (
    idamortizacion 		serial,
    idcuentaactivo		int4,
    idcuentaamortizacion	int4,
    descamortizacion    	character varying(2000),
    nomamortizacion		character varying(200),
    fechacompra			date,
    fecha1cuota			date,
    valorcompra			float8,
    periodicidad		int4,
    numcuotas			int4,
    metodo			int4,
    nifproveedor		character varying(12),
    nomproveedor		character varying(150),
    dirproveedor		character varying(200),
    telproveedor		character varying(20),
    agrupacion			character varying(150),
    PRIMARY KEY ("idamortizacion"),
    CONSTRAINT "amortizacion_cuenta_fk" FOREIGN KEY ("idcuentaactivo")
    REFERENCES "cuenta" ("idcuenta"),
    CONSTRAINT "amortizacion_cuentaamort_fk" FOREIGN KEY ("idcuentaamortizacion")
    REFERENCES "cuenta" ("idcuenta")
);

CREATE TABLE linamortizacion (
    idlinamortizacion 		serial,
    idamortizacion		int4,
    idasiento			int4,
    ejercicio			int4,
    fechaprevista		date,
    cantidad			float8,
    PRIMARY KEY ("idlinamortizacion"),
    CONSTRAINT "linamoritzacion_amortizacion_fk" FOREIGN KEY ("idamortizacion")
    REFERENCES "amortizacion" ("idamortizacion"),
    CONSTRAINT "linamortizacion_asiento_fk" FOREIGN KEY ("idasiento")
    REFERENCES "asiento" ("idasiento")
);    
    
CREATE TABLE ejercicios (
    ejercicio 			int4,
    periodo			smallint,
    bloqueado			boolean
);    

ALTER TABLE asiento ADD COLUMN clase smallint;



INSERT INTO ejercicios (ejercicio, periodo, bloqueado) VALUES (2003, 0, FALSE);
INSERT INTO ejercicios (ejercicio, periodo, bloqueado) VALUES (2003, 1, FALSE);
INSERT INTO ejercicios (ejercicio, periodo, bloqueado) VALUES (2003, 2, FALSE);
INSERT INTO ejercicios (ejercicio, periodo, bloqueado) VALUES (2003, 3, FALSE);
INSERT INTO ejercicios (ejercicio, periodo, bloqueado) VALUES (2003, 4, FALSE);
INSERT INTO ejercicios (ejercicio, periodo, bloqueado) VALUES (2003, 5, FALSE);
INSERT INTO ejercicios (ejercicio, periodo, bloqueado) VALUES (2003, 6, FALSE);
INSERT INTO ejercicios (ejercicio, periodo, bloqueado) VALUES (2003, 7, FALSE);
INSERT INTO ejercicios (ejercicio, periodo, bloqueado) VALUES (2003, 8, FALSE);
INSERT INTO ejercicios (ejercicio, periodo, bloqueado) VALUES (2003, 9, FALSE);
INSERT INTO ejercicios (ejercicio, periodo, bloqueado) VALUES (2003, 10, FALSE);
INSERT INTO ejercicios (ejercicio, periodo, bloqueado) VALUES (2003, 11, FALSE);
INSERT INTO ejercicios (ejercicio, periodo, bloqueado) VALUES (2003, 12, FALSE);

INSERT INTO ejercicios (ejercicio, periodo, bloqueado) VALUES (2004, 0, FALSE);
INSERT INTO ejercicios (ejercicio, periodo, bloqueado) VALUES (2004, 1, FALSE);
INSERT INTO ejercicios (ejercicio, periodo, bloqueado) VALUES (2004, 2, FALSE);
INSERT INTO ejercicios (ejercicio, periodo, bloqueado) VALUES (2004, 3, FALSE);
INSERT INTO ejercicios (ejercicio, periodo, bloqueado) VALUES (2004, 4, FALSE);
INSERT INTO ejercicios (ejercicio, periodo, bloqueado) VALUES (2004, 5, FALSE);
INSERT INTO ejercicios (ejercicio, periodo, bloqueado) VALUES (2004, 6, FALSE);
INSERT INTO ejercicios (ejercicio, periodo, bloqueado) VALUES (2004, 7, FALSE);
INSERT INTO ejercicios (ejercicio, periodo, bloqueado) VALUES (2004, 8, FALSE);
INSERT INTO ejercicios (ejercicio, periodo, bloqueado) VALUES (2004, 9, FALSE);
INSERT INTO ejercicios (ejercicio, periodo, bloqueado) VALUES (2004, 10, FALSE);
INSERT INTO ejercicios (ejercicio, periodo, bloqueado) VALUES (2004, 11, FALSE);
INSERT INTO ejercicios (ejercicio, periodo, bloqueado) VALUES (2004, 12, FALSE);


DROP FUNCTION reordenaasientos();
DROP FUNCTION reordenaasientos(integer);

-- Insertamos el nuevo parametro de configuracion.
INSERT INTO configuracion (idconfiguracion, nombre, valor) VALUES (2,'Amortizacion', 'Amortizacion');

CREATE FUNCTION fainteligente() RETURNS integer 
AS '
DECLARE 
    bs RECORD;
BEGIN
-- Insertamos la nueva amortizacion, que se corresponda con el campo Amortizacion.
RAISE NOTICE ''empezamos lo bonito'';
INSERT INTO ainteligente (descripcion, comentariosasiento) VALUES (''Amortizacion'', NULL);
SELECT INTO bs currval(''ainteligente_idainteligente_seq'') AS valor;
RAISE NOTICE '' Ahora insertamos los valores %'', bs.valor;
INSERT INTO binteligente (idainteligente, iddiario, fecha, conceptocontable, codcuenta, descripcion, debe, haber, contrapartida, comentario, canal, marcaconciliacion, idc_coste) VALUES (bs.valor, NULL, ''$fechaasiento, Fecha Asiento$'', ''Amortizacion'',''$cuenta, Cuenta Amortizacion$'',NULL,''$cuota, Cuota$'',''0'',NULL,NULL,NULL,NULL,NULL );
INSERT INTO binteligente (idainteligente, iddiario, fecha, conceptocontable, codcuenta, descripcion, debe, haber, contrapartida, comentario, canal, marcaconciliacion, idc_coste) VALUES (bs.valor, NULL, ''$fechaasiento, Fecha Asiento$'', ''Amortizacion'',''$cuenta, Cuenta Amortizacion$'',NULL,''$cuota, Cuota$'',''0'',NULL,NULL,NULL,NULL,NULL );
RAISE NOTICE ''Terminamos lo bonito '';
RETURN 0;
END;
' LANGUAGE plpgsql;

SELECT fainteligente();

DROP FUNCTION fainteligente();



CREATE FUNCTION reordenaasientos(integer) RETURNS integer
    AS '
DECLARE
    ejercicio ALIAS FOR $1;
    as RECORD;
    cont integer;
BEGIN
    cont := 1;
    FOR as IN SELECT * from asiento WHERE EXTRACT(YEAR FROM fecha)=ejercicio ORDER BY fecha,clase,ordenasiento LOOP
	IF (cont <> as.ordenasiento) THEN
	    UPDATE asiento SET ordenasiento = cont WHERE idasiento = as.idasiento;
	END IF;
	cont := cont + 1;
    END LOOP;
    RETURN 0;
END;
'
    LANGUAGE plpgsql;


