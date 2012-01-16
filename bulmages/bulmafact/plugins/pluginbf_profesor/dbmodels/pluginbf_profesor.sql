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

-- 
-- Funcion que prepara la base de datos para funcionar como una AMPA
-- 

CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
    rs RECORD;

BEGIN
    SELECT INTO rs * FROM configuracion  WHERE nombre = ''PeriodoCuotas'';

    IF NOT FOUND THEN
	INSERT INTO configuracion (nombre, valor) VALUES (''PeriodoCuotas'', ''Mensual'');
    END IF;

    SELECT INTO rs * FROM pg_tables  WHERE tablename=''profesor'';

    IF NOT FOUND THEN
        CREATE TABLE profesor (
            idprofesor SERIAL PRIMARY KEY,
            nombreprofesor VARCHAR NOT NULL,
            dniprofesor VARCHAR,
            dirprofesor VARCHAR,
            cpprofesor VARCHAR,
            poblprofesor VARCHAR,
            telprofesor VARCHAR,
            faxprofesor VARCHAR,
            movilprofesor VARCHAR,
            mailprofesor VARCHAR,
            urlprofesor VARCHAR,
            estudiosprofesor TEXT,
            comentprofesor TEXT,
            idprovincia INTEGER REFERENCES provincia(idprovincia)
        );

    END IF;

    SELECT INTO rs * FROM pg_tables  WHERE tablename=''clase'';

    IF NOT FOUND THEN
        CREATE TABLE clase (
            idclase SERIAL PRIMARY KEY,
            nomclase VARCHAR NOT NULL
        );
    END IF;

    SELECT INTO rs * FROM pg_tables  WHERE tablename=''alumno'';

    IF NOT FOUND THEN
        CREATE TABLE alumno (
            idalumno SERIAL PRIMARY KEY,
            dnialumno VARCHAR,
            nombrealumno VARCHAR NOT NULL,
            idprovincia INTEGER REFERENCES provincia(idprovincia),
            apellido1alumno VARCHAR,
            apellido2alumno VARCHAR,
            diralumno VARCHAR,
            cpalumno VARCHAR,
            municipioalumno VARCHAR,
            observalumno VARCHAR,
            telalumno VARCHAR,
            movilalumno VARCHAR,
            emailalumno VARCHAR,
            fechanacimientoalumno DATE,
            idclase INTEGER REFERENCES clase (idclase),
            activoalumno BOOLEAN
        );
        
    END IF;

    SELECT INTO rs attname, relname FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid=pg_class.oid WHERE attname=''idclase'' AND relname=''alumno'';

    IF NOT FOUND THEN
        ALTER TABLE alumno ADD COLUMN idclase INTEGER REFERENCES clase (idclase);
    END IF;

    SELECT INTO rs * FROM pg_attribute WHERE attname = ''activoalumno'';

    IF NOT FOUND THEN
        ALTER TABLE alumno ADD COLUMN activoalumno BOOLEAN;
    END IF;

    SELECT INTO rs * FROM pg_attribute WHERE attname = ''apellido1alumno'';

    IF NOT FOUND THEN
        ALTER TABLE alumno ADD COLUMN apellido1alumno VARCHAR;
        ALTER TABLE alumno ADD COLUMN fechanacimientoalumno DATE;
    END IF;

    SELECT INTO rs * FROM pg_attribute WHERE attname = ''apellido2alumno'';

    IF NOT FOUND THEN
        ALTER TABLE alumno ADD COLUMN apellido2alumno VARCHAR;
        ALTER TABLE alumno ADD COLUMN diralumno VARCHAR;
        ALTER TABLE alumno ADD COLUMN cpalumno VARCHAR;
        ALTER TABLE alumno ADD COLUMN municipioalumno VARCHAR;
        ALTER TABLE alumno ADD COLUMN observalumno VARCHAR;
        ALTER TABLE alumno ADD COLUMN telalumno VARCHAR;
        ALTER TABLE alumno ADD COLUMN movilalumno VARCHAR;
        ALTER TABLE alumno ADD COLUMN emailalumno VARCHAR;
    END IF;

    SELECT INTO rs * FROM pg_tables  WHERE tablename=''recibo'';

    IF NOT FOUND THEN
        CREATE TABLE recibo (
            idrecibo SERIAL PRIMARY KEY,
            fecharecibo DATE DEFAULT now(),
            cantrecibo NUMERIC(12,2),
            idforma_pago INTEGER REFERENCES forma_pago(idforma_pago),
            descrecibo VARCHAR,
            pagadorecibo BOOLEAN DEFAULT false,
            devueltorecibo BOOLEAN DEFAULT false,
            idcliente INTEGER REFERENCES cliente(idcliente),
            idbanco INTEGER REFERENCES banco(idbanco)
        );
    END IF;
  
    SELECT INTO rs attname, relname FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid=pg_class.oid WHERE attname=''idcliente'' AND relname=''recibo'';

    IF NOT FOUND THEN
        ALTER TABLE recibo ADD COLUMN idcliente INTEGER REFERENCES cliente(idcliente);
    END IF;

    SELECT INTO rs attname, relname FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid=pg_class.oid WHERE attname=''devueltorecibo'' AND relname=''recibo'';

    IF NOT FOUND THEN
        ALTER TABLE recibo ADD COLUMN devueltorecibo BOOLEAN DEFAULT false;
    END IF;

    SELECT INTO rs attname, relname FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid=pg_class.oid WHERE attname=''idforma_pago'' AND relname=''recibo'';

    IF NOT FOUND THEN
        ALTER TABLE recibo ADD COLUMN idforma_pago INTEGER REFERENCES forma_pago(idforma_pago);
    END IF;

    SELECT INTO rs attname, relname FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid=pg_class.oid WHERE attname=''descrecibo'' AND relname=''recibo'';

    IF NOT FOUND THEN
        ALTER TABLE recibo ADD COLUMN descrecibo VARCHAR;
    END IF;

    SELECT INTO rs * FROM pg_attribute WHERE attname = ''cantrecibo'';

    IF NOT FOUND THEN
        ALTER TABLE recibo ADD COLUMN cantrecibo NUMERIC(12,2);
    END IF;

    SELECT INTO rs * FROM pg_attribute WHERE attname = ''fecharecibo'';

    IF NOT FOUND THEN
        ALTER TABLE recibo ADD COLUMN fecharecibo DATE DEFAULT now();
    END IF;

    SELECT INTO rs * FROM pg_attribute WHERE attname = ''nombrerecibo'';

    IF FOUND THEN
        ALTER TABLE recibo DROP COLUMN nombrerecibo;
    END IF;

    SELECT INTO rs * FROM pg_tables  WHERE tablename=''lrecibo'';

    IF NOT FOUND THEN
        CREATE TABLE lrecibo (
            idlrecibo SERIAL PRIMARY KEY,
            idrecibo INTEGER NOT NULL REFERENCES recibo(idrecibo),
            cantlrecibo NUMERIC(12,2),
	    ordenlrecibo integer,
            conceptolrecibo VARCHAR
        );
    END IF;

    SELECT INTO rs * FROM pg_attribute WHERE attname = ''ordenlrecibo'';

    IF NOT FOUND THEN
            ALTER TABLE lrecibo ADD COLUMN ordenlrecibo integer;
    END IF;

    SELECT INTO rs * FROM pg_attribute WHERE attname = ''sociocliente'';

    IF NOT FOUND THEN
            ALTER TABLE cliente ADD COLUMN sociocliente boolean DEFAULT FALSE;
    END IF;

    SELECT INTO rs * FROM pg_attribute WHERE attname = ''numsociocliente'';

    IF NOT FOUND THEN
        ALTER TABLE cliente ADD COLUMN numsociocliente int;
    END IF;

    SELECT INTO rs * FROM pg_attribute WHERE attname = ''apellido1cliente'';

    IF NOT FOUND THEN
        ALTER TABLE cliente ADD COLUMN apellido1cliente varchar;
    END IF;

    SELECT INTO rs * FROM pg_attribute WHERE attname = ''apellido2cliente'';
    IF NOT FOUND THEN
        ALTER TABLE cliente ADD COLUMN apellido2cliente varchar;
    END IF;

    SELECT INTO rs * FROM pg_tables WHERE tablename = ''socio'';
    IF NOT FOUND THEN
        CREATE TABLE socio (
            idsocio SERIAL PRIMARY KEY,
            nomsocio VARCHAR NOT NULL,
            numsocio VARCHAR NOT NULL,
            idcliente INTEGER REFERENCES cliente (idcliente)
        );
    END IF;
    
    SELECT INTO rs * FROM pg_tables  WHERE tablename=''alumnocliente'';
    IF NOT FOUND THEN
        CREATE TABLE alumnocliente (
            idalumnocliente SERIAL PRIMARY KEY,
            idalumno INTEGER NOT NULL REFERENCES alumno(idalumno),
            idcliente INTEGER NOT NULL REFERENCES cliente(idcliente),
	    porcentalumnocliente NUMERIC(12,2) DEFAULT 100
        );
    END IF;
   
    SELECT INTO rs * FROM pg_attribute WHERE attname = ''porcentalumnocliente'';
    IF NOT FOUND THEN
        ALTER TABLE alumnocliente ADD COLUMN porcentalumnocliente NUMERIC(12,2) DEFAULT 100;
    END IF;


    SELECT INTO rs * FROM pg_tables WHERE tablename = ''cuota'';
    IF NOT FOUND THEN
        CREATE TABLE cuota (
            idcuota SERIAL PRIMARY KEY,
            idsocio INTEGER NOT NULL REFERENCES socio(idsocio),
            fechacuota DATE,
            cantcuota NUMERIC (12,2)
        );
    END IF;

    SELECT INTO rs * FROM pg_tables  WHERE tablename=''tipoactividad'';
    IF NOT FOUND THEN
        CREATE TABLE tipoactividad (
            idtipoactividad SERIAL PRIMARY KEY,
            nombretipoactividad VARCHAR NOT NULL
        );
    END IF;

    SELECT INTO rs * FROM pg_tables  WHERE tablename=''actividad'';
    IF NOT FOUND THEN
        CREATE TABLE actividad (
            idactividad SERIAL PRIMARY KEY,
            idprofesor  INTEGER NOT NULL REFERENCES profesor(idprofesor),
            idtipoactividad INTEGER REFERENCES tipoactividad(idtipoactividad),
            codigoactividad VARCHAR UNIQUE NOT NULL,
            nombreactividad VARCHAR NOT NULL,
            precioactividad NUMERIC (12,2),
            vigenteactividad BOOLEAN NOT NULL DEFAULT false,
            comentariosactividad TEXT,
            finicialactividad DATE,
            ffinalactividad DATE,
	    lunesactividad BOOLEAN DEFAULT FALSE,
	    martesactividad BOOLEAN DEFAULT FALSE,
	    miercolesactividad BOOLEAN DEFAULT FALSE,
	    juevesactividad BOOLEAN DEFAULT FALSE,
	    viernesactividad BOOLEAN DEFAULT FALSE,
	    sabadoactividad BOOLEAN DEFAULT FALSE,
	    domingoactividad BOOLEAN DEFAULT FALSE,
	    periodoactividad INTERVAL,
       hinicialactividad VARCHAR,
       duracionactividad VARCHAR,
       lugaractividad VARCHAR,
       diractividad VARCHAR

        );
    END IF;
    
    SELECT INTO rs * FROM pg_tables  WHERE tablename=''faltaasistenciaalumnoactividad'';
    IF NOT FOUND THEN
        CREATE TABLE faltaasistenciaalumnoactividad (
            idfaltaasistenciaalumnoactividad SERIAL PRIMARY KEY,
            idactividad INTEGER REFERENCES actividad(idactividad),
            idalumno INTEGER REFERENCES alumno(idalumno),
            fechafaltaasistenciaalumnoactividad DATE NOT NULL DEFAULT now()
        );
    END IF;

    SELECT INTO rs attname, relname FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid=pg_class.oid WHERE attname=''periodoactividad'' AND relname=''actividad'';
    IF NOT FOUND THEN
        ALTER TABLE actividad ADD COLUMN periodoactividad INTERVAL;
    END IF;

    SELECT INTO rs attname, relname FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid=pg_class.oid WHERE attname=''idtipoactividad'' AND relname=''actividad'';
    IF NOT FOUND THEN
        ALTER TABLE actividad ADD COLUMN idtipoactividad INTEGER REFERENCES tipoactividad(idtipoactividad);
    END IF;
    
    SELECT INTO rs attname, relname FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid=pg_class.oid WHERE attname=''lugaractividad'' AND relname=''actividad'';
    IF NOT FOUND THEN
        ALTER TABLE actividad ADD COLUMN lugaractividad VARCHAR;
        ALTER TABLE actividad ADD COLUMN diractividad VARCHAR;
    END IF;


    SELECT INTO rs attname, relname FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid=pg_class.oid WHERE attname=''hinicialactividad'' AND relname=''actividad'';
    IF NOT FOUND THEN
        ALTER TABLE actividad ADD COLUMN hinicialactividad VARCHAR;
        ALTER TABLE actividad ADD COLUMN duracionactividad VARCHAR;
    END IF;

    SELECT INTO rs attname, relname FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid=pg_class.oid WHERE attname=''precioactividad'' AND relname=''actividad'';
    IF NOT FOUND THEN
        ALTER TABLE actividad ADD COLUMN precioactividad NUMERIC (12,2);
        ALTER TABLE actividad ADD COLUMN finicialactividad DATE;
        ALTER TABLE actividad ADD COLUMN ffinalactividad DATE;
    END IF;

    SELECT INTO rs attname, relname FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid=pg_class.oid WHERE attname=''lunesactividad'' AND relname=''actividad'';
    IF NOT FOUND THEN
        ALTER TABLE actividad ADD COLUMN lunesactividad BOOLEAN;
        ALTER TABLE actividad ADD COLUMN martesactividad BOOLEAN;
        ALTER TABLE actividad ADD COLUMN miercolesactividad BOOLEAN;
        ALTER TABLE actividad ADD COLUMN juevesactividad BOOLEAN;
        ALTER TABLE actividad ADD COLUMN viernesactividad BOOLEAN;
        ALTER TABLE actividad ADD COLUMN sabadoactividad BOOLEAN;
        ALTER TABLE actividad ADD COLUMN domingoactividad BOOLEAN;
    END IF;

    
    SELECT INTO rs attname, relname FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid=pg_class.oid WHERE attname=''codigoactividad'' AND relname=''actividad'';
    IF NOT FOUND THEN
        ALTER TABLE actividad ADD COLUMN codigoactividad VARCHAR;
    END IF;
      
    SELECT INTO rs * FROM pg_tables  WHERE tablename=''alumnoactividad'';
    IF NOT FOUND THEN
        CREATE TABLE alumnoactividad (
            idalumnoactividad SERIAL PRIMARY KEY,
            idalumno INTEGER NOT NULL REFERENCES alumno(idalumno),
            idactividad INTEGER NOT NULL REFERENCES actividad(idactividad),
	    bancoalumnoactividad VARCHAR
        );
    END IF;

    SELECT INTO rs attname, relname FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid=pg_class.oid WHERE attname=''bancoalumnoactividad'' AND relname=''alumnoactividad'';
    IF NOT FOUND THEN
        ALTER TABLE alumnoactividad ADD COLUMN bancoalumnoactividad VARCHAR;
    END IF;


    SELECT INTO rs * FROM pg_tables  WHERE tablename=''clienteactividad'';
    IF NOT FOUND THEN
        CREATE TABLE clienteactividad (
            idclienteactividad SERIAL PRIMARY KEY,
            idcliente INTEGER NOT NULL REFERENCES cliente(idcliente),
            idactividad INTEGER NOT NULL REFERENCES actividad(idactividad)
        );
    END IF;


    
    SELECT INTO rs * FROM pg_tables  WHERE tablename=''sesionactividad'';
    IF NOT FOUND THEN
        CREATE TABLE sesionactividad (
            idsesionactividad SERIAL PRIMARY KEY,
            idactividad INTEGER NOT NULL REFERENCES actividad(idactividad),
            idprofesor INTEGER NOT NULL REFERENCES profesor(idprofesor),
            fechainsesion timestamp DEFAULT now() NOT NULL,
            fechafinsesion timestamp DEFAULT now() NOT NULL
        );
    END IF;

    SELECT INTO rs * FROM pg_tables  WHERE tablename=''cuotaporalumno'';
    IF NOT FOUND THEN
        CREATE TABLE cuotaporalumno (
            numalumnoscuotaporalumno INTEGER NOT NULL PRIMARY KEY,
            cuotacuotaporalumno NUMERIC (12,2),
            descuentocuotaporalumno NUMERIC (12,2) DEFAULT 0
        );
    END IF;

    SELECT INTO rs attname, relname FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid=pg_class.oid WHERE attname=''descuentocuotaporalumno'' AND relname=''cuotaporalumno'';
    IF NOT FOUND THEN
        ALTER TABLE cuotaporalumno ADD COLUMN descuentocuotaporalumno NUMERIC (12,2) DEFAULT 0;
    END IF;


    SELECT INTO rs * FROM pg_tables  WHERE tablename=''cuotaporactividad'';
    IF NOT FOUND THEN
        CREATE TABLE cuotaporactividad (
            numactividadescuotaporactividad INTEGER NOT NULL PRIMARY KEY,
            descuentocuotaporactividad NUMERIC (12,2) DEFAULT 0
        );
    END IF;


    SELECT INTO rs * FROM pg_tables  WHERE tablename=''jdirectiva'';
    IF NOT FOUND THEN
        CREATE TABLE jdirectiva (
            idjdirectiva SERIAL PRIMARY KEY,
            fechainjdirectiva date DEFAULT now() NOT NULL,
            fechafinjdirectiva date DEFAULT now() NOT NULL
        );
    END IF;
    
    SELECT INTO rs * FROM pg_tables  WHERE tablename=''miembrojdirectiva'';
    IF NOT FOUND THEN
        CREATE TABLE miembrojdirectiva (
            idmiembrojdirectiva SERIAL PRIMARY KEY,
            fechainmiembrojdirectiva date DEFAULT now() NOT NULL,
            fechafinmiembrojdirectiva date DEFAULT now() NOT NULL,
            idjdirectiva INTEGER NOT NULL REFERENCES jdirectiva(idjdirectiva),
            cargomiembrojdirectiva VARCHAR NOT NULL,
            idcliente INTEGER NOT NULL REFERENCES cliente(idcliente)
        );
    END IF;
    
    SELECT INTO rs attname, relname FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid=pg_class.oid WHERE attname=''cargomiembrojdirectiva'' AND relname=''miembrojdirectiva'';
    IF NOT FOUND THEN
        ALTER TABLE miembrojdirectiva ADD COLUMN cargomiembrojdirectiva VARCHAR;
    END IF;

    SELECT INTO rs * FROM pg_tables  WHERE tablename=''reunion'';
    IF NOT FOUND THEN
        CREATE TABLE reunion (
            idreunion SERIAL PRIMARY KEY,
            tiporeunion VARCHAR NOT NULL,
            fecha1convocatoriareunion date DEFAULT now() NOT NULL,
            fecha2convocatoriareunion date DEFAULT now(),
            hora1convocatoriareunion varchar NOT NULL,
            hora2convocatoriareunion varchar,
            conceptoreunion TEXT,
            resolucionreunion TEXT
        );
    END IF;

    SELECT INTO rs attname, relname FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid=pg_class.oid WHERE attname=''hora1convocatoriareunion'' AND relname=''reunion'';
    IF NOT FOUND THEN
        ALTER TABLE reunion ADD COLUMN hora1convocatoriareunion varchar NOT NULL;
        ALTER TABLE reunion ADD COLUMN hora2convocatoriareunion varchar;
    END IF;

    SELECT INTO rs attname, relname FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid=pg_class.oid WHERE attname=''resolucionreunion'' AND relname=''reunion'';
    IF NOT FOUND THEN
        ALTER TABLE reunion ADD COLUMN resolucionreunion TEXT;
    END IF;

    SELECT INTO rs * FROM pg_tables  WHERE tablename=''ordendiareunion'';
    IF NOT FOUND THEN
        CREATE TABLE ordendiareunion (
            idordendiareunion SERIAL PRIMARY KEY,
            idreunion INTEGER NOT NULL REFERENCES reunion(idreunion),
            conceptoordendiareunion VARCHAR,
            textoordendiareunion TEXT,
            resolucionordendiareunion TEXT
        );
    END IF;

    SELECT INTO rs * FROM pg_tables  WHERE tablename=''asistentereunion'';
    IF NOT FOUND THEN
        CREATE TABLE asistentereunion (
            idasistentereunion SERIAL PRIMARY KEY,
            idreunion INTEGER NOT NULL REFERENCES reunion(idreunion),
            idcliente INTEGER NOT NULL REFERENCES cliente(idcliente)
        );
    END IF;

    SELECT INTO rs * FROM pg_tables  WHERE tablename=''comision'';
    IF NOT FOUND THEN
        CREATE TABLE comision (
            idcomision SERIAL PRIMARY KEY,
            fechaincomision date DEFAULT now() NOT NULL,
            fechafincomision date,
	    nomcomision varchar NOT NULL,
	    comentcomision VARCHAR 
        );
    END IF;

    SELECT INTO rs * FROM pg_tables  WHERE tablename=''miembrocomision'';
    IF NOT FOUND THEN
        CREATE TABLE miembrocomision (
            idmiembrocomision SERIAL PRIMARY KEY,
            fechainmiembrocomision date DEFAULT now() NOT NULL,
            fechafinmiembrocomision date DEFAULT now() NOT NULL,
            idcomision INTEGER NOT NULL REFERENCES comision(idcomision),
            cargomiembrocomision VARCHAR NOT NULL,
            idcliente INTEGER NOT NULL REFERENCES cliente(idcliente)
        );
    END IF;



