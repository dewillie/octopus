#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "position.h"
#include "movement.h";
#include "joint.h"
#include "jointmvt.h"
#include "clientosc.h"
#include "serverosc.h"
#include "controller.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Controller* c = new Controller();

}

MainWindow::~MainWindow()
{
    delete ui;
}

