/*******************************************************************************
 *                                                                             *
 *   Copyright (C) 2009-2010 by Fco. Javier M. C.                              *
 *   comercial@todo-redes.com                                                  *
 *   http://www.todo-redes.com                                                 *
 *                                                                             *
 *  Permission is granted to copy, distribute and/or modify this document      *
 *  under the terms of the GNU Free Documentation License, Version 1.3         *
 *  or any later version published by the Free Software Foundation;            *
 *  with no Invariant Sections, no Front-Cover Texts, and no Back-Cover Texts. *
 *  A copy of the license is included in the section entitled "GNU             *
 *  Free Documentation License".                                               *
 *                                                                             *
 *******************************************************************************/


ENTORNO DE DESARROLLO DE APLICACIONES PARA LA NINTENDO DS EN LINUX
---------------------------------------------------------------------

Fecha de creación: 21/Septiembre/2009
Basado en las librerías 'devkitPro': http://devkitpro.org/
Descargado de: http://sourceforge.net/projects/devkitpro/files/

Para poder desarrollar aplicaciones para la Nintendo DS se tiene
que disponer de un entorno de desarrollo formado por muchos
componentes, para evitar tener que descargarlos todos se ofrece
una copia de los archivos necesarios, así como información
para construir un nuevo entorno con versiones más modernas cuando
estén disponibles.


LISTADO DE COMPONENTES Y SUS VERSIONES INCLUIDAS EN ESTE DIRECTORIO:
---------------------------------------------------------------------

default_arm7-0.5.6.tar.bz2
default_arm7-src-0.5.6.tar.bz2
devkitARM_r26-i686-linux.tar.bz2
dswifi-0.3.10.tar.bz2
dswifi-src-0.3.10.tar.bz2
libfat-nds-1.0.4.tar.bz2
libfat-src-1.0.4.tar.bz2
libfilesystem-0.9.2.tar.bz2
libfilesystem-src-0.9.2.tar.bz2
libnds-1.3.8.tar.bz2
libnds-src-1.3.8.tar.bz2
maxmod-nds-1.0.6.tar.bz2
maxmod-src-1.0.6.tar.bz2
nds-examples-20090730.tar.bz2


Como se puede ver se incluyen los binarios, el código fuente
y ejemplos de desarrollo.


EJECUCIÓN:
---------------------------------------------------------------------

Los archivos ejecutables tienen la extensión '.nds'

El mejor entorno de ejecución es una Nintendo DS, pero para desarrollar
lo más cómodo es utilizar un emulador. En Linux, el emulador que está
funcionando muy bien es 'desmume' http://desmume.org/


Los requisitos básicos para la ejecución de los programas en la NDS son:

- Video consola Nintendo DS, DS Lite, DSi o DSi XL
- Cartucho de desarrollo 'Homebrew' tipo R4, M3, Ez Flash, o similar.
- Memoria flash de tipo Micro SD donde grabar los programas y los datos
formateado con el sistema de archivos FAT32.

Con un adaptador Micro SD a USB o un lector de tarjetas de memoria se pueden
copiar los archivos '.nds' necesarios.

NOTA: Algunos cartucho no parchean automáticamente los archivos '.nds' para
que puedan tener acceso al sistema de archivos para guardar datos. En este
caso hay que parchearlos manualmente con el programa que el fabricante del
cartucho dispone para estos casos antes de copiarlo a la memoria flash.


---------------------------------------------------------------------


