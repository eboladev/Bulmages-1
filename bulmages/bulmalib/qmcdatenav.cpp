/***************************************************************************
 *   Copyright (c) 1998 - 2000, Michael van der Westhuizen.                *
 *   All rights reserved.                                                  *
 *                                                                         *
 *   See LICENSE.BSD for details.                                          *
 *                                                                         *
 * If this file is being built with or linked against any part of the      *
 * K Desktop Environment, KOffice Office Suite, KDE Libraries or KDE core  * 
 * applications, the licensing model that may, at the developers           *
 * discretion, apply is LGPL.                                              *
 *                                                                         *
 *   Modified by:                                                          *
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *                                                                         *
 ***************************************************************************/

#include <QPixmap>
#include <QPainter>
#include <QRect>
#include <QMenu>
#include <QCursor>
#include <QPaintEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QListIterator>
#include <QPalette>

#include "qmcdatenav.h"
#include "funcaux.h"



struct QmcDate {
    QDate d; /// Date
    QRect r; /// Rect
    bool s; /// Selected
    bool t; /// Today
};

QList<QmcDate> dateList; /// All visible days

QList<QDate> *eventDayList; /// Only ever a pointer to a list
QList<QDate> *nonWorkingDayList; /// Ditto

bool doOutlook; /// Are we to paint in outlook compatible mode?

/// Los QRects descritos siven para determinar si un click realmente afecta.
QRect captionRect;
QRect leftArrowRect;
QRect rightArrowRect;

QRect sunBR;
QRect monBR;
QRect tueBR;
QRect wedBR;
QRect thuBR;
QRect friBR;
QRect satBR;

QRect daysRect;


QmcDateNav::QmcDateNav(class QWidget *parent) : QWidget(parent) {
    _depura("QmcDateNav::QmcDateNav", 0);
    m_currentMonth = QDate(QDate::currentDate());
    init();
    setFocusPolicy(Qt::StrongFocus);
    _depura("END QmcDateNav::QmcDateNav", 0);
}


QmcDateNav::QmcDateNav(const class QDate &d, class QWidget *parent) : QWidget(parent) {
    _depura("QmcDateNav::QmcDateNav", 0);
    m_currentMonth = QDate(d);
    if (!m_currentMonth.isValid()) {
        m_currentMonth = QDate(QDate::currentDate());
    }
    init();
    _depura("END QmcDateNav::QmcDateNav", 0);
}


QmcDateNav::~QmcDateNav() {
    _depura("QmcDateNav::~QmcDateNav", 0);
    if (m_pm)
        delete m_pm;
    if (m_sevenMonthPopup)
        delete m_sevenMonthPopup;
    if (m_rbPopup)
        delete m_rbPopup;
    dateList.clear(); /// will delete all QmcDate objects.

    /// Do NOT delete these - that's the job of the app programmer.
    eventDayList = 0;
    nonWorkingDayList = 0;
    _depura("END QmcDateNav::~QmcDateNav", 0);
}


void QmcDateNav::init() {
    _depura("QmcDateNav::init", 0);
    m_sevenMonthPopup = new QMenu(this);
    connect(m_sevenMonthPopup, SIGNAL(activated(int)), this, SLOT(monthMenuClicked(int)));

    m_rbPopup = new QMenu(this);
    connect(m_rbPopup, SIGNAL(activated(int)), this, SLOT(rbMenuClicked(int)));

    dateList.clear();

    /// Initialise to 0 to prevent crashes.
    eventDayList = 0;
    nonWorkingDayList = 0;

    m_pmDirty = true;
    m_doFrame = true;
    doOutlook = false;
    m_pm = 0;

    setMouseTracking(true); /// So we can do the cursor changes properly.
    _depura("END QmcDateNav::init", 0);
}


QSizePolicy QmcDateNav::sizePolicy() const {
    _depura("QmcDateNav::sizePolicy", 0);
    return QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}


QSize QmcDateNav::sizeHint() const {
    _depura("QmcDateNav::sizeHint", 0);
    return minimumSizeHint();
}


QSize QmcDateNav::minimumSizeHint() const {
    _depura("QmcDateNav::minimumSizeHint", 0);
    int w, w1, w2;
    w1 = w2 = w = 0;
    int h = 0;
    int fw = frame() ? 1 : 0;

    w1 = ((fontMetrics().width("00") + 6 ) * 7) + 12 + (fw * 2);
    w2 = fontMetrics().width(tr("SEPTEMBER")) + fontMetrics().width(" 0000") + 12 + 6 + (fw * 2);

    w = w1 > w2 ? w1 : w2;

    h = fw + 4 + fontMetrics().height() + 4 + fontMetrics().boundingRect(QChar('S')).height() + 3 + 1;
    h += ((fontMetrics().boundingRect(QChar('0')).height()) * 6) + 30 + 8 + fw + 1;

    _depura("END QmcDateNav::minimumSizeHint", 0);
    return QSize(w, h);
}


/// Pintar el calendario.
/** Este m&eacute;todo se lanza cada vez que hay que redibujar el calendario.
    @param QPaintEvent * evento producido. (No se utiliza). */
void QmcDateNav::paintEvent(QPaintEvent *) {
    _depura("QmcDateNav::paintEvent", 0);
    if (m_pmDirty || m_pm == 0)
        makePixmap();
    QPainter p;
    p.begin(m_pm);
    p.setPen(QPalette(this->palette()).color(QPalette::WindowText));
    p.setBackgroundMode(Qt::OpaqueMode);
    p.setBackground(this->palette().brush(QPalette::Window));
    p.setFont(this->font());

    if (m_pmDirty && m_pm != 0) {
        p.fillRect(QRectF (QPoint(0,0), QSize(width(),height())), palette().brush(QPalette::Base));
        if (frame())
            drawFrame(p);
        drawCaption(p);
        drawDays(p);
        drawDates(p);
    }
    p.end();
    p.begin(this);
    p.drawPixmap(0, 0, *m_pm);
    p.end();
    m_pmDirty = false;

    drawSelections();
    /// The 'today' indicator - a red box around todays date.
    if (m_currentMonth.month() == QDate::currentDate().month()) {
        for (int i = 0; i <= 41; i++) {
            if (dateList.at(i).t) {
                drawToday(i);
                break;
            } // end if
        } // end for
    } // end if
    /// If we set this in init(), the widget is black before the first paint event,
    /// which is not cool.
    /// Once we've done the initial paint, this will reduce flicker.
    _depura("END QmcDateNav::paintEvent", 0);
}

