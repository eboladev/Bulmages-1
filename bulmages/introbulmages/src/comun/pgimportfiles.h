/***************************************************************************
 *   Copyright (C) 2003 by Santiago Capel                                  *
 *   Santiago Capel Torres (GestiONG)                                      *
 *   Tomeu Borrás Riera                                                    *
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
 ***************************************************************************/
#ifndef __PGIMPORTFILES__
#define __PGIMPORTFILES__

#include <qfile.h>
#include <qstring.h>
#include <qxml.h>

#include "postgresiface2.h"

class pgimportfiles {
private:
	postgresiface2 *conexionbase;
	void (*alerta)(int,int);
	void (*mensajeria) (QString);
	bool m_modoTest;
	QString m_fInicial;
	QString m_fFinal;
public:

	void setFInicial(QString f) {m_fInicial = f;};
	void setFFinal(QString f) {m_fFinal = f;};
	void setModoTest() {m_modoTest=TRUE;};
	void setModoNormal() {m_modoTest=FALSE;};
	bool modoTest() {return m_modoTest == TRUE;};
	pgimportfiles(postgresiface2 *, void(*)(int,int), void(*) (QString));
	~pgimportfiles(){};
	int contaplus2Bulmages(QFile &, QFile &);
	int bulmages2Contaplus(QFile &, QFile &);
	int bulmages2XML(QFile &);
	int XML2Bulmages(QFile &);
	QString searchParent(QString);
};




class StructureParser : public QXmlDefaultHandler
{
public:
    bool startDocument();
    bool startElement( const QString&, const QString&, const QString& ,
                       const QXmlAttributes& );
    bool endElement( const QString&, const QString&, const QString& );
    bool characters (const QString&);

private:
    QString indent;
};


#endif
