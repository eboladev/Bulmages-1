
CREATE TABLE empresafact (
    idempresafact serial PRIMARY KEY,
    nombre character varying(50),
    ano character varying(10),
    nombredb character varying(15),
    "password" character varying(15),
    ejant character varying(15)
);


CREATE TABLE usuario_empresafact (
    idusuarioempresafact serial PRIMARY KEY,
    idempresafact integer NOT NULL REFERENCES empresafact(idempresafact),
    idusuario integer NOT NULL REFERENCES usuario(idusuario),
    permisos integer
);