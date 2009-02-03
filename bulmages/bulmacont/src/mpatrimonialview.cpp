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

#include "mpatrimonialview.h"
#include "mpatrimonialesview.h"
#include "empresa.h"


///
/**
\param emp
\param parent
\param fl
**/
mpatrimonialview::mpatrimonialview ( Empresa *emp, QWidget *parent, Qt::WFlags fl )
        : QDialog ( parent, fl ), PEmpresaBase ( emp )
{
    _depura ( "mpatrimonialview::mpatrimonialview", 0 );
    setupUi ( this );
    idmpatrimonial = "";

    componentessuma->setColumnCount ( 4 );
    componentesresta->setColumnCount ( 4 );

    QStringList etiquetas;
    etiquetas << _( "identificador" ) << _( "codigo" ) << _( "descripcion" ) << _( "tipo" );
    componentessuma->setHorizontalHeaderLabels ( etiquetas );
    componentesresta->setHorizontalHeaderLabels ( etiquetas );
    _depura ( "END mpatrimonialview::mpatrimonialview", 0 );
}


///
/**
**/
mpatrimonialview::~mpatrimonialview()
{
    _depura ( "mpatrimonialview::~mpatrimonialview", 0 );
    _depura ( "END mpatrimonialview::~mpatrimonialview", 0 );
}


///
/**
\return
**/
QString mpatrimonialview::getidmasa()
{
    _depura ( "mpatrimonialview::getidmasa", 0 );
    _depura ( "END mpatrimonialview::getidmasa", 0 );
    return idmpatrimonial;
}


///
/**
\param idmpatrimonial1
**/
void mpatrimonialview::inicializa1 ( QString idmpatrimonial1 )
{
    _depura ( "mpatrimonialview::inicializa1", 0 );
    QTableWidgetItem *it0, *it1, *it2, *it3;
    int i = 0;
    idmpatrimonial = idmpatrimonial1;
    QString query = "SELECT * FROM mpatrimonial WHERE idmpatrimonial = " + idmpatrimonial;
    empresaBase() ->begin();
    cursor2 *cursor = empresaBase() ->cargacursor ( query, "micursor" );
    empresaBase() ->commit();
    if ( !cursor->eof() ) {
        descmpatrimonial->setText ( cursor->valor ( "descmpatrimonial" ) );
    } // end if
    delete cursor;

    /// Leemos las cuentas y las metemos en la lista que corresponda.
    empresaBase() ->begin();
    query = "SELECT * FROM cuenta WHERE idcuenta IN (SELECT idcuenta FROM compmasap WHERE masaperteneciente = " + idmpatrimonial + " AND signo = true)";
    cursor2 *cursoraux = empresaBase() ->cargacursor ( query, "cursorusuario" );
    empresaBase() ->commit();

    while ( !cursoraux->eof() ) {
        /// Anyade una nueva fila a la tabla.
        componentessuma->insertRow ( i );
        it1 = new QTableWidgetItem ( cursoraux->valor ( "codigo" ) );
        componentessuma->setItem ( i, 1, it1 );
        it2 = new QTableWidgetItem ( cursoraux->valor ( "descripcion" ) );
        componentessuma->setItem ( i, 2, it2 );
        it3 = new QTableWidgetItem ( "cuenta" );
        componentessuma->setItem ( i, 3, it3 );
        it0 = new QTableWidgetItem ( cursoraux->valor ( "idcuenta" ) );
        componentessuma->setItem ( i, 0, it0 );
        cursoraux->siguienteregistro();
        i++;
    } // end while
    delete cursoraux;

    /// Leemos las masas patrimoniales y las metemos en la lista que corresponda.
    empresaBase() ->begin();
    query = "SELECT * FROM mpatrimonial WHERE idmpatrimonial IN (SELECT idmpatrimonial FROM compmasap WHERE masaperteneciente = " + idmpatrimonial + " AND signo = true)";
    cursoraux = empresaBase() ->cargacursor ( query, "cursorusuario" );
    empresaBase() ->commit();
    i = 0;
    while ( !cursoraux->eof() ) {
        componentessuma->insertRow ( i );
        it1 = new QTableWidgetItem ( "" );
        componentessuma->setItem ( i, 1, it1 );
        it2 = new QTableWidgetItem ( cursoraux->valor ( "descmpatrimonial" ) );
        componentessuma->setItem ( i, 2, it2 );
        it3 = new QTableWidgetItem ( "masa patrimonial" );
        componentessuma->setItem ( i, 3, it3 );
        it0 = new QTableWidgetItem ( cursoraux->valor ( "idmpatrimonial" ) );
        componentessuma->setItem ( i, 0, it0 );
        i++;
        cursoraux->siguienteregistro();
    } // end while
    delete cursoraux;

    /// Leemos las cuentas y las metemos en la lista que corresponda.
    empresaBase() ->begin();
    query = "SELECT * FROM cuenta WHERE idcuenta IN (SELECT idcuenta FROM compmasap WHERE masaperteneciente = " + idmpatrimonial + " AND signo = false)";
    cursor2 *cursoraux1 = empresaBase() ->cargacursor ( query, "cursorusuario" );
    empresaBase() ->commit();
    i = 0;
    while ( !cursoraux1->eof() ) {
        componentesresta->insertRow ( i );
        it1 = new QTableWidgetItem ( cursoraux1->valor ( "codigo" ) );
        componentesresta->setItem ( i, 1, it1 );
        it2 = new QTableWidgetItem ( cursoraux1->valor ( "descripcion" ) );
        componentesresta->setItem ( i, 2, it2 );
        it3 = new QTableWidgetItem ( "cuenta" );
        componentesresta->setItem ( i, 3, it3 );
        it0 = new QTableWidgetItem ( cursoraux1->valor ( "idcuenta" ) );
        componentesresta->setItem ( i, 0, it0 );
        i++;
        cursoraux1->siguienteregistro();
    } // end while
    delete cursoraux1;

    /// Leemos las masas patrimoniales y las metemos en la lista que corresponda.
    empresaBase() ->begin();
    query = "SELECT * FROM mpatrimonial WHERE idmpatrimonial IN (SELECT idmpatrimonial FROM compmasap WHERE masaperteneciente = " + idmpatrimonial + " AND signo = false)";
    cursoraux = empresaBase() ->cargacursor ( query, "cursorusuario" );
    empresaBase() ->commit();
    i = 0;
    while ( !cursoraux->eof() ) {
        componentesresta->insertRow ( i );
        it1 = new QTableWidgetItem ( "" );
        componentesresta->setItem ( i, 1, it1 );
        it2 = new QTableWidgetItem ( cursoraux->valor ( "descmpatrimonial" ) );
        componentesresta->setItem ( i, 2, it2 );
        it3 = new QTableWidgetItem ( "masa patrimonial" );
        componentesresta->setItem ( i, 3, it3 );
        it0 = new QTableWidgetItem ( cursoraux->valor ( "idmpatrimonial" ) );
        componentesresta->setItem ( i, 0, it0 );
        i++;
        cursoraux->siguienteregistro();
    } // end while
    delete cursoraux;
    _depura ( "mpatrimonialview::inicializa1", 0 );
}


