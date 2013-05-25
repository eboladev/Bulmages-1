#include "mesas.h"
#include "btbulmatpv.h"
#include <QtWidgets/QWidget>
#include <QtWidgets/QMenu>
#include <QtSvg/QSvgRenderer>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QInputDialog>
#include <QtCore/QTextStream>
#include <QtXml/QDomDocument>
#include <QtCore/QTimer>

#include "blmainwindow.h"
#include "blplugins.h"

int g_escala;
Mesas *g_mesas;
Mesa  *g_mesaAct;
/// Modo configuracion de mesas o no.
bool g_bloqueo;
/// Modo selector de mesas: normal (true) o especial (false) (no activa mesas).
bool g_selectMode;
bool g_joinTables;
QList<QString> g_selectedTables;



Mesas::Mesas ( BtCompany *emp, QWidget *parent ) : BlWidget ( emp, parent )
{
    setupUi ( this );

    /// Cambiamos el icono de las mesas.
    mui_mesas->setIcon ( QIcon ( g_confpr->value( CONF_PROGDATA ) + "icons/table.svg" ) );

    m_centralWidget = NULL;
    m_distro = NULL;
    g_mesas = this;
    g_mesaAct = NULL;
}


Mesas::~Mesas()
{}

void Mesas::on_mui_mesas_clicked()
{

    /// Se asegura que esta en modo normal.
    g_selectMode = true;
    g_joinTables = false;

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

    m_distro->mui_table_move -> setChecked (false);

}

//  ========================0




DistroMesas::DistroMesas ( BtCompany *emp, QWidget *parent ) : BlWidget ( emp, parent )
{
  setupUi(this);

  m_background = "";
  g_escala = 200;
  g_bloqueo = true;
  g_selectMode = true;
  g_joinTables = false;

  mui_borrar -> setVisible(!g_bloqueo);
  mui_cambiar_imagen -> setVisible(!g_bloqueo);
  mui_mesa6 -> setVisible(!g_bloqueo);
  mui_cambiar_nombre -> setVisible(!g_bloqueo);
  mui_eliminarpantalla -> setVisible (!g_bloqueo);
  mui_nuevapantalla -> setVisible (!g_bloqueo);
  mui_table_move -> setVisible (g_bloqueo);


  importXML("");

  
  QTimer *timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(repaint()));
  timer->start(4000);
  
}


DistroMesas::~DistroMesas()
{
  exportXML();
}



void DistroMesas::on_mui_table_move_toggled(bool togg) {
  /// Si se pulsa la primera vez se activa el procedimiento de mover mesas.
  /// Si se vuelve a pulsar antes de completarse el procedimiento este queda cancelado
  /// y el boton vuelve a su estado de reposo.

  if (g_selectMode == true) {
      /// Modo especial.
      g_selectMode = false;
      g_joinTables = true;
      g_selectedTables.clear();
      
  } else {
      /// Modo normal.
      g_selectMode = true;
      g_joinTables = false;
      g_selectedTables.clear();
  } // end if
  
  update();
}


void DistroMesas::on_mui_mesa6_clicked() {

  Mesa *mesa = new Mesa((BtCompany *) mainCompany(), mui_widget);
  mesa->setGeometry(50,50,g_escala + 20 , g_escala + 20);
  mesa->m_pantalla = m_pantallaactual;
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
  mui_eliminarpantalla -> setVisible (!g_bloqueo);
  mui_nuevapantalla -> setVisible (!g_bloqueo);
  mui_table_move -> setVisible (g_bloqueo);
  repaint();
}


void DistroMesas::paintEvent ( QPaintEvent * event ) {

        QPainter painter;
        painter.begin ( this ); // paint in picture
	
	QSvgRenderer *arender;
	
	if (m_background.isEmpty()) {
	  arender = new QSvgRenderer(mui_widget);
	} else {
	  arender = new QSvgRenderer(m_background, mui_widget);
	} // end if
	
        arender->render(&painter);
        painter.end();
	
	QList<Mesa *> mesas = findChildren<Mesa *>();
	for (int i = 0; i < mesas.size(); ++i) {
	    if ( mesas.at(i)->m_pantalla == m_pantallaactual) {
		mesas.at(i)->setVisible(true);
	    } else {
		mesas.at(i)->setVisible(false);
	    } // end if
	} // end for
	
	delete arender;

}

