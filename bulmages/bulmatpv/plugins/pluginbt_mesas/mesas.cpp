#include "mesas.h"
#include "btbulmatpv.h"
#include <QWidget>
#include <QMenu>
#include <QSvgRenderer>
#include <QFileDialog>
#include <QInputDialog>
#include <QTextStream>
#include <QDomDocument>
#include <QTimer>

#include "blmainwindow.h"
#include "blplugins.h"

int g_escala;
Mesas * g_mesas;
Mesa  * g_mesaAct;
bool  g_bloqueo;


Mesas::Mesas ( BtCompany *emp, QWidget *parent ) : BlWidget ( emp, parent )
{
    setupUi ( this );

    /// Cambiamos el icono de las mesas.
    mui_mesas->setIcon ( QIcon ( g_confpr->valor ( CONF_PROGDATA ) + "icons/table.svg" ) );

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
  g_bloqueo = TRUE;

  mui_borrar -> setVisible(!g_bloqueo);
  mui_cambiar_imagen -> setVisible(!g_bloqueo);
  mui_mesa6 -> setVisible(!g_bloqueo);
  mui_cambiar_nombre -> setVisible(!g_bloqueo);



  importXML("");

  
  QTimer *timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(repaint()));
  timer->start(5000);
  
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


void DistroMesas::on_mui_ver_ticket_clicked() {
  if (g_mesaAct) {
    g_mesaAct->abrirMesa();
  } // end if
}


void DistroMesas::on_mui_cambiar_nombre_clicked() {
  if (g_mesaAct) {
    g_mesaAct->cambiarNombre();
  } // end if
}

void DistroMesas::on_mui_cambiar_imagen_clicked() {
  if (g_mesaAct) {
    g_mesaAct->cambiarImagen();
  } // end if
}

void DistroMesas::on_mui_bloquear_toggled(bool bloq) {
  g_bloqueo = !bloq;
  mui_borrar -> setVisible(!g_bloqueo);
  mui_cambiar_imagen -> setVisible(!g_bloqueo);
  mui_mesa6 -> setVisible(!g_bloqueo);
  mui_cambiar_nombre -> setVisible(!g_bloqueo);
  repaint();
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
  m_XScale = 100;
  m_YScale = 100;
  m_escalando = 0;
}


Mesa::~Mesa()
{}


