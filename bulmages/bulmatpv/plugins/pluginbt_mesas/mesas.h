#ifndef MESAS_H
#define MESAS_H

#include <QTableWidget>
#include "btcompany.h"
#include "blwidget.h"




#include "ui_distromesasbase.h"

class DistroMesas : public BlWidget, public Ui_DistroMesasBase
{
Q_OBJECT

public:
  QString m_background;

public:
  DistroMesas( BtCompany *emp, QWidget *parent);
  virtual ~DistroMesas();
  void paintEvent ( QPaintEvent * event );
  void importXML(const QString val);
  QString exportXML();

public slots:
  virtual void on_mui_mesa6_clicked();
  virtual void on_mui_escala_clicked();
  virtual void on_mui_escala1_clicked();
  virtual void on_mui_cerrar_clicked();
  virtual void on_mui_borrar_clicked();
  virtual void on_mui_ver_ticket_clicked();
  virtual void on_mui_cambiar_nombre_clicked();
  virtual void on_mui_cambiar_imagen_clicked();
  virtual void on_mui_bloquear_toggled(bool bloq);
};


#include "ui_mesasbase.h"



class Mesas : public BlWidget, public Ui_MesasBase
{
    Q_OBJECT
public:
    QWidget *m_centralWidget;
    DistroMesas *m_distro;
public:
    Mesas ( BtCompany *emp, QWidget *parent );
    virtual ~Mesas();

public slots:
    virtual void on_mui_mesas_clicked();
};




class Mesa : public BlWidget
{
Q_OBJECT
public:
  QPoint offset;
  QString m_filename;
  QString m_nombreMesa;
  int m_XScale;
  int m_YScale;
  int m_escalando;

public:
  Mesa( BtCompany *emp, QWidget *parent);
  virtual ~Mesa();
  void paintEvent ( QPaintEvent * event );
  void mousePressEvent(QMouseEvent* event);
  void mouseMoveEvent(QMouseEvent* event);
  void mouseReleaseEvent(QMouseEvent* event);
  void mouseDoubleClickEvent ( QMouseEvent * event );
  void importXML(const QString val);
  QString exportXML();
public slots:
  virtual void ctxMenu(const QPoint &pos);
  virtual void cambiarImagen();
  virtual void cambiarNombre();
  virtual void abrirMesa();
};


#endif



