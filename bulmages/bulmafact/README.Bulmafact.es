========================================
Software de Gestión Empresarial BulmaGés

(C) 2003 - 2008 Asociacion IGLUES
http://www.iglues.org
========================================
#README.Bulmafact
20080501

BulmaFact
=========

Este directorio contiene todo el código fuente del programa 
BulmaFact. Tanto el programa principal como su plugins.

Estructura
==========

Dentro del directorio src/ está el código fuente de la aplicación. El el
directorio plugins estan los plugins del programa.
  

Módulos adicionales y personalizaciones
=======================================
Usted puede crear extensiones al programa creando dichas extensiones en forma de
plugin.

Nota: Cree plugins específicos para sus clientes y ofrezca un servicio de
mantenimiento compilando bulmafact en sus nuevas versiones junto a sus plugins.

Plugins
=======

Actualmente se compilan estos plugins por defecto en la mayoria de plataformas:

- comercialbf (Plugin de Comerciales)
- plugintpv (Plugin de TPV)
- plugin (Plugin ejemplo)
- barcodeopen (Plugin de apertura rápida a partir de códigos de barras)
- plugincatalogo (Plugin de catálogos)
- plugincontratos (Plugin de Informes de Clientes)
- impresionesmultiples (Plugin de Impresiones Múltiples)
- plugininformeclientes (Plugin de Informes de Clientes)
- plugininventarios (Plugin de Inventarios)
- pluginmail (Plugin de  envio de documentos por e-mail)
- pluginpromedios (Plugin promedios)
- pluginq19 (Plugin q19)
- pluginsyncbulmacont (Sincronización con Bulmacont)
- plugintarifas (Plugin de tarifas)
- pluginticket (Plugin de  impresion de tickets)
- plugintrazabilidad (Plugin de Trazabilidad)
- plugintipostrabajo (Plugin de trabajadores)
- pluginresarticulos (Plugin de Resumenes por Articulo)
- plugincuadrante (Plugin de cuadrante)
- plugininformeclientessxc (Plugin de Informes de Clientes)


#sense info
add_subdirectory(pluginalmacen)
add_subdirectory(pluginasterisk)
add_subdirectory(plugindebugbf)
add_subdirectory(pluginpreciocoste)

#mateix info que plugintickets
add_subdirectory(pluginetiquetado)

