# -*- coding: utf-8 -*-

import sys
import os
from PyQt4.QtGui import *
from PyQt4.QtCore import *
from eliminarusuariobase import *
from empresa import Empresa
from config import *
# Libreria de acceso a bases de datos PostgreSQL
import psycopg2

class EliminarUsuario(Ui_EliminarUsuario, Empresa):

    def __init__(self, parent = None):
        Empresa.__init__(self,parent)
        self.setupUi(self)
        self.process = QtCore.QProcess()
        self.initListaUsuarios()

    def initListaUsuarios(self):
        self.conectar('template1')
        usuarios = self.execute("SELECT * FROM pg_user")
        texto = ""
        
        # Rellenamos la lista con los usuarios de PostgreSQL
        for row in usuarios:
            texto = row[0]
            if (str(texto) != 'root') and (str(texto) != 'postgres'):
                if (row[3] == True):
                    texto = texto + "  (su)"                
                self.listWidget.addItem(QString(texto))
            
    def on_mui_eliminarusuario_released(self):
        
        Yes = 'Si'
        No = 'No'
        message = QtGui.QMessageBox(self)
        message.setText('Seguro que deseas eliminar el usuario ?')
        message.setWindowTitle('Confirmacion')
        message.setIcon(QtGui.QMessageBox.Question)
        message.addButton(Yes, QtGui.QMessageBox.AcceptRole)
        message.addButton(No, QtGui.QMessageBox.RejectRole)
        message.exec_()
        respuesta = message.clickedButton().text()
        
        if respuesta == Yes:
            
            numero = self.listWidget.count()
            temp = QtGui.QListWidgetItem()
            
            for x in range (numero):
                    temp = self.listWidget.item(x)
                    
                    if (temp.isSelected()):
                            self.username = temp.text()
                            self.listWidget.takeItem(x)
                            break
                            
            if self.username.contains("  (su)"):
                    self.username.remove("  (su)")
                    
            self.comando = 'su postgres -c "dropuser ' + str(self.username) + '"'
            self.process.start(self.comando)
            self.process.waitForFinished(-1)
        
def main(args):
    app=QtGui.QApplication(args)
    win=EliminarUsuario()
    win.exec_()
    sys.exit(app.exec_())

if __name__=="__main__":
    main(sys.argv)
