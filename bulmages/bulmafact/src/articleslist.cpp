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
#include <q3table.h>
#include <qmessagebox.h>
#include <qfile.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qcombobox.h>
//Added by qt3to4:
#include <QTextStream>
#include "pgimportfiles.h"

#include <q3filedialog.h>
#include "company.h"
#include "articleedit.h"

#include "busquedafamilia.h"
#include "busquedatipoarticulo.h"
#include "qtable1.h"
#include "funcaux.h"

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
#define COL_PVPARTICULO 19

void articleslist::guardaconfig() {
    _depura("articleslist::INIT_guardaconfig()\n",0);

    QString aux = "";
    mver_idarticulo->isChecked() ? aux += "1,":aux+="0,";
    mver_codcompletoarticulo->isChecked() ? aux += "1,":aux+="0,";
    mver_nomarticulo->isChecked() ? aux += "1,":aux+="0,";
    mver_descarticulo->isChecked() ? aux += "1,":aux+="0,";
    mver_cbarrasarticulo->isChecked() ? aux += "1,":aux+="0,";
    mver_tipoarticulo->isChecked() ? aux += "1,":aux+="0,";
    mver_descuentoarticulo->isChecked() ? aux += "1,":aux+="0,";
    mver_especificacionesarticulo->isChecked() ? aux += "1,":aux+="0,";
    mver_iconoarticulo->isChecked() ? aux += "1,":aux+="0,";
    mver_fotoarticulo->isChecked() ? aux += "1,":aux+="0,";
    mver_posterarticulo->isChecked() ? aux += "1,":aux+="0,";
    mver_margenarticulo->isChecked() ? aux += "1,":aux+="0,";
    mver_sobrecostearticulo->isChecked() ? aux += "1,":aux+="0,";
    mver_modeloarticulo->isChecked() ? aux += "1,":aux+="0,";
    mver_idtipo_iva->isChecked() ? aux += "1,":aux+="0,";
    mver_desctipo_iva->isChecked() ? aux += "1,":aux+="0,";
    mver_idlinea_prod->isChecked() ? aux += "1,":aux+="0,";
    mver_codarticulo->isChecked() ? aux += "1,":aux+="0,";
    mver_stockarticulo->isChecked() ? aux += "1,":aux+="0,";
    mver_pvparticulo->isChecked() ? aux += "1,":aux+="0,";

    QFile file( confpr->valor(CONF_DIR_USER)+"confarticleslist.cfn" );
    if ( file.open( QIODevice::WriteOnly ) ) {
        QTextStream stream( &file );
        stream << aux << "\n";
        for (int i = 0; i < m_list->numCols(); i++) {
            stream << m_list->columnWidth(i) << "\n";
        }// end for
        file.close();
    }// end if

    _depura("articleslist::END_guardaconfig()\n",0);
}// end guardaconfig()

void articleslist::cargaconfig() {
    _depura("articleslist::INIT_cargaconfig()\n",0);

    QFile file( confpr->valor(CONF_DIR_USER)+"confarticleslist.cfn" );
    QString line;
    if ( file.open( QIODevice::ReadOnly ) ) {
        QTextStream stream( &file );
        line = stream.readLine(); // line of text excluding '\n'
        for (int i = 0; i < m_list->numCols(); i++) {
            QString linea = stream.readLine();
            m_list->setColumnWidth(i, linea.toInt());
        }// end for
        file.close();
    } else
        return;

    mver_idarticulo->setChecked(line.at(0)=='1');
    mver_codcompletoarticulo->setChecked(line.at(2)=='1');
    mver_nomarticulo->setChecked(line.at(4)=='1');
    mver_descarticulo->setChecked(line.at(6)=='1');
    mver_cbarrasarticulo->setChecked(line.at(8)=='1');
    mver_tipoarticulo->setChecked(line.at(10)=='1');
    mver_descuentoarticulo->setChecked(line.at(12)=='1');
    mver_especificacionesarticulo->setChecked(line.at(14)=='1');
    mver_iconoarticulo->setChecked(line.at(16)=='1');
    mver_fotoarticulo->setChecked(line.at(18)=='1');
    mver_posterarticulo->setChecked(line.at(20)=='1');
    mver_margenarticulo->setChecked(line.at(22)=='1');
    mver_sobrecostearticulo->setChecked(line.at(24)=='1');
    mver_modeloarticulo->setChecked(line.at(26)=='1');
    mver_idtipo_iva->setChecked(line.at(28)=='1');
    mver_desctipo_iva->setChecked(line.at(30)=='1');
    mver_idlinea_prod->setChecked(line.at(32)=='1');
    mver_codarticulo->setChecked(line.at(34)=='1');
    mver_stockarticulo->setChecked(line.at(36)=='1');
    mver_pvparticulo->setChecked(line.at(38)=='1');

    _depura("articleslist::END_guardaconfig()\n",0);
}// end cargaconfig



