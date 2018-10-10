#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class MyOpenGL;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    MyOpenGL *GLWidget;
};

#endif // MAINWINDOW_H
