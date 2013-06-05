/***************************************************************************
 *   Copyright (C) 2009 by Tomeu Borras Riera                              *
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

#include <cstdio>

#include <QtWidgets/QAction>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QWidget>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtCore/QObject>
#include <QtCore/QTextCodec>
#include <QtCore/QLocale>
#include <QtWidgets/QInputDialog>

#include "pluginbf_asiento.h"
#include "bcasientoview.h"
#include "bfcompany.h"
#include "bcasientolistview.h"
#include "bcasientointeligenteview.h"

BcAsientoView *g_asiento;
BfBulmaFact *g_pluginbf_asiento = NULL;
BcAsientoInteligenteView *g_asientoInteligente = NULL;

///
/**
\param bcont
**/
int entryPoint ( BfBulmaFact *bcont )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_asiento", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );

    g_asiento = NULL;
    g_pluginbf_asiento = bcont;


    /// Creamos la ventana de asientos que va a ser fija.
    g_asiento = new BcAsientoView ( ( BfCompany * ) g_pluginbf_asiento->company(), 0 );
    g_pluginbf_asiento->company()->pWorkspace() ->addSubWindow ( g_asiento );

    QMenu *pPluginMenu = bcont->newMenu(_("&Asiento"), "menuAsiento", "menuMaestro");

    BlAction *accionA = new BlAction ( _ ( "&Asiento contable" ), 0 );
    accionA->setStatusTip ( _ ( "Permite ver y modificar asientos" ) );
    accionA->setWhatsThis ( _ ( "Podra disponer de la informacion del asiento" ) );
    accionA->setIcon(QIcon(QString::fromUtf8(":/Images/book.png")));
    accionA->setObjectName("mui_actionAsientoContable");
    pPluginMenu->addAction ( accionA );
    bcont->Listados->addAction ( accionA );


    BlAction *accionB = new BlAction ( _ ( "&Lista de asientos contables" ), 0 );
    accionB->setStatusTip ( _ ( "Permite ver el listado de asientos" ) );
    accionB->setWhatsThis ( _ ( "Permite ver el listado de asientos" ) );
    accionB->setIcon(QIcon(QString::fromUtf8(":/Images/book-list.png")));
    accionB->setObjectName("mui_actionAsientoContableLista");
    pPluginMenu->addAction ( accionB );
    bcont->Listados->addAction ( accionB );

    pPluginMenu->addSeparator();

    BlAction *accionC = new BlAction ( _ ( "&Espaciar asientos contables" ), 0 );
    accionC->setStatusTip ( _ ( "Ordena los asientos dejando espacios entre ellos" ) );
    accionC->setWhatsThis ( _ ( "Ordena los asientos dejando espacios entre ellos" ) );
    accionC->setObjectName("mui_actionAsientoContableEspaciar");
    pPluginMenu->addAction ( accionC );


    BlAction *accionD = new BlAction ( _ ( "&Reordenar asientos contables" ), 0 );
    accionD->setStatusTip ( _ ( "Ordena los asientos" ) );
    accionD->setWhatsThis ( _ ( "Ordena los asientos" ) );
    accionD->setObjectName("mui_actionAsientoContableOrdenar");
    pPluginMenu->addAction ( accionD );

    pPluginMenu->addSeparator();

    BlAction *accionE = new BlAction ( _ ( "Asiento de a&pertura" ), 0 );
    accionE->setStatusTip ( _ ( "Asiento de apertura" ) );
    accionE->setWhatsThis ( _ ( "Asiento de apertura" ) );
    accionE->setObjectName("mui_actionAsientoApertura");
    pPluginMenu->addAction ( accionE );

    BlAction *accionF = new BlAction ( _ ( "Asiento de &cierre" ), 0 );
    accionF->setStatusTip ( _ ( "Asiento de cierre" ) );
    accionF->setWhatsThis ( _ ( "Asiento de cierre" ) );
    accionF->setObjectName("mui_actionAsientoCierre");
    pPluginMenu->addAction ( accionF );

    BlAction *accionG = new BlAction ( _ ( "Asiento de &regularizacion" ), 0 );
    accionG->setStatusTip ( _ ( "Asiento de regularizacion" ) );
    accionG->setWhatsThis ( _ ( "Asiento de regularizacion" ) );
    accionG->setObjectName("mui_actionAsientoRegularizacion");
    pPluginMenu->addAction ( accionG );


    
    return 0;
}

