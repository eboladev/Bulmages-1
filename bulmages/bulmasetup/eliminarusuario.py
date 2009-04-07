# -*- coding: utf-8 -*-

import sys
import os
from PyQt4.QtGui import *
from PyQt4.QtCore import *
from eliminarusuariobase import *
# Libreria de acceso a bases de datos PostgreSQL
import psycopg2

class EliminarUsuario(QtGui.QDialog, Ui_EliminarUsuario):

    def __init__(self, parent = None):
        QtGui.QDialog.__init__(self,parent)
	self.setupUi(self)
	self.process = QtCore.QProcess()
        self.initListaUsuarios()

    def initListaUsuarios(self):
        try:
            # Aqui puede verse que hace falta agregar al usuario root como
            # superusuario de bases de datos de postgres para que este script
            # funcione correctamente. Con el usuario postgres basta con poner:
            # createuser root
            conn = psycopg2.connect("dbname='template1' user='root'")
        except:
            print "Fallo en la conexion con PostgreSQL."
            sys.exit()

        cur = conn.cursor()
        
        try:
            cur.execute("SELECT * FROM pg_user")
        except:
            print "Fallo en la consulta para obtener los usuarios de PostgreSQL del sistema."
            sys.exit()
            
        usuarios = cur.fetchall()
        texto = ""
        
        # Rellenamos la lista con los usuarios de PostgreSQL
        for row in usuarios:
            texto = row[0]
            
            if (row[3] == True):
                texto = texto + "  (su)"
            
            self.listWidget.addItem(QString(texto))
            
    def on_mui_eliminarusuario_released(self):
        
        Yes = 'Si'
        No = 'No'
        respuesta = False
        
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
