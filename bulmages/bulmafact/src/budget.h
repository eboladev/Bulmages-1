/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borrás Riera                              *
 *   tborras@conetxia.com                                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef BUDGET_H
#define BUDGET_H

// PRESUPUESTOS.

#include "budgetbase.h"

/** @author Tomeu Borrás Riera */
class company;


class Budget : public BudgetBase {
Q_OBJECT
private:
   company *companyact;
   QString m_idpresupuesto;
   QString m_idclient;
	bool m_modified;
	
public:
    Budget(company *, QWidget *, const char *);
    ~Budget();
    void chargeBudget(QString);
	 void chargeBudgetLines(QString);
	 void chargeBudgetDiscounts(QString);
	 void inicialize();
	 void manageArticle(int);
	 QString searchArticle();
	
	 
private:
	int saveBudget();
	int saveBudgetLines();
	int saveBudgetDiscountLines();
	int insertBudgetLine(int);
	int updateBudgetLine(int);
	int deleteBudgetLine(int);
	int insertBudgetDiscountLine(int);
	int updateBudgetDiscountLine(int);
	int deleteBudgetDiscountLine(int);
	void nextCell(QObject *);
	void antCell(QObject *);
	void duplicateCell(QObject *);
	void calculateImports();
	void installEventFilters(QString);
		
protected:
	bool eventFilter( QObject *, QEvent *);
	 
public slots:
   virtual void searchClient();
	virtual void budgetDateLostFocus();
	virtual void budgetExpiryLostFocus();
	virtual void newBudgetLine();
	virtual void removeBudgetLine();
	virtual void newBudgetDiscountLine();
	virtual void removeBudgetDiscountLine();
	virtual void valueBudgetLineChanged(int, int);
	virtual void valueBudgetDiscountLineChanged(int, int);
	virtual void accept();
	virtual void cancel();
	virtual void s_saveBudget();

};

#endif
