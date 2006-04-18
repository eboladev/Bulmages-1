-- -- ----------------------------------------------------------------------------------------
-- (C)  Joan Miquel Torres Rigo & Tomeu Borras Riera & Mateu Borras Marco, Agosto 2004
--  joanmi@bulma.net, tborras@conetxia.com mborras@conetxia.com
-- Este documento esta licenciado bajo licencia GPL, el cual no escribimos aqui por pereza.
--  ----------------------------------------------------------------------------------------
--     psql xxxx < bulmafact-0_0_1.sql
--  ---------------------------------------------------------------------------------------

SET SESSION AUTHORIZATION 'postgres';
SET search_path = public, pg_catalog;

SET DATESTYLE TO European;

CREATE FUNCTION plpgsql_call_handler() RETURNS language_handler
    AS '$libdir/plpgsql', 'plpgsql_call_handler'
    LANGUAGE c;
    
--CREATE FUNCTION plpgsql_call_handler() RETURNS language_handler
--    AS '/usr/lib/postgresql/8.0/lib/plpgsql.so', 'plpgsql_call_handler'
--    LANGUAGE c;
    
CREATE TRUSTED PROCEDURAL LANGUAGE plpgsql HANDLER plpgsql_call_handler;


-- NOTACION:
-- Considerar las siguientes opciones de codificaci�:
-- Los nombres de tabla estan escritos SIEMPRE en singular.
-- Todos los campos de una tabla terminan siempre con el nombre de la tabla (salvo las claves foraneas).
-- Las claves foraneas tienen el mismo nombre que el campo con que se corresponden en la tabla relacionada.
-- En caso de que haya diversas claves foraneas referentes al mismo campo, el criterio es que una de ellas tenga el nombre del campo con el que se corresponde y la otra tenga un nombre significativo.
-- Los campos de clave automatica empiezan por id
-- Los enums se simulan (normalmente) con campos numericos, el significado de los valores debe estar
-- explicado en este archivo.


-- La tabla de configuraci�.
-- En esta tabla se guardan parametros que el programa va a utilizar.
-- Como por ejemplo el numero de d�itos por defecto de las cuentas o el asiento inteligente que se enlaza con
-- facturacion.
-- Tiene tres campos
-- idconfiguracion: el identificador (No tiene ningn uso especial).
-- nombre: El nombre del parametro de configuracion.
-- valor: El valor que toma dicho parametro.
CREATE TABLE configuracion (
    nombre character varying(25) PRIMARY KEY,
    valor character varying(350)
);


-- Codi: Clau artificial.
-- Descripcio: Nom identificatiu o descripci�breu.
-- Dies_1T: Dies abans del primer termini computant els blocs de 30 com a mesos naturals.
-- Descompte: Descompte autom�ic per l's d'aquesta forma de pagament.
CREATE TABLE forma_pago (
   idforma_pago serial PRIMARY KEY,
   descforma_pago character varying(500),
   dias1tforma_pago integer,
   descuentoforma_pago numeric(12,2)
);


-- Codigo: Clave artificial.
-- Nombre: Nombre identificativo del almac�.
-- diralmacen: Direcci� del almac�.
-- poblalmacen: Poblaci� del almac�.
-- cpalmacenc: c�igo postal almac�.
-- telfalmacen: Tel�ono del almac�.
-- faxalmacen: Fax del almac�.
-- emailalmacen: correo electr�ico del almac�.
-- presupuestoautoalmacen el numero de presupuesto es automatico? N=No, 
-- albaranautoalmacen el numero de albaran es automatico? N=No, 
-- facturaautoalmacen el numero de  es automatico? N=No, 
CREATE TABLE almacen (
 idalmacen serial PRIMARY KEY,
 codigoalmacen numeric(5, 0) NOT NULL,
 nomalmacen character varying(50),
 diralmacen character varying(150),
 poblalmacen character varying(50),
 cpalmacen character varying(20),
 telalmacen character varying(20),
 faxalmacen character varying(20),
 emailalmacen character varying(100),
 inactivoalmacen character(1),
 UNIQUE(codigoalmacen)
);


--
-- TOC entry 47 (OID 3090354)
-- Name: staff; Type: TABLE; Schema: public; Owner: fewa
--

CREATE TABLE trabajador (
    idtrabajador serial PRIMARY KEY,
    nomtrabajador character varying NOT NULL,
    apellidostrabajador character varying,
    dirtrabajador character varying,
    nsstrabajador character varying,
    teltrabajador character varying,
    moviltrabajador character varying,
    emailtrabajador character varying,
    fototrabajador character varying,
    activotrabajador boolean DEFAULT TRUE NOT NULL
);


-- Tabla de pa�es
-- cod2: c�igo de dos d�itos
-- cod3: c�igo de tres d�itos
-- desc: descripci� del pa�
CREATE TABLE pais (
   idpais serial PRIMARY KEY,
   cod2pais character varying(2),
   cod3pais character varying(3),
	descpais character varying(50)
);


-- Tabla de monedas
-- cod2: c�igo de dos d�itos
-- cod3: c�igo de tres d�itos
-- desc: descripci� de la moneda
CREATE TABLE moneda (
   idmoneda serial PRIMARY KEY,
   cod2moneda character varying(2),
   cod3moneda character varying(3),
	descmoneda character varying(50)
);


-- Esta tabla contiene las descripciones de los ivas que se pueden aplicar.
-- Descripcio: Text descriptiu del tipus d'IVA.
CREATE TABLE tipo_iva (
   idtipo_iva serial PRIMARY KEY,
   desctipo_iva character varying(2000)
);

-- Esta tabla contiene las tasas de cada tipo de iva a partir de una fecha dada.
-- porcentasa_iva contiene el porcentaje de la tasa de iva a aplicar.
-- fechatasa_iva es la fecha de entrada en vigor del % de IVA para el tipo descrito.
CREATE TABLE tasa_iva (
	idtasa_iva serial PRIMARY KEY,
	idtipo_iva integer REFERENCES tipo_iva(idtipo_iva) NOT NULL,
	porcentasa_iva NUMERIC(5, 2) NOT NULL,
	fechatasa_iva date NOT NULL,
	UNIQUE (idtipo_iva, fechatasa_iva)
);


-- Tabla con series de Iva, c�igo i descripci�
-- B�icamente sirve para garantizar la integridad referencial en las  series de facturaci�
-- Deber�n existir en contabilidad tambien.
CREATE TABLE serie_factura (
--	idserie_factura serial PRIMARY KEY,
	codigoserie_factura character varying (6) PRIMARY KEY,
	descserie_factura character varying(50) NOT NULL,
	UNIQUE (codigoserie_factura)
);


-- codigofamilia c�igo de la familia.
-- nombrefamilia nombre de la familia
-- descfamilia descripci� extendida de la familia.
-- codcompfamilia c�igo compuesto de familia: Es la concatenaci� del c�igo de familia con sus c�igos padres. 
-- codigocompletofamilia Este campo es de s�o lectura, no se puede escribir sobre �.

CREATE TABLE familia (
	idfamilia serial PRIMARY KEY,
	codigofamilia character varying(12) NOT NULL,
 	nombrefamilia character varying(50) NOT NULL,
	descfamilia character varying(300),
	padrefamilia integer REFERENCES familia(idfamilia),
	codigocompletofamilia character varying(50) UNIQUE
);

CREATE FUNCTION calculacodigocompletofamilia () RETURNS "trigger"
AS '
DECLARE
	as RECORD;
	codigocompleto character varying(50);
BEGIN
	codigocompleto := NEW.codigofamilia;
	SELECT INTO as codigocompletofamilia FROM familia WHERE idfamilia = NEW.padrefamilia;
	IF FOUND THEN
		codigocompleto := as.codigocompletofamilia || codigocompleto;
	END IF;
        NEW.codigocompletofamilia := codigocompleto;
	RETURN NEW;
END;
' LANGUAGE plpgsql;


CREATE TRIGGER calculacodigocompletofamiliatrigger
    BEFORE INSERT OR UPDATE ON familia
    FOR EACH ROW
    EXECUTE PROCEDURE calculacodigocompletofamilia();
    
    
CREATE FUNCTION propagacodigocompletofamilia () RETURNS "trigger"
AS '
DECLARE
BEGIN
	UPDATE familia SET codigocompletofamilia=codigocompletofamilia WHERE padrefamilia = NEW.idfamilia;
	UPDATE articulo SET codigocompletoarticulo = codigocompletoarticulo WHERE articulo.idfamilia = NEW.idfamilia;
	RETURN NEW;
END;
' LANGUAGE plpgsql;

CREATE TRIGGER propagacodigocompletofamiliatrigger
    AFTER UPDATE ON familia
    FOR EACH ROW
    EXECUTE PROCEDURE propagacodigocompletofamilia();

-- Esta funci� nos da el identificador de familia dado un c�igo.
--CREATE OR REPLACE FUNCTION idfamilia (text) RETURNS "trigger"
--    AS '
--DECLARE
--    codigo ALIAS FROR $1;
--    mrecord RECORD;
--BEGIN
--    FOR mrecord IN SELECT SUM(baseiva) AS suma, SUM(ivaiva) AS sumaiva FROM iva WHERE iva.idregistroiva=NEW.idregistroiva LOOP
--    	UPDATE registroiva SET baseimp=mrecord.suma, iva=mrecord.sumaiva WHERE idregistroiva=NEW.idregistroiva;
--    END LOOP;
--    RETURN NEW;
--END;
--'    LANGUAGE plpgsql;


-- El tipo de art�ulo es una tabla que permite crear una forma alternativa de agrupar los art�ulos.
-- codigo: identificador del tipo.
-- desc:
CREATE TABLE tipo_articulo (
	idtipo_articulo serial PRIMARY KEY,
	codtipo_articulo character varying(10),
	desctipo_articulo character varying(50)
);


-- Codigo: Clave artificial.
-- Nombre: Descripci� corta del art�ulo.
-- abrev: Nombre abreviado del articulo (para tpv o cartelitos estanterias...)
-- idtipo_articulo: identificador de tipo de art�ulo que se utilizar�para agrupar art�ulos como clasificaci� alternativa a el surtido (familias).
-- Observaciones: Campo de texto para a comentarios y observaciones.
-- El campo codigocompletoarticulo s�o puede ser de lectura.
CREATE TABLE articulo (
    idarticulo serial PRIMARY KEY,
    codarticulo character varying(12),
    nomarticulo character varying(50),
    abrevarticulo character varying(30),
    obserarticulo character varying(2000),
    presentablearticulo boolean NOT NULL DEFAULT TRUE,
    controlstockarticulo boolean NOT NULL DEFAULT TRUE,
    idtipo_articulo numeric(2, 0) REFERENCES tipo_articulo(idtipo_articulo),
    idtipo_iva integer REFERENCES tipo_iva (idtipo_iva),
    codigocompletoarticulo character varying(100) UNIQUE,
    idfamilia integer REFERENCES familia(idfamilia) NOT NULL,
    stockarticulo numeric(12,2) DEFAULT 0,    
    inactivoarticulo character(1),
    -- ATENCION, este campo no da el pvp real del art�ulo, solo es una de las multiples formas de acceder al precio del articulo.
    -- Para obtener el precio de un art�ulo se debe usar la funcion articulo.
    -- Para saber el iva correspondiente a un articulo se debe usar la funci� ivaarticulo.
    pvparticulo numeric(12,2) NOT NULL DEFAULT 0
);



