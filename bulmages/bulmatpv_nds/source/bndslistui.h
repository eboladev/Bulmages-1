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

#ifndef BNDSLISTUI_H
#define BNDSLISTUI_H

#include <nds.h>
#include <stdio.h>
#include <string>
#include <list>

using namespace std;


class BndsListUi
{
  protected:
    int m_currentPage;
    int m_totalPages;
    int m_totalItems;
    int m_totalItemsCurrentPage;
    int m_maxItemsPerPage;
    int m_currentLine;
  
  public:
    BndsListUi();
    ~BndsListUi();
    void setTotalItems(int);
      
    /// En pantalla:
    virtual void show();
    virtual int exec();
    virtual void nextPage();
    virtual void previousPage();
    virtual void firstPage();
    virtual void lastPage();
    virtual bool isFirstPage();
    virtual bool isLastPage();
    virtual void setTotalItemsCurrentPage(int item);
    virtual void calculateTotalItemsCurrentPage();
    virtual void lineUp();
    virtual void lineDown();
    virtual void setCurrentLine(int line);
    virtual int currentLine();
    virtual int currentPage();
    virtual int currentItem();
};


#endif

