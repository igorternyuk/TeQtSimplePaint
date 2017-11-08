#ifndef MODEL_H
#define MODEL_H

#include "matrix.h"
#include <QPointF>

class Model
{
public:
    explicit Model();
    static void parabolaByThreePoints(const QPointF &p1, const QPointF &p2,
                                      const QPointF &p3, double &a, double &b,
                                      double &c);
    static double squareParabolaFunc(double a, double b, double c, double x);
    static void circleByTwoPoints(const QPointF &p1, const QPointF &p2,
                                  QPointF &center, double &radius);
    static void circleByThreePoints(const QPointF &p1, const QPointF &p2,
                                    const QPointF &p3, QPointF &center,
                                    double &radius);
    /**
      Calculates circle points according to the equations
      x = R * cos(alpha)
      y = R * sin(alpha)
    */
    static QPointF circlePoint(const QPointF &center, double radius,
                               double angle_rad);
};

#endif // MODEL_H
