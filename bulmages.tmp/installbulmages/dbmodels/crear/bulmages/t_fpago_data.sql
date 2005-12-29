--
-- PostgreSQL database dump
--

SET client_encoding = 'UNICODE';
SET check_function_bodies = false;

SET search_path = public, pg_catalog;

--
-- Data for TOC entry 3 (OID 3347651)
-- Name: fpago; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO fpago (idfpago, nomfpago, nplazosfpago, plazoprimerpagofpago, tipoplazoprimerpagofpago, plazoentrerecibofpago, tipoplazoentrerecibofpago) VALUES (1, '1 pago - 30 dias', 1, 30, NULL, 0, NULL);
INSERT INTO fpago (idfpago, nomfpago, nplazosfpago, plazoprimerpagofpago, tipoplazoprimerpagofpago, plazoentrerecibofpago, tipoplazoentrerecibofpago) VALUES (2, '2 pagos - 30 y 60 dias', 2, 30, NULL, 20, NULL);
INSERT INTO fpago (idfpago, nomfpago, nplazosfpago, plazoprimerpagofpago, tipoplazoprimerpagofpago, plazoentrerecibofpago, tipoplazoentrerecibofpago) VALUES (3, '3 pagos - 30 , 60 y 90 dias', 3, 30, NULL, 30, NULL);


--
-- TOC entry 2 (OID 3347649)
-- Name: fpago_idfpago_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('fpago_idfpago_seq', 3, true);


