/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
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

#include <QLineEdit>

#include "bcmasapatrimonialview.h"
#include "bcmasapatrimoniallistview.h"
#include "bccompany.h"


///
/**
\param emp
\param parent
\param fl
**/
BcMasaPatrimonialView::BcMasaPatrimonialView ( BcCompany *emp, QWidget *parent, Qt::WFlags fl )
        : QDialog ( parent, fl ), BlMainCompanyPointer ( emp )
{
    BL_FUNC_DEBUG
    setupUi ( this );
    idmpatrimonial = "";

    componentessuma->setColumnCount ( 4 );
    componentesresta->setColumnCount ( 4 );

    QStringList etiquetas;
    etiquetas << _ ( "identificador" ) << _ ( "codigo" ) << _ ( "descripcion" ) << _ ( "tipo" );
    componentessuma->setHorizontalHeaderLabels ( etiquetas );
    componentesresta->setHorizontalHeaderLabels ( etiquetas );
    
}


///
/**
**/
BcMasaPatrimonialView::~BcMasaPatrimonialView()
{
    BL_FUNC_DEBUG
    
}


///
/**
\return
**/
QString BcMasaPatrimonialView::getIdMasa()
{
    BL_FUNC_DEBUG
    
    return idmpatrimonial;
}


///
/**
\param idmpatrimonial1
**/
void BcMasaPatrimonialView::inicializa1 ( QString idmpatrimonial1 )
{
    BL_FUNC_DEBUG
    QTableWidgetItem *it0, *it1, *it2, *it3;
    int i = 0;
    idmpatrimonial = idmpatrimonial1;
    QString query = "SELECT * FROM mpatrimonial WHERE idmpatrimonial = " + idmpatrimonial;
    mainCompany() ->begin();
    BlDbRecordSet *cursor = mainCompany() ->loadQuery ( query, "micursor" );
    mainCompany() ->commit();
    if ( !cursor->eof() ) {
        descmpatrimonial->setText ( cursor->value( "descmpatrimonial" ) );
    } // end if
    delete cursor;

    /// Leemos las cuentas y las metemos en la lista que corresponda.
    mainCompany() ->begin();
    query = "SELECT * FROM cuenta WHERE idcuenta IN (SELECT idcuenta FROM compmasap WHERE masaperteneciente = " + idmpatrimonial + " AND signo = true)";
    BlDbRecordSet *cursoraux = mainCompany() ->loadQuery ( query, "cursorusuario" );
    mainCompany() ->commit();

    while ( !cursoraux->eof() ) {
        /// Anyade una nueva fila a la tabla.
        componentessuma->insertRow ( i );
        it1 = new QTableWidgetItem ( cursoraux->value( "codigo" ) );
        componentessuma->setItem ( i, 1, it1 );
        it2 = new QTableWidgetItem ( cursoraux->value( "descripcion" ) );
        componentessuma->setItem ( i, 2, it2 );
        it3 = new QTableWidgetItem ( "cuenta" );
        componentessuma->setItem ( i, 3, it3 );
        it0 = new QTableWidgetItem ( cursoraux->value( "idcuenta" ) );
        componentessuma->setItem ( i, 0, it0 );
        cursoraux->nextRecord();
        i++;
    } // end while
    delete cursoraux;

    /// Leemos las masas patrimoniales y las metemos en la lista que corresponda.
    mainCompany() ->begin();
    query = "SELECT * FROM mpatrimonial WHERE idmpatrimonial IN (SELECT idmpatrimonial FROM compmasap WHERE masaperteneciente = " + idmpatrimonial + " AND signo = true)";
    cursoraux = mainCompany() ->loadQuery ( query, "cursorusuario" );
    mainCompany() ->commit();
    i = 0;
    while ( !cursoraux->eof() ) {
        componentessuma->insertRow ( i );
        it1 = new QTableWidgetItem ( "" );
        componentessuma->setItem ( i, 1, it1 );
        it2 = new QTableWidgetItem ( cursoraux->value( "descmpatrimonial" ) );
        componentessuma->setItem ( i, 2, it2 );
        it3 = new QTableWidgetItem ( "masa patrimonial" );
        componentessuma->setItem ( i, 3, it3 );
        it0 = new QTableWidgetItem ( cursoraux->value( "idmpatrimonial" ) );
        componentessuma->setItem ( i, 0, it0 );
        i++;
        cursoraux->nextRecord();
    } // end while
    delete cursoraux;

    /// Leemos las cuentas y las metemos en la lista que corresponda.
    mainCompany() ->begin();
    query = "SELECT * FROM cuenta WHERE idcuenta IN (SELECT idcuenta FROM compmasap WHERE masaperteneciente = " + idmpatrimonial + " AND signo = false)";
    BlDbRecordSet *cursoraux1 = mainCompany() ->loadQuery ( query, "cursorusuario" );
    mainCompany() ->commit();
    i = 0;
    while ( !cursoraux1->eof() ) {
        componentesresta->insertRow ( i );
        it1 = new QTableWidgetItem ( cursoraux1->value( "codigo" ) );
        componentesresta->setItem ( i, 1, it1 );
        it2 = new QTableWidgetItem ( cursoraux1->value( "descripcion" ) );
        componentesresta->setItem ( i, 2, it2 );
        it3 = new QTableWidgetItem ( "cuenta" );
        componentesresta->setItem ( i, 3, it3 );
        it0 = new QTableWidgetItem ( cursoraux1->value( "idcuenta" ) );
        componentesresta->setItem ( i, 0, it0 );
        i++;
        cursoraux1->nextRecord();
    } // end while
    delete cursoraux1;

    /// Leemos las masas patrimoniales y las metemos en la lista que corresponda.
    mainCompany() ->begin();
    query = "SELECT * FROM mpatrimonial WHERE idmpatrimonial IN (SELECT idmpatrimonial FROM compmasap WHERE masaperteneciente = " + idmpatrimonial + " AND signo = false)";
    cursoraux = mainCompany() ->loadQuery ( query, "cursorusuario" );
    mainCompany() ->commit();
    i = 0;
    while ( !cursoraux->eof() ) {
        componentesresta->insertRow ( i );
        it1 = new QTableWidgetItem ( "" );
        componentesresta->setItem ( i, 1, it1 );
        it2 = new QTableWidgetItem ( cursoraux->value( "descmpatrimonial" ) );
        componentesresta->setItem ( i, 2, it2 );
        it3 = new QTableWidgetItem ( "masa patrimonial" );
        componentesresta->setItem ( i, 3, it3 );
        it0 = new QTableWidgetItem ( cursoraux->value( "idmpatrimonial" ) );
        componentesresta->setItem ( i, 0, it0 );
        i++;
        cursoraux->nextRecord();
    } // end while
    delete cursoraux;
    
}


