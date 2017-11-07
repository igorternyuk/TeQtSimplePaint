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
protected:
    void paintEvent(QPaintEvent *event) override;
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
        SQUARE,
        CIRCLE_BY_BOUNDING_RECT,
        CIRCLE_BY_CENTER_END_RADIUS,
        CIRCLE_BY_TWO_POINTS,
        CIRCLE_BY_THREE_POINTS,
        ELLIPSE,
        ARC_BY_THREE_POINTS,
        PARABOLA_BY_THREE_POINTS
    };
    enum class DrawingMode
    {
        FILL_SHAPES,
        ONLY_CONTOURS
    };

    Model mModel;
    QImage *mBufferFront, *mBufferBack;
    QPainter *mPainterFront, *mPainterBack;
    QPointF mFirstPoint, mSecondPoint;
    QColor mPenColor;
    QPen mPenBackground, mPenForeground;
    bool mEnabled {false};
    bool mIsFirstPointSelected{false};
    bool mIsSecondPointSelected{false};

    void drawParabola(QPainter *painter, const QPointF &begin,
                      const QPointF &end, double a, double b, double c);

};

#endif // CANVAS_H
