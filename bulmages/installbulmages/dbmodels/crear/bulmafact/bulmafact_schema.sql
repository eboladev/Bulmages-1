-- -------------------------------------------------------------------------------------------
-- (C) Joan Miquel Torres Rigo & Tomeu Borras Riera & Mateu Borras Marco, Agosto 2007
-- joanmi@bulma.net, tborras@conetxia.com mborras@conetxia.com
-- Revisado, Jose Maria Avendaño Cabezas, Junio 2008
-- jac_bulmages@ya.com
-- 
--   This program is free software; you can redistribute it and/or modify
--   it under the terms of the GNU General Public License as published by
--   the Free Software Foundation; either version 2 of the License, or
--   (at your option) any later version.
--
--   This program is distributed in the hope that it will be useful,
--   but WITHOUT ANY WARRANTY; without even the implied warranty of
--   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--   GNU General Public License for more details.
--
--   You should have received a copy of the GNU General Public License
--   along with this program; if not, write to the
--   Free Software Foundation, Inc.,
--   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
-- -------------------------------------------------------------------------------------------
-- Usage:    psql database_name < bulmafact_schema.sql
-- -------------------------------------------------------------------------------------------

\echo '********* INICIADO FICHERO DE ESTRUCTURA DE LA BASE DE DATOS DE BULMAFACT *********'

\echo ':: Establecemos los mensajes minimos a avisos y otros parametros ... '
\echo -n ':: '
SET client_min_messages TO warning;

\echo -n ':: '
SET SESSION AUTHORIZATION 'postgres';
\echo -n ':: '
SET search_path = public, pg_catalog;

\echo -n ':: '
SET DATESTYLE TO European;

\echo -n ':: plpgsql_call_handler ... '
CREATE FUNCTION plpgsql_call_handler() RETURNS language_handler
    AS '$libdir/plpgsql', 'plpgsql_call_handler'
    LANGUAGE c;
    
\echo -n ':: Establecemos el lenguaje de procedimientos ... '
--~ CREATE TRUSTED PROCEDURAL LANGUAGE plpgsql HANDLER plpgsql_call_handler;

CREATE OR REPLACE FUNCTION create_language_plpgsql()
 RETURNS BOOLEAN AS $$
     CREATE LANGUAGE plpgsql;
     SELECT TRUE;
 $$ LANGUAGE SQL;

SELECT CASE WHEN NOT
     (
         SELECT  TRUE AS exists
         FROM    pg_language
         WHERE   lanname = 'plpgsql'
         UNION
         SELECT  FALSE AS exists
         ORDER BY exists DESC
         LIMIT 1
     )
 THEN
     create_language_plpgsql()
 ELSE
     FALSE
 END AS plpgsql_created;

DROP FUNCTION create_language_plpgsql();


-- NOTACION:
-- Considerar las siguientes opciones de codificacion:
-- Los nombres de tabla estan escritos SIEMPRE en singular.
-- Todos los campos de una tabla terminan siempre con el nombre de la tabla (salvo las claves
-- foraneas). Las claves foraneas tienen el mismo nombre que el campo con que se corresponden
-- en la tabla relacionada. En caso de que haya diversas claves foraneas referentes al mismo
-- campo, el criterio es que una de ellas tenga el nombre del campo con el que se corresponde
-- y la otra tenga un nombre significativo. Los campos de clave automatica empiezan por id
-- Los enums se simulan (normalmente) con campos numericos, el significado de los valores debe
-- estar explicado en este archivo.

\echo -n ':: '
BEGIN;

\echo -n '::Funciones auxiliares ...'
-- Esta Funcion devuelve una fecha si la fecha es correcta o NULL si esta no es correcta.
-- Pensada para usar en conjuncion con COALESCE y no generar un error de SQL

 
-- ** configuracion **
-- En esta tabla se guardan parametros que el programa va a utilizar.
-- Como por ejemplo el numero de digitos por defecto de las cuentas o el asiento inteligente
-- que se enlaza con facturacion.
-- Tiene dos campos
-- nombre: El nombre del parametro de configuracion.
-- valor: El valor que toma dicho parametro.
\echo -n ':: Configuracion ... '
CREATE TABLE configuracion (
    nombre character varying(25) PRIMARY KEY,
    valor character varying(350)
);


-- ** pais **
-- idpais: Identificador de pais.
-- cod2pais: codigo de 2 caracteres de pais.
-- cod3pais: Codigo de 3 caracteres de pais.
-- descpais: Nombre del pais.
\echo -n ':: Pais ... '
CREATE TABLE pais (
	idpais serial PRIMARY KEY,
	cod2pais character varying(2),
	cod3pais character varying(3),
	descpais character varying(50)
);


-- ** provincia **
-- idprovincia: Identificador de provincia.
-- idpais: Identificador de pais.
-- provincia: Nombre de la provincia.
\echo -n ':: Provincia ... '
CREATE TABLE provincia (
	idprovincia serial PRIMARY KEY,
	idpais integer REFERENCES pais(idpais),
	provincia character varying(500)
);


-- ** banco **
-- nombanco: Nombre de la entidad bancaria.
-- dirbanco: Direccion de la entidad bancaria.
-- poblbanco: Poblacion de la entidad bancaria.
-- cpbanco: Codigo postal de la entidad bancaria.
-- telbanco: Telefono de la entidad bancaria (solo puede tener uno??)
-- faxbanco: Fax de la entidad bancaria.
-- emailbanco: Direccion de correo electronico de la entidad bancaria.
-- contactobanco: Persona de contacto en la entidad bancaria.
-- codentidadbanco: Codigo de la entidad bancaria.
-- codagenciabanco: Codigo de la agencia bancaria.
-- numcuentabanco : Codigo de la cuenta bancaria
-- dcbanco: Digito de control de la cuenta bancaria.
-- comentbanco: Comentario de la entidad bancaria.
-- webbanco: Direccion web de la entidad bancaria.
-- sufijo: código diferente para cada entitat receptora de ficheros de remesas de recibos q19
\echo -n ':: Banco ... '
CREATE TABLE banco (
    idbanco serial PRIMARY KEY,
    nombanco character varying(50),
    dirbanco character varying(150),
    poblbanco character varying(50),
    cpbanco character varying(20),
    telbanco character varying(20),
    faxbanco character varying(20),
    emailbanco character varying(100),
    contactobanco character varying,
    codentidadbanco character varying(14),
    codagenciabanco character varying(17),
    numcuentabanco  character varying(17),
    dcbanco character varying(2),
    comentbanco character varying,
    webbanco character varying(100),
    sufijobanco numeric(3,0)
);


-- ** forma_pago **
-- descforma_pago: Nombre identificativo o descripcion breve.
-- dias1tforma_pago: Dias antes del primer vencimiento calculando los bloque de 30 como
--                   meses naturales.
-- descuentoforma_pago: Descuento automatico para esa forma de pago.
-- idbanco: Si la forma de pago es a través de un banco este campo nos permite especificar dicho banco.
\echo -n ':: Forma de pago ... '
CREATE TABLE forma_pago (
    idforma_pago serial PRIMARY KEY,
    descforma_pago character varying(500),
    dias1tforma_pago integer,
    idbanco integer REFERENCES banco(idbanco),
    descuentoforma_pago numeric(12, 2)
);


-- ** almacen **
-- codigoalmacen: Codigo numerico del almacen.
-- nomalmacen: Nombre identificativo del almacen.
-- diralmacen: Direccion del almacen.
-- poblalmacen: Poblacion del almacen.
-- cpalmacen: Codigo Postal del almacen.
-- telalmacen: Telefono del almacen.
-- faxalmacen: Fax del almacen.
-- emailalmacen: Direccion de correo electronico del almacen.
-- tipoalmacen: Tipo de almacen (Tienda, Almacen, Coche ...)
-- inactivoalmacen:
\echo -n ':: Almacen ... '
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
    tipoalmacen  character varying,
    inactivoalmacen character(1),
    UNIQUE(codigoalmacen)
);


-- ** trabajador **
-- nomtrabajador: Nombre del trabajador.
-- apellidostrabajador: Apellidos del trabajador.
-- dirtrabajador: Direccion del trabajador.
-- nsstrabajador: Numero de la Seguridad Social del trabajador.
-- teltrabajador: Numero de telefono fijo del trabajador.
-- moviltrabajador: Numero de telefono movil del trabajador.
-- emailtrabajador: Direccion de correo electronico del trabajador.
-- fototrabajador: Fotografia del trabajador.
-- activotrabajador: Indica si el trabajador esta activo actualmente en la empresa.
\echo -n ':: Trabajador ... '
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


-- ** moneda **
-- cod2moneda: Codigo internacional de dos digitos.
-- cod3moneda: Codigo internacional de tres digitos.
-- descmoneda: Descripcion de la moneda.
\echo -n ':: Moneda ... '
CREATE TABLE moneda (
    idmoneda serial PRIMARY KEY,
    cod2moneda character varying(2),
    cod3moneda character varying(3),
    descmoneda character varying(50)
);


-- ** tipo_iva **
-- Esta tabla contiene las descripciones de los IVAs que se pueden aplicar.
-- desctipo_iva: Texto descriptivo del tipo de IVA.
\echo -n ':: Tipo de IVA ... '
CREATE TABLE tipo_iva (
    idtipo_iva serial PRIMARY KEY,
    desctipo_iva character varying(2000)
);


-- ** tasa_iva **
-- Esta tabla contiene las tasas de cada tipo de IVA a partir de una fecha dada.
-- idtipo_iva: 
-- porcentasa_iva: Contiene el porcentaje de la tasa de IVA a aplicar.
-- porcentretasa_iva: Indica el recargo de equivalencia a aplicar.
-- fechatasa_iva: Es la fecha de entrada en vigor del % de IVA para el tipo descrito.
\echo -n ':: Tasa de IVA ... '
CREATE TABLE tasa_iva (
    idtasa_iva serial PRIMARY KEY,
    idtipo_iva integer REFERENCES tipo_iva(idtipo_iva) NOT NULL,
    porcentasa_iva NUMERIC(5, 2) NOT NULL,
    porcentretasa_iva NUMERIC(5,2) DEFAULT 0,
    fechatasa_iva date NOT NULL,
    UNIQUE (idtipo_iva, fechatasa_iva)
);


-- ** serie_factura **
-- Tabla con series de IVA, codigo y descripcion.
-- Basicamente sirve para garantizar la integridad referencial en las series de facturacion.
-- Deberan existir en contabilidad tambien.
-- codigoserie_factura: Codigo para la serie de facturacion.
-- descserie_factura: Descripcion de la serie de facturacion.
\echo -n ':: Serie de factura ... '
CREATE TABLE serie_factura (
    codigoserie_factura character varying (6) PRIMARY KEY,
    descserie_factura character varying(50) NOT NULL,
    UNIQUE (codigoserie_factura)
);


-- ** familia **
-- codigofamilia: Codigo de la familia.
-- nombrefamilia: Nombre de la familia
-- descfamilia: Descripcion extendida de la familia.
-- padrefamilia: 
-- codigocompletofamilia: Codigo completo de la familia. Es la concatenacion del codigo de
--                        familia con sus codigos padrs. Este campo es de solo lectura.
-- productofisicofamilia: TRUE indica que es producto fisico, FALSE que es servicio.
\echo -n ':: Familia ... '
CREATE TABLE familia (
    idfamilia serial PRIMARY KEY,
    codigofamilia character varying(12) NOT NULL,
    nombrefamilia character varying(50) NOT NULL,
    descfamilia character varying(300),
    padrefamilia integer REFERENCES familia(idfamilia),
    codigocompletofamilia character varying(50) UNIQUE,
    productofisicofamilia boolean DEFAULT TRUE NOT NULL
);


\echo -n ':: Funcion que calcula el codigo completo de la familia ... '
CREATE FUNCTION calculacodigocompletofamilia() RETURNS "trigger"
AS '
DECLARE
    rs RECORD;
    codigocompleto character varying(50);

BEGIN
    codigocompleto := NEW.codigofamilia;
    SELECT INTO rs codigocompletofamilia FROM familia WHERE idfamilia = NEW.padrefamilia;
    IF FOUND THEN
   codigocompleto := rs.codigocompletofamilia || codigocompleto;
    END IF;
    NEW.codigocompletofamilia := codigocompleto;
    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador que calcula el codigo completo de la familia ... '
CREATE TRIGGER calculacodigocompletofamiliatrigger
    BEFORE INSERT OR UPDATE ON familia
    FOR EACH ROW
    EXECUTE PROCEDURE calculacodigocompletofamilia();
    
    
\echo -n ':: Funcion que propaga el codigo completo de la familia ... '
CREATE FUNCTION propagacodigocompletofamilia() RETURNS "trigger"
AS '
DECLARE

BEGIN
    UPDATE articulo SET codigocompletoarticulo = codigocompletoarticulo WHERE articulo.idfamilia = NEW.idfamilia;
    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador que propaga el codigo completo de la familia ... '
CREATE TRIGGER propagacodigocompletofamiliatrigger
    AFTER UPDATE ON familia
    FOR EACH ROW
    EXECUTE PROCEDURE propagacodigocompletofamilia();


-- ** tipo_articulo **
-- Permite crear una forma alternativa de agrupar los articulos.
-- codtipo_articulo: Codigo de tipo de articulo.
-- desctipo_articulo: Descripcion del tipo de articulo.
\echo -n ':: Tipo de articulo ... '
CREATE TABLE tipo_articulo (
    idtipo_articulo serial PRIMARY KEY,
    codtipo_articulo character varying(10),
    desctipo_articulo character varying(50)
);


-- ** articulo **
-- codarticulo: Codigo del articulo.
-- nomarticulo: Descripcion corta del articulo.
-- abrevarticulo: Nombre abreviado del articulo (para TPV o cartelitos de estanteria)
-- observarticulo: Campo de texto para a comentarios y observaciones.
-- presentablearticulo: Indica que se incluira en presentaciones (catalogos de articulos).
-- controlstockarticulo: Define si se ha de mantener control de stocks sobre existencias.
--                       Por defecto si (TRUE), si es un servicion seria no (FALSE).
-- idtipo_articulo: Identificador de tipo de articulo que se utilizara para agrupar
--                  articulos como clasificacion.
-- idtipo_iva: Identificador del tipo de IVA que se aplica a este articulo.
-- codigocompletoarticulo: Codigo completo del articulo. Este campo solo puede ser de lectura.
-- idfamilia: Identificador de la familia a la que pertenece el articulo.
-- stockarticulo: ???Total de unidades disponibles en stock.
-- inactivoarticulo: Indica si el articulo esta activo o no.
-- pvparticulo: ???
--
-- Peso del articulo.
-- Volumen del articulo.
\echo -n ':: Articulo ... '
CREATE TABLE articulo (
    idarticulo serial PRIMARY KEY,
    codarticulo character varying(12),
    nomarticulo character varying(50),
    abrevarticulo character varying(30),
    obserarticulo character varying(2000),
    presentablearticulo boolean NOT NULL DEFAULT TRUE,
    controlstockarticulo boolean NOT NULL DEFAULT TRUE,
    idtipo_articulo integer REFERENCES tipo_articulo(idtipo_articulo),
    idtipo_iva integer REFERENCES tipo_iva (idtipo_iva),
    codigocompletoarticulo character varying(100) UNIQUE,
    idfamilia integer REFERENCES familia(idfamilia) NOT NULL,
    stockarticulo numeric(12,2) DEFAULT 0,
    inactivoarticulo character(1),
    -- ATENCION, este campo no da el PVP real del articulo, solo es una de las multiples formas
    -- de acceder al precio del articulo.
    -- Para obtener el precio de un articulo se debe usar la funcion articulo.
    -- Para saber el iva correspondiente a un articulo se debe usar la funcion ivaarticulo.
    pvparticulo numeric(12,2) NOT NULL DEFAULT 0,
    pesoundarticulo numeric(12,2) DEFAULT 0,
    volumenundarticulo numeric(12,2) DEFAULT 0
);


\echo -n ':: Crea o reemplaza funcion is_number ... '
CREATE OR REPLACE FUNCTION is_number(varchar) RETURNS BOOLEAN AS
'select $1 ~ ''^[-+]?[0-9]+$''' strict immutable LANGUAGE sql;


