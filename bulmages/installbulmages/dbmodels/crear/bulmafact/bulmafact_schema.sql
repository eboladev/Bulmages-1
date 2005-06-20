-- ----------------------------------------------------------------------------------------
-- (C)  Joan Miquel Torrer Rigo & Tomeu Borras Riera & Mateu Borras Marco, Agosto 2004
--  joanmi@bulma.net, tborras@conetxia.com mborras@conetxia.com
-- Este documento esta licenciado bajo licencia GPL, el cual no escribimos aqui por pereza.
--  ----------------------------------------------------------------------------------------
--     psql xxxx < bulmafact-0_0_1.sql
--  ---------------------------------------------------------------------------------------

SET SESSION AUTHORIZATION 'postgres';
SET search_path = public, pg_catalog;

SET DATESTYLE TO European;


CREATE FUNCTION plpgsql_call_handler() RETURNS language_handler
    AS '/usr/lib/postgresql/8.0/lib/plpgsql.so', 'plpgsql_call_handler'
    LANGUAGE c;
    
CREATE TRUSTED PROCEDURAL LANGUAGE plpgsql HANDLER plpgsql_call_handler;


-- NOTACION:
-- Considerar las siguientes opciones de codificación:
-- Los nombres de tabla estan escritos SIEMPRE en singular.
-- Todos los campos de una tabla terminan siempre con el nombre de la tabla (salvo las claves foraneas).
-- Las claves foraneas tienen el mismo nombre que el campo con que se corresponden en la tabla relacionada.
-- En caso de que haya diversas claves foraneas referentes al mismo campo, el criterio es que una de ellas tenga el nombre del campo con el que se corresponde y la otra tenga un nombre significativo.
-- Los campos de clave automatica empiezan por id
-- Los enums se simulan (normalmente) con campos numericos, el significado de los valores debe estar
-- explicado en este archivo.


-- La tabla de configuración.
-- En esta tabla se guardan parametros que el programa va a utilizar.
-- Como por ejemplo el numero de dígitos por defecto de las cuentas o el asiento inteligente que se enlaza con
-- facturacion.
-- Tiene tres campos
-- idconfiguracion: el identificador (No tiene ningún uso especial).
-- nombre: El nombre del parametro de configuracion.
-- valor: El valor que toma dicho parametro.
CREATE TABLE configuracion (
    nombre character varying(25) PRIMARY KEY,
    valor character varying(350)
);


-- Codi: Clau artificial.
-- Descripcio: Nom identificatiu o descripció breu.
-- Dies_1T: Dies abans del primer termini computant els blocs de 30 com a mesos naturals.
-- Descompte: Descompte automàtic per l'ús d'aquesta forma de pagament.
CREATE TABLE forma_pago (
   idforma_pago serial PRIMARY KEY,
   descforma_pago character varying(500),
   dias1tforma_pago integer,
   descuentoforma_pago numeric(12,2)
);


-- Codigo: Clave artificial.
-- Nombre: Nombre identificativo del almacén.
-- diralmacen: Dirección del almacén.
-- poblalmacen: Población del almacén.
-- cpalmacenc: código postal almacén.
-- telfalmacen: Teléfono del almacén.
-- faxalmacen: Fax del almacén.
-- emailalmacen: correo electrónico del almacén.
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


-- Tabla de países
-- cod2: código de dos dígitos
-- cod3: código de tres dígitos
-- desc: descripción del país
CREATE TABLE pais (
   idpais serial PRIMARY KEY,
   cod2pais character varying(2),
   cod3pais character varying(3),
	descpais character varying(50)
);


-- Tabla de monedas
-- cod2: código de dos dígitos
-- cod3: código de tres dígitos
-- desc: descripción de la moneda
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


-- Tabla con series de Iva, código i descripción
-- Básicamente sirve para garantizar la integridad referencial en las  series de facturación
-- Deberían existir en contabilidad tambien.
CREATE TABLE serie_factura (
--	idserie_factura serial PRIMARY KEY,
	codigoserie_factura character varying (6) PRIMARY KEY,
	descserie_factura character varying(50) NOT NULL,
	UNIQUE (codigoserie_factura)
);


