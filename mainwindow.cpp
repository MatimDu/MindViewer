#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    init();
    updateTimer = new QTimer;
    connect(updateTimer,SIGNAL(timeout()),this,SLOT(updateTime()));
    updateTimer->start(1000);
    connect(mycomWidget,SIGNAL(sendFilteredData(bool,unsigned char,unsigned char,unsigned char,unsigned char,unsigned char,unsigned char,unsigned char,unsigned char,unsigned char,unsigned char,unsigned char,unsigned char,unsigned char)),
            this,SLOT(showGraphics(bool,unsigned char,unsigned char,unsigned char,unsigned char,unsigned char,unsigned char,unsigned char,unsigned char,unsigned char,unsigned char,unsigned char,unsigned char,unsigned char)));
}

MainWindow::~MainWindow()
{
}

void MainWindow::init()
{
    num=0;
    sum=0;
    mvStatusBar = new QStatusBar();
    this->setStatusBar(mvStatusBar);
    mvMenuBar = new QMenuBar();
    this->setMenuBar(mvMenuBar);
    mvFile = mvMenuBar->addMenu(tr("文件"));
    fileOpen = new QAction(QIcon(":/icons/open.png"),tr("打开"),this);
    mvFile->addAction(fileOpen);
    connect(fileOpen,SIGNAL(triggered(bool)),this,SLOT(on_actionOpen_triggered()));
    fileClose = new QAction(QIcon(":/icons/close.png"),tr("关闭"),this);
    mvFile->addAction(fileClose);
    connect(fileClose,SIGNAL(triggered(bool)),this,SLOT(on_actionClose_triggered()));
    fileSaveAs = new QAction(QIcon(":/icons/save_all.png"),tr("另存为"),this);
    mvFile->addAction(fileSaveAs);
    connect(fileSaveAs,SIGNAL(triggered(bool)),this,SLOT(on_actionSaveAs_triggered()));
    mvFile->addSeparator();
    fileSetting = new QAction(QIcon(":/icons/config.png"),tr("设置"),this);
    mvFile->addAction(fileSetting);
    connect(fileSetting,SIGNAL(triggered(bool)),this,SLOT(comSettings()));
    mvFile->addSeparator();
    fileExit = new QAction(QIcon(":/icons/exit.png"),tr("退出"),this);
    connect(fileExit,SIGNAL(triggered(bool)),this,SLOT(exitFully()));
    mvFile->addAction(fileExit);

    mvModule = this->mvMenuBar->addMenu(tr("模式(&M)"));
    moduleData = new QAction(QIcon(":/icons/data.png"),tr("数据(&D)"),this);
    mvModule->addAction(moduleData);
    moduleGraphic = new QAction(QIcon(":/icons/image.png"),tr("图像(&G)"),this);
    mvModule->addAction(moduleGraphic);

    mvhelp = this->mvMenuBar->addMenu(tr("帮助(&H)"));
    helpAbout = new QAction(QIcon(":/icons/update.png"),tr("关于"),this);
    connect(helpAbout,SIGNAL(triggered(bool)),this,SLOT(aboutThis()));
    mvhelp->addAction(helpAbout);
    helpInformation = new QAction(QIcon(":/icons/version.png"),tr("信息"),this);
    connect(helpInformation,SIGNAL(triggered(bool)),this,SLOT(versionInfo()));
    mvhelp->addAction(helpInformation);

//    rawData = new QPlainTextEdit();
//    this->setCentralWidget(rawData);
//    rawData->show();
//    while(rawData->isVisible())
//    {
//        rawData->appendPlainText(rawData->toPlainText());
//        QTextCursor cursor = rawData->textCursor();
//        cursor.movePosition(QTextCursor::End);
//        rawData->setTextCursor(cursor);//移动鼠标光标至编辑框底部
//    }
    mvChart = new QChart;
    chartView = new QChartView(mvChart);
    chartView->setRubberBand(QChartView::RectangleRubberBand);//可放大
    chartView->setRenderHint(QPainter::Antialiasing);//抗锯齿
    this->setCentralWidget(chartView);
    mycomWidget = new comWidget();
    signalSeries= new QSplineSeries;
    deltaSeries = new QSplineSeries;
    thetaSeries = new QSplineSeries;
    lowalphaSeries = new QSplineSeries;
    highalphaSeries = new QSplineSeries;
    lowbetaSeries = new QSplineSeries;
    highbetaSeries = new QSplineSeries;
    lowgammaSeries = new QSplineSeries;
    midgammaSeries = new QSplineSeries;
    attentionSeries = new QSplineSeries;
    meditationSeries = new QSplineSeries;
    rawdataSeries = new QSplineSeries;
    blinkSeries = new QSplineSeries;
    mvChart->addSeries(signalSeries);
    mvChart->addSeries(deltaSeries);
    mvChart->addSeries(thetaSeries);
    mvChart->addSeries(lowalphaSeries);
    mvChart->addSeries(highalphaSeries);
    mvChart->addSeries(lowbetaSeries);
    mvChart->addSeries(highbetaSeries);
    mvChart->addSeries(lowgammaSeries);
    mvChart->addSeries(midgammaSeries);
    mvChart->addSeries(attentionSeries);
    mvChart->addSeries(meditationSeries);
    mvChart->addSeries(rawdataSeries);
    mvChart->addSeries(blinkSeries);
    mvChart->setTitle(tr("脑电波数据动态显示"));
    mvChart->legend()->setAlignment(Qt::AlignBottom);
    mvChart->createDefaultAxes();
    mvChart->setAnimationOptions(QChart::SeriesAnimations);
    signalSeries->setName(tr("signal"));
    deltaSeries->setName(tr("delta"));
    thetaSeries->setName(tr("theta"));
    lowalphaSeries->setName(tr("lowalpha"));
    highalphaSeries->setName(tr("highalpha"));
    lowbetaSeries->setName(tr("lowbeta"));
    highbetaSeries->setName(tr("highbeta"));
    lowgammaSeries->setName(tr("lowgamma"));
    midgammaSeries->setName(tr("midgamma"));
    attentionSeries->setName(tr("attention"));
    meditationSeries->setName(tr("meditation"));
    rawdataSeries->setName(tr("rawdata"));
    blinkSeries->setName(tr("blink"));
    signalSeries->setUseOpenGL(true);
    deltaSeries->setUseOpenGL(true);
    thetaSeries->setUseOpenGL(true);
    lowalphaSeries->setUseOpenGL(true);
    highalphaSeries->setUseOpenGL(true);
    lowbetaSeries->setUseOpenGL(true);
    highbetaSeries->setUseOpenGL(true);
    lowgammaSeries->setUseOpenGL(true);
    midgammaSeries->setUseOpenGL(true);
    attentionSeries->setUseOpenGL(true);
    meditationSeries->setUseOpenGL(true);
    rawdataSeries->setUseOpenGL(true);
    blinkSeries->setUseOpenGL(true);
    signalSeries->setBrush(QBrush(QColor(0,200,50),Qt::SolidPattern));
    deltaSeries->setPen(QPen(Qt::red,2,Qt::SolidLine));
    thetaSeries->setPen(QPen(Qt::yellow,2,Qt::SolidLine));
    lowalphaSeries->setPen(QPen(Qt::green,2,Qt::SolidLine));
    highalphaSeries->setPen(QPen(Qt::cyan,2,Qt::SolidLine));
    lowbetaSeries->setPen(QPen(Qt::blue,2,Qt::SolidLine));
    highbetaSeries->setPen(QPen(Qt::magenta,2,Qt::SolidLine));
    lowgammaSeries->setPen(QPen(Qt::black,2,Qt::SolidLine));
    midgammaSeries->setPen(QPen(Qt::gray,2,Qt::SolidLine));
    attentionSeries->setPen(QPen(QColor(150,75,0),2,Qt::SolidLine));
    meditationSeries->setPen(QPen(QColor(255,0,122),2,Qt::SolidLine));
    rawdataSeries->setPen(QPen(QColor(128,0,255),2,Qt::SolidLine));
    blinkSeries->setBrush(QBrush(QColor(200,200,0),Qt::SolidPattern));
    //x轴
    QValueAxis *axisx = new QValueAxis;
    axisx->setTitleText("时间");
    axisx->setRange(0,60);//一分钟
    axisx->setLabelFormat("%d");
    axisx->setGridLineVisible(true);
    axisx->setTickCount(10);//设置多少格
    axisx->setMinorTickCount(2);//多少小格
    mvChart->setAxisX(axisx,signalSeries);
    mvChart->setAxisX(axisx,deltaSeries);
    mvChart->setAxisX(axisx,thetaSeries);
    mvChart->setAxisX(axisx,lowalphaSeries);
    mvChart->setAxisX(axisx,highalphaSeries);
    mvChart->setAxisX(axisx,lowbetaSeries);
    mvChart->setAxisX(axisx,highbetaSeries);
    mvChart->setAxisX(axisx,lowgammaSeries);
    mvChart->setAxisX(axisx,midgammaSeries);
    mvChart->setAxisX(axisx,attentionSeries);
    mvChart->setAxisX(axisx,meditationSeries);
    mvChart->setAxisX(axisx,rawdataSeries);
    mvChart->setAxisX(axisx,blinkSeries);
    //y轴
    QValueAxis *axisy = new QValueAxis;
    axisy->setTitleText(tr("值"));
    axisy->setRange(-255,255);
    axisy->setLabelFormat("%d");
    axisy->setGridLineVisible(true);
    axisy->setTickCount(10);
    axisy->setMinorTickCount(1);
    mvChart->setAxisY(axisy,signalSeries);
    mvChart->setAxisY(axisy,deltaSeries);
    mvChart->setAxisY(axisy,thetaSeries);
    mvChart->setAxisY(axisy,lowalphaSeries);
    mvChart->setAxisY(axisy,highalphaSeries);
    mvChart->setAxisY(axisy,lowbetaSeries);
    mvChart->setAxisY(axisy,highbetaSeries);
    mvChart->setAxisY(axisy,lowgammaSeries);
    mvChart->setAxisY(axisy,midgammaSeries);
    mvChart->setAxisY(axisy,attentionSeries);
    mvChart->setAxisY(axisy,meditationSeries);
    mvChart->setAxisY(axisy,blinkSeries);
    axisz= new QValueAxis;
    axisz->setTitleText(tr("源数据"));
    axisz->setRange(-32768,32767);
    axisz->setLabelFormat("%d");
    axisz->setGridLineVisible(true);
    axisz->setTickCount(10);
    axisz->setMinorTickCount(1);
    mvChart->addAxis(axisz,Qt::AlignRight);
}

