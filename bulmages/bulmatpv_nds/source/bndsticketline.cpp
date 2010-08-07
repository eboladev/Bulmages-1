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

#include "bndsticketline.h"


BndsTicketLine::BndsTicketLine()
{
    m_quantArticle = 0;
    m_subTotalLine = 0.0;
}


BndsTicketLine::~BndsTicketLine()
{
}


int BndsTicketLine::quantityArticle()
{
    return m_quantArticle;
}


void BndsTicketLine::setQuantityArticle(int quantity)
{
    m_quantArticle = quantity;
    recalculeSubTotalLine();
}


float BndsTicketLine::subTotalLine()
{
    return m_subTotalLine;
}



void BndsTicketLine::recalculeSubTotalLine()
{
    m_subTotalLine = m_quantArticle * pvpArticle();
}


void BndsTicketLine::setPvpArticle(string pvp)
{
    BndsArticle::setPvpArticle(pvp);
    recalculeSubTotalLine();
}