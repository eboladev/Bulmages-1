#ifndef _SPLASH_H
#define _SPLASH_H

#include <qdialog.h>
#include <qpixmap.h>
#include <qstring.h>
#include <qprogressbar.h>
#include <qlabel.h>
#include <stdio.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qprogressbar.h>
#include <qmessagebox.h>
#include <qpixmap.h>
#include <stdlib.h>
#include <qtimer.h>


#include <qtextbrowser.h>

class Splash : public QDialog
{
  Q_OBJECT
private:
 QLabel *l;
// QLabel *l2;
QTextBrowser *l2;
public:
   Splash();
   ~Splash();
public slots:
   virtual void paint();
   virtual bool event( QEvent *);
};
#endif // _SPLASH_H
