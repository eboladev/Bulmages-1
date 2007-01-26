#!/bin/bash

nombrebd="bulmacont"
#nombrebd=$1

echo $nombrebd

dropdb $nombrebd
createdb $nombrebd

# Esquema de la base de datos.
psql $nombrebd < bulmacont_schema.sql
# Datos iniciales.
psql $nombrebd < t_ainteligente_data.sql
psql $nombrebd < t_amortizacion_data.sql
psql $nombrebd < t_apunte_data.sql
psql $nombrebd < t_ejercicios_data.sql
psql $nombrebd < t_asiento_data.sql
psql $nombrebd < t_balance_data.sql
psql $nombrebd < t_binteligente_data.sql
psql $nombrebd < t_canal_data.sql
psql $nombrebd < t_c_coste_data.sql
psql $nombrebd < t_compbalance_data.sql
psql $nombrebd < t_compmasap_data.sql
psql $nombrebd < t_configuracion_data.sql
psql $nombrebd < t_diario_data.sql
psql $nombrebd < t_fpago_data.sql
psql $nombrebd < t_grupo_data.sql
psql $nombrebd < t_linamortizacion_data.sql
psql $nombrebd < t_cuenta_data.sql
psql $nombrebd < t_cuenta_data1.sql
psql $nombrebd < t_borrador_data.sql
psql $nombrebd < t_registroiva_data.sql
psql $nombrebd < t_tipoiva_data.sql
 