# -*- coding: utf-8 -*-

import sys
import os
from PyQt4.QtGui import *
from PyQt4.QtCore import *
from restbackupbase import *

class RestBackup(QtGui.QDialog, Ui_RestBackupBase):
    def __init__(self, parent = None):
        QtGui.QDialog.__init__(self,parent)
	self.setupUi(self)
	self.proceso = QtCore.QProcess()	
	
    def on_mui_aceptar_released(self):
    
	self.command = 'tar xzf ' + self.mui_filename.text() + ' --directory /'
	self.proceso.start(self.command)
	self.proceso.waitForFinished(-1)
	
	self.nomdb = self.mui_dbname.text()
	
	# Creamos la base de datos
	self.command = 'su postgres -c "createdb -E UNICODE ' + self.nomdb +'"'
	self.proceso.start(self.command)
	self.proceso.waitForFinished(-1)
	
	# Cargamos la esquematica de la base de datos
	self.command = 'su postgres -c \"psql  -f /etc/bulmages/' + self.database + '.sql ' + self.nomdb + ' \"'
	self.proceso.start(self.command)
	self.proceso.waitForFinished(-1)
	
	# Borramos el sql recien extraido en /etc/bulmages, ya que se conserva una copia dentro del backup
	self.command = 'rm /etc/bulmages/' + self.database + '.sql'
	self.proceso.start(self.command)
	self.proceso.waitForFinished(-1)
	
	Yes = 'Ok'
	error = QtGui.QMessageBox(self)
	error.setWindowTitle('Restauracion de Backup')
	error.addButton(Yes, QtGui.QMessageBox.AcceptRole)
	error.setText('<b>El backup ha sido restaurado correctamente</b>')
	error.exec_()
	
	self.accept()

    def on_mui_filesearch_released(self):
	self.openfile = QFileDialog.getOpenFileName(self,"Restaurar - Elija archivo de backup","/home","TAR.GZ (*.tar.gz)")
	self.mui_filename.setText(self.openfile)

	self.arra = self.openfile.split('/')
	self.database = self.arra[-1]
	if self.database.contains('.tar.gz'):
	  self.database.remove('.tar.gz')
	self.mui_dbname.setText(self.database)
	
def main(args):
    app=QtGui.QApplication(args)
    win=RestBackup()
    win.exec_()
    sys.exit(app.exec_())

if __name__=="__main__":
    main(sys.argv)