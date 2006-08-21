/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef __DIALOGCHANGES__
#define __DIALOGCHANGES__

#include <QObject>

#include "funcaux.h"


/// Esta clase se encarga de proporcionar algo de infraestructura al control de cambios.
/// La idea original es de Alvaro de Miguel en la que lo que hace la funcion es
/// coger todos los campos de un determinado formulario y formar una cadena.
/// Luego para saber si hay cambios en el formulario vuelve a comparar.
class dialogChanges {

private:
    int m_maxQLine;
    int m_maxQText;
    int m_maxQTable;
    QString valorinicial;
    QObject *m_obje;
    QObject *m_listaQLine[999];
    QObject *m_listaQText[999];
    QObject *m_listaQTable[999];

public:
    dialogChanges(QObject *ob) {
        _depura("dialogChanges::dialogChanges", 0);
        m_obje = ob;
        _depura("END dialogChanges::dialogChanges", 0);

    };
    ~dialogChanges() {}
    ;
    void dialogChanges_cargaInicial();
    bool dialogChanges_hayCambios();

private:
    QString calculateValues();
    QString retrieveValues(QString qsWidget);
};

#endif

