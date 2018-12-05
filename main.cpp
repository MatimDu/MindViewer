#include "Widget.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    MainWindow w;
    w.setWindowIcon(QIcon(":/icons/mind.svg"));
    w.setWindowTitle(QObject::tr("MindViewer"));
    QFile file(":/configuration/style1.qss");
    file.open(QIODevice::Text|QIODevice::ReadOnly);
    w.setStyleSheet(file.readAll());
    w.setMinimumSize(600,400);
    w.resize(800,600);
    w.show();

    return a.exec();
}