\echo -n ':: Crea o reemplaza funcion to_number ... '
CREATE OR REPLACE FUNCTION to_number(character varying) RETURNS INT8 AS '
DECLARE
BEGIN
    RAISE NOTICE ''to_number %'', $1;
    RETURN CAST(text($1) AS INT8);
END
' LANGUAGE 'plpgsql';


\echo -n ':: Funcion que calcula el codigo completo del articulo ... '
CREATE OR REPLACE FUNCTION calculacodigocompletoarticulo() RETURNS "trigger"
AS $$
DECLARE
    rs RECORD;
    codigocompleto character varying(100);
    codnumeric integer;

BEGIN
    -- Lo primero comprobamos es que el codigo del articulo no este vacio y de ser asi lo llenamos.
    IF NEW.codarticulo = '' OR NEW.codarticulo ISNULL THEN
	SELECT INTO rs max(codarticulo::int)::varchar AS m FROM articulo WHERE idfamilia = NEW.idfamilia;

	IF FOUND THEN
	    IF is_number(rs.m) THEN
		codnumeric := to_number(rs.m);
		codnumeric := codnumeric +1;
		NEW.codarticulo := CAST (codnumeric AS varchar);
		WHILE length(NEW.codarticulo) < 4 LOOP
		    NEW.codarticulo := '0' || NEW.codarticulo;
		END LOOP;
	    ELSE
		NEW.codarticulo := '0000';
	    END IF;
	ELSE
	    NEW.codarticulo = '0000';
	END IF;
    END IF;

    codigocompleto := NEW.codarticulo;

    SELECT INTO rs codigocompletofamilia FROM familia WHERE idfamilia = NEW.idfamilia;

    IF FOUND THEN
	codigocompleto := rs.codigocompletofamilia || codigocompleto;
    END IF;

    NEW.codigocompletoarticulo := codigocompleto;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;


\echo -n ':: Disparador que calcula el codigo completo del articulo ... '
CREATE TRIGGER calculacodigocompletoarticulotrigger
    BEFORE INSERT OR UPDATE ON articulo
    FOR EACH ROW
    EXECUTE PROCEDURE calculacodigocompletoarticulo();


-- ** comparticulo **
-- idarticulo: Referencia del articulo que incluye componentes.
-- cantcomparticulo: Cantidad del articulo referenciado.
-- idcomponente: Referencia al articulo componente.
\echo -n ':: Componentes de articulo ... '
CREATE TABLE comparticulo (
    idarticulo integer NOT NULL REFERENCES articulo(idarticulo),
    cantcomparticulo numeric(12, 2) NOT NULL DEFAULT 1,
    idcomponente integer NOT NULL REFERENCES articulo(idarticulo),
    PRIMARY KEY (idarticulo, idcomponente)
);


\echo -n ':: Funcion con restricciones en los componentes de articulo ... '
CREATE FUNCTION restriccionescomparticulo() RETURNS "trigger"
AS '
DECLARE
BEGIN
    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador de las restricciones antes de insertar o actualizar un albaran ... '
CREATE TRIGGER restriccionesalbarantrigger
    BEFORE INSERT OR UPDATE ON comparticulo
    FOR EACH ROW
    EXECUTE PROCEDURE restriccionescomparticulo();



-- ** tarifa **
-- nomtarifa: Nombre de la tarifa.
-- finiciotarifa: Fecha de inicio vigencia del precio.
-- ffintarifa: Fecha de finalizacion de vigencia del precio.
\echo -n ':: Tarifa ... '
CREATE TABLE tarifa (
    idtarifa serial PRIMARY KEY,
    nomtarifa varchar(60),
    finiciotarifa date,
    ffintarifa date
);


-- ** ltarifa **
-- idalmacen:
-- idarticulo:
-- idtarifa:
-- pvpltarifa:: Precio de venta.
\echo -n ':: Lineas de tarifa ... '
CREATE TABLE ltarifa (
    idltarifa serial PRIMARY KEY,
    idalmacen integer NOT NULL REFERENCES almacen(idalmacen),
    idarticulo integer NOT NULL REFERENCES articulo(idarticulo),
    idtarifa integer NOT NULL REFERENCES tarifa(idtarifa),
    pvpltarifa numeric(12, 2)
);


\echo -n ':: Crea indice de lineas de tarifa ... '
CREATE UNIQUE INDEX indice_ltarifa ON ltarifa(idalmacen, idarticulo, idtarifa);


\echo -n ':: Funcion que calcula el precio segun cliente y almacen ... '
CREATE OR REPLACE FUNCTION pvparticuloclial(integer, integer, integer) RETURNS numeric(12, 2)
AS'
DECLARE
    idarticulo ALIAS FOR $1;
    idclient ALIAS FOR $2;
    idalmacen ALIAS FOR $3;
    rs RECORD;

BEGIN
    SELECT INTO rs pvpltarifa FROM ltarifa WHERE ltarifa.idarticulo = idarticulo AND ltarifa.idalmacen = idalmacen AND idtarifa IN (SELECT idtarifa FROM cliente WHERE idcliente = idclient);

    IF FOUND THEN
	RETURN rs.pvpltarifa;
    END IF;

    SELECT INTO rs pvparticulo FROM  articulo WHERE articulo.idarticulo = idarticulo;

    IF FOUND THEN
	RETURN rs.pvparticulo;
    END IF;

    RETURN 0.0;
END;
' LANGUAGE plpgsql;


-- ** proveedor **
-- Los proveedores son los que nos suministran articulos y/o servicios.
-- nomproveedor: Nombre fiscal del proveedor.
-- nomaltproveedor: Nombre comercial del proveedor.
-- cifprovedor: Codigo de Identificacion Fiscal.
-- codproveedor: Codigo de proveedor.
-- codicliprovedor: Codigo que el proveedor nos da para identificarnos como cliente suyo.
-- cbancproveedor: Codigo bancario del proveedor.
-- comentproveedor: Comentario.
-- dirproveedor: Direccion del proveedor.
-- poblproveedor: Poblacion del proveedor.
-- cpproveedor: Codigo Postal del proveedor.
-- telproveedor: Numero de telefono del proveedor.
-- faxproveedor: Numero de fax del proveedor.
-- emailproveedor: Direccion de correo electronico del proveedor.
-- corpproveedor: Nombre del grupo al que pertenece la empresa.
-- urlproveedor: URL de la web de comercio electronico B2B del proveedor.
-- clavewebproveedor: Clave para entrar en el comercio electronico B2B del proveedor.
-- inactivoproveedor: Indica si el proveedor lo tenemos marcado como inactivo para suministro.
-- recargoeqproveedor: Recargo de equivalencia del proveedor.
-- irpfproveedor: Indica el IRPF que usa el proveedor por defecto.
-- idforma_pago: Forma de pago al proveedor.
-- regimenfiscalproveedor: Regimen fiscal en que se encuentra el proveedor.
-- idprovincia: Provincia del proveedor.
\echo -n ':: Proveedor ... '
CREATE TABLE proveedor (
    idproveedor serial PRIMARY KEY,
    nomproveedor character varying(200),
    nomaltproveedor character varying(200),
    cifproveedor character varying(12) UNIQUE,
    codproveedor character varying(12) UNIQUE,
    codicliproveedor character varying(30),
    cbancproveedor character varying(120),
    comentproveedor character varying(2000),
    dirproveedor character varying(250),
    poblproveedor character varying(50),
    cpproveedor character varying(9) NOT NULL,
    telproveedor character varying(50),
    faxproveedor character varying(50),
    emailproveedor character varying(100),
    corpproveedor character varying(200),
    urlproveedor character varying(100),
    clavewebproveedor character varying(100),
    inactivoproveedor character(1),
    recargoeqproveedor boolean DEFAULT FALSE,
    irpfproveedor NUMERIC(12,2) DEFAULT 0,
    idforma_pago integer REFERENCES forma_pago(idforma_pago),
    regimenfiscalproveedor character varying(50) DEFAULT 'Normal' NOT NULL,
    idprovincia integer REFERENCES provincia(idprovincia)
);


-- ** division **
-- Detalle de todas las divisiones de un proveedor.
-- descdivision: Descripcion completa de la division.
-- contactosdivision: Nombre de persona o personas de contacto en esa division.
-- comentdivision: Comentario de la division.
-- teldivision: Numero de telefono de la division.
-- faxdivision: Numero de fax de la division.
-- maildivision: Direccion de correo electronico de la division.
-- idproveedor: Proveedor al que pertenece la division.
-- inactivodivision: Marca de inactivo de la division.
\echo -n ':: Division ... '
CREATE TABLE division (
    iddivision serial PRIMARY KEY,
    descdivision character varying(1000),
    contactosdivision character varying(500),
    comentdivision character varying(2000),
    teldivision character varying(20),
    faxdivision character varying(20),
    maildivision character varying(100),
    idproveedor integer NOT NULL REFERENCES proveedor(idproveedor),
    inactivodivision character(1)
);


-- ** cliente **
-- nomcliente: Nombre completo del cliente.
-- nomaltcliente: Nombre alternativo. Si es empresa nombre comercial.
-- cifcliente: Codigo de Identificacion Fiscal. CIF/NIF/NIE
-- codcliente: Codigo de cliente
-- bancocliente: Cuenta Corriente del banco del cliente donde domiciliar pagos.
-- dircliente: Direccion del cliente.
-- poblcliente: Poblacion de cliente.
-- cpcliente: Codigo Postal.
-- telcliente: Numero de telefono fijo particular.
-- teltrabcliente: Numero de telefono del trabajo.
-- movilcliente: Numero de telefono movil.
-- faxcliente: Numero de fax.
-- mailcliente: Direccion de correo electronico.
-- urlcliente: Direccion web de cliente.
-- corpcliente: Grupo al que pertenece el cliente.
-- faltacliente: Fecha alta como cliente.
-- fbajacliente: Fecha de baja como cliente.
-- regimenfiscalcliente Indica el regimen al que se acoge el cliente (Normal, comunitario, extracomunitario)
-- comentcliente: Comentario.
-- ecommercedatacliente: ???Comercio electronico.
-- inactivocliente:
-- idtarifa: Identificador de la tarifa aplicada a este cliente.
-- recargoeqcliente: Recargo de equivalencia a aplicar al cliente.
-- idforma_pago: Forma de pago del cliente.
-- departamento: ???Departamento al que pertenece el cliente.
-- idprovincia: Provincia del cliente.
\echo -n ':: Cliente ... '
CREATE TABLE cliente (
    idcliente serial PRIMARY KEY,
    nomcliente character varying(100) UNIQUE NOT NULL,
    nomaltcliente character varying(300),
    cifcliente character varying(200) UNIQUE,
    codcliente character varying(12) UNIQUE,
    bancocliente character varying(120),
    dircliente character varying(100),
    poblcliente character varying(40),
    cpcliente character varying(10),
    telcliente character varying(20),
    teltrabcliente character varying(20),
    movilcliente character varying(20),
    faxcliente character varying(20),
    mailcliente character varying(100),
    urlcliente character varying(150),
    corpcliente character varying(200),
    faltacliente date DEFAULT NOW(),
    fbajacliente date,
    regimenfiscalcliente character varying(50) DEFAULT 'Normal' NOT NULL,
    comentcliente character varying(2000),
    ecommercedatacliente character varying(2000),
    inactivocliente character(1),
    idtarifa integer references tarifa(idtarifa),
    recargoeqcliente boolean DEFAULT FALSE,
    idforma_pago integer REFERENCES forma_pago(idforma_pago),
    departamento character varying(32),
    idprovincia integer REFERENCES provincia(idprovincia)
);


-- ** cobro **
-- idcliente: Identidad del cliente.
-- fechacobro: Fecha del cobro.
-- fechavenccobro: ???Fecha vencimiento cobro.
-- cantcobro: Cantidad cobrada.
-- refcobro: Codigo de referencia del cobro.
-- previsioncobro:
-- comentcobro: Comentario.
-- idtrabajador: ???Trabajador que realiza el cobro.
-- idbanco: ???Entidad bancaria que realiza el cobro. (OBSOLETO, DEBE DESAPARECER)
\echo -n ':: Cobro ... '
CREATE TABLE cobro (
    idcobro serial PRIMARY KEY,
    idcliente integer NOT NULL REFERENCES cliente(idcliente),
    fechacobro date DEFAULT NOW(),
    fechavenccobro date DEFAULT NOW(),
    cantcobro numeric(12, 2) DEFAULT 0,
    refcobro character varying(12) NOT NULL,
    previsioncobro boolean DEFAULT FALSE,
    comentcobro character varying(500),
    idtrabajador integer REFERENCES trabajador(idtrabajador),
    idbanco integer REFERENCES banco(idbanco),
    idforma_pago integer REFERENCES forma_pago(idforma_pago)
);
   
\echo -n ':: Funcion que crea restricciones en cobro ... '
CREATE FUNCTION restriccionescobro() RETURNS "trigger"
AS '
DECLARE
    rs RECORD;

BEGIN
    IF NEW.fechacobro IS NULL THEN
	NEW.fechacobro := now();
    END IF;

    IF NEW.refcobro IS NULL OR NEW.refcobro = '''' THEN
	SELECT INTO rs crearef() AS m;

	IF FOUND THEN
	    NEW.refcobro := rs.m;
	END IF;
    END IF;

    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador de restricciones en cobro ... '
CREATE TRIGGER restriccionescobrotrigger
    BEFORE INSERT OR UPDATE ON cobro
    FOR EACH ROW
    EXECUTE PROCEDURE restriccionescobro();

-- ** pago **
-- idproveedor: Proveedor al que se realiza el pago.
-- fechapago: Fecha del pago.
-- cantpago: Cantidad del pago.
-- refpago: Codigo de referencia del pago.
-- previsiopago:
-- comentpago: Comentario.
-- idtrabajador: Trabajador que realiza el pago.
-- idbanco: Entidad bancaria que realiza el pago.
\echo -n ':: Pago ... '
CREATE TABLE pago (
    idpago serial PRIMARY KEY,
    idproveedor integer NOT NULL REFERENCES proveedor(idproveedor),
    fechapago date DEFAULT NOW(),
    cantpago numeric(12, 2) DEFAULT 0,
    refpago character varying(12) NOT NULL,
    previsionpago boolean DEFAULT FALSE,
    comentpago character varying(500),
    idtrabajador integer REFERENCES trabajador(idtrabajador),
    idbanco integer REFERENCES banco(idbanco)
);

\echo -n ':: Funcion que crea restricciones en pago ... '
CREATE FUNCTION restriccionespago() RETURNS "trigger"
AS '
DECLARE
    rs RECORD;

BEGIN
    IF NEW.fechapago IS NULL THEN
	NEW.fechapago := now();
    END IF;

    IF NEW.refpago IS NULL OR NEW.refpago = '''' THEN
	SELECT INTO rs crearef() AS m;

	IF FOUND THEN
	    NEW.refpago := rs.m;
	END IF;
    END IF;

    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador de restricciones en pago ... '
CREATE TRIGGER restriccionespagotrigger
    BEFORE INSERT OR UPDATE ON pago
    FOR EACH ROW
    EXECUTE PROCEDURE restriccionespago();


-- ** pedido **
-- numpedido: Numero de pedido.
-- fechapedido: Fecha de salida del pedido.
-- descpedido: Descripcion del pedido.
-- iddivision: Division que realiza el pedido.
-- idalmacen: Almacen que realiza el pedido.
-- idtrabajador: Trabajador que realiza el pedido.
\echo -n ':: Pedido ... '
CREATE TABLE pedido (
    idpedido serial PRIMARY KEY,
    numpedido character varying(60),
    fechapedido date,
    descpedido character varying(500),
    iddivision integer NOT NULL REFERENCES division(iddivision),
    idalmacen integer NOT NULL REFERENCES almacen(idalmacen),
    idtrabajador integer REFERENCES trabajador(idtrabajador)   
);


