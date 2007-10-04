--
-- PostgreSQL database dump
--
BEGIN;

INSERT INTO configuracion (nombre, valor) VALUES ('CodCuenta', 'xxxxyyy');
INSERT INTO configuracion (nombre, valor) VALUES ('Amortizacion', 'Amortizacion');
INSERT INTO configuracion (nombre, valor) VALUES ('Cobro', 'Cobro');
INSERT INTO configuracion (nombre, valor) VALUES ('CIF', '--');
INSERT INTO configuracion (nombre, valor) VALUES ('TipoIva', '--');
INSERT INTO configuracion (nombre, valor) VALUES ('NombreVia', '--');
INSERT INTO configuracion (nombre, valor) VALUES ('NumeroVia', '--');
INSERT INTO configuracion (nombre, valor) VALUES ('Escalera', '--');
INSERT INTO configuracion (nombre, valor) VALUES ('Piso', '--');
INSERT INTO configuracion (nombre, valor) VALUES ('Puerta', '--');
INSERT INTO configuracion (nombre, valor) VALUES ('CodPostal', '--');
INSERT INTO configuracion (nombre, valor) VALUES ('Municipio', '--');
INSERT INTO configuracion (nombre, valor) VALUES ('Provincia', '--');
INSERT INTO configuracion (nombre, valor) VALUES ('Pais', '--');
INSERT INTO configuracion (nombre, valor) VALUES ('Tipo', 'BulmaCont');
INSERT INTO configuracion (nombre, valor) VALUES ('Ejercicio', '2007');
INSERT INTO configuracion (nombre, valor) VALUES ('NombreEmpresa', 'Sin Definir');
INSERT INTO configuracion (nombre, valor) VALUES ('RegistroEmitida', '472');
INSERT INTO configuracion (nombre, valor) VALUES ('RegistroSoportada', '477');
INSERT INTO configuracion (nombre, valor) VALUES ('CuentaRegularizacion', '129');
INSERT INTO configuracion (nombre, valor) VALUES ('CuentasIngresos', '60;62');
INSERT INTO configuracion (nombre, valor) VALUES ('CuentasGastos', '70');
INSERT INTO configuracion (nombre, valor) VALUES ('CuentasDerechos', '43;3');
INSERT INTO configuracion (nombre, valor) VALUES ('CuentasObligaciones', '40;41');
INSERT INTO configuracion (nombre, valor) VALUES ('Pago', 'Pago');

COMMIT;
