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
#ifndef importainteligente_H
#define importainteligente_H

#include <qxml.h>
#include <qptrstack.h>
#include <qmap.h>

class QString;
class empresa;
class postgresiface2;

class importainteligente: public QXmlDefaultHandler {
private:
   QString tag;
   QString data;
   QMap<QString, QString> tvalores;
   postgresiface2 *conexionbase;
   empresa *empresaactual;
public:
    importainteligente( empresa * );
    bool startElement( const QString&, const QString&, const QString& , const QXmlAttributes& );
    bool endElement( const QString&, const QString&, const QString& );
    bool characters (const QString &);
};


#endif
