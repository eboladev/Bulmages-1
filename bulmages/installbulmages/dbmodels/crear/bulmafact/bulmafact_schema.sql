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
-- En caso de que haya diversas claves foraneas referentes al mismo campo, el criterio es que una de lleas tenga el nombre del campo con el que se corresponde y la otra tenga un nombre significativo.
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
-- Nom: Nom identificatiu del magatzem.
-- Adr: Adreça.
-- Pobl: Població.
-- CProv: Codi de provincia (dos primers dígits del codi postal).
-- sCP: Tres darrers dígits del codi postal.
-- Telf: Telèfon.
-- Fax: Fax.
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
 UNIQUE(codigoalmacen)
);



-- Codi: Clau artificial.
-- Nom
-- Url
-- Comentaris
CREATE TABLE marca (
   idmarca serial PRIMARY KEY,
   nommarca character varying(150),
   urlmarca character varying(150),
   comentmarca character varying(2000)
);


-- Linea de producto, las lineas de producto son una catalogación de productos por linea.
-- Codi: Clau artificial.
-- Descripcio
CREATE TABLE linea_prod(
   idlinea_prod serial PRIMARY KEY,
   desclinea_prod character varying(500),
   idmarca integer REFERENCES marca(idmarca)
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
-- codcompfamilia código compuesto de familia: codigo de sector + código de sección + código de familia.
-- comprobaciones de integridad respecto a la tabla sector con idsector.
-- comprobaciones de integridad respecto a la tabla seccion con idseccion. 
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
		codigocompleto := codigocompleto || as.codigocompletofamilia;
	END IF;
        NEW.codigocompletofamilia := codigocompleto;
	RETURN NEW;
END;
' LANGUAGE plpgsql;


CREATE TRIGGER calculacodigocompletofamiliatrigger
    BEFORE INSERT OR UPDATE ON familia
    FOR EACH ROW
    EXECUTE PROCEDURE calculacodigocompletofamilia();



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



--COMPROVACIONS D'INTEGRITAT>Genriques:
--Els articles "contenidors" de tots els components son articles composts.
--Els articles "contingut" de tots els components son articles no composts.
--COMPROVACIONS D'INTEGRITAT>A realitzar en moments especEn convertir un article a compost, comprovar abans que no s component de ning.
--En afegir components a un article, comprovar que el nou component no s un article compost.
--PANTALLES ESTADISTIQUES>Articles:
--Nombre d'articles que no pertanyen a cap familia.
--
--tipoarticulo: [Simple | Compost calculat | Compost escalat | Ampliat]. Determina el tipus d'article i com calcular-ne el preu. Simple = Article no compost amb preu PVP; Compost = Article compost amb preu suma dels PVPs dels components (Calculat) o PVP (escalat); Ampliat = Compost d'ell mateix ms els components amb preu suma de PVP i PVPs dels components
--

-- Codi: Clau artificial.
-- Nom: Descripció curta de l'article.
-- Descripcio: Descripció completa (llarga) de l'article.
-- CBarres: Codi de barres.
-- Tipus: [Simple (0) | Compost calculat (1) | Compost escalat (2) | Ampliat (3)]. Determina el tipus d'article i com --  calcular-ne el preu. Simple = Article no compost amb preu PVP; Compost = Article compost amb preu suma dels PVPs dels components (Calculat) o PVP (escalat); Ampliat = Compost d'ell mateix més els components amb preu suma de PVP i PVPs dels components.
-- Descompte: Descompte "invisible" que s'aplica al preu resultant. Útil sobretot per a diferenciar el preu dels articles composts del preu de compra per separat.
-- Especificacions: Camp de text per a comentaris i informacions varies.
-- Icona: Icona de l'article.
-- Fotografia: Fotografia de l'article.
-- Poster: Fotografia de gran tamany de l'article.
-- Marge: (Per defecte) Percentatge de càlcul del PVP sobre el PVD. Vàlid només quan no estigui definit un marge específic per aquest article i el proveïdor corresponent.
-- Sobrecost: (Per defecte) Import a sumar en el càlcul del PVP en concepte de despeses de transport o altres.
-- Model: Referència, o nom identificatiu del fabricant.

-- El campo codigocompletoarticulo sólo puede ser de lectura.
CREATE TABLE articulo (
    idarticulo serial PRIMARY KEY,
    codarticulo character varying(12),
    nomarticulo character varying(50),
    descarticulo character varying(500),
    cbarrasarticulo character varying(22),
    tipoarticulo integer,
    descuentoarticulo float,
    especificacionesarticulo character varying(2000),
    iconoarticulo oid,
    fotoarticulo oid,
    posterarticulo oid,
    margenarticulo float,
    sobrecostearticulo float,
    modeloarticulo character varying(1000),
    idtipo_iva integer REFERENCES tipo_iva (idtipo_iva),
    idlinea_prod integer REFERENCES linea_prod(idlinea_prod),
    codigocompletoarticulo character varying(100) UNIQUE,
    idfamilia integer REFERENCES familia(idfamilia) NOT NULL
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
		codigocompleto := codigocompleto || as.codigocompletofamilia;
	END IF;
        NEW.codigocompletoarticulo := codigocompleto;
	RETURN NEW;
END;
' LANGUAGE plpgsql;


CREATE TRIGGER calculacodigocompletoarticulotrigger
    BEFORE INSERT OR UPDATE ON articulo
    FOR EACH ROW
    EXECUTE PROCEDURE calculacodigocompletoarticulo();




-- Yo me imagino que un catalogo es una agrupación de articulos para poder hacer listados
-- de ofertas y cosas de estas.
--La catalogació per families tendrà preferència sobre la catalogació per articles. Si un article pertany a un catàleg sel·leccionat però no a cap familia sel·leccionada pel seu catàleg, no serà llistat.
--Codi: Clau artificial.
--Nom: Descripció curta del catàleg.
--Descripcio: Descripció detallada del catàleg.
--Exclusiu: (Booleà) La pertinença a un catàleg exclusiu anula (que no esborra) la pertinença de l'article o familia a qualsevol catàleg no exclusiu. Un article o familia només pot pertànyer a un únic catàleg exclusiu. Útil per a catàlegs de descatalogació (descatalogats temporals, definitius, sense stock, pendents de revisió...).
CREATE TABLE catalogo (
   idcatalogo serial PRIMARY KEY,
   nombrecatalogo character varying(50),
   desccatalogo character varying(1000),
   exclusivocatalogo boolean NOT NULL
);


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
   clavewebproveedor character varying(100)
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
   idproveedor integer NOT NULL REFERENCES proveedor(idproveedor)
);



--TARIFES AUTOMÀTIQUES:
--Els prearticles son els articles obtinguts de tarifes de proveïdors que no tenim com a articles però que s'enregistren (i esborren en caducar) automàticament per oferir-nos una recerca ràpida sobre els articles dels nostres proveïdors que no hem treballat mai.
--Refpro: Referència del proveïdor.
--Descripcio: Descripció de l'article.
--PVD
--Data: Data d'actualització del PVD del prearticle.
--Caducitat: Temps de vida (en dies) del prearticle a comptar des de la darrera data d'actualització.
-- Això ha de desapareixer així que no pos res aqui.
--CREATE TABLE prearticulo (
--);



-- La tabla recargo indica los recargos que se aplican a un cliente. O los descuentos.
-- Codi: Clau artificial.
-- Nom: Nom descriptiu del tipus de Recàrrec.
-- Taxa: Percentatge de Recàrrec d'Equivalència.
--  Recàrrec d'Equivalència.
CREATE TABLE recargo (
   idrecargo serial PRIMARY KEY,
   nomrecargo character varying(150),
   tasarecargo float
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

   idrecargo integer NOT NULL REFERENCES recargo(idrecargo)
);


-- Numero: Número de sucursal (Clau artificial).
-- Nom: Nom de la sucursal.
-- Adr: Adreça.
-- Pobl: Població.
-- CProv: Codi de provincia (dos primers dígits del codi postal).
-- sCP: Tres darrers dígits del codi postal.
-- Telf: Telèfon.
-- Fax: Fax.
-- Email: eMail.
-- Url: Url.
-- Comentaris
CREATE TABLE sucursal (
   idsucursal serial PRIMARY KEY,
   nomsucursal character varying(100),
   dirsucursal character varying(150),
   poblsucursal character varying(100),
   cpsucursal character varying(15),
   telsucursal character varying(20),
   faxsucursal character varying(20),
   mailsucursal character varying(50),
   comentsucursal character varying(2000),

   idcliente integer NOT NULL REFERENCES cliente(idcliente)
);



-- Any: Any en que s'efectua la comanda.
-- Numero: Número de comanda (començant de 1 cada any).
-- Descripcio: Breu descripció o comentari opcional.
-- Data: Data d'emisió de la comanda.
CREATE TABLE pedido (
   idpedido serial PRIMARY KEY,
   numpedido character varying(60),
   anopedido integer,
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
   anofra_pro integer,
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
   anoalb_pro integer,
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




-- modalidad_g es la talba que contiene las modalidades de garanti9a.
-- Codi: Clau artificial.
-- Nom
-- Descripcio
CREATE TABLE modalidad_g (
   idmodalidad_g serial PRIMARY KEY,
   nommodalitat_g character varying(150),
   descmodalidad_g character varying(2000)
);


-- Codi
-- Descripcio
-- RetardC: Temps a transcórrer per a l'inici de la cobertura.
-- TempsC: Temps de Cobertura.
-- Comentaris
-- Condicions de garantia. Defineixen un periode d'aplicabilitat d'una determinada modalitat de garantia per a una determinada línia de productes d'un mateix fabricant.
CREATE TABLE cond_garantia (
  idcond_garantia serial PRIMARY KEY,
  desccond_garantia character varying(2000),
  retardcond_garantia timestamp,
  tempsccond_garantia timestamp,
  comentcond_garantia character varying(2000),

  idmodalidad_g integer NOT NULL REFERENCES modalidad_g(idmodalidad_g)
);



-- Condicions de Venta, Agrupa articles, formes de pagament i clients.
CREATE TABLE condiciones_v (
   idcondiciones_v serial PRIMARY KEY
);

-- Numero
-- Descompte
-- AnyInici: Any d'inici d'aplicació (NULL => Periòdic)
-- MesInici: Mes d'inici d'aplicació (NULL => Periòdic)
-- DiaInici: Dia d'inici d'aplicació (NULL => Des de sempre)
-- AnyFi: Any de fi d'aplicació (NULL => Periòdic)
-- MesFi: Mes de fi d'aplicació (NULL => Periòdic)
-- DiaFi: Dia de fi d'aplicació (NULL => Indefinit)
-- Comentari
CREATE TABLE oferta (
   numoferta integer PRIMARY KEY,
   descoferta float,
   fechainicio timestamp,
   fechafin timestamp,
   periodicidad integer
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


-- Numero
-- Proporcio: Percentatge a abonar en aquest termini.
-- Dies: Dies a transcorrer a partir de la data de facturació computant els blocs de 30 com a mesos naturals.
CREATE TABLE termino_fp (
   idtermino_fp serial PRIMARY KEY,
   proportermino_fp float,
   diastermino_fp integer
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



-- Numero
-- Data: Data del venciment.
-- Import: Import a abonar.
-- Abonat
-- Venciment d'un pagament.
CREATE TABLE vencimiento (
   idvencimiento serial PRIMARY KEY,
   fechavencimiento date,
   importevencimiento float,
   abonadovencimiento float,
   idfactura integer REFERENCES factura(idfactura)
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
   refpro integer,
   pvdsuministra float,
   beneficiosuministra float,
   sobrecostesuministra float,
   principalsuministra float,
   referentsuministra float,

   idproveedor integer REFERENCES proveedor(idproveedor),
        idarticulo integer REFERENCES articulo(idarticulo)
);


-- Con cada pedido/albaran podemos asignar un numero de serie a los productos que han llegado.
-- Esta relación es con los pedidos y no con los articulos pq los articulos son genericos
-- y los pedidos instancian los stocks de los pedidos.
--Numero: Numero de srie d'un article.
--Tipus: (Intern|Majorista|Fabricant) Indica si s un nmero intern (posat per nosaltres), del majorista o del fabricant.
-- Intern 1
-- Majorista 2
-- Fabricante 3
-- Segun mis calculos un numero de serie debe ir sólo con un pedido o sólo con un albaran
-- Asi que los campos pueden ser NULL pq o bien va con uno o bien va con el otro.
CREATE TABLE num_serie (
    numnum_serie character varying(30) PRIMARY KEY,
    tiponumserie integer,
    numlpedido integer REFERENCES lpedido (numlpedido),
    numlalbaran integer REFERENCES lalbaran (numlalbaran)
);



CREATE TABLE artC (
   idcatalogo integer REFERENCES catalogo(idcatalogo),
   idarticulo integer REFERENCES articulo(idarticulo)
);



CREATE TABLE componente (
   idarticulo integer REFERENCES articulo(idarticulo),
   articulocomponente integer REFERENCES articulo(idarticulo),
   id integer,
   prioridad integer,
   descripcion character varying(200),
   cantidad float,
   escalable boolean
);

CREATE TABLE stock (
   idarticulo integer REFERENCES articulo(idarticulo),
   idalmacen integer REFERENCES almacen(idalmacen),
   minimo float,
   maximo float,
   actual float
);


CREATE TABLE dxart (
   idcliente integer REFERENCES cliente(idcliente),
   idarticulo integer REFERENCES articulo(idarticulo),
   descdxart character varying(150)
);

-- Esta tabla tiene algo que ver con las ofertas
-- pero aun no he descubierto que es (si alguien logra saberlo algun dia
-- por favor que satisfaga mi curiosidad.
CREATE TABLE dxof (
  idarticulo integer REFERENCES articulo(idarticulo),
  numoferta integer REFERENCES oferta(numoferta)
);

-- Esta debe de ser la tabla de representantes.
CREATE TABLE representa (
   idusuario integer, -- no hace referencia pq esta en base de datos externa y vinculada
   idcliente integer REFERENCES cliente(idcliente),
   permisrepresnta integer
);


-- Presupuesto - forma de pago
-- Esta tabla indica las formas de pago que tiene un presupuesto
CREATE TABLE prfp (
  idpresupuesto integer REFERENCES presupuesto(idpresupuesto),
  idforma_pago integer REFERENCES forma_pago(idforma_pago),
  descprfp float
);

-- Forma de pago posible para los articulos
CREATE TABLE afp (
   idarticulo integer REFERENCES articulo(idarticulo),
   idforma_pago integer REFERENCES forma_pago(idforma_pago)
);

CREATE TABLE albpre (
   idpresupuesto integer REFERENCES presupuesto(idpresupuesto),
   idalbaran integer REFERENCES albaran(idalbaran)
);

CREATE TABLE meta (
    id character varying(10) NOT NULL,
    valor character varying(50),
    PRIMARY KEY ("id")
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
