#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <windows.h>
#include <QMainWindow>
#include <QFileSystemModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_ConvertBtn_clicked();

private:
    Ui::MainWindow *ui;
    QFileSystemModel *getMdl;
    QFileSystemModel *setMdl;

    enum RetimeType{
        RT_ORIGIN,
        RT_CFT,
        RT_MFT,
        RT_AFT,
        RT_NOW
    };
    void retime(QString g,QString s,int c=0,int m=0,int a=0);
};
#endif // MAINWINDOW_H
