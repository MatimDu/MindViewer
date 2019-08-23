#ifndef CURVEBASE_HPP
#define CURVEBASE_HPP

#include <QObject>
#include <QWidget>
#include <QThread>

class CurveBase : public QThread ,public QWidget {
public:
    CurveBase():
    ui(new QWidget),
    bPthread(new QThread){}

    ~CurveBase(){}

    virtual void parseData() = 0;



private:
    QWidget *ui;
    QThread *bPthread;
};

#endif // CURVEBASE_HPP