int BlAction_actionTriggered(BlAction *accion) {

    if (accion->objectName() == "mui_actionAsientoContable") {
        if (g_asiento == NULL) {
            g_asiento = new BcAsientoView ( g_pluginbf_asiento->company(), 0 );
            g_pluginbf_asiento->company()->pWorkspace() -> addSubWindow ( g_asiento );
        } // end if
        g_asiento->hide();
        g_asiento->show();
    } // end if
    if (accion->objectName() == "mui_actionAsientoContableLista") {

    	if (!g_pluginbf_asiento->company()->showWindow("BcAsientoListView")) {
	    BcAsientoListView *asientoListView = new BcAsientoListView ( g_pluginbf_asiento->company(), 0 );
	    asientoListView->setObjectName("BcAsientoListView");
	    asientoListView->presentar();
            g_pluginbf_asiento->company()->pWorkspace() -> addSubWindow ( asientoListView );
	    asientoListView->show();
	} // end if

    } // end if

    if (accion->objectName() == "mui_actionAsientoContableEspaciar") {
        BlDbRecordSet *cur = NULL;
        try {
            cur = g_pluginbf_asiento->company()->loadQuery ( "SELECT abreasientos()" );
            g_main->statusBar() ->showMessage ( _ ( "Se han espaciado los asientos" ), 2000 );
            delete cur;
        } catch ( ... ) {
            blMsgError ( _("Ha habido un error al espaciar los asientos") );
            if ( cur ) delete cur;
        return 1;
        } // end try
    } // end if

    if (accion->objectName() == "mui_actionAsientoContableOrdenar") {
        QString query = "SELECT reordenaasientosall()";
        BlDbRecordSet *cur = NULL;
        try {
            cur = g_pluginbf_asiento->company()->loadQuery ( query );
            g_main->statusBar() ->showMessage ( _ ( "Se han ordenado los asientos" ), 2000 );
            delete cur;
        } catch ( ... ) {
            blMsgError ( _("Ha habido un error al ordenar los asientos") );
            if ( cur ) delete cur;
            return 1;
        }
    } // end if

    if (accion->objectName() == "mui_actionAsientoApertura") {
        QString hoy = QDate::currentDate().toString ( "dd/MM/yyyy" );
        QString finicial = "01/01/" + hoy.right ( 4 );
        bool ok;
    
        QString text1 = QInputDialog::getText ( 0,
                _ ( "Fecha" ), _ ( "Fecha:" ), QLineEdit::Normal,
                hoy, &ok );
        if ( ok && !text1.isEmpty() ) {
            hoy = text1;
        } else {
            return 0;
        } // end if
        g_asiento->show();
        g_asiento->asientoApertura ( hoy );
    } // end if

    if (accion->objectName() == "mui_actionAsientoCierre") {
        QString hoy = QDate::currentDate().toString ( "dd/MM/yyyy" );
        QString finicial = "01/01/" + hoy.right ( 4 );
    
        bool ok;
        QString text = QInputDialog::getText ( 0,
                _ ( "Fecha inicial" ), _ ( "Fecha inicial ejercicio:" ), QLineEdit::Normal,
                finicial, &ok );
    
        if ( ok && !text.isEmpty() ) {
            finicial = text;
        } else {
            return 0;
        } // end if
    
        QString text1 = QInputDialog::getText ( 0,
                _ ( "Fecha final" ), _ ( "Fecha final ejercicio:" ), QLineEdit::Normal,
                hoy, &ok );

        if ( ok && !text1.isEmpty() ) {   
            hoy = text1;
        } else {
            return 0;
        } // end if

        g_asiento->show();
        g_asiento->asientoCierre ( finicial, hoy );

        } // end if
        if (accion->objectName() == "mui_actionAsientoRegularizacion") {
            QString hoy = QDate::currentDate().toString ( "dd/MM/yyyy" );
            QString finicial = "01/01/" + hoy.right ( 4 );

            bool ok;
            QString text = QInputDialog::getText ( 0,
                    _ ( "Fecha inicial" ), _ ( "Fecha inicial regularizacion:" ), QLineEdit::Normal,
                    finicial, &ok );
            if ( ok && !text.isEmpty() ) {
                finicial = text;
            } else {
                return 0;
            } // end if
   
            QString text1 = QInputDialog::getText ( 0,
                    _ ( "Fecha final" ), _ ( "Fecha final regularizacion:" ), QLineEdit::Normal,
                    hoy, &ok );
            if ( ok && !text1.isEmpty() ) {
                hoy = text1;
            } else {
                return 0;
            } // end if
            g_asiento->show();
            g_asiento->asientoRegularizacion ( finicial, hoy );
        } // end if

        return 0;
}


/// Esta llamada de plugin es bastante novedosa ya es una llamada que no responde a una funcion
/// Sino que se llama desde multiples partes del sistema.
int SNewBcAsientoView ( BfCompany * )
{
    g_plugParams = g_asiento;
    return 1;
}

/// Esta llamada de plugin es bastante novedosa ya es una llamada que no responde a una funcion
/// Sino que se llama desde multiples partes del sistema.
int SNewBcAsientoInteligenteView ( BfCompany *company )
{
    BcAsientoInteligenteView *nueva = new BcAsientoInteligenteView ( company, 0 );
    g_plugParams = nueva;
    return 1;
}







/// --------------------------------------------------------------
/// --------- Implemento la seleccion de diarios -----------------
/// Con esta funcionalidad creamos menus contextuales en todos los subformularios donde
/// Aparezca el identificador de articulo como elemento y permite de forma sencilla
/// La creacion, la edicion, y la seleccion.
///

/**
\param parent
**/
SubForm_Asiento::SubForm_Asiento ( BlSubForm *parent ) : QObject ( parent )
{
    BL_FUNC_DEBUG
    
}

///
/**
**/
SubForm_Asiento::~SubForm_Asiento()
{
    BL_FUNC_DEBUG
    
}


///
/**
\param menu
**/
void SubForm_Asiento::s_pintaMenu ( QMenu *menu )
{
    BL_FUNC_DEBUG
    BlSubForm *sub = ( BlSubForm * ) parent();
    BlSubFormHeader *header = sub->header ( "idasiento" );
    if ( header ) {
        menu->addSeparator();
        QString idcuenta = sub->dbValue ( "idasiento" );
        if ( idcuenta != "" )  {
	    menu->addAction ( _ ( "Mostrar Asiento" ) );
	} // end if
    } // end if
}


