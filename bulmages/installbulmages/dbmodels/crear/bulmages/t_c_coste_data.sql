--
-- PostgreSQL database dump
--

SET client_encoding = 'UNICODE';
SET check_function_bodies = false;

SET search_path = public, pg_catalog;

--
-- Data for TOC entry 3 (OID 3768326)
-- Name: c_coste; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO c_coste (idc_coste, descripcion, nombre, codigo, padre, imputacion, debe, haber) VALUES (10, 'Conetxia.', 'Son Oliva', NULL, NULL, NULL, NULL, NULL);

--
-- TOC entry 2 (OID 3768324)
-- Name: c_coste_idc_coste_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('c_coste_idc_coste_seq', 100, false);


