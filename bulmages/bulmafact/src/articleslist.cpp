/***************************************************************************
 *   Copyright (C) 2004 by J. M. Estopa Rey                                *
 *   pepma@telefonica.net                                                  *
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

#include "articleslist.h"
#include <qtable.h>
#include <qmessagebox.h>
#include <qfile.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qcombobox.h>

#include "company.h"
#include "articleedit.h"

#include "busquedafamilia.h"
#include "busquedatipoarticulo.h"


#define COL_IDARTICULO 0
#define COL_CODCOMPLETOARTICULO 1
#define COL_NOMARTICULO 2
#define COL_DESCARTICULO 3
#define COL_CBARRASARTICULO 4
#define COL_TIPOARTICULO 5
#define COL_DESCUENTOARTICULO 6
#define COL_ESPECIFICACIONESARTICULO 7
#define COL_ICONOARTICULO 8
#define COL_FOTOARTICULO 9
#define COL_POSTERARTICULO 10
#define COL_MARGENARTICULO 11
#define COL_SOBRECOSTEARTICULO 12
#define COL_MODELOARTICULO 13
#define COL_IDTIPO_IVA 14
#define COL_DESCTIPO_IVA 15
#define COL_IDLINEA_PROD 16
#define COL_CODARTICULO 17
#define COL_STOCKARTICULO 18


void articleslist::s_configurar() {

    if(mver_idarticulo->isChecked() )
        m_list->showColumn(COL_IDARTICULO);
    else
        m_list->hideColumn(COL_IDARTICULO);

    if(mver_codcompletoarticulo->isChecked() )
        m_list->showColumn(COL_CODCOMPLETOARTICULO);
    else
        m_list->hideColumn(COL_CODCOMPLETOARTICULO);

    if(mver_nomarticulo->isChecked() )
        m_list->showColumn(COL_NOMARTICULO);
    else
        m_list->hideColumn(COL_NOMARTICULO);

    if(mver_descarticulo->isChecked() )
        m_list->showColumn(COL_DESCARTICULO);
    else
        m_list->hideColumn(COL_DESCARTICULO);

    if(mver_cbarrasarticulo->isChecked() )
        m_list->showColumn(COL_CBARRASARTICULO);
    else
        m_list->hideColumn(COL_CBARRASARTICULO);

    if(mver_tipoarticulo->isChecked() )
        m_list->showColumn(COL_TIPOARTICULO);
    else
        m_list->hideColumn(COL_TIPOARTICULO);

    if(mver_descuentoarticulo->isChecked() )
        m_list->showColumn(COL_DESCUENTOARTICULO);
    else
        m_list->hideColumn(COL_DESCUENTOARTICULO);

    if(mver_especificacionesarticulo->isChecked() )
        m_list->showColumn(COL_ESPECIFICACIONESARTICULO);
    else
        m_list->hideColumn(COL_ESPECIFICACIONESARTICULO);

    if(mver_iconoarticulo->isChecked() )
        m_list->showColumn(COL_ICONOARTICULO);
    else
        m_list->hideColumn(COL_ICONOARTICULO);

    if(mver_fotoarticulo->isChecked() )
        m_list->showColumn(COL_FOTOARTICULO);
    else
        m_list->hideColumn(COL_FOTOARTICULO);

    if(mver_posterarticulo->isChecked() )
        m_list->showColumn(COL_POSTERARTICULO);
    else
        m_list->hideColumn(COL_POSTERARTICULO);

    if(mver_margenarticulo->isChecked() )
        m_list->showColumn(COL_MARGENARTICULO);
    else
        m_list->hideColumn(COL_MARGENARTICULO);

    if(mver_sobrecostearticulo->isChecked() )
        m_list->showColumn(COL_SOBRECOSTEARTICULO);
    else
        m_list->hideColumn(COL_SOBRECOSTEARTICULO);

    if(mver_modeloarticulo->isChecked() )
        m_list->showColumn(COL_MODELOARTICULO);
    else
        m_list->hideColumn(COL_MODELOARTICULO);

    if(mver_idtipo_iva->isChecked() )
        m_list->showColumn(COL_IDTIPO_IVA);
    else
        m_list->hideColumn(COL_IDTIPO_IVA);

    if(mver_desctipo_iva->isChecked() )
        m_list->showColumn(COL_DESCTIPO_IVA);
    else
        m_list->hideColumn(COL_DESCTIPO_IVA);

    if(mver_idlinea_prod->isChecked() )
        m_list->showColumn(COL_IDLINEA_PROD);
    else
        m_list->hideColumn(COL_IDLINEA_PROD);

    if(mver_codarticulo->isChecked() )
        m_list->showColumn(COL_CODARTICULO);
    else
        m_list->hideColumn(COL_CODARTICULO);

    if(mver_stockarticulo->isChecked() )
        m_list->showColumn(COL_STOCKARTICULO);
    else
        m_list->hideColumn(COL_STOCKARTICULO);
}// end s_configurar


articleslist::articleslist(company *comp, QWidget *parent, const char *name, int flag)
: articleslistbase(parent, name, flag) {
    companyact = comp;
    m_tipoarticulo->setcompany(comp);
    m_familia->setcompany(comp);
    inicializa();
    m_modo=0;
    comp->meteWindow("Articulos",this);
    hideBusqueda();
    hideConfiguracion();    
}// end articleslist


void articleslist::inicializa() {
    m_list->setNumRows( 0 );
    m_list->setNumCols( 0 );
    m_list->setSelectionMode( QTable::SingleRow );
    m_list->setSorting( FALSE );
    m_list->setSelectionMode( QTable::SingleRow );
    m_list->setColumnMovingEnabled( TRUE );
    m_list->setNumCols(19);
    m_list->horizontalHeader()->setLabel( COL_IDARTICULO, tr( "Identificador" ) );
    m_list->horizontalHeader()->setLabel( COL_CODARTICULO, tr( "Código" ) );
    m_list->horizontalHeader()->setLabel( COL_CODCOMPLETOARTICULO, tr( "Código Completo" ) );
    m_list->horizontalHeader()->setLabel( COL_NOMARTICULO, tr( "Descripción" ) );
    m_list->horizontalHeader()->setLabel( COL_DESCARTICULO, tr( "Descripción Completa" ) );
    m_list->horizontalHeader()->setLabel( COL_CBARRASARTICULO, tr( "Código de Barras" ) );
    m_list->horizontalHeader()->setLabel( COL_TIPOARTICULO, tr( "Tipo de Artículo" ) );
    m_list->horizontalHeader()->setLabel( COL_DESCUENTOARTICULO, tr( "Descuento" ) );
    m_list->horizontalHeader()->setLabel( COL_ESPECIFICACIONESARTICULO, tr("Especificaciones") );
    m_list->horizontalHeader()->setLabel( COL_ICONOARTICULO, tr("Icono") );
    m_list->horizontalHeader()->setLabel( COL_FOTOARTICULO, tr("Foto") );
    m_list->horizontalHeader()->setLabel( COL_POSTERARTICULO, tr("Poster") );
    m_list->horizontalHeader()->setLabel( COL_MARGENARTICULO, tr("Margen") );
    m_list->horizontalHeader()->setLabel( COL_SOBRECOSTEARTICULO, tr("Sobrecoste") );
    m_list->horizontalHeader()->setLabel( COL_MODELOARTICULO, tr("Modelo") );
    m_list->horizontalHeader()->setLabel( COL_IDTIPO_IVA, tr("Tipo de IVA") );
    m_list->horizontalHeader()->setLabel( COL_DESCTIPO_IVA, tr("Tipo de IVA") );
    m_list->horizontalHeader()->setLabel( COL_IDLINEA_PROD, tr("Línea de Producción") );
    m_list->horizontalHeader()->setLabel( COL_STOCKARTICULO, tr("Stock") );
    
    m_list->setColumnWidth(COL_IDARTICULO,100);
    m_list->setColumnWidth(COL_CODARTICULO,100);
    m_list->setColumnWidth(COL_CODCOMPLETOARTICULO, 100);
    m_list->setColumnWidth(COL_NOMARTICULO,300);
    m_list->setColumnWidth(COL_DESCARTICULO,300);
    m_list->setColumnWidth(COL_CBARRASARTICULO,100);
    m_list->setColumnWidth(COL_TIPOARTICULO,100);
    m_list->setColumnWidth(COL_DESCUENTOARTICULO,75);
    m_list->setColumnWidth(COL_ESPECIFICACIONESARTICULO,400);
    m_list->setColumnWidth(COL_ICONOARTICULO,75);
    m_list->setColumnWidth(COL_FOTOARTICULO,75);
    m_list->setColumnWidth(COL_POSTERARTICULO,75);
    m_list->setColumnWidth(COL_MARGENARTICULO,75);
    m_list->setColumnWidth(COL_SOBRECOSTEARTICULO,75);
    m_list->setColumnWidth(COL_MODELOARTICULO,200);
    m_list->setColumnWidth(COL_IDTIPO_IVA,50);
    m_list->setColumnWidth(COL_DESCTIPO_IVA,50);
    m_list->setColumnWidth(COL_IDLINEA_PROD,50);
    m_list->setColumnWidth(COL_STOCKARTICULO,50);
    
    //listado->setPaletteBackgroundColor(QColor(150,230,230));
    // Establecemos el color de fondo del extracto. El valor lo tiene la clase configuracion que es global.
    m_list->setPaletteBackgroundColor(confpr->valor(CONF_BG_LISTARTICULOS));
    m_list->setReadOnly(TRUE);
    m_list->hideColumn(COL_IDARTICULO);
    m_list->hideColumn(COL_IDTIPO_IVA);
    cursor2 * cur= companyact->cargacursor(formaQuery());
    m_list->setNumRows( cur->numregistros() );
    int i=0;
    while (!cur->eof()) {
        m_list->setText(i,COL_IDARTICULO,cur->valor("idarticulo"));
        m_list->setText(i,COL_CODARTICULO,cur->valor("codarticulo"));
        m_list->setText(i,COL_CODCOMPLETOARTICULO, cur->valor("codigocompletoarticulo"));
        m_list->setText(i,COL_NOMARTICULO,cur->valor("nomarticulo"));
        m_list->setText(i,COL_DESCARTICULO,cur->valor("obserarticulo"));
//        m_list->setText(i,COL_CBARRASARTICULO,cur->valor("cbarrasarticulo"));
//        m_list->setText(i,COL_TIPOARTICULO,cur->valor("tipoarticulo"));
//        m_list->setText(i,COL_DESCUENTOARTICULO,cur->valor("descuentoarticulo"));
//        m_list->setText(i,COL_ESPECIFICACIONESARTICULO,cur->valor("especificacionesarticulo"));
//        m_list->setText(i,COL_ICONOARTICULO,cur->valor("iconoarticulo"));
//        m_list->setText(i,COL_FOTOARTICULO,cur->valor("fotoarticulo"));
//        m_list->setText(i,COL_POSTERARTICULO,cur->valor("posterarticulo"));
//        m_list->setText(i,COL_MARGENARTICULO,cur->valor("margenarticulo"));
//        m_list->setText(i,COL_SOBRECOSTEARTICULO,cur->valor("sobrecostearticulo"));
//        m_list->setText(i,COL_MODELOARTICULO,cur->valor("modeloarticulo"));
        m_list->setText(i,COL_IDTIPO_IVA,cur->valor("idtipo_iva"));
        m_list->setText(i,COL_DESCTIPO_IVA,cur->valor("desctipo_iva"));
//        m_list->setText(i,COL_IDLINEA_PROD,cur->valor("idlinea_prod"));
	m_list->setText(i,COL_STOCKARTICULO, cur->valor("stockarticulo"));
        i++;
        cur->siguienteregistro();
    }// end while
    delete cur;
    s_configurar();
} //end inicializa


void articleslist::articleSelected(int a, int b, int c, const QPoint &) {
    m_idArticle = m_list->text(a,COL_IDARTICULO);
    mdb_nomarticulo = m_list->text(a,COL_NOMARTICULO);
    mdb_codigocompletoarticulo = m_list->text(a,COL_CODCOMPLETOARTICULO);
    if (m_modo ==0 ) {
        QString idArt = m_list->text(a, COL_IDARTICULO);
        fprintf(stderr, "parm a: %d  parm b: %d  parm c %d \n", a, b, c);
        articleedit *art = new articleedit(companyact,companyact->m_pWorkspace,theApp->translate("Edicion de Articulos", "company"));
        art->chargeArticle(idArt);
        art->show();
        inicializa();
    } else {
        close();
    }// end if
}

articleslist::~articleslist() {
    companyact->sacaWindow(this);
}// end ~articleslist


void articleslist::newArticle() {
    QString idArt = "0";
    articleedit *art = new articleedit(companyact,companyact->m_pWorkspace,theApp->translate("Edicion de Articulos", "company"));
    art->chargeArticle(idArt);
    art->show();
    inicializa();
}


void articleslist::removeArticle() {
    if ( QMessageBox::Yes == QMessageBox::question(this,"Borrar Artículo","Esta a punto de borrar un artículo, Estos datos pueden dar problemas.",QMessageBox::Yes, QMessageBox::No)) {
        QString SQLQuery="DELETE FROM articulo WHERE idarticulo="+m_list->text(m_list->currentRow(),COL_IDARTICULO);
        companyact->begin();
        companyact->ejecuta(SQLQuery);
        companyact->commit();
        inicializa();
    }// end if
}


QString articleslist::formaQuery() {
    QString orden[] = {"nomarticulo","codigocompletoarticulo","stockarticulo"};
    
    QString query="";
    query += "SELECT * FROM articulo LEFT JOIN tipo_iva ON articulo.idtipo_iva = tipo_iva.idtipo_iva WHERE 1=1 ";
    if(m_presentablearticulo->isChecked())
        query += " AND presentablearticulo ";
    if(m_filtro->text() != "")
        query += " AND nomarticulo LIKE '%"+m_filtro->text()+"%' ";
	
    if(m_familia->idfamilia() != "" ) {

	query += " AND idfamilia IN (SELECT idfamilia FROM familia WHERE codigocompletofamilia LIKE '"+m_familia->codigocompletofamilia()+"%')";
    }// end if
    if (m_tipoarticulo->idtipo_articulo() != "") {
    	query += " AND idtipo_articulo = "+m_tipoarticulo->idtipo_articulo();
    }// end if
    query +=" ORDER BY "+orden[m_orden->currentItem()];
    return (query);
}// end formaQuery


QString articleslist::detalleArticulos() {
    QString texto="";
    cursor2 *cur=companyact->cargacursor(formaQuery());
    int i=0;
    while(!cur->eof()) {
        i= !i;
        if (i) {
            texto += "<blockTable style=\"tabla1\" colWidths=\"5cm, 8cm\" rowHeights=\"5.5cm\">\n";
        } else {
            texto += "<blockTable style=\"tabla2\" colWidths=\"8cm, 5cm\" rowHeights=\"5.5cm\">\n";
        }// end if
        texto += "<tr>\n";

        if (i) {
            texto += "<td><h1>"+cur->valor("nomarticulo")+"</h1>";
            texto += "<para><pre>"+cur->valor("obserarticulo")+"</pre></para></td>\n";
        }// end if
        texto += "	<td><illustration x=\"0\" y=\"0\" height=\"5cm\">\n"
                 "<image file=\""+confpr->valor(CONF_DIR_IMG_ARTICLES)+cur->valor("codigocompletoarticulo")+".jpg\" x=\"0\" y=\"0\" height=\"5cm\"/>\n"
                 "</illustration></td>\n";
        if (!i) {
            texto += "<td><h1>"+cur->valor("nomarticulo")+"</h1>";
            texto += "<pre>"+cur->valor("obserarticulo")+"</pre></td>\n";
        }// end if
        texto += "</tr>\n";
        texto += "</blockTable>";
        cur->siguienteregistro();
    }// end while
    delete cur;
    return texto;
}// end detalleArticulos

void articleslist::Imprimir() {
    /// Copiamos el archivo
    QString archivo=confpr->valor(CONF_DIR_OPENREPORTS)+"articulos.rml";
    archivo = "cp "+archivo+" /tmp/articulos.rml";
    system (archivo.ascii());

    /// Copiamos el logo
    archivo=confpr->valor(CONF_DIR_OPENREPORTS)+"logo.jpg";
    archivo = "cp "+archivo+" /tmp/logo.jpg";
    system (archivo.ascii());


    QFile file;
    file.setName( "/tmp/articulos.rml" );
    file.open( IO_ReadOnly );
    QTextStream stream(&file);
    QString buff = stream.read();
    file.close();
    QString texto;
    // Linea de totales del presupuesto


    buff.replace("[detallearticulos]",detalleArticulos());

    if ( file.open( IO_WriteOnly ) ) {
        QTextStream stream( &file );
        stream << buff;
        file.close();
    }// end if

    system("trml2pdf.py /tmp/articulos.rml > /tmp/articulos.pdf");
    system("kpdf /tmp/articulos.pdf &");

}// end Imprimir
