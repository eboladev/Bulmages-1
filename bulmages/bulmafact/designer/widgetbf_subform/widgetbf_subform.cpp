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
