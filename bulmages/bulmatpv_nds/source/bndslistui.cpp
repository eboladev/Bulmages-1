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

#include "bndslistui.h"


BndsListUi::BndsListUi()
{
    /// Siempre tenenos, al menos 1 pagina de datos, aunque este vacia de contenido.
    m_currentPage = 1;
    m_totalPages = 1;
    m_totalItems = 0;
    m_totalItemsCurrentPage = 0;
    m_maxItemsPerPage = 20;
    m_currentLine = 0;
}


BndsListUi::~BndsListUi()
{
}


void BndsListUi::setTotalItems(int items)
{
    m_totalItems = items;
    m_totalPages = items % m_maxItemsPerPage > 0 ? (items / m_maxItemsPerPage) + 1 : items / m_maxItemsPerPage;
    firstPage();
}


void BndsListUi::show()
{
}


int BndsListUi::exec()
{
      return 0;
}


void BndsListUi::nextPage()
{
    m_currentPage++;

    /// Si seleccionamos la pagina posterior a la ultima (que no existe)
    /// entonces vamos a la primera.
    if (m_currentPage > m_totalPages) {
	m_currentPage = 1;
    } // end if

    calculateTotalItemsCurrentPage();
    setCurrentLine(1);

    show();
}


void BndsListUi::previousPage()
{
    m_currentPage--;
    
    /// Si seleccionamos la pagina anterior a la principal (que no existe)
    /// entonces vamos a la ultima.
    if (m_currentPage <= 0) {
	m_currentPage = m_totalPages;
    } // end if

    calculateTotalItemsCurrentPage();
    setCurrentLine(1);

    show();
}


void BndsListUi::firstPage()
{
    m_currentPage = 1;

    calculateTotalItemsCurrentPage();
    setCurrentLine(1);

    show();
}

void BndsListUi::lastPage()
{
    m_currentPage = m_totalPages;
    
    calculateTotalItemsCurrentPage();
    setCurrentLine(1);
    
    show();
}


bool BndsListUi::isFirstPage()
{
    if (m_currentPage == 1) {
	return true;
    } else {
	return false;
    } // end if
}


bool BndsListUi::isLastPage()
{
    if (m_currentPage == m_totalPages) {
	return true;
    } else {
	return false;
    } // end if
}


void BndsListUi::calculateTotalItemsCurrentPage()
{
    /// A reimplementar.
}


void BndsListUi::setTotalItemsCurrentPage(int items)
{
    m_totalItemsCurrentPage = items;
}


void BndsListUi::lineUp()
{
    m_currentLine--;
    
    if (m_totalItemsCurrentPage >= 1) {
	if (m_currentLine <= 1) m_currentLine = 1;
    } else if (m_totalItemsCurrentPage == 0) {
	m_currentLine = 0;
    } // end if
    
    show();
}


void BndsListUi::lineDown()
{
    m_currentLine++;

    if (m_totalItemsCurrentPage == 0) {
	m_currentLine = 0;
    } else if (m_currentLine > m_totalItemsCurrentPage) {
	m_currentLine = m_totalItemsCurrentPage;
    } // end if

    show();
}


void BndsListUi::setCurrentLine(int line)
{
    m_currentLine = line;

    /// Selecciona el primer elemento de la lista si existe.
    if (m_totalItemsCurrentPage >= 1) {
	m_currentLine = 1;
    } else {
	m_currentLine = 0;
    } // end if

}


int BndsListUi::currentLine()
{
    return m_currentLine;
}


int BndsListUi::currentPage()
{
    return m_currentPage;
}


int BndsListUi::currentItem()
{
    /// Devuelve el elemento actual de la lista completa.
    return (m_maxItemsPerPage * m_currentPage) - m_maxItemsPerPage + m_currentLine;
}

