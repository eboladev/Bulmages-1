#include <listventanas.h>
#include <qobject.h>
//Added by qt3to4:
#include <QPixmap>

#include <map>
#include <q3ptrlist.h>

#include "bulmafact.h"
#include "funcaux.h"

extern bulmafact *bges;

using namespace std;
typedef QObject *pQObject;

typedef map<int, pQObject> mapaw;

mapaw elmapa;
listventanas::listventanas(const QString & title,  QWidget *a, Qt::WFlags b): Q3DockWindow (a,title,b) {
  _depura("listventanas::INIT_listventanas()\n",0);

  m_listBox = new Q3ListBox( 0 ,0 );
  m_listBox->setCaption( tr( "Ventanas Abiertas") );
  m_listBox->setGeometry(0,0,100,500);
  m_listBox->showMaximized();
  setCloseMode(Q3DockWindow::Always);
  setWidget(m_listBox);
  showMaximized();
  setResizeEnabled(TRUE);
  setMovingEnabled(TRUE);
  connect(m_listBox, SIGNAL(doubleClicked(Q3ListBoxItem *)), this, SLOT(dclicked()));
  connect(m_listBox, SIGNAL(clicked(Q3ListBoxItem *)), this, SLOT(clicked()));
  _depura("listventanas::END_listventanas()\n",0);
}// end listventanas

void listventanas::dclicked() {
  _depura("listventanas::clicked()\n",0);
  int item = m_listBox->currentItem();
  QWidget *widget = (QWidget *)elmapa[item];
  if (widget != NULL)
  {
      widget->hide();
	  widget->showMaximized();	  

  }// end if

  _depura("END listventanas::clicked()\n",0);
}// end clicked



void listventanas::clicked()
{
  _depura("listventanas::clicked()\n",0);
  mapaw::iterator iterador = elmapa.begin();
  int item = m_listBox->currentItem();
  QWidget *widget = (QWidget *)elmapa[item];
  if (widget != NULL)
  {
    //        fprintf(stderr,"El estado de la ventana es: %d, deberia ser %d\n",widget->windowState(),Qt::WindowMaximized);
    QWidget *punt = bges->workspace()->activeWindow();
    if (widget != punt)
    {
	  widget->hide();
      widget->showNormal();
    }
  }// end if



  fprintf(stderr,"END listventanas::clicked()\n");
}// end clicked

listventanas::~listventanas()
{
  _depura("listventanas::destructor()\n",0);
  delete m_listBox;
}// end ~listventanas

#include <qimage.h>

/** \brief This function provides the ability of push a window in the dock window
 ** \param  nombre This QStrin is the name of the window that was shown in the listbox
 ** \param obj This QObject * contains the pointer of the window for furtner reference.
 */
int  listventanas::meteWindow(QString nombre, QObject *obj)
{
  _depura("listventanas::INIT_meteWindow()\n",0);
  mapaw::iterator iterador = elmapa.begin();
  while (iterador != elmapa.end())
  {
    if ((*iterador).second == obj) {
      m_listBox->removeItem((*iterador).first);
      elmapa.erase((*iterador).first);
    }// end if

    /// Comprobamos ventanas duplicadas y las fulminamos.
    if (m_listBox->text((*iterador).first) == nombre) {
 	 _depura("hay una duplicada y la cerramos()\n",0);
	((QWidget *)obj)->close();
	 _depura("mostramos la original()\n",0);
	((QWidget *)(*iterador).second)->hide(); 
	((QWidget *)(*iterador).second)->show(); 
 	 _depura("Establecemos la nueva primaria()\n",0);
	m_listBox->setCurrentItem((*iterador).first);
	return 1;
    }// end if


    iterador++;
  }// end while
  if( iterador == elmapa.end())
  {
    if (((QWidget *)obj)->icon())
    {
      QPixmap icon = *((QWidget *)obj)->icon();
      QImage imgicon = icon.convertToImage();
      imgicon = imgicon.scaled(32,32);
      icon = imgicon;
      m_listBox->insertItem(icon,nombre,-1);
    }
    else
    {
      m_listBox->insertItem(nombre,-1);
    }// end if
    elmapa[m_listBox->count()-1]=obj;
  }// end if
  _depura("listventanas::END_meteWindow()\n",0);
  return 0;
}// end meteWindow


void listventanas::sacaWindow(QObject *obj)
{
  _depura("listventanas::INIT_sacaWindow()\n",0);

  mapaw mapaaux;

  /// Buscamos la ventana correspondiente y la borramos.
  mapaw::iterator iterador = elmapa.begin();
  while (iterador != elmapa.end())
  {
    if ((*iterador).second == obj)
    {
      elmapa.erase((*iterador).first);
      m_listBox->removeItem((*iterador).first);
      break;
    }
    iterador++;
  }// end while

  /// Reorganizamos los elementos para que el listbox corresponda con el mapa.
  iterador = elmapa.begin();
  int i = 0;
  while (iterador != elmapa.end())
  {
    mapaaux[i++]= (*iterador).second;
    iterador++;
  }// end while

  /// Borramos el mapa para que no haya elementos basura.
  elmapa.clear();

  /// Rehacemos el mapa a partir de mapaaux.
  iterador = mapaaux.begin();
  while (iterador != mapaaux.end())
  {
    elmapa[(*iterador).first] = (*iterador).second;
    iterador ++;
  }// end while

  _depura("listventanas::END_sacaWindow()\n",0);

}// end sacaWindow

