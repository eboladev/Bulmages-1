/***************************************************************************
 *   Copyright (C) 2004 by Marcos Mezo                                     *
 *   mmezo@selexco.net                                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef CLIENTEDIT_H
#define CLIENTEDIT_H

#include "clienteditbase.h"

/**
*    @author M. Mezo
*/
class company;

class ClientEdit : public ClientEditBase {
Q_OBJECT
private:
   company *companyact;
   QString clientId;
   bool modified;
   
   void setModified(bool formContentsChanged);
   void emptyForm();
   
public:
    ClientEdit(company *emp, QWidget *parent = 0, const char *name = 0);
    ~ClientEdit();
    
public:
   void loadClient(QString client);
   void saveClient();
   void deleteClient();
   
public slots:
   virtual void saveButton_clicked();
   virtual void deleteButton_clicked();
   virtual void cancelButton_clicked();   
   virtual void formModified();
   virtual void close();
};

#endif
