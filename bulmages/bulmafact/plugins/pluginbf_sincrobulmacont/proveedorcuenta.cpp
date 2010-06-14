#include <QWidget>

#include "proveedorcuenta.h"


///
/**
\param comp
\param parent
**/
ProveedorCuenta::ProveedorCuenta (QWidget *parent) : QWidget(parent)
{

    _depura ( "ProveedorCuenta::ProveedorCuenta", 1 );

    m_proveedorview = (ProveedorView *) parent;

    QWidget *w = new QWidget();
    m_proveedorview->mui_tab->insertTab(1, w, _("Contabilidad"));
        
    QWidget *frame = new QFrame(w);

    QLineEdit *lineedit_proveedor = new QLineEdit();
    lineedit_proveedor->setObjectName("mui_cuenta_proveedor");
    lineedit_proveedor->setFixedWidth(100);
    lineedit_proveedor->setStatusTip("Sobreescribe la configuracion por defecto para la cuenta contable de este proveedor. La cuenta debe existir en la contabilidad.");
    QLabel *label_proveedor = new QLabel("Cuenta contable:");

    /// Campo para guardar datos de forma temporal.
    QLineEdit *lineedit_tmp_proveedor = new QLineEdit();
    lineedit_tmp_proveedor->setObjectName("tmp_cuenta_proveedor");
    lineedit_tmp_proveedor->setText("");
    lineedit_tmp_proveedor->hide();

    
    QHBoxLayout *layout_proveedor = new QHBoxLayout;
    layout_proveedor->addWidget(label_proveedor);
    layout_proveedor->addWidget(lineedit_proveedor);
    layout_proveedor->addWidget(lineedit_tmp_proveedor);


    frame->setLayout(layout_proveedor);
    frame->setMinimumHeight(70);
    
    
    _depura ( "END ProveedorCuenta::ProveedorCuenta", 1 );

}


///
/**
**/
ProveedorCuenta::~ProveedorCuenta()
{
    _depura ( "ProveedorCuenta::~ProveedorCuenta", 0 );
    _depura ( "END ProveedorCuenta::~ProveedorCuenta", 0 );
}