/// Se ha pulsado sobre el bot&oacute; de agregar donde est&aacute; la suma.
/// Tenemos que a&ntilde;dir la masa patrimonial o la cuenta a la suma.
/**
**/
void BcMasaPatrimonialView::on_mui_nuevasuma_clicked()
{
    BL_FUNC_DEBUG
    QTableWidgetItem *it0, *it1, *it2, *it3;
    int i;

    /// Lo primero de todo es coger el c&oacute;igo de cuenta.
    QString codcuenta = codigocta1->text();
    QString masapatrimonial = mpatrimonial->text();

    /// Leemos las cuentas y las metemos en la lista que corresponda.
    mainCompany() ->begin();
    QString query = "SELECT * FROM cuenta WHERE codigo = '" + codcuenta + "'";
    BlDbRecordSet *cursoraux1 = mainCompany() ->loadQuery ( query, "cursorusuario" );
    mainCompany() ->commit();
    i = componentessuma->rowCount();
    while ( !cursoraux1->eof() ) {
        componentessuma->insertRow ( i );
        it1 = new QTableWidgetItem ( cursoraux1->value( "codigo" ) );
        componentessuma->setItem ( i, 1, it1 );
        it2 = new QTableWidgetItem ( cursoraux1->value( "descripcion" ) );
        componentessuma->setItem ( i, 2, it2 );
        it3 = new QTableWidgetItem ( "cuenta" );
        componentessuma->setItem ( i, 3, it3 );
        it0 = new QTableWidgetItem ( cursoraux1->value( "idcuenta" ) );
        componentessuma->setItem ( i, 0, it0 );
        i++;
        cursoraux1->nextRecord();
    } // end while
    delete cursoraux1;

    /// Leemos las masas patrimoniales y las metemos en la lista que corresponda.
    mainCompany() ->begin();
    query = "SELECT * FROM mpatrimonial WHERE idmpatrimonial = " + masapatrimonial;
    cursoraux1 = mainCompany() ->loadQuery ( query, "cursormpatrimonial" );
    mainCompany() ->commit();
    i = componentessuma->rowCount();
    while ( !cursoraux1->eof() ) {
        componentessuma->insertRow ( i );
        it1 = new QTableWidgetItem ( "" );
        componentessuma->setItem ( i, 1, it1 );
        it2 = new QTableWidgetItem ( cursoraux1->value( "descmpatrimonial" ) );
        componentessuma->setItem ( i, 2, it2 );
        it3 = new QTableWidgetItem ( "masa patrimonial" );
        componentessuma->setItem ( i, 3, it3 );
        it0 = new QTableWidgetItem ( cursoraux1->value( "idmpatrimonial" ) );
        componentessuma->setItem ( i, 0, it0 );
        i++;
        cursoraux1->nextRecord();
    } // end while
    
}