/// Creacion de una imagen
/** Este m&eacute;todo crea una imagen con el ancho y alto predeterminados, siempre que
    se pueda. Si la imagen ya est&aacute; creada se borra previamente. */
void QmcDateNav::makePixmap() {
    _depura("QmcDateNav::makePixmap", 0);
    if (!m_pmDirty)
        return;
    if (m_pm != 0)
        delete m_pm;

    m_pm = new QPixmap(width(), height());
    _depura("END QmcDateNav::makePixmap", 0);
}

/// Reestablece el Frame.
/** Si la ventana ha sido redimensionada, o el frame ha cambiado se reestable. */
void QmcDateNav::setFrame(bool f) {
    _depura("QmcDateNav::setFrame", 0);
    if (f == m_doFrame)
        return;
    m_doFrame = f;
    updateGeometry();
    _depura("END QmcDateNav::setFrame", 0);
}


/// Indica si el frame debe ser reestablecido o no.
/** */
bool QmcDateNav::frame() const {
    _depura("QmcDateNav::frame", 0);
    return m_doFrame;
}


/// Dibuja las flechas para el cambio de mes.
/** Este m&eacute;todo se encarga de dibujar las flechas para el cambio de mes.
    @param p el QPainter en el que se deben pintar las flechas.*/
void QmcDateNav::drawArrows(QPainter &p) {
    _depura("QmcDateNav::drawArrows", 0);
    QRect *tbr = drawCaptionText(p);
    int topBase = ((tbr->bottom() + 2) - (frame() ? 1 : 0)) / 2;
    delete tbr;
    tbr = 0;
    int leftBase = 4 + (frame() ? 1 : 0);
    captionRect.setLeft(leftBase + 1 + 4 + 2);
    leftArrowRect.setLeft(leftBase);
    leftArrowRect.setRight(leftBase + 1 + 4 + 1);
    leftArrowRect.setTop(topBase + 1 - 4 - 1);
    leftArrowRect.setBottom(topBase + 1 + 4 + 1);
    p.setPen(palette().color(QPalette::Active, QPalette::Text));
    /// left triangle - outline.
    p.drawLine(leftBase + 1, topBase + 1, leftBase + 1 + 4, topBase + 1 - 4);
    p.drawLine(leftBase + 1, topBase + 1, leftBase + 1 + 4, topBase + 1 + 4);
    p.drawLine(leftBase + 1 + 4, topBase + 1 - 4, leftBase + 1 + 4, topBase + 1 + 4);
    /// left triangle - fill.
    p.drawLine(leftBase + 1 + 3, topBase + 1 - 3, leftBase + 1 + 3, topBase + 1 + 3);
    p.drawLine(leftBase + 1 + 2, topBase + 1 - 2, leftBase + 1 + 2, topBase + 1 + 2);
    p.drawLine(leftBase + 1 + 1, topBase + 1 - 1, leftBase + 1 + 1, topBase + 1 + 1);
    /// adjust for right hand triangle.
    leftBase = width() - (frame() ? 1 : 0) - 2 - 4;
    captionRect.setRight(leftBase - 1 - 4 - 1);
    rightArrowRect.setLeft(leftBase - 1 - 4);
    rightArrowRect.setRight(leftBase + 1);
    rightArrowRect.setTop(topBase + 1 - 4 - 1);
    rightArrowRect.setBottom(topBase + 1 + 4 + 1);
    /// right triangle - outline.
    p.drawLine(leftBase - 1, topBase + 1, leftBase - 1 - 4, topBase + 1 - 4);
    p.drawLine(leftBase - 1, topBase + 1, leftBase - 1 - 4, topBase + 1 + 4);
    p.drawLine(leftBase - 1 - 4, topBase + 1 - 4, leftBase - 1 - 4, topBase + 1 + 4);
    /// right triangle - fill.
    p.drawLine(leftBase - 1 - 3, topBase + 1 - 3, leftBase - 1 - 3, topBase + 1 + 3);
    p.drawLine(leftBase - 1 - 2, topBase + 1 - 2, leftBase - 1 - 2, topBase + 1 + 2);
    p.drawLine(leftBase - 1 - 1, topBase + 1 - 1, leftBase - 1 - 1, topBase + 1 + 1);
    _depura("END QmcDateNav::drawArrows", 0);
}


///
/** */
QRect *QmcDateNav::drawCaptionText(QPainter &p) {
    QRectF br(0, 3, width(), 15);
    QRect br2 = br.toRect();
    p.setPen(palette().color(QPalette::Active, QPalette::Text));
    QString cText = monthName(m_currentMonth.month());
    cText += " ";
    cText += QString::number(m_currentMonth.year(), 10);
    p.drawText(br, cText, Qt::AlignTop | Qt::AlignHCenter);
    captionRect.setTop(br2.top());
    captionRect.setBottom(br2.bottom());
    return new QRect(br2);
}


///
/* */
void QmcDateNav::drawCaption(QPainter & p) {
    QRect *tbr = drawCaptionText(p);
    int fw = frame() ? 1 : 0;
    p.fillRect(fw, fw, width() - (fw * 2), tbr->bottom() + 2 + (frame() ? 0 : 1), palette().background());
    drawCaptionText(p);
    p.setPen(palette().color(QPalette::Active, QPalette::Dark));
    p.drawLine(fw, tbr->bottom() + 3, width() - fw - 1, tbr->bottom() + 3);
    if (frame())
        p.drawLine(width() - fw - 1, tbr->bottom() + 3, width() - fw - 1, fw);
    delete tbr;
    tbr = 0;
    drawArrows(p);
}


///
/** */
void QmcDateNav::drawFrame(QPainter & p) {
    if(!m_doFrame)
        return;
    p.setPen(palette().color(QPalette::Active, QPalette::Dark));
    p.drawLine(0, 0, width() - 2, 0);
    p.drawLine(0, 0, 0, height() - 2);
}


