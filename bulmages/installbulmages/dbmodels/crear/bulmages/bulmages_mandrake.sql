--
-- PostgreSQL database definition of Bulmages
-- Creamos el lenguaje plpgsql y lo preparamos para ser usado con toda la base
-- de datos.

SET client_encoding = 'SQL_ASCII';
SET check_function_bodies = false;

SET search_path = public, pg_catalog;

--
-- TOC entry 84 (OID 1345917)
-- Name: plpgsql_call_handler(); Type: FUNC PROCEDURAL LANGUAGE; Schema: public; Owner: postgres
--

CREATE FUNCTION plpgsql_call_handler() RETURNS language_handler
    AS '/usr/lib/pgsql/plpgsql.so', 'plpgsql_call_handler'
    LANGUAGE c;


--
-- TOC entry 83 (OID 1345918)
-- Name: plpgsql; Type: PROCEDURAL LANGUAGE; Schema: public; Owner: 
--

CREATE TRUSTED PROCEDURAL LANGUAGE plpgsql HANDLER plpgsql_call_handler;


--
-- TOC entry 4 (OID 2200)
-- Name: public; Type: ACL; Schema: -; Owner: postgres
--

REVOKE ALL ON SCHEMA public FROM PUBLIC;
GRANT ALL ON SCHEMA public TO PUBLIC;

