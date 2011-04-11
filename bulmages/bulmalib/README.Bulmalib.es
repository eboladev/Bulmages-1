========================================
Software de Gestión Empresarial BulmaGés

(C) 2003 - 2008 Asociación IGLUES
http://www.iglues.org
========================================
#README.Bulmalib
20080501

Bulmalib
==========

Este directorio contiene todo el código fuente del programa de la biblioteca
bulmalib. Hay algunas funciones que son comunes a varios de los proyectos.
postgresiface, configuración, algunos widgets raros. Es la funcionalidad
principal de la suite.

A partir de la versión 0.11.x se intentará que todas las versiones x.y.z con 
numeración x.y igual sean compatibles en ABI de biblioteca y base de datos. 


Estructura
==========

Dentro del directorio src/ está el código fuente de la aplicación. En el
directorio plugins estan los plugins del programa.
  

Plugins
=======

Bulmalib contiene los siguientes plugins: 

- pluginbl_attachdocument
- pluginbl_embed
- pluginbl_importcsv
- pluginbl_report2ods
- pluginbl_subform2pdf
- pluginbl_template
- pluginbl_template2ods
- pluginbl_clipboard
- pluginbl_formlock
- pluginbl_report
- pluginbl_subform2ods
- pluginbl_subform2sxc
- pluginbl_template2mail
