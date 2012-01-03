-- -------------------------------------------------------------------------------------------
-- (C) Joan Miquel Torres Rigo & Tomeu Borras Riera & Mateu Borras Marco, Agosto 2007
-- joanmi@bulma.net, tborras@conetxia.com mborras@conetxia.com
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
-- BULMAFACT. Base de datos de ejemplo.
-- -------------------------------------------------------------------------------------------

\echo '********* INICIADO FICHERO DE DATOS DE EJEMPLO PARA BULMAFACT *********'

\echo -n ':: '
SET DATESTYLE TO European;

\echo -n ':: Forma de pago ... '
CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
    rs RECORD;
BEGIN
    SELECT INTO rs * FROM forma_pago WHERE descforma_pago = ''Contado'';
    IF NOT FOUND THEN
	INSERT INTO forma_pago (descforma_pago, dias1tforma_pago, descuentoforma_pago) VALUES (''Contado'', 1, 5);
    END IF;

    SELECT INTO rs * FROM forma_pago WHERE descforma_pago = ''Pagaré 30 días'';
    IF NOT FOUND THEN
	INSERT INTO forma_pago (descforma_pago, dias1tforma_pago, descuentoforma_pago) VALUES (''Pagare 30 días'', 30, 0);
    END IF;

    SELECT INTO rs * FROM forma_pago WHERE descforma_pago = ''Pagaré 60 días'';
    IF NOT FOUND THEN
	INSERT INTO forma_pago (descforma_pago, dias1tforma_pago, descuentoforma_pago) VALUES (''Pagare 60 días'', 60, 0);
    END IF;

    SELECT INTO rs * FROM forma_pago WHERE descforma_pago = ''Talón 15 días'';
    IF NOT FOUND THEN
	INSERT INTO forma_pago (descforma_pago, dias1tforma_pago, descuentoforma_pago) VALUES (''Talón 15 días'', 15, 0);
    END IF;
    
    RETURN 0;
END;
' LANGUAGE plpgsql;    
    
BEGIN;
\echo -n ':: Forma de pago ... '
SELECT aux();
COMMIT;

\echo -n ':: Almacen ... '
BEGIN;
INSERT INTO almacen (codigoalmacen, nomalmacen, diralmacen, poblalmacen, cpalmacen, telalmacen, faxalmacen) VALUES (100, 'La Botica del Abuelo', 'Camí Vell de Bunyola', 'Palma de Mallorca', '07009', '971434343', '971434344');
INSERT INTO almacen (codigoalmacen, nomalmacen, diralmacen, poblalmacen, cpalmacen, telalmacen, faxalmacen) VALUES (200, 'Colmado de Mateo', 'Joan Maura', 'Palma de Mallorca', '07005', '971467911', '971776776');
INSERT INTO almacen (codigoalmacen, nomalmacen, diralmacen, poblalmacen, cpalmacen, telalmacen, faxalmacen) VALUES (300, 'Almacén Norte', 'Políono homogeneo', 'Alcudia', '07012', '971167911', '971111776');
\echo -n ':: Almacen ... '
COMMIT;

\echo -n ':: Proveedor ... '
BEGIN;
INSERT INTO proveedor (nomproveedor, cifproveedor, cpproveedor) VALUES ('Tomeu', '45678098D', '07008');
INSERT INTO proveedor (nomproveedor, cifproveedor, cpproveedor) VALUES ('Cristina', '45679998D', '07008');
INSERT INTO proveedor (nomproveedor, cifproveedor, cpproveedor) VALUES ('Marco Aurelio', '45668998D', '07008');
INSERT INTO proveedor (nomproveedor, cifproveedor, cpproveedor) VALUES ('Torrijas', '45678995D', '07008');
\echo -n ':: Proveedor ... '
COMMIT;

