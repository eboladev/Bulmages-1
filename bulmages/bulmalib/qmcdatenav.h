/***************************************************************************
 *   Copyright (c) 1998 - 2000, Michael van der Westhuizen.                *
 *   All rights reserved.                                                  *
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
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
#include <Q3PtrList>
#include <QPixmap>
#include <QMouseEvent>
#include <QKeyEvent>
#include <Q3PopupMenu>
#include <QPaintEvent>


class Q3PopupMenu;


class QmcDateNav : public QWidget {
    Q_OBJECT

public:
    QmcDateNav(class QWidget * parent = 0, const char * name = 0);
    QmcDateNav(const class QDate &, class QWidget * parent = 0, const char * name = 0);
    ~QmcDateNav();
    virtual QSizePolicy sizePolicy() const;
    virtual QSize sizeHint() const;
    virtual QSize minimumSizeHint() const;
    bool frame() const;
    void setFrame(bool);
    Q3PtrList<QDate> selectedDates() const;
    bool setDate(const QDate d);
    bool outlook() const;
    void setOutlook(bool);
    void installEventDayList(Q3PtrList<QDate> *);
    void removeEventDayList();
    void installNonWorkingDayList(Q3PtrList<QDate> *);
    void removeNonWorkingDayList();

public slots:
    void forceUpdate();

signals:
    void dateChanged();
    void yearChanged(int);
    void close();

private slots:
    void monthMenuClicked(int);
    void rbMenuClicked(int);

private:
    QDate currentMonth;
    Q3PopupMenu *sevenMonthPopup;
    Q3PopupMenu *rbPopup;
    bool pmDirty;
    bool doFrame;
    QPixmap *pm;
    void init();
    void makePixmap();
    void drawArrows(QPainter &);
    QRect * drawCaptionText(QPainter &);
    void drawCaption(QPainter &);
    void drawFrame(QPainter &);
    void drawDays(QPainter &);
    void drawDates(QPainter &);
    void drawSelectedDate(int, bool pe = false);
    void undrawSelectedDate(int);
    int numSelected();
    void drawToday(int);
    void drawSelections();
    void clear();
    void changeMonth(bool forward, int steps);
    QString monthName(int);
    int fixDays(int, int, int);
    void prepPopup();
    void prepRBPopup();
    bool hasEvent(const QDate &);
    bool isNonWorkingDay(const QDate &);
    int findIndex(QRect &);
    int findIndex(QDate &);

protected:
    virtual void paintEvent(QPaintEvent *);
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseDoubleClickEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);
    virtual void keyPressEvent(QKeyEvent *);
};

#endif

