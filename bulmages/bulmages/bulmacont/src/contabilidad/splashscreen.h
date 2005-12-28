#ifndef _SPLASH_H
#define _SPLASH_H

#include <qlabel.h>
#include <q3textbrowser.h>
#include <qdialog.h>
#include <q3textbrowser.h>
//Added by qt3to4:
#include <QEvent>


/** 
  * @author Tomeu Borrás Riera 
  * \class splashscreen splashscreen.h
  * \brief Pantalla de bienvenida
  *
  * Clase que muestra la imagen del splash y algunos mensajes chorras. 
  * Esta clase no tiene mucha utilidad, se utiliza para dar mayor apariencia al programa.
  */

class Splash : public QDialog
{
  Q_OBJECT
private:
 QLabel *l;
 Q3TextBrowser *l2;
public:
   Splash();
   ~Splash();
public slots:
   virtual void paint();
   virtual bool event( QEvent *);
};
#endif // _SPLASH_H