void MainWindow::write2file(unsigned char signal,unsigned char delta,unsigned char theta,
                            unsigned char lowalpha,unsigned char highalpha,unsigned char lowbeta,
                            unsigned char highbeta,unsigned char lowgamma,unsigned char midgamma,
                            unsigned char attention,unsigned char meditation,unsigned char rawdata,
                            unsigned char blink)
{
    QString filename = QDateTime::currentDateTime().toString("yyyyMMdd") + "data.txt";
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text |QIODevice::Append))
    {
        QMessageBox::information(this,tr("警告"),tr("无法打开文件，请检查"),QMessageBox::Ok);
    }
    QTextStream out(&file);
    out.setCodec("UTF-8");
    out<<signal+ ' ' +delta +' '+theta+' '+lowalpha+' '+highalpha+' '+lowbeta+' '+highbeta+' '+lowgamma
         +' '+midgamma+' '+attention+' '+meditation+' '+rawdata+' '+blink+'\n';
    file.close();
}

void MainWindow::updateTime()
{
    QDateTime currentDateTime = QDateTime::currentDateTime();
    this->mvStatusBar->showMessage(currentDateTime.toString("yyyy-MM-dd hh:mm:ss dddd"));
}

void MainWindow::exitFully()
{
    delete mvStatusBar;
    delete mvMenuBar;
    delete updateTimer;
    this->close();
    qApp->quit();
    exit(0);
}