-- Agregamos el campo de cuota por reemision de recibo
   SELECT INTO rs * FROM configuracion WHERE nombre=''CuotaReemisionRecibo'';
   IF NOT FOUND THEN
      INSERT INTO configuracion (nombre, valor) VALUES (''CuotaReemisionRecibo'', ''3.5'');
   END IF;

-- Quitamos restricciones para la tabla de clientes
-- y agregamos unas menos restrictivas
   SELECT INTO rs * FROM pg_constraint WHERE conname =''cliente_codcliente_key'';
   IF FOUND THEN
      ALTER TABLE cliente DROP constraint cliente_nomcliente_key;
      ALTER TABLE cliente DROP CONSTRAINT cliente_codcliente_key;
      ALTER TABLE cliente DROP CONSTRAINT cliente_cifcliente_key;
   END IF;

   SELECT INTO rs * FROM pg_constraint WHERE conname =''cliente_fapac_key'';
   IF NOT FOUND THEN
      ALTER TABLE cliente ADD CONSTRAINT cliente_fapac_key UNIQUE (nomcliente, cifcliente, apellido1cliente, apellido2cliente);
   END IF;

    RETURN 0;
    
END;
'   LANGUAGE plpgsql;

SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo "Creamos la tabla profesor 333333."

-- 
-- Agregamos tres tristes triggers para que se actualice debidamente el campo
-- cantrecibo a partir de los cambios en las lineas de este
-- 
SELECT drop_if_exists_proc('actualizacantrecibo_insert', '');

