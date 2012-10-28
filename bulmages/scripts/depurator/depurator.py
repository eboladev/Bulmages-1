__version__ = "$Revision: 0.1 $"[11:-2]
__date__ = "$Date: 2007/01/07 08:09:13 $"[7:-2]
__author__ = "Tomeu Borras <tborras@conetxia.com>"
__all__ = ["depurator"]
__doc__ = "Sistema de Depuracion para BulmaGes a partir de los logs.\r\n"

import sys
import re
import os
from principal import *
from PyQt4.QtGui import *
from PyQt4.QtCore import *


class HelloWindow(QtGui.QMainWindow, Ui_Principal):
    def __init__(self, *args):
        apply(QtGui.QMainWindow.__init__, (self,) + args)
	self.setupUi(self)

	self.table.setColumnCount(3)
	self.table.hideColumn(1)
	self.table.hideColumn(2)
	self.table.setColumnWidth(0,450)
	
	self.semaforo = 0
	

	self.f = None

	self.t = QtCore.QTimer()
	self.connect(self.t, QtCore.SIGNAL("timeout()"), self.hazalgo)
	self.connect(self.mui_clear, QtCore.SIGNAL("triggered()"), self.on_mui_clear_clicked)
	self.connect(self.mui_conectar, QtCore.SIGNAL("triggered()"), self.on_mui_conectar_clicked)
	self.connect(self.mui_abrir_traza, QtCore.SIGNAL("triggered()"), self.on_mui_abrir_traza_clicked)
	self.t.start(10)

    def on_mui_abrir_traza_clicked(self):
        print "Abriendo!!"
        self.openfile = QFileDialog.getOpenFileName(self,"Traza - Elija archivo de traza", "~/.bulmages","XML (*.xml)")
	self.f = open(self.openfile, 'r')

    def on_mui_conectar_clicked(self):
	print "Conexion !!"
	
<<<<<<< HEAD
	self.f = open(os.getenv("HOME")+"/.bulmages/bulmagesout.xml", 'r')	
	self.f.seek(0,2)

    def on_mui_tableclear_triggered(self):
	self.table.clear()
	self.table.setRowCount(0)
	
    def on_mui_clear_clicked(self):
	self.table.clear()
	self.table.setRowCount(0)
	self.tree.clear()

    def on_mui_expandtree_triggered(self):
	print "Expansion !!"
