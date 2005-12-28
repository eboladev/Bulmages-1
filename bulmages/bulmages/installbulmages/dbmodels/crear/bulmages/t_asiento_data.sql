--
-- PostgreSQL database dump
--

SET client_encoding = 'UNICODE';
SET check_function_bodies = false;

SET search_path = public, pg_catalog;

--
-- Data for TOC entry 3 (OID 3839794)
-- Name: asiento; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO asiento (idasiento, descripcion, fecha, comentariosasiento, ordenasiento, clase) VALUES (1, '20/01/2005', '2005-01-20 00:00:00+01', NULL, 1, 1);


--
-- TOC entry 2 (OID 3839792)
-- Name: asiento_idasiento_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('asiento_idasiento_seq', 100, false);


