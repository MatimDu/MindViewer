#include "Data.h"

Data::Data(QString connectName)
{
    db = QSqlDatabase::addDatabase("QSQLITE",connectName);
    db.setDatabaseName("/home/jackey/Development/BlackWidow/resources/blackwidow.db");
    if(!db.open()){
        qDebug()<<"Cannot open database file: "<<db.lastError();
    }
    else{
        qDebug()<<"Connect to database successfully";
    }
    query = new QSqlQuery(db);
}

Data::~Data()
{
    db.close();
    delete query;
}

int Data::getMaxID(QString tableName)
{
    int maxId;
    sql = "select max(id) from "+tableName;
    query->prepare(sql);
    if(query->exec()){
        while(query->next()){
            maxId = query->value(0).toInt();
        }
    }
    sql.clear();
    return maxId;
}

QStringList Data::getLevel()
{
    QStringList level;
    query->prepare("select value from level");
    if(query->exec()){
        while(query->next()){
            level += query->value(0).toString();
        }
    }
    return level;
}

QString Data::getQuestionDetail(int id)
{
    QSqlRecord record;
    QString detail = QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                             "\n<detail>"
                             "\n<question>");
    query->prepare(QString("select * from subject where id = %1").arg(id));
    if(query->exec()){
        while(query->next()){
            record = query->record();
            detail += QString("<id>%1</id>\n").arg(record.field(0).value().toString());
            detail += QString("<title>%2</title>\n").arg(record.field(1).value().toString());
            detail += QString("<detail>%3</detail>\n").arg(record.field(2).value().toString());
            detail += QString("<option>%4</option>\n").arg(record.field(3).value().toString());
            detail += QString("<answer>%5</answer>\n").arg(record.field(4).value().toString());
            detail += QString("<tip>%6</tip>\n").arg(record.field(5).value().toString());
        }
    }
    detail +=QString("</question>\n<property>\n");
    query->prepare(QString("select * from property where id = %1").arg(id));
    if(query->exec()){
        while(query->next()){
            record = query->record();
            detail += QString("<type>%2</type>\n").arg(record.field(1).value().toString());
            detail += QString("<level>%3</level>\n").arg(record.field(2).value().toString());
            detail += QString("<source>%4</source>\n").arg(record.field(3).value().toString());
            detail += QString("<cata>%5</cata>\n").arg(record.field(4).value().toString());
        }
    }
    detail += QString("</property>\n</detail>");
    return detail;
}

QStringList Data::getLanguage()
{
    QStringList languageList;
    query->prepare("select value from language");
    if(!query->exec()){
        qDebug()<<"Query data wrong: "<<query->lastError();
    }
    else{
        while(query->next()){
            languageList += query->value(0).toString();
        }
    }
    return languageList;
}
