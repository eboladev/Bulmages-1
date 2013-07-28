/***************************************************************************
 *   Copyright (C) 2009 by Aron Galdon                                     *
 *   auryn@wanadoo.es                                                      *
 *                                                                         *
 *   http://www.iglues.org                                                 *
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

#ifndef BLARGPARSER_H
#define BLARGPARSER_H

#include <QtCore/QString>
#include <QtCore/QTextStream>
#include "blconfiguration.h"
#include "blfunctions.h"


class BL_EXPORT BlArgParser
{
private:
    QString m_dbName;
    QString m_executable;
    QString m_host;
    QString m_port;
    QString m_userName;
    QString m_confReplaceString;
    bool m_askPassword;
    bool m_showVersion;
    bool m_showHelp;

public:
    BlArgParser( int argc, char **argv );
    QString dbName();
    QString host();
    QString port();
    QString userName();
    QString confReplaceString();
    bool askPassword();
    bool showVersion();
    bool showHelp();

};


inline QString BlArgParser::confReplaceString()
{
   return m_confReplaceString;
}


inline QString BlArgParser::dbName()
{
   return m_dbName;
}


inline QString BlArgParser::host()
{
      return m_host;
}


inline QString BlArgParser::port()
{
      return m_port;
}


inline QString BlArgParser::userName()
{
      return m_userName;
}


inline bool BlArgParser::askPassword()
{
   return m_askPassword;
}


#endif

