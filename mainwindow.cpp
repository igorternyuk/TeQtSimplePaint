#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMouseEvent>
#include <QLabel>
#include <QStringList>
#include <QColorDialog>
#include <QMessageBox>
#include <QDebug>
#include "canvas.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("TeSimplePaint");
    QStringList items;
    items << "Pen" << "Straight line" << "Polyline" <<
             "Rectangle by opposite corners" << "Rectangle by center" <<
             "Square by opposite corners" << "Square by center" <<
             "Circle by bounding rectangle" << "Circle by center and radius" <<
             "Circle by two points" << "Circle by three points" <<
             "Ellipse by bounding rectangle" << "Ellipse by center" <<
             "Parabola by 3 points" <<
             "Quadratic Bezier curve" << "Cubic Bezier curve";
    ui->comboDrawingType->addItems(items);
    ui->comboDrawingType->setCurrentIndex(0);
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
    connect(ui->comboDrawingType, SIGNAL(currentIndexChanged(int)), ui->canvas,
            SLOT(changeDrawingType(int)));
    connect(ui->btnChooseBrushColor, &QPushButton::clicked, [this](){
        QColor color = QColorDialog::getColor(Qt::darkGreen, this, "Choose the color for brush");
        emit brushColorChanged(color);
    });
    connect(this, &MainWindow::brushColorChanged, ui->canvas, &Canvas::changeBrushColor);
    connect(ui->spinBoxBrushSize, SIGNAL(valueChanged(int)), ui->canvas, SLOT(changeBrushSize(int)));
    connect(ui->checkBoxFillShape, SIGNAL(clicked(bool)), ui->canvas, SLOT(setShapeFill(bool)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    QMainWindow::mouseMoveEvent(event);
}

void MainWindow::on_action_open_file_triggered()
{
    ui->canvas->openFile();
}

void MainWindow::on_action_save_file_triggered()
{
    ui->canvas->saveFile();
}

void MainWindow::on_action_quit_triggered()
{
    QMessageBox::StandardButton reply =
            QMessageBox::question(this, "Confirm quit",
                                  "Do you really want to quit application?",
                                  QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes)
        this->close();
}
