#include <QtWidgets/QWidget>

#include "fpagocuenta.h"
#include "bfbuscarbanco.h"


///
/**
\param comp
\param parent
**/
FPagoCuenta::FPagoCuenta (QWidget *parent) : QWidget(parent)
{

    BL_FUNC_DEBUG

    m_fpagoview = (FPagoView *) parent;

    connect ( parent->findChild<QListWidget *>("mui_lista"), SIGNAL(currentItemChanged ( QListWidgetItem *, QListWidgetItem * )), this, SLOT( mui_lista_currentItemChanged ( QListWidgetItem *, QListWidgetItem * ) ) );
    connect ( parent->findChild<BfBuscarBanco *>("mui_idbanco"), SIGNAL(currentIndexChanged ( int )), this, SLOT( mui_idbanco_currentIndexChanged ( int ) ) );

    QWidget *frame = parent->findChild<QWidget *>("mui_plugin_frame");

    QLineEdit *lineedit_fpago = new QLineEdit();
    lineedit_fpago->setObjectName("mui_cuenta_forma_pago");
    lineedit_fpago->setFixedWidth(100);
    lineedit_fpago->setStatusTip(_("Sobreescribe la configuracion por defecto para la cuenta contable de esta forma de pago. La cuenta debe existir en la contabilidad."));
    lineedit_fpago->setEnabled(false);
    QLabel *label_fpago = new QLabel(_("Cuenta contable:"));


    QHBoxLayout *layout_fpago = new QHBoxLayout;
    layout_fpago->addWidget(label_fpago);
    layout_fpago->addWidget(lineedit_fpago);


    frame->setLayout(layout_fpago);


    

}


///
/**
**/
FPagoCuenta::~FPagoCuenta()
{
    BL_FUNC_DEBUG
    
}



void FPagoCuenta::mui_lista_currentItemChanged ( QListWidgetItem *current, QListWidgetItem *previous )
{

    BlDbRecordSet *rec;

    /// Si se ha seleccionado un banco entonces las cuentas contables establecidas en ellos tienen
    /// mas prioridad que las establecidas en las formas de pago.
    if (m_fpagoview->findChild<BfBuscarBanco *>("mui_idbanco")->currentIndex() > 0) {
        m_fpagoview->findChild<QLineEdit *>("mui_cuenta_forma_pago")->setEnabled(false);
        m_fpagoview->findChild<QLineEdit *>("mui_cuenta_forma_pago")->setText("");
	return;
    } else {
        m_fpagoview->findChild<QLineEdit *>("mui_cuenta_forma_pago")->setEnabled(true);
    } // end if

    if ( m_fpagoview->idFormaPago().isEmpty() ) {
      m_fpagoview->findChild<QLineEdit *>("mui_cuenta_forma_pago")->setEnabled(false);
    } else {
      /*
      m_fpagoview->findChild<QLineEdit *>("mui_cuenta_forma_pago")->setEnabled(true);
      
      QString query = "SELECT prefcuentaforma_pago FROM forma_pago WHERE idforma_pago = '" + m_fpagoview->idFormaPago() + "' LIMIT 1";

      m_fpagoview->mainCompany()->begin();
      
      rec = m_fpagoview->mainCompany()->loadQuery(query);
      
      m_fpagoview->findChild<QLineEdit *>("mui_cuenta_forma_pago")->setText( rec->value("prefcuentaforma_pago") );
      
      m_fpagoview->mainCompany()->commit();
      */
        m_fpagoview->mainCompany()->begin();

      
	// 1) coge idcuentaforma_pago de forma_pago.
	// 2) se conecta a contabilidad.
	// 3) busca el codigo cuenta usando el idcuenta de forma_pago.
	// 4) rellena qlineedit con codigo cuenta.
	
	QString query = "SELECT idcuentaforma_pago FROM forma_pago WHERE idforma_pago = '" + m_fpagoview->idFormaPago() + "' LIMIT 1";
	rec = m_fpagoview->mainCompany()->loadQuery(query);
	
	// Si no hay datos en idcuenta no se hace nada.
	if ( rec != NULL ) {

	    m_fpagoview->mainCompany()->run("SELECT conectabulmacont()");
	    
	    QString query = "SELECT codigo FROM bc_cuenta WHERE idcuenta = '" + rec->value("idcuentaforma_pago") + "' LIMIT 1";
	    rec = m_fpagoview->mainCompany()->loadQuery(query);
	    
	    m_fpagoview->findChild<QLineEdit *>("mui_cuenta_forma_pago")->setText( rec->value("codigo") );
	   
	} // end if
    
      
	m_fpagoview->mainCompany()->commit();

    } // end if
    m_fpagoview->dialogChanges_readValues();

}



void FPagoCuenta::mui_idbanco_currentIndexChanged ( int index )
{
    /// Si se ha seleccionado un banco entonces las cuentas contables establecidas en ellos tienen
    /// mas prioridad que las establecidas en las formas de pago.
    if (index > 0) {
        m_fpagoview->findChild<QLineEdit *>("mui_cuenta_forma_pago")->setEnabled(false);
        m_fpagoview->findChild<QLineEdit *>("mui_cuenta_forma_pago")->setText("");
    } else {
        m_fpagoview->findChild<QLineEdit *>("mui_cuenta_forma_pago")->setEnabled(true);
    } // end if
}

