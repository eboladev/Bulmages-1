#ifndef DBRECORD_H
#define DBRECORD_H

#include <QString>
#include <Q3PtrList>
#include "postgresiface2.h"
#include "funcaux.h"

class DBCampo {
public:
   enum dbtype {DBint=1, DBvarchar=2, DBdate=3, DBnumeric=4, DBboolean=5};
   enum dbrestrict {DBNothing=0, DBNotNull=1, DBPrimaryKey=2, DBNoSave=4};
private:
	QString m_nomcampo;
	QString m_valorcampo;
	QString m_nompresentacion;
	dbrestrict   m_restrict;
	dbtype m_type;
   	postgresiface2 *conexionbase;
public:
	DBCampo(postgresiface2 *com, QString nom, dbtype typ, dbrestrict res, QString nomp=""){
		conexionbase = com;
		m_nomcampo = nom;
		m_valorcampo="";
		m_nompresentacion = nomp;
		m_restrict = res;
		m_type = typ;
	};
	~DBCampo(){};
	int set(QString val) {m_valorcampo=val;return 0;};
	dbrestrict restrictcampo() {return m_restrict;};
	QString nomcampo() {return m_nomcampo;};
	QString valorcampo() {return m_valorcampo;};
	QString valorcampoprep() {
		switch (m_type) {
			case DBint:
				if (m_valorcampo == "") return "NULL";
				return conexionbase->sanearCadena(m_valorcampo);
			case DBvarchar:
				return "'"+conexionbase->sanearCadena(m_valorcampo)+"'";
			case DBdate:
				if (m_valorcampo == "") return "NULL";
				return "'"+conexionbase->sanearCadena(m_valorcampo)+"'";
			case DBnumeric:
				if (m_valorcampo == "") return "NULL";
				return m_valorcampo;
			case DBboolean:
				if (m_valorcampo == "") return "NULL";
				if (m_valorcampo == "f" || m_valorcampo == "t")
				return "'"+conexionbase->sanearCadena(m_valorcampo)+"'";
				return conexionbase->sanearCadena(m_valorcampo);
		}// end switch
		_depura("Error en la conversion de tipos",2);
		return "";
	};
};
	
class DBRecord {
private:
    Q3PtrList<DBCampo> m_lista;
    postgresiface2 *conexionbase;
    QString m_tablename;
    QString m_campoid;
public:
	DBRecord(postgresiface2 *);
	~DBRecord();
	int DBload(cursor2 *);
	int DBsave(QString &id);
	int setDBvalue(QString, QString);
	QString DBvalue(QString);
	QString DBvalueprep(QString);
	void setDBTableName(QString nom) {m_tablename=nom;};
	void setDBCampoId(QString nom) {m_campoid = nom;};
	int addDBCampo(QString, DBCampo::dbtype, DBCampo::dbrestrict, QString);
	void DBclear();
};


#endif

