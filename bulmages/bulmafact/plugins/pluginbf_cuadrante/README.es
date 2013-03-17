#######
#Plugins
#######
#README.es
#20080501

Nombre: Cuadrantes de personal

Biblioteca: libpluginbf_cuadrante.so

Descripción: <H1>Cuadrante de personal</H1>
<P>Gestiona la creación de cuadrantes de trabajo pudiendo asignar las horas que
corresponda a cada trabajador y la plublicación del cuadrante en formato PDF.</P>
<B>Author:</B>Tomeu Borrás (tborras@conetxia.com)<BR>
<HR>

ParmBD: PluginBf_Cuadrante

ArchivoSQLpatch: pluginbf_cuadrante.sql

ArchivoSQLdeins: pluginbf_cuadrante-rm.sql

Dependencias: libpluginbf_asterisk.so;libpluginbf_trabajador.so;libpluginbf_almacenamplia.so;libpluginbf_tipotrabajo.so

Incompatibles:

Reports: cuadrante.rml

Categorias: Utilidades avanzadas;Experimentales;Servicios Generales;

Orden: 780

Comentarios: Muy útil en sistemas de control de personal y en combinacion con el pluginasterisk.
