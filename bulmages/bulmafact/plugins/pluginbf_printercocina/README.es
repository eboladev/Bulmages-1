#######
#Plugins
#######
#README.es
#20080501

Nombre: Impresion en Cocina

Biblioteca: libpluginbf_printercocina.so

Descripción: <H1>Impresion en Cocina</H1>
<P>Da el soporte a la impresion de pedidos generados en bulmatpv en la cocina</>
<P> Este componente modifica la base de datos para agregarle los campos necesarios para la impresion en cocina.</P>
<P> Tambien agrega el selector de impresora de cocina a la ficha de articulos.
Las impresoras deben estar guardadas en la tabla printercocina que (de momento) no tiene gestion en bulmafact y 
debe ser mantenida a través del SQL. Supuestamente es el técnico quien instala las impresoras y modifica la
base de datos para que estas tenga efecto.
</P>
<P>Importante que las impresoras deben ser ticketeras compatibles <B>ESC/POS</B> y deben estar compartidas con CUPS. En la tabla
printer cocina hay que poner el nombre de la cola cups de la impresora</P>
<B>Author:</B>Tomeu Borrás (tborras@conetxia.com)<BR>
<HR>

ParmBD: PluginBf_PrinterCocina

ArchivoSQLpatch: pluginbf_printercocina.sql

ArchivoSQLdeins: pluginbf_printercocina-rm.sql

Dependencias:

Incompatibles:

Categorias: Restauración;

Orden: 900

Comentarios: No se instala propiamente como un plugin ya que no lleva librerías asociadas.


Prerequisitos:
