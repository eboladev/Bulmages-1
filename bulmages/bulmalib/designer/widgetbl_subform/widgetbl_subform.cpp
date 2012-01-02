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

#include "widgetbl_subform.h"


WidgetBl_SubForm::WidgetBl_SubForm(QObject *parent)
    : QObject(parent)
{
    initialized = false;
}

void WidgetBl_SubForm::initialize(QDesignerFormEditorInterface *formEditor)
{
    if (initialized)
        return;

    initialized = true;
}

bool WidgetBl_SubForm::isInitialized() const
{
    return initialized;
}

QWidget *WidgetBl_SubForm::createWidget(QWidget *parent)
{
    return new BlSubForm(parent);
}

QString WidgetBl_SubForm::name() const
{
    return "BlSubForm";
}

QString WidgetBl_SubForm::group() const
{
    return "Iglues/BulmaFact";
}

QIcon WidgetBl_SubForm::icon() const
{
    return QIcon();
}

QString WidgetBl_SubForm::toolTip() const
{
    return "";
}

QString WidgetBl_SubForm::whatsThis() const
{
    return "";
}

bool WidgetBl_SubForm::isContainer() const
{
    return false;
}

QString WidgetBl_SubForm::domXml() const
{

    QString xml =  "<ui language=\"c++\">\n"
           " <widget class=\"BlSubForm\" name=\"mui_subform\">\n"
           "  <property name=\"geometry\">\n"
           "   <rect>\n"
           "    <x>0</x>\n"
           "    <y>0</y>\n"
           "    <width>140</width>\n"
           "    <height>30</height>\n"
           "   </rect>\n"
           "  </property>\n"
           "  <property name=\"toolTip\" >\n"
           "   <string>BlSubForm</string>\n"
           "  </property>\n"
           "  <property name=\"whatsThis\" >\n"
           "   <string>BlSubForm</string>\n"
           "  </property>\n"
           " </widget>\n"
           "</ui>\n";



    return xml;
	   
}

QString WidgetBl_SubForm::includeFile() const
{
    return "blsubform.h";
}

Q_EXPORT_PLUGIN2(widgetbl_subform, WidgetBl_SubForm)


BlDebug::BlDebug(const QString &, int, const QString &) {
}

void BlDebug::blDebug(const QString &text, int level, const QString &params) {
}

BlDebug::~BlDebug() {
}

