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
    m_modifier = 0;
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
    m_subTotalLine = m_quantArticle * m_article->pvpArticle();
}


void BndsTicketLine::setPvpArticle(string pvp)
{
    m_article->setPvpArticle(pvp);
    recalculeSubTotalLine();
}


void BndsTicketLine::setModifier(BndsModifier* mod)
{    
    m_modifier = mod;
}


BndsModifier* BndsTicketLine::getModifier()
{
    return m_modifier;
}


void BndsTicketLine::removeModifier()
{
      /// Si existe un modificador se borra.
      if (m_modifier != 0) {
	delete m_modifier;
      } // end if
}

 
void BndsTicketLine::setArticle(BndsArticle *art)
{
    m_article = art;
}


BndsArticle* BndsTicketLine::article()
{
    return m_article;
}

    