#ifndef DATA_H
#define DATA_H
//数据库操作
#include <QDebug>
#include <QString>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlError>
#include <QSqlQuery>

class Data
{
public:
    Data(QString connectName="default");
    ~Data();
    int getMaxID(QString tableName);
    QStringList getLevel();
    QStringList getQuestionIDList();
    QString getQuestionDetail(int id);
    QStringList getLanguage();
public:
    QSqlQuery *query;
    QSqlTableModel *model;
private:
    QSqlDatabase db;
    QString sql;
};

#endif // DATA_H
