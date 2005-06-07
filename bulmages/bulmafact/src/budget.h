/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borr� Riera                              *
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
#include "postgresiface2.h"

#include "presupuesto.h"
#include "listlinpresupuestoview.h"

#include <qlineedit.h>
#include <qtextedit.h>
#include <qlabel.h>
#include <qcheckbox.h>

/** @author Tomeu Borr� Riera */
class company;

class Budget : public BudgetBase , public presupuesto  {
    Q_OBJECT
public:
    Budget(company *, QWidget *, const char *);
    ~Budget();
    void chargeBudgetLines(QString);
    void chargeBudgetDiscounts(QString);
    void inicialize();
    void manageArticle(int);
    QString searchArticle();

    
    
void	pintaIdClient(QString id) { m_idclient = id;};	
void	pintaIdAlmacen(QString id) {m_idalmacen = id;};
void	pintaNumPresupuesto(QString id) {m_numpresupuesto->setText(id);};
void	pintaFPresupuesto(QString id) {	m_fpresupuesto->setText(id);};
void	pintaVencPresupuesto(QString id) {m_vencpresupuesto->setText(id);};
void	pintaContractPresupuesto(QString id) {m_contactpresupuesto->setText(id);};
void	pintaTelPresupuesto(QString id) {m_telpresupuesto->setText(id);};
void	pintaComentPresupuesto(QString id) {m_comentpresupuesto->setText(id);};
void	pintaNomClient(QString id) {m_nomclient->setText(id);};
void	pintaCifClient(QString id) {m_cifclient->setText(id);};
void	pintaCodigoAlmacen(QString id) {m_codigoalmacen-> setText(id);};
void	pintaNomAlmacen(QString id) {m_nomalmacen-> setText(id);};
void    pintaprocesadopresupuesto(QString id) {
	if (id == "t" || id == "TRUE") m_procesadopresupuesto->setChecked(TRUE);
	else m_procesadopresupuesto->setChecked(FALSE);
};
void pintadescpresupuesto(QString id) {m_descpresupuesto->setText(id);};

void   pintatotales(float base, float iva);

private:
    int insertfpBudget();
    int updatefpBudget();
    int saveBudgetDiscountLines();
    int deleteBudgetLine(int);
    int insertBudgetDiscountLine(int);
    int updateBudgetDiscountLine(int);
    int deleteBudgetDiscountLine(int);
    void nextCell(QObject *);
    void antCell(QObject *);
    void duplicateCell(QObject *);
    void calculateImports();
    QString retrieveValues(QString);
    QString calculateValues();
    void cargarcomboformapago(QString);
    void buscarAlmacen();
    QString newBudgetNumber();
    void presentaReports();

public slots:
    virtual void searchClient();
    virtual void budgetDateLostFocus();
    virtual void budgetExpiryLostFocus();
    virtual void newBudgetLine();
    virtual void removeBudgetLine();
    virtual void newBudgetDiscountLine();
    virtual void removeBudgetDiscountLine();
    virtual void valueBudgetDiscountLineChanged(int, int);
    virtual void accept();
    virtual void cancel();
    virtual void s_saveBudget() {guardapresupuesto();};
    virtual void s_removeBudget();
    virtual void s_contextMenuDiscount(int, int, int, const QPoint &);
    virtual void s_almacenLostFocus();
    virtual void s_printBudget();
    virtual void s_removeBudgetLine() {subform2->borralinpresupuestoact();};
    
    
    virtual void s_comentariotextChanged() { setComentPresupuesto(m_comentpresupuesto->text());};
    virtual void s_cifclienttextChanged(const QString &str) {setCifClient(str);};
    virtual void s_codigoalmacentextChanged(const QString &str) {setCodigoAlmacen(str);};
    virtual void s_contactotextChanged(const QString &str) {setContractPresupuesto(str);};
    virtual void s_telpresupuestotextChanged(const QString &str) {setTelPresupuesto(str);};
    virtual void s_procesadopresupuestostateChanged(int i) {
    	if (i) setprocesadopresupuesto("TRUE");
	else setprocesadopresupuesto("FALSE");
    }    
    virtual void s_descpresupuestotextChanged(const QString &str) {setdescpresupuesto(str);};
	/// Este slot se activa cuando cambia la fecha del presupuesto.
    virtual void s_fpresupuestotextChanged(const QString &str) {/*setFPresupuesto(str);*/};
    
    	/// Este slot se activa cuando hay cambios en los subformularios.
    virtual void s_pintaTotales() {  
   	 pintatotales(listalineas->calculabase(), listalineas->calculaiva());
    }// end pintaTotales
};

#endif
