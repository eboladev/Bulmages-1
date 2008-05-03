#######
#Plugins
#######
#README
#20080501

Nombre: Plugin Bloqueo de Fichas

Biblioteca: libpluginbloqueos.so

Descripción: Detecta la apertura duplicada de fichas y avisa al usuario de que la
ficha que esta intentando abrir ya esta siendo utilizada. El plugin permite ver la ficha aunque desabilita los botones de guardado, eliminación y de aceptar.

ParmBD: DBRev-Bloqueo

ArchivoSQLpatch: rev-pluginbloqueos.sql

ArchivoSQLdeins: 

Comentarios: Este plugin es importante en entornos multi-usuario ya que sin el se puede cometer facilmente un cambio que sea borrado por otro usuario.
