#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopWidget>
#include <QPolygon>
#include <QVector>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QImage>
#include <QLabel>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mBufferFront = new QImage(QApplication::desktop()->size(),
                        QImage::Format_ARGB32_Premultiplied);
    mBufferBack = new QImage(QApplication::desktop()->size(),
                         QImage::Format_ARGB32_Premultiplied);
    mPainterFront = new QPainter(mBufferFront);
    mPainterBack = new QPainter(mBufferBack);
    mPainterFront->fillRect(0,0,mBufferFront->width(), mBufferFront->height(),
                                    QBrush(Qt::transparent));
    mPainterBack->fillRect(0,0,mBufferFront->width(), mBufferFront->height(),
                                    QBrush(Qt::white));
    mLabelStatus = new QLabel(this);
    mLabelStatus->setText("Select first point");
    //Label with current cursor coordinates will be placed on the statusbar
    mLabelCursor = new QLabel(this);
    ui->statusBar->addPermanentWidget(mLabelStatus);
    ui->statusBar->addPermanentWidget(mLabelCursor);
    this->setMouseTracking(true);
    this->centralWidget()->setMouseTracking(true);
}

MainWindow::~MainWindow()
{
    delete mPainterBack;
    delete mPainterFront;
    delete mBufferBack;
    delete mBufferFront;
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawImage(0, 0, *mBufferBack);
    painter.drawImage(0, 0, *mBufferFront);
    //event->accept();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    /*mEnabled = true;
    mBegin = event->pos();
    qDebug() << "First point = " << mBegin;
    event->accept();*/
    event->accept();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    mLabelCursor->setText(QString("cursor x:%1 y:%2").arg(event->x()).arg(event->y()));

    if(mIsFirstPointSelected)
    {
        QPen pen;
        pen.setBrush(Qt::black);
        pen.setWidth(1);
        pen.setCapStyle(Qt::RoundCap);
        pen.setStyle(Qt::DashLine);
        mPainterBack->setPen(pen);
        mPainterBack->fillRect(0,0,mBufferBack->width(), mBufferBack->height(),
                                QBrush(Qt::white));
        mPainterBack->fillRect(0,0,mBufferBack->width(), mBufferBack->height(),
                                QBrush(Qt::transparent));
        if(mIsSecondPointSelected)
        {
            QVector<QPointF> points;
            QPointF thirdPoint(event->pos());
            double a, b, c;
            try
            {
                mModel.parabolaByThreePoints(mFirstPoint, mSecondPoint, thirdPoint,
                                             a, b, c);
                drawParabola(mPainterBack, mFirstPoint, thirdPoint, a, b, c);
            }
            catch(std::exception &ex)
            {
                std::cout << ex.what() << std::endl;
            }

        }
        else
        {
            mPainterBack->drawLine(mFirstPoint, event->pos());

        }
        this->update();
    }
    event->accept();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(mIsFirstPointSelected)
    {
        if(mIsSecondPointSelected)
        {
            mPen.setBrush(QBrush(Qt::darkGreen));
            mPen.setWidth(5);
            mPen.setCapStyle(Qt::RoundCap);
            mPen.setStyle(Qt::SolidLine);
            mPainterFront->setPen(mPen);
            mPainterBack->fillRect(0,0,mBufferFront->width(), mBufferFront->height(),
                                    QBrush(Qt::white));
            mPainterBack->fillRect(0,0,mBufferFront->width(), mBufferFront->height(),
                                    QBrush(Qt::transparent));

            QVector<QPointF> points;
            QPointF thirdPoint(event->pos());
            double a, b, c;
            mModel.parabolaByThreePoints(mFirstPoint, mSecondPoint, thirdPoint,
                                         a, b, c);
            drawParabola(mPainterFront, mFirstPoint, thirdPoint, a, b, c);

            //mPainterFront->drawLine(mFirstPoint, mSecondPoint);

            this->update();
            mIsFirstPointSelected = false;
            mIsSecondPointSelected = false;
            mLabelStatus->setText("Select first point");
        }
        else
        {
            mSecondPoint = event->pos();
            mIsSecondPointSelected = true;
            mLabelStatus->setText("Select third point");
        }
    }
    else
    {
        mFirstPoint = event->pos();
        mIsFirstPointSelected = true;
        mLabelStatus->setText("Select second point");
        qDebug() << "release event mIsFirstPointSelected = " << mIsFirstPointSelected;
    }
}

void MainWindow::drawParabola(QPainter *painter, const QPointF &begin,
                              const QPointF &end, double a, double b, double c)
{
    QVector<QPointF> points;
    int counter = 0;
     for(auto x = begin.x(); x <= end.x(); x += 0.005)
    {
        points << QPoint(x, mModel.squareParabolaFunc(a, b, c, x));
        ++counter;
    }
    QPolygonF path(points);
    painter->drawLines(points);
}
