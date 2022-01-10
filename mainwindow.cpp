#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    getMdl = new QFileSystemModel(this);
    getMdl->setRootPath(QDir::currentPath());
    ui->getTree->setModel(getMdl);
    ui->getTree->setRootIndex(getMdl->index(QDir::currentPath()));
    setMdl = new QFileSystemModel(this);
    setMdl->setRootPath(QDir::currentPath());
    ui->setTree->setModel(setMdl);
    ui->setTree->setRootIndex(setMdl->index(QDir::currentPath()));


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_ConvertBtn_clicked()
{
    QModelIndex gI = ui->getTree->currentIndex();
    QModelIndex sI = ui->setTree->currentIndex();
    QString gF = getMdl->filePath(gI);
    QString sF = setMdl->filePath(sI);
    retime(gF,sF,RT_CFT);
}

void MainWindow::retime(QString g, QString s, int c, int m, int a)
{
    if (!c && !m && !a)
    {
        return;
    }
    HANDLE gHDL = CreateFileA((LPCSTR)g.toLocal8Bit(),GENERIC_READ,0,NULL,OPEN_EXISTING,0,0);
    HANDLE sHDL = CreateFileA((LPCSTR)s.toLocal8Bit(),GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,0);
    if (gHDL == INVALID_HANDLE_VALUE || sHDL == INVALID_HANDLE_VALUE)
    {
        CloseHandle(gHDL);
        CloseHandle(sHDL);
        return;
    }
    FILETIME cFT,mFT,aFT;
    GetFileTime(gHDL,&cFT,&mFT,&aFT);
    switch (c) {
    case RT_CFT:
        SetFileTime(sHDL,&cFT,0,0);
        break;
    case RT_MFT:
        SetFileTime(sHDL,&mFT,0,0);
        break;
    case RT_AFT:
        SetFileTime(sHDL,&aFT,0,0);
        break;
    default:
        break;
    }
    switch (m) {
    case RT_CFT:
        SetFileTime(sHDL,0,&cFT,0);
        break;
    case RT_MFT:
        SetFileTime(sHDL,0,&mFT,0);
        break;
    case RT_AFT:
        SetFileTime(sHDL,0,&aFT,0);
        break;
    default:
        break;
    }
    switch (a) {
    case RT_CFT:
        SetFileTime(sHDL,0,0,&cFT);
        break;
    case RT_MFT:
        SetFileTime(sHDL,0,0,&mFT);
        break;
    case RT_AFT:
        SetFileTime(sHDL,0,0,&aFT);
        break;
    default:
        break;
    }
    CloseHandle(gHDL);
    CloseHandle(sHDL);
}