-- ** usuario **
-- loginusuario: Identidad del usuario.
-- nombreusuario: Nombre real del usuario.
-- apellido1usuario: Primer apellido del usuario.
-- apellido2usuario: Segundo apellido del usuario.
-- claveusuario: Clave de acceso del usuario.
-- permisousuario: Permisos del usuario.
\echo -n ':: Usuario ... '
CREATE TABLE usuario (
    loginusuario character varying(15) PRIMARY KEY,
    nombreusuario character varying(35),
    apellido1usuario character varying(35),
    apellido2usuario character varying(35),
    claveusuario character varying(35),
    permisosusuario text
);


-- ** fra_pro **
-- numfra_pro: Numero de factura del proveedor.
-- fcrefra_pro: Fecha de la factura el proveedor.
-- comentafra_pro: Comentario a la factura.
\echo -n ':: Factura de proveedor ... '
CREATE TABLE fra_pro (
    idfra_pro serial PRIMARY KEY,
    numfra_pro character varying(60),
    fcrefra_pro date,
    comentfra_pro character varying(2000)
);


-- ** alb_pro **
-- ncompraalb_pro: Numero de compra. Se usa cuando no se ha emitido albaran.
-- nalbalb_pro: Numero del albaran.
-- fcrealb_pro: Fecha de creacion del albaran.
-- frecepalb_pro: Fecha de recepcion del albaran.
-- comentalb_pro: Comentario.
-- idfra_pro: Factura de proveedor al que pertenece el albaran.
-- idalmacen: Almacen que recepciona la mercancia del proveedor.
\echo -n ':: Albaran de proveedor ... '
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


-- TODO: ESTA TABLA ESTA MARCADA PARA SER BORRADA.....................................
-- ** lpedido **
-- numlpedido: Numero de linea de pedido.
-- desclpedido: Descripcion de la linea de pedido.
-- cantlpedido: Cantidad.
-- pvdlpedido: Precio al que nos vende el proveedor.
-- prevlpedido: Prevision de recepccion de la mercancia.
-- ivalpedido: IVA que se aplica al articulo pedido.
-- descuentolpedido: Descuento que se aplica a esa linea de pedido.
\echo -n ':: Lineas de pedido ... '
CREATE TABLE lpedido (
    numlpedido serial PRIMARY KEY,
    desclpedido character varying(150),
    cantlpedido numeric(12, 2),
    pvdlpedido numeric(12, 2),
    prevlpedido date,
    ivalpedido numeric(12, 2),
    receqlpedido NUMERIC(12,2) DEFAULT 0,
    descuentolpedido numeric(12, 2),
    idpedido integer NOT NULL REFERENCES pedido(idpedido),
    idalb_pro integer REFERENCES alb_pro(idalb_pro),
    idarticulo integer REFERENCES articulo(idarticulo)
);


-- ** presupuesto **
-- Entendemos que un presupuesto es una relacion de materiales y/o trabajos cuantificada que
-- hacemos a peticion de un cliente determinado.
-- numpresupuesto: Numero de presupuesto.
-- refpresupuesto: Codigo de referencia que se asigna a ese presupuesto.
-- fpresupuesto: Fecha de realizacion del presupuesto.
-- descpresupuesto: Descripcion del presupuesto.
-- contactpresupuesto: Persona de contacto a la que hacer llegar el presupuesto. Se usa si es
--                     diferente a los datos de cliente.
-- telpresupuesto: Telefono de la persona de contacto.
-- vencpresupuesto: Fecha limite de validez del presupuesto.
-- comentpresupuesto: Comentario al presupuesto.
-- idusuari: ???Usuario que realiza el presupuesto.
-- procesadopresupuesto: Indica si al presupuesto se le ha dado curso o esta en modo borrador.
-- idcliente: Cliente que solicita el presupuesto.
-- idalmacen: Almacen que realiza el presupuesto.
-- idforma_pago: Forma de pago para el presupuesto.
-- idtrabajador: ???Trabajador que realiza el presupuesto.
-- bimppresupuesto: Total de Base Imponible del presupuesto.
-- imppresupuesto: Total de impuestos (IVA) del presupuesto.
-- totalpresupuesto: Total presupuesto.
\echo -n ':: Presupuesto ... '
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
    bimppresupuesto numeric(12, 2) DEFAULT 0,
    imppresupuesto  numeric(12, 2) DEFAULT 0,
    totalpresupuesto numeric(12, 2) DEFAULT 0,
    UNIQUE (idalmacen, numpresupuesto)
);


\echo -n ':: Funcion que crea restricciones en presupuesto ... '
CREATE FUNCTION restriccionespresupuesto() RETURNS "trigger"
AS '
DECLARE
    rs RECORD;

BEGIN
    IF NEW.fpresupuesto IS NULL THEN
	NEW.fpresupuesto := now();
    END IF;

    IF NEW.numpresupuesto IS NULL THEN
	SELECT INTO rs max(numpresupuesto) AS m FROM presupuesto;

	IF rs.m IS NOT NULL THEN
	    NEW.numpresupuesto := rs.m + 1;
	ELSE
	    NEW.numpresupuesto := 1;
	END IF;
    END IF;

    IF NEW.refpresupuesto IS NULL OR NEW.refpresupuesto = '''' THEN
	SELECT INTO rs crearef() AS m;

	IF FOUND THEN
	    NEW.refpresupuesto := rs.m;
	END IF;
    END IF;

    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador de restricciones en presupuesto ... '
CREATE TRIGGER restriccionespresupuestotrigger
    BEFORE INSERT OR UPDATE ON presupuesto
    FOR EACH ROW
    EXECUTE PROCEDURE restriccionespresupuesto();


-- ** dpresupuesto **
-- Descuento en presupuesto a clientes.
-- conceptdpresupuesto: Motivo del descuento en presupuesto.
-- proporciondpresupuesto: Porcentaje del descuento.
-- idpresupuesto: Presupuesto sobre el que se realiza el descuento.
\echo -n ':: Descuento de presupuesto ... '
CREATE TABLE dpresupuesto (
    iddpresupuesto serial PRIMARY KEY,
    conceptdpresupuesto character varying(2000),
    proporciondpresupuesto numeric(12, 2),
    idpresupuesto integer REFERENCES presupuesto(idpresupuesto)
);


-- ** lpresupuesto **
-- Linea de presupuesto.
-- desclpresupuesto: Descipcion del articulo.
-- cantlpresupuesto: Cantidad.
-- pvplpresupuesto: Precio de venta del articulo.
-- ivalpresupuesto: IVA aplicado al articulo.
-- reqeqlpresupuesto: Recargo equivalencia a aplicar.
-- descuentolpresupuesto: Porcentaje de descuento aplicado al precio de venta.
-- ordenlpresupuesto: Indica el orden en que aparecen las lineas listadas.
-- idpresupuesto: Presupuesto al que pertenece esta linea.
-- idarticulo: Articulo de la linea del presupuesto.
\echo -n ':: Lineas de presupuesto ... '
CREATE TABLE lpresupuesto (
    idlpresupuesto serial PRIMARY KEY,
    desclpresupuesto character varying,
    cantlpresupuesto numeric(12, 2),
    pvplpresupuesto numeric(12, 2),
    ivalpresupuesto numeric(12, 2),
    reqeqlpresupuesto numeric(12,2) DEFAULT 0,
    descuentolpresupuesto numeric(12, 2),
    ordenlpresupuesto integer,
    idpresupuesto integer NOT NULL REFERENCES presupuesto(idpresupuesto),
    idarticulo integer REFERENCES articulo(idarticulo)
);
-- Falta poner por defecto el PVP y el IVA.


\echo -n ':: Funcion que actualiza el total del presupuesto ... '
CREATE OR REPLACE FUNCTION actualizatotpres() returns TRIGGER
AS '
DECLARE
    tot NUMERIC(12, 2);
    bimp NUMERIC(12, 2);
    imp NUMERIC(12, 2);

BEGIN
    tot := calctotalpres(NEW.idpresupuesto);
    bimp := calcbimppres(NEW.idpresupuesto);
    imp := calcimpuestospres(NEW.idpresupuesto);
    UPDATE presupuesto SET totalpresupuesto = tot, bimppresupuesto = bimp, imppresupuesto = imp WHERE idpresupuesto = NEW.idpresupuesto;
    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador al insertar o actualizar lineas en un pedido de cliente ... '
CREATE TRIGGER restriccionespedidoclientetrigger
    AFTER INSERT OR UPDATE ON lpresupuesto
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotpres();


\echo -n ':: Disparador al insertar o actualizar descuentos en un pedido de cliente ... '
CREATE TRIGGER restriccionespedidoclientetrigger1
    AFTER INSERT OR UPDATE ON dpresupuesto
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotpres();


\echo -n ':: Funcion que actualiza el total del presupuesto al borrar una linea ... '
CREATE OR REPLACE FUNCTION actualizatotpresb() returns TRIGGER
AS '
DECLARE
    tot NUMERIC(12, 2);
    bimp NUMERIC(12, 2);
    imp NUMERIC(12, 2);

BEGIN
    tot := calctotalpres(OLD.idpresupuesto);
    bimp := calcbimppres(OLD.idpresupuesto);
    imp := calcimpuestospres(OLD.idpresupuesto);
    UPDATE presupuesto SET totalpresupuesto = tot, bimppresupuesto = bimp, imppresupuesto = imp WHERE idpresupuesto = OLD.idpresupuesto;
    RETURN OLD;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador al borrar o actualizar lineas de un presupuesto a cliente ... '
CREATE TRIGGER restriccionespedidoclientetriggerd
    AFTER DELETE OR UPDATE ON lpresupuesto
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotpresb();


\echo -n ':: Disparador al borrar o actualizar dewcuentos de un presupuesto a cliente ... '
CREATE TRIGGER restriccionespedidoclientetriggerd1
    AFTER DELETE OR UPDATE ON dpresupuesto
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotpresb();


-- ** pedidocliente **
-- numpedidocliente: Numero de pedido de cliente.
-- fechapedidocliente: Fecha del pedido.
-- refpedidocliente: Codigo referencia del pedido.
-- descpedidocliente: Descripccion del pedido.
-- comentpedidocliente: Comentario.
-- contactpedidocliente: Persona de contacto del pedido. Solo en caso de que sea diferente
--                       que los datos del cliente asociado.
-- telpedidoclient: Telefono de la persona de contacto.
-- idusuari: ???Ususario que realiza el pedido.
-- idpresupuesto: Presupuesto al que corresponde este pedido.
-- procesadopedidocliente: Indica si se ha dado curso al pedido o es un borrador.
-- idcliente: Cliente que realiza el pedido.
-- idforma_pago: Forma de pago para este pedido.
-- idalmacen: Almacen que prepara el pedido.
-- idtrabajador: ???Trabajador que realiza el pedido.
-- totalpedidocliente: Total del pedido.
-- bimppedidocliente: Base imponible total del pedido.
-- imppedidocliente: Total de impuestos (IVA) del pedido.
\echo -n ':: Pedido de cliente ... '
CREATE TABLE pedidocliente (
    idpedidocliente serial PRIMARY KEY,
    numpedidocliente integer UNIQUE NOT NULL,
    fechapedidocliente date DEFAULT now(),
    refpedidocliente character varying(12) NOT NULL,   
    descpedidocliente character varying(150),
    comentpedidocliente character varying(3000),
    contactpedidocliente character varying(90),
    telpedidocliente character varying(20),
    idusuari integer,
    idpresupuesto integer REFERENCES presupuesto(idpresupuesto),
    procesadopedidocliente boolean DEFAULT FALSE,   
    idcliente integer NOT NULL REFERENCES cliente(idcliente),
    idforma_pago integer REFERENCES forma_pago(idforma_pago),
    idalmacen integer NOT NULL REFERENCES almacen(idalmacen),
    idtrabajador integer REFERENCES trabajador(idtrabajador),
    totalpedidocliente numeric(12,2) DEFAULT 0,
    bimppedidocliente NUMERIC(12,2) DEFAULT 0,
    imppedidocliente NUMERIC(12,2) DEFAULT 0
);


\echo -n ':: Funcion con restricciones de un pedido de cliente ... '
CREATE FUNCTION restriccionespedidocliente() RETURNS "trigger"
AS '
DECLARE
    rs RECORD;

BEGIN
    IF NEW.fechapedidocliente IS NULL THEN
	NEW.fechapedidocliente := now();
    END IF;

    IF NEW.numpedidocliente IS NULL THEN
	SELECT INTO rs max(numpedidocliente) AS m FROM pedidocliente;

	IF rs.m IS NOT NULL THEN
	    NEW.numpedidocliente := rs.m + 1;
	ELSE
	    NEW.numpedidocliente := 1;
	END IF;
    END IF;

    IF NEW.refpedidocliente IS NULL OR NEW.refpedidocliente = '''' THEN
	SELECT INTO rs crearef() AS m;

	IF FOUND THEN
	    NEW.refpedidocliente := rs.m;
	END IF;
    END IF;

    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador de restricciones de un pedido de cliente ... '
CREATE TRIGGER restriccionespedidoclientetrigger
    BEFORE INSERT OR UPDATE ON pedidocliente
    FOR EACH ROW
    EXECUTE PROCEDURE restriccionespedidocliente();


-- ** dpedidocliente **
-- Descuento de pedidocliente.
-- conceptdpedidocliente: Descripcion del motivo de descuento.
-- proporciondpedidocliente: Porcentaje del descuento.
-- idpedidocliente: Pedido de cliente sobre el que se realiza el descuento.
\echo -n ':: Descuento de pedido de cliente ... '
CREATE TABLE dpedidocliente (
    iddpedidocliente serial PRIMARY KEY,
    conceptdpedidocliente character varying(2000),
    proporciondpedidocliente numeric(12, 2),
    idpedidocliente integer NOT NULL REFERENCES pedidocliente(idpedidocliente)
    -- Falta poner el lugar donde se aplica el descuento, antes de la factura o despues de esta.
);    


-- ** lpedidocliente **
-- Linea de detalle del pedido de cliente.
-- numlpedidocliente: Numero de linea del pedido de cliente.
-- desclpedidocliente: Descripcion del articulo.
-- cantlpedidocliente: Cantidad.
-- pvplpedidocliente: Precio de venta del articulo.
-- prevlpedidocliente: Fecha prevista de recepcion de la mercancia.
-- ivalpedidocliente: Total IVA aplicado al articulo.
-- reqeqlpedidocliente: Recargo de equivalencia para la linea de pedido.
-- descuentolpedidocliente: Porcentaje de descuento aplicado a la linea de articulo.
-- idpedidocliente: Pedido de cliente al que corresponde esta linea.
-- ordenlpedidocliente: ???
-- idarticulo: Articulo de la linea de pedido de cliente. 
\echo -n ':: Lineas de pedido de cliente ... '
CREATE TABLE lpedidocliente (
    numlpedidocliente serial PRIMARY KEY,
    desclpedidocliente character varying,
    cantlpedidocliente numeric(12, 2),
    pvplpedidocliente numeric(12, 2),
    prevlpedidocliente date,
    ivalpedidocliente numeric(12, 2),
    reqeqlpedidocliente NUMERIC(12,2) DEFAULT 0,
    descuentolpedidocliente numeric(12, 2),
    idpedidocliente integer NOT NULL REFERENCES pedidocliente(idpedidocliente),
    ordenlpedidocliente integer,
    puntlpedidocliente boolean DEFAULT FALSE,
    idarticulo integer REFERENCES articulo(idarticulo)
);


\echo -n ':: Funcion para actualizar el total de pedido de cliente ... '
CREATE OR REPLACE FUNCTION actualizatotpedcli() returns TRIGGER
AS '
DECLARE
    tot NUMERIC(12, 2);
    bimp NUMERIC(12, 2);
    imp NUMERIC(12, 2);

BEGIN
    tot := calctotalpedcli(NEW.idpedidocliente);
    bimp := calcbimppedcli(NEW.idpedidocliente);
    imp := calcimpuestospedcli(NEW.idpedidocliente);
    UPDATE pedidocliente SET totalpedidocliente = tot, bimppedidocliente = bimp, imppedidocliente = imp WHERE idpedidocliente = NEW.idpedidocliente;
    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador que actualiza el total de un pedido de cliente al insertar una linea ... '
