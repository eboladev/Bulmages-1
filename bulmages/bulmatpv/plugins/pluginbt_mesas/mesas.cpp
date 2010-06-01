#include "mesas.h"
#include "btbulmatpv.h"
#include <QWidget>
#include <QMenu>
#include <QSvgRenderer>
#include <QFileDialog>
#include <QInputDialog>
#include <QTextStream>
#include <QDomDocument>

#include "blmainwindow.h"
#include "blplugins.h"

int g_escala;
Mesas * g_mesas;
Mesa  * g_mesaAct;

Mesas::Mesas ( BtCompany *emp, QWidget *parent ) : BlWidget ( emp, parent )
{
    setupUi ( this );

    /// Cambiamos el icono de las mesas.
//    mui_mesas->setMinimumSize ( QSize ( 32, 32 ) );
    mui_mesas->setIcon ( QIcon ( g_confpr->valor ( CONF_PROGDATA ) + "icons/table.svg" ) );
//    mui_mesas->setIconSize ( QSize ( 32, 32 ) );

    m_centralWidget = NULL;
    m_distro = NULL;
    g_mesas = this;
    g_mesaAct = NULL;
}


Mesas::~Mesas()
{}

void Mesas::on_mui_mesas_clicked()
{

    if (m_centralWidget == NULL) {
        /// Creamos y presentamos el plugin de mesas.
         if (m_distro == NULL)
            m_distro= new DistroMesas((BtCompany *) mainCompany(), this);
        m_centralWidget = ((BtBulmaTPV *)g_main)->centralBtWidget();
        ((BtBulmaTPV *)g_main) -> setCentralBtWidget (m_distro);
    } else {
        ((BtBulmaTPV *)g_main) -> setCentralBtWidget (m_centralWidget);
        m_centralWidget = NULL;
        m_distro->exportXML();
    } // end if

}

//  ========================0




DistroMesas::DistroMesas ( BtCompany *emp, QWidget *parent ) : BlWidget ( emp, parent )
{
  setupUi(this);

  m_background = "";
  g_escala = 200;

  importXML("");

}


DistroMesas::~DistroMesas()
{
  exportXML();
}


void DistroMesas::on_mui_mesa6_clicked() {

  Mesa *mesa = new Mesa((BtCompany *) mainCompany(), mui_widget);
  mesa->setGeometry(50,50,g_escala + 20 , g_escala + 20);
  mesa->show();

}


void DistroMesas::on_mui_escala_clicked() {

  if (g_escala < 1000)  g_escala += 10;
  repaint();

}

void DistroMesas::on_mui_escala1_clicked() {

  if (g_escala > 30)  g_escala -= 10;
  repaint();

}



void DistroMesas::on_mui_cerrar_clicked() {
  g_mesas->on_mui_mesas_clicked();
}


void DistroMesas::on_mui_borrar_clicked() {
  if (g_mesaAct != NULL ) {
      delete g_mesaAct;
      g_mesaAct = NULL;
  } // end if
}


void DistroMesas::paintEvent ( QPaintEvent * event ) {

        QPainter painter;
        painter.begin ( this ); // paint in picture
        QSvgRenderer arender(m_background, mui_widget);
        arender.render(&painter);
        painter.end();

}

void DistroMesas::importXML(const QString val) {




  QFile file ( g_confpr->valor ( CONF_DIR_USER ) + "distromesas_" + mainCompany()->dbName() + ".cfn" );

    if (file.exists()) {
        if ( !file.open ( QIODevice::ReadOnly ) ) {
            _depura ( "END DistroMesas::syncXML", 0, "Fichero no se puede abrir" );
            return;
        } // end if
        QString result (file.readAll());
        file.close(); 


    QDomDocument doc ( "mydocument" );

    if ( !doc.setContent ( result ) ) {
        _depura ( "END Mesa::importXML", 0, "XML Invalido" );
        return;
    } // end if

    QDomElement docElem = doc.documentElement();
    QDomElement principal = docElem.firstChildElement ( "BACKGROUND" );
    m_background = principal.text();

    principal = docElem.firstChildElement ( "ESCALA" );
    g_escala = principal.text().toInt();


    QDomNodeList nodos = docElem.elementsByTagName ( "MESA" );
    int i = 0;
    while (i < nodos.count() ) {
        QDomNode ventana = nodos.item ( i++ );
        QDomElement e1 = ventana.toElement(); /// try to convert the node to an element.
        if ( !e1.isNull() ) { /// the node was really an element.
            QString nodoText = e1.text();
            /// Pasamos el XML a texto para poder procesarlo como tal.
            QString result;
            QTextStream stream ( &result );
            ventana.save(stream,5);

            Mesa *mesa = new Mesa((BtCompany *) mainCompany(), mui_widget);
            mesa->importXML(result);
            mesa->show();

        } // end if
    } // end while

} // end if

}


