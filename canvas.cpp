#include "canvas.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QtMath>
#include <QVector>
#include <QImage>
#include <QPainter>
#include <QPainterPath>
#include <QRectF>
#include <QPointF>
#include <QLineF>
#include <QBrush>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QStandardPaths>
#include <QFileDialog>
#include <QMessageBox>
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

    mPenForeground.setBrush(mBrushColor);
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

void Canvas::changeDrawingType(int type)
{
    mDrawingType = static_cast<DrawingType>(type);
}

void Canvas::changeBrushColor(QColor color)
{
    mBrushColor = color;
    mPenForeground.setBrush(color);
    mPainterFront->setPen(mPenForeground);
}

void Canvas::changeBrushSize(int size)
{
    mPenForeground.setWidth(size);
    mPainterFront->setPen(mPenForeground);
}

void Canvas::setShapeFill(bool fill)
{
    mFillShapes = fill;
}

void Canvas::openFile()
{
    auto startDir = QStandardPaths::standardLocations(
                QStandardPaths::PicturesLocation).value(0, QDir::homePath());

    auto fileName = QFileDialog::getOpenFileName(this, "Open a picture", startDir,
                                                 "Images (*.jpg *.jpeg *\
                                                 *.png *.bmp);;All files (*.*)");
    QImage img;
    bool isLoaded = img.load(fileName);
    if(!isLoaded)
    {
        QMessageBox::critical(this, "Loading error", "Could not load file");
    }
    else
    {
        mPainterFront->drawImage(0, 0, img);
        this->update();
    }
}

void Canvas::saveFile()
{
    auto startDir = QStandardPaths::standardLocations(
                QStandardPaths::PicturesLocation).value(0, QDir::homePath());

    auto fileName = QFileDialog::getSaveFileName(this, "Save picture", startDir,
                                                 "Images (*.jpg *.jpeg *\
                                                 *.png *.bmp);;All files (*.*)");
    bool isSaved = mBufferFront->save(fileName);
    if(!isSaved)
    {
        QMessageBox::critical(this, "Saving error", "Could not save file");
    }
}

