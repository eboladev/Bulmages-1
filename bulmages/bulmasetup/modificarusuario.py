# -*- coding: utf-8 -*-

import sys
import os
from PyQt4.QtGui import *
from PyQt4.QtCore import *
from modificarusuariobase import *
from empresa import Empresa
from config import *
import psycopg2

class ModificarUsuario(Ui_ModificarUsuario, Empresa):

    def __init__(self, parent = None):
        Empresa.__init__(self,parent)
        self.setupUi(self)
        self.proceso = QtCore.QProcess()
        self.inicio()
        global username

    def inicio(self): 
        self.tabWidget.setTabEnabled(1, False)
        self.tabWidget.setTabEnabled(2, False)
        self.tabWidget.setTabEnabled(3, False)
        self.tabWidget.setCurrentIndex(0)
        self.initListaDatabase()

        self.connect(self.checkBox_select_all, SIGNAL("stateChanged(int)"), self.checkBox_todos)
        self.connect(self.checkBox_select_all_2, SIGNAL("stateChanged(int)"), self.checkBox_todos_2)
        self.connect(self.checkBox_dball, SIGNAL("stateChanged(int)"), self.checkBox_dball_Change)
        self.connect(self.checkBox_dbrevoke, SIGNAL("stateChanged(int)"), self.checkBox_dbrevoke_Change)
        self.connect(self.checkBox_all, SIGNAL("stateChanged(int)"), self.checkBox_all_Change)
        self.connect(self.checkBox_revoke, SIGNAL("stateChanged(int)"), self.checkBox_revoke_Change)
        self.connect(self.checkBox_all_2, SIGNAL("stateChanged(int)"), self.checkBox_all_2_Change)
        self.connect(self.checkBox_revoke_2, SIGNAL("stateChanged(int)"), self.checkBox_revoke_2_Change)
        self.connect(self.proceso, SIGNAL("readyReadStandardError()"), self.readErrors)
        self.connect(self.listWidgetDatabase, SIGNAL("itemSelectionChanged()"), self.capturaDatabase)
        self.connect(self.listWidgetUser, SIGNAL("itemSelectionChanged()"), self.capturaUsuario)
        self.connect(self.listWidgetTable, SIGNAL("itemSelectionChanged()"), self.actualizarCheckboxTable)
                    
    def initListaDatabase(self):
        self.conectar('template1')
        dbs = self.execute("SELECT datname FROM pg_database ORDER BY datname")
                
        # Recorremos todas las bases de datos a partir del array
        for row in dbs:
        
            # Conectamos a cada base de datos excepto a template0 que no permite conexiones
            if str(row[0]) != 'template0':
                self.conectar( str(row[0]) )
                tipo = self.executeone("select valor from configuracion where nombre = 'Tipo'")
                    
                # Rellenamos la lista de bases de datos.
                if str(tipo) == "('BulmaFact',)":
                    texto = "BulmaFact   -  " + str(row[0])            
                    self.listWidgetDatabase.addItem(QString(texto))
                    
                if str(tipo) == "('BulmaCont',)":
                    texto = "BulmaCont  -  " + str(row[0])            
                    self.listWidgetDatabase.addItem(QString(texto))
        
    def capturaDatabase(self):
        self.listWidgetUser.clear()
    
        # Pasamos el nombre de la base de datos seleccionada en listWidgetDatabase a la variable database
        numero = self.listWidgetDatabase.count()
        temp = QtGui.QListWidgetItem()
        global dbase
        
        for x in range (numero):
            temp = self.listWidgetDatabase.item(x)
            
            if (temp.isSelected()):
                dbase = temp.text()
                if dbase.contains("BulmaFact   -  "):
                    dbase.remove("BulmaFact   -  ")
                if dbase.contains("BulmaCont  -  "):
                    dbase.remove("BulmaCont  -  ")
                break

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
                self.listWidgetUser.addItem(QString(texto))
            
    def capturaUsuario(self): 
        # Vaciamos la lista de tablas, ya que esta se regenera cada vez que cambiamos el usuario seleccionado
        self.listWidgetTable.clear()
        self.listWidgetSecuencias.clear()
        # Pasamos el nombre del usuario seleccionado en listWidgetUser a la variable username
        numero = self.listWidgetUser.count()
        temp = QtGui.QListWidgetItem()
        global username
        
        for x in range (numero):
            temp = self.listWidgetUser.item(x)
                
            if (temp.isSelected()):
                username = temp.text()
                if username.contains("  (su)"):
                    username.remove("  (su)")
                break
            
        self.tabWidget.setTabEnabled(1, True)
        self.tabWidget.setTabEnabled(2, True)
        self.capturaDatabase
        self.initListaTablas()
        self.initListaSecuencias()
        self.actualizarCheckboxDB()
        
        for x in range (numero):
            temp = self.listWidgetUser.item(x)
            if (temp.isSelected()):
                self.tabWidget.setCurrentIndex(1)
                
        self.on_deseleccionarTablas_released()

    def initListaTablas(self):
        self.conectar(str(dbase))
        tablas = self.execute("SELECT relname FROM pg_class WHERE relkind = 'r' AND relname NOT LIKE ('pg_%') AND relname NOT LIKE ('sql_%') ORDER BY relname")
              
        # Rellenamos la lista con lablas de la base de datos seleccionada
        for row in tablas:
            texto = row[0]                        
            self.listWidgetTable.addItem(QString(texto))
   
    def initListaSecuencias(self):
        self.conectar(str(dbase))
        secuencias = self.execute("SELECT relname FROM pg_class WHERE relkind = 'S' AND relname NOT LIKE ('pg_%') AND relname NOT LIKE ('sql_%') AND relname LIKE ('%_seq') ORDER BY relname")
                  
        # Rellenamos la lista con las secuencias
        for row in secuencias:
            texto = row[0]                        
            self.listWidgetSecuencias.addItem(QString(texto))

        
    def on_seleccionarTablas_released(self):
        numero = self.listWidgetTable.count()
        for x in range (numero):
            self.listWidgetTable.item(x).setSelected(True)

    def on_deseleccionarTablas_released(self):
        numero = self.listWidgetTable.count()
        for x in range (numero):
            self.listWidgetTable.item(x).setSelected(False)
            
    def on_seleccionarSeq_released(self):
        numero = self.listWidgetSecuencias.count()
        for x in range (numero):
            self.listWidgetSecuencias.item(x).setSelected(True)

    def on_deseleccionarSeq_released(self):
        numero = self.listWidgetSecuencias.count()
        for x in range (numero):
            self.listWidgetSecuencias.item(x).setSelected(False)

    def actualizarCheckboxDB(self):
        #Conectamos con la base de datos
        self.conectar( str(dbase))
        permiso = self.execute("select has_database_privilege('" + str(username) + "', '" + str(dbase) + "', 'create');")
        if str(permiso) == "(True,)":
            self.checkBox_create.setCheckState(Qt.Checked)
        else:
            self.checkBox_create.setCheckState(Qt.Unchecked)    
            
        permiso = self.execute("select has_database_privilege('" + str(username) + "', '" + str(dbase) + "', 'temporary');")
        if str(permiso) == "(True,)":
            self.checkBox_temporary.setCheckState(Qt.Checked)
        else:
            self.checkBox_temporary.setCheckState(Qt.Unchecked)

        
    def actualizarCheckboxTable(self):
        temp = self.listWidgetTable.currentItem()
        if (temp != None):
            table = temp.text()
            
            #Conectamos con la base de datos
            self.conectar(str(dbase))

            permiso = self.executeone("select has_table_privilege('" + str(username) + "', '" + str(table) + "', 'select');")
            if str(permiso) == "(True,)":
                self.checkBox_select.setCheckState(Qt.Checked)
            else:
                self.checkBox_select.setCheckState(Qt.Unchecked)    
                
            permiso = self.executeone("select has_table_privilege('" + str(username) + "', '" + str(table) + "', 'insert');")
            if str(permiso) == "(True,)":
                self.checkBox_insert.setCheckState(Qt.Checked)
            else:
                self.checkBox_insert.setCheckState(Qt.Unchecked)
                
            permiso = self.executeone("select has_table_privilege('" + str(username) + "', '" + str(table) + "', 'update');")
            if str(permiso) == "(True,)":
                self.checkBox_update.setCheckState(Qt.Checked)
            else:
                self.checkBox_update.setCheckState(Qt.Unchecked)

            permiso = self.executeone("select has_table_privilege('" + str(username) + "', '" + str(table) + "', 'delete');")
            if str(permiso) == "(True,)":
                self.checkBox_delete.setCheckState(Qt.Checked)
            else:
                self.checkBox_delete.setCheckState(Qt.Unchecked)        

            permiso = self.executeone("select has_table_privilege('" + str(username) + "', '" + str(table) + "', 'references');")
            if str(permiso) == "(True,)":
                self.checkBox_references.setCheckState(Qt.Checked)
            else:
                self.checkBox_references.setCheckState(Qt.Unchecked)              

            permiso = self.executeone("select has_table_privilege('" + str(username) + "', '" + str(table) + "', 'trigger');")
            if str(permiso) == "(True,)":
                self.checkBox_trigger.setCheckState(Qt.Checked)
            else:
                self.checkBox_trigger.setCheckState(Qt.Unchecked)   
        
    def checkBox_todos(self):
        if (self.checkBox_select_all.isChecked()):
            self.checkBox_select.setCheckState(Qt.Checked)
            self.checkBox_insert.setCheckState(Qt.Checked)
            self.checkBox_update.setCheckState(Qt.Checked)
            self.checkBox_delete.setCheckState(Qt.Checked)
            self.checkBox_references.setCheckState(Qt.Checked)
            self.checkBox_trigger.setCheckState(Qt.Checked)
        else:
            self.checkBox_select.setCheckState(Qt.Unchecked)
            self.checkBox_insert.setCheckState(Qt.Unchecked)
            self.checkBox_update.setCheckState(Qt.Unchecked)
            self.checkBox_delete.setCheckState(Qt.Unchecked)
            self.checkBox_references.setCheckState(Qt.Unchecked)
            self.checkBox_trigger.setCheckState(Qt.Unchecked)
            
    def checkBox_todos_2(self):
        if (self.checkBox_select_all_2.isChecked()):
            self.checkBox_usage.setCheckState(Qt.Checked)
            self.checkBox_select2.setCheckState(Qt.Checked)
            self.checkBox_update2.setCheckState(Qt.Checked)
        else:
            self.checkBox_usage.setCheckState(Qt.Unchecked)
            self.checkBox_select2.setCheckState(Qt.Unchecked)
            self.checkBox_update2.setCheckState(Qt.Unchecked)
            
    def checkBox_dball_Change(self):
        if (self.checkBox_dball.isChecked()):
            self.checkBox_dbrevoke.setCheckState(Qt.Unchecked)
            self.checkBox_create.setCheckState(Qt.Checked)
            self.checkBox_temporary.setCheckState(Qt.Checked)
            
    def checkBox_dbrevoke_Change(self):
        if (self.checkBox_dbrevoke.isChecked()):
            self.checkBox_dball.setCheckState(Qt.Unchecked)
            self.checkBox_create.setCheckState(Qt.Unchecked)
            self.checkBox_temporary.setCheckState(Qt.Unchecked)
            
    def checkBox_all_Change(self):
        if (self.checkBox_all.isChecked()):
            self.checkBox_revoke.setCheckState(Qt.Unchecked)
            self.checkBox_select.setCheckState(Qt.Checked)
            self.checkBox_insert.setCheckState(Qt.Checked)
            self.checkBox_update.setCheckState(Qt.Checked)
            self.checkBox_delete.setCheckState(Qt.Checked)
            self.checkBox_references.setCheckState(Qt.Checked)
            self.checkBox_trigger.setCheckState(Qt.Checked)
            
    def checkBox_revoke_Change(self):
        if (self.checkBox_revoke.isChecked()):
            self.checkBox_all.setCheckState(Qt.Unchecked)
            self.checkBox_select.setCheckState(Qt.Unchecked)
            self.checkBox_insert.setCheckState(Qt.Unchecked)
            self.checkBox_update.setCheckState(Qt.Unchecked)
            self.checkBox_delete.setCheckState(Qt.Unchecked)
            self.checkBox_references.setCheckState(Qt.Unchecked)
            self.checkBox_trigger.setCheckState(Qt.Unchecked)
            
    def checkBox_all_2_Change(self):
        if (self.checkBox_all_2.isChecked()):
            self.checkBox_revoke_2.setCheckState(Qt.Unchecked)
            self.checkBox_select2.setCheckState(Qt.Checked)
            self.checkBox_usage.setCheckState(Qt.Checked)
            self.checkBox_update2.setCheckState(Qt.Checked)
            
    def checkBox_revoke_2_Change(self):
        if (self.checkBox_revoke_2.isChecked()):
            self.checkBox_all_2.setCheckState(Qt.Unchecked)
            self.checkBox_select2.setCheckState(Qt.Unchecked)
            self.checkBox_usage.setCheckState(Qt.Unchecked)
            self.checkBox_update2.setCheckState(Qt.Unchecked)
            
    def on_mui_guardar_released(self):
        # Activo y cambio a la 3ª pestaña del programa, la consola. para poder ver el resultado de los comandos
        self.tabWidget.setTabEnabled(3, True)
        self.tabWidget.setCurrentIndex(3)
        self.mui_textBrowser.clear()
        self.writeDB()        

        #Creamos la bara de progreso
        self.progress = QtGui.QProgressBar(self)
        self.progress.setGeometry(self.width() / 2 -100, self.height() /2 -10, 200, 20)
        self.progress.show()

        #Calculamos el % de aumento de la barra de progreso
        total_tablas = 0.0
        numero = self.listWidgetTable.count()

        for x in range (numero):
            temp = self.listWidgetTable.item(x)
            
            if (temp.isSelected()):
                total_tablas = total_tablas + 1.0
         
        actual = 0.0
        if (total_tablas != 0.0):
            mas = 100.0/(total_tablas*6.0)
        
	# Conectamos con la base de datos desde la que ejecutaremos los Grants y Revokes
	self.conectarIsolation("template1")

        # Empezamos a conceder permisos, primero a la base de datos seleccionada.
	if (self.checkBox_dball.isChecked()):
	    self.executeGrant("GRANT all on database \"" + str(dbase) + "\" to " + str(username))

        if (self.checkBox_dbrevoke.isChecked()):
            self.executeGrant('REVOKE all on database \"' + str(dbase) + '\" from ' + str(username))

        if (self.checkBox_create.isChecked()):
            self.executeGrant('GRANT create on database \"' + str(dbase) + '\" to ' + str(username))
        else:
            self.executeGrant('REVOKE create on database \"' + str(dbase) + '\" from ' + str(username))
            
        if (self.checkBox_temporary.isChecked()):
            self.executeGrant('GRANT temporary on database \"' + str(dbase) + '\" to ' + str(username))
        else:
            self.executeGrant('REVOKE temporary on database \"' + str(dbase) + '\" from ' + str(username))
            
	self.desconectar()
	self.writecommand("")

        """ EMPEZAMOS A CONCEDER LOS PERMISOS POR CADA TABLA."""
	""" ================================================="""

        numero = self.listWidgetTable.count()
        global table

	self.conectarIsolation(str(dbase))

        for x in range (numero):
            temp = self.listWidgetTable.item(x)
            
            if (temp.isSelected()):
                table = temp.text()
		self.writecommand("")
                self.writeTable()
        
                if (self.checkBox_all.isChecked()):
                    self.executeGrant('GRANT all on ' + str(table) + ' to ' + str(username))
                    self.progress.setValue(actual)

                if (self.checkBox_revoke.isChecked()):
                    self.executeGrant('REVOKE all on ' + str(table) + ' from ' + str(username))
                    self.progress.setValue(actual)
        
                if (self.checkBox_select.isChecked()):
                    self.executeGrant('GRANT select on ' + str(table) + ' to ' + str(username))
                else:
                    self.executeGrant('REVOKE select on ' + str(table) + ' from ' + str(username))
                    
                actual = actual + mas
                self.progress.setValue(actual)

                if (self.checkBox_insert.isChecked()):
                    self.executeGrant('GRANT insert on ' + str(table) + ' to ' + str(username))
                else:
                    self.executeGrant('REVOKE insert on ' + str(table) + ' from ' + str(username))
                    
                actual = actual + mas
                self.progress.setValue(actual)

                if (self.checkBox_update.isChecked()):
                    self.executeGrant('GRANT update on ' + str(table) + ' to ' + str(username))
                else:
                    self.executeGrant('REVOKE update on ' + str(table) + ' from ' + str(username))
                    
                actual = actual + mas
                self.progress.setValue(actual)

                if (self.checkBox_delete.isChecked()):
                    self.executeGrant('GRANT delete on ' + str(table) + ' to ' + str(username))
                else:
                    self.executeGrant('REVOKE delete on ' + str(table) + ' from ' + str(username))
                    
                actual = actual + mas
                self.progress.setValue(actual)

                if (self.checkBox_references.isChecked()):
                    self.executeGrant('GRANT references on ' + str(table) + ' to ' + str(username))
                else:
                    self.executeGrant('REVOKE references on ' + str(table) + ' from ' + str(username))
                    
                actual = actual + mas
                self.progress.setValue(actual)

                if (self.checkBox_trigger.isChecked()):
                    self.executeGrant('GRANT trigger on ' + str(table) + ' to ' + str(username))
                else:
                    self.executeGrant('REVOKE trigger on ' + str(table) + ' from ' + str(username))
                    
                actual = actual + mas
                self.progress.setValue(actual)

        self.progress.hide()
	self.writecommand("")
	self.writecommand("")
	self.writecommand("")

	""" AHORA PASAMOS A APLICAR LOS PERMISOS SOBRE LAS SEQUENCIAS"""
	"""=========================================================="""

	#Creamos la bara de progreso
        self.progress = QtGui.QProgressBar(self)
        self.progress.setGeometry(self.width() / 2 -100, self.height() /2 -10, 200, 20)
        self.progress.show()

        #Calculamos el % de aumento de la barra de progreso
        total_seq = 0.0
        numero = self.listWidgetSecuencias.count()

        for x in range (numero):
            temp = self.listWidgetSecuencias.item(x)
            
            if (temp.isSelected()):
                total_seq = total_seq + 1.0
         
        actual = 0.0
        if (total_seq != 0.0):
            mas = 100.0/(total_seq*3.0)
    
        #Ahora pasamos a conceder los permisos por cada tabla seleccionada en la lista.
        numero = self.listWidgetSecuencias.count()
        global seq

        for x in range (numero):
            temp = self.listWidgetSecuencias.item(x)
            
            if (temp.isSelected()):
	      self.writeDB() 
	      break

        for x in range (numero):
            temp = self.listWidgetSecuencias.item(x)
            
            if (temp.isSelected()):
                seq = temp.text()
		self.writecommand("")
                self.writeSeq()
                        
                if (self.checkBox_all_2.isChecked()):
                    self.executeGrant('GRANT all on ' + str(seq) +  ' to ' + str(username))
                    self.progress.setValue(actual)

                if (self.checkBox_revoke_2.isChecked()):
                    self.executeGrant('REVOKE all on ' + str(seq) + ' from ' + str(username))
                    self.progress.setValue(actual)
                
                if (self.checkBox_usage.isChecked()):
                    self.executeGrant('GRANT usage on ' + str(seq) + ' to ' + str(username))
                else:
                    self.executeGrant('REVOKE usage on ' + str(seq) + ' from ' + str(username))
                    
                actual = actual + mas
                self.progress.setValue(actual)
        
                if (self.checkBox_select.isChecked()):
                    self.executeGrant('GRANT select on ' + str(seq) + ' to ' + str(username))
                else:
                    self.executeGrant('REVOKE select on ' + str(seq) + ' from ' + str(username))
                    
                actual = actual + mas
                self.progress.setValue(actual)

                if (self.checkBox_update2.isChecked()):
                    self.executeGrant('GRANT update on ' + str(seq) + ' to ' + str(username))
                else:
                    self.executeGrant('REVOKE update on ' + str(seq) + ' from ' + str(username))
                    
                actual = actual + mas
                self.progress.setValue(actual)

        self.progress.hide()
	self.desconectar()
        
    def readOutput(self):
        self.mui_textBrowser.append(QString(self.proceso.readAllStandardOutput()))

    def readErrors(self):
        self.mui_textBrowser.append("<font color =\"#FF0000\">" + QString(self.proceso.readAllStandardError()) + "</font>")

    def writecommand(self, command):
        self.mui_textBrowser.append("<font color =\"#0000FF\">" + str(command) + "</font>")
        
    def writeDB(self):
        self.mui_textBrowser.append("<font color =\"#00A000\">====== Aplicando permisos en la base de datos " + str(dbase) + " a " + str(username) + " ======</font>")
        
    def writeTable(self):
        self.mui_textBrowser.append("<font color =\"#008000\">---- Aplicando permisos en la tabla " + str(table) + " a " + str(username) + " ----</font>")
         
    def writeSeq(self):
        self.mui_textBrowser.append("<font color =\"#008000\">---- Aplicando permisos en la secuencia " + str(seq) + " a " + str(username) + " ----</font>")

def main(args):
    app=QtGui.QApplication(args)
    win=ModificarUsuario()
    win.exec_()
    sys.exit(app.exec_())

if __name__=="__main__":
    main(sys.argv)