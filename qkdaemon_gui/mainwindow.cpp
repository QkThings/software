#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qkdaemon_gui.h"
#include "qkconnectworker.h"
#include "aboutdialog.h"

#include <QDebug>
#include <QListWidget>
#include <QTime>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("QkDaemon");

#ifdef Q_OS_UNIX
    ui->textMessages->setFont(QFont("Monospace", 9));
#endif

    connect(ui->buttonSerialReload, SIGNAL(clicked()),
            this, SLOT(slotReloadSerial()));
    connect(ui->buttonAddConn, SIGNAL(clicked()),
            this, SLOT(slotAddConnection()));
    connect(ui->buttonMessages, SIGNAL(clicked()),
            this, SLOT(slotShowHideMessages()));
    connect(ui->buttonAbout, SIGNAL(clicked()),
            this, SLOT(slotAbout()));

    _qkconnectWorker = new QkConnectWorker(this);
    connect(_qkconnectWorker, SIGNAL(availableSerialPorts(QStringList)),
            this, SLOT(setAvailableSerial(QStringList)));
    connect(_qkconnectWorker, SIGNAL(message(int,QString)),
            this, SLOT(slotMessage(int,QString)));

    _qkconnectWorker->version();
    _qkconnectWorker->listSerial();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotConnect()
{

}

void MainWindow::slotAddConnection()
{
    QString connStr = QString().sprintf("  %-20s %-20s %s",
                                        ui->comboConnType->currentText().toStdString().c_str(),
                                        ui->comboSerialPort->currentText().toStdString().c_str(),
                                        ui->comboSerialBaud->currentText().toStdString().c_str());

    ui->listConn->addItem(connStr);
}

void MainWindow::slotRemoveConnection()
{

}

void MainWindow::slotMessage(int type, QString text)
{
    QString msg;
    msg += "(" + QTime::currentTime().toString("hh:mm:ss") + ") ";

    switch(type)
    {
    case MESSAGE_INFO: msg += "[i] "; break;
    case MESSAGE_ERROR: msg += "[e] "; break;
    }

    msg += text;

    ui->textMessages->append(msg);
}

void MainWindow::slotShowHideMessages()
{
    ui->textMessages->setVisible(!ui->textMessages->isVisible());
}

void MainWindow::slotReloadSerial()
{
    _qkconnectWorker->listSerial();
}

void MainWindow::setAvailableSerial(QStringList portNames)
{
    ui->comboSerialPort->clear();
    ui->comboSerialPort->addItems(portNames);
}


void MainWindow::slotAbout()
{
    AboutDialog aboutDialog;
    aboutDialog.exec();
}
