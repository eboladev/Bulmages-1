#include <QPainter>

#include "tecladonumerico.h"


TecladoNumerico::TecladoNumerico ( BlMainCompany *emp, QWidget *parent ) : BlWidget ( emp, parent )
{
    setupUi ( this );
}


TecladoNumerico::~TecladoNumerico()
{}


/* Necesario para poner estilos a traves de hojas CSS.
*/
void TecladoNumerico::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
