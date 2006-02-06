//
// C++ Implementation: linpresupuesto
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "linasiento1.h"

/*
CREATE TABLE borrador (
    idborrador serial PRIMARY KEY,
    codigoborrador integer,
    idasiento integer NOT NULL REFERENCES asiento(idasiento),
    iddiario integer REFERENCES diario(iddiario),
    fecha timestamp with time zone,
    conceptocontable character varying(50),
    idcuenta integer NOT NULL REFERENCES cuenta(idcuenta),
    descripcion character varying(100),
    debe numeric(12,2) NOT NULL DEFAULT 0,
    haber numeric(12,2) NOT NULL DEFAULT 0,
    contrapartida integer REFERENCES cuenta(idcuenta),
    comentario character varying(100),
    idcanal integer REFERENCES canal(idcanal),
    marcaconciliacion character(12),
    idc_coste integer REFERENCES c_coste(idc_coste),
 -- El campo idapunte no se utiliza, existe la combinacion idasiento, orden que lo sustituye.
    idapunte integer,
    idtipoiva integer,
    orden integer NOT NULL
);
*/

void LinAsiento1::definetabla() {
    setDBTableName("borrador");
    setDBCampoId("idborrador");
    addDBCampo("idborrador", DBCampo::DBint, DBCampo::DBPrimaryKey, "Identificador Borrador");
    addDBCampo("codigoborrador", DBCampo::DBint, DBCampo::DBNotNull, "Identificador Presupuesto");
    addDBCampo("idasiento", DBCampo::DBint, DBCampo::DBNothing, "Descripcion");
    addDBCampo("iddiario", DBCampo::DBint, DBCampo::DBNothing, "Cantidad");
    addDBCampo("fecha", DBCampo::DBdate, DBCampo::DBNotNull, "Precio Linea Presupuesto");
    addDBCampo("conceptocontable", DBCampo::DBvarchar, DBCampo::DBNothing, "Descuento Linea Presupuesto");
    addDBCampo("idcuenta", DBCampo::DBint, DBCampo::DBNothing, "Articulo Presupuesto");
    addDBCampo("descripcion", DBCampo::DBvarchar, DBCampo::DBNothing, "IVA linea Presupuesto");
    addDBCampo("debe", DBCampo::DBnumeric, DBCampo::DBNothing, "Codigo Articulo");
    addDBCampo("haber", DBCampo::DBnumeric, DBCampo::DBNothing, "Nombre 	Articulo");
    addDBCampo("contrapartida", DBCampo::DBint, DBCampo::DBNothing, "Nombre 	Articulo");
    addDBCampo("comentario", DBCampo::DBvarchar, DBCampo::DBNothing, "Nombre 	Articulo");
    addDBCampo("idcanal", DBCampo::DBint, DBCampo::DBNothing, "Nombre 	Articulo");
    addDBCampo("marcaconciliacion", DBCampo::DBvarchar, DBCampo::DBNothing, "Nombre 	Articulo");
    addDBCampo("idc_coste", DBCampo::DBint, DBCampo::DBNothing, "Nombre 	Articulo");
    addDBCampo("idapunte", DBCampo::DBint, DBCampo::DBNothing, "Nombre 	Articulo");
    addDBCampo("idtipoiva", DBCampo::DBint, DBCampo::DBNothing, "Nombre 	Articulo");
    addDBCampo("orden", DBCampo::DBint, DBCampo::DBNothing, "Nombre 	Articulo");
}// end definetabla


LinAsiento1::LinAsiento1(empresa *comp) : DBRecord(comp) {
    companyact = comp;
    definetabla();
}


LinAsiento1::LinAsiento1(empresa *comp, QString idborrador) : DBRecord(comp) {
    companyact = comp;
    definetabla();
    QString SQLQuery = "SELECT * FROM borrador, cuenta WHERE borrador.idcuenta=cuenta.idcuenta AND idborrador="+idborrador;
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        DBload(cur);

    } else {
        vaciaLinAsiento1();
    }// end if
}// end linpresupuesto


LinAsiento1::LinAsiento1(empresa *comp, cursor2 *cur) : DBRecord(comp) {
    companyact = comp;
    definetabla();
    DBload(cur);
}


