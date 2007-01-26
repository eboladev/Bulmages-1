BEGIN;

INSERT INTO tipoiva (nombretipoiva, porcentajetipoiva, idcuenta) VALUES ('Soportado 16%',   16, id_cuenta('4720016'));
INSERT INTO tipoiva (nombretipoiva, porcentajetipoiva, idcuenta) VALUES ('Soportado 7%',     7, id_cuenta('4720007'));
INSERT INTO tipoiva (nombretipoiva, porcentajetipoiva, idcuenta) VALUES ('Soportado 4%',     4, id_cuenta('4720004'));
INSERT INTO tipoiva (nombretipoiva, porcentajetipoiva, idcuenta) VALUES ('Repercutido 16%', 16, id_cuenta('4770016'));
INSERT INTO tipoiva (nombretipoiva, porcentajetipoiva, idcuenta) VALUES ('Repercutido 7%', 7, id_cuenta('4770007'));
INSERT INTO tipoiva (nombretipoiva, porcentajetipoiva, idcuenta) VALUES ('Repercutido 4%', 4, id_cuenta('4770004'));

COMMIT;