/// Se ha pulsado sobre el bot&oacute; de agregar donde est&aacute; la suma.
/// Tenemos que a&ntilde;dir la masa patrimonial o la cuenta a la suma.
/**
**/
void mpatrimonialview::on_mui_nuevasuma_clicked()
{
    _depura ( "mpatrimonialview::on_mui_nuevasuma_clicked", 0 );
    QTableWidgetItem *it0, *it1, *it2, *it3;
    int i;

    /// Lo primero de todo es coger el c&oacute;igo de cuenta.
    QString codcuenta = codigocta1->text();
    QString masapatrimonial = mpatrimonial->text();

    /// Leemos las cuentas y las metemos en la lista que corresponda.
    empresaBase() ->begin();
    QString query = "SELECT * FROM cuenta WHERE codigo = '" + codcuenta + "'";
    cursor2 *cursoraux1 = empresaBase() ->cargacursor ( query, "cursorusuario" );
    empresaBase() ->commit();
    i = componentessuma->rowCount();
    while ( !cursoraux1->eof() ) {
        componentessuma->insertRow ( i );
        it1 = new QTableWidgetItem ( cursoraux1->valor ( "codigo" ) );
        componentessuma->setItem ( i, 1, it1 );
        it2 = new QTableWidgetItem ( cursoraux1->valor ( "descripcion" ) );
        componentessuma->setItem ( i, 2, it2 );
        it3 = new QTableWidgetItem ( "cuenta" );
        componentessuma->setItem ( i, 3, it3 );
        it0 = new QTableWidgetItem ( cursoraux1->valor ( "idcuenta" ) );
        componentessuma->setItem ( i, 0, it0 );
        i++;
        cursoraux1->siguienteregistro();
    } // end while
    delete cursoraux1;

    /// Leemos las masas patrimoniales y las metemos en la lista que corresponda.
    empresaBase() ->begin();
    query = "SELECT * FROM mpatrimonial WHERE idmpatrimonial = " + masapatrimonial;
    cursoraux1 = empresaBase() ->cargacursor ( query, "cursormpatrimonial" );
    empresaBase() ->commit();
    i = componentessuma->rowCount();
    while ( !cursoraux1->eof() ) {
        componentessuma->insertRow ( i );
        it1 = new QTableWidgetItem ( "" );
        componentessuma->setItem ( i, 1, it1 );
        it2 = new QTableWidgetItem ( cursoraux1->valor ( "descmpatrimonial" ) );
        componentessuma->setItem ( i, 2, it2 );
        it3 = new QTableWidgetItem ( "masa patrimonial" );
        componentessuma->setItem ( i, 3, it3 );
        it0 = new QTableWidgetItem ( cursoraux1->valor ( "idmpatrimonial" ) );
        componentessuma->setItem ( i, 0, it0 );
        i++;
        cursoraux1->siguienteregistro();
    } // end while
    _depura ( "END mpatrimonialview::on_mui_nuevasuma_clicked", 0 );
}


