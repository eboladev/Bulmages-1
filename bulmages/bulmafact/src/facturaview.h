//
// C++ Interface: factura
//
// Description: 
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef FACTURAVIEW_H
#define FACTURAVIEW_H

#include <facturabase.h>
#include "factura.h"
#include "listlinpresupuestoview.h"

#include <qlineedit.h>
#include <qtextedit.h>
#include <qlabel.h>

#include "postgresiface2.h"

/**
@author Tomeu Borras
*/

class FacturaView : public FacturaBase, public Factura {
Q_OBJECT
private:
    cursor2 *m_cursorcombo;
    
public:
    FacturaView(company *, QWidget *parent = 0, const char *name = 0);
    ~FacturaView();
    void inicialize();
        
void	pintaIdClient(QString id) { m_idclient = id;};	
void	pintaIdAlmacen(QString id) {m_idalmacen = id;};
void	pintaNumFactura(QString id) {m_numfactura->setText(id);};
void	pintaFFactura(QString id) {m_ffactura->setText(id);};
void	pintaContractFactura(QString id) {m_contactfactura->setText(id);};
void	pintaTelFactura(QString id) {m_telfactura->setText(id);};
void	pintaComentFactura(QString id) {m_comentfactura->setText(id);};
void	pintaNomClient(QString id) {m_nomclient->setText(id);};
void	pintaCifClient(QString id) {m_cifclient->setText(id);};
void	pintaCodigoAlmacen(QString id) {m_codigoalmacen-> setText(id);};
void	pintaNomAlmacen(QString id) {m_nomalmacen-> setText(id);};
void    pintaFormaPago(QString);
void    pintatotales(float base, float iva);   

public slots:
    virtual void s_comentfacturatextChanged() { setComentFactura(m_comentfactura->text());};
    virtual void s_codigoalmacentextChanged(const QString &val) {setCodigoAlmacen(val);};
    virtual void s_numfacturatextChanged(const QString &val) {setNumFactura(val);};
    virtual void s_cifclienttextChanged(const QString &val) {setCifClient(val);};
    virtual void s_ffacturatextChanged(const QString &val) {setFFactura(val);};
    virtual void s_contactfacturatextChanged(const QString &val) {setContactFactura(val);};
    virtual void s_telfacturatextChanged(const QString &val) {setTelFactura(val);};
    virtual void s_formapagoactivated(int a) {setFormaPago(m_cursorcombo->valor("idforma_pago",a));};
    
    virtual void s_saveFactura() {guardaFactura();};
    virtual void s_deleteFactura() {borraFactura();};
    virtual void s_printFactura(){};
    virtual void s_searchClient();
    
     /// Este slot se activa cuando hay cambios en los subformularios.
    virtual void s_pintaTotales() {  
   	 pintatotales(listalineas->calculabase(), listalineas->calculaiva());
    }// end pintaTotales
 
};

#endif