void Canvas::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.drawImage(0, 0, *mBufferBack);
    painter.drawImage(0, 0, *mBufferFront);
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    if(mDrawingType == DrawingType::PEN)
    {
        mEnabled = true;
        mFirstPoint = event->pos();
    }
}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    emit cursorPosChanged(event->pos());

    mPainterBack->fillRect(0,0,mBufferBack->width(), mBufferBack->height(),
                            QBrush(Qt::white));
    mPainterBack->fillRect(0,0,mBufferBack->width(), mBufferBack->height(),
                            QBrush(Qt::transparent));

    if(mDrawingType == DrawingType::PEN)
    {
        if(mEnabled && event->buttons() & Qt::LeftButton)
        {
            mPainterFront->drawLine(mFirstPoint, event->pos());
            mFirstPoint = event->pos();
            this->update();
        }
    }
    else if(mDrawingType == DrawingType::ELLIPSE_BY_CENTER)
    {
        if(mIsFirstPointSelected)
        {
            if(mIsSecondPointSelected)
            {
                //Ellipse semiaxes
                auto rx = qAbs(mFirstPoint.x() - mSecondPoint.x());
                auto ry = qAbs(event->pos().y() - mFirstPoint.y());
                mPainterBack->drawEllipse(mFirstPoint, rx, ry);
            }
            else
            {
                mPainterBack->drawLine(mFirstPoint, QPointF(event->pos().x(),
                                                            mFirstPoint.y()));
            }
            this->update();
        }
    }
    else if(mDrawingType == DrawingType::CIRCLE_BY_THREE_POINTS)
    {
        if(mIsFirstPointSelected)
        {
            if(mIsSecondPointSelected)
            {
                QPointF thirdPoint(event->pos());
                QPointF center;
                double radius;
                try
                {
                    mModel.circleByThreePoints(mFirstPoint, mSecondPoint,
                                               thirdPoint, center, radius);
                    mPainterBack->drawEllipse(center, radius, radius);
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
    }
    else if(mDrawingType == DrawingType::PARABOLA_BY_THREE_POINTS)
    {
        if(mIsFirstPointSelected)
        {
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
    }
    else if(mDrawingType == DrawingType::QUADRATIC_BEZIER_CURVE)
    {
        if(mIsFirstPointSelected)
        {
            if(mIsSecondPointSelected)
            {
                QPointF controlPoint(event->pos());
                drawQuadraticBezierCurve(mPainterBack, mFirstPoint,
                                         mSecondPoint, controlPoint);
            }
            else
            {
                mPainterBack->drawLine(mFirstPoint, event->pos());
            }
            this->update();
        }
    }
    else if(mDrawingType == DrawingType::CUBIC_BEZIER_CURVE)
    {
        if(mIsFirstPointSelected)
        {
            if(mIsSecondPointSelected)
            {
                if(mIsFirstControlPointSelected)
                {
                    drawCubicBezierCurve(mPainterBack, mFirstPoint,
                                         mSecondPoint, mFirstControlPoint,
                                         event->pos());
                }
                else
                {
                    drawQuadraticBezierCurve(mPainterBack, mFirstPoint,
                                             mSecondPoint, event->pos());
                }
            }
            else
            {
                mPainterBack->drawLine(mFirstPoint, event->pos());
            }
            this->update();
        }
    }
    else
    {
        if(mIsFirstPointSelected)
        {
            QRectF rect;
            QPointF center;
            double radius;
            QLineF line;

            switch (mDrawingType) {
            case DrawingType::LINE:
            case DrawingType::POLYLINE:
                 mPainterBack->drawLine(mFirstPoint, event->pos());
                break;
            case DrawingType::SQUARE_BY_OPPOSITE_CORNERS:
                rect = getRectByCornerPoints(mFirstPoint, event->pos());
                rect.setHeight(rect.width());
                mPainterBack->drawRect(rect);
                break;
            case DrawingType::SQUARE_BY_CENTER:
                rect = getSquareByCenterAndCornerPoints(mFirstPoint,
                                                        event->pos());
                mPainterBack->drawRect(rect);
                break;
            case DrawingType::RECTANGLE_BY_OPPOSITE_CORNERS:
                mPainterBack->drawRect(getRectByCornerPoints(mFirstPoint,
                                                             event->pos()));
                break;
            case DrawingType::RECTANGLE_BY_CENTER:
                rect = getRectByCenterAndCornerPoints(mFirstPoint, event->pos());
                mPainterBack->drawRect(rect);
                break;
            case DrawingType::ELLIPSE_BY_BOUNDING_RECT:
                rect = getRectByCornerPoints(mFirstPoint, event->pos());
                mPainterBack->drawRect(rect);
                mPainterBack->drawEllipse(rect);
                break;
            case DrawingType::CIRCLE_BY_CENTER_END_RADIUS:
                line.setP1(mFirstPoint);
                line.setP2(event->pos());
                radius = line.length();
                mPainterBack->drawEllipse(mFirstPoint, radius, radius);
                break;
            case DrawingType::CIRCLE_BY_TWO_POINTS:
                mModel.circleByTwoPoints(mFirstPoint, event->pos(), center, radius);
                mPainterBack->drawEllipse(center, radius, radius);
                break;
            case DrawingType::CIRCLE_BY_BOUNDING_RECT:
                rect = getRectByCornerPoints(mFirstPoint, event->pos());
                rect.setHeight(rect.width());
                mPainterBack->drawRect(rect);
                mPainterBack->drawEllipse(rect);
                break;
            default:
                break;
            }
            this->update();
        }
    }

    event->accept();
}

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug() << "mDrawingType = " << static_cast<int>(mDrawingType);
    mPainterBack->fillRect(0,0,mBufferFront->width(), mBufferFront->height(),
                            QBrush(Qt::white));
    mPainterBack->fillRect(0,0,mBufferFront->width(), mBufferFront->height(),
                            QBrush(Qt::transparent));

    if(mFillShapes)
        mPainterFront->setBrush(mBrushColor);
    else
        mPainterFront->setBrush(Qt::NoBrush);

    if(mDrawingType == DrawingType::PEN)
    {
        mEnabled = false;
    }
    //Фигуры которые строятся по трем точками
    else if(mDrawingType == DrawingType::ELLIPSE_BY_CENTER)
    {
        if(mIsFirstPointSelected)
        {
            if(mIsSecondPointSelected)
            {
                //Ellipse semiaxes
                auto rx = qAbs(mFirstPoint.x() - mSecondPoint.x());
                auto ry = qAbs(event->pos().y() - mFirstPoint.y());
                mPainterFront->drawEllipse(mFirstPoint, rx, ry);
                mIsFirstPointSelected = false;
                mIsSecondPointSelected = false;
                emit statusChanged(QString("Select ellipse center"));
            }
            else
            {
                mSecondPoint = event->pos();
                mIsSecondPointSelected = true;
                emit statusChanged(QString("Choose vertical semiaxis lenth"));
            }
            this->update();
        }
        else
        {
            mFirstPoint = event->pos();
            mIsFirstPointSelected = true;
            emit statusChanged(QString("Choose horizontal semiaxis lenth"));
        }
    }
    else if(mDrawingType == DrawingType::CIRCLE_BY_THREE_POINTS)
    {
        if(mIsFirstPointSelected)
        {
            if(mIsSecondPointSelected)
            {
                QPointF thirdPoint(event->pos());
                QPointF center;
                double radius;
                try
                {
                    mModel.circleByThreePoints(mFirstPoint, mSecondPoint,
                                               thirdPoint, center, radius);
                    mPainterFront->drawEllipse(center, radius, radius);
                }
                catch(std::exception &ex)
                {
                    std::cout << ex.what() << std::endl;
                }
                this->update();
                mIsFirstPointSelected = false;
                mIsSecondPointSelected = false;
                emit statusChanged(QString("Select first point"));
            }
            else
            {
                mSecondPoint = event->pos();
                mIsSecondPointSelected = true;
                emit statusChanged(QString("Select third point"));
            }
        }
        else
        {
            mFirstPoint = event->pos();
            mIsFirstPointSelected = true;
            emit statusChanged(QString("Select second point"));
        }
    }
    else if(mDrawingType == DrawingType::PARABOLA_BY_THREE_POINTS)
    {
        if(mIsFirstPointSelected)
        {
            if(mIsSecondPointSelected)
            {
                QVector<QPointF> points;
                QPointF thirdPoint(event->pos());
                double a, b, c;
                mModel.parabolaByThreePoints(mFirstPoint, mSecondPoint, thirdPoint,
                                             a, b, c);
                drawParabola(mPainterFront, mFirstPoint, thirdPoint, a, b, c);

                this->update();
                mIsFirstPointSelected = false;
                mIsSecondPointSelected = false;
                emit statusChanged(QString("Select first point"));
            }
            else
            {
                mSecondPoint = event->pos();
                mIsSecondPointSelected = true;
                emit statusChanged(QString("Select third point"));
            }
        }
        else
        {
            mFirstPoint = event->pos();
            mIsFirstPointSelected = true;
            emit statusChanged(QString("Select second point"));
        }
    }
    else if(mDrawingType == DrawingType::QUADRATIC_BEZIER_CURVE)
    {
        if(mIsFirstPointSelected)
        {
            if(mIsSecondPointSelected)
            {
                QPointF controlPoint(event->pos());
                drawQuadraticBezierCurve(mPainterFront, mFirstPoint,
                                         mSecondPoint, controlPoint);
                this->update();
                mIsFirstPointSelected = false;
                mIsSecondPointSelected = false;
                emit statusChanged(QString("Select first point"));
            }
            else
            {
                mSecondPoint = event->pos();
                mIsSecondPointSelected = true;
                emit statusChanged(QString("Select control point"));
            }
        }
        else
        {
            mFirstPoint = event->pos();
            mIsFirstPointSelected = true;
            emit statusChanged(QString("Select second point"));
        }
    }
    else if(mDrawingType == DrawingType::CUBIC_BEZIER_CURVE)
    {
        if(mIsFirstPointSelected)
        {
            if(mIsSecondPointSelected)
            {
                if(mIsFirstControlPointSelected)
                {
                    QPointF secondControlPoint(event->pos());
                    drawCubicBezierCurve(mPainterFront, mFirstPoint, mSecondPoint,
                                         mFirstControlPoint, secondControlPoint);
                    this->update();
                    mIsFirstPointSelected = false;
                    mIsSecondPointSelected = false;
                    mIsFirstControlPointSelected = false;
                    emit statusChanged(QString("Select first point"));
                }
                else
                {
                    mFirstControlPoint = event->pos();
                    mIsFirstControlPointSelected = true;
                    emit statusChanged(QString("Select second control point"));
                }
            }
            else
            {
                mSecondPoint = event->pos();
                mIsSecondPointSelected = true;
                emit statusChanged(QString("Select first control point"));
            }
        }
        else
        {
            mFirstPoint = event->pos();
            mIsFirstPointSelected = true;
            emit statusChanged(QString("Select second point"));
        }
    }
    else
    {
        //Тут фигуры которые строятся по двум точкам
        QRectF rect;
        if(mIsFirstPointSelected)
        {
            QPointF center, topLeft, bottomRight;
            QLineF line;
            double radius;

            switch (mDrawingType) {
            case DrawingType::LINE:
            case DrawingType::POLYLINE:
                mPainterFront->drawLine(mFirstPoint, event->pos());
                break;
            case DrawingType::RECTANGLE_BY_OPPOSITE_CORNERS:
                rect = getRectByCornerPoints(mFirstPoint, event->pos());
                if(mFillShapes)
                    mPainterFront->fillRect(rect, QBrush(mBrushColor));
                else
                    mPainterFront->drawRect(rect);
                break;
            case DrawingType::RECTANGLE_BY_CENTER:
                rect = getRectByCenterAndCornerPoints(mFirstPoint, event->pos());
                if(mFillShapes)
                    mPainterFront->fillRect(rect, QBrush(mBrushColor));
                else
                    mPainterFront->drawRect(rect);
                break;
            case DrawingType::SQUARE_BY_OPPOSITE_CORNERS:
                rect = getRectByCornerPoints(mFirstPoint, event->pos());
                rect.setHeight(rect.width());
                if(mFillShapes)
                    mPainterFront->fillRect(rect, QBrush(mBrushColor));
                else
                    mPainterFront->drawRect(rect);
                break;
            case DrawingType::SQUARE_BY_CENTER:
                rect = getSquareByCenterAndCornerPoints(mFirstPoint,
                                                        event->pos());
                if(mFillShapes)
                    mPainterFront->fillRect(rect, QBrush(mBrushColor));
                else
                    mPainterFront->drawRect(rect);
                break;
            case DrawingType::ELLIPSE_BY_BOUNDING_RECT:
                rect = getRectByCornerPoints(mFirstPoint, event->pos());
                if(mFillShapes)
                    mPainterFront->fillRect(rect, QBrush(mBrushColor));
                else
                    mPainterFront->drawRect(rect);
                break;
            case DrawingType::CIRCLE_BY_CENTER_END_RADIUS:
                line.setP1(mFirstPoint);
                line.setP2(event->pos());
                radius = line.length();
                mPainterFront->drawEllipse(mFirstPoint, radius, radius);
                break;
            case DrawingType::CIRCLE_BY_TWO_POINTS:
                mModel.circleByTwoPoints(mFirstPoint, event->pos(), center, radius);
                mPainterFront->drawEllipse(center, radius, radius);
                break;
            case DrawingType::CIRCLE_BY_BOUNDING_RECT:
                rect = getRectByCornerPoints(mFirstPoint, event->pos());
                rect.setHeight(rect.width());
                mPainterFront->drawEllipse(rect);
                break;
            default:
                break;
            }

            if((mDrawingType != DrawingType::POLYLINE) ||
               (mDrawingType == DrawingType::POLYLINE && event->button() &
                Qt::RightButton))
            {
                mIsFirstPointSelected = false;
                emit statusChanged(QString("Select first point"));
            }
            else
            {
                mFirstPoint = event->pos();
                emit statusChanged(QString("Select next point"));
            }
            this->update();
        }
        else
        {
            mFirstPoint = event->pos();
            mIsFirstPointSelected = true;
            if(mDrawingType == DrawingType::POLYLINE)
            {
                emit statusChanged(QString("Select next point"));
            }
            else
                emit statusChanged(QString("Select second point"));
        }
    }
}

QRectF Canvas::getRectByCornerPoints(const QPointF &topLeft,
                                     const QPointF &bottomRight)
{
    return QRectF(topLeft.x() < bottomRight.x() ?
                  topLeft.x() : bottomRight.x(),
                  topLeft.y() < bottomRight.y() ?
                  topLeft.y() : bottomRight.y(),
                  qAbs(topLeft.x() - bottomRight.x()),
                  qAbs(topLeft.y() - bottomRight.y()));
}

QRectF Canvas::getRectByCenterAndCornerPoints(const QPointF &center,
                                              const QPointF &cornerPoint)
{
    QPointF topLeft, bottomRight;
    QLineF line;
    line.setP1(center);
    line.setP2(cornerPoint);
    topLeft.setX(center.x() - qAbs(line.dx()));
    topLeft.setY(center.y() - qAbs(line.dy()));
    bottomRight.setX(center.x() + qAbs(line.dx()));
    bottomRight.setY(center.y() + qAbs(line.dy()));
    return QRectF(topLeft, bottomRight);
}

QRectF Canvas::getSquareByCenterAndCornerPoints(const QPointF &center,
                                                const QPointF &cornerPoint)
{
    auto rect = getRectByCenterAndCornerPoints(center, cornerPoint);
    auto R = qMin(rect.width(), rect.height()) / 2;
    return QRectF(QPointF(center.x() - R, center.y() - R),
                  QPointF(center.x() + R, center.y() + R));
}

void Canvas::drawParabola(QPainter *painter, const QPointF &begin,
                          const QPointF &end, double a, double b, double c)
{
    QPainterPath path(begin);
    for(auto x = begin.x(); x <= end.x(); x += 0.005)
        path.lineTo(QPoint(x, mModel.squareParabolaFunc(a, b, c, x)));
    painter->drawPath(path);
}

void Canvas::drawQuadraticBezierCurve(QPainter *painter, const QPointF &begin,
                                      const QPointF &end, const QPointF &control)
{
    QPainterPath path(begin);
    path.quadTo(control, end);
    painter->drawPath(path);
}

void Canvas::drawCubicBezierCurve(QPainter *painter, const QPointF &begin,
                                  const QPointF &end, const QPointF &control1,
                                  const QPointF &control2)
{
    QPainterPath path(begin);
    path.cubicTo(control1, control2, end);
    painter->drawPath(path);
}
