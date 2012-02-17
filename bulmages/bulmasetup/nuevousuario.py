# -*- coding: utf-8 -*-

import sys
import os
from PyQt4.QtGui import *
from PyQt4.QtCore import *
from nuevousuariobase import *
from empresa import Empresa
from config import *
import psycopg2
import functions

class NuevoUsuario(Ui_NuevoUsuario, Empresa):

    def __init__(self, parent = None):
        Empresa.__init__(self,parent)
        self.setupUi(self)
        self.proceso = QtCore.QProcess()

    def on_mui_botonera_accepted(self):
        # Creamos el usuario

        if (self.mui_superusuario.isChecked()):
            if os.name == 'posix':
                self.execComando(functions.createuser + " -s -d -r  \'" + self.mui_nombre.text() + "\'" + functions.end_sql)
            else:
                self.execComando(functions.createuser + " -s -d -r  " + self.mui_nombre.text() + functions.end_sql)
            
        else:
            if os.name == 'posix':
                self.execComando(functions.createuser + " -S -d -r  \'" + self.mui_nombre.text() + "\'" + functions.end_sql)
            else:
                self.execComando(functions.createuser + " -S -d -r  " + self.mui_nombre.text() + functions.end_sql)

            
        # Cambiamos el password del usuario
        self.execComando(functions.psql + " template1 -c \"ALTER ROLE " + str(self.mui_nombre.text()) + " WITH PASSWORD '" + str(self.mui_password.text()) + "'\"")

        # Y como ya hemos acabado, cerramos la aplicacion.
        self.accept()

    def execComando(self, command):
        self.proceso.start(command)
        self.proceso.waitForFinished(-1)

def main(args):
    app=QtGui.QApplication(args)
    win=NuevoUsuario()
    win.exec_()
    sys.exit(app.exec_())

if __name__=="__main__":
    main(sys.argv)