void DistroMesas::importXML(const QString val) {
  QFile file ( g_confpr->value( CONF_DIR_USER ) + "distromesas_" + mainCompany()->dbName() + ".cfn" );

    if (file.exists()) {
        if ( !file.open ( QIODevice::ReadOnly ) ) {
            
            return;
        } // end if
        QString result (file.readAll());
        file.close(); 


    QDomDocument doc ( "mydocument" );

    if ( !doc.setContent ( result ) ) {
	
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
	    
	    if (! m_listapantallas.contains(mesa->m_pantalla)) {
	        if (m_pantallaactual == "") {
		    m_pantallaactual = mesa->m_pantalla;
		} // end if
		m_listapantallas.append(mesa->m_pantalla);
		QToolButton *but = new QToolButton(this);
		but->setObjectName("p_"+mesa->m_pantalla);
		but->setText(mesa->m_pantalla);
		but->setMinimumHeight(42);
		but->setMinimumWidth(42);
		but->setCheckable(true);
		mui_espaciopantallas->addWidget(but);
		connect(but, SIGNAL(clicked()), this, SLOT(cambiarPantalla()));
	    } // end if
	    if (mesa->m_pantalla == m_pantallaactual) 
		mesa->show();

        } // end if
    } // end while

} // end if

}


void DistroMesas::cambiarPantalla() {
  m_pantallaactual = ((QToolButton *)sender())->text();
  ((QToolButton *) sender())->setChecked(true);
  QList<QToolButton *> allPButtons = findChildren<QToolButton *>(QRegExp("p_*"));
  for (int i = 0; i < allPButtons.size(); ++i) {
      if (allPButtons.at(i) != sender()) {
	  allPButtons.at(i)->setChecked(false);
      } // end if
  } // end for
  repaint();
}

void DistroMesas::on_mui_nuevapantalla_clicked() {
            bool ok;
        QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                             tr("Nombre Pantalla:"), QLineEdit::Normal,
                                             "", &ok);
        if (ok && !text.isEmpty())
            	m_listapantallas.append(text);
	
	QToolButton *but = new QToolButton(this);
	but->setObjectName("p_" + text);
	but->setText(text);
	but->setCheckable(true);
	but->setMinimumHeight(42);
	but->setMinimumWidth(42);
	mui_espaciopantallas->addWidget(but);
	connect(but, SIGNAL(clicked()), this, SLOT(cambiarPantalla()));
    repaint();
}


void DistroMesas::on_mui_eliminarpantalla_clicked() {
  
  QString pantalla = m_pantallaactual;
  
  /// Tambien borramos las mesas de la pantalla
  QList<Mesa *> mesas = findChildren<Mesa *>();
  for (int i = 0; i < mesas.size(); ++i) {
      if (mesas.at(i)->m_pantalla == pantalla ) {
	  delete mesas.at(i);
      } // end if
  } // end for
  
  QList<QToolButton *> allPButtons = findChildren<QToolButton *>(QRegExp("p_*"));
  for (int i = 0; i < allPButtons.size(); ++i) {
      if (allPButtons.at(i)->text() == pantalla) {
	  delete allPButtons.at(i);
      } else {
	  m_pantallaactual = allPButtons.at(i)->text();
      } // end if
  } // end for
  
}