create or replace function is_number(varchar) returns boolean as
'select $1 ~ ''^[-+]?[0-9]+$''' strict immutable language sql;

CREATE OR REPLACE FUNCTION to_number(character varying) RETURNS INT8 AS '
DECLARE
BEGIN
	RAISE NOTICE ''to_number %'', $1;
        RETURN CAST(text($1) AS INT8);
END
' LANGUAGE 'plpgsql';

CREATE OR REPLACE FUNCTION calculacodigocompletoarticulo () RETURNS "trigger"
AS '
DECLARE
	as_ RECORD;
	codigocompleto character varying(100);
	codnumeric integer;
BEGIN
	-- Lo primero comprobamos el el código del articulo no esté vacio y de ser así lo llenamos.
	IF NEW.codarticulo = '''' THEN
		SELECT INTO as_ max(codarticulo) AS m FROM articulo WHERE idfamilia = NEW.idfamilia;
		IF FOUND THEN
			IF is_number(as_.m) THEN
				codnumeric := to_number(as_.m);
				codnumeric := codnumeric +1;
				NEW.codarticulo := CAST (codnumeric AS varchar);
				WHILE length(NEW.codarticulo) < 4 LOOP
					NEW.codarticulo := ''0'' || NEW.codarticulo;
				END LOOP;
			ELSE
				NEW.codarticulo := ''0000'';
			END IF;
		ELSE
			NEW.codarticulo = ''0000'';
		END IF;
	END IF; 

	codigocompleto := NEW.codarticulo;
	SELECT INTO as_ codigocompletofamilia FROM familia WHERE idfamilia = NEW.idfamilia;
	IF FOUND THEN
		codigocompleto := as_.codigocompletofamilia || codigocompleto;
	END IF;
        NEW.codigocompletoarticulo := codigocompleto;
	RETURN NEW;
END;
' LANGUAGE plpgsql;


CREATE TRIGGER calculacodigocompletoarticulotrigger
    BEFORE INSERT OR UPDATE ON articulo
    FOR EACH ROW
    EXECUTE PROCEDURE calculacodigocompletoarticulo();



    

	
        
-- Componentes de Art�ulo
CREATE TABLE comparticulo (
	idarticulo integer NOT NULL REFERENCES articulo(idarticulo),
	cantcomparticulo integer NOT NULL DEFAULT 1,
	idcomponente integer NOT NULL REFERENCES articulo(idarticulo),
	PRIMARY KEY (idarticulo, idcomponente)
);





-- ================================== MODULO DE TARIFAS ================================
-- =====================================================================================
-- La tabla tarifa contiene los precios de venta y oferta incluidas las ofertas MxN.
-- idalmacen: Almac� o tienda a la que corresponden los precios.
-- idarticulo: idetificador del articulo al que corresponde el precio.
-- finicio: fecha de inicio vigencia del precio
-- ffin: fecha de finalizaci� de vigencia del precio.
-- esoferta: indica si el precio es de oferta.
-- esmxn: indica si la oferta es mxn (p.e. 3x2).
-- cantidadm: cantidad de unidades para primer valor en oferta mxn (valor de unidades llevadas).
-- cantidadn: cantidad de unidades para segundo valor en oferta mxn (valor de unidades pagadas).
-- FALTA DEFINIR LAS REGLAS PARA EVITAR SOLAPAMIENTOS ENTRE OFERTAS.

CREATE TABLE tarifa (
	idtarifa serial PRIMARY KEY,
	nomtarifa varchar(60),
	finiciotarifa date,
	ffintarifa date
);



CREATE TABLE ltarifa (
	idltarifa serial PRIMARY KEY,
   	idalmacen integer NOT NULL REFERENCES almacen(idalmacen),
	idarticulo integer NOT NULL REFERENCES articulo(idarticulo),
	idtarifa integer NOT NULL REFERENCES tarifa(idtarifa),
	pvpltarifa numeric(12, 2)
);


CREATE UNIQUE INDEX indice_ltarifa ON ltarifa (idalmacen, idarticulo, idtarifa);


CREATE OR REPLACE FUNCTION pvparticuloclial(integer, integer, integer) RETURNS numeric(12,2)
AS'
DECLARE
	idarticulo ALIAS FOR $1;
	idclient ALIAS FOR $2;
	idalmacen ALIAS FOR $3;
	as RECORD;
BEGIN
	SELECT INTO AS pvpltarifa FROM ltarifa WHERE ltarifa.idarticulo = idarticulo AND ltarifa.idalmacen = idalmacen AND idtarifa IN (SELECT idtarifa FROM cliente WHERE idcliente=idclient);
	IF FOUND THEN
		RETURN as.pvpltarifa;
	END IF;


	SELECT INTO AS pvparticulo FROM  articulo WHERE articulo.idarticulo = idarticulo;
	IF FOUND THEN
		RETURN as.pvparticulo;
	END IF;
	RETURN 0.0;
END;
' LANGUAGE plpgsql;

-- ================================== MODULO DE TARIFAS ================================
-- =====================================================================================





-- Los proveedores son los que nos suminstran articulos y/o servicios.
-- COMPROVACIONS D'INTEGRITAT>Gen�iques:
-- 1 Article t�1 sol prove�or principal.
-- 1 Article t�1 sol prove�or referent.
-- CAMPOS
-- ======
-- Codi: Clau artificial.
-- Nom: Nom comercial o fiscal.
-- Nom_alternatiu: Nom comercial o fiscal.
-- CIF: Codi d'Identificaci�Fiscal.
-- CodiCli: Codi de client amb que ens facturen. �il per a identificar-nos.
-- C_Banc
-- Comentaris
-- Adre�: Adre�.
-- Poblaci� Poblaci�
-- CProv: Codi de provincia (dos primers d�its del codi postal).
-- sCP: Tres darrers d�its del codi postal.
-- Telf: Tel�on.
-- Fax: Fax.
-- Email: eMail.
-- Url: Url.
-- CompteWeb: Dades de login si disposen de tenda o tarifes en l�ia
CREATE TABLE proveedor (
   idproveedor serial PRIMARY KEY,
   nomproveedor character varying(200),
   nomaltproveedor character varying(200),
   cifproveedor character varying(12) UNIQUE,
   codicliproveedor character varying(30),
   cbancproveedor character varying(20),
   comentproveedor character varying(2000),
   dirproveedor character varying(250),
   poblproveedor character varying(50),
   cpproveedor character varying(9) NOT NULL,
   telproveedor character varying(50),
   faxproveedor character varying(50),
   emailproveedor character varying(100),
   urlproveedor character varying(100),
   clavewebproveedor character varying(100),
   inactivoproveedor character(1),
   provproveedor character varying
);


--Numero: Nmero de divisi�(clau artificial).
--Descripcio: Nom o descripci�de la divisi�
--Contactes: Nom de persona o persones de contacte.
--Comentaris
--Telf: Tel�on.
--Fax: Fax.
--Email
CREATE TABLE division (
   iddivision serial PRIMARY KEY,
   descdivision character varying(1000),
   contactosdivisioon character varying(500),
   comentdivision character varying(2000),
   teldivision character varying(20),
   faxdivision character varying(20),
   maildivision character varying(100),
   idproveedor integer NOT NULL REFERENCES proveedor(idproveedor),
   inactivodivision character(1)
);


-- El cliente siempre tiene la raz�, bueno, o por lo menos eso cree.
--Codi: Clau artificial.
--Nom: Nom comercial o fiscal.
--Nom_alternatiu: Nom comercial o fiscal.
--CIF: Codi d'Identificaci�Fiscal.
--C_Banc: Compte Bancari.
--Adr: Adre�.
--Pobl: Poblaci�
--CProv: Codi de provincia (dos primers d�its del codi postal).
--sCP: Tres darrers d�its del codi postal.
--Telf: Tel�on.
--Fax: Fax.
--Email: eMail.
--Url: Url.
--Data_alta
--Data_Baixa
---Comentaris
CREATE TABLE cliente (
   idcliente serial PRIMARY KEY,
   nomcliente character varying(100),
   nomaltcliente character varying(300),
   cifcliente character varying(200) UNIQUE,
   bancocliente character varying(35),
   dircliente character varying(100),
   poblcliente character varying(40),
   cpcliente character varying(10),
   telcliente character varying(20),
   faxcliente character varying(20),
   mailcliente character varying(100),
   urlcliente character varying(150),
   faltacliente date DEFAULT NOW(),
   fbajacliente date,
   comentcliente character varying(2000),
   inactivocliente character(1),
   provcliente character varying,
   idtarifa integer references tarifa(idtarifa)
);


CREATE TABLE cobro (
   idcobro serial PRIMARY KEY,
   idcliente integer NOT NULL REFERENCES cliente(idcliente),
   fechacobro date DEFAULT NOW(),
   cantcobro numeric(12,2) DEFAULT 0,
   refcobro character varying(12) NOT NULL,
   previsioncobro boolean DEFAULT FALSE,
   comentcobro character varying(500),
   idtrabajador integer REFERENCES trabajador(idtrabajador)
);
   

CREATE TABLE pago (
   idpago serial PRIMARY KEY,
   idproveedor integer NOT NULL REFERENCES proveedor(idproveedor),
   fechapago date DEFAULT NOW(),
   cantpago numeric(12,2) DEFAULT 0,
   refpago character varying(12) NOT NULL,
   previsionpago boolean DEFAULT FALSE,
   comentpago character varying(500),
   idtrabajador integer REFERENCES trabajador(idtrabajador)   
);

-- Any: Any en que s'efectua la comanda.
-- Numero: Nmero de comanda (comen�nt de 1 cada any).
-- Descripcio: Breu descripci�o comentari opcional.
-- Data: Data d'emisi�de la comanda.
CREATE TABLE pedido (
   idpedido serial PRIMARY KEY,
   numpedido character varying(60),
   fechapedido date,
   descpedido character varying(500),
   iddivision integer NOT NULL REFERENCES division(iddivision),
   idalmacen integer NOT NULL REFERENCES almacen(idalmacen),
   idtrabajador integer REFERENCES trabajador(idtrabajador)   
);



CREATE TABLE usuario (
    loginusuario character varying(15) PRIMARY KEY,
    nombreusuario character varying(35),
    apellido1usuario character varying(35),
    apellido2usuario character varying(35),
    claveusuario character varying(35),
    permisosusuario text
);



-- Any: Any de facturaci�
-- Numero: Nmero de factura.
-- Data Comentaris
-- Factura de prove�or.
CREATE TABLE fra_pro (
   idfra_pro serial PRIMARY KEY,
   numfra_pro character varying(60),
   fcrefra_pro date,
   comentfra_pro character varying(2000)
);



-- Albaran de proveedor
-- Any: Any en que s'efectua la comanda.
-- NumCompra: Numero de Compra (Clau artificial per poder registrar recepcions que ens arribin sense l'albar�postposant la cumplimentaci�del nmero d'albar�.
-- NumAlbara: Nmero d'albar�
-- Data: Data de l'albar�-- Recepcio: Data de recepci�
-- Comentaris
CREATE TABLE alb_pro (
   idalb_pro serial PRIMARY KEY,
   ncompraalb_pro integer,
   nalbalb_pro character varying(60),
   fcrealb_pro date,
   frecepalb_pro date,
   comentalb_pro character varying(2000),

   idfra_pro integer REFERENCES fra_pro(idfra_pro),
   idalmacen integer NOT NULL REFERENCES almacen(idalmacen)
);



-- Linea de pedido
-- Numero: Nmero de l�ia.
-- Descripcio: Descripcio de l'article.
-- Quantitat
-- PVD
-- Previsi� Data prevista de recepci�

CREATE TABLE lpedido (
   numlpedido serial PRIMARY KEY,
   desclpedido character varying(150),
   cantlpedido numeric(12,2),
   pvdlpedido numeric(12,2),
   prevlpedido date,
   ivalpedido numeric(12,2),
   descuentolpedido numeric(12,2),
   idpedido integer NOT NULL REFERENCES pedido(idpedido),
   idalb_pro integer REFERENCES alb_pro(idalb_pro),
   idarticulo integer REFERENCES articulo(idarticulo)
--      PRIMARY KEY(idpedido, numlpedido)
);




-- Entendemos que un presupuesto es una relaci� de materiales y trabajos cuantificada que
-- hacemos a petici� de un cliente determinado
-- Numero
-- Data: Data d'emisi�del presupost.
-- PersContacte: Nom de persona de contacte (si cal).
-- TelfContacte: Tel�on.
-- Venciment: Data m�ima de validesa del presupost.
-- Comentaris
--  Pressupost a clients.
CREATE TABLE presupuesto (
   idpresupuesto serial PRIMARY KEY,
   numpresupuesto integer NOT NULL UNIQUE,
   refpresupuesto character varying(12) NOT NULL,
   fpresupuesto date DEFAULT now(),
   descpresupuesto character varying(150),
   contactpresupuesto character varying(90),
   telpresupuesto character varying(20),
   vencpresupuesto date,
   comentpresupuesto character varying(3000),
   idusuari integer,
   procesadopresupuesto boolean DEFAULT FALSE,
   idcliente integer REFERENCES cliente(idcliente),
   idalmacen integer NOT NULL REFERENCES almacen(idalmacen),
   idforma_pago integer NOT NULL REFERENCES forma_pago(idforma_pago),
   idtrabajador integer REFERENCES trabajador(idtrabajador),
   bimppresupuesto numeric(12,2) DEFAULT 0,
   imppresupuesto  numeric(12,2) DEFAULT 0,
   totalpresupuesto numeric(12,2) DEFAULT 0,
   UNIQUE (idalmacen, numpresupuesto)
);

CREATE FUNCTION restriccionespresupuesto () RETURNS "trigger"
AS '
DECLARE
asd RECORD;
BEGIN
	IF NEW.fpresupuesto IS NULL THEN
		NEW.fpresupuesto := now();
	END IF;
        IF NEW.numpresupuesto IS NULL THEN
                SELECT INTO asd max(numpresupuesto) AS m FROM presupuesto;
		IF asd.m IS NOT NULL THEN	
			NEW.numpresupuesto := asd.m + 1;
		ELSE

			NEW.numpresupuesto := 1;
		END IF;			
        END IF;
	IF NEW.refpresupuesto IS NULL OR NEW.refpresupuesto = '''' THEN
		SELECT INTO asd crearef() AS m;
		IF FOUND THEN
			NEW.refpresupuesto := asd.m;
		END IF;
	END IF;
        RETURN NEW;
