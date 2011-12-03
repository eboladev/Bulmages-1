CREATE TABLE categorialibro
(
   idcategorialibro serial NOT NULL,
   nomcategorialibro character varying(255) NOT NULL,
   PRIMARY KEY (idcategorialibro)
);

CREATE TABLE autor
(
   idautor serial NOT NULL,
   nomautor character varying(255) NOT NULL,
   idpais integer,
   fechanacimientoautor date,
   PRIMARY KEY (idautor),
   CONSTRAINT idpais FOREIGN KEY (idpais) REFERENCES pais (idpais)
);

CREATE TABLE socio
(
   idsocio serial NOT NULL,
   nomsocio character varying(50),
   apellido1socio character varying(50),
   apellido2socio character varying(50),
   fechanacimientosocio date,
   telsocio character varying(20),
   mailsocio character varying(50),
   dirsocio character varying(100),
   idprovincia integer,
   CONSTRAINT idsocioprimarykey PRIMARY KEY (idsocio),
   CONSTRAINT socioprovincia FOREIGN KEY (idprovincia) REFERENCES provincia (idprovincia) ON UPDATE NO ACTION ON DELETE NO ACTION
);

CREATE TABLE libro
(
   idlibro serial NOT NULL,
   isbnlibro character varying(40) NOT NULL,
   titulolibro character varying(200) NOT NULL,
   idautor integer,
   resumenlibro text,
   idcategorialibro integer,
   CONSTRAINT libroidliborprimarykey PRIMARY KEY (idlibro),
   CONSTRAINT idcategorialibro_libro_externkey FOREIGN KEY (idcategorialibro) REFERENCES categorialibro (idcategorialibro) ON UPDATE NO ACTION ON DELETE NO ACTION,
   CONSTRAINT autor_idautor_fkey FOREIGN KEY (idautor) REFERENCES autor (idautor) ON UPDATE NO ACTION ON DELETE NO ACTION
);


CREATE TABLE prestamo
(
   idprestamo serial,
   fechainprestamo date NOT NULL DEFAULT now(),
   fechafinprestamo date NOT NULL DEFAULT now() +' @ 1 week',
   idsocio integer NOT NULL,
   idlibro integer NOT NULL,
   devueltoprestamo boolean DEFAULT FALSE,
   CONSTRAINT prestamo_idprestamo_p_key PRIMARY KEY (idprestamo),
   CONSTRAINT socio_idsocio_f_key FOREIGN KEY (idsocio) REFERENCES socio (idsocio) ON UPDATE NO ACTION ON DELETE NO ACTION,
   CONSTRAINT libro_idlibro_f_key FOREIGN KEY (idlibro) REFERENCES libro (idlibro) ON UPDATE NO ACTION ON DELETE NO ACTION
);


CREATE TABLE zcomercial (
    idzcomercial SERIAL PRIMARY KEY,
    nomzonacomercial VARCHAR(60),
    avzcomercial VARCHAR(4)
);

ALTER TABLE cliente ADD COLUMN idzcomercial integer REFERENCES zcomercial(idzcomercial) ;

INSERT INTO zcomercial (nomzonacomercial, avzcomercial) VALUES ('Palma', 'PM');
INSERT INTO zcomercial (nomzonacomercial, avzcomercial) VALUES ('Raiguer', 'RG');
INSERT INTO zcomercial (nomzonacomercial, avzcomercial) VALUES ('Llevant', 'LL');