LinAsiento1::~LinAsiento1() {}


void LinAsiento1::vaciaLinAsiento1() {
    DBclear();
}


void LinAsiento1::borrar() {
    if (DBvalue("idborrador") != "") {
        companyact->begin();
        int error = companyact->ejecuta("DELETE FROM borrador WHERE idborrador="+DBvalue("idborrador"));
        if (error) {
            companyact->rollback();
            return;
        }// end if
        companyact->commit();
        vaciaLinAsiento1();
    }// end if
}// end delete


void LinAsiento1::guardaLinAsiento1() {
    QString id;
    companyact->begin();
    int error = DBsave(id);
    if (error ) {
        companyact->rollback();
        return;
    }// end if
    setDBvalue("idborrador",id);
    companyact->commit();
}// end guardalinpresupuesto


/*
void linpresupuesto::setcodigocompletoarticulo(QString val) {
    _depura("setcodigocompletoarticulo()\n", 0);
    setDBvalue("codigocompletoarticulo",val);
    QString SQLQuery = "SELECT nomarticulo, idarticulo, pvparticulo(idarticulo) AS pvp, ivaarticulo(idarticulo) AS iva FROM articulo WHERE codigocompletoarticulo='"+val+"'";
    cursor2 *cur=companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        setDBvalue("nomarticulo",cur->valor("nomarticulo"));
        setDBvalue("desclpresupuesto",cur->valor("nomarticulo"));
        setDBvalue("idarticulo", cur->valor("idarticulo"));
        setDBvalue("pvplpresupuesto",cur->valor("pvp"));
        setDBvalue("ivalpresupuesto", cur->valor("iva"));
        if (DBvalue("cantlpresupuesto") == "") {
            setDBvalue("cantlpresupuesto" , "1");
            setDBvalue("descuentolpresupuesto" , "0");
        }// end if
    }// end if
    delete cur;
}// end setcodigocompletoarticulo


void linpresupuesto::setidarticulo(QString val) {
    _depura("setidarticulo()\n", 0);
    setDBvalue("idarticulo",val);
    QString SQLQuery = "SELECT nomarticulo, codigocompletoarticulo, pvparticulo(idarticulo) AS pvp, ivaarticulo(idarticulo) AS iva FROM articulo WHERE idarticulo="+val+"";
    cursor2 *cur=companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        setDBvalue("nomarticulo" ,cur->valor("nomarticulo"));
        setDBvalue("desclpresupuesto",cur->valor("nomarticulo"));
        setDBvalue("codigocompletoarticulo",cur->valor("codigocompletoarticulo"));
        setDBvalue("pvplpresupuesto", cur->valor("pvp"));
        setDBvalue("ivalpresupuesto", cur->valor("iva"));
        if (DBvalue("cantlpresupuesto") == "") {
            setDBvalue("cantlpresupuesto", "1");
            setDBvalue("descuentolpresupuesto", "0");
        }// end if
    }// end if
    delete cur;
    _depura("end setidarticulo\n",0);
}// end setidarticulo


float linpresupuesto::calculabase() {
    _depura("calculabase()\n",0);
    float cant=0;
    if (DBvalue("cantlpresupuesto") != "" && DBvalue("pvplpresupuesto") != "" && DBvalue("desclpresupuesto") != "") {
        cant = DBvalue("cantlpresupuesto").toFloat() * DBvalue("pvplpresupuesto").toFloat();
        cant = cant - (cant* DBvalue("desclpresupuesto").toFloat());
    }// end if
    return cant;
}// end calculabase


float linpresupuesto::calculaiva() {
    _depura("calculaiva()\n",0);
    float cant=0;
    if (DBvalue("cantlpresupuesto") != "" && DBvalue("pvplpresupuesto") != "" && DBvalue("desclpresupuesto") != "" && DBvalue("ivalpresupuesto") != "") {
        cant = DBvalue("cantlpresupuesto").toFloat() * DBvalue("pvplpresupuesto").toFloat();
        cant = cant - (cant* DBvalue("desclpresupuesto").toFloat()/100);
        cant = cant * DBvalue("ivalpresupuesto").toFloat()/100;
    }// end if
    return cant;
}// end calculabase
*/