-- codigofamilia código de la familia.
-- nombrefamilia nombre de la familia
-- descfamilia descripción extendida de la familia.
-- codcompfamilia código compuesto de familia: Es la concatenación del código de familia con sus códigos padres. 
-- codigocompletofamilia Este campo es de sólo lectura, no se puede escribir sobre él.

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

-- Esta función nos da el identificador de familia dado un código.
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


-- El tipo de artículo es una tabla que permite crear una forma alternativa de agrupar los artículos.
-- codigo: identificador del tipo.
-- desc:
CREATE TABLE tipo_articulo (
	idtipo_articulo serial PRIMARY KEY,
	codtipo_articulo character varying(10),
	desctipo_articulo character varying(50)
);


-- Codigo: Clave artificial.
-- Nombre: Descripción corta del artículo.
-- abrev: Nombre abreviado del articulo (para tpv o cartelitos estanterias...)
-- idtipo_articulo: identificador de tipo de artículo que se utilizará para agrupar artículos como clasificación alternativa a el surtido (familias).
-- Observaciones: Campo de texto para a comentarios y observaciones.
-- El campo codigocompletoarticulo sólo puede ser de lectura.
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
    -- ATENCION, este campo no da el pvp real del artículo, solo es una de las multiples formas de acceder al precio del articulo.
    -- Para obtener el precio de un artículo se debe usar la funcion pvparticulo.
    -- Para saber el iva correspondiente a un articulo se debe usar la función ivaarticulo.
    pvparticulo numeric(12,2) NOT NULL DEFAULT 0
);


CREATE FUNCTION calculacodigocompletoarticulo () RETURNS "trigger"
AS '
DECLARE
	as RECORD;
	codigocompleto character varying(100);
BEGIN
	codigocompleto := NEW.codarticulo;
	SELECT INTO as codigocompletofamilia FROM familia WHERE idfamilia = NEW.idfamilia;
	IF FOUND THEN
		codigocompleto := as.codigocompletofamilia || codigocompleto;
	END IF;
        NEW.codigocompletoarticulo := codigocompleto;
	RETURN NEW;
END;
' LANGUAGE plpgsql;


CREATE TRIGGER calculacodigocompletoarticulotrigger
    BEFORE INSERT OR UPDATE ON articulo
    FOR EACH ROW
    EXECUTE PROCEDURE calculacodigocompletoarticulo();


-- Los proveedores son los que nos suminstran articulos y/o servicios.
-- COMPROVACIONS D'INTEGRITAT>Genèriques:
-- 1 Article té 1 sol proveïdor principal.
-- 1 Article té 1 sol proveïdor referent.
-- CAMPOS
-- ======
-- Codi: Clau artificial.
-- Nom: Nom comercial o fiscal.
-- Nom_alternatiu: Nom comercial o fiscal.
-- CIF: Codi d'Identificació Fiscal.
-- CodiCli: Codi de client amb que ens facturen. Útil per a identificar-nos.
-- C_Banc
-- Comentaris
-- Adreça: Adreça.
-- Població: Població.
-- CProv: Codi de provincia (dos primers dígits del codi postal).
-- sCP: Tres darrers dígits del codi postal.
-- Telf: Telèfon.
-- Fax: Fax.
-- Email: eMail.
-- Url: Url.
-- CompteWeb: Dades de login si disposen de tenda o tarifes en línia
CREATE TABLE proveedor (
   idproveedor serial PRIMARY KEY,
   nomproveedor character varying(200),
   nomaltproveedor character varying(200),
   cifproveedor character varying(12),
   codicliproveedor character varying(30),
   cbancproveedor character varying(20),
   comentproveedor character varying(2000),
   dirproveedor character varying(50),
   poblproveedor character varying(50),
   cpproveedor character varying(9) NOT NULL,
   telproveedor character varying(12),
   faxproveedor character varying(12),
   emailproveedor character varying(100),
   urlproveedor character varying(100),
   clavewebproveedor character varying(100),
   inactivoproveedor character(1)
);


--Numero: Número de divisió (clau artificial).
--Descripcio: Nom o descripció de la divisió.
--Contactes: Nom de persona o persones de contacte.
--Comentaris
--Telf: Telèfon.
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


