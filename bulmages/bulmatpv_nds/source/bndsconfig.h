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

#ifndef BNDSCONFIG_H
#define BNDSCONFIG_H

#include <nds.h>
#include <stdio.h>
#include <string>
#include <dswifi9.h>

using namespace std;


class BndsConfig
{
private:
	string m_ipserver;
	string m_posuser;
	string m_accesspointssid;
	string m_accesspointwep128key;
  
public:
	Wifi_AccessPoint* m_ap;

	BndsConfig();
	~BndsConfig();
	
	void editConfig();
	
	void saveConfigurationToFile();
	void loadConfigurationFromFile();

	void processConfigData(string configdata);
	
	void setIpServer(string ip);
	string ipServer();

	void setPosUser(string posuser);
	string posUser();
	
	void setAccessPointSsid(string ssid);
	string accessPointSsid();
	
	void setAccessPointWep128Key(string wep128key);
	string accessPointWep128Key();
	
	Wifi_AccessPoint* findAP(string ssidName = "");

};


#endif

