# -*- coding: utf-8 -*-

import sys
import os
from config import *
from PyQt4 import QtCore, QtGui
from PyQt4.QtGui import *
from PyQt4.QtCore import *
from plugins import PluginsBulmaSetup
from modificarfacturacionbase import *
from empresa import Empresa
import plugins

class Facturacion(Ui_ModificarFacturacionBase, Empresa):
   def __init__(self,database, parent = None):
      Empresa.__init__(self, database, parent)
      self.setupUi(self)
      
      # Ocultamos la columna de las descripciones.
      self.mui_plugins.setColumnCount(12)
      self.mui_plugins1.setColumnCount(12)

      self.mui_plugins.hideColumn(1)
      self.mui_plugins1.hideColumn(1)

      self.mui_plugins.hideColumn(3)
      self.mui_plugins1.hideColumn(3)

      self.mui_plugins.hideColumn(4)
      self.mui_plugins1.hideColumn(4)

      self.mui_plugins.hideColumn(5)
      self.mui_plugins1.hideColumn(5)

      self.mui_plugins.hideColumn(6)
      self.mui_plugins1.hideColumn(6)

      self.mui_plugins.hideColumn(7)
      self.mui_plugins1.hideColumn(7)

      self.mui_plugins.hideColumn(8)
      self.mui_plugins1.hideColumn(8)

      self.mui_plugins.hideColumn(9)
      self.mui_plugins1.hideColumn(9)

      self.mui_plugins.hideColumn(10)
      self.mui_plugins1.hideColumn(10)
      
      self.mui_plugins.hideColumn(11)
      self.mui_plugins1.hideColumn(11)

      self.mui_plugins1.setEnabled(False)
      
      # Desabilitamos el Sorting para que en versiones previas se rellenen bien los campos
      self.mui_plugins.setSortingEnabled(False)
      self.mui_plugins1.setSortingEnabled(False)

      # Hacemos un Show y así el padre recupera el control.
      self.show()

      self.inicializar()
      
      # Habilitamos el Sortin pq los campos ya estan rellenos.
      self.mui_plugins.setSortingEnabled(False)
      self.mui_plugins1.setSortingEnabled(False)

   def inicializar(self):
      # Ponemos la pestanya de consola como la visible
      self.tabWidget.setCurrentIndex(2)
      self.tabWidget.setCurrentIndex(0)

      # Buscamos los Plugins
      self.buscaPlugins()
      # Ajustamos la presentacion
      self.mui_plugins.resizeColumnsToContents()
      self.mui_plugins1.resizeColumnsToContents()
      self.mui_plugins.resizeRowsToContents()
      self.mui_plugins1.resizeRowsToContents()


      # Inicializamos el combo de categoria.
      self.mui_categoria.clear()
      self.i =0 ;
      self.cadcategorias = '';
      while (self.i < len(self.pluginsbulmafact)):
         self.cadcategorias = self.cadcategorias + self.pluginsbulmafact[self.i][8]
         self.i = self.i + 1
      self.i = 0
      while (self.i < len(self.pluginsbulmatpv)):
         self.cadcategorias = self.cadcategorias + self.pluginsbulmatpv[self.i][8]
         self.i = self.i + 1
      # Tratamos toda la cadena de categorias.
      self.cadcategorias = self.cadcategorias.replace('; ',';')
      self.cadcategorias = self.cadcategorias.replace(' ;',';')
      self.arra = self.cadcategorias.split(';')
      self.arra.sort()
      self.j = 0
      self.mui_categoria.addItem('--Todas las Categorias--')
      while (self.j < len ( self.arra)):
         if (self.j < 1 or self.arra[self.j] <> self.arra[self.j-1]):
            self.mui_categoria.addItem( self.arra[self.j])
         self.j = self.j + 1
         
      #Ponemos la pestanya de seleccion de plugins como la visible.
      self.tabWidget.setCurrentIndex(0)

   def trataOpenReports(self):
      self.writecommand('Generando plantillas RML y PYS')
      # Creamos el directorio especifico para guardar las plantillas
      self.string = "mkdir -p /opt/bulmages/openreports_" + self.database 
      self.process.start(self.string)
      self.process.waitForFinished(-1)

      # Hacemos un backup de openreports
      self.string = "cp -R /opt/bulmages/openreports_" + self.database + " /opt/bulmages/openreports_" + self.database + "_old"
      self.process.start(self.string)
      self.process.waitForFinished(-1)

      # Copiamos los archivos genericos
      # Copiamos las plantillas
      self.string = "cp /usr/local/share/bulmages/openreports/informereferencia.rml" +  " /opt/bulmages/openreports_" + self.database
      self.writecommand(self.string)
      self.process.start(self.string)
      self.process.waitForFinished(-1)
      self.string = "cp /usr/local/share/bulmages/openreports/plantilla.rml" +  " /opt/bulmages/openreports_" + self.database
      self.writecommand(self.string)
      self.process.start(self.string)
      self.process.waitForFinished(-1)
      self.string = "cp /usr/local/share/bulmages/openreports/plantilla1.rml" +  " /opt/bulmages/openreports_" + self.database
      self.writecommand(self.string)
      self.process.start(self.string)
      self.process.waitForFinished(-1)
      self.string = "cp /usr/local/share/bulmages/openreports/estilos.rml" +  " /opt/bulmages/openreports_" + self.database
      self.writecommand(self.string)
      self.process.start(self.string)
      self.process.waitForFinished(-1)
      self.string = "cp /usr/local/share/bulmages/openreports/listado.rml" +  " /opt/bulmages/openreports_" + self.database
      self.writecommand(self.string)
      self.process.start(self.string)
      self.process.waitForFinished(-1)
      self.string = "cp /usr/local/share/bulmages/openreports/logo.jpg" +  " /opt/bulmages/openreports_" + self.database
      self.writecommand(self.string)
      self.process.start(self.string)
      self.process.waitForFinished(-1)
      self.string = "cp /usr/local/share/bulmages/openreports/ficha.rml" +  " /opt/bulmages/openreports_" + self.database
      self.writecommand(self.string)
      self.process.start(self.string)
      self.process.waitForFinished(-1)

      # Iteramos sobre la lista de plugins disponibles en bulmafact para copiar sus plantillas
      self.i = 0
      while (self.i < self.mui_plugins.rowCount()):
	# Si el plugin tiene el orden adecuado lo consideramos.
	self.writecommand('Tratando ' + self.mui_plugins.item(self.i,0).text())
	# Si el plugin esta checked lo escribimos.
	if (self.mui_plugins.item(self.i, 0).checkState() == Qt.Checked and len(self.mui_plugins.item(self.i,11).text()) > 3):
	  # Si hay que aplicar un plugin entonces lo escribimos
	  if (self.mui_plugins.item(self.i,11).text() != 'None' and len(self.mui_plugins.item(self.i,11).text()) > 3):
	    # Tratamos toda la cadena de categorias.
	    self.cadreports = self.mui_plugins.item(self.i,11).text().replace('; ',';')
	    self.cadreports = self.cadreports.replace(' ;',';')
	    self.arra = self.cadreports.split(';')
	    self.arra.sort()
	    self.j = 0
	    while (self.j < len ( self.arra)):
	      # Copiamos las plantillas
	      self.string = "cp /usr/local/share/bulmages/openreports/" +self.arra[self.j] + " /opt/bulmages/openreports_" + self.database
	      self.writecommand(self.string)
	      self.process.start(self.string)
	      self.process.waitForFinished(-1)
	      self.j = self.j + 1
	self.i = self.i + 1

      # Iteramos sobre la lista de plugins disponibles en bulmatpv para copiar sus plantillas
      self.i = 0
      while (self.i < self.mui_plugins1.rowCount()):
	# Si el plugin tiene el orden adecuado lo consideramos.
	self.writecommand('Tratando ' + self.mui_plugins1.item(self.i,0).text())
	# Si el plugin esta checked lo escribimos.
	if (self.mui_plugins1.item(self.i, 0).checkState() == Qt.Checked and len(self.mui_plugins1.item(self.i,11).text()) > 3):
	  # Si hay que aplicar un plugin entonces lo escribimos
	  if (self.mui_plugins1.item(self.i,11).text() != 'None' and len(self.mui_plugins1.item(self.i,11).text()) > 3):
	    # Tratamos toda la cadena de categorias.
	    self.cadreports = self.mui_plugins1.item(self.i,11).text().replace('; ',';')
	    self.cadreports = self.cadreports.replace(' ;',';')
	    self.arra = self.cadreports.split(';')
	    self.arra.sort()
	    self.j = 0
	    while (self.j < len ( self.arra)):
	      # Copiamos las plantillas
	      self.string = "cp /usr/local/share/bulmages/openreports/" + self.arra[self.j] + " /opt/bulmages/openreports_" + self.database
	      self.writecommand(self.string)
	      self.process.start(self.string)
	      self.process.waitForFinished(-1)
	      self.j = self.j + 1
	self.i = self.i + 1

      # Hacemos un backup del archivo
      self.string = "cp " + plugins.configfiles + "bulmafact_" + self.database + ".conf " + plugins.configfiles + "bulmafact_" + self.database + ".conf~ "
      self.process.start(self.string)
      self.process.waitForFinished(-1)
      #self.writecommand(self.process.readAllStandardOutput())

      # Abrimos el backup para lectura
      self.file1 = QFile( plugins.configfiles + "bulmafact_" + self.database + ".conf~");
      if not(self.file1.open(QIODevice.ReadOnly | QIODevice.Text)):
        return;
      self.vin = QTextStream(self.file1)
      
      # Abrimos el archivo para escritura.
      self.file = QFile( plugins.configfiles + "bulmafact_" + self.database + ".conf");
      if not(self.file.open(QIODevice.WriteOnly | QIODevice.Text)):
        return;
      self.out = QTextStream(self.file)
      
      # Leemos las lineas iniciales (hasta el parametro deseado) y las ponemos de nuevo.
      self.text = self.vin.readLine()
      while (not (self.text.isNull()) and not(self.text.contains("CONF_DIR_OPENREPORTS")) ):
        self.out << self.text << "\n"
        self.text = self.vin.readLine()

      # Escribimos el parametro como lo deseamos
      self.out << "\n\nCONF_DIR_OPENREPORTS /opt/bulmages/openreports_" + self.database +"/\n\n"


      # Terminamos de poner el resto de las linea.
      if (not (self.text.isNull()) ):
        self.text = self.vin.readLine()
      while (not (self.text.isNull()) ):
        self.out << self.text << "\n"
        self.text = self.vin.readLine()
        
      # Cerramos los ficheros.
      self.file.close()
      self.file1.close()

   def writeConfig(self):
      self.writecommand('ESCRIBIENDO CONFIGURACION')
      self.writecommand("Escribiendo configuracion en "+ plugins.configfiles + "bulmafact_" + self.database + ".conf " )
      

      # TRATAMOS EL ARCHIVO DE BULMAFACT
      # ================================

      # Tocamos el archivo por si no existe
      self.string = "touch " + plugins.configfiles + "bulmafact_" + self.database + ".conf "
      self.process.start(self.string)
      self.process.waitForFinished(-1)
      #self.writecommand(self.process.readAllStandardOutput())

      # Hacemos un backup del archivo
      self.string = "cp " + plugins.configfiles + "bulmafact_" + self.database + ".conf " + plugins.configfiles + "bulmafact_" + self.database + ".conf~ "
      self.process.start(self.string)
      self.process.waitForFinished(-1)
      #self.writecommand(self.process.readAllStandardOutput())
      
      # Abrimos el backup para lectura
      self.file1 = QFile( plugins.configfiles + "bulmafact_" + self.database + ".conf~");
      if not(self.file1.open(QIODevice.ReadOnly | QIODevice.Text)):
        return;
      self.vin = QTextStream(self.file1)
      
      # Abrimos el archivo para escritura.
      self.file = QFile( plugins.configfiles + "bulmafact_" + self.database + ".conf");
      if not(self.file.open(QIODevice.WriteOnly | QIODevice.Text)):
        return;
      self.out = QTextStream(self.file)
      
      # Leemos las lineas iniciales (hasta el parametro deseado) y las ponemos de nuevo.
      self.text = self.vin.readLine()
      while (not (self.text.isNull()) and not(self.text.contains("CONF_PLUGINS_BULMAFACT")) ):
        self.out << self.text << "\n"
        self.text = self.vin.readLine()
        
      # Quitamos todo lo que es el parametro porque no debe estar
      while (not (self.text.isNull()) and self.text.contains("lib")):
        self.text = self.vin.readLine()
      
      self.nuevo = 1
   
      # Como los plugins van por orden iteramos sobre el orden para arreglarlo.
      self.x = 1
      while (self.x < 1000) :
        # Iteramos sobre la lista de plugins disponibles en bulmafact
        self.i = 0
        while (self.i < self.mui_plugins.rowCount()):
          # Si el plugin tiene el orden adecuado lo consideramos.
          if (str(self.mui_plugins.item(self.i,7).text()) == str(self.x) ):
            self.writecommand('Tratando ' + self.mui_plugins.item(self.i,0).text())
            # Si el plugin esta checked lo escribimos.
            if (self.mui_plugins.item(self.i, 0).checkState() == Qt.Checked and len(self.mui_plugins.item(self.i,10).text()) > 3):
              if (self.nuevo == 1):
                 self.nuevo = 0
                 # Escribimos la configuracion de plugins.
                 self.terminador = ""
                 self.out << "CONF_PLUGINS_BULMAFACT   "
	      # Si hay que aplicar un plugin entonces lo escribimos
	      if (self.mui_plugins.item(self.i,10).text() != 'None' and len(self.mui_plugins.item(self.i,10).text()) > 3):
		self.writecommand('Hay que actualizar ' + self.mui_plugins.item(self.i,0).text())
		self.out << self.terminador << self.mui_plugins.item(self.i,10).text()
		self.terminador = "; \\\n";
          self.i = self.i + 1
        self.x = self.x + 1
      self.out << "\n"
      
      # Terminamos de poner el resto de las linea.
      while (not (self.text.isNull()) ):
        self.out << self.text << "\n"
        self.text = self.vin.readLine()
        
      # Cerramos slo ficheros.
      self.file.close()
      self.file1.close()
   
      # TRATAMOS EL ARCHIVO DEL BULMATPV
      # ================================
   
      if (self.mui_soporteTPV.isChecked()):
      
        # Tocamos el archivo por si no existe
        self.string = "touch " + plugins.configfiles + "bulmatpv_" + self.database + ".conf "
        self.process.start(self.string)
        self.process.waitForFinished(-1)
        self.writecommand(self.process.readAllStandardOutput())

        # Hacemos un backup del archivo
        self.string = "cp " + plugins.configfiles + "bulmatpv_" + self.database + ".conf " + plugins.configfiles + "bulmatpv_" + self.database + ".conf~ "
        self.process.start(self.string)
        self.process.waitForFinished(-1)
        self.writecommand(self.process.readAllStandardOutput())
        
        # Abrimos el backup para lectura
        self.file1 = QFile( plugins.configfiles + "bulmatpv_" + self.database + ".conf~");
        if not(self.file1.open(QIODevice.ReadOnly | QIODevice.Text)):
          return;
        self.vin = QTextStream(self.file1)
      
        self.file = QFile( plugins.configfiles + "bulmatpv_" + self.database + ".conf");
        if not(self.file.open(QIODevice.WriteOnly | QIODevice.Text)):
          return;
        self.out = QTextStream(self.file)

        # Leemos las lineas iniciales (hasta el parametro deseado) y las ponemos de nuevo.
        self.text = self.vin.readLine()
        while (not (self.text.isNull()) and not(self.text.contains("CONF_PLUGINS_BULMATPV")) ):
          self.out << self.text << "\n"
          self.text = self.vin.readLine()
          
        # Quitamos todo lo que es el parametro porque no debe estar
        while (not (self.text.isNull()) and self.text.contains("lib")):
          self.text = self.vin.readLine()
      
      # Escribimos la configuracion de plugins.
        self.terminador = ""

        self.nuevo = 1
        # Como los plugins van por orden iteramos sobre el orden para arreglarlo.
        self.x = 1
        while (self.x < 1000) :
          self.i = 0
          while (self.i < self.mui_plugins1.rowCount()):
              if ( str(self.mui_plugins1.item(self.i,7).text()) == str(self.x)):
                self.writecommand('Tratando ' + self.mui_plugins1.item(self.i,0).text())
                if (self.mui_plugins1.item(self.i, 0).checkState() == Qt.Checked and len (self.mui_plugins1.item (self.i, 10).text()) > 3):
                  if (self.nuevo == 1):
                    self.nuevo = 0
                    self.out << "CONF_PLUGINS_BULMATPV   "
                  self.writecommand('Hay que actualizar ' + self.mui_plugins1.item(self.i,0).text())
		  # Si hay que aplicar un plugin entonces lo escribimos
		  if (self.mui_plugins1.item(self.i,10).text() != 'None' and len(self.mui_plugins1.item(self.i,10).text()) > 3):
		    self.writecommand('Hay que actualizar ' + self.mui_plugins1.item(self.i,0).text())
		    self.out << self.terminador << self.mui_plugins1.item(self.i,10).text()
		    self.terminador = "; \\\n";
              self.i = self.i + 1
          self.x = self.x + 1
        self.out << "\n"
        
        # Terminamos de poner el resto de las linea.
        while (not (self.text.isNull()) ):
          self.out << self.text << "\n"
          self.text = self.vin.readLine()

        self.file.close()
        self.file1.close()
      self.trataOpenReports()

   def marcar(self, plug, rec, first = 0):
      i = 0
      while (i < self.mui_plugins1.rowCount()):
         if (self.mui_plugins1.item(i,10).text() == plug):
            if (self.mui_plugins1.item(i, 0).checkState() == Qt.Unchecked or first == 1):
	      if (rec == 1):
		self.mui_plugins1.item(i,0).setCheckState(Qt.Checked)
		# Desmarcamos las incompatibilidades
		self.arr = self.mui_plugins1.item(i,6).text().replace(' ;',';').replace('; ',';').split(QString(";"))
		if self.arr != ['']:
			for self.dep in self.arr:
			    self.desmarcar(self.dep,1)
		# Marcamos las dependencias
		self.arr = self.mui_plugins1.item(i,5).text().replace(' ;',';').replace('; ',';').split(QString(";"))
		if self.arr != ['']:
			for self.dep in self.arr:
			    self.marcar(self.dep,1)
         i = i + 1
      i = 0
      while (i < self.mui_plugins.rowCount()):
         if (self.mui_plugins.item(i,10).text() == plug):
            if (self.mui_plugins.item(i, 0).checkState() == Qt.Unchecked or first == 1):
	      self.mui_plugins.item(i,0).setCheckState(Qt.Checked)
	      if (rec == 1):
		# Desmarcamos las incompatibilidades
		self.arr = self.mui_plugins.item(i,6).text().replace(' ;',';').replace('; ',';').split(QString(";"))
		if self.arr != ['']:
			for self.dep in self.arr:
			    self.desmarcar(self.dep,1)
		# Marcamos las dependencias
		self.arr = self.mui_plugins.item(i,5).text().replace(' ;',';').replace('; ',';').split(QString(";"))
		if self.arr != ['']:
			for self.dep in self.arr:
			    self.marcar(self.dep,1)
         i = i + 1

   def desmarcar(self, plug, rec, first = 0):
      i = 0
      while (i < self.mui_plugins1.rowCount()):
	 if (self.mui_plugins1.item(i, 0).checkState() == Qt.Checked or first == 1):
	   if (rec == 1):
	     # Marcamos las dependencias
	     self.arr = self.mui_plugins1.item(i,5).text().replace(' ;',';').replace('; ',';').split(QString(";"))
	     if self.arr != ['']:
		for self.dep in self.arr:
		      if (self.dep == plug):
			  self.desmarcar(self.mui_plugins1.item(i,10).text(),1)
			  self.mui_plugins1.item(i,0).setCheckState(Qt.Unchecked)
         i = i + 1
      i = 0
      while (i < self.mui_plugins.rowCount()):
         if (self.mui_plugins.item(i, 0).checkState() == Qt.Checked or first == 1):
	   if (rec == 1):
	      # Marcamos las dependencias
	      self.arr = self.mui_plugins.item(i,5).text().replace(' ;',';').replace('; ',';').split(QString(";"))
	      if self.arr != ['']:
	         for self.dep in self.arr:
		    if (self.dep == plug):
		       self.desmarcar(self.mui_plugins.item(i,10).text(),1)
		       self.mui_plugins.item(i,0).setCheckState(Qt.Unchecked)
         i = i + 1


   def on_mui_plugins_cellPressed(self, row, col):
      self.estado = self.mui_plugins.item(row,0).checkState()
      # Escribimos la descripcion
      self.mui_descripcion.setText(self.mui_plugins.item(row,1).text() + "<b>" + self.mui_plugins.item(row,10).text() + "</b><br>"+ self.mui_plugins.item(row,3).text() + "<br><b>Categorias:</b> " + self.mui_plugins.item(row,8).text() + "<br><br><b>Dependencias:</b> " + self.mui_plugins.item(row,5).text() + "<br><br><b>Incompatibilidades:</b> " + self.mui_plugins.item(row,6).text() + "<br><br><b>Parches SQL:</b>" + self.mui_plugins.item(row,4).text() + "<br>" + self.mui_plugins.item(row,9).text() + "<br><b>Informes:</b><br>" + self.mui_plugins.item(row,11).text())
     
   def on_mui_plugins_cellClicked(self, row, col):

    if (self.semaforo == 1):
        if (self.estado != self.mui_plugins.item(row,0).checkState()):
            # Comprobamos que esta marcado el checkbox de la fila donde hemos clicado:
            if (self.mui_plugins.item(row, 0).checkState() == Qt.Unchecked):
            # Comprobamos que esta marcado el checkbox de la fila donde hemos clicado:
		self.desmarcar(self.mui_plugins.item(row,10).text(),1,1)
            if (self.mui_plugins.item(row, 0).checkState() == Qt.Checked):
                # Desmarcamos las incompatibilidades
                self.arr = self.mui_plugins.item(row,6).text().replace(' ;',';').replace('; ',';').split(QString(";"))
                if self.arr != ['']:
                    Yes = 'Si'
                    No = 'No'
                    message = QtGui.QMessageBox(self)
                    message.setText('El plugin <b>' +str(self.mui_plugins.item(row,10).text() + "</b> tiene incompatibilidades. Quieres desinstalarlas?"))
                    message.setWindowTitle('Atencion!')
                    message.setIcon(QtGui.QMessageBox.Warning)
                    message.addButton(Yes, QtGui.QMessageBox.AcceptRole)
                    message.addButton(No, QtGui.QMessageBox.RejectRole)
                    message.exec_()
                    respuesta = message.clickedButton().text()
                    if respuesta == Yes:
                        for self.dep in self.arr:
                            self.desmarcar(self.dep,1,1)
                # Marcamos las dependencias
                self.arr = self.mui_plugins.item(row,5).text().replace(' ;',';').replace('; ',';').split(QString(";"))
                if self.arr != ['']:
                    Yes = 'Si'
                    No = 'No'
                    message = QtGui.QMessageBox(self)
                    message.setText('El plugin <b>' +str(self.mui_plugins.item(row,10).text() + "</b> tiene dependencias. Quieres instalarlas?"))
                    message.setWindowTitle('Atencion!')
                    message.setIcon(QtGui.QMessageBox.Warning)
                    message.addButton(Yes, QtGui.QMessageBox.AcceptRole)
                    message.addButton(No, QtGui.QMessageBox.RejectRole)
                    message.exec_()
                    respuesta = message.clickedButton().text()
                    if respuesta == Yes:
                        for self.dep in self.arr:
                            self.marcar(self.dep,1,1)

                    
   def on_mui_plugins1_cellPressed(self, row, col):
      self.estado = self.mui_plugins1.item(row,0).checkState()
      # Escribimos la descripcion
      self.mui_descripcion.setText(self.mui_plugins1.item(row,1).text() + "<b>" + self.mui_plugins1.item(row,10).text() + "</b><br>"+ self.mui_plugins1.item(row,3).text() + "<br><b>Categorias:</b> " + self.mui_plugins1.item(row,8).text() + "<br><br><b>Dependencias:</b> " + self.mui_plugins1.item(row,5).text() + "<br><br><b>Incompatibilidades:</b> " + self.mui_plugins1.item(row,6).text() + "<br><br><b>Parches SQL:</b>" + self.mui_plugins1.item(row,4).text() + "<br>" + self.mui_plugins1.item(row,9).text() + "<br><b>Informes:</b><br>" + self.mui_plugins1.item(row,11).text())
      
   def on_mui_plugins1_cellClicked(self, row, col):
      if (self.semaforo == 1):
        if (self.estado != self.mui_plugins1.item(row,0).checkState()):
            if (self.mui_plugins1.item(row, 0).checkState() == Qt.Unchecked):
            # Comprobamos que esta marcado el checkbox de la fila donde hemos clicado:
		self.desmarcar(self.mui_plugins1.item(row,10).text(),1,1)
            # Comprobamos que esta marcado el checkbox de la fila donde hemos clicado:
            if (self.mui_plugins1.item(row, 0).checkState() == Qt.Checked):
                # Desmarcamos las incompatibilidades
                self.arr = self.mui_plugins1.item(row,6).text().replace(' ;',';').replace('; ',';').split(QString(";"))
                if self.arr != ['']:
                    Yes = 'Si'
                    No = 'No'
                    message = QtGui.QMessageBox(self)
                    message.setText('El plugin <b>' +str(self.mui_plugins1.item(row,10).text() + "</b> tiene incompatibilidades. Quieres desinstalarlas?"))
                    message.setWindowTitle('Atencion!')
                    message.setIcon(QtGui.QMessageBox.Warning)
                    message.addButton(Yes, QtGui.QMessageBox.AcceptRole)
                    message.addButton(No, QtGui.QMessageBox.RejectRole)
                    message.exec_()
                    respuesta = message.clickedButton().text()
                    if respuesta == Yes:
                        for self.dep in self.arr:
                            self.desmarcar(self.dep,1,1)
                # Marcamos las dependencias
                self.arr = self.mui_plugins1.item(row,5).text().replace(' ;',';').replace('; ',';').split(QString(";"))
                if self.arr != ['']:
                    Yes = 'Si'
                    No = 'No'
                    message = QtGui.QMessageBox(self)
                    message.setText('El plugin <b>' +str(self.pluginsbulmatpv[row][1] + "</b> tiene dependencias. Quieres instalarlas?"))
                    message.setWindowTitle('Atencion!')
                    message.setIcon(QtGui.QMessageBox.Warning)
                    message.addButton(Yes, QtGui.QMessageBox.AcceptRole)
                    message.addButton(No, QtGui.QMessageBox.RejectRole)
                    message.exec_()
                    respuesta = message.clickedButton().text()
                    if respuesta == Yes:
                        for self.dep in self.arr:
                            self.marcar(self.dep,1,1)


        
   def actualizarPlugins(self):
      self.writecommand('ACTUALIZANDO PLUGINS')
      
      #Creamos la bara de progreso
      self.progress = QtGui.QProgressBar(self)
      self.progress.setGeometry(self.width() / 2 -100, self.height() /2 -10, 200, 40)
      self.progress.setRange(0, 4000)
      self.progress.show()
      
      # Como los plugins van por orden iteramos sobre el orden para arreglarlo.
      self.x = 1
      while (self.x < 1000) :
        # Iteramos sobre la lista de plugins disponibles en bulmafact
        self.i = 0
        while (self.i < self.mui_plugins.rowCount()):
          # Si el plugin tiene el orden adecuado lo consideramos.
          if (self.mui_plugins.item(self.i,7) <> None and str(self.mui_plugins.item(self.i,7).text()) == str(self.x) ):   
	    self.writecommand('Tratando ' + self.mui_plugins.item(self.i,0).text())
	    if (self.mui_plugins.item(self.i, 0).checkState() == Qt.Checked):
	      self.writecommand('Actualizando ' + self.mui_plugins.item(self.i,0).text())
	      self.writecommand('============ \n ' )
	      # Si hay parche de actualizacion lo aplicamos
	      # En realidad deberia comprobarse si hay archivo para aplicarlo o no en
	      # lugar de comprobar la longitud del archivo.
	      if (len(self.mui_plugins.item(self.i,4).text()) > 4):
		self.command = 'su postgres -c \"psql -t -f  ' + plugins.pathdbplugins + self.mui_plugins.item(self.i,4).text() +' '+ self.database +'\"'
		self.writecommand(self.command)
		self.process.start(self.command)
		self.process.waitForFinished(-1)
		self.writecommand(self.process.readAllStandardOutput())
	  self.i = self.i +1
        self.x = self.x +1
        self.progress.setValue(self.progress.value() + 1)


      # Como los plugins van por orden iteramos sobre el orden para arreglarlo.
      self.x = 1000
      while (self.x >= 1) :
        # Iteramos sobre la lista de plugins disponibles en bulmafact
        self.i = 0
        while (self.i < self.mui_plugins.rowCount()):
          # Si el plugin tiene el orden adecuado lo consideramos.
          if (self.mui_plugins.item(self.i,7) <> None and str(self.mui_plugins.item(self.i,7).text()) == str(self.x) ):   
	    self.writecommand('Tratando ' + self.mui_plugins.item(self.i,0).text())
	    if (self.mui_plugins.item(self.i, 0).checkState() <> Qt.Checked):
	      # Si no esta chequeado hacemos un borrado del plugin
	      if (len(self.mui_plugins.item(self.i,9).text()) > 4):
		# Aplicamos el parche  de borrado.
		self.command = 'su postgres -c \"psql -t -f  ' + plugins.pathdbplugins + self.mui_plugins.item(self.i,9).text() +' '+ self.database +'\"'
		self.writecommand(self.command)
		self.process.start(self.command)
		self.process.waitForFinished(-1)
		self.writecommand(self.process.readAllStandardOutput())
	  self.i = self.i +1
        self.x = self.x - 1
        self.progress.setValue(self.progress.value() + 1)
   
      if (self.mui_soporteTPV.isChecked()):
        # Como los plugins van por orden iteramos sobre el orden para arreglarlo.
        self.x = 1
        while (self.x < 1000) :
          # Iteramos sobre la lista de plugins disponibles en bulmafact
          self.i = 0
          while (self.i < self.mui_plugins1.rowCount()):
            # Si el plugin tiene el orden adecuado lo consideramos.
            if (self.mui_plugins1.item(self.i,7) <> None and str(self.mui_plugins1.item(self.i,7).text()) == str(self.x) ): 
              self.writecommand('Tratando ' + self.mui_plugins1.item(self.i,0).text())
              if (self.mui_plugins1.item(self.i, 0).checkState() == Qt.Checked):
                if (len(self.mui_plugins1.item(self.i,4).text()) > 4):
                  self.writecommand('Actualizando ' + self.mui_plugins1.item(self.i,0).text())
                  self.command = 'su postgres -c \"psql -t -f  ' + plugins.pathdbplugins + self.mui_plugins1.item(self.i,4).text() +' '+ self.database +'\"'
                  self.writecommand(self.command)
                  self.process.start(self.command)
                  self.process.waitForFinished(-1)
                  self.writecommand(self.process.readAllStandardOutput())
            self.i = self.i + 1
          self.x = self.x + 1
          self.progress.setValue(self.progress.value() + 1)
        
        
        # Como los plugins van por orden iteramos sobre el orden para arreglarlo.
        self.x = 1000
        while (self.x >= 1) :
          # Iteramos sobre la lista de plugins disponibles en bulmafact
          self.i = 0
          while (self.i < self.mui_plugins1.rowCount()):
            # Si el plugin tiene el orden adecuado lo consideramos.
            if (self.mui_plugins1.item(self.i,7) <> None and str(self.mui_plugins1.item(self.i,7).text()) == str(self.x) ): 
              self.writecommand('Tratando ' + self.mui_plugins1.item(self.i,0).text())
              if (self.mui_plugins1.item(self.i, 0).checkState() <> Qt.Checked):
                # Si no esta chequeado hacemos un borrado del plugin
                if (len(self.mui_plugins1.item(self.i,9).text()) > 4):
                  # Aplicamos el parche  de borrado.
                  self.command = 'su postgres -c \"psql -t -f  ' + plugins.pathdbplugins + self.mui_plugins1.item(self.i,9).text() +' '+ self.database +'\"'
                  self.writecommand(self.command)
                  self.process.start(self.command)
                  self.process.waitForFinished(-1)
                  self.writecommand(self.process.readAllStandardOutput())
            self.i = self.i + 1
          self.x = self.x - 1
          self.progress.setValue(self.progress.value() + 1)



      self.progress.hide()

   def buscaPlugins(self):
      self.writecommand("Buscando Pluggins")

      #Conectamos con la base de datos
      if (self.database <> None):
        self.conectar(self.database)
      
      #Creamos la bara de progreso
      self.progress = QtGui.QProgressBar()
      self.progress.setGeometry(self.width() / 2 -100, self.height() /2 -10, 200, 40)
      self.progress.setRange(0, len(self.pluginsbulmafact) + len(self.pluginsbulmatpv))
      self.progress.show()
      self.semaforo = 0
      self.mui_plugins.setRowCount(len(self.pluginsbulmafact))
      self.i = 0
      while (self.i < len(self.pluginsbulmafact)):
        self.versioninst = self.buscaPluginInstalado(self.pluginsbulmafact[self.i][3], self.pluginsbulmafact[self.i][1])
        self.check = QTableWidgetItem(QtGui.QApplication.translate("MainWindow", self.pluginsbulmafact[self.i][0], None, QtGui.QApplication.UnicodeUTF8))
        self.check.setFlags(Qt.ItemIsUserCheckable | Qt.ItemIsEnabled)
        self.check.setCheckState(Qt.Unchecked)
        if (self.versioninst != ''):
          self.check.setCheckState(Qt.Checked)
        self.mui_plugins.setItem(self.i, 0, self.check)
        self.mui_plugins.setItem(self.i, 2, QTableWidgetItem(self.versioninst))
        self.mui_plugins.setItem(self.i , 1 , QTableWidgetItem(QtGui.QApplication.translate("MainWindow",self.pluginsbulmafact[self.i][2], None, QtGui.QApplication.UnicodeUTF8)))
        self.mui_plugins.setItem(self.i , 3 , QTableWidgetItem(QtGui.QApplication.translate("MainWindow",self.pluginsbulmafact[self.i][3], None, QtGui.QApplication.UnicodeUTF8)))
        self.mui_plugins.setItem(self.i , 4 , QTableWidgetItem(QtGui.QApplication.translate("MainWindow",self.pluginsbulmafact[self.i][4], None, QtGui.QApplication.UnicodeUTF8)))
        self.mui_plugins.setItem(self.i , 5 , QTableWidgetItem(QtGui.QApplication.translate("MainWindow",self.pluginsbulmafact[self.i][5], None, QtGui.QApplication.UnicodeUTF8)))
        self.mui_plugins.setItem(self.i , 6 , QTableWidgetItem(QtGui.QApplication.translate("MainWindow",self.pluginsbulmafact[self.i][6], None, QtGui.QApplication.UnicodeUTF8)))
        self.mui_plugins.setItem(self.i , 7 , QTableWidgetItem(QtCore.QString.number(self.pluginsbulmafact[self.i][7])))
        self.mui_plugins.setItem(self.i , 8 , QTableWidgetItem(QtGui.QApplication.translate("MainWindow",self.pluginsbulmafact[self.i][8], None, QtGui.QApplication.UnicodeUTF8)))
        self.mui_plugins.setItem(self.i , 9 , QTableWidgetItem(QtGui.QApplication.translate("MainWindow",self.pluginsbulmafact[self.i][9], None, QtGui.QApplication.UnicodeUTF8)))
        self.mui_plugins.setItem(self.i , 10 , QTableWidgetItem(QtGui.QApplication.translate("MainWindow",self.pluginsbulmafact[self.i][1], None, QtGui.QApplication.UnicodeUTF8)))
        self.mui_plugins.setItem(self.i , 11 , QTableWidgetItem(QtGui.QApplication.translate("MainWindow",self.pluginsbulmafact[self.i][10], None, QtGui.QApplication.UnicodeUTF8)))
