#ifndef DBRECORD_H
#define DBRECORD_H

#include <QString>
#include <QList>
#include "postgresiface2.h"
#include "funcaux.h"

class DBCampo {
public:
   enum dbtype {DBint=1, DBvarchar=2, DBdate=3, DBnumeric=4, DBboolean=5};
   enum dbrestrict {DBNothing=0, DBNotNull=1, DBPrimaryKey=2, DBNoSave=4, DBAuto=8, DBDupPrimaryKey=16, DBRequired=32};
protected:
	QString m_nomcampo;
	QString m_valorcampo;
	QString m_nompresentacion;
	int   m_restrict;
	dbtype m_tipo;
   	postgresiface2 *m_conexionbase;
public:
	DBCampo(postgresiface2 *com, QString nom, dbtype typ, int res, QString nomp="");
	virtual ~DBCampo(){_depura("DBCampo::~DBCampo",1);};

	postgresiface2 *conexionbase() {return m_conexionbase;};
	void setconexionbase(postgresiface2 *comp) {m_conexionbase=comp;};

	dbtype tipo() {return m_tipo;};
	virtual int set(QString val) {m_valorcampo=val;return 0;};
	int restrictcampo() {return m_restrict;};
	QString nomcampo() {return m_nomcampo;};
	QString nompresentacion() {return m_nompresentacion;};
	QString valorcampo() {return m_valorcampo;};
	QString valorcampoprep(int &error);
};
	
class DBRecord {
protected:
    QList<DBCampo *> m_lista;
    postgresiface2 *m_conexionbase;
    QString m_tablename;
    QString m_campoid;
    bool m_nuevoCampo;
public:
	DBRecord(postgresiface2 *);
	virtual ~DBRecord();
	void setconexionbase(postgresiface2 *comp) {m_conexionbase=comp;};
	postgresiface2 *conexionbase() {return m_conexionbase;};
	int DBload(cursor2 *);
	virtual int DBsave(QString &id);
	int setDBvalue(QString, QString);
	QString DBvalue(QString);
	QString DBvalueprep(QString);
	void setDBTableName(QString nom) {m_tablename=nom;};
	void setDBCampoId(QString nom) {m_campoid = nom;};
	int addDBCampo(QString, DBCampo::dbtype, int, QString);
	void DBclear();

	QList<DBCampo *> *lista() {return &m_lista;};

	virtual int borrar();
	virtual int guardar();
	virtual void vaciar() {DBclear();};
	virtual int cargar(QString);
};


#endif