/// Dibuja los d&iacute;s en el calendario.
/** */
void QmcDateNav::drawDays(QPainter &p) {
    int dayTop = drawCaptionText(p)->bottom() + 4;
    int colWidth = p.fontMetrics().width("00") + 6;
    int tableWidth = (colWidth * 7);
    int leftBase = (width() - tableWidth) / 2;

    sunBR = QRect(leftBase, dayTop, p.fontMetrics().width("00") + 6, p.fontMetrics().height());
    monBR = QRect(sunBR.right() + 1, dayTop, sunBR.width(), sunBR.height());
    tueBR = QRect(monBR.right() + 1, dayTop, sunBR.width(), sunBR.height());
    wedBR = QRect(tueBR.right() + 1, dayTop, sunBR.width(), sunBR.height());
    thuBR = QRect(wedBR.right() + 1, dayTop, sunBR.width(), sunBR.height());
    friBR = QRect(thuBR.right() + 1, dayTop, sunBR.width(), sunBR.height());
    satBR = QRect(friBR.right() + 1, dayTop, sunBR.width(), sunBR.height());
    QRect tSatBR;

    p.setPen(palette().color(QPalette::Active, QPalette::Text));

    p.drawText(sunBR, QString(tr("Sunday")[0]), Qt::AlignTop | Qt::AlignHCenter);
    p.drawText(monBR, QString(tr("Monday")[0]), Qt::AlignTop | Qt::AlignHCenter);
    p.drawText(tueBR, QString(tr("Tuesday")[0]), Qt::AlignTop | Qt::AlignHCenter);
    p.drawText(wedBR, QString(tr("Wednesday")[0]), Qt::AlignTop | Qt::AlignHCenter);
    p.drawText(thuBR, QString(tr("Thursday")[0]), Qt::AlignTop | Qt::AlignHCenter);
    p.drawText(friBR, QString(tr("Friday")[0]), Qt::AlignTop | Qt::AlignHCenter);
    p.drawText(satBR, QString(tr("Saturday")[0]), Qt::AlignTop | Qt::AlignHCenter);

    p.setPen(palette().color(QPalette::Active, QPalette::Mid));

    p.drawLine(leftBase, tSatBR.bottom() + 1, leftBase + tableWidth, tSatBR.bottom() + 1);
    daysRect = QRect(sunBR.left(), sunBR.top(), satBR.right() - sunBR.left(), sunBR.height());
}


///
/** */
void QmcDateNav::drawDates(QPainter &p) {
    QDate *previousMonth;
    QDate *nextMonth;
    dateList.clear();

    /// We need to find out what day the first of this month was.
    QDate *fdom = new QDate(m_currentMonth.year(), m_currentMonth.month(), 1);

    /// That value is assigned to the start position of our matrix.
    /// The problem here is that Qt returns monday as 1, not 0, and Sunday
    /// as 7! We count our matrix from 0, so we just turn a 7 into 0.
    int matrixStartPos = fdom->dayOfWeek();

    if (matrixStartPos == 7)
        matrixStartPos = 0;

    /// If we are starting the month before thursday, we give the previous month
    /// a little more space.
    if (matrixStartPos < 4)
        matrixStartPos = matrixStartPos + 7;
    if (!(m_currentMonth.month() == 1))
        previousMonth = new QDate(m_currentMonth.year(), m_currentMonth.month() - 1, 1);
    else
        previousMonth = new QDate(m_currentMonth.year() - 1, 12, 1);

    if (!(m_currentMonth.month() == 12))
        nextMonth = new QDate(m_currentMonth.year(), m_currentMonth.month() + 1, 1);
    else
        nextMonth = new QDate(m_currentMonth.year() + 1, 1, 1);

    int leftBase = 0;
    int topBase = 0;
    int dayCounter = 1;
    int nextMonthDayCounter = 1;
    QDate *insertedDate;
    int col = 0;
    int row = 0;

    for (int i = 0; i <= 41; i++) {
        if (!matrixStartPos == 0) {
            insertedDate = new QDate(previousMonth->year(), previousMonth->month(), previousMonth->daysInMonth() - matrixStartPos + 1);
            matrixStartPos--;
        } else {
            if (!(dayCounter == (m_currentMonth.daysInMonth() + 1))) {
                insertedDate = new QDate(m_currentMonth.year(), m_currentMonth.month(), dayCounter);
                dayCounter++;
            } else {
                insertedDate = new QDate(nextMonth->year(), nextMonth->month(), nextMonthDayCounter);
                nextMonthDayCounter++;
            } // end if
        } // end if

        if (col == 0)
            leftBase = (frame() ? 1 : 0) + 6;
        else
            leftBase = dateList.last().r.right() + 1;

        if (row == 0)
            topBase = (frame() ? 1 : 0) + 4 + fontMetrics().height() + 4 + fontMetrics().boundingRect(QChar('S')).height() + 3 + 1 + 3;
        else {
            topBase = (frame() ? 1 : 0) + 4 + fontMetrics().height() + 4 + fontMetrics().boundingRect(QChar('S')).height() + 3 + 1 + 3;
            topBase += (((fontMetrics().boundingRect(QChar('0')).height()) + 6) * row);
        } // end if

        if (insertedDate->month() != m_currentMonth.month())
            p.setPen(palette().color(QPalette::Active, QPalette::Mid));
        else
            p.setPen(palette().color(QPalette::Active, QPalette::Text));

        bool he = hasEvent(*insertedDate);
        bool nwd = isNonWorkingDay(*insertedDate);
        QFont nFont = p.font();
        QFont oFont = p.font();
        if (he)
            nFont.setBold(true);
        if (nwd)
            nFont.setItalic(true);
        p.setFont(nFont);
        p.drawText(leftBase, topBase, fontMetrics().width("00") + 6, fontMetrics().boundingRect(QChar('0')).height() + 6, Qt::AlignCenter, QString::number(insertedDate->day(), 10));

        if (he || nwd)
            p.setFont(oFont);

        QmcDate d;
        d.d = QDate(insertedDate->year(), insertedDate->month(), insertedDate->day());
        d.r = QRect(leftBase, topBase, fontMetrics().width("00") + 6, fontMetrics().boundingRect(QChar('0')).height() + 6);
        d.s = false;
        d.t = false;
        if (d.d == QDate::currentDate())
            d.t = true;
        if (d.d == m_currentMonth)
            d.s = true;

        dateList.append(d);

        if (col != 6)
            col++;
        else if (col == 6) {
            col = 0;
            row++;
        } // end if
    } // end for

    if (previousMonth)
        delete previousMonth;
    if (nextMonth)
        delete nextMonth;
    if (fdom)
        delete fdom;
    if (insertedDate)
        delete insertedDate;
    previousMonth = nextMonth = fdom = insertedDate = 0;
}


