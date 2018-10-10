#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "myopengl.h"
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QVBoxLayout *layout = new QVBoxLayout();
    GLWidget = new MyOpenGL();
    layout->addWidget(GLWidget);
    this->ui->centralWidget->setLayout(layout);
    this->setWindowTitle("simpleTexcoord");

}

MainWindow::~MainWindow()
{
    delete ui;
}
