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

#include <QPixmap>
#include <QLabel>
#include <QDomDocument>

#include "aplinteligentesview.h"
#include "bccompany.h"
#include "asiento1view.h"

#define TIPO_CTA         0
#define TIPO_FECHA       1
#define TIPO_TEXTO       2
#define TIPO_NUMERO      3
#define TIPO_PREDEFINIDO 4

/// Estos defines indican posiciones de las variables de apunte.
/// son de uso interno, asi que s&oacute;lo deben conocerse si se agregan variables de apunte.
#define VAR_APUNT_CIFCUENTA 0

/// Estos defines indican posiciones de las variables predefinidas
/// Son de uso interno, asi que s&oacute;lo deben conocerse si se agrgan variables
/// predefinidas.
#define VAR_PRED_FECHAACTUAL  0
#define VAR_PRED_FECHAASIENTO 1


///
/**
\param emp
\param parent
**/
aplinteligentesview::aplinteligentesview ( BcCompany *emp, QWidget *parent )
        : BcForm ( emp, parent )
{
    _depura ( "aplinteligentesview::aplinteligentesview", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );

    setTitleName ( _( "Asiento" ) );
    /// Establezco cual es la tabla en la que basarse para los permisos
    setDbTableName ( "asiento" );

    /// iniciamos los contadores de variables para que no haya problemas.
    indvariablescta = 0;
    indvariablesfecha = 0;
    indvariablesnumero = 0;
    indvariablestexto = 0;
    /// Este array es est&aacute;tico, pero como no se modifica nunca se ponen los
    /// valores al principio y la parte din&aacute;mica queda igual.
    indvariablespredefinidas = 0;
    /// Cada apunte la tiene o no la tiene, pero no se debe aplicar.
    indvariablesapunte = 1;
    variablesapunte[VAR_APUNT_CIFCUENTA][0] = "$cifcuenta$";
    mainCompany() ->meteWindow ( windowTitle(), this );
    setmodo ( 0 );
    mainCompany() ->intapuntsempresa()->mui_inteligente->setDisabled ( TRUE );
    _depura ( "END aplinteligentesview::aplinteligentesview", 0 );
}


///
/**
**/
aplinteligentesview::~aplinteligentesview()
{
    _depura ( "aplinteligentesview::~aplinteligentesview", 0 );
    borrawidgets();
    mainCompany() ->sacaWindow ( this );
    mainCompany() ->intapuntsempresa()->mui_inteligente->setEnabled ( TRUE );
    _depura ( "END aplinteligentesview::~aplinteligentesview", 0 );
}


///
/**
\param idasiento
**/
void aplinteligentesview::inicializa ( int idasiento )
{
    _depura ( "aplinteligentesview::inicializa", 0 );
    numasiento = idasiento;
    inicializavariables();

    QDir dir ( confpr->valor ( CONF_DIR_AINTELIGENTES ) );
    dir.setFilter ( QDir::Files );
    dir.setNameFilters ( QStringList ( "*.xml" ) );
    dir.setSorting ( QDir::Size | QDir::Reversed );

    QFileInfoList list = dir.entryInfoList();

    if ( list.size() > 0 ) {
        /// Cargamos el combo con los ficheros de asientos inteligentes disponibles
        QStringList listaOrdenada;
        while ( !list.isEmpty() ) {
            QFileInfo fileInfo = list.takeFirst();
            listaOrdenada.append ( fileInfo.fileName().replace ( ".xml", "" ) ); /// cogemos los nombres de los ficheros sin la extension
            listasientos.append ( fileInfo.filePath() ); /// y la ruta completa
        }
        listaOrdenada.sort(); /// se ordena la lista alfabeticamente
        listasientos.sort(); /// y la de la ruta, que se ordenara igual
        while ( !listaOrdenada.isEmpty() )
            mui_comboainteligentes->addItem ( listaOrdenada.takeFirst() ); /// y se carga en el combo

        /// Calculamos el n&uacute;mero de d&iacute;gitos que tiene una cuenta.
        mainCompany() ->begin();
        QString query1 = "SELECT * FROM configuracion WHERE nombre = 'CodCuenta'";
        BlDbRecordSet *cursoraux1 = mainCompany() ->loadQuery ( query1, "codcuenta" );
        numdigitos = cursoraux1->valor ( 2 ).length();
        mainCompany() ->commit();
        delete cursoraux1;

        on_mui_comboainteligentes_activated ( 0 );
    }
    _depura ( "END aplinteligentesview::inicializa", 0 );
}


