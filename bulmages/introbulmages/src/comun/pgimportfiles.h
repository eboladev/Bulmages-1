#include <qfile.h>
#include <qstring.h>

#include "postgresiface2.h"

class pgimportfiles {
private:
	postgresiface2 *conexionbase;
	void (*alerta)(int,int);
	void (*mensajeria) (QString);
	bool m_modoTest;
public:
	void setModoTest() {m_modoTest=TRUE;};
	void setModoNormal() {m_modoTest=FALSE;};
	bool modoTest() {return m_modoTest == TRUE;};
	pgimportfiles(postgresiface2 *, void(*)(int,int), void(*) (QString));
	~pgimportfiles(){};
	int contaplus2Bulmages(QFile &subcuentas, QFile &asientos);
	int bulmages2Contaplus(QFile &subcuentas, QFile &asientos);
	QString searchParent(QString);
};
