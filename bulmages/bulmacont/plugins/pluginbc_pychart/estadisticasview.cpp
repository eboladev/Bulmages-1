/***************************************************************************
                          estadisticasview.cpp  -  description
                             -------------------
    begin                : mar jul 15 2003
    copyright            : (C) 2003 by Tomeu Borras Riera
    email                : tborras@conetxia.com
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "estadisticasview.h"
#include <qlabel.h>
#include <q3popupmenu.h>
#include <qaction.h>
#include <qobject.h>
#include <qmessagebox.h>
//Added by qt3to4:
#include <QtGui/QPixmap>

int estadisticasview::inicializa ( BlPostgreSqlClient *conexion, int tipo )
{
    dbConnection = conexion;
    if ( tipo == 0 )
        presentar();
    else
        presentarbarras();
    return ( 0 );
}// end inicializa

estadisticasview::estadisticasview ( QWidget *parent, const char *name ) : estadisticasdlg ( parent, name )
{}


estadisticasview::~estadisticasview()
{}


void estadisticasview::presentar()
{
    QString query;
    BlDbRecordSet *cursorapt;
    QString finicial = "01/01/2003";
    QString ffinal = "31/12/2005";
    QString cinicial = "10";
    QString cfinal = "99";

    // La consulta es compleja, requiere la creacion de una tabla temporal y de cierta mandanga por lo que puede
    // Causar problemas con el motor de base de datos.
    fprintf ( stderr, "BALANCE: Empezamos a hacer la presentacion\n" );
    dbConnection->begin();
    query.sprintf ( "CREATE TEMPORARY TABLE balance AS SELECT cuenta.idcuenta, codigo, nivel(codigo) AS nivel, cuenta.descripcion, padre, tipocuenta ,debe, haber, tdebe, thaber,(tdebe-thaber) AS tsaldo, (debe-haber) AS saldo, adebe, ahaber, (adebe-ahaber) AS asaldo FROM cuenta LEFT JOIN (SELECT idcuenta, sum(debe) AS tdebe, sum(haber) AS thaber FROM apunte WHERE fecha >= '%s' AND fecha<= '%s' GROUP BY idcuenta) AS t1 ON t1.idcuenta = cuenta.idcuenta LEFT JOIN (SELECT idcuenta, sum(debe) AS adebe, sum(haber) AS ahaber FROM apunte WHERE fecha < '%s' GROUP BY idcuenta) AS t2 ON t2.idcuenta = cuenta.idcuenta", finicial.ascii(), ffinal.ascii(), finicial.ascii() );
    dbConnection->runQuery ( query );
    query.sprintf ( "UPDATE BALANCE SET padre=0 WHERE padre ISNULL" );
    dbConnection->runQuery ( query );
    query.sprintf ( "DELETE FROM balance WHERE debe=0 AND haber =0" );
    dbConnection->runQuery ( query );

    // Para evitar problemas con los nulls hacemos algunos updates
    query.sprintf ( "UPDATE BALANCE SET tsaldo=0 WHERE tsaldo ISNULL" );
    dbConnection->runQuery ( query );
    query.sprintf ( "UPDATE BALANCE SET tdebe=0 WHERE tdebe ISNULL" );
    dbConnection->runQuery ( query );
    query.sprintf ( "UPDATE BALANCE SET thaber=0 WHERE thaber ISNULL" );
    dbConnection->runQuery ( query );
    query.sprintf ( "UPDATE BALANCE SET asaldo=0 WHERE asaldo ISNULL" );
    dbConnection->runQuery ( query );


    query.sprintf ( "SELECT idcuenta FROM balance ORDER BY padre DESC" );
    cursorapt = dbConnection->loadQuery ( query, "Balance1view" );

    while ( !cursorapt->eof() )  {
        query.sprintf ( "SELECT * FROM balance WHERE idcuenta=%s", cursorapt->value( "idcuenta" ).ascii() );
        BlDbRecordSet *mycur = dbConnection->loadQuery ( query, "cursorrefresco" );

        query.sprintf ( "UPDATE balance SET tsaldo = tsaldo + (%2.2f), tdebe = tdebe + (%2.2f), thaber = thaber +(%2.2f), asaldo= asaldo+(%2.2f) WHERE idcuenta = %d", atof ( mycur->value( "tsaldo" ).ascii() ), atof ( mycur->value( "tdebe" ).ascii() ), atof ( mycur->value( "thaber" ).ascii() ), atof ( mycur->value( "asaldo" ).ascii() ),  atoi ( mycur->value( "padre" ).ascii() ) );
        //   fprintf(stderr,"%s para el codigo\n",query, cursorapt->value("codigo").c_str());
        dbConnection->runQuery ( query );
        delete mycur;
        cursorapt->nextRecord();
    }// end while
    delete cursorapt;


    // Borramos todo lo que no es de este nivel
    query.sprintf ( "DELETE FROM balance where nivel(codigo)>%s", "2" );
    dbConnection->runQuery ( query );

    //Borramos todo lo que tiene un hijo en el balance
    query.sprintf ( "DELETE FROM balance WHERE idcuenta IN (SELECT padre FROM balance)" );
    dbConnection->runQuery ( query );


    query.sprintf ( "SELECT descripcion, abs(tsaldo)::integer AS tsaldoi FROM balance WHERE debe <> 0  OR haber <> 0 ORDER BY codigo" );
    cursorapt = dbConnection->loadQuery ( query, "mycursor" );

    QString valores = "data=[";
    int init = 0;


    while ( !cursorapt->eof() ) {
        if ( init != 0 ) valores += ",";
        // Acumulamos los totales para al final poder escribirlos
        valores += "(\\\"" + cursorapt->value( "descripcion" ) + "\\\"," + QString::number ( cursorapt->value( "tsaldoi" ).toInt() ) + ")";
        cursorapt->nextRecord();
        init = 1;
    }// end while
    valores += "]";

    // Vaciamos el cursor de la base de datos.
    delete cursorapt;
    query.sprintf ( "DROP TABLE balance" );
    dbConnection->runQuery ( query );
    dbConnection->commit();

    /* values to chart */
    fprintf ( stderr, "Llamamos a sacapie\n" );
    system ( "cp /usr/lib/bulmages/plugins/plotpie.py /tmp" );
    QString cadena = " sed -e \"s&###MARCA###&" + valores + "&g\"  /tmp/plotpie.py > /tmp/plotpie1.py";
    system ( cadena.ascii() );
    system ( "python /tmp/plotpie1.py --format=png > /tmp/pie.png" );



    fprintf ( stderr, "Hemos terminado sacapie \n" );
    QPixmap *imag = new QPixmap ( "/tmp/pie.png" );
    fprintf ( stderr, "Y ahora hemos creado la imagen\n" );
    m_imagen->setPixmap ( *imag );
    fprintf ( stderr, "Y ahora la hemos mostrado\n" );
    //Destruimos la memoria utilizada
    delete imag;

}// end presentar



