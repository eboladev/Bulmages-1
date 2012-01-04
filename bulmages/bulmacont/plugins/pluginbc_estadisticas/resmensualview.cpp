/***************************************************************************
                          resmensualview.cpp  -  description
                             -------------------
    begin                : dom jul 20 2003
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

#include "resmensualview.h"
// Si se va a compilar con el modulo de estadisticas se ponen las estadisticas
#ifdef ESTADISTICAS
#include "estadisticas/tolinechart.h"
#include "estadisticas/topiechart.h"
#endif

#ifdef GDCHART
//#include "gdc.h"
//#include "gdchart.h"
#include "estadisticasview.h"
#include <qlabel.h>
#endif

#ifdef DISLIN
#include <stdio.h>
#include "dislin.h"
#endif

#include "qdatetime.h"
#include "qstring.h"
#include "qlineedit.h"
#include "qobject.h"
#include "bcplancontablelistview.h"
#include "bcmasapatrimoniallistview.h"
#include "bccompany.h"


resmensualview::resmensualview ( empresa *emp, QWidget *parent, const char *name ) : resmensualdlg ( parent, name )
{

    company = emp;
    dbConnection = emp->bdempresa();
// Inicializamos los valores de las masas patrimoniales.
    idmpatrimonial1 = "";
    idmpatrimonial2 = "";
    idmpatrimonial3 = "";

}
resmensualview::~resmensualview()
{}

int resmensualview::inicializa()
{
//    presentar();
//    pie->show();
    return ( 0 );
}// end inicializa

void resmensualview::accept()
{
    presentar();
    presentarpie();
}// end accept

void resmensualview::presentar()
{
    std::list<double> milistad[12];
//   std::list<double> milistah[12];
    std::list<double> milistas[12];
    char *mes[] = {"ene", "feb", "mar", "abr", "may", "jun", "jul", "ago", "sep", "oct", "nov", "dec"};


    QString codigo[3];
    codigo[0] = cod1->text();
    codigo[1] = cod2->text();
    codigo[2] = cod3->text();
    QString codigomasa[3];
    codigomasa[0] = idmpatrimonial1.ascii();
    codigomasa[1] = idmpatrimonial2.ascii();
    codigomasa[2] = idmpatrimonial3.ascii();

    string ano = "2004";

    for ( int j = 0; j < 3; j ++ ) {
        QDate fecha ( 2004, 1, 1 );
        QDate fecha1 ( 2004, 1, 31 );
        float saldoant = 0;
        float movant = 0;
        for ( int i = 0; i < 12;i++ ) {
            // Buscamos los datos de enero.
            QString finicial = fecha.toString ( "dd/MM/yyyy" );
            QString ffinal = fecha1.toString ( "dd/MM/yyyy" );
            QString query;
            string cod = codigo[j].ascii();
            string cod1 = codigomasa[j].ascii();
            query = "";
            if ( cod != "" ) {
                query.sprintf ( "SELECT debetotal(id_cuenta('%s'),'%s','%s') as tdebe, habertotal(id_cuenta('%s'),'%s','%s') as thaber", codigo[j].ascii(), ( char * ) finicial.ascii(), ( char * ) ffinal.ascii(), codigo[j].ascii(), ( char * ) finicial.ascii(), ( char * ) ffinal.ascii() );
//         } else if (cod1 != "" ) {
            } else if ( cod1 != "" ) {
                query.sprintf ( "SELECT debempatrimonial(%s,'%s','%s') as tdebe, habermpatrimonial(%s,'%s','%s') as thaber", codigomasa[j].ascii(), ( char * ) finicial.ascii(), ( char * ) ffinal.ascii(), codigomasa[j].ascii(), ( char * ) finicial.ascii(), ( char * ) ffinal.ascii() );
            }// end if
            if ( query != "" ) {
                fprintf ( stderr, "%s\n", query.ascii() );
                dbConnection->begin();
                BlDbRecordSet *curs = dbConnection->loadQuery ( query, "midursor" );
                dbConnection->commit();
                if ( !curs->eof() ) {
                    milistad[i].push_back ( movant + atof ( curs->value( "tdebe" ).ascii() ) + atof ( curs->value( "thaber" ).ascii() ) );
                    milistas[i].push_back ( saldoant + atof ( curs->value( "tdebe" ).ascii() ) - atof ( curs->value( "thaber" ).ascii() ) );
                    saldoant = saldoant + atof ( curs->value( "tdebe" ).ascii() ) - atof ( curs->value( "thaber" ).ascii() );
                    movant = movant +  atof ( curs->value( "tdebe" ).ascii() ) + atof ( curs->value( "thaber" ).ascii() );
                    fprintf ( stderr, "metido en el grafico%s\n", curs->value( "tdebe" ).ascii() );
                } else {
                    milistad[i].push_back ( movant );
                    //            milistad[i].push_back(0);
                    milistas[i].push_back ( saldoant );
                }// end if
                delete curs;
                fecha = fecha.addMonths ( 1 );
                fecha1 = fecha1.addMonths ( 1 );
            }// end if
        }// end for
    }// end for
#ifdef ESTADISTICAS
    line->clear();
    line->showGrid ( 12 );
    line->setTitle ( _ ( "Progresion de saldos" ) );
    line->setSamples ( 12 );
    line1->clear();
    line1->showGrid ( 12 );
    line1->setTitle ( _ ( "Movimientos menusales" ) );
    line1->setSamples ( 12 );
    for ( int i = 0; i < 12;i++ ) {
        line->addValues ( milistas[i], mes[i] );
        line1->addValues ( milistad[i], mes[i] );
    }// end for
#endif
}// end presentar

void resmensualview::buscacodigocta()
{
    BcPlanContableListView * listcuentas = new BcPlanContableListView ( company );
    listcuentas->setModoLista();
    listcuentas->inicializa();
    listcuentas->exec();
    cod1->setText ( listcuentas->codcuenta() );
    delete listcuentas;
}// end buscacodigocta


void resmensualview::buscacodigocta1()
{
    BcPlanContableListView * listcuentas = new BcPlanContableListView ( company );
    listcuentas->setModoLista();
    listcuentas->inicializa();
    listcuentas->exec();
    cod2->setText ( listcuentas->codcuenta() );
    delete listcuentas;
}// end buscacodigocta

void resmensualview::buscacodigocta2()
{
    BcPlanContableListView * listcuentas = new BcPlanContableListView ( company );
    listcuentas->setModoLista();
    listcuentas->inicializa();
    listcuentas->exec();
    cod3->setText ( listcuentas->codcuenta() );
    delete listcuentas;
}// end buscacodigocta

void resmensualview::buscampatrimonial1()
{
    BcMasaPatrimonialListView * nuevae = new BcMasaPatrimonialListView ( 0, "mpatrimoniales", true );
    nuevae->inicializa ( dbConnection );
    nuevae->setModoSelector();
    nuevae->exec();
    mpatrimonial1->setText ( ( char * ) ( nuevae->getNomMasa() ).ascii() );
    idmpatrimonial1 = nuevae->getIdMasa();
    delete nuevae;

}// end buscampatrimonial1

void resmensualview::buscampatrimonial2()
{
    BcMasaPatrimonialListView * nuevae = new BcMasaPatrimonialListView ( 0, "mpatrimoniales", true );
    nuevae->inicializa ( dbConnection );
    nuevae->setModoSelector();
    nuevae->exec();
    mpatrimonial2->setText ( ( char * ) ( nuevae->getNomMasa() ).ascii() );
    idmpatrimonial2 = nuevae->getIdMasa();
    delete nuevae;

}// end buscampatrimonial1

void resmensualview::buscampatrimonial3()
{
    BcMasaPatrimonialListView * nuevae = new BcMasaPatrimonialListView ( 0, "mpatrimoniales", true );
    nuevae->inicializa ( dbConnection );
    nuevae->setModoSelector();
    nuevae->exec();
    mpatrimonial3->setText ( ( char * ) ( nuevae->getNomMasa() ).ascii() );
    idmpatrimonial3 = nuevae->getIdMasa();
    delete nuevae;

}// end buscampatrimonial1

void resmensualview::presentarpie()
{
#ifdef GDCHART
    char * label[1000];
    float p[1000];
    int j = 0;
#endif
#ifdef DISLIN
    char *label[1000];
    float p[1000];
    int j = 0;
#endif
    char query[2900];
    BlDbRecordSet *cursorapt;

    QString finicial = "01/01/2003";
    QString ffinal = "31/12/2003";
    QString cinicial = "10";
    QString cfinal = "99";
    QString codigo[3];
    codigo[0] = cod1->text();
    codigo[1] = cod2->text();
    codigo[2] = cod3->text();

#ifdef ESTADISTICAS
    toPieChart *pies[3];
    pies[0] = pie;
    pies[1] = pie1;
    pies[2] = pie2;
#endif

    for ( int i = 0; i < 3; i++ ) {
        dbConnection->begin();
        sprintf ( query, "SELECT sum(debe) as tdebe, sum(haber) as thaber, contrapartida FROM apunte WHERE apunte.idcuenta=id_cuenta('%s') GROUP BY contrapartida", codigo[i].ascii() );
        cursorapt = dbConnection->loadQuery ( query, "mycursor" );
        dbConnection->commit();
        // Calculamos cuantos registros van a crearse y dimensionamos la tabla.
        std::list<double> valores;
        std::list<QString> labels;

        while ( !cursorapt->eof() ) {
            QString nomcuenta;
            // Acumulamos los totales para al final poder escribirlos
            sprintf ( query, "SELECT * FROM cuenta WHERE idcuenta = %s", cursorapt->value( "contrapartida" ).ascii() );
            dbConnection->begin();
            BlDbRecordSet *micurs = dbConnection->loadQuery ( query, "mioldcursor" );
            if ( !micurs->eof() ) {
                nomcuenta = micurs->value( "codigo" ) + " " + micurs->value( "descripcion" );
            }// end if
            delete micurs;

            float valor =  atof ( cursorapt->value( "tdebe" ).ascii() ) + atof ( cursorapt->value( "thaber" ).ascii() );
            if ( valor > 0 ) {
                //                  pie->addValue(valor,nomcuenta.substr(0,25).c_str());
                valores.push_back ( valor );
                labels.push_back ( nomcuenta.mid ( 0, 25 ).ascii() );
#ifdef GDCHART
                label[j] = new char[30];
                strcpy ( label[j], cursorapt->value( "descripcion" ).mid ( 0, 15 ).ascii() );
                p[j] = valor;
                fprintf ( stderr, "%s %d", label[j], p[j] );
#endif

            } else {
                //                  pie->addValue(-valor,nomcuenta.substr(0,25).c_str());
                valores.push_back ( -valor );
                labels.push_back ( nomcuenta.mid ( 0, 25 ).ascii() );
#ifdef GDCHART
                label[j] = new char[30];
                strcpy ( label[j], cursorapt->value( "descripcion" ).mid ( 0, 15 ).ascii() );
                p[j] = -valor;
                fprintf ( stderr, "%s %d", label[j], p[j] );
#endif

            }// end if

            // Calculamos la siguiente cuenta registro y finalizamos el bucle
            cursorapt->nextRecord();
#ifdef GDCHART
            j++;
#endif
        }// end while

        // Vaciamos el cursor de la base de datos.
        delete cursorapt;
#ifdef ESTADISTICAS
        pies[i]->setValues ( valores, labels );
#endif
#ifdef GDCHART
        generargrafico ( p, label );
        fprintf ( stderr, "Llamamos a sacapie\n" );
        fprintf ( stderr, "Hemos terminado sacapie \n" );
        QPixmap *imag = new QPixmap ( "/tmp/graf.gif" );
        fprintf ( stderr, "Y ahora hemos creado la imagen\n" );
        imgsaldos->setPixmap ( *imag );
        fprintf ( stderr, "Y ahora la hemos mostrado\n" );
        //Destruimos la memoria utilizada
        for ( int i = 0;i < j;i++ ) delete label[i];
        delete imag;
#endif

#ifdef DISLIN
        generargrafico ( p, label );
#endif
    }// end for


}// end presentarpie




#ifdef ESTADISTICAS
// Si se compila con las librerias de estadisticas se supone que no se va a llamar a esta funcion,
// por tanto no hace nada.
void resmensualview::generargrafico ( float *, char** )
{}// end resmensualview
#endif

#ifdef GDCHART
// Esta funcion es si se compila con GDCHART
void resmensualview::generargrafico ( float *h1, char**t2 )
{
    fprintf ( stderr, "Principio de generargrafico\n" );
    /* set some sample data points */
    float h[12]  = { 17.8,  17.1,  17.3,  0,  17.2,  17.1,
                     17.3,  17.3,  17.3,  17.1,         17.5,  17.4
                   };

    float c[12]  =  { 17.0,  16.8,  16.9,  0,  16.9,  16.8,
                      17.2,  16.8,  17.0,  16.9,         16.4,  16.1
                    };

    float l[12]  = {  16.8,  16.8,  16.7,  0,  16.5,  16.0,
                      16.1,  16.8,  16.5,  16.9,         16.2,  16.0
                   };

    float v[12]  = {  150.0, 100.0, 340.0,  0, 999.0, 390.0,
                      420.0, 150.0, 100.0,  340.0,       1590.0, 700.0
                   };
    char *t[12] = { "May", "Jun", "Jul",  "Aug",       "Sep",  "Oct",
                    "Nov", "Dec", "Jan", "Feb",        "Mar",  "Apr"
                  };