\echo -n ':: Cliente ... '
BEGIN;
INSERT INTO cliente (nomcliente, cifcliente, telcliente, poblcliente) VALUES('Jorge Gutiérrez', '39881444W', '971998877', 'Palma de Mallorca');
INSERT INTO cliente (nomcliente, cifcliente, telcliente, poblcliente) VALUES('Carlos Santana', '43881555Z', '871998877', 'Campos');
INSERT INTO cliente (nomcliente, cifcliente, telcliente, poblcliente) VALUES('Dolores Martínez', '66881444E', '666998877', 'Muro');
INSERT INTO cliente (nomcliente, cifcliente, telcliente, poblcliente) VALUES('Pepe Infante', '55881444W', '9321998877', 'Barcelona');
\echo -n ':: Cliente ... '
COMMIT;

\echo -n ':: Tipo de IVA ... '
BEGIN;
INSERT INTO tipo_iva (desctipo_iva) VALUES('Exento');
INSERT INTO tipo_iva (desctipo_iva) VALUES('Normal');
\echo -n ':: Tipo de IVA ... '
COMMIT;

\echo -n ':: Tasa de IVA ... '
BEGIN;
INSERT INTO tasa_iva (idtipo_iva, porcentasa_iva, fechatasa_iva) VALUES(1, 0, '0001-01-01');
INSERT INTO tasa_iva (idtipo_iva, porcentasa_iva, fechatasa_iva) VALUES(2, 16.00, '0001-01-01');
\echo -n ':: Tasa de IVA ... '
COMMIT;

\echo -n ':: Familia ... '
BEGIN;
INSERT INTO familia (codigofamilia, nombrefamilia, padrefamilia) VALUES ('CO', 'Comestibles', NULL);
INSERT INTO familia (codigofamilia, nombrefamilia, padrefamilia) VALUES ('DR', 'Drogueria', NULL);
INSERT INTO familia (codigofamilia, nombrefamilia, padrefamilia) VALUES ('VE', 'Vehiculos', NULL);
INSERT INTO familia (codigofamilia, nombrefamilia, padrefamilia) VALUES ('LA', 'Lacteos', 1);
INSERT INTO familia (codigofamilia, nombrefamilia, padrefamilia) VALUES ('CA', 'Carnicos', 1);
INSERT INTO familia (codigofamilia, nombrefamilia, padrefamilia) VALUES ('CH', 'Charcuteria', 1);
INSERT INTO familia (codigofamilia, nombrefamilia, padrefamilia) VALUES ('PE', 'Pescaderia', 1);
INSERT INTO familia (codigofamilia, nombrefamilia, padrefamilia) VALUES ('PA', 'Panaderia', 1);
INSERT INTO familia (codigofamilia, nombrefamilia, padrefamilia) VALUES ('FU', 'Fruteria', 1);
INSERT INTO familia (codigofamilia, nombrefamilia, padrefamilia) VALUES ('BE', 'Bebidas', 1);
INSERT INTO familia (codigofamilia, nombrefamilia, padrefamilia) VALUES ('IN', 'Infusiones', 1);
\echo -n ':: Familia ... '
COMMIT;

\echo -n ':: Articulo ... '
BEGIN;
INSERT INTO articulo (codarticulo, nomarticulo, obserarticulo, idtipo_iva, idfamilia) VALUES('1', 'Leche Entera', 'Brick leche entera 1 litro', 1, 4);
INSERT INTO articulo (codarticulo, nomarticulo, obserarticulo, idtipo_iva, idfamilia) VALUES('2', 'Natillas', 'Pack dos envases de natillas de vainilla', 2, 4);
INSERT INTO articulo (codarticulo, nomarticulo, obserarticulo, idtipo_iva, idfamilia) VALUES('3', 'Iogurt', 'Pack 4 unidades yogures sabores', 1, 5);
INSERT INTO articulo (codarticulo, nomarticulo, obserarticulo, idtipo_iva, idfamilia) VALUES('4', 'Nestcafé', 'Bote 400 gramos café soluble natural', 1, 5);
\echo -n ':: Articulo ... '
COMMIT;

