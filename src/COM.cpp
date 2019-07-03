#include "COM.h"
#include "ui_COM.h"

COM::COM(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::COM)
{
    ui->setupUi(this);
    initWidget();
    myCom=NULL;
    internalTimer = new QTimer;
    QObject::connect(internalTimer,SIGNAL(timeout()),this,SLOT(internalTime()));
    internalTimer->start(1000*60*5);
}

COM::~COM()
{
    if(myCom!=NULL){
        if(myCom->isOpen()){
            myCom->close();
        }
        delete myCom;
    }
    delete ui;
}

void COM::initWidget()
{
#ifdef Q_OS_LINUX
    ComList<< "ttyUSB0"<<"ttyUSB1"<<"ttyUSB2"<<"ttyUSB3"<<"ttyS0"<<
               "ttyS1"<<"ttyS2"<<"ttyS3"<<"ttyS4"<<"ttyS5"<<"ttyS6";
#elif defined (Q_OS_WIN32)
    ComList<<"COM0"<<"COM1"<<"COM2"<<"COM3"<<"COM4"<<"COM5"<<
                "COM6"<<"COM7"<<"COM8";
#endif
    ui->boxPort->addItems(ComList);
    braudList<<"50"<<"75"<<"100"<<"150"<<"300"<<"600"<<"1200"<<"2400"
                <<"4800"<<"9600"<<"19200"<<"38400"<<"43000"<<"56000"
               <<"57600"<<"115200"<<"128000";
    ui->boxBraud->addItems(braudList);
    ui->boxBraud->setCurrentText("57600");
    ui->boxDataBit->setCurrentText("8");
    ui->boxStopBit->setCurrentText("1");
    ui->boxCheckBit->setCurrentText("NONE");
    ui->lineStauts->setText(tr("Close"));
    ui->lineStauts->setReadOnly(true);
    QFile file(":/configuration/style1.qss");
    file.open(QIODevice::Text|QIODevice::ReadOnly);
    this->setStyleSheet(file.readAll());
    file.close();
    QString filename_log = QDateTime::currentDateTime().toString("yyyyMMdd") + ".txt";
    file.setFileName(filename_log);
    //如果打开失败则给出提示并退出函数
    if(!file.open(QFile::WriteOnly | QIODevice::Text|QIODevice::Append))
        QMessageBox::information(this,tr("提示"),tr("无法打开日志文件"),QMessageBox::Ok);
    file.close();
    write2file(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss dddd") + '\n');
    QString filename = QDir::currentPath() + QDir::separator() + "data.txt";
    if(filename.isEmpty()){
        return;
    }
}

void COM::write2file(QString str)
{
    QString filename = QDateTime::currentDateTime().toString("yyyyMMdd") + ".txt";
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text |QIODevice::Append)){
        QMessageBox::information(this,tr("警告"),tr("无法打开文件，请检查"),QMessageBox::Ok);
    }
    QTextStream out(&file);
    out.setCodec("UTF-8");
    out<<str;
    file.close();
}

