#!/usr/bin/python
# -*- coding: utf-8 -*-

#***************************************************************************
#*   Copyright (C) 2010 by  Albert P. Castell                              *
#*   tr3bl4@gmail.com                                                      *
#*   Copyright (C) 2010 by Fco. Javier M. C.                               *
#*   fcojavmc@todo-redes.com                                               *
#*                                                                         *
#*   This program is free software; you can redistribute it and/or modify  *
#*   it under the terms of the GNU General Public License as published by  *
#*   the Free Software Foundation; either version 2 of the License, or     *
#*   (at your option) any later version.                                   *
#*                                                                         *
#*   This program is distributed in the hope that it will be useful,       *
#*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
#*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
#*   GNU General Public License for more details.                          *
#*                                                                         *
#*   You should have received a copy of the GNU General Public License     *
#*   along with this program; if not, write to the                         *
#*   Free Software Foundation, Inc.,                                       *
#*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
#***************************************************************************
 
# Este script sirve para generar un catalogo de imagenes. El pluginbt_articulograficodb utiliza las imagenes para generar 
# una parrilla grafica con los articulos de la base de datos.

# Al ejecutarlo descarga las imagenes en un directorio llamado "captura" para luego convertilas segun sea necesario en otro directorio 
# llamado "articles". Tambien crea un .sql que es el que se usa para introducir los datos en la base de datos llamado "bulmatpv_articles.sql".

# Para introducir los datos en la base de datos se ejecuta el siguiente comando: psql "nombre db" < bulmatpv_articles.sql
# Las imagenes hay que copiarlas en el directorio desde donde se cargan las imagenes en el bulmatpv, con permisos 775.
# El directorio puede encontrarse en /etc/bulmages/bulmatpv.conf en la opcion "CONF_DIR_THUMB_ARTICLES".


import sys, os, hashlib, csv, subprocess
from iniciobase import *
from about import About


urls = csv.reader(open('btpvcatalog_url.csv'), delimiter='	', quotechar='|')
md5 = csv.reader(open('btpvcatalog_url.csv'), delimiter='	', quotechar='|')
parametros = csv.reader(open('btpvcatalog_url.csv'), delimiter='	', quotechar='|')
sql = csv.reader(open('btpvcatalog_url.csv'), delimiter='	', quotechar='|')
a,b,c = 0,0,0
archivos, errores = "", []
size = "120"