END;
' LANGUAGE plpgsql;


CREATE TRIGGER restriccionespresupuestotrigger
    BEFORE INSERT OR UPDATE ON presupuesto
    FOR EACH ROW
    EXECUTE PROCEDURE restriccionespresupuesto();


-- Descuento de presupuesto.
-- Numero
--Concepte: Descripci�del motiu de descompte.
--Proporcio: Percentatge a descomptar.
-- Descompte de pressupost a clients.
CREATE TABLE dpresupuesto (
   iddpresupuesto serial PRIMARY KEY,
   conceptdpresupuesto character varying(2000),
   proporciondpresupuesto numeric(12,2),
   idpresupuesto integer REFERENCES presupuesto(idpresupuesto)
   -- Falta poner el lugar donde se aplica el descuento, antes de la factura o despu� de �ta.
);



-- Linea de presupuesto
-- Numero
-- Descripcio: Descripci�de l'article en el moment de ser presupostat.
-- Quantitat
-- PVP: Preu de l'article en el moment de ser pressupostat
-- Descompte: Percentatge de descompte en l�ia.
-- Linia de pressupost a clients.
CREATE TABLE lpresupuesto (
   idlpresupuesto serial PRIMARY KEY,
   desclpresupuesto character varying(150),
   cantlpresupuesto numeric(12,2),
   pvplpresupuesto numeric(12,2),
   ivalpresupuesto numeric(12,2),
   descuentolpresupuesto numeric(12,2),
   idpresupuesto integer NOT NULL REFERENCES presupuesto(idpresupuesto),
   idarticulo integer REFERENCES articulo(idarticulo)
);

-- Falta poner por defecto el pvp y el iva


CREATE OR REPLACE FUNCTION actualizatotpres() returns TRIGGER
AS '
DECLARE
	tot NUMERIC(12,2);
	bimp NUMERIC(12,2);
	imp NUMERIC(12,2);
BEGIN
	tot := calctotalpres(NEW.idpresupuesto);
	bimp := calcbimppres(NEW.idpresupuesto);
	imp := calcimpuestospres(NEW.idpresupuesto);
	UPDATE presupuesto SET totalpresupuesto = tot, bimppresupuesto = bimp, imppresupuesto = imp WHERE idpresupuesto = NEW.idpresupuesto;
	RETURN NEW;
END;
' LANGUAGE plpgsql;

CREATE TRIGGER restriccionespedidoclientetrigger
    AFTER INSERT OR UPDATE ON lpresupuesto
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotpres();

CREATE TRIGGER restriccionespedidoclientetrigger1
    AFTER INSERT OR UPDATE ON dpresupuesto
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotpres();

CREATE OR REPLACE FUNCTION actualizatotpresb() returns TRIGGER
AS '
DECLARE
	tot NUMERIC(12,2);
	bimp NUMERIC(12,2);
	imp NUMERIC(12,2);
BEGIN
	tot := calctotalpres(OLD.idpresupuesto);
	bimp := calcbimppres(OLD.idpresupuesto);
	imp := calcimpuestospres(OLD.idpresupuesto);
	UPDATE presupuesto SET totalpresupuesto = tot, bimppresupuesto = bimp, imppresupuesto = imp WHERE idpresupuesto = OLD.idpresupuesto;
	RETURN OLD;
END;
' LANGUAGE plpgsql;

CREATE TRIGGER restriccionespedidoclientetriggerd
    AFTER DELETE OR UPDATE ON lpresupuesto
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotpresb();

CREATE TRIGGER restriccionespedidoclientetriggerd1
    AFTER DELETE OR UPDATE ON dpresupuesto
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotpresb();




-- Any: Any en que s'efectua la comanda.
-- Numero: Nmero de comanda (comen�nt de 1 cada any).
-- Descripcio: Breu descripci�o comentari opcional.
-- Data: Data d'emisi�de la comanda.
CREATE TABLE pedidocliente (
   idpedidocliente serial PRIMARY KEY,
   numpedidocliente integer UNIQUE NOT NULL,
   fechapedidocliente date DEFAULT now(),
   refpedidocliente character varying(12) NOT NULL,   
   descpedidocliente character varying(500),
   comentpedidocliente character varying(3000),
   contactpedidocliente character varying(90),
   telpedidocliente character varying(20),
   idusuari integer,
   idpresupuesto integer REFERENCES presupuesto(idpresupuesto),
   procesadopedidocliente boolean DEFAULT FALSE,   
   idcliente integer NOT NULL REFERENCES cliente(idcliente),
   idforma_pago integer REFERENCES forma_pago(idforma_pago),
   idalmacen integer NOT NULL REFERENCES almacen(idalmacen),
   idtrabajador integer REFERENCES trabajador(idtrabajador)
);

CREATE FUNCTION restriccionespedidocliente () RETURNS "trigger"
AS '
DECLARE
asd RECORD;
BEGIN
	IF NEW.fechapedidocliente IS NULL THEN
		NEW.fechapedidocliente := now();
	END IF;
        IF NEW.numpedidocliente IS NULL THEN
                SELECT INTO asd max(numpedidocliente) AS m FROM pedidocliente;
		IF asd.m IS NOT NULL THEN
			NEW.numpedidocliente := asd.m + 1;
		ELSE
			NEW.numpedidocliente := 1;
		END IF;
        END IF;
	IF NEW.refpedidocliente IS NULL OR NEW.refpedidocliente = '''' THEN
		SELECT INTO asd crearef() AS m;
		IF FOUND THEN
			NEW.refpedidocliente := asd.m;
		END IF;
	END IF;
        RETURN NEW;
END;
' LANGUAGE plpgsql;


CREATE TRIGGER restriccionespedidoclientetrigger
    BEFORE INSERT OR UPDATE ON pedidocliente
    FOR EACH ROW
    EXECUTE PROCEDURE restriccionespedidocliente();


-- Descuento de pedidocliente.
-- Numero
--Concepte: Descripci�del motiu de descompte.
--Proporcio: Percentatge a descomptar.
-- Descompte de pressupost a clients.
CREATE TABLE dpedidocliente (
   iddpedidocliente serial PRIMARY KEY,
   conceptdpedidocliente character varying(2000),
   proporciondpedidocliente numeric(12,2),
   idpedidocliente integer NOT NULL REFERENCES pedidocliente(idpedidocliente)
   -- Falta poner el lugar donde se aplica el descuento, antes de la factura o despu� de �ta.
);    
    
-- Linea de pedido
-- Numero: Nmero de l�ia.
-- Descripcio: Descripcio de l'article.
-- Quantitat
-- PVD
-- Previsi� Data prevista de recepci�
CREATE TABLE lpedidocliente (
   numlpedidocliente serial PRIMARY KEY,
   desclpedidocliente character varying(150),
   cantlpedidocliente numeric(12,2),
   pvplpedidocliente numeric(12,2),
   prevlpedidocliente date,
   ivalpedidocliente numeric(12,2),
   descuentolpedidocliente numeric(12,2),   
   idpedidocliente integer NOT NULL REFERENCES pedidocliente(idpedidocliente),
   puntlpedidocliente boolean DEFAULT FALSE,
   idarticulo integer REFERENCES articulo(idarticulo)
);




-- FACTURACIO>Albarans:
-- Albarans pendents: S'entendran com albarans pendents tots aquells dels quals no existeixi ticket, factura ni nofactura.
-- Numero
-- Data
-- Factura a clients.
CREATE TABLE factura (
   idfactura serial PRIMARY KEY,
   codigoserie_factura character varying (6) NOT NULL REFERENCES serie_factura(codigoserie_factura),
   numfactura integer NOT NULL,
   reffactura character varying(15) NOT NULL,
   ffactura date DEFAULT now(),
   descfactura character varying(500),   
   idalmacen integer NOT NULL REFERENCES almacen(idalmacen),
   contactfactura character varying(90),
   telfactura character varying(20),
   comentfactura character varying(3000),
   procesadafactura boolean DEFAULT FALSE, 
   idusuari integer,
   idcliente integer REFERENCES cliente(idcliente),
   idforma_pago integer REFERENCES forma_pago(idforma_pago),   
   UNIQUE (idalmacen, codigoserie_factura, numfactura),
   idtrabajador integer REFERENCES trabajador(idtrabajador)
);

CREATE FUNCTION restriccionesfactura () RETURNS "trigger"
AS '
DECLARE
asd RECORD;
BEGIN
	IF NEW.ffactura IS NULL THEN
		NEW.ffactura := now();
	END IF;
        IF NEW.numfactura IS NULL THEN
                SELECT INTO asd max(numfactura) AS m FROM factura WHERE idserie_factura=NEW.idserie_factura AND idalmacen = NEW.idalmacen;
		IF asd.m IS NOT NULL THEN
			NEW.numfactura := asd.m + 1;
		ELSE
			NEW.numfactura := 1;
		END IF;
        END IF;
	IF NEW.reffactura IS NULL OR NEW.reffactura = '''' THEN
		SELECT INTO asd crearef() AS m;
		IF FOUND THEN
			NEW.reffactura := asd.m;
		END IF;
	END IF;
        RETURN NEW;
