#include <QtWidgets/QWidget>

#include "bancocuenta.h"


///
/**
\param comp
\param parent
**/
BancoCuenta::BancoCuenta (QWidget *parent) : QWidget(parent)
{

    BL_FUNC_DEBUG
    BlDbRecordSet *rec;

    m_bancoview = (BancoView *) parent;

    QWidget *frame = parent->findChild<QWidget *>("mui_plugin_frame");

    QLineEdit *lineedit_banco = new QLineEdit();
    lineedit_banco->setObjectName("mui_cuenta_banco");
    lineedit_banco->setFixedWidth(100);
    lineedit_banco->setStatusTip(_("Sobreescribe la configuracion por defecto para la cuenta contable de este banco. La cuenta debe existir en la contabilidad."));
    lineedit_banco->setEnabled(false);
    QLabel *label_banco = new QLabel(_("Cuenta contable:"));

    QHBoxLayout *layout_banco = new QHBoxLayout;
    layout_banco->addWidget(label_banco);
    layout_banco->addWidget(lineedit_banco);


    frame->setLayout(layout_banco);
    frame->setMinimumHeight(70);
    
    m_bancoview->resize ( m_bancoview->width(), m_bancoview->height () + 70);
    
    connect ( parent->findChild<QWidget *>("mui_lista"), SIGNAL(currentItemChanged ( QListWidgetItem *, QListWidgetItem * )), this, SLOT( mui_lista_currentItemChanged ( QListWidgetItem *, QListWidgetItem * ) ) );
    
    

}


///
/**
**/
BancoCuenta::~BancoCuenta()
{
    BL_FUNC_DEBUG
    
}



void BancoCuenta::mui_lista_currentItemChanged ( QListWidgetItem *current, QListWidgetItem *previous )
{

    BlDbRecordSet *rec;

    if ( m_bancoview->idBanco().isEmpty() ) {
      m_bancoview->findChild<QWidget *>("mui_cuenta_banco")->setEnabled(false);
    } else {
      m_bancoview->findChild<QWidget *>("mui_cuenta_banco")->setEnabled(true);

      /*
      QString query = "SELECT prefcuentabanco FROM banco WHERE idbanco = '" + m_bancoview->idBanco() + "' LIMIT 1";

      m_bancoview->mainCompany()->begin();
      
      rec = m_bancoview->mainCompany()->loadQuery(query);
      
      m_bancoview->findChild<QLineEdit *>("mui_cuenta_banco")->setText( rec->value("prefcuentabanco") );
      
      m_bancoview->mainCompany()->commit();
      */

        m_bancoview->mainCompany()->begin();

      
	// 1) coge idcuentabanco de banco.
	// 2) se conecta a contabilidad.
	// 3) busca el codigo cuenta usando el idcuenta de banco.
	// 4) rellena qlineedit con codigo cuenta.
	
	QString query = "SELECT idcuentabanco FROM banco WHERE idbanco = '" + m_bancoview->idBanco() + "' LIMIT 1";
	rec = m_bancoview->mainCompany()->loadQuery(query);
	
	// Si no hay datos en idcuenta no se hace nada.
	if ( rec != NULL ) {

	    m_bancoview->mainCompany()->run("SELECT conectabulmacont()");
	    
	    QString query = "SELECT codigo FROM bc_cuenta WHERE idcuenta = '" + rec->value("idcuentabanco") + "' LIMIT 1";
	    rec = m_bancoview->mainCompany()->loadQuery(query);
	    
	    m_bancoview->findChild<QLineEdit *>("mui_cuenta_banco")->setText( rec->value("codigo") );
	   
	} // end if
    
      
	m_bancoview->mainCompany()->commit();
      
    } // end if
    m_bancoview->dialogChanges_readValues();

}

