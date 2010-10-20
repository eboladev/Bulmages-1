#######
#Plugins
#######
#README.es
#20080501

Nombre: Informes personalizados en ODS

Biblioteca: libpluginbl_report2ods.so

Descripción: <H1>Informes personalizados en hoja de calculo</H1>
<P>Presenta un nuevo menu Informes que permite lanzar los informes personalizados
de que se disponga. Dichos informes son scripts Python con ooolib.</P>
<P>Los informes deben estar en formato PYS y deben llamarse inf_XXXX.pys para que aparezcan en el menu
<B>Informes personales SXC</B></P>
<P>Existen varios ejemplos del formato pys en la seccion de informes de la instalación del programa.</P>
<HR>

ParmBD: 

ArchivoSQLpatch: 

ArchivoSQLdeins: 

Dependencias:

Incompatibles:

Reports: inf_clientes.pys; inf_detalleventascliente.pys; inf_resumcompras.pys; inf_detalleventasarticulo.pys; inf_familiacliente.pys

Categorias: Utilidades genericas;

Orden: 950

Comentarios: Util para realizar informes de forma rápida sin tener que modificar el codigo fuente ni hacer un plugin específico para cada uno de ellos.