///
/** */
void QmcDateNav::keyPressEvent(QKeyEvent *e) {
    switch (e->key()) {
    case 4100:
        emit close();
        break;
    case 4117:
        for (int i = 0; i <= 41; i++) {
            if (dateList.at(i).t) {
                dateList[i].t = false;
                dateList[i].s = false;
                dateList[i + 7].t = true;
                dateList[i + 7].s = true;
                break;
            } // end if
        } // end for
        break;
    case 4116:
        for (int i = 0; i <= 41; i++) {
            if (dateList.at(i).t) {
                dateList[i].t = false;
                dateList[i].s = false;
                dateList[i + 1].t = true;
                dateList[i + 1].s = true;
                drawToday(i + 1);
                break;
            } // end if
        } // end for
        break;
    case 4114:
        for (int i = 0; i <= 41; i++) {
            if (dateList.at(i).t) {
                dateList[i].t = false;
                dateList[i].s = false;
                dateList[i - 1].t = true;
                dateList[i - 1].s = true;
                drawToday(i - 1);
                break;
            } // end if
        } // end for
        break;
    case 4115:
        for (int i = 0; i <= 41; i++) {
            if (dateList.at(i).t) {
                dateList[i].t = false;
                dateList[i].s = false;
                dateList[i - 7].t = true;
                dateList[i - 7].s = true;
                drawToday(i - 7);
                break;
            } // end if
        } // end for
        break;
    } // end switch
    drawSelections();
    emit dateChanged();
}


///
/** */
void QmcDateNav::mouseDoubleClickEvent(QMouseEvent *e) {
    fprintf(stderr, "Doble click\n");
    if (e->button() == Qt::RightButton) {
        prepRBPopup();
        m_rbPopup->exec(QCursor::pos());
    } // end if
    if (e->button() != Qt::LeftButton)
        return;
    /// Mouse Rect.
    QRect mr(e->x(), e->y(), 1, 1);

    if (captionRect.contains(mr, true)) {
        prepPopup();
        QPoint pa = mapToGlobal(QPoint(captionRect.left(), captionRect.bottom() + 4));
        m_sevenMonthPopup->exec(pa, m_sevenMonthPopup->actionAt(pa));
        return;
    } // end if
    if (leftArrowRect.contains(mr, true)) {
        changeMonth(false, 1);
    } // end if
    if (rightArrowRect.contains(mr, true)) {
        changeMonth(true, 1);
    } // end if
    if (daysRect.contains(mr, true)) {
        int dayToSelect = 0;
        if (sunBR.contains(mr, true))
            dayToSelect = 7;
        if (monBR.contains(mr, true))
            dayToSelect = 1;
        if (tueBR.contains(mr, true))
            dayToSelect = 2;
        if (wedBR.contains(mr, true))
            dayToSelect = 3;
        if (thuBR.contains(mr, true))
            dayToSelect = 4;
        if (friBR.contains(mr, true))
            dayToSelect = 5;
        if (satBR.contains(mr, true))
            dayToSelect = 6;
        if (dayToSelect == 0)
            return;
        if (theApp->keyboardModifiers() != Qt::ControlModifier) {
            clear();
        }
        for (int i = 0; i <= 41; i++)
            if (dateList.at(i).d.dayOfWeek() == dayToSelect)
                dateList[i].s = true;
        drawSelections();
        emit dateChanged();
        emit close();
        return;
    } // end if
    QRect weeksRect(frame() ? 1 : 0,
                    dateList.at(0).r.top(),
                    dateList.at(0).r.left() - (frame() ? 1 : 0),
                    dateList.at(41).r.bottom() - dateList.at(0).r.top());

    if (weeksRect.contains(mr, true)) {
        if (theApp->keyboardModifiers() != Qt::ControlModifier) {
            clear();
        }
        int i = -1;
        if (e->y() > dateList.at(35).r.top())
            for (i = 35; i <= 41; i++)
                dateList[i].s = true;
        else if (e->y() > dateList.at(28).r.top())
            for (i = 28; i <= 34; i++)
                dateList[i].s = true;
        else if (e->y() > dateList.at(21).r.top())
            for (i = 21; i <= 27; i++)
                dateList[i].s = true;
        else if (e->y() > dateList.at(14).r.top())
            for (i = 14; i <= 20; i++)
                dateList[i].s = true;
        else if (e->y() > dateList.at(7).r.top())
            for (i = 7; i <= 13; i++)
                dateList[i].s = true;
        else if (e->y() > dateList.at(0).r.top())
            for (i = 0; i <= 6; i++)
                dateList[i].s = true;
        drawSelections();
        emit dateChanged();
        return;
    } // end if
    for (int r = 0; r <= 41; r++) {
        if (dateList.at(r).r.contains(mr, true)) {
            /// Click outside the current month.
            if (dateList.at(r).d.month() != m_currentMonth.month() && theApp->keyboardModifiers() != Qt::ControlModifier && theApp->keyboardModifiers() != Qt::ShiftModifier) {
                QDate sd(dateList.at(r).d.year(), dateList.at(r).d.month(), dateList.at(r).d.day());
                setDate(sd);
                return;
            } // end if
            /// MultiSelection : Control Button.
            if (theApp->keyboardModifiers() & Qt::ControlModifier) {
                drawSelectedDate(r);
                emit dateChanged();
                return;
            } // end if
            /// MultiSelection : Shift Button.
            if (theApp->keyboardModifiers() & Qt::ShiftModifier) {
                /// Always draw from the start of the selection.
                int markStart = findIndex(selectedDates()[0]);
                clear();
                int markEnd = r;
                int b = markStart < markEnd ? markStart : markEnd;
                int e = markStart > markEnd ? markStart : markEnd;
                for (; b <= e; b++)
                    dateList[b].s = true;
                drawSelections();
                return;
            } else {
                clear();
                drawSelectedDate(r);
                emit dateChanged();
                emit close();
                return;
            } // end if
        } // end if
    } // end for
}


