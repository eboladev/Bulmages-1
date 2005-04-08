/***************************************************************************
 *   Copyright (C) 2004 by Alvaro de Miguel                                *
 *   alvaro_demiguel@gmail.com                                             *
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

/*
CREATE TABLE articulo (
    idarticulo serial PRIMARY KEY,
    codarticulo character varying(12),
    nomarticulo character varying(50),
    descarticulo character varying(500),
    cbarrasarticulo character varying(22),
    tipoarticulo integer,
    descuentoarticulo float,
    especificacionesarticulo character varying(2000),
    iconoarticulo oid,
    fotoarticulo oid,
    posterarticulo oid,
    margenarticulo float,
    sobrecostearticulo float,
    modeloarticulo character varying(1000),
    
    idtipo_iva integer REFERENCES tipo_iva (idtipo_iva),
    idlinea_prod integer REFERENCES linea_prod(idlinea_prod)
); 
*/

#include "articleedit.h"
#include "company.h"
#include "division.h"
#include "familiasview.h"

#include <qlineedit.h>
#include <qmessagebox.h>
#include <qtable.h>
#include <qcombobox.h>
#include <qtextedit.h>
#include <qlabel.h>

#define COL_SUMINISTRA_IDSUMINISTRA 0
#define COL_SUMINISTRA_IDPROVEEDOR 0
#define COL_SUMINISTRA_NOMPROVEEDOR 1
#define COL_SUMINISTRA_REFPRO 2
#define COL_SUMINISTRA_PVD 3
#define COL_SUMINISTRA_BENEFICIO 4
#define COL_SUMINISTRA_SOBRECOSTE 5
#define COL_SUMINISTRA_PRINCIPAL 6
#define COL_SUMINISTRA_REFERENT 7


articleedit::articleedit(company *comp, QWidget *parent, const char *name)
        : articleeditbase(parent, name, Qt::WDestructiveClose) {
    companyact = comp;
    idArticle = "0";


    // Arreglamos la tabla de proveedores del artículo
    m_suministra->setNumRows( 0 );
    m_suministra->setNumCols( 0 );
    m_suministra->setSelectionMode( QTable::SingleRow );
    m_suministra->setSorting( TRUE );
    m_suministra->setSelectionMode( QTable::SingleRow );
    m_suministra->setColumnMovingEnabled( TRUE );
    m_suministra->setNumCols(8);
    m_suministra->horizontalHeader()->setLabel( COL_SUMINISTRA_IDPROVEEDOR, tr( "Id. Proveedor" ) );
    m_suministra->horizontalHeader()->setLabel( COL_SUMINISTRA_NOMPROVEEDOR, tr( "Nombre Proveedor" ) );
    m_suministra->horizontalHeader()->setLabel( COL_SUMINISTRA_REFPRO, tr( "Referencia del Proveedor" ) );
    m_suministra->horizontalHeader()->setLabel( COL_SUMINISTRA_PVD, tr( "PVD" ) );
    m_suministra->horizontalHeader()->setLabel( COL_SUMINISTRA_BENEFICIO, tr( "Beneficio" ) );
    m_suministra->horizontalHeader()->setLabel( COL_SUMINISTRA_SOBRECOSTE, tr( "Sobrecoste" ) );
    m_suministra->horizontalHeader()->setLabel( COL_SUMINISTRA_PRINCIPAL, tr( "Es Principal" ) );
    m_suministra->horizontalHeader()->setLabel( COL_SUMINISTRA_REFERENT, tr( "Es de Referencia" ) );

    m_suministra->setColumnWidth(COL_SUMINISTRA_IDPROVEEDOR,100);
    m_suministra->setColumnWidth(COL_SUMINISTRA_NOMPROVEEDOR,300);
    m_suministra->setColumnWidth(COL_SUMINISTRA_REFPRO,100);
    m_suministra->setColumnWidth(COL_SUMINISTRA_PVD,100);
    m_suministra->setColumnWidth(COL_SUMINISTRA_BENEFICIO,100);
    m_suministra->setColumnWidth(COL_SUMINISTRA_SOBRECOSTE,100);
    m_suministra->setColumnWidth(COL_SUMINISTRA_PRINCIPAL,100);
    m_suministra->setColumnWidth(COL_SUMINISTRA_REFERENT,100);

    m_suministra->hideColumn(COL_SUMINISTRA_IDPROVEEDOR);
    m_suministra->setColumnReadOnly(COL_SUMINISTRA_NOMPROVEEDOR,true);

    companyact->meteWindow("Articulo Edicion",this);
}// end articleedit

articleedit::~articleedit() {
    companyact->refreshArticles();
    companyact->sacaWindow(this);
}// end ~articleedit



void articleedit::s_familiaLostFocus() {
        QString SQLQuery = "SELECT * FROM familia WHERE codigocompletofamilia='"+m_codigoCompletoFamilia->text()+"'";
        cursor2 *cur= companyact->cargacursor(SQLQuery, "unquery");
        if (!cur->eof()) {
	    m_idFamilia = cur->valor(cur->valor("idfamilia"));
            m_nomFamilia->setText(cur->valor("nombrefamilia"));	    
	}// end if
	delete cur;
}// end s_familiaLostFocus


