/*******************************************************************************
 *                                                                             *
 *   Copyright (C) 2009-2011 by Fco. Javier M. C.                              *
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


COMPILACIÓN:
---------------------------------------------------------------------

Para poder compilar el proyecto se debe tener instalado y bien configurado el
conjunto de librerías devkitPro.

Existen dos tipos de compilación posibles: Demo y final.

DEMO:
-----
Se usa para tener un ejecutable que no se conecta vía WIFI a ningún servidor
y que incorpora datos de ejemplo que se pueden usar como demostración del
funcionamiento del programa en su versión final. Para compilar el proyecto en
este modo hay que ejecutar:


    make -f Makefile.demo clean  /// Recomendable para limpiar archivos antiguos.

    make -f Makefile.demo


FINAL:
------
Esta compilación produce el ejecutable destinado a su uso en producción. Es decir,
con todas las funcionalidades activadas y que utiliza la configuración de IP del
servidor y la conexión WIFI al servidor para recibir y enviar los datos. Para
compilar en este modo hay que ejecutar:


    make -f Makefile.final clean  /// Recomendable para limpiar archivos antiguos.

    make -f Makefile.final





PARCHEAR ARCHIVO PARA GUARDAR DATOS EN LA MEMORIA:
---------------------------------------------------------------------

Algunos cartuchos modernos parchean automáticamente el archivo .nds para
que se pueda escribir directamente en el sistema de archivos FAT de la
tarjeta de memoria.

En caso de no disponer de la característica de parcheo automático hay que
proceder utilizando un programa correspondiente y el archivo DLDI del
cartucho.

Para GNU/Linux existe la herramienta gráfica 'DLDI Linux GUI'.
Web del proyecto: http://www.return1.net/site/projects/Linux-DLDI-Gui

En consola 'dlditool':
http://chishm.drunkencoders.com/DLDI/