QString DistroMesas::exportXML() {
  QFile file ( g_confpr->valor ( CONF_DIR_USER ) + "distromesas_" + mainCompany()->dbName() + ".cfn" );
  QString val;
    if ( file.open ( QIODevice::WriteOnly ) ) {
        QTextStream stream ( &file );


  val = "<DISTROMESAS>\n";
  val += "\t<BACKGROUND>" + m_background + "</BACKGROUND>\n";
  val += "\t<ESCALA>" + QString::number(g_escala) + "</ESCALA>\n";

  QList<Mesa *> mesas = findChildren<Mesa *>();
 for (int i = 0; i < mesas.size(); ++i) {
     val += mesas.at(i)->exportXML();

 } // end for
  val += "</DISTROMESAS>\n";

  stream << val;
  file.close();

} // end if
  return val;
}

//  ===================================

Mesa::Mesa ( BtCompany *emp, QWidget *parent ) : BlWidget ( emp, parent )
{
  setFixedSize(g_escala + 20,g_escala + 20);
  setContextMenuPolicy(Qt::CustomContextMenu);
  connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(ctxMenu(const QPoint &)));
  m_nombreMesa = "Sin Definir";
}


Mesa::~Mesa()
{}


void Mesa::paintEvent ( QPaintEvent * event ) {

        QPainter painter;
        painter.begin ( this ); // paint in picture

            painter.setPen(QColor(0, 0, 0, 255));


    BtCompany * emp = ( BtCompany * ) mainCompany();

    /// Miramos que no haya ningun ticket abierto con el nombre usado
    BtTicket *ticket;
    for ( int i = 0; i < emp->listaTickets() ->size(); ++i ) {
        ticket = emp->listaTickets() ->at ( i );
        if ( m_nombreMesa == ticket->dbValue ( "nomticket" )) {
            painter.setPen(QColor(255, 0, 0, 127));
        }// end if
    }// end for

        painter.drawText(0, 10, m_nombreMesa);

        QSvgRenderer arender(QString(m_filename), this);
        arender.render(&painter, QRectF (0, 15, g_escala, g_escala));

    if (g_mesaAct == this) {
            painter.setPen(QColor(0, 0, 0, 127));
            painter.setPen(Qt::DashDotLine);
//            painter.drawText(0,20,"ACTUAL");
            painter.drawRect(0,0,g_escala,g_escala);

            painter.setPen(QColor(0, 0, 0, 127));
            painter.setPen(Qt::DashDotLine);
            painter.setBrush(Qt::green);
//            painter.drawText(0,20,"ACTUAL");
            painter.drawRect(0,0,5,5);
            painter.drawRect(0,g_escala -5, 5, 5);
            painter.drawRect(g_escala -5,0, 5, 5);
            painter.drawRect(g_escala -5,g_escala -5, 5, 5);

            painter.drawRect(g_escala / 2 -3,g_escala -5, 5, 5);
            painter.drawRect(g_escala / 2 -3, 0, 5, 5);
            painter.drawRect(0,g_escala / 2 -3, 5, 5);
            painter.drawRect(g_escala - 5,g_escala / 2 -3, 5, 5);

        
    } // end if


        painter.end();

}

// "offset" is a member variable of type QPoint

void Mesa::mousePressEvent(QMouseEvent* event)
{
    if (event -> button() == Qt::LeftButton) {
      event->accept(); // do not propagate
      if (g_mesaAct != this) {
        Mesa *old = g_mesaAct;
        g_mesaAct = this;
        if (old != NULL) old->repaint();
        repaint();
      } // end if
      if (isWindow())
          offset = event->globalPos() - pos();
      else
          offset = event->pos();
    } // end if
}

void Mesa::mouseMoveEvent(QMouseEvent* event)
{


      event->accept(); // do not propagate
      if (isWindow())
          move(event->globalPos() - offset);
      else
          move(mapToParent(event->pos() - offset));

}

void Mesa::mouseReleaseEvent(QMouseEvent* event)
{
    if (event -> button() == Qt::LeftButton) {
      event->accept(); // do not propagate
      offset = QPoint();
    } // end if
}


void Mesa::mouseDoubleClickEvent ( QMouseEvent * event ) {
  if (event->type() == QEvent::MouseButtonDblClick) {
      event->accept(); 
      abrirMesa();
  } // end if
}

