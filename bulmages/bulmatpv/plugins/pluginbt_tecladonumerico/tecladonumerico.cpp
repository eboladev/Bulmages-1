#include <QtCore/QFile>

#include "tecladonumerico.h"


TecladoNumerico::TecladoNumerico ( BlMainCompany *emp, QWidget *parent ) : BlWidget ( emp, parent )
{
    setupUi ( this );
}


TecladoNumerico::~TecladoNumerico()
{}


void TecladoNumerico::playSound()
{
}


void TecladoNumerico::on_mui_00_clicked() {
    playSound();
    ( ( BtCompany * ) mainCompany() ) ->pulsaTecla ( Qt::Key_0, "0" );
    ( ( BtCompany * ) mainCompany() ) ->pulsaTecla ( Qt::Key_0, "0" );
}


void TecladoNumerico::on_mui_0_clicked() {
    playSound();
    ( ( BtCompany * ) mainCompany() ) ->pulsaTecla ( Qt::Key_0, "0" );
}


void TecladoNumerico::on_mui_1_clicked() {
    playSound();
    ( ( BtCompany * ) mainCompany() ) ->pulsaTecla ( Qt::Key_1, "1" );
}


void TecladoNumerico::on_mui_2_clicked() {
    playSound();
    ( ( BtCompany * ) mainCompany() ) ->pulsaTecla ( Qt::Key_2, "2" );
}


void TecladoNumerico::on_mui_3_clicked() {
    playSound();
    ( ( BtCompany * ) mainCompany() ) ->pulsaTecla ( Qt::Key_3, "3" );
}


void TecladoNumerico::on_mui_4_clicked() {
    playSound();
    ( ( BtCompany * ) mainCompany() ) ->pulsaTecla ( Qt::Key_4, "4" );
}


void TecladoNumerico::on_mui_5_clicked() {
    playSound();
    ( ( BtCompany * ) mainCompany() ) ->pulsaTecla ( Qt::Key_5, "5" );
}


void TecladoNumerico::on_mui_6_clicked() {
    playSound();
    ( ( BtCompany * ) mainCompany() ) ->pulsaTecla ( Qt::Key_6, "6" );
}


void TecladoNumerico::on_mui_7_clicked() {
    playSound();
    ( ( BtCompany * ) mainCompany() ) ->pulsaTecla ( Qt::Key_7, "7" );
}


void TecladoNumerico::on_mui_8_clicked() {
    playSound();
    ( ( BtCompany * ) mainCompany() ) ->pulsaTecla ( Qt::Key_8, "8" );
}


void TecladoNumerico::on_mui_9_clicked() {
    playSound();
    ( ( BtCompany * ) mainCompany() ) ->pulsaTecla ( Qt::Key_9, "9" );
}


void TecladoNumerico::on_mui_punto_clicked() {
    playSound();
    ( ( BtCompany * ) mainCompany() ) ->pulsaTecla ( Qt::Key_Colon, "." );
}


void TecladoNumerico::on_mui_borrar_clicked() {
    playSound();
    ( ( BtCompany * ) mainCompany() ) ->pulsaTecla ( Qt::Key_Backspace );
}


void TecladoNumerico::on_mui_articulo_clicked() {
    playSound();
    ( ( BtCompany * ) mainCompany() ) ->pulsaTecla ( Qt::Key_Return );
}


void TecladoNumerico::on_mui_cantidad_clicked() {
    playSound();
    ( ( BtCompany * ) mainCompany() ) ->pulsaTecla ( Qt::Key_Asterisk, "*" );
}


void TecladoNumerico::on_mui_precio_clicked() {
    playSound();
    ( ( BtCompany * ) mainCompany() ) ->pulsaTecla ( Qt::Key_Slash, "/" );
}


void TecladoNumerico::on_mui_c_clicked() {
    playSound();
    ( ( BtCompany * ) mainCompany() ) ->pulsaTecla ( Qt::Key_F4, "" );
}


void TecladoNumerico::on_mui_plus_clicked() {
    playSound();
    ( ( BtCompany * ) mainCompany() ) ->pulsaTecla ( Qt::Key_Plus, "+" );
}


void TecladoNumerico::on_mui_minus_clicked() {
    playSound();
    ( ( BtCompany * ) mainCompany() ) ->pulsaTecla ( Qt::Key_Minus, "-" );
}


void TecladoNumerico::on_mui_mesa_clicked() {
    playSound();
    ( ( BtCompany * ) mainCompany() ) ->pulsaTecla ( Qt::Key_F3, "" );
}

