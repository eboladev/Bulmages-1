/***************************************************************************
 *   Copyright (C) 2005 by Alvaro de Miguel                                *
 *   alvaro.demiguel@gmail.com                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef CLIENTDELIVNOTE_H
#define CLIENTDELIVNOTE_H

// PRESUPUESTOS.

#include "clientdelivnotebase.h"
#include "postgresiface2.h"

/** @author Alvaro de Miguel */
class company;


class ClientDelivNote : public ClientDelivNoteBase {
Q_OBJECT
private:
	company *companyact;
	QString m_idalbaran;
	QString m_idclient;
	QString m_initialValues;
	cursor2 *m_cursorcombo;
	
public:
	ClientDelivNote(company *, QWidget *, const char *);
	~ClientDelivNote();
	void chargeClientDelivNote(QString);
	void chargeClientDelivNoteLines(QString);
	 void chargeClientDelivNoteDiscounts(QString);
	 void inicialize();
	 void manageArticle(int);
	 QString searchArticle();
	
	 
private:
	int saveClientDelivNote();
	int saveClientDelivNoteLines();
	int saveClientDelivNoteDiscountLines();
	int insertClientDelivNoteLine(int);
	int updateClientDelivNoteLine(int);
	int deleteClientDelivNoteLine(int);
	int insertClientDelivNoteDiscountLine(int);
	int updateClientDelivNoteDiscountLine(int);
	int deleteClientDelivNoteDiscountLine(int);
	void nextCell(QObject *);
	void antCell(QObject *);
	void duplicateCell(QObject *);
	void calculateImports();
	QString retrieveValues(QString);
	QString calculateValues();
	void cargarcomboformapago(QString);
		
protected:
	bool eventFilter( QObject *, QEvent *);
	 
public slots:
   virtual void s_searchClient();
	virtual void s_clientDelivNoteDateLostFocus();
	virtual void s_newClientDelivNoteLine();
	virtual void s_removeClientDelivNoteLine();
	virtual void s_newClientDelivNoteDiscountLine();
	virtual void s_removeClientDelivNoteDiscountLine();
	virtual void s_valueClientDelivNoteLineChanged(int, int);
	virtual void s_valueClientDelivNoteDiscountLineChanged(int, int);
	virtual void s_accept();
	virtual void s_cancel();
	virtual void s_saveClientDelivNote();
	virtual void s_removeClientDelivNote();
	virtual void s_contextMenu(int, int, int, const QPoint &);
	virtual void s_contextMenuDiscount(int, int, int, const QPoint &);

};

#endif
