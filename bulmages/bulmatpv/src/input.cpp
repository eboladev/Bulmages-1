

#include "input.h"
#include "empresatpv.h"
#include "plugins.h"

Input::Input(EmpresaTPV *emp) : QObject(NULL) {
    m_valorInput = "";
    m_empresaTPV = emp;
}

Input::~Input() {}

void Input::pulsaTecla(int tecla) {
    QKeyEvent key(QEvent::KeyRelease, tecla, Qt::NoModifier );
    keyReleaseEvent(&key);
}


void Input::keyReleaseEvent ( QKeyEvent * e ) {
    switch (e->key()) {
    case Qt::Key_F1:
        m_empresaTPV->cobrar();
        break;
    case Qt::Key_F2:
	m_empresaTPV->ticketActual()->imprimir();
        break;
    case Qt::Key_Up:
	m_empresaTPV->ticketActual()->subir();
        break;
    case Qt::Key_Down:
	m_empresaTPV->ticketActual()->bajar();
        break;
    case Qt::Key_Plus:
	m_empresaTPV->ticketActual()->agregarCantidad(m_valorInput);
	m_valorInput = "";
	break;
    case Qt::Key_Minus:
	m_empresaTPV->ticketActual()->agregarCantidad("-" + m_valorInput);
	m_valorInput = "";
	break;
    case Qt::Key_Asterisk:
	m_empresaTPV->ticketActual()->ponerCantidad(m_valorInput);
	m_valorInput = "";
	break;
    case Qt::Key_Slash:
	m_empresaTPV->ticketActual()->ponerPrecio(m_valorInput);
	m_valorInput = "";
	break;
    case Qt::Key_Return:
	m_empresaTPV->ticketActual()->insertarArticuloCodigo(m_valorInput);
	m_valorInput = "";
	break;
    case Qt::Key_Enter:
	m_empresaTPV->ticketActual()->insertarArticuloCodigoNL(m_valorInput);
	m_valorInput = "";
	break;
    default:
	m_valorInput += e->key();
        break;
    } // end switch

    /// Disparamos los plugins.
    int res = g_plugins->lanza("Input_keyReleaseEvent_Post", this);
}



