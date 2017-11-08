#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QString>
#include <QPointF>
#include <QPen>
#include "model.h"

class QImage;
class QPainter;

class Canvas : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas(QWidget *parent = nullptr);
    virtual ~Canvas();
signals:
    void cursorPosChanged(QPointF pos);
    void statusChanged(QString msg);
public slots:
    void changeDrawingType(int type);
    void changeBrushColor(QColor color);
    void changeBrushSize(int size);
    void setShapeFill(bool fill);
    void openFile();
    void saveFile();
protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
private:
    enum class DrawingType
    {
        PEN,
        LINE,
        POLYLINE,
        RECTANGLE_BY_OPPOSITE_CORNERS,
        RECTANGLE_BY_CENTER,
        SQUARE_BY_OPPOSITE_CORNERS,
        SQUARE_BY_CENTER,
        CIRCLE_BY_BOUNDING_RECT,
        CIRCLE_BY_CENTER_END_RADIUS,
        CIRCLE_BY_TWO_POINTS,
        CIRCLE_BY_THREE_POINTS,
        ELLIPSE_BY_BOUNDING_RECT,
        ELLIPSE_BY_CENTER,
        PARABOLA_BY_THREE_POINTS,
        QUADRATIC_BEZIER_CURVE,
        CUBIC_BEZIER_CURVE
    };
    enum class DrawingMode
    {
        FILL_SHAPES,
        ONLY_CONTOURS
    };

    Model mModel;
    QImage *mBufferFront, *mBufferBack;
    QPainter *mPainterFront, *mPainterBack;
    DrawingType mDrawingType{ DrawingType::PEN };
    QPointF mFirstPoint, mSecondPoint, mFirstControlPoint;
    QColor mBrushColor{Qt::darkGreen};
    QPen mPenBackground, mPenForeground;
    bool mEnabled {false};
    bool mIsFirstPointSelected{false};
    bool mIsSecondPointSelected{false};
    bool mIsFirstControlPointSelected{false};
    bool mFillShapes{false};
    QRectF getRectByCornerPoints(const QPointF &topLeft, const QPointF &bottomRight);
    QRectF getRectByCenterAndCornerPoints(const QPointF &center, const QPointF &cornerPoint);
    QRectF getSquareByCenterAndCornerPoints(const QPointF &center,
                                            const QPointF &cornerPoint);
    void drawParabola(QPainter *painter, const QPointF &begin,
                      const QPointF &end, double a, double b, double c);
    void drawQuadraticBezierCurve(QPainter *painter, const QPointF &begin,
                                  const QPointF &end, const QPointF &control);
    void drawCubicBezierCurve(QPainter *painter, const QPointF &begin,
                              const QPointF &end, const QPointF &control1,
                              const QPointF &control2);


};

#endif // CANVAS_H
