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

#include "articuloview.h"
#include "company.h"
#include "division.h"
#include "familiasview.h"
#include "tiposarticuloview.h"

#include <QLineEdit>
#include <QMessageBox>
#include <Q3Table>
#include <QComboBox>
#include <Q3TextEdit>
#include <QLabel>
#include <QPixmap>
#include <QCheckBox>
#include <Q3FileDialog>
#include <QCloseEvent>

#include "busquedafamilia.h"
#include "busquedatipoarticulo.h"

#include "listcomparticuloview.h"
#include "listcomparticulo.h"
#include "funcaux.h"
#include "plugins.h"

#define COL_SUMINISTRA_IDSUMINISTRA 0
#define COL_SUMINISTRA_IDPROVEEDOR 0
#define COL_SUMINISTRA_NOMPROVEEDOR 1
#define COL_SUMINISTRA_REFPRO 2
#define COL_SUMINISTRA_PRINCIPAL 3
#define COL_SUMINISTRA_REFERENT 4


ArticuloView::ArticuloView(company *comp, QWidget *parent, const char *name)
        : ArticuloViewbase(parent, name, Qt::WDestructiveClose) ,dialogChanges(this) {

    _depura("ArticuloView::INIT_constructor()\n",0);
    m_companyact = comp;

	    /// Disparamos los plugins con presupuesto_imprimirPresupuesto
    int res = g_plugins->lanza("ArticuloView_ArticuloView", this);
    if (res != 0) return;


    m_familia->setcompany(comp);
    m_tipoarticulo->setcompany(comp);
    m_componentes->setcompany(comp);
    mdb_idarticulo = "0";
    m_archivoimagen="";


    // Desabilitamos los tabs que aun no se utilizan
    tabWidget2->setTabEnabled(3,FALSE);
    tabWidget2->setTabEnabled(4,FALSE);
    tabWidget2->setTabEnabled(5,FALSE);
    tabWidget2->setTabEnabled(6,FALSE);
    tabWidget2->setTabEnabled(7,FALSE);


    // Arreglamos la tabla de proveedores del art�ulo
    m_suministra->setNumRows( 0 );
    m_suministra->setNumCols( 0 );
    m_suministra->setSelectionMode( Q3Table::SingleRow );
    m_suministra->setSorting( TRUE );
    m_suministra->setSelectionMode( Q3Table::SingleRow );
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

    m_imagen->setPixmap(QPixmap("/usr/share/bulmages/logopeq.png"));
    if (m_companyact->meteWindow("Articulo Edicion",this)) return;

    dialogChanges_cargaInicial();
    _depura("ArticuloView::END_constructor()\n",0);
}// end ArticuloView

ArticuloView::~ArticuloView() {
    _depura("ArticuloView::INIT_destructor()\n",0);
    m_companyact->refreshArticles();
    m_companyact->sacaWindow(this);
    _depura("ArticuloView::END_destructor()\n",0);
}// end ~ArticuloView




/************************************************************************
* Esta función carga un artículo de la base de datos y lo presenta.     *
* Si el parametro pasado no es un identificador válido entonces se pone *
* la ventana de edición en modo de inserción                            *
*************************************************************************/
int ArticuloView::cargar(QString idarticulo) {
    _depura("ArticuloView::INIT_chargeArticle()\n",0);
    int error =0;

    mdb_idarticulo = idarticulo;

	/// Disparamos los plugins con presupuesto_imprimirPresupuesto
    int res = g_plugins->lanza("ArticuloView_cargar", this);
    if (res != 0) return res;


    QString ivaType="";
    if (mdb_idarticulo != "0") {
        QString SQLQuery = "SELECT * FROM articulo LEFT JOIN familia ON articulo.idfamilia = familia.idfamilia WHERE idarticulo="+mdb_idarticulo;
        cursor2 *cur= m_companyact->cargacursor(SQLQuery);
	if (cur->error())  error =1;


        if (!cur->eof()) {
            m_familia->setidfamilia(cur->valor("idfamilia"));
            m_tipoarticulo->setidtipo_articulo(cur->valor("idtipo_articulo"));
            m_codigoarticulo->setText(cur->valor("codarticulo"));
            m_nombrearticulo->setText(cur->valor("nomarticulo"));
            m_obserarticulo->setText(cur->valor("obserarticulo"));
            m_pvparticulo->setText(cur->valor("pvparticulo"));
            m_abrevarticulo->setText(cur->valor("abrevarticulo"));
            ivaType=cur->valor("idtipo_iva");
            m_codigocompletoarticulo->setText(cur->valor("codigocompletoarticulo"));
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

/*
            // Cargamos las relaciones artículo - proveedor.
            QString SQLQuery1 = "SELECT * FROM suministra, proveedor WHERE suministra.idproveedor=proveedor.idproveedor and idarticulo="+idArt;
            cursor2 *cur1 = m_companyact->cargacursor(SQLQuery1);
	    if (cur1->error()) {
		error = 1;
	    }// end if

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
*/
        } else {
            mdb_idarticulo="0";
        }// end if
        delete cur;
    }

    m_imagen->setPixmap(QPixmap(confpr->valor(CONF_DIR_IMG_ARTICLES)+m_codigocompletoarticulo->text()+".jpg"));
    int ret = cargarcomboiva(ivaType);
    if (ret) error = 1;


    setCaption(tr("Articulo ")+m_codigocompletoarticulo->text());

    ret = m_companyact->meteWindow(caption(),this);
    if (ret) error = 1;

    m_componentes->cargaListCompArticulo(mdb_idarticulo);
    m_componentes->pintaListCompArticulo();
    dialogChanges_cargaInicial();

    /// Tratamiento de excepciones
    if (error == 1) {
        _depura("ArticuloView::END_chargeArticle Error en la carga del articulo()\n",0);
        return -1;
    }// end if

    _depura("ArticuloView::END_chargeArticle()\n",0);
    return 0;
}// end chargeArticle


