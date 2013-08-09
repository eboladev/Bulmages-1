#######
#Plugins
#######
#README.es
#20080801

Nombre: Alarmas

Biblioteca: libpluginbl_alarm.so

Descripción: <H1><IMG SRC="@CMAKE_INSTALL_PREFIX@/share/bulmages/images/chrono.png" WIDTH="48"/><BR>Alarmas</H1>
<P>Permite crear avisos programados sobre las fichas del programa, a modo de recordatorios y con fecha de aviso. Sobre una misma ficha se pueden gestionar varios avisos.</P>
<P> El plugin proporciona un gestor de avisos que permite ver todos los avisos pendientes, posponeros, borrarlos y acceder a las fichas en las cuales estan generados de forma muy sencilla. Además los avisos expirados aparecen destacados en color rojo. </P>
<P> Se pueden poner avisos sobre cualquier ficha, tanto si esta realizada con autoforms como si esta generada 
mediante un plugin</P>
<h2>Instalación</h2>
<P>Este módulo no requiere de intervención del usuario en su instalación si se realiza con bulmasetup</P>
<P>En la instalación manual debe tenerse en cuenta que hay que instalar un componente en la base de datos</P>
<P>Una vez instalado el plugin en todas las fichas del programa aparecerá la opción <IMG SRC="@CMAKE_INSTALL_PREFIX@/share/bulmages/images/chrono.png" WIDTH="16"/> en el menú y en el menú contextual de la ficha que nos permitirá agregar avisos, mostrar los avisos programados y borrarlos.</P>
<P>Cuando expira un aviso aparece remarcado en rojo en el gestor de alarmas, a través del cual se permite acceder de forma rápida a la ficha que lo ha generado </P>
<P>El plugin no tiene dependencias, ni utiliza componentes externos.</P>
<BR>
<IMG SRC="@CMAKE_INSTALL_PREFIX@/share/bulmages/images/chrono.png" WIDTH="48"/>
<HR>

ParmBD: PluginBl_Alarm

ArchivoSQLpatch: pluginbl_alarm.sql

ArchivoSQLdeins: pluginbl_alarm-rm.sql

Dependencias:

Incompatibles:

Categorias: Asociaciones de padres de alumnos;Utilidades genericas;Experimentales; Colmados; Ferreterías; Droguerías;Facturacion generica;Talleres;Distribución; Supermercados;Distribución Alimentaria;Servicios Generales;Alquileres;Restauración; Parques Infantiles; Tallas y Colores;

Orden: 90

Comentarios:

