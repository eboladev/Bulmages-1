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
private:
    cursor2 *m_cursorcombo;
public:
    Budget(company *, QWidget *, const char *);
    ~Budget();
    void inicialize();
    void manageArticle(int);

    
    
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
void    pintarefpresupuesto(QString id) {m_refpresupuesto->setText(id);};
void    pintaidforma_pago(QString);
void    pintaprocesadopresupuesto(QString id) {
	if (id == "t" || id == "TRUE") m_procesadopresupuesto->setChecked(TRUE);
	else m_procesadopresupuesto->setChecked(FALSE);
};
void pintadescpresupuesto(QString id) {m_descpresupuesto->setText(id);};

void   pintatotales(float base, float iva);

private:
//    void cargarcomboformapago(QString);
    void buscarAlmacen();
    void generarPedidoCliente();

public slots:
    virtual void budgetDateLostFocus();
    virtual void budgetExpiryLostFocus();
    virtual void searchClient();
    virtual void s_saveBudget() {guardapresupuesto();};
    virtual void s_removeBudget();
    virtual void s_almacenLostFocus();
    virtual void s_printBudget();
    virtual void s_removeBudgetLine() {subform2->borralinpresupuestoact();};
    
    virtual void s_comentariotextChanged() { setComentPresupuesto(m_comentpresupuesto->text());};
    virtual void s_cifclienttextChanged(const QString &str) {setCifClient(str);};
    virtual void s_codigoalmacentextChanged(const QString &str) {setCodigoAlmacen(str);};
    virtual void s_contactotextChanged(const QString &str) {setContractPresupuesto(str);};
    virtual void s_telpresupuestotextChanged(const QString &str) {setTelPresupuesto(str);};
    virtual void s_refpresupuestotextChanged(const QString &str) {setrefpresupuesto(str);};
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
    
    virtual void s_realizarPedidoCliente() {generarPedidoCliente();};
    virtual void s_idforma_pagoactivated(int a) {setidforma_pago(m_cursorcombo->valor("idforma_pago",a));};
};

#endif
