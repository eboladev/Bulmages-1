--
-- PostgreSQL database dump
--

SET client_encoding = 'SQL_ASCII';
SET check_function_bodies = false;

SET search_path = public, pg_catalog;

--
-- Data for TOC entry 3 (OID 3839850)
-- Name: borrador; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO borrador ( codigoborrador, idasiento, iddiario, fecha, conceptocontable, idcuenta, descripcion, debe, haber, contrapartida, comentario, idcanal, marcaconciliacion, idc_coste, idapunte, idtipoiva, orden) VALUES ( NULL, 1, NULL, '2005-01-20 00:00:00+01', 'un concepto', id_cuenta('4720016'), NULL, 16, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 2);
INSERT INTO borrador ( codigoborrador, idasiento, iddiario, fecha, conceptocontable, idcuenta, descripcion, debe, haber, contrapartida, comentario, idcanal, marcaconciliacion, idc_coste, idapunte, idtipoiva, orden) VALUES ( NULL, 1, NULL, '2005-01-20 00:00:00+01', 'un concepto', id_cuenta('4300001'), NULL, 0, 116, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 1);
INSERT INTO borrador ( codigoborrador, idasiento, iddiario, fecha, conceptocontable, idcuenta, descripcion, debe, haber, contrapartida, comentario, idcanal, marcaconciliacion, idc_coste, idapunte, idtipoiva, orden) VALUES ( NULL, 1, NULL, '2005-01-20 00:00:00+01', 'un concepto', id_cuenta('7000000'), NULL, 100, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0);

SELECT cierraasiento(1);

--
-- TOC entry 2 (OID 3839848)
-- Name: borrador_idborrador_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('borrador_idborrador_seq', 3, true);


