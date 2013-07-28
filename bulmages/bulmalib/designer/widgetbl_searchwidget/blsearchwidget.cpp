#include <QtWidgets/QFrame>
#include "blsearchwidget.h"


BlSearchWidget::BlSearchWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);
    m_textBusqueda->setFrameShadow(QFrame::Raised);
}

