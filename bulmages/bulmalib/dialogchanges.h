#ifndef __DIALOGCHANGES__
#define __DIALOGCHANGES__

/* #define NUMQTEXT = 999
#define NUMQLINE = 999
#define NUMQTABLE = 99999
*/
#include "funcaux.h"
#include <QObject>

/** Esta clase se encarga de proporcionar algo de infraestructura al control de cambios
  * La idea original es de Alvaro de Miguel en la que lo que hace la funci� es
  * coger todos los campos de un determinado formulario y formar una cadena.
  * Luego para saber si hay cambios en el formulario vuelve a comparar.
  */
class dialogChanges {

private:
	int m_maxQLine;
	int m_maxQText;
	int m_maxQTable;
	QString valorinicial;
	QObject *m_obje;
	QObject *m_listaQLine[999];
	QObject *m_listaQText[999];
	QObject *m_listaQTable[999];

public:
	dialogChanges(QObject *ob) {
_depura("dialogChanges::dialogChanges",0);
m_obje=ob;
_depura("END dialogChanges::dialogChanges",0);

};
	~dialogChanges(){};
	void dialogChanges_cargaInicial();
	bool dialogChanges_hayCambios();
private:
QString calculateValues();
QString retrieveValues(QString qsWidget);
};


#endif