void Mesa::paintEvent ( QPaintEvent * event ) {

        QPainter painter;
        painter.begin ( this ); // paint in picture
	painter.setPen(QColor(0, 0, 0, 255));
	QFont sansFont("Helvetica [Cronyx]", 8);
	QFont normal("Helvetica [Cronyx]", 10);
	painter.setFont(normal);
	
    BtCompany * emp = ( BtCompany * ) mainCompany();

    /// Miramos que no haya ningun ticket abierto con el nombre usado
    BtTicket *ticket = NULL;;
    for ( int i = 0; i < emp->listaTickets() ->size(); ++i ) {
        ticket = emp->listaTickets() ->at ( i );
        if ( m_nombreMesa == ticket->dbValue ( "nomticket" )) {
	  
	    if (ticket->dbValue("idalbaran") != "") {
	       painter.setBrush(Qt::cyan);
		painter.drawRect(QRectF (0, 0, m_XScale, 30));
		painter.setBrush(Qt::NoBrush);

	    } // end if
	  
	  
	    int min = ticket->elapsed() / 60000;
	    int sec = (ticket->elapsed() % 60000) / 1000;
	    QString tiempo = QString::number(min) + ":" + QString::number(sec);
	    painter.setFont(sansFont);
	    painter.drawText(0,20, tiempo);
	    painter.setFont(normal);
            painter.setPen(QColor(255, 0, 0, 127));
        }// end if
    }// end for
    painter.drawText(0, 10, m_nombreMesa);

if (m_escalando == 0)  {
        QSvgRenderer arender(QString(m_filename), this);
//        arender.render(&painter, QRectF (0, 15, g_escala, g_escala));
        arender.render(&painter, QRectF (0, 15, m_XScale, m_YScale));
}

    if (g_mesaAct == this) {
        if (!g_bloqueo) {
            painter.setPen(QColor(0, 0, 0, 127));
            painter.setPen(Qt::DashDotLine);
            painter.drawRect(0,0,m_XScale,m_YScale);

            painter.setPen(QColor(0, 0, 0, 127));
            painter.setPen(Qt::DashDotLine);
            painter.setBrush(Qt::green);
            painter.drawRect(0,0,5,5);
            painter.drawRect(0,m_YScale -5, 5, 5);
            painter.drawRect(m_XScale -5,0, 5, 5);
            painter.drawRect(m_XScale -5,m_YScale -5, 5, 5);

            painter.drawRect(m_XScale / 2 -3,m_YScale -5, 5, 5);
            painter.drawRect(m_XScale / 2 -3, 0, 5, 5);
            painter.drawRect(0,m_YScale / 2 -3, 5, 5);
            painter.drawRect(m_XScale - 5,m_YScale / 2 -3, 5, 5);

        
      } else {
            painter.setPen(Qt::DashDotLine);
            painter.drawRect(0,0,m_XScale,m_YScale);
      }// end if
    } //end if


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

      /// Hacemos la comprobacion de escalado hacia abajo.
      if (!g_bloqueo) {
          if (offset.x() > m_XScale -6 && offset.y() > m_YScale - 6) {
              m_escalando = 1;
          } else if (offset.x() > m_XScale /2-3 && offset.x() < m_XScale /2 +3 && offset.y() > m_YScale - 6) {
              m_escalando = 2;
          } else if (offset.y() > m_YScale /2-3 && offset.y() < m_YScale /2 +3 && offset.x() > m_XScale - 6) {
              m_escalando = 3;
          } else if (offset.x() < 6  && offset.y() < 6) {
              m_escalando = 4;
          } else if (offset.x() > m_XScale -6 && offset.y() < 6) {
              /// Arriba a la derecha
              m_escalando = 5;
          } else if (offset.y() > m_YScale -6 && offset.x() < 6) {
              /// Abajo a la izquierda
              m_escalando = 6;
          } else if (offset.y() > m_YScale /2-3 && offset.y() < m_YScale /2 +3 && offset.x() < 6) {
              /// Enmedio a la izquierda
              m_escalando = 7;
          } else if (offset.x() > m_XScale /2-3 && offset.x() < m_XScale /2 +3 && offset.y() < 6) {
              /// Arriba enmedio
              m_escalando = 8;
          } else {
            /// Movemos el elemento sin escalar.
            m_escalando  = 9;
          } // end if
      } // end if
    } // end if
}

