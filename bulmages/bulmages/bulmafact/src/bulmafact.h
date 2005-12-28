#ifndef BULMAFACT_H
#define BULMAFACT_H

#include <q3mainwindow.h>
#include <qworkspace.h>
#include <q3vbox.h>
#include "bulmafactbase.h"
#include "company.h"
#include "listventanas.h"

class bulmafact: public bulmafactbase
{
  Q_OBJECT
private:
  company *m_company;
  Q3VBox *view_back;
  QWorkspace *pWorkspace;
  listventanas *m_list;

public:
  bulmafact(QString bd);
  ~bulmafact();
  QWorkspace *workspace()
  {
    return pWorkspace;
  };
public slots:
  virtual void listproviders();
  virtual void s_newProveedor();
  virtual void caja();
  virtual void clientes();
  virtual void emitirfactura();
  virtual void recibirfactura();
  virtual void listClients();
  virtual void newClient();
  virtual void listarticles();
  virtual void listdelivnotes();
  virtual void newClientDelivNote();
  virtual void listBudgets();
  virtual void listClientDelivNotes();
  virtual void s_FPago();
  virtual void s_Familias();
  virtual void s_trabajadores();
  virtual void s_ventanaCompleta();
  virtual void s_seriesFactura();
  virtual void s_listPedidosCli();
  virtual void s_listFacturasCli() {m_company->s_listFacturasCli();};
  virtual void s_listPedidosPro() {m_company->s_listPedidosPro();};
  virtual void s_listFacturasPro() {m_company->s_listFacturasPro();};
  virtual void s_newFacturaPro() {m_company->s_newFacturaPro();};
  virtual void s_newAlbaranPro() {m_company->s_newAlbaranPro();};
  virtual void s_newPedidoPro() {m_company->s_newPedidoPro();};
  virtual void s_newPresupuestoCli() {m_company->s_newPresupuestoCli();};
  virtual void s_newPedidoCli() {m_company->s_newPedidoCli();};
  virtual void s_newAlbaranCli() {m_company->s_newAlbaranCli();};
  virtual void s_newFacturaCli() {m_company->s_newFacturaCli();};
  virtual void s_newArticulo() {m_company->s_newArticulo();};
  virtual void s_provincias() {m_company->s_provincias();};
  virtual void s_inventarios() {m_company->s_inventarios();};


  virtual void s_indexador()
  {
    if (m_list->isHidden())
    {
      m_list->show();
      m_list->undock();
      m_list->dock();
    }
    else
      m_list->hide();
  };
  void about();
  void aboutQt();
  virtual void s_About();
};
#endif
