#include "EventHandler.h"
#include <qwidget.h>
#include <qtable.h>
#include <qlineedit.h>
#include "funcaux.h"


EventHandler::EventHandler(QWidget *EH, const int textEditType=0 ){
	m_TextEditType=textEditType;
};


EventHandler::EventHandler(QWidget *EH, QTable *table ){
	
};

EventHandler::~EventHandler(){
};

bool EventHandler::eventFilter( QObject *obj, QEvent *ev ) {
	if ( obj->isA("QTable")) {
		if ( ev->type() == QEvent::KeyPress ) {
			QKeyEvent *k = (QKeyEvent *)ev;
			switch (k->key()) {
				case Qt::Key_Enter:  {
					nextCell(obj);
					return TRUE;
				}
				case Qt::Key_Return: {
					nextCell(obj);
					return TRUE;
				}
				case Qt::Key_Asterisk: {
					qDebug( "ASTERISCO" );
					duplicateCell(obj);
					return TRUE;
				}
			} 
		}
	} 
	
	if ( obj->isA("QLineEdit")) {
		if ( ev->type() == QEvent::FocusOut ) {
			switch ( m_TextEditType ) {
				case QUANTITY:  {
					
					return TRUE;
				}
				case DATE: {
					manageDate(obj);
					return TRUE;
				}
			} 
		}
	}
	
	return FALSE;
} //end eventFilter


void EventHandler::nextCell(QObject *obj) {
	QTable *t = (QTable *)obj;
	int row = t->currentRow();
	int col = t->currentColumn();
	int lastCol = t->numCols()-1;
	if (t->isReadOnly()==FALSE) {
		//qDebug( "Fila, Columna = %d, %d", row, col);
		col++;
		while (true) {
			qDebug( "Fila, Columna = %d, %d", row, col);
			if (col > lastCol) {
				col = 0;
				row++;
				if (row == (t->numRows())) {
					t->setNumRows(row+1);
				}
			} else {
				if (t->isColumnHidden(col) || t->isColumnReadOnly(col) || t->isRowHidden(row) || t->isRowReadOnly(row)) {
					col++;
				} else {
					t->setCurrentCell(row, col);
					//t->editCell(row, col);
					break;
				}
			}
		}
	}
}


void EventHandler::duplicateCell(QObject *obj) {
	QTable *t = (QTable *)obj;
	int row = t->currentRow();
	int col = t->currentColumn();
	if (t->text(row, col) == "" && row>0) {
		t->setText(row, col, t->text(row-1, col));
	}
}


void EventHandler::manageDate(QObject *obj) {
	QLineEdit *t = (QLineEdit *)obj;
	t->setText(normalizafecha(t->text()).toString("dd/MM/yyyy"));
}


void EventHandler::manageQuantity(QObject *obj) {
	QLineEdit *t = (QLineEdit *)obj;
	t->setText(t->text().replace(",","."));
}


