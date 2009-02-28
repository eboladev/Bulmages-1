========================================
Software de Gestion Empresarial BulmaGes

(C) 2003 - 2009 Asociacion IGLUES
http://www.iglues.org
========================================
#README.es

BulmaGés 0.11.1 (Mayo 2008)

Contenido
---------

- Introducción
- Características
- Requerimientos
- Estructura de directorios
- Construir la suite
- Puesta en marcha
- Bugs, comentarios, problemas, etc
- Código específico o no liberado

Introducción
============

BulmaGés es una aplicación modular que pretende cubrir las necesidades de
gestión empresarial de cualquier pequeña y mediana empresa dentro del entorno
del software libre. Se identifican claramente cuatro áreas que en las que el
software puede actuar: 
a) Contabilidad - BulmaCont
b) Facturación - BulmaFact
c) Terminal Punto de Venta - BulmaTPV

En la actualidad (Mayo 2008), la aplicación de contabilidad (BulmaCont), facturación 
(BulmaFact) y el terminal Punto de Venta (BulmaTPV) ya son operativos.

Características
===============

Aunque todas estas aplicaciones están pensadas para funcionar como una sola, su
modularidad permite que funcione cada una de forma independiente, facilitando
así la migración progresiva desde otras aplicaciones o la adopción sólo de
aquellas herramientas que sean necesarias para la empresa. BulmaGés es software
libre y está liberado bajo licencia GPL, se puede obtener, tanto el software
como el código fuente, de forma gratuita a través de Internet o de las
diferentes distribuciones (CD) que facilita la asociación IGLUES. 

BulmaGés ofrece la máxima calidad de una completa herramienta de gestión
integrada configurable para adaptarse a las necesidades de la empresa mediante
un sistema de módulos y plugins en función del sector en el que se opere y el
tamaño de la organización.

BulmaGés puede funcionar en múltiples plataformas. Los desarrolladores trabajan
principalmente con GNU/Linux. También se puede utilizar en MacOSX y en
MSWindows. Las limitaciones estan en los requerimientos, más que en la
suite. El aplicativo puede ser ejecutado en local y en remoto, sin límites de
licencias, empresas o usuarios. 

BulmaGés es una suite de software que incluye varios binarios, una biblioteca, 
un programa de configuración, enlaze con base de datos, múltiples scripts, 
múltiples plugins (pequeñas bibliotecas) y cierta documentación. Todo ello hace
que sea un software complejo. 

BulmaGés está desarrollado por diferentes personas que han colaborado en
el proyecto, algunas activas actualmente y otras no. En general són personas en
torno a la Asociación Iglues (www.iglues.org). 

El ámbito de aplicación del programa es el de empresas, asociaciones o entidades
que deseen llevar un sistema de gestión ya sea a nivel de contabilidad, o de 
factuaración o de venta. El sistema de contabilidad és el hispano, teniendo
incorporados los planes contables españoles. Ahora bien, el sistema de
contabilidad (facturación) se puede adaptar a otros sistemas contables,
únicamente cambiando el plan contable.


Requerimientos
==============

Para poder utilizar BulmaGés hay que compilar el código fuente y realizar una 
instalación. Se pueden realizar pruebas sin instalar el software, pero es
recomendable sólo a nivel de desarrollo. Toda la suite necesita ulitzar
una base de datos, en concreto Postgresql.

Para compilar el código fuente se necesita:

- biblioteca Qt, como mínimo 4.3. http://trolltech.com/downloads/opensource
Esta és la biblioteca de los widgets, de la interfície gráfica. 

- python y pyqt4. python 2.4 y el "binding" pyqt4 acorde a la versión de Qt
instalada. 

- cmake, como mínimo versión 2.4.8 http://www.cmake.org/HTML/index.html. És la
herramienta de construcción del software.

- libpq y postgresql. Se necesita como mínimo postgresql 8.1. Para poder
compilar el aplicativo se necesita libpq, que és la biblioteca para poder
acceder a una base de datos Postgresql. Y Postgresql para poder ejecutar la
aplicación. http://www.postgresql.org/download/


Estructura de directorios
=========================

Estos son los directorios principales del svn y sus respectivas explicaciones.

- bulmalib 
Hay algunas librerias que son comunes a varios de los proyectos. postgresiface,
configuracion, algunos widgets raros. Son las librerias principales de la suite.

- bulmages 
Contiene el lanzador del programa. 

- bulmacont
Directorio del programa bulmacont.
 
- bulmasetup
Directorio del programa bulmasetup, para realizar la configuración del programa.

