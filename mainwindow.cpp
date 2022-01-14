#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Retimer");

    ui->cCbb->setCurrentIndex(RT_GCT);
    ui->mCbb->setCurrentIndex(RT_GMT);
    ui->aCbb->setCurrentIndex(RT_NOW);

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
    QString dF = e->mimeData()->urls().first().toLocalFile();   // drop file path
    if (e->pos().x() < this->width()/2)
    {
        getRP = dF; // Root Path
        getMdl->setRootPath(getRP);
        ui->getTree->setModel(getMdl);
        ui->getTree->setRootIndex(getMdl->index(getRP));
        ui->getTree->setColumnHidden(2,true);
        ui->getTree->setColumnWidth(0,250);
        ui->getLbl->setText(getRP);
    }
    else
    {
        setRP = dF; // Root Path
        setMdl->setRootPath(setRP);
        ui->setTree->setModel(setMdl);
        ui->setTree->setRootIndex(setMdl->index(setRP));
        ui->setTree->setColumnHidden(2,true);
        ui->setTree->setColumnWidth(0,250);
        ui->setLbl->setText(setRP);
    }
}

// Get/Set file path, Create/Modify/Access time retime type
void MainWindow::retime(QString g, QString s, int c, int m, int a)
{
    if (!c && !m && !a)
    {
        return;
    }
    // Get/Set file HanDLe
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
    // Get/Set/Now Create/Modify/Access/File Time
    FILETIME gCT,gMT,gAT,sCT,sMT,sAT,nFT;
    // Now System Time
    SYSTEMTIME nST;
    GetFileTime(gHDL,&gCT,&gAT,&gMT);
    GetFileTime(sHDL,&sCT,&sAT,&sMT);
    GetSystemTime(&nST);
    SystemTimeToFileTime(&nST,&nFT);
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
    case RT_NOW:
        SetFileTime(sHDL,&nFT,0,0);
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
    case RT_NOW:
        SetFileTime(sHDL,0,&nFT,0);
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
    case RT_NOW:
        SetFileTime(sHDL,0,0,&nFT);
        break;
    default:
        break;
    }
    CloseHandle(gHDL);
    CloseHandle(sHDL);
}

void MainWindow::on_cvtBtn_clicked()
{
    // Create/Modify/Access Retime Type
    int cRT = ui->cCbb->currentIndex();
    int mRT = ui->mCbb->currentIndex();
    int aRT = ui->aCbb->currentIndex();
    QDirIterator sItr(setRP,QDir::Files|QDir::NoDotAndDotDot);
    while(sItr.hasNext())
    {
        sItr.next();
        // Set/Get File path
        QString sF = sItr.filePath();
        QString gF = getRP+'/'+sItr.fileName();
        if (QFile::exists(gF))
        {
            retime(gF,sF,cRT,mRT,aRT);
        }
    }
    setMdl->setRootPath(QDir::currentPath());
    setMdl->setRootPath(setRP);
}

void MainWindow::on_argBtn_clicked()
{
    QDirIterator sItr(setRP,QDir::Files|QDir::NoDotAndDotDot);
    while(sItr.hasNext())
    {
        sItr.next();
        // Create DateTime
        QDateTime cDT = sItr.fileInfo().birthTime();
        // New file Path
        QString nP = setRP+'/'+cDT.toString("yyMMd");
        if (!QDir(nP).exists())
        {
            QDir().mkdir(nP);
        }
        QFile::rename(sItr.filePath(),nP+'/'+sItr.fileName());
    }
}