-- El cliente siempre tiene la razón, bueno, o por lo menos eso cree.
--Codi: Clau artificial.
--Nom: Nom comercial o fiscal.
--Nom_alternatiu: Nom comercial o fiscal.
--CIF: Codi d'Identificació Fiscal.
--C_Banc: Compte Bancari.
--Adr: Adreça.
--Pobl: Població.
--CProv: Codi de provincia (dos primers dígits del codi postal).
--sCP: Tres darrers dígits del codi postal.
--Telf: Telèfon.
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
   cifcliente character varying(200),
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
   inactivocliente character(1)
);

CREATE TABLE cobro (
   idcobro serial PRIMARY KEY,
   idcliente integer NOT NULL REFERENCES cliente(idcliente),
   fechacobro date DEFAULT NOW(),
   cantcobro numeric(12,2) DEFAULT 0,
   refcobro character varying(12) NOT NULL,
   previsioncobro boolean DEFAULT FALSE,
   comentcobro character varying(500)
);
   

CREATE TABLE pago (
   idpago serial PRIMARY KEY,
   idproveedor integer NOT NULL REFERENCES proveedor(idproveedor),
   fechapago date DEFAULT NOW(),
   cantpago numeric(12,2) DEFAULT 0,
   refpago character varying(12) NOT NULL,
   previsionpago boolean DEFAULT FALSE,
   comentpago character varying(500)
);

-- Any: Any en que s'efectua la comanda.
-- Numero: Número de comanda (començant de 1 cada any).
-- Descripcio: Breu descripció o comentari opcional.
-- Data: Data d'emisió de la comanda.
CREATE TABLE pedido (
   idpedido serial PRIMARY KEY,
   numpedido character varying(60),
   fechapedido date,
   descpedido character varying(500),
   iddivision integer NOT NULL REFERENCES division(iddivision),
   idalmacen integer NOT NULL REFERENCES almacen(idalmacen)
);



CREATE TABLE usuario (
    loginusuario character varying(15) PRIMARY KEY,
    nombreusuario character varying(35),
    apellido1usuario character varying(35),
    apellido2usuario character varying(35),
    claveusuario character varying(35),
    permisosusuario text
);



-- Any: Any de facturació.
-- Numero: Número de factura.
-- Data Comentaris
-- Factura de proveïdor.
CREATE TABLE fra_pro (
   idfra_pro serial PRIMARY KEY,
   numfra_pro character varying(60),
   fcrefra_pro date,
   comentfra_pro character varying(2000)
);



-- Albaran de proveedor
-- Any: Any en que s'efectua la comanda.
-- NumCompra: Numero de Compra (Clau artificial per poder registrar recepcions que ens arribin sense l'albarà postposant la cumplimentació del número d'albarà).
-- NumAlbara: Número d'albarà.
-- Data: Data de l'albarà
-- Recepcio: Data de recepció.
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
-- Numero: Número de línia.
-- Descripcio: Descripcio de l'article.
-- Quantitat
-- PVD
-- Previsió: Data prevista de recepció
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




-- Entendemos que un presupuesto es una relación de materiales y trabajos cuantificada que
-- hacemos a petición de un cliente determinado
-- Numero
-- Data: Data d'emisió del presupost.
-- PersContacte: Nom de persona de contacte (si cal).
-- TelfContacte: Telèfon.
-- Venciment: Data màxima de validesa del presupost.
-- Comentaris
--  Pressupost a clients.
CREATE TABLE presupuesto (
   idpresupuesto serial PRIMARY KEY,
   numpresupuesto integer NOT NULL UNIQUE,
   refpresupuesto character varying(12) NOT NULL,
   fpresupuesto date,
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
   UNIQUE (idalmacen, numpresupuesto)
);

