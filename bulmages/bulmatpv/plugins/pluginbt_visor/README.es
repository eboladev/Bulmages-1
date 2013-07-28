#######
#Plugins
#######
#README.es
#20080501 

Nombre: Visor TPV

Biblioteca: libpluginbt_visor.so

Descripción: <H1>Visor TPV</H1>
<P>Maneja un visor externo para un TPV mostrando los articulos que se van introduciendo,
el total a pagar y el cambio a devolver en el cobro. Usa el parámetro CONF_TPV_VISOR_FILE para determinar
en qué dispositivo debe escribir.</P>
<P>Debe establecersa la velocidad de comunicacion con el visor cada vez que arranca el sistema.
Una posible forma es editar el archivo /etc/init.d/bootmisc.sh y agregar al final de este el comando
<B>stty -F /dev/ttyUSB0 9600</B></P>
<HR>

ParmBD: 

ArchivoSQLpatch: 

ArchivoSQLdeins: 

Dependencias:

Incompatibles:

Categorias: Utilidades avanzadas; Experimentales; Colmados; Restauración; Tallas y Colores; Supermercados;

Orden: 10

Comentarios: Testeado con un visor Samsung Bixolon BCD1000
