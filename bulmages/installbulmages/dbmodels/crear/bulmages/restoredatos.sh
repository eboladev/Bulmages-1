#!/bin/bash

# Restauramos el esquema de la base de datos.
psql bulmages < bulmages_schema.pgdump

# Restauramos tabla a tabla los datos.
psql bulmages < t_configuracion_data.pgdump
psql bulmages < t_tiposiva_data.pgdump
psql bulmages < t_grupo_data.pgdump
psql bulmages < t_cuenta_data.pgdump
psql bulmages < t_canal_data.pgdump
psql bulmages < t_c_coste_data.pgdump
psql bulmages < t_diario_data.pgdump
psql bulmages < t_asiento_data.pgdump
psql bulmages < t_apunte_data.pgdump
psql bulmages < t_borrador_data.pgdump
psql bulmages < t_registroiva_data.pgdump
psql bulmages < t_ainteligente_data.pgdump
psql bulmages < t_binteligente_data.pgdump
psql bulmages < t_ivainteligente_data.pgdump
psql bulmages < t_compmasap_data.pgdump
psql bulmages < t_compbalance_data.pgdump
psql bulmages < t_balance_data.pgdump
psql bulmages < t_amortizacion_data.pgdump
psql bulmages < t_linamortizacion_data.pgdump
psql bulmages < t_ejercicios_data.pgdump