CREATE FUNCTION restriccionespresupuesto () RETURNS "trigger"
AS '
DECLARE
asd RECORD;
BEGIN
        IF NEW.numpresupuesto IS NULL THEN
                SELECT INTO asd max(numpresupuesto) AS m FROM presupuesto;
		IF FOUND THEN
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
--Concepte: Descripció del motiu de descompte.
--Proporcio: Percentatge a descomptar.
-- Descompte de pressupost a clients.
CREATE TABLE dpresupuesto (
   iddpresupuesto serial PRIMARY KEY,
   conceptdpresupuesto character varying(2000),
   proporciondpresupuesto numeric(12,2),
   idpresupuesto integer REFERENCES presupuesto(idpresupuesto)
   -- Falta poner el lugar donde se aplica el descuento, antes de la factura o después de ésta.
);



-- Linea de presupuesto
-- Numero
-- Descripcio: Descripció de l'article en el moment de ser presupostat.
-- Quantitat
-- PVP: Preu de l'article en el moment de ser pressupostat
-- Descompte: Percentatge de descompte en línia.
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


-- Any: Any en que s'efectua la comanda.
-- Numero: Número de comanda (començant de 1 cada any).
-- Descripcio: Breu descripció o comentari opcional.
-- Data: Data d'emisió de la comanda.
CREATE TABLE pedidocliente (
   idpedidocliente serial PRIMARY KEY,
   numpedidocliente integer UNIQUE NOT NULL,
   fechapedidocliente date,
   refpedidocliente character varying(12) NOT NULL,   
   descpedidocliente character varying(500),
   comentpedidocliente character varying(3000),
   contactpedidocliente character varying(90),
   telpedidocliente character varying(20),   
   idpresupuesto integer REFERENCES presupuesto(idpresupuesto),
   procesadopedidocliente boolean DEFAULT FALSE,   
   idcliente integer NOT NULL REFERENCES cliente(idcliente),
   idforma_pago integer REFERENCES forma_pago(idforma_pago),    
   idalmacen integer NOT NULL REFERENCES almacen(idalmacen)
);

CREATE FUNCTION restriccionespedidocliente () RETURNS "trigger"
AS '
DECLARE
asd RECORD;
BEGIN
        IF NEW.numpedidocliente IS NULL THEN
                SELECT INTO asd max(numpedidocliente) AS m FROM pedidocliente;
		IF FOUND THEN
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
--Concepte: Descripció del motiu de descompte.
--Proporcio: Percentatge a descomptar.
-- Descompte de pressupost a clients.
CREATE TABLE dpedidocliente (
   iddpedidocliente serial PRIMARY KEY,
   conceptdpedidocliente character varying(2000),
   proporciondpedidocliente numeric(12,2),
   idpedidocliente integer NOT NULL REFERENCES pedidocliente(idpedidocliente)
   -- Falta poner el lugar donde se aplica el descuento, antes de la factura o después de ésta.
);    
    
-- Linea de pedido
-- Numero: Número de línia.
-- Descripcio: Descripcio de l'article.
-- Quantitat
-- PVD
-- Previsió: Data prevista de recepció
CREATE TABLE lpedidocliente (
   numlpedidocliente serial PRIMARY KEY,
   desclpedidocliente character varying(150),
   cantlpedidocliente numeric(12,2),
   pvplpedidocliente numeric(12,2),
   prevlpedidocliente date,
   ivalpedidocliente numeric(12,2),
   descuentolpedidocliente numeric(12,2),   
   idpedidocliente integer NOT NULL REFERENCES pedidocliente(idpedidocliente),
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
   ffactura date,
   descfactura character varying(500),   
   idalmacen integer NOT NULL REFERENCES almacen(idalmacen),
   contactfactura character varying(90),
   telfactura character varying(20),
   comentfactura character varying(3000),
   procesadafactura boolean DEFAULT FALSE, 
   idusuari integer,
   idcliente integer REFERENCES cliente(idcliente),
   idforma_pago integer REFERENCES forma_pago(idforma_pago),   
   UNIQUE (idalmacen, codigoserie_factura, numfactura)	
);