QString DistroMesas::exportXML() {
  QFile file ( g_confpr->value( CONF_DIR_USER ) + "distromesas_" + mainCompany()->dbName() + ".cfn" );
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
  m_nombreMesa = _("Sin definir");
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
    
    
    if (g_selectedTables.contains(m_nombreMesa) && (!g_selectMode)) {    
      painter.fillRect(0,0,m_XScale,m_YScale, QColor(255,255,0));
    } // end if
    
    /// Miramos que no haya ningun ticket abierto con el nombre usado
    BtTicket *ticket = NULL;
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
	    QString tiempo;
	    tiempo.sprintf( "%02i:%02i",  min, sec );

	    
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

// "m_offset" is a member variable of type QPoint

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
          m_offset = event->globalPos() - pos();
      else
          m_offset = event->pos();

      /// Hacemos la comprobacion de escalado hacia abajo.
      if (!g_bloqueo) {
          if (m_offset.x() > m_XScale -6 && m_offset.y() > m_YScale - 6) {
	      /// Abajo a la derecha
              m_escalando = 1;
          } else if (m_offset.x() > m_XScale /2-3 && m_offset.x() < m_XScale /2 +3 && m_offset.y() > m_YScale - 6) {
	      /// Abajo en el centro
              m_escalando = 2;
          } else if (m_offset.y() > m_YScale /2-3 && m_offset.y() < m_YScale /2 +3 && m_offset.x() > m_XScale - 6) {
	      /// A la derecha en el centro.
              m_escalando = 3;
          } else if (m_offset.x() < 6  && m_offset.y() < 6) {
              m_escalando = 4;
          } else if (m_offset.x() > m_XScale -6 && m_offset.y() < 6) {
              /// Arriba a la derecha
              m_escalando = 5;
          } else if (m_offset.y() > m_YScale -6 && m_offset.x() < 6) {
              /// Abajo a la izquierda
              m_escalando = 6;
          } else if (m_offset.y() > m_YScale /2-3 && m_offset.y() < m_YScale /2 +3 && m_offset.x() < 6) {
              /// Enmedio a la izquierda
              m_escalando = 7;
          } else if (m_offset.x() > m_XScale /2-3 && m_offset.x() < m_XScale /2 +3 && m_offset.y() < 6) {
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
	  /// Abajo a la derecha
          if (isWindow()) {
              m_XScale += (event->globalPos().x() - m_offset.x());
              m_YScale += (event->globalPos().y() - m_offset.y());
          } else {
	      m_XScale = event->pos().x();
	      m_YScale = event->pos().y();
          } // end if
        } else if (m_escalando == 2) {
	  /// Abajo en el centro.
          if (isWindow()) {
              m_YScale += (event->globalPos().y() - m_offset.y());
          } else {
	      m_YScale = event->pos().y();
          } // end if
        } else if (m_escalando == 3) {
	  /// A la derecha en el centro
          if (isWindow()) {
              m_XScale += (event->globalPos().x() - m_offset.x());
          } else {
	      m_XScale = event->pos().x();
          } // end if
        } else if (m_escalando == 4) {
          /// Arriba a la izquierda
          if (isWindow()) {
              m_XScale -= (event->globalPos().x() - m_offset.x());
              m_YScale -= (event->globalPos().y() - m_offset.y());
              move(event->globalPos() - m_offset);
          } else {
              m_XScale -= (event->pos().x() - m_offset.x());
              m_YScale -= (event->pos().y() - m_offset.y());
              move(mapToParent(event->pos() - m_offset));
          } // end if
        } else if (m_escalando == 5) {
          /// Arriba a la derecha
          if (isWindow()) {
              m_XScale += (event->globalPos().x() - m_offset.x());
              m_YScale -= (event->globalPos().y() - m_offset.y());
              QPoint rm_offset = event->globalPos() - m_offset;
              rm_offset.setX(x());
              move(rm_offset);
          } else {
	      m_XScale = event->pos().x();
              m_YScale -= (event->pos().y() - m_offset.y());
              QPoint rm_offset = event->pos() - m_offset;
              rm_offset.setX(mapFromParent(pos()).x());
              move(mapToParent(rm_offset));
          } // end if
        } else if (m_escalando == 6) {
          /// Abajo a la izquierda
          if (isWindow()) {
              m_XScale -= (event->globalPos().x() - m_offset.x());
              m_YScale += (event->globalPos().y() - m_offset.y());
              QPoint rm_offset = event->globalPos() - m_offset;
              rm_offset.setY(y());
              move(rm_offset);
          } else {
              m_XScale -= (event->pos().x() - m_offset.x());
	      m_YScale = event->pos().y();
              QPoint rm_offset = event->pos() - m_offset;
              rm_offset.setY(mapFromParent(pos()).y());
              move(mapToParent(rm_offset));
          } // end if
        } else if (m_escalando == 7) {
          /// Enmedio a la izquierda
          if (isWindow()) {
              m_XScale -= (event->globalPos().x() - m_offset.x());
              QPoint rm_offset = event->globalPos() - m_offset;
              rm_offset.setY(y());
              move(rm_offset);
          } else {
              m_XScale -= (event->pos().x() - m_offset.x());
              QPoint rm_offset = event->pos() - m_offset;
              rm_offset.setY(mapFromParent(pos()).y());
              move(mapToParent(rm_offset));
          } // end if
        } else if (m_escalando == 8) {   
          /// El cuadro de arriba en el centro
          if (isWindow()) {
              m_YScale -= (event->globalPos().y() - m_offset.y());
              QPoint rm_offset = event->globalPos() - m_offset;
              rm_offset.setX(x());
              move(rm_offset);
          } else {
              m_YScale -= (event->pos().y() - m_offset.y());
              QPoint rm_offset = event->pos() - m_offset;
              rm_offset.setX(mapFromParent(pos()).x());
              move(mapToParent(rm_offset));
          } // end if
        }

        if (m_XScale > 20 && m_YScale > 20)
          setFixedSize(m_XScale+10 ,m_YScale+10);
              
      } else if (m_escalando == 9) {
          if (isWindow())
              move(event->globalPos() - m_offset);
          else
              move(mapToParent(event->pos() - m_offset));
      } // end if
      event->accept(); // do not propagate
  } // end if
}

