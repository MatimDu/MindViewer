#ifndef PARSER_HPP
#define PARSER_HPP
//头文件
#include <iostream>
#include <cmath>
#include <string>
#include <QString>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QRegExp>
#include <vector>

using namespace std;

struct packageData_{
    int delta;
    int theta;
    int lowAlpha;
    int highAlpha;
    int lowBeta;
    int highBeta;
    int middleGamma;
    int attention;//0-100
    int meditation;//0-100

    bool valid;//此包数据是否有效
};

class Parser{
public:
    Parser(){
        parserFinished = false;
        currentData = nullptr;
    }

    ~Parser(){}

    bool parserFile(QString filePath){
        QString result;
        if(filePath.isEmpty()){
            return false;
        }

        QFile file(filePath);
        if(file.open(QIODevice::ReadOnly|QIODevice::Text)){
            QTextStream readIn(&file);
            result = readIn.readAll();
        }
        else return false;
        parserData(result);
        file.close();

        return true;
    }

    bool parserPacket(QString packet){
        return true;
    }

    bool parserPacket(string packet){

    }

    bool parserData(QString data){
        if(data.isEmpty()){
            qDebug()<<"There is no data";
            return false;
        }
        int size = data.size();
        int i=0;
        while(i<size){
        }
        return true;
    }

    bool parserData(string data){
        return true;
    }

private:
    bool parserFinished;
    QString currentData;
    vector<packageData_> result;
};





#endif // PARSER_HPP
