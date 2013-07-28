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
#include <QtWidgets/QLabel>

#ifdef GDCHART
#include "gdc.h"
#include "gdcpie.h"
#include <gdchart.h>
#endif


int estadisticasview::inicializa ( BlPostgreSqlClient *conexion )
{
    dbConnection = conexion;
    presentar();
#ifdef GDCHART
    delete m_pie;
#endif

#ifdef ESTADISTICAS
    delete m_imagen;
#endif


    return ( 0 );
}// end inicializa

estadisticasview::estadisticasview ( QWidget *parent, const char *name ) : estadisticasdlg ( parent, name )
{}


estadisticasview::~estadisticasview()
{}


void estadisticasview::presentar()
{
    int j, num1;
    QString query;
    BlDbRecordSet *cursorapt;
    QString finicial = "01/01/2003";
    QString ffinal = "31/12/2003";
    QString cinicial = "10";
    QString cfinal = "99";

    // Hacemos la consulta de los apuntes a listar en la base de datos.
//     int idc_coste = ccostes[combocoste->currentItem()];

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


    query.sprintf ( "SELECT * FROM balance WHERE debe <> 0  OR haber <> 0 ORDER BY codigo" );
    cursorapt = dbConnection->loadQuery ( query, "mycursor" );


    // Calculamos cuantos registros van a crearse y dimensionamos la tabla.
    num1 = cursorapt->numregistros();
    j = 0;

#ifdef GDCHART
    char *label[1000];
    float p[1000];
#endif
    while ( !cursorapt->eof() ) {
        // Acumulamos los totales para al final poder escribirlos
        float valor =  atof ( cursorapt->value( "tsaldo" ).ascii() );
        if ( valor > 0 ) {
#ifdef ESTADISTICAS
            m_pie->addValue ( valor, cursorapt->value( "descripcion" ).mid ( 0, 15 ).ascii() );
#endif
#ifdef GDCHART
            label[j] = new char[30];
            strcpy ( label[j], cursorapt->value( "descripcion" ).mid ( 0, 15 ).ascii() );
            p[j] = valor;
            fprintf ( stderr, "%s %d", label[j], p[j] );
#endif

        } else {
#ifdef ESTADISTICAS
            m_pie->addValue ( -valor, cursorapt->value( "descripcion" ).mid ( 0, 15 ).ascii() );
#endif
#ifdef GDCHART
            label[j] = new char[30];
            strcpy ( label[j], cursorapt->value( "descripcion" ).mid ( 0, 15 ).ascii() );
            p[j] = -valor;
            fprintf ( stderr, "%s %d", label[j], p[j] );
#endif
        } // end if

        // Calculamos la siguiente cuenta registro y finalizamos el bucle
        cursorapt->nextRecord();
        j++;
    }// end while

    // Vaciamos el cursor de la base de datos.
    delete cursorapt;
    query.sprintf ( "DROP TABLE balance" );
    dbConnection->runQuery ( query );
    dbConnection->commit();

    /* values to chart */

#ifdef GDCHART
    fprintf ( stderr, "Llamamos a sacapie\n" );
//      sacapie(p, label, j);
    sacapie ( p, label, ( j - 1 > 10 ) ? 10 : j - 1 );
//      sacapie(p, label, 4);
    fprintf ( stderr, "Hemos terminado sacapie \n" );
    QPixmap *imag = new QPixmap ( "/tmp/pie.gif" );
    fprintf ( stderr, "Y ahora hemos creado la imagen\n" );
    m_imagen->setPixmap ( *imag );
    fprintf ( stderr, "Y ahora la hemos mostrado\n" );
    //Destruimos la memoria utilizada
    for ( int i = 0;i < j;i++ ) delete label[i];
    delete imag;
#endif
}// end presentar


#ifdef GDCHART
void estadisticasview::sacapie ( float *p1, char **lbl1, int numslices1 )
{

    fprintf ( stderr, "Hemos terminado de compilar   \n" );
    /* labels */
    FILE  *fp = fopen ( "/tmp/pie.gif", "wb" );
    /* labels */
    char  *lbl[] = { "CPQ\n(DEC)",
                     "HP",
                     "SCO",
                     "IBM",
                     "SGI",
                     "SUN\nSPARC",
                     "other"
                   };
    /* values to chart */
    float   p[] = { 12.5,
                    20.1,
                    2.0,
                    22.0,
                    5.0,
                    18.0,
                    13.0
                  };

    /* set which slices to explode, and by how much */
    int    expl[] = { 0, 0, 0, 0, 0, 20, 0 };

    /* set missing slices */
    unsigned char missing[] = { false, false, false, false, false, false, true };

    /* colors */
    unsigned long clr[] = { 0xFF4040L, 0x80FF80L, 0x8080FFL, 0xFF80FFL, 0xFFFF80L, 0x80FFFFL, 0x0080FFL };

    /* set options  */
    /* a lot of options are set here for illustration */
    /* none need be - see gdcpie.h for defaults */
    /* GDCPIE_title = "Sample\nPIE"; */
    GDCPIE_label_line = true;
    GDCPIE_label_dist = 15;    /* dist. labels to slice edge */
    /* can be negative */
    GDCPIE_LineColor = 0x000000L;
    GDCPIE_label_size = GDC_SMALL;
    GDCPIE_3d_depth  = 25;
    GDCPIE_3d_angle  = 180;    /* 0 - 359 */
    GDCPIE_perspective = 70;    /* 0 - 99 */
    GDCPIE_explode   = expl;   /* default: NULL - no explosion */
    GDCPIE_Color     = clr;
    GDCPIE_BGColor   = 0xFFFFFFL;
    /* GDCPIE_EdgeColor = 0x000000L;     default is GDCPIE_NOCOLOR */
    /* for no edging */
    GDCPIE_missing   = missing;   /* default: NULL - none missing */

    /* add percentage to slice label */
    /* below the slice label */
    GDCPIE_percent_labels = GDCPIE_PCT_BELOW;
    GDC_image_type     = GDC_PNG;
    /* call the lib */
    GDC_out_pie ( 300,   /* width */
                  200,   /* height */
                  fp,   /* open file pointer */
                  GDC_3DPIE,  /* or GDC_2DPIE */
                  7,    /* number of slices */
                  NULL,   /* can be NULL */
                  p );   /* data array */

    fprintf ( stderr, "Hemos terminado la imagen\n" );
    fclose ( fp );
// exit( 0 );
}
#endif


