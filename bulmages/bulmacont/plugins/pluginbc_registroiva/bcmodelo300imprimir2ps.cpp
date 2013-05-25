/***************************************************************************
 *   Copyright (C) 2003 by Josep Burcion                                   *
 *   josep@burcion.com                                                     *
 *   David Gutierrez Rubio                                                 *
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

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <unistd.h>

#include <QtCore/QString>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMessageBox>

#include "bcmodelo300imprimir2ps.h"
#include "blconfiguration.h"
#include "blpostgresqlclient.h"
#include "blfunctions.h"


/// El constructor llama al cuadro de di&aacute;logo para seleccionar par&aacute;metros
/// adecuados para rellenar el modelo.
/** Realiza una consulta para obtener las cuentas bancarias. */
/**
\param parent
**/
BcModelo300Imprimir2PS::BcModelo300Imprimir2PS ( QWidget *parent ) : QDialog ( parent )
{
    BL_FUNC_DEBUG
    setupUi ( this );
    QString query = "SELECT descripcion, bancoent_cuenta, codigo FROM cuenta WHERE codigo LIKE '572%%' AND codigo > 572";

    BlPostgreSqlClient *metabase = new BlPostgreSqlClient();

    metabase->inicializa ( "bulmages" ); ///[TODO] CAMBIAR!!!!

    metabase->begin();
    fprintf ( stderr, "%s\n", query.toLatin1().constData() );
    BlDbRecordSet *cur = metabase->loadQuery ( query, "bancos" );

    int nTuples = cur->numregistros();
    nombresccc = new QString[nTuples];

    numerccc = new QString[nTuples];

    for ( int i = 0; i < nTuples; i++ ) {
        nombresccc[i] = cur->value( "descripcion" );
        nombresccc[i] += "  ";
        nombresccc[i] += cur->value( "codigo" );

        numerccc[i] = cur->value( "bancoent_cuenta" );
//        cout << nombresccc[i].toLatin1().constData() << "\t" << numerccc[i].toLatin1().constData() << "\n";
        combocuentas->addItem ( nombresccc[i] );
        cur->nextRecord();
    } // end for
    delete cur;
    delete metabase;

    if ( nTuples == 0 ) { /// Si no tenemos ninguna cuenta de banco...
        sincuentasbancarias = true;
        personalButton->setChecked ( true );
        personalButtonPressed();
        cuentaButton->setDisabled ( true );
    } // end if
    
}


///
/**
**/
BcModelo300Imprimir2PS::~BcModelo300Imprimir2PS()
{
    BL_FUNC_DEBUG
    
}


/// When pressed, it calls to the \ref generaps method.
/**
**/
void BcModelo300Imprimir2PS::accept()
{
    BL_FUNC_DEBUG
    m_es_borrador = borradorcheckbox->isChecked();

    if ( cuentaButton->isChecked() ) {
        ccc = new BlSpainBankAccountNumber ( numerccc[combocuentas->currentIndex() ] );
    } else
        ccc = new BlSpainBankAccountNumber ( banco->text(), entidad->text(), dc->text(), cuenta->text() );

//    cout << "Elegida cuenta numero " << ccc->getcodigo("-").toLatin1().constData() << "\n";
//    cout << "dc=" << ccc->getdc().toLatin1().constData() << "\n";

    if ( !ccc->cuentaesvalida() ) {
        switch ( QMessageBox::warning ( this,
                                        _ ( "Formulario 300" ),
                                        _ ( "Aviso: El numero de cuenta bancario introducido\n"
                                            "no se corresponde con un CCC correcto." ),
                                        _ ( "Generar de todas formas" ), _ ( "Volver" ), 0,
                                        0, 1 ) ) {
        case 0:
            generaps();
            break;
        case 1:
            break;
        }
    } else {
        generaps();
    } // end if
    
}

