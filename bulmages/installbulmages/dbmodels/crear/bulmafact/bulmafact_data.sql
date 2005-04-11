INSERT INTO configuracion (nombre, valor) VALUES ('CodCuenta', 'xxxxyyy');
INSERT INTO configuracion (nombre, valor) VALUES ('CIF', '--');
INSERT INTO configuracion (nombre, valor) VALUES ('TipoVia', '--');
INSERT INTO configuracion (nombre, valor) VALUES ('NombreVia', '--');
INSERT INTO configuracion (nombre, valor) VALUES ('NumeroVia', '--');
INSERT INTO configuracion (nombre, valor) VALUES ('Escalera', '--');
INSERT INTO configuracion (nombre, valor) VALUES ('Piso', '--');
INSERT INTO configuracion (nombre, valor) VALUES ('Puerta', '--');
INSERT INTO configuracion (nombre, valor) VALUES ('CodPostal', '--');
INSERT INTO configuracion (nombre, valor) VALUES ('Municipio', '--');
INSERT INTO configuracion (nombre, valor) VALUES ('Provincia', '--');
INSERT INTO configuracion (nombre, valor) VALUES ('Pais', '--');
INSERT INTO configuracion (nombre, valor) VALUES ('Tipo', 'BulmaFact');
INSERT INTO configuracion (nombre, valor) VALUES ('ProgramaContabilidad', 'BulmaCont');
INSERT INTO configuracion (nombre, valor) VALUES ('NombreEmpresa', 'Sin Definir');
INSERT INTO configuracion (nombre, valor) VALUES ('AlmacenDefecto', '100');


INSERT INTO almacen (codigoalmacen, nomalmacen, diralmacen, poblalmacen, cpalmacen, telalmacen, faxalmacen) VALUES (100, 'La Botica del Abuelo', 'Camí Vell de Bunyola', 'Palma de Mallorca', '07009', '971434343', '971434344');
INSERT INTO almacen (codigoalmacen, nomalmacen, diralmacen, poblalmacen, cpalmacen, telalmacen, faxalmacen) VALUES (200, 'Colmado de Mateo', 'Joan Maura', 'Palma de Mallorca', '07005', '971467911', '971776776');
INSERT INTO almacen (codigoalmacen, nomalmacen, diralmacen, poblalmacen, cpalmacen, telalmacen, faxalmacen) VALUES (300, 'Almacén Norte', 'PolÃ­gono homogeneo', 'Alcudia', '07012', '971167911', '971111776');


INSERT INTO proveedor (nomproveedor, cifproveedor, cpproveedor) VALUES ('Tomeu', '45678098D', '07008');
INSERT INTO proveedor (nomproveedor, cifproveedor, cpproveedor) VALUES ('Cristina', '45679998D', '07008');
INSERT INTO proveedor (nomproveedor, cifproveedor, cpproveedor) VALUES ('Marco Aurelio', '45668998D', '07008');
INSERT INTO proveedor (nomproveedor, cifproveedor, cpproveedor) VALUES ('Torrijas', '45678995D', '07008');

INSERT INTO division (idproveedor, descdivision) VALUES (1, 'division 1 de mateu');
INSERT INTO division (idproveedor, descdivision) VALUES (1, 'division 2 de mateu');
INSERT INTO division (idproveedor, descdivision) VALUES (2, 'division 1 de cristina');
INSERT INTO division (idproveedor, descdivision) VALUES (2, 'division 2 de cristina');
INSERT INTO division (idproveedor, descdivision) VALUES (2, 'division 3 de cristina');
INSERT INTO division (idproveedor, descdivision) VALUES (4, 'division 1 de Torrijas');


INSERT INTO recargo (nomrecargo, tasarecargo) VALUES('Exento', 0);

