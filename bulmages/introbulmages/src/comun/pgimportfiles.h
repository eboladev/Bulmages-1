#include <qfile.h>
#include "postgresiface2.h"

class pgimportfiles {
private:
	postgresiface2 *conexionbase;
public:
	pgimportfiles(postgresiface2 *);
	~pgimportfiles(){};
	int contaplus2Bulmages(QFile &subcuentas, QFile &asientos);
	QString searchParent(QString);
};
