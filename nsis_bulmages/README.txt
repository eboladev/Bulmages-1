CREACION DEL INSTALADOR DE BULMAGES PARA WINDOWS
================================================

1.- Instalar todos los requisitos:
      - Postgres
      - Qt
      - Cmake
      - GetText


      ....

2.- Crear el directorio build dentro de la carpeta bulmages.

3.- Desde ese directorio ejecutar cmake-gui

Con el fin de hacer la aplicación semi-portable usaremos las siguientes rutas relativas. 


BULMAGES_CONFIG_DIR               ../etc
CMAKE_INSTALL_PREFIX              ..


4.- Compilaremos e instalaremos con:

make install


5.- Una vez que la compilacion a terminado bien 
ejecutaremos con NSIS el script

bulmages_nsis_script.nsi


Tras estos pasos tendremos listo el instalador para Windows de bulmages.
