/***************************************************************************
 *   Copyright (C) 2004 by Alvaro de Miguel                                *
 *   alvaro.demiguel@gmail.com                                             *
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

#include "articleedit.h"
#include "company.h"
#include "division.h"
#include "familiasview.h"
#include "tiposarticuloview.h"

#include <qlineedit.h>
#include <qmessagebox.h>
#include <qtable.h>
#include <qcombobox.h>
#include <qtextedit.h>
#include <qlabel.h>
#include <qpixmap.h>
#include <qcheckbox.h>

#define COL_SUMINISTRA_IDSUMINISTRA 0
#define COL_SUMINISTRA_IDPROVEEDOR 0
#define COL_SUMINISTRA_NOMPROVEEDOR 1
#define COL_SUMINISTRA_REFPRO 2
#define COL_SUMINISTRA_PRINCIPAL 3
#define COL_SUMINISTRA_REFERENT 4


articleedit::articleedit(company *comp, QWidget *parent, const char *name)
        : articleeditbase(parent, name, Qt::WDestructiveClose) {
    companyact = comp;
    idArticle = "0";
    m_archivoimagen="";


    // Arreglamos la tabla de proveedores del artículo
    m_suministra->setNumRows( 0 );
    m_suministra->setNumCols( 0 );
    m_suministra->setSelectionMode( QTable::SingleRow );
    m_suministra->setSorting( TRUE );
    m_suministra->setSelectionMode( QTable::SingleRow );
    m_suministra->setColumnMovingEnabled( TRUE );
    m_suministra->setNumCols(5);
    m_suministra->horizontalHeader()->setLabel( COL_SUMINISTRA_IDPROVEEDOR, tr( "Id. Proveedor" ) );
    m_suministra->horizontalHeader()->setLabel( COL_SUMINISTRA_NOMPROVEEDOR, tr( "Nombre Proveedor" ) );
    m_suministra->horizontalHeader()->setLabel( COL_SUMINISTRA_REFPRO, tr( "Referencia del Proveedor" ) );
    m_suministra->horizontalHeader()->setLabel( COL_SUMINISTRA_PRINCIPAL, tr( "Es Principal" ) );
    m_suministra->horizontalHeader()->setLabel( COL_SUMINISTRA_REFERENT, tr( "Es de Referencia" ) );

    m_suministra->setColumnWidth(COL_SUMINISTRA_IDPROVEEDOR,100);
    m_suministra->setColumnWidth(COL_SUMINISTRA_NOMPROVEEDOR,300);
    m_suministra->setColumnWidth(COL_SUMINISTRA_REFPRO,100);
    m_suministra->setColumnWidth(COL_SUMINISTRA_PRINCIPAL,100);
    m_suministra->setColumnWidth(COL_SUMINISTRA_REFERENT,100);

    m_suministra->hideColumn(COL_SUMINISTRA_IDPROVEEDOR);
    m_suministra->setColumnReadOnly(COL_SUMINISTRA_NOMPROVEEDOR,true);

    companyact->meteWindow("Articulo Edicion",this);
    
    m_imagen->setPixmap(QPixmap("/usr/share/bulmages/logopeq.png"));
    
}// end articleedit

articleedit::~articleedit() {
    companyact->refreshArticles();
    companyact->sacaWindow(this);
}// end ~articleedit



void articleedit::s_familiaLostFocus() {
        QString SQLQuery = "SELECT * FROM familia WHERE codigocompletofamilia='"+m_codigocompletofamilia->text()+"'";
        cursor2 *cur= companyact->cargacursor(SQLQuery);
        if (!cur->eof()) {
	    m_idFamilia = cur->valor("idfamilia");
	    fprintf(stderr,"idfamilia:%s\n", m_idFamilia.ascii());
            m_nombrefamilia->setText(cur->valor("nombrefamilia"));	    
	}// end if
	delete cur;
}// end s_familiaLostFocus


void articleedit::s_searchFamily() {
	familiasview *fam = new familiasview(companyact, 0,0);
	fam->setModoConsulta();
	if (fam->exec() == 1) {
		m_codigocompletofamilia->setText(fam->codigoCompletoFamilia());
	}// end if
	delete fam;
	s_familiaLostFocus();
}// end s_searchFamily


/**
  * Se dispara este SLOT cuando se pulsa sobre el botón de buscar tipo de artículo
  */
void articleedit::s_searchTipo() {
	tiposarticuloview *tip = new tiposarticuloview(companyact, 0,0);
	tip->setModoConsulta();
	if (tip->exec() == 1) {
		m_codtipo_articulo->setText(tip->codTipo());
	}// end if
	delete tip;
//	s_familiaLostFocus();
}// end s_searchFamily