#        self.mui_plugins.setRowHeight(self.i, 50)
        self.i = self.i + 1
        self.progress.setValue(self.progress.value() + 1)
        
        
      self.mui_plugins1.setRowCount(len(self.pluginsbulmatpv))
      self.i = 0
      while (self.i < len(self.pluginsbulmatpv)):
        self.versioninst = self.buscaPluginInstalado(self.pluginsbulmatpv[self.i][3], self.pluginsbulmatpv[self.i][1])
        self.check = QTableWidgetItem(QtGui.QApplication.translate("MainWindow", self.pluginsbulmatpv[self.i][0], None, QtGui.QApplication.UnicodeUTF8))
        self.check.setFlags(Qt.ItemIsUserCheckable | Qt.ItemIsEnabled)
        self.check.setCheckState(Qt.Unchecked)
        if (self.versioninst != ''):
          self.check.setCheckState(Qt.Checked)
        self.mui_plugins1.setItem(self.i, 0, self.check)
        self.mui_plugins1.setItem(self.i, 2, QTableWidgetItem(self.versioninst))
        self.mui_plugins1.setItem(self.i , 1 , QTableWidgetItem(QtGui.QApplication.translate("MainWindow",self.pluginsbulmatpv[self.i][2], None, QtGui.QApplication.UnicodeUTF8)))
        self.mui_plugins1.setItem(self.i , 3 , QTableWidgetItem(QtGui.QApplication.translate("MainWindow",self.pluginsbulmatpv[self.i][3], None, QtGui.QApplication.UnicodeUTF8)))
        self.mui_plugins1.setItem(self.i , 4 , QTableWidgetItem(QtGui.QApplication.translate("MainWindow",self.pluginsbulmatpv[self.i][4], None, QtGui.QApplication.UnicodeUTF8)))
        self.mui_plugins1.setItem(self.i , 5 , QTableWidgetItem(QtGui.QApplication.translate("MainWindow",self.pluginsbulmatpv[self.i][5], None, QtGui.QApplication.UnicodeUTF8)))
        self.mui_plugins1.setItem(self.i , 6 , QTableWidgetItem(QtGui.QApplication.translate("MainWindow",self.pluginsbulmatpv[self.i][6], None, QtGui.QApplication.UnicodeUTF8)))
        self.mui_plugins1.setItem(self.i , 7 , QTableWidgetItem(QtCore.QString.number(self.pluginsbulmatpv[self.i][7])))
        self.mui_plugins1.setItem(self.i , 8 , QTableWidgetItem(QtGui.QApplication.translate("MainWindow",self.pluginsbulmatpv[self.i][8], None, QtGui.QApplication.UnicodeUTF8)))
        self.mui_plugins1.setItem(self.i , 9 , QTableWidgetItem(QtGui.QApplication.translate("MainWindow",self.pluginsbulmatpv[self.i][9], None, QtGui.QApplication.UnicodeUTF8)))
        self.mui_plugins1.setItem(self.i , 10 , QTableWidgetItem(QtGui.QApplication.translate("MainWindow",self.pluginsbulmatpv[self.i][1], None, QtGui.QApplication.UnicodeUTF8)))
        self.mui_plugins1.setItem(self.i , 11 , QTableWidgetItem(QtGui.QApplication.translate("MainWindow",self.pluginsbulmatpv[self.i][10], None, QtGui.QApplication.UnicodeUTF8)))
        self.i = self.i + 1
        self.progress.setValue(self.progress.value() + 1)
        
      self.semaforo = 1
      self.progress.hide()
      
      #Desconectamos la base de datos
      if (self.database <> None):
        self.desconectar()
      
   def buscaPluginInstalado(self, plugin, libreria):
   
      # Si la base de datos no existe no hace falta comprobar nada.
      if (self.database == None):
        return ''
        
      self.version = self.executeone('SELECT valor FROM configuracion WHERE nombre = \'' + plugin +'\'')

      if (self.version <> None):
        return self.version[0]
      
      self.version = ''
      if (libreria == ''):
        return ''

      self.process1 = QtCore.QProcess()
      self.mfile = QFile(plugins.configfiles + 'bulmafact_' + self.database + '.conf')
      if (self.mfile.exists()):
        self.command = 'grep '+libreria+' '+ plugins.configfiles + 'bulmafact_' + self.database + '.conf'
        self.writecommand(self.command)
        self.process1.start(self.command)
        self.process1.waitForFinished(-1)
        self.version = self.process1.readAllStandardOutput()



      if (self.version == ''):
        self.mfile = QFile(plugins.configfiles + 'bulmatpv_' + self.database + '.conf')
        if (self.mfile.exists()):
          self.command = 'grep '+libreria+' '+ plugins.configfiles + 'bulmatpv_' + self.database + '.conf'
          self.writecommand(self.command)
          self.process1.start(self.command)
          self.process1.waitForFinished(-1)
          self.version = self.process1.readAllStandardOutput()         
         
      if (self.version != ''):
        self.version = '0.12'
      return QString(self.version)
      
   def buscaPlugins1(self):
      self.plugins = self.execQuery('SELECT nombre, valor FROM configuracion WHERE nombre LIKE \'DBRev-%\'')
      self.writecommand(self.plugins)
      self.arrplugins = self.plugins.split(QString("\n"))
      self.mui_plugins.setRowCount(self.arrplugins.count() -3)
      self.i = 0
      while (self.i < self.arrplugins.count() ):
        self.writecommand(self.arrplugins[self.i])
        self.valores = self.arrplugins[self.i].split(QString("|"))
        if (self.valores.count() >= 2):
          self.mui_plugins.setItem(self.i-1 , 1 , QTableWidgetItem(self.valores[1].replace('\n', '')))
          self.mui_plugins.setItem(self.i-1 , 0 , QTableWidgetItem(self.valores[0].replace('\n', '')))
        self.i = self.i + 1
         
   def actualizarDatabase(self):
      # Aplicamos el parche de bulmatpv
      if (self.mui_soporteTPV.isChecked()):
        self.command = 'su postgres -c "psql ' + self.database + ' < '+ plugins.pathdbbulmatpv+'bulmatpv_schema.sql"'
        self.writecommand(self.command)
        self.process.start(self.command)
        self.process.waitForFinished(-1)
         
         
      self.revisiones = ["revf-0.5.9.sql","revf-0.9.1.sql", "revf-0.9.3.sql", "revf-0.10.sql", "revf-0.11.sql", "revf-0.12.sql"]
      #Parcheamos todo lo que hay que parchear
      for self.parche in self.revisiones:
        self.command = 'su postgres -c \"psql -t -f  ' + plugins.pathdbparches + self.parche + ' ' + self.database  + '\"'
        self.writecommand(self.command)
        self.process.start(self.command)
        self.process.waitForFinished(-1)
        self.writecommand(self.process.readAllStandardOutput())
        

   def on_mui_categoria_currentIndexChanged(self, index):
      self.presentar()
  
  
   def on_mui_vertodos_stateChanged(self, status):
      self.presentar()
  
   def presentar(self):
      status = self.mui_vertodos.checkState()
      
      # Hacemos todas las lineas visibles para luego ocultarlas.
      self.i = 0
      while (self.i < self.mui_plugins1.rowCount()):
        self.mui_plugins1.showRow(self.i)
        self.i = self.i + 1

      self.i = 0
      while (self.i < self.mui_plugins.rowCount()):
        self.mui_plugins.showRow(self.i)
        self.i = self.i + 1
      
      if (status == 2):
        # Establezco la tabla de bulmafact
        self.i = 0
        while (self.i < self.mui_plugins.rowCount()):
          if (self.mui_plugins.item(self.i, 0).checkState() != Qt.Checked ):
            self.mui_plugins.hideRow(self.i)
          self.i = self.i + 1
        
        # Establezco la tabla de bulmatpv
        self.i = 0
        while (self.i < self.mui_plugins1.rowCount()):
          if (self.mui_plugins1.item(self.i, 0).checkState() != Qt.Checked ):
            self.mui_plugins1.hideRow(self.i)
          self.i = self.i + 1
          
      if (self.mui_categoria.currentIndex() > 0):
        # Vamos a trabajar con el combo Box
        cat = self.mui_categoria.currentText()
        self.i = 0
        while (self.i < self.mui_plugins.rowCount()):
	  if self.mui_plugins.item(self.i,8) <> None:
	    text = QString(self.mui_plugins.item(self.i,8).text())
	    a = text.contains(cat)
	    if (not a):
	      self.mui_plugins.hideRow(self.i)
          self.i = self.i +1
          
        # Vamos a trabajar con el combo Box
        self.i = 0
        while (self.i < self.mui_plugins1.rowCount()):
	  if self.mui_plugins1.item(self.i,8) <> None:
            text = QString(self.mui_plugins1.item(self.i,8).text())
            a = text.contains(cat)
            if (not a):
              self.mui_plugins1.hideRow(self.i)
          self.i = self.i +1
