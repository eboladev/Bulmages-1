Instalacion del software de gestion BulmaGes
--------------------------------------------
(C) 2007, Asociacion IGLUES
http://www.iglues.org
--------------------------------------------

BulmaGés 0.10.1 (octubre 2007)


Introducción
============

BulmaGés es una aplicación modular que pretende cubrir las necesidades de
gestión empresarial de cualquier pequeña y mediana empresa dentro del entorno
del software libre. Se identifican claramente cuatro áreas que en las que el
software debe actuar: 
a) Contabilidad - BulmaCont
b) Facturación - BulmaFact
c) Terminal Punto de Venta - BTPV


En la actualidad, la aplicación de contabilidad (BulmaCont), facturacion (BulmaFact) y el terminal Punto de Venta ya son operativos.

Características
===============

Aunque todas estas aplicaciones están pensadas para funcionar como una sola, su
modularidad permite que funcione cada una de forma independiente, facilitando
así la migración progresiva desde otras aplicaciones o la adopción sólo de
aquellas herramientas que sean necesarias para la empresa. BulmaGés es software
libre y está liberado bajo licencia GPL, se puede obtener, tanto el software
como el código fuente, de forma gratuita a través de Internet o de las
diferentes distribuciones (CD) que facilita la asociación IGLUES. 

Módulos
=======

Estos son los directorios principales del svn y sus respectivas explicaciones.

bulmafact 
---------- 
Este directorio contiene el codigo fuente de la facturacion. Es un programa en
C++ y Qt. En las code partys trabajamos sobre ello, aunque Josep Burcion tiene
un repositorio especial con el programa. Luego tendremos que adaptarlo. Todo
depende del bulmalib. 

Actualmente, (versión 0.10.1) bulmafact realiza las tareas de facturación y de gestión habituales de una empresa
y contiene los siguientes plugins:

1) comercialbf (Plugin de Comerciales)

Permite el control del trabajo de los comerciales y la combinación de éstos con 
el aplicativo para PDA ConPython

2) efacturabf (Plugin De Factura Electrónica)

Permite la exportacion e importacion de facturas en formato UBL 1.0 desde BulmaFact.

3) plugin (Plugin ejemplo)

Ejemplo de código para desarrollar un plugin a medida.

4) barcodeopen (Plugin de apertura Rapida a partir de códigos de barras)

Permite abrir cualquier documento identificado con el codigo de barras. Basta con seleccionar la apertura rapida, pasar el documento por el lector y se abrira automaticamente el documento correspondiente.

5) plugincatalogo (Plugin de catálogos)

Permite la impresion de catalogos de productos

6) plugincontratos (Plugin de Informes de Clientes)

Permite el control de contratos con clientes y generar una facturacion asociada a dichos contratos.

7) impresionesmultiples (Plugin de Impresiones Multiples)

Agrega un checkbox en cada uno de los miembros del listado de presupuestos, pedidos, albaranes y facturas.
Agrega tres botones de menu en cada uno de dichos listados con las siguientes funcionalidades:
	1.- Invertir Seleccion: Invierte el estado de los checkbox agregados.
	2.- Imprimir multiples elementos: Imprime los elementos seleccionados de forma directa.
	3.- Envio multiple de e-mail: Prepara el envio de e-mail con los documentos seleccionados como adjuntos.

8) plugininformeclientes (Plugin de Informes de Clientes)

Permite la impresion del informe de compras y ventas  por clientes.

9) plugininventarios (Plugin de Inventarios)

Permite el control de inventarios.

10) pluginmail (Plugin de  envio de documentos por e-mail)

Permite el envio de documentos generados con BulmaGés via e-mai mediante un boton en el programa. 

11) pluginpromedios (Plugin promedios)

Muestra el promédio de compra y de venta por artículo.

12) pluginq19 (Plugin q19)

Genera un fichero con formato q19 para los cobros en una entidad bancária.

13) pluginsyncbulmacont (Sincronizacion con Bulmacont)

Todas las operaciones que se realicen con faturacion serán traspasadas a contabilidad.

14) plugintarifas (Plugin de tarifas)

Permite especificar diferentes tarifas donde el mismo artículo puede tener precios distintos.

15) pluginticket (Plugin de  impresion de tickets)

Permite la impresion de tickets.

16) plugintrazabilidad (Plugin de Trazabilidad)

Permite el control de trazabilidad por lotes. Agrega una tabla de movimientos de material y un campo
"lote" que permite identificar productos desde su compra hasta su venta.

