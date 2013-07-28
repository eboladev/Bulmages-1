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

#include <sys/types.h>
#include <netinet/in.h>

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
    BndsConfigView *configview = new BndsConfigView(this);
    configview->exec();
}


void BndsConfig::loadConfigurationFromFile()
{

#ifdef DEMO
      string datosConf = "<CONFIG> \
		      <ACCESSPOINTSSID></ACCESSPOINTSSID> \
		      <ACCESSPOINTWEP128KEY></ACCESSPOINTWEP128KEY> \
		      <IPSERVER></IPSERVER> \
		      <POSUSER></POSUSER> \
		      </CONFIG>";
		      
	processConfigData(datosConf);
	
#else
  
      /// Inicia la consola.
      PrintConsole conSub = g_video->consoleSub();
      consoleSelect( &conSub );
      
      consoleClear();

      string confFileName = "bnds.conf";
      FILE* confFile;
      
      if ( !(confFile = fopen (confFileName.c_str(), "r") )) {
	  /// El archivo no existe. Se debe configurar el programa
	  /// antes de usarlo.
      } else {
	confFile = fopen(confFileName.c_str(),"r");

	if (confFile == NULL){
	    iprintf("Error al abrir el archivo de configuracion,");
	    exit(1);
	} // end if
	
	fseek(confFile, 0, SEEK_END);
	unsigned int fileSize = ftell(confFile);
	rewind(confFile);

	char* data = (char*) malloc (sizeof(char) * fileSize);
	
	fread(data, 1, fileSize, confFile);

	if (ferror(confFile)){
	    iprintf("Error al leer el archivo de configuracion.");
	    exit(1);
	} // end if

	string datosConf(data);
	
	processConfigData(datosConf);
	  
      } // end if
      
      fclose(confFile);
      
#endif
}


void BndsConfig::saveConfigurationToFile()
{
      string confFileName = "bnds.conf";
      FILE* confFile;
     
      /// El archivo se crea con los datos
      /// de la configuracion del programa.
      confFile = fopen(confFileName.c_str(),"w");

      string accesspointssid = "	<ACCESSPOINTSSID>" + accessPointSsid() + "</ACCESSPOINTSSID>\n";
      string accessoointwep128key = "	<ACCESSPOINTWEP128KEY>" + accessPointWep128Key() + "</ACCESSPOINTWEP128KEY>\n";
      string ipserver = "	<IPSERVER>" + ipServer() + "</IPSERVER>\n";
      string posuser =  "	<POSUSER>" + posUser() + "</POSUSER>\n";
      
      fprintf(confFile, "<CONFIG>\n");
      fprintf(confFile, accesspointssid.c_str());
      fprintf(confFile, accessoointwep128key.c_str());
      fprintf(confFile, ipserver.c_str());
      fprintf(confFile, posuser.c_str());
      fprintf(confFile, "</CONFIG>\n");
      
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


Wifi_AccessPoint* BndsConfig::findAP(string ssidName)
{

	static Wifi_AccessPoint ap;
	
	Wifi_InitDefault(false);
	
	Wifi_ScanMode(); //this allows us to search for APs

	/// Inicia la consola.
	PrintConsole conSub = g_video->consoleSub();
	consoleSelect( &conSub );
	
	consoleClear();
	
#ifdef DEMO

#else

if (ssidName != "") {
  	
	int selected = 0;  
	int i;
	int count = 0;
	bool autoSelected = false;

	string cadena = "Buscando la red: " + ssidName + " (espere)";
	
	iprintf(cadena.c_str());

	while(1)
	{
		//find out how many APs there are in the area
		count = Wifi_GetNumAP();
		for(i = 0; i < count; i++)
		{
			Wifi_AccessPoint ap;

			Wifi_GetAPData(i, &ap);
			
			if (string(ap.ssid) == ssidName) {
			    autoSelected = true;
			    selected = i;
			    break;
			} // end if

		}

		if (autoSelected) {
		    break;
		} // end if
		swiWaitForVBlank();
	} // end while
	
	//user has made a choice so grab the ap and return it
	Wifi_GetAPData(selected, &ap);
	
} else {

	int selected = 0;  
	int i;
	int count = 0;

	while(1)
	{
		scanKeys();

		//find out how many APs there are in the area
		count = Wifi_GetNumAP();

		consoleClear();

		iprintf("Numero de AP encontrados: %d\n", count);

		//display the APs to the user
		for(i = 0; i < count; i++)
		{
			Wifi_AccessPoint ap;

			Wifi_GetAPData(i, &ap);

			// display the name of the AP
			iprintf("%s %s Wep:%s Sig:%i\n", 
				i == selected ? "*" : " ", 
				ap.ssid, 
				ap.flags & WFLAG_APDATA_WEP ? "Si " : "No ",
				ap.rssi * 100 / 0xD0);

		}

		//move the selection asterick
		if(keysDown() & KEY_UP)
		{
			selected--;
			if(selected < 0)
			{
				selected = 0;
			} // end if
		} // end if

		if(keysDown()&KEY_DOWN)
		{
			selected++;
			if(selected >= count) 
			{
				selected = count - 1;
			} // end if
		} // end if

		if(keysDown() & KEY_A)
		{
		    /// Red seleccionada de la lista.
		    break;
		} // end if
		swiWaitForVBlank();
	} // end while

	//user has made a choice so grab the ap and return it
	Wifi_GetAPData(selected, &ap);

} // end if
	
#endif

	return &ap;
}



