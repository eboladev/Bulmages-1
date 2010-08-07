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

#include "bndsdb.h"
#include "bndsfunctions.h"


BndsDb *g_db = NULL;


BndsDb::BndsDb()
{
  	m_bndsSocket = NULL;
	
	/// Inicia el sistema de archivos FAT donde leer y escribir configuracion y otros datos.
	fatInitDefault();
}


BndsDb::~BndsDb()
{
}


BndsSocket *BndsDb::linkSocket() {
    return m_bndsSocket;
}


int BndsDb::loadData()
{
	processCategoryArticles( m_datos );
	
	return 0;
}


int BndsDb::initConnection()
{
	m_bndsSocket = new BndsSocket();

	m_bndsSocket->startWifi();

#ifdef DEMO
	m_datos = m_bndsSocket->pullCategoryArticles();
#else
	string mensaje =  "<DOCUMENT><GETCOMMAND>categoria_articulo</GETCOMMAND></DOCUMENT>"; 
	m_bndsSocket->openServerSocket();
	m_bndsSocket->sendDataToServer(mensaje);
	m_datos = m_bndsSocket->readDataFromServer();
	m_bndsSocket->closeServerSocket();
#endif
	
	return 0;
}


void BndsDb::processCategoryArticles(string categoryArticlesData)
{
	/// Recorre todos los datos creando categorias y sus articulos.
	int i = categoryArticlesData.size() + 1;

	tinyxml_t* tinyxml = tinyxml_new(i, callback_processCategoryArticles, 0);
	tinyxml_feed(tinyxml, categoryArticlesData.c_str(), i );
	tinyxml_free(tinyxml);
}


BndsCategoryArticle* BndsDb::newCategoryArticle(string cod = "", string nom = "")
{
	BndsCategoryArticle *categArt = new BndsCategoryArticle();
	categArt->setCodCategoryArticle(cod);
	categArt->setNomCategoryArticle(nom);

	m_categoryArticles.push_back(categArt);

	return categArt;
}



list<BndsCategoryArticle*> BndsDb::categoryArticleList()
{
	return m_categoryArticles;
}


list<BndsTicket*> BndsDb::ticketsList()
{
	return m_tickets;
}



void BndsDb::addTicket(BndsTicket *ticket)
{
      m_currentTicket = ticket;
      m_tickets.push_back(ticket);

}


BndsTicket* BndsDb::currentTicket()
{
    return m_currentTicket;
}


void BndsDb::saveTicket(string ticketData)
{

#ifdef DEMO

#else

	m_bndsSocket->openServerSocket();
	m_bndsSocket->sendDataToServer(ticketData);
	m_bndsSocket->closeServerSocket();

#endif

}