void articleslist::s_configurar() {
    _depura("articleslist::INIT_s_configurar()\n",0);

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

    if(mver_pvparticulo->isChecked() )
        m_list->showColumn(COL_PVPARTICULO);
    else
        m_list->hideColumn(COL_PVPARTICULO);



    _depura("articleslist::END_s_configurar()\n",0);
}// end s_configurar


articleslist::articleslist(company *comp, QWidget *parent, const char *name, Qt::WFlags flag, edmode editmodo)
        : articleslistbase(parent, name, flag)  , pgimportfiles(comp) {
    _depura("articleslist::INIT_articleslist()\n",0);


    companyact = comp;
    m_tipoarticulo->setcompany(comp);
    m_familia->setcompany(comp);
    inicializa();
    cargaconfig();
    s_configurar();
    presenta();
    m_modo=editmodo;
    if (m_modo == EditMode)
        comp->meteWindow("Articulos",this);
    hideBusqueda();
    hideConfiguracion();

    _depura("articleslist::END_articleslist()\n",0);
}// end articleslist


void articleslist::inicializa() {
    _depura("articleslist::INIT_inicializa()\n",0);

    m_list->setNumRows( 0 );
    m_list->setNumCols( 0 );
    m_list->setRowMovingEnabled( TRUE );
    m_list->setColumnMovingEnabled( TRUE );
    m_list->setSorting( TRUE );
    m_list->setSelectionMode( Q3Table::SingleRow );
    m_list->setNumCols(20);
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
    m_list->horizontalHeader()->setLabel( COL_POSTERARTICULO, tr("Póster") );
    m_list->horizontalHeader()->setLabel( COL_MARGENARTICULO, tr("Margen") );
    m_list->horizontalHeader()->setLabel( COL_SOBRECOSTEARTICULO, tr("Sobrecoste") );
    m_list->horizontalHeader()->setLabel( COL_MODELOARTICULO, tr("Modelo") );
    m_list->horizontalHeader()->setLabel( COL_IDTIPO_IVA, tr("Tipo de IVA") );
    m_list->horizontalHeader()->setLabel( COL_DESCTIPO_IVA, tr("Tipo de IVA") );
    m_list->horizontalHeader()->setLabel( COL_IDLINEA_PROD, tr("Línea de Producción") );
    m_list->horizontalHeader()->setLabel( COL_STOCKARTICULO, tr("Stock") );
    m_list->horizontalHeader()->setLabel( COL_PVPARTICULO, tr("PVP") );

    _depura("articleslist::END_inicializa()\n",0);
} //end inicializa


void articleslist::presenta() {
    _depura("articleslist::INIT_presenta()\n",0);

    cursor2 * cur= companyact->cargacursor(formaQuery());
    m_list->setNumRows( cur->numregistros() );
    int i=0;
    while (!cur->eof()) {
        m_list->setText(i,COL_IDARTICULO,cur->valor("idarticulo"));
        m_list->setText(i,COL_CODARTICULO,cur->valor("codarticulo"));
        m_list->setText(i,COL_CODCOMPLETOARTICULO, cur->valor("codigocompletoarticulo"));
        m_list->setText(i,COL_NOMARTICULO,cur->valor("nomarticulo"));
        m_list->setText(i,COL_DESCARTICULO,cur->valor("obserarticulo"));
        m_list->setText(i,COL_IDTIPO_IVA,cur->valor("idtipo_iva"));
        m_list->setText(i,COL_DESCTIPO_IVA,cur->valor("desctipo_iva"));
        m_list->setText(i,COL_STOCKARTICULO, cur->valor("stockarticulo"));
        m_list->setText(i,COL_PVPARTICULO, cur->valor("pvparticulo"));

        i++;
        cur->siguienteregistro();
    }// end while
    delete cur;


    _depura("articleslist::END_presenta()\n",0);
} //end presenta


