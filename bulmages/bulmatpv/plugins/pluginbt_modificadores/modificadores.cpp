#include <QWidget>
#include <QPushButton>

#include "modificadores.h"
#include "blfunctions.h"
#include "btticket.h"
#include "btcompany.h"


Modificadores::Modificadores ( BlMainCompany *emp, QWidget *parent ) : QDialog ( parent ), BlMainCompanyPointer ( emp )
{
    blDebug("Modificadores::Modificadores");
    
    bool hayModificadores = FALSE;
    
    setupUi ( this );

    QString idarticulo = ((BtCompany *) emp)->ticketActual()->lineaActBtTicket()->dbValue ( "idarticulo");
    
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT * FROM modificador WHERE idarticulo = " + idarticulo );
    while ( !cur->eof() ) {
	hayModificadores = TRUE;
        QPushButton * toolbutton = new QPushButton ( mui_frame );
        toolbutton->setText ( cur->valor ( "nombremodificador" ) );

        QVBoxLayout *m_hboxLayout1 = mui_frame->findChild<QVBoxLayout *> ( "hboxLayout1" );
        if ( !m_hboxLayout1 ) {
            m_hboxLayout1 = new QVBoxLayout ( mui_frame );
            m_hboxLayout1->setSpacing ( 5 );
            m_hboxLayout1->setMargin ( 5 );
            m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
        } // end if
        m_hboxLayout1->addWidget ( toolbutton );
        connect ( toolbutton, SIGNAL ( pressed() ), this, SLOT ( trabajadorClicked() ) );
        cur->nextRecord();
    } // end while
    delete cur;

    if (!hayModificadores) {
	/// Si no hay modificadores predeterminados. Se situa el 'tab' en escritura texto.
	mui_tab->setCurrentWidget(tab_texto);
        done(0);
    } else {
	mui_tab->setCurrentWidget(tab_predefinidos);
    } // end if
    blDebug("END Modificadores::Modificadores");

    mui_texto->ensureCursorVisible();
    
}


Modificadores::~Modificadores()
{}



