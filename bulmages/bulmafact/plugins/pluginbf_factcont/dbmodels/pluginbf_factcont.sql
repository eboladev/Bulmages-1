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
--
-- Modificacion de campos y funciones de la BD para la adaptacion para el plugin de profesores
--
\echo "********* INICIADO FICHERO DE ESTRUCTURA DE ASOCIACIONES DE PADRES DE ALUMNOS *********"

\echo ":: Establecemos los mensajes minimos a avisos y otros parametros ... "
\echo -n ":: "
SET client_min_messages TO WARNING;
SET log_min_messages TO WARNING;
-- SET log_error_verbosity TO TERSE;
BEGIN;

--
-- Funcion auxiliar para borrar funciones limpiamente
--
create or replace function drop_if_exists_table (text) returns INTEGER AS '
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


create or replace function drop_if_exists_proc (text,text) returns INTEGER AS '
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


\echo '********* INICIADO FICHERO DE ESTRUCTURA DE LA BASE DE DATOS DE BULMACONT *********'


CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
    rs RECORD;

BEGIN


-- Si ya se han creado las tablas entonces no lo hago.

    SELECT INTO rs * FROM pg_tables  WHERE tablename=''canal'';

    IF NOT FOUND THEN

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
    padre integer REFERENCES c_coste(idc_coste),
    imputacion boolean,
    debe numeric(12, 2) DEFAULT 0,
    haber numeric(12, 2) DEFAULT 0
);


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




-- El campo ordenasiento se puede dejar en nulo solo porque luego el trigger lo reasigna. Con un default
-- sera ma adecuado pero no se como se implementa un default tan complicado.

CREATE TABLE asiento (
    idasiento serial PRIMARY KEY,
    descripcion character varying(100),
    fecha date,
    comentariosasiento character varying(2000),
    ordenasiento integer,
    clase smallint DEFAULT 0
);


CREATE TABLE apunte (
    idapunte serial PRIMARY KEY,
    codigoborrador integer,
    idasiento integer NOT NULL REFERENCES asiento(idasiento),
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
    ordenapunte integer,
    punteo boolean DEFAULT false
);


CREATE TABLE borrador (
    idborrador serial PRIMARY KEY,
    codigoborrador integer,
    idasiento integer NOT NULL REFERENCES asiento(idasiento),
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
    ordenborrador integer
);





CREATE TABLE ainteligente (
    idainteligente serial PRIMARY KEY,
    descripcion character varying(100),
    comentariosasiento character varying(2000)
);


