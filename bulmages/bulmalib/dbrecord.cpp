#include "dbrecord.h"
#include "funcaux.h"


DBRecord::DBRecord(postgresiface2 *con) {
    conexionbase = con;
}


DBRecord::~DBRecord() {}

int DBRecord::DBload(cursor2 *cur) {
    _depura("DBRecord::DBload",0);
    DBCampo *linea;
    int error = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        QString nom =linea->nomcampo();
        QString val = cur->valor(nom);
        error += linea->set
                 (val);
    }// end for
    _depura("END DBRecord::DBload",0);
    return error;
}// end DBload

void DBRecord::DBclear() {
    _depura("DBRecord::DBclear",0);
    DBCampo *linea;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        linea->set
        ("");
    }// end for
    _depura("END DBRecord::DBclear",0);
}// end DBload


int DBRecord::DBsave(QString &id) {
    _depura("DBRecord::DBsave",0);
    DBCampo *linea;
    QString listcampos="";
    QString listvalores="";
    QString queryupdate="";
    QString separador = "";
    QString separadorwhere = "";
    QString querywhere = "";
    bool esinsert = TRUE;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        if (!(linea->restrictcampo() & DBCampo::DBNoSave)) {
            if (linea->restrictcampo() & DBCampo::DBPrimaryKey) {
                if (linea->nomcampo() == m_campoid)
                    id = linea->valorcampo();
                if (linea->valorcampo() != "") {
                    esinsert = FALSE;
                }
		int err;
		QString lin = linea->valorcampoprep(err);
		if (err) return -1;
                querywhere += linea->nomcampo() + " = " + lin;
                separadorwhere = " AND ";
            } else {
		int err;
                listcampos += separador + linea->nomcampo();
                listvalores += separador + linea->valorcampoprep(err);
		if (err) return -1;
                queryupdate += separador + linea->nomcampo() + "=" + linea->valorcampoprep(err);
		if (err) return -1;
                separador = ", ";
            }// end if
        }// end if
    }// end for
    if (esinsert) {
        QString query = "INSERT INTO "+m_tablename+" ("+listcampos+") VALUES ("+listvalores+")";
        int error = conexionbase->ejecuta(query);
        if (error)
            return -1;
        cursor2 *cur = conexionbase->cargacursor("SELECT "+m_campoid+" FROM "+m_tablename+" ORDER BY "+m_campoid+" DESC LIMIT 1");
        id = cur->valor(m_campoid);
        delete cur;
    } else {
        QString query = "UPDATE "+m_tablename+" SET "+queryupdate + " WHERE "+ querywhere;
        int error = conexionbase->ejecuta(query);
        if (error)
            return -1;
        _depura(query,0);
    }// end if
    return 0;
}

int DBRecord::setDBvalue(QString nomb, QString valor) {
    _depura("DBRecord::setDBvalue",0);
    DBCampo *linea;
    int error = 0;
    linea = m_lista.first();
    while (linea && linea->nomcampo() != nomb)
        linea = m_lista.next();
    if (!linea) {
        _depura("Campo "+nomb+" no encontrado",2);
        return -1;
    }// end if
    if ( linea->nomcampo() == nomb)
        error = linea->set
                (valor);

    return error;
}

QString DBRecord::DBvalue(QString nomb) {
    _depura("DBRecord::value",0);
    DBCampo *linea;
    linea = m_lista.first();
    while (linea && linea->nomcampo()!= nomb)
        linea = m_lista.next();
    if (!linea) {
        _depura("Campo "+nomb+" no encontrado",2);
        return "";
    }// end if
    if ( linea->nomcampo() == nomb) {
        return linea->valorcampo();
    }
    return "";
}

QString DBRecord::DBvalueprep(QString nomb) {
    _depura("DBRecord::DBvalueprep",0);
    DBCampo *linea;
    linea = m_lista.first();
    while (linea && linea->nomcampo()!= nomb)
        linea = m_lista.next();
    if (!linea) {
        _depura("Campo "+nomb+" no encontrado",2);
        return "";
    }// end if
    if ( linea->nomcampo() == nomb) {
	int err;
        return linea->valorcampoprep(err);
    }
    return "";
}


int DBRecord::addDBCampo(QString nom, DBCampo::dbtype typ, DBCampo::dbrestrict res, QString nomp="") {
    DBCampo *camp = new DBCampo(conexionbase, nom, typ, res, nomp);
    camp->set
    ("");
    m_lista.append(camp);
    return 0;
}



void DBRecord::borrar() {
    _depura("DBRecord::borrar",0);
    if (DBvalue(m_campoid) != "") {
        conexionbase->begin();
        int error = conexionbase->ejecuta("DELETE FROM "+m_tablename+" WHERE "+m_campoid+"="+DBvalue(m_campoid));
        if (error) {
            conexionbase->rollback();
            return;
        }// end if
        conexionbase->commit();
        DBclear();
    }// end if
}// end borrar


void DBRecord::guardar() {
    _depura("DBRecord::guardar",0);
    QString id;
    conexionbase->begin();
    int error = DBsave(id);
    if (error ) {
        conexionbase->rollback();
        return;
    }// end if
    setDBvalue(m_campoid,id);
    conexionbase->commit();
}// end guardar



// Esta funci� carga un Articulo.
int DBRecord::cargar(QString id) {
	_depura("DBRecord::cargar",0);
    QString query = "SELECT * FROM "+m_tablename+" WHERE "+m_campoid+"="+id;
    cursor2 * cur= conexionbase->cargacursor(query);
    if (!cur->eof()) {
        DBload(cur);
    }// end if
    delete cur;
	_depura("END DBRecord::cargar",0);
	return 0;
}// end chargeBudget