void COM::readMyCom()
{
    bool valid;
    unsigned char PLENGTH,signal,delta, theta, lowalpha,highalpha,lowbeta,
            highbeta,lowgamma,midgamma,attention,
            meditation,checksum=0x0,rawdata,blink;
    QByteArray temp = myCom->readAll();
    for(int i=0;i<temp.count();i++)//数据包含三个部分，包头，有效数据（最多169位），检验值
    {
        if(temp.at(i)==0xAA)//0
        {
            i++;
            if(temp.at(i)==0xAA)//1
            {
                i++;
                PLENGTH=(unsigned char)temp.at(i);//2
                if(PLENGTH>170) return;//POOR_SIGNAL Quality 信号质量 0-200，值越大，证明噪声越多，值为200时表示电极未与皮肤接触
                //0x02
                for(int t=i+1;t<PLENGTH+i;t++)
                {
                    checksum+=temp.at(t);
                }
                for(int j=i+1;j<=PLENGTH+i;)//3-34
                {
                    switch(temp.at(j))//3
                    {
                    case 0x02:
                        signal=temp.at(++j);//4
                        j++;//5
                        break;
                    case 0x83:
                        if(temp.at(++j)==0x18)//6
                        {
                            delta=temp.at(j+2);
                            theta=temp.at(j+5);
                            lowalpha=temp.at(j+8);
                            highalpha=temp.at(j+11);
                            lowbeta=temp.at(j+14);
                            highbeta=temp.at(j+17);
                            lowgamma=temp.at(j+20);
                            midgamma=temp.at(j+23);
                            //ASIC_EEG_POWER 0x83
                            /* this is Data Value is output as a series of eight 3-byte unsigned integers in little-endian format.
                             e eight EEG powers are output in the following order: delta (0.5 - 2.75Hz), theta (3.5 -
                                6.75Hz), low-alpha (7.5 - 9.25Hz), high-alpha (10 - 11.75Hz), low-beta (13 - 16.75Hz), high-beta
                                (18 - 29.75Hz), low-gamma (31 - 39.75Hz), and mid-gamma (41 - 49.75Hz).*/
                            //单独数据没有意义
                        }
                        j=j+25;//31
                        break;
                    case 0x04:
                        attention=temp.at(++j);//32
                        j++;//33
                        break;
                        //eSenses (i.e. Attention, Meditation)注意度，冥想度
                        //0-100 各8位 1次/s 0x04 0x05
                        //0 未接触 1-20 非常低 20-40 低 40-60 中等 60-80 偏高 80-100 非常高
                    case 0x05:
                        meditation=temp.at(++j);//34
                        j++;//35
                        break;
                    case 0x16:
                        blink=temp.at(++j);
                        j++;//37
                        break;
                        //眨眼强度 0-255
                        //0x16
                    case 0x80:
                        if(temp.at(++j)==0x02)
                        {
                            unsigned char low=temp.at(++j);
                            unsigned char high=temp.at(++j);
                            rawdata=(low<<8)|high;
                            //RAW WAVE DATA 16位 -32768 to 32767 高低位，每位8位 512次/s 0x80
                            /*short raw = (Value[0]<<8) | Value[1];
                            where Value[0] is the high-order byte, and Value[1] is the low-order byte.
                            In systems or languages where bit operations are inconvenient, the following arithmetic operations
                            may be substituted instead:
                            raw = Value[0]*256 + Value[1];
                            if( raw >= 32768 ) raw = raw - 65536;*/
                        }
                        j++;
                        break;
                    }
                }
                checksum &=0xFF;
                checksum = ~checksum &(0xFF);
                if(checksum==temp.back())
                {
                    valid=true;
                }
                else{
                    valid=false;
                }
            }
        }
        else{
            continue;
        }
    }
    emit sendFilteredData(valid,signal,delta,theta,lowalpha,highalpha,lowbeta,highbeta,lowgamma,midgamma,
                          attention,meditation,rawdata,blink);
}

void COM::internalTime()
{
    QDateTime intertime = QDateTime::currentDateTime();
        QString time = intertime.toString("yyyy-MM-dd hh:mm:ss dddd");
        write2file(time + '\n');
}

void COM::on_pushButtonOpen_clicked()
{
    QString portName = ui->boxPort->currentText();//获取串口名
#ifdef Q_OS_LINUX
    myCom = new QextSerialPort("/dev/" + portName);
#elif defined (Q_OS_WIN)
    myCom = new QextSerialPort(portName);
#endif
    QObject::connect(myCom,SIGNAL(readyRead()),this,SLOT(readMyCom()));
    //设置波特率
    myCom->setBaudRate((BaudRateType)ui->boxBraud->currentText().toInt());
    //设置数据位
    myCom->setDataBits((DataBitsType)ui->boxDataBit->currentText().toInt());
    //设置校验
    switch(ui->boxCheckBit->currentIndex())
    {
        case 0:
            myCom->setParity(PAR_NONE);
            break;
        case 1:
            myCom->setParity(PAR_ODD);
            break;
        case 2:
            myCom->setParity(PAR_EVEN);
            break;
        default:
            myCom->setParity(PAR_NONE);
            qDebug("set to default : PAR_NONE");
            break;
    }
    //设置停止位
    switch(ui->boxStopBit->currentIndex())
    {
         case 0:
             myCom->setStopBits(STOP_1);
             break;
         case 1:
             #ifdef Q_OS_WIN
             myCom->setStopBits(STOP_1_5);
             #endif
             break;
         case 2:
             myCom->setStopBits(STOP_2);
             break;
         default:
             myCom->setStopBits(STOP_1);
             qDebug("set to default : STOP_1");
             break;
    }
    //设置数据流控制
    myCom->setFlowControl(FLOW_OFF);
    //设置延时
    myCom->setTimeout(TIME_OUT);
    if(myCom->open(QIODevice::ReadWrite)){
        QMessageBox::information(this, tr("打开成功"), tr("已成功打开串口") + portName, QMessageBox::Ok);
        //界面控制
        }else{
        QMessageBox::critical(this, tr("打开失败"), tr("未能打开串口 ") + portName + tr("\n该串口设备不存在或已被占用"), QMessageBox::Ok);
        return;
    }
}

void COM::on_pushButtonClose_clicked()
{
    myCom->close();
    myCom=NULL;
    ui->lineStauts->setText(tr("Close"));
    QMessageBox::information(this,tr("Tips"),tr("COM has been closed successfully"),QMessageBox::Ok);
}