CREATE TRIGGER totalesautomaticospedidoclientetrigger
    AFTER INSERT OR UPDATE ON lpedidocliente
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotpedcli();


\echo -n ':: Disparador que actualiza el total de un pedido de cliente al insertar un descuento... '
CREATE TRIGGER totalesautomaticospedidoclientetrigger1
    AFTER INSERT OR UPDATE ON dpedidocliente
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotpedcli();


\echo -n ':: Funcion que actualiza el total del pedido de cliente al borrar una linea ... '
CREATE OR REPLACE FUNCTION actualizatotpedclib() returns TRIGGER
AS '
DECLARE
    tot NUMERIC(12, 2);
    bimp NUMERIC(12, 2);
    imp NUMERIC(12, 2);

BEGIN
    tot := calctotalpedcli(OLD.idpedidocliente);
    bimp := calcbimppedcli(OLD.idpedidocliente);
    imp := calcimpuestospedcli(OLD.idpedidocliente);
    UPDATE pedidocliente SET totalpedidocliente = tot, bimppedidocliente = bimp, imppedidocliente = imp WHERE idpedidocliente = OLD.idpedidocliente;
    RETURN OLD;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador que actualiza el total de un pedido de cliente al borrar una linea ... '
CREATE TRIGGER totalesautomaticospedidoclientetriggerd
    AFTER DELETE OR UPDATE ON lpedidocliente
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotpedclib();


\echo -n ':: Disparador que actualiza el total de un pedido de cliente al borrar un descuento ... '
CREATE TRIGGER totalesautomaticospedidoclientetriggerd1
    AFTER DELETE OR UPDATE ON dpedidocliente
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotpedclib();


-- ** factura **
-- factura a cliente.
-- codigoserie_factura: Codgio de serie de la factura de cliente.
-- numfactura: Numero factura cliente.
-- reffactura: Referencia factura cliente.
-- ffactura: Fecha de la factura de cliente.
-- descfactura: Descripcion factura cliente.
-- idalmacen: Almacen que emite la factura de cliente.
-- contactfactura: Contacto factura cliente.
-- telfactura: Telefono factura cliente.
-- comentfactura: Comentario factura cliente.
-- procesadafactura:
-- idusuari: ???Usuario que emite la factura de cliente.
-- idcliente: Cliente al que se emite la factura de cliente.
-- idforma_pago: Forma de pago factura cliente.
-- UNIQUE: Almacen que emite la factura, numero de serie de la factura y numero de factura de cliente.
-- idtrabajador: ???Trabajador que realiza la factura.
-- totalfactura: Total factura cliente.
-- bimpfactura: Base imponible factura cliente.
-- impfactura: Total impuestos factura cliente.
\echo -n ':: Factura ... '
CREATE TABLE factura (
    idfactura serial PRIMARY KEY,
    codigoserie_factura character varying(6) NOT NULL REFERENCES serie_factura(codigoserie_factura),
    numfactura integer NOT NULL,
    reffactura character varying(15) NOT NULL,
    ffactura date DEFAULT now(),
    descfactura character varying(150),
    idalmacen integer NOT NULL REFERENCES almacen(idalmacen),
    contactfactura character varying(90),
    telfactura character varying(20),
    comentfactura character varying(3000),
    procesadafactura boolean DEFAULT FALSE, 
    idusuari integer,
    idcliente integer REFERENCES cliente(idcliente),
    idforma_pago integer REFERENCES forma_pago(idforma_pago),   
    UNIQUE (idalmacen, codigoserie_factura, numfactura),
    idtrabajador integer REFERENCES trabajador(idtrabajador),
    totalfactura NUMERIC(12, 2) DEFAULT 0,
    bimpfactura NUMERIC(12, 2) DEFAULT 0,
    impfactura NUMERIC(12, 2) DEFAULT 0
);


\echo -n ':: Funcion con restricciones en factura ... '
CREATE FUNCTION restriccionesfactura() RETURNS "trigger"
AS '
DECLARE
    rs RECORD;

BEGIN
    IF NEW.ffactura IS NULL THEN
	NEW.ffactura := now();
    END IF;

    IF NEW.numfactura IS NULL THEN
	SELECT INTO rs max(numfactura) AS m FROM factura WHERE codigoserie_factura = NEW.codigoserie_factura AND idalmacen = NEW.idalmacen;
	IF rs.m IS NOT NULL THEN
	    NEW.numfactura := rs.m + 1;
	ELSE
	    NEW.numfactura := 1;
	END IF;
    END IF;

    IF NEW.reffactura IS NULL OR NEW.reffactura = '''' THEN
	SELECT INTO rs crearef() as m;

	IF FOUND THEN
	    NEW.reffactura := rs.m;
	END IF;
    END IF;

    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador antes de insertar o actualizar una factura ... '
CREATE TRIGGER restriccionesfacturatrigger
    BEFORE INSERT OR UPDATE ON factura
    FOR EACH ROW
    EXECUTE PROCEDURE restriccionesfactura();

    
-- ** dfactura **
-- conceptdfactura: Motivo del descuento.
-- proporciondfactura: Porcentaje de descuento aplicado al total de la factura.
\echo -n ':: Descuento de factura ... '
CREATE TABLE dfactura (
    iddfactura serial PRIMARY KEY,
    conceptdfactura character varying(2000),
    proporciondfactura numeric(12, 2),
    idfactura integer NOT NULL REFERENCES factura(idfactura)
    -- Falta poner el lugar donde se aplica el descuento, antes de la factura o despues de esta.
); 


-- ** lfactura **
-- Linea de detalle de la factura.
-- desclfactura: Descripcion de la linea de factura.
-- cantlfactura: Cantidad de articulos.
-- pvplfactura: Precio de venta unitario del articulo.
-- ivalfactura: IVA aplicado al articulo.
-- reqeqlfactura: Recargo equivalencia factura.
-- descuentolfactura: Porcentaja de descuento aplicado al articulo.
-- ordenlfactura: Numero orden linea factura.
-- idfactura: Factura a la que pretenece esta linea de factura.
-- idarticulo: Articulo linea factura.
\echo -n ':: Lineas de factura ... '
CREATE TABLE lfactura (
    idlfactura serial PRIMARY KEY,
    desclfactura character varying,
    cantlfactura numeric(12, 2),
    pvplfactura numeric(12, 2),
    ivalfactura numeric(12, 2),
    reqeqlfactura NUMERIC(12,2) DEFAULT 0,
    descuentolfactura numeric(12, 2),
    ordenlfactura integer,
    idfactura integer NOT NULL REFERENCES factura(idfactura),
    idarticulo integer REFERENCES articulo(idarticulo)
);


\echo -n ':: Funcion con restricciones en lineas de factura ... '
CREATE FUNCTION restriccioneslfactura() RETURNS "trigger"
AS '
DECLARE
    rs RECORD;
    reg RECORD;

BEGIN
    IF NEW.idarticulo IS NULL THEN
	RAISE EXCEPTION ''ARTICULO INVALIDO'';
	return OLD;
    END IF;

    FOR rs IN SELECT * FROM articulo WHERE idarticulo = NEW.idarticulo LOOP
	IF NEW.desclfactura IS NULL THEN
	    NEW.desclfactura := rs.nomarticulo;
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
' LANGUAGE plpgsql;


\echo -n ':: Disparador al insertar o actualizar lineas de una factura ... '
CREATE TRIGGER restriccionesalfacturatrigger
    BEFORE INSERT OR UPDATE ON lfactura
    FOR EACH ROW
    EXECUTE PROCEDURE restriccioneslfactura();


\echo -n ':: Funcion que calcula el total de una factura ... '
CREATE OR REPLACE FUNCTION calctotalfactura(integer) RETURNS numeric(12, 2)
AS '
DECLARE
    idp ALIAS FOR $1;
    totalBImponibleLineas numeric(12, 2);
    totalIRPF numeric(12, 2);
    totalIVA numeric(12, 2);
    totalRE numeric(12, 2);
    totalTotal numeric(12, 2);
    rs RECORD;

BEGIN
    totalBImponibleLineas := 0;
    totalIRPF := 0;
    totalIVA := 0;
    totalRE := 0;
    totalTotal := 0;

    FOR rs IN SELECT cantlfactura * pvplfactura * (1 - descuentolfactura / 100) AS subtotal1 FROM lfactura WHERE idfactura = idp LOOP
	totalBImponibleLineas := totalBImponibleLineas + rs.subtotal1;
    END LOOP;

    SELECT INTO rs valor::numeric FROM configuracion WHERE LOWER(nombre) = ''irpf'';

    IF FOUND THEN
        totalIRPF := totalBImponibleLineas * (rs.valor / 100);
    END IF;

    FOR rs IN SELECT cantlfactura * pvplfactura * (1 - descuentolfactura / 100) * (ivalfactura / 100) AS subtotal1 FROM lfactura WHERE idfactura = idp LOOP
	totalIVA := totalIVA + rs.subtotal1;
    END LOOP;

    FOR rs IN SELECT cantlfactura * pvplfactura * (1 - descuentolfactura / 100) * (reqeqlfactura / 100) AS subtotal1 FROM lfactura WHERE idfactura = idp LOOP
	totalRE := totalRE + rs.subtotal1;
    END LOOP;

    FOR rs IN SELECT proporciondfactura FROM dfactura WHERE idfactura = idp LOOP
	totalBImponibleLineas := totalBImponibleLineas * (1 - rs.proporciondfactura / 100);
	totalIRPF := totalIRPF * (1 - rs.proporciondfactura / 100);
	totalIVA := totalIVA * (1 - rs.proporciondfactura / 100);
	totalRE := totalRE * (1 - rs.proporciondfactura / 100);
    END LOOP;

    totalTotal = totalBImponibleLineas - totalIRPF + totalIVA + totalRE;

    RETURN totalTotal;
END;
' LANGUAGE plpgsql;


\echo -n ':: Funcion que calcula la base imponible total de una factura ... '
CREATE OR REPLACE FUNCTION calcbimpfactura(integer) RETURNS numeric(12, 2)
AS '
DECLARE
    idp ALIAS FOR $1;
    total numeric(12, 2);
    rs RECORD;

BEGIN
    total := 0;

    FOR rs IN SELECT cantlfactura * pvplfactura * (1 - descuentolfactura / 100) AS subtotal1 FROM lfactura WHERE idfactura = idp LOOP
	total := total + rs.subtotal1;
    END LOOP;

    FOR rs IN SELECT proporciondfactura FROM dfactura WHERE idfactura = idp LOOP
	total := total * (1 - rs.proporciondfactura / 100);
    END LOOP;

    RETURN total;
END;
' LANGUAGE plpgsql;


\echo -n ':: Funcion que calcula los impuestos totales de una factura ... '
CREATE OR REPLACE FUNCTION calcimpuestosfactura(integer) RETURNS numeric(12, 2)
AS '
DECLARE
    idp ALIAS FOR $1;
    total numeric(12, 2);
    rs RECORD;

BEGIN
    total := 0;

    FOR rs IN SELECT cantlfactura * pvplfactura * (1 - descuentolfactura / 100) * ((ivalfactura+reqeqlfactura) / 100)  AS subtotal1 FROM lfactura WHERE idfactura = idp LOOP
	total := total + rs.subtotal1;
    END LOOP;

    FOR rs IN SELECT proporciondfactura FROM dfactura WHERE idfactura = idp LOOP
	total := total * (1 - rs.proporciondfactura / 100);
    END LOOP;

    RETURN total;
END;
' LANGUAGE plpgsql;


\echo -n ':: Funcion que actualiza el total de una factura ... '
CREATE OR REPLACE FUNCTION actualizatotfactura() returns TRIGGER
AS '
DECLARE
    tot NUMERIC(12, 2);
    bimp NUMERIC(12, 2);
    imp NUMERIC(12, 2);

BEGIN
    tot := calctotalfactura(NEW.idfactura);
    bimp := calcbimpfactura(NEW.idfactura);
    imp := calcimpuestosfactura(NEW.idfactura);

    UPDATE factura SET totalfactura = tot, bimpfactura = bimp, impfactura = imp WHERE idfactura = NEW.idfactura;

    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador que actualizar el total de una linea de factura al insertar una linea ... '
CREATE TRIGGER totalesautomaticosfacturatrigger
    AFTER INSERT OR UPDATE ON lfactura
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotfactura();


\echo -n ':: Disparador que actualizar el total de una linea de factura al insertar un descuento ... '
CREATE TRIGGER totalesautomaticosfacturatrigger1
    AFTER INSERT OR UPDATE ON dfactura
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotfactura();


\echo -n ':: Funcion que actualiza el total de una factura al borrar una linea de detalle ... '
CREATE OR REPLACE FUNCTION actualizatotfacturab() returns TRIGGER
AS '
DECLARE
    tot NUMERIC(12, 2);
    bimp NUMERIC(12, 2);
    imp NUMERIC(12, 2);

BEGIN
    tot := calctotalfactura(OLD.idfactura);
    bimp := calcbimpfactura(OLD.idfactura);
    imp := calcimpuestosfactura(OLD.idfactura);
    UPDATE factura SET totalfactura = tot, bimpfactura = bimp, impfactura = imp WHERE idfactura = OLD.idfactura;

    RETURN OLD;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador que actualiza el total de una factura al borrar o actualizar una linea de detalle ... '
CREATE TRIGGER totalesautomaticosfacturatriggerd
    AFTER DELETE OR UPDATE ON lfactura
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotfacturab();


\echo -n ':: Disparador que actualiza el total de una factura al borrar o actualizar un descuento ... '
CREATE TRIGGER totalesautomaticosfacturatriggerd1
    AFTER DELETE OR UPDATE ON dfactura
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotfacturab();


-- ** facturap **
-- Factura de proveedor.
-- numfacturap: Numero de factura de proveedor.
-- reffacturap: Codigo referencia de la factura.
-- ffacturap: Fecha de emision de la factura de proveedor.
-- descfacturap: Descripcion de la factura de proveedor.
-- contactfacturap: Persona de contacto. La persona que ha emitido la factura.
-- telfacturap: Numero de telefono de la persona de contacto.
-- comentfacturap: Comentario.
-- procesadafacturap: Indica que la factura de proveedor esta procesada.
-- idusuari: ???Usuario que emite la factura.
-- idproveedor: Proveedor que emite la factura.
-- idforma_pago: Forma de pago de la factura de proveedor.
-- idtrabajador: ???Trabajador que emite la factura.
-- totalfacturap: Total de factura.
-- bimpfacturap: Base Imponible de la factura.
-- impfacturap: Total de impuestos (IVA).
\echo -n ':: Factura de proveedor ... '
CREATE TABLE facturap (
    idfacturap serial PRIMARY KEY,
    numfacturap character varying (20) NOT NULL UNIQUE,
    reffacturap character varying(15) NOT NULL,
    ffacturap date DEFAULT now(),
    descfacturap character varying(150),
    contactfacturap character varying(90),
    telfacturap character varying(20),
    comentfacturap character varying(3000),
    procesadafacturap boolean DEFAULT FALSE, 
    idusuari integer,
    idproveedor integer REFERENCES proveedor(idproveedor),
    idforma_pago integer REFERENCES forma_pago(idforma_pago),
    idtrabajador integer REFERENCES trabajador(idtrabajador),
    totalfacturap NUMERIC(12, 2) DEFAULT 0,
    bimpfacturap NUMERIC(12, 2) DEFAULT 0,
    impfacturap NUMERIC(12, 2) DEFAULT 0
);


\echo -n ':: Funcion ... '
CREATE FUNCTION restriccionesfacturap() RETURNS "trigger"
AS '
DECLARE
    rs RECORD;

BEGIN
    IF NEW.ffacturap IS NULL THEN
	NEW.ffacturap := now();
    END IF;

    IF NEW.reffacturap IS NULL OR NEW.reffacturap = '''' THEN
	SELECT INTO rs crearef() AS m;

	IF FOUND THEN
	    NEW.reffacturap := rs.m;
	END IF;
    END IF;

    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ':: Creadas las restricciones para la tabla de facturas a proveedores ... '