/// Hace la carga del combo-box de tipos de iva para el articulo.
int ArticuloView::cargarcomboiva(QString idIva) {
    _depura("ArticuloView::INIT_cargacomboiva()\n",0);

    m_cursorcombo = NULL;
    if (m_cursorcombo != NULL)
        delete m_cursorcombo;
    m_cursorcombo = m_companyact->cargacursor("SELECT * FROM tipo_iva");
    if (m_cursorcombo->error()) {
	 delete m_cursorcombo;
	 return -1;
    }// end if
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

    _depura("ArticuloView::END_cargacomboiva()\n",0);
    return 0;
} // end cargarcomboalmacen


/************************************************************************
* Esta función se ejecuta cuando se ha pulsado sobre el botón de nuevo  *
*************************************************************************/
void ArticuloView::boton_nuevo() {
    _depura("ArticuloView::INIT_boton_nuevo()\n",0);

    mdb_idarticulo = "0";
    m_codigoarticulo->setText("");
    m_nombrearticulo->setText("");
    m_obserarticulo->setText("");
    m_pvparticulo->setText("0");
    m_combotipo_iva->setCurrentItem(0);
    dialogChanges_cargaInicial();
    _depura("ArticuloView::END_boton_nuevo()\n",0);
}// end boton_nuevo

/*************************************************************************
* Esta función es la respuesta a la pulsación del boton de guardar       *
* Comprueba si es una inserción o una modificación y hace los pasos      *
* pertinentes                                                            *
**************************************************************************/
void ArticuloView::accept() {
}// end accept


/**
  * Esta función se ejecuta cuando se ha pulsado sobre el botón de borrar *
  */
void ArticuloView::boton_borrar() {
    _depura("ArticuloView::INIT_boton_borrar()\n",0);

    if (mdb_idarticulo != "0") {
        m_componentes->borrar();
        if ( QMessageBox::question(this,tr("Borrar Articulo"),tr("Esta a punto de borrar un articulo, Desea continuar?."),tr("Si"),tr("No"),0,1,0) == 0) {
            QString SQLQuery="DELETE FROM articulo WHERE idarticulo="+mdb_idarticulo;
            m_companyact->begin();
            if (m_companyact->ejecuta(SQLQuery)==0) {
                m_companyact->commit();
                dialogChanges_cargaInicial();
                close();
            } else {
                m_companyact->rollback();
            }// end if
        }// end if
    }// end if
    _depura("ArticuloView::END_boton_borrar()\n",0);
}// end boton_borrar


void ArticuloView::s_findArticulo() {
    _depura("ArticuloView::INIT_s_findArticulo()\n",0);

    QString SQlQuery = "SELECT * FROM articulo WHERE codigocompletoarticulo = '"+m_codigocompletoarticulo->text()+"'";
    cursor2 *cur = m_companyact->cargacursor(SQlQuery);
    if (!cur->eof()) {
        cargar(cur->valor("idarticulo"));
    }// end if
    delete cur;
    _depura("ArticuloView::END_s_findArticulo()\n",0);
}// end s_findArticulo