#	self.tree.expandAll()


    def ejecuta(self):
	line = self.f.readline(300)
	if line != "":
		self.lmen.procesaMensaje(line.replace("\n", ""))

    def hazalgo(self):
        if (self.f == None):
	  return
        if (self.semaforo == 0):
	  self.semaforo=1
	else:
	  return
	if self.table.rowCount() > 500:
		self.table.clear()
		self.table.setRowCount(0)
		
	while self.table.rowCount() > 1000:
		self.table.removeRow(0)
		
	line = self.f.readline(3000)
	i=0
	while len(line) > 3 and i < 100:
		if len(line) > 3:
			self.procesaMensaje(line)
		line = self.f.readline(3000)
		i = i + 1
	self.semaforo=0

    def sacaMensaje(self, mens):
	self.table.insertRow(self.table.rowCount())
	item = QtGui.QTableWidgetItem(mens)
	self.table.setItem(self.table.rowCount() - 1, 0, item)
	item1 = QtGui.QTableWidgetItem("hola 1")
	self.table.setItem(self.table.rowCount() - 1, 1, item1)
	item2 = QtGui.QTableWidgetItem("hola 2")
	self.table.setItem(self.table.rowCount() - 1, 2, item2)
	self.table.scrollToItem(item)
	
	
    def procesaMensaje(self, mens):
	sacamensaje = 0	
	mensajefin = 1
	tiempo = 0

	mensaje = mens.strip()
	# Descartamos el sistema de comentarios.
	if mensaje.startswith("<comment"):
		self.sacaMensaje(mensaje)
		return
	if mensaje.startswith("</comment"):
		return
		
	# Quitamos los argumentos entre parentesis.
	matchObj = re.search('(\(.*\))', mens)
	if matchObj != None:
		mensaje = mensaje.replace (matchObj.group(1), " ")
	else:
		print "Descartado: "+ mensaje
		
	if mensaje.startswith("</"):
		mensaje = mensaje.replace("</", "  ")
		mensajefin = 2
		# Es un mensaje de cierre
		partmsg = mensaje.split(" ")
		if len (partmsg) > 4:
		    mensaje = partmsg[4]
		if not "::" in mensaje:
		    if len ( partmsg) > 3:
		      mensaje = partmsg[3]
		if not "::" in mensaje:
		  mensaje = partmsg[2]
		if not "::" in mensaje:
		  mensaje = partmsg[1]
		if not "::" in mensaje:
		    mensaje = partmsg[0]
		if not "::" in mensaje:
		    print "Descartado: " + mensaje
		    return
		  
		# Buscamos el tiempo invertido
		matchObj = re.search('time=\"([0-9]+)\"', mens)
		if matchObj != None:
		    tiempo = int(matchObj.group(1))
		  
	else:
		mensaje = mensaje.replace("<",  " ")
		# Es un mensaje de apertura
		partmsg = mensaje.split(" ")
		if len (partmsg) > 4:
		    mensaje = partmsg[4]
		if not "::" in mensaje:
		    if len ( partmsg) > 3:
		      mensaje = partmsg[3]
		if not "::" in mensaje:
		  mensaje = partmsg[2]
		if not "::" in mensaje:
		  mensaje = partmsg[1]
		if not "::" in mensaje:
		    mensaje = partmsg[0]
		if not "::" in mensaje:
		    print "Descartado: " + mensaje
		    return
		  
		  


		  
	lmensaje = mensaje.split("::")
	if len(lmensaje) < 2:
		return
	if len (lmensaje) > 2:
	  	submens = lmensaje[2].split(" ")
		lmensaje[1] = submens[0].split("(")[0]

	else:
		# Ya tenemos clase y subclase.
		submens = lmensaje[1].split(" ")
		lmensaje[1] = submens[0].split("(")[0]

	lis = QtCore.QStringList(lmensaje[0])
	lis1 = QtCore.QStringList(lmensaje[1])
	if mensajefin == 2:
		lis1.insert(1, "0")
		lis1.insert(2, "1")
		lis.insert(1, "0")
		lis.insert(2, "1")
	else:
		lis1.insert(1, "1")
		lis1.insert(2, "0")
		lis.insert(1, "1")
		lis.insert(2, "0")


	ttiempo=0
	ttiempomax=0
	ttiempomin=9999999
	# Tratamos el arbol.
	
	ldtree = self.tree.findItems(lmensaje[0], QtCore.Qt.MatchExactly)
	if len(ldtree) > 0:
		titem = ldtree[0]
		texto = titem.text(mensajefin).toInt()
		texto = texto[0] + 1
		titem.setText(mensajefin, QtCore.QString.number(texto))
		# Ponemos el tiempo total consumido en la funcion.

		if str(titem.text(3)) <> "":
		  ttiempo = int(str(titem.text(3)))
		titem.setText(3,QtCore.QString.number(ttiempo + tiempo))
		if str(titem.text(4)) <> "":
		    ttiempomax = int(str(titem.text(4)))
		if tiempo > ttiempomax:
		    titem.setText(4,QtCore.QString.number(tiempo))
		if str(titem.text(5)) <> "":
		    ttiempomin = int(str(titem.text(5)))
		if tiempo < ttiempomin:
		    titem.setText(5,QtCore.QString.number(tiempo))


		# Bajamos en el arbol de llamadas y vamos actualizando los datos.
		encontrado = 0
		i = 0
		while i < titem.childCount():
			titem1 = titem.child(i)
			texto = titem1.text(0)
			if texto == lmensaje[1]:
				encontrado = 1
				texto = titem1.text(mensajefin).toInt()
				texto = texto[0] + 1
				titem1.setText(mensajefin, QtCore.QString.number(texto))
				if titem1.checkState(0) == QtCore.Qt.Checked:
					sacamensaje = 1
					
				# Ponemos el tiempo total consumido en la funcion.
				ttiempo=0
				ttiempomax=0
				ttiempomin=999
				if str(titem1.text(3)) <> "":
				  ttiempo = int(str(titem1.text(3)))
				titem1.setText(3,QtCore.QString.number(ttiempo + tiempo))
				if str(titem1.text(4)) <> "":
				   ttiempomax = int(str(titem1.text(4)))
				if tiempo > ttiempomax:
				   titem1.setText(4,QtCore.QString.number(tiempo))
				if str(titem1.text(5)) <> "":
				   ttiempomin = int(str(titem1.text(5)))
				if tiempo < ttiempomin:
				   titem1.setText(5,QtCore.QString.number(tiempo))

				  
			i = i +1
				

		# Si no hemos encontrado el elemento lo insertamos.
		if encontrado == 0:
			titem1 = QtGui.QTreeWidgetItem(lis1)
			titem1.setCheckState(0, QtCore.Qt.Unchecked)
			titem.addChild(titem1)
			# Ponemos el tiempo total consumido en la funcion.
			titem1.setText(3,QtCore.QString.number(tiempo))
			titem1.setText(4,QtCore.QString.number(ttiempomax))
			titem1.setText(5,QtCore.QString.number(ttiempomin))
			titem1.setTextAlignment(1,Qt.AlignRight)
			titem1.setTextAlignment(2,Qt.AlignRight)
			titem1.setTextAlignment(3,Qt.AlignRight)
			titem1.setTextAlignment(4,Qt.AlignRight)
			titem1.setTextAlignment(5,Qt.AlignRight)


			
		
	else:
		# Agregamos la clase y el metodo (una clase nunca va sola)
		titem =  QtGui.QTreeWidgetItem(lis)
