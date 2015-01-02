#include "qkgui.h"

#include <QDebug>
#include <QApplication>
#include <QStyleFactory>
#include <QPalette>

void QkGUI::qt_fusionDark()
{
    qApp->setStyle(QStyleFactory::create("Fusion"));

    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53,53,53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25,25,25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53,53,53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53,53,53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::white);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));

    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, QColor("#353535"));
    qApp->setPalette(darkPalette);


    QString customStyle;
    customStyle += "QToolTip { color: #ffffff; background: #2a82da; border: 1px solid white; }\n";
    customStyle += "QToolBar { border: 1px solid transparent; }\n";
    customStyle += "QMenuBar { background:#353535; color:white; }\n";
    customStyle += "QMenuBar::item {background:#353535;}\n";
    customStyle += "QMenuBar::item:selected {background:#1283de; color:white;}\n";
    customStyle += "QMenuBar::item:pressed {background:#1283de;}\n";
    customStyle += "QMenu {background:#353535; color:#eeeeee; padding:0px 4px;}\n";
    customStyle += "QMenu::item:selected {background:#1283de; color:#eeeeee;}\n";
    customStyle += "QPlainTextEdit {border:1px solid transparent}\n";

    qApp->setStyleSheet(customStyle);
}
