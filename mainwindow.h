#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "globalheaders.h"
#include "comwidget.h"
#include "highlighter.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void init();//初始化所有相关的控件
    unsigned long long int num;
    unsigned long long int sum;
    //文件 菜单 包括 打开、关闭、保存、另存为、退出
    QMenu *mvFile;
    QAction *fileOpen;
    QAction *fileClose;
    QAction *fileSaveAs;
    QAction *fileSetting;//串口设置
    QAction *fileExit;
    //编辑 菜单
    QMenu *mvModule;
    QAction *moduleData;
    QAction *moduleGraphic;
    //帮助 菜单
    QMenu *mvhelp;
    QAction *helpAbout;
    QAction *helpInformation;
    //菜单栏
    QMenuBar *mvMenuBar;
    //状态栏
    QStatusBar *mvStatusBar;
    //更新时间的计时器
    QTimer *updateTimer;
    //串口设置窗口
    QPlainTextEdit *rawData;
    QChart *mvChart;
    comWidget *mycomWidget;
    Highlighter *highlighter;
    QChartView *chartView;
    QSplineSeries *signalSeries;
    QSplineSeries *deltaSeries;
    QSplineSeries *thetaSeries;
    QSplineSeries *lowalphaSeries;
    QSplineSeries *highalphaSeries;
    QSplineSeries *lowbetaSeries;
    QSplineSeries *highbetaSeries;
    QSplineSeries *lowgammaSeries;
    QSplineSeries *midgammaSeries;
    QSplineSeries *attentionSeries;
    QSplineSeries *meditationSeries;
    QSplineSeries *rawdataSeries;
    QSplineSeries *blinkSeries;
    QVector<QPointF> deltaPointF;
    QVector<QPointF> deltaOldPointF;
    QVector<QPointF> thetaPointF;
    QVector<QPointF> thetaOldPointF;
    QVector<QPointF> signalPointF;
    QVector<QPointF> signalOldPointF;
    QVector<QPointF> lowalphaPointF;
    QVector<QPointF> lowalphaOldPointF;
    QVector<QPointF> lowbetaPointF;
    QVector<QPointF> lowbetaOldPointF;
    QVector<QPointF> highalphaPointF;
    QVector<QPointF> highalphaOldPointF;
    QVector<QPointF> highbetaPointF;
    QVector<QPointF> highbetaOldPointF;
    QVector<QPointF> rawdataPointF;
    QVector<QPointF> rawdataOldPointF;
    QVector<QPointF> lowgammaPointF;
    QVector<QPointF> lowgammaOldPointF;
    QVector<QPointF> midgammaPointF;
    QVector<QPointF> midgammaOldPointF;
    QVector<QPointF> attentionPointF;
    QVector<QPointF> attentionOldPointF;
    QVector<QPointF> meditationPointF;
    QVector<QPointF> meditationOldPointF;
    QVector<QPointF> blinkPointF;
    QVector<QPointF> blinkOldPointF;
    void write2file(unsigned char signal,unsigned char delta,unsigned char theta,
                    unsigned char lowalpha,unsigned char highalpha,unsigned char lowbeta,
                    unsigned char highbeta,unsigned char lowgamma,unsigned char midgamma,
                    unsigned char attention,unsigned char meditation,unsigned char rawdata,
                    unsigned char blink);
    QValueAxis *axisz;
public slots:
    void updateTime();//更新时间的槽
    void exitFully();//完全退出程序
    void comSettings();//串口设置
    void versionInfo();//软件信息和说明
    void aboutThis();
    void closeEvent(QCloseEvent *);
    void on_actionOpen_triggered();
    void on_actionClose_triggered();
    void on_actionSaveAs_triggered();
    void showGraphics(bool valid,
                      unsigned char signal,unsigned char delta,unsigned char theta,
                      unsigned char lowalpha,unsigned char highalpha,unsigned char lowbeta,
                      unsigned char highbeta,unsigned char lowgamma,unsigned char midgamma,
                      unsigned char attention,unsigned char meditation,unsigned char rawdata,
                      unsigned char blink);
};

#endif // MAINWINDOW_H
