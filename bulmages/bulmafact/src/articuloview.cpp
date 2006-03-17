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


#include "articuloview.h"
#include "company.h"
#include "division.h"
#include "familiasview.h"
#include "tiposarticuloview.h"
#include "busquedafamilia.h"
#include "busquedatipoarticulo.h"
#include "comparticulolistview.h"
#include "comparticulolist.h"
#include "funcaux.h"
#include "plugins.h"




ArticuloView::ArticuloView(company *comp, QWidget *parent, const char *name)
        : QWidget(parent, name, Qt::WDestructiveClose) ,dialogChanges(this), Articulo(comp) {

    _depura("ArticuloView::INIT_constructor()\n",0);
    m_companyact = comp;

	setupUi(this);


    /// Disparamos los plugins con presupuesto_imprimirPresupuesto
    int res = g_plugins->lanza("ArticuloView_ArticuloView", this);
    if (res != 0)
        return;



    m_familia->setcompany(comp);
    m_tipoarticulo->setcompany(comp);
    m_componentes->setcompany(comp);
    m_archivoimagen="";


    m_imagen->setPixmap(QPixmap("/usr/share/bulmages/logopeq.png"));
    if (m_companyact->meteWindow("Articulo Edicion",this))
        return;

    dialogChanges_cargaInicial();
    _depura("ArticuloView::END_constructor()\n",0);
}// end ArticuloView


ArticuloView::~ArticuloView() {
    _depura("ArticuloView::INIT_destructor()\n",0);
    //    m_companyact->refreshArticles();
    m_companyact->sacaWindow(this);
    _depura("ArticuloView::END_destructor()\n",0);
}// end ~ArticuloView


void ArticuloView::pintar() {
    _depura("ArticuloView::pintar",0);

    m_familia->setidfamilia(DBvalue("idfamilia"));
    m_tipoarticulo->setidtipo_articulo(DBvalue("idtipo_articulo"));
    m_codigoarticulo->setText(DBvalue("codarticulo"));
    m_nombrearticulo->setText(DBvalue("nomarticulo"));
    m_obserarticulo->setText(DBvalue("obserarticulo"));
    m_pvparticulo->setText(DBvalue("pvparticulo"));
    m_abrevarticulo->setText(DBvalue("abrevarticulo"));

    m_codigocompletoarticulo->setText(DBvalue("codigocompletoarticulo"));
    // Pintamos el stockable y el presentable
    if (DBvalue("presentablearticulo") == "t") {
        m_presentablearticulo->setChecked(TRUE);
    } else {
        m_presentablearticulo->setChecked(FALSE);
    }// end if
    if (DBvalue("controlstockarticulo") == "t") {
        m_controlstockarticulo->setChecked(TRUE);
    } else {
        m_controlstockarticulo->setChecked(FALSE);
    }// end if



    m_imagen->setPixmap(QPixmap(confpr->valor(CONF_DIR_IMG_ARTICLES)+m_codigocompletoarticulo->text()+".jpg"));

	/// Pintamos la parte de componentes
	m_componentes->pintar();


    setCaption(tr("Articulo ")+m_codigocompletoarticulo->text());
    _depura("END ArticuloView::pintar",1);
}




/************************************************************************
* Esta función carga un artículo de la base de datos y lo presenta.     *
* Si el parametro pasado no es un identificador válido entonces se pone *
* la ventana de edición en modo de inserción                            *
*************************************************************************/
int ArticuloView::cargar(QString idarticulo) {
    _depura("ArticuloView::cargar()\n",0);
    int error =0;
	setDBvalue( "idarticulo", idarticulo);


        /// Disparamos los plugins
        int res = g_plugins->lanza("ArticuloView_cargar", this);
        if (res != 0)
            return res;



    QString ivaType="";
    Articulo::cargar(idarticulo);
    ivaType=DBvalue("idtipo_iva");

    int ret = cargarcomboiva(ivaType);
    if (ret)
        error = 1;
    ret = m_companyact->meteWindow(caption(),this);
    if (ret)
        error = 1;
    m_componentes->cargar(DBvalue("idarticulo"));


    dialogChanges_cargaInicial();
    /// Tratamiento de excepciones
    if (error == 1) {
        _depura("ArticuloView::END_chargeArticle Error en la carga del articulo()\n",0);
        return -1;
    }// end if

    pintar();
    _depura("END ArticuloView::cargar()\n",0);
    return 0;
}// end chargeArticle


/// Hace la carga del combo-box de tipos de iva para el articulo.
int ArticuloView::cargarcomboiva(QString idIva) {
    _depura("ArticuloView::INIT_cargarcomboiva()\n",0);

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

    _depura("ArticuloView::END_cargarcomboiva()\n",0);
    return 0;
}


