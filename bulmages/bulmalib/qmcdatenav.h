/***************************************************************************
 *   Copyright (c) 1998 - 2000, Michael van der Westhuizen.                *
 *   All rights reserved.                                                  *
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org                                                 *
 *                                                                         *
 *   See LICENSE.BSD for details.                                          *
 *                                                                         *
 * If this file is being built with or linked against any part of the      *
 * K Desktop Environment, KOffice Office Suite, KDE Libraries or KDE core  *
 * applications, the licensing model that may, at the developers           *
 * discretion, apply is LGPL.                                              *
 ***************************************************************************/

#ifndef QmcDATENAV_H
#define QmcDATENAV_H

#include <QWidget>
#include <QDateTime>
#include <QList>
#include <QPixmap>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QMenu>
#include <QPaintEvent>


class QMenu;


/// Abre el di&aacute;logo del Calendario.
/** Esta clase muestra el di&aacute;logo de seleccionar una fecha
    para ello muestra un calendario por meses en el que se puede
    hacer doble click sobre cualquier d&iacute;a, y da ese d&iacute;a como v&aacute;lido.
    La clase utiliza un Pixmap para mostrar el calendario y captura los
    diferentes enventos del rat&oacute; que se producen sobre el pixmap para
    mostrar los meses, cambiar el a&ntilde;o o seleccionar los diferentes dias.   */
class QmcDateNav : public QWidget {
    Q_OBJECT

public:
    /// Constructor simple
    QmcDateNav(class QWidget *parent = 0);
    /// Constructor mostrando una fecha inicial
    QmcDateNav(const class QDate &, class QWidget *parent = 0);
    /// Destructor
    ~QmcDateNav();
    virtual QSizePolicy sizePolicy() const;
    virtual QSize sizeHint() const;
    virtual QSize minimumSizeHint() const;
    bool frame() const;
    void setFrame(bool);
    /// Devuelve las fechas seleccionadas en forma de lista.
    QList<QDate> selectedDates() const;
    /// Establece la fecha en el calendario.
    bool setDate(const QDate d);
    bool outlook() const;
    void setOutlook(bool);
    void installEventDayList(QList<QDate> *);
    void removeEventDayList();
    /// Agrega dias festivos al calendario.
    void installNonWorkingDayList(QList<QDate> *);
    /// Quita dias festivos del calendario.
    void removeNonWorkingDayList();

public slots:
    void forceUpdate();

signals:
    /// Signal de cambio de fecha.
    void dateChanged();
    /// Signal de cambio de a&ntilde;o.
    void yearChanged(int);
    /// Signal de cerrar el widget.
    void close();

private slots:
    void monthMenuClicked(int);
    void rbMenuClicked(int);

private:
    /// El mes actual.
    QDate m_currentMonth;
    /// El men&uacute; desplegable de los meses.
    QMenu *m_sevenMonthPopup;
    /// El men&uacute; desplegable del bot&oacute; derecho.
    QMenu *m_rbPopup;
    /// Indica si la pantalla necesita repintar.
    bool m_pmDirty;
    /// Indica si el frame debe ser rehecho.
    bool m_doFrame;
    /// Imagen del calendario renderizada.
    QPixmap *m_pm;
    void init();
    /// Crea la imagen.
    void makePixmap();
    /// Dibuja las felchas para el cambio de mes.
    void drawArrows(QPainter &);
    /// Dibuja el texto de encabezado.
    QRect *drawCaptionText(QPainter &);
    /// Dibuja el t&iacute;tulo.
    void drawCaption(QPainter &);
    /// Dibuja el Frame.
    void drawFrame(QPainter &);
    /// Dibuja los dias.
    void drawDays(QPainter &);
    /// Dibuja las fechas marcadas.
    void drawDates(QPainter &);
    /// Dibuja las fechas seleccionadas.
    void drawSelectedDate(int, bool pe = false);
    /// Dibuja una fecha deseleccionada.
    void undrawSelectedDate(int);
    int numSelected();
    /// Dibuja el d&iacute;a actual.
    void drawToday(int);
    /// Dibuja las selecciones.
    void drawSelections();
    /// Limpia el calendario.
    void clear();
    /// Hace un c&acute;mbio de m&eacute;s.
    void changeMonth(bool forward, int steps);
    /// Devuelve el nombre del m&eacute;s.
    QString monthName(int);
    int fixDays(int, int, int);
    /// Preparaciones para el men&uacute; contextual de los meses.
    void prepPopup();
    /// Preparaciones para el men&uacute; contextual del bot&oacute;n izquierdo del rat&oacute;n.
    void prepRBPopup();
    bool hasEvent(const QDate &);
    /// Determina si un d&iacute;a es festivo.
    bool isNonWorkingDay(const QDate &);
    int findIndex(QRect &);
    int findIndex(QDate &);

protected:
    /// Evento de pintado del widget.
    virtual void paintEvent(QPaintEvent *);
    /// Evento de pulsacion del rat&oacute;n.
    virtual void mousePressEvent(QMouseEvent *);
    /// Evento de doble pulsaci&oacute;n del rat&oacute;n.
    virtual void mouseDoubleClickEvent(QMouseEvent *);
    /// Evento de movimiento del rat&oacute;.
    virtual void mouseMoveEvent(QMouseEvent *);
    /// Evento de pulsaci&oacute;n de una tecla.
    virtual void keyPressEvent(QKeyEvent *);
};

#endif