/// Las variables predefinidas se declaran aqu&iacute;.
/// De momento tenemos dos fariables fechaactual y fechaasiento.
/**
**/
void aplinteligentesview::inicializavariables()
{
    _depura ( "aplinteligentesview::inicializavariables", 0 );
    QString subcadena;
    QString buffer;
    QDate fecha;
    fecha = QDate::currentDate();
    buffer.sprintf ( "%d/%d/%d", fecha.day(), fecha.month(), fecha.year() );
    subcadena = buffer;
    variablespredefinidas[VAR_PRED_FECHAACTUAL][0] = "$fechaactual$";
    variablespredefinidas[VAR_PRED_FECHAACTUAL][1] = subcadena;
    buffer.sprintf ( "SELECT * FROM asiento WHERE idasiento = %d", numasiento );
    mainCompany() ->begin();
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( buffer, "cargaasiento" );
    mainCompany() ->commit();
    if ( !cur->eof() ) {
        variablespredefinidas[VAR_PRED_FECHAASIENTO][0] = "$fechaasiento$";
        variablespredefinidas[VAR_PRED_FECHAASIENTO][1] = cur->valor ( "fecha" );
    } else {
        variablespredefinidas[VAR_PRED_FECHAASIENTO][0] = "$fechaasiento$";
        variablespredefinidas[VAR_PRED_FECHAASIENTO][1] = "";
    } // end if
    delete cur;
    indvariablespredefinidas = 2;
    _depura ( "END aplinteligentesview::inicializavariables", 0 );
}


/// Esta funci&oacute;n carga la variable de uso temporal $cifcuenta$
/// con el valor del NIF de la cuenta introducida.
/// NOTA: N&oacute;tese que el sistema normalmente carga el NIF de la contrapartida
/// del asiento inteligente y que este campo normalmente va en la parte de CIFCUENTA.
/**
\param idcuenta
**/
void aplinteligentesview::cifcuenta ( int idcuenta )
{
    _depura ( "aplinteligentesview::cifcuenta", 0 );
    QString query;
    query.sprintf ( "SELECT * FROM cuenta WHERE idcuenta = %d", idcuenta );
    mainCompany() ->begin();
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( query, "cursor" );
    mainCompany() ->commit();
    if ( !cur->eof() ) {
        variablesapunte[VAR_APUNT_CIFCUENTA][1] = cur->valor ( "cifent_cuenta" );
    } else {
        variablesapunte[VAR_APUNT_CIFCUENTA][1] = "";
    } // end if
    delete cur;
    _depura ( "END aplinteligentesview::cifcuenta", 0 );
}


///
/**
**/
void aplinteligentesview::eturn_cta()
{
    _depura ( "aplinteligentesview::eturn_cta", 0 );
    BusquedaCuenta *numero;
    numero = ( BusquedaCuenta * ) sender();
    selectsiguiente ( numero );
    _depura ( "END aplinteligentesview::eturn_cta", 0 );
}

///
/**
**/
void aplinteligentesview::eturn_fecha()
{
    _depura ( "aplinteligentesview::eturn_fecha", 0 );
    BlDateSearch *numero;
    numero = ( BlDateSearch * ) sender();
    selectsiguiente ( numero );
    _depura ( "END aplinteligentesview::eturn_fecha", 0 );
}

///
/**
**/
void aplinteligentesview::eturn_numero()
{
    _depura ( "aplinteligentesview::eturn_numero", 0 );
    QLineEdit *numero;
    numero = ( QLineEdit * ) sender();
    selectsiguiente ( numero );
    _depura ( "END aplinteligentesview::eturn_numero", 0 );
}


///
/**
**/
void aplinteligentesview::eturn_texto()
{
    _depura ( "aplinteligentesview::eturn_texto", 0 );
    QLineEdit *texto;
    texto = ( QLineEdit * ) sender();
    selectsiguiente ( texto );
    _depura ( "END aplinteligentesview::eturn_texto", 0 );
}


/// Esta funci&oacute;n se llama a trav&eacute;s del array de asientos inteligentes.
/// El valor que se pasa es un valor num&eacute;rico del combobox.
/**
**/
void aplinteligentesview::on_mui_comboainteligentes_activated ( int )
{
    _depura ( "aplinteligentesview::on_mui_comboainteligentes_activated", 0 );
    mostrarplantilla();
    _depura ( "END aplinteligentesview::on_mui_comboainteligentes_activated", 0 );
}


/// Esta funci&oacute;n muestra la plantilla correspondiente con el idainteligente.
/// Tambi&eacute;n pone el combobox en la posici&oacute;n adecuada.
/// Si la plantilla no existe el resultado es indefinido.
/**
\param plantilla
**/
void aplinteligentesview::muestraplantilla ( QString plantilla )
{
    _depura ( "aplinteligentesview::muestraplantilla", 0 );
    int i = mui_comboainteligentes->findText ( plantilla );
    if ( i >= 0 )
        mui_comboainteligentes->setCurrentIndex ( i );
    mostrarplantilla();
    _depura ( "aplinteligentesview::muestraplantilla", 0 );
}


