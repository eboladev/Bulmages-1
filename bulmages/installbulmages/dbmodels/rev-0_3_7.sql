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


-- DROP TABLE linamortizacion;
-- DROP TABLE amortizacion;
-- DROP SEQUENCE amortizacion_idamortizacion_seq;
-- DROP SEQUENCE linamortizacion_idlinamortizacion_seq;

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
    
ALTER TABLE asiento ADD COLUMN clase int4;

DROP FUNCTION reordenaasientos();

CREATE FUNCTION reordenaasientos() RETURNS integer
    AS '
DECLARE
    as RECORD;
    cont integer;
BEGIN
    cont := 1;
    FOR as IN SELECT * from asiento ORDER BY clase,fecha,ordenasiento LOOP
	IF (cont <> as.ordenasiento) THEN
	    UPDATE asiento SET ordenasiento = cont WHERE idasiento = as.idasiento;
	END IF;
	cont := cont + 1;
    END LOOP;
    RETURN 0;
END;
'
    LANGUAGE plpgsql;


