# -*- coding: utf-8 -*-

import sys
import os
from PyQt4.QtGui import *
from PyQt4.QtCore import *
from modificarusuariobase import *
# Libreria de acceso a bases de datos PostgreSQL
import psycopg2

class ModificarUsuario(QtGui.QDialog, Ui_ModificarUsuario):

    def __init__(self, parent = None):
        QtGui.QDialog.__init__(self,parent)
        self.setupUi(self)
        self.process = QtCore.QProcess()
        self.inicio()

    def inicio(self): 
        self.tabWidget.setTabEnabled(1, False)
        self.tabWidget.setTabEnabled(2, False)
        self.tabWidget.setCurrentIndex(0)
        self.initListaDatabase()
        self.desactivaCheckboxTable()
        self.connect(self.checkBox_all, SIGNAL("stateChanged(int)"), self.checkBox_change)
        self.connect(self.checkBox_dball, SIGNAL("stateChanged(int)"), self.checkBox_dball_Change)
        self.connect(self.checkBox_dbrevoke, SIGNAL("stateChanged(int)"), self.checkBox_dbrevoke_Change)
        self.connect(self.listWidgetDatabase, SIGNAL("itemSelectionChanged()"), self.capturaDatabase)
        self.connect(self.listWidgetUser, SIGNAL("itemSelectionChanged()"), self.capturaUsuario)
        self.connect(self.listWidgetTable, SIGNAL("itemSelectionChanged()"), self.capturaTabla)
        self.connect(self.process, SIGNAL("readyReadStandardError()"), self.readErrors)
        
                    
    def initListaDatabase(self):
        try:
            conn = psycopg2.connect("dbname='template1' user='root'")
        except:
            print "Fallo en la conexion con PostgreSQL."
            sys.exit()

        cur = conn.cursor()
        
        #Buscamos todas las bases de datos y las guardamos en un array.
        try:
            cur.execute("SELECT datname FROM pg_database ORDER BY datname")
        except:
            print "Fallo en la consulta para obtener las bases de datos de PostgreSQL."
            sys.exit()
            
        dbs = cur.fetchall()
                
        # Recorremos todas las bases de datos a partir del array
        for row in dbs:
        
            # Conectamos a cada base de datos excepto a template0 que no permite conexiones
            if str(row[0]) != 'template0':
                try:
                    conn2 = psycopg2.connect("dbname='" + str(row[0]) + "' user='root'")
                except:
                    print "Se produjo un error al intentar conectar con la base de datos " + str(row[0]) + " de PostgreSQL."
                    sys.exit()
                    
                cur = conn2.cursor()

                # Averiguamos el tipo de base de datos con la cual estamos tratando.
                try:
                    cur.execute("select valor from configuracion where nombre = 'Tipo'")
                    tipo = cur.fetchone()
                except:
                    continue
                    
                # Rellenamos la lista de bases de datos.
                if str(tipo) == "('BulmaFact',)":
                    texto = "BulmaFact  -  " + str(row[0])            
                    self.listWidgetDatabase.addItem(QString(texto))
                    
                if str(tipo) == "('BulmaCont',)":
                    texto = "BulmaCont  -  " + str(row[0])            
                    self.listWidgetDatabase.addItem(QString(texto))
                    
        conn.close()
        
    def abrirPermisosDesdeFacturacion(self):
        # Pasamos el nombre de la base de datos seleccionada en ModificarFacturacion a la variable database y seleccionamos el Item correspondiente
        numero = self.listWidgetDatabase.count()
        temp = QtGui.QListWidgetItem()
        global dbase
        dbase = self.database
        print str(dbase)
        
        for x in range (numero):
            temp = self.listWidgetDatabase.item(x)
            
            if (temp.isSelected()):
                dbase = temp.text()
                break
                        
        if dbase.contains("BulmaFact  -  "):
            dbase.remove("BulmaFact  -  ")
            
        if dbase.contains("BulmaCont  -  "):
            dbase.remove("BulmaCont  -  ")
            
        self.initListaUsuarios()
        self.desactivaCheckboxTable()
        
    
        
    def capturaDatabase(self):
        # Vaciamos la lista de las usuarios, ya que esta se regenera cada vez que cambiamos la base de datos seleccionada
        self.listWidgetUser.clear()
        # Pasamos el nombre de la base de datos seleccionada en listWidgetDatabase a la variable database
        numero = self.listWidgetDatabase.count()
        temp = QtGui.QListWidgetItem()
        global dbase
        
        for x in range (numero):
            temp = self.listWidgetDatabase.item(x)
            
            if (temp.isSelected()):
                dbase = temp.text()
                break
                        
        if dbase.contains("BulmaFact  -  "):
            dbase.remove("BulmaFact  -  ")
            
        if dbase.contains("BulmaCont  -  "):
            dbase.remove("BulmaCont  -  ")
            
        self.initListaUsuarios()
        self.desactivaCheckboxTable()

    def initListaUsuarios(self):
        try:
            """Aqui puede verse que hace falta agregar al usuario root como
            superusuario de bases de datos de postgres para que este script
            funcione correctamente. Con el usuario postgres basta con poner:
            createuser root"""
            conn = psycopg2.connect("dbname='template1' user='root'")
        except:
            print "Fallo en la conexion con PostgreSQL."
            print "Para poder administrar los permisos debes tener creado el usuario ROOT en Postgres:"
            print "Como SuperUsuario escribe: createuser root"
            sys.exit()

        cur = conn.cursor()
        
        try:
            cur.execute("SELECT * FROM pg_user")
        except:
            print "Fallo en la consulta para obtener los usuarios de PostgreSQL del sistema."
            sys.exit()
            
        usuarios = cur.fetchall()
        conn.close()
        texto = ""
        
        # Rellenamos la lista con los usuarios de PostgreSQL
        for row in usuarios:
            texto = row[0]
            if (str(texto) != 'root') and (str(texto) != 'postgres'):            
                if (row[3] == True):
                    texto = texto + "  (su)"                
                self.listWidgetUser.addItem(QString(texto))
            
    def capturaUsuario(self): 
        # Vaciamos la lista de bbdd, ya que esta se regenera cada vez que cambiamos el usuario seleccionado
        self.listWidgetTable.clear()
        # Pasamos el nombre del usuario seleccionado en listWidgetUser a la variable username
        numero = self.listWidgetUser.count()
        temp = QtGui.QListWidgetItem()
        global username
        
        for x in range (numero):
            temp = self.listWidgetUser.item(x)
                
            if (temp.isSelected()):
                username = temp.text()
                break

        if username.contains("  (su)"):
            username.remove("  (su)")
            
        self.initListaTablas()
        self.actualizarCheckboxDB()
        self.tabWidget.setTabEnabled(1, True)
        self.tabWidget.setCurrentIndex(1)
        
    def initListaTablas(self):
        try:
            conn = psycopg2.connect("dbname='" + str(dbase) + "' user='root'")
        except:
            print "Fallo en la conexion con PostgreSQL."
            sys.exit()

        cur = conn.cursor()
        
        #Buscamos todas las tablas de la base de datos seleccionada y las guardamos en un array.
        try:
            cur.execute("SELECT relname FROM pg_class WHERE relkind = 'r' AND relname NOT LIKE ('pg_%') AND relname NOT LIKE ('sql_%') ORDER BY relname")
        except:
            print "Fallo en la consulta para obtener tablas de PostgreSQL."
            sys.exit()              
        tablas = cur.fetchall()
        conn.close()
              
        # Rellenamos la lista con los usuarios de PostgreSQL
        for row in tablas:
            texto = row[0]                        
            self.listWidgetTable.addItem(QString(texto))
            
        self.listWidgetTable.setSelectionMode(self.listWidgetTable.MultiSelection) 

    def capturaTabla(self):
        self.activaCheckboxTable()
        self.actualizarCheckboxTable()
        
    def on_seleccionarTablas_released(self):
    
        numero = self.listWidgetTable.count()

        for x in range (numero):
            item = self.listWidgetTable.item(x)
            self.listWidgetTable.setCurrentItem(item)
            self.listWidgetTable.item(x).setSelected(True)
            

    def on_deseleccionarTablas_released(self):
    
        numero = self.listWidgetTable.count()

        for x in range (numero):
            self.listWidgetTable.item(x).setSelected(False)

    def actualizarCheckboxDB(self):
        #Conectamos con la base de datos
        try:
            conn = psycopg2.connect("dbname='" + str(dbase) + "' user='root'")
        except:
            print "Fallo en la conexion con PostgreSQL."
            sys.exit()
            
        cur = conn.cursor()
            
        #Sacamos los permisos del Usuario y la BBDD seleccionados, para despues actualizar los Checkbox's
        try:
            cur.execute("select has_database_privilege('" + str(username) + "', '" + str(dbase) + "', 'create');")
        except:
            print "Fallo en la consulta para obtener permisos (create) de PostgreSQL."
        permiso = cur.fetchone()
        if str(permiso) == "(True,)":
            self.checkBox_create.setCheckState(Qt.Checked)
        else:
            self.checkBox_create.setCheckState(Qt.Unchecked)    
            
        try:
            cur.execute("select has_database_privilege('" + str(username) + "', '" + str(dbase) + "', 'temporary');")
        except:
            print "Fallo en la consulta para obtener permisos (temporary) de PostgreSQL."
        permiso = cur.fetchone()
        if str(permiso) == "(True,)":
            self.checkBox_temporary.setCheckState(Qt.Checked)
        else:
            self.checkBox_temporary.setCheckState(Qt.Unchecked) 

        conn.close()
        
    def actualizarCheckboxTable(self):

        temp = self.listWidgetTable.currentItem()
        table = temp.text()
        
        #Conectamos con la base de datos
        try:
            conn = psycopg2.connect("dbname='" + str(dbase) + "' user='root'")
        except:
            print "Fallo en la conexion con PostgreSQL."
            sys.exit()
            
        cur = conn.cursor()

        #Sacamos los permisos del Usuario y la BBDD seleccionados, para despues actualizar los Checkbox's
        try:
            cur.execute("select has_table_privilege('" + str(username) + "', '" + str(table) + "', 'select');")
        except:
            print "Fallo en la consulta para obtener permisos (select) de PostgreSQL."        
        permiso = cur.fetchone()  
        if str(permiso) == "(True,)":
            self.checkBox_select.setCheckState(Qt.Checked)
        else:
            self.checkBox_select.setCheckState(Qt.Unchecked)    
            
        try:
            cur.execute("select has_table_privilege('" + str(username) + "', '" + str(table) + "', 'insert');")
        except:
            print "Fallo en la consulta para obtener permisos (insert) de PostgreSQL."
        permiso = cur.fetchone()
        if str(permiso) == "(True,)":
            self.checkBox_insert.setCheckState(Qt.Checked)
        else:
            self.checkBox_insert.setCheckState(Qt.Unchecked)
            
        try:
            cur.execute("select has_table_privilege('" + str(username) + "', '" + str(table) + "', 'update');")
        except:
            print "Fallo en la consulta para obtener permisos (update) de PostgreSQL."
        permiso = cur.fetchone()
        if str(permiso) == "(True,)":
            self.checkBox_update.setCheckState(Qt.Checked)
        else:
            self.checkBox_update.setCheckState(Qt.Unchecked)
            
        try:
            cur.execute("select has_table_privilege('" + str(username) + "', '" + str(table) + "', 'delete');")
        except:
            print "Fallo en la consulta para obtener permisos (delete) de PostgreSQL."
        permiso = cur.fetchone()
        if str(permiso) == "(True,)":
            self.checkBox_delete.setCheckState(Qt.Checked)
        else:
            self.checkBox_delete.setCheckState(Qt.Unchecked)        
             
        try:
            cur.execute("select has_table_privilege('" + str(username) + "', '" + str(table) + "', 'references');")
        except:
            print "Fallo en la consulta para obtener permisos (references) de PostgreSQL."
        permiso = cur.fetchone()
        if str(permiso) == "(True,)":
            self.checkBox_references.setCheckState(Qt.Checked)
        else:
            self.checkBox_references.setCheckState(Qt.Unchecked)              

        try:
            cur.execute("select has_table_privilege('" + str(username) + "', '" + str(table) + "', 'trigger');")
        except:
            print "Fallo en la consulta para obtener permisos (trigger) de PostgreSQL."
        permiso = cur.fetchone()
        if str(permiso) == "(True,)":
            self.checkBox_trigger.setCheckState(Qt.Checked)
        else:
            self.checkBox_trigger.setCheckState(Qt.Unchecked)   

        conn.close()
        
    def checkBox_change(self):
        if (self.checkBox_all.isChecked()):
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

    def desactivaCheckboxTable(self):
        self.checkBox_select.setEnabled(False)
        self.checkBox_insert.setEnabled(False)
        self.checkBox_update.setEnabled(False)
        self.checkBox_delete.setEnabled(False)
        self.checkBox_references.setEnabled(False)
        self.checkBox_trigger.setEnabled(False)
        self.checkBox_all.setEnabled(False)
        self.mui_guardar.setEnabled(False)
        
    def activaCheckboxTable(self):
        self.checkBox_select.setEnabled(True)
        self.checkBox_insert.setEnabled(True)
        self.checkBox_update.setEnabled(True)
        self.checkBox_delete.setEnabled(True)
        self.checkBox_references.setEnabled(True)
        self.checkBox_trigger.setEnabled(True)
        self.checkBox_all.setEnabled(True)
        self.mui_guardar.setEnabled(True)
        self.checkBox_all.setCheckState(Qt.Unchecked)

    def on_mui_guardar_released(self):
        # Activo y cambio a la 3ª pestaña del programa, la consola. para poder ver el resultado cuando termine de procesar
        self.tabWidget.setTabEnabled(2, True)
        self.tabWidget.setCurrentIndex(2)
        global command
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

        #Empezamos a conceder permisos, primero a la base de datos seleccionada.
        if (self.checkBox_dball.isChecked()):
            command = 'psql template1 -c "GRANT all on database ' + str(dbase) +  ' to ' + str(username) + '"'
            self.execQuery()
            
        self.progress.setValue(actual)

        if (self.checkBox_dbrevoke.isChecked()):
            command = 'psql template1 -c "REVOKE all on database ' + str(dbase) + ' from ' + str(username) + '"'
            self.execQuery()
            
        self.progress.setValue(actual)

        if (self.checkBox_create.isChecked()):
            command = 'psql template1 -c "GRANT create on database ' + str(dbase) + ' to ' + str(username) + '"'
            self.execQuery()
        else:
            command = 'psql template1 -c "REVOKE create on database ' + str(dbase) + ' from ' + str(username) + '"'
            self.execQuery()

        self.progress.setValue(actual)
            
        if (self.checkBox_temporary.isChecked()):
            command = 'psql template1 -c "GRANT temporary on database ' + str(dbase) + ' to ' + str(username) + '"'
            self.execQuery()
        else:
            command = 'psql template1 -c "REVOKE temporary on database ' + str(dbase) + ' from ' + str(username) + '"'
            self.execQuery()
            
        self.progress.setValue(actual)
            
        global table

        #Ahora pasamos a conceder los permisos por cada tabla seleccionada en la lista.
        numero = self.listWidgetTable.count()

        for x in range (numero):
            temp = self.listWidgetTable.item(x)
            
            if (temp.isSelected()):
                table = temp.text()
                self.writeTable()
        
                if (self.checkBox_select.isChecked()):
                    command = 'psql ' + str(dbase) + ' -c "GRANT select on ' + str(table) + ' to ' + str(username) + '"'
                    self.execQuery()
                else:
                    command = 'psql ' + str(dbase) + ' -c "REVOKE select on ' + str(table) + ' from ' + str(username) + '"'
                    self.execQuery()
                    
                actual = actual + mas
                self.progress.setValue(actual)

                if (self.checkBox_insert.isChecked()):
                    command = 'psql ' + str(dbase) + ' -c "GRANT insert on ' + str(table) + ' to ' + str(username) + '"'
                    self.execQuery()
                else:
                    command = 'psql ' + str (dbase) + ' -c "REVOKE insert on ' + str(table) + ' from ' + str(username) + '"'
                    self.execQuery()
                    
                actual = actual + mas
                self.progress.setValue(actual)

                if (self.checkBox_update.isChecked()):
                    command = 'psql ' + str(dbase) + ' -c "GRANT update on ' + str(table) + ' to ' + str(username) + '"'
                    self.execQuery()
                else:
                    command = 'psql ' + str(dbase) + ' -c "REVOKE update on ' + str(table) + ' from ' + str(username) + '"'
                    self.execQuery()
                    
                actual = actual + mas
                self.progress.setValue(actual)

                if (self.checkBox_delete.isChecked()):
                    command = 'psql ' + str(dbase) + ' -c "GRANT delete on ' + str(table) + ' to ' + str(username) + '"'
                    self.execQuery()
                else:
                    command = 'psql ' + str(dbase) + ' -c "REVOKE delete on ' + str(table) + ' from ' + str(username) + '"'
                    self.execQuery()
                    
                actual = actual + mas
                self.progress.setValue(actual)

                if (self.checkBox_references.isChecked()):
                    command = 'psql ' + str(dbase) + ' -c "GRANT references on ' + str(table) + ' to ' + str(username) + '"'
                    self.execQuery()
                else:
                    command = 'psql ' + str(dbase) + ' -c "REVOKE references on ' + str(table) + ' from ' + str(username) + '"'
                    self.execQuery()
                    
                actual = actual + mas
                self.progress.setValue(actual)

                if (self.checkBox_trigger.isChecked()):
                    command = 'psql ' + str(dbase) + ' -c "GRANT trigger on ' + str(table) + ' to ' + str(username) + '"'
                    self.execQuery()
                else:
                    command = 'psql ' + str(dbase) + ' -c "REVOKE trigger on ' + str(table) + ' from ' + str(username) + '"'
                    self.execQuery()
                    
                actual = actual + mas
                self.progress.setValue(actual)
        self.progress.hide()
                    
        # LAS SIGUIENTES 80 LINEAS ES LO MISMO QUE LO ANTERIOR, PERO SE CONCEDEN LOS PERMISOS 
        # A TRAVES DE UNA CONEXION DIRECTA CON POSTGRES, QUE SERIA LO CORRECTO, COMO NO FUNCIONA
        # PASO A APLICAR LOS PERMISOS DIRECTAMENTE CON COMANDOS DESDE EL SHELL,
        # PROVISIONALEMENTE, HASTA ENCONTRAR PORQUE NO FUNCIONA HACIENDOLO CON CONEXIONES A POSTGRES.
                    
        #try:
            #conn = psycopg2.connect("dbname='" + str(dbase) + "' user='root'")
        #except:
            #print "Fallo en la conexion con PostgreSQL."
            #sys.exit()
            
        #cur = conn.cursor()

        #if (self.checkBox_dball.isChecked()):
            #try:
                #cur.execute("grant all on database " + str(dbase) + " to " + str(username))
            #except:
                #print "Se produjo un error al intentar cambiar los permisos (grant all) de las bases de datos de PostgreSQL."
                #sys.exit()

        #if (self.checkBox_dbrevoke.isChecked()):
            #try:
                #cur.execute("revoke all on database " + str(dbase) + " from " + str(username))
            #except:
                #print "Se produjo un error al intentar cambiar los permisos (revoke all) de las bases de datos de PostgreSQL."
                #sys.exit()

        #if (self.checkBox_select.isChecked()):
            #try:
                #cur.execute("grant select on " + str(table) + " to " + str(username))
            #except:
                #print "Se produjo un error al intentar cambiar los permisos (select) de las bases de datos de PostgreSQL."
                #sys.exit()

        #if (self.checkBox_insert.isChecked()):
            #try:
                #cur.execute("grant insert on " + str(table) + " to " + str(username))
            #except:
                #print "Se produjo un error al intentar cambiar los permisos (insert) de las bases de datos de PostgreSQL."
                #sys.exit()

        #if (self.checkBox_update.isChecked()):
            #try:
                #cur.execute("grant update on " + str(table) + " to " + str(username))
            #except:
                #print "Se produjo un error al intentar cambiar los permisos (update) de las bases de datos de PostgreSQL."
                #sys.exit()

        #if (self.checkBox_delete.isChecked()):
            #try:
                #cur.execute("grant delete on " + str(table) + " to " + str(username))
            #except:
                #print "Se produjo un error al intentar cambiar los permisos (delete) de las bases de datos de PostgreSQL."
                #sys.exit()

        #if (self.checkBox_references.isChecked()):
            #try:
                #cur.execute("grant references on " + str(table) + " to " + str(username))
            #except:
                #print "Se produjo un error al intentar cambiar los permisos (references) de las bases de datos de PostgreSQL."
                #sys.exit()

        #if (self.checkBox_trigger.isChecked()):
            #try:
                #cur.execute("grant trigger on " + str(table) + " to " + str(username))
            #except:
                #print "Se produjo un error al intentar cambiar los permisos (trigger) de las bases de datos de PostgreSQL."
                #sys.exit()

        #if (self.checkBox_create.isChecked()):
            #try:
                #cur.execute("grant create on database " + str(dbase) + " to " + str(username))
            #except:
                #print "Se produjo un error al intentar cambiar los permisos (create) de las bases de datos de PostgreSQL."
                #sys.exit()

        #if (self.checkBox_temporary.isChecked()):
            #try:
                #cur.execute("grant temporary on database " + str(dbase) + " to " + str(username))
            #except:
                #print "Se produjo un error al intentar cambiar los permisos (temporary) de las bases de datos de PostgreSQL."
                #sys.exit()

    def execQuery(self):
        self.writecommand()
        self.process.start(command)
        self.process.waitForFinished(-1)
        return QString(self.process.readAllStandardOutput())
        
    def readOutput(self):
        self.mui_textBrowser.append(QString(self.process.readAllStandardOutput()))

    def readErrors(self):
        self.mui_textBrowser.append("<font color =\"#FF0000\">error: " + QString(self.process.readAllStandardError()) + "</font>")

    def writecommand(self):
        self.mui_textBrowser.append("<font color =\"#0000FF\">" + str(command) + "</font>")
        
    def writeDB(self):
        self.mui_textBrowser.append("<font color =\"#00A000\">====== Aplicando permisos en la base de datos " + str(dbase) + " a " + str(username) + " ======</font>")
        
    def writeTable(self):
        self.mui_textBrowser.append("<font color =\"#008000\">---- Aplicando permisos en la tabla " + str(table) + " a " + str(username) + " ----</font>")
            
def main(args):
    app=QtGui.QApplication(args)
    win=ModificarUsuario()
    win.exec_()
    sys.exit(app.exec_())

if __name__=="__main__":
    main(sys.argv)