/// La pulsaci&oacute;n sobre el bot&oacute;n de creaci&oacute;n del asiento.
/**
**/
void aplinteligentesview::on_mui_aceptar_clicked()
{
    _depura ( "aplinteligentesview::on_mui_aceptar_clicked", 0 );
	try {
		
		
		/// Se est&aacute; insertando sobre un asiento abierto, con lo que debemos
		/// Cerrar la ventana, ya que es un introducci&oacute;n de asiento normal.
		if ( numasiento != 0 ) {
			recogevalores();
			creaasiento();
			mainCompany() ->intapuntsempresa() ->muestraasiento ( numasiento );
			selectfirst();
		} else {
			/// Se est&aacute; insertando de forma sistem&aacute;tica asientos inteligentes.
			/// Asi que debemos facilitar las cosas al m&aacute;ximo.
			variablespredefinidas[VAR_PRED_FECHAASIENTO][1] = fechaasiento->text().toAscii().constData();
			recogevalores();
			mainCompany() ->intapuntsempresa() ->setFecha ( fechaasiento->text() );
			mainCompany() ->intapuntsempresa() ->vaciar();
			mainCompany() ->intapuntsempresa() ->dialogChanges_cargaInicial();
			mainCompany() ->intapuntsempresa() ->iniciar_asiento_nuevo();
			numasiento = mainCompany() ->intapuntsempresa() ->idasiento().toInt();
			creaasiento();
			mainCompany() ->intapuntsempresa() ->cerrar();
			mainCompany() ->intapuntsempresa() ->dialogChanges_cargaInicial();
			numasiento = 0;
			fechaasiento->selectAll();
			fechaasiento->setFocus();
		} // end if
		/// Si estamos en modo exclusivo cerramos la ventana. Y as&iacute; devolvemos
		/// el control a la aplicaci&oacute;n principal.
		if ( modo == 1 ) {
			close();
		} else {
			/// Reseteamos los valores de numeros y texto para que no haya dobles inserciones.
			for ( int i = 0; i < indvariablesnumero; i++ ) {
			varnumero[i]->setText ( "" );
			} // end for
			for ( int i = 0; i < indvariablestexto; i++ ) {
			vartexto[i]->setText ( "" );
			} // end for
		} // end if

	} catch (...) {
		mensajeInfo ("Fue imposible crear el asiento");
	} /// end try
    _depura ( "END aplinteligentesview::on_mui_aceptar_clicked", 0 );
}