void Mesa::mouseReleaseEvent(QMouseEvent* event)
{
    BtTicket *ticket;
    BtCompany * emp = ( BtCompany * ) mainCompany();
  
    if (event -> button() == Qt::LeftButton) {
      if (!g_bloqueo) {
        event->accept(); // do not propagate
        if (m_escalando > 0) {
          m_escalando = 0;
          repaint();
        } // end if
      } else {
	
	if (g_selectMode) {
	  abrirMesa();
	} else {
	  
	  /// Comprueba que la mesa no este seleccionada previamente.
	  /// Si es el caso se deselecciona.
	  int indexNombreMesa = g_selectedTables.indexOf(m_nombreMesa);
	  
	  if (indexNombreMesa == -1) {
	    /// No esta en la lista.
	    /// Comprueba que la mesa este abierta. Si no no se puede seleccionar.	  
	    for ( int i = 0; i < emp->listaTickets() ->size(); ++i ) {
		ticket = emp->listaTickets() ->at ( i );
		if ( m_nombreMesa == ticket->dbValue ( "nomticket" )) {
		    g_selectedTables.push_back(m_nombreMesa);
		    break;
		} // end if
	    } // end for
	    
	  } else {
	    /// Esta en la lista. Se borra.
	    g_selectedTables.removeAt(indexNombreMesa);
	  } // end if
	  
	  update();
	  	  
	  if ((g_selectedTables.count() == 2) && g_joinTables) {
	      /// Se han seleccionado 2 mesas y esta activo el modo 'unir mesas'.
	      /// Encuentra a que 2 tickets corresponden.
	      BtTicket *ticket1;
	      BtTicket *ticket2;

	      for ( int i = 0; i < emp->listaTickets() ->size(); ++i ) {
		  ticket1 = emp->listaTickets() ->at ( i );
		  if ( g_selectedTables[0] == ticket1->dbValue ( "nomticket" ) ) {
		      break;
		  } // end if
	      } // end for
	      
	      for ( int i = 0; i < emp->listaTickets() ->size(); ++i ) {
		  ticket2 = emp->listaTickets() ->at ( i );
		  if ( g_selectedTables[1] == ticket2->dbValue ( "nomticket" ) ) {
		      break;
		  } // end if
	      } // end for
	      
	      emp->joinTickets(ticket1, ticket2);
	      
	      g_selectMode = true;
	      g_joinTables = false;
	      g_selectedTables.clear();
	      g_mesas->m_distro->mui_table_move -> setChecked (false);

	      update();
	      
	      abrirMesa();
	      
	  } // end if
	  
	} // end if
	
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
                    g_confpr->value( CONF_PROGDATA ) + "images/clipart/",
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
            emp->ticketActual() ->setDbValue( "bloqueadoticket", "false");
            
            /// Llamamos a plugins para poder hacer lo pertinente
            g_plugins->run("Abrevs_on_mui_aparcar_clicked", this);



            /// Ponemos al trabajador creado el trabajador del ticket actual.
            emp->setTicketActual ( ticket );
            /// Borra el valor del Input.
            emp->pulsaTecla ( Qt::Key_F4, "" );

            /// Llamamos a plugins para poder hacer lo pertinente
            g_plugins->run("Abrevs_on_mui_aparcar_clicked_Post", this);
            
            /// Ponemos el nuevo bloqueo
            ticket->setDbValue("bloqueadoticket", "true");
            
            ticket->pintar();


            g_mesas->on_mui_mesas_clicked();


            return;
        }// end if
    }// end for


    BtTicket *tick;


    tick = emp->newBtTicket();
    tick->setDbValue ( "idtrabajador", emp->ticketActual() ->dbValue ( "idtrabajador" ) );
    emp->setValorBtInput(m_nombreMesa);

    emp->listaTickets() ->insert ( 0, tick );

    /// El nombre del ticket a aparcar lo coge del BtInput.
    tick ->setDbValue ( "nomticket", m_nombreMesa );

    /// Quitamos el bloqueo
    emp->ticketActual() ->setDbValue( "bloqueadoticket", "false");
    
    /// Llamamos a plugins para poder hacer lo pertinente
     g_plugins->run("Abrevs_on_mui_aparcar_clicked", this);



    /// Ponemos al trabajador creado el trabajador del ticket actual.
    emp->setTicketActual ( tick );
    /// Borra el valor del Input.
    emp->pulsaTecla ( Qt::Key_F4, "" );

    /// Llamamos a plugins para poder hacer lo pertinente
    g_plugins->run("Abrevs_on_mui_aparcar_clicked_Post", this);
    
    /// Ponemos el nuevo bloqueo
    tick->setDbValue("bloqueadoticket", "true");
    
    tick->pintar();

    g_mesas->on_mui_mesas_clicked();
}