void Mesa::ctxMenu(const QPoint &pos) {
    QMenu *menu = new QMenu;
    menu->addAction(tr("Cambiar imagen"), this, SLOT(test_slot()));
    menu->addAction(tr("Cambiar nombre"), this, SLOT(cambiarNombre()));
    menu->addAction(tr("Abrir Mesa"), this, SLOT(abrirMesa()));
    menu->exec(mapToGlobal(pos));
}

void Mesa::test_slot() {
   m_filename = QFileDialog::getOpenFileName(
                    this,
                    "Choose a file",
                    "",
                    "Images (*.svg)");
    repaint();
}

void Mesa::cambiarNombre() {
        bool ok;
        QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                             tr("Nombre Mesa:"), QLineEdit::Normal,
                                             m_nombreMesa, &ok);
        if (ok && !text.isEmpty())
            m_nombreMesa = text;
    repaint();
}

void Mesa::abrirMesa() {
    BtCompany * emp = ( BtCompany * ) mainCompany();


    /// Miramos que no haya ningun ticket abierto con el nombre usado
    BtTicket *ticket;
    for ( int i = 0; i < emp->listaTickets() ->size(); ++i ) {
        ticket = emp->listaTickets() ->at ( i );
        if ( m_nombreMesa == ticket->dbValue ( "nomticket" )) {

            /// Quitamos el bloqueo
            emp->ticketActual() ->setDbValue( "bloqueadoticket", "FALSE");
            
            /// Llamamos a plugins para poder hacer lo pertinente
            g_plugins->lanza("Abrevs_on_mui_aparcar_clicked", this);



            /// Ponemos al trabajador creado el trabajador del ticket actual.
            emp->setTicketActual ( ticket );
            /// Borra el valor del Input.
            emp->pulsaTecla ( Qt::Key_F4, "" );

            /// Llamamos a plugins para poder hacer lo pertinente
            g_plugins->lanza("Abrevs_on_mui_aparcar_clicked_Post", this);
            
            /// Ponemos el nuevo bloqueo
            ticket->setDbValue("bloqueadoticket", "TRUE");
            
            ticket->pintar();


            g_mesas->on_mui_mesas_clicked();


            return;
        }// end if
    }// end for


    BtTicket *tick;

    tick = emp->newBtTicket();
    tick->setDbValue ( "idtrabajador", emp->ticketActual() ->dbValue ( "idtrabajador" ) );
     emp->setValorBtInput(m_nombreMesa);


    emp->listaTickets() ->append ( tick );

    /// El nombre del ticket a aparcar lo coge del BtInput.
    tick ->setDbValue ( "nomticket", m_nombreMesa );

    /// Quitamos el bloqueo
    emp->ticketActual() ->setDbValue( "bloqueadoticket", "FALSE");
    
    /// Llamamos a plugins para poder hacer lo pertinente
     g_plugins->lanza("Abrevs_on_mui_aparcar_clicked", this);



    /// Ponemos al trabajador creado el trabajador del ticket actual.
    emp->setTicketActual ( tick );
    /// Borra el valor del Input.
    emp->pulsaTecla ( Qt::Key_F4, "" );

    /// Llamamos a plugins para poder hacer lo pertinente
    g_plugins->lanza("Abrevs_on_mui_aparcar_clicked_Post", this);
    
    /// Ponemos el nuevo bloqueo
    tick->setDbValue("bloqueadoticket", "TRUE");
    
    tick->pintar();

    g_mesas->on_mui_mesas_clicked();
}




QString Mesa::exportXML() {
  QString val;
  val = "<MESA>\n";
  val += "\t<IMAGEN>" + m_filename + "</IMAGEN>\n";
  val += "\t<NOMBRE>" + m_nombreMesa + "</NOMBRE>\n";
  val += "\t<POSX>" + QString::number(x()) + "</POSX>\n";
  val += "\t<POSY>" + QString::number(y()) + "</POSY>\n";
  val += "</MESA>\n";

  return val;
}


void Mesa::importXML(const QString val) {

    QDomDocument doc ( "mydocument" );

    if ( !doc.setContent ( val ) ) {
        _depura ( "END Mesa::importXML", 0, "XML Invalido" );
        return;
    } // end if

    QDomElement docElem = doc.documentElement();
    QDomElement principal = docElem.firstChildElement ( "IMAGEN" );
    m_filename = principal.text();

    QDomElement nombre = docElem.firstChildElement ( "NOMBRE" );
    m_nombreMesa = nombre.text();

    QDomElement posx = docElem.firstChildElement ( "POSX" );
    QDomElement posy = docElem.firstChildElement ( "POSY" );
    setGeometry(posx.text().toInt(),posy.text().toInt(),g_escala + 20 , g_escala + 20);

    repaint();
}