17) plugintipostrabajo (Plugin de trabajadores)

Permite especificar diferentes tipos de trabajo de los diferentes trabajadores.

18) pluginvehiculos (Plugin de vehiculos)

Permite la gestión de una flota de vehículos.



bulmacont
---------
Contiene la parte de contabilidad y contiene los siguientes plugins:

1) pluginregistroiva (Plugin registro de IVA)

Permite registras las facturas para hacer el listado modelo 377.

2) pluginadocumental (Plugin de documentos)

Permite introducir documentos. Actualmente (0.10.1) no está operativo.

3) plugincorrector (Plugin corrector)

permite testear los errors contables de la base de datos.

4) plugin (Plugin ejemplo)

Código ejemplo para desarrollar un plugin.
         
5) plugincanualesods (Plugin de balances)

Permite generar balances en una hoja de cálculo.


bulmalib 
-------- 
Hay algunas librerias que son comunes a varios de los proyectos. postgresiface,
configuracion, algunos widgets raros. Son las librerias principales de la suite. A partir de la versión 0.10.x se intentará que todas las versiones x.y.z con numeración x.y igual sean compatibles en ABI de libraria y base de datos. Bulmalib contiene los siguientes plugins: 
- pluginclipboardbf. Permite el intercambio con hojas de cálculo kspread i oocalc utilizando el portapapeles.
- pluginsubformods. Permite el intercambio utilizando ficheros OpenDocument. Necesita una libreria de python.
- pluginsubformsxc. Permite el intercambio utilizando ficheros OpenOffice. Necesita una libreria de perl.


bulmatpv 
--------- 
Contiene el terminal punto de venta. El programa permite vender y controlar el hardware especifico de venta. Impresoras de Tickets y Lectores de codigos de Barras. En este programa la velocidad de las operaciones es lo fundamental.

BulmaTPV es adaptabla a los distintos modelos de negocio gracias a sus sistema de plugins que dominan toda la interficie gráfica del programa.

bulmages 
------- 
Contiene el lanzador del programa.

installbulmages 
--------------- 
Con el directorio fuentes obtenemos el binario de bulmages. En este directorio
tenemos todo lo necesario para realizar una instalacion.
Lo que se traduce en que: 
1.- Si modificas la base de datos tienes que plasmar los cambios aqui para que
se hagan en las nuevas versiones. Por coherencia de datos tenemos los scripts de
creacion de una base de datos nueva y los scripts de cambios entre versiones y
hay que mantenerlos.

- Los scripts de instalacion estan en dbmodels/crear
- Los scripts de actualizacion estan en dbmodels/actualizar
 
2.- Tambien esta el archivos bulmages.conf que se instalara en las próximas
versiones. Por tanto todo lo que se agrega en la configuracion debe ir tambien
reflejado en su correspondiente archivo aquí. Tambien hay unos archivos de
revisión entre versiones del archivo. Para que las actualizaciones no cambien
toda la configuracion del sistema. 

3.- Tambien estan los scripts de instalacion.

- installbulmages-server que monta las bases de datos y prepara un servidor.
- installbulmages-client que monta un clienta.

scripts
-------
Contiene scripts utiles para el desarrollo.


images
------
Contiene imagenes (recursos) que son usadas junto con el archivo
bulmages.qrc como iconos de los programas.


COMPILACION
-----------

Para compilar basta con hacer en el directorio que se desee (para compilar todo
lo que haya dentro del directorio)

$qmake-qt4
$make

Los ejecutables estaran dentro de installbulmages

INSTALACION
-----------

Nota: Debe tener privilegios de root para instalar BulmaGes.

Ejecute los scripts 

#installbulmages-server
#installbulmages-client


Notas para la instalacion
-------------------------
El programa depende de ciertos archivos en /etc para ejecutarse correctamente.
Sin embargo cuando se esta desarrollando es engorroso reinstalar para
comprobar el correcto funcionamiento del programa.
En dicho caso es util cambiar en la instalacion los siguientes archivos
por soft links a los ejecutables resultantes en el directorio installbulmages
del
directorio de desarrollo.

Estos son los archivos que se deben enlazar:
/usr/lib/libbulmages.0.10.1.so
/usr/bin/bulmages
/usr/bin/bulmafact
/usr/bin/bulmacont

De esta forma siempre se puede tener en ejecucion una version en desarrollo