\echo -n ':: Funcion que calcula el campo de totales de un recibo (INSERT)'
CREATE FUNCTION actualizacantrecibo_insert() RETURNS "trigger"
AS '
DECLARE
    rs RECORD;
    total numeric(12,2);

BEGIN
    SELECT INTO total SUM(cantlrecibo) FROM lrecibo WHERE idrecibo = NEW.idrecibo;
    UPDATE recibo SET cantrecibo = total WHERE idrecibo = NEW.idrecibo;
    
    RETURN NEW;
END;
' LANGUAGE plpgsql;

\echo -n ':: Disparador que recalcula el campo de totales de un recibo (INSERT)'
CREATE TRIGGER actualizacantrecibotrigger_insert
    AFTER INSERT ON lrecibo
    FOR EACH ROW
    EXECUTE PROCEDURE actualizacantrecibo_insert();

SELECT drop_if_exists_proc('actualizacantrecibo_update', '');
\echo -n ':: Funcion que calcula/actualiza el campo de totales de un recibo (UPDATE)'
CREATE FUNCTION actualizacantrecibo_update() RETURNS "trigger"
AS '
DECLARE
    rs RECORD;
    total numeric(12,2);

BEGIN
    SELECT INTO total SUM(cantlrecibo) FROM lrecibo WHERE idrecibo = NEW.idrecibo;
    UPDATE recibo SET cantrecibo = total WHERE idrecibo = NEW.idrecibo;
    
    RETURN NEW;
