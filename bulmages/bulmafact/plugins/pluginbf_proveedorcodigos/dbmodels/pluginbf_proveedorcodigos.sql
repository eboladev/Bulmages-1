--
-- Modificacion de campos y funciones de la BD para la adaptacion para el plugin de Alias.
--
\echo "********* INICIADO FICHERO DE ESTRUCTURA DEL PLUGIN DE CODIGOS DE PROVEEDOR *********"

\echo ":: Establecemos los mensajes minimos a avisos y otros parametros ... "
\echo -n ":: "
SET client_min_messages TO WARNING;
SET log_min_messages TO WARNING;
-- SET log_error_verbosity TO TERSE;
BEGIN;

--
-- Estas primeras funciones cambiaran los tipos de columnas que estan como flotantes a NUMERIC.
-- Se trata de un parche que se desea aplicar para almacenar los tipos monetarios
-- ya que actualmente se encuantran almacenados como 'doubles' y es preferible
-- que se almacenen como tipo 'numeric'.
-- Todas devuelven como valor numerico el nmero de filas influenciadas por el cambio
-- NOTA: Si alguien sabe como pasar por parametro un nombre de tabla y campo a modificar se
-- hara mucho mas sencillito porque solo habra que implementar una funcion ya que siempre
-- hay que hacer lo mismo.
--

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

CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
        rs RECORD;

BEGIN
        SELECT INTO rs * FROM pg_attribute WHERE attname = ''idcodprov'';
        IF NOT FOUND THEN
                CREATE TABLE codprov (
                        idcodprov SERIAL PRIMARY KEY,
                        idarticulo integer NOT NULL REFERENCES articulo(idarticulo),
			idproveedor integer NOT NULL REFERENCES proveedor(idproveedor),
                        cadcodprov VARCHAR
                );
        END IF;
        RETURN 0;
END;
' LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo ":: Agregada la tabla de Alias ... "

CREATE OR REPLACE FUNCTION drop_index(text) RETURNS INTEGER AS '
DECLARE
        nom_index ALIAS FOR $1;
        rs RECORD;

BEGIN
        SELECT INTO rs * FROM pg_class WHERE relname = $1; 
        IF FOUND THEN
             EXECUTE ''DROP INDEX '' || $1 ;
        END IF;
        RETURN 0;
END;
' LANGUAGE plpgsql;
SELECT drop_index('ix_codprov_cadcodprov');

create index ix_codprov_cadcodprov on codprov (cadcodprov varchar_pattern_ops );

SELECT drop_index('ix_codprov_len_codprov');
create index ix_codprov_len_codprov on codprov (length(cadcodprov));
\echo ":: Indexada la tabla de Alias ... "


-- ==============================================================================


-- Agregamos nuevos parametros de configuracion.
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	rs RECORD;
BEGIN
	SELECT INTO rs * FROM configuracion WHERE nombre=''PluginBf_CodigosProveedor'';
	IF FOUND THEN
		UPDATE CONFIGURACION SET valor=''0.13.2'' WHERE nombre=''PluginBf_CodigosProveedor'';
	ELSE
		INSERT INTO configuracion (nombre, valor) VALUES (''PluginBf_CodigosProveedor'', ''0.13.2'');
	END IF;
	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT actualizarevision();
DROP FUNCTION actualizarevision() CASCADE;
\echo "Actualizada la revision de la base de datos a la version 0.13.1"


DROP FUNCTION drop_if_exists_table(text) CASCADE;
DROP FUNCTION drop_if_exists_proc(text,text) CASCADE;


COMMIT;
