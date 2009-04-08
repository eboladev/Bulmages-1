# -*- coding: utf-8 -*-

import sys
import os
from PyQt4.QtGui import *
from PyQt4.QtCore import *
from gestionusuariosbase import *
from nuevousuario import NuevoUsuario
from modificarusuario import ModificarUsuario
from eliminarusuario import EliminarUsuario

class GestionUsuarios(QtGui.QDialog, Ui_GestionUsuarios):

    def __init__(self, parent = None):
        QtGui.QDialog.__init__(self,parent)
        self.setupUi(self)
        self.process = QtCore.QProcess()

    # Evento de hacer clic en el boton para crear un nuevo usuario
    def on_crear_usuario_postgres_released(self):
            win = NuevoUsuario()
            win.exec_()
            
    # Evento de hacer clic en el boton para crear un nuevo usuario
    def on_editar_usuario_postgres_released(self):
            win = ModificarUsuario()
            win.exec_()
            
    # Evento de hacer clic en el boton para eliminar un usuario
    def on_eliminar_usuario_postgres_released(self):
            win = EliminarUsuario()
            win.exec_()

def main(args):
    app=QtGui.QApplication(args)
    win=GestionUsuarios()
    win.exec_()
    sys.exit(app.exec_())

if __name__=="__main__":
    main(sys.argv)
