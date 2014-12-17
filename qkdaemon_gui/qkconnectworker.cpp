#include "qkconnectworker.h"
#include "qkdaemon_gui.h"
#include <QDebug>
#include <QProcess>

QkConnectWorker::QkConnectWorker(QObject *parent) :
    QObject(parent)
{
    _process = new QProcess(this);
    _process->setProcessChannelMode(QProcess::MergedChannels);
    _process->setProgram(QKCONNECT_EXE);

    connect(_process, SIGNAL(finished(int)),
            this, SLOT(_slotProcessFinished(int)));
    connect(_process, SIGNAL(error(QProcess::ProcessError)),
            this, SLOT(_slotProcessError(QProcess::ProcessError)));
}

void QkConnectWorker::version()
{
    _processWait();
    _currentTask = taskVersion;
    QStringList args;
    args << "--version";
    _process->setArguments(args);
    _process->start();

}

void QkConnectWorker::listSerial()
{
    _processWait();
    _currentTask = taskListSerial;
    QStringList args;
    args << "--list-serial";
    _process->setArguments(args);
    _process->start();
}

void QkConnectWorker::_slotProcessFinished(int status)
{
    switch(_currentTask)
    {
    case taskVersion:
        emit message(MESSAGE_INFO,
                     _process->readAll());
        break;
    case taskListSerial:
        QStringList portNames;
        while(1)
        {
            QString line = QString(_process->readLine());
            if(line.isEmpty()) break;
            if(line.at(0) != '#')
            {
                portNames << line.split(' ', QString::SkipEmptyParts).at(0);
            }
        }
        emit availableSerialPorts(portNames);
        break;
    }
}

void QkConnectWorker::_slotProcessError(QProcess::ProcessError error)
{
    qDebug() << __PRETTY_FUNCTION__ << _process->errorString();
}

void QkConnectWorker::_processWait()
{
    if(_process->isOpen())
        _process->waitForFinished();
}

