#include <QWidget>
#include <QPushButton>
#include <QKeyEvent>

#include "modificadores.h"
#include "blfunctions.h"
#include "btticket.h"
#include "btcompany.h"


Modificadores::Modificadores ( BlMainCompany *emp, QWidget *parent, bool editFieldToModify, QString fieldToModify ) : QDialog ( parent ), BlMainCompanyPointer ( emp )
{
    BL_FUNC_DEBUG
    
    
    setupUi ( this );

    if (editFieldToModify) {
      
	m_fieldToModify = fieldToModify;
	mui_tab->setCurrentWidget(tab_texto);
	
	mui_tab->removeTab(mui_tab->indexOf(tab_predefinidos));
	mui_tab->removeTab(mui_tab->indexOf(tab_grafico));
      
    } else {
    
	bool hayModificadores = FALSE;
	QString idarticulo = ((BtCompany *) emp)->ticketActual()->lineaActBtTicket()->dbValue ( "idarticulo");
	
	BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT * FROM modificador WHERE idarticulo = " + idarticulo );
	while ( !cur->eof() ) {
	    hayModificadores = TRUE;
	    QPushButton * toolbutton = new QPushButton ( mui_frame );
	    toolbutton->setText ( cur->value( "nombremodificador" ) );

	    QVBoxLayout *m_hboxLayout1 = mui_frame->findChild<QVBoxLayout *> ( "hboxLayout1" );
	    if ( !m_hboxLayout1 ) {
		m_hboxLayout1 = new QVBoxLayout ( mui_frame );
		m_hboxLayout1->setSpacing ( 5 );
		m_hboxLayout1->setMargin ( 5 );
		m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
	    } // end if
	    m_hboxLayout1->addWidget ( toolbutton );
	    connect ( toolbutton, SIGNAL ( pressed() ), this, SLOT ( modificadorClicked() ) );
	    cur->nextRecord();
	} // end while
	delete cur;

	m_fieldToModify = "textolibremodificador";
//	mui_texto->setPlainText( ((BtCompany *) emp)->ticketActual()->lineaActBtTicket()->dbValue ( m_fieldToModify ) );
	
	if (!hayModificadores) {
	    /// Si no hay modificadores predeterminados. Se situa el 'tab' en escritura texto.
	    mui_tab->setCurrentWidget(tab_texto);
	    done(0);
	} else {
	    mui_tab->setCurrentWidget(tab_predefinidos);
	} // end if

//	mui_texto->ensureCursorVisible();
//	mui_texto->setFocus(Qt::MouseFocusReason);
	
    } // end if
    
    mui_texto->setPlainText( ((BtCompany *) emp)->ticketActual()->lineaActBtTicket()->dbValue ( m_fieldToModify ) ); 
    
    mui_texto->ensureCursorVisible();
    mui_texto->setFocus(Qt::MouseFocusReason);
}


Modificadores::~Modificadores()
{}


void Modificadores::modificadorClicked()
{
    BL_FUNC_DEBUG

    BtCompany * emp1 = ( BtCompany * ) mainCompany();
    BtTicket *ticket = NULL;
    BtTicket *ticketv = NULL;
    bool encontrado = FALSE;

    QString idarticulo = emp1->ticketActual()->lineaActBtTicket()->dbValue ( "idarticulo");

    /// Buscamos cual ha sido el modificador pulsado.
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT * FROM modificador WHERE idarticulo = " + idarticulo );
    while ( !encontrado ) {
        if ( ( ( QPushButton * ) sender() ) ->text() == cur->value( "nombremodificador" ) ) {
            encontrado = TRUE;
        } else {
            cur->nextRecord();
        } // end if
    } // end while

    /// Buscamos un espacio vacio en la tabla para poner el modificador.
    int i = 3;
    while ( (i <= 9) && (emp1->ticketActual()->lineaActBtTicket()->dbValue ( "idmodificador" + QString::number(i)) != "") ) {
      i++;
    } // end while
    if (i <= 9) 
      emp1->ticketActual()->lineaActBtTicket()->setDbValue ( "idmodificador" + QString::number(i), cur->value("idmodificador"));

    delete cur;

    done ( 0 );

}


void Modificadores::sendKey(int tecla, QString texto)
{
    mui_texto->setFocus(Qt::MouseFocusReason);
    QKeyEvent key(QKeyEvent::KeyPress, tecla,  NULL, texto, false, 1 );
    QApplication::sendEvent(mui_texto, &key);
}

void Modificadores::on_mui_tecla_limpiar_clicked()
{
    mui_texto->setFocus(Qt::MouseFocusReason);
    mui_texto->clear();
}

void Modificadores::on_mui_tecla_1_clicked()
{
    sendKey(Qt::Key_1, "1");
}

void Modificadores::on_mui_tecla_2_clicked()
{
    sendKey(Qt::Key_2, "2");
}


void Modificadores::on_mui_tecla_3_clicked()
{
    sendKey(Qt::Key_3, "3");
}

void Modificadores::on_mui_tecla_4_clicked()
{
    sendKey(Qt::Key_4, "4");
}

void Modificadores::on_mui_tecla_5_clicked()
{
    sendKey(Qt::Key_5, "5");
}

void Modificadores::on_mui_tecla_6_clicked()
{
    sendKey(Qt::Key_6, "6");
}

void Modificadores::on_mui_tecla_7_clicked()
{
    sendKey(Qt::Key_7, "7");
}