///
/** */
void QmcDateNav::mousePressEvent(QMouseEvent *e) {
    if (e->button() == Qt::RightButton) {
        prepRBPopup();
        m_rbPopup->exec(QCursor::pos());
    } // end if
    if (e->button() != Qt::LeftButton)
        return;
    /// Mouse Rect.
    QRect mr(e->x(), e->y(), 1, 1);
    if (captionRect.contains(mr, true)) {
        prepPopup();
        QPoint pa = mapToGlobal(QPoint(captionRect.left(), captionRect.bottom() + 4));
        m_sevenMonthPopup->exec(pa, m_sevenMonthPopup->actionAt(pa));
        return;
    } // end if
    if (leftArrowRect.contains(mr, true)) {
        changeMonth(false, 1);
    } // end if
    if (rightArrowRect.contains(mr, true)) {
        changeMonth(true, 1);
    } // end if
    if (daysRect.contains(mr, true)) {
        int dayToSelect = 0;
        if (sunBR.contains(mr, true))
            dayToSelect = 7;
        if (monBR.contains(mr, true))
            dayToSelect = 1;
        if (tueBR.contains( mr, true))
            dayToSelect = 2;
        if (wedBR.contains(mr, true))
            dayToSelect = 3;
        if (thuBR.contains(mr, true))
            dayToSelect = 4;
        if (friBR.contains(mr, true))
            dayToSelect = 5;
        if (satBR.contains(mr, true))
            dayToSelect = 6;
        if (dayToSelect == 0)
            return;
        if (theApp->keyboardModifiers() != Qt::ControlModifier)
            clear();
        for (int i = 0; i <= 41; i++)
            if (dateList.at(i).d.dayOfWeek() == dayToSelect)
                dateList[i].s = true;
        drawSelections();
        emit dateChanged();
        return;
    }

    QRect weeksRect(frame() ? 1 : 0,
                    dateList.at(0).r.top(),
                    dateList.at(0).r.left() - (frame() ? 1 : 0),
                    dateList.at(41).r.bottom() - dateList.at(0).r.top());

    if (weeksRect.contains(mr, true)) {
        if (theApp->keyboardModifiers() != Qt::ControlModifier)
            clear();
        int i = -1;
        if (e->y() > dateList.at( 35 ).r.top())
            for (i = 35; i <= 41; i++)
                dateList[i].s = true;
        else if (e->y() > dateList.at(28).r.top())
            for (i = 28; i <= 34; i++)
                dateList[i].s = true;
        else if (e->y() > dateList.at(21).r.top())
            for (i = 21; i <= 27; i++)
                dateList[i].s = true;
        else if (e->y() > dateList.at(14).r.top())
            for (i = 14; i <= 20; i++)
                dateList[i].s = true;
        else if (e->y() > dateList[7].r.top())
            for (i = 7; i <= 13; i++)
                dateList[i].s = true;
        else if (e->y() > dateList.at(0).r.top())
            for (i = 0; i <= 6; i++)
                dateList[i].s = true;
        drawSelections();
        emit dateChanged();
        return;
    } // end if
    for (int r = 0; r <= 41; r++) {
        if (dateList.at(r).r.contains(mr, true)) {
            /// Click outside the current month.
            if (dateList.at(r).d.month() != m_currentMonth.month() && theApp->keyboardModifiers() != Qt::ControlModifier && theApp->keyboardModifiers() != Qt::ShiftModifier) {
                QDate sd(dateList.at(r).d.year(), dateList.at(r).d.month(), dateList.at(r).d.day());
                setDate( sd );
                return;
            } // end if
            /// MultiSelection : Control Button.
            if (theApp->keyboardModifiers() & Qt::ControlModifier) {
                drawSelectedDate(r);
                emit dateChanged();
                return;
            } // end if
            /// MultiSelection : Shift Button.
            if (theApp->keyboardModifiers() & Qt::ShiftModifier) {
                /// Always draw from the start of the selection.
                int markStart = findIndex(selectedDates()[0]);
                clear();
                int markEnd = r;
                int b = markStart < markEnd ? markStart : markEnd;
                int e = markStart > markEnd ? markStart : markEnd;
                for(; b <= e; b++)
                    dateList[b].s = true;
                drawSelections();
                return;
            } else {
                clear();
                drawSelectedDate(r);
                emit dateChanged();
                return;
            } // end if
        } // end if
    } // end for
}


///
/** */
/// i == the index of the date.
/// pe == in paint event (shouldn't toggle selection).
void QmcDateNav::drawSelectedDate(int i, bool pe) {
    if (dateList.at(i).s && !pe) {
        if (numSelected() <= 1)
            return;
        undrawSelectedDate(i);
        dateList[i].s = false;
        drawToday(i);
        return;
    } // end if
    dateList[i].s = true;
    QPainter p;
    p.begin(this);
    if (doOutlook)
        p.fillRect(dateList.at(i).r, palette().background());
    else
        p.fillRect(dateList.at(i).r, palette().highlight());
    p.setPen(QPalette(this->palette()).color(QPalette::HighlightedText));
    QFont pFont = p.font();
    if (hasEvent(dateList.at(i).d))
        pFont.setBold(true);
    if (isNonWorkingDay(dateList.at(i).d))
        pFont.setItalic(true);
    p.setFont(pFont);
    p.drawText(dateList.at(i).r, Qt::AlignCenter, QString::number(dateList.at(i).d.day(), 10));
    p.end();
    drawToday(i);
}


///
/** */
void QmcDateNav::undrawSelectedDate(int i) {
    QPainter p;
    p.begin(this);
    p.fillRect(dateList.at(i).r, palette().base());
    p.setPen(QPalette(this->palette()).color(QPalette::Mid));
    if (dateList.at(i).d.month() == m_currentMonth.month())
        p.setPen(QPalette(this->palette()).color(QPalette::Text));
    QFont pFont = p.font();
    if (hasEvent(dateList.at(i).d))
        pFont.setBold(true);
    if (isNonWorkingDay(dateList.at(i).d))
        pFont.setItalic(true);
    p.setFont(pFont);
    p.drawText(dateList.at(i).r, Qt::AlignCenter, QString::number(dateList.at(i).d.day(), 10));
    p.end();
}