///
/**
\param action
**/
void SubForm_Asiento::s_trataMenu ( QAction *action )
{
    BL_FUNC_DEBUG
    BlSubForm *sub = ( BlSubForm * ) parent();
    QString idcuenta = sub->dbValue ( "idasiento" );

    if ( idcuenta != "" ) {
	if ( action->text() == _ ( "Mostrar Asiento" ) ) {
            mostrarAsiento ();
	} // end if
    } // end if
}



/// Si el parametro pasado es un:
/// 0 -> del d&iacute;a actual
/// 1 -> del mes actual
/// 2 -> del a&ntilde;o actual
/**
\param tipo
**/
void SubForm_Asiento::mostrarAsiento (  )
{
    BL_FUNC_DEBUG

    BlSubForm *sub = ( BlSubForm * ) parent();
    
    QString numasiento = sub->dbValue ( "idasiento" );
    if ( numasiento != "" ) {
        g_asiento ->muestraAsiento ( numasiento.toInt() );
        g_asiento->hide();
	g_asiento->show();
    } // end if  
}



///
/**
\param sub
\return
**/
int BlSubForm_BlSubForm_Post ( BlSubForm *sub )
{
    BL_FUNC_DEBUG
    SubForm_Asiento *subformods = new SubForm_Asiento ( sub );
    sub->QObject::connect ( sub, SIGNAL ( pintaMenu ( QMenu * ) ), subformods, SLOT ( s_pintaMenu ( QMenu * ) ) );
    sub->QObject::connect ( sub, SIGNAL ( trataMenu ( QAction * ) ), subformods, SLOT ( s_trataMenu ( QAction * ) ) );
    
    return 0;
}



/// Miramos de poner los iconos del menu de subformularios
///
/**
\param sub
\return
**/
int BlSubForm_preparaMenu ( BlSubForm *sub ) {
    BL_FUNC_DEBUG
    BlSubFormHeader *header = sub->header ( "codigo" );
    if ( header ) {
	SubForm_Asiento *subformods = new SubForm_Asiento ( sub );
	
	QHBoxLayout *m_hboxLayout1 = sub->mui_menusubform->findChild<QHBoxLayout *> ( "hboxLayout1" );
	if ( !m_hboxLayout1 ) {
	    m_hboxLayout1 = new QHBoxLayout ( sub->mui_menusubform );
	    m_hboxLayout1->setSpacing (0 );
	    m_hboxLayout1->setMargin ( 0 );
	    m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
	} // end if
	

	  QToolButton *sel = new QToolButton ( sub->mui_menusubform );
	  sel->setStatusTip ( "Mostrar Asiento" );
	  sel->setToolTip ( "Mostrar Asiento" );
	  sel->setMinimumSize ( QSize ( 18, 18 ) );
	  sel->setIcon ( QIcon ( ":/Images/book.png" ) );
	  sel->setIconSize ( QSize ( 18, 18 ) );    
	  m_hboxLayout1->addWidget ( sel );
	  sel->connect (sel, SIGNAL(released()), subformods, SLOT(mostrarAsiento()));
	
    } // end if
    return 0;
}




///
/**
\param l
\return
**/
int FacturaView_FacturaView ( FacturaView *l )
{
    BL_FUNC_DEBUG
    BlToolButton *agalbaran = new BlToolButton ( l );
    agalbaran->setObjectName("mui_asiento");
    
    agalbaran->setStatusTip ( "Mostrar asiento" );
    agalbaran->setToolTip ( "Mostrar asiento" );
    agalbaran->setMinimumSize ( QSize ( 32, 32 ) );
    agalbaran->setMaximumSize ( QSize ( 32, 32 ) );
    agalbaran->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/book.png" ) ) );
    agalbaran->setIconSize ( QSize ( 22, 22 ) );
    agalbaran->setContentsMargins ( 0, 0, 0, 0 );
    
    
    QHBoxLayout *m_hboxLayout1 = l->mui_plugbotones->findChild<QHBoxLayout *> ( "hboxLayout1" );

    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QHBoxLayout ( l->mui_plugbotones );
        m_hboxLayout1->setSpacing ( 5 );
        m_hboxLayout1->setMargin ( 0 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    }// end if
    
    m_hboxLayout1->addWidget ( agalbaran );
    
    return 0;
}