CREATE TRIGGER restriccionesfacturaptrigger
    BEFORE INSERT OR UPDATE ON facturap
    FOR EACH ROW
    EXECUTE PROCEDURE restriccionesfacturap();


-- ** lfacturap **
-- Linea detalle de la factura.
-- desclfacturap: Descripcion linea factura proveedor.
-- cantlfacturap: Cantidad linea factura proveedor.
-- pvplfacturap: Precio linea factura proveedor.
-- ivalfacturap: IVA linea factura proveedor.
-- reqeqlfacturap: Recargo equivalencia linea facura proveedor.
-- descuentolfacturap: Descuento linea factura proveedor.
-- ordenlfacturap: Orden linea factura proveedor.
-- idfacturap: Factura proveedor de esta linea de factura de proveedor.
-- idarticulo: Articulo de la linea de proveedor.
\echo -n ':: Lineas de factura de proveedor ... '
CREATE TABLE lfacturap (
    idlfacturap serial PRIMARY KEY,
    desclfacturap character varying,
    cantlfacturap numeric(12, 2),
    pvplfacturap numeric(12, 2),
    ivalfacturap numeric(12, 2),
    reqeqlfacturap NUMERIC(12,2) DEFAULT 0,
    descuentolfacturap numeric(12, 2),
    ordenlfacturap integer,
    idfacturap integer NOT NULL REFERENCES facturap(idfacturap),
    idarticulo integer REFERENCES articulo(idarticulo)
);


-- Descuento de factura proveedor
-- conceptdfacturap: Descripcion del descuento de la factura de proveedor.
-- proporciondfacturap: Descuento de la factura de proveedor.
-- idfacturap: Factura de proveedor a la que corresponde este descuento.
\echo -n ':: Descuento de facturas de proveedor ... '
CREATE TABLE dfacturap (
    iddfacturap serial PRIMARY KEY,
    conceptdfacturap character varying(2000),
    proporciondfacturap numeric(12, 2),
    idfacturap integer NOT NULL REFERENCES facturap(idfacturap)
    -- Falta poner el lugar donde se aplica el descuento, antes de la factura o despues de esta.
); 


\echo -n ':: Funcion que calcula el total de una factura de proveedor ... '
CREATE OR REPLACE FUNCTION calctotalfacpro(integer) RETURNS numeric(12, 2)
AS '
DECLARE
    idp ALIAS FOR $1;
    totalBImponibleLineas numeric(12, 2);
    totalIRPF numeric(12, 2);
    totalIVA numeric(12, 2);
    totalRE numeric(12, 2);
    totalTotal numeric(12, 2);
    rs RECORD;
    rs2 RECORD;

BEGIN
    totalBImponibleLineas := 0;
    totalIRPF := 0;
    totalIVA := 0;
    totalRE := 0;
    totalTotal := 0;

    FOR rs IN SELECT cantlfacturap * pvplfacturap * (1 - descuentolfacturap / 100) AS subtotal1 FROM lfacturap WHERE idfacturap = idp LOOP
	totalBImponibleLineas := totalBImponibleLineas + rs.subtotal1;
    END LOOP;

    SELECT INTO rs2 idproveedor FROM facturap WHERE idfacturap = idp;

    SELECT INTO rs irpfproveedor FROM proveedor WHERE idproveedor = rs2.idproveedor;

    IF FOUND THEN
        totalIRPF := totalBImponibleLineas * (rs.irpfproveedor / 100);
    END IF;

    FOR rs IN SELECT cantlfacturap * pvplfacturap * (1 - descuentolfacturap / 100) * (ivalfacturap / 100) AS subtotal1 FROM lfacturap WHERE idfacturap = idp LOOP
	totalIVA := totalIVA + rs.subtotal1;
    END LOOP;

    FOR rs IN SELECT cantlfacturap * pvplfacturap * (1 - descuentolfacturap / 100) * (reqeqlfacturap / 100) AS subtotal1 FROM lfacturap WHERE idfacturap = idp LOOP
	totalRE := totalRE + rs.subtotal1;
    END LOOP;

    FOR rs IN SELECT proporciondfacturap FROM dfacturap WHERE idfacturap = idp LOOP
	totalBImponibleLineas := totalBImponibleLineas * (1 - rs.proporciondfacturap / 100);
	totalIRPF := totalIRPF * (1 - rs.proporciondfacturap / 100);
	totalIVA := totalIVA * (1 - rs.proporciondfacturap / 100);
	totalRE := totalRE * (1 - rs.proporciondfacturap / 100);
    END LOOP;

    totalTotal = totalBImponibleLineas - totalIRPF + totalIVA + totalRE;

    RETURN totalTotal;
END;
' LANGUAGE plpgsql;


\echo -n ':: Funcion que calcula la Base Imponible total de una factura de proveedor ... '
CREATE OR REPLACE FUNCTION calcbimpfacpro(integer) RETURNS numeric(12, 2)
AS '
DECLARE
    idp ALIAS FOR $1;
    total numeric(12, 2);
    rs RECORD;

BEGIN
    total := 0;

    FOR rs IN SELECT cantlfacturap * pvplfacturap * (1 - descuentolfacturap / 100) AS subtotal1 FROM lfacturap WHERE idfacturap = idp LOOP
	total := total + rs.subtotal1;
    END LOOP;

    FOR rs IN SELECT proporciondfacturap FROM dfacturap WHERE idfacturap = idp LOOP
	total := total * (1 - rs.proporciondfacturap / 100);
    END LOOP;

    RETURN total;
END;
' LANGUAGE plpgsql;


\echo -n ':: Funcion que calcula el total de impuestos de una factura de proveedor ... '
CREATE OR REPLACE FUNCTION calcimpuestosfacpro(integer) RETURNS numeric(12, 2)
AS '
DECLARE
    idp ALIAS FOR $1;
    total numeric(12, 2);
    rs RECORD;

BEGIN
    total := 0;

    FOR rs IN SELECT cantlfacturap * pvplfacturap * (1 - descuentolfacturap / 100) * (ivalfacturap / 100) AS subtotal1 FROM lfacturap WHERE idfacturap = idp LOOP
	total := total + rs.subtotal1;
    END LOOP;

    FOR rs IN SELECT proporciondfacturap FROM dfacturap WHERE idfacturap = idp LOOP
	total := total * (1 - rs.proporciondfacturap / 100);
    END LOOP;

    RETURN total;
END;
' LANGUAGE plpgsql;


\echo -n ':: Funcion que actualiza el total de una factura de proveedor ... '
CREATE OR REPLACE FUNCTION actualizatotfacturap() returns TRIGGER
AS '
DECLARE
    tot NUMERIC(12, 2);
    bimp NUMERIC(12, 2);
    imp NUMERIC(12, 2);

BEGIN
    tot := calctotalfacpro(NEW.idfacturap);
    bimp := calcbimpfacpro(NEW.idfacturap);
    imp := calcimpuestosfacpro(NEW.idfacturap);

    UPDATE facturap SET totalfacturap = tot, bimpfacturap = bimp, impfacturap = imp WHERE idfacturap = NEW.idfacturap;

    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador que actualiza el total de una factura de proveedor al insertar una linea de detalle ... '
CREATE TRIGGER totalesautomaticosfacturaptrigger
    AFTER INSERT OR UPDATE ON lfacturap
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotfacturap();


\echo -n ':: Disparador que actualiza el total de una factura de proveedor al insertar un descuento ... '
CREATE TRIGGER totalesautomaticosfacturaptrigger1
    AFTER INSERT OR UPDATE ON dfacturap
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotfacturap();


\echo -n ':: Funcion que actualiza el total de una factura de proveedor al borrar una linea de detalle ... '
CREATE OR REPLACE FUNCTION actualizatotfacturapb() returns TRIGGER
AS '
DECLARE
    tot NUMERIC(12, 2);
    bimp NUMERIC(12, 2);
    imp NUMERIC(12, 2);
    
BEGIN
    tot := calctotalfacpro(OLD.idfacturap);
    bimp := calcbimpfacpro(OLD.idfacturap);
    imp := calcimpuestosfacpro(OLD.idfacturap);

    UPDATE facturap SET totalfacturap = tot, bimpfacturap = bimp, impfacturap = imp WHERE idfacturap = OLD.idfacturap;

    RETURN OLD;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador que actualiza el total de una factura de proveedor al borrar una linea de detalle ... '
CREATE TRIGGER totalesautomaticosfacturaptriggerd
    AFTER DELETE OR UPDATE ON lfacturap
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotfacturapb();


\echo -n ':: Disparador que actualiza el total de una factura de proveedor al borrar un descuento ... '
CREATE TRIGGER totalesautomaticosfacturaptriggerd1
    AFTER DELETE OR UPDATE ON dfacturap
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotfacturapb();


-- ** albaranp **
-- Albaranes de proveedor.
-- numalbaranp: Numero albaran de proveedor.
-- descalbaranp: Descripcion albaran de proveedor.
-- refalbaranp: Referencia albaran de proveedor.
-- fechaalbaranp: Fecha albaran de proveedor.
-- comentalbaranp: Comentario albaran de proveedor.
-- procesadoalbaranp: Procesado albaran de proveedor.
-- idproveedor: Proveedor del albaran.
-- idforma_pago: Forma de pago del albaran de proveedor.
-- idusuari: ???Usuario que tramita el albaran de proveedor.
-- idalmacen: Almacen que recibe el albaran de proveedor.
-- idtrabajador: ???Trabajador que tramita el albaran de proveedor.
-- UNIQUE: 
-- totalalbaranp: Total albaran proveedor.
-- bimpalbaranp: Base imponible albaran proveedor.
-- impalbaranp: Total impuestos albaran proveedor.
-- COMPROBACIONES DE INTEGRIDAD: Todos los albaranes de una factura corresponden al mismo
--                               proveedor.
\echo -n ':: Albaran de proveedor ... '
CREATE TABLE albaranp (
   idalbaranp serial PRIMARY KEY,
   numalbaranp character varying(20) NOT NULL UNIQUE,
   descalbaranp character varying(150),
   refalbaranp character varying(12) NOT NULL,
   fechaalbaranp date DEFAULT now(),
   comentalbaranp character varying(3000),
   procesadoalbaranp boolean DEFAULT FALSE,
   idproveedor integer REFERENCES proveedor(idproveedor),
   idforma_pago integer REFERENCES forma_pago(idforma_pago),
   idusuari integer,
   idalmacen integer NOT NULL REFERENCES almacen(idalmacen),
   idtrabajador integer REFERENCES trabajador(idtrabajador),   
   UNIQUE (idalmacen, numalbaranp),
   totalalbaranp NUMERIC(12, 2) DEFAULT 0,
   bimpalbaranp NUMERIC(12, 2) DEFAULT 0,
   impalbaranp NUMERIC(12, 2)
);


\echo -n ':: Funcion con restricciones en los albaranes de proveedor ... '
CREATE FUNCTION restriccionesalbaranp() RETURNS "trigger"
AS '
DECLARE
    rs RECORD;

BEGIN
    IF NEW.fechaalbaranp IS NULL THEN
	NEW.fechaalbaranp := now();
    END IF;

    IF NEW.numalbaranp IS NULL THEN
	NEW.numalbaranp := NEW.idalbaranp;
    END IF;

    IF NEW.refalbaranp IS NULL OR NEW.refalbaranp = '''' THEN
	SELECT INTO rs crearef() AS m;

	IF FOUND THEN
	    NEW.refalbaranp := rs.m;
	END IF;
    END IF;

    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador de restricciones antes de insertar o actualizar un albaran de proveedor ... '
CREATE TRIGGER restriccionesalbaranptrigger
    BEFORE INSERT OR UPDATE ON albaranp
    FOR EACH ROW
    EXECUTE PROCEDURE restriccionesalbaranp();


-- ** lalbaranp **
-- desclalbaranp: Descripcion linea albaran proveedor.
-- cantlalbaranp: Cantidad.
-- ivalalbaranp: IVA aplicado al precio.
-- reqeqlalbaranp: Recargo equivalencia.
-- pvplalbaranp: Precio sin IVA.
-- descuentolbaranp: Porcentaje de descuento.
-- ordenlalbaranp: Orden linea albaran proveedor.
-- idalbaranp: Albaran al que corresponde la linea de albaran de proveedor.
-- idarticulo: Articulo de la linea de albaran de proveedor.
\echo -n ':: Lineas de albaran de proveedor ... '

CREATE TABLE lalbaranp (
    numlalbaranp serial PRIMARY KEY,
    desclalbaranp character varying,
    cantlalbaranp numeric(12, 2),
    ivalalbaranp numeric(12, 2),
    reqeqlalbaranp NUMERIC(12,2) DEFAULT 0,
    pvplalbaranp numeric(12, 2),
    descuentolalbaranp numeric(12, 2),
    ordenlalbaranp integer,
    idalbaranp integer NOT NULL REFERENCES albaranp(idalbaranp),
    idarticulo integer NOT NULL REFERENCES articulo(idarticulo)
);


-- ** dalbaranp **
-- Descuento en albaran de proveedor
-- conceptdalbaranp: Concepto por el que se realiza el descuento en el albaran.
-- proporciondalbaranp: Porcentaje de descuento.
-- idalbaranp: Albaran al que corresponde el descuento albaran de proveedor.
\echo -n ':: Descuento en albaran de proveedor ... '
CREATE TABLE dalbaranp (
    iddalbaranp serial PRIMARY KEY,
    conceptdalbaranp character varying(500),
    proporciondalbaranp numeric(12, 2),
    idalbaranp integer NOT NULL REFERENCES albaranp(idalbaranp)
);


\echo -n ':: Funcion que calcula el total del albaran de proveedor ... '
CREATE OR REPLACE FUNCTION calctotalalbpro(integer) RETURNS numeric(12, 2)
AS '
DECLARE
    idp ALIAS FOR $1;
    totalBImponibleLineas numeric(12, 2);
    totalIRPF numeric(12, 2);
    totalIVA numeric(12, 2);
    totalRE numeric(12, 2);
    totalTotal numeric(12, 2);
    rs RECORD;
    rs2 RECORD;

BEGIN
    totalBImponibleLineas := 0;
    totalIRPF := 0;
    totalIVA := 0;
    totalRE := 0;
    totalTotal := 0;

    FOR rs IN SELECT cantlalbaranp * pvplalbaranp * (1 - descuentolalbaranp / 100) AS subtotal1 FROM lalbaranp WHERE idalbaranp = idp LOOP
	totalBImponibleLineas := totalBImponibleLineas + rs.subtotal1;
    END LOOP;

    SELECT INTO rs2 idproveedor FROM albaranp WHERE idalbaranp = idp;

    SELECT INTO rs irpfproveedor FROM proveedor WHERE idproveedor = rs2.idproveedor;

    IF FOUND THEN
        totalIRPF := totalBImponibleLineas * (rs.irpfproveedor / 100);
    END IF;

    FOR rs IN SELECT cantlalbaranp * pvplalbaranp * (1 - descuentolalbaranp / 100) * (ivalalbaranp / 100) AS subtotal1 FROM lalbaranp WHERE idalbaranp = idp LOOP
	totalIVA := totalIVA + rs.subtotal1;
    END LOOP;

    FOR rs IN SELECT cantlalbaranp * pvplalbaranp * (1 - descuentolalbaranp / 100) * (reqeqlalbaranp / 100) AS subtotal1 FROM lalbaranp WHERE idalbaranp = idp LOOP
	totalRE := totalRE + rs.subtotal1;
    END LOOP;

    FOR rs IN SELECT proporciondalbaranp FROM dalbaranp WHERE idalbaranp = idp LOOP
	totalBImponibleLineas := totalBImponibleLineas * (1 - rs.proporciondalbaranp / 100);
	totalIRPF := totalIRPF * (1 - rs.proporciondalbaranp / 100);
	totalIVA := totalIVA * (1 - rs.proporciondalbaranp / 100);
	totalRE := totalRE * (1 - rs.proporciondalbaranp / 100);
    END LOOP;

    totalTotal = totalBImponibleLineas - totalIRPF + totalIVA + totalRE;

    RETURN totalTotal;
END;
' LANGUAGE plpgsql;


\echo -n ':: Funcion que calcula la Base Imponible total de un albaran de proveedor ... '
CREATE OR REPLACE FUNCTION calcbimpalbpro(integer) RETURNS numeric(12, 2)
AS '
DECLARE
    idp ALIAS FOR $1;
    total numeric(12, 2);
    rs RECORD;

BEGIN
    total := 0;
    FOR rs IN SELECT cantlalbaranp * pvplalbaranp * (1 - descuentolalbaranp / 100) AS subtotal1 FROM lalbaranp WHERE idalbaranp = idp LOOP
	total := total + rs.subtotal1;
    END LOOP;
    FOR rs IN SELECT proporciondalbaranp FROM dalbaranp WHERE idalbaranp = idp LOOP
	total := total * (1 - rs.proporciondalbaranp / 100);
    END LOOP;
    RETURN total;
END;
' LANGUAGE plpgsql;


\echo -n ':: Funcion que calcula el total de impuestos de un albaran de proveedor ... '
CREATE OR REPLACE FUNCTION calcimpuestosalbpro(integer) RETURNS numeric(12, 2)
AS '
DECLARE
    idp ALIAS FOR $1;
    total numeric(12, 2);
    rs RECORD;

BEGIN
    total := 0;

    FOR rs IN SELECT cantlalbaranp * pvplalbaranp * (1 - descuentolalbaranp / 100) * (ivalalbaranp / 100) AS subtotal1 FROM lalbaranp WHERE idalbaranp = idp LOOP
	total := total + rs.subtotal1;
    END LOOP;

    FOR rs IN SELECT proporciondalbaranp FROM dalbaranp WHERE idalbaranp = idp LOOP
	total := total * (1 - rs.proporciondalbaranp / 100);
    END LOOP;

    RETURN total;
END;
' LANGUAGE plpgsql;


\echo -n ':: Funcion que actualiza el total de un albaran de proveedor ... '
CREATE OR REPLACE FUNCTION actualizatotalbaranp() returns TRIGGER
AS '
DECLARE
    tot NUMERIC(12, 2);
    bimp NUMERIC(12, 2);
    imp NUMERIC(12, 2);

BEGIN
    tot := calctotalalbpro(NEW.idalbaranp);
    bimp := calcbimpalbpro(NEW.idalbaranp);
    imp := calcimpuestosalbpro(NEW.idalbaranp);

    UPDATE albaranp SET totalalbaranp = tot, bimpalbaranp = bimp, impalbaranp = imp WHERE idalbaranp = NEW.idalbaranp;

    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador que actualiza el total de un albaran de proveedor al insertar una linea de detalle ... '
CREATE TRIGGER totalesautomaticosalbaranptrigger
    AFTER INSERT OR UPDATE ON lalbaranp
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotalbaranp();


\echo -n ':: Disparador que actualiza el total de un albaran de proveedor al insertar un descuento ... '
CREATE TRIGGER totalesautomaticosalbaranptrigger1
    AFTER INSERT OR UPDATE ON dalbaranp
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotalbaranp();


\echo -n ':: Funcion que actualiza el total de un albaran de proveedor al borrar una linea de detalle ... '
CREATE OR REPLACE FUNCTION actualizatotalbaranpb() returns TRIGGER
AS '
DECLARE
    tot NUMERIC(12, 2);
    bimp NUMERIC(12, 2);
    imp NUMERIC(12, 2);

BEGIN
    tot := calctotalalbpro(OLD.idalbaranp);
    bimp := calcbimpalbpro(OLD.idalbaranp);
    imp := calcimpuestosalbpro(OLD.idalbaranp);

    UPDATE albaranp SET totalalbaranp = tot, bimpalbaranp = bimp, impalbaranp = imp WHERE idalbaranp = OLD.idalbaranp;

    RETURN OLD;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador que actualiza el total de un albaran de proveedor al borrar una linea de detalle ... '
CREATE TRIGGER totalesautomaticosalbaranptriggerd
    AFTER DELETE OR UPDATE ON lalbaranp
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotalbaranpb();


\echo -n ':: Disparador que actualiza el total de un albaran de proveedor al borrar un descuento ... '
CREATE TRIGGER totalesautomaticosalbaranptriggerd1
    AFTER DELETE OR UPDATE ON dalbaranp
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotalbaranpb();


-- **********************************************************************
-- APARTADO DE COMPROBACIONES DE INTEGRIDAD EXTRA Y DETECCION DE ERROrs.
-- **********************************************************************
-- **********************************************************************
\echo -n ':: Funcion random_string que genera una cadena aleatoria ... '
CREATE FUNCTION random_string(int4) RETURNS "varchar" AS '
DECLARE
    iLoop int4;
    result varchar;

BEGIN
    result = '''';

    IF ($1 > 0) AND ($1 < 255) THEN
	FOR iLoop in 1 .. $1 LOOP
	    result = result || chr(int4(random()*25)+65);
	END LOOP;

	RETURN result;
    ELSE
	RETURN ''f'';
    END IF;
