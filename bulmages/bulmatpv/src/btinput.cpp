#include "btinput.h"
#include "btcompany.h"
#include "blplugins.h"


BtInput::BtInput ( BtCompany *emp )
{
    /// Establece valores iniciales.
    setText ( "" );
    setMaxLength ( 13 );
    m_company = emp;
}

BtInput::~BtInput()
{}

void BtInput::pulsaTecla ( int tecla, const QString &texto )
{
    event ( new QKeyEvent ( QEvent::KeyPress, tecla, Qt::NoModifier, texto ) );
}

void BtInput::keyPressEvent ( QKeyEvent *e )
{
    switch ( e->key() ) {
        case Qt::Key_F3:
            setText ( g_confpr->value(CONF_TPV_TEXTOMESA) );
            break;
        case Qt::Key_F4:
            // Limpia el display.
            setText ( "" );
            break;
        case Qt::Key_F5:
            m_company->cobrar();
            break;
        case Qt::Key_F6:
            m_company->ticketActual() ->imprimir();
            break;
        case Qt::Key_F7:
            m_company->ticketActual() ->abrircajon();
            break;
        case Qt::Key_Up:
            m_company->ticketActual() ->subir();
            break;
        case Qt::Key_Down:
            m_company->ticketActual() ->bajar();
            break;
        case Qt::Key_Plus:
            m_company->ticketActual() ->agregarCantidad ( text() );
            setText ( "" );
            break;
        case Qt::Key_Minus:
            m_company->ticketActual() ->agregarCantidad ( "-" + text() );
            setText ( "" );
            break;
        case Qt::Key_Asterisk:
            m_company->ticketActual() ->ponerCantidad ( text() );
            setText ( "" );
            break;
        case Qt::Key_Slash:
            m_company->ticketActual() ->ponerPrecio ( text() );
            setText ( "" );
            break;
        case Qt::Key_Return:
            m_company->ticketActual() ->insertarArticuloCodigo ( text() );
            setText ( "" );
            break;
        case Qt::Key_Enter:
            m_company->ticketActual() ->insertarArticuloCodigoNL ( text() );
            setText ( "" );
            break;
        default:
            QLineEdit::keyPressEvent ( e );
            break;
    } // end switch

    /// Disparamos los plugins.
    g_plugins->run ( "BtInput_keyPressEvent_Post", this );

}

QString BtInput::valorBtInput()
{
    return text();
}

void BtInput::setValorBtInput ( QString val )
{
    setText ( val );
}
