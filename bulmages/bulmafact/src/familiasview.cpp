//
// C++ Implementation: %{MODULE}
//
// Description:
//
//
// Author: %{AUTHOR} <%{EMAIL}>, (C) %{YEAR}
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "familiasview.h"
#include "company.h"
#include <qlistview.h>
#include <qmap.h>
#include <qlineedit.h>
#include <qtextedit.h>
#include <qmessagebox.h>

#define COL_NOMFAMILIA 0
#define COL_CODFAMILIA 1
#define COL_DESCFAMILIA 2
#define COL_IDFAMILIA  3
#define COL_CODCOMPLETOFAMILIA 4


familiasview::familiasview(company *comp, QWidget *parent, const char *name) : familiasdlg(parent, name) {
    companyact = comp;
    m_listFamilias->addColumn("NOMBRE",-1);
    m_listFamilias->addColumn("CODIGO",-1);
    m_listFamilias->addColumn("DESCRIPCION",-1);
    m_listFamilias->addColumn("IDFAMILIA",0);
    m_listFamilias->addColumn("CODIGOCOMPLETO",-1);
    m_idfamilia="";
    pintar();
}// end familiasview


familiasview::~familiasview() {}// end ~familiasview


void familiasview::pintar() {
    QListViewItem * it;
    QMap <int, QListViewItem*> Lista1;
    int padre;
    int idfamilia=0;
    cursor2 *cursoraux1, *cursoraux2;
    m_listFamilias->clear();
    cursoraux1 = companyact->cargacursor("SELECT * FROM familia WHERE padrefamilia ISNULL ORDER BY idfamilia");
    while (!cursoraux1->eof()) {
        padre = atoi( cursoraux1->valor("padrefamilia").ascii());
        idfamilia = atoi( cursoraux1->valor("idfamilia").ascii());
        it =new QListViewItem(m_listFamilias);
        Lista1[idfamilia]=it;
        it->setText(COL_NOMFAMILIA, cursoraux1->valor("nombrefamilia"));
        it->setText(COL_CODFAMILIA,cursoraux1->valor("codigofamilia"));
        it->setText(COL_DESCFAMILIA, cursoraux1->valor("descfamilia"));
        it->setText(COL_IDFAMILIA, cursoraux1->valor("idfamilia"));
        it->setText(COL_CODCOMPLETOFAMILIA, cursoraux1->valor("codigocompletofamilia"));
        it->setOpen(true);
        cursoraux1->siguienteregistro ();
    }// end while
    delete cursoraux1;
    cursoraux2= companyact->cargacursor("SELECT * FROM familia WHERE padrefamilia IS NOT NULL ORDER BY idfamilia");
    while (!cursoraux2->eof()) {
        padre = atoi( cursoraux1->valor("padrefamilia").ascii());
        idfamilia = atoi( cursoraux1->valor("idfamilia").ascii());
        it = new QListViewItem(Lista1[padre]);
        Lista1[idfamilia]=it;
        it->setText(COL_NOMFAMILIA, cursoraux1->valor("nombrefamilia"));
        it->setText(COL_CODFAMILIA,cursoraux1->valor("codigofamilia"));
        it->setText(COL_DESCFAMILIA, cursoraux1->valor("descfamilia"));
        it->setText(COL_IDFAMILIA, cursoraux1->valor("idfamilia"));
        it->setText(COL_CODCOMPLETOFAMILIA, cursoraux1->valor("codigocompletofamilia"));
        it->setOpen(true);
        cursoraux2->siguienteregistro();
    }// end while
    delete cursoraux2;
    s_releaseModificado();
    m_idfamilia="";
}// end pintar


/**
  * Se ha seleccionado un item en la lista
  * Lo que hacemos es mostar el elemento
  * Si el anterior ha sido modificado pedimos para actuar en consecuencia.
  */
void familiasview::s_seleccionado(QListViewItem *it) {
    trataModificado();
    m_idfamilia = it->text(COL_IDFAMILIA);
    mostrarplantilla();
}// end seleccionado


void familiasview::mostrarplantilla() {
    fprintf(stderr,"mostramos la plantilla");
    QString query;
    query= "SELECT * from familia WHERE idfamilia="+m_idfamilia;
    cursor2 *cursorfamilia = companyact->cargacursor(query);
    if (!cursorfamilia->eof()) {
        m_nomFamilia->setText(cursorfamilia->valor("nombrefamilia"));
        m_descFamilia->setText(cursorfamilia->valor("descfamilia"));
        m_codCompletoFamilia->setText(cursorfamilia->valor("codigocompletofamilia"));
        m_codFamilia->setText(cursorfamilia->valor("codigofamilia"));
    }// end if
    delete cursorfamilia;
}// end mostrarplantilla


/** Antes de salir de la ventana debemos hacer la comprobación de si se ha modificado algo
  * Esta función está dedicada a Francina, Bienvenida al mundo 
  */
void familiasview::close() {
    trataModificado();
    QDialog::close();
}// end close


void familiasview::trataModificado() {
    /// Si se ha modificado el contenido advertimos y guardamos.
    if (m_modificado) {
    	    if ( QMessageBox::warning( this, "Guardar Familia",
		"Desea guardar los cambios.",
		QMessageBox::Ok ,
		QMessageBox::Cancel ) == QMessageBox::Ok)
		s_saveFamilia();	
    }// end if
}// end trataModificado


/** SLOT que responde a la pulsación del botón de guardar el tipo de iva que se está editando.
  * Lo que hace es que se hace un update de todos los campos
  */
void familiasview::s_saveFamilia() {
    QString query = "UPDATE familia SET nombrefamilia='"+m_nomFamilia->text()+"', descfamilia= '"+m_descFamilia->text()+"' , codigofamilia = '"+m_codFamilia->text()+"' WHERE idfamilia="+m_idfamilia;
    companyact->ejecuta(query);
    s_releaseModificado();
    pintar();
}// end s_saveTipoIVA


/** SLOT que responde a la pulsación del botón de nuevo tipo de iva
  * Inserta en la tabla de ivas
  */
void familiasview::s_newFamilia() {
    /// Si se ha modificado el contenido advertimos y guardamos.
    trataModificado();
    QString padrefamilia;
    if (m_idfamilia != "")
    	padrefamilia = m_idfamilia;
    else 
        padrefamilia = "NULL";
    
    QString query = "INSERT INTO familia (nombrefamilia, descfamilia, padrefamilia, codigofamilia) VALUES ('NUEVA FAMILIA','Descripcion de la familia',"+padrefamilia+",'XXX')";
    companyact->begin();
    companyact->ejecuta(query);
    cursor2 *cur = companyact->cargacursor("SELECT max(idfamilia) AS idfamilia FROM familia");
    companyact->commit();
    m_idfamilia = cur->valor("idfamilia");
    pintar();
    delete cur;
}// end s_newTipoIVA