QString Mesa::exportXML() {
  QString val;
  val = "<MESA>\n";
  val += "\t<IMAGEN>" + m_filename + "</IMAGEN>\n";
  val += "\t<NOMBRE>" + m_nombreMesa + "</NOMBRE>\n";
  val += "\t<PANTALLA>" + m_pantalla + "</PANTALLA>\n";
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
	
        return;
    } // end if

    QDomElement docElem = doc.documentElement();
    QDomElement principal = docElem.firstChildElement ( "IMAGEN" );
    m_filename = principal.text();

    QDomElement nombre = docElem.firstChildElement ( "NOMBRE" );
    m_nombreMesa = nombre.text();

    QDomElement pantalla = docElem.firstChildElement ( "PANTALLA" );
    m_pantalla = pantalla.text();
    
    
    QDomElement posx = docElem.firstChildElement ( "POSX" );
    QDomElement posy = docElem.firstChildElement ( "POSY" );

    QDomElement xscale = docElem.firstChildElement ( "XSCALE" );
    QDomElement yscale = docElem.firstChildElement ( "YSCALE" );
    m_XScale = xscale.text().toInt();
    m_YScale = yscale.text().toInt();
    setGeometry(posx.text().toInt(),posy.text().toInt(),m_XScale+1 , m_YScale+1 );
    setFixedSize(m_XScale+10 ,m_YScale+10);

    repaint();
}