CREATE FUNCTION restriccionesfactura () RETURNS "trigger"
AS '
DECLARE
asd RECORD;
BEGIN
        IF NEW.numfactura IS NULL THEN
                SELECT INTO asd max(numfactura) AS m FROM factura WHERE idserie_factura=NEW.idserie_factura AND idalmacen = NEW.idalmacen;
		IF FOUND THEN
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
--Concepte: Descripció del motiu de descompte.
--Proporcio: Percentatge a descomptar.
-- Descompte de pressupost a clients.
CREATE TABLE dfactura (
   iddfactura serial PRIMARY KEY,
   conceptdfactura character varying(2000),
   proporciondfactura numeric(12,2),
   idfactura integer NOT NULL REFERENCES factura(idfactura)
   -- Falta poner el lugar donde se aplica el descuento, antes de la factura o después de ésta.
); 
    

-- Linea de presupuesto
-- Numero
-- Descripcio: Descripció de l'article en el moment de ser presupostat.
-- Quantitat
-- PVP: Preu de l'article en el moment de ser pressupostat
-- Descompte: Percentatge de descompte en línia.
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
   ffacturap date,
   descfacturap character varying(500),   
   contactfacturap character varying(90),
   telfacturap character varying(20),
   comentfacturap character varying(3000),
   procesadafacturap boolean DEFAULT FALSE, 
   idusuari integer,
   idproveedor integer REFERENCES proveedor(idproveedor),
   idforma_pago integer REFERENCES forma_pago(idforma_pago)
);

CREATE FUNCTION restriccionesfacturap () RETURNS "trigger"
AS '
DECLARE
asd RECORD;
BEGIN
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
-- Descripcio: Descripció de l'article en el moment de ser presupostat.
-- Quantitat
-- PVP: Preu de l'article en el moment de ser pressupostat
-- Descompte: Percentatge de descompte en línia.
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
-- -------------------------------------------------------------------------------------------

-- FACTURACIO>Albarans:
-- Albarans pendents: S'entendran com albarans pendents tots aquells dels quals no existeixi ticket, factura ni nofactura.
-- Numero: Numero de nofactura
-- Data
-- Concepte: Descripcio del concepte pel qual no es poden facturar els albarans aqui agrupats (garantia, -- contracte de manteniment, regals a clients, etc...).
-- Observacions
-- Agrupacio d'albarans no facturables en funció d'un determinat concepte.
CREATE TABLE nofactura (
	idnofactura serial PRIMARY KEY,
   numnofactura integer NOT NULL,
   fechanofactura date,
   conceptnofactura character varying(150),
   observnofactura character varying(150),
	idalmacen integer NOT NULL REFERENCES almacen(idalmacen),
	UNIQUE (idalmacen, numnofactura)
);



-- COMPROVACIONS D'INTEGRITAT>Genèriques:
-- Tots els albarans d'una factura corresponen al mateix client.
-- FACTURACIO>Albarans:
-- Albarans pendents: S'entendran com albarans pendents tots aquells dels quals no existeixi ticket, factura ni nofactura.
-- Numero
-- Data
-- Albarà a clients.
CREATE TABLE albaranp (
   idalbaranp serial PRIMARY KEY,
   numalbaranp integer NOT NULL UNIQUE,
   descalbaranp character varying(150),
   refalbaranp character varying(12) NOT NULL,
   fechaalbaranp date DEFAULT now(),
   loginusuario character varying(15) REFERENCES usuario(loginusuario),
   comentalbaranp character varying(3000),
   procesadoalbaranp boolean DEFAULT FALSE,
   idproveedor integer REFERENCES proveedor(idproveedor),
   idforma_pago integer REFERENCES forma_pago(idforma_pago),
--   idfactura integer REFERENCES factura(idfactura),
--   idnofactura integer REFERENCES nofactura(idnofactura),
   idalmacen integer NOT NULL REFERENCES almacen(idalmacen),
   UNIQUE (idalmacen, numalbaranp)
);

CREATE FUNCTION restriccionesalbaranp () RETURNS "trigger"
AS '
DECLARE
asd RECORD;
BEGIN
        IF NEW.numalbaranp IS NULL THEN
                SELECT INTO asd max(numalbaranp) AS m FROM albaranp;
		IF FOUND THEN
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
-- Línia d'albarà a clients.
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