/************************************************************************
* Esta función se ejecuta cuando se ha pulsado sobre el botón de nuevo  *
*************************************************************************/
void ArticuloView::on_mui_crear_clicked() {
    _depura("ArticuloView::INIT_boton_nuevo()\n",0);
    vaciar();
	m_componentes->vaciar();
    pintar();
    _depura("ArticuloView::END_boton_nuevo()\n",0);
}



/**
  * Esta función se ejecuta cuando se ha pulsado sobre el botón de borrar *
  */
void ArticuloView::on_mui_borrar_clicked() {
    _depura("ArticuloView::INIT_boton_borrar()\n",0);

    if (DBvalue("idarticulo") != "") {

        if ( QMessageBox::question(this,tr("Borrar Articulo"),tr("Esta a punto de borrar un articulo, Desea continuar?."),tr("Si"),tr("No"),0,1,0) == 0) {
            m_componentes->borrar();
            borrar();
		dialogChanges_cargaInicial();
        }// end if
    }// end if
    _depura("ArticuloView::END_boton_borrar()\n",0);
}


void ArticuloView::on_m_codigocompletoarticulo_editingFinished() {
    _depura("ArticuloView::INIT_s_findArticulo()\n",0);

    QString SQlQuery = "SELECT * FROM articulo WHERE codigocompletoarticulo = '"+m_codigocompletoarticulo->text()+"'";
    cursor2 *cur = m_companyact->cargacursor(SQlQuery);
    if (!cur->eof()) {
        cargar(cur->valor("idarticulo"));
    }// end if
    delete cur;

    _depura("ArticuloView::END_s_findArticulo()\n",0);
}// end s_findArticulo


int ArticuloView::guardar() {
    _depura("ArticuloView::INIT_s_grabarClicked()\n",0);
    setDBvalue("presentablearticulo",  m_presentablearticulo->isChecked() ? "TRUE" : "FALSE");
    setDBvalue("controlstockarticulo", m_controlstockarticulo->isChecked() ? "TRUE" : "FALSE");
    setDBvalue("idtipo_articulo", m_tipoarticulo->idtipo_articulo());
    setDBvalue("codarticulo", m_codigoarticulo->text());
    setDBvalue("nomarticulo", m_nombrearticulo->text());
    setDBvalue("idfamilia", m_familia->idfamilia());
    setDBvalue("obserarticulo", m_obserarticulo->text());
    setDBvalue("abrevarticulo", m_abrevarticulo->text());
    setDBvalue("pvparticulo", m_pvparticulo->text());
    setDBvalue("idtipo_iva", m_cursorcombo->valor("idtipo_iva",m_combotipo_iva->currentItem()));
    Articulo::guardar();
	/// Guardamos la imagen, si es que existe
    if (m_archivoimagen != "") {
        cursor2 *cur1 = m_companyact->cargacursor("SELECT codigocompletoarticulo FROM articulo WHERE idarticulo="+DBvalue("idarticulo"));
        QString cadena = "cp "+m_archivoimagen+" "+confpr->valor(CONF_DIR_IMG_ARTICLES)+cur1->valor("codigocompletoarticulo")+".jpg";
        delete cur1;
        system(cadena.ascii());
    }// end if
	/// Guardamos la lista de componentes
	m_componentes->setColumnValue("idarticulo",DBvalue("idarticulo"));
    m_componentes->guardar();


        /// Disparamos los plugins
        int res = g_plugins->lanza("ArticuloView_guardar_post", this);
        if (res != 0)
            return res;


	dialogChanges_cargaInicial();

    _depura("ArticuloView::END_s_grabarClicked()\n",0);
	return 0;
}// end s_grabarClicked



void ArticuloView::on_mui_cambiarimagen_clicked() {
    _depura("ArticuloView::INIT_s_cambiarimagen()\n",0);

    m_archivoimagen = Q3FileDialog::getOpenFileName(
                          "",
                          "Images (*.jpg)",
                          this,
                          "open file dialog",
                          "Choose a file" );
    m_imagen->setPixmap(QPixmap(m_archivoimagen));
    _depura("ArticuloView::END_s_cambiarimagen()\n",0);
}// end s_cambiarimagen


void ArticuloView::closeEvent( QCloseEvent *e) {
    _depura("closeEvent",0);
    if (dialogChanges_hayCambios())  {
        int val = QMessageBox::warning( this, "Guardar Articulo",
                                        "Desea guardar los cambios.","Si","No","Cancelar",0,2);
        if (val == 0)
            on_mui_guardar_clicked();
        if (val == 2)
            e->ignore();
    }// end if
}


void ArticuloView::on_mui_aceptar_clicked() {
	on_mui_guardar_clicked();
	close();
}