#		titem.setCheckState(0, QtCore.Qt.Unchecked)
		titem.setTextColor(0, QtGui.QColor(255,0,0))
		# Ponemos el tiempo total consumido en la funcion.
		titem.setText(3,QtCore.QString.number(tiempo))
		titem.setText(4,QtCore.QString.number(ttiempomax))
		titem.setText(5,QtCore.QString.number(ttiempomin))
		titem.setTextAlignment(1,Qt.AlignRight)
		titem.setTextAlignment(2,Qt.AlignRight)
		titem.setTextAlignment(3,Qt.AlignRight)
		titem.setTextAlignment(4,Qt.AlignRight)
		titem.setTextAlignment(5,Qt.AlignRight)
		self.tree.addTopLevelItem(titem)

		
		titem1 = QtGui.QTreeWidgetItem(lis1)
		titem1.setCheckState(0, QtCore.Qt.Unchecked)
		titem.addChild(titem1)
		# Ponemos el tiempo total consumido en la funcion.
		titem1.setText(3,QtCore.QString.number(tiempo))
		titem1.setText(4,QtCore.QString.number(ttiempomax))
		titem1.setText(5,QtCore.QString.number(ttiempomin))
		titem1.setTextAlignment(1,Qt.AlignRight)
		titem1.setTextAlignment(2,Qt.AlignRight)
		titem1.setTextAlignment(3,Qt.AlignRight)
		titem1.setTextAlignment(4,Qt.AlignRight)
		titem1.setTextAlignment(5,Qt.AlignRight)

	if sacamensaje == 1:
		self.sacaMensaje(mens)
	
	


def main(args):
    app=QtGui.QApplication(args)
    win=HelloWindow()
    win.show()
    app.connect(app, QtCore.SIGNAL("lastWindowClosed()"),
                app, QtCore.SLOT("quit()"))
    app.exec_()

if __name__=="__main__":
    main(sys.argv)
=======
	def imprimir(self):
		print "      " + self.m_nombreMetodo + "  %s " % self.m_invocacionesInit