void articleedit::s_searchFamily() {
	familiasview *fam = new familiasview(companyact, 0,0);
	fam->exec();
	delete fam;
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
	    m_idFamilia = cur->valor(cur->valor("idfamilia"));
            m_codigoCompletoFamilia->setText(cur->valor("codigocompletofamilia"));
            m_articleCode->setText(cur->valor("codarticulo"));
            m_articleName->setText(cur->valor("nomarticulo"));
            m_articleDesc->setText(cur->valor("descarticulo"));
            m_barCode->setText(cur->valor("cbarrasarticulo"));
            m_articleDiscount->setText(cur->valor("descuentoarticulo"));
            m_specifications->setText(cur->valor("especificacionesarticulo"));
            m_articleMargin->setText(cur->valor("margenarticulo"));
            m_articleOverCost->setText(cur->valor("sobrecostearticulo"));
            m_articleModel->setText(cur->valor("modeloarticulo"));
            ivaType=cur->valor("idtipo_iva");
            m_comboArtType->setCurrentItem(cur->valor("tipoarticulo").toInt());


            // Suministra relation loading
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
                m_suministra->setText(i,COL_SUMINISTRA_PVD,cur1->valor("pvdsuministra"));
                m_suministra->setText(i,COL_SUMINISTRA_BENEFICIO,cur1->valor("beneficiosuministra"));
                m_suministra->setText(i,COL_SUMINISTRA_SOBRECOSTE,cur1->valor("sobrecostesuministra"));
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
    cargarcomboiva(ivaType);
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
        m_comboIvaType->insertItem(m_cursorcombo->valor("desctipo_iva"));
        m_cursorcombo->siguienteregistro();
    }
    if (i1 != 0 ) {
        m_comboIvaType->setCurrentItem(i1-1);
    }
} // end cargarcomboalmacen


/************************************************************************
* Esta función se ejecuta cuando se ha pulsado sobre el botón de nuevo  *
*************************************************************************/
void articleedit::boton_nuevo() {
    idArticle = "0";
    m_articleCode->setText("");
    m_articleName->setText("");
    m_articleDesc->setText("");
    m_barCode->setText("");
    //       m_comboIvaType ...
    m_articleDiscount->setText("");
    m_specifications->setText("");
    m_articleMargin->setText("");
    m_articleOverCost->setText("");
    m_articleModel->setText("");
    m_productionLine->setText("");
    m_comboArtType->setCurrentItem(0);
    m_comboIvaType->setCurrentItem(0);
}// end boton_nuevo

/*************************************************************************
* Esta función es la respuesta a la pulsación del boton de guardar       *
* Comprueba si es una inserción o una modificación y hace los pasos      *
* pertinentes                                                            *
**************************************************************************/
void articleedit::accept() {
    QString SQLQuery;
    if (idArticle != "0") {
        SQLQuery = "UPDATE articulo SET codarticulo='"+m_articleCode->text()+"'";
        SQLQuery += " , nomarticulo='"+m_articleName->text()+"'";
        SQLQuery += " , descarticulo='"+m_articleDesc->text()+"'";
        SQLQuery += " , cbarrasarticulo='"+m_barCode->text()+"'";
        SQLQuery += " , tipoarticulo="+QString().sprintf("%d", m_comboArtType->currentItem());
        SQLQuery += " , descuentoarticulo="+m_articleDiscount->text();
        SQLQuery += " , especificacionesarticulo='"+m_specifications->text()+"'";
        SQLQuery += " , margenarticulo="+m_articleMargin->text();
        SQLQuery += " , sobrecostearticulo="+m_articleOverCost->text();
        SQLQuery += " , modeloarticulo='"+m_articleModel->text()+"'";
        SQLQuery += " , idtipo_iva="+m_cursorcombo->valor("idtipo_iva",m_comboIvaType->currentItem());
        SQLQuery += " WHERE idarticulo ="+idArticle;
    } else {
        QString SQLQuery = " INSERT INTO articulo (codarticulo, nomarticulo, descarticulo, cbarrasarticulo, tipoarticulo, descuentoarticulo, especificacionesarticulo, margenarticulo, sobrecostearticulo, modeloarticulo, idtipo_iva)";
        SQLQuery += " VALUES (";
        SQLQuery += " "+m_articleCode->text();
        SQLQuery += " , '"+m_articleName->text()+"'";
        SQLQuery += " , '"+m_articleDesc->text()+"'";
        SQLQuery += " , '"+m_barCode->text()+"'";
        SQLQuery += " , "+QString().sprintf("%d", m_comboArtType->currentItem());
        SQLQuery += " , "+m_articleDiscount->text();
        SQLQuery += " , '"+m_specifications->text()+"'";
        SQLQuery += " , "+m_articleMargin->text();
        SQLQuery += " , "+m_articleOverCost->text();
        SQLQuery += " , '"+m_articleModel->text()+"'";
        SQLQuery += " , "+m_cursorcombo->valor("idtipo_iva",m_comboIvaType->currentItem());
        SQLQuery += ")";
        companyact->begin();
        companyact->ejecuta(SQLQuery);
        companyact->commit();
        close();
    }// end if */
    companyact->begin();
    if (companyact->ejecuta(SQLQuery)==0) {
        companyact->commit();
        close();
    } else {
        companyact->rollback();
    }
}// end accept

/************************************************************************
* Esta función se ejecuta cuando se ha pulsado sobre el botón de borrar *
*************************************************************************/
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


void articleedit::articleDiscountLostFocus() {
    m_articleDiscount->setText(m_articleDiscount->text().replace(",","."));
}


void articleedit::articleMarginLostFocus() {
    m_articleMargin->setText(m_articleMargin->text().replace(",","."));
}


void articleedit::articleOverCostLostFocus() {
    m_articleOverCost->setText(m_articleOverCost->text().replace(",","."));
}
