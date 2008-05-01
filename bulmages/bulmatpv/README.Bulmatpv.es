========================================
Software de Gestión Empresarial BulmaGés

(C) 2003 - 2008 Asociación IGLUES
http://www.iglues.org
========================================
#README.Bulmatpv
20080501

BulmaTPV
========

Este directorio contiene todo el código fuente del programa BulmaTPV. Tanto el
programa principal como su plugins.

El programa permite vender y controlar el hardware específico de venta.
Impresoras de Tickets y Lectores de códigos de Barras. En este programa la
velocidad de las operaciones es lo fundamental.

BulmaTPV es adaptable a los distintos modelos de negocio gracias a sus sistema
de plugins que dominan toda la interfície gráfica del programa.

Estructura
==========

Dentro del directorio src/ está el código fuente de la aplicación. El el
directorio plugins estan los plugins del programa.
  

Módulos adicionales y personalizaciones
=======================================
Usted puede crear extensiones al programa creando dichas extensiones en forma de
plugin.

Nota: Cree plugins específicos para sus clientes y ofrezca un servicio de
mantenimiento compilando bulmatpv en sus nuevas versiones junto a sus plugins.

Plugins
=======

Actualmente se compilan estos plugins por defecto en la mayoria de plataformas:

- pluginticketbasico 
- plugintecladonumerico
- plugintotal
- pluginartgraficos
- pluginadmin
- plugincobrar
- pluginabrevs * no en OSX

