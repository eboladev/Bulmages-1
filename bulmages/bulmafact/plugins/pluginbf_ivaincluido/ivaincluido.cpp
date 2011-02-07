#include "ivaincluido.h"
#include "blfunctions.h"


IVAincluido::IVAincluido(ArticuloView *trab)
{
    m_trab = trab;
    trab->addDbField ( "pvpivaincarticulo", BlDbField::DbNumeric, BlDbField::DbNothing, _ ( "Precio de coste" ) );


    QHBoxLayout *hboxLayout160 = new QHBoxLayout();
    hboxLayout160->setSpacing ( 2 );
    hboxLayout160->setMargin ( 0 );
    hboxLayout160->setObjectName ( QString::fromUtf8 ( "hboxLayout19" ) );

    QLabel *textLabel2_9_26 = new QLabel ( trab->m_frameplugin );
    textLabel2_9_26->setObjectName ( QString::fromUtf8 ( "textLabel2_9_9" ) );
    hboxLayout160->addWidget ( textLabel2_9_26 );
    textLabel2_9_26->setText ( _("PVP Iva Inc.") );
    
    m_ivainc = new BlDoubleSpinBox ( trab->m_frameplugin );
    m_ivainc->setValue ( 0.0 );
    m_ivainc->setMinimum( -10000000 );
    m_ivainc->setMaximum(  10000000 );
    m_ivainc->setObjectName ( QString::fromUtf8 ( "mui_pvpivaincarticulo" ) );
    hboxLayout160->addWidget ( m_ivainc );

    QPushButton *botonTranspasa = new QPushButton ( trab->m_frameplugin );
    botonTranspasa->setObjectName ( QString::fromUtf8 ( "botonTranspasa" ) );
    hboxLayout160->addWidget ( botonTranspasa );
    botonTranspasa->setText ( _("Calc. precio s.IVA") );
    botonTranspasa->setMaximumWidth(120);
    
    connect (botonTranspasa, SIGNAL(clicked()), this, SLOT(botonTranspasa_clicked()) );

    /// Comprobamos que exista el layout.
    QVBoxLayout *m_hboxLayout1 = trab->m_frameplugin->findChild<QVBoxLayout *> ( "hboxLayout1" );
    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QVBoxLayout ( trab->m_frameplugin );
        m_hboxLayout1->setSpacing ( 0 );
        m_hboxLayout1->setMargin ( 0 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    } // end if
    m_hboxLayout1->addLayout ( hboxLayout160 );

}


IVAincluido::~IVAincluido()
{
}



void IVAincluido::botonTranspasa_clicked()
{

    QString SQlQuery = "SELECT * FROM tasa_iva WHERE idtipo_iva = '" + m_trab->mui_idtipo_iva->id() + "'";
    BlDbRecordSet *cur = m_trab->mainCompany() ->loadQuery ( SQlQuery );
   
    if ( !cur->eof() ) {
	m_trab->mui_pvparticulo->setValue( m_ivainc->value() / (1 + (cur->valor ( "porcentasa_iva" ).toDouble() / 100)) );
    } else {
	m_trab->mui_pvparticulo->setValue( m_ivainc->value() );
    } // end if

    delete cur;
  
}