// /* set color RGB as ulong array */
// unsigned long setcolor[3]    = { 0xC0C0FF, 0xFF4040, 0xFFFFFF };

    GDC_ANNOTATION_T anno;

    /* need an open FILE pointer  - can be stdout */
    FILE    *outgif1 = fopen ( "/tmp/graf.gif", "wb" ); /* rem: test open() fail */


    anno.color = 0x00FF00;
    strncpy ( anno.note, "Did Not\nTrade", MAX_NOTE_LEN ); /* don't exceed MAX_NOTE_LEN */
    anno.point = 3;           /* first is 0 */
// GDC_annotation_font = GDC_TINY;
    GDC_annotation = &anno;         /* set annote option */

    GDC_HLC_style = ( GDC_HLC_STYLE_T ) ( GDC_HLC_I_CAP | GDC_HLC_CLOSE_CONNECTED );
    GDC_HLC_cap_width = 45;

    GDC_bar_width     = 75;         /* % */

// GDC_BGImage = "W.gif";

    GDC_title = "Widget Corp.";
    GDC_ytitle = "Price ($)";
    GDC_ytitle2 = "Volume (K)";
    GDC_ytitle_size = GDC_SMALL;
    GDC_VolColor = 0x4040FFL;        /* aka combo */
    GDC_3d_depth  = 4.0;         /* % entire gif */