END;
'  LANGUAGE 'plpgsql';


\echo -n ':: Funcion crearef para crear codigos de referencia ... '
CREATE FUNCTION crearef() RETURNS character varying(15)
AS $$
DECLARE
    rs RECORD;
    result character varying(15);
    efound boolean;

BEGIN
    efound := FALSE;

    WHILE efound = FALSE LOOP
	result := random_string(6);
	efound := TRUE;

	SELECT INTO rs idpresupuesto FROM presupuesto WHERE refpresupuesto = result;

	IF FOUND THEN
	    efound := FALSE;
	END IF;

	SELECT INTO rs idpedidocliente FROM pedidocliente WHERE refpedidocliente = result;

	IF FOUND THEN
	    efound := FALSE;
	END IF;

	SELECT INTO rs idalbaran FROM albaran WHERE refalbaran = result;

	IF FOUND THEN
	    efound := FALSE;
	END IF;

	SELECT INTO rs idfactura FROM factura WHERE reffactura = result;

	IF FOUND THEN
		efound := FALSE;
	END IF;

	SELECT INTO rs idcobro FROM cobro WHERE refcobro = result;

	IF FOUND THEN
		efound := FALSE;
	END IF;

	SELECT INTO rs idpedidoproveedor FROM pedidoproveedor WHERE refpedidoproveedor = result;

	IF FOUND THEN
		efound := FALSE;
	END IF;

	SELECT INTO rs idalbaranp FROM albaranp WHERE refalbaranp = result;

	IF FOUND THEN
		efound := FALSE;
	END IF;

	SELECT INTO rs idfacturap FROM facturap WHERE reffacturap = result;
	IF FOUND THEN
		efound := FALSE;
	END IF;

	SELECT INTO rs idpago FROM pago WHERE refpago = result;

	IF FOUND THEN
		efound := FALSE;
	END IF;

-- PLUGINS

    END LOOP;
    RETURN result;
END;
$$ LANGUAGE plpgsql;


-- ** albaran **
-- COMPROBACIONES DE INTEGRIDAD: Todos los albaranes de una factura corresponden con un mismo
--                               cliente.
-- numalbaran: Numero de albaran.
-- descalbaran: Descripcion del albaran.
-- refalbaran: Codigo de referencia del albaran.
-- fechaalbaran: Fecha de emision del albaran.
-- comentalbaran: Comentario.
-- comentprivalbaran: Comentario privado del albaran.
-- procesadoalbaran: Indica si se ha procesado el albaran.
-- contactalbaran: Persona de contacto que ha realizado el albaran.
-- telalbaran: Telefono de la persona de contacto.
-- idusuari: ???Usuario que tramita el albaran.
-- idcliente: Cliente al que se emite el albaran.
-- iidforma_pago: Forma de pago del abaran.
-- idfactura: Factura a la que pertene el albaran.
-- idalmacen: Almacen que emite el albaran.
-- idtrabajador: ???Trabajador que tramita el abaran.
-- totalalbaran: Importe total albaran
-- bimpalbaran: Base imponible albaran.
-- impalbaran: Total impuestos albaran.
-- UNIQUE:
\echo -n ':: Albaran ... '
CREATE TABLE albaran (
    idalbaran serial PRIMARY KEY,
    numalbaran integer NOT NULL UNIQUE,
    descalbaran character varying(150),
    refalbaran character varying(12) NOT NULL,
    fechaalbaran date DEFAULT now(),
    comentalbaran character varying(3000),
    comentprivalbaran character varying(3000),
    procesadoalbaran boolean DEFAULT FALSE,
    contactalbaran character varying,
    telalbaran character varying,
    idusuari integer,
    idcliente integer REFERENCES cliente(idcliente),
    idforma_pago integer REFERENCES forma_pago(idforma_pago),
    idfactura integer REFERENCES factura(idfactura),
    idalmacen integer NOT NULL REFERENCES almacen(idalmacen),
    idtrabajador integer REFERENCES trabajador(idtrabajador),   
    totalalbaran NUMERIC(12, 2) DEFAULT 0,
    bimpalbaran NUMERIC(12, 2) DEFAULT 0,
    impalbaran NUMERIC(12, 2) DEFAULT 0,
    UNIQUE (idalmacen, numalbaran)
);


\echo -n ':: Funcion con restricciones en el albaran ... '
CREATE FUNCTION restriccionesalbaran() RETURNS "trigger"
AS '
DECLARE
    rs RECORD;

BEGIN
    IF NEW.fechaalbaran IS NULL THEN
	NEW.fechaalbaran := now();
    END IF;

    IF NEW.numalbaran IS NULL THEN
	SELECT INTO rs max(numalbaran) AS m FROM albaran;

	IF rs.m IS NOT NULL THEN
	    NEW.numalbaran := rs.m + 1;
	ELSE
	    NEW.numalbaran := 1;
	END IF;
    END IF;

    IF NEW.refalbaran IS NULL OR NEW.refalbaran = '''' THEN
	SELECT INTO rs crearef() AS m;

	IF FOUND THEN
	    NEW.refalbaran := rs.m;
	END IF;
    END IF;

    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador de las restricciones antes de insertar o actualizar un albaran ... '
CREATE TRIGGER restriccionesalbarantrigger
    BEFORE INSERT OR UPDATE ON albaran
    FOR EACH ROW
    EXECUTE PROCEDURE restriccionesalbaran();


-- ** dalbaran **
-- conceptdalbaran: Motivo por el que se realiza el descuento en el albaran.
-- proporciondalbaran: Porcentaje de descuento a aplicar.
-- idalbaran: Albaran al que se realiza el descuento de albaran.
\echo -n ':: Descuento de albaran ... '
CREATE TABLE dalbaran (
    iddalbaran serial PRIMARY KEY,
    conceptdalbaran character varying(500),
    proporciondalbaran numeric(12, 2),
    idalbaran integer NOT NULL REFERENCES albaran(idalbaran)
);


-- ** lalbaran **
-- Linea de detalle del albaran.
-- desclalbaran: Descripcion linea albaran.
-- cantlalbaran: Cantidad.
-- pvplalbaran: Precio de venta.
-- descuentolalbaran: Porcentaje de descuento a aplicar.
-- ivalalbaran: IVA correspondiente al articulo.
-- reqeqlalbaran: Recargo de equivalencia linea albaran.
-- ordenlalbaran: 
-- idalbaran: Albaran al que pertenece la linea de albaran.
-- idarticulo: Articulo linea albaran.
\echo -n ':: Lineas de albaran ... '
CREATE TABLE lalbaran (
    numlalbaran serial PRIMARY KEY,
    desclalbaran character varying,
    cantlalbaran numeric(12, 2),
    pvplalbaran numeric(12, 2),
    descuentolalbaran numeric(12, 2),
    ivalalbaran numeric(12, 2),
    reqeqlalbaran NUMERIC(12,2) DEFAULT 0,
    ordenlalbaran integer,
    idalbaran integer NOT NULL REFERENCES albaran(idalbaran),
    idarticulo integer NOT NULL REFERENCES articulo(idarticulo)
);


\echo -n ':: Funcion que actualiza el total de un albaran ... '
CREATE OR REPLACE FUNCTION actualizatotalbaran() returns TRIGGER
AS '
DECLARE
    tot NUMERIC(12, 2);
    bimp NUMERIC(12, 2);
    imp NUMERIC(12, 2);

BEGIN
    tot := calctotalalbaran(NEW.idalbaran);
    bimp := calcbimpalbaran(NEW.idalbaran);
    imp := calcimpuestosalbaran(NEW.idalbaran);

    UPDATE albaran SET totalalbaran = tot, bimpalbaran = bimp, impalbaran = imp WHERE idalbaran = NEW.idalbaran;

    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador que actualiza el total de un albaran al insertar o actualizar una linea de detalle ... '
CREATE TRIGGER totalesautomaticosalbarantrigger
    AFTER INSERT OR UPDATE ON lalbaran
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotalbaran();


\echo -n ':: Disparador que actualiza el total de un albaran al insertar o actualizar una descuento ... '
CREATE TRIGGER totalesautomaticosalbarantrigger1
    AFTER INSERT OR UPDATE ON dalbaran
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotalbaran();


\echo -n ':: Funcion que actualiza el total de un albaran al borrar una linea de detalle ... '
CREATE OR REPLACE FUNCTION actualizatotalbaranb() returns TRIGGER
AS '
DECLARE
    tot NUMERIC(12, 2);
    bimp NUMERIC(12, 2);
    imp NUMERIC(12, 2);

BEGIN
    tot := calctotalalbaran(OLD.idalbaran);
    bimp := calcbimpalbaran(OLD.idalbaran);
    imp := calcimpuestosalbaran(OLD.idalbaran);

    UPDATE albaran SET totalalbaran = tot, bimpalbaran = bimp, impalbaran = imp WHERE idalbaran = OLD.idalbaran;

    RETURN OLD;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador que actualiza el total de un albaran al borrar una linea de detalle ... '
CREATE TRIGGER totalesautomaticosalbarantriggerd
    AFTER DELETE OR UPDATE ON lalbaran
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotalbaranb();


\echo -n ':: Disparador que actualiza el total de un albaran al borrar un descuento ... '
CREATE TRIGGER totalesautomaticosalbarantriggerd1
    AFTER DELETE OR UPDATE ON dalbaran
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotalbaranb();

    
-- ** ticket **
-- Para uso exclusivo con TPV:
-- fechaticket: Fecha emision de ticket.
\echo -n ':: Ticket ... '
CREATE TABLE ticket (
    numticket integer PRIMARY KEY,
    fechaticket date
);


-- ** suministra **
-- Almacena la informacion necesaria para saber que proveedor suministra un determinado
-- articulo.
-- refpro: Referencia del proveedor.
-- principalsuministra:
-- idproveedor: Proveedor que suministra.
-- idarticulo: Articulo a suministrar.
\echo -n ':: Controla que proveedor suministra un articulo ... '
CREATE TABLE suministra (
    idsuministra serial PRIMARY KEY,
    refpro character varying(100),
    principalsuministra numeric(12,2),
    idproveedor integer REFERENCES proveedor(idproveedor),
    idarticulo integer REFERENCES articulo(idarticulo)
);


-- ** precio_compra **
-- idarticulo: Articulo precio compra.
-- iddivision: Division precio compra.
-- idalmacen: Almacen precio compra.
-- fechapreciocompra: Fecha precio de compra.
-- valorpreciocompra: Importe precio compra.
\echo -n ':: Precio de compra ... '
CREATE TABLE precio_compra (
    idprecio_compra serial PRIMARY KEY,
    idarticulo integer NOT NULL REFERENCES articulo(idarticulo),
    iddivision integer REFERENCES division(iddivision),
    idalmacen integer REFERENCES almacen(idalmacen),
    fechapreciocompra date,
    valorpreciocompra numeric(13, 4) NOT NULL
);


-- ** codigobarras **
-- idarticulo: Articulo codigo barras.
-- ean14codigobarras: Codigo de barras del articulo.
-- unixcajacodigobarras: Codigo de barras del articulo (envase: cajas).
-- cajxpaletcodigobarras: Codigo de barras del articulo (envase: palets).
-- unidadcodigobarras: 
\echo -n ':: Codigo de barras ... '
CREATE TABLE codigobarras (
    idcodigobarras serial PRIMARY KEY,
    idarticulo integer NOT NULL REFERENCES articulo(idarticulo),
    ean14codigobarras numeric(14, 0) NOT NULL UNIQUE,
    unixcajacodigobarras numeric(4, 0),
    cajxpaletcodigobarras numeric(4, 0),
    unidadcodigobarras character(1)
);


-- FUNCIONES VARIAS DE SOPORTE.
\echo -n ':: Funcion que calcula el IVA de un articulo ... '
CREATE OR REPLACE FUNCTION ivaarticulo(integer) RETURNS numeric(12, 2)
AS'
DECLARE
    idarticulo ALIAS FOR $1;
    rs RECORD;

BEGIN
    SELECT INTO rs * FROM tipo_iva, tasa_iva, articulo WHERE tasa_iva.idtipo_iva = tipo_iva.idtipo_iva AND tipo_iva.idtipo_iva = articulo.idtipo_iva AND articulo.idarticulo = idarticulo ORDER BY fechatasa_iva;

    IF FOUND THEN
	RETURN rs.porcentasa_iva;
    END IF;

    RETURN 0.0;
END;
' LANGUAGE plpgsql;


\echo -n ':: Funcion que calcula el PVP de un articulo ... '
CREATE OR REPLACE FUNCTION pvparticulo(integer) RETURNS numeric(12, 2)
AS'
DECLARE
    idarticulo ALIAS FOR $1;
    rs RECORD;

BEGIN
    SELECT INTO rs pvparticulo FROM articulo WHERE articulo.idarticulo = idarticulo;

    IF FOUND THEN
	RETURN rs.pvparticulo;
    END IF;

    RETURN 0.0;
END;
' LANGUAGE plpgsql;


-- ** pedidoproveedor **
-- numpedidoproveedor: Numero de pedido a proveedor.
-- fechapedidoproveedor: Fecha de pedido a proveedor.
-- refpedidoproveedor: Referencia de pedido a proveedor.
-- descpedidoproveedor: Descripcion pedido a proveedor.
-- comentpedidoproveedor: Comentario pedido a provedor.
-- contactpedidoproveedor: Contacto pedido a proveedor.
-- telpedidoproveedor: Telefono contacto pedido a proveedor.
-- procesadopedidoproveedor: Pedido de proveedor procesado.
-- idproveedor: Proveedor del pedido.
-- idforma_pago: Forma de pago de pedido a proveedor.
-- idalmacen: Almacen que realiza el pedido a proveedor.
-- idtrabajador: Trabajador que realiza el pedido a proveedor.
-- totalpedidoproveedor: Total pedido a proveedor.
-- bimppedidoproveedor: Base imponible pedido a proveedor.
-- imppedidoproveedor: Impuestos pedido a proveedor.
\echo -n ':: Pedido a proveedor ... '
CREATE TABLE pedidoproveedor (
    idpedidoproveedor serial PRIMARY KEY,
    numpedidoproveedor character varying(20) UNIQUE NOT NULL,
    fechapedidoproveedor date DEFAULT now(),
    refpedidoproveedor character varying(12) NOT NULL,   
    descpedidoproveedor character varying(150),
    comentpedidoproveedor character varying(3000),
    contactpedidoproveedor character varying(90),
    telpedidoproveedor character varying(20),   
    procesadopedidoproveedor boolean DEFAULT FALSE,   
    idproveedor integer NOT NULL REFERENCES proveedor(idproveedor),
    idforma_pago integer REFERENCES forma_pago(idforma_pago),    
    idalmacen integer NOT NULL REFERENCES almacen(idalmacen),
    idtrabajador integer REFERENCES trabajador(idtrabajador),
    totalpedidoproveedor NUMERIC(12, 2) DEFAULT 0,
    bimppedidoproveedor NUMERIC(12, 2) DEFAULT 0,
    imppedidoproveedor NUMERIC(12, 2) DEFAULT 0
);


\echo -n ':: Funcion con restricciones en pedidos a proveedor ... '
CREATE FUNCTION restriccionespedidoproveedor() RETURNS "trigger"
AS '
DECLARE
    rs RECORD;

BEGIN
    IF NEW.fechapedidoproveedor IS NULL THEN
	NEW.fechapedidoproveedor := now();
    END IF;

    IF NEW.numpedidoproveedor IS NULL THEN
	NEW.numpedidoproveedor := NEW.idpedidoproveedor;
    END IF;

    IF NEW.refpedidoproveedor IS NULL OR NEW.refpedidoproveedor = '''' THEN
	SELECT INTO rs crearef() AS m;

	IF FOUND THEN
	    NEW.refpedidoproveedor := rs.m;
	END IF;
    END IF;

    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador de restricciones antes de insertar o actualizar un pedidos a proveedor ... '
