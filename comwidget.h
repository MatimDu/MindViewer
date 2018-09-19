#ifndef COMWIDGET_H
#define COMWIDGET_H

#include <QString>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QRadioButton>
#include <QComboBox>
#include <QMessageBox>
#include <iostream>
using namespace std;

#define nullptr NULL
#define TIME_OUT 10

class comWidget : public QWidget
{
    Q_OBJECT
public:
    explicit comWidget(QWidget *parent = nullptr);
    ~comWidget();
    void init();
    void write2file(QString str);//将所有数据写入文件待用
    QextSerialPort *myCom;
    QString write2fileName;
    QLabel *comLabel;
    QComboBox *comComboBox;
    QLabel *braudLabel;
    QComboBox *braudComboBox;
    QLabel *bitLabel;
    QComboBox *bitComboBox;
    QLabel *stopBitLabel;
    QComboBox *stopComboBox;
    QLabel *checkBitLabel;
    QComboBox *checkComboBox;
    QLabel *dataModulLabel;
    QLineEdit *dataModulLineEdit;
    QLabel *statusLabel;
    QLineEdit *statusLineEdit;
    QLabel *openButtonLabel;
    QPushButton *openButton;
    QLabel *closeButtonLabel;
    QPushButton *closeButton;
    QRadioButton *dataRadioButton;
    QRadioButton *graphicRadioButton;
    QPushButton *showButton;
    QTimer *internalTimer;
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
    void on_comButtonOpen_triggered();
    void on_comButtonClose_triggered();
    void readMyCom();//读取串口数据
    //void showButton_triggered();//根据不同的选择显示不同的结果
    void internalTime();//间隔时间
};

#endif // COMWIDGET_H
