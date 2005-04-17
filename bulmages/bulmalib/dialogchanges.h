#ifndef __DIALOGCHANGES__
#define __DIALOGCHANGES__

#include <qobject.h>
#include <qobjectlist.h>

/* EN DESUSO PERO DE INTERES COMO EJEMPLO
class dialogChanges1: public QObject {
Q_OBJECT
private:
	bool m_modificaciones;
public:
	dialogChanges1(QObject *parent) : QObject(parent) {
		m_modificaciones=FALSE;
		parent->installEventFilter(this);
	};
	~dialogChanges1(){};
	bool dialogChanged() {return m_modificaciones;}; 
	void resetDialogChanged() {m_modificaciones=FALSE;};
	void setDialogChanged() {m_modificaciones=TRUE;};
	virtual bool eventFilter( QObject *obj, QEvent *ev ) {
		if(!dialogChanged()) {
//	if ( obj->isA("QTable") || obj->isA("QLineEdit")) {
		if ( ev->type() == QEvent::KeyPress || ev->type() == QEvent::KeyRelease) {
			setDialogChanged();
		}// end if
		}// end if
//	}// end if
	return FALSE;
	}; //end eventFilter;	
};
*/


// ESta es la buena y que funciona.
/** Esta clase se encarga de proporcionar algo de infraestructura al control de cambios
  * La idea original es de Alvaro de Miguel en la que lo que hace la función es
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
