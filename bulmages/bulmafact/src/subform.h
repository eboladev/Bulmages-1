#ifndef SUBFORM_H
#define SUBFORM_H

#include "postgresiface2.h"
#include "funcaux.h"
#include "qtable2.h"
#include "dbrecord.h"

class SHeader {
public:
    enum dboptions {DBNone=0, DBReadOnly=1, DBNoView=2, DBNoWrite=4};
public:
    QString m_nomcampo;
    QString m_valorcampo;
    QString m_nompresentacion;
    int   m_restrict;
    DBCampo::dbtype m_type;
    int m_options;
public:
    SHeader(QString nom, DBCampo::dbtype typ, int res, int opt, QString nomp="") {
        m_nomcampo = nom;
        m_valorcampo="";
        m_nompresentacion = nomp;
        m_restrict = res;
        m_options = opt;
        m_type = typ;
    };
    ~SHeader() {};
    int set
        (QString val) {
        m_valorcampo=val;
        return 0;
    };
    int restrictcampo() {
        return m_restrict;
    };
    QString nomcampo() {
        return m_nomcampo;
    };
};


class SDBCampo: public QTableWidgetItem2, public DBCampo {
public:
    SDBCampo(postgresiface2 *com, QString nom, dbtype typ, int res, QString nomp=""): QTableWidgetItem2(), DBCampo(com,  nom,  typ,  res,  nomp) {}
    ;
    virtual ~SDBCampo() {}
    ;
    virtual int set
        (QString val) {
        _depura("SDBCampo::set");
        QTableWidgetItem2::setText(val);
        DBCampo::set
            (val);
        return 0;
    };
	void refresh() {DBCampo::set(text());};
};


class SDBRecord: public DBRecord {
public:
    SDBRecord(postgresiface2 *con) : DBRecord(con) {}
    ;
    ~SDBRecord() {}
    ;

    int addDBCampo(QString nom, DBCampo::dbtype typ, int res, QString nomp="") {
        SDBCampo *camp = new SDBCampo(conexionbase, nom, typ, res, nomp);
        camp->set
        ("");
        m_lista.append(camp);
        return 0;
    };
};


class SubForm2: public QTableWidget2 {
    Q_OBJECT
private:
    Q3PtrList<SHeader>   m_lcabecera;
    Q3PtrList<SDBRecord> m_lista;
    postgresiface2 *m_companyact;
    QString m_tablename;
    QString m_campoid;
	bool m_insercion;

public:
	void guardaconfig();
	void cargaconfig();
	void situarse(int, int);
	void setinsercion(bool b) {m_insercion=b;};
    postgresiface2 *companyact() {
        return m_companyact;
    };
    SubForm2(QWidget *parent);
    virtual ~SubForm2() {guardaconfig();}
    ;
    virtual bool eventFilter(QObject *obj, QEvent *ev);

    void setDBTableName(QString nom) {
        m_tablename=nom;
    };
    void setDBCampoId(QString nom) {
        m_campoid = nom;
    };
    int addSHeader(QString nom, DBCampo::dbtype typ, int res, int opt, QString nomp);
    SDBRecord *newSDBRecord();
    void setcompany(postgresiface2 *c) {
        _depura("ListCompArticulo setCompany", 0);
        m_companyact = c;
    };
    virtual void nuevoRegistro();
	void pintaCabeceras();
	void setColumnValue(QString, QString);
	QString DBvalue(QString campo, int row=-1);

    virtual int borrar();
    virtual int borrar(int);
    virtual void guardar();
    virtual void vaciar() {};
    virtual void pintar() {};
    virtual int cargar(cursor2 *cur);
    SDBRecord *SubForm2::lineaact();
    SDBRecord *SubForm2::lineaat(int row);
public:
    virtual void editFinished(int row, int col) {_depura ("editFinished aun no implementado",1);};
    virtual void pressedAsterisk(int row, int col) {_depura ("pressedAsterisk aun no implementado",1);};
};

#endif

