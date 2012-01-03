--
-- Modificación de campos y funciones de la BD para la adaptacion para el plugin de Trazabilidad
--
\echo "********* INICIADO FICHERO DE ESTRUCTURA DEL PLUGIN DE TRAZABILIDAD *********"

\echo ":: Establecemos los mensajes minimos a avisos y otros parametros ... "
\echo -n ":: "
SET client_min_messages TO WARNING;
SET log_min_messages TO WARNING;
-- SET log_error_verbosity TO TERSE;
BEGIN;

--
-- Estas primeras funciones cambiar� los tipos de columnas que est� como flotantes a NUMERIC.
-- Se trata de un parche que se desea aplicar para almacenar los tipos monetarios
-- ya que actualmente se encuantran almacenados como 'doubles' y es preferible
-- que se almacenen como tipo 'numeric'.
-- Todas devuelven como valor num�ico el nmero de filas influenciadas por el cambio
-- NOTA: Si alguien sabe como pasar por par�etro un nombre de tabla y campo a modificar se
-- har� mucho m� sencillito porque s�o habr� que implementar un funci� ya que siempre
-- hay que hacer lo mismo.
--

--
-- Función auxiliar para borrar funciones limpiamente
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


CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
	rs RECORD;

BEGIN
	SELECT INTO rs * FROM pg_attribute  WHERE attname=''lotelalbaranp'';

	IF NOT FOUND THEN
		ALTER TABLE lalbaranp ADD COLUMN lotelalbaranp VARCHAR;
		ALTER TABLE lfacturap ADD COLUMN lotelfacturap VARCHAR;
		ALTER TABLE lalbaran ADD COLUMN lotelalbaran VARCHAR;
		ALTER TABLE lfactura ADD COLUMN lotelfactura VARCHAR;
	END IF;

	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo "Agregamos los campos para Trazabilidad en las tablas necesarias."

CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
        rs RECORD;

BEGIN
--	DROP TABLE movimiento;
        SELECT INTO rs * FROM pg_attribute WHERE attname = ''idmovimiento'';
        IF NOT FOUND THEN
                CREATE TABLE movimiento (
                        idmovimiento SERIAL PRIMARY KEY,
                        fechamovimiento DATE NOT NULL,
                        idarticulo integer NOT NULL REFERENCES articulo(idarticulo),
                        cantidadmovimiento NUMERIC(12,2) NOT NULL,
                        lotemovimiento VARCHAR,
                        idalmacen integer NOT NULL REFERENCES almacen(idalmacen),
                        idlalbaran INTEGER REFERENCES lalbaran(numlalbaran),
			idlalbaranp INTEGER REFERENCES lalbaranp(numlalbaranp)
                );
        END IF;
        RETURN 0;
END;
' LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo ":: Agregamos la tabla de movimientos ... "





\echo -n ":: Funcion que actualiza el total de una factura al borrar una linea de detalle ... "
SELECT drop_if_exists_proc ('trazabilidadalbaranpi','');
CREATE OR REPLACE FUNCTION trazabilidadalbaranpi() returns TRIGGER
AS '
DECLARE
    rs RECORD;

BEGIN
    SELECT INTO rs idalmacen FROM albaranp WHERE idalbaranp = NEW.idalbaranp;
    INSERT INTO movimiento (idarticulo, cantidadmovimiento, idlalbaranp, fechamovimiento, lotemovimiento, idalmacen) VALUES (NEW.idarticulo, NEW.cantlalbaranp, NEW.numlalbaranp, NOW(), NEW.lotelalbaranp, rs.idalmacen);

    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ":: Disparador que actualiza el total de una factura al borrar o actualizar una linea de detalle ... "
CREATE TRIGGER trazabilidadalbaranpi_trigger
    AFTER INSERT ON lalbaranp
    FOR EACH ROW
    EXECUTE PROCEDURE trazabilidadalbaranpi();

\echo -n ":: Funcion que actualiza el total de una factura al borrar una linea de detalle ... "
SELECT drop_if_exists_proc ('trazabilidadalbaranpu','');
CREATE OR REPLACE FUNCTION trazabilidadalbaranpu() returns TRIGGER
AS '
DECLARE
    rs RECORD;

BEGIN
    SELECT INTO rs idalmacen FROM albaranp WHERE idalbaranp = NEW.idalbaranp;
    UPDATE movimiento SET cantidadmovimiento = NEW.cantlalbaranp, idalmacen = rs.idalmacen WHERE idlalbaranp = NEW.numlalbaranp;
    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ":: Disparador que actualiza el total de una factura al borrar o actualizar una linea de detalle ... "
