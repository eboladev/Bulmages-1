PERFORM dblink_connect('dbname=soncatiu');
PERFORM dblink_exec( 'SET DATESTYLE TO SQL,European');
CREATE OR REPLACE view proveedor AS SELECT * FROM dblink( 'dbname=soncatiu', 'SELECT idproveedor, nomproveedor, nomaltproveedor, cifproveedor, codproveedor, codicliproveedor, cbancproveedor, comentproveedor, dirproveedor, poblproveedor, cpproveedor, telproveedor, faxproveedor, emailproveedor, corpproveedor, urlproveedor, clavewebproveedor, inactivoproveedor, recargoeqproveedor, irpfproveedor, idforma_pago, regimenfiscalproveedor, idprovincia, idcuentaproveedor FROM proveedor') AS t1 ( idproveedor integer, nomproveedor varchar, nomaltproveedor varchar, cifproveedor varchar, codproveedor varchar, codicliproveedor varchar, cbancproveedor varchar, comentproveedor varchar, dirproveedor varchar, poblproveedor varchar, cpproveedor varchar, telproveedor varchar, faxproveedor varchar, emailproveedor varchar, corpproveedor varchar, urlproveedor varchar, clavewebproveedor varchar, inactivoproveedor character(1), recargoeqproveedor boolean, irpfproveedor numeric(12,2), idforma_pago integer, regimenfiscalproveedor varchar, idprovincia integer, idcuentaproveedor integer);


create or replace view provincia AS select * FROM dblink( 'dbname=soncatiu', 'SELECT idprovincia, idpais, provincia from provincia') as t1 (idprovincia integer, idpais integer, provincia varchar);


create or replace view pais AS select * from dblink('dbname=soncatiu',  'SELECT idpais, cod2pais, cod3pais, descpais from pais') as t1 (idpais integer, cod2pais varchar, cod3pais varchar, descpais varchar);


create or replace view forma_pago AS select * from dblink('dbname=soncatiu',  'SELECT idforma_pago, descforma_pago, dias1tforma_pago, descuentoforma_pago from forma_pago') as t1 (idforma_pago integer, descforma_pago varchar, dias1tforma_pago integer, descuentoforma_pago numeric(12,2));

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


NOTICE:  eliminando además restricción «suministra_idproveedor_fkey» en tabla suministra
NOTICE:  eliminando además restricción «pedidoproveedor_idproveedor_fkey» en tabla pedidoproveedor
NOTICE:  eliminando además restricción «pago_idproveedor_fkey» en tabla pago
NOTICE:  eliminando además restricción «facturap_idproveedor_fkey» en tabla facturap
NOTICE:  eliminando además restricción «division_idproveedor_fkey» en tabla division
NOTICE:  eliminando además restricción «albaranp_idproveedor_fkey» en tabla albaranp
ALTER TABLE albaranp ADD CONSTRAINT albaranp_idproveedor_fkey CHECK (idproveedor IN (SELECT idproveedor FROM proveedor));


CREATE OR REPLACE RULE proveedor_crule AS ON INSERT TO proveedor
    DO INSTEAD
    select dblink_exec(
	'dbname=soncatiu', 
	dblink_current_query()
	);

CREATE OR REPLACE RULE proveedor_urule AS ON UPDATE TO proveedor
    DO INSTEAD
    select dblink_exec(
	'dbname=soncatiu', 
	dblink_current_query()
	);


CREATE OR REPLACE RULE proveedor_drule AS ON DELETE TO proveedor
    DO INSTEAD
    select dblink_exec(
	'dbname=soncatiu', 
	dblink_current_query()
	);



CREATE OR REPLACE RULE provincia_crule AS ON INSERT TO provincia
    DO INSTEAD
    select dblink_exec(
	'dbname=soncatiu', 
	dblink_current_query()
	);

CREATE OR REPLACE RULE provincia_urule AS ON UPDATE TO provincia
    DO INSTEAD
    select dblink_exec(
	'dbname=soncatiu', 
	dblink_current_query()
	);


CREATE OR REPLACE RULE provincia_drule AS ON DELETE TO provincia
    DO INSTEAD
    select dblink_exec(
	'dbname=soncatiu', 
	dblink_current_query()
	);

CREATE OR REPLACE RULE pais_crule AS ON INSERT TO pais
    DO INSTEAD
    select dblink_exec(
	'dbname=soncatiu', 
	dblink_current_query()
	);

CREATE OR REPLACE RULE pais_urule AS ON UPDATE TO pais
    DO INSTEAD
    select dblink_exec(
	'dbname=soncatiu', 
	dblink_current_query()
	);


CREATE OR REPLACE RULE pais_drule AS ON DELETE TO pais
    DO INSTEAD
    select dblink_exec(
	'dbname=soncatiu', 
	dblink_current_query()
	);



CREATE OR REPLACE RULE forma_pago_crule AS ON INSERT TO forma_pago
    DO INSTEAD
    select dblink_exec(
	'dbname=soncatiu', 
	dblink_current_query()
	);

CREATE OR REPLACE RULE forma_pago_urule AS ON UPDATE TO forma_pago
    DO INSTEAD
    select dblink_exec(
	'dbname=soncatiu', 
	dblink_current_query()
	);


CREATE OR REPLACE RULE forma_pago_drule AS ON DELETE TO forma_pago
    DO INSTEAD
    select dblink_exec(
	'dbname=soncatiu', 
	dblink_current_query()
	);

