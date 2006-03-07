#include <stdio.h>
#include "plugintarifas.h"
#include "company.h"
#include "funcaux.h"

#include <q3popupmenu.h>
#include <qaction.h>
#include <qobject.h>
#include <qmessagebox.h>



void entryPoint(bulmafact *bges) {
    _depura("Estoy dentro del plugin\n",0);
    bges->setCaption("Prueba de plugin Tarifas.");
}