///
/**
**/
void BcMasaPatrimonialView::on_mui_borrasuma_clicked()
{
    BL_FUNC_DEBUG
    componentessuma->removeRow ( componentessuma->currentRow() );
    
}


/// Se ha pulsado sobre el bot&oacute; de agregar donde est&aacute; la suma.
/// Tenemos que a&ntilde;dir la masa patrimonial o la cuenta a la suma.
/**
**/
void BcMasaPatrimonialView::on_mui_nuevaresta_clicked()
{
    BL_FUNC_DEBUG
    QTableWidgetItem *it0, *it1, *it2, *it3;
    int i;

    /// Lo primero de todo es coger el c&oacute;igo de cuenta.
    QString codcuenta = codigocta1->text();
    QString masapatrimonial = mpatrimonial->text();

    /// Leemos las cuentas y las metemos en la lista que corresponda.
    mainCompany() ->begin();
    QString query = "SELECT * FROM cuenta WHERE codigo = '" + codcuenta + "'";
    BlDbRecordSet *cursoraux1 = mainCompany() ->loadQuery ( query, "cursorusuario" );
    mainCompany() ->commit();
    i = componentesresta->rowCount();
    while ( !cursoraux1->eof() ) {
        componentesresta->insertRow ( i );
        it1 = new QTableWidgetItem ( cursoraux1->value( "codigo" ) );
        componentesresta->setItem ( i, 1, it1 );
        it2 = new QTableWidgetItem ( cursoraux1->value( "descripcion" ) );
        componentesresta->setItem ( i, 2, it2 );
        it3 = new QTableWidgetItem ( "cuenta" );
        componentesresta->setItem ( i, 3, it3 );
        it0 = new QTableWidgetItem ( cursoraux1->value( "idcuenta" ) );
        componentesresta->setItem ( i, 0, it0 );
        i++;
        cursoraux1->nextRecord();
    } // end while
    delete cursoraux1;

    /// Leemos las masas patrimoniales y las metemos en la lista que corresponda.
    mainCompany() ->begin();
    query = "SELECT * FROM mpatrimonial WHERE idmpatrimonial = " + masapatrimonial;
    cursoraux1 = mainCompany() ->loadQuery ( query, "cursormpatrimonial" );
    mainCompany() ->commit();
    i = componentesresta->rowCount();
    while ( !cursoraux1->eof() ) {
        componentesresta->insertRow ( i );
        it1 = new QTableWidgetItem ( "" );
        componentesresta->setItem ( i, 1, it1 );
        it2 = new QTableWidgetItem ( cursoraux1->value( "descmpatrimonial" ) );
        componentesresta->setItem ( i, 2, it2 );
        it3 = new QTableWidgetItem ( "masa patrimonial" );
        componentesresta->setItem ( i, 3, it3 );
        it0 = new QTableWidgetItem ( cursoraux1->value( "idmpatrimonial" ) );
        componentesresta->setItem ( i, 0, it0 );
        i++;
        cursoraux1->nextRecord();
    } // end while
    
}


///
/**
**/
void BcMasaPatrimonialView::on_mui_borraresta_clicked()
{
    BL_FUNC_DEBUG
    componentesresta->removeRow ( componentesresta->currentRow() );
    
}


