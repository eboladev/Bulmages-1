========================================
Software de Gestión Empresarial BulmaGés

(C) 2003 - 2012 Asociación IGLUES
http://www.iglues.org

========================================
#README.es

BulmaGés 0.13.0 (Setiembre 2009)

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

En la actualidad (Mayo 2009), la aplicación de contabilidad (BulmaCont), facturación 
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
MSWindows. Las limitaciones están en los requerimientos, más que en la
suite. El aplicativo puede ser ejecutado en local y en remoto, sin límites de
licencias, empresas o usuarios. 

BulmaGés es una suite de software que incluye varios binarios, una biblioteca, 
un programa de configuración, enlaze con base de datos, múltiples scripts, 
múltiples plugins (pequeñas bibliotecas) y cierta documentación. Todo ello hace
que sea un software complejo. 

BulmaGés está desarrollado por diferentes personas que han colaborado en
el proyecto, algunas activas actualmente y otras no. En general son personas en
torno a la Asociación Iglues (www.iglues.org). 

El ámbito de aplicación del programa es el de empresas, asociaciones o entidades
que deseen llevar un sistema de gestión ya sea a nivel de contabilidad, o de 
factuaración o de venta. El sistema de contabilidad es el hispano, teniendo
incorporados los planes contables españoles. Ahora bien, el sistema de
contabilidad (facturación) se puede adaptar a otros sistemas contables,
únicamente cambiando el plan contable.


Requerimientos
==============

Para poder utilizar BulmaGés hay que compilar el código fuente y realizar una 
instalación. Se pueden realizar pruebas sin instalar el software, pero es
recomendable sólo a nivel de desarrollo. Toda la suite necesita utilizar
una base de datos, en concreto PostgreSql.

Para compilar el código fuente se necesita:

- biblioteca Qt, como mínimo 4.3 http://www.qtsoftware.com/downloads.
Esta es la biblioteca de los widgets, de la interfaz gráfica. 

- python y pyqt4. python 2.4 y el "binding" pyqt4 acorde a la versión de Qt
instalada. 

- cmake, como mínimo versión 2.4.8 http://www.cmake.org/HTML/index.html. Es la
herramienta de construcción del software.

- libpq y postgresql. Se necesita como mínimo PostgreSql 8.1. Para poder
compilar el aplicativo se necesita libpq, que es la biblioteca para poder
acceder a una base de datos PostgreSql. Y PostgreSql para poder ejecutar la
aplicación. http://www.postgresql.org/download/

Para ejecutar todos los programas y plugins además existen componentes adicionales.

- python-psycopg2 para ejecutar correctamente bulmasetup.

- python-reportlab para generar algunos informes.

- python-ooolib y ooolib-perl para los plugins que generan documentos en formato openoffice


Estructura de directorios
=========================

Estos son los directorios principales del git y sus respectivas explicaciones.

- bulmalib
Hay algunas librerias que son comunes a varios de los proyectos. Acceso a PostgreSql,
configuración y algunos widgets raros. Son las librerías principales de la suite.

- bulmages
Contiene el lanzador del programa.

- bulmacont
Directorio del programa bulmacont.
 
- bulmasetup
Directorio del programa bulmasetup, para realizar la configuración del programa.

- bulmafact
Directorio del programa bulmafact.

- installbulmages
En este directorio está la parte de la suite que falta para hacer una
instalación. Archivos de configuración, etc.

- scripts
Contiene scripts utiles para el desarrollo.

- images
Contiene imágenes (recursos) que son usadas junto con el archivo bulmages.qrc
como iconos de los programas.

- ModulesCMake 
Código CMake para poder construir la suite.

- doc
Documentación sobre la suite.


Construir la suite
==================

Se puede obtener de varias manera el directorio con el código fuente de la
suite. Sea descargandose el comprimido (tar.bz2, tar.gz, tgz, zip, etc) o
haciendo un checkout del repositorio subversion: 

svn checkout http://svn.berlios.de/svnroot/repos/bulmages/trunk/bulmages 

Dentro del directorio hay que hacer:

