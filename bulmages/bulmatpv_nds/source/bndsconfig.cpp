/***************************************************************************
 *                                                                         *
 *   BulmaTPV remote client for Nintendo DS console.                       *
 *   -----------------------------------------------                       *
 *                                                                         *
 *   Copyright (C) 2010 by Fco. Javier M. C.                               *
 *   comercial@todo-redes.com                                              *
 *   http://www.todo-redes.com                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 ***************************************************************************/

#include "bndsconfig.h"
#include "bndsfunctions.h"
#include "bndsconfigview.h"


BndsConfig::BndsConfig()
{
    m_ipserver = "";
    m_posuser = "";
    m_accesspointssid = "";
    m_accesspointwep128key = "";
}


BndsConfig::~BndsConfig()
{
}

void BndsConfig::editConfig()
{
    BndsConfigView *configview = new BndsConfigView();
    configview->exec();
}


void BndsConfig::loadConfigurationFromFile()
{
      string confFileName = "bnds.conf";
      FILE* confFile;
      
      if ( !(confFile = fopen (confFileName.c_str(), "r") )) {
	  /// El archivo no existe. Se crea en modo de escritura.
	  confFile = fopen(confFileName.c_str(),"w");
	  
	  fprintf(confFile, "<CONFIG>\n");
	  fprintf(confFile, "	<IPSERVER></IPSERVER>\n");
	  fprintf(confFile, "	<USER></USER>\n");
	  fprintf(confFile, "	<ACCESSPOINTSSID></ACCESSPOINTSSID>\n");
	  fprintf(confFile, "	<ACCESSPOINTWEP128KEY></ACCESSPOINTWEP128KEY>\n");
	  fprintf(confFile, "</CONFIG>\n");

      } else {
	
	  
	  
      } // end if
      
      fclose(confFile);
}


void BndsConfig::saveConfigurationToFile()
{
      string confFileName = "bnds.conf";
      FILE* confFile;
      
      if( !(confFile = fopen (confFileName.c_str(), "r") )) {
	  /// El archivo no existe. Se crea en modo de escritura.
	  confFile = fopen(confFileName.c_str(),"w");
	  
	  fprintf(confFile, "<CONFIG>\n");
	  fprintf(confFile, "	<IPSERVER></IPSERVER>\n");
	  fprintf(confFile, "	<POSUSER></POSUSER>\n");
	  fprintf(confFile, "	<ACCESSPOINTSSID></ACCESSPOINTSSID>\n");
	  fprintf(confFile, "	<ACCESSPOINTWEP128KEY></ACCESSPOINTWEP128KEY>\n");
	  fprintf(confFile, "</CONFIG>\n");

      } // end if
      
      fclose(confFile);
}


void BndsConfig::processConfigData(string configdata)
{
	/// Recorre todos los datos creando categorias y sus articulos.
	int i = configdata.size() + 1;

	tinyxml_t* tinyxml = tinyxml_new(i, callback_processConfigFile, 0);
	tinyxml_feed(tinyxml, configdata.c_str(), i );
	tinyxml_free(tinyxml);
}


void BndsConfig::setIpServer(string ip)
{
   m_ipserver = ip;
}


string BndsConfig::ipServer()
{
    return m_ipserver;
}


void BndsConfig::setPosUser(string posUser)
{
    m_posuser = posUser;
}


string BndsConfig::posUser()
{
    return m_posuser;
}


void BndsConfig::setAccessPointSsid(string ssid)
{
    m_accesspointssid = ssid;
}


string BndsConfig::accessPointSsid()
{
    return m_accesspointssid;
}


void BndsConfig::setAccessPointWep128Key(string wep128key)
{
    m_accesspointwep128key = wep128key;
}


string BndsConfig::accessPointWep128Key()
{
    return m_accesspointwep128key;
}