/// Esta funcion va a crear la plantilla necesaria para la creaci&oacute;n
/// de los asientos inteligentes. La idea es coger las variables de
/// la base de datos que son datos variables y presentarlos en el widget
/// Es el algoritmo mas bonito con el que he topado de momento en el
/// desarrollo de Bulmag&eacute;s y me hace mucha ilusi&oacute;n tenerlo listo pronto.
/**
**/
void aplinteligentesview::mostrarplantilla()
{
    _depura ( "aplinteligentesview::mostrarplantilla", 0 );
    QString query;
    int inc = 30;
    QString cadena;
    QString subcadena;
    numainteligente = idainteligente;
    /// Vamos a intentar borrar todos los datos antes de empezar.
    borrawidgets();
    if ( mui_comboainteligentes->currentIndex() != -1 ) {
        QFile f ( listasientos.at ( mui_comboainteligentes->currentIndex() ) );
        if ( !f.open ( QIODevice::ReadOnly ) )
            return;
        if ( !m_doc.setContent ( &f ) ) {
            f.close();
            return;
        }
        f.close();
        /// Recogemos los valores de cuenta.
        QDomNodeList litems = m_doc.elementsByTagName ( "codcuenta" );
        for ( int i = 0; i < litems.count(); i++ ) {
            QDomNode item = litems.item ( i );
            /// Try to convert the node to an element.
            QDomElement e1 = item.toElement();
            /// The node was really an element.
            if ( !e1.isNull() ) {
                recogevariables ( e1.text(), TIPO_CTA );
            } // end if
        } // end for
        /// Recogemos los valores de contrapartida.
        litems = m_doc.elementsByTagName ( "contrapartida" );
        for ( int i = 0; i < litems.count(); i++ ) {
            QDomNode item = litems.item ( i );
            /// Try to convert the node to an element.
            QDomElement e1 = item.toElement();
            /// The node was really an element.
            if ( !e1.isNull() ) {
                recogevariables ( e1.text(), TIPO_CTA );
            } // end if
        } // end for
        /// Recogemos los valores de fecha.
        litems = m_doc.elementsByTagName ( "fecha" );
        for ( int i = 0; i < litems.count(); i++ ) {
            QDomNode item = litems.item ( i );
            /// Try to convert the node to an element.
            QDomElement e1 = item.toElement();
            /// the node was really an element.
            if ( !e1.isNull() ) {
                recogevariables ( e1.text(), TIPO_FECHA );
            } // end if
        } // end for
        /// Recogemos los valores de debe.
        litems = m_doc.elementsByTagName ( "debe" );
        for ( int i = 0; i < litems.count(); i++ ) {
            QDomNode item = litems.item ( i );
            /// Try to convert the node to an element.
            QDomElement e1 = item.toElement();
            /// The node was really an element.
            if ( !e1.isNull() ) {
                recogevariables ( e1.text(), TIPO_NUMERO );
            } // end if
        } // end for
        /// Recogemos los valores de haber.
        litems = m_doc.elementsByTagName ( "haber" );
        for ( int i = 0; i < litems.count(); i++ ) {
            QDomNode item = litems.item ( i );
            /// Try to convert the node to an element.
            QDomElement e1 = item.toElement();
            /// The node was really an element.
            if ( !e1.isNull() ) {
                recogevariables ( e1.text(), TIPO_NUMERO );
            } // end if
        } // end for
        /// Recogemos los valores de conceptocontable
        litems = m_doc.elementsByTagName ( "conceptocontable" );
        for ( int i = 0; i < litems.count(); i++ ) {
            QDomNode item = litems.item ( i );
            /// Try to convert the node to an element.
            QDomElement e1 = item.toElement();
            /// the node was really an element.
            if ( !e1.isNull() ) {
                recogevariables ( e1.text(), TIPO_TEXTO );
            } // end if
        } // end for
        /// Recogemos los valores de descripci&oacute;n.
        litems = m_doc.elementsByTagName ( "descripcion" );
        for ( int i = 0; i < litems.count(); i++ ) {
            QDomNode item = litems.item ( i );
            /// Try to convert the node to an element.
            QDomElement e1 = item.toElement();
            /// The node was really an element.
            if ( !e1.isNull() ) {
                recogevariables ( e1.text(), TIPO_TEXTO );
            } // end if
        } // end for

        for ( int i = 0;i < indvariablescta; i++ ) {
            labelcta[i] = new QLabel ( "", mui_datosAsiento );
            labelcta[i]->setGeometry ( QRect ( 5, inc , 150, 25 ) );
            labelcta[i]->setText ( variablescta[i][2] );
            labelcta[i]->show();
            varcta[i] = new BusquedaCuenta ( mui_datosAsiento );
            varcta[i]->setGeometry ( QRect ( 150, inc , 300, 50 ) );
            varcta[i]->setMainCompany ( mainCompany() );
            connect ( varcta[i], SIGNAL ( enterPressed() ), this, SLOT ( eturn_cta() ) );
            connect ( varcta[i], SIGNAL ( textChanged ( const QString & ) ), this, SLOT ( codigo_textChanged ( const QString & ) ) );
            varcta[i]->show();
	    inc += 57;
        } // end for

        for ( int i = 0;i < indvariablesfecha; i++ ) {
            labelfecha[i] = new QLabel ( "", mui_datosAsiento );
            labelfecha[i]->setGeometry ( QRect ( 5, inc , 150, 25 ) );
            labelfecha[i]->setText ( variablesfecha[i][2] );
            labelfecha[i]->show();

            varfecha[i] = new BlDateSearch ( mui_datosAsiento );
            varfecha[i]->setGeometry ( QRect ( 150, inc , 150, 25 ) );

            connect ( varfecha[i], SIGNAL ( enterPressed() ), this, SLOT ( eturn_fecha() ) );
            connect ( varfecha[i], SIGNAL ( textChanged ( const QString & ) ), this, SLOT ( fecha_textChanged ( const QString & ) ) );
            varfecha[i]->show();
	    inc += 32;
        } // end for

        for ( int i = 0;i < indvariablesnumero; i++ ) {
            labelnumero[i] = new QLabel ( "", mui_datosAsiento );
            labelnumero[i]->setGeometry ( QRect ( 5, inc , 150, 25 ) );
            labelnumero[i]->setText ( variablesnumero[i][2] );
            labelnumero[i]->show();
            varnumero[i] = new QLineEdit ( "", mui_datosAsiento );
            varnumero[i]->setGeometry ( QRect ( 150, inc , 150, 25 ) );
            connect ( varnumero[i], SIGNAL ( enterPressed() ), this, SLOT ( eturn_numero() ) );
            varnumero[i]->show();
	    inc += 32;
        } // end for

        for ( int i = 0;i < indvariablestexto; i++ ) {
            labeltexto[i] = new QLabel ( "", mui_datosAsiento );
            labeltexto[i]->setGeometry ( QRect ( 5, inc , 150, 25 ) );
            labeltexto[i]->setText ( variablestexto[i][2] );
            labeltexto[i]->show();
            vartexto[i] = new QLineEdit ( "", mui_datosAsiento );
            vartexto[i]->setGeometry ( QRect ( 150, inc , 350, 25 ) );
            connect ( vartexto[i], SIGNAL ( enterPressed() ), this, SLOT ( eturn_texto() ) );
            vartexto[i]->show();
	    inc += 32;
        } // end for
    }
    _depura ( "END aplinteligentesview::mostrarplantilla", 0 );
}


