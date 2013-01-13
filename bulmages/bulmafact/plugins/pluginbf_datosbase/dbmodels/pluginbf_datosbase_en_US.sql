--
-- Modificación de campos y funciones de la BD para la adaptacion para el plugin de Trazabilidad
--
\echo "********* INICIADO FICHERO DE ESTRUCTURA DEL PLUGIN DE DATOS BASE *********"

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
-- Todas devuelven como valor numeico el nmero de filas influenciadas por el cambio
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


CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS $$
DECLARE
	bs RECORD;
	ds RECORD;
	fpago RECORD;
	prov RECORD;
BEGIN

	SELECT INTO ds * FROM tipo_iva;
	IF NOT FOUND THEN
            INSERT INTO tipo_iva (desctipo_iva) VALUES ('Exempt');
            INSERT INTO tipo_iva (desctipo_iva) VALUES ('Super Reduced');
            INSERT INTO tipo_iva (desctipo_iva) VALUES ('Reduced');
            INSERT INTO tipo_iva (desctipo_iva) VALUES ('General');
	
	    SELECT INTO bs idtipo_iva FROM tipo_iva WHERE desctipo_iva='Exempt';
            IF FOUND THEN
		INSERT INTO tasa_iva (idtipo_iva, porcentasa_iva, fechatasa_iva, porcentretasa_iva) VALUES (bs.idtipo_iva, 0, '01/01/1973', 0);
    	    END IF;

            SELECT INTO bs idtipo_iva FROM tipo_iva WHERE desctipo_iva='Super Reduced';
	    IF FOUND THEN
	        INSERT INTO tasa_iva (idtipo_iva, porcentasa_iva, fechatasa_iva, porcentretasa_iva) VALUES (bs.idtipo_iva, 4, '01/01/1973', 0.5);
	    END IF;

            SELECT INTO bs idtipo_iva FROM tipo_iva WHERE desctipo_iva='Reduced';
	    IF FOUND THEN
	        INSERT INTO tasa_iva (idtipo_iva, porcentasa_iva, fechatasa_iva, porcentretasa_iva) VALUES (bs.idtipo_iva, 7, '01/01/1973', 2);
           INSERT INTO tasa_iva (idtipo_iva, porcentasa_iva, fechatasa_iva, porcentretasa_iva) VALUES (bs.idtipo_iva, 8, '01/07/2010', 2);
	    END IF;

	    SELECT INTO bs idtipo_iva FROM tipo_iva WHERE desctipo_iva='General';
	    IF FOUND THEN
	        INSERT INTO tasa_iva (idtipo_iva, porcentasa_iva, fechatasa_iva, porcentretasa_iva) VALUES (bs.idtipo_iva, 16, '01/01/1973', 4);
           INSERT INTO tasa_iva (idtipo_iva, porcentasa_iva, fechatasa_iva, porcentretasa_iva) VALUES (bs.idtipo_iva, 18, '01/07/2010', 4);
	    END IF;
	END IF;
	

	SELECT INTO ds * FROM irpf;
	IF NOT FOUND THEN
            INSERT INTO irpf (fechairpf, tasairpf) VALUES ('01/01/1901', 15);
            INSERT INTO irpf (fechairpf, tasairpf) VALUES ('01/01/2011', 19);
            INSERT INTO irpf (fechairpf, tasairpf) VALUES ('01/01/2012', 21);
	END IF;

	SELECT INTO ds * FROM almacen;
	IF NOT FOUND THEN
	    INSERT INTO almacen (codigoalmacen, nomalmacen) VALUES ('000', 'Main');
	END IF;
	
	SELECT INTO ds * FROM familia;
	IF NOT FOUND THEN
	    INSERT INTO familia (codigofamilia, nombrefamilia) VALUES ('000', 'Several');
	END IF;
	
	SELECT INTO ds * FROM tipo_articulo;
	IF NOT FOUND THEN
	    INSERT INTO tipo_articulo (codtipo_articulo, desctipo_articulo) VALUES ('000', 'Generic');
	END IF;

	SELECT INTO ds * FROM articulo;
	IF NOT FOUND THEN
	    INSERT INTO articulo (codarticulo, nomarticulo, idtipo_iva, idfamilia) VALUES ('001', 'SEVERAL', 1, 1);
	END IF;
	
	SELECT INTO ds * FROM trabajador;
	IF NOT FOUND THEN
	    INSERT INTO trabajador (nomtrabajador) VALUES ('Undefined');
	END IF;
	

	SELECT INTO ds * from forma_pago;
	IF NOT FOUND THEN
	    INSERT INTO forma_pago (descforma_pago) VALUES ('Cash');
	    INSERT INTO forma_pago (descforma_pago) VALUES ('Check');
	    INSERT INTO forma_pago (descforma_pago) VALUES ('Card');
	    INSERT INTO forma_pago (descforma_pago) VALUES ('Promissory Note');
	    INSERT INTO forma_pago (descforma_pago) VALUES ('Transfer');
	END IF;

	SELECT INTO ds * FROM serie_factura;
	IF NOT FOUND THEN
	    INSERT INTO serie_factura (codigoserie_factura, descserie_factura) VALUES ('DD', 'Main');
	    INSERT INTO serie_factura (codigoserie_factura, descserie_factura) VALUES ('REC', 'Amending');
	    INSERT INTO serie_factura (codigoserie_factura, descserie_factura) VALUES ('BBB', 'Billing Test');
	END IF;

	SELECT INTO ds * FROM pais;
	IF NOT FOUND THEN
	INSERT INTO pais (descpais, cod2pais, cod3pais) VALUES ('Spain','es','esp');
	INSERT INTO pais (descpais, cod2pais, cod3pais) VALUES ('Germany','de','deu');
	INSERT INTO pais (descpais, cod2pais, cod3pais) VALUES ('France','fr','fra');
	INSERT INTO pais (descpais, cod2pais, cod3pais) VALUES ('Belgium','ne','nel');
	INSERT INTO pais (descpais, cod2pais, cod3pais) VALUES ('Italy','it','ita');
	INSERT INTO pais (descpais, cod2pais, cod3pais) VALUES ('Portugal','pt','prt');
	INSERT INTO pais (descpais, cod2pais, cod3pais) VALUES ('United Kingdom','gb','gbr');
	INSERT INTO pais (descpais, cod2pais, cod3pais) VALUES ('Turkey','tr','tur');
	INSERT INTO pais (descpais, cod2pais, cod3pais) VALUES ('Poland','pl','pol');
	INSERT INTO pais (descpais, cod2pais, cod3pais) VALUES ('United States','us','usa');
    INSERT INTO pais (descpais, cod2pais, cod3pais) VALUES ('Andorra','ad','and');
    INSERT INTO pais (descpais, cod2pais, cod3pais) VALUES ('Morocco','ma','mar');
    INSERT INTO pais (descpais, cod2pais, cod3pais) VALUES ('Algeria','dz','dza');
    INSERT INTO pais (descpais, cod2pais, cod3pais) VALUES ('Netherlands','nl','nld');
    INSERT INTO pais (descpais, cod2pais, cod3pais) VALUES ('Ireland','ie','irl');
	    SELECT INTO bs idpais FROM pais WHERE cod2pais = 'es';
	    IF FOUND THEN
		INSERT INTO provincia (idpais, provincia) VALUES (bs.idpais, 'Araba');
		INSERT INTO provincia (idpais, provincia) VALUES (bs.idpais, 'Albacete');
		INSERT INTO provincia (idpais, provincia) VALUES (bs.idpais, 'Alicante');
		INSERT INTO provincia (idpais, provincia) VALUES (bs.idpais, 'Almería');
		INSERT INTO provincia (idpais, provincia) VALUES (bs.idpais, 'Asturias');
		INSERT INTO provincia (idpais, provincia) VALUES (bs.idpais, 'Avila');
		INSERT INTO provincia (idpais, provincia) VALUES (bs.idpais, 'Badajoz');
		INSERT INTO provincia (idpais, provincia) VALUES (bs.idpais, 'Illes Balears');
		INSERT INTO provincia (idpais, provincia) VALUES (bs.idpais, 'Barcelona');
		INSERT INTO provincia (idpais, provincia) VALUES (bs.idpais, 'Burgos');
		INSERT INTO provincia (idpais, provincia) VALUES (bs.idpais, 'Cáceres');
		INSERT INTO provincia (idpais, provincia) VALUES (bs.idpais, 'Cádiz');
		INSERT INTO provincia (idpais, provincia) VALUES (bs.idpais, 'Cantabria');
		INSERT INTO provincia (idpais, provincia) VALUES (bs.idpais, 'Castellón');
		INSERT INTO provincia (idpais, provincia) VALUES (bs.idpais, 'Ceuta');
		INSERT INTO provincia (idpais, provincia) VALUES (bs.idpais, 'Ciudad Real');
		INSERT INTO provincia (idpais, provincia) VALUES (bs.idpais, 'Córdoba');
		INSERT INTO provincia (idpais, provincia) VALUES (bs.idpais, 'A Coruña');
		INSERT INTO provincia (idpais, provincia) VALUES (bs.idpais, 'Cuenca');
		INSERT INTO provincia (idpais, provincia) VALUES (bs.idpais, 'Girona');
		INSERT INTO provincia (idpais, provincia) VALUES (bs.idpais, 'Granada');
		INSERT INTO provincia (idpais, provincia) VALUES (bs.idpais, 'Guadalajara');
		INSERT INTO provincia (idpais, provincia) VALUES (bs.idpais, 'Gipuzcoa');
		INSERT INTO provincia (idpais, provincia) VALUES (bs.idpais, 'Huelva');
		INSERT INTO provincia (idpais, provincia) VALUES (bs.idpais, 'Huesca');
		INSERT INTO provincia (idpais, provincia) VALUES (bs.idpais, 'Jaen');
		INSERT INTO provincia (idpais, provincia) VALUES (bs.idpais, 'León');
		INSERT INTO provincia (idpais, provincia) VALUES (bs.idpais, 'Lleida');
		INSERT INTO provincia (idpais, provincia) VALUES (bs.idpais, 'Lugo');
		INSERT INTO provincia (idpais, provincia) VALUES (bs.idpais, 'Madrid');
		INSERT INTO provincia (idpais, provincia) VALUES (bs.idpais, 'Málaga');
		INSERT INTO provincia (idpais, provincia) VALUES (bs.idpais, 'Melilla');
		INSERT INTO provincia (idpais, provincia) VALUES (bs.idpais, 'Murcia');
		INSERT INTO provincia (idpais, provincia) VALUES (bs.idpais, 'Navarra');
		INSERT INTO provincia (idpais, provincia) VALUES (bs.idpais, 'Ourense');
		INSERT INTO provincia (idpais, provincia) VALUES (bs.idpais, 'Palencia');
		INSERT INTO provincia (idpais, provincia) VALUES (bs.idpais, 'Palmas (Las)');
		INSERT INTO provincia (idpais, provincia) VALUES (bs.idpais, 'Pontevedra');
		INSERT INTO provincia (idpais, provincia) VALUES (bs.idpais, 'Rioja (La)');
		INSERT INTO provincia (idpais, provincia) VALUES (bs.idpais, 'Salamanca');
		INSERT INTO provincia (idpais, provincia) VALUES (bs.idpais, 'S.C.de Tenerife');
		INSERT INTO provincia (idpais, provincia) VALUES (bs.idpais, 'Segovia');
		INSERT INTO provincia (idpais, provincia) VALUES (bs.idpais, 'Sevilla');
		INSERT INTO provincia (idpais, provincia) VALUES (bs.idpais, 'Soria');
		INSERT INTO provincia (idpais, provincia) VALUES (bs.idpais, 'Tarragona');
		INSERT INTO provincia (idpais, provincia) VALUES (bs.idpais, 'Teruel');
		INSERT INTO provincia (idpais, provincia) VALUES (bs.idpais, 'Toledo');
		INSERT INTO provincia (idpais, provincia) VALUES (bs.idpais, 'Valencia');
		INSERT INTO provincia (idpais, provincia) VALUES (bs.idpais, 'Valladolid');
		INSERT INTO provincia (idpais, provincia) VALUES (bs.idpais, 'Bizkaia');
		INSERT INTO provincia (idpais, provincia) VALUES (bs.idpais, 'Zamora');
		INSERT INTO provincia (idpais, provincia) VALUES (bs.idpais, 'Zaragoza');
	    END IF;
	END IF;

	
	SELECT INTO ds * FROM cliente;
	IF NOT FOUND THEN

	    SELECT INTO fpago idforma_pago FROM forma_pago WHERE descforma_pago = 'Cash';
	    IF FOUND THEN
		SELECT INTO prov idprovincia FROM provincia WHERE provincia='Zaragoza';
		IF FOUND THEN
		    INSERT INTO cliente (nomcliente, cifcliente,idforma_pago, idprovincia) VALUES ('Cash Client','12345678Z',fpago.idforma_pago,prov.idprovincia);
		END IF;
	    END IF;


	END IF;

	RETURN 0;
END;
$$   LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo "Agregamos los datos de inicializacion."





DROP FUNCTION drop_if_exists_table(text) CASCADE;
DROP FUNCTION drop_if_exists_proc(text,text) CASCADE;


COMMIT;
