#!/bin/bash

#/***************************************************************************
# *   Copyright (C) 2010 by Fco. Javier M. C.                               *
# *   comercial@todo-redes.com                                              *
# *   http://www.todo-redes.com                                             *
# *                                                                         *
# *   This program is free software; you can redistribute it and/or modify  *
# *   it under the terms of the GNU General Public License as published by  *
# *   the Free Software Foundation; either version 2 of the License, or     *
# *   (at your option) any later version.                                   *
# *                                                                         *
# *   This program is distributed in the hope that it will be useful,       *
# *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
# *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
# *   GNU General Public License for more details.                          *
# *                                                                         *
# *   You should have received a copy of the GNU General Public License     *
# *   along with this program; if not, write to the                         *
# *   Free Software Foundation, Inc.,                                       *
# *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
# ***************************************************************************/

#
# Este 'script' establece las variables de entorno necesarias para poder
# desarrollar programas para la Nintendo DS utilizando las librerias 'devkitPro'.
# 
# USO:
#
# Entrar en el directorio donde se encuentra este 'script' y ejecutarlo.
#
# ejemplo:
#         1)    cd ~/bulmages/svn/trunk/devkitPro
#         2)    ./devkitPro_setenv.sh
#
#


# Ruta a '.bashrc':
BASHRC=$HOME/.bashrc
# Ruta al directorio 'devkitPro':
DEVKITPRO=`pwd`

echo "1) Parcheando .bashrc"

# Localiza .bashrc del usuario y lo crea si no existe.
if [ -f $BASHRC ]
then
    echo "2) .bashrc localizado."
else
    echo "2) .bashrc no existe, se crea."
    touch $BASHRC
fi

# Verificamos si ya esta parcheado el archivo .bashrc
GREP=`grep "DEVKITPRO" $BASHRC`

if [ ${#GREP} -eq 0 ]
then
    # Añade datos al final del archivo.
    echo "3) Añadiendo datos a .bashrc"

        echo "declare -x DEVKITPRO=$DEVKITPRO/" >> $BASHRC
        echo "declare -x DEVKITARM=$DEVKITPRO/devkitARM/" >> $BASHRC

else
    echo "3) No ha sido necesario modificar .bashrc"
fi