// GDC_SetColor  = setcolor;        /* see README */
    GDC_PlotColor = 0xFFFFFF;
// GDC_grid = FALSE;

// GDC_xtitle="fy.1998";

// fprintf( stdout, "Content-Type: image/gif\n\n" );  /* rem: for web use */
    /* finally: make the call */
    fprintf ( stderr, "Voy a ejecutar out_graph\n" );

    out_graph ( 200, 175,         // overall width, height
                outgif1,          // open FILE pointer
                GDC_COMBO_HLC_AREA,       // chart type
                12,           // number of points
                t,           // X axis label array
                1,           // number of sets (see README)
                h,           // set 1 (high)
                l,           // low
                c,           // close
                v );           // combo/volume

    fprintf ( stderr, "He terminado de ejecutar out_graph\n" );

    fclose ( outgif1 );
    fprintf ( stderr, "Fin de generargrafico|n" );

}// end generargrafico
#endif


#ifdef DISLIN
// Si se compila con las librerias de DISLIN se ejecutara esta version.
void resmensualview::generargrafico ( float *h1, char**t2 )
{
    char cbuf[80];
    float xray[5]  = {2., 4., 6., 8., 10.},
                     y1ray[5] = {0., 0., 0., 0., 0.},
                                y2ray[5] = {3.2, 1.5, 2.0, 1.0, 3.0};

    int ic1ray[5]  = {50, 150, 100, 200, 175},
                     ic2ray[5]  = {50, 150, 100, 200, 175};


    fprintf ( stderr, "resmensualview::generargrafico DISLIN version. \n" );

    metafl ( "XWIN" );
    setpag ( "da4p" );
    disini();
    pagera();
    hwfont();

//  titlin("3-D Bar Graph / 3-D Pie Chart", 2);
    titlin ( "Resumen", 2 );
    htitle ( 40 );

    shdpat ( 16 );
    axslen ( 1500, 1000 );
    axspos ( 300, 1400 );

    barwth ( 0.5 );
    bartyp ( "3dvert" );
    labels ( "second", "bars" );
    labpos ( "outside", "bars" );
    labclr ( 255, "bars" );
    graf ( 0., 12., 0., 2., 0., 5., 0., 1. );
    title();
    color ( "red" );
//  bars(xray,y1ray,y2ray,5);
    bars ( h1, h1, h1, 5 );
    endgrf();
    /*
      shdpat(16);
      labels("data","pie");
      labclr(255,"pie");
      chnpie("none");
      pieclr(ic1ray,ic2ray,5);
      pietyp("3d");
      axspos(300,2700);
      piegrf(cbuf,0,y2ray,5);
    */
    disfin();
}// end resmensualview
#endif