///
/**
**/
void mpatrimonialview::on_mui_borrasuma_clicked()
{
    _depura ( "mpatrimonialview::on_mui_nuevasuma_clicked", 0 );
    componentessuma->removeRow ( componentessuma->currentRow() );
    _depura ( "END mpatrimonialview::on_mui_borrasuma_clicked", 0 );
}


/// Se ha pulsado sobre el bot&oacute; de agregar donde est&aacute; la suma.
/// Tenemos que a&ntilde;dir la masa patrimonial o la cuenta a la suma.
/**
**/
void mpatrimonialview::on_mui_nuevaresta_clicked()
{
    _depura ( "mpatrimonialview::on_mui_nuevaresta_clicked", 0 );
    QTableWidgetItem *it0, *it1, *it2, *it3;
    int i;

    /// Lo primero de todo es coger el c&oacute;igo de cuenta.
    QString codcuenta = codigocta1->text();
    QString masapatrimonial = mpatrimonial->text();

    /// Leemos las cuentas y las metemos en la lista que corresponda.
    empresaBase() ->begin();
    QString query = "SELECT * FROM cuenta WHERE codigo = '" + codcuenta + "'";
    cursor2 *cursoraux1 = empresaBase() ->cargacursor ( query, "cursorusuario" );
    empresaBase() ->commit();
    i = componentesresta->rowCount();
    while ( !cursoraux1->eof() ) {
        componentesresta->insertRow ( i );
        it1 = new QTableWidgetItem ( cursoraux1->valor ( "codigo" ) );
        componentesresta->setItem ( i, 1, it1 );
        it2 = new QTableWidgetItem ( cursoraux1->valor ( "descripcion" ) );
        componentesresta->setItem ( i, 2, it2 );
        it3 = new QTableWidgetItem ( "cuenta" );
        componentesresta->setItem ( i, 3, it3 );
        it0 = new QTableWidgetItem ( cursoraux1->valor ( "idcuenta" ) );
        componentesresta->setItem ( i, 0, it0 );
        i++;
        cursoraux1->siguienteregistro();
    } // end while
    delete cursoraux1;

    /// Leemos las masas patrimoniales y las metemos en la lista que corresponda.
    empresaBase() ->begin();
    query = "SELECT * FROM mpatrimonial WHERE idmpatrimonial = " + masapatrimonial;
    cursoraux1 = empresaBase() ->cargacursor ( query, "cursormpatrimonial" );
    empresaBase() ->commit();
    i = componentesresta->rowCount();
    while ( !cursoraux1->eof() ) {
        componentesresta->insertRow ( i );
        it1 = new QTableWidgetItem ( "" );
        componentesresta->setItem ( i, 1, it1 );
        it2 = new QTableWidgetItem ( cursoraux1->valor ( "descmpatrimonial" ) );
        componentesresta->setItem ( i, 2, it2 );
        it3 = new QTableWidgetItem ( "masa patrimonial" );
        componentesresta->setItem ( i, 3, it3 );
        it0 = new QTableWidgetItem ( cursoraux1->valor ( "idmpatrimonial" ) );
        componentesresta->setItem ( i, 0, it0 );
        i++;
        cursoraux1->siguienteregistro();
    } // end while
    _depura ( "END mpatrimonialview::on_mui_nuevaresta_clicked", 0 );
}


///
/**
**/
void mpatrimonialview::on_mui_borraresta_clicked()
{
    _depura ( "mpatrimonialview::on_mui_borraresta_clicked", 0 );
    componentesresta->removeRow ( componentesresta->currentRow() );
    _depura ( "END mpatrimonialview::on_mui_borraresta_clicked", 0 );
}


