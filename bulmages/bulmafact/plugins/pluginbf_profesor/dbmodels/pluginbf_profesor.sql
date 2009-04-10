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



-- ========================== VARIACION DE TARIFA =======================


CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
	as RECORD;
BEGIN

        SELECT INTO as * FROM pg_tables  WHERE tablename=''profesor'';
        IF NOT FOUND THEN

        CREATE TABLE profesor (
            idprofesor SERIAL PRIMARY KEY,
	    nombreprofesor VARCHAR NOT NULL,
	    idprovincia INTEGER REFERENCES provincia(idprovincia)
        );

        END IF;

        SELECT INTO as * FROM pg_tables  WHERE tablename=''alumno'';
        IF NOT FOUND THEN

        CREATE TABLE alumno (
	    idalumno SERIAL PRIMARY KEY,
	    nombrealumno VARCHAR NOT NULL,
	    idprovincia INTEGER REFERENCES provincia(idprovincia),
       apellido1alumno VARCHAR,
       fechanacimientoalumno DATE
        );
        END IF;

   SELECT INTO as * FROM pg_attribute WHERE attname = ''apellido1alumno'';
   IF NOT FOUND THEN
      ALTER TABLE alumno ADD COLUMN apellido1alumno VARCHAR;
      ALTER TABLE alumno ADD COLUMN fechanacimientoalumno DATE;
   END IF;



  SELECT INTO as * FROM pg_tables  WHERE tablename=''recibo'';
    IF NOT FOUND THEN
    CREATE TABLE recibo (
    idrecibo SERIAL PRIMARY KEY,
    fecharecibo DATE DEFAULT now(),
    cantrecibo NUMERIC(12,2),
    idforma_pago INTEGER REFERENCES forma_pago(idforma_pago),
    idcliente INTEGER REFERENCES cliente(idcliente)
    );
  END IF;
  
  SELECT INTO as attname, relname FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid=pg_class.oid WHERE attname=''idcliente'' AND relname=''recibo'';
   IF NOT FOUND THEN
      ALTER TABLE recibo ADD COLUMN idcliente INTEGER REFERENCES cliente(idcliente);
   END IF;

  SELECT INTO as attname, relname FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid=pg_class.oid WHERE attname=''idforma_pago'' AND relname=''recibo'';
   IF NOT FOUND THEN
      ALTER TABLE recibo ADD COLUMN idforma_pago INTEGER REFERENCES forma_pago(idforma_pago);
   END IF;

   SELECT INTO as * FROM pg_attribute WHERE attname = ''cantrecibo'';
   IF NOT FOUND THEN
      ALTER TABLE recibo ADD COLUMN cantrecibo NUMERIC(12,2);
   END IF;

   SELECT INTO as * FROM pg_attribute WHERE attname = ''fecharecibo'';
   IF NOT FOUND THEN
      ALTER TABLE recibo ADD COLUMN fecharecibo DATE DEFAULT now();
   END IF;

   SELECT INTO as * FROM pg_attribute WHERE attname = ''nombrerecibo'';
   IF FOUND THEN
      ALTER TABLE recibo DROP COLUMN nombrerecibo;
   END IF;


  SELECT INTO as * FROM pg_tables  WHERE tablename=''lrecibo'';
    IF NOT FOUND THEN
    CREATE TABLE lrecibo (
    idlrecibo SERIAL PRIMARY KEY,
    idrecibo INTEGER NOT NULL REFERENCES recibo(idrecibo),
    cantlrecibo NUMERIC(12,2),
    conceptolrecibo VARCHAR
    );
  END IF;







   SELECT INTO as * FROM pg_attribute WHERE attname = ''sociocliente'';
   IF NOT FOUND THEN
      ALTER TABLE cliente ADD COLUMN sociocliente boolean DEFAULT FALSE;
   END IF;

   SELECT INTO as * FROM pg_attribute WHERE attname = ''numsociocliente'';
   IF NOT FOUND THEN
      ALTER TABLE cliente ADD COLUMN numsociocliente int;
   END IF;


	SELECT INTO as * FROM pg_tables WHERE tablename = ''socio'';
	IF NOT FOUND THEN
	    CREATE TABLE socio (
		idsocio SERIAL PRIMARY KEY,
		nomsocio VARCHAR NOT NULL,
		numsocio VARCHAR NOT NULL,
		idcliente INTEGER REFERENCES cliente (idcliente)
	    );
	END IF;
	
           SELECT INTO as * FROM pg_tables  WHERE tablename=''alumnocliente'';
        IF NOT FOUND THEN

        CREATE TABLE alumnocliente (
       idalumnocliente SERIAL PRIMARY KEY,
       idalumno INTEGER NOT NULL REFERENCES alumno(idalumno),
       idcliente INTEGER NOT NULL REFERENCES cliente(idcliente)
        );

        END IF;
   
   
	SELECT INTO as * FROM pg_tables WHERE tablename = ''cuota'';
	IF NOT FOUND THEN
	    CREATE TABLE cuota (
		idcuota SERIAL PRIMARY KEY,
		idsocio INTEGER NOT NULL REFERENCES socio(idsocio),
		fechacuota DATE,
		cantcuota NUMERIC (12,2)
	    );
	END IF;


        SELECT INTO as * FROM pg_tables  WHERE tablename=''tipoactividad'';
        IF NOT FOUND THEN

        CREATE TABLE tipoactividad (
       idtipoactividad SERIAL PRIMARY KEY,
       nombretipoactividad VARCHAR NOT NULL
        );

        END IF;

        SELECT INTO as * FROM pg_tables  WHERE tablename=''actividad'';
        IF NOT FOUND THEN

        CREATE TABLE actividad (
	    idactividad SERIAL PRIMARY KEY,
	    idprofesor  INTEGER NOT NULL REFERENCES profesor(idprofesor),
       idtipoactividad INTEGER REFERENCES tipoactividad(idtipoactividad),
       codigoactividad VARCHAR UNIQUE NOT NULL,
	    nombreactividad VARCHAR NOT NULL,
       precioactividad NUMERIC (12,2),
       finicialactividad DATE,
       ffinalactividad DATE
        );
        END IF;

  SELECT INTO as attname, relname FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid=pg_class.oid WHERE attname=''idtipoactividad'' AND relname=''actividad'';
   IF NOT FOUND THEN
      ALTER TABLE actividad ADD COLUMN idtipoactividad INTEGER REFERENCES tipoactividad(idtipoactividad);
   END IF;

  SELECT INTO as attname, relname FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid=pg_class.oid WHERE attname=''precioactividad'' AND relname=''actividad'';
   IF NOT FOUND THEN
      ALTER TABLE actividad ADD COLUMN precioactividad NUMERIC (12,2);
      ALTER TABLE actividad ADD COLUMN finicialactividad DATE;
      ALTER TABLE actividad ADD COLUMN ffinalactividad DATE;
   END IF;

  SELECT INTO as attname, relname FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid=pg_class.oid WHERE attname=''codigoactividad'' AND relname=''actividad'';
   IF NOT FOUND THEN
      ALTER TABLE actividad ADD COLUMN codigoactividad VARCHAR;
   END IF;
   
   
   
  SELECT INTO as * FROM pg_tables  WHERE tablename=''alumnoactividad'';
        IF NOT FOUND THEN

        CREATE TABLE alumnoactividad (
       idalumnoactividad SERIAL PRIMARY KEY,
       idalumno INTEGER NOT NULL REFERENCES alumno(idalumno),
       idactividad INTEGER NOT NULL REFERENCES actividad(idactividad)
        );

        END IF;

        SELECT INTO as * FROM pg_tables  WHERE tablename=''sesionactividad'';
        IF NOT FOUND THEN
        CREATE TABLE sesionactividad (
	    idsesionactividad SERIAL PRIMARY KEY,
	    idactividad INTEGER NOT NULL REFERENCES actividad(idactividad),
	    idprofesor INTEGER NOT NULL REFERENCES profesor(idprofesor),
	    fechainsesion timestamp DEFAULT now() NOT NULL,
	    fechafinsesion timestamp DEFAULT now() NOT NULL
        );
        END IF;

        SELECT INTO as * FROM pg_tables  WHERE tablename=''cuotaporalumno'';
        IF NOT FOUND THEN

        CREATE TABLE cuotaporalumno (
       numalumnoscuotaporalumno INTEGER NOT NULL PRIMARY KEY,
       cuotacuotaporalumno NUMERIC (12,2)
        );

        END IF;


	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo "Creamos la tabla profesor."

-- ======================== Actualizada la revision de la base de datos a la version. =====================


-- Agregamos nuevos parametros de configuracion.
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	as RECORD;
BEGIN
	SELECT INTO as * FROM configuracion WHERE nombre=''PluginBf_Profesor'';
	IF FOUND THEN
		UPDATE CONFIGURACION SET valor=''0.11.1-0001'' WHERE nombre=''PluginBf_Profesor'';
	ELSE
		INSERT INTO configuracion (nombre, valor) VALUES (''PluginBf_Profesor'', ''0.11.1-0001'');
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