/// Generate the postscript of the 300-model with the given parameters.
/** The hardest part is converting the official pdf to postscript. */
/**
**/
void BcModelo300Imprimir2PS::generaps()
{
    BL_FUNC_DEBUG
    QString pdfname, tempname, psname, command;
    QString cadaux;
    QTextStream inputstr;

//    cout << "Elegido trimestre" << trimestre->currentIndex() << "\n";

    tempname = QString ( getenv ( "HOME" ) ) + "/.bulmages/mod300temp.ps";
    pdfname = QString ( getenv ( "HOME" ) ) + "/.bulmages/formularios/mod300e.pdf";

    if ( !QFile::exists ( pdfname ) )
        pdfname = g_confpr->value( CONF_PROGDATA ) + "formularios/mod300e.pdf";

    bool doit = true;
    if ( !QFile::exists ( pdfname ) ) {
        QMessageBox::warning ( this,
                               _ ( "Formulario 300" ),
                               _ ( "Lo siento, no encuentro el formulario original en pdf.\n"
                                   "Pruebe a descargarlo desde www.aeat.es y guaedelo en\n"
                                   "/usr/share/bulmages/formularios/ o en\n"
                                   "~/.bulmages/formularios/." ),
                               _ ( "&Aceptar" ), 0, 0, 0, 1 );
        doit = false;
    } // end if

    if ( doit ) {
//        cout << "Convirtiendo a postscript...\n";
        if ( m_es_borrador ) {
            command = "pdftops " + pdfname + " " + tempname;
            system ( command.toLatin1().constData() );
        } else {
            BcPsProgressDialog progress ( _ ( "Creando formulario" ), _ ( "&Cancelar" ), 0, 50, this, 0 );
            this->convierte_a_postscript = new BcPsThread ( pdfname, tempname, &progress );
            progress.setValue ( 0 );

            /// LLama a la rutina para convertir el pdf en ps y conservar el dichoso
            /// numerito de serie.
            /// Lo hace mediante un thread, para no bloquear la interfaz de usuario.
            this->convierte_a_postscript->start();

            progress.setModal ( true );
            progress.exec();
            if ( progress.wasCanceled() ) {
                system ( "kill $(ps aux|grep 'Xvfb :5.0'|grep -v grep|awk '{print $2}')" );
                doit = false;
            }
        }
    } // end if

    /// Ahora tengo que procesar tempname y generar psname.
    if ( doit ) {
        psname = QString ( getenv ( "HOME" ) ) + "/.bulmages/mod300.ps";
//        cout << psname.toLatin1().constData();
        m_fich.setFileName ( psname );
        if ( m_fich.open ( QIODevice::WriteOnly ) ) {
            m_output.setDevice ( &m_fich );
            m_fichlec.setFileName ( tempname );
            if ( !m_fichlec.open ( QIODevice::ReadOnly | QIODevice::Text ) ) {
		BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Error al abrir el fichero.") );
                exit ( 1 );
            } // end if
            inputstr.setDevice( &m_fichlec );
            cadaux = inputstr.readLine();
            while ( cadaux.left ( 7 ) != "%%Page:" ) {
                m_output << cadaux ;
                cadaux = inputstr.readLine();
            }
            m_output << cadaux;
            /// Inserta definiciones de postscript al principio de la 1a p&aacute;gina.
            escribe_postscriptdefs();

            cadaux = inputstr.readLine();
            while ( cadaux != "showpage\n" ) { ///h Hsta que encuentre un showpage.
                m_output << cadaux;
                cadaux = inputstr.readLine();
            } // end while
            if ( m_es_borrador ) {
                marcadeagua_borrador();
            } // end if
            rellena_identificacion();
            rellena_liquidacion();
            rellena_compensacion();
            m_output << cadaux;

            cadaux = inputstr.readLine();
            while ( cadaux != "showpage\n" ) { /// Hasta que encuentre un showpage.
                m_output << cadaux;
                cadaux = inputstr.readLine();
            } // end while
            if ( m_es_borrador ) {
                marcadeagua_borrador();
            } // end if
            rellena_identificacion();
            rellena_liquidacion();
            rellena_compensacion();
            m_output << cadaux;

            cadaux = inputstr.readLine();
            while ( cadaux != "showpage\n" ) { /// Hasta que encuentre un showpage.
                m_output << cadaux;
                cadaux = inputstr.readLine();
            } // end while
            if ( m_es_borrador ) {
                marcadeagua_borrador();
            } // end if
            rellena_identificacion();
            rellena_compensacion();
            m_output << cadaux;

            while ( !inputstr.atEnd() ) { ///Leo el resto del fichero.
                cadaux = inputstr.readLine();
                m_output << cadaux << "\n";
            } // end while

            m_fichlec.close();
            m_fich.close();

//            cout << "Se supone que tengo que leer los formularios desde " << g_confpr->value(CONF_PROGDATA).toLatin1().constData() << "\n";
//            cout << "[TODO] OJO!! Los formularios que genera no son validos, ya que han de tener un numero de serie UNICO\n";
//            cout << "[TODO]  Es decir, hay que bajarse de internet uno nuevo CADA VEZ que se haga un modelo nuevo\n";

            command = "rm " + tempname + "; kghostview " + psname;
            system ( command.toLatin1().constData() );
        } else {
	    BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Error, no se ha abierto el archivo.") );
        } // end if
    } // end if
    
}


