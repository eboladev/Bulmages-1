=======
Plugins
=======
#README
20080502

Nombre: Impresion de Informes Personalizados

Biblioteca: libpluginimpers.so

Descripción: <H1>Informes Personales</H1>
<div style="align:justify;">Permite la generacion de informes personalizados relacionados con
una ficha generica. Sin necesidad de programar. Para ello utiliza directrices y
marcas en el RML.</div>
<br>
<div align="justify">Una vez instalado al pulsar con el boton derecho en cualquier parte vacia de una ficha
aparecera la opcion informes personales que desplegara todos los informes disponibles para
dicha ficha</div>
<br>
<div style="align:justify;">Se pueden crear informes personalizados generando plantillas RML que tengan el nombre
impers_tabla.rml en el directorio de plantillas donde (tabla) es la tabla sobre la que actua el informe (Ver opciones avanzadas de ficha
si se desconoce el nombre de la tabla</div>
<HR>

ParmBD:

ArchivoSQLpatch:

ArchivoSQLdeins:

Dependencias:

Incompatibles:

Orden: 60

Comentarios: Su funcionamiento se basa en que haya un documento RML cuyo nombre
contenga el nombre de la tabla a la que representa la ficha. De este modo el
informe aparecerá como opcion al pulsar sobre la opcion informes personalizados
en el menu contextual de la ficha.

(c) 2007 Asociación IGLUES. Software Bajo Licencia G.P.L.