void Modificadores::on_mui_tecla_8_clicked()
{
    sendKey(Qt::Key_8, "8");
}

void Modificadores::on_mui_tecla_9_clicked()
{
    sendKey(Qt::Key_9, "9");
}

void Modificadores::on_mui_tecla_0_clicked()
{
    sendKey(Qt::Key_0, "0");
}

void Modificadores::on_mui_tecla_borrar_clicked()
{
      sendKey(Qt::Key_Backspace, "Backspace");
}

void Modificadores::on_mui_tecla_q_clicked()
{
      sendKey(Qt::Key_Q, "Q");
}

void Modificadores::on_mui_tecla_w_clicked()
{
      sendKey(Qt::Key_W, "W");
}

void Modificadores::on_mui_tecla_e_clicked()
{
      sendKey(Qt::Key_E, "E");
}

void Modificadores::on_mui_tecla_r_clicked()
{
      sendKey(Qt::Key_R, "R");
}

void Modificadores::on_mui_tecla_t_clicked()
{
      sendKey(Qt::Key_T, "T");
}

void Modificadores::on_mui_tecla_y_clicked()
{
      sendKey(Qt::Key_Y, "Y");
}

void Modificadores::on_mui_tecla_u_clicked()
{
      sendKey(Qt::Key_U, "U");
}

void Modificadores::on_mui_tecla_i_clicked()
{
      sendKey(Qt::Key_I, "I");
}

void Modificadores::on_mui_tecla_o_clicked()
{
      sendKey(Qt::Key_O, "O");
}

void Modificadores::on_mui_tecla_p_clicked()
{
      sendKey(Qt::Key_P, "P");
}

void Modificadores::on_mui_tecla_a_clicked()
{
      sendKey(Qt::Key_A, "A");
}

void Modificadores::on_mui_tecla_s_clicked()
{
      sendKey(Qt::Key_S, "S");
}

void Modificadores::on_mui_tecla_d_clicked()
{
      sendKey(Qt::Key_D, "D");
}

void Modificadores::on_mui_tecla_f_clicked()
{
      sendKey(Qt::Key_F, "F");
}

void Modificadores::on_mui_tecla_g_clicked()
{
      sendKey(Qt::Key_G, "G");
}

void Modificadores::on_mui_tecla_h_clicked()
{
      sendKey(Qt::Key_H, "H");
}

void Modificadores::on_mui_tecla_j_clicked()
{
      sendKey(Qt::Key_J, "J");
}

void Modificadores::on_mui_tecla_k_clicked()
{
      sendKey(Qt::Key_K, "K");
}

void Modificadores::on_mui_tecla_l_clicked()
{
      sendKey(Qt::Key_L, "L");
}

void Modificadores::on_mui_tecla_enye_clicked()
{
      sendKey(Qt::Key_Ntilde, "Ñ");
}

void Modificadores::on_mui_tecla_intro_clicked()
{
      sendKey(Qt::Key_Enter, "Enter");
}

void Modificadores::on_mui_tecla_z_clicked()
{
      sendKey(Qt::Key_Z, "Z");
}

void Modificadores::on_mui_tecla_x_clicked()
{
    sendKey(Qt::Key_X, "X");
}

void Modificadores::on_mui_tecla_c_clicked()
{
    sendKey(Qt::Key_C, "C");
}

void Modificadores::on_mui_tecla_v_clicked()
{
    sendKey(Qt::Key_V, "V");
}

void Modificadores::on_mui_tecla_b_clicked()
{
    sendKey(Qt::Key_B, "B");
}

void Modificadores::on_mui_tecla_n_clicked()
{
    sendKey(Qt::Key_N, "N");
}

void Modificadores::on_mui_tecla_m_clicked()
{
    sendKey(Qt::Key_M, "M");
}

void Modificadores::on_mui_tecla_coma_clicked()
{
    sendKey(Qt::Key_Comma, ",");
}

void Modificadores::on_mui_tecla_punto_clicked()
{
    sendKey(Qt::Key_Period, ".");
}

void Modificadores::on_mui_tecla_guion_clicked()
{
    sendKey(Qt::Key_Minus, "-");
}

void Modificadores::on_mui_tecla_espacio_clicked()
{
    sendKey(Qt::Key_Space, " ");
}

void Modificadores::on_mui_tecla_inicio_clicked()
{
    sendKey(Qt::Key_Home, "Home");
}

void Modificadores::on_mui_tecla_fin_clicked()
{
    sendKey(Qt::Key_End, "End");
}

void Modificadores::on_mui_tecla_izquierda_clicked()
{
    sendKey(Qt::Key_Left, "Left");
}

void Modificadores::on_mui_tecla_derecha_clicked()
{
    sendKey(Qt::Key_Right, "Right");
}

void Modificadores::on_mui_tecla_arriba_clicked()
{
    sendKey(Qt::Key_Up, "Up");
}

void Modificadores::on_mui_tecla_abajo_clicked()
{
    sendKey(Qt::Key_Down, "Down");
}

void Modificadores::on_mui_tecla_suprimir_clicked()
{
    sendKey(Qt::Key_Delete, "Delete");
}




void Modificadores::on_mui_aceptar_clicked()
{
    BtCompany * emp1 = ( BtCompany * ) mainCompany();
    emp1->ticketActual()->lineaActBtTicket()->setDbValue ( m_fieldToModify, mui_texto->toPlainText() );
    done ( 0 );
}

