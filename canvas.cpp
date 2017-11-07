#include "canvas.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QVector>
#include <QImage>
#include <QPainter>
#include <QPointF>
#include <QLineF>
#include <QBrush>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QDebug>

Canvas::Canvas(QWidget *parent):
    QWidget(parent)
{
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

    mPenBackground.setBrush(Qt::black);
    mPenBackground.setWidth(1);
    mPenBackground.setCapStyle(Qt::RoundCap);
    mPenBackground.setStyle(Qt::DashLine);
    mPainterBack->setPen(mPenBackground);

    mPenForeground.setBrush(QBrush(Qt::darkGreen));
    mPenForeground.setWidth(5);
    mPenForeground.setCapStyle(Qt::RoundCap);
    mPenForeground.setStyle(Qt::SolidLine);
    mPainterFront->setPen(mPenForeground);

    this->setMouseTracking(true);
}

Canvas::~Canvas()
{
    delete mPainterBack;
    delete mPainterFront;
    delete mBufferBack;
    delete mBufferFront;
}

void Canvas::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawImage(0, 0, *mBufferBack);
    painter.drawImage(0, 0, *mBufferFront);
}

void Canvas::mousePressEvent(QMouseEvent *event)
{

}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    emit cursorPosChanged(event->pos());
    if(mIsFirstPointSelected)
    {
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

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    if(mIsFirstPointSelected)
    {
        if(mIsSecondPointSelected)
        {

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
            emit statusChanged(QString("Select first point"));
            //mLabelStatus->setText("Select first point");
        }
        else
        {
            mSecondPoint = event->pos();
            mIsSecondPointSelected = true;
            emit statusChanged(QString("Select third point"));
            //mLabelStatus->setText("Select third point");
        }
    }
    else
    {
        mFirstPoint = event->pos();
        mIsFirstPointSelected = true;
        emit statusChanged(QString("Select second point"));
        //mLabelStatus->setText("Select second point");
        qDebug() << "release event mIsFirstPointSelected = " << mIsFirstPointSelected;
    }
}

void Canvas::drawParabola(QPainter *painter, const QPointF &begin,
                          const QPointF &end, double a, double b, double c)
{
    //QVector<QPointF> points;
    QVector<QLineF> lines;
    QPointF prev = begin;
    QPointF next;
    for(auto x = begin.x(); x <= end.x(); x += 0.005)
    {
        next = QPoint(x, mModel.squareParabolaFunc(a, b, c, x));
        QLineF line(prev, next);
        lines << line;
        prev = next;
    }
     //QPolygonF path(points);
     painter->drawLines(lines);
}