void Modificadores::trabajadorClicked()
{
    blDebug("Modificadores::trabajadorClicked");

    BtCompany * emp1 = ( BtCompany * ) mainCompany();
    BtTicket *ticket = NULL;
    BtTicket *ticketv = NULL;
    bool encontrado = FALSE;

    QString idarticulo =  emp1->ticketActual()->lineaActBtTicket()->dbValue ( "idarticulo");
	
    /// Buscamos cual ha sido el modificador pulsado.
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT * FROM modificador WHERE idarticulo = " + idarticulo );
    while ( !encontrado ) {
        if ( ( ( QPushButton * ) sender() ) ->text() == cur->valor ( "nombremodificador" ) ) {
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
      emp1->ticketActual()->lineaActBtTicket()->setDbValue ( "idmodificador" + QString::number(i), cur->valor("idmodificador"));



    delete cur;

    done ( 0 );

    blDebug("END Modificadores::trabajadorClicked");

}

void Modificadores::moveCursor()
{
    mui_texto->setFocus(Qt::MouseFocusReason);
    QTextCursor cursor = mui_texto->textCursor();
    cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
    mui_texto->setTextCursor(cursor);
}

void Modificadores::on_mui_tecla_limpiar_clicked()
{
    mui_texto->clear();
    moveCursor();
}

void Modificadores::on_mui_tecla_1_clicked()
{
    mui_texto->setPlainText(mui_texto->toPlainText() + "1");
    moveCursor();
}

void Modificadores::on_mui_tecla_2_clicked()
{
    mui_texto->setPlainText(mui_texto->toPlainText() + "2");
    moveCursor();
}


void Modificadores::on_mui_tecla_3_clicked()
{
    mui_texto->setPlainText(mui_texto->toPlainText() + "3");
    moveCursor();
}

void Modificadores::on_mui_tecla_4_clicked()
{
    mui_texto->setPlainText(mui_texto->toPlainText() + "4");
    moveCursor();
}

void Modificadores::on_mui_tecla_5_clicked()
{
    mui_texto->setPlainText(mui_texto->toPlainText() + "5");
    moveCursor();
}

void Modificadores::on_mui_tecla_6_clicked()
{
    mui_texto->setPlainText(mui_texto->toPlainText() + "6");
    moveCursor();
}

void Modificadores::on_mui_tecla_7_clicked()
{
    mui_texto->setPlainText(mui_texto->toPlainText() + "7");
    moveCursor();
}

void Modificadores::on_mui_tecla_8_clicked()
{
    mui_texto->setPlainText(mui_texto->toPlainText() + "8");
    moveCursor();
}

void Modificadores::on_mui_tecla_9_clicked()
{
    mui_texto->setPlainText(mui_texto->toPlainText() + "9");
    moveCursor();
}

void Modificadores::on_mui_tecla_0_clicked()
{
    mui_texto->setPlainText(mui_texto->toPlainText() + "0");
    moveCursor();
}

void Modificadores::on_mui_tecla_borrar_clicked()
{
    mui_texto->setPlainText(mui_texto->toPlainText().mid(0, mui_texto->toPlainText().length() - 1));
    moveCursor();
}

void Modificadores::on_mui_tecla_q_clicked()
{
    mui_texto->setPlainText(mui_texto->toPlainText() + "Q");
    moveCursor();
}

void Modificadores::on_mui_tecla_w_clicked()
{
    mui_texto->setPlainText(mui_texto->toPlainText() + "W");
    moveCursor();
}

void Modificadores::on_mui_tecla_e_clicked()
{
    mui_texto->setPlainText(mui_texto->toPlainText() + "E");
    moveCursor();
}

void Modificadores::on_mui_tecla_r_clicked()
{
    mui_texto->setPlainText(mui_texto->toPlainText() + "R");
    moveCursor();
}

void Modificadores::on_mui_tecla_t_clicked()
{
    mui_texto->setPlainText(mui_texto->toPlainText() + "T");
    moveCursor();
}

void Modificadores::on_mui_tecla_y_clicked()
{
    mui_texto->setPlainText(mui_texto->toPlainText() + "Y");
    moveCursor();
}

void Modificadores::on_mui_tecla_u_clicked()
{
    mui_texto->setPlainText(mui_texto->toPlainText() + "U");
    moveCursor();
}

void Modificadores::on_mui_tecla_i_clicked()
{
    mui_texto->setPlainText(mui_texto->toPlainText() + "I");
    moveCursor();
}

void Modificadores::on_mui_tecla_o_clicked()
{
    mui_texto->setPlainText(mui_texto->toPlainText() + "O");
    moveCursor();
}

void Modificadores::on_mui_tecla_p_clicked()
{
    mui_texto->setPlainText(mui_texto->toPlainText() + "P");
    moveCursor();
}

void Modificadores::on_mui_tecla_a_clicked()
{
    mui_texto->setPlainText(mui_texto->toPlainText() + "A");
    moveCursor();
}

void Modificadores::on_mui_tecla_s_clicked()
{
    mui_texto->setPlainText(mui_texto->toPlainText() + "S");
    moveCursor();
}

void Modificadores::on_mui_tecla_d_clicked()
{
    mui_texto->setPlainText(mui_texto->toPlainText() + "D");
    moveCursor();
}

void Modificadores::on_mui_tecla_f_clicked()
{
    mui_texto->setPlainText(mui_texto->toPlainText() + "F");
    moveCursor();
}

void Modificadores::on_mui_tecla_g_clicked()
{
    mui_texto->setPlainText(mui_texto->toPlainText() + "G");
    moveCursor();
}

void Modificadores::on_mui_tecla_h_clicked()
{
    mui_texto->setPlainText(mui_texto->toPlainText() + "H");
    moveCursor();
}

void Modificadores::on_mui_tecla_j_clicked()
{
    mui_texto->setPlainText(mui_texto->toPlainText() + "J");
    moveCursor();
}

void Modificadores::on_mui_tecla_k_clicked()
{
    mui_texto->setPlainText(mui_texto->toPlainText() + "K");
    moveCursor();
}

void Modificadores::on_mui_tecla_l_clicked()
{
    mui_texto->setPlainText(mui_texto->toPlainText() + "L");
    moveCursor();
}

void Modificadores::on_mui_tecla_enye_clicked()
{
    mui_texto->setPlainText(mui_texto->toPlainText() + "Ñ");
    moveCursor();
}

void Modificadores::on_mui_tecla_intro_clicked()
{
    mui_texto->setPlainText(mui_texto->toPlainText() + "\n");
    moveCursor();
}

void Modificadores::on_mui_tecla_z_clicked()
{
    mui_texto->setPlainText(mui_texto->toPlainText() + "Z");
    moveCursor();
}

void Modificadores::on_mui_tecla_x_clicked()
{
    mui_texto->setPlainText(mui_texto->toPlainText() + "X");
    moveCursor();
}

void Modificadores::on_mui_tecla_c_clicked()
{
    mui_texto->setPlainText(mui_texto->toPlainText() + "C");
    moveCursor();
}

void Modificadores::on_mui_tecla_v_clicked()
{
    mui_texto->setPlainText(mui_texto->toPlainText() + "V");
    moveCursor();
}

void Modificadores::on_mui_tecla_b_clicked()
{
    mui_texto->setPlainText(mui_texto->toPlainText() + "B");
    moveCursor();
}

void Modificadores::on_mui_tecla_n_clicked()
{
    mui_texto->setPlainText(mui_texto->toPlainText() + "N");
    moveCursor();
}

void Modificadores::on_mui_tecla_m_clicked()
{
    mui_texto->setPlainText(mui_texto->toPlainText() + "M");
    moveCursor();
}

void Modificadores::on_mui_tecla_coma_clicked()
{
    mui_texto->setPlainText(mui_texto->toPlainText() + ",");
    moveCursor();
}

void Modificadores::on_mui_tecla_punto_clicked()
{
    mui_texto->setPlainText(mui_texto->toPlainText() + ".");
    moveCursor();
}

void Modificadores::on_mui_tecla_guion_clicked()
{
    mui_texto->setPlainText(mui_texto->toPlainText() + "-");
    moveCursor();
}

void Modificadores::on_mui_tecla_espacio_clicked()
{
    mui_texto->setPlainText(mui_texto->toPlainText() + " ");
    moveCursor();
}