CREATE TRIGGER restriccionespedidoproveedortrigger
    BEFORE INSERT OR UPDATE ON pedidoproveedor
    FOR EACH ROW
    EXECUTE PROCEDURE restriccionespedidoproveedor();


-- ** dpedidoproveedor **
-- conceptdpedidoproveedor: Motivo por el que se aplica el descuento al pedido.
-- proporciondpedidoproveedor: Porcentaje de descuento.
-- idpedidoproveedor: Pedido a proveedor sobre el que se realiza el descuento.
\echo -n ':: Descuento en pedido a proveedor ... '
CREATE TABLE dpedidoproveedor (
    iddpedidoproveedor serial PRIMARY KEY,
    conceptdpedidoproveedor character varying(2000),
    proporciondpedidoproveedor numeric(12, 2),
    idpedidoproveedor integer NOT NULL REFERENCES pedidoproveedor(idpedidoproveedor)
);    

    
-- ** lpedidoproveedor **
-- Linea de detalle del pedido a proveedor.
-- desclpedidoproveedor: Descripcion linea pedido proveedor.
-- cantlpedidoproveedor: Cantidad.
-- pvplpedidoproveedor: Precio de compra.
-- prevlpedidoproveedor: Prevision de recepcion de la mercancia.
-- ivalpedidoproveedor: IVA aplicado al articulo.
-- reqeqlpedidoproveedor: Recaro de equivalencia linea pedido proveedor.
-- descuentolpedidoproveedor: Descuento aplicado al articulo.
-- ordenlpedidoproveedor: Numero de orden linea pedido proveedor.
-- idpedidoproveedor: Pedido a proveedor al que pertenece esta linea de pedido.
-- puntlpedidoproveedor: 
-- idarticulo: Articulo linea pedido proveedor.
\echo -n ':: Lineas de pedido a proveedor ... '
CREATE TABLE lpedidoproveedor (
    idlpedidoproveedor serial PRIMARY KEY,
    desclpedidoproveedor character varying,
    cantlpedidoproveedor numeric(12, 2),
    pvplpedidoproveedor numeric(12, 2),
    prevlpedidoproveedor date,
    ivalpedidoproveedor numeric(12, 2),
    reqeqlpedidoproveedor NUMERIC(12,2) DEFAULT 0,
    descuentolpedidoproveedor numeric(12, 2),
    ordenlpedidoproveedor integer,
    idpedidoproveedor integer NOT NULL REFERENCES pedidoproveedor(idpedidoproveedor),
    puntlpedidoproveedor boolean DEFAULT FALSE,
    idarticulo integer REFERENCES articulo(idarticulo)
);


\echo -n ':: Funcion que calcula el total del pedido a proveedor ... '
CREATE OR REPLACE FUNCTION calctotalpedpro(integer) RETURNS numeric(12, 2)
AS '
DECLARE
    idp ALIAS FOR $1;
    totalBImponibleLineas numeric(12, 2);
    totalIRPF numeric(12, 2);
    totalIVA numeric(12, 2);
    totalRE numeric(12, 2);
    totalTotal numeric(12, 2);
    rs RECORD;
    rs2 RECORD;

BEGIN
    totalBImponibleLineas := 0;
    totalIRPF := 0;
    totalIVA := 0;
    totalRE := 0;
    totalTotal := 0;

    FOR rs IN SELECT cantlpedidoproveedor * pvplpedidoproveedor * (1 - descuentolpedidoproveedor / 100) AS subtotal1 FROM lpedidoproveedor WHERE idpedidoproveedor = idp LOOP
	totalBImponibleLineas := totalBImponibleLineas + rs.subtotal1;
    END LOOP;

    SELECT INTO rs2 idproveedor FROM pedidoproveedor WHERE idpedidoproveedor = idp;

    SELECT INTO rs irpfproveedor FROM proveedor WHERE idproveedor = rs2.idproveedor;

    IF FOUND THEN
        totalIRPF := totalBImponibleLineas * (rs.irpfproveedor / 100);
    END IF;

    FOR rs IN SELECT cantlpedidoproveedor * pvplpedidoproveedor * (1 - descuentolpedidoproveedor / 100) * (ivalpedidoproveedor / 100) AS subtotal1 FROM lpedidoproveedor WHERE idpedidoproveedor = idp LOOP
	totalIVA := totalIVA + rs.subtotal1;
    END LOOP;

    FOR rs IN SELECT cantlpedidoproveedor * pvplpedidoproveedor * (1 - descuentolpedidoproveedor / 100) * (reqeqlpedidoproveedor / 100) AS subtotal1 FROM lpedidoproveedor WHERE idpedidoproveedor = idp LOOP
	totalRE := totalRE + rs.subtotal1;
    END LOOP;

    FOR rs IN SELECT proporciondpedidoproveedor FROM dpedidoproveedor WHERE idpedidoproveedor = idp LOOP
	totalBImponibleLineas := totalBImponibleLineas * (1 - rs.proporciondpedidoproveedor / 100);
	totalIRPF := totalIRPF * (1 - rs.proporciondpedidoproveedor / 100);
	totalIVA := totalIVA * (1 - rs.proporciondpedidoproveedor / 100);
	totalRE := totalRE * (1 - rs.proporciondpedidoproveedor / 100);
    END LOOP;

    totalTotal = totalBImponibleLineas - totalIRPF + totalIVA + totalRE;

    RETURN totalTotal;
END;
' LANGUAGE plpgsql;


\echo -n ':: Funcion que calcula la Base Imponible total de un pedido a proveedor ... '
CREATE OR REPLACE FUNCTION calcbimppedpro(integer) RETURNS numeric(12, 2)
AS '
DECLARE
    idp ALIAS FOR $1;
    total numeric(12, 2);
    rs RECORD;

BEGIN
    total := 0;
    FOR rs IN SELECT cantlpedidoproveedor * pvplpedidoproveedor * (1 - descuentolpedidoproveedor / 100) AS subtotal1 FROM lpedidoproveedor WHERE idpedidoproveedor = idp LOOP
	total := total + rs.subtotal1;
    END LOOP;
    FOR rs IN SELECT proporciondpedidoproveedor FROM dpedidoproveedor WHERE idpedidoproveedor = idp LOOP
	total := total * (1 - rs.proporciondpedidoproveedor  / 100);
    END LOOP;
    RETURN total;
END;
' LANGUAGE plpgsql;


\echo -n ':: Funcion que calcula el total de impuestos de un pedido a proveedor ... '
CREATE OR REPLACE FUNCTION calcimpuestospedpro(integer) RETURNS numeric(12, 2)
AS '
DECLARE
    idp ALIAS FOR $1;
    total numeric(12, 2);
    rs RECORD;

BEGIN
    total := 0;

    FOR rs IN SELECT cantlpedidoproveedor * pvplpedidoproveedor * (1 - descuentolpedidoproveedor / 100) * (ivalpedidoproveedor / 100) AS subtotal1 FROM lpedidoproveedor WHERE idpedidoproveedor = idp LOOP
	total := total + rs.subtotal1;
    END LOOP;

    FOR rs IN SELECT proporciondpedidoproveedor FROM dpedidoproveedor WHERE idpedidoproveedor = idp LOOP
	total := total * (1 - rs.proporciondpedidoproveedor / 100);
    END LOOP;

    RETURN total;
END;
' LANGUAGE plpgsql;


\echo -n ':: Funcion que actualiza el total de un pedido a proveedor ... '
CREATE OR REPLACE FUNCTION actualizatotpedidoproveedor() returns TRIGGER
AS '
DECLARE
    tot NUMERIC(12, 2);
    bimp NUMERIC(12, 2);
    imp NUMERIC(12, 2);

BEGIN
    tot := calctotalpedpro(NEW.idpedidoproveedor);
    bimp := calcbimppedpro(NEW.idpedidoproveedor);
    imp := calcimpuestospedpro(NEW.idpedidoproveedor);

    UPDATE pedidoproveedor SET totalpedidoproveedor = tot, bimppedidoproveedor = bimp, imppedidoproveedor = imp WHERE idpedidoproveedor = NEW.idpedidoproveedor;

    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador que actualiza el total de un pedido a proveedor al insertar una linea de detalle ... '
CREATE TRIGGER totalesautomaticospedidoproveedortrigger
    AFTER INSERT OR UPDATE ON lpedidoproveedor
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotpedidoproveedor();


\echo -n ':: Disparador que actualiza el total de un pedido a proveedor al insertar un descuento ... '
CREATE TRIGGER totalesautomaticospedidoproveedortrigger1
    AFTER INSERT OR UPDATE ON dpedidoproveedor
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotpedidoproveedor();


\echo -n ':: Funcion que actualiza un pedido a proveedor al borrar una linea de detalle ... '
CREATE OR REPLACE FUNCTION actualizatotpedidoproveedorb() returns TRIGGER
AS '
DECLARE
    tot NUMERIC(12, 2);
    bimp NUMERIC(12, 2);
    imp NUMERIC(12, 2);

BEGIN
    tot := calctotalpedpro(OLD.idpedidoproveedor);
    bimp := calcbimppedpro(OLD.idpedidoproveedor);
    imp := calcimpuestospedpro(OLD.idpedidoproveedor);

    UPDATE pedidoproveedor SET totalpedidoproveedor = tot, bimppedidoproveedor = bimp, imppedidoproveedor = imp WHERE idpedidoproveedor = OLD.idpedidoproveedor;

    RETURN OLD;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador que actualiza el total de un pedido a proveedor al borrar una linea de detalle ... '
CREATE TRIGGER totalesautomaticospedidoproveedortriggerd
    AFTER DELETE OR UPDATE ON lpedidoproveedor
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotpedidoproveedorb();


\echo -n ':: Disparador que actualiza el total de un pedido a proveedor al borrar un descuento ... '
CREATE TRIGGER totalesautomaticospedidoproveedortriggerd1
    AFTER DELETE OR UPDATE ON dpedidoproveedor
    FOR EACH ROW
    EXECUTE PROCEDURE actualizatotpedidoproveedorb();


\echo -n ':: Funcion que calcula el total de un presupuesto a cliente ... '
CREATE OR REPLACE FUNCTION calctotalpres(integer) RETURNS numeric(12, 2)
AS '
DECLARE
    idp ALIAS FOR $1;
    totalBImponibleLineas numeric(12, 2);
    totalIRPF numeric(12, 2);
    totalIVA numeric(12, 2);
    totalRE numeric(12, 2);
    totalTotal numeric(12, 2);
    rs RECORD;
    rs2 RECORD;

BEGIN
    totalBImponibleLineas := 0;
    totalIRPF := 0;
    totalIVA := 0;
    totalRE := 0;
    totalTotal := 0;

    FOR rs IN SELECT cantlpresupuesto * pvplpresupuesto * (1 - descuentolpresupuesto / 100) AS subtotal1 FROM lpresupuesto WHERE idpresupuesto = idp LOOP
	totalBImponibleLineas := totalBImponibleLineas + rs.subtotal1;
    END LOOP;

    SELECT INTO rs valor::numeric FROM configuracion WHERE LOWER(nombre) = ''irpf'';

    IF FOUND THEN
        totalIRPF := totalBImponibleLineas * (rs.valor / 100);
    END IF;

    FOR rs IN SELECT cantlpresupuesto * pvplpresupuesto * (1 - descuentolpresupuesto / 100) * (ivalpresupuesto / 100) AS subtotal1 FROM lpresupuesto WHERE idpresupuesto = idp LOOP
	totalIVA := totalIVA + rs.subtotal1;
    END LOOP;

    FOR rs IN SELECT cantlpresupuesto * pvplpresupuesto * (1 - descuentolpresupuesto / 100) * (reqeqlpresupuesto / 100) AS subtotal1 FROM lpresupuesto WHERE idpresupuesto = idp LOOP
	totalRE := totalRE + rs.subtotal1;
    END LOOP;

    FOR rs IN SELECT proporciondpresupuesto FROM dpresupuesto WHERE idpresupuesto = idp LOOP
	totalBImponibleLineas := totalBImponibleLineas * (1 - rs.proporciondpresupuesto / 100);
	totalIRPF := totalIRPF * (1 - rs.proporciondpresupuesto / 100);
	totalIVA := totalIVA * (1 - rs.proporciondpresupuesto / 100);
	totalRE := totalRE * (1 - rs.proporciondpresupuesto / 100);
    END LOOP;

    totalTotal = totalBImponibleLineas - totalIRPF + totalIVA + totalRE;

    RETURN totalTotal;
