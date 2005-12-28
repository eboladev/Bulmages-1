#!/bin/bash


# Este script saca una tabla determinada y la guarda en el archivo determinado

pg_dump --no-owner --inserts --data-only --column-inserts -t $1      bulmages > $2
