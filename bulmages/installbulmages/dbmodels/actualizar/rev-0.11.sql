--
-- ACTUALIZACION DE LA BASE DE DATOS DE BULMACONT
--


-- Ocultamos las noticias
SET client_min_messages TO warning;


BEGIN;

--
-- Estas primeras funciones cambiaran los tipos de columnas que estan como flotantes a NUMERIC.
-- Se trata de un parche que se desea aplicar para almacenar los tipos monetarios
-- ya que actualmente se encuantran almacenados como 'doubles' y es preferible
-- que se almacenen como tipo 'numeric'.
-- Todas devuelven como valor numerico el nmero de filas influenciadas por el cambio
-- NOTA: Si alguien sabe como pasar por parametro un nombre de tabla y campo a modificar se
-- hara mucho mas sencillito porque solo habran que implementar un funcion ya que siempre
-- hay que hacer lo mismo.
--

--
-- Función auxiliar para borrar funciones limpiamente
--
CREATE OR REPLACE FUNCTION drop_if_exists_table(text) RETURNS INTEGER AS '
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


CREATE OR REPLACE FUNCTION drop_if_exists_proc(text, text) RETURNS INTEGER AS '
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

-- SELECT drop_if_exists_proc ('calculacodigocompletoarticulo','');

-- ======================== COMPROBACION DE CUAL ES LA ULTIMA VERSION ==================================

CREATE OR REPLACE FUNCTION compruebarevision() RETURNS INTEGER AS '
DECLARE
	as RECORD;
BEGIN
	SELECT INTO as * FROM configuracion WHERE nombre=''DatabaseRevision'' AND ( valor LIKE ''0.11.1%'' OR valor = ''0.10.1-0005'');
	IF FOUND THEN
		RETURN 0;
	ELSE
		RAISE EXCEPTION ''La base de datos no tiene la version adecuada para aplicar este parche'';
		RETURN -1;		 
	END IF;
END;
'   LANGUAGE plpgsql;
SELECT compruebarevision();
DROP FUNCTION compruebarevision() CASCADE;
\echo "Comprobada la revision"






-- ================================= A PARTIR DE AQUI EL PARCHE=========================
-- =====================================================================================

\echo -n ':: Funcion cerrar asiento ... '
SELECT drop_if_exists_proc('cierraasiento','integer');
CREATE FUNCTION cierraasiento(integer) RETURNS integer
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
	IF mrecord.tdebe = 0 THEN
		RETURN -1;
	END IF;
     ELSE
	RETURN -1;
     END IF;

     -- // Linia afegida per Josep B.
    FOR mrecord IN SELECT * from borrador WHERE idasiento = id_asiento LOOP
	INSERT INTO apunte (codigoborrador, idasiento, iddiario, fecha, conceptocontable, idcuenta, descripcion, debe,
		    haber, contrapartida, comentario, idcanal, marcaconciliacion, idc_coste, idtipoiva, orden) VALUES
		    (mrecord.codigoborrador, mrecord.idasiento, mrecord.iddiario, mrecord.fecha, mrecord.conceptocontable,
		    mrecord.idcuenta, mrecord.descripcion, mrecord.debe, mrecord.haber, mrecord.contrapartida,
		    mrecord.comentario, mrecord.idcanal, mrecord.marcaconciliacion, mrecord.idc_coste, mrecord.idtipoiva,
		    mrecord.orden);
    END LOOP;
    -- Cuando cerramos el asiento, tambien recalculamos todas las contrapartidas.	
    PERFORM contraasiento(id_asiento);
    RETURN 1;
END;
'
    LANGUAGE plpgsql;



-- ================================== FIN PARCHE. ACTUALIZACION  =======================
-- =====================================================================================

-- Agregamos nuevos parametros de configuracion
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	as RECORD;
BEGIN
	SELECT INTO as * FROM configuracion WHERE nombre = ''DatabaseRevision'';
	IF FOUND THEN
		UPDATE CONFIGURACION SET valor = ''0.11.1-0001'' WHERE nombre = ''DatabaseRevision'';
	ELSE
		INSERT INTO configuracion (nombre, valor) VALUES (''DatabaseRevision'', ''0.11.1-0001'');
	END IF;
	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT actualizarevision();
DROP FUNCTION actualizarevision() CASCADE;
\echo "Actualizada la revision de la base de datos a la version 0.11.1"

DROP FUNCTION drop_if_exists_table(text) CASCADE;
DROP FUNCTION drop_if_exists_proc(text, text) CASCADE;


COMMIT;