/************************************************************************
* Esta función carga un artículo de la base de datos y lo presenta.     *
* Si el parametro pasado no es un identificador válido entonces se pone *
* la ventana de edición en modo de inserción                            *
*************************************************************************/
void articleedit::chargeArticle(QString idArt) {
    idArticle = idArt;
    QString ivaType="";
    fprintf(stderr,"chargeArticle activado \n");
    if (idArticle != "0") {
        QString SQLQuery = "SELECT * FROM articulo LEFT JOIN familia ON articulo.idfamilia = familia.idfamilia WHERE idarticulo="+idArt;
        cursor2 *cur= companyact->cargacursor(SQLQuery, "unquery");
        if (!cur->eof()) {
            idArticle = idArt;
	    m_idFamilia = cur->valor("idfamilia");
            m_codigocompletofamilia->setText(cur->valor("codigocompletofamilia"));
            m_codigoarticulo->setText(cur->valor("codarticulo"));
            m_nombrearticulo->setText(cur->valor("nomarticulo"));
            m_obserarticulo->setText(cur->valor("obserarticulo"));
	    m_pvparticulo->setText(cur->valor("pvparticulo"));
            ivaType=cur->valor("idtipo_iva");
	    
	    // Pintamos el stockable y el presentable
	    if (cur->valor("presentablearticulo") == "t") {
	    	m_presentablearticulo->setChecked(TRUE);
	    } else {
	    	m_presentablearticulo->setChecked(FALSE);
	    }// end if
	    
	    if (cur->valor("controlstockarticulo") == "t") {
	    	m_controlstockarticulo->setChecked(TRUE);
	    } else {
	    	m_controlstockarticulo->setChecked(FALSE);
	    }// end if	    
	    
	    
            m_codigocompletoarticulo->setText(cur->valor("codigocompletoarticulo"));

            // Cargamos las relaciones artículo - proveedor.
            QString SQLQuery1 = "SELECT * FROM suministra, proveedor WHERE suministra.idproveedor=proveedor.idproveedor and idarticulo="+idArt;
            companyact->begin();
            cursor2 *cur1 = companyact->cargacursor(SQLQuery1, "cargaSuministra");
            companyact->commit();
            m_suministra->setNumRows(cur1->numregistros());
            int i=0;
            while (!cur1->eof()) {
                m_suministra->setText(i,COL_SUMINISTRA_IDPROVEEDOR,cur1->valor("idproveedor"));
                m_suministra->setText(i,COL_SUMINISTRA_NOMPROVEEDOR,cur1->valor("nomproveedor"));
                m_suministra->setText(i,COL_SUMINISTRA_REFPRO,cur1->valor("refprosuministra"));
                m_suministra->setText(i,COL_SUMINISTRA_PRINCIPAL,cur1->valor("principalsuministra"));
                m_suministra->setText(i++,COL_SUMINISTRA_REFERENT,cur1->valor("referentsuministra"));
                cur1->siguienteregistro();
            }// end while
            delete cur1;
        } else {
            idArticle="0";
        }// end if
        delete cur;
    }
    
    m_imagen->setPixmap(QPixmap(confpr->valor(CONF_DIR_IMG_ARTICLES)+m_codigocompletoarticulo->text()+".jpg"));     
    cargarcomboiva(ivaType);
    setCaption("Artículo "+m_codigocompletoarticulo->text());companyact->meteWindow(caption(),this);
}// end chargeArticle


void articleedit::cargarcomboiva(QString idIva) {
    m_cursorcombo = NULL;
    companyact->begin();
    if (m_cursorcombo != NULL)
        delete m_cursorcombo;
    m_cursorcombo = companyact->cargacursor("SELECT * FROM tipo_iva","unquery");
    companyact->commit();
    int i = 0;
    int i1 = 0;
    while (!m_cursorcombo->eof()) {
        i ++;
        if (idIva == m_cursorcombo->valor("idtipo_iva")) {
            i1 = i;
        }
        m_combotipo_iva->insertItem(m_cursorcombo->valor("desctipo_iva"));
        m_cursorcombo->siguienteregistro();
    }// end while
    if (i1 != 0 ) {
        m_combotipo_iva->setCurrentItem(i1-1);
    }// end if
} // end cargarcomboalmacen


/************************************************************************
* Esta función se ejecuta cuando se ha pulsado sobre el botón de nuevo  *
*************************************************************************/
void articleedit::boton_nuevo() {
    idArticle = "0";
    m_codigoarticulo->setText("");
    m_nombrearticulo->setText("");
    m_obserarticulo->setText("");
    m_pvparticulo->setText("0");
    m_combotipo_iva->setCurrentItem(0);
}// end boton_nuevo

