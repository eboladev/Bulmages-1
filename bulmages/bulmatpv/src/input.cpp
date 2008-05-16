

#include "input.h"
#include "empresatpv.h"
#include "plugins.h"


Input::Input ( EmpresaTPV *emp )
{
    /// Establece valores iniciales.
    setText ( "" );
    setMaxLength ( 13 );
    m_empresaTPV = emp;
}


Input::~Input()
{}


void Input::pulsaTecla ( int tecla, const QString &texto )
{

    event ( new QKeyEvent ( QEvent::KeyPress, tecla, Qt::NoModifier, texto ) );
}


void Input::keyPressEvent ( QKeyEvent *e )
{

    switch ( e->key() ) {
    case Qt::Key_F1:
        m_empresaTPV->cobrar();
        break;
    case Qt::Key_F2:
        m_empresaTPV->ticketActual() ->imprimir();
        break;
    case Qt::Key_F3:
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
    case Qt::Key_C:
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
    case Qt::Key_M:
        setText ( "MESA " );
        break;
    default:
        QLineEdit::keyPressEvent ( e );
        break;
    } // end switch

    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "Input_keyPressEvent_Post", this );

}


QString Input::valorInput()
{
    return text();
}


void Input::setValorInput ( QString val )
{
    setText ( val );
}