1) mkdir build
2) cd build
3) cmake ../
4) make

Hasta aquí, si todos los requerimientos han sido completados, la suite debería
compilarse sin problemas. Si sólo desea compilar una parte del programa puede
entrar dentro del directorio correspondiente y compilar ejecutando make. Pero
tenga en cuenta que hay una parte mínima que necesita compilar.


5) make install
Instala el software en /usr/local (plataformas Unix),
en %PROGRAMFILES%\bulmages\ en MSWindows.

Notas para la instalación:

El programa depende de ciertos archivos en /etc/bulmages para ejecutarse
correctamente.

Cuando se está desarrollando es engorroso reinstalar para comprobar el
correcto funcionamiento del programa.

En dicho caso, es útil cambiar en la instalación los siguientes archivos
por soft links a los ejecutables y librerías resultantes en nuestro directorio
de compilación.

OJO:
Este es un proceso delicado. Si no se comprende bien, es mejor dejarlo estar.

Antes de hacer esto, hay que realizar una primera instalación para que se generen,
entre otras cosas, los plugins y el directorio de configuración.

Estos son los archivos que se deben enlazar con los binarios que se vayan compilando:
/usr/local/lib/libbulmacont.so.X.Y.Z
/usr/local/lib/libbulmafact.so.X.Y.Z
/usr/local/lib/libbulmalib.so.X.Y.Z
/usr/local/lib/libbulmages.so.X.Y.Z
/usr/local/lib/libbulmatpv.so.X.Y.Z
/usr/local/bin/bulmacont
/usr/local/bin/bulmafact
/usr/local/bin/bulmages
/usr/local/bin/bulmatpv

Además, hay que reemplazar los libpluginb...so.X.Y.Z por enlaces simbólicos a los
plugins sobre los que vayamos a trabajar. Enlazarlos todos sería demasiado laborioso.

De esta forma, siempre se puede tener en ejecución una versión en desarrollo.
Pero teniendo cuidado con los plugins sobre los que se trabaja en cada momento.


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
- realizando copias de seguridad.

Se necesitan privilegios de superusuario de postgres para poder ejecutar este
aplicación. En una instalación típica de GNU/Linux, se ejecuta com permisos de
root, sea con sudo o desde un terminal root.

2) de forma manual

En el directorio share/bulmages/examples hay una serie de scripts que pueden
servir de ejemplo de cómo dar de alta a un usuario, crear una empresa y
realizar las tareas típicas de gestión del programa.


Bugs, comentarios, problemas, etc
=================================

Si tiene algún comentario, sugerencia, error encontrado, lo mejor que puede
hacer es informar a los desarrolladores. Para hacerlo puede:

- subscribirse a la llista de correo de BulmaGés:
https://lists.berlios.de/mailman/listinfo/bulmages-main

lista de tráfico medio donde los desarrolladores y la gente del proyecto
utilizan para intercambiar noticias, dudas, etc sobre BulmaGés.

- poner un bug en el sistema de gestión de bugs de Berlios.

http://developer.berlios.de/bugs/?func=browse&group_id=4472

*ha de estar dado de alta como usuario de Berlios.

Código específico o no liberado
===============================

Puede crear sus propios plugins de proyecto. Recuerde que BulmaGés está
liberado bajo licencia GPL y por tanto sus plugins también lo serán. Ahora
bien, usted no está obligado a hacer públicos sus plugins, por mucho que a
nosotros nos gustaría.

El proyecto BulmaGés tiene como fin promocionar el software libre al nivel de
gestión empresarial y poder fomentar la pequeña industria "local" en este
ámbito. Cree un plugins de personalizaciones para cada uno de sus clientes
y ofrezca un servicio de mantenimiento compilando las últimas versiones de
la suite con sus plugins especificos.

Si tiene código "privado", añadalo creando un fichero CMakeLists.txt.private,
apuntando a los directorios que necesite. De esta forma sus plugins se
compilaran junto con todo el programa, incluso podrá actualizar el repositorio
del programa y mantener sus desarrollos particulares actualizados en todo
momento sin tener que realizar complicadas gestiones.
