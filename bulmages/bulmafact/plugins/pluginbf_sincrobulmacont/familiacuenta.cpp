#include <QtWidgets/QWidget>
#include "familiacuenta.h"


///
/**
\param comp
\param parent
**/
FamiliaCuenta::FamiliaCuenta (QWidget *parent) : QWidget(parent)
{
    BL_FUNC_DEBUG

    m_familiasview = (FamiliasView *) parent;
    
    connect ( parent->findChild<QWidget *>("m_listFamilias"), SIGNAL(currentItemChanged ( QTreeWidgetItem *, QTreeWidgetItem * )), this, SLOT( on_m_listFamilias_currentItemChanged ( QTreeWidgetItem *, QTreeWidgetItem * ) ) );

    QWidget *frame = parent->findChild<QWidget *>("mui_contable_frame");
    
    QLineEdit *lineedit_venta = new QLineEdit();
    lineedit_venta->setObjectName("mui_cuenta_venta");
    lineedit_venta->setFixedWidth(100);
    lineedit_venta->setStatusTip(_("Sobreescribe la configuracion por defecto para la cuenta contable de esta familia. La cuenta debe existir en la contabilidad."));
    lineedit_venta->setEnabled(false);
    QLabel *label_venta = new QLabel(_("Cuenta venta:"));

    QLineEdit *lineedit_compra = new QLineEdit();
    lineedit_compra->setObjectName("mui_cuenta_compra");
    lineedit_compra->setFixedWidth(100);
    lineedit_compra->setStatusTip(_("Sobreescribe la configuracion por defecto para la cuenta contable de esta familia. La cuenta debe existir en la contabilidad."));
    lineedit_compra->setEnabled(false);
    QLabel *label_compra = new QLabel(_("Cuenta compra:"));


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

    
    
}


///
/**
**/
FamiliaCuenta::~FamiliaCuenta()
{
    BL_FUNC_DEBUG
    
}



void FamiliaCuenta::on_m_listFamilias_currentItemChanged ( QTreeWidgetItem *current, QTreeWidgetItem *previous )
{
    BlDbRecordSet *rec;
    BlDbRecordSet *rec2;

    if ( m_familiasview->idFamilia().isEmpty() ) {
      m_familiasview->findChild<QWidget *>("mui_cuenta_venta")->setEnabled(false);
      m_familiasview->findChild<QWidget *>("mui_cuenta_compra")->setEnabled(false);
    } else {
      m_familiasview->findChild<QWidget *>("mui_cuenta_venta")->setEnabled(true);
      m_familiasview->findChild<QWidget *>("mui_cuenta_compra")->setEnabled(true);
/*      
      QString query = "SELECT prefcuentaventafamilia, prefcuentacomprafamilia FROM familia WHERE idfamilia = '" + m_familiasview->idFamilia() + "' LIMIT 1";

      m_familiasview->mainCompany()->begin();
      
      rec = m_familiasview->mainCompany()->loadQuery(query);
      
      m_familiasview->findChild<QLineEdit *>("mui_cuenta_venta")->setText( rec->value("prefcuentaventafamilia") );
      m_familiasview->findChild<QLineEdit *>("mui_cuenta_compra")->setText( rec->value("prefcuentacomprafamilia") );
      
      m_familiasview->mainCompany()->commit();
*/

        m_familiasview->mainCompany()->begin();

      
	// 1) coge iidcuentaventafamilia, idcuentacomprafamilia de familia.
	// 2) se conecta a contabilidad.
	// 3) busca el codigo cuenta usando el idcuenta de la familia.
	// 4) rellena qlineedit con codigo cuenta.
	QString query;

	query = "SELECT idcuentaventafamilia, idcuentacomprafamilia FROM familia WHERE idfamilia = '" + m_familiasview->idFamilia() + "' LIMIT 1";
	rec = m_familiasview->mainCompany()->loadQuery(query);
	
	// Si no hay datos en idcuenta no se hace nada.
	if ( rec != NULL ) {

	    m_familiasview->mainCompany()->run("SELECT conectabulmacont()");
	    
	    query = "SELECT codigo FROM bc_cuenta WHERE idcuenta = '" + rec->value("idcuentaventafamilia") + "' LIMIT 1";
	    rec2 = m_familiasview->mainCompany()->loadQuery(query);
	    
	    m_familiasview->findChild<QLineEdit *>("mui_cuenta_venta")->setText( rec2->value("codigo") );

	    query = "SELECT codigo FROM bc_cuenta WHERE idcuenta = '" + rec->value("idcuentacomprafamilia") + "' LIMIT 1";
	    rec2 = m_familiasview->mainCompany()->loadQuery(query);

	    m_familiasview->findChild<QLineEdit *>("mui_cuenta_compra")->setText( rec2->value("codigo") );
	    
	} // end if
    
      
	m_familiasview->mainCompany()->commit();

    } // end if
    m_familiasview->dialogChanges_readValues();
    
}

