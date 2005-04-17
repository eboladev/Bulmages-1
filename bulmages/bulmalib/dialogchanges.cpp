#include "dialogchanges.h"

#include <qlineedit.h>
#include <qtextedit.h>


	void dialogChanges::dialogChanges_cargaInicial() {
	   valorinicial = calculateValues();
	}
	bool dialogChanges::dialogChanges_hayCambios() {
		return !(valorinicial == calculateValues());
	}
QString dialogChanges::calculateValues() {
	QString values = retrieveValues("QTable");
	values += retrieveValues("QLineEdit");
	values += retrieveValues("QTextEdit");
	return values;
}
	
QString dialogChanges::retrieveValues(QString qsWidget) {
	QObjectList *l = obje->queryList( qsWidget );
	QObjectListIterator it( *l );
	QObject *obj;
	QString values="";
    while ( (obj = it.current() ) != 0 ) {
		++it;
		
		if (qsWidget=="QLineEdit") {
			//qDebug("QLineEdit Name = %s",obj->name());
			values += ((QLineEdit*)obj)->text();
		}
		
		if (qsWidget=="QTextEdit") {
			//qDebug("QTextEdit Name = %s",obj->name());
			values += ((QTextEdit*)obj)->text();
		}
		
/*		if (qsWidget=="QTable") {
			//qDebug("QTable Name = %s",obj->name());
			for (int i=0; i < m_list->numRows(); i++) {
				for (int j=0; j < m_list->numCols(); j++) {
					values += ((QTable*)obj)->text(i, j);
				}
			}
		}
*/
	}
	delete l; // delete the list, not the objects
	return values;
}
