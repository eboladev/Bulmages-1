# -*- coding: utf-8 -*-

import sys
import os
from PyQt4.QtGui import *
from PyQt4.QtCore import *
from editarusuariobase import *
from empresa import Empresa
from config import *
import functions

import psycopg2

class EditarUsuario(Ui_EditarUsuario, Empresa):

    def __init__(self, parent = None):
        Empresa.__init__(self,parent)
        self.setupUi(self)
        self.proceso = QtCore.QProcess()
        self.lineEdit.setVisible(False)
        self.initListaUsuarios()

        self.connect(self.CheckBox_password, SIGNAL("stateChanged(int)"), self.activaTexto)
        self.connect(self.listWidget, SIGNAL("itemSelectionChanged()"), self.activaGuardar)

    def initListaUsuarios(self):
        self.listWidget.clear()
        self.aplicar_cambios.setEnabled(False)
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

    def activaTexto(self):
        if (self.CheckBox_password.isChecked()):
            self.lineEdit.setVisible(True)
        else:
            self.lineEdit.setVisible(False)

    def activaGuardar(self):
        self.aplicar_cambios.setEnabled(True)

    def on_aplicar_cambios_released(self):
	# Cogemos el numero de elementos de la lista para iterar sobre el.
        numero = self.listWidget.count()

	# Quitamos las siglas de superusuario ya que no son parte del nombre del usuario y necesitamos su nombre
        for x in range (numero):
            temp = self.listWidget.item(x)
            if (temp.isSelected()):
                self.username = temp.text()
                if self.username.contains("  (su)"):
                    self.username.remove("  (su)")
                break
                
        # Si hemos elegido cambiar el password lo cambiamos.
        if (self.CheckBox_password.isChecked()):
            self.password = self.lineEdit.text()
            if os.name == 'posix':
                self.execCommand(functions.psql + " -c \\\"ALTER ROLE " + str(self.username) + " WITH PASSWORD '" + str(self.password) + "'\\\"" + " template1" + functions.end_sql)
            else:
                self.execCommand(functions.psql + " -c \"ALTER ROLE " + str(self.username) + " WITH PASSWORD \"" + str(self.password) + "\"\"" + " template1")
            
        # Ejecutamos la alteracion de superusuario segun este el checkbox activado o no.
        if (self.Radial_su.isChecked()):
	    if os.name == 'posix':
		self.execCommand(functions.psql + " -c 'ALTER ROLE " + str(self.username) + " WITH superuser'"  + " template1" + functions.end_sql)
	    else:
		self.execCommand(functions.psql + " -c \"ALTER ROLE " + str(self.username) + " WITH superuser\""  + " template1")
        else:
	    if os.name == 'posix':
		self.execCommand(functions.psql + " -c 'ALTER ROLE " + str(self.username) + " WITH nosuperuser'" + " template1" + functions.end_sql)
	    else:
		self.execCommand(functions.psql + " -c \"ALTER ROLE " + str(self.username) + " WITH nosuperuser\"" + " template1")
		
	# Repintamos la lista de usuarios para que muestre los nuevos cambios
        self.initListaUsuarios()

    def execCommand(self, command):
        self.proceso.start(command)
        self.proceso.waitForFinished(-1)

def main(args):
    app=QtGui.QApplication(args)
    win=EditarUsuario()
    win.exec_()
    sys.exit(app.exec_())

if __name__=="__main__":
    main(sys.argv)
