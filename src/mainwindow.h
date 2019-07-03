#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QString>
#include <QDebug>
#include <QDesktopServices>
#include <QMessageBox>
#include <QUrl>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void loadSkin(int i);

private slots:
    void on_actionExit_triggered();

    void on_actionQt_triggered();

    void on_actionAbout_triggered();

    void on_actionGitHub_triggered();

    void on_actionBlack_triggered();

    void on_actionLighter_triggered();

    void on_actionGray_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
