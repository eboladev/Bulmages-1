#ifndef MODIFICADORES_H
#define MODIFICADORES_H

#include <QDialog>
#include <QProcess>
#include <QDockWidget>
#include <QString>
#include "blwidget.h"
#include "ui_modificadoresbase.h"

class BtCompany;

class Modificadores : public QDialog, BlMainCompanyPointer, public Ui_ModificadoresBase
{
    Q_OBJECT

private:
    void sendKey(int tecla, QString texto);
    QString m_fieldToModify;

public:
    Modificadores ( BlMainCompany *emp, QWidget *parent, bool editFieldToModify = false, QString fielToModify = "" );
    virtual ~Modificadores();
    
public slots:
    virtual void modificadorClicked();
    void on_mui_tecla_limpiar_clicked();
    void on_mui_tecla_1_clicked();
    void on_mui_tecla_2_clicked();
    void on_mui_tecla_3_clicked();
    void on_mui_tecla_4_clicked();
    void on_mui_tecla_5_clicked();
    void on_mui_tecla_6_clicked();
    void on_mui_tecla_7_clicked();
    void on_mui_tecla_8_clicked();
    void on_mui_tecla_9_clicked();
    void on_mui_tecla_0_clicked();
    void on_mui_tecla_borrar_clicked();
    void on_mui_tecla_q_clicked();
    void on_mui_tecla_w_clicked();
    void on_mui_tecla_e_clicked();
    void on_mui_tecla_r_clicked();
    void on_mui_tecla_t_clicked();
    void on_mui_tecla_y_clicked();
    void on_mui_tecla_u_clicked();
    void on_mui_tecla_i_clicked();
    void on_mui_tecla_o_clicked();
    void on_mui_tecla_p_clicked();
    void on_mui_tecla_a_clicked();
    void on_mui_tecla_s_clicked();
    void on_mui_tecla_d_clicked();
    void on_mui_tecla_f_clicked();
    void on_mui_tecla_g_clicked();
    void on_mui_tecla_h_clicked();
    void on_mui_tecla_j_clicked();
    void on_mui_tecla_k_clicked();
    void on_mui_tecla_l_clicked();
    void on_mui_tecla_enye_clicked();
    void on_mui_tecla_intro_clicked();
    void on_mui_tecla_z_clicked();
    void on_mui_tecla_x_clicked();
    void on_mui_tecla_c_clicked();
    void on_mui_tecla_v_clicked();
    void on_mui_tecla_b_clicked();
    void on_mui_tecla_n_clicked();
    void on_mui_tecla_m_clicked();
    void on_mui_tecla_coma_clicked();
    void on_mui_tecla_punto_clicked();
    void on_mui_tecla_guion_clicked();
    void on_mui_tecla_espacio_clicked();
    void on_mui_tecla_inicio_clicked();
    void on_mui_tecla_fin_clicked();
    void on_mui_tecla_izquierda_clicked();
    void on_mui_tecla_derecha_clicked();
    void on_mui_tecla_arriba_clicked();
    void on_mui_tecla_abajo_clicked();
    void on_mui_tecla_suprimir_clicked();    
    void on_mui_aceptar_clicked();
};

#endif