INSERT INTO cliente (idrecargo, nomcliente, cifcliente, telcliente, poblcliente) VALUES(1, 'Pepma de la rioja', '39881444W', '971998877', 'Palma de Mallorca');
INSERT INTO cliente (idrecargo, nomcliente, cifcliente, telcliente, poblcliente) VALUES(1, 'Rene Merou', '43881555Z', '871998877', 'Campos');
INSERT INTO cliente (idrecargo, nomcliente, cifcliente, telcliente, poblcliente) VALUES(1, 'Dolores Barriga', '66881444E', '666998877', 'Muro');
INSERT INTO cliente (idrecargo, nomcliente, cifcliente, telcliente, poblcliente) VALUES(1, 'Pepe Infante', '55881444W', '9321998877', 'Barcelona');

INSERT INTO tipo_iva (desctipo_iva) VALUES('Exento');
INSERT INTO tipo_iva (desctipo_iva) VALUES('Normal');
INSERT INTO tasa_iva(idtipo_iva, porcentasa_iva, fechatasa_iva) VALUES(1, 0, '0001-01-01');
INSERT INTO tasa_iva(idtipo_iva, porcentasa_iva, fechatasa_iva) VALUES(2, 16.00, '0001-01-01');
INSERT INTO tasa_iva(idtipo_iva, porcentasa_iva, fechatasa_iva) VALUES(2, 17.00, '2005-01-01');

INSERT INTO marca (nommarca, urlmarca, comentmarca) VALUES('Nestlé', 'www.nestle.com', 'Productos Alimentícios');

INSERT INTO linea_prod (desclinea_prod, idmarca) VALUES('LÃ­nea de producciÃ³n 1', 1);
\echo "Hemos insertado las linea_prod"

INSERT INTO familia(codigofamilia, nombrefamilia, padrefamilia) VALUES ('CO', 'Comestibles', NULL);
INSERT INTO familia(codigofamilia, nombrefamilia, padrefamilia) VALUES ('DR', 'Drogueria', NULL);
INSERT INTO familia(codigofamilia, nombrefamilia, padrefamilia) VALUES ('VE', 'Vehiculos', NULL);
INSERT INTO familia(codigofamilia, nombrefamilia, padrefamilia) VALUES ('LA', 'Lacteos', 1);
INSERT INTO familia(codigofamilia, nombrefamilia, padrefamilia) VALUES ('CA', 'Carnicos', 1);
INSERT INTO familia(codigofamilia, nombrefamilia, padrefamilia) VALUES ('CH', 'Charcuteria', 1);
INSERT INTO familia(codigofamilia, nombrefamilia, padrefamilia) VALUES ('PE', 'Pescaderia', 1);
INSERT INTO familia(codigofamilia, nombrefamilia, padrefamilia) VALUES ('PA', 'Panaderia', 1);
INSERT INTO familia(codigofamilia, nombrefamilia, padrefamilia) VALUES ('FU', 'Fruteria', 1);
INSERT INTO familia(codigofamilia, nombrefamilia, padrefamilia) VALUES ('BE', 'Bebidas', 1);
INSERT INTO familia(codigofamilia, nombrefamilia, padrefamilia) VALUES ('IN', 'Infusiones', 1);
\echo "Hemos insertado las familias"

INSERT INTO articulo (codarticulo, nomarticulo, descarticulo, idtipo_iva, idlinea_prod, idfamilia) VALUES('1', 'Leche Entera', 'Brick leche entera 1 litro', 1, 1,4);
INSERT INTO articulo (codarticulo, nomarticulo, descarticulo, idtipo_iva, idlinea_prod, idfamilia) VALUES('2', 'Natillas', 'Pack dos envases de natillas de vainilla', 2, 1,4);
INSERT INTO articulo (codarticulo, nomarticulo, descarticulo, idtipo_iva, idlinea_prod, idfamilia) VALUES('3', 'Iogurt', 'Pack 4 unidades yogures sabores', 1, 1,5);
INSERT INTO articulo (codarticulo, nomarticulo, descarticulo, idtipo_iva, idlinea_prod, idfamilia) VALUES('4', 'Nestcafé', 'Bote 400 gramos café soluble natural', 1, 1,5);
\echo "Hemos insertado los articulos"

