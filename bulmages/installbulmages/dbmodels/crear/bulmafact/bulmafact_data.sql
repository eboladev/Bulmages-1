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
-- BULMAFACT. En este script solo debe haber aquellas instrucciones que meten datos en la base
-- de datos necesarios para arrancar una base de datos de bulmafact por primera vez.
-- -------------------------------------------------------------------------------------------

\echo '********* INICIADO FICHERO DE DATOS MINIMO PARA BULMAFACT *********'

\echo -n ':: '
SET DATESTYLE TO European;

\echo -n ':: Configuracion ... '
BEGIN;
INSERT INTO configuracion (nombre, valor) VALUES ('NombreEmpresa', 'Sin Definir');
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
INSERT INTO configuracion (nombre, valor) VALUES ('AlmacenDefecto', '**CAMBIAME**');
INSERT INTO configuracion (nombre, valor) VALUES ('DireccionCompleta', 'Sin definir');
INSERT INTO configuracion (nombre, valor) VALUES ('Ciudad', 'Sin definir');
INSERT INTO configuracion (nombre, valor) VALUES ('CodArticuloGenerico', 'Sin Definir');
INSERT INTO configuracion (nombre, valor) VALUES ('SerieFacturaDefecto', '**CAMBIAME**');
INSERT INTO configuracion (nombre, valor) VALUES ('CodCuenta', 'xxxxyyy');
\echo -n ':: Configuraciones ... '
COMMIT;

\echo '********* FIN FICHERO DE DATOS MINIMO PARA BULMAFACT *********'