///
/** */
int QmcDateNav::numSelected() {
    int rv = 0;
    for (int i = 0; i <= 41; i++) {
        if (dateList.at(i).s == true)
            rv++;
    }
    return rv;
}


///
/** */
void QmcDateNav::drawToday(int i) {
    if (!dateList.at(i).t)
        return;
    QPainter p;
    p.begin(this);
    p.setPen(QColor(255, 0, 0));
    p.drawRect(dateList.at(i).r);
    p.end();
}


///
/** */
void QmcDateNav::drawSelections() {
    for (int i = 0; i <= 41; i++) {
        if (dateList.at(i).s)
            drawSelectedDate(i, true);
    } // end for
}


///
/** */
void QmcDateNav::clear() {
    for (int i = 0; i <= 41; i++) {
        if (dateList.at(i).s) {
            undrawSelectedDate(i);
            dateList[i].s = false;
            drawToday(i);
        } // end if
    } // end for
}


///
/** */
void QmcDateNav::changeMonth(bool forward, int steps) {
    int cyn, cmn, cdn;
    cyn = cmn = cdn = 0;
    cmn = m_currentMonth.month();
    cyn = m_currentMonth.year();
    for (int i = 0; i <= 41; i++) {
        if (dateList.at(i).s)
            cdn = dateList.at(i).d.day();
    } // end for
    if (numSelected() > 1 || numSelected() < 1)
        cdn = 1;
    if (forward) {
        cmn += steps;
        if (cmn > 12) {
            cmn -= 12;
            cyn++;
        } // end if
    } else {
        cmn -= steps;
        if (cmn < 1) {
            cmn += 12;
            cyn--;
        } // end if
    } // end if

    QDate nd(cyn, cmn, fixDays(cyn, cmn, cdn));

    int py = m_currentMonth.year();
    m_currentMonth = QDate(nd);
    emit dateChanged();
    if (m_currentMonth.year() != py) {
        emit yearChanged(m_currentMonth.year());
    } // end if
    m_pmDirty = true;
    repaint();
}


///
/** */
QString QmcDateNav::monthName(int mn) {
    QString rv = QString::null;
    switch (mn) {
    case 1: {
            rv += tr("January");
            break;
        }
    case 2: {
            rv += tr("February");
            break;
        }
    case 3: {
            rv += tr("March");
            break;
        }
    case 4: {
            rv += tr("April");
            break;
        }
    case 5: {
            rv += tr("May");
            break;
        }
    case 6: {
            rv += tr("June");
            break;
        }
    case 7: {
            rv += tr("July");
            break;
        }
    case 8: {
            rv += tr("August");
            break;
        }
    case 9: {
            rv += tr("September");
            break;
        }
    case 10: {
            rv += tr("October");
            break;
        }
    case 11: {
            rv += tr("November");
            break;
        }
    case 12: {
            rv += tr("December");
            break;
        }
    } // end switch
    return rv;
}


///
/** */
int QmcDateNav::fixDays(int tmpYear, int tmpMonth, int d) {
    int tmpDay = d;

    switch (tmpMonth) {
    case 2: {
            if (tmpDay >= 29) {
                if (QDate::isLeapYear(tmpYear))
                    tmpDay = 29;
                else
                    tmpDay = 28;
            } // end if
            break;
        }
    case 4: {
            if (tmpDay == 31)
                tmpDay = 30;
            break;
        }
    case 6: {
            if (tmpDay == 31)
                tmpDay = 30;
            break;
        }
    case 9: {
            if (tmpDay == 31)
                tmpDay = 30;
            break;
        }
    case 11: {
            if (tmpDay == 31)
                tmpDay = 30;
            break;
        }
    } // end switch
    return tmpDay;
}


///
/** */
void QmcDateNav::prepRBPopup() {
    m_rbPopup->clear();
    m_rbPopup->addAction(tr("Go to Today"));
}


///
/** */
void QmcDateNav::prepPopup() {
    QDate backThree, backTwo, backOne, forwardOne, forwardTwo, forwardThree;

    m_sevenMonthPopup->clear();

    int newY = m_currentMonth.year();
    int newM = m_currentMonth.month();
    int newD = 15;

    newM -= 3;
    if (newM < 1) {
        newM += 12;
        newY--;
    } // end if
    backThree = QDate(newY, newM, newD);

    newM++;
    if (newM > 12) {
        newM -= 12;
        newY++;
    } // end if
    backTwo = QDate(newY, newM, newD);

    newM++;
    if (newM > 12) {
        newM -= 12;
        newY++;
    } // end if
    backOne = QDate(newY, newM, newD);

    newM += 2;
    if (newM > 12) {
        newM -= 12;
        newY++;
    } // end if
    forwardOne = QDate(newY, newM, newD);

    newM++;
    if (newM > 12) {
        newM -= 12;
        newY++;
    } // end if
    forwardTwo = QDate(newY, newM, newD);

    newM++;
    if (newM > 12) {
        newM -= 12;
        newY++;
    } // end if
    forwardThree = QDate(newY, newM, newD);

    m_sevenMonthPopup->addAction(QString(monthName(backThree.month()) + " " + QString::number(backThree.year(), 10)));
    m_sevenMonthPopup->addAction(QString(monthName(backTwo.month()) + " " + QString::number(backTwo.year(), 10)));
    m_sevenMonthPopup->addAction(QString(monthName(backOne.month()) + " " + QString::number(backOne.year(), 10)));
    m_sevenMonthPopup->addAction(QString(monthName(m_currentMonth.month()) + " " + QString::number(m_currentMonth.year(), 10)));
    m_sevenMonthPopup->addAction(QString(monthName(forwardOne.month()) + " " + QString::number(forwardOne.year(), 10)));
    m_sevenMonthPopup->addAction(QString(monthName(forwardTwo.month()) + " " + QString::number(forwardTwo.year(), 10)));
    m_sevenMonthPopup->addAction(QString(monthName(forwardThree.month()) + " " + QString::number(forwardThree.year(), 10)));
    m_sevenMonthPopup->setFixedWidth(captionRect.width());
}


