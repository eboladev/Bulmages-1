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

#ifndef BNDSTICKET_H
#define BNDSTICKET_H

#include <nds.h>
#include <stdio.h>
#include <string>
#include <list>

using namespace std;

#include "bndsticketline.h"

class BndsDb;

class BndsTicket
{
protected:
    list<BndsTicketLine*> m_ticketLines;
    string m_nomTicket;
    float m_total;
  
public:
    BndsTicket();
    ~BndsTicket();
    string defaultTicketName();
    void addArticle(BndsArticle* article, BndsModifier* modifier = 0);
    void setNomTicket(string nombre);
    string nomTicket();
    float total();
    void recalculeTotal();
    list<BndsTicketLine*> ticketLines();
    void removeTicketLine(BndsTicketLine *ticketLine);
    void clearTicket();
    string ticket2xml(bool envia = false);
};


#endif