#!/usr/bin/python

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
# Las imagenes hay que copiarlas en el directorio desde donde se cargan las imagentes en el bulmatpv, con permisos 775.
# El directorio puede encontrarse en /etc/bulmages/bulmatpv.conf en la opcion "CONF_DIR_THUMB_ARTICLES".


import csv
import subprocess
import sys
import os
import hashlib

urls = csv.reader(open('btpvcatalog_url.csv'), delimiter='	', quotechar='|')
md5 = csv.reader(open('btpvcatalog_url.csv'), delimiter='	', quotechar='|')
parametros = csv.reader(open('btpvcatalog_url.csv'), delimiter='	', quotechar='|')
sql = csv.reader(open('btpvcatalog_url.csv'), delimiter='	', quotechar='|')
errores = []

#Tamanyo de las imagenes
size = "120"

def comprobar_md5():
	
	f = file("captura/" + imagen, 'rb')
	m = hashlib.md5()
	a = f.read()
	m.update(a)
	
	return m.hexdigest()


def descargar_imagen(forzar):
	
	if not imagen in archivos or forzar == 1:
		# No existe la imagen
		
		print "Descargando: " + imagen
				
		retcode = subprocess.call("wget " + list(row)[1] + " -O ./captura/" + imagen + " -o ./captura/logs.txt", shell=True)
		if retcode < 0:
			print >> sys.stderr, "El proceso finalizo con el signal: ", -retcode
		else:
			if retcode == 0:
				print >> sys.stdout, "[OK]"
			else:
				print >> sys.stderr, "Codigo de error devuelto: ", retcode

	return 0


def convertir_imagen():
	#Parametro por defecto
	para = " -background white -flatten -strip -quality 60 -resize " + size + "x" + size + "\!"

	if not list(row)[3] == "nop":
		#Se anyaden los comandos del archivo
		para = row[3] + para

	print "Convirtiendo: " + imagen

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

def comprobar_carpeta(carpeta):
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

def crear_sql():
	
	f = open("bulmatpv_articles.sql", "w")
	
	f.write("BEGIN\n")
	for row in sql:
		if (len(row) > 0):
			if list(row)[1].startswith("http"):
				if not familia + list(row)[0] in errores:
					#Si la imagen del articulo esta correctamente, se anyade al archivo sql
					a = "INSERT INTO articulo(codarticulo, nomarticulo, idfamilia, pvparticulo, codigocompletoarticulo" \
					", pvpivaincarticulo, idtipo_iva)" \
					" VALUES('" + list(row)[0] + "', '" + list(row)[4] + "', " + familia + ", " + list(row)[5] + ", '" \
					+ familia + list(row)[0] + "', " + list(row)[5] + ", "+ "1" + ");\n"
					f.write(a)
			else:
				familia = list(row)[0]
				a = "\nINSERT INTO familia(codigofamilia, nombrefamilia, codigocompletofamilia," \
				" descfamilia) VALUES('" + list(row)[0] + "', '" + list(row)[1] + "', '" + list(row)[0] \
				+ "', '" + list(row)[1] + "');\n\n"
				f.write(a)
	f.write("\nCOMMIT")
	f.close()
	return 0
	
#Descarga
comprobar_carpeta("captura")
archivos = os.listdir("captura")

for row in urls:

	if (len(row) > 0):
		
		if list(row)[1].startswith("http"):
			#Es un articulo
			imagen = familia + list(row)[0]
			descargar_imagen(0)

		else:
			#Es una familia
			
			familia = list(row)[0]

#Comprobacion
comprobar_carpeta("captura")
archivos = os.listdir("captura")

for row in md5:

	if (len(row) > 0):

		if list(row)[1].startswith("http"):

			imagen = familia + list(row)[0]

			if imagen in archivos and list(row)[2] != comprobar_md5():
				# La imagen esta corrupta
				
				os.remove("captura/" + imagen)
				print "La imagen " + imagen + " esta corrupta"
			
				descargar_imagen(1)

		else:

			familia = list(row)[0]

#Conversion
comprobar_carpeta("articles")
for row in parametros:

	if (len(row) > 0):

		if list(row)[1].startswith("http"):

			imagen = familia + list(row)[0]

			convertir_imagen()

		else:

			familia = list(row)[0]


crear_sql()