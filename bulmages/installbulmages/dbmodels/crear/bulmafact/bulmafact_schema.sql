-- ----------------------------------------------------------------------------------------
-- (C)  Joan Miquel Torrer Rigo & Tomeu Borras Riera & Mateu Borras Marco, Agosto 2004
--  joanmi@bulma.net, tborras@conetxia.com mborras@conetxia.com
-- Este documento esta licenciado bajo licencia GPL, el cual no escribimos aqui por pereza.
--  ----------------------------------------------------------------------------------------
--     psql xxxx < bulmafact-0_0_1.sql
--  ---------------------------------------------------------------------------------------

SET SESSION AUTHORIZATION 'postgres';
SET search_path = public, pg_catalog;


CREATE FUNCTION plpgsql_call_handler() RETURNS language_handler
    AS '/usr/lib/postgresql/lib/plpgsql.so', 'plpgsql_call_handler'
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
-- facturaautoalmacen el numero de factura es automatico? N=No, 
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
	idtipo_iva integer REFERENCES tipo_iva(idtipo_iva),
	porcentasa_iva NUMERIC(5, 2),
	fechatasa_iva date NOT NULL,
	UNIQUE (idtipo_iva, fechatasa_iva)
);


-- Tabla con series de Iva, código i descripción
-- Básicamente sirve para garantizar la integridad referencial en las  series de facturación
-- Deberían existir en contabilidad tambien.
CREATE TABLE serie_factura (
	idserie_factura serial PRIMARY KEY,
	codigoserie_factura character(2),
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
    idtipo_articulo numeric(2, 0) REFERENCES tipo_articulo(idtipo_articulo),
    idtipo_iva integer REFERENCES tipo_iva (idtipo_iva),
    codigocompletoarticulo character varying(100) UNIQUE,
    idfamilia integer REFERENCES familia(idfamilia) NOT NULL,
	 inactivoarticulo character(1)
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
   cantlpedido float,
   pvdlpedido float,
   prevlpedido date,

   idpedido integer NOT NULL REFERENCES pedido(idpedido),
   idalb_pro integer REFERENCES alb_pro(idalb_pro),
   idarticulo integer REFERENCES articulo(idarticulo)
--      PRIMARY KEY(idpedido, numlpedido)
);


-- Codi: Clau artificial.
-- Descripcio: Nom identificatiu o descripció breu.
-- Dies_1T: Dies abans del primer termini computant els blocs de 30 com a mesos naturals.
-- Descompte: Descompte automàtic per l'ús d'aquesta forma de pagament.
CREATE TABLE forma_pago (
   idforma_pago serial PRIMARY KEY,
   descforma_pago character varying(500),
   dias1tforma_pago integer,
   descuentoforma_pago float
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
   numpresupuesto integer NOT NULL,
   fpresupuesto date,
   contactpresupuesto character varying(90),
   telpresupuesto character varying(20),
   vencpresupuesto date,
   comentpresupuesto character varying(3000),
   idusuari integer,

   idcliente integer REFERENCES cliente(idcliente),
	idalmacen integer NOT NULL REFERENCES almacen(idalmacen),
	UNIQUE (idalmacen, numpresupuesto)
);


-- Descuento de presupuesto.
-- Numero
--Concepte: Descripció del motiu de descompte.
--Proporcio: Percentatge a descomptar.
-- Descompte de pressupost a clients.
CREATE TABLE dpresupuesto (
   iddpresupuesto serial PRIMARY KEY,
   conceptdpresupuesto character varying(2000),
   proporciondpresupuesto float,
   idpresupuesto integer REFERENCES presupuesto(idpresupuesto)
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
   cantlpresupuesto float,
   pvplpresupuesto float,
   descuentolpresupuesto float,

   idpresupuesto integer NOT NULL REFERENCES presupuesto(idpresupuesto),
   idarticulo integer REFERENCES articulo(idarticulo)
);


-- FACTURACIO>Albarans:
-- Albarans pendents: S'entendran com albarans pendents tots aquells dels quals no existeixi ticket, factura ni nofactura.
-- Numero
-- Data
-- Factura a clients.
CREATE TABLE factura (
	idfactura serial PRIMARY KEY,
	idserie_factura char(2) NOT NULL,
   numfactura integer NOT NULL,
   fechafactura date,
	idalmacen integer NOT NULL REFERENCES almacen(idalmacen),
	UNIQUE (idalmacen, idserie_factura, numfactura)
);


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
CREATE TABLE albaran (
   idalbaran serial PRIMARY KEY,
   numalbaran integer NOT NULL,
   fechaalbaran date,
   idusuario integer,
   comentalbaran character varying(3000),
--   idpresupuesto integer REFERENCES presupuesto(idpresupuesto),
   idcliente integer REFERENCES cliente(idcliente),
   idforma_pago integer REFERENCES forma_pago(idforma_pago),
   idfactura integer REFERENCES factura(idfactura),
   idnofactura integer REFERENCES nofactura(idnofactura),
	idalmacen integer NOT NULL REFERENCES almacen(idalmacen),
	UNIQUE (idalmacen, numalbaran)
);


-- Descuento albaran
-- Numero
-- Concepte: Descripció del motiu de descompte.
-- Proporcio: Percentatge a descomptar.
-- Descompte d'albarà a clients.
CREATE TABLE dalbaran (
   numdalbaran serial PRIMARY KEY,
   conceptdalbaran character varying(500),
   propordalbaran float,

   idalbaran integer REFERENCES albaran(idalbaran)
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
   cantlalbaran float,
   pvplalbaran float,
   descontlalbaran float,

   idalbaran integer REFERENCES albaran(idalbaran),
   idarticulo integer REFERENCES articulo(idarticulo)
);


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
   principalsuministra float,

   idproveedor integer REFERENCES proveedor(idproveedor),
   idarticulo integer REFERENCES articulo(idarticulo)
);


-- ESTA TABLA DEBE DESAPARECER -- LO HARE CUANDO TOQUE EL LA ENTRADA DEL PRESUPUESTO.
-- Presupuesto - forma de pago
-- Esta tabla indica las formas de pago que tiene un presupuesto
CREATE TABLE prfp (
  idpresupuesto integer REFERENCES presupuesto(idpresupuesto),
  idforma_pago integer REFERENCES forma_pago(idforma_pago),
  descprfp float
);


CREATE TABLE usuarios (
    login character varying(15) NOT NULL,
    nombre character varying(35),
    apellido1 character varying(35),
    apellido2 character varying(35),
    clave character varying(35),
    permisos text,
    PRIMARY KEY ("login")
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
