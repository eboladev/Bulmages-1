#!/bin/bash

# Restauramos el esquema de la base de datos.
psql metabd < metabd_schema.pgdump
#Restauramos los datos
psql metabd < metabd_data.pgdump
