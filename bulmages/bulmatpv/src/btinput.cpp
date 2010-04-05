#include "btinput.h"
#include "btcompany.h"
#include "blplugins.h"


BtInput::BtInput ( BtCompany *emp )
{
    /// Establece valores iniciales.
    setText ( "" );
    setMaxLength ( 13 );
    m_empresaTPV = emp;
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
            setText ( "MESA " );
            break;
        case Qt::Key_F4:
            // Limpia el display.
            setText ( "" );
            break;
        case Qt::Key_F5:
            m_empresaTPV->cobrar();
            break;
        case Qt::Key_F6:
            m_empresaTPV->ticketActual() ->imprimir();
            break;
        case Qt::Key_F7:
            m_empresaTPV->ticketActual() ->abrircajon();
            break;
        case Qt::Key_Up:
            m_empresaTPV->ticketActual() ->subir();
            break;
        case Qt::Key_Down:
            m_empresaTPV->ticketActual() ->bajar();
            break;
        case Qt::Key_Plus:
            m_empresaTPV->ticketActual() ->agregarCantidad ( text() );
            setText ( "" );
            break;
        case Qt::Key_Minus:
            m_empresaTPV->ticketActual() ->agregarCantidad ( "-" + text() );
            setText ( "" );
            break;
        case Qt::Key_Asterisk:
            m_empresaTPV->ticketActual() ->ponerCantidad ( text() );
            setText ( "" );
            break;
        case Qt::Key_Slash:
            m_empresaTPV->ticketActual() ->ponerPrecio ( text() );
            setText ( "" );
            break;
        case Qt::Key_Return:
            m_empresaTPV->ticketActual() ->insertarArticuloCodigo ( text() );
            setText ( "" );
            break;
        case Qt::Key_Enter:
            m_empresaTPV->ticketActual() ->insertarArticuloCodigoNL ( text() );
            setText ( "" );
            break;
        default:
            QLineEdit::keyPressEvent ( e );
            break;
    } // end switch

    /// Disparamos los plugins.
    g_plugins->lanza ( "BtInput_keyPressEvent_Post", this );

}

QString BtInput::valorBtInput()
{
    return text();
}

void BtInput::setValorBtInput ( QString val )
{
    setText ( val );
}
