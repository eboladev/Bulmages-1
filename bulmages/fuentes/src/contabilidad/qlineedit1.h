//
//
// C++ Interface: $MODULE$
//
// Description: 
//
//
// Author: Tomeu Borrás Riera <tborras@conetxia.com>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef _QLINEEDIT1_H_
#define _QLINEEDIT1_H_

#include <qlineedit.h>

/*
#include <Foundation/NSObject.h>

@interface QLineEdit1 : QLineEdit
@end

#endif
*/

class QLineEdit1: public QLineEdit {
public:
	int hola;
	QLineEdit1(QWidget *, const char *); 
	~QLineEdit1();
};// end QLineEdit1


#endif


