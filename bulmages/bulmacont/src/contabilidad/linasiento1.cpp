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
    addDBCampo("descripcioncuenta", DBCampo::DBvarchar, DBCampo::DBNoSave, "Descripcion Cuenta");
    addDBCampo("codigo", DBCampo::DBvarchar, DBCampo::DBNoSave, "Codigo Cuenta");
    addDBCampo("descripcioncanal", DBCampo::DBvarchar, DBCampo::DBNoSave, "Descripcion Canal");
    addDBCampo("nombrecanal", DBCampo::DBvarchar, DBCampo::DBNoSave, "Nombre Canal");
    addDBCampo("descripcionc_coste", DBCampo::DBvarchar, DBCampo::DBNoSave, "Descripcion Canal");
    addDBCampo("nombrec_coste", DBCampo::DBvarchar, DBCampo::DBNoSave, "Nombre Canal");
}// end definetabla


LinAsiento1::LinAsiento1(empresa *comp) : DBRecord(comp) {
    companyact = comp;
    definetabla();
}


LinAsiento1::LinAsiento1(empresa *comp, QString idborrador) : DBRecord(comp) {
    companyact = comp;
    definetabla();
    QString SQLQuery = "SELECT * FROM borrador ";
    SQLQuery += " LEFT JOIN (SELECT codigo, descripcion AS descripcionc, idcuenta  FROM cuenta) AS t1 ON borrador.idcuenta=t1.idcuenta ";
    SQLQuery += " LEFT JOIN (SELECT idcanal, nombre AS nombrecanal, descripcion AS descripcioncanal FROM canal) AS t2 ON borrador.idcanal = t2.idcanal ";
    SQLQuery += " LEFT JOIN (SELECT idc_coste, nombre AS nombrec_coste, descripcion AS descripcionc_coste FROM c_coste) AS t3 ON borrador.idc_coste = t3.idc_coste ";
    SQLQuery += "WHERE  idborrador="+idborrador;
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


int LinAsiento1::guardaLinAsiento1() {
	_depura("LinAsiento1::guardaLinAsiento1",0);
    QString id;
    companyact->begin();
    int error = DBsave(id);
    if (error ) {
        companyact->rollback();
        return -1;
    }// end if
    setDBvalue("idborrador",id);
    companyact->commit();
    return 0;
}// end guardalinpresupuesto



void LinAsiento1::setcodigo(QString val) {
    _depura("LinAsiento1::setcodigocuenta()\n", 0);
    setDBvalue("codigo",val);
    QString SQLQuery = "SELECT descripcion, idcuenta FROM cuenta WHERE codigo='"+val+"'";
    cursor2 *cur=companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        setDBvalue("descripcioncuenta",cur->valor("descripcion"));
        setDBvalue("idcuenta",cur->valor("idcuenta"));
    }// end if
    delete cur;
}// end setcodigocuenta

void LinAsiento1::setidcuenta(QString val) {
    _depura("LinAsiento1::setidcuenta()\n", 0);
    setDBvalue("idcuenta",val);
    QString SQLQuery = "SELECT idcuenta, descripcion, codigo FROM cuenta WHERE idcuenta="+val+"";
    cursor2 *cur=companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        setDBvalue("descripcioncuenta" ,cur->valor("descripcion"));
        setDBvalue("idcuenta",cur->valor("idcuenta"));
	setDBvalue("codigo",cur->valor("codigo"));
    }// end if
    delete cur;
    _depura("end LinAsiento1::setidcuenta\n",0);
}// end setidarticulo



void LinAsiento1::setidcanal(QString val) {
    _depura("LinAsiento1::setidcanal()\n", 0);
    setDBvalue("idcanal",val);
    QString SQLQuery = "SELECT idcanal, nombre, descripcion FROM canal WHERE idcanal="+val+"";
    cursor2 *cur=companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        setDBvalue("descripcioncanal" ,cur->valor("descripcion"));
        setDBvalue("nombrecanal",cur->valor("nombre"));
    }// end if
    delete cur;
    _depura("end LinAsiento1::setidcanal\n",0);
}// end setidarticulo


void LinAsiento1::setidc_coste(QString val) {
    _depura("LinAsiento1::setidc_coste()\n", 0);
    setDBvalue("idc_coste",val);
    QString SQLQuery = "SELECT idc_coste, nombre, descripcion FROM c_coste WHERE idc_coste="+val+"";
    cursor2 *cur=companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        setDBvalue("descripcionc_coste" ,cur->valor("descripcion"));
        setDBvalue("nombrec_coste",cur->valor("nombre"));
    }// end if
    delete cur;
    _depura("end LinAsiento1::setidc_coste\n",0);
}// end setidarticulo


void LinAsiento1::setnombrecanal(QString val) {
    _depura("LinAsiento1::setnombrecanal()\n", 0);
    setDBvalue("nombrecanal",val);
    QString SQLQuery = "SELECT * FROM canal WHERE nombre='"+val+"'";
    cursor2 *cur=companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        setDBvalue("idcanal",cur->valor("idcanal"));
        setDBvalue("descripcioncanal",cur->valor("descripcion"));
    }// end if
    delete cur;
}// end setcodigocuenta


void LinAsiento1::setnombrec_coste(QString val) {
    _depura("LinAsiento1::setnombrec_coste()\n", 0);
    setDBvalue("nombrec_coste",val);
    QString SQLQuery = "SELECT * FROM c_coste WHERE nombre='"+val+"'";
    cursor2 *cur=companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        setDBvalue("idc_coste",cur->valor("idc_coste"));
        setDBvalue("descripcionc_coste",cur->valor("descripcion"));
    }// end if
    delete cur;
}// end setcodigocuenta


Fixed LinAsiento1::calculadebe() {
    _depura("calculadebe()\n",0);
    Fixed cant=Fixed("0.0");
    if (DBvalue("debe") != "" ) {
        cant = Fixed(DBvalue("debe"));
    }// end if
    return cant;
}// end calculadebe

Fixed LinAsiento1::calculahaber() {
    _depura("calculadebe()\n",0);
    Fixed cant=Fixed("0.0");
    if (DBvalue("haber") != "" ) {
        cant = Fixed(DBvalue("haber"));
    }// end if
    return cant;
}// end calculadebe

/*
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
