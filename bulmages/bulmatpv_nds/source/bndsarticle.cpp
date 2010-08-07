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

#include "bndsarticle.h"


BndsArticle::BndsArticle() {
    m_idArticle.assign("");
    m_nomArticle.assign("");
    m_pvpArticle.assign("");
}


BndsArticle::~BndsArticle() {
}


void BndsArticle::setIdArticle(string id)
{
    m_idArticle.assign(id);
}


void BndsArticle::setNomArticle(string nombre)
{
    m_nomArticle.assign(nombre);
}


void BndsArticle::setPvpArticle(string pvp)
{
    m_pvpArticle.assign(pvp);
}


string BndsArticle::idArticle()
{
    return m_idArticle;
}


string BndsArticle::nomArticle()
{
    return m_nomArticle;
}


float BndsArticle::pvpArticle()
{
    return atof( m_pvpArticle.c_str() );
}



