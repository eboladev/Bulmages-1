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

#include "bndscategoryarticle.h"


BndsCategoryArticle::BndsCategoryArticle() {
    m_codCategoryArticle.assign("");
    m_nomCategoryArticle.assign("");
}


BndsCategoryArticle::~BndsCategoryArticle() {
}


void BndsCategoryArticle::setCodCategoryArticle(string codigo)
{
    m_codCategoryArticle.assign(codigo);
}


void BndsCategoryArticle::setNomCategoryArticle(string nombre)
{
    m_nomCategoryArticle.assign(nombre);
}


string BndsCategoryArticle::codCategoryArticle()
{
    return m_codCategoryArticle;
}


string BndsCategoryArticle::nomCategoryArticle()
{
    return m_nomCategoryArticle;
}


BndsArticle* BndsCategoryArticle::newArticle(string cod = "", string nom = "", string pvp = "")
{
	BndsArticle *art = new BndsArticle();
	art->setIdArticle(cod);
	art->setNomArticle(nom);
	art->setPvpArticle(pvp);

	m_articles.push_back(art);

	return art;
}


list<BndsArticle*> BndsCategoryArticle::articleList()
{
	return m_articles;
}