CREATE FUNCTION disminuyestockp () RETURNS "trigger"
AS '
DECLARE
BEGIN
	UPDATE articulo SET stockarticulo = stockarticulo - OLD.cantlalbaranp WHERE idarticulo= OLD.idarticulo;
	RETURN OLD;
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
	RETURN NEW;
END;
' LANGUAGE plpgsql;


CREATE TRIGGER aumentastockpt
    AFTER INSERT OR UPDATE ON lalbaranp
    FOR EACH ROW
    EXECUTE PROCEDURE aumentastockp(); 


-- COMPROVACIONS D'INTEGRITAT>Genèriques:
-- Tots els albarans d'una factura corresponen al mateix client.
-- FACTURACIO>Albarans:
-- Albarans pendents: S'entendran com albarans pendents tots aquells dels quals no existeixi ticket, factura ni nofactura.
-- Numero
-- Data
-- Albarà a clients.
CREATE TABLE albaran (
   idalbaran serial PRIMARY KEY,
   numalbaran integer NOT NULL UNIQUE,
   descalbaran character varying(150),
   refalbaran character varying(12) NOT NULL,
   fechaalbaran date,
   loginusuario character varying(15) REFERENCES usuario(loginusuario),
   comentalbaran character varying(3000),
   procesadoalbaran boolean DEFAULT FALSE,
   idcliente integer REFERENCES cliente(idcliente),
   idforma_pago integer REFERENCES forma_pago(idforma_pago),
   idfactura integer REFERENCES factura(idfactura),
   idnofactura integer REFERENCES nofactura(idnofactura),
   idalmacen integer NOT NULL REFERENCES almacen(idalmacen),
   UNIQUE (idalmacen, numalbaran)
);

-- **********************************************************************
-- APARTADO DE COMPROBACIONES DE INTEGRIDAD EXTRA Y DETECCIÓN DE ERRORES.
-- **********************************************************************
-- **********************************************************************
CREATE FUNCTION restriccionesalbaran () RETURNS "trigger"
AS '
DECLARE
asd RECORD;
BEGIN
        IF NEW.numalbaran IS NULL THEN
                SELECT INTO asd max(numalbaran) AS m FROM albaran;
		IF FOUND THEN
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
-- Concepte: Descripció del motiu de descompte.
-- Proporcio: Percentatge a descomptar.
-- Descompte d'albarà a clients.
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
-- Línia d'albarà a clients.
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
CREATE FUNCTION disminuyestock () RETURNS "trigger"
AS '
DECLARE
BEGIN
	UPDATE articulo SET stockarticulo = stockarticulo + OLD.cantlalbaran WHERE idarticulo= OLD.idarticulo;
	RETURN OLD;
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
	UPDATE articulo SET stockarticulo = stockarticulo - NEW.cantlalbaran WHERE idarticulo = NEW.idarticulo;
	RETURN NEW;
END;
' LANGUAGE plpgsql;


CREATE TRIGGER aumentastockt
    AFTER INSERT OR UPDATE ON lalbaran
    FOR EACH ROW
    EXECUTE PROCEDURE aumentastock();    
    
    
-- FACTURACIO>Albarans:
-- Albarans pendents: S'entendran com albarans pendents tots aquells dels quals no existeixi ticket, factura ni nofactura.
CREATE TABLE ticket (
   numticket integer PRIMARY KEY,
   fechaticket date
);


-- COMPROVACIONS D'INTEGRITAT>Genèriques:
-- 1 Article té 1 sol proveïdor principal.
-- 1 Article té 1 sol proveïdor referent.
CREATE TABLE suministra (
   idsuministra serial PRIMARY KEY,
   refpro character varying(100),
   principalsuministra numeric(12,2),
   idproveedor integer REFERENCES proveedor(idproveedor),
   idarticulo integer REFERENCES articulo(idarticulo)
);