class Clase:
	"Representa una clase"
        def __init__(self):
                self.m_nombreClase = ""
		self.m_invocaciones = 0
		self. m_listaMetodos = {}

	def procesaMensaje(self, mens):
		inicial = 1
		final = 1
		mensaje = mens.strip()
		# Descartamos el sistema de comentarios.
		if mensaje.startswith("<comment"):
			return
		if mensaje.startswith("</comment"):
			return
		if mensaje.startswith("</"):
			inicial = 0
			# Es un mensaje de cierre
			partmsg = mensaje.split(" ")
			if len ( partmsg) > 3:
			  mensaje = partmsg[3]
			else:
			  return
			if not "::" in mensaje:
			  mensaje = partmsg[2]
			if not "::" in mensaje:
			  mensaje = partmsg[1]
			if not "::" in mensaje:
			  return
		else:
			final = 0
			# Es un mensaje de cierre
			partmsg = mensaje.split(" ")
			if len ( partmsg) > 3:
			  mensaje = partmsg[3]
			else:
			  return

			if not "::" in mensaje:
			  mensaje = partmsg[2]
			if not "::" in mensaje:
			  mensaje = partmsg[1]
			if not "::" in mensaje:
			  return
			  
		lmensaje = mensaje.split("::")
		if len(lmensaje) < 2:
			return
			
		submens = lmensaje[1].split(" ")
		lmensaje[1] = submens[0].split("(")[0]
		
		print "Metodo es: "+ lmensaje[1]

		if lmensaje[1] in self.m_listaMetodos:
			a = self.m_listaMetodos[lmensaje[1]]
			a.m_invocacionesInit = a.m_invocacionesInit + inicial
			a.m_invocacionesEnd = a.m_invocacionesEnd + final
			self.m_listaMetodos[lmensaje[1]] = a
		else:
			# los cierres no los contamos, aunque si los mostramos y asi no descuadramos el computo.
			a = Metodo()
			a.m_nombreMetodo = lmensaje[1]
			a.m_invocacionesInit = inicial
			self.m_listaMetodos[lmensaje[1]] = a

	def imprimir(self):
		print self.m_nombreClase + "  %s" % self.m_invocaciones
		for x in self.m_listaMetodos:
			self.m_listaMetodos[x].imprimir()

class ListaClases:
	"Listado de clases"
	def __init__(self):
		self.m_listaClases = {}

	def procesaMensaje(self, mens):
		# Buscamos la clase del mensaje.
		mensaje = mens.strip()
		#Quitamos espacios en blanco.
		# print mensaje
		
		
		# Descartamos el sistema de comentarios.
		if mensaje.startswith("<comment"):
			return
		if mensaje.startswith("</comment"):
			return
		if mensaje.startswith("</"):
			# Es un mensaje de cierre
			partmsg = mensaje.split(" ")
			if len ( partmsg) > 3:
			  mensaje = partmsg[3]
			else:
			  return

			if not "::" in mensaje:
			  mensaje = partmsg[2]
			if not "::" in mensaje:
			  mensaje = partmsg[1]
			if not "::" in mensaje:
			  return
		else:
			# Es un mensaje de cierre
			partmsg = mensaje.split(" ")
			if len ( partmsg) > 3:
			  mensaje = partmsg[3]
			else:
			  return

			if not "::" in mensaje:
			  mensaje = partmsg[2]
			if not "::" in mensaje:
			  mensaje = partmsg[1]
			if not "::" in mensaje:
			  return
			  
		lmensaje = mensaje.split("::")
		if len(lmensaje) < 2:
			return
		print "La clase es: " + lmensaje[0]
		
		if lmensaje[0] in self.m_listaClases:
			a = self.m_listaClases[lmensaje[0]]
			a.m_invocaciones = a.m_invocaciones +1
			a.procesaMensaje(mens)
			self.m_listaClases[lmensaje[0]] = a
		else:
			a = Clase()
			a.m_nombreClase = lmensaje[0]
			a.m_invocaciones = 1
			a.procesaMensaje(mens)
			self.m_listaClases[lmensaje[0]] = a

	def imprimir(self):
		for x in self.m_listaClases:
			self.m_listaClases[x].imprimir()


if (__name__=='__main__'):
	lmen = ListaClases()
		
#	f = open("~/.bulmages/bulmagesout.txt", 'r')
	f = open("/home/tborras/.bulmages/bulmagesout.xml", 'r')
#	f.seek(0,2)
	while 1:
		line = f.readline(300)
		if line != "":
			print line.strip()
			lmen.procesaMensaje(line.replace("\n", ""))

	lmen.imprimir()
	
>>>>>>> release
