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
};

#endif // MODEL_H
