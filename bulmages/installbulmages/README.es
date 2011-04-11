========================================
Software de Gestión Empresarial BulmaGés

(C) 2003 - 2008 Asociacion IGLUES
http://www.iglues.org
========================================
#README.installbulmages.es

Mayo 2008

En ente directorio es donde antiguamente se ubicaban los ficheros que se
generaban con qmake. A partir de la vesión 0.11.x el sistema de construcción és
cmake, y por tanto la utilidad de este directorio como destino de los binarios,
ha pasado a segundo término.

También decir, que hasta la versión 0.11, generar empresas, o usuarios y
configurar bulmages era una tarea "un poco complicada". Para ayudar a resolver
todas estas dificultades se habian creado uns "scripts" que automatizaban el
uso de comandos de postgres.

Ahora bien, los desarrolladores, viendo las dificultades que habian, han
desarrolado un programa gráfico "bulmasetup", que ejecutado como
superususario, ayuda a configurar y crear empresas i/o usuarios. Este cambio ha
ocasionado que todo este montaje de scripts, más que desfasado, sirva de emp
lo para configuraciones muy adaptadas. Los scripts (o la metodologia) sigue
siendo válida y por eso estan en el directorio de examples.

