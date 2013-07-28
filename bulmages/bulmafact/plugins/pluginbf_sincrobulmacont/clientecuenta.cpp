#include <QtWidgets/QWidget>

#include "clientecuenta.h"


///
/**
\param comp
\param parent
**/
ClienteCuenta::ClienteCuenta (QWidget *parent) : QWidget(parent)
{

    BL_FUNC_DEBUG

    m_clienteview = (ClienteView *) parent;

    QWidget *w = new QWidget();
    m_clienteview->mui_tab->insertTab(1, w, _("Contabilidad"));
        
    QWidget *frame = new QFrame(w);

    QLineEdit *lineedit_cliente = new QLineEdit();
    lineedit_cliente->setObjectName("mui_cuenta_cliente");
    lineedit_cliente->setFixedWidth(100);
    lineedit_cliente->setStatusTip(_("Sobreescribe la configuracion por defecto para la cuenta contable de este cliente. La cuenta debe existir en la contabilidad."));
    QLabel *label_cliente = new QLabel(_("Cuenta contable:"));

    /// Campo para guardar datos de forma temporal.
    QLineEdit *lineedit_tmp_cliente = new QLineEdit();
    lineedit_tmp_cliente->setObjectName("tmp_cuenta_cliente");
    lineedit_tmp_cliente->setText("");
    lineedit_tmp_cliente->hide();

    QHBoxLayout *layout_cliente = new QHBoxLayout;
    layout_cliente->addWidget(label_cliente);
    layout_cliente->addWidget(lineedit_cliente);
    layout_cliente->addWidget(lineedit_tmp_cliente);
    

    frame->setLayout(layout_cliente);
    frame->setMinimumHeight(70);
    
    

}


///
/**
**/
ClienteCuenta::~ClienteCuenta()
{
    BL_FUNC_DEBUG
    
}

