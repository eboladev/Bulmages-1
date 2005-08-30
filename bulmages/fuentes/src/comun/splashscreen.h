#ifndef _SPLASH_H
#define _SPLASH_H

#include <qlabel.h>
#include <qtextbrowser.h>
#include <qdialog.h>
#include <qtextbrowser.h>


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
 QTextBrowser *l2;
public:
   Splash();
   ~Splash();
public slots:
   virtual void paint();
   virtual bool event( QEvent *);
};
#endif // _SPLASH_H
