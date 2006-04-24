#ifndef __DIALOGCHANGES__
#define __DIALOGCHANGES__

#include <QObject>

/** Esta clase se encarga de proporcionar algo de infraestructura al control de cambios
  * La idea original es de Alvaro de Miguel en la que lo que hace la funci� es
  * coger todos los campos de un determinado formulario y formar una cadena.
  * Luego para saber si hay cambios en el formulario vuelve a comparar.
  */
class dialogChanges {
private:
	QString valorinicial;
	QObject *obje;
public:
	dialogChanges(QObject *ob) {obje=ob;};
	~dialogChanges(){};
	void dialogChanges_cargaInicial();
	bool dialogChanges_hayCambios();
private:
QString calculateValues();
QString retrieveValues(QString qsWidget);
};


#endif
