========================================
Software de Gestión Empresarial BulmaGés

(C) 2003 - 2008 Asociacion IGLUES
http://www.iglues.org
========================================
#README.scripts.es

Mayo 2008

Ejemplos de scripts para una instalación manual de bulmages.


NOTAS PARA LA INSTALACION:
--------------------------
El script de instalación ha sido probado sobre Debian, Mandrake, Ubuntu y SUSE.
Seguramente sea fácil extenderlo a otras distribuciones.

La instalación del software se divide en dos partes, cliente y servidor

Servidor
--------
La parte servidor instala y configura las bases de datos iniciales.


Para instalar el programa entre en el directorio de instalación y escriba
"como root" 

./installbulmages-server


Nota: Vea el directorio dbmodels para mas informacion sobre como instalar
 manualmente o como actualizar un sistema.


Cliente
-------
En cada uno de los clientes debe instalar el ejecutable y configurarlo para
que acceda al servidor.

Para instalar el cliente escriba (como root)

# ./installbulmages-client


Consideraciones Especiales
--------------------------

Hay un modo de instalación que no usa dialog, para hacer una instalación en
modo texto escriba
# ./installbulmages --texto