\echo -n ':: Pedido de cliente ... '
BEGIN;
INSERT INTO pedidocliente (numpedidocliente, fechapedidocliente, descpedidocliente, idcliente, idalmacen) VALUES(1, '25/10/2007', 'Pedido  carne y pescado', 1, 1);
INSERT INTO pedidocliente (numpedidocliente, fechapedidocliente, descpedidocliente, idcliente, idalmacen) VALUES(2, '30/10/2007', 'Pedido especial noche ', 2, 3);
INSERT INTO pedidocliente (numpedidocliente, fechapedidocliente, descpedidocliente, idcliente, idalmacen) VALUES(3, '02/11/2007', 'Frutos de temporada', 1, 2);
INSERT INTO pedidocliente (numpedidocliente, fechapedidocliente, descpedidocliente, idcliente, idalmacen) VALUES(4, '02/10/2007', 'Pedido de lencería', 2, 1);
\echo -n ':: Pedido de cliente ... '
COMMIT;

\echo -n ':: Presupuesto ... '
BEGIN;
INSERT INTO presupuesto (numpresupuesto, fpresupuesto, contactpresupuesto, telpresupuesto, vencpresupuesto, comentpresupuesto, idusuari, idcliente, idalmacen, idforma_pago) VALUES (1, '30/11/2007', 'Primer Contacto', '971 32 32 78', '12/12/2007', 'Sin comentarios', 1, 1, 1,1);
INSERT INTO presupuesto (numpresupuesto, fpresupuesto, contactpresupuesto, telpresupuesto, vencpresupuesto, comentpresupuesto, idusuari, idcliente, idalmacen, idforma_pago) VALUES (2, '29/11/2007', 'Segundo Contacto', '91 454 45 30', '13/12/2007', 'Sin comentarios', 1, 1, 1,1);
INSERT INTO presupuesto (numpresupuesto, fpresupuesto, contactpresupuesto, telpresupuesto, vencpresupuesto, comentpresupuesto, idusuari, idcliente, idalmacen, idforma_pago) VALUES (3, '10/11/2007', 'Tercer Contacto', '971 29 06 29', '14/12/2007', 'Sin comentarios', 1, 2, 1,1);
\echo -n ':: Presupuesto ... '
COMMIT;

\echo -n ':: Serie de factura ... '
BEGIN;
INSERT INTO serie_factura (codigoserie_factura, descserie_factura) VALUES ('DD', 'Serie de ejemplo');
INSERT INTO serie_factura (codigoserie_factura, descserie_factura) VALUES ('REC', 'Facturas Rectificativas');
\echo -n ':: Serie de factura ... '
COMMIT;

\echo -n ':: Factura a clientes ... '
BEGIN;
INSERT INTO factura (codigoserie_factura, numfactura, ffactura, contactfactura, telfactura, comentfactura, idusuari, idcliente, idalmacen) VALUES ('DD', 1, '30/11/2007', 'Primer Contacto', '971 32 32 78', 'Sin comentarios', 1, 1, 1);
INSERT INTO factura (codigoserie_factura,numfactura, ffactura, contactfactura, telfactura, comentfactura, idusuari, idcliente, idalmacen) VALUES ('DD', 2, '29/11/2007', 'Segundo Contacto', '91 454 45 30', 'Sin comentarios', 1, 1, 1);
INSERT INTO factura (codigoserie_factura,numfactura, ffactura, contactfactura, telfactura, comentfactura, idusuari, idcliente, idalmacen) VALUES ('REC', 3, '10/11/2007', 'Tercer Contacto', '971 29 06 29',  'Sin comentarios', 1, 2, 1);
\echo -n ':: Factura a clientes ... '
COMMIT;