int BlToolButton_released(BlToolButton *bot) {

  if (bot->objectName() == "mui_asiento") {
    FacturaView *fact1 = (FacturaView *) bot->parent()->parent()->parent();

    QString query = "SELECT idasiento FROM factura WHERE idfactura = " + fact1->dbValue("idfactura");
    BlDbRecordSet *cur = fact1->mainCompany()->loadQuery(query);
    if (!cur->eof()) {
      if (cur->value("idasiento") != "") {
        g_asiento ->muestraAsiento ( cur->value("idasiento").toInt() );
        g_asiento->hide();
	g_asiento->show();
      } // end if
    } // end if
    
  } //end if
  
  
  if (bot->objectName() == "mui_asientoc") {
    CobroView *fact1 = (CobroView *) bot->parent()->parent()->parent();

    QString query = "SELECT idasiento FROM cobro WHERE idcobro = " + fact1->dbValue("idcobro");
    BlDbRecordSet *cur = fact1->mainCompany()->loadQuery(query);
    if (!cur->eof()) {
      if (cur->value("idasiento") != "") {
        g_asiento ->muestraAsiento ( cur->value("idasiento").toInt() );
        g_asiento->hide();
	g_asiento->show();
      } // end if
    } // end if
    
  } //end if
  
  
  if (bot->objectName() == "mui_asientop") {
    FacturaProveedorView *fact1 = (FacturaProveedorView *) bot->parent()->parent()->parent();

    QString query = "SELECT idasiento FROM facturap WHERE idfacturap = " + fact1->dbValue("idfacturap");
    BlDbRecordSet *cur = fact1->mainCompany()->loadQuery(query);
    if (!cur->eof()) {
      if (cur->value("idasiento") != "") {
        g_asiento ->muestraAsiento ( cur->value("idasiento").toInt() );
        g_asiento->hide();
	g_asiento->show();
      } // end if
    } // end if
    
  } //end if
  
  
  if (bot->objectName() == "mui_asientopa") {
    PagoView *fact1 = (PagoView *) bot->parent()->parent()->parent();

    QString query = "SELECT idasiento FROM pago WHERE idpago = " + fact1->dbValue("idpago");
    BlDbRecordSet *cur = fact1->mainCompany()->loadQuery(query);
    if (!cur->eof()) {
      if (cur->value("idasiento") != "") {
        g_asiento ->muestraAsiento ( cur->value("idasiento").toInt() );
        g_asiento->hide();
	g_asiento->show();
      } // end if
    } // end if
    
  } //end if
  
  return 0;
}


int FacturaView_afterSave_Post(FacturaView *fact) {
    BL_FUNC_DEBUG
try {  
    QString idasiento = "";
    QString fecha = fact->dbValue("ffactura");
    if (fecha == "") fecha = "now()";
      
    QString query = "SELECT idasiento FROM factura WHERE idfactura = " + fact->dbValue("idfactura");
    BlDbRecordSet *cur = fact->mainCompany()->loadQuery(query);
    if (!cur->eof()) {
      if (cur->value("idasiento") != "") {
         query = "DELETE FROM borrador WHERE idasiento = " + cur->value("idasiento");
	 fact->mainCompany()->runQuery(query);
         query = "DELETE FROM apunte WHERE idasiento = " + cur->value("idasiento");
	 fact->mainCompany()->runQuery(query);
	 idasiento = cur->value("idasiento");
      } // end if
    } // end if
    delete cur;

  if (idasiento == "") {

    query= "SELECT * FROM ejercicios WHERE ejercicio = EXTRACT (YEAR FROM '"+fecha+"'::date) AND periodo = 0";
    cur = fact->mainCompany()->loadQuery(query);
    if (cur->eof()) {
      delete cur;
      blMsgInfo(_("Ejercicio contable inexistente. No se puede crear el asiento."));
      return 0;
    } // end if
    if ( cur->value("bloqueado") == "t") {
      delete cur;
      blMsgInfo(_("Ejercicio contable bloqueado. No se puede crear el asiento."));
      return 0;
    } // end if
    delete cur;
      

    query = "SELECT * FROM ejercicios WHERE ejercicio = EXTRACT (YEAR FROM '"+fecha+"'::date) AND periodo = EXTRACT (MONTH FROM '"+fecha+"'::date)";
    cur = fact->mainCompany()->loadQuery(query);
    if ( cur->value("bloqueado") == "t") {
      delete cur;
      blMsgInfo(_("Ejercicio contable bloqueado. No se puede crear el asiento."));
      return 0;
    } // end if
    delete cur;
    
    
      query = "INSERT INTO asiento (fecha, descripcion, comentariosasiento, clase) VALUES ('"+fecha+"','Fra. Cliente"+ fact->dbValue("codigoserie_factura") + fact->dbValue("numfactura") +"','"+fact->dbValue("descfactura")+"',1)";

      fact->mainCompany()->runQuery(query);
      
      query = "SELECT max(idasiento) AS id FROM asiento ";
      cur = fact->mainCompany()->loadQuery(query);
      if (!cur->eof()) {
	  idasiento = cur->value("id");
      } // end if
      delete cur;
   
  }// end if
  
  /// El apunte por cuenta del cliente.
  query = "SELECT * FROM cliente WHERE idcliente = "+fact->dbValue("idcliente");
  cur = fact->mainCompany()->loadQuery(query);
  if (!cur->eof()) {
      if (cur->value("idcuenta") == "") {
	  blMsgInfo("No se pudo crear el asiento ya que el cliente no tiene cuenta asociada");
	  delete cur;
	  return 0;
      } /// end if
      query = "INSERT INTO borrador(idasiento, fecha, conceptocontable, idcuenta, debe, descripcion) VALUES ("+idasiento+",'"+fecha+"','Fra. Cliente"+ fact->dbValue("codigoserie_factura") + fact->dbValue("numfactura") +"',"+cur->value("idcuenta")+","+fact->m_totalfactura->text().replace(",",".")+",'"+fact->dbValue("descfactura")+"')";

      fact->mainCompany()->runQuery(query);
  } // end if
  delete cur;
  
  
  query = "SELECT id_cuenta('47300001') AS id";
  cur = fact->mainCompany()->loadQuery(query);
  if (cur->value("id") == "0") {
       blMsgInfo("No se puede crear el asiento porque no existe la cuenta de IRPF 47300001");
       delete cur;
       return 0;
  } // end if
  delete cur;
  
  // El apunte por el irpf
  query = "INSERT INTO borrador(idasiento, fecha, conceptocontable, idcuenta, debe, descripcion) VALUES ("+idasiento+",'"+fecha+"','Fra. Cliente"+ fact->dbValue("codigoserie_factura") + fact->dbValue("numfactura") +"',id_cuenta('47300001'),"+fact->m_totalIRPF->text().replace(",",".")+",'"+fact->dbValue("descfactura")+"')";

  fact->mainCompany()->runQuery(query);
  
  
  /// Los apuntes por cuenta del IVA
  query = "SELECT ivalfactura::INTEGER, SUM(cantlfactura*pvplfactura*(1-descuentolfactura/100)*ivalfactura/100)::NUMERIC(12,2) AS subbase FROM lfactura WHERE idfactura = "+fact->dbValue("idfactura")+"  GROUP BY ivalfactura" ;
  cur = fact->mainCompany()->loadQuery(query);
  while (!cur->eof()) {
      QString codint = "472";
      // PONER UN PARAMETRO APROPIADO
      while ( codint.length() <  g_confpr->value(CONF_CONT_NUMDIGITOSEMPRESA).toInt() - cur->value("ivalfactura").length() ) {
	  codint = codint + "0";
      } // end while
      codint = codint + cur->value("ivalfactura");	
      
      query = "SELECT id_cuenta('"+codint+"') AS id";
      BlDbRecordSet *cur1 = fact->mainCompany()->loadQuery(query);
      if (cur1->value("id") == "0") {
	  blMsgInfo("No se puede crear el asiento porque no existe la cuenta de IVA "+ codint);
	  delete cur1;
	  delete cur;
	  return 0;
      } // end if
      delete cur1;
      
      query = "INSERT INTO borrador(idasiento, fecha, conceptocontable, idcuenta, haber, descripcion) VALUES ("+idasiento+",'"+fecha+"','Fra. Cliente"+ fact->dbValue("codigoserie_factura") + fact->dbValue("numfactura") +"',id_cuenta('"+codint+"'),"+cur->value("subbase")+",'"+fact->dbValue("descfactura")+"')";

      fact->mainCompany()->runQuery(query);
      cur->nextRecord();
  } // end while
  delete cur;
  
  /// Los apuntes por servicio u obra.
  query = "SELECT idcuenta, SUM(cantlfactura*pvplfactura*(1-descuentolfactura/100))::NUMERIC(12,2) AS subbase FROM lfactura " \
  "LEFT JOIN articulo AS t1 ON t1.idarticulo = lfactura.idarticulo " \
  "LEFT JOIN familia ON t1.idfamilia = familia.idfamilia WHERE idfactura = "+fact->dbValue("idfactura")+"  GROUP BY idcuenta" ;

  cur = fact->mainCompany()->loadQuery(query);
  while (!cur->eof()) {	  
    
      if (cur->value("idcuenta") == "") {
	blMsgInfo("No se pudo crear el asiento porque hay una familia sin cuenta asociada de venta o de servicio");
	delete cur;
	return 0;
      } // end if
      query = "INSERT INTO borrador(idasiento, fecha, conceptocontable, idcuenta, haber, descripcion) VALUES ("+idasiento+",'"+fecha+"','Fra. Cliente"+ fact->dbValue("codigoserie_factura") + fact->dbValue("numfactura") +"',"+cur->value("idcuenta")+","+cur->value("subbase")+",'"+fact->dbValue("descfactura")+"')";

      fact->mainCompany()->runQuery(query);
      cur->nextRecord();
  } // end while
  delete cur;
  
  query = "UPDATE factura SET idasiento= " + idasiento + " WHERE idfactura= "+ fact->dbValue("idfactura");
  fact->mainCompany()->runQuery(query);
  
  
  query = "SELECT cierraasiento("+idasiento+")";
  fact->mainCompany()->runQuery(query);
  
} catch(...) {
  blMsgInfo("No se pudo crear el asiento contable");
  return 0;
} // end try
  return 0;
}



