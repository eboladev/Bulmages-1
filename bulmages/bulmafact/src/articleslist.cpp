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

 /*
--- Codi: Clau artificial.
-- Nom: Descripció curta de l'article.
-- Descripcio: Descripció completa (llarga) de l'article.
-- CBarres: Codi de barres.
-- Tipus: [Simple | Compost calculat | Compost escalat | Ampliat]. Determina el tipus d'article i com --  calcular-ne el preu. Simple = Article no compost amb preu PVP; Compost = Article compost amb preu suma dels PVPs dels components (Calculat) o PVP (escalat); Ampliat = Compost d'ell mateix més els components amb preu suma de PVP i PVPs dels components.
-- Descompte: Descompte "invisible" que s'aplica al preu resultant. Útil sobretot per a diferenciar el preu dels articles composts del preu de compra per separat.
-- Especificacions: Camp de text per a comentaris i informacions varies.
-- Icona: Icona de l'article.
-- Fotografia: Fotografia de l'article.
-- Poster: Fotografia de gran tamany de l'article.
-- Marge: (Per defecte) Percentatge de càlcul del PVP sobre el PVD. Vàlid només quan no estigui definit un marge específic per aquest article i el proveïdor corresponent.
-- Sobrecost: (Per defecte) Import a sumar en el càlcul del PVP en concepte de despeses de transport o altres.
-- Model: Referència, o nom identificatiu del fabricant.
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

#include "articleslist.h"
#include <qtable.h>
#include <qmessagebox.h>
#include "company.h"
#include "articleedit.h"


#define COL_IDARTICULO 0
#define COL_CODARTICULO 1
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
#define COL_IDLINEA_PROD 15



articleslist::articleslist(company *comp, QWidget *parent, const char *name, int flag)
 : articleslistbase(parent, name, flag) {
	companyact = comp;
	inicializa();
	m_modo=0;
}// end articleslist


void articleslist::inicializa() {
	m_list->setNumRows( 0 );
   m_list->setNumCols( 0 );
   m_list->setSelectionMode( QTable::SingleRow );
   m_list->setSorting( TRUE );
   m_list->setSelectionMode( QTable::SingleRow );
   m_list->setColumnMovingEnabled( TRUE );
   m_list->setNumCols(16);
   m_list->horizontalHeader()->setLabel( COL_IDARTICULO, tr( "Identificador" ) );
   m_list->horizontalHeader()->setLabel( COL_CODARTICULO, tr( "Código" ) );
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
   m_list->horizontalHeader()->setLabel( COL_IDLINEA_PROD, tr("Línea de Producción") );
   
   m_list->setColumnWidth(COL_IDARTICULO,100);
   m_list->setColumnWidth(COL_CODARTICULO,100);
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
   m_list->setColumnWidth(COL_IDLINEA_PROD,50);
   
	//listado->setPaletteBackgroundColor(QColor(150,230,230));
	// Establecemos el color de fondo del extracto. El valor lo tiene la clase configuracion que es global.
	m_list->setPaletteBackgroundColor("#FAAFFA");   
	m_list->setReadOnly(TRUE);        
	companyact->begin();
	cursor2 * cur= companyact->cargacursor("SELECT * FROM articulo","unquery");
	companyact->commit();
	m_list->setNumRows( cur->numregistros() );
	int i=0;
	while (!cur->eof()) {
		m_list->setText(i,COL_IDARTICULO,cur->valor("idarticulo"));
		m_list->setText(i,COL_CODARTICULO,cur->valor("codarticulo"));
		m_list->setText(i,COL_NOMARTICULO,cur->valor("nomarticulo"));
		m_list->setText(i,COL_DESCARTICULO,cur->valor("descarticulo"));
		m_list->setText(i,COL_CBARRASARTICULO,cur->valor("cbarrasarticulo"));
		m_list->setText(i,COL_TIPOARTICULO,cur->valor("tipoarticulo"));
		m_list->setText(i,COL_DESCUENTOARTICULO,cur->valor("descuentoarticulo"));
		m_list->setText(i,COL_ESPECIFICACIONESARTICULO,cur->valor("especificacionesarticulo"));
		m_list->setText(i,COL_ICONOARTICULO,cur->valor("iconoarticulo"));
		m_list->setText(i,COL_FOTOARTICULO,cur->valor("fotoarticulo"));
		m_list->setText(i,COL_POSTERARTICULO,cur->valor("posterarticulo"));
		m_list->setText(i,COL_MARGENARTICULO,cur->valor("margenarticulo"));
		m_list->setText(i,COL_SOBRECOSTEARTICULO,cur->valor("sobrecostearticulo"));
		m_list->setText(i,COL_MODELOARTICULO,cur->valor("modeloarticulo"));
		m_list->setText(i,COL_IDTIPO_IVA,cur->valor("idtipo_iva"));
		m_list->setText(i,COL_IDLINEA_PROD,cur->valor("idlinea_prod"));
		i++;
		cur->siguienteregistro();
	}// end while
	delete cur;
} //end inicializa


void articleslist::articleSelected(int a, int b, int c, const QPoint &) {
   m_idArticle = m_list->text(a,COL_IDARTICULO);
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
