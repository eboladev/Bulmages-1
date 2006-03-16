//
// C++ Implementation: CompArticulo
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "comparticulo.h"

void CompArticulo::definetabla() {
	_depura("CompArticulo::definetabla",0);
    setDBTableName("comparticulo");
    setDBCampoId("idcomponente");
    addDBCampo("idarticulo", DBCampo::DBint, DBCampo::DBPrimaryKey, "Identificador Linea Presupuesto");
    addDBCampo("idcomponente", DBCampo::DBint, DBCampo::DBPrimaryKey, "Identificador Linea Presupuesto");
    addDBCampo("cantcomparticulo", DBCampo::DBnumeric, DBCampo::DBNotNull, "Cantidad");
    addDBCampo("codigocompletocomponente", DBCampo::DBvarchar, DBCampo::DBNoSave, "Codigo Articulo");
    addDBCampo("nomcomponente", DBCampo::DBvarchar, DBCampo::DBNoSave, "Nombre 	Articulo");
}// end definetabla


CompArticulo::CompArticulo(company *comp) : DBRecord(comp) {
	_depura("CompArticulo::CompArticulo",0);
    companyact = comp;
    definetabla();
}

CompArticulo::CompArticulo(company *comp, QString idarticulo, QString idcomponente) : DBRecord(comp) {
	_depura("CompArticulo::CompArticulo",0);
    companyact = comp;
    definetabla();
    QString SQLQuery = "SELECT * FROM comparticulo, articulo WHERE comparticulo.idcomponente=articulo.idarticulo AND idarticulo="+idarticulo+" AND idcomponente="+idcomponente;
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        DBload(cur);
    } else {
        vaciaCompArticulo();
    }// end if
}// end CompArticulo


CompArticulo::CompArticulo(company *comp, QString a, QString b, QString c, QString d, QString e) : DBRecord(comp) {
	_depura("CompArticulo::CompArticulo",0);
    companyact = comp;
    definetabla();
    setDBvalue("idarticulo", a);
    setDBvalue("idcomponente", b);
    setDBvalue("cantcomparticulo", c);
    setDBvalue("codigocompletocomponente",d);
    setDBvalue("nomcomponente", e);
}// end CompArticulo


CompArticulo::~CompArticulo() {}


void CompArticulo::vaciaCompArticulo() {
    DBclear();
}




void CompArticulo::guardaCompArticulo() {
    /// Segun est�la linea en la base de datos o no se hace una cosa u otra.
    if (DBvalue("idarticulo") != "" && DBvalue("idcomponente") != "") {
        QString SQLQuery = "INSERT INTO comparticulo (idarticulo, idcomponente, cantcomparticulo) VALUES ("+
                           DBvalueprep("idarticulo")+","+
                           DBvalueprep("idcomponente")+","+
                           DBvalueprep("cantcomparticulo")+")";
        companyact->begin();
        int error = companyact->ejecuta("DELETE FROM comparticulo WHERE idarticulo="+DBvalueprep("idarticulo")+" AND idcomponente="+DBvalueprep("idcomponente"));
        if (error) {
            companyact->rollback();
            return;
        }// end if
        error = companyact->ejecuta(SQLQuery);
        if (error) {
            companyact->rollback();
            return;
        }// end if
        companyact->commit();
    }// end if
}// end guardaCompArticulo


void CompArticulo::setcodigocompletocomponente(QString val) {
    _depura("setcodigocompletoarticulo()\n",0);
    setDBvalue("codigocompletocomponente",val);
    QString SQLQuery = "SELECT nomarticulo, idarticulo FROM articulo WHERE codigocompletoarticulo='"+val+"'";
    cursor2 *cur=companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        setDBvalue("nomcomponente",cur->valor("nomarticulo"));
        setDBvalue("idcomponente",cur->valor("idarticulo"));
    }// end if
    delete cur;
}// end setcodigocompletoarticulo


void CompArticulo::setidcomponente(QString val) {
    _depura("setidcomponente()\n", 0);
    setDBvalue("idcomponente",val);
    QString SQLQuery = "SELECT nomarticulo, codigocompletoarticulo FROM articulo WHERE idarticulo="+val+"";
    cursor2 *cur=companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        setDBvalue("nomcomponente",cur->valor("nomarticulo"));
        setDBvalue("codigocompletocomponente", cur->valor("codigocompletoarticulo"));
    }// end if
    delete cur;
}// end setidarticulo
