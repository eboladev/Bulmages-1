/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the Graphics Dojo project on Qt Labs.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 or 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.fsf.org/licensing/licenses/info/GPLv2.html and
** http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

BarChart.prototype = new QWidget;

BarChart.prototype.suffix = "";

function BarChart(parent) {
    QWidget.call(this, parent);
}

// find the maximum value and widest (pixel-wise) label and suffix
BarChart.prototype.showEvent = function(event) {
    var fm = new QFontMetrics(this);

    this.margin = 20;
    this.titleHeight = fm.height();
    this.barHeight = 1.5 * fm.height();
    this.barSpacing = 0.6 * fm.height();

    this.maxValue = this.suffixWidth = this.labelWidth = 0;
    var count = 0;
    for (d in this.data) {
        this.labelWidth = Math.max(this.labelWidth, fm.width(d));
        this.maxValue = Math.max(this.maxValue, this.data[d]);
        this.suffixWidth = Math.max(this.suffixWidth, fm.width(String(this.data[d]) + " " + this.suffix));
        count++;
    }

    this.startHue = 15;
    this.hueDelta = 360 / count;

    this.size = new QSize(640, this.titleHeight + 2 * this.margin + (this.barHeight + this.barSpacing) * count);
}

BarChart.prototype.paintEvent = function(event) {


    var p = new QPainter;
    p.begin(this);

    // background and title
    p.fillRect(this.rect, new QBrush(new QColor(255, 255, 255)));
    p.drawText(0, 0, this.width, this.margin + this.titleHeight, Qt.AlignCenter, this.windowTitle, 0);

    var ofs = this.labelWidth + this.margin;
    var ww = this.width - this.suffixWidth - ofs - 2 * this.margin;
    var hue = this.startHue;
    var y = 0;

    p.translate(this.margin, this.titleHeight + 1.5 * this.margin);

    for (d in this.data) {

        // label on the left side
        p.setPen(new QColor(Qt.black));
        p.drawText(0, y, this.labelWidth, this.barHeight, Qt.AlignVCenter + Qt.AlignRight, d, 0);

        // the colored bar
        var gradient = new QLinearGradient(new QPoint(ofs, y), new QPoint(ofs, y + this.barHeight));
        gradient.setColorAt(0, QColor.fromHsv(hue, 255, 240));
        gradient.setColorAt(1, QColor.fromHsv(hue, 255, 92));
        p.setBrush(new QBrush(gradient));
        p.setPen(new QColor(96, 96, 96));
        var bw = this.data[d] * ww / this.maxValue;
        p.drawRect(ofs, y, bw, 
        this.barHeight);

        // extra text at the end of the bar
        var text = new String(this.data[d] + " " + this.suffix);
        p.setPen(new QColor(Qt.black));
        p.drawText(ofs + bw + this.margin/2, y, this.suffixWidth, this.barHeight, Qt.AlignVCenter + Qt.AlignLeft, text, 0);

        // for the next bar
        y += (this.barHeight + this.barSpacing);
        hue += this.hueDelta;
        if (hue >= 360)
            hue -= 360;
    }
    p.end();
}

BarChart.prototype.wheelEvent = function(event) {
    this.startHue += event.delta() / 8 / 5;
    if (this.startHue >= 360)
        this.startHue -= 360;
    if (this.startHue < 0)
        this.startHue += 360;
    this.update();
    event.ignore();
}

BarChart.prototype.mousePressEvent = function(event) {
    var fname = QFileDialog.getSaveFileName(this, "Save", ".", "*.png", 0, 0);
    if (fname.length > 0) {
        var img = new QImage(this.size, QImage.Format_ARGB32_Premultiplied);
        this.render(img);
        img.save(new QFile(fname));
    }
}

/*
chart.show();

QCoreApplication.exec();
*/



var tab = ArticuloView.findChild("mui_tab");

if ( tab != null) {
  
var chart = new BarChart;

chart.windowTitle = "Demo de QtScript";

chart.suffix = "runs";

chart.data = { 
          "WebKit 3.0" :  5.4, 
          "WebKit 3.1" : 18.8,
        "SquirrelFish" : 29.9,
"SquirrelFish Extreme" : 63.6
};
    tab.insertTab(3, chart, "Demo de QtScript");

//    var hgf= new QWidget();
//    tab.insertTab(3,hgf,"Pruebas de SCRIPT");
} // end if


var lay = ArticuloView.findChild("mui_tab");
//if(lay)
//    lay.setTabPosition(ArticuloView.East);
//    print lay.tabPosition();
if (lay != null) {
//      print (lay.tabPosition);
      lay.tabPosition = QTabWidget.East;
//      lay.tabsClosable = true;
//      lay.setTabPosition(ArticuloView.West);
//      print (lay.tabPosition);
}


//function hola () {
//variable = new QWidget();
//variable.show();
//ActividadView.mui_guardar.setEnabled(false);
//var a = ActividadView.mui_codigoactividad;
//a.hide();
//ActividadView.mui_codigoactividad.hide();
//ActividadView.mui_guardar.hide();
//}


//ActividadView.pintaMenu.connect(hola);

//ActividadView.mui_codigoactividad.setEnabled(false);

//ActividadView.clicked.connect(mui_guardar,hola);

//var saveButton = ActividadView.findChild("mui_guardar");
//if (saveButton != null) {
//    saveButton.setEnabled(false);
//    saveButton.clicked.connect(hola);
//    saveButton.hide();
//} // end if

//BlForm.valueChanged.connect(hola);

//print "hola";


//var a = variable.objectName;

//variable.setObjectName("hola");

//print variable.objectName;

//print a;


// BlForm.showMaximized();

//BlForm.setEnabled(false);

//var hola = BlForm.objectName();


// BlForm.setModoConsulta();

//var a =  BlForm.tableName();



// variable = QObject();

//variable.setParent(0);

//variable.showMaximized();


//print BlForm.enabled();

//a = new QDialog();

//BlForm.pintaMenu.connect(hola);




//a.exec();