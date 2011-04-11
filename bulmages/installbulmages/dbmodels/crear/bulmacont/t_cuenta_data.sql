
--
-- PostgreSQL database dump
--
BEGIN;

SET client_encoding = 'UNICODE';
SET check_function_bodies = false;

SET search_path = public, pg_catalog;



-- Como hay un trigger recursivo y hay cuentas de nivel 4 que se actualizan despues que las de nivel 1 hay que hacer el update
-- tantas veces como niveles de cuentas tenemos.
-- El scripting de base de datos funciona muy bien.
-- UPDATE CUENTA SET debe=0, haber=0;
-- UPDATE CUENTA SET debe=0, haber=0;
-- UPDATE CUENTA SET debe=0, haber=0;
-- UPDATE CUENTA SET debe=0, haber=0;
-- UPDATE CUENTA SET debe=0, haber=0;
-- UPDATE CUENTA SET debe=0, haber=0;
-- UPDATE CUENTA SET debe=0, haber=0;
-- UPDATE CUENTA SET debe=0, haber=0;

--
-- TOC entry 2 (OID 3697052)
-- Name: cuenta_idcuenta_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('cuenta_idcuenta_seq', 10000, false);

COMMIT;