///
/**
**/
void BcMasaPatrimonialView::on_mui_buscampatrimonial_clicked()
{
    BL_FUNC_DEBUG
    BcMasaPatrimonialListView *nuevae = new BcMasaPatrimonialListView ( ( BcCompany * ) mainCompany(), 0 );
    nuevae->inicializa();
    nuevae->setModoSelector();
    nuevae->exec();
    mpatrimonial->setText ( nuevae->getIdMasa() );
    delete nuevae;
    
}


///
/**
**/
void BcMasaPatrimonialView::on_mui_cancelar_clicked()
{
    BL_FUNC_DEBUG
    close();
    
}


///
/**
**/
void BcMasaPatrimonialView::on_mui_aceptar_clicked()
{
    BL_FUNC_DEBUG
    int i;
    QString query;

    /// Si idmpatrimonial == NULL implica que es una inserci&oacute;n y no una modificaci&oacute;n
    if ( idmpatrimonial == "" ) {
        mainCompany() ->begin();
        query.sprintf ( "INSERT INTO mpatrimonial (descmpatrimonial) VALUES ('nueva masa')" );
        mainCompany() ->runQuery ( query.toAscii() );
        query.sprintf ( "SELECT MAX(idmpatrimonial) as id FROM mpatrimonial" );
        BlDbRecordSet *curs = mainCompany() ->loadQuery ( query, "cargaid" );
        mainCompany() ->commit();
        idmpatrimonial = curs->value( "id" ).toAscii();
    } // end if

    /// Ponemos los datos correctos sobre la masa patrimonial.
    QString text = descmpatrimonial->text();
    query.sprintf ( "UPDATE mpatrimonial SET descmpatrimonial = '%s' WHERE idmpatrimonial = %s", text.toAscii().constData(), idmpatrimonial.toAscii().constData() );
    mainCompany() ->runQuery ( query );

    query.sprintf ( "DELETE FROM compmasap WHERE masaperteneciente = %s", idmpatrimonial.toAscii().constData() );
    mainCompany() ->runQuery ( query );

    for ( i = 0; i < componentessuma->rowCount(); i++ ) {
        QString id = componentessuma->item ( i, 0 ) ->text();
        QString tipo = componentessuma->item ( i, 3 ) ->text();

        if ( tipo == "cuenta" ) {
            query.sprintf ( "INSERT INTO compmasap(idcuenta, idmpatrimonial, masaperteneciente, signo) VALUES (%s, NULL, %s, true)",
                            mainCompany() ->sanearCadena ( id ).toAscii().constData(),
                            mainCompany() ->sanearCadena ( idmpatrimonial ).toAscii().constData() );
        } else {
            query.sprintf ( "INSERT INTO compmasap(idcuenta, idmpatrimonial, masaperteneciente, signo) VALUES (NULL, %s, %s, true)",
                            mainCompany() ->sanearCadena ( id ).toAscii().constData(),
                            mainCompany() ->sanearCadena ( idmpatrimonial ).toAscii().constData() );
        } // end if
        mainCompany() ->runQuery ( query );
    } // end for

    for ( i = 0; i < componentesresta->rowCount(); i++ ) {
        QString id = componentesresta->item ( i, 0 ) ->text();
        QString tipo = componentesresta->item ( i, 3 ) ->text();

        if ( tipo == "cuenta" ) {
            query.sprintf ( "INSERT INTO compmasap(idcuenta, idmpatrimonial, masaperteneciente, signo) VALUES (%s, NULL, %s, false)",
                            mainCompany() ->sanearCadena ( id ).toAscii().constData(),
                            mainCompany() ->sanearCadena ( idmpatrimonial ).toAscii().constData() );
        } else {
            query.sprintf ( "INSERT INTO compmasap(idcuenta, idmpatrimonial, masaperteneciente, signo) VALUES (NULL, %s, %s, false)",
                            mainCompany() ->sanearCadena ( id ).toAscii().constData(),
                            mainCompany() ->sanearCadena ( idmpatrimonial ).toAscii().constData() );
        } // end if
        mainCompany() ->runQuery ( query );
    } // end for
    close();
    
}


///
/**
\return
**/
QString BcMasaPatrimonialView::getNomMasa()
{
    BL_FUNC_DEBUG
    
    return descmpatrimonial->text().toAscii();
}