INSERT INTO pedido (numpedido, anopedido, fechapedido, descpedido, iddivision, idalmacen) VALUES('P-1', 2004, '25/10/2004', 'Pedido semanal de carne y pescado', 1, 1);
INSERT INTO pedido (numpedido, anopedido, fechapedido, descpedido, iddivision, idalmacen) VALUES('P-2', 2004, '30/10/2004', 'Pedido especial noche halloween', 2, 3);
INSERT INTO pedido (numpedido, anopedido, fechapedido, descpedido, iddivision, idalmacen) VALUES('P-3', 2004, '02/11/2004', 'Frutos secos de temporada', 3, 2);
INSERT INTO pedido (numpedido, anopedido, fechapedido, descpedido, iddivision, idalmacen) VALUES('P-4', 2004, '02/10/2004', 'Pedido semanal de lencería', 6, 1);

INSERT INTO lpedido (desclpedido, cantlpedido, pvdlpedido, prevlpedido, idpedido, idarticulo) VALUES (' ', 10, 1.2, '30/10/2004', 1, 1);
INSERT INTO lpedido (desclpedido, cantlpedido, pvdlpedido, prevlpedido, idpedido, idarticulo) VALUES (' ', 100, 2.23, '02/11/2004', 1, 2);
INSERT INTO lpedido (desclpedido, cantlpedido, pvdlpedido, prevlpedido, idpedido, idarticulo) VALUES (' ', 15, 3.34, '30/10/2004', 1, 3);
INSERT INTO lpedido (desclpedido, cantlpedido, pvdlpedido, prevlpedido, idpedido, idarticulo) VALUES (' ', 34, 11.20, '05/11/2004', 2, 1);
INSERT INTO lpedido (desclpedido, cantlpedido, pvdlpedido, prevlpedido, idpedido, idarticulo) VALUES (' ', 45, 2.55, '05/11/2004', 2, 3);
INSERT INTO lpedido (desclpedido, cantlpedido, pvdlpedido, prevlpedido, idpedido, idarticulo) VALUES (' ', 1, 4.28, '10/11/2004', 2, 4);
INSERT INTO lpedido (desclpedido, cantlpedido, pvdlpedido, prevlpedido, idpedido, idarticulo) VALUES (' ', 34, 1.7, '31/12/2004', 3, 2);
INSERT INTO lpedido (desclpedido, cantlpedido, pvdlpedido, prevlpedido, idpedido, idarticulo) VALUES (' ', 11, 1.3, '30/12/2004', 3, 3);
INSERT INTO lpedido (desclpedido, cantlpedido, pvdlpedido, prevlpedido, idpedido, idarticulo) VALUES (' ', 28, 2.2, '30/12/2004', 3, 4);
INSERT INTO lpedido (desclpedido, cantlpedido, pvdlpedido, prevlpedido, idpedido, idarticulo) VALUES (' ', 15, 3.2, '15/11/2004', 4, 1);
INSERT INTO lpedido (desclpedido, cantlpedido, pvdlpedido, prevlpedido, idpedido, idarticulo) VALUES (' ', 10, 8.59, '10/11/2004', 4, 2);
INSERT INTO lpedido (desclpedido, cantlpedido, pvdlpedido, prevlpedido, idpedido, idarticulo) VALUES (' ', 6, 6.15, '25/11/2004', 4, 3);

INSERT INTO alb_pro (anoalb_pro, ncompraalb_pro, nalbalb_pro, fcrealb_pro, frecepalb_pro, comentalb_pro, idalmacen) VALUES(2004, 1, '123', '01/12/2004', '30/11/2004', 'AlbarÃ¡n nÂº 123', 2);
INSERT INTO alb_pro (anoalb_pro, ncompraalb_pro, nalbalb_pro, fcrealb_pro, frecepalb_pro, comentalb_pro, idalmacen) VALUES(2004, 2, '2323', '15/11/2004', '10/11/2004', 'AlbarÃ¡n nÂº 2323', 1);
INSERT INTO alb_pro (anoalb_pro, ncompraalb_pro, nalbalb_pro, fcrealb_pro, frecepalb_pro, comentalb_pro, idalmacen) VALUES(2004, 3, '34', '23/11/2004', '23/11/2004', 'AlbarÃ¡n nÂº 34', 3);

