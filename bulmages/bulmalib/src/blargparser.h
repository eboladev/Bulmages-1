/***************************************************************************
 *   Copyright (C) 2009 by Aron Galdon                               *
 *   auryn@wanadoo.es                                               *
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

#include <QString>
#include <QTextStream>
#include "blfunctions.h"

class BlArgParser
{
public:
   BlArgParser( int argc, char **argv );
   QString DbName();
   QString Host();
   QString Port();
   QString UserName();
   bool AskPassword();
   bool ShowVersion();
   bool ShowHelp();
 private:
    QString m_dbName;
    QString m_host;
    QString m_port;
    QString m_userName;
    bool m_askPassword;
    bool m_showVersion;
    bool m_showHelp;
};

inline QString BlArgParser::DbName()
{
   return( m_dbName );
}

inline QString BlArgParser::Host()
{
      return( m_host );
}

inline QString BlArgParser::Port()
{
      return( m_port );
}

inline QString BlArgParser::UserName()
{
      return( m_userName );
}

inline bool BlArgParser::AskPassword()
{
   return( m_askPassword );
}

inline bool BlArgParser::ShowVersion()
{
   return( m_showVersion );
}

inline bool BlArgParser::ShowHelp()
{
   return( m_showHelp );
}

#endif // BLARGPARSER_H