CREATE TABLE binteligente (
    idbinteligente serial PRIMARY KEY,
    idainteligente integer,
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




CREATE TABLE ejercicios (
    ejercicio integer,
    periodo smallint,
    bloqueado boolean
);


END IF;
-- Hasta aqui la creacion de las tablas. Luego viene su extinción.


    SELECT INTO rs attname, relname FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid=pg_class.oid WHERE attname=''idcuenta'' AND relname=''cliente'';

    IF NOT FOUND THEN
        ALTER TABLE cliente ADD COLUMN idcuenta INTEGER REFERENCES cuenta(idcuenta);
        ALTER TABLE proveedor ADD COLUMN idcuenta INTEGER REFERENCES cuenta(idcuenta);
        ALTER TABLE trabajador ADD COLUMN idcuenta INTEGER REFERENCES cuenta(idcuenta);
        ALTER TABLE banco ADD COLUMN idcuenta INTEGER REFERENCES cuenta(idcuenta);
        ALTER TABLE almacen ADD COLUMN idcuenta INTEGER REFERENCES cuenta(idcuenta);
        ALTER TABLE familia ADD COLUMN idcuenta INTEGER REFERENCES cuenta(idcuenta);

    END IF;

    SELECT INTO rs attname, relname FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid=pg_class.oid WHERE attname=''idcuenta'' AND relname=''forma_pago'';

    IF NOT FOUND THEN
        ALTER TABLE forma_pago ADD COLUMN idcuenta INTEGER REFERENCES cuenta(idcuenta);

    END IF;

    SELECT INTO rs attname, relname FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid=pg_class.oid WHERE attname=''idasiento'' AND relname=''factura'';
    IF NOT FOUND THEN
        ALTER TABLE factura ADD COLUMN idasiento INTEGER REFERENCES asiento(idasiento);
    END IF;


    SELECT INTO rs attname, relname FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid=pg_class.oid WHERE attname=''idasiento'' AND relname=''cobro'';
    IF NOT FOUND THEN
        ALTER TABLE cobro ADD COLUMN idasiento INTEGER REFERENCES asiento(idasiento);
    END IF;


    SELECT INTO rs attname, relname FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid=pg_class.oid WHERE attname=''idasiento'' AND relname=''facturap'';
    IF NOT FOUND THEN
        ALTER TABLE facturap ADD COLUMN idasiento INTEGER REFERENCES asiento(idasiento);
    END IF;

    SELECT INTO rs attname, relname FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid=pg_class.oid WHERE attname=''idcuentav'' AND relname=''familia'';
    IF NOT FOUND THEN
        ALTER TABLE familia ADD COLUMN idcuentav INTEGER REFERENCES cuenta(idcuenta);
    END IF;

    SELECT INTO rs attname, relname FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid=pg_class.oid WHERE attname=''idasiento'' AND relname=''pago'';
    IF NOT FOUND THEN
        ALTER TABLE pago ADD COLUMN idasiento INTEGER REFERENCES asiento(idasiento);
    END IF;

    RETURN 0;
    
END;
'   LANGUAGE plpgsql;

SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo "Creamos la parte de contabilidad"




\echo -n ':: Funcion ID cuenta ... '
CREATE OR REPLACE FUNCTION id_cuenta(character varying) RETURNS integer
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
\echo "Creamos la funcion de contabilidad id_cuenta"


\echo -n ':: Funcion nivel de codigo de cuenta ... '
\echo -n ':: Usamos la longitud del codigo para determinarlo ya que los sistemas contables usan esa notacion. Podria usarse el nivel en el arbol de cuentas y seria mas general y correcto'
CREATE OR REPLACE FUNCTION nivel(character varying) RETURNS integer
    AS '
DECLARE
    codcuenta ALIAS FOR $1;
BEGIN
    RETURN LENGTH(codcuenta);
END;
'
    LANGUAGE plpgsql;

\echo "Creamos la funcion de contabilidad nivel"




\echo "Vamos a crear la funcion abreasiento"
CREATE OR REPLACE FUNCTION abreasiento(integer) RETURNS integer
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
\echo "Funcion abreasiento"


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
	INSERT INTO apunte (codigoborrador, idasiento,  fecha, conceptocontable, idcuenta, descripcion, debe,
		    haber, contrapartida, comentario, idcanal, marcaconciliacion, idc_coste, ordenapunte) VALUES
		    (mrecord.codigoborrador, mrecord.idasiento, mrecord.fecha, mrecord.conceptocontable,
		    mrecord.idcuenta, mrecord.descripcion, mrecord.debe, mrecord.haber, mrecord.contrapartida,
		    mrecord.comentario, mrecord.idcanal, mrecord.marcaconciliacion, mrecord.idc_coste, 
		    mrecord.ordenborrador);
    END LOOP;
    -- Cuando cerramos el asiento, tambien recalculamos todas las contrapartidas.	
    -- PERFORM contraasiento(id_asiento);
    RETURN 1;
END;
'
    LANGUAGE plpgsql;
\echo "Funcion cierraasiento"



SELECT drop_if_exists_proc('restriccionesasiento','');

\echo -n ":: Funcion restricciones asiento ... "
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


\echo -n ":: Disparador restricciones asiento ... "
CREATE TRIGGER restriccionesasientotrigger
   BEFORE INSERT OR UPDATE ON asiento
   FOR EACH ROW
   EXECUTE PROCEDURE restriccionesasiento();


-- ******************************************************
-- FIN DEL APARTADO DE COMPROBACIONES.
-- ******************************************************
-- ******************************************************

\echo "********* FIN FICHERO DE ESTRUCTURA DE LA BASE DE DATOS DE BULMACONT *********"

-- ================================== ACTUALIZACION  ===================================
-- =====================================================================================

-- Agregamos nuevos parametros de configuracion
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	rs RECORD;
BEGIN
	SELECT INTO rs * FROM configuracion WHERE nombre = ''PluginBf_FactCont'';
	IF FOUND THEN
		UPDATE CONFIGURACION SET valor = ''0.15.0-0001'' WHERE nombre = ''PluginBf_FactCont'';
	ELSE
		INSERT INTO configuracion (nombre, valor) VALUES (''PluginBf_FactCont'', ''0.15.0-0001'');
	END IF;
	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT actualizarevision();
DROP FUNCTION actualizarevision() CASCADE;
\echo "Actualizada la revision de la base de datos a la version 0.15.0"

\echo -n ':: '
COMMENT ON SCHEMA public IS 'Standard public schema';

\echo -n ':: '
COMMIT;