END;
' LANGUAGE plpgsql;

\echo -n ':: Disparador que calcula/actualiza el campo de totales de un recibo (UPDATE)'
CREATE TRIGGER actualizacantrecibotrigger_update
    AFTER UPDATE ON lrecibo
    FOR EACH ROW
    EXECUTE PROCEDURE actualizacantrecibo_update();
    
SELECT drop_if_exists_proc('actualizacantrecibo_delete', '');
\echo -n ':: Funcion que calcula/actualiza el campo de totales de un recibo (DELETE)'
CREATE FUNCTION actualizacantrecibo_delete() RETURNS "trigger"
AS '
DECLARE
    rs RECORD;
    total numeric(12,2);

BEGIN
    SELECT INTO total SUM(cantlrecibo) FROM lrecibo WHERE idrecibo = OLD.idrecibo;
    UPDATE recibo SET cantrecibo = total WHERE idrecibo = OLD.idrecibo;
    
    RETURN OLD;
END;
' LANGUAGE plpgsql;

\echo -n ':: Disparador que calcula/actualiza el campo de totales de un recibo (DELETE)'
CREATE TRIGGER actualizacantrecibotrigger_delete
    AFTER DELETE ON lrecibo
    FOR EACH ROW
    EXECUTE PROCEDURE actualizacantrecibo_delete();

-- ======================== Actualizada la revision de la base de datos a la version. =====================


-- Agregamos nuevos parametros de configuracion.
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	rs RECORD;

BEGIN
	SELECT INTO rs * FROM configuracion WHERE nombre=''PluginBf_Profesor'';
	IF FOUND THEN
		UPDATE CONFIGURACION SET valor=''0.12.1-0003'' WHERE nombre=''PluginBf_Profesor'';
	ELSE
		INSERT INTO configuracion (nombre, valor) VALUES (''PluginBf_Profesor'', ''0.12.1-0003'');
	END IF;
	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT actualizarevision();
DROP FUNCTION actualizarevision() CASCADE;
\echo "Actualizada la revision de la base de datos a la version"

DROP FUNCTION drop_if_exists_table(text) CASCADE;
DROP FUNCTION drop_if_exists_proc(text,text) CASCADE;

COMMIT;