/// Esta funci&oacute;n permite establecer los valores de las variables siempre y
/// cuando estas variables existan.
/// De esta forma podemos establecer valores por defecto de determinados asientos de obligada
/// existencia. Como los de Amortizaciones.
/**
\param var
\param val
**/
void aplinteligentesview::setvalores ( QString var, QString val )
{
    _depura ( "aplinteligentesview::setvalores", 0 );
    for ( int i = 0;i < indvariablescta; i++ ) {
        if ( variablescta[i][0] == var ) {
            varcta[i]->setText ( val );
        } // end if
    } // end for
    for ( int i = 0; i < indvariablesfecha; i++ ) {
        if ( variablesfecha[i][0] == var ) {
            varfecha[i]->setText ( val );
        } // end if
    } // end for
    for ( int i = 0; i < indvariablesnumero; i++ ) {
        if ( variablesnumero[i][0] == var ) {
            varnumero[i]->setText ( val );
        } // end if
    } // end for
    for ( int i = 0; i < indvariablestexto; i++ ) {
        if ( variablestexto[i][0] == var ) {
            vartexto[i]->setText ( val );
        } // end if
    } // end for
    _depura ( "END aplinteligentesview::setvalores", 0 );
}


/// Esta funci&oacute;n recoge los valores de los cuadros de texto y
/// y rellena las tablas de variables con los valores recogidos.
/**
**/
void aplinteligentesview::recogevalores()
{
    _depura ( "aplinteligentesview::recogevalores", 0 );
    int i;
    for ( i = 0; i < indvariablestexto; i++ ) {
        variablestexto[i][1] = vartexto[i]->text();
    } // end for
    for ( i = 0; i < indvariablesnumero; i++ ) {
        variablesnumero[i][1] = varnumero[i]->text();
	if (variablesnumero[i][1].isEmpty())
		throw -1;
    } // end for
    for ( i = 0; i < indvariablesfecha; i++ ) {
        variablesfecha[i][1] = varfecha[i]->text();
	if (variablesfecha[i][1].isEmpty())
		throw -1;
    } // end for
    for ( i = 0; i < indvariablescta; i++ ) {
        variablescta[i][1] = varcta[i]->text();
	if (variablescta[i][1].isEmpty())
		throw -1;
    } // end for
    _depura ( "END aplinteligentesview::recogevalores", 0 );
}


