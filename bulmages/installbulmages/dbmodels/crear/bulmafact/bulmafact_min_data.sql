-- -------------------------------------------------------------------------------------------
-- (C) Joan Miquel Torres Rigo & Tomeu Borras Riera & Mateu Borras Marco, Agosto 2007
-- joanmi@bulma.net, tborras@conetxia.com mborras@conetxia.com
-- (C) 2007 Fco. Javier M. C. <fcojavmc@todo-redes.com>
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
-- BULMAFACT. Base de datos minima para BulmaFact.
-- -------------------------------------------------------------------------------------------

\echo '********* INICIADO FICHERO DE DATOS MINIMOS PARA BULMAFACT *********'

\echo -n ':: '
SET DATESTYLE TO European;

\echo -n ':: Forma de pago ... '
BEGIN;
INSERT INTO forma_pago (descforma_pago, dias1tforma_pago, descuentoforma_pago) VALUES ('Contado', 1, 5);
INSERT INTO forma_pago (descforma_pago, dias1tforma_pago, descuentoforma_pago) VALUES ('Pagare 30 días', 30, 0);
INSERT INTO forma_pago (descforma_pago, dias1tforma_pago, descuentoforma_pago) VALUES ('Pagare 60 días', 60, 0);
INSERT INTO forma_pago (descforma_pago, dias1tforma_pago, descuentoforma_pago) VALUES ('Talón 15 dias', 15, 1);
\echo -n ':: Forma de pago ... '
COMMIT;

\echo -n ':: Almacen ... '
BEGIN;
INSERT INTO almacen (codigoalmacen, nomalmacen, diralmacen, poblalmacen, cpalmacen, telalmacen, faxalmacen) VALUES (100, '(Por defecto)', '', '', '', '', '');
\echo -n ':: Almacen ... '
COMMIT;

\echo -n ':: Tipo de IVA ... '
BEGIN;
INSERT INTO tipo_iva (desctipo_iva) VALUES('16%');
INSERT INTO tipo_iva (desctipo_iva) VALUES('7%');
INSERT INTO tipo_iva (desctipo_iva) VALUES('4%');
INSERT INTO tipo_iva (desctipo_iva) VALUES('Exento 0%');
\echo -n ':: Tipo de IVA ... '
COMMIT;

\echo -n ':: Tasa de IVA ... '
BEGIN;
INSERT INTO tasa_iva (idtipo_iva, porcentasa_iva, fechatasa_iva) VALUES(1, 16.00, '0001-01-01');
INSERT INTO tasa_iva (idtipo_iva, porcentasa_iva, fechatasa_iva) VALUES(2, 7.00, '0001-01-01');
INSERT INTO tasa_iva (idtipo_iva, porcentasa_iva, fechatasa_iva) VALUES(3, 4.00, '0001-01-01');
INSERT INTO tasa_iva (idtipo_iva, porcentasa_iva, fechatasa_iva) VALUES(4, 0.00, '0001-01-01');
\echo -n ':: Tasa de IVA ... '
COMMIT;

\echo -n ':: Familia ... '
BEGIN;
INSERT INTO familia (codigofamilia, nombrefamilia, padrefamilia) VALUES ('0', '0', NULL);
\echo -n ':: Familia ... '
COMMIT;

\echo -n ':: Serie de factura ... '
BEGIN;
INSERT INTO serie_factura (codigoserie_factura, descserie_factura) VALUES ('SA', 'Serie A');
INSERT INTO serie_factura (codigoserie_factura, descserie_factura) VALUES ('SB', 'Serie B');
INSERT INTO serie_factura (codigoserie_factura, descserie_factura) VALUES ('REC', 'Facturas Rectificativas');
\echo -n ':: Serie de factura ... '
COMMIT;

\echo '********* FIN FICHERO DE DATOS MINIMOS PARA BULMAFACT *********'
