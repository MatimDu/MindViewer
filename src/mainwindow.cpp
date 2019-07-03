#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loadSkin(1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadSkin(int i)
{
    QFile file(QString(":/resources/skin/style%1.qss").arg(i));
    file.open(QIODevice::Text|QIODevice::ReadOnly);
    this->setStyleSheet(file.readAll());
    file.close();
}

void MainWindow::on_actionExit_triggered()
{
    qApp->exit(0);
}

void MainWindow::on_actionQt_triggered()
{
    qApp->aboutQt();
}

void MainWindow::on_actionAbout_triggered()
{
    QString msg = QString("<h1>MindViewer</h1>"
                          "<h2>TGAM module tools</h2>"
                          "<h3>Author: JackeyLea</h3>"
                          "<h3>E-mail: 1768478912@qq.com</h3>"
                          "<h3>Phone: 13812991101</h3>"
                          "<h3>GitHub: https://github.com/JackeyLea/MindViewer</h3>"
                          "<h3>Gitee: https://gitee.com/JackeyLea/MindViewer</h3>"
                          "<h4>Do what you want but please obey the LGPL3 rules</h4>"
                          "<h4>And keep those message within application</h4>");
    QMessageBox::information(this,tr("About"),msg,QMessageBox::Ok);
}

void MainWindow::on_actionGitHub_triggered()
{
    QDesktopServices::openUrl(QUrl("https://github.com/JackeyLea/MindViewer"));
}

void MainWindow::on_actionBlack_triggered()
{
    loadSkin(2);
}

void MainWindow::on_actionLighter_triggered()
{
    loadSkin(1);
}

void MainWindow::on_actionGray_triggered()
{
    loadSkin(3);
}