- bulmafact
Directorio del programa bulmafact.

- installbulmages 
En este directorio esta la parte de la suite que falta para hacer una
instalación. Archivos de configuración, etc.

- scripts
Contiene scripts utiles para el desarrollo.

- images
Contiene imagenes (recursos) que son usadas junto con el archivo bulmages.qrc
como iconos de los programas.

- ModulesCMake    
Código CMake para poder construir la suite.

- doc
Documencatión sobre la suite.


Construir la suite
==================

Se puede obtener de varias manera el directorio con el código fuente de la
suite. Sea descargandose el comprimido (tar.bz2, tar.gz, tgz, zip, etc) o
haciendo un checkout del repositorio subversion: 

svn checkout http://svn.berlios.de/svnroot/repos/bulmages/trunk/bulmages 

Dentro del directorio hay que hacer:

1)	mkdir build
2)	cd build
3)	cmake ../
4)	make

hasta aquí, si todos los requerimientos han sido completados, la suite deberia
compilarse sin problemas. Si solo desea compilar una parte del programa puede
entrar dentro del directorio correspondiente y compilar ejecutando make. Pero
tenga en cuenta que hay una parte mínima que necesita compilar.


5) make install
Instala el software en /usr/local (plataformas Unix), 
en %PROGRAMFILES%\bulmages\ en MSWindows.

Notas para la instalacion:

El programa depende de ciertos archivos en /etc para ejecutarse correctamente.
Sin embargo cuando se esta desarrollando es engorroso reinstalar para
comprobar el correcto funcionamiento del programa.

En dicho caso es útil cambiar en la instalacion los siguientes archivos
por soft links a los ejecutables resultantes en el directorio installbulmages
del directorio de desarrollo.

Estos son los archivos que se deben enlazar:
/usr/lib/libbulmages.X.Y.Z.so
/usr/bin/bulmages
/usr/bin/bulmafact
/usr/bin/bulmacont

De esta forma siempre se puede tener en ejecucion una version en desarrollo


Puesta en marcha
================

Una vez instalada la suite se tiene que poner en marcha las bases de datos, los
usuarios y las empresas. Este proceso se puede realizar principalmente de dos
formas: utilizando bulmasetup o de forma manual.

1) bulmasetup

BulmaSetup es un aplicativo que ayuda a realizar la puesta en marcha de la
suite,
- dando de alta usuarios a la base de datos
- creando empresas para la contabilidad, facturación/tpv
- realizando cópias de seguridad.

Se necesitan privilegios de superusuario de postgres para poder ejecutar este
aplicación. En una instalación típica de GNU/Linux, se ejecuta com permisos de
root, sea con sudo o desde un terminal root.

2) de forma manual

En el directorio share/bulmages/examples hay una série de scripts que pueden
servir de ejemplo de como dar de alta a un usuario, crear una empresa y
realizar las tareas típicas de gestión del programa.


Bugs, comentarios, problemas, etc
=================================

Si tiene algun comentario, sugerencia, error encontrado, lo mejor que puede
hacer es informar a los desarrolladores. Para hacerlo puede:

- subscribirse a la llista de correo bulmages:
http://llistes.bulma.net/mailman/listinfo/bulmages

lista de tráfico medio donde los desarrolladores y la gente del proyecto
utilizan para intercambiar noticias, dudas, etc sobre bulmages.

- poner un bug en el sistema de gestión de bugs de berlios.

http://developer.berlios.de/bugs/?func=browse&group_id=4472

*ha de estar dado de alta como usuario de berlios.

Código específico o no liberado
===============================

Puede crear sus propios plugins de proyecto. Recuerde que BulmaGés está
liberado bajo licencia GPL y por tanto sus plugins también lo serán. Ahora
bien, usted no está obligado a hacer públicos sus plugins, por mucho que a
nosotros nos gustaria. 

El proyecto BulmaGés tiene como fin promocionar el software libre al nivel de
gestión empresarial y poder fomentar la pequeña industria "local" en éste
ámbito. Cree un plugins de personalizaciones para cada uno de sus clientes
y ofrezca un servicio de mantenimiento compilando las últimas versiones de
la suite con sus plugins especificos.

Si tiene código "privado", añadalo creando un fichero CMakeLists.txt.private,
apuntando a los directorios que necesite. De esta forma sus plugins se
compilaran junto con todo el programa, incluso podra actualizar el repositorio
del programa y mantener sus desarrollos particulares actualizados en todo
momento sin tener que realizar complicadas gestiones.


 
