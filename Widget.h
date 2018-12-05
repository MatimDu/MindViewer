#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <COM.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void on_actionCOm_triggered();
private:
    Ui::MainWindow *ui;
    COM *comWidget;
};

#endif // MAINWINDOW_H
