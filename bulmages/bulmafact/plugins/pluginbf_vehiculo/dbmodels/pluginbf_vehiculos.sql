
SET client_min_messages TO warning;
--
-- Modificacion de campos y funciones de la BD para la adaptacion para el plugin de ComercialBF.
--
\echo -n ':: Instalacion Plugin Vehiculos ... '
\echo -n ':: Iniciamos transaccion ... '
BEGIN;

--
-- Estas primeras funciones cambiaran los tipos de columnas que estan como flotantes a NUMERIC.
-- Se trata de un parche que se desea aplicar para almacenar los tipos monetarios
-- ya que actualmente se encuantran almacenados como 'doubles' y es preferible
-- que se almacenen como tipo 'numeric'.
-- Todas devuelven como valor numerico el numero de filas influenciadas por el cambio
-- NOTA: Si alguien sabe como pasar por parametro un nombre de tabla y campo a modificar se
-- haran mucho mas sencillito porque solo habra que implementar un funcion ya que siempre
-- hay que hacer lo mismo.
--

--
-- Funcion auxiliar para borrar funciones limpiamente.
--
CREATE OR REPLACE FUNCTION drop_if_exists_table(text) RETURNS INTEGER AS '
DECLARE
    tbl_name ALIAS FOR $1;

BEGIN
    IF (select count(*) from pg_tables where tablename = $1) THEN
	EXECUTE ''DROP TABLE '' || $1;
	RETURN 1;
    END IF;

    RETURN 0;
END;
' language 'plpgsql';


CREATE OR REPLACE FUNCTION drop_if_exists_proc(text, text) RETURNS INTEGER AS '
DECLARE
    proc_name ALIAS FOR $1;
    proc_params ALIAS FOR $2;

BEGIN
    IF (select count(*) from pg_proc where proname = $1) THEN
	EXECUTE ''DROP FUNCTION '' || $1 || ''(''||$2||'') CASCADE'';
	RETURN 1;
    END IF;

    RETURN 0;
END;
' language 'plpgsql';


CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
	rs RECORD;

BEGIN
	SELECT INTO rs * FROM pg_attribute WHERE attname = ''idvehiculo'';

	IF NOT FOUND THEN
		CREATE TABLE vehiculo (
			idvehiculo SERIAL PRIMARY KEY,
			matriculavehiculo VARCHAR NOT NULL,
			marcavehiculo VARCHAR,
			modelovehiculo VARCHAR,
			bastidorvehiculo VARCHAR,
			colorvehiculo VARCHAR,
			fechacompravehiculo DATE
			preciovehiculo NUMERIC(12,2)
		);
		CREATE TABLE mantvehiculo (
			idmantvehiculo SERIAL PRIMARY KEY,
			fechamantvehiculo DATE,
			conceptomantvehiculo VARCHAR,
			preciomantvehiculo NUMERIC(12,2),
			idvehiculo INT REFERENCES vehiculo(idvehiculo)
		);
	END IF;

	RETURN 0;
END;
' LANGUAGE plpgsql;

SELECT aux();

DROP FUNCTION aux() CASCADE;
\echo ':: Agregamos los campos y tablas para el plugin PluginVehiculosBF ... '


-- Agregamos nuevos parametros de configuracion.
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	rs RECORD;

BEGIN
	SELECT INTO rs * FROM configuracion WHERE nombre = ''PluginBf_Vehiculos'';

	IF FOUND THEN
		UPDATE CONFIGURACION SET valor = ''0.5.9'' WHERE nombre = ''PluginBf_Vehiculos'';
	ELSE
		INSERT INTO configuracion (nombre, valor) VALUES (''PluginBf_Vehiculos'', ''0.5.9'');
	END IF;

	RETURN 0;
END;
' LANGUAGE plpgsql;

SELECT actualizarevision();

DROP FUNCTION actualizarevision() CASCADE;
\echo ':: Actualizada la revision de la base de datos a la version 0.5.9 ... '


DROP FUNCTION drop_if_exists_table(text) CASCADE;
DROP FUNCTION drop_if_exists_proc(text, text) CASCADE;


\echo -n ':: Finalizamos la transaccion ... '
COMMIT;

