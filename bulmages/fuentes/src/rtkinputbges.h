/*<<<<<COPYRIGHT*/
/* GestiONG Gestión para ONGs y Asociaciones sin ánimo de lucro
 * Copyright (C) 2003 Francisco Santiago Capel Torres
 * 
 * rtkinputsql.h RTK standard sql input object
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
/*>>>>>COPYRIGHT*/
#ifndef RTKINPUTBGES_H
#define RTKINPUTBGES_H

#include <rtk.h>

// -------------- Inicializaciones de bulmages --------------
#include "empresa.h"
#include "postgresiface2.h"



namespace RTK
{

class InputBGes : public Input
{
public:
   enum tipoinput { diario, extracto, balance };
private:
   empresa *empresaactual;
   postgresiface2 *conexionbase;
   tipoinput ti;
   cursor2 *consulta;
   int inicializado;
        /*<<<<<INPUTSQL_CONSTRUCTOR*/
public:
        InputBGes(tipoinput t, empresa *emp, cursor2 *cons,
                 String from = String(), String where = String(), String orderby = String(),
                 String name = String(), String driver = String())
                        :Input(name, driver),
                        mFrom(from), mWhere(where), mOrderBy(orderby)  {
           empresaactual=emp;
           conexionbase=empresaactual->bdempresa();
           ti = t;
           consulta = cons;
           inicializado=1;
        };

        ~InputBGes();

        virtual bool init();
        virtual bool next();
        virtual Variant getValue(uint i) const;
        virtual Variant getValue(char *) const;
            
        /*<<<<<INPUTSQL_GETSET*/
public:
        String getFrom() const
        {
                return mFrom;
        };
        void setFrom(String from)
        {
                mFrom = from;
        };
        String getWhere() const
        {
                return mWhere;
        };
        void setWhere(String where)
        {
                mWhere = where;
        };
        String getOrderBy() const
        {
                return mOrderBy;
        };
        void setOrderBy(String orderby)
        {
                mOrderBy = orderby;
        };
protected:
        String mFrom;
        String mWhere;
        String mOrderBy;
        /*>>>>>INPUTSQL_GETSET*/
};

}; // namespace

#endif


