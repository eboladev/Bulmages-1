

#include <QLineEdit>
#include <QTextEdit>

#include "dialogchanges.h"
#include "funcaux.h"


void dialogChanges::dialogChanges_cargaInicial() {
    valorinicial = calculateValues();
}


bool dialogChanges::dialogChanges_hayCambios() {
    return !(valorinicial == calculateValues());
}


QString dialogChanges::calculateValues() {
    _depura("dialogChanges::calculateValues",0);
    QString values = retrieveValues("QTable");
    values += retrieveValues("QLineEdit");
    values += retrieveValues("QTextEdit");
    _depura("END dialogChanges::calculateValues",0);
    return values;
}

QString dialogChanges::retrieveValues(QString qsWidget) {
    _depura("dialogChanges::retrieveValues",0);
    QObjectList l = obje->queryList( qsWidget );
    QListIterator<QObject *> it (l);
    QObject *obj;
    QString values="";
    while ( it.hasNext() ) {
        obj = it.next();
        //		++it;
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
    //	delete l; // delete the list, not the objects
    _depura("END dialogChanges::retrieveValues",0);
    return values;
}