/*************************************************************************
* Esta función es la respuesta a la pulsación del boton de guardar       *
* Comprueba si es una inserción o una modificación y hace los pasos      *
* pertinentes                                                            *
**************************************************************************/
void articleedit::accept() {
	
}// end accept


/**
  * Esta función se ejecuta cuando se ha pulsado sobre el botón de borrar *
  */
void articleedit::boton_borrar() {
    if (idArticle != "0") {
        if ( QMessageBox::Yes == QMessageBox::question(this,"Borrar Artículo","Esta a punto de borrar un artículo, Estos datos pueden dar problemas.",QMessageBox::Yes, QMessageBox::No)) {
            QString SQLQuery="DELETE FROM articulo WHERE idarticulo="+idArticle;
            companyact->begin();
            if (companyact->ejecuta(SQLQuery)==0) {
                companyact->commit();
                close();
            } else {
                companyact->rollback();
            }// end if
        }// end if
    }// end if
}// end boton_borrar


void articleedit::s_findArticulo() {
	QString SQlQuery = "SELECT * FROM articulo WHERE codigocompletoarticulo = '"+m_codigocompletoarticulo->text()+"'";
	cursor2 *cur = companyact->cargacursor(SQlQuery);
	if (!cur->eof()) {
		chargeArticle(cur->valor("idarticulo"));
	}// end if
	delete cur;
}// end s_findArticulo


void articleedit::s_grabarClicked() {
	QString presentablearticulo = m_presentablearticulo->isChecked() ? "TRUE" : "FALSE";	
	QString controlstockarticulo = m_controlstockarticulo->isChecked() ? "TRUE" : "FALSE";	

    QString SQLQuery;
    if (idArticle != "0") {
        SQLQuery = "UPDATE articulo SET codarticulo='"+m_codigoarticulo->text()+"'";
        SQLQuery += " , nomarticulo='"+m_nombrearticulo->text()+"'"; 
		  SQLQuery += " , idfamilia="+m_idFamilia;
        SQLQuery += " , obserarticulo='"+m_obserarticulo->text()+"'";
	SQLQuery += " , pvparticulo="+m_pvparticulo->text()+" ";
        SQLQuery += " , idtipo_iva="+m_cursorcombo->valor("idtipo_iva",m_combotipo_iva->currentItem());
	SQLQuery += " , controlstockarticulo="+controlstockarticulo;
	SQLQuery += " , presentablearticulo="+presentablearticulo;
        SQLQuery += " WHERE idarticulo ="+idArticle;
        companyact->begin();
        companyact->ejecuta(SQLQuery);
        companyact->commit();	
    } else {
        QString SQLQuery = " INSERT INTO articulo (codarticulo, nomarticulo, obserarticulo, idtipo_iva, idfamilia, pvparticulo, presentablearticulo, controlstockarticulo)";
        SQLQuery += " VALUES (";
        SQLQuery += " '"+m_codigoarticulo->text()+"' ";
        SQLQuery += " , '"+m_nombrearticulo->text()+"'";
        SQLQuery += " , '"+m_obserarticulo->text()+"'";
        SQLQuery += " , "+m_cursorcombo->valor("idtipo_iva",m_combotipo_iva->currentItem());
	SQLQuery += " , "+m_idFamilia;
	SQLQuery += " , "+m_pvparticulo->text()+" ";
	SQLQuery += " , "+presentablearticulo;
	SQLQuery += " , "+controlstockarticulo;
        SQLQuery += ")";
        companyact->begin();
        companyact->ejecuta(SQLQuery);
	cursor2 *cur= companyact->cargacursor("SELECT max(idarticulo) AS m FROM articulo");
        companyact->commit();
	if (!cur->eof()) {
		idArticle = cur->valor("m");
	}// end if
	delete cur;	

    }// end if */
    companyact->begin();
    if (m_archivoimagen != "") {
	cursor2 *cur1 = companyact->cargacursor("SELECT codigocompletoarticulo FROM articulo WHERE idarticulo="+idArticle);
    	QString cadena = "cp "+m_archivoimagen+" "+confpr->valor(CONF_DIR_IMG_ARTICLES)+cur1->valor("codigocompletoarticulo")+".jpg";
	delete cur1;
	fprintf(stderr,"%s\n",cadena.ascii());
	system(cadena.ascii());
    }// end if
	chargeArticle(idArticle);  
}// end s_grabarClicked


#include <qfiledialog.h>
void articleedit::s_cambiarimagen() {
    m_archivoimagen = QFileDialog::getOpenFileName(
                    "",
                    "Images (*.jpg)",
                    this,
                    "open file dialog",
                    "Choose a file" );
    fprintf(stderr," Archivo Seleccionado: %s\n",m_archivoimagen.ascii());
    m_imagen->setPixmap(QPixmap(m_archivoimagen));   
}// end s_cambiarimagen

