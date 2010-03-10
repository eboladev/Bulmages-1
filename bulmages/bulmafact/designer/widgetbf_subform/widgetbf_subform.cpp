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

#include "widgetbf_subform.h"


WidgetBf_SubForm::WidgetBf_SubForm(QObject *parent)
    : QObject(parent)
{
    initialized = false;

    // Cargamos la lista con valores.
    DbOptions << "DbNone" << "DbReadOnly" << "DbHideView" << "DbNoWrite" << "DbDisableView";
}

void WidgetBf_SubForm::initialize(QDesignerFormEditorInterface *formEditor)
{
    if (initialized)
        return;

    QExtensionManager *manager = formEditor->extensionManager();
    Q_ASSERT(manager != 0);

    manager->registerExtensions(new BfSubFormTaskMenuFactory(manager), Q_TYPEID(QDesignerTaskMenuExtension));

    initialized = true;
}

bool WidgetBf_SubForm::isInitialized() const
{
    return initialized;
}

QWidget *WidgetBf_SubForm::createWidget(QWidget *parent)
{
    return new BfSubForm(parent);
}

QString WidgetBf_SubForm::name() const
{
    return "BfSubForm";
}

QString WidgetBf_SubForm::group() const
{
    return "Iglues/BulmaFact";
}

QIcon WidgetBf_SubForm::icon() const
{
    return QIcon();
}

QString WidgetBf_SubForm::toolTip() const
{
    return "";
}

QString WidgetBf_SubForm::whatsThis() const
{
    return "";
}

bool WidgetBf_SubForm::isContainer() const
{
    return false;
}

QString WidgetBf_SubForm::domXml() const
{

    QString xml =  "<ui language=\"c++\">\n"
           " <widget class=\"BfSubForm\" name=\"mui_list\">\n"
           "  <property name=\"geometry\">\n"
           "   <rect>\n"
           "    <x>0</x>\n"
           "    <y>0</y>\n"
           "    <width>100</width>\n"
           "    <height>100</height>\n"
           "   </rect>\n"
           "  </property>\n"
           "  <property name=\"toolTip\" >\n"
           "   <string>BfSubForm</string>\n"
           "  </property>\n"
           "  <property name=\"whatsThis\" >\n"
           "   <string>BfSubForm</string>\n"
           "  </property>\n"
           "  <property name=\"setDbTableName\" >\n"
           "   <string></string>\n"
           "  </property>\n"
           "  <property name=\"setDbFieldId\" >\n"
           "   <string></string>\n"
           "  </property>\n"
           "  <property name=\"setInsert\" >\n"
           "   <bool></bool>\n"
           "  </property>\n"
           "  <property name=\"setDelete\" >\n"
           "   <bool></bool>\n"
           "  </property>\n";
/*
    for (int i = 0; i < DbOptions.size(); ++i) {

           xml += "  <property name=\"" + DbOptions.at(i) + "\" >\n" +
	       "   <string></string>\n" +
    	       "  </property>\n";

    } // end for
*/
           xml += " </widget>\n"
           "</ui>\n";



    return xml;
	   
}

QString WidgetBf_SubForm::includeFile() const
{
    return "bfsubform.h";
}

Q_EXPORT_PLUGIN2(widgetbf_subform, WidgetBf_SubForm)