-- Los tipos de tarifa permiten tener diferentes precios para un mismo artículo en función de alguna variable que queramos definir (para un cliente, para una zona, si es para un minorista, tienda propia franquiciada ...
-- codigo: es un identificador pnemotécnico de la tarifa.
-- desc: es un texto descriptivo del tipo de tarifa.

CREATE TABLE tipo_tarifa ( 
	idtipo_tarifa serial PRIMARY KEY,
	codtipo_tarifa character varying(10) NOT NULL UNIQUE,
	desctipo_tarifa character varying(50)
);


-- La tabla tarifa contiene los precios de venta y oferta incluidas las ofertas MxN.
-- idalmacen: Almacén o tienda a la que corresponden los precios.
-- idarticulo: idetificador del articulo al que corresponde el precio.
-- finicio: fecha de inicio vigencia del precio
-- ffin: fecha de finalización de vigencia del precio.
-- esoferta: indica si el precio es de oferta.
-- esmxn: indica si la oferta es mxn (p.e. 3x2).
-- cantidadm: cantidad de unidades para primer valor en oferta mxn (valor de unidades llevadas).
-- cantidadn: cantidad de unidades para segundo valor en oferta mxn (valor de unidades pagadas).
-- FALTA DEFINIR LAS REGLAS PARA EVITAR SOLAPAMIENTOS ENTRE OFERTAS.

CREATE TABLE tarifa (
	idtarifa serial PRIMARY KEY,
   idalmacen integer NOT NULL REFERENCES almacen(idalmacen),
	idarticulo integer NOT NULL REFERENCES articulo(idarticulo),
	idtipo_tarifa integer NOT NULL REFERENCES tipo_tarifa(idtipo_tarifa),
	finiciotarifa date,
	ffintarifa date,
	preciotarifa numeric(13, 4),
	esofertatarifa character(1) NOT NULL CHECK(esofertatarifa='S' OR esofertatarifa='N'),
	esmxntarifa character(1),
	cantidadmtarifa numeric(5, 0),
	cantidadntarifa numeric(5, 0)	
);


-- Restricciones para la tabla tarifa:
-- Para un mismo almacén, artículo y tarifa, no puede haber más de un precio a una misma fecha
-- Para un mismo almacén, artículo y tarifa, no puede haber más de una oferta precio a una misma fecha
-- Sí que se permite que haya solapamiento entre la tarifa normal y una oferta. Prevalece siempre la oferta.

--DROP TRIGGER restriccionestarifatrigger ON cuenta CASCADE;
--DROP FUNCTION restriccionestarifa();

CREATE FUNCTION restriccionestarifa () RETURNS "trigger"
AS '
DECLARE
	cont INTEGER;
BEGIN

RAISE NOTICE '' IDTARIFA = % '',NEW.idtarifa;
	 SELECT count(*) INTO cont FROM tarifa 
		WHERE tarifa.idtipo_tarifa = NEW.idtipo_tarifa AND tarifa.idarticulo = NEW.idarticulo AND tarifa.idalmacen = NEW.idalmacen AND tarifa.esofertatarifa = NEW.esofertatarifa AND
			tarifa.idtarifa != NEW.idtarifa AND tarifa.finiciotarifa <= NEW.finiciotarifa AND tarifa.ffintarifa >= NEW.finiciotarifa;
				if (NOT(cont ISNULL) AND cont > 0) THEN
 					RAISE EXCEPTION '' Solapamiento de fechas en fecha inicio '';
				END IF;
	RAISE NOTICE '' CONTADOR = % '', cont;

 	SELECT count(*) INTO cont FROM tarifa 
		WHERE tarifa.idtipo_tarifa = NEW.idtipo_tarifa AND tarifa.idarticulo = NEW.idarticulo AND tarifa.idalmacen = NEW.idalmacen AND tarifa.esofertatarifa = NEW.esofertatarifa AND
			tarifa.idtarifa != NEW.idtarifa AND tarifa.finiciotarifa <= NEW.ffintarifa AND tarifa.ffintarifa >= NEW.ffintarifa;
				if (NOT(cont ISNULL) AND cont > 0) THEN
 					RAISE EXCEPTION '' Solapamiento de fechas en fecha fin '';
				END IF;
	RAISE NOTICE '' CONTADOR = % '', cont;

        RETURN NEW;
END;
' LANGUAGE plpgsql;


CREATE TRIGGER restriccionestarifatrigger
    BEFORE INSERT OR UPDATE ON tarifa
    FOR EACH ROW
    EXECUTE PROCEDURE restriccionestarifa();

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