END;
' LANGUAGE plpgsql;


\echo -n ':: Funcion que calcula la Base Imponible total de un presupuesto a cliente ... '
CREATE OR REPLACE FUNCTION calcbimppres(integer) RETURNS numeric(12, 2)
AS '
DECLARE
    idp ALIAS FOR $1;
    total numeric(12, 2);
    rs RECORD;

BEGIN
    total := 0;

    FOR rs IN SELECT cantlpresupuesto * pvplpresupuesto * (1 - descuentolpresupuesto / 100) AS subtotal1 FROM lpresupuesto WHERE idpresupuesto = idp LOOP
	total := total + rs.subtotal1;
    END LOOP;

    FOR rs IN SELECT proporciondpresupuesto FROM dpresupuesto WHERE idpresupuesto = idp LOOP
	total := total * (1 - rs.proporciondpresupuesto / 100);
    END LOOP;

    RETURN total;
END;
' LANGUAGE plpgsql;


\echo -n ':: Funcion que calcula el total de impuestos de un presupuesto a cliente ... '
CREATE OR REPLACE FUNCTION calcimpuestospres(integer) RETURNS numeric(12, 2)
AS '
DECLARE
    idp ALIAS FOR $1;
    total numeric(12, 2);
    rs RECORD;

BEGIN
    total := 0;

    FOR rs IN SELECT cantlpresupuesto * pvplpresupuesto * (1 - descuentolpresupuesto / 100) * (ivalpresupuesto / 100) AS subtotal1 FROM lpresupuesto WHERE idpresupuesto = idp LOOP
	total := total + rs.subtotal1;
    END LOOP;

    FOR rs IN SELECT proporciondpresupuesto FROM dpresupuesto WHERE idpresupuesto = idp LOOP
	total := total * (1 - rs.proporciondpresupuesto / 100);
    END LOOP;

    RETURN total;
END;
' LANGUAGE plpgsql;


\echo -n ':: Funcion que calcula el total de un pedido de cliente ... '
CREATE OR REPLACE FUNCTION calctotalpedcli(integer) RETURNS numeric(12, 2)
AS '
DECLARE
    idp ALIAS FOR $1;
    totalBImponibleLineas numeric(12, 2);
    totalIRPF numeric(12, 2);
    totalIVA numeric(12, 2);
    totalRE numeric(12, 2);
    totalTotal numeric(12, 2);
    rs RECORD;
    rs2 RECORD;

BEGIN
    totalBImponibleLineas := 0;
    totalIRPF := 0;
    totalIVA := 0;
    totalRE := 0;
    totalTotal := 0;

    FOR rs IN SELECT cantlpedidocliente * pvplpedidocliente * (1 - descuentolpedidocliente / 100) AS subtotal1 FROM lpedidocliente WHERE idpedidocliente = idp LOOP
	totalBImponibleLineas := totalBImponibleLineas + rs.subtotal1;
    END LOOP;

    SELECT INTO rs valor::numeric FROM configuracion WHERE LOWER(nombre) = ''irpf'';

    IF FOUND THEN
        totalIRPF := totalBImponibleLineas * (rs.valor / 100);
    END IF;

    FOR rs IN SELECT cantlpedidocliente * pvplpedidocliente * (1 - descuentolpedidocliente / 100) * (ivalpedidocliente / 100) AS subtotal1 FROM lpedidocliente WHERE idpedidocliente = idp LOOP
	totalIVA := totalIVA + rs.subtotal1;
    END LOOP;

    FOR rs IN SELECT cantlpedidocliente * pvplpedidocliente * (1 - descuentolpedidocliente / 100) * (reqeqlpedidocliente / 100) AS subtotal1 FROM lpedidocliente WHERE idpedidocliente = idp LOOP
	totalRE := totalRE + rs.subtotal1;
    END LOOP;

    FOR rs IN SELECT proporciondpedidocliente FROM dpedidocliente WHERE idpedidocliente = idp LOOP
	totalBImponibleLineas := totalBImponibleLineas * (1 - rs.proporciondpedidocliente / 100);
	totalIRPF := totalIRPF * (1 - rs.proporciondpedidocliente / 100);
	totalIVA := totalIVA * (1 - rs.proporciondpedidocliente / 100);
	totalRE := totalRE * (1 - rs.proporciondpedidocliente / 100);
    END LOOP;

    totalTotal = totalBImponibleLineas - totalIRPF + totalIVA + totalRE;

    RETURN totalTotal;
END;
' LANGUAGE plpgsql;


\echo -n ':: Funcion que calcula la Base Imponible total de un pedido de cliente ... '
CREATE OR REPLACE FUNCTION calcbimppedcli(integer) RETURNS numeric(12, 2)
AS '
DECLARE
    idp ALIAS FOR $1;
    total numeric(12, 2);
    rs RECORD;

BEGIN
    total := 0;

    FOR rs IN SELECT cantlpedidocliente * pvplpedidocliente * (1 - descuentolpedidocliente / 100) AS subtotal1 FROM lpedidocliente WHERE idpedidocliente = idp LOOP
    	total := total + rs.subtotal1;
    END LOOP;

    FOR rs IN SELECT proporciondpedidocliente FROM dpedidocliente WHERE idpedidocliente = idp LOOP
	total := total * (1 - rs.proporciondpedidocliente / 100);
    END LOOP;

    RETURN total;
END;
' LANGUAGE plpgsql;


\echo -n ':: Funcion que calcula el total de impuestos de un pedido de cliente ... '
CREATE OR REPLACE FUNCTION calcimpuestospedcli(integer) RETURNS numeric(12, 2)
AS '
DECLARE
    idp ALIAS FOR $1;
    total numeric(12, 2);
    rs RECORD;

BEGIN
    total := 0;

    FOR rs IN SELECT cantlpedidocliente * pvplpedidocliente * (1 - descuentolpedidocliente / 100) * (ivalpedidocliente / 100) AS subtotal1 FROM lpedidocliente WHERE idpedidocliente = idp LOOP
	total := total + rs.subtotal1;
    END LOOP;

    FOR rs IN SELECT proporciondpedidocliente FROM dpedidocliente WHERE idpedidocliente = idp LOOP
	total := total * (1 - rs.proporciondpedidocliente / 100);
    END LOOP;

    RETURN total;
END;
' LANGUAGE plpgsql;


\echo -n ':: Funcion que calcula el total de un albaran a cliente ... '
CREATE OR REPLACE FUNCTION calctotalalbaran(integer) RETURNS numeric(12, 2)
AS '
DECLARE
    idp ALIAS FOR $1;
    totalBImponibleLineas numeric(12, 2);
    totalIRPF numeric(12, 2);
    totalIVA numeric(12, 2);
    totalRE numeric(12, 2);
    totalTotal numeric(12, 2);
    rs RECORD;
    rs2 RECORD;

BEGIN
    totalBImponibleLineas := 0;
    totalIRPF := 0;
    totalIVA := 0;
    totalRE := 0;
    totalTotal := 0;

    FOR rs IN SELECT cantlalbaran * pvplalbaran * (1 - descuentolalbaran / 100) AS subtotal1 FROM lalbaran WHERE idalbaran = idp LOOP
	totalBImponibleLineas := totalBImponibleLineas + rs.subtotal1;
    END LOOP;

    SELECT INTO rs valor::numeric FROM configuracion WHERE LOWER(nombre) = ''irpf'';

    IF FOUND THEN
        totalIRPF := totalBImponibleLineas * (rs.valor / 100);
    END IF;

    FOR rs IN SELECT cantlalbaran * pvplalbaran * (1 - descuentolalbaran / 100) * (ivalalbaran / 100) AS subtotal1 FROM lalbaran WHERE idalbaran = idp LOOP
	totalIVA := totalIVA + rs.subtotal1;
    END LOOP;

    FOR rs IN SELECT cantlalbaran * pvplalbaran * (1 - descuentolalbaran / 100) * (reqeqlalbaran / 100) AS subtotal1 FROM lalbaran WHERE idalbaran = idp LOOP
	totalRE := totalRE + rs.subtotal1;
    END LOOP;

    FOR rs IN SELECT proporciondalbaran FROM dalbaran WHERE idalbaran = idp LOOP
	totalBImponibleLineas := totalBImponibleLineas * (1 - rs.proporciondalbaran / 100);
	totalIRPF := totalIRPF * (1 - rs.proporciondalbaran / 100);
	totalIVA := totalIVA * (1 - rs.proporciondalbaran / 100);
	totalRE := totalRE * (1 - rs.proporciondalbaran / 100);
    END LOOP;

    totalTotal = totalBImponibleLineas - totalIRPF + totalIVA + totalRE;

    RETURN totalTotal;
END;
' LANGUAGE plpgsql;


\echo -n ':: Funcion que calcula la Base Imponible total de un albaran a cliente ... '
CREATE OR REPLACE FUNCTION calcbimpalbaran(integer) RETURNS numeric(12, 2)
AS '
DECLARE
    idp ALIAS FOR $1;
    total numeric(12, 2);
    rs RECORD;

BEGIN
    total := 0;

    FOR rs IN SELECT cantlalbaran * pvplalbaran * (1 - descuentolalbaran / 100) AS subtotal1 FROM lalbaran WHERE idalbaran = idp LOOP
	total := total + rs.subtotal1;
    END LOOP;

    FOR rs IN SELECT proporciondalbaran FROM dalbaran WHERE idalbaran = idp LOOP
	total := total * (1 - rs.proporciondalbaran / 100);
    END LOOP;

    RETURN total;
END;
' LANGUAGE plpgsql;


\echo -n ':: Funcion que calcula los impuestos totales de un albaran a cliente ... '
CREATE OR REPLACE FUNCTION calcimpuestosalbaran(integer) RETURNS numeric(12, 2)
AS '
DECLARE
    idp ALIAS FOR $1;
    total numeric(12, 2);
    rs RECORD;

BEGIN
    total := 0;

    FOR rs IN SELECT cantlalbaran * pvplalbaran * (1 - descuentolalbaran / 100) * (ivalalbaran / 100) AS subtotal1 FROM lalbaran WHERE idalbaran = idp LOOP
    	total := total + rs.subtotal1;
    END LOOP;

    FOR rs IN SELECT proporciondalbaran FROM dalbaran WHERE idalbaran = idp LOOP
    	total := total * (1 - rs.proporciondalbaran / 100);
    END LOOP;

    RETURN total;
END;
' LANGUAGE plpgsql;


-- ========================== CONTROL DE STOCK ==============================


\echo -n ':: Funcion cambia stock por la parte de albaranes de cliente ... '
CREATE FUNCTION cambiastock() RETURNS "trigger"
AS '
DECLARE

BEGIN
    -- Hacemos el update del stock del articulo
    IF TG_OP = ''DELETE'' OR TG_OP = ''UPDATE'' THEN
	UPDATE articulo SET stockarticulo = stockarticulo + OLD.cantlalbaran WHERE idarticulo= OLD.idarticulo;
    END IF;

    IF TG_OP = ''INSERT'' OR TG_OP = ''UPDATE'' THEN
	UPDATE articulo SET stockarticulo = stockarticulo - NEW.cantlalbaran WHERE idarticulo= NEW.idarticulo;
    END IF;

    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador que disminuye stock al borrar o actualizar el detalle de un albaran de proveedor ... '
CREATE TRIGGER cambiastockt
    AFTER INSERT OR DELETE OR UPDATE ON lalbaran
    FOR EACH ROW
    EXECUTE PROCEDURE cambiastock();


\echo -n ':: Funcion cambia stock por la parte de albaranes de proveedor ... '
CREATE FUNCTION cambiastockp() RETURNS "trigger"
AS '
DECLARE

BEGIN
    -- Hacemos el update del stock del articulo
    IF TG_OP = ''DELETE'' OR TG_OP = ''UPDATE'' THEN
	UPDATE articulo SET stockarticulo = stockarticulo - OLD.cantlalbaranp WHERE idarticulo= OLD.idarticulo;
    END IF;

    IF TG_OP = ''INSERT'' OR TG_OP = ''UPDATE'' THEN
	UPDATE articulo SET stockarticulo = stockarticulo + NEW.cantlalbaranp WHERE idarticulo= NEW.idarticulo;
    END IF;

    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador que disminuye stock al borrar o actualizar el detalle de un albaran de proveedor ... '
CREATE TRIGGER cambiastockpt
    AFTER INSERT OR DELETE OR UPDATE ON lalbaranp
    FOR EACH ROW
    EXECUTE PROCEDURE cambiastockp();


\echo -n ':: Funcion que modifica el stock de un articulo ... '
CREATE OR REPLACE FUNCTION modificadostock() RETURNS "trigger"
AS '
DECLARE 
    cant numeric;
    rs RECORD;

BEGIN
    IF NEW.stockarticulo <> OLD.stockarticulo THEN
	cant := NEW.stockarticulo - OLD.stockarticulo;

	FOR rs IN SELECT * FROM comparticulo WHERE idarticulo = NEW.idarticulo LOOP
	    UPDATE articulo SET stockarticulo = stockarticulo + cant * rs.cantcomparticulo WHERE idarticulo = rs.idcomponente;
	END LOOP;
    END IF;

    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador que modifica el stock al actualizar los datos de un articulo ... '
CREATE TRIGGER modificastocktrigger
    AFTER UPDATE ON articulo
    FOR EACH ROW
    EXECUTE PROCEDURE modificadostock();


-- ========================== FUNCIONES AUXILIARES ==============================

\echo -n ':: Funcion que muestra cadena de texto sin acentos ... '
CREATE OR REPLACE FUNCTION sinacentos (text) RETURNS text AS $$
    SELECT translate($1,'áéíóúÁÉÍÓÚäëïöüÄËÏÖÜñÑ','aeiouAEIOUaeiouAEIOUnN');
$$ LANGUAGE sql;


\echo -n ':: '

CREATE OR REPLACE FUNCTION BgValidDate(CHAR) RETURNS date AS $$
 DECLARE
     result DATE;
      res BOOL;
     validFormat TEXT := 'DD/MM/YYYY';
 BEGIN
    result := NULL;
     SELECT TO_CHAR(TO_DATE($1,validFormat),validFormat) = $1
     INTO res;
     if (res = TRUE) THEN
   RETURN $1::DATE;
     END IF;
     RETURN result;
 END;
 $$ LANGUAGE plpgsql;



\echo -n ':: '
COMMIT;

\echo '********* FIN FICHERO DE ESTRUCTURA DE LA BASE DE DATOS DE BULMAFACT *********'

-- ================================== ACTUALIZACION  ===================================
-- =====================================================================================

-- Agregamos nuevos parametros de configuracion
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
    rs RECORD;

BEGIN
    SELECT INTO rs * FROM configuracion WHERE nombre = ''DatabaseRevision'';

    IF FOUND THEN
	UPDATE CONFIGURACION SET valor = ''0.13.1-0006'' WHERE nombre = ''DatabaseRevision'';
    ELSE
	INSERT INTO configuracion (nombre, valor) VALUES (''DatabaseRevision'', ''0.13.1-0006'');
    END IF;

    RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT actualizarevision();
DROP FUNCTION actualizarevision() CASCADE;
\echo "Actualizada la revision de la base de datos a la version 0.13.1"
