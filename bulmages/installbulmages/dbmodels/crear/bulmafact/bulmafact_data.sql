-- En este script sólo debe haber aquellas instrucciones que meten datos en la base de datos necesarios para arrancar una base de datos de bulmafact por primera vez.

SET DATESTYLE TO European;

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