CREATE TRIGGER trazabilidadalbaranpu_trigger
    AFTER UPDATE ON lalbaranp
    FOR EACH ROW
    EXECUTE PROCEDURE trazabilidadalbaranpu();


\echo -n ":: Funcion que actualiza el total de una factura al borrar una linea de detalle ... "
SELECT drop_if_exists_proc ('trazabilidadalbaranpd','');
CREATE OR REPLACE FUNCTION trazabilidadalbaranpd() returns TRIGGER
AS '
DECLARE
BEGIN
    DELETE FROM movimiento WHERE idlalbaranp = OLD.numlalbaranp;
    RETURN OLD;
END;
' LANGUAGE plpgsql;


\echo -n ":: Disparador que actualiza el total de una factura al borrar o actualizar una linea de detalle ... "
CREATE TRIGGER trazabilidadalbaranpd_trigger
    BEFORE DELETE ON lalbaranp
    FOR EACH ROW
    EXECUTE PROCEDURE trazabilidadalbaranpd();



-- ==============================================================================

\echo -n ":: Funcion que actualiza el total de una factura al borrar una linea de detalle ... "
SELECT drop_if_exists_proc ('trazabilidadalbarani','');
CREATE OR REPLACE FUNCTION trazabilidadalbarani() returns TRIGGER
AS '
DECLARE
    rs RECORD;

BEGIN
    SELECT INTO rs idalmacen FROM albaran WHERE idalbaran = NEW.idalbaran;
    INSERT INTO movimiento (idarticulo, cantidadmovimiento, idlalbaran, fechamovimiento, lotemovimiento, idalmacen) VALUES (NEW.idarticulo, -NEW.cantlalbaran, NEW.numlalbaran, NOW(), NEW.lotelalbaran, rs.idalmacen);
    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ":: Disparador que actualiza el total de una factura al borrar o actualizar una linea de detalle ... "
CREATE TRIGGER trazabilidadalbarani_trigger
    AFTER INSERT ON lalbaran
    FOR EACH ROW
    EXECUTE PROCEDURE trazabilidadalbarani();

\echo -n ":: Funcion que actualiza el total de una factura al borrar una linea de detalle ... "
SELECT drop_if_exists_proc ('trazabilidadalbaranu','');
CREATE OR REPLACE FUNCTION trazabilidadalbaranu() returns TRIGGER
AS '
DECLARE
    rs RECORD;

BEGIN
    SELECT INTO rs idalmacen FROM albaran WHERE idalbaran = NEW.idalbaran;
    UPDATE movimiento SET cantidadmovimiento = -NEW.cantlalbaran, idalmacen = rs.idalmacen WHERE idlalbaran = NEW.numlalbaran;
    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ":: Disparador que actualiza el total de una factura al borrar o actualizar una linea de detalle ... "
CREATE TRIGGER trazabilidadalbaranu_trigger
    AFTER UPDATE ON lalbaran
    FOR EACH ROW
    EXECUTE PROCEDURE trazabilidadalbaranu();


\echo -n ":: Funcion que actualiza el total de una factura al borrar una linea de detalle ... "
SELECT drop_if_exists_proc ('trazabilidadalbarand','');
CREATE OR REPLACE FUNCTION trazabilidadalbarand() returns TRIGGER
AS '
DECLARE
BEGIN
    DELETE FROM movimiento WHERE idlalbaran = OLD.numlalbaran;
    RETURN OLD;
END;
' LANGUAGE plpgsql;


\echo -n ":: Disparador que actualiza el total de una factura al borrar o actualizar una linea de detalle ... "
CREATE TRIGGER trazabilidadalbarand_trigger
    BEFORE DELETE ON lalbaran
    FOR EACH ROW
    EXECUTE PROCEDURE trazabilidadalbarand();


-- ==============================================================================


-- Agregamos nuevos parametros de configuraci�.
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	rs RECORD;

BEGIN
	SELECT INTO rs * FROM configuracion WHERE nombre=''PluginBf_Trazabilidad'';

	IF FOUND THEN
		UPDATE CONFIGURACION SET valor=''0.9.1'' WHERE nombre=''PluginBf_Trazabilidad'';
	ELSE
		INSERT INTO configuracion (nombre, valor) VALUES (''PluginBf_Trazabilidad'', ''0.9.1'');
	END IF;

	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT actualizarevision();
DROP FUNCTION actualizarevision() CASCADE;
\echo "Actualizada la revision de la base de datos a la version 0.9.1"


DROP FUNCTION drop_if_exists_table(text) CASCADE;
DROP FUNCTION drop_if_exists_proc(text,text) CASCADE;


COMMIT;