///
/**
**/
void BcModelo300Imprimir2PS::personalButtonPressed()
{
    BL_FUNC_DEBUG
    bool dis = cuentaButton->isChecked();
    banco->setDisabled ( dis );
    entidad->setDisabled ( dis );
    dc->setDisabled ( dis );
    cuenta->setDisabled ( dis );
    combocuentas->setDisabled ( !dis );
    
}

/// Write definitions in the postscript output file, necessary to insert text later.
/** It set the Courier-Bold 12pt font, and defines functions in postscript to write
    left-aligned text and right-aligned text.
    Escribe un clipping path para evitar que aparezcan las casillas de "rellenar formulario"
    que aparecen si no se usa Acrobat para convertir el pdf a ps. */
/**
**/
void BcModelo300Imprimir2PS::escribe_postscriptdefs()
{
    BL_FUNC_DEBUG

    m_output << "%bulmages\n"
    "%Texto introducido manualmente\n"
    "/Courier-Bold\n"
    "findfont\n"
    "12 scalefont\n"
    "setfont\n"
    "%(texto) numcar escrizq\n"
    "% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"
    "/escrizq{\n"
    "% (texto) x_barrita y_barrita numcar escrizq\n"
    " -7  mul -5 add exch 3 add 3 -2 roll add  exch moveto\n"
    "  show\n"
    "  } def\n"
    "  %\n"
    "  /escrder{\n"
    "  % (texto) x_barrita y_barrita numcar escrizq\n"
    "  %newpath\n"
    "  exch 3 add 3 -2 roll add 5 add exch moveto\n"
    "  show} def\n";
    
}


/// Write the CCC-number in the 300-model.
/**
\param x
\param y
**/
void BcModelo300Imprimir2PS::escribe_cuenta_bancaria ( int x, int y )
{
    BL_FUNC_DEBUG

    const int steps[] = {
        0, 11, 11, 11, 12, 11, 11, 11, 12, 11, 12, 11, 11, 11, 12, 11,
        11, 11, 12, 11, 11
    };
    //76,32 para devolver
    //338,73 para pagar
    int acum = x; /// Coordenada x de la primera casilla a rellenar.
    QString tem = ccc->getcodigo();
    for ( int i = 0; i < 20; i++ ) {
        acum += steps[i];
        marca_casilla ( QString ( tem[i] ), acum, y );
    } // end for
    
}


