#!/bin/bash

pg_dump --no-owner --data-only metabd > metabd_data.pgdump
pg_dump --no-owner --schema-only metabd > metabd_schema.pgdump