void MainWindow::comSettings()
{
    mycomWidget->show();
}

void MainWindow::versionInfo()
{
    QMessageBox::information(this,"版本信息","<h3>V2018.3.30</h3>"
                                 "<h4>没有现成的软件肯定要做啊，不做没有软件用</h4>"
                                 "<h4>原创是不可能原创的，这辈子都不可能原创的</h4>"
                                 "<h4>C++精通太难，MFC又不会，Windows软件又不兼容Linux</h4>"
                                 "<h4>只能靠抄抄别人的代码这个样子</h4>"
                                 "<h4>Github里面个个都是人才，代码写得六，格式工整，又是跨平台</h4>"
                                 "<h4>我超喜欢这里的</h4>"
                                 "<h4 align=\"right\">-----幽弥狂</h4>",QMessageBox::Ok);
}

void MainWindow::aboutThis()
{
    QMessageBox::aboutQt(this);
}

void MainWindow::closeEvent(QCloseEvent *)
{
    if(mycomWidget->isVisible())
    {
        delete mycomWidget;
    }
    this->close();
    qApp->quit();
    exit(0);
}

void MainWindow::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("打开文件"),QString(),"*.*");
    QString rawString;
    if(!filename.isEmpty())
    {
        QFile file(filename);
        if(file.open(QIODevice::ReadOnly|QIODevice::Text))
        {
            if(rawData->isVisible())
            {
                QTextStream readIn(&file);
                while (!readIn.atEnd()) {
                    rawString = readIn.readAll();
                }
            }
        }
        file.close();
    }