void ArticuloView::s_grabarClicked() {
    _depura("ArticuloView::INIT_s_grabarClicked()\n",0);

    QString presentablearticulo = m_presentablearticulo->isChecked() ? "TRUE" : "FALSE";
    QString controlstockarticulo = m_controlstockarticulo->isChecked() ? "TRUE" : "FALSE";
    QString idtipo_articulo = m_tipoarticulo->idtipo_articulo();
    if (idtipo_articulo == "")
        idtipo_articulo="NULL";

    QString SQLQuery;

    if (mdb_idarticulo != "0") {
        SQLQuery = "UPDATE articulo SET codarticulo='"+m_companyact->sanearCadena(m_codigoarticulo->text())+"'";
        SQLQuery += " , nomarticulo='"+m_companyact->sanearCadena(m_nombrearticulo->text())+"'";
        SQLQuery += " , idfamilia="+m_companyact->sanearCadena(m_familia->idfamilia());
        SQLQuery += " , obserarticulo='"+m_companyact->sanearCadena(m_obserarticulo->text())+"'";
        SQLQuery += " , abrevarticulo='"+m_companyact->sanearCadena(m_abrevarticulo->text())+"'";
        SQLQuery += " , pvparticulo="+m_companyact->sanearCadena(m_pvparticulo->text())+" ";
        SQLQuery += " , idtipo_iva="+m_companyact->sanearCadena(m_cursorcombo->valor("idtipo_iva",m_combotipo_iva->currentItem()));
        SQLQuery += " , controlstockarticulo="+m_companyact->sanearCadena(controlstockarticulo);
        SQLQuery += " , presentablearticulo="+m_companyact->sanearCadena(presentablearticulo);
        SQLQuery += " , idtipo_articulo="+idtipo_articulo;
        SQLQuery += " WHERE idarticulo ="+mdb_idarticulo;
        m_companyact->begin();
        int error = m_companyact->ejecuta(SQLQuery);
        if (error) {
            m_companyact->rollback();
            return;
        }// end if
        m_companyact->commit();
    } else {
        QString SQLQuery = " INSERT INTO articulo (codarticulo, nomarticulo, obserarticulo, idtipo_iva, idfamilia, pvparticulo, presentablearticulo, controlstockarticulo, idtipo_articulo, abrevarticulo)";
        SQLQuery += " VALUES (";
        SQLQuery += " '"+m_companyact->sanearCadena(m_codigoarticulo->text())+"' ";
        SQLQuery += " , '"+m_companyact->sanearCadena(m_nombrearticulo->text())+"'";
        SQLQuery += " , '"+m_companyact->sanearCadena(m_obserarticulo->text())+"'";
        SQLQuery += " , "+m_companyact->sanearCadena(m_cursorcombo->valor("idtipo_iva",m_combotipo_iva->currentItem()));
        SQLQuery += " , "+m_companyact->sanearCadena(m_familia->idfamilia());
        SQLQuery += " , "+m_companyact->sanearCadena(m_pvparticulo->text())+" ";
        SQLQuery += " , "+m_companyact->sanearCadena(presentablearticulo);
        SQLQuery += " , "+m_companyact->sanearCadena(controlstockarticulo);
        SQLQuery += " , "+idtipo_articulo;
        SQLQuery += " , '"+m_companyact->sanearCadena(m_abrevarticulo->text())+"'";
        SQLQuery += ")";
        m_companyact->begin();
        int error = m_companyact->ejecuta(SQLQuery);
        if (error) {
            m_companyact->rollback();
            return;
        }// end if
        cursor2 *cur= m_companyact->cargacursor("SELECT max(idarticulo) AS m FROM articulo");
        m_companyact->commit();
        if (!cur->eof()) {
            mdb_idarticulo = cur->valor("m");
        }// end if
        delete cur;

    }// end if */
    if (m_archivoimagen != "") {
        cursor2 *cur1 = m_companyact->cargacursor("SELECT codigocompletoarticulo FROM articulo WHERE idarticulo="+mdb_idarticulo);
        QString cadena = "cp "+m_archivoimagen+" "+confpr->valor(CONF_DIR_IMG_ARTICLES)+cur1->valor("codigocompletoarticulo")+".jpg";
        delete cur1;
        fprintf(stderr,"%s\n",cadena.ascii());
        system(cadena.ascii());
    }// end if
    m_componentes->guardaListCompArticulo();
    cargar(mdb_idarticulo);
    _depura("ArticuloView::END_s_grabarClicked()\n",0);
}// end s_grabarClicked



void ArticuloView::s_cambiarimagen() {
    _depura("ArticuloView::INIT_s_cambiarimagen()\n",0);

    m_archivoimagen = Q3FileDialog::getOpenFileName(
                          "",
                          "Images (*.jpg)",
                          this,
                          "open file dialog",
                          "Choose a file" );
    fprintf(stderr," Archivo Seleccionado: %s\n",m_archivoimagen.ascii());
    m_imagen->setPixmap(QPixmap(m_archivoimagen));
    _depura("ArticuloView::END_s_cambiarimagen()\n",0);
}// end s_cambiarimagen


void ArticuloView::closeEvent( QCloseEvent *e) {
    _depura("closeEvent",0);
    if (dialogChanges_hayCambios())  {
        int val = QMessageBox::warning( this, "Guardar Articulo",
                                        "Desea guardar los cambios.","Si","No","Cancelar",0,2);
        if (val == 0)
            s_grabarClicked();
        if (val == 2)
            e->ignore();
    }// end if
}