END;
' LANGUAGE plpgsql;


CREATE TRIGGER restriccionesfacturatrigger
    BEFORE INSERT OR UPDATE ON factura
    FOR EACH ROW
    EXECUTE PROCEDURE restriccionesfactura();

    
-- Descuento de pedidocliente.
-- Numero
--Concepte: Descripci�del motiu de descompte.
--Proporcio: Percentatge a descomptar.
-- Descompte de pressupost a clients.
CREATE TABLE dfactura (
   iddfactura serial PRIMARY KEY,
   conceptdfactura character varying(2000),
   proporciondfactura numeric(12,2),
   idfactura integer NOT NULL REFERENCES factura(idfactura)
   -- Falta poner el lugar donde se aplica el descuento, antes de la factura o despu� de �ta.
); 
    

-- Linea de factura
-- Numero
-- Descripcio: Descripci�de l'article en el moment de ser presupostat.
-- Quantitat
-- PVP: Preu de l'article en el moment de ser pressupostat
-- Descompte: Percentatge de descompte en l�ia.
-- Linia de pressupost a clients.
CREATE TABLE lfactura (
   idlfactura serial PRIMARY KEY,
   desclfactura character varying(150),
   cantlfactura numeric(12,2),
   pvplfactura numeric(12,2),
   ivalfactura numeric(12,2),
   descuentolfactura numeric(12,2),
   idfactura integer NOT NULL REFERENCES factura(idfactura),
   idarticulo integer REFERENCES articulo(idarticulo)
);

CREATE FUNCTION restriccioneslfactura() RETURNS "trigger"
    AS '
DECLARE
asd RECORD;
reg RECORD;
BEGIN
	IF NEW.idarticulo IS NULL THEN
	RAISE EXCEPTION ''ARTICULO INVALIDO'';
	return OLD;
	END IF;

	FOR asd IN SELECT * FROM articulo WHERE idarticulo=NEW.idarticulo LOOP
        	IF NEW.desclfactura IS NULL THEN
			NEW.desclfactura := asd.nomarticulo;
		END IF;
		IF NEW.cantlfactura IS NULL THEN
			NEW.cantlfactura := 1;
		END IF;
		IF NEW.descuentolfactura IS NULL THEN
			NEW.descuentolfactura := 0;
		END IF;
		IF NEW.pvplfactura IS NULL THEN
			SELECT INTO reg pvparticulo(NEW.idarticulo) AS precio;
			NEW.pvplfactura := reg.precio;
		END IF;
		IF NEW.ivalfactura IS NULL then
			SELECT INTO reg ivaarticulo(NEW.idarticulo) AS iva;
			NEW.ivalfactura := reg.iva;
		END IF;
	END LOOP;	
        RETURN NEW;
END;
'
    LANGUAGE plpgsql;

CREATE TRIGGER restriccionesalfacturatrigger
    BEFORE INSERT OR UPDATE ON lfactura
    FOR EACH ROW
    EXECUTE PROCEDURE restriccioneslfactura();

-- -------------------------------------------------------------------------------------------
-- FACTURACIO>Albarans:
-- Albarans pendents: S'entendran com albarans pendents tots aquells dels quals no existeixi ticket, factura ni nofactura.
-- Numero
-- Data
-- Factura a clients.
CREATE TABLE facturap (
   idfacturap serial PRIMARY KEY,
   numfacturap character varying (20) NOT NULL UNIQUE,
   reffacturap character varying(15) NOT NULL,
   ffacturap date DEFAULT now(),
   descfacturap character varying(500),   
   contactfacturap character varying(90),
   telfacturap character varying(20),
   comentfacturap character varying(3000),
   procesadafacturap boolean DEFAULT FALSE, 
   idusuari integer,
   idproveedor integer REFERENCES proveedor(idproveedor),
   idforma_pago integer REFERENCES forma_pago(idforma_pago),
   idtrabajador integer REFERENCES trabajador(idtrabajador)   
);

CREATE FUNCTION restriccionesfacturap () RETURNS "trigger"
AS '
DECLARE
asd RECORD;
BEGIN
	IF NEW.ffacturap IS NULL THEN
		NEW.ffacturap := now();
	END IF;
	IF NEW.reffacturap IS NULL OR NEW.reffacturap = '''' THEN
		SELECT INTO asd crearef() AS m;
		IF FOUND THEN
			NEW.reffacturap := asd.m;
		END IF;
	END IF;
        RETURN NEW;
END;
' LANGUAGE plpgsql;


CREATE TRIGGER restriccionesfacturaptrigger
    BEFORE INSERT OR UPDATE ON facturap
    FOR EACH ROW
    EXECUTE PROCEDURE restriccionesfacturap();


-- Linea de presupuesto
-- Numero
-- Descripcio: Descripci�de l'article en el moment de ser presupostat.
-- Quantitat
-- PVP: Preu de l'article en el moment de ser pressupostat
-- Descompte: Percentatge de descompte en l�ia.
-- Linia de pressupost a clients.
CREATE TABLE lfacturap (
   idlfacturap serial PRIMARY KEY,
   desclfacturap character varying(150),
   cantlfacturap numeric(12,2),
   pvplfacturap numeric(12,2),
   ivalfacturap numeric(12,2),
   descuentolfacturap numeric(12,2),
   idfacturap integer NOT NULL REFERENCES facturap(idfacturap),
   idarticulo integer REFERENCES articulo(idarticulo)
);



-- Descuento de factura proveedor
-- Numero
--Concepte: Descripci�del motiu de descompte.
--Proporcio: Percentatge a descomptar.
-- Descompte de pressupost a clients.
CREATE TABLE dfacturap (
   iddfacturap serial PRIMARY KEY,
   conceptdfacturap character varying(2000),
   proporciondfacturap numeric(12,2),
   idfacturap integer NOT NULL REFERENCES factura(idfactura)
   -- Falta poner el lugar donde se aplica el descuento, antes de la factura o despu� de �ta.
); 

-- -------------------------------------------------------------------------------------------

-- FACTURACIO>Albarans:
-- Albarans pendents: S'entendran com albarans pendents tots aquells dels quals no existeixi ticket, factura ni nofactura.
-- Numero: Numero de nofactura
-- Data
-- Concepte: Descripcio del concepte pel qual no es poden facturar els albarans aqui agrupats (garantia, -- contracte de manteniment, regals a clients, etc...).
-- Observacions
-- Agrupacio d'albarans no facturables en funci�d'un determinat concepte.
CREATE TABLE nofactura (
	idnofactura serial PRIMARY KEY,
   numnofactura integer NOT NULL,
   fechanofactura date,
   conceptnofactura character varying(150),
   observnofactura character varying(150),
	idalmacen integer NOT NULL REFERENCES almacen(idalmacen),
	UNIQUE (idalmacen, numnofactura)
);



-- COMPROVACIONS D'INTEGRITAT>Gen�iques:
-- Tots els albarans d'una factura corresponen al mateix client.
-- FACTURACIO>Albarans:
-- Albarans pendents: S'entendran com albarans pendents tots aquells dels quals no existeixi ticket, factura ni nofactura.
-- Numero
-- Data
-- Albar�a clients.
CREATE TABLE albaranp (
   idalbaranp serial PRIMARY KEY,
   numalbaranp integer NOT NULL UNIQUE,
   descalbaranp character varying(150),
   refalbaranp character varying(12) NOT NULL,
   fechaalbaranp date DEFAULT now(),
--   loginusuario character varying(15) REFERENCES usuario(loginusuario),
   comentalbaranp character varying(3000),
   procesadoalbaranp boolean DEFAULT FALSE,
   idproveedor integer REFERENCES proveedor(idproveedor),
   idforma_pago integer REFERENCES forma_pago(idforma_pago),
   idusuari integer,
--   idfactura integer REFERENCES factura(idfactura),
--   idnofactura integer REFERENCES nofactura(idnofactura),
   idalmacen integer NOT NULL REFERENCES almacen(idalmacen),
   idtrabajador integer REFERENCES trabajador(idtrabajador),   
   UNIQUE (idalmacen, numalbaranp)
);

CREATE FUNCTION restriccionesalbaranp () RETURNS "trigger"
AS '
DECLARE
asd RECORD;
BEGIN
	IF NEW.fechaalbaranp IS NULL THEN
		NEW.fechaalbaranp := now();
	END IF;
        IF NEW.numalbaranp IS NULL THEN
                SELECT INTO asd max(numalbaranp) AS m FROM albaranp;
		IF asd.m IS NOT NULL THEN
			NEW.numalbaranp := asd.m + 1;
		ELSE
			NEW.numalbaranp := 1;
		END IF;
        END IF;
	IF NEW.refalbaranp IS NULL OR NEW.refalbaranp = '''' THEN
		SELECT INTO asd crearef() AS m;
		IF FOUND THEN
			NEW.refalbaranp := asd.m;
		END IF;
	END IF;
        RETURN NEW;
