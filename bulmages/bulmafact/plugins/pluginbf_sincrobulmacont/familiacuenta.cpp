#include <QWidget>
#include "familiacuenta.h"


///
/**
\param comp
\param parent
**/
FamiliaCuenta::FamiliaCuenta (QWidget *parent) : QWidget(parent)
{
    blDebug ( "FamiliaCuenta::FamiliaCuenta", 1 );

    m_familiasview = (FamiliasView *) parent;
    
    connect ( parent->findChild<QWidget *>("m_listFamilias"), SIGNAL(currentItemChanged ( QTreeWidgetItem *, QTreeWidgetItem * )), this, SLOT( on_m_listFamilias_currentItemChanged ( QTreeWidgetItem *, QTreeWidgetItem * ) ) );

    QWidget *frame = parent->findChild<QWidget *>("mui_contable_frame");
    
    QLineEdit *lineedit_venta = new QLineEdit();
    lineedit_venta->setObjectName("mui_cuenta_venta");
    lineedit_venta->setFixedWidth(100);
    lineedit_venta->setStatusTip("Sobreescribe la configuracion por defecto para la cuenta contable de esta familia. La cuenta debe existir en la contabilidad.");
    lineedit_venta->setEnabled(FALSE);
    QLabel *label_venta = new QLabel("Cuenta venta:");

    QLineEdit *lineedit_compra = new QLineEdit();
    lineedit_compra->setObjectName("mui_cuenta_compra");
    lineedit_compra->setFixedWidth(100);
    lineedit_compra->setStatusTip("Sobreescribe la configuracion por defecto para la cuenta contable de esta familia. La cuenta debe existir en la contabilidad.");
    lineedit_compra->setEnabled(FALSE);
    QLabel *label_compra = new QLabel("Cuenta compra:");


    QHBoxLayout *layout_venta = new QHBoxLayout;
    layout_venta->addWidget(label_venta);
    layout_venta->addWidget(lineedit_venta);

    QHBoxLayout *layout_compra = new QHBoxLayout;
    layout_compra->addWidget(label_compra);
    layout_compra->addWidget(lineedit_compra);


    QVBoxLayout *layout_cuenta = new QVBoxLayout;
    layout_cuenta->addLayout(layout_venta);
    layout_cuenta->addLayout(layout_compra);


    frame->setLayout(layout_cuenta);

    
    blDebug ( "END FamiliaCuenta::FamiliaCuenta", 1 );
}


///
/**
**/
FamiliaCuenta::~FamiliaCuenta()
{
    blDebug ( "FamiliaCuenta::~FamiliaCuenta", 0 );
    blDebug ( "END FamiliaCuenta::~FamiliaCuenta", 0 );
}



void FamiliaCuenta::on_m_listFamilias_currentItemChanged ( QTreeWidgetItem *current, QTreeWidgetItem *previous )
{
    BlDbRecordSet *rec;

    if ( m_familiasview->idFamilia().isEmpty() ) {
      m_familiasview->findChild<QWidget *>("mui_cuenta_venta")->setEnabled(FALSE);
      m_familiasview->findChild<QWidget *>("mui_cuenta_compra")->setEnabled(FALSE);
    } else {
      m_familiasview->findChild<QWidget *>("mui_cuenta_venta")->setEnabled(TRUE);
      m_familiasview->findChild<QWidget *>("mui_cuenta_compra")->setEnabled(TRUE);
      
      QString query = "SELECT prefcuentaventafamilia, prefcuentacomprafamilia FROM familia WHERE idfamilia = '" + m_familiasview->idFamilia() + "' LIMIT 1";

      m_familiasview->mainCompany()->begin();
      
      rec = m_familiasview->mainCompany()->loadQuery(query);
      
      m_familiasview->findChild<QLineEdit *>("mui_cuenta_venta")->setText( rec->valor("prefcuentaventafamilia") );
      m_familiasview->findChild<QLineEdit *>("mui_cuenta_compra")->setText( rec->valor("prefcuentacomprafamilia") );
      
      m_familiasview->mainCompany()->commit();
      
    } // end if
    m_familiasview->dialogChanges_cargaInicial();
    
}

