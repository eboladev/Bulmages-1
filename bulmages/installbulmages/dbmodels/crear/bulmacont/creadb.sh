#!/bin/bash

dropdb bulmacont
createdb bulmacont

# Esquema de la base de datos.
psql bulmacont < bulmacont_schema.sql
# Datos iniciales.
psql bulmacont < t_ainteligente_data.sql
psql bulmacont < t_amortizacion_data.sql
psql bulmacont < t_apunte_data.sql
psql bulmacont < t_ejercicios_data.sql
psql bulmacont < t_asiento_data.sql
psql bulmacont < t_balance_data.sql
psql bulmacont < t_binteligente_data.sql
psql bulmacont < t_canal_data.sql
psql bulmacont < t_c_coste_data.sql
psql bulmacont < t_compbalance_data.sql
psql bulmacont < t_compmasap_data.sql
psql bulmacont < t_configuracion_data.sql
psql bulmacont < t_diario_data.sql
psql bulmacont < t_fpago_data.sql
psql bulmacont < t_grupo_data.sql
psql bulmacont < t_linamortizacion_data.sql
psql bulmacont < t_cuenta_data.sql
psql bulmacont < t_cuenta_data1.sql
psql bulmacont < t_borrador_data.sql
psql bulmacont < t_registroiva_data.sql
psql bulmacont < t_tipoiva_data.sql
 