#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    getMdl = new QFileSystemModel(this);
//    getMdl->setRootPath(QDir::currentPath());
//    ui->getTree->setModel(getMdl);
//    ui->getTree->setRootIndex(getMdl->index(QDir::currentPath()));
    setMdl = new QFileSystemModel(this);
//    setMdl->setRootPath(QDir::currentPath());
//    ui->setTree->setModel(setMdl);
//    ui->setTree->setRootIndex(setMdl->index(QDir::currentPath()));

    this->setAcceptDrops(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::dragEnterEvent(QDragEnterEvent *e)
{
    e->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *e)
{
    QString dF = e->mimeData()->urls().first().toLocalFile();
    if (e->pos().x() < this->width()/2)
    {
        getRP = dF;
        getMdl->setRootPath(getRP);
        ui->getTree->setModel(getMdl);
        ui->getTree->setRootIndex(getMdl->index(getRP));
    }
    else
    {
        setRP = dF;
        setMdl->setRootPath(setRP);
        ui->setTree->setModel(setMdl);
        ui->setTree->setRootIndex(setMdl->index(setRP));
    }
}

void MainWindow::retime(QString g, QString s, int c, int m, int a)
{
    if (!c && !m && !a)
    {
        return;
    }
    HANDLE gHDL = CreateFileA((LPCSTR)g.toLocal8Bit(),\
                              GENERIC_READ,0,NULL,OPEN_EXISTING,0,0);
    HANDLE sHDL = CreateFileA((LPCSTR)s.toLocal8Bit(),\
                              GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,0);
    if (gHDL == INVALID_HANDLE_VALUE || sHDL == INVALID_HANDLE_VALUE)
    {
        CloseHandle(gHDL);
        CloseHandle(sHDL);
        return;
    }
    FILETIME gCT,gMT,gAT,sCT,sMT,sAT;
    GetFileTime(gHDL,&gCT,&gAT,&gMT);
    GetFileTime(sHDL,&sCT,&sAT,&sMT);
    switch (c) {
    case RT_GCT:
        SetFileTime(sHDL,&gCT,0,0);
        break;
    case RT_GMT:
        SetFileTime(sHDL,&gMT,0,0);
        break;
    case RT_GAT:
        SetFileTime(sHDL,&gAT,0,0);
        break;
    case RT_SCT:
        SetFileTime(sHDL,&sCT,0,0);
        break;
    case RT_SMT:
        SetFileTime(sHDL,&sMT,0,0);
        break;
    case RT_SAT:
        SetFileTime(sHDL,&sAT,0,0);
        break;
    default:
        break;
    }
    switch (a) {
    case RT_GCT:
        SetFileTime(sHDL,0,&gCT,0);
        break;
    case RT_GMT:
        SetFileTime(sHDL,0,&gMT,0);
        break;
    case RT_GAT:
        SetFileTime(sHDL,0,&gAT,0);
        break;
    case RT_SCT:
        SetFileTime(sHDL,0,&sCT,0);
        break;
    case RT_SMT:
        SetFileTime(sHDL,0,&sMT,0);
        break;
    case RT_SAT:
        SetFileTime(sHDL,0,&sAT,0);
        break;
    default:
        break;
    }
    switch (m) {
    case RT_GCT:
        SetFileTime(sHDL,0,0,&gCT);
        break;
    case RT_GMT:
        SetFileTime(sHDL,0,0,&gMT);
        break;
    case RT_GAT:
        SetFileTime(sHDL,0,0,&gAT);
        break;
    case RT_SCT:
        SetFileTime(sHDL,0,0,&sCT);
        break;
    case RT_SMT:
        SetFileTime(sHDL,0,0,&sMT);
        break;
    case RT_SAT:
        SetFileTime(sHDL,0,0,&sAT);
        break;
    default:
        break;
    }
    CloseHandle(gHDL);
    CloseHandle(sHDL);
}


void MainWindow::on_ConvertBtn_clicked()
{
    QDirIterator sItr(setRP,QDir::Files|QDir::NoDotAndDotDot);
    while(sItr.hasNext())
    {
        sItr.next();
        QString sF = sItr.filePath();
        QString gF = getRP+'/'+sItr.fileName();
        if (QFile::exists(gF))
        {
            qDebug() << gF << sF;
            retime(gF,sF,RT_GCT,RT_SCT);
        }
    }
}
