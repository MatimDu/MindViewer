#ifndef COM_H
#define COM_H

#include <QWidget>
#include <QString>
#include <QDateTime>
#include <QTextStream>
#include <QDir>
#include <QTimer>
#include <QMessageBox>
#include <QFile>
#include "qextserialport.h"
#include <iostream>

using namespace std;

#define nullptr NULL
#define TIME_OUT 10

namespace Ui {
class COM;
}

class COM : public QWidget
{
    Q_OBJECT
public:
    explicit COM(QWidget *parent = 0);
    ~COM();
    void initWidget();
    void write2file(QString str);//将所有数据写入文件待用
signals:
    void sendRawData(QString str);
        void sendFilteredData(bool valid,
                              unsigned char signal,
                              unsigned char delta,
                              unsigned char theta,
                              unsigned char lowalpha,
                              unsigned char highalpha,
                              unsigned char lowbeta,
                              unsigned char highbeta,
                              unsigned char lowgamma,
                              unsigned char midgamma,
                              unsigned char attention,
                              unsigned char meditation,
                              unsigned char rawdata,
                              unsigned char blink);
public slots:
        void readMyCom();//读取串口数据
        void internalTime();//间隔时间
private slots:
        void on_pushButtonOpen_clicked();

        void on_pushButtonClose_clicked();

private:
    Ui::COM *ui;
    QStringList ComList,braudList;
    QextSerialPort *myCom;
    QTimer *internalTimer;
};

#endif // COM_H