void articleslist::editArticle(int  row) {
    _depura("articleslist::INIT_editArticle()\n",0);

    m_idArticle = m_list->text(row,COL_IDARTICULO);
    mdb_nomarticulo = m_list->text(row,COL_NOMARTICULO);
    mdb_codigocompletoarticulo = m_list->text(row,COL_CODCOMPLETOARTICULO);
    if (m_modo ==0 ) {
        articleedit *art = new articleedit(companyact,0,theApp->translate("Edición de Artículos", "company"));
        /// Si la carga no va bien entonces terminamos.
        if (art->chargeArticle(m_idArticle))
            return;
        companyact->m_pWorkspace->addWindow(art);
        art->show();
    } else {
        close();
    }// end if
    _depura("articleslist::END_editArticle()\n",0);
}

void articleslist::s_editArticle(int a, int, int, const QPoint &) {
    _depura("articleslist::INIT_s_editArticle(int a, int, int, const QPoint &)\n",0);

    editArticle(a);

    _depura("articleslist::END_s_editArticle(int a, int, int, const QPoint &)\n",0);
}

void articleslist::s_editArticle() {
    _depura("articleslist::INIT_s_editArticle()\n",0);

    int a = m_list->currentRow();
    if (a < 0)
        return;
    editArticle(a);

    _depura("articleslist::END_s_editArticle()\n",0);
}


articleslist::~articleslist() {
    _depura("articleslist::INIT_destructor()\n",0);

    if(m_modo == EditMode)
        companyact->sacaWindow(this);
    guardaconfig();
    _depura("articleslist::END_destructor()\n",0);
}// end ~articleslist




void articleslist::removeArticle() {
    _depura("articleslist::INIT_removeArticle()\n",0);

    if ( QMessageBox::Yes == QMessageBox::question(this,"Borrar Artículo","Esta a punto de borrar un artículo, Estos datos pueden dar problemas.",QMessageBox::Yes, QMessageBox::No)) {
        QString SQLQuery="DELETE FROM articulo WHERE idarticulo="+m_list->text(m_list->currentRow(),COL_IDARTICULO);
        companyact->begin();
        int error = companyact->ejecuta(SQLQuery);
        if (error) {
            companyact->rollback();
            return;
        }// end if
        companyact->commit();
        presenta();
    }// end if

    _depura("articleslist::END_removeArticle()\n",0);
}

QString articleslist::formaQuery() {
    _depura("articleslist::INIT_formaQuery()\n",0);

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
    query +=" ORDER BY codigocompletoarticulo";
    return (query);

    _depura("articleslist::END_formaQuery()\n",0);
}// end formaQuery


QString articleslist::detalleArticulos() {
    _depura("articleslist::INIT_detalleArticulos()\n",0);

    QString texto="";
    cursor2 *cur=companyact->cargacursor(formaQuery());
    while(!cur->eof()) {
        texto += "<blockTable style=\"tabla1\">\n";

        texto += "<tr>\n";
        texto += "<td><h1>"+XMLProtect(cur->valor("nomarticulo"))+"</h1>";
        texto += "<para>"+XMLProtect(cur->valor("obserarticulo"))+"</para></td></tr><tr>\n";
        QString file = confpr->valor(CONF_DIR_IMG_ARTICLES)+XMLProtect(cur->valor("codigocompletoarticulo"))+".jpg";
        QFile f( file );
        if (f.exists() ) {
            texto += "	<td><illustration x=\"0\" y=\"0\" height=\"5cm\">\n"
                     "<image file=\""+confpr->valor(CONF_DIR_IMG_ARTICLES)+XMLProtect(cur->valor("codigocompletoarticulo"))+".jpg\" x=\"0\" y=\"0\" height=\"5cm\"/>\n"
                     "</illustration></td>\n";
        } else {
            texto += "<td></td>\n";
        }
        texto += "</tr>\n";
        texto += "</blockTable>";
        cur->siguienteregistro();
    }// end while
    delete cur;
    return texto;

    _depura("articleslist::END_detalleArticulos()\n",0);
}// end detalleArticulos