void estadisticasview::presentarbarras()
{
    QString query;
    BlDbRecordSet *cursorapt;
    QString finicial = "01/01/2003";
    QString ffinal = "31/12/2005";
    QString cinicial = "10";
    QString cfinal = "99";

    // La consulta es compleja, requiere la creacion de una tabla temporal y de cierta mandanga por lo que puede
    // Causar problemas con el motor de base de datos.
    fprintf ( stderr, "BALANCE: Empezamos a hacer la presentacion\n" );
    dbConnection->begin();
    query.sprintf ( "CREATE TEMPORARY TABLE balance AS SELECT cuenta.idcuenta, codigo, nivel(codigo) AS nivel, cuenta.descripcion, padre, tipocuenta ,debe, haber, tdebe, thaber,(tdebe-thaber) AS tsaldo, (debe-haber) AS saldo, adebe, ahaber, (adebe-ahaber) AS asaldo FROM cuenta LEFT JOIN (SELECT idcuenta, sum(debe) AS tdebe, sum(haber) AS thaber FROM apunte WHERE fecha >= '%s' AND fecha<= '%s' GROUP BY idcuenta) AS t1 ON t1.idcuenta = cuenta.idcuenta LEFT JOIN (SELECT idcuenta, sum(debe) AS adebe, sum(haber) AS ahaber FROM apunte WHERE fecha < '%s' GROUP BY idcuenta) AS t2 ON t2.idcuenta = cuenta.idcuenta", finicial.ascii(), ffinal.ascii(), finicial.ascii() );
    dbConnection->runQuery ( query );
    query.sprintf ( "UPDATE BALANCE SET padre=0 WHERE padre ISNULL" );
    dbConnection->runQuery ( query );
    query.sprintf ( "DELETE FROM balance WHERE debe=0 AND haber =0" );
    dbConnection->runQuery ( query );

    // Para evitar problemas con los nulls hacemos algunos updates
    query.sprintf ( "UPDATE BALANCE SET tsaldo=0 WHERE tsaldo ISNULL" );
    dbConnection->runQuery ( query );
    query.sprintf ( "UPDATE BALANCE SET tdebe=0 WHERE tdebe ISNULL" );
    dbConnection->runQuery ( query );
    query.sprintf ( "UPDATE BALANCE SET thaber=0 WHERE thaber ISNULL" );
    dbConnection->runQuery ( query );
    query.sprintf ( "UPDATE BALANCE SET asaldo=0 WHERE asaldo ISNULL" );
    dbConnection->runQuery ( query );


    query.sprintf ( "SELECT idcuenta FROM balance ORDER BY padre DESC" );
    cursorapt = dbConnection->loadQuery ( query, "Balance1view" );

    while ( !cursorapt->eof() )  {
        query.sprintf ( "SELECT * FROM balance WHERE idcuenta=%s", cursorapt->value( "idcuenta" ).ascii() );
        BlDbRecordSet *mycur = dbConnection->loadQuery ( query, "cursorrefresco" );

        query.sprintf ( "UPDATE balance SET tsaldo = tsaldo + (%2.2f), tdebe = tdebe + (%2.2f), thaber = thaber +(%2.2f), asaldo= asaldo+(%2.2f) WHERE idcuenta = %d", atof ( mycur->value( "tsaldo" ).ascii() ), atof ( mycur->value( "tdebe" ).ascii() ), atof ( mycur->value( "thaber" ).ascii() ), atof ( mycur->value( "asaldo" ).ascii() ),  atoi ( mycur->value( "padre" ).ascii() ) );
        //   fprintf(stderr,"%s para el codigo\n",query, cursorapt->value("codigo").c_str());
        dbConnection->runQuery ( query );
        delete mycur;
        cursorapt->nextRecord();
    }// end while
    delete cursorapt;


    // Borramos todo lo que no es de este nivel
    query.sprintf ( "DELETE FROM balance where nivel(codigo)>%s", "2" );
    dbConnection->runQuery ( query );

    //Borramos todo lo que tiene un hijo en el balance
    query.sprintf ( "DELETE FROM balance WHERE idcuenta IN (SELECT padre FROM balance)" );
    dbConnection->runQuery ( query );


    query.sprintf ( "SELECT descripcion, tsaldo::integer AS tsaldoi FROM balance WHERE debe <> 0  OR haber <> 0 ORDER BY codigo" );
    cursorapt = dbConnection->loadQuery ( query, "mycursor" );

    QString valores = "data=[(";
    QString plot = "";
    int init = 0;
    int iplot = 1;

    while ( !cursorapt->eof() ) {
        if ( init != 0 ) valores += ",";
        // Acumulamos los totales para al final poder escribirlos
        valores += QString::number ( cursorapt->value( "tsaldoi" ).toInt() );

        plot += "plot" + QString::number ( iplot ) + "=bar_plot.T(label=\\\"" + cursorapt->value( "descripcion" ) + "\\\", hcol=" + QString::number ( iplot - 1 ) + ", cluster=(" + QString::number ( iplot - 1 ) + ",3)) \\n";
        iplot++;
        cursorapt->nextRecord();
        init = 1;
    }// end while
    valores += ")]";
    plot += "ar.add_plot(plot1";
    for ( int i = 2;i < iplot;i++ ) {
        plot += ",plot" + QString::number ( i );
    }// end for
    plot += ")\\n";


    // Vaciamos el cursor de la base de datos.
    delete cursorapt;
    query.sprintf ( "DROP TABLE balance" );
    dbConnection->runQuery ( query );
    dbConnection->commit();

    /* values to chart */
    fprintf ( stderr, "Llamamos a sacapie\n" );
    system ( "cp /usr/lib/bulmages/plugins/plotbars.py /tmp" );
    QString cadena = " sed -e \"s&###DATA###&" + valores + "&g\"  /tmp/plotbars.py > /tmp/plotbars1.py";
    system ( cadena.ascii() );
    cadena = " sed -e \"s&###PLOT###&" + plot + "&g\"  /tmp/plotbars1.py > /tmp/plotbars2.py";
    fprintf ( stderr, "%s\n", cadena.ascii() );
    system ( cadena.ascii() );
    system ( "python /tmp/plotbars2.py --format=png > /tmp/pie.png" );



    fprintf ( stderr, "Hemos terminado sacapie \n" );
    QPixmap *imag = new QPixmap ( "/tmp/pie.png" );
    fprintf ( stderr, "Y ahora hemos creado la imagen\n" );
    m_imagen->setPixmap ( *imag );
    fprintf ( stderr, "Y ahora la hemos mostrado\n" );
    //Destruimos la memoria utilizada
    delete imag;

}// end presentarbarras



myplugin:: myplugin()
{}
myplugin::~myplugin()
{}


void myplugin::BalanceGrafico()
{
    // Aprovechamos para empezar a trabajar con estadisticas a ver que tal van.
    estadisticasview * est = new estadisticasview ( 0, 0 );
    est->inicializa ( dbConnection, 0 );
    est->exec();
    delete est;
}// end BalanceGrafico

void myplugin::BalanceBarras()
{
    // Aprovechamos para empezar a trabajar con estadisticas a ver que tal van.
    estadisticasview * est = new estadisticasview ( 0, 0 );
    est->inicializa ( dbConnection, 1 );
    est->exec();
    delete est;
}


