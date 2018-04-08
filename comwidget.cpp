#include "comwidget.h"

comWidget::comWidget(QWidget *parent) : QWidget(parent)
{
    init();
    myCom=NULL;
    QString filename_log = QDateTime::currentDateTime().toString("yyyyMMdd") + ".txt";
    QFile file(filename_log);
    //如果打开失败则给出提示并退出函数
    if(!file.open(QFile::WriteOnly | QIODevice::Text|QIODevice::Append))
        QMessageBox::information(this,tr("提示"),tr("无法打开日志文件"),QMessageBox::Ok);
    file.close();
    write2file(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss dddd") + '\n');
    internalTimer = new QTimer;
    QObject::connect(internalTimer,SIGNAL(timeout()),this,SLOT(internalTime()));
    internalTimer->start(1000*60*5);
    QObject::connect(openButton,SIGNAL(clicked(bool)),this,SLOT(on_comButtonOpen_triggered()));
    QObject::connect(closeButton,SIGNAL(clicked(bool)),this,SLOT(on_comButtonClose_triggered()));
    QString filename = QDir::currentPath() + QDir::separator() + "data.txt";
    if(filename.isEmpty())
    {
        return;
    }
}

comWidget::~comWidget()
{
    delete comLabel;
    delete comComboBox;
    delete braudLabel;
    delete braudComboBox;
    delete bitLabel;
    delete bitComboBox;
    delete stopBitLabel;
    delete stopComboBox;
    delete checkBitLabel;
    delete checkComboBox;
    delete dataModulLabel;
    delete dataModulLineEdit;
    delete statusLabel;
    delete statusLineEdit;
    delete openButtonLabel;
    delete openButton;
    delete closeButtonLabel;
    delete closeButton;
    delete dataRadioButton;
    delete graphicRadioButton;
    delete showButton;
    if(myCom!=NULL)
    {
        if(myCom->isOpen()){
            myCom->close();
        }
        delete myCom;
    }
}

void comWidget::init()
{
    QSize commonSize(60,25);
    this->setFixedSize(commonSize.width()*3+20,commonSize.height()*7+40);
    comComboBox = new QComboBox(this);
    comComboBox->move(5,commonSize.height()+5);
    comComboBox->resize(commonSize);
#ifdef Q_OS_LINUX
    comComboBox->addItem( "ttyUSB0");
    comComboBox->addItem( "ttyUSB1");
    comComboBox->addItem( "ttyUSB2");
    comComboBox->addItem( "ttyUSB3");
    comComboBox->addItem( "ttyS0");
    comComboBox->addItem( "ttyS1");
    comComboBox->addItem( "ttyS2");
    comComboBox->addItem( "ttyS3");
    comComboBox->addItem( "ttyS4");
    comComboBox->addItem( "ttyS5");
    comComboBox->addItem( "ttyS6");
#elif defined (Q_OS_WIN32)
    comComboBox->addItem("COM0");
    comComboBox->addItem("COM1");
    comComboBox->addItem("COM2");
    comComboBox->addItem("COM3");
    comComboBox->addItem("COM4");
    comComboBox->addItem("COM5");
    comComboBox->addItem("COM6");
    comComboBox->addItem("COM7");
    comComboBox->addItem("COM8");
#endif
    comLabel = new QLabel(this);
    comLabel->setText(tr("串口："));
    comLabel->setAlignment(Qt::AlignCenter);
    comLabel->resize(commonSize);
    comLabel->move(5,5);
    braudLabel = new QLabel(this);
    braudLabel->setText(tr("波特率："));
    braudLabel->setAlignment(Qt::AlignCenter);
    braudLabel->resize(commonSize);
    braudLabel->move(commonSize.width()+10,5);
    braudComboBox = new QComboBox(this);
    QStringList braudList;
    braudList<<"50"<<"75"<<"100"<<"150"<<"300"<<"600"<<"1200"<<"2400"
            <<"4800"<<"9600"<<"19200"<<"38400"<<"43000"<<"56000"
           <<"57600"<<"115200"<<"128000";
    braudComboBox->addItems(braudList);
    braudComboBox->setCurrentText("57600");
    braudComboBox->move(commonSize.width() +10,commonSize.height()+10);
    bitLabel = new QLabel(this);
    bitLabel->setText(tr("数据位："));
    bitLabel->setAlignment(Qt::AlignCenter);
    bitLabel->resize(commonSize);
    bitLabel->move(commonSize.width()*2+15,5);
    bitComboBox = new QComboBox(this);
    QStringList bitList;
    bitList<<"5"<<"6"<<"7"<<"8";
    bitComboBox->addItems(bitList);
    bitComboBox->setCurrentText("8");
    bitComboBox->resize(commonSize);
    bitComboBox->move(commonSize.width()*2+15,commonSize.height()+10);
    //第二行
    stopBitLabel = new QLabel(this);
    stopBitLabel->setText(tr("停止位"));
    stopBitLabel->setAlignment(Qt::AlignCenter);
    stopBitLabel->resize(commonSize);
    stopBitLabel->move(5,commonSize.height()*2+15);
    stopComboBox = new QComboBox(this);
    QStringList stopBitList;
    stopBitList<<"1"<<"1.5"<<"2";
    stopComboBox->addItems(stopBitList);
    stopComboBox->setCurrentText("1");
    stopComboBox->resize(commonSize);
    stopComboBox->move(5,commonSize.height()*3+20);
    checkBitLabel = new QLabel(this);
    checkBitLabel->setText(tr("校验位："));
    checkBitLabel->setAlignment(Qt::AlignCenter);
    checkBitLabel->resize(commonSize);
    checkBitLabel->move(commonSize.width()+10,commonSize.height()*2+15);
    checkComboBox = new QComboBox(this);
    QStringList checkList;
    checkList<<tr("无")<<tr("奇")<<tr("偶");
    checkComboBox->addItems(checkList);
    checkComboBox->setCurrentText(tr("无"));
    checkComboBox->resize(commonSize);
    checkComboBox->move(commonSize.width() +10,commonSize.height()*3 +20);
    dataModulLabel = new QLabel(this);
    dataModulLabel->setText(tr("数据模式"));
    dataModulLabel->setAlignment(Qt::AlignCenter);
    dataModulLabel->resize(commonSize);
    dataModulLabel->move(commonSize.width()*2+15,commonSize.height()*2+15);
    dataModulLineEdit = new QLineEdit(this);
    dataModulLineEdit->setText(tr("十六进制"));
    dataModulLineEdit->setAlignment(Qt::AlignLeft);
    dataModulLineEdit->setEnabled(false);
    dataModulLineEdit->resize(commonSize);
    dataModulLineEdit->move(commonSize.width()*2+15,commonSize.height()*3+20);
    dataModulLineEdit->show();
    //第三行
    statusLabel = new QLabel(this);
    statusLabel->setText(tr("串口状态："));
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->resize(commonSize);
    statusLabel->move(5,commonSize.height()*4+25);
    statusLineEdit = new QLineEdit(this);
    statusLineEdit->setText(tr("关"));
    statusLineEdit->show();
    statusLineEdit->resize(commonSize);
    statusLineEdit->move(5,commonSize.height()*5 +30);
    statusLineEdit->setAlignment(Qt::AlignLeft);
    statusLineEdit->setEnabled(false);
    openButtonLabel = new QLabel(this);
    openButtonLabel->setText(tr("串口："));
    openButtonLabel->setAlignment(Qt::AlignCenter);
    openButtonLabel->resize(commonSize);
    openButtonLabel->move(commonSize.width()+10,commonSize.height()*4+25);
    openButton = new QPushButton(this);
    openButton->setText(tr("打开"));
    openButton->resize(commonSize);
    openButton->move(commonSize.width()+10,commonSize.height()*5+30);
    closeButtonLabel = new QLabel(this);
    closeButtonLabel->setText(tr("串口"));
    closeButtonLabel->setAlignment(Qt::AlignCenter);
    closeButtonLabel->move(commonSize.width()*2+15,commonSize.height()*4+25);
    closeButtonLabel->resize(commonSize);
    closeButton = new QPushButton(this);
    closeButton->setText(tr("关闭"));
    closeButton->resize(commonSize);
    closeButton->move(commonSize.width()*2+15,commonSize.height()*5+30);
    //第四行
    dataRadioButton = new QRadioButton(this);
    dataRadioButton->setText(tr("数据"));
    dataRadioButton->setChecked(true);
    dataRadioButton->resize(commonSize);
    dataRadioButton->move(5,commonSize.height()*6+35);
    graphicRadioButton = new QRadioButton(this);
    graphicRadioButton->setText(tr("图像"));
    graphicRadioButton->setChecked(false);
    graphicRadioButton->resize(commonSize);
    graphicRadioButton->move(commonSize.width()+10,commonSize.height()*6+35);
    showButton = new QPushButton(this);
    showButton->setText(tr("显示"));
    showButton->resize(commonSize);
    showButton->move(commonSize.width()*2+15,commonSize.height()*6+35);
}

void comWidget::write2file(QString str)
{
    QString filename = QDateTime::currentDateTime().toString("yyyyMMdd") + ".txt";
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text |QIODevice::Append))
    {
        QMessageBox::information(this,tr("警告"),tr("无法打开文件，请检查"),QMessageBox::Ok);
    }
    QTextStream out(&file);
    out.setCodec("UTF-8");
    out<<str;
    file.close();
}