///
/**
**/
void mpatrimonialview::on_mui_buscampatrimonial_clicked()
{
    _depura ( "mpatrimonialview::on_mui_buscampatrimonial_clicked", 0 );
    mpatrimonialesview *nuevae = new mpatrimonialesview ( ( Empresa * ) empresaBase(), 0 );
    nuevae->inicializa();
    nuevae->setmodoselector();
    nuevae->exec();
    mpatrimonial->setText ( nuevae->getidmasa() );
    delete nuevae;
    _depura ( "END mpatrimonialview::on_mui_buscampatrimonial_clicked", 0 );
}


///
/**
**/
void mpatrimonialview::on_mui_cancelar_clicked()
{
    _depura ( "mpatrimonialview::on_mui_cancelar_clicked", 0 );
    close();
    _depura ( "END mpatrimonialview::on_mui_cancelar_clicked", 0 );
}


///
/**
**/
void mpatrimonialview::on_mui_aceptar_clicked()
{
    _depura ( "mpatrimonialview::on_mui_aceptar_clicked", 0 );
    int i;
    QString query;

    /// Si idmpatrimonial == NULL implica que es una inserci&oacute;n y no una modificaci&oacute;n
    if ( idmpatrimonial == "" ) {
        empresaBase() ->begin();
        query.sprintf ( "INSERT INTO mpatrimonial (descmpatrimonial) VALUES ('nueva masa')" );
        empresaBase() ->ejecuta ( query.toAscii() );
        query.sprintf ( "SELECT MAX(idmpatrimonial) as id FROM mpatrimonial" );
        cursor2 *curs = empresaBase() ->cargacursor ( query, "cargaid" );
        empresaBase() ->commit();
        idmpatrimonial = curs->valor ( "id" ).toAscii();
    } // end if

    /// Ponemos los datos correctos sobre la masa patrimonial.
    QString text = descmpatrimonial->text();
    query.sprintf ( "UPDATE mpatrimonial SET descmpatrimonial = '%s' WHERE idmpatrimonial = %s", text.toAscii().constData(), idmpatrimonial.toAscii().constData() );
    empresaBase() ->ejecuta ( query );

    query.sprintf ( "DELETE FROM compmasap WHERE masaperteneciente = %s", idmpatrimonial.toAscii().constData() );
    empresaBase() ->ejecuta ( query );

    for ( i = 0; i < componentessuma->rowCount(); i++ ) {
        QString id = componentessuma->item ( i, 0 ) ->text();
        QString tipo = componentessuma->item ( i, 3 ) ->text();

        if ( tipo == "cuenta" ) {
            query.sprintf ( "INSERT INTO compmasap(idcuenta, idmpatrimonial, masaperteneciente, signo) VALUES (%s, NULL, %s, true)",
                            empresaBase() ->sanearCadena ( id ).toAscii().constData(),
                            empresaBase() ->sanearCadena ( idmpatrimonial ).toAscii().constData() );
        } else {
            query.sprintf ( "INSERT INTO compmasap(idcuenta, idmpatrimonial, masaperteneciente, signo) VALUES (NULL, %s, %s, true)",
                            empresaBase() ->sanearCadena ( id ).toAscii().constData(),
                            empresaBase() ->sanearCadena ( idmpatrimonial ).toAscii().constData() );
        } // end if
        empresaBase() ->ejecuta ( query );
    } // end for

    for ( i = 0; i < componentesresta->rowCount(); i++ ) {
        QString id = componentesresta->item ( i, 0 ) ->text();
        QString tipo = componentesresta->item ( i, 3 ) ->text();

        if ( tipo == "cuenta" ) {
            query.sprintf ( "INSERT INTO compmasap(idcuenta, idmpatrimonial, masaperteneciente, signo) VALUES (%s, NULL, %s, false)",
                            empresaBase() ->sanearCadena ( id ).toAscii().constData(),
                            empresaBase() ->sanearCadena ( idmpatrimonial ).toAscii().constData() );
        } else {
            query.sprintf ( "INSERT INTO compmasap(idcuenta, idmpatrimonial, masaperteneciente, signo) VALUES (NULL, %s, %s, false)",
                            empresaBase() ->sanearCadena ( id ).toAscii().constData(),
                            empresaBase() ->sanearCadena ( idmpatrimonial ).toAscii().constData() );
        } // end if
        empresaBase() ->ejecuta ( query );
    } // end for
    close();
    _depura ( "END mpatrimonialview::on_mui_aceptar_clicked", 0 );
}


///
/**
\return
**/
QString mpatrimonialview::getnommasa()
{
    _depura ( "mpatrimonialview::getnommasa", 0 );
    _depura ( "END mpatrimonialview::getnommasa", 0 );
    return descmpatrimonial->text().toAscii();
}

