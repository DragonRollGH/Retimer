#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <windows.h>
#include <QMainWindow>
#include <QFileSystemModel>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QDateTime>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void dragEnterEvent(QDragEnterEvent *e);
    void dropEvent(QDropEvent *e);

private slots:
    void on_cvtBtn_clicked();

    void on_argBtn_clicked();

private:
    Ui::MainWindow *ui;
    QFileSystemModel *getMdl;
    QFileSystemModel *setMdl;
    QString getRP;  // Root Path
    QString setRP;
    // Get/Set Create/Modify/Access Time
    enum RetimeType{
        RT_ORIGIN,
        RT_GCT,
        RT_GMT,
        RT_GAT,
        RT_SCT,
        RT_SMT,
        RT_SAT,
        RT_NOW,
        RT_CUSTOM
    };
    void retime(QString g,QString s,int c=0,int m=0,int a=0);
};
#endif // MAINWINDOW_H
