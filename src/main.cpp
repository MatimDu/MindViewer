#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    MainWindow w;
    w.setWindowIcon(QIcon(":/resources/icons/mind.svg"));
    w.setWindowTitle(QObject::tr("MindViewer"));
    w.setMinimumSize(600,400);
    w.resize(800,600);
    w.show();

    return a.exec();
}