/// Esta funci&oacute;n se encarga de crear el nuevo asiento a partir
/// de los datos introducidos.
/**
\return
**/
void aplinteligentesview::creaasiento()
{
    _depura ( "aplinteligentesview::creaasiento", 0 );
    QString codcuenta;
    QString contrapartida;
    QString debe;
    QString haber;
    QString fecha;
    QString conceptocontable;
    QString descripcion;
    int idcuenta = 0;
    QString idcontrapartida;
    QString query;
    BlDbRecordSet *cur1 = NULL;
    try {
        mainCompany() ->begin();
        /// Calculamos a partir de que orden debemos empezar.
        int orden = 0;
        query = "SELECT max(orden) AS ordmax FROM borrador WHERE idasiento = " + QString::number ( numasiento );
        cur1 = mainCompany() ->loadQuery ( query );
        if ( !cur1 ) throw - 1;
        if ( !cur1->eof() ) {
            orden = cur1->valor ( "ordmax" ).toInt() + 1;
        } // end if
        delete cur1;
        QDomNodeList litems = m_doc.elementsByTagName ( "binteligente" );
        for ( int i = 0; i < litems.count(); i++ ) {
            QDomNode item = litems.item ( i );
            codcuenta = aplicavariable ( item.firstChildElement ( "codcuenta" ).text() );
            query.sprintf ( "SELECT * FROM cuenta where codigo = '%s'", codcuenta.toAscii().constData() );
            cur1 = mainCompany() ->loadQuery ( query, "buscacodigo" );
            if ( !cur1 ) throw - 1;
            if ( !cur1->eof() ) {
                idcuenta = atoi ( cur1->valor ( "idcuenta" ).toAscii().constData() );
            } // end if
            delete cur1;

            contrapartida = aplicavariable ( item.firstChildElement ( "contrapartida" ).text() );
            query.sprintf ( "SELECT * FROM cuenta where codigo = '%s'", contrapartida.toAscii().constData() );
            cur1 = mainCompany() ->loadQuery ( query, "buscacodigo" );
            if ( !cur1 ) throw - 1;
            if ( !cur1->eof() ) {
                idcontrapartida = cur1->valor ( "idcuenta" );
            } else {
                idcontrapartida = "NULL";
            } // end if
            delete cur1;
            debe = aplicavariable ( item.firstChildElement ( "debe" ).text() );
            haber = aplicavariable ( item.firstChildElement ( "haber" ).text() );
            fecha = aplicavariable ( item.firstChildElement ( "fecha" ).text() );
            conceptocontable = aplicavariable ( item.firstChildElement ( "conceptocontable" ).text() );
            descripcion = aplicavariable ( item.firstChildElement ( "descripcion" ).text() );
            query.sprintf ( "INSERT INTO borrador (idasiento, idcuenta, contrapartida, debe, haber, fecha, conceptocontable, descripcion, orden) VALUES (%d, %d, %s, %s, %s, '%s', '%s', '%s', %d)", numasiento, idcuenta, idcontrapartida.toAscii().constData(), debe.toAscii().constData(), haber.toAscii().constData(), fecha.toAscii().constData(), conceptocontable.toAscii().constData(), descripcion.toAscii().constData(), orden++ );
            mainCompany() ->runQuery ( query );
            mainCompany() ->commit();
        } // end for
    } catch ( ... ) {
        mensajeInfo ( _( "Error al crear el asiento" ) );
        mainCompany() ->rollback();
        return;
    } // end try
    _depura ( "END aplinteligentesview::creaasiento", 0 );
}


/// Esta funci&oacute;n busca variables en una cadena de texto y la coloca en los arrays
/// de variables que forman el asiento inteligente.
/**
\param texto
\param tipo
\return
**/
void aplinteligentesview::recogevariables ( QString texto, int tipo )
{
    _depura ( "aplinteligentesview::recogevariables", 0 );
    int posinicial, posfinal, posaux, posaux1;
    QString cadena = texto;
    QString subcadena;
    QString nomvar, descvar;
    int j;
    /// Recogemos variables en el c&oacute;digo de cuenta.
    posinicial = cadena.indexOf ( "$", 0 );
    if ( posinicial != -1 ) {
        posfinal = cadena.indexOf ( "$", posinicial + 1 );
        if ( posfinal != -1 ) {
            subcadena = cadena.mid ( posinicial, posfinal + 1 );
            posaux = subcadena.indexOf ( ",", 0 );
            if ( posaux != -1 ) {
                posaux1 = subcadena.indexOf ( "$", posaux + 1 );
                descvar = subcadena.mid ( posaux + 1, posaux1 - posaux - 1 );
                nomvar = subcadena.mid ( 0, posaux + 1 );
                /// Si hay un comentario lo debemos borrar tras considerarlo para que no
                /// estorbe.
                nomvar.replace ( nomvar.length() - 1, nomvar.length(), "$" );
            } else {
                return;
            } // end if
            /// Buscamos si es una variable predefinida, y en caso de serlo obviamos una
            /// inserci&oacute;n de esta.
            j = 0;
            while ( j < indvariablespredefinidas && variablespredefinidas[j][0] != nomvar ) {
                j++;
            } // end while
            /// Buscamos si es una variable de apunte, y en caso afirmativo obviamos
            /// una inserci&oacute;n de esta.
            int d = 0;
            while ( d < indvariablesapunte && variablesapunte[d][0] != nomvar ) {
                d++;
            } // end while
            if ( j == indvariablespredefinidas && d == indvariablesapunte ) {
                switch ( tipo ) {
                case TIPO_CTA:
                    for ( j = 0; j < indvariablescta && variablescta[j][0] != nomvar; j++ )
                        ;
                    if ( j == indvariablescta ) {
                        /// Se trata de una inserci&oacute;n
                        variablescta[indvariablescta][0] = nomvar;
                        variablescta[indvariablescta][2] = descvar;
                        indvariablescta++;
                    } else {
                        /// Se trata de una modificaci&oacute;n.
                        if ( variablescta[j][2] == "" )
                            variablescta[j][2] = descvar;
                    } // end if
                    break;
                case TIPO_FECHA:
                    for ( j = 0; j < indvariablesfecha && variablesfecha[j][0] != subcadena; j++ )
                        ;
                    if ( j == indvariablesfecha ) {
                        variablesfecha[indvariablesfecha][0] = nomvar;
                        variablesfecha[indvariablesfecha][2] = descvar;
                        indvariablesfecha++;
                    } else {
                        if ( variablesfecha[j][2] == "" )
                            variablesfecha[j][2] = descvar;
                    } // end if
                    break;
                case TIPO_NUMERO:
                    for ( j = 0; j < indvariablesnumero && variablesnumero[j][0] != subcadena; j++ )
                        ;
                    if ( j == indvariablesnumero ) {
                        variablesnumero[indvariablesnumero][0] = nomvar;
                        variablesnumero[indvariablesnumero][2] = descvar;
                        indvariablesnumero++;
                    } else {
                        if ( variablesnumero[j][2] == "" )
                            variablesnumero[j][2] = descvar;
                    } // end if
                    break;
                case TIPO_TEXTO:
                    for ( j = 0; j < indvariablestexto && variablestexto[j][0] != subcadena; j++ )
                        ;
                    if ( j == indvariablestexto ) {
                        variablestexto[indvariablestexto][0] = nomvar;
                        variablestexto[indvariablestexto][2] = descvar;
                        indvariablestexto++;
                    } else {
                        if ( variablestexto[j][2] == "" )
                            variablestexto[j][2] = descvar;
                    } // end if
                    break;
                } // end switch
            } // end if
        } // end if
    } // end if
    _depura ( "END aplinteligentesview::recogevariables", 0 );
}


