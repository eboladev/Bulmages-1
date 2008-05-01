========================================
Software de Gestion Empresarial BulmaGes

(C) 2007, Asociacion Iglues
http://www.iglues.org
========================================

BulmaFact
---------

Este directorio contiene todo el codigo fuente del programa de facturacion 
Bulmafact. Tanto el programa principal como algunos plugins especificos que
se distribuyen junto a este.


Estructura
----------

Dentro del directorio src esta el codigo fuente de la aplicacion.
Cada subdirectorio de src corresponde con a un plugin.

Compilacion
-----------
Este programa se desarrolla normalmente con Debian, Ubuntu y Suse por lo que 
no deberia tener problemas para compilar con estas distribuciones.

Ejecute desde este directorio:
   $ qmake-qt4 (O qmake si usa Suse)
   $ make
   
Si solo desea compilar una parte del programa puede entrar dentro
del directorio correspondiente y compilar.

El programa compilado y los plugins se ubicaran en el directorio installbulmages.

Mire los archivos .pro de cada directorio para comprender mejor el
sistema de compilacion.


Codigo especifico, no liberado etc
----------------------------------
Puede crear sus propios plugins de proyecto sin necesidad de generar complicadas
estructuras para sortear los commits de bulmages y los de sus parches. 

Agrege los directorios que considere necesarios dentro de src y enlacelos con la compilacion
general a traves del archivo privatecode.pro (consulte el archivo bulmafact.pro para entender como
funciona)

De esta forma sus plugins se compilaran junto con todo el programa, incluso podra actualizar
el repositorio del programa y mantener sus desarrollos particulares actualizados en todo momento
sin tener que realizar complicadas gestiones.

Recomendacion: Cree un plugin de personalizaciones para cada uno de sus clientes y ofrezca un
servicio de mantenimiento compilando las ultimas versiones de bulmafact con los plugins especificos.

 