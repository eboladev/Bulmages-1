/***************************************************************************
                          aboutview.h  -  description
                             -------------------
    begin                : Fri Nov 27 2005
    copyright            : (C) 2005 by Tomeu Borrás Riera
    email                : tborras@conetxia.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef ABOUTVIEW_H
#define ABOUTVIEW_H

#include <QDialog>
#include <about.h>

/**
  *@author Tomeu Borrás Riera
 * \class aboutview aboutview.h
 * \brief Presentar el About del programa
 *  
 * Esta clase muestra el dialogo de About. Presentan los créditos del programa.
 * this class presents the about of the program.
 */

class aboutview : public QDialog, public Ui::aboutdlg  {
public: 
	aboutview(QDialog *parent=0);  /// Constructor de la clase
	~aboutview();
};

#endif