END;
' LANGUAGE plpgsql;


CREATE TRIGGER restriccionesalbaranptrigger
    BEFORE INSERT OR UPDATE ON albaranp
    FOR EACH ROW
    EXECUTE PROCEDURE restriccionesalbaranp();


-- Numero
-- Descripcio
-- Quantitat
-- PVP: Preu de l'article en el moment de la compra o de ser presupostat.
-- Descompte
-- L�ia d'albar�a clients.
CREATE TABLE lalbaranp (
   numlalbaranp serial PRIMARY KEY,
   desclalbaranp character varying(100),
   cantlalbaranp numeric(12,2),
   ivalalbaranp numeric(12,2),
   pvplalbaranp numeric(12,2),
   descontlalbaranp numeric(12,2),
   idalbaranp integer NOT NULL REFERENCES albaranp(idalbaranp),
   idarticulo integer NOT NULL REFERENCES articulo(idarticulo)
);




-- COMPROVACIONS D'INTEGRITAT>Gen�iques:
-- Tots els albarans d'una factura corresponen al mateix client.
-- FACTURACIO>Albarans:
-- Albarans pendents: S'entendran com albarans pendents tots aquells dels quals no existeixi ticket, factura ni nofactura.
-- Numero
-- Data
-- Albar�a clients.
CREATE TABLE albaran (
   idalbaran serial PRIMARY KEY,
   numalbaran integer NOT NULL UNIQUE,
   descalbaran character varying(150),
   refalbaran character varying(12) NOT NULL,
   fechaalbaran date DEFAULT now(),
--   loginusuario character varying(15) REFERENCES usuario(loginusuario),
   comentalbaran character varying(3000),
   comentprivalbaran character varying(3000),
   procesadoalbaran boolean DEFAULT FALSE,
   contactalbaran character varying,
   telalbaran character varying,
   idusuari integer,
   idcliente integer REFERENCES cliente(idcliente),
   idforma_pago integer REFERENCES forma_pago(idforma_pago),
   idfactura integer REFERENCES factura(idfactura),
   idnofactura integer REFERENCES nofactura(idnofactura),
   idalmacen integer NOT NULL REFERENCES almacen(idalmacen),
   idtrabajador integer REFERENCES trabajador(idtrabajador),   
   UNIQUE (idalmacen, numalbaran)
);

CREATE FUNCTION restriccionesalbaran () RETURNS "trigger"
AS '
DECLARE
asd RECORD;
BEGIN
	IF NEW.fechaalbaran IS NULL THEN
		NEW.fechaalbaran := now();
	END IF;
        IF NEW.numalbaran IS NULL THEN
                SELECT INTO asd max(numalbaran) AS m FROM albaran;
		IF asd.m IS NOT NULL THEN
			NEW.numalbaran := asd.m + 1;
		ELSE
			NEW.numalbaran := 1;
		END IF;
        END IF;
	IF NEW.refalbaran IS NULL OR NEW.refalbaran = '''' THEN
		SELECT INTO asd crearef() AS m;
		IF FOUND THEN
			NEW.refalbaran := asd.m;
		END IF;
	END IF;
        RETURN NEW;
END;
' LANGUAGE plpgsql;


CREATE TRIGGER restriccionesalbarantrigger
    BEFORE INSERT OR UPDATE ON albaran
    FOR EACH ROW
    EXECUTE PROCEDURE restriccionesalbaran();



-- Descuento albaran proveedor
-- Numero
-- Concepte: Descripci�del motiu de descompte.
-- Proporcio: Percentatge a descomptar.
-- Descompte d'albar�a clients.
CREATE TABLE dalbaranp (
   iddalbaranp serial PRIMARY KEY,
   conceptdalbaranp character varying(500),
   proporciondalbaranp numeric(12,2),
   idalbaranp integer NOT NULL REFERENCES albaran(idalbaran)
);




-- **********************************************************************
-- APARTADO DE COMPROBACIONES DE INTEGRIDAD EXTRA Y DETECCI� DE ERRORES.
-- **********************************************************************
-- **********************************************************************

    
CREATE FUNCTION random_string(int4) RETURNS "varchar" AS '
DECLARE
iLoop int4;
result varchar;
BEGIN
result = '''';
IF ($1>0) AND ($1 < 255) THEN
  FOR iLoop in 1 .. $1 LOOP
    result = result || chr(int4(random()*26)+65);
  END LOOP;
  RETURN result;
ELSE
  RETURN ''f'';
END IF;
END;
'  LANGUAGE 'plpgsql';
    

CREATE FUNCTION crearef () RETURNS character varying (15)
AS '
DECLARE
asd RECORD;
result character varying(15);
efound boolean;
BEGIN
	efound := FALSE;
	WHILE efound = FALSE LOOP
		result := random_string(6);
		efound := TRUE;
		SELECT INTO asd idpresupuesto FROM presupuesto WHERE refpresupuesto=result;
		IF FOUND THEN
			efound := FALSE;
		END IF;
		SELECT  INTO asd idpedidocliente FROM pedidocliente WHERE refpedidocliente=result;
		IF FOUND THEN
			efound := FALSE;
		END IF;
		SELECT  INTO asd idalbaran FROM albaran WHERE refalbaran=result;
		IF FOUND THEN
			efound := FALSE;
		END IF;	
		SELECT INTO asd  idfactura FROM factura WHERE reffactura=result;
		IF FOUND THEN
			efound := FALSE;
		END IF;	
	END LOOP;
	RETURN result;
END;
' LANGUAGE plpgsql;

-- Descuento albaran
-- Numero
-- Concepte: Descripci�del motiu de descompte.
-- Proporcio: Percentatge a descomptar.
-- Descompte d'albar�a clients.
CREATE TABLE dalbaran (
   iddalbaran serial PRIMARY KEY,
   conceptdalbaran character varying(500),
   proporciondalbaran numeric(12,2),
   idalbaran integer NOT NULL REFERENCES albaran(idalbaran)
);


-- Numero
-- Descripcio
-- Quantitat
-- PVP: Preu de l'article en el moment de la compra o de ser presupostat.
-- Descompte
-- L�ia d'albar�a clients.
CREATE TABLE lalbaran (
   numlalbaran serial PRIMARY KEY,
   desclalbaran character varying(100),
   cantlalbaran numeric(12,2),
   pvplalbaran numeric(12,2),
   descontlalbaran numeric(12,2),
   ivalalbaran numeric(12,2),
   idalbaran integer NOT NULL REFERENCES albaran(idalbaran),
   idarticulo integer NOT NULL REFERENCES articulo(idarticulo)
);

    
    
-- FACTURACIO>Albarans:
-- Albarans pendents: S'entendran com albarans pendents tots aquells dels quals no existeixi ticket, factura ni nofactura.
CREATE TABLE ticket (
   numticket integer PRIMARY KEY,
   fechaticket date
);


-- COMPROVACIONS D'INTEGRITAT>Gen�iques:
-- 1 Article t�1 sol prove�or principal.
-- 1 Article t�1 sol prove�or referent.
CREATE TABLE suministra (
   idsuministra serial PRIMARY KEY,
   refpro character varying(100),
   principalsuministra numeric(12,2),
   idproveedor integer REFERENCES proveedor(idproveedor),
   idarticulo integer REFERENCES articulo(idarticulo)
);






-- Vemos la tabla de provincias.
CREATE TABLE provincia (
    provincia character varying(500)
);

CREATE TABLE precio_compra (
	idprecio_compra serial PRIMARY KEY,
	idarticulo integer NOT NULL REFERENCES articulo(idarticulo),
	iddivision integer REFERENCES division(iddivision),
	idalmacen integer REFERENCES almacen(idalmacen),
	fechapreciocompra date,
	valorpreciocompra numeric(13, 4) NOT NULL
);


CREATE TABLE codigobarras (
	idcodigobarras serial PRIMARY KEY,
	idarticulo integer NOT NULL REFERENCES articulo(idarticulo),
	ean14codigobarras numeric(14, 0) NOT NULL UNIQUE,
	unixcajacodigobarras numeric(4, 0),
	cajxpaletcodigobarras numeric(4, 0),
	unidadcodigobarras character(1)
);

-- FUNCIONES VARIAS DE SOPORTE.

CREATE OR REPLACE FUNCTION ivaarticulo(integer) RETURNS numeric(12,2)
AS'
DECLARE
	idarticulo ALIAS FOR $1;
	as RECORD;
BEGIN
	SELECT INTO AS * FROM tipo_iva, tasa_iva, articulo WHERE tasa_iva.idtipo_iva = tipo_iva.idtipo_iva AND tipo_iva.idtipo_iva = articulo.idtipo_iva AND articulo.idarticulo = idarticulo ORDER BY fechatasa_iva;
	IF FOUND THEN
		RETURN as.porcentasa_iva;
	END IF;
	RETURN 0.0;
END;
' LANGUAGE plpgsql;


CREATE OR REPLACE FUNCTION pvparticulo(integer) RETURNS numeric(12,2)
AS'
DECLARE
	idarticulo ALIAS FOR $1;
	as RECORD;
BEGIN
	SELECT INTO AS pvparticulo FROM  articulo WHERE articulo.idarticulo = idarticulo;
	IF FOUND THEN
		RETURN as.pvparticulo;
	END IF;
	RETURN 0.0;
END;
' LANGUAGE plpgsql;


-- Any: Any en que s'efectua la comanda.
-- Numero: Nmero de comanda (comen�nt de 1 cada any).
-- Descripcio: Breu descripci�o comentari opcional.
-- Data: Data d'emisi�de la comanda.
CREATE TABLE pedidoproveedor (
   idpedidoproveedor serial PRIMARY KEY,
   numpedidoproveedor integer UNIQUE NOT NULL,
   fechapedidoproveedor date DEFAULT now(),
   refpedidoproveedor character varying(12) NOT NULL,   
   descpedidoproveedor character varying(500),
   comentpedidoproveedor character varying(3000),
   contactpedidoproveedor character varying(90),
   telpedidoproveedor character varying(20),   
   procesadopedidoproveedor boolean DEFAULT FALSE,   
   idproveedor integer NOT NULL REFERENCES proveedor(idproveedor),
   idforma_pago integer REFERENCES forma_pago(idforma_pago),    
   idalmacen integer NOT NULL REFERENCES almacen(idalmacen),
   idtrabajador integer REFERENCES trabajador(idtrabajador)   
);

