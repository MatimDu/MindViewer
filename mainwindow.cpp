#include "mainwindow.h"
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    updateTimer = new QTimer;
    createActions();//创建动作
    createMenu();//创建菜单
    connect(updateTimer,SIGNAL(timeout()),this,SLOT(updateTime()));
    updateTimer->start(1000);
}

MainWindow::~MainWindow()
{
    delete actionOpen;
    delete actionClose;
    delete actionSave;
    delete actionSaveAs;
    delete actionPrint;
    delete actionExit;
    delete actionUndo;
    delete actionRedo;
    delete actionCopy;
    delete actionPaste;
    delete actionCut;
    delete actionSelectAll;
    delete actionPreferrence;
    delete actionAbout;
    delete actionAboutQt;
    delete menuFile;
    delete menuEdit;
    delete menuHelp;
}

void MainWindow::createActions()
{
    actionOpen = new QAction(QIcon(":/images/open"),tr("打开"),this);
    connect(actionOpen,SIGNAL(triggered(bool)),this,SLOT(OpenFile()));
    actionClose = new QAction(QIcon(":/images/close"),tr("关闭"),this);
    actionSave = new QAction(QIcon(":/images/save"),tr("保存"),this);
    actionSaveAs = new QAction(QIcon(":/images/send"),tr("另存为"),this);
    actionPrint = new QAction(QIcon(":/images/print"),tr("打印"),this);
    actionExit = new QAction(QIcon(":/images/exit"),tr("退出"),this);
    connect(actionExit,SIGNAL(triggered(bool)),this,SLOT(close()));

    actionUndo = new QAction(QIcon(":/images/undo"),tr("撤销"),this);
    actionRedo = new QAction(QIcon(":/images/redo"),tr("恢复"),this);
    actionCopy = new QAction(QIcon(":/images/copy"),tr("复制"),this);
    actionPaste = new QAction(QIcon(":/images/clipboard"),tr("粘贴"),this);
    actionCut = new QAction(QIcon(":/images/cut"),tr("剪切"),this);
    actionSelectAll = new QAction(QIcon(":/images/done-all"),tr("全选"),this);
    actionPreferrence = new QAction(QIcon(":/images/settings"),tr("偏好设置"),this);

    actionAbout = new QAction(QIcon(":/images/info"),tr("关于"),this);
    actionAboutQt = new QAction(QIcon(":/images/qt"),tr("Qt"),this);
    connect(actionAboutQt,SIGNAL(triggered(bool)),this,SLOT(aboutQt()));
}

void MainWindow::createMenu()
{
    menuFile = menuBar()->addMenu(tr("文件(&F)"));
    menuFile->addAction(actionOpen);
    menuFile->addAction(actionClose);
    menuFile->addSeparator();
    menuFile->addAction(actionSave);
    menuFile->addAction(actionSaveAs);
    menuFile->addSeparator();
    menuFile->addAction(actionPrint);
    menuFile->addSeparator();
    menuFile->addAction(actionExit);

    menuEdit = menuBar()->addMenu(tr("编辑(&E)"));
    menuEdit->addAction(actionUndo);
    menuEdit->addAction(actionRedo);
    menuEdit->addSeparator();
    menuEdit->addAction(actionCopy);
    menuEdit->addAction(actionPaste);
    menuEdit->addAction(actionCut);
    menuEdit->addAction(actionSelectAll);
    menuEdit->addSeparator();
    menuEdit->addAction(actionPreferrence);

    menuHelp = menuBar()->addMenu(tr("帮助(&H)"));
    menuHelp->addAction(actionAbout);
    menuHelp->addAction(actionAboutQt);
}

void MainWindow::updateTime()
{
    QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
    this->statusBar()->showMessage(currentTime);
}

void MainWindow::OpenFile()
{
    QString openFilePath = QFileDialog::getOpenFileName(this,tr("打开"),QString(),"*");
    if(openFilePath.isNull()) return;
    QFile openFile(openFilePath);
    if(openFile.open(QIODevice::ReadOnly|QIODevice::Text)){

    }
    openFile.close();
}

void MainWindow::aboutQt()
{
    qApp->aboutQt();
}