void articleslist::Imprimir() {
    _depura("articleslist::INIT_Imprimir()\n",0);

    QString archivo=confpr->valor(CONF_DIR_OPENREPORTS)+"articulos.rml";
    QString archivod = confpr->valor(CONF_DIR_USER)+"articulos.rml";
    QString archivologo=confpr->valor(CONF_DIR_OPENREPORTS)+"logo.jpg";

    /// Copiamos el archivo
#ifdef WINDOWS

    archivo = "copy "+archivo+" "+archivod;
#else

    archivo = "cp "+archivo+" "+archivod;
#endif

    system (archivo.ascii());

    /// Copiamos el logo

#ifdef WINDOWS

    archivologo = "copy "+archivologo+" "+confpr->valor(CONF_DIR_USER)+"logo.jpg";
#else

    archivologo = "cp "+archivologo+" "+confpr->valor(CONF_DIR_USER)+"logo.jpg";
#endif

    system (archivologo.ascii());



    QFile file;
    file.setName( archivod );
    file.open( QIODevice::ReadOnly );
    QTextStream stream(&file);
    QString buff = stream.read();
    file.close();
    QString texto;
    // Linea de totales del presupuesto
    buff.replace("[detallearticulos]",detalleArticulos());
    if ( file.open( QIODevice::WriteOnly ) ) {
        QTextStream stream( &file );
        stream << buff;
        file.close();
    }// end if

    invocaPDF("articulos");

    _depura("articleslist::END_Imprimir()\n",0);
}// end Imprimir