INSERT INTO presupuesto (numpresupuesto, fpresupuesto, contactpresupuesto, telpresupuesto, vencpresupuesto, comentpresupuesto, idusuari, idcliente, idalmacen) VALUES (1, '30/11/2004', 'Primer Contacto', '971 32 32 78', '12/12/2004', 'Sin comentarios', 1, 1, 1);
INSERT INTO presupuesto (numpresupuesto, fpresupuesto, contactpresupuesto, telpresupuesto, vencpresupuesto, comentpresupuesto, idusuari, idcliente, idalmacen) VALUES (2, '29/11/2004', 'Segundo Contacto', '91 454 45 30', '13/12/2004', 'Sin comentarios', 1, 1, 1);
INSERT INTO presupuesto (numpresupuesto, fpresupuesto, contactpresupuesto, telpresupuesto, vencpresupuesto, comentpresupuesto, idusuari, idcliente, idalmacen) VALUES (3, '10/11/2004', 'Tercer Contacto', '971 29 06 29', '14/12/2004', 'Sin comentarios', 1, 2, 1);

INSERT INTO suministra (idarticulo, idproveedor) VALUES (1, 1);
INSERT INTO suministra (idarticulo, idproveedor) VALUES (2, 1);
INSERT INTO suministra (idarticulo, idproveedor) VALUES (3, 2);
INSERT INTO suministra (idarticulo, idproveedor) VALUES (4, 2);
INSERT INTO suministra (idarticulo, idproveedor) VALUES (2, 3);
INSERT INTO suministra (idarticulo, idproveedor) VALUES (3, 3);
INSERT INTO suministra (idarticulo, idproveedor) VALUES (4, 1);

INSERT INTO forma_pago (descforma_pago, dias1tforma_pago, descuentoforma_pago) VALUES ('Contado', 1, 5);
INSERT INTO forma_pago (descforma_pago, dias1tforma_pago, descuentoforma_pago) VALUES ('Pagare 30 días', 30, 0);
INSERT INTO forma_pago (descforma_pago, dias1tforma_pago, descuentoforma_pago) VALUES ('Pagare 60 días', 60, 0);
INSERT INTO forma_pago (descforma_pago, dias1tforma_pago, descuentoforma_pago) VALUES ('Talón 15 dias', 15, 1);

INSERT INTO prfp (idpresupuesto, idforma_pago) VALUES (1, 1);
INSERT INTO prfp (idpresupuesto, idforma_pago) VALUES (2, 2);
INSERT INTO prfp (idpresupuesto, idforma_pago) VALUES (3, 3);

INSERT INTO albaran (numalbaran, fechaalbaran, idusuario, idcliente, idforma_pago, idalmacen) VALUES (1, '01/01/2005', 1, 1, 1, 1);
INSERT INTO albaran (numalbaran, fechaalbaran, idusuario, idcliente, idforma_pago, idalmacen) VALUES (2, '02/01/2005', 1, 1, 2, 1);
INSERT INTO albaran (numalbaran, fechaalbaran, idusuario, idcliente, idforma_pago, idalmacen) VALUES (3, '03/01/2005', 1, 2, 3, 1);
INSERT INTO albaran (numalbaran, fechaalbaran, idusuario, idcliente, idforma_pago, idalmacen) VALUES (4, '01/01/2005', 1, 2, 4, 1);
INSERT INTO albaran (numalbaran, fechaalbaran, idusuario, idcliente, idforma_pago, idalmacen) VALUES (5, '02/01/2005', 1, 3, 2, 1);
INSERT INTO albaran (numalbaran, fechaalbaran, idusuario, idcliente, idforma_pago, idalmacen) VALUES (6, '03/01/2005', 1, 4, 1, 1);
