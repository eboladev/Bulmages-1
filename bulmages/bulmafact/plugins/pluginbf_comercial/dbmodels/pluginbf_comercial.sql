
SET client_min_messages TO warning;
--
-- Modificacion de campos y funciones de la BD para la adaptacion para el plugin de ComercialBF.
--
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
	SELECT INTO rs * FROM pg_attribute WHERE attname = ''idzonacomercial'';
	IF NOT FOUND THEN
		CREATE TABLE zonacomercial (
			idzonacomercial SERIAL PRIMARY KEY,
			nomzonacomercial VARCHAR
		);
		CREATE TABLE rutacomercial (
			idrutacomercial SERIAL PRIMARY KEY,
			fecharutacomercial DATE,
			idcliente integer REFERENCES cliente(idcliente),
			comentariosrutacomercial VARCHAR,
			horarutacomercial TIME,
			refrutacomercial VARCHAR
		);
		CREATE TABLE incidenciacomercial (
			idincidenciacomercial SERIAL PRIMARY KEY,
			fechaincidenciacomercial DATE,
			idcliente integer REFERENCES cliente(idcliente),
			comentincidenciacomercial VARCHAR,
			refincidenciacomercial VARCHAR,
			estadoincidenciacomercial INTEGER,
			horaincidenciacomercial TIME,
			idrutacomercial integer REFERENCES rutacomercial(idrutacomercial),
			idpedidocliente integer REFERENCES pedidocliente(idpedidocliente),
			idcobro integer REFERENCES cobro(idcobro),
			idtrabajador integer REFERENCES trabajador(idtrabajador)
		);

		ALTER TABLE trabajador  ADD COLUMN tipotrabajador  VARCHAR;
		ALTER TABLE trabajador  ADD COLUMN idzonacomercial INTEGER;
		ALTER TABLE trabajador  ADD CONSTRAINT idzonacomercialfk FOREIGN KEY (idzonacomercial) REFERENCES zonacomercial(idzonacomercial);
		ALTER TABLE cliente ADD COLUMN comentcomercialcliente VARCHAR;
		ALTER TABLE cliente ADD COLUMN idzonacomercial INTEGER;
		ALTER TABLE cliente ADD CONSTRAINT idzonacomercialfk FOREIGN KEY (idzonacomercial) REFERENCES zonacomercial(idzonacomercial);
		ALTER TABLE cliente ADD COLUMN periodocomercialcliente INTERVAL;
		ALTER TABLE cliente ALTER COLUMN periodocomercialcliente SET DEFAULT ''@ 1 weeks'';
		UPDATE cliente SET periodocomercialcliente = ''@ 1 weeks'';
		ALTER TABLE cliente ALTER COLUMN periodocomercialcliente SET NOT NULL;
		ALTER TABLE cliente ADD COLUMN fechabasecomercialcliente DATE;
		ALTER TABLE cliente ALTER COLUMN fechabasecomercialcliente SET DEFAULT now();
		UPDATE cliente SET fechabasecomercialcliente = now();
		ALTER TABLE cliente ALTER COLUMN fechabasecomercialcliente SET NOT NULL;
		ALTER TABLE cliente ADD COLUMN horarutacomercialcliente TIME;
		ALTER TABLE cliente ALTER COLUMN horarutacomercialcliente SET DEFAULT  ''08:00'';
		UPDATE cliente SET horarutacomercialcliente = ''08:00'';
		ALTER TABLE cliente ALTER COLUMN horarutacomercialcliente SET NOT NULL;
	END IF;
	RETURN 0;
END;
' LANGUAGE plpgsql;

SELECT aux();

DROP FUNCTION aux() CASCADE;
\echo ':: Agregamos los campos y tablas para el plugin ComercialBF ... '


CREATE OR REPLACE FUNCTION crearutacomercial(date) RETURNS INTEGER AS '
DECLARE
	fecha ALIAS FOR $1;
	client RECORD;
	nfechabase DATE;

BEGIN
	FOR client IN SELECT * FROM cliente LOOP
		nfechabase := client.fechabasecomercialcliente;
		RAISE NOTICE ''Nuevo Cliente'';
		WHILE nfechabase <= fecha LOOP
			RAISE NOTICE '' Nuevo ciclo % %'', nfechabase, fecha;
			IF fecha = nfechabase THEN
				-- Aqui toca crear la hoja de ruta.
				-- Al final actualizamos la fecha base para que los calculos sean siempre lo menos
				-- complejos y largos posible.
				RAISE NOTICE ''Hemos llegado al quiz'';
				-- UPDATE cliente set fechabasecomercialcliente = nfechabase WHERE idcliente = client.idcliente;
			END IF;
			nfechabase := nfechabase + client.periodocomercialcliente;
--			RAISE NOTICE '' Calculo % %'', nfechabase, fecha;
		END LOOP;
	END LOOP;
	RETURN 0;
END;
' LANGUAGE plpgsql;
\echo ':: Creamos la funcion de crear las rutas comerciales de forma automatica ... '


-- Agregamos nuevos parametros de configuracion.
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	rs RECORD;

BEGIN
	SELECT INTO rs * FROM configuracion WHERE nombre = ''PluginBf_Comercial'';

	IF FOUND THEN
		UPDATE CONFIGURACION SET valor = ''0.5.9'' WHERE nombre = ''PluginBf_Comercial'';
	ELSE
		INSERT INTO configuracion (nombre, valor) VALUES (''PluginBf_Comercial'', ''0.5.9'');
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