void articleslist::s_imprimir1() {
    _depura("articleslist::INIT_s_imprimir1()\n",0);

    QString archivo=confpr->valor(CONF_DIR_OPENREPORTS)+"articulos1.rml";
    QString archivod = confpr->valor(CONF_DIR_USER)+"articulos1.rml";
    QString archivologo=confpr->valor(CONF_DIR_OPENREPORTS)+"logo.jpg";

    /// Copiamos el archivo
#ifdef WINDOWS

    archivo = "copy "+archivo+" "+archivod;
#else

    archivo = "cp "+archivo+" "+archivod;
#endif

    system (archivo.ascii());

    /// Copiamos el logo

#ifdef WINDOWS

    archivologo = "copy "+archivologo+" "+confpr->valor(CONF_DIR_USER)+"logo.jpg";
#else

    archivologo = "cp "+archivologo+" "+confpr->valor(CONF_DIR_USER)+"logo.jpg";
#endif

    system (archivologo.ascii());

    QFile file;
    file.setName( archivod );
    file.open( QIODevice::ReadOnly );
    QTextStream stream(&file);
    QString buff = stream.read();
    file.close();
    QString fitxersortidatxt;
    // L�ea de totales del presupuesto
    fitxersortidatxt = "<blockTable style=\"tabla\" repeatRows=\"1\">";
    fitxersortidatxt += "<tr>";

    /// ------------------------------------------------------------------
    if(mver_idarticulo->isChecked() )
        fitxersortidatxt += "	<td>Id. Articulo</td>";
    if(mver_codcompletoarticulo->isChecked() )
        fitxersortidatxt += "	<td>Codigo</td>";
    if(mver_nomarticulo->isChecked() )
        fitxersortidatxt += "	<td>Nombre</td>";
    if(mver_descarticulo->isChecked() )
        fitxersortidatxt += "	<td>Código</td>";
    if(mver_cbarrasarticulo->isChecked() )
        fitxersortidatxt += "	<td>Codigo Barras</td>";
    if(mver_tipoarticulo->isChecked() )
        fitxersortidatxt += "	<td>Tipo</td>";
    if(mver_descuentoarticulo->isChecked() )
        fitxersortidatxt += "	<td>Descuento</td>";
    if(mver_especificacionesarticulo->isChecked() )
        fitxersortidatxt += "	<td>Especificaciones</td>";
    if(mver_iconoarticulo->isChecked() )
        fitxersortidatxt += "	<td>Icono</td>";
    if(mver_fotoarticulo->isChecked() )
        fitxersortidatxt += "	<td>Foto</td>";
    if(mver_posterarticulo->isChecked() )
        fitxersortidatxt += "	<td>Poster</td>";
    if(mver_margenarticulo->isChecked() )
        fitxersortidatxt += "	<td>Margen</td>";
    if(mver_sobrecostearticulo->isChecked() )
        fitxersortidatxt += "	<td>Sobrecote</td>";
    if(mver_modeloarticulo->isChecked() )
        fitxersortidatxt += "	<td>Modelo</td>";
    if(mver_idtipo_iva->isChecked() )
        fitxersortidatxt += "	<td>Id. IVA</td>";
    if(mver_desctipo_iva->isChecked() )
        fitxersortidatxt += "	<td>IVA</td>";
    if(mver_idlinea_prod->isChecked() )
        fitxersortidatxt += "	<td>Id. Linea Prod</td>";
    if(mver_codarticulo->isChecked() )
        fitxersortidatxt += "	<td>Codigo</td>";
    if(mver_stockarticulo->isChecked() )
        fitxersortidatxt += "	<td>Stock</td>";
    /// ------------------------------------------------------------------
    fitxersortidatxt += "</tr>";

    cursor2 *cur=companyact->cargacursor(formaQuery());
    while(!cur->eof()) {
        fitxersortidatxt += "<tr>";
        /// ------------------------------------------------------------------
        if(mver_idarticulo->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("idarticulo"))+"</td>";
        if(mver_codcompletoarticulo->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("codigocompletoarticulo"))+"</td>";
        if(mver_nomarticulo->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("nomarticulo"))+"</td>";
        if(mver_descarticulo->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("descarticulo"))+"</td>";
        if(mver_cbarrasarticulo->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("cbarrasarticulo"))+"</td>";
        if(mver_tipoarticulo->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("tipoarticulo"))+"</td>";
        if(mver_descuentoarticulo->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("descuentoarticulo"))+"</td>";
        if(mver_especificacionesarticulo->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("especificacionesarticulo"))+"</td>";
        if(mver_iconoarticulo->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("iconoarticulo"))+"</td>";
        if(mver_fotoarticulo->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("fotoarticulo"))+"</td>";
        if(mver_posterarticulo->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("posterarticulo"))+"</td>";
        if(mver_margenarticulo->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("margenarticulo"))+"</td>";
        if(mver_sobrecostearticulo->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("sobrecostearticulo"))+"</td>";
        if(mver_modeloarticulo->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("modeloarticulo"))+"</td>";
        if(mver_idtipo_iva->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("idtipo_iva"))+"</td>";
        if(mver_desctipo_iva->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("desctipo_iva"))+"</td>";
        if(mver_idlinea_prod->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("idlinea_prod"))+"</td>";
        if(mver_codarticulo->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("codarticulo"))+"</td>";
        if(mver_stockarticulo->isChecked() )
            fitxersortidatxt += "<td>"+XMLProtect(cur->valor("stockarticulo"))+"</td>";
        /// ------------------------------------------------------------------
        fitxersortidatxt += "</tr>";
        cur->siguienteregistro();
    }// end if
    delete cur;
    fitxersortidatxt += "</blockTable>";

    buff.replace("[story]",fitxersortidatxt);

    if ( file.open( QIODevice::WriteOnly ) ) {
        QTextStream stream( &file );
        stream << buff;
        file.close();
    }

    invocaPDF("articulos1");

    _depura("articleslist::END_s_imprimir1()\n",0);
}// end imprimir


void articleslist::s_exportar() {
    _depura("articleslist::INIT_s_exportar()\n",0);

    QFile filexml (Q3FileDialog::getSaveFileName(confpr->valor(CONF_DIR_USER),"Clientes (*.xml)", this, "select file", "Elija el Archivo"));
    if(filexml.open(QIODevice::WriteOnly)) {
        bulmafact2XML(filexml, IMPORT_ARTICULOS);
        filexml.close();
    } else {
        _depura("ERROR AL ABRIR ARCHIVO\n",2);
    }// end if

    _depura("articleslist::END_s_exportar()\n",0);
}//


void articleslist::s_importar() {
    _depura("articleslist::INIT_s_importar()\n",0);

    QFile filexml (Q3FileDialog::getOpenFileName(confpr->valor(CONF_DIR_USER),"Clientes (*.xml)", this, "select file", "Elija el Archivo"));
    if (filexml.open(QIODevice::ReadOnly))  {
        XML2BulmaFact(filexml, IMPORT_ARTICULOS);
        filexml.close();
        presenta();
    }  else  {
        _depura("ERROR AL ABRIR ARCHIVO\n",2);
    }// end if

    _depura("articleslist::END_s_importar()\n",0);
}

