#######
#Plugins
#######
#README.es
#20080501 

Nombre: Visor TPV

Biblioteca: libpluginbt_bascula.so

Descripción: <H1>Control de la Bascula</H1>
<P>Maneja un bascula externo para un TPV mostrando los articulos que se van introduciendo,
el total a pagar y el cambio a devolver en el cobro. Usa el parámetro CONF_TPV_BASCULA_FILE para determinar
en qué dispositivo debe escribir.</P>
<P>Debe establecersa la velocidad de comunicacion con el bascula cada vez que arranca el sistema.
Una posible forma es editar el archivo /etc/init.d/bootmisc.sh y agregar al final de este el comando
<B>stty -F /dev/ttyUSB0 9600</B></P>
<HR>

ParmBD: 

ArchivoSQLpatch: 

ArchivoSQLdeins: 

Dependencias:

Incompatibles:

Categorias: Utilidades avanzadas; Experimentales; Colmados;

Orden: 10

Comentarios: Testeado con un bascula Baxtran PTI