void comWidget::on_comButtonOpen_triggered()
{
    QString portName = comComboBox->currentText();//获取串口名
#ifdef Q_OS_LINUX
    myCom = new QextSerialPort("/dev/" + portName);
#elif defined (Q_OS_WIN)
    myCom = new QextSerialPort(portName);
#endif
    QObject::connect(myCom,SIGNAL(readyRead()),this,SLOT(readMyCom()));
    //设置波特率
    myCom->setBaudRate((BaudRateType)braudComboBox->currentText().toInt());
    //设置数据位
    myCom->setDataBits((DataBitsType)bitComboBox->currentText().toInt());
    //设置校验
    switch(checkComboBox->currentIndex())
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
    switch(stopComboBox->currentIndex())
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
        statusLineEdit->setText(tr("开"));
        }else{
        QMessageBox::critical(this, tr("打开失败"), tr("未能打开串口 ") + portName + tr("\n该串口设备不存在或已被占用"), QMessageBox::Ok);
        return;
    }
}

void comWidget::on_comButtonClose_triggered()
{
    myCom->close();
    myCom=NULL;
    statusLineEdit->setText(tr("关"));
}

void comWidget::readMyCom()
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

void comWidget::internalTime()
{
    QDateTime intertime = QDateTime::currentDateTime();
    QString time = intertime.toString("yyyy-MM-dd hh:mm:ss dddd");
    write2file(time + '\n');
}