/// Se hace un repaso que hace que se aplique una variable.
/**
\param texto
**/
QString aplinteligentesview::aplicavariable ( QString texto )
{
    _depura ( "aplinteligentesview::aplicavariable", 0 );
    QString cadena = texto;
    int posinicial, posfinal;
    int i;
    int fin = 0;
    /// Vamos a quitar todos los comentarios que aparezcan.
    while ( !fin ) {
        fin = 1;
        posinicial = cadena.indexOf ( ",", 0 );
        if ( posinicial != -1 ) {
            posfinal = cadena.indexOf ( "$", posinicial );
            if ( posfinal != -1 ) {
                cadena.replace ( posinicial, posfinal, "$" );
                fin = 0;
            } // end if
        } // end if
    } // end while
    for ( i = 0; i < indvariablespredefinidas; i++ ) {
        posinicial = cadena.indexOf ( variablespredefinidas[i][0], 0 );
        while ( posinicial != -1 ) {
            cadena.replace ( posinicial, variablespredefinidas[i][0].length(), variablespredefinidas[i][1] );
            posinicial = cadena.indexOf ( variablespredefinidas[i][0], 0 );
        } // end if
    } // end for
    for ( i = 0;i < indvariablestexto; i++ ) {
        posinicial = cadena.indexOf ( variablestexto[i][0], 0 );
        while ( posinicial != -1 ) {
            cadena.replace ( posinicial, variablestexto[i][0].length(), variablestexto[i][1] );
            posinicial = cadena.indexOf ( variablestexto[i][0], 0 );
        } // end if
    } // end for
    for ( i = 0; i < indvariablesfecha; i++ ) {
        posinicial = cadena.indexOf ( variablesfecha[i][0], 0 );
        while ( posinicial != -1 ) {
            cadena.replace ( posinicial, variablesfecha[i][0].length(), variablesfecha[i][1] );
            posinicial = cadena.indexOf ( variablesfecha[i][0], 0 );
        } // end if
    } // end for
    for ( i = 0;i < indvariablesnumero; i++ ) {
        posinicial = cadena.indexOf ( variablesnumero[i][0], 0 );
        while ( posinicial != -1 ) {
            cadena.replace ( posinicial, variablesnumero[i][0].length(), variablesnumero[i][1] );
            posinicial = cadena.indexOf ( variablesnumero[i][0], 0 );
        } // end if
    } // end for
    for ( i = 0;i < indvariablescta; i++ ) {
        posinicial = cadena.indexOf ( variablescta[i][0], 0 );
        while ( posinicial != -1 ) {
            cadena.replace ( posinicial, variablescta[i][0].length(), variablescta[i][1] );
            posinicial = cadena.indexOf ( variablescta[i][0], 0 );
        } // end if
    } // end for
    /// aplicamos las variables de apunte.
    for ( i = 0; i < indvariablesapunte; i++ ) {
        posinicial = cadena.indexOf ( variablesapunte[i][0], 0 );
        while ( posinicial != -1 ) {
            cadena.replace ( posinicial, variablesapunte[i][0].length(), variablesapunte[i][1] );
            posinicial = cadena.indexOf ( variablesapunte[i][0], 0 );
        } // end if
    } // end for
    return cadena;
    _depura ( "END aplinteligentesview::aplicavariable", 0 );
}


