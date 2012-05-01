import sys
import getopt



class Mensaje:
	"Representa la llegada de un mensaje"
	def __init__(self):
		self.m_mensaje = ""

class Metodo:
	"Representa un metodo invocado"
	def __init__(self):
		self.m_nombreMetodo = ""
		self.m_invocacionesInit = 0
		self.m_invocacionesEnd = 0
	
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
	