/// ===================================== LOS COBROS A CLIENTES


///
/**
\param l
\return
**/
int CobroView_CobroView ( CobroView *l )
{
    BL_FUNC_DEBUG
    BlToolButton *agalbaran = new BlToolButton ( l );
    agalbaran->setObjectName("mui_asientoc");
    
    agalbaran->setStatusTip ( "Mostrar asiento" );
    agalbaran->setToolTip ( "Mostrar asiento" );
    agalbaran->setMinimumSize ( QSize ( 32, 32 ) );
    agalbaran->setMaximumSize ( QSize ( 32, 32 ) );
    agalbaran->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/book.png" ) ) );
    agalbaran->setIconSize ( QSize ( 22, 22 ) );
    agalbaran->setContentsMargins ( 0, 0, 0, 0 );
    
    
    QHBoxLayout *m_hboxLayout1 = l->mui_plugbotones->findChild<QHBoxLayout *> ( "hboxLayout1" );

    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QHBoxLayout ( l->mui_plugbotones );
        m_hboxLayout1->setSpacing ( 5 );
        m_hboxLayout1->setMargin ( 0 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    }// end if
    
    m_hboxLayout1->addWidget ( agalbaran );
    
    return 0;
}





int CobroView_afterSave_Post(CobroView *fact) {

    QString idasiento = "";

  
    QString query = "SELECT * FROM cobro WHERE idcobro = " + fact->dbValue("idcobro");
    BlDbRecordSet *curcobro = fact->mainCompany()->loadQuery(query);
    if (!curcobro->eof()) {
      if (curcobro->value("idasiento") != "") {
         query = "DELETE FROM borrador WHERE idasiento = " + curcobro->value("idasiento");
	 fact->mainCompany()->runQuery(query);
         query = "DELETE FROM apunte WHERE idasiento = " + curcobro->value("idasiento");
	 fact->mainCompany()->runQuery(query);
	 idasiento = curcobro->value("idasiento");
      } // end if
    } // end if
  
  QString fecha = curcobro->value("fechacobro");
  if (fecha == "") fecha = "now()";
    
  if (idasiento == "") {
      query = "INSERT INTO asiento (fecha, descripcion, comentariosasiento, clase) VALUES ('"+fecha+"','Cobro. Cliente"+ curcobro->value("refcobro") +"','"+curcobro->value("comentcobro")+"',1)";

      fact->mainCompany()->runQuery(query);
      
      query = "SELECT max(idasiento) AS id FROM asiento ";
      BlDbRecordSet *cur = fact->mainCompany()->loadQuery(query);
      if (!cur->eof()) {
	  idasiento = cur->value("id");
      } // end if
      delete cur;
  }// end if
  
  /// El apunte por cuenta del cliente.
  query = "SELECT * FROM cliente WHERE idcliente = "+fact->dbValue("idcliente");
  BlDbRecordSet *cur = fact->mainCompany()->loadQuery(query);
  if (!cur->eof()) {
      if (cur->value("idcuenta") == "") {
	 blMsgInfo("No se puede crear el asiento porque el cliente no tiene una cuenta asociada");
	 delete curcobro;
	 delete cur;
	 return 0;
      } // end if
      query = "INSERT INTO borrador(idasiento, fecha, conceptocontable, idcuenta, haber, descripcion) VALUES ("+idasiento+",'"+fecha+"','Cobro. Cliente"+ curcobro->value("refcobro") +"',"+cur->value("idcuenta")+","+curcobro->value("cantcobro").replace(",",".")+",'"+curcobro->value("comentcobro")+"')";

      fact->mainCompany()->runQuery(query);
  } // end if
  delete cur;
 
  
  /// El apunte por cuenta de la forma de pago.
  query = "SELECT * FROM forma_pago WHERE idforma_pago = "+curcobro->value("idforma_pago");
  cur = fact->mainCompany()->loadQuery(query);
  if (!cur->eof()) {
      if (cur->value("idcuenta") == "") {
	 blMsgInfo("No se puede crear el asiento porque la forma de pago no tiene una cuenta asociada");
	 delete curcobro;
	 delete cur;
	 return 0;
      } // end if
    
      query = "INSERT INTO borrador(idasiento, fecha, conceptocontable, idcuenta, debe, descripcion) VALUES ("+idasiento+",'"+fecha+"','Cobro. Cliente"+ curcobro->value("refcobro") +"',"+cur->value("idcuenta")+","+curcobro->value("cantcobro").replace(",",".")+",'"+curcobro->value("comentcobro")+"')";

      fact->mainCompany()->runQuery(query);
  } // end if
  delete cur;
  
  query = "UPDATE cobro SET idasiento= " + idasiento + " WHERE idcobro= "+ fact->dbValue("idcobro");
  fact->mainCompany()->runQuery(query);
  
  
  query = "SELECT cierraasiento("+idasiento+")";
  fact->mainCompany()->runQuery(query);
  
  delete curcobro;
 
  
  return 0;
}


// ================================ FACTURAS DE PROVEEDORES


///
/**
\param l
\return
**/
int FacturaProveedorView_FacturaProveedorView ( FacturaProveedorView *l )
{
    BL_FUNC_DEBUG
    BlToolButton *agalbaran = new BlToolButton ( l );
    agalbaran->setObjectName("mui_asientop");
    
    agalbaran->setStatusTip ( "Mostrar asiento" );
    agalbaran->setToolTip ( "Mostrar asiento" );
    agalbaran->setMinimumSize ( QSize ( 32, 32 ) );
    agalbaran->setMaximumSize ( QSize ( 32, 32 ) );
    agalbaran->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/book.png" ) ) );
    agalbaran->setIconSize ( QSize ( 22, 22 ) );
    agalbaran->setContentsMargins ( 0, 0, 0, 0 );

    QHBoxLayout *m_hboxLayout1 = l->mui_plugbotones->findChild<QHBoxLayout *> ( "hboxLayout1" );

    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QHBoxLayout ( l->mui_plugbotones );
        m_hboxLayout1->setSpacing ( 5 );
        m_hboxLayout1->setMargin ( 0 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    }// end if
    
    m_hboxLayout1->addWidget ( agalbaran );
    
    return 0;
}





int FacturaProveedorView_afterSave_Post(FacturaProveedorView *fact) {
    BL_FUNC_DEBUG
    QString idasiento = "";
    QString fecha = fact->dbValue("ffacturap");
    if (fecha == "") fecha = "now()";
    
    
    QString query = "SELECT idasiento FROM facturap WHERE idfacturap = " + fact->dbValue("idfacturap");
    BlDbRecordSet *cur = fact->mainCompany()->loadQuery(query);
    if (!cur->eof()) {
      if (cur->value("idasiento") != "") {
         query = "DELETE FROM borrador WHERE idasiento = " + cur->value("idasiento");
	 fact->mainCompany()->runQuery(query);
         query = "DELETE FROM apunte WHERE idasiento = " + cur->value("idasiento");
	 fact->mainCompany()->runQuery(query);
	 idasiento = cur->value("idasiento");
      } // end if
    } // end if
    delete cur;

  if (idasiento == "") {
      query = "INSERT INTO asiento (fecha, descripcion, comentariosasiento, clase) VALUES ('"+fecha+"','Fra. Proveedor"+ fact->dbValue("codigoserie_facturap") + fact->dbValue("numfacturap") +"','"+fact->dbValue("descfacturap")+"',1)";

      fact->mainCompany()->runQuery(query);
      
      query = "SELECT max(idasiento) AS id FROM asiento ";
      BlDbRecordSet *cur = fact->mainCompany()->loadQuery(query);
      if (!cur->eof()) {
	  idasiento = cur->value("id");
      } // end if
      delete cur;
   
  }// end if
  
  
  /// El apunte por cuenta del proveedor.
  query = "SELECT * FROM proveedor WHERE idproveedor = "+fact->dbValue("idproveedor");
  cur = fact->mainCompany()->loadQuery(query);
  if (!cur->eof()) {
       if (cur->value("idcuenta") == "") {
	 blMsgInfo("No se puede crear el asiento porque el proveedor no tiene cuenta asociada");
	 delete cur;
	 return 0;
       } // end if
    
      query = "INSERT INTO borrador(idasiento, fecha, conceptocontable, idcuenta, haber, descripcion) VALUES ("+idasiento+",'"+fecha+"','Fra. Proveedor " + fact->dbValue("numfacturap") +"',"+cur->value("idcuenta")+","+fact->m_totalfacturap->text().replace(",",".")+",'"+fact->dbValue("descfacturap")+"')";
      fact->mainCompany()->runQuery(query);
  } // end if
  delete cur;
  
  
  query = "SELECT id_cuenta('47300001') AS id";
  cur = fact->mainCompany()->loadQuery(query);
  if (cur->value("id") == "0") {
       blMsgInfo("No se puede crear el asiento porque no existe la cuenta de IRPF 47300001");
       delete cur;
       return 0;
  } // end if
  delete cur;
  
  
  // El apunte por el irpf
  query = "INSERT INTO borrador(idasiento, fecha, conceptocontable, idcuenta, haber, descripcion) VALUES ("+idasiento+",'"+fecha+"','Fra. Proveedor "+ fact->dbValue("numfacturap") +"',id_cuenta('47300001'),"+fact->m_totalIRPF->text().replace(",",".")+",'"+fact->dbValue("descfacturap")+"')";

  fact->mainCompany()->runQuery(query);
  
  
  /// Los apuntes por cuenta del IVA
  query = "SELECT ivalfacturap::INTEGER, SUM(cantlfacturap*pvplfacturap*(1-descuentolfacturap/100)*ivalfacturap/100)::NUMERIC(12,2) AS subbase FROM lfacturap WHERE idfacturap = "+fact->dbValue("idfacturap")+"  GROUP BY ivalfacturap" ;
  cur = fact->mainCompany()->loadQuery(query);
  while (!cur->eof()) {
      QString codint = "477";
      // PONER UN PARAMETRO APROPIADO
      while ( codint.length() <  g_confpr->value(CONF_CONT_NUMDIGITOSEMPRESA).toInt() - cur->value("ivalfacturap").length() ) {
	  codint = codint + "0";
      } // end while
      codint = codint + cur->value("ivalfacturap");	
      
      
      query = "SELECT id_cuenta('"+codint+"') AS id";
      BlDbRecordSet *cur1 = fact->mainCompany()->loadQuery(query);
      if (cur1->value("id") == "0") {
	  blMsgInfo("No se puede crear el asiento porque no existe la cuenta de IVA "+ codint);
	  delete cur1;
	  delete cur;
	  return 0;
      } // end if
      delete cur1;
      
      query = "INSERT INTO borrador(idasiento, fecha, conceptocontable, idcuenta, debe, descripcion) VALUES ("+idasiento+",'"+fecha+"','Fra. Proveedor"+ fact->dbValue("codigoserie_facturap") + fact->dbValue("numfacturap") +"',id_cuenta('"+codint+"'),"+cur->value("subbase")+",'"+fact->dbValue("descfacturap")+"')";

      fact->mainCompany()->runQuery(query);
      cur->nextRecord();
  } // end while
  delete cur;
  
  
  
  
  
  /// Los apuntes por servicio u obra.
  
  
  
  query = "SELECT idcuentav, SUM(cantlfacturap*pvplfacturap*(1-descuentolfacturap/100))::NUMERIC(12,2) AS subbase FROM lfacturap " \
  "LEFT JOIN articulo AS t1 ON t1.idarticulo = lfacturap.idarticulo " \
  "LEFT JOIN familia ON t1.idfamilia = familia.idfamilia WHERE idfacturap = "+fact->dbValue("idfacturap")+"  GROUP BY idcuentav" ;

  cur = fact->mainCompany()->loadQuery(query);
  while (!cur->eof()) {	  
    
    
    
    
      if (cur->value("idcuentav") == "") {
	blMsgInfo("No se pudo crear el asiento porque hay una familia sin cuenta asociada de compra o de suministro");
	delete cur;
	return 0;
      } // end if
    
    
      query = "INSERT INTO borrador(idasiento, fecha, conceptocontable, idcuenta, debe, descripcion) VALUES ("+idasiento+",'"+fecha+"','Fra. Proveedor "+ fact->dbValue("numfacturap") +"',"+cur->value("idcuentav")+","+cur->value("subbase")+",'"+fact->dbValue("descfacturap")+"')";

      fact->mainCompany()->runQuery(query);
      cur->nextRecord();
  } // end while
  delete cur;
  
  query = "UPDATE facturap SET idasiento= " + idasiento + " WHERE idfacturap= "+ fact->dbValue("idfacturap");
  fact->mainCompany()->runQuery(query);
  
  
  query = "SELECT cierraasiento("+idasiento+")";
  fact->mainCompany()->runQuery(query);
  
  return 0;
}