/// Solucionar problema cuando el nombre de la Empresa (u otro campo) lleva acentos.
/** \todo El tel&eacute;ono no cabe (pero no es culpa m&aacute;!!) */
/**
**/
void BcModelo300Imprimir2PS::rellena_identificacion()
{
    BL_FUNC_DEBUG
    QString cad1;

    BlPostgreSqlClient *m = new BlPostgreSqlClient();

    m->inicializa ( "bulmages" ); ///[TODO] CAMBIAR!!!!
    escrder ( m->propiedadempresa ( "CIF" ), 78, 601 );
    escrder ( m->propiedadempresa ( "NombreEmpresa" ), 213, 601 );
    escrder ( m->propiedadempresa ( "TipoVia" ), 78, 576 );
    escrder ( m->propiedadempresa ( "NombreVia" ), 141, 576 );
    escrder ( m->propiedadempresa ( "NumeroVia" ), 389, 576 );
    escrder ( m->propiedadempresa ( "Escalera" ), 431, 576 );
    escrder ( m->propiedadempresa ( "Piso" ), 461, 576 );
    escrder ( m->propiedadempresa ( "Puerta" ), 490, 576 );
    ///\bug Por ahora, el n&uacute;mero de tel&eacute;fono no cabe!!
    escrder ( m->propiedadempresa ( "Municipio" ), 78, 550 );
    escrder ( m->propiedadempresa ( "Provincia" ), 360, 550 );
    escribe_codigo_postal ( m->propiedadempresa ( "CodPostal" ) );

    delete m;

    cad1.sprintf ( "%d", trimestre->currentIndex() + 1 ); /// Si elegido item 0 ---> cad1="1", etc.

    marca_casilla ( cad1, 452, 690 );
    marca_casilla ( "T", 467, 690 );

    escrder ( empresa, 209, 601 );
    for ( int i = 0; i < 4; i++ ) {
        marca_casilla ( QString ( ano[i] ), 453 + i * 14, 706 );
    } // end for
    
}


/// Write the data in the second part of the 300-model.
/**
**/
void BcModelo300Imprimir2PS::rellena_liquidacion()
{
    BL_FUNC_DEBUG
    escrizqder ( baser16, 328, 516 ); /// Casilla 01.
    escrizqder ( baser7, 328, 502 ); /// Casilla 04.
    escrizqder ( baser4, 328, 487 ); /// Casilla 07.
    escrizq ( 16, 435, 516 ); /// Casilla 02.
    escrizq ( 7, 435, 502 ); /// Casilla 05.
    escrizq ( 4, 435, 487 ); /// Casilla 08.
    escrizqder ( ivar16, 531, 516 ); /// Casilla 03.
    escrizqder ( ivar7, 531, 502 ); /// Casilla 06.
    escrizqder ( ivar4, 531, 487 ); /// Casilla 09.
    escrizqder ( ivar16 + ivar7 + ivar4, 531, 400 ); /// Casilla 21.
    escrizqder ( ivas4 + ivas7 + ivas16, 532, 366 ); /// Casilla 22.
    escrizqder ( ivas4 + ivas7 + ivas16, 532, 295 ); /// Casilla 27.
    cas34 = ivas4 + ivas7 + ivas16 - ( ivar16 + ivar7 + ivar4 );
    escrizqder ( cas34, 532, 196 ); /// Casilla 34.
    
}


///
/**
**/
void BcModelo300Imprimir2PS::rellena_compensacion()
{
    BL_FUNC_DEBUG
    if ( cas34 < 0 ) {
        escrizqder ( -cas34, 248, 145 ); /// Casilla a compensar si la 34 sale negativa.
        if ( trimestre->currentIndex() == 3 ) { /// Si estamos en el cuarto trimestre...
            escribe_cuenta_bancaria ( 76, 32 ); /// Ponemos la cuenta bancaria si hay que devolver.
        } // end if
    } else {
        if ( trimestre->currentIndex() == 3 ) { /// Si estamos en el cuarto trimestre...
            marca_casilla ( "X", 464, 134 ); /// Casilla de adeudo en cuenta.
            escribe_cuenta_bancaria ( 338, 73 );
        } // end if
    } // end if
    
}


/// Escribe el c&oacute;digo postal en las min&uacute;sculas casillas
/// destinadas a tal fin.
/**
\param cod
**/
void BcModelo300Imprimir2PS::escribe_codigo_postal ( QString cod )
{
    BL_FUNC_DEBUG
    int offset = 3;
    marca_casilla ( QString ( cod[0] ), 528 - offset, 550 );
    marca_casilla ( QString ( cod[1] ), 537 - offset, 550 );
    marca_casilla ( QString ( cod[2] ), 546 - offset, 550 );
    marca_casilla ( QString ( cod[3] ), 554 - offset, 550 );
    marca_casilla ( QString ( cod[4] ), 563 - offset, 550 );
    
}