/// Se ha seleccionado una opcion del menu de meses.
/** Este metodo se activa cuando se ha hecho una pulsacion sobre el menu de
    los meses. 
    @param id indica la opcion que se ha pulsado.*/
void QmcDateNav::monthMenuClicked(int id) {
    _depura("QmcDateNav::monthMenuClicked", 2);
    if (id < 4 && id > 0) {
        if( id == 1 )
            id = 3;
        else if (id == 3)
            id = 1;
        changeMonth(false, id);
    } else if (id > 4 && id < 8) {
        changeMonth(true, id - 4);
    } // end if
    _depura("END QmcDateNav::monthMenuClicked", 2);
}


/// Devuelve las fechas seleccionadas.
/** Este metodo devuelve todas las fechas seleccionadas en el calendario.*/
QList<QDate> QmcDateNav::selectedDates() const {
    _depura("", 0);
    QList<QDate> rl;
    rl.clear();
    for (int i = 0; i <= 41; i++)
        if (dateList.at(i).s)
            rl.append(QDate(dateList.at(i).d));
    return rl;
}


///
/** */
bool QmcDateNav::setDate(const QDate d) {
    if(!d.isValid()) {
        return false;
    } // end if
    int py = m_currentMonth.year();
    m_currentMonth = QDate(d);
    m_pmDirty = true;
    repaint();
    emit dateChanged();
    if (m_currentMonth.year() != py) {
        emit yearChanged(m_currentMonth.year());
    } // end if
    return true;
}


///
/** */
bool QmcDateNav::hasEvent(const QDate &d) {
    if (eventDayList == 0)
        return false;
    QDate ld;
    QListIterator<QDate> it(*nonWorkingDayList);
    it.toFront();
    for (ld = it.next(); it.hasNext(); ld = it.next()) {
        if (d.year() == ld.year() && d.month() == ld.month() && d.day() == ld.day())
            return true;
    } // end for
    return false;
}


/// Funci&oacute;n que sirve para saber si un d&iacute; del calendario es o no laborable.
/** TRUE = El d&iacute; es laborable.
    FALSE = El d&iacute; no el laborable. */
bool QmcDateNav::isNonWorkingDay(const QDate &d) {
    if (nonWorkingDayList == 0)
        return false;
    QDate ld;
    QListIterator<QDate> it(*nonWorkingDayList);
    it.toFront();
    for (ld = it.next(); it.hasNext(); ld = it.next()) {
        if (d.year() == ld.year() && d.month() == ld.month() && d.day() == ld.day())
            return true;
    } // end for
    return false;
}


///
/** */
bool QmcDateNav::outlook() const {
    return doOutlook;
}


///
/** */
void QmcDateNav::setOutlook(bool on) {
    if (on == doOutlook)
        return;
    doOutlook = on;
}


///
/** */
void QmcDateNav::installEventDayList(QList<QDate> *n) {
    eventDayList = n;
}


///
/** */
void QmcDateNav::removeEventDayList() {
    eventDayList = 0;
}


/// Instala la lista de d&iacute;s no laborables.
/** Marca en el calendario los d&iacute;s no laborables. */
void QmcDateNav::installNonWorkingDayList(QList<QDate> *n) {
    nonWorkingDayList = n;
}


/// Elimina la lista de d&iacute;s no laborables.
/** Quita del calendario la lista de d&iacute;s no laborables. */
void QmcDateNav::removeNonWorkingDayList() {
    nonWorkingDayList = 0;
}


/// Fuerza la actualizaci&oacute;n del calendario.
/** */
void QmcDateNav::forceUpdate() {
    m_pmDirty = true;
    QWidget::update();
}


///
/** */
/// Returns the index of the passed rect, or -1 on failure.
int QmcDateNav::findIndex(QRect & ir) {
    for (int i = 0; i <= 41; i++) {
        if (dateList.at( i ).r == ir)
            return i;
    }
    return -1;
}


///
/** */
/// Returns the index of the passed rect, or -1 on failure.
int QmcDateNav::findIndex(QDate & id) {
    for (int i = 0; i <= 41; i++) {
        if (dateList.at(i).d == id)
            return i;
    } // end for
    return -1;
}


///
/** */
/// Handles cursor changing for mouse move events.
/// TODO: Correct pointer cursors for this stuff.
/// Notes: Cursors are system specific see $QTDIR/src/kernel/qcursor_x11.cpp
/// I need to figure out how to get the system specific cursor, and
/// modify it for my use.
void QmcDateNav::mouseMoveEvent(QMouseEvent *e) {
    QRect mr(e->x(), e->y(), 1, 1);

    if (daysRect.contains(mr, true)) {
        setCursor(Qt::PointingHandCursor);
        return;
    } // end if

    QRect weeksRect(frame() ? 1 : 0,
                    dateList.at(0).r.top(),
                    dateList.at(0).r.left() - (frame() ? 1 : 0),
                    dateList.at(41).r.bottom() - dateList.at(0).r.top());

    if (weeksRect.contains(mr, true)) {
        setCursor(Qt::PointingHandCursor);
        return;
    } // end if
    unsetCursor();
}


///
/**  */
/// Callback for an item on the right-mouse buttton menu being clicked.
void QmcDateNav::rbMenuClicked(int id) {
    if (id == 1)
        setDate(QDate::currentDate());
}


