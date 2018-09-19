#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QDateTime>
#include <QString>
#include <QToolBar>
#include <QTimer>
#include <QFileDialog>
#include <QDialog>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void createActions();
    void createMenu();
    void createToolBar();

    QMenu* menuFile;
    QMenu* menuEdit;
    QMenu* menuTool;
    QMenu* menuHelp;

    QAction* actionOpen;
    QAction* actionClose;
    QAction* actionSave;
    QAction* actionSaveAs;
    QAction* actionPrint;
    QAction* actionExit;

    QAction* actionUndo;
    QAction* actionRedo;
    QAction* actionCopy;
    QAction* actionPaste;
    QAction* actionCut;
    QAction* actionSelectAll;
    QAction* actionPreferrence;

    QAction* actionAbout;
    QAction* actionAboutQt;

    QTimer* updateTimer;
public slots:
    void updateTime();
    void OpenFile();
    void aboutQt();
};

#endif // MAINWINDOW_H
