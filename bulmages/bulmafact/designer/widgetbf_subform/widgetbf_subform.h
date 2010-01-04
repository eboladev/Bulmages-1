#ifndef WIDGETBF_SUBFORM_H
#define WIDGETBF_SUBFORM_H

#include <QtDesigner>
#include <QtGui>
#include <QtPlugin>
#include <QDesignerCustomWidgetInterface>
#include <QList>
#include <QWidget>

#include "bfsubform.h"
#include "bfsubformtaskmenu.h"


class WidgetBf_SubForm : public QObject, public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)

protected:
    QList<QString> DbOptions;

public:
    WidgetBf_SubForm(QObject *parent = 0);

    bool isContainer() const;
    bool isInitialized() const;
    QIcon icon() const;
    QString domXml() const;
    QString group() const;
    QString includeFile() const;
    QString name() const;
    QString toolTip() const;
    QString whatsThis() const;
    QWidget *createWidget(QWidget *parent);
    void initialize(QDesignerFormEditorInterface *formEditor);

private:
    bool initialized;
};

#endif