void Mesa::mouseMoveEvent(QMouseEvent* event)
{
    if (!g_bloqueo) {
      if (m_escalando > 0 && m_escalando < 9 ) {
        if (m_escalando == 1) {
          if (isWindow()) {
              m_XScale += (event->globalPos().x() - offset.x());
              m_YScale += (event->globalPos().y() - offset.y());
          } else {
              m_XScale += (event->pos().x() - offset.x());
              m_YScale += (event->pos().y() - offset.y());
          } // end if
        } else if (m_escalando == 2) {
          if (isWindow()) {
              m_YScale += (event->globalPos().y() - offset.y());
          } else {
              m_YScale += (event->pos().y() - offset.y());
          } // end if
        } else if (m_escalando == 3) {
          if (isWindow()) {
              m_XScale += (event->globalPos().x() - offset.x());
          } else {
              m_XScale += (event->pos().x() - offset.x());
          } // end if
        } else if (m_escalando == 4) {
          /// Arriba a la izquierda
          if (isWindow()) {
              m_XScale -= (event->globalPos().x() - offset.x());
              m_YScale -= (event->globalPos().y() - offset.y());
              move(event->globalPos() - offset);
          } else {
              m_XScale -= (event->pos().x() - offset.x());
              m_YScale -= (event->pos().y() - offset.y());
              move(mapToParent(event->pos() - offset));
          } // end if
        } else if (m_escalando == 5) {
          /// Arriba a la derecha
          if (isWindow()) {
              m_XScale += (event->globalPos().x() - offset.x());
              m_YScale -= (event->globalPos().y() - offset.y());
              QPoint roffset = event->globalPos() - offset;
              roffset.setX(x());
              move(roffset);
          } else {
              m_XScale += (event->pos().x() - offset.x());
              m_YScale -= (event->pos().y() - offset.y());
              QPoint roffset = event->pos() - offset;
              roffset.setX(mapFromParent(pos()).x());
              move(mapToParent(roffset));
          } // end if
        } else if (m_escalando == 6) {
          /// Abajo a la izquierda
          if (isWindow()) {
              m_XScale -= (event->globalPos().x() - offset.x());
              m_YScale += (event->globalPos().y() - offset.y());
              QPoint roffset = event->globalPos() - offset;
              roffset.setY(y());
              move(roffset);
          } else {
              m_XScale -= (event->pos().x() - offset.x());
              m_YScale += (event->pos().y() - offset.y());
              QPoint roffset = event->pos() - offset;
              roffset.setY(mapFromParent(pos()).y());
              move(mapToParent(roffset));
          } // end if
        } else if (m_escalando == 7) {
          /// Enmedio a la izquierda
          if (isWindow()) {
              m_XScale -= (event->globalPos().x() - offset.x());
              QPoint roffset = event->globalPos() - offset;
              roffset.setY(y());
              move(roffset);
          } else {
              m_XScale -= (event->pos().x() - offset.x());
              QPoint roffset = event->pos() - offset;
              roffset.setY(mapFromParent(pos()).y());
              move(mapToParent(roffset));
          } // end if
        } else if (m_escalando == 8) {   
          /// El cuadro de arriba en el centro
          if (isWindow()) {
              m_YScale -= (event->globalPos().y() - offset.y());
              QPoint roffset = event->globalPos() - offset;
              roffset.setX(x());
              move(roffset);
          } else {
              m_YScale -= (event->pos().y() - offset.y());
              QPoint roffset = event->pos() - offset;
              roffset.setX(mapFromParent(pos()).x());
              move(mapToParent(roffset));
          } // end if
        }

//          setGeometry(posx.text().toInt(),posy.text().toInt(),m_XScale , m_YScale );
          if (m_XScale > 20 && m_YScale > 20)
            setFixedSize(m_XScale+10 ,m_YScale+10);
              
      } else if (m_escalando == 9) {
          if (isWindow())
              move(event->globalPos() - offset);
          else
              move(mapToParent(event->pos() - offset));
      } // end if
      event->accept(); // do not propagate
  } // end if
}

void Mesa::mouseReleaseEvent(QMouseEvent* event)
{
    if (event -> button() == Qt::LeftButton) {
      if (!g_bloqueo) {
        event->accept(); // do not propagate
        offset = QPoint();
        if (m_escalando > 0) {
          m_escalando = 0;
          repaint();
        } // end if
      } else {
        abrirMesa();
      } // end if
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
    menu->addAction(tr("Cambiar imagen"), this, SLOT(cambiarImagen()));
    menu->addAction(tr("Cambiar nombre"), this, SLOT(cambiarNombre()));
    menu->addAction(tr("Abrir Mesa"), this, SLOT(abrirMesa()));
    menu->addAction(tr("Aumentar Escala"), this, SLOT(abrirMesa()));
    menu->addAction(tr("Disminuir Escala"), this, SLOT(abrirMesa()));
    menu->exec(mapToGlobal(pos));
}

void Mesa::cambiarImagen() {
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
  val += "\t<XSCALE>" + QString::number(m_XScale) + "</XSCALE>\n";
  val += "\t<YSCALE>" + QString::number(m_YScale) + "</YSCALE>\n";
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
//    setGeometry(posx.text().toInt(),posy.text().toInt(),g_escala + 20 , g_escala + 20);

    QDomElement xscale = docElem.firstChildElement ( "XSCALE" );
    QDomElement yscale = docElem.firstChildElement ( "YSCALE" );
    m_XScale = xscale.text().toInt();
    m_YScale = yscale.text().toInt();
    setGeometry(posx.text().toInt(),posy.text().toInt(),m_XScale+1 , m_YScale+1 );
    setFixedSize(m_XScale+10 ,m_YScale+10);

    repaint();
}
