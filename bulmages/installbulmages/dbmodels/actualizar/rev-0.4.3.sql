CREATE TABLE adocumental (
    idadocumental serial PRIMARY KEY,
    idasiento integer REFERENCES asiento(idasiento),
    descripcionadocumental character varying(200),
    fechaintadocumental timestamp with time zone,
    fechaasadocumental timestamp with time zone,
    archivoadocumental character varying(300)
);



ALTER TABLE ONLY registroiva
	ADD CONSTRAINT registroiva_pkey PRIMARY KEY (idregistroiva);


-- La tabla prevcobro es prevision de cobros / pagos para facturas.
-- Esta tabla proporciona las formas de pago de cada factura que se pasa a un cliente o que recibimos de un proveedor.

-- tipoprevcobro indica si es un cobro (true) o un pago (false).
-- fpagoprevcobro es un identificador a la otra tabla de Formas de Pago.
-- idcuenta La cuenta sobre la que se hará el cobro / pago.
-- idasiento Asiento de cobro (Si está hecho).
-- cantidadprevistaprevcobro Cantidad Prevista del cobro.
-- cantidadprevcobro         Cantidad Cobrada.
-- fprevistaprevcobro        Fecha en que se prevee el cobro / pago.
-- fcobroprevcobro           Fecha en que se ha realizado el cobro / pago.
-- idregistroiva             Registro de IVA con el que se corresponde, o factura con la que se corresponde.
-- tipoprevcobro             Indica si es un cobro o un pago.


CREATE TABLE prevcobro (
    idprevcobro serial PRIMARY KEY,
    fprevistaprevcobro date,
    fcobroprevcobro date,
    fpagoprevcobro integer,
    idcuenta integer REFERENCES cuenta(idcuenta),
    idasiento integer REFERENCES asiento(idasiento),
    cantidadprevistaprevcobro double precision,
    cantidadprevcobro         double precision,
    idregistroiva integer NOT NULL REFERENCES registroiva(idregistroiva),
    tipoprevcobro boolean
);