/// ===================================== LOS PAGOS A PROVEEDORES


///
/**
\param l
\return
**/
int PagoView_PagoView ( PagoView *l )
{
    BL_FUNC_DEBUG
    BlToolButton *agalbaran = new BlToolButton ( l );
    agalbaran->setObjectName("mui_asientopa");
    
    agalbaran->setStatusTip ( "Mostrar asiento" );
    agalbaran->setToolTip ( "Mostrar asiento" );
    agalbaran->setMinimumSize ( QSize ( 32, 32 ) );
    agalbaran->setMaximumSize ( QSize ( 32, 32 ) );
    agalbaran->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/book.png" ) ) );
    agalbaran->setIconSize ( QSize ( 22, 22 ) );
    agalbaran->setContentsMargins ( 0, 0, 0, 0 );
    
    
    QHBoxLayout *m_hboxLayout1 = l->mui_plugbotones->findChild<QHBoxLayout *> ( "hboxLayout1" );

    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QHBoxLayout ( l->mui_plugbotones );
        m_hboxLayout1->setSpacing ( 5 );
        m_hboxLayout1->setMargin ( 0 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    }// end if
    
    m_hboxLayout1->addWidget ( agalbaran );
    
    return 0;
}





int PagoView_afterSave_Post(PagoView *fact) {

    QString idasiento = "";

  
    QString query = "SELECT * FROM pago WHERE idpago = " + fact->dbValue("idpago");
    BlDbRecordSet *curpago = fact->mainCompany()->loadQuery(query);
    if (!curpago->eof()) {
      if (curpago->value("idasiento") != "") {
         query = "DELETE FROM borrador WHERE idasiento = " + curpago->value("idasiento");
	 fact->mainCompany()->runQuery(query);
         query = "DELETE FROM apunte WHERE idasiento = " + curpago->value("idasiento");
	 fact->mainCompany()->runQuery(query);
	 idasiento = curpago->value("idasiento");
      } // end if
    } // end if
  

  if (idasiento == "") {
      query = "INSERT INTO asiento (fecha, descripcion, comentariosasiento, clase) VALUES ('"+curpago->value("fechapago")+"','Pago. Proveedor"+ curpago->value("refpago") +"','"+curpago->value("comentpago")+"',1)";

      fact->mainCompany()->runQuery(query);
      
      query = "SELECT max(idasiento) AS id FROM asiento ";
      BlDbRecordSet *cur = fact->mainCompany()->loadQuery(query);
      if (!cur->eof()) {
	  idasiento = cur->value("id");
      } // end if
      delete cur;
  }// end if
  
  /// El apunte por cuenta del cliente.
  query = "SELECT * FROM proveedor WHERE idproveedor = "+fact->dbValue("idproveedor");
  BlDbRecordSet *cur = fact->mainCompany()->loadQuery(query);
  if (!cur->eof()) {
    
          if (cur->value("idcuenta") == "") {
	 blMsgInfo("No se puede crear el asiento porque el proveedor no tiene una cuenta asociada");
	 delete curpago;
	 delete cur;
	 return 0;
      } // end if
      
      
      query = "INSERT INTO borrador(idasiento, fecha, conceptocontable, idcuenta, haber, descripcion) VALUES ("+idasiento+",'"+curpago->value("fechapago")+"','Pago. Proveedor"+ curpago->value("refpago") +"',"+cur->value("idcuenta")+","+curpago->value("cantpago").replace(",",".")+",'"+curpago->value("comentpago")+"')";

      fact->mainCompany()->runQuery(query);
  } // end if
  delete cur;
 
  
  /// El apunte por cuenta de la forma de pago.
  query = "SELECT * FROM forma_pago WHERE idforma_pago = "+curpago->value("idforma_pago");
  cur = fact->mainCompany()->loadQuery(query);
  if (!cur->eof()) {
    
      if (cur->value("idcuenta") == "") {
	 blMsgInfo("No se puede crear el asiento porque la forma de pago no tiene una cuenta asociada");
	 delete curpago;
	 delete cur;
	 return 0;
      } // end if
      
      
      query = "INSERT INTO borrador(idasiento, fecha, conceptocontable, idcuenta, debe, descripcion) VALUES ("+idasiento+",'"+curpago->value("fechapago")+"','Pago. Proveedor"+ curpago->value("refpago") +"',"+cur->value("idcuenta")+","+curpago->value("cantpago").replace(",",".")+",'"+curpago->value("comentpago")+"')";

      fact->mainCompany()->runQuery(query);
  } // end if
  delete cur;
  
  query = "UPDATE pago SET idasiento= " + idasiento + " WHERE idpago= "+ fact->dbValue("idpago");
  fact->mainCompany()->runQuery(query);
  
  
  query = "SELECT cierraasiento("+idasiento+")";
  fact->mainCompany()->runQuery(query);
  
  delete curpago;
 
  
  return 0;
}