CREATE FUNCTION restriccionespedidoproveedor () RETURNS "trigger"
AS '
DECLARE
asd RECORD;
BEGIN
	IF NEW.fechapedidoproveedor IS NULL THEN
		NEW.fechapedidoproveedor := now();
	END IF;
        IF NEW.numpedidoproveedor IS NULL THEN
                SELECT INTO asd max(numpedidoproveedor) AS m FROM pedidoproveedor;
		IF asd.m IS NOT NULL THEN
			NEW.numpedidoproveedor := asd.m + 1;
		ELSE
			NEW.numpedidoproveedor := 1;
		END IF;
        END IF;
	IF NEW.refpedidoproveedor IS NULL OR NEW.refpedidoproveedor = '''' THEN
		SELECT INTO asd crearef() AS m;
		IF FOUND THEN
			NEW.refpedidoproveedor := asd.m;
		END IF;
	END IF;
        RETURN NEW;
END;
' LANGUAGE plpgsql;


CREATE TRIGGER restriccionespedidoproveedortrigger
    BEFORE INSERT OR UPDATE ON pedidoproveedor
    FOR EACH ROW
    EXECUTE PROCEDURE restriccionespedidoproveedor();


-- Descuento de pedidocliente.
-- Numero
--Concepte: Descripci�del motiu de descompte.
--Proporcio: Percentatge a descomptar.
-- Descompte de pressupost a clients.
CREATE TABLE dpedidoproveedor (
   iddpedidoproveedor serial PRIMARY KEY,
   conceptdpedidoproveedor character varying(2000),
   proporciondpedidoproveedor numeric(12,2),
   idpedidoproveedor integer NOT NULL REFERENCES pedidoproveedor(idpedidoproveedor)
   -- Falta poner el lugar donde se aplica el descuento, antes de la factura o despu� de �ta.
);    
    
-- Linea de pedido
-- Numero: Nmero de l�ia.
-- Descripcio: Descripcio de l'article.
-- Quantitat
-- PVD
-- Previsi� Data prevista de recepci�
CREATE TABLE lpedidoproveedor (
   numlpedidoproveedor serial PRIMARY KEY,
   desclpedidoproveedor character varying(150),
   cantlpedidoproveedor numeric(12,2),
   pvplpedidoproveedor numeric(12,2),
   prevlpedidoproveedor date,
   ivalpedidoproveedor numeric(12,2),
   descuentolpedidoproveedor numeric(12,2),   
   idpedidoproveedor integer NOT NULL REFERENCES pedidoproveedor(idpedidoproveedor),
   puntlpedidoproveedor boolean DEFAULT FALSE,
   idarticulo integer REFERENCES articulo(idarticulo)
);


-- Calculo de totales para presupuestos.
CREATE OR REPLACE FUNCTION calctotalpres(integer) RETURNS numeric(12,2)
AS '
DECLARE
idp ALIAS FOR $1;
total numeric(12,2);
res RECORD;
BEGIN
	total := 0;
	FOR  res IN SELECT cantlpresupuesto * pvplpresupuesto * (1 - descuentolpresupuesto/100) *(1+ ivalpresupuesto/100) AS subtotal1 FROM lpresupuesto WHERE idpresupuesto = idp LOOP
		total := total + res.subtotal1;
	END LOOP;
	FOR res IN SELECT proporciondpresupuesto FROM dpresupuesto WHERE idpresupuesto = idp LOOP
		total := total * (1 - res.proporciondpresupuesto/100);
	END LOOP;
	RETURN total;
END;
' language plpgsql;

-- Calculo de totales para presupuestos.
CREATE OR REPLACE FUNCTION calcbimppres(integer) RETURNS numeric(12,2)
AS '
DECLARE
idp ALIAS FOR $1;
total numeric(12,2);
res RECORD;
BEGIN
	total := 0;
	FOR  res IN SELECT cantlpresupuesto * pvplpresupuesto * (1 - descuentolpresupuesto/100) AS subtotal1 FROM lpresupuesto WHERE idpresupuesto = idp LOOP
		total := total + res.subtotal1;
	END LOOP;
	FOR res IN SELECT proporciondpresupuesto FROM dpresupuesto WHERE idpresupuesto = idp LOOP
		total := total * (1 - res.proporciondpresupuesto/100);
	END LOOP;
	RETURN total;
END;
' language plpgsql;

-- Calculo de totales para presupuestos.
CREATE OR REPLACE FUNCTION calcimpuestospres(integer) RETURNS numeric(12,2)
AS '
DECLARE
idp ALIAS FOR $1;
total numeric(12,2);
res RECORD;
BEGIN
	total := 0;
	FOR  res IN SELECT cantlpresupuesto * pvplpresupuesto * (1 - descuentolpresupuesto/100) * (ivalpresupuesto/100) AS subtotal1 FROM lpresupuesto WHERE idpresupuesto = idp LOOP
		total := total + res.subtotal1;
	END LOOP;
	FOR res IN SELECT proporciondpresupuesto FROM dpresupuesto WHERE idpresupuesto = idp LOOP
		total := total * (1 - res.proporciondpresupuesto/100);
	END LOOP;
	RETURN total;
END;
' language plpgsql;



-- Cálculo de totales para pedido cliente.
CREATE OR REPLACE FUNCTION calctotalpedcli(integer) RETURNS numeric(12,2)
AS '
DECLARE
idp ALIAS FOR $1;
total numeric(12,2);
res RECORD;
BEGIN
	total := 0;
	FOR  res IN SELECT cantlpedidocliente * pvplpedidocliente * (1 - descuentolpedidocliente/100) *(1+ ivalpedidocliente/100) AS subtotal1 FROM lpedidocliente WHERE idpedidocliente = idp LOOP
		total := total + res.subtotal1;
	END LOOP;
	FOR res IN SELECT proporciondpedidocliente FROM dpedidocliente WHERE idpedidocliente = idp LOOP
		total := total * (1 - res.proporciondpedidocliente/100);
	END LOOP;
	RETURN total;
END;
' language plpgsql;


-- Cálculo de totales para pedido cliente.
CREATE OR REPLACE FUNCTION calcbimppedcli(integer) RETURNS numeric(12,2)
AS '
DECLARE
idp ALIAS FOR $1;
total numeric(12,2);
res RECORD;
BEGIN
	total := 0;
	FOR  res IN SELECT cantlpedidocliente * pvplpedidocliente * (1 - descuentolpedidocliente/100) AS subtotal1 FROM lpedidocliente WHERE idpedidocliente = idp LOOP
		total := total + res.subtotal1;
	END LOOP;
	FOR res IN SELECT proporciondpedidocliente FROM dpedidocliente WHERE idpedidocliente = idp LOOP
		total := total * (1 - res.proporciondpedidocliente/100);
	END LOOP;
	RETURN total;
END;
' language plpgsql;

-- Cálculo de totales para pedido cliente.
CREATE OR REPLACE FUNCTION calcimpuestospedcli(integer) RETURNS numeric(12,2)
AS '
DECLARE
idp ALIAS FOR $1;
total numeric(12,2);
res RECORD;
BEGIN
	total := 0;
	FOR  res IN SELECT cantlpedidocliente * pvplpedidocliente * (1 - descuentolpedidocliente/100) *( ivalpedidocliente/100) AS subtotal1 FROM lpedidocliente WHERE idpedidocliente = idp LOOP
		total := total + res.subtotal1;
	END LOOP;
	FOR res IN SELECT proporciondpedidocliente FROM dpedidocliente WHERE idpedidocliente = idp LOOP
		total := total * (1 - res.proporciondpedidocliente/100);
	END LOOP;
	RETURN total;
END;
' language plpgsql;



-- Cálculo de totales para albaranes.
CREATE OR REPLACE FUNCTION calctotalalbaran(integer) RETURNS numeric(12,2)
AS '
DECLARE
idp ALIAS FOR $1;
total numeric(12,2);
res RECORD;
BEGIN
	total := 0;
	FOR  res IN SELECT cantlalbaran * pvplalbaran * (1 - descontlalbaran/100) *(1+ ivalalbaran/100) AS subtotal1 FROM lalbaran WHERE idalbaran = idp LOOP
		total := total + res.subtotal1;
	END LOOP;
	FOR res IN SELECT proporciondalbaran FROM dalbaran WHERE idalbaran = idp LOOP
		total := total * (1 - res.proporciondalbaran/100);
	END LOOP;
	RETURN total;
END;
' language plpgsql;

-- Cálculo de totales para albaranes.
CREATE OR REPLACE FUNCTION calcbimpalbaran(integer) RETURNS numeric(12,2)
AS '
DECLARE
idp ALIAS FOR $1;
total numeric(12,2);
res RECORD;
BEGIN
	total := 0;
	FOR  res IN SELECT cantlalbaran * pvplalbaran * (1 - descontlalbaran/100) AS subtotal1 FROM lalbaran WHERE idalbaran = idp LOOP
		total := total + res.subtotal1;
	END LOOP;
	FOR res IN SELECT proporciondalbaran FROM dalbaran WHERE idalbaran = idp LOOP
		total := total * (1 - res.proporciondalbaran/100);
	END LOOP;
	RETURN total;
END;
' language plpgsql;

-- Cálculo de totales para albaranes.
CREATE OR REPLACE FUNCTION calcimpuestosalbaran(integer) RETURNS numeric(12,2)
AS '
DECLARE
idp ALIAS FOR $1;
total numeric(12,2);
res RECORD;
BEGIN
	total := 0;
	FOR  res IN SELECT cantlalbaran * pvplalbaran * (1 - descontlalbaran/100)*(ivalalbaran/100) AS subtotal1 FROM lalbaran WHERE idalbaran = idp LOOP
		total := total + res.subtotal1;
	END LOOP;
	FOR res IN SELECT proporciondalbaran FROM dalbaran WHERE idalbaran = idp LOOP
		total := total * (1 - res.proporciondalbaran/100);
	END LOOP;
	RETURN total;
END;
' language plpgsql;


-- Cálculo de totales para facturas
CREATE OR REPLACE FUNCTION calctotalfactura(integer) RETURNS numeric(12,2)
AS '
DECLARE
idp ALIAS FOR $1;
total numeric(12,2);
res RECORD;
BEGIN
	total := 0;
	FOR  res IN SELECT cantlfactura * pvplfactura * (1 - descuentolfactura/100) *(1+ ivalfactura/100) AS subtotal1 FROM lfactura WHERE idfactura = idp LOOP
		total := total + res.subtotal1;
	END LOOP;
	FOR res IN SELECT proporciondfactura FROM dfactura WHERE idfactura = idp LOOP
		total := total * (1 - res.proporciondfactura/100);
	END LOOP;
	RETURN total;
END;
' language plpgsql;

-- Cálculo de totales para facturas
CREATE OR REPLACE FUNCTION calcbimpfactura(integer) RETURNS numeric(12,2)
AS '
DECLARE
idp ALIAS FOR $1;
total numeric(12,2);
res RECORD;
BEGIN
	total := 0;
	FOR  res IN SELECT cantlfactura * pvplfactura * (1 - descuentolfactura/100) AS subtotal1 FROM lfactura WHERE idfactura = idp LOOP
		total := total + res.subtotal1;
	END LOOP;
	FOR res IN SELECT proporciondfactura FROM dfactura WHERE idfactura = idp LOOP
		total := total * (1 - res.proporciondfactura/100);
	END LOOP;
	RETURN total;
END;
' language plpgsql;

-- Cálculo de totales para facturas
CREATE OR REPLACE FUNCTION calcimpuestosfactura(integer) RETURNS numeric(12,2)
AS '
DECLARE
idp ALIAS FOR $1;
total numeric(12,2);
res RECORD;
BEGIN
	total := 0;
	FOR  res IN SELECT cantlfactura * pvplfactura * (1 - descuentolfactura/100) *(ivalfactura/100) AS subtotal1 FROM lfactura WHERE idfactura = idp LOOP
		total := total + res.subtotal1;
	END LOOP;
	FOR res IN SELECT proporciondfactura FROM dfactura WHERE idfactura = idp LOOP
		total := total * (1 - res.proporciondfactura/100);
	END LOOP;
	RETURN total;
END;
' language plpgsql;

-- Cálculo de totales para pedido proveedor
CREATE OR REPLACE FUNCTION calctotalpedpro(integer) RETURNS numeric(12,2)
AS '
DECLARE
idp ALIAS FOR $1;
total numeric(12,2);
res RECORD;
BEGIN
	total := 0;
	FOR  res IN SELECT cantlpedidoproveedor * pvplpedidoproveedor * (1 - descuentolpedidoproveedor/100) *(1+ ivalpedidoproveedor/100) AS subtotal1 FROM lpedidoproveedor WHERE idpedidoproveedor = idp LOOP
		total := total + res.subtotal1;
	END LOOP;
	FOR res IN SELECT proporciondpedidoproveedor FROM dpedidoproveedor WHERE idpedidoproveedor = idp LOOP
		total := total * (1 - res.proporciondpedidoproveedor/100);
	END LOOP;
	RETURN total;
END;
' language plpgsql;

CREATE OR REPLACE FUNCTION calcbimppedpro(integer) RETURNS numeric(12,2)
AS '
DECLARE
idp ALIAS FOR $1;
total numeric(12,2);
res RECORD;
BEGIN
	total := 0;
	FOR  res IN SELECT cantlpedidoproveedor * pvplpedidoproveedor * (1 - descuentolpedidoproveedor/100) AS subtotal1 FROM lpedidoproveedor WHERE idpedidoproveedor = idp LOOP
		total := total + res.subtotal1;
	END LOOP;
	FOR res IN SELECT proporciondpedidoproveedor FROM dpedidoproveedor WHERE idpedidoproveedor = idp LOOP
		total := total * (1 - res.proporciondpedidoproveedor/100);
	END LOOP;
	RETURN total;
END;
' language plpgsql;


CREATE OR REPLACE FUNCTION calcimpuestospedpro(integer) RETURNS numeric(12,2)
AS '
DECLARE
idp ALIAS FOR $1;
total numeric(12,2);
res RECORD;
BEGIN
	total := 0;
	FOR  res IN SELECT cantlpedidoproveedor * pvplpedidoproveedor * (1 - descuentolpedidoproveedor/100) *( ivalpedidoproveedor/100) AS subtotal1 FROM lpedidoproveedor WHERE idpedidoproveedor = idp LOOP
		total := total + res.subtotal1;
	END LOOP;
	FOR res IN SELECT proporciondpedidoproveedor FROM dpedidoproveedor WHERE idpedidoproveedor = idp LOOP
		total := total * (1 - res.proporciondpedidoproveedor/100);
	END LOOP;
	RETURN total;
END;
' language plpgsql;

-- Cálculo de totales para albaaran proveedor
CREATE OR REPLACE FUNCTION calctotalalbpro(integer) RETURNS numeric(12,2)
AS '
DECLARE
idp ALIAS FOR $1;
total numeric(12,2);
res RECORD;
BEGIN
	total := 0;
	FOR  res IN SELECT cantlalbaranp * pvplalbaranp * (1 - descontlalbaranp/100) *(1+ ivalalbaranp/100) AS subtotal1 FROM lalbaranp WHERE idalbaranp = idp LOOP
		total := total + res.subtotal1;
	END LOOP;
	FOR res IN SELECT proporciondalbaranp FROM dalbaranp WHERE idalbaranp = idp LOOP
		total := total * (1 - res.proporciondalbaranp/100);
	END LOOP;
	RETURN total;
END;
' language plpgsql;

CREATE OR REPLACE FUNCTION calcbimpalbpro(integer) RETURNS numeric(12,2)
AS '
DECLARE
idp ALIAS FOR $1;
total numeric(12,2);
res RECORD;
BEGIN
	total := 0;
	FOR  res IN SELECT cantlalbaranp * pvplalbaranp * (1 - descontlalbaranp/100) AS subtotal1 FROM lalbaranp WHERE idalbaranp = idp LOOP
		total := total + res.subtotal1;
	END LOOP;
	FOR res IN SELECT proporciondalbaranp FROM dalbaranp WHERE idalbaranp = idp LOOP
		total := total * (1 - res.proporciondalbaranp/100);
	END LOOP;
	RETURN total;
END;
' language plpgsql;
\echo "Cálculo de totales para albaranes de proveedor"

CREATE OR REPLACE FUNCTION calcimpuestosalbpro(integer) RETURNS numeric(12,2)
AS '
DECLARE
idp ALIAS FOR $1;
total numeric(12,2);
res RECORD;
BEGIN
	total := 0;
	FOR  res IN SELECT cantlalbaranp * pvplalbaranp * (1 - descontlalbaranp/100) *(ivalalbaranp/100) AS subtotal1 FROM lalbaranp WHERE idalbaranp = idp LOOP
		total := total + res.subtotal1;
	END LOOP;
	FOR res IN SELECT proporciondalbaranp FROM dalbaranp WHERE idalbaranp = idp LOOP
		total := total * (1 - res.proporciondalbaranp/100);
	END LOOP;
	RETURN total;
END;
' language plpgsql;

-- Cálculo de totales para factura proveedor
CREATE OR REPLACE FUNCTION calctotalfacpro(integer) RETURNS numeric(12,2)
AS '
DECLARE
idp ALIAS FOR $1;
total numeric(12,2);
res RECORD;
BEGIN
	total := 0;
	FOR  res IN SELECT cantlfacturap * pvplfacturap * (1 - descuentolfacturap/100) *(1+ ivalfacturap/100) AS subtotal1 FROM lfacturap WHERE idfacturap = idp LOOP
		total := total + res.subtotal1;
	END LOOP;
	FOR res IN SELECT proporciondfacturap FROM dfacturap WHERE idfacturap = idp LOOP
		total := total * (1 - res.proporciondfacturap/100);
	END LOOP;
	RETURN total;
END;
' language plpgsql;


-- Cálculo de totales para factura proveedor
CREATE OR REPLACE FUNCTION calcbimpfacpro(integer) RETURNS numeric(12,2)
AS '
DECLARE
idp ALIAS FOR $1;
total numeric(12,2);
res RECORD;
BEGIN
	total := 0;
	FOR  res IN SELECT cantlfacturap * pvplfacturap * (1 - descuentolfacturap/100) AS subtotal1 FROM lfacturap WHERE idfacturap = idp LOOP
		total := total + res.subtotal1;
	END LOOP;
	FOR res IN SELECT proporciondfacturap FROM dfacturap WHERE idfacturap = idp LOOP
		total := total * (1 - res.proporciondfacturap/100);
	END LOOP;
	RETURN total;
END;
' language plpgsql;

-- Cálculo de totales para factura proveedor
CREATE OR REPLACE FUNCTION calcimpuestosfacpro(integer) RETURNS numeric(12,2)
AS '
DECLARE
idp ALIAS FOR $1;
total numeric(12,2);
res RECORD;
BEGIN
	total := 0;
	FOR  res IN SELECT cantlfacturap * pvplfacturap * (1 - descuentolfacturap/100) *(ivalfacturap/100) AS subtotal1 FROM lfacturap WHERE idfacturap = idp LOOP
		total := total + res.subtotal1;
	END LOOP;
	FOR res IN SELECT proporciondfacturap FROM dfacturap WHERE idfacturap = idp LOOP
		total := total * (1 - res.proporciondfacturap/100);
	END LOOP;
	RETURN total;
END;
' language plpgsql;


-- ------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------

-- ------------------------------------------------------------------------------------------

-- ------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------

-- COMENTARIO TEMPORAL
-- Cambios para el contrl de stock que aun no han sido trasladados al lugar pertinente en 
-- revf

CREATE TABLE inventario (
	idinventario SERIAL PRIMARY KEY,
	nominventario varchar NOT NULL,
	fechainventario date default now()
);

-- stockantcontrolstock es un campo de solo lectura, es autorregulado internamente mediante triggers
CREATE TABLE controlstock (
	idinventario integer NOT NULL REFERENCES inventario(idinventario),
	idalmacen  integer NOT NULL REFERENCES almacen(idalmacen),
	idarticulo   integer NOT NULL REFERENCES articulo(idarticulo),
	stockantcontrolstock numeric(12,2) NOT NULL,
	stocknewcontrolstock numeric(12,2) NOT NULL,
	punteocontrolstock boolean NOT NULL DEFAULT FALSE,
	PRIMARY KEY (idinventario, idalmacen, idarticulo)
);


-- Esta tabla es mantenida por el SGDB y sirve solo para hacer consultas.
CREATE TABLE  stock_almacen (
	idarticulo integer NOT NULL REFERENCES articulo(idarticulo),
	idalmacen integer NOT NULL REFERENCES almacen(idalmacen),
	stock numeric(12,2) DEFAULT 0,
	PRIMARY KEY (idarticulo, idalmacen)
);

CREATE FUNCTION narticulo () RETURNS "trigger"
AS '
DECLARE
	as RECORD;
BEGIN
		FOR as IN SELECT * FROM almacen LOOP
			INSERT INTO stock_almacen (idarticulo, idalmacen, stock) VALUES (NEW.idarticulo, as.idalmacen, 0);
		END LOOP;
	RETURN NEW;
END;
' LANGUAGE plpgsql;

CREATE TRIGGER narticulot
    AFTER INSERT ON articulo
    FOR EACH ROW
    EXECUTE PROCEDURE narticulo();

CREATE FUNCTION darticulo () RETURNS "trigger"
AS '
DECLARE
BEGIN
	DELETE FROM stock_almacen WHERE idarticulo = OLD.idarticulo;
	RETURN OLD;
END;
' LANGUAGE plpgsql;

CREATE TRIGGER darticulot
    BEFORE DELETE ON articulo
    FOR EACH ROW
    EXECUTE PROCEDURE darticulo();


CREATE FUNCTION nalmacen () RETURNS "trigger"
AS '
DECLARE
	as RECORD;
BEGIN
		FOR as IN SELECT * FROM articulo LOOP
			INSERT INTO stock_almacen (idarticulo, idalmacen, stock) VALUES (as.idarticulo, NEW.idalmacen, 0);
		END LOOP;
	RETURN NEW;
END;
' LANGUAGE plpgsql;

CREATE TRIGGER nalmacent
    AFTER INSERT ON almacen
    FOR EACH ROW
    EXECUTE PROCEDURE nalmacen();

CREATE FUNCTION dalmacen () RETURNS "trigger"
AS '
DECLARE
BEGIN
	DELETE FROM stock_almacen WHERE idalmacen = OLD.idalmacen;
	RETURN OLD;
END;
' LANGUAGE plpgsql;

CREATE TRIGGER dalmacent
    BEFORE DELETE ON almacen
    FOR EACH ROW
    EXECUTE PROCEDURE dalmacen();


CREATE FUNCTION disminuyestock () RETURNS "trigger"
AS '
DECLARE
BEGIN
	-- Hacemos el update del stock del articulo
	UPDATE articulo SET stockarticulo = stockarticulo + OLD.cantlalbaran WHERE idarticulo= OLD.idarticulo;
	-- Hacemos el update del stock por almacenes
	UPDATE stock_almacen SET stock = stock + OLD.cantlalbaran WHERE idarticulo = OLD.idarticulo AND idalmacen IN (SELECT idalmacen FROM albaran WHERE idalbaran=OLD.idalbaran);
	RETURN NEW;
END;
' LANGUAGE plpgsql;


CREATE TRIGGER disminuyestockt
    AFTER DELETE OR UPDATE ON lalbaran
    FOR EACH ROW
    EXECUTE PROCEDURE disminuyestock();


CREATE FUNCTION aumentastock () RETURNS "trigger"
AS '
DECLARE
BEGIN
	-- Hacemos el update del stock del articulo
	UPDATE articulo SET stockarticulo = stockarticulo - NEW.cantlalbaran WHERE idarticulo = NEW.idarticulo;
	-- Hacemos el update del stock por almacenes
	UPDATE stock_almacen SET stock = stock - NEW.cantlalbaran WHERE idarticulo = NEW.idarticulo AND idalmacen IN (SELECT idalmacen FROM albaran WHERE idalbaran=NEW.idalbaran);
	RETURN NEW;
END;
' LANGUAGE plpgsql;


CREATE TRIGGER aumentastockt
    AFTER INSERT OR UPDATE ON lalbaran
    FOR EACH ROW
    EXECUTE PROCEDURE aumentastock();    


CREATE FUNCTION disminuyestockp () RETURNS "trigger"
AS '
DECLARE
BEGIN
	-- Hacemos el update del stock del articulo
	UPDATE articulo SET stockarticulo = stockarticulo - OLD.cantlalbaranp WHERE idarticulo= OLD.idarticulo;
	-- Hacemos el update del stock por almacenes
	UPDATE stock_almacen SET stock = stock - OLD.cantlalbaranp WHERE idarticulo = OLD.idarticulo AND idalmacen IN (SELECT idalmacen FROM albaranp WHERE idalbaranp=OLD.idalbaranp);
	RETURN NEW;
END;
' LANGUAGE plpgsql;


CREATE TRIGGER disminuyestockpt
    AFTER DELETE OR UPDATE ON lalbaranp
    FOR EACH ROW
    EXECUTE PROCEDURE disminuyestockp();


CREATE FUNCTION aumentastockp () RETURNS "trigger"
AS '
DECLARE
BEGIN
	UPDATE articulo SET stockarticulo = stockarticulo + NEW.cantlalbaranp WHERE idarticulo = NEW.idarticulo;
	-- Hacemos el update del stock por almacenes
	UPDATE stock_almacen SET stock = stock + NEW.cantlalbaranp WHERE idarticulo = NEW.idarticulo AND idalmacen IN (SELECT idalmacen FROM albaranp WHERE idalbaranp=NEW.idalbaranp);
	RETURN NEW;
END;
' LANGUAGE plpgsql;


CREATE TRIGGER aumentastockpt
    AFTER INSERT OR UPDATE ON lalbaranp
    FOR EACH ROW
    EXECUTE PROCEDURE aumentastockp(); 


CREATE OR REPLACE FUNCTION modificadostock () RETURNS "trigger"
AS '
DECLARE 
	cant numeric;
	as RECORD;
BEGIN
	IF NEW.stockarticulo <> OLD.stockarticulo THEN
		cant := NEW.stockarticulo - OLD.stockarticulo;
		FOR as IN SELECT * FROM comparticulo WHERE idarticulo = NEW.idarticulo LOOP
			UPDATE articulo SET stockarticulo = stockarticulo + cant * as.cantcomparticulo WHERE idarticulo = as.idcomponente;
		END LOOP;
	END IF;
	RETURN NEW;
END;
' LANGUAGE plpgsql;

CREATE TRIGGER modificastocktrigger
	AFTER UPDATE ON articulo
	FOR EACH ROW
	EXECUTE PROCEDURE modificadostock();


CREATE OR REPLACE FUNCTION modificadostockalmacen () RETURNS "trigger"
AS '
DECLARE 
	cant numeric;
	as RECORD;
BEGIN
	IF NEW.stock <> OLD.stock THEN
		cant := NEW.stock - OLD.stock;
		FOR as IN SELECT * FROM comparticulo WHERE idarticulo = NEW.idarticulo LOOP
			UPDATE stock_almacen SET stock = stock + cant * as.cantcomparticulo WHERE idarticulo = as.idcomponente AND idalmacen = NEW.idalmacen;
		END LOOP;
	END IF;
	RETURN NEW;
END;
' LANGUAGE plpgsql;

CREATE TRIGGER modificastocktrigger
	AFTER UPDATE ON stock_almacen
	FOR EACH ROW
	EXECUTE PROCEDURE modificadostockalmacen();


-- aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
CREATE FUNCTION disminuyecontrolstock () RETURNS "trigger"
AS '
DECLARE
BEGIN
		-- Hacemos el update del stock del articulo
		UPDATE articulo SET stockarticulo = stockarticulo - OLD.stocknewcontrolstock + OLD.stockantcontrolstock WHERE idarticulo= OLD.idarticulo;
		-- Hacemos el update del stock por almacenes
		UPDATE stock_almacen SET stock = stock - OLD.stocknewcontrolstock + OLD.stockantcontrolstock WHERE idarticulo = OLD.idarticulo AND idalmacen = OLD.idalmacen;
	RETURN OLD;
END;
' LANGUAGE plpgsql;


CREATE TRIGGER disminuyecontrolstockt
    BEFORE DELETE ON controlstock
    FOR EACH ROW
    EXECUTE PROCEDURE disminuyecontrolstock();


CREATE FUNCTION disminuyecontrolstock1 () RETURNS "trigger"
AS '
DECLARE
	rant RECORD;
BEGIN
	-- Cogemos el stock anterior.
	FOR  rant IN SELECT * FROM stock_almacen WHERE idarticulo = NEW.idarticulo AND idalmacen = NEW.idalmacen LOOP
		NEW.stockantcontrolstock := rant.stock;
	END LOOP;
	RETURN NEW;
END;
' LANGUAGE plpgsql;


CREATE TRIGGER disminuyecontrolstockt1
    BEFORE INSERT ON controlstock
    FOR EACH ROW
    EXECUTE PROCEDURE disminuyecontrolstock1();

CREATE FUNCTION disminuyecontrolstock2 () RETURNS "trigger"
AS '
DECLARE
	rant RECORD;
BEGIN
		-- Hacemos el update del stock del articulo
		UPDATE articulo SET stockarticulo = stockarticulo - OLD.stocknewcontrolstock + OLD.stockantcontrolstock WHERE idarticulo= OLD.idarticulo;
		-- Hacemos el update del stock por almacenes
		UPDATE stock_almacen SET stock = stock - OLD.stocknewcontrolstock + OLD.stockantcontrolstock WHERE idarticulo = OLD.idarticulo AND idalmacen = OLD.idalmacen;

	-- Cogemos el stock anterior.
	FOR  rant IN SELECT * FROM stock_almacen WHERE idarticulo = NEW.idarticulo AND idalmacen = NEW.idalmacen LOOP
		NEW.stockantcontrolstock := rant.stock;
	END LOOP;
	RETURN NEW;
END;
' LANGUAGE plpgsql;


CREATE TRIGGER disminuyecontrolstockt2
    BEFORE UPDATE ON controlstock
    FOR EACH ROW
    EXECUTE PROCEDURE disminuyecontrolstock2();





CREATE FUNCTION aumentacontrolstock () RETURNS "trigger"
AS '
DECLARE
BEGIN
	UPDATE articulo SET stockarticulo = stockarticulo + NEW.stocknewcontrolstock - NEW.stockantcontrolstock WHERE idarticulo = NEW.idarticulo;
	-- Hacemos el update del stock por almacenes
	UPDATE stock_almacen SET stock = NEW.stocknewcontrolstock WHERE idarticulo = NEW.idarticulo AND idalmacen = NEW.idalmacen;
	RETURN NEW;
END;
' LANGUAGE plpgsql;


CREATE TRIGGER aumentacontrolstockt
    AFTER INSERT OR UPDATE ON controlstock
    FOR EACH ROW
    EXECUTE PROCEDURE aumentacontrolstock(); 


-- Cuando cambiamos el almacen en un albaran o albaranp hay problemas con el control de stock que se descuadra.
CREATE FUNCTION cambiaalbaran () RETURNS "trigger"
AS '
DECLARE
	as RECORD;
BEGIN
	IF NEW.idalmacen <> OLD.idalmacen THEN
		FOR as IN SELECT * FROM lalbaran WHERE idalbaran = NEW.idalbaran LOOP
			UPDATE stock_almacen SET stock = stock + as.cantlalbaran WHERE idarticulo = as.idarticulo AND idalmacen = OLD.idalmacen;
			UPDATE stock_almacen SET stock = stock - as.cantlalbaran WHERE idarticulo = as.idarticulo AND idalmacen = NEW.idalmacen;
		END LOOP;
	END IF;
	RETURN NEW;
END;
' LANGUAGE plpgsql;

CREATE TRIGGER cambiadoalbarant
	AFTER UPDATE ON albaran
	FOR EACH ROW
	EXECUTE PROCEDURE cambiaalbaran();


CREATE FUNCTION cambiaalbaranp () RETURNS "trigger"
AS '
DECLARE
	as RECORD;
BEGIN
	IF NEW.idalmacen <> OLD.idalmacen THEN
		FOR as IN SELECT * FROM lalbaranp WHERE idalbaranp = NEW.idalbaranp LOOP
			UPDATE stock_almacen SET stock = stock - as.cantlalbaranp WHERE idarticulo = as.idarticulo AND idalmacen = OLD.idalmacen;
			UPDATE stock_almacen SET stock = stock + as.cantlalbaranp WHERE idarticulo = as.idarticulo AND idalmacen = NEW.idalmacen;
		END LOOP;
	END IF;
	RETURN NEW;
END;
' LANGUAGE plpgsql;

CREATE TRIGGER cambiadoalbaranpt
	AFTER UPDATE ON albaranp
	FOR EACH ROW
	EXECUTE PROCEDURE cambiaalbaranp();
