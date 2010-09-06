#!/bin/bash

# VERSION BETA. USAR CON CUIDADO


# 1) Hacer copia de la base de datos de contabilidad a la que apunta.
# 2) Desinstalar plugin de sincronizacion con BulmaCont.
# 3) Aplicar parche de BulmaFact
# 4) Instalar plugin de sincronizacion con BulmaCont.
# 5) Restautar configuracion de dase de datos de contabilidad a la que apunta.


# PARAMETROS:
#
# $1 = Nombre de la base de datos a parchear.
# $2 = Nombre del archivo con el parche SQL de la base de datos a aplicar.


echo "1)"
DATABASEBULMACONT=`psql $1 -t -c "select valor from configuracion where nombre='DataBaseContabilidad';" | tr -d ' '`

echo "2)"
psql $1 < pluginbf_sincrobulmacont-rm.sql

echo "3)"
psql $1 < $2

echo "4)"
psql $1 < pluginbf_sincrobulmacont.sql

echo "5)"
psql $1 -c "UPDATE configuracion SET valor = '$DATABASEBULMACONT' WHERE nombre = 'DataBaseContabilidad';"
