/***************************************************************************
 *Copyright (C) 2004 by Tomeu Borrás Riera                                 *
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
#ifndef importbalance_H
#define importbalance_H

#include <qxml.h>
#include <qptrstack.h>
#include <qmap.h>

class QString;
class empresa;
class postgresiface2;

class importbalance: public QXmlDefaultHandler {
private:
   QString tag;
   QString data;
   QMap<QString, QString> tvalores;
   QMap<QString, QString> identmasasp;  // Este hash sirve para almacenar las claves provisionales de las masas patrimoniales.
   postgresiface2 *conexionbase;
   empresa *empresaactual;
   int paso; // Indica que paso estamos realizando.
public:
    importbalance( empresa * );
    bool startElement( const QString&, const QString&, const QString& , const QXmlAttributes& );
    bool endElement( const QString&, const QString&, const QString& );
    bool startElement1( const QString&, const QString&, const QString& , const QXmlAttributes& );
    bool endElement1( const QString&, const QString&, const QString& );
    bool startElement2( const QString&, const QString&, const QString& , const QXmlAttributes& );
    bool endElement2( const QString&, const QString&, const QString& );
    bool characters (const QString &);
    void cambiapaso() {paso=1;};
};


#endif