class HelloWindow(QtGui.QMainWindow, Ui_MainWindow):

	def __init__(self, *args):
		global b, c
		QtGui.QWidget.__init__(self,parent = None)

		fi = open("btpvcatalog_url.csv", "r")
		for l in fi:
			b += 1
		c = b * 4
		apply(QtGui.QMainWindow.__init__, (self,) + args)
		self.setupUi(self)
		QtCore.QObject.connect(self.mui_iniciar, QtCore.SIGNAL("clicked()"), self.on_mui_iniciar)
		QtCore.QObject.connect(self.mui_pushButton_salir, QtCore.SIGNAL("clicked()"), self.on_mui_cancelar)
		self.label_tarea.setText("Tarea: 0/" + str(b))
		self.label_completado.setText("Completado: 0/" + str(c) )
		self.label_imagen.setText("Imagen actual" )

	
	def on_mui_cancelar(self):
		sys.exit()
		return 0


	def comprobar_carpeta(self, carpeta):
		if os.path.exists(carpeta):
			#La carpeta ya existe
			pass
		else:
			#Se intenta crear la carpeta, si falla acaba el programa
			print "Creando carpeta " + carpeta
			try:
				os.makedirs(carpeta)
			except OSError, e:
				print "No se puede crear la carpeta:", e
				sys.exit()

		return 0


	def descargar_imagen(self, row, imagen, forzar):
		
		global archivos
		
		if not imagen in archivos or forzar == 1:
			# No existe la imagen
			
			print "Descargando: " + imagen
			QtGui.QApplication.processEvents()	
			retcode = subprocess.call("wget -T 30 " + list(row)[1] + " -O ./captura/" + imagen + " -o ./captura/logs.txt", shell=True)
			if retcode < 0:
				print >> sys.stderr, "El proceso finalizo con el signal: ", -retcode
			else:
				if retcode == 0:
					print >> sys.stdout, "[OK]"
				else:
					print >> sys.stderr, "Codigo de error devuelto: ", retcode

		return 0


	def comprobar_md5(self, imagen):

		f = file("captura/" + imagen, 'rb')
		m = hashlib.md5()
		a = f.read()
		m.update(a)
		
		return m.hexdigest()


	def convertir_imagen(self, row, imagen, familia):
		
		global errores
		
		#Parametro por defecto
		para = " -background white -flatten -strip -quality 60 -resize " + size + "x" + size + "\!"

		if not list(row)[3] == "nop":
			#Se anyaden los comandos del archivo
			para = row[3] + para

		print "Convirtiendo: " + imagen
		QtGui.QApplication.processEvents()
		retcode = subprocess.call("convert" + " captura/" + imagen + " " + para + " articles/" + imagen + ".jpg", shell=True)
		if retcode < 0:
			print >> sys.stderr, "El proceso finalizo con el signal: ", -retcode
		else:
			if retcode == 0:
				print >> sys.stdout, "[OK]"
			else:
				print >> sys.stderr, "Codigo de error devuelto: ", retcode
				errores.append(familia + list(row)[0])
				
		return 0


	def crear_sql(self, fam, row, familia):

		if fam == 0:
			#Si la imagen del articulo esta correctamente, se anyade al archivo sql
			a = "INSERT INTO articulo(codarticulo, nomarticulo, idfamilia, pvparticulo, codigocompletoarticulo" \
			", pvpivaincarticulo, idtipo_iva)" \
			" VALUES('" + list(row)[0] + "', '" + list(row)[4] + "', " + familia + ", " + list(row)[5] + ", '" \
			+ familia + list(row)[0] + "', " + list(row)[5] + ", "+ "1" + ");\n"

		else:

			a = "\nINSERT INTO familia(codigofamilia, nombrefamilia, codigocompletofamilia," \
			" descfamilia) VALUES('" + list(row)[0] + "', '" + list(row)[1] + "', '" + list(row)[0] \
			+ "', '" + list(row)[1] + "');\n\n"

		return a


	def on_mui_iniciar(self):

		self.mui_iniciar.setEnabled(False)
		self.mui_pushButton_salir.setText("Cancelar")
		global a, b, c, urls, md5, archivos, parametros, size, errores

		#Descarga
		self.comprobar_carpeta("captura")
		archivos = os.listdir("captura")

		for row in urls:
			QtGui.QApplication.processEvents()
			a += 1
			self.label_tarea.setText("Descargando: " + str(a) + "/" + str(b))
			self.label_completado.setText("Completado: " + str(a) + "/" + str(b * 4))
			if (len(row) > 0):

				if list(row)[1].startswith("http"):
					#Es un articulo

					imagen = familia + list(row)[0]
					self.label_imagen.setText("Imagen:  " + imagen )
					QtGui.QApplication.processEvents()
					self.descargar_imagen(row , imagen, 0)
					
				else:

					#Es una familia
					familia = list(row)[0]
			self.mui_progressBar.setValue((a * 100) / b)	
			self.mui_progressBar_2.setValue(((a * 100) / b) / 4)


		#Comprobacion
		a = 0
		for row in md5:
			QtGui.QApplication.processEvents()
			a += 1
			self.label_tarea.setText("Comprobando: " + str(a) + "/" + str(b))
			self.label_completado.setText("Completado: " + str(a + b) + "/" + str(b * 4))
			if (len(row) > 0):

				if list(row)[1].startswith("http"):
					imagen = familia + list(row)[0]
					self.label_imagen.setText("Imagen:  " + imagen )
					if imagen in archivos and list(row)[2] != self.comprobar_md5(imagen):
						# La imagen esta corrupta

						os.remove("captura/" + imagen)
						print "La imagen " + imagen + " esta corrupta"
						self.descargar_imagen(row, imagen, 1)

				else:

					familia = list(row)[0]	
			self.mui_progressBar.setValue((a * 100) / b)	
			self.mui_progressBar_2.setValue((((a * 100) / b) / 4) + 25)

		#Conversion
		self.comprobar_carpeta("articles")
		a = 0
		for row in parametros:
			QtGui.QApplication.processEvents()
			a += 1
			self.label_tarea.setText("Convirtiendo: " + str(a) + "/" + str(b))
			self.label_completado.setText("Completado: " + str(a + (b * 2)) + "/" + str(b * 4))

			if (len(row) > 0):

				if list(row)[1].startswith("http"):

					imagen = familia + list(row)[0]
					self.label_imagen.setText("Imagen:  " + imagen )
					QtGui.QApplication.processEvents()
					self.convertir_imagen(row, imagen, familia)
					

				else:

					familia = list(row)[0]

			self.mui_progressBar.setValue((a * 100) / b)
			self.mui_progressBar_2.setValue((((a * 100) / b) / 4) + 50)


		#Creacion SQL
		a = 0
		f = open("bulmatpv_articles.sql", "w")
		f.write("BEGIN\n")

		for row in sql:
			QtGui.QApplication.processEvents()
			a += 1
			self.label_tarea.setText("Creando SQL: " + str(a) + "/" + str(b))
			self.label_completado.setText("Completado: " + str(a + (b * 3)) + "/" + str(b * 4))
			if (len(row) > 0):
				if list(row)[1].startswith("http"):
					if not familia + list(row)[0] in errores:

						imagen = familia + list(row)[0]
						f.write(self.crear_sql(0, row, familia))
						self.label_imagen.setText("Imagen:  " + imagen )

				else:
					
					familia = list(row)[0]
					
					f.write(self.crear_sql(1, row, familia))
			self.mui_progressBar.setValue((a * 100) / b)
			self.mui_progressBar_2.setValue((((a * 100) / b) / 4) + 75)


		f.write("\nCOMMIT")
		f.close()
		self.mui_pushButton_salir.setText("Cerrar")
		self.label_tarea.setText("Finalizado")


	#def on_mui_about(self):

		#win = About()
		#win.exec_()



def main(args):

	app=QtGui.QApplication(args)
	app.processEvents()
	win=HelloWindow()

	win.show()
	
	app.exec_()
	
if __name__=="__main__":
	main(sys.argv)