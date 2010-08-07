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

#ifndef BNDSDB_H
#define BNDSDB_H

#include <nds.h>
#include <fat.h>
#include <stdio.h>
#include <string>
#include <list>

using namespace std;

#include "tinyxml.h"
#include "bndssocket.h"
#include "bndsarticle.h"
#include "bndscategoryarticle.h"
#include "bndsticket.h"


class BndsDb
{

	BndsSocket *m_bndsSocket;
	list<BndsCategoryArticle*> m_categoryArticles;
	list<BndsTicket*> m_tickets;
	BndsTicket* m_currentTicket;
	string m_datos;

protected:
	void processCategoryArticles(string categoryArticlesData);

public:
	BndsDb();
	~BndsDb();
	int initConnection();
	void loadConfigurationFromFile();
	void saveConfigurationToFile();
	BndsSocket *linkSocket();
	int loadData();
	BndsCategoryArticle* newCategoryArticle(string, string);
	list<BndsCategoryArticle*> categoryArticleList();
	list<BndsTicket*> ticketsList();
	void addTicket(BndsTicket *ticket);
	BndsTicket* currentTicket();
	void saveTicket(string ticketData);
};


extern BndsDb *g_db;


#endif
