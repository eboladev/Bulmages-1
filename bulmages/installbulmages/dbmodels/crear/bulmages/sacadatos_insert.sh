#!/bin/bash

#pg_dump --no-owner --data-only --column-inserts bulmages > bulmages_data.pgdump
pg_dump --no-owner --data-only bulmages > bulmages_data.pgdump
pg_dump --no-owner --schema-only bulmages > bulmages_schema.pgdump

pg_dump --no-owner --inserts --data-only --column-inserts -t configuracion   bulmages > t_configuracion_data.pgdump
pg_dump --no-owner --inserts --data-only --column-inserts -t tiposiva        bulmages > t_tiposiva_data.pgdump
pg_dump --no-owner --inserts --data-only --column-inserts -t grupo           bulmages > t_grupo_data.pgdump
pg_dump --no-owner --inserts --data-only --column-inserts -t cuenta          bulmages > t_cuenta_data.pgdump
pg_dump --no-owner --inserts --data-only --column-inserts -t canal           bulmages > t_canal_data.pgdump
pg_dump --no-owner --inserts --data-only --column-inserts -t c_coste         bulmages > t_c_coste_data.pgdump
pg_dump --no-owner --inserts --data-only --column-inserts -t diario          bulmages > t_diario_data.pgdump
pg_dump --no-owner --inserts --data-only --column-inserts -t asiento         bulmages > t_asiento_data.pgdump
pg_dump --no-owner --inserts --data-only --column-inserts -t apunte          bulmages > t_apunte_data.pgdump
pg_dump --no-owner --inserts --data-only --column-inserts -t borrador        bulmages > t_borrador_data.pgdump
pg_dump --no-owner --inserts --data-only --column-inserts -t registroiva     bulmages > t_registroiva_data.pgdump
pg_dump --no-owner --inserts --data-only --column-inserts -t ainteligente    bulmages > t_ainteligente_data.pgdump
pg_dump --no-owner --inserts --data-only --column-inserts -t binteligente    bulmages > t_binteligente_data.pgdump
pg_dump --no-owner --inserts --data-only --column-inserts -t ivainteligente  bulmages > t_ivainteligente_data.pgdump
pg_dump --no-owner --inserts --data-only --column-inserts -t compmasap       bulmages > t_compmasap_data.pgdump
pg_dump --no-owner --inserts --data-only --column-inserts -t compbalance     bulmages > t_compbalance_data.pgdump
pg_dump --no-owner --inserts --data-only --column-inserts -t balance         bulmages > t_balance_data.pgdump
pg_dump --no-owner --inserts --data-only --column-inserts -t amortizacion    bulmages > t_amortizacion_data.pgdump
pg_dump --no-owner --inserts --data-only --column-inserts -t linamortizacion bulmages > t_linamortizacion_data.pgdump
pg_dump --no-owner --inserts --data-only --column-inserts -t ejercicios      bulmages > t_ejercicios_data.pgdump