///
/**
**/
void aplinteligentesview::borrawidgets()
{
    _depura ( "aplinteligentesview::borrawidgets", 0 );
    int i;
    /// Vamos a intentar borrar todos los datos antes de empezar.
    for ( i = 0; i < indvariablescta; i++ ) {
        delete varcta[i];
        delete labelcta[i];
    } // end for
    for ( i = 0; i < indvariablesfecha; i++ ) {
        delete varfecha[i];
        delete labelfecha[i];
    }// end for
    for ( i = 0; i < indvariablesnumero; i++ ) {
        delete varnumero[i];
        delete labelnumero[i];
    } // end for
    for ( i = 0; i < indvariablestexto; i++ ) {
        delete vartexto[i];
        delete labeltexto[i];
    } // end for
    indvariablescta = 0;
    indvariablesfecha = 0;
    indvariablesnumero = 0;
    indvariablestexto = 0;
    _depura ( "END aplinteligentesview::borrawidgets", 0 );
}


///
/**
**/
void aplinteligentesview::selectfirst()
{
    _depura ( "aplinteligentesview::selectfirst", 0 );
    if ( indvariablescta > 0 ) {
        varcta[0]->selectAll();
        varcta[0]->setFocus();
    } else if ( indvariablesfecha > 0 ) {
        varfecha[0]->selectAll();
        varfecha[0]->setFocus();
    } else if ( indvariablesnumero > 0 ) {
        varnumero[0]->selectAll();
        varnumero[0]->setFocus();
    } else if ( indvariablestexto > 0 ) {
        vartexto[0]->selectAll();
        vartexto[0]->setFocus();
    } // end if
    _depura ( "END aplinteligentesview::selectfirst", 0 );
}


/// Esta funci&oacute;n sirve para cambiar de un widget a otro dentro de la pantalla
/// creada virtualmente.
/// Si el widget es el &uacute;ltimo de la lista se hace un aceptar.
/**
\param edit
**/
void aplinteligentesview::selectsiguiente ( QObject *edit )
{
    _depura ( "aplinteligentesview::selectsiguiente", 0 );
    int encontrado = 0;
    int i = 0;
    for ( i = 0; i < indvariablescta; i++ ) {
        if ( encontrado == 1 ) {
            varcta[i]->selectAll();
            varcta[i]->setFocus();
            encontrado = 2;
        } // end if
        if ( varcta[i] == edit ) {
            encontrado = 1;
        } // end if
    } // end for
    for ( i = 0; i < indvariablesfecha; i++ ) {
        if ( encontrado == 1 ) {
            varfecha[i]->selectAll();
            varfecha[i]->setFocus();
            encontrado = 2;
        } // end if
        if ( varfecha[i] == edit ) {
            encontrado = 1;
        } // end if
    } // end for
    for ( i = 0; i < indvariablesnumero; i++ ) {
        if ( encontrado == 1 ) {
            varnumero[i]->selectAll();
            varnumero[i]->setFocus();
            encontrado = 2;
        } // end if
        if ( varnumero[i] == edit ) {
            encontrado = 1;
        } // end if
    } // end for
    for ( i = 0; i < indvariablestexto; i++ ) {
        if ( encontrado == 1 ) {
            vartexto[i]->selectAll();
            vartexto[i]->setFocus();
            encontrado = 2;
        } // end if
        if ( vartexto[i] == edit ) {
            encontrado = 1;
        } // end if
    } // end for
    /// En caso de que se haya encontrado el edit, y sea el &uacute;ltimo
    /// Se hace un aceptar.
    if ( encontrado == 1 ) {
        on_mui_aceptar_clicked();
    } // end if
    _depura ( "END aplinteligentesview::selectsiguiente", 0 );
}


///
/**
\param fecha
**/
void aplinteligentesview::setfechaasiento ( QString fecha )
{
    _depura ( "aplinteligentesview::setfechaasiento", 0 );
    fechaasiento->setText ( fecha );
    _depura ( "END aplinteligentesview::setfechaasiento", 0 );
}


/// La funci&oacute;n sirve para especificar el modo de funcionamiento de los asientos
/// intelgientes. Consultar la variable m&eacute;todo para m&aacute;s detalles.
/**
\param i
**/
void aplinteligentesview::setmodo ( int i )
{
    _depura ( "aplinteligentesview::setmodo", 0 );
    modo = i;
    _depura ( "END aplinteligentesview::setmodo", 0 );
}


///
/**
**/
void aplinteligentesview::on_mui_guardar_clicked()
{
    _depura ( "aplinteligentesview::on_mui_guardar_clicked", 0 );
    guardar();
    _depura ( "END aplinteligentesview::on_mui_guardar_clicked", 0 );
}
