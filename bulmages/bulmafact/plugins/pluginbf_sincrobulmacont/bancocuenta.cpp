#include <QWidget>

#include "bancocuenta.h"


///
/**
\param comp
\param parent
**/
BancoCuenta::BancoCuenta (QWidget *parent) : QWidget(parent)
{

    blDebug ( "BancoCuenta::BancoCuenta", 1 );

    m_bancoview = (BancoView *) parent;

    connect ( parent->findChild<QWidget *>("mui_lista"), SIGNAL(currentItemChanged ( QListWidgetItem *, QListWidgetItem * )), this, SLOT( mui_lista_currentItemChanged ( QListWidgetItem *, QListWidgetItem * ) ) );

    QWidget *frame = parent->findChild<QWidget *>("mui_plugin_frame");

    QLineEdit *lineedit_banco = new QLineEdit();
    lineedit_banco->setObjectName("mui_cuenta_banco");
    lineedit_banco->setFixedWidth(100);
    lineedit_banco->setStatusTip("Sobreescribe la configuracion por defecto para la cuenta contable de este banco. La cuenta debe existir en la contabilidad.");
    lineedit_banco->setEnabled(FALSE);
    QLabel *label_banco = new QLabel("Cuenta contable:");

    QHBoxLayout *layout_banco = new QHBoxLayout;
    layout_banco->addWidget(label_banco);
    layout_banco->addWidget(lineedit_banco);


    frame->setLayout(layout_banco);
    frame->setMinimumHeight(70);
    
    m_bancoview->resize ( m_bancoview->width(), m_bancoview->height () + 70);
    
    blDebug ( "END BancoCuenta::BancoCuenta", 1 );

}


///
/**
**/
BancoCuenta::~BancoCuenta()
{
    blDebug ( "BancoCuenta::~BancoCuenta", 0 );
    blDebug ( "END BancoCuenta::~BancoCuenta", 0 );
}



void BancoCuenta::mui_lista_currentItemChanged ( QListWidgetItem *current, QListWidgetItem *previous )
{

    BlDbRecordSet *rec;

    if ( m_bancoview->idBanco().isEmpty() ) {
      m_bancoview->findChild<QWidget *>("mui_cuenta_banco")->setEnabled(FALSE);
    } else {
      m_bancoview->findChild<QWidget *>("mui_cuenta_banco")->setEnabled(TRUE);
      
      QString query = "SELECT prefcuentabanco FROM banco WHERE idbanco = '" + m_bancoview->idBanco() + "' LIMIT 1";

      m_bancoview->mainCompany()->begin();
      
      rec = m_bancoview->mainCompany()->loadQuery(query);
      
      m_bancoview->findChild<QLineEdit *>("mui_cuenta_banco")->setText( rec->valor("prefcuentabanco") );
      
      m_bancoview->mainCompany()->commit();
      
    } // end if
    m_bancoview->dialogChanges_cargaInicial();

}