//    rawData->appendPlainText(rawString);
//    highlighter = new Highlighter(rawData->document());
//    if(!rawData->isVisible()){
//        rawData->setVisible(true);
//    }
}

void MainWindow::on_actionClose_triggered()
{
}

void MainWindow::on_actionSaveAs_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this,tr("另存为"),QString(),"*.data\n*.pic");
    if(!filename.isEmpty())
    {
        QFile file(filename);
        if(file.open(QIODevice::WriteOnly|QIODevice::Text))
        {
        }
    }
}

void MainWindow::showGraphics(bool valid,
                              unsigned char signal,unsigned char delta,unsigned char theta,
                              unsigned char lowalpha,unsigned char highalpha,unsigned char lowbeta,
                              unsigned char highbeta,unsigned char lowgamma,unsigned char midgamma,
                              unsigned char attention,unsigned char meditation,unsigned char rawdata,
                              unsigned char blink)
{
    write2file(signal,delta,theta,lowalpha,highalpha,lowbeta,highbeta,
               lowgamma,midgamma,attention,meditation,rawdata,blink);
    sum++;
    if(valid)
    {
        num++;
    }
    mvStatusBar->showMessage(QString::number((double)num/sum));
    signalSeries->append(num%60,signal);
    deltaSeries->append(num%60,delta);
    thetaSeries->append(num%60,theta);
    lowalphaSeries->append(num%60,lowalpha);
    highalphaSeries->append(num%60,highalpha);
    lowbetaSeries->append(num%60,lowbeta);
    highbetaSeries->append(num%60,highbeta);
    lowgammaSeries->append(num%60,lowgamma);
    midgammaSeries->append(num%60,midgamma);
    attentionSeries->append(num%60,attention);
    meditationSeries->append(num%60,meditation);
    rawdataSeries->append(num%60,rawdata);
    rawdataSeries->attachAxis(axisz);
    blinkSeries->append(num%60,blink);
    chartView->chart()->scroll(20,0);
}
