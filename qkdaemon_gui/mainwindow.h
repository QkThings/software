#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class QkConnectWorker;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void slotConnect();
    void slotAddConnection();
    void slotRemoveConnection();
    void slotReloadSerial();
    void slotMessage(int type, QString text);
    void slotShowHideMessages();
    void slotAbout();

    void setAvailableSerial(QStringList portNames);

private:
    Ui::MainWindow *ui;
    QkConnectWorker *_qkconnectWorker;
};

#endif // MAINWINDOW_H