\echo -n ':: Factura de proveedor ... '
BEGIN;
INSERT INTO facturap (numfacturap, ffacturap, contactfacturap, telfacturap, comentfacturap, idusuari, idproveedor) VALUES (1, '30/11/2007', 'Primer Contacto', '971 32 32 78', 'Sin comentarios', 1, 1);
INSERT INTO facturap (numfacturap, ffacturap, contactfacturap, telfacturap, comentfacturap, idusuari, idproveedor) VALUES (2, '29/11/2007', 'Segundo Contacto', '91 454 45 30', 'Sin comentarios', 1, 1);
INSERT INTO facturap (numfacturap, ffacturap, contactfacturap, telfacturap, comentfacturap, idusuari, idproveedor) VALUES (3, '10/11/2007', 'Tercer Contacto', '971 29 06 29',  'Sin comentarios', 1, 2);
\echo -n ':: Factura de proveedor ... '
COMMIT;

\echo -n ':: Albaran a clientes ... '
BEGIN;
INSERT INTO albaran (numalbaran, fechaalbaran, idtrabajador, idcliente, idforma_pago, idalmacen, refalbaran) VALUES (1, '01/01/2005', NULL, 1, 1, 1,'R1');
INSERT INTO albaran (numalbaran, fechaalbaran, idtrabajador, idcliente, idforma_pago, idalmacen, refalbaran) VALUES (2, '02/01/2005', NULL, 1, 2, 1,'R2');
INSERT INTO albaran (numalbaran, fechaalbaran, idtrabajador, idcliente, idforma_pago, idalmacen, refalbaran) VALUES (3, '03/01/2005', NULL, 2, 3, 1,'R3');
INSERT INTO albaran (numalbaran, fechaalbaran, idtrabajador, idcliente, idforma_pago, idalmacen, refalbaran) VALUES (4, '01/01/2005', NULL, 2, 4, 1,'R4');
INSERT INTO albaran (numalbaran, fechaalbaran, idtrabajador, idcliente, idforma_pago, idalmacen, refalbaran) VALUES (5, '02/01/2005', NULL, 3, 2, 1,'R5');
INSERT INTO albaran (numalbaran, fechaalbaran, idtrabajador, idcliente, idforma_pago, idalmacen, refalbaran) VALUES (6, '03/01/2005', NULL, 4, 1, 1,'R6');
\echo -n ':: Albaran a clientes ... '
COMMIT;

\echo -n ':: Albaran de proveedor ... '
BEGIN;
INSERT INTO albaranp (numalbaranp, fechaalbaranp, idtrabajador, idproveedor, idforma_pago, idalmacen, refalbaranp) VALUES (1, '01/01/2005', NULL, 1, 1, 1,'RP1');
INSERT INTO albaranp (numalbaranp, fechaalbaranp, idtrabajador, idproveedor, idforma_pago, idalmacen, refalbaranp) VALUES (2, '02/01/2005', NULL, 1, 2, 1,'RP2');
INSERT INTO albaranp (numalbaranp, fechaalbaranp, idtrabajador, idproveedor, idforma_pago, idalmacen, refalbaranp) VALUES (3, '03/01/2005', NULL, 2, 3, 1,'RP3');
INSERT INTO albaranp (numalbaranp, fechaalbaranp, idtrabajador, idproveedor, idforma_pago, idalmacen, refalbaranp) VALUES (4, '01/01/2005', NULL, 2, 4, 1,'RP4');
INSERT INTO albaranp (numalbaranp, fechaalbaranp, idtrabajador, idproveedor, idforma_pago, idalmacen, refalbaranp) VALUES (5, '02/01/2005', NULL, 3, 2, 1,'RP5');
INSERT INTO albaranp (numalbaranp, fechaalbaranp, idtrabajador, idproveedor, idforma_pago, idalmacen, refalbaranp) VALUES (6, '03/01/2005', NULL, 4, 1, 1,'RP6');
\echo -n ':: Albaran de proveedor ... '
COMMIT;

\echo '********* FIN FICHERO DE DATOS DE EJEMPLO PARA BULMAFACT *********'
