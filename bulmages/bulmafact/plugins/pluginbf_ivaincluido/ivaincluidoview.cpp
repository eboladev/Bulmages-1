#include "ivaincluidoview.h"
#include "blfunctions.h"


IVAincluidoView::IVAincluidoView(ArticuloView *trab)
{
    m_trab = trab;
    trab->addDbField ( "pvpivaincarticulo", BlDbField::DbNumeric, BlDbField::DbNothing, _ ( "Precio de coste" ) );

    /// Comprobamos que exista el layout.
    QVBoxLayout *m_hboxLayout1 = trab->m_frameplugin->findChild<QVBoxLayout *> ( "hboxLayout1" );
    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QVBoxLayout ( trab->m_frameplugin );
        m_hboxLayout1->setSpacing ( 0 );
        m_hboxLayout1->setMargin ( 0 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    } // end if

    setupUi(this);

    m_hboxLayout1->addWidget ( this );

}


IVAincluidoView::~IVAincluidoView()
{
}



/* Calcula el precio sin IVA a partir del PVP IVA incluido.
*/
void IVAincluidoView::on_mui_calcula_sin_iva_clicked()
{

    QString SQlQuery = "SELECT * FROM tasa_iva WHERE idtipo_iva = '" + m_trab->mui_idtipo_iva->id() + "' ORDER BY fechatasa_iva DESC LIMIT 1";
    BlDbRecordSet *cur = m_trab->mainCompany() ->loadQuery ( SQlQuery );
   
    if ( !cur->eof() ) {
	m_trab->mui_pvparticulo->setValue( mui_pvpivaincarticulo->value() / (1 + (cur->value( "porcentasa_iva" ).toDouble() / 100)) );
    } else {
	m_trab->mui_pvparticulo->setValue( mui_pvpivaincarticulo->value() );
    } // end if

    delete cur;
  
}


/* Calcula el PVP IVA incluido a partir del precio sin IVA.
*/
void IVAincluidoView::on_mui_calcula_iva_inc_clicked()
{
    QString SQlQuery = "SELECT * FROM tasa_iva WHERE idtipo_iva = '" + m_trab->mui_idtipo_iva->id() + "' ORDER BY fechatasa_iva DESC LIMIT 1";
    BlDbRecordSet *cur = m_trab->mainCompany() ->loadQuery ( SQlQuery );

    if ( !cur->eof() ) {
        mui_pvpivaincarticulo->setValue( m_trab->mui_pvparticulo->value() * (1 + (cur->value( "porcentasa_iva" ).toDouble() / 100)) );
    } else {
        mui_pvpivaincarticulo->setValue( m_trab->mui_pvparticulo->value() );
    } // end if

    delete cur;

}

