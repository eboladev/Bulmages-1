
INSERT INTO division (descdivision) VALUES ('una division');
INSERT INTO proveedor (iddivision, nomproveedor, cifproveedor, cpproveedor) VALUES (1,'Tomeu', '45678098D', '07008');
INSERT INTO proveedor (iddivision, nomproveedor, cifproveedor, cpproveedor) VALUES (1,'Cristina', '45679998D', '07008');
INSERT INTO proveedor (iddivision, nomproveedor, cifproveedor, cpproveedor) VALUES (1,'Marco Aurelio', '45668998D', '07008');
INSERT INTO proveedor (iddivision, nomproveedor, cifproveedor, cpproveedor) VALUES (1,'Torrijas', '45678995D', '07008');


INSERT INTO recargo (nomrecargo, tasarecargo) VALUES('Exento', 0);

INSERT INTO cliente (idrecargo, nomcliente, cifcliente, telcliente, poblcliente) VALUES(1, 'Pepma de la rioja', '39881444W', '971998877', 'Palma de Mallorca');
INSERT INTO cliente (idrecargo, nomcliente, cifcliente, telcliente, poblcliente) VALUES(1, 'Rene Merou', '43881555Z', '871998877', 'Campos');
INSERT INTO cliente (idrecargo, nomcliente, cifcliente, telcliente, poblcliente) VALUES(1, 'Dolores Fuertes de Barriga', '66881444E', '666998877', 'Muro');
INSERT INTO cliente (idrecargo, nomcliente, cifcliente, telcliente, poblcliente) VALUES(1, 'Pepe Gotera', '55881444W', '9321998877', 'Barcelona');

INSERT INTO tipo_iva (desctipo_iva, tasatipo_iva) VALUES('Exento', 0);

INSERT INTO marca (nommarca, urlmarca, comentmarca) VALUES('Nestlé', 'www.nestle.com', 'Productos Alimentícios');

INSERT INTO linea_prod (desclinea_prod, idmarca) VALUES('Línea de producción 1', 1);

INSERT INTO articulo (codarticulo, nomarticulo, descarticulo, idtipo_iva, idlinea_prod) VALUES('1', 'Leche Entera', 'Brick leche entera 1 litro', 1, 1);
INSERT INTO articulo (codarticulo, nomarticulo, descarticulo, idtipo_iva, idlinea_prod) VALUES('2', 'Natillas', 'Pack dos envases de natillas de vainilla', 1, 1);
INSERT INTO articulo (codarticulo, nomarticulo, descarticulo, idtipo_iva, idlinea_prod) VALUES('3', 'Yogurt', 'Pack 4 unidades yogures sabores', 1, 1);
INSERT INTO articulo (codarticulo, nomarticulo, descarticulo, idtipo_iva, idlinea_prod) VALUES('4', 'Nestcafé', 'Bote 400 gramos café soluble natural', 1, 1);
