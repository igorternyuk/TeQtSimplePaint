#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMouseEvent>
#include <QLabel>
#include <QDebug>
#include "canvas.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("TeSimplePaint");
   // this->setMinimumSize(840, 640);
    mLabelStatus = new QLabel(this);
    mLabelStatus->setText("Select first point");
    //Label with current cursor coordinates will be placed on the statusbar
    mLabelCursor = new QLabel(this);
    ui->statusBar->addPermanentWidget(mLabelStatus);
    ui->statusBar->addPermanentWidget(mLabelCursor);
    this->setMouseTracking(true);
    this->centralWidget()->setMouseTracking(true);
    connect(ui->canvas, &Canvas::cursorPosChanged, [this](QPointF pos){
        mLabelCursor->setText(QString("cursor x:%1 y:%2").arg(pos.x()).arg(pos.y()));
    });
    connect(ui->canvas, &Canvas::statusChanged, [this](QString msg){
        mLabelStatus->setText(msg);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    QMainWindow::mouseMoveEvent(event);
}
