/***************************************************************************
 *   Copyright (C) 2010 by Fco. Javier M. C.                               *
 *   fcojavmc@todo-redes.com                                               *
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
 

#include <QtPlugin>

#include "widgetbl_searchwidget.h"


WidgetBl_SearchWidget::WidgetBl_SearchWidget(QObject *parent)
    : QObject(parent)
{
    initialized = false;
}

void WidgetBl_SearchWidget::initialize(QDesignerFormEditorInterface *formEditor)
{
    if (initialized)
        return;

    initialized = true;
}

bool WidgetBl_SearchWidget::isInitialized() const
{
    return initialized;
}

QWidget *WidgetBl_SearchWidget::createWidget(QWidget *parent)
{
    return new BlSearchWidget(parent);
}

QString WidgetBl_SearchWidget::name() const
{
    return "BlSearchWidget";
}

QString WidgetBl_SearchWidget::group() const
{
    return "Iglues/BulmaFact";
}

QIcon WidgetBl_SearchWidget::icon() const
{
    return QIcon();
}

QString WidgetBl_SearchWidget::toolTip() const
{
    return "";
}

QString WidgetBl_SearchWidget::whatsThis() const
{
    return "";
}

bool WidgetBl_SearchWidget::isContainer() const
{
    return false;
}

QString WidgetBl_SearchWidget::domXml() const
{

    QString xml =  "<ui language=\"c++\">\n"
           " <widget class=\"BlSearchWidget\" name=\"mui_search\">\n"
           "  <property name=\"geometry\">\n"
           "   <rect>\n"
           "    <x>0</x>\n"
           "    <y>0</y>\n"
           "    <width>220</width>\n"
           "    <height>70</height>\n"
           "   </rect>\n"
           "  </property>\n"
           "  <property name=\"toolTip\" >\n"
           "   <string>BlSearchWidget</string>\n"
           "  </property>\n"
           "  <property name=\"whatsThis\" >\n"
           "   <string>BlSearchWidget</string>\n"
           "  </property>\n"
           " </widget>\n"
           "</ui>\n";



    return xml;
	   
}

QString WidgetBl_SearchWidget::includeFile() const
{
    return "blsearchwidget.h";
}

Q_EXPORT_PLUGIN2(widgetbl_searchwidget, WidgetBl_SearchWidget)




BlDebug::BlDebug(const QString &, int, const QString &) {
}

void BlDebug::blDebug(const QString &text, int level, const QString &params) {
}

BlDebug::~BlDebug() {
}