/*
 * The QmcDateNav widget is visually similar to the calendar navigation
 * widgets found in many popular PIMs.
 *
 * Currently the widget is quite simple, with constructors to create a widget
 * with either the current date selected or a specific date selected, and
 * methods to retrieve a list of all dates selected or set the selected date.
 *
 * A signal is emitted whenever the selection changes so that you can retrieve
 * the selection list and your application can react to that data.
 *
 * Non-Working Days and Event Days.
 *
 * A PIM application generally displays days that have events or are non-working days (such as weekends) in either
 * a different font or colour.
 *
 * %QmcDateNav paints non-working days with an italic font face, and days containing event with a bold font face.
 *
 *
 * Event days and non-working days are two areas that will almost definitely cause headaches and grey hairs for an application developer.
 *
 * The grey area that tends to cause confusion is the ownership and maintenance of the lists of non-working and event days. %QmcDateNav
 * assigns this responsibility solely to the application writer, and provides hooks for passing the address of the list to the widget
 * for iteration.
 *
 * The widget will, under no circumstances, attempt to modify the lists in any way whatsoever.
 *
 * The lists may be any size, and contain any number of dates, although iteration performance will (clearly) suffer on large lists.<br>
 * To avoid unusually large lists, it is recommended that you use the yearChanged() and, to a lesser extent, the dateChanged() signals
 * to alert your application to the fact that the list needs updating.
 *
 * It's a good idea to force a widget repaint using the forceUpdate() method if you add or remove a date in your list that is
 * in the currently visible month.
 *
 * \warning Unexpected behaviour will result if the lists are deleted behind the widgets back.<br>
 * The correct way to go about removing a list is to use the supplied remove method before using delete on the list.
 *
 * See the test application source for the recommended usage of the list manipulation methods.
 *
 * \sa installEventDayList()
 * \sa removeEventDayList()
 * \sa installNonWorkingDayList()
 * \sa removeNonWorkingDayList()
 *
 * \author Michael van der Westhuizen (michael@mudpuppies.co.za).
 * \date $Date$
 */
/*!
 * \fn QmcDateNav::QmcDateNav(class QWidget * parent = 0)
 * \brief Default Constructor.
 *
 * This constructor creates a date navigation widget with todays date selected.
 *
 * The \c parent argument is passed to the %QWidget constructor.
 */
/*!
 * \fn QmcDateNav::QmcDateNav(const class QDate & d, class QWidget * parent = 0)
 * \brief Alternate Constructor.
 *
 * This constructor creates a date navigation widget with the date specified by \c d selected.
 *
 * If \c d is an invalid date, todays date is selected.
 *
 * The \c parent argument is passed to the %QWidget constructor.
 *
 * \param d The date on which to base this widget.
 * \sa setDate()
 */
/*!
 * \fn QmcDateNav::~QmcDateNav()
 * \brief Destructor
 *
 * Destroys the widget, freeing allocated memory and emptying lists.
 *
 */
/*!
 * \fn QSizePolicy QmcDateNav::sizePolicy() const
 *
 * \return A %QSizePolicy of QSizePolicy::Fixed by QSizePolicy::Fixed with no height-for-width capabilities.
 */
/*!
 * \fn QSize QmcDateNav::sizeHint() const
 *
 * \return minimumSizeHint().
 */
/*!
 * \fn QSize QmcDateNav::minimumSizeHint() const
 *
 * \return The amount of space this widget requires to be useful.
 */
/*!
 * \fn bool QmcDateNav::frame() const
 *
 * \return True if this widget is drawn with a frame, false otherwise.
 * \sa setFrame()
 */
/*!
 * \fn void QmcDateNav::setFrame(bool f)
 *
 * Toggles the drawing of a frame.
 *
 * \param f Set to true to enable the frame, false to disable.
 *
 * The default is to draw a frame.
 *
 * \sa frame()
 */
/*!
 * \fn QList<QDate> QmcDateNav::selectedDates() const
 *
 * \return A QList containing all of the selected dates in the navigator.
 *
 */
/*!
 * \fn void QmcDateNav::dateChanged()
 *
 * This signal is emitted whenever the selection changes.
 */
/*!
 * \fn bool QmcDateNav::outlook() const
 *
 * \return True if this widget is in Outlook mode, false otherwise.
 * \sa setOutlook()
 */
/*!
 * \fn void QmcDateNav::setOutlook( bool on )
 *
 * The QmcDateNav widget can paint itself in a way similar to Microsoft Outlook or
 * using highlight colours from the current %Qt style.
 *
 * The default is to use standard %Qt colours.
 *
 * \param on Pass true if you wish the widget to be painted in Outlook mode, false if you want standard %Qt colours.
 * \sa outlook()
 */
/*!
 * \fn bool QmcDateNav::setDate( const QDate d )
 *
 * If the new date is valid the date is changed to that date.
 *
 * \param d The date to change to.
 * \return True if the date is changed, false otherwise.
 */
/*!
 * \fn QmcDateNav::yearChanged( int )
 *
 * Emitted whenever the current year changes.
 */
/*!
 * \fn void QmcDateNav::paintEvent( QPaintEvent * )
 *
 * \reimp
 */
/*!
 * \fn void QmcDateNav::mousePressEvent( QMouseEvent * )
 *
 * \reimp
 */
/*!
 * \fn void QmcDateNav::installEventDayList( QList<QDate> *n )
 *
 * Assigns the address of the list \c n to be iterated over when searching for dates with events.
 *
 * \param n The new list address to assign to the internal event day list pointer.
 * \warning This is the one area you can really break your application. Always call removeEventDayList() before using delete on your list.<br>
 * If you don't, you may find the internal pointer pointing to deallocated memory and crashing your application.
 *
 * \sa removeEventDayList()
 */
/*!
 * \fn void QmcDateNav::removeEventDayList()
 *
 * Sets the internal event day list pointer to 0 and prevents any attempts at list iteration.
 *
 * Always call this before using the delete operator on your list.
 *
 * \sa installEventDayList()
 */
/*!
 * \fn void QmcDateNav::installNonWorkingDayList( QList<QDate> *n )
 *
 * Assigns the address of the list \c n to be iterated over when searching for dates that are non-working days.
 *
 * \param n The new list address to assign to the internal non-working day list pointer.
 * \warning This is the one area you can really break your application. Always call removeNonWorkingDayList() before using delete on your list.<br>
 * If you don't, you may find the internal pointer pointing to deallocated memory and crashing your application.
 *
 * \sa removeNonWorkingDayList()
 */
/*!
 * \fn void QmcDateNav::removeNonWorkingDayList()
 *
 * Sets the internal non-working day list pointer to 0 and prevents any attempts at list iteration.
 *
 * Always call this before using the delete operator on your list.
 *
 * \sa installNonWorkingDayList()
 */
/*!
 * \fn void QmcDateNav::forceUpdate()
 *
 * Ensures that the entire widget is repainted by setting the offscreen buffer state flag to dirty and calling QWidget::update().
 */
/*!
 * \fn void QmcDateNav::mouseMoveEvent( QMouseEvent * )
 *
 * \reimp
 */

