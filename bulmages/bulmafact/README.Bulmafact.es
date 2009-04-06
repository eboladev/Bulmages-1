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

- pluginbf_comercial (Plugin de Comerciales)
- pluginbf_bulmatpv (Plugin de TPV)
- pluginbf_abrecodigobarras (Plugin de apertura rápida a partir de códigos de barras)
- pluginbf_catalogo (Plugin de catálogos)
- pluginbf_contrato (Plugin de Informes de Clientes)
- pluginbf_multiimprimir (Plugin de Impresiones Múltiples)
- pluginbf_informecliente (Plugin de Informes de Clientes)
- pluginbf_inventario (Plugin de Inventarios)
- pluginbf_correo (Plugin de  envio de documentos por e-mail)
- pluginbf_promedios (Plugin promedios)
- pluginbf_q19 (Plugin q19)
- pluginbf_sincrobulmacont (Sincronización con Bulmacont)
- pluginbf_tarifa (Plugin de tarifas)
- pluginbF_ticket (Plugin de  impresion de tickets)
- pluginbf_trazabilidad (Plugin de Trazabilidad)
- pluginbf_tipotrabajo (Plugin de trabajadores)
- pluginbf_articulo (Plugin de Resumenes por Articulo)
- pluginbf_cuadrante (Plugin de cuadrante)
- pluginbf_informecliente2sxc (Plugin de Informes de Clientes)


Todos los Plugins
=================

Aqui esta la lista de todos los plugins disponibles para BulmaFact


- pluginalmacen
- pluginbf_abrecodigobarras
- pluginbf_abrereferencia
- pluginbf_actividad
- pluginbf_alias
- pluginbf_almacen
- pluginbf_alumno
- pluginbf_articulo
- pluginbf_asociacion
- pluginbf_asterisk
- pluginbf_banco
- pluginbf_bulmacont
- pluginbf_bulmatpv
- pluginbf_catalogo
- pluginbf_cliente
- pluginbf_clientealbaran
- pluginbf_clientealbaranesfactura
- pluginbf_clientecobro
- pluginbf_clientefactura
- pluginbf_clientepedido
- pluginbf_clientepresupuesto
- pluginbf_clienteproveedor
- pluginbf_comercial
- pluginbf_compraventa
- pluginbf_contrato
- pluginbf_corrector
- pluginbf_correo
- pluginbf_correothunderbird
- pluginbf_cuadrante
- pluginbf_datosbase
- pluginbf_debug
- pluginbf_efactura
- pluginbf_etiquetado
- pluginbf_etiquetas
- pluginbf_example
- pluginbf_formasdepago
- pluginbf_gestassociacio
- pluginbf_informecliente
- pluginbf_informecliente2sxc
- pluginbf_inventario
- pluginbf_iva
- pluginbf_ivaincluido
- pluginbf_multiimprimir
- pluginbf_preciocoste
- pluginbf_preciocoste
- pluginbf_profesor
- pluginbf_promedios
- pluginbf_proveedor
- pluginbf_proveedoralbaran
- pluginbf_proveedorfactura
- pluginbf_proveedorpago
- pluginbf_proveedorpedido
- pluginbf_ql9
- pluginbf_qwebcam
- pluginbf_recibo
- pluginbf_seriesfactura
- pluginbf_sincrobulmacont
- pluginbf_tallascolores
- pluginbf_tarifa
- pluginbf_ticket
- pluginbf_tipotrabajo
- pluginbf_trabajador
- pluginbf_trazabilidad
- pluginbf_tutor
- pluginbf_vehiculo
- pluginbf_z2z
- pluginresarticulos



add_subdirectory(pluginalmacen)
add_subdirectory(pluginasterisk)
add_subdirectory(plugindebugbf)
add_subdirectory(pluginpreciocoste)

#mateix info que plugintickets
add_subdirectory(pluginetiquetado)

