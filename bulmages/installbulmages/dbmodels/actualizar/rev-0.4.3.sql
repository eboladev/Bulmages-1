CREATE TABLE adocumental (
    idadocumental serial PRIMARY KEY,
    idasiento integer REFERENCES asiento(idasiento),
    descripcionadocumental character varying(200),
    fechaintadocumental timestamp with time zone,
    fechaasadocumental timestamp with time zone,
    archivoadocumental character varying(300)
);
