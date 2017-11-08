#include "model.h"
#include <QtMath>
#include <iostream>

Model::Model()
{

}

void Model::parabolaByThreePoints(const QPointF &p1, const QPointF &p2,
                                  const QPointF &p3, double &a, double &b,
                                  double &c)
{
    /*
    a * x1^2 + b * x1 + c = y1
    a * x2^2 + b * x2 + c = y2
    a * x3^2 + b * x3 + c = y3
   */
    iat::Matrix<double> lhs(3, 1.0f);
    lhs.setValueAt(0, 0, p1.x() * p1.x());
    lhs.setValueAt(1, 0, p2.x() * p2.x());
    lhs.setValueAt(2, 0, p3.x() * p3.x());

    lhs.setValueAt(0, 1, p1.x());
    lhs.setValueAt(1, 1, p2.x());
    lhs.setValueAt(2, 1, p3.x());

    std::cout << "lhs:\n" << lhs << std::endl;

    iat::Matrix<double> rhs(3, 1, 1.0f);
    rhs.setValueAt(0, 0, p1.y());
    rhs.setValueAt(1, 0, p2.y());
    rhs.setValueAt(2, 0, p3.y());

    std::cout << "rhs:\n" << rhs << std::endl;

    iat::Matrix<double> result = lhs.solveEquationSystem(rhs);
    a = result.getValueAt(0, 0);
    b = result.getValueAt(1, 0);
    c = result.getValueAt(2, 0);
    std::cout << "result:\n" << result << std::endl;
}

double Model::squareParabolaFunc(double a, double b, double c, double x)
{
    return a * x * x + b * x + c;
}

void Model::circleByTwoPoints(const QPointF &p1, const QPointF &p2,
                              QPointF &center, double &radius)
{
    center.setX((p1.x() + p2.x()) / 2);
    center.setY((p1.y() + p2.y()) / 2);
    radius = qSqrt((p1.x() - center.x()) * (p1.x() - center.x()) +
                   (p1.y() - center.y()) * (p1.y() - center.y()));
}

void Model::circleByThreePoints(const QPointF &p1, const QPointF &p2,
                                const QPointF &p3, QPointF &center,
                                double &radius)
{
    /*
    2 * (x3 - x2) * Xc + 2 * (y3 - y2) * Yc = x3^2 + y3^2 - x2^2 - y2^2
    2 * (x3 - x1) * Xc + 2 * (y3 - y1) * Yc = x3^2 + y3^2 - x1^2 - y1^2
   */
    iat::Matrix<double> lhs(2, 1.0f);
    lhs.setValueAt(0, 0, 2 * (p3.x() - p2.x()));
    lhs.setValueAt(1, 0, 2 * (p3.x() - p1.x()));
    lhs.setValueAt(0, 1, 2 * (p3.y() - p2.y()));
    lhs.setValueAt(1, 1, 2 * (p3.y() - p1.y()));

    std::cout << "lhs:\n" << lhs << std::endl;

    iat::Matrix<double> rhs(2, 1, 1.0f);
    rhs.setValueAt(0, 0, p3.x() * p3.x() + p3.y() * p3.y() - p2.x() * p2.x() -
                   p2.y() * p2.y());
    rhs.setValueAt(1, 0, p3.x() * p3.x() + p3.y() * p3.y() - p1.x() * p1.x() -
                   p1.y() * p1.y());

    std::cout << "rhs:\n" << rhs << std::endl;

    iat::Matrix<double> result = lhs.solveEquationSystem(rhs);
    double xc = result.getValueAt(0, 0);
    double yc = result.getValueAt(1, 0);
    std::cout << "result:\n" << result << std::endl;
    center.setX(xc);
    center.setY(yc);
    radius = qSqrt((p1.x() - xc) * (p1.x() - xc) + (p1.y() - yc) * (p1.y() - yc));
}

QPointF Model::circlePoint(const QPointF &center, double radius,
                           double angle_rad)
{
    return QPointF(center.x() + radius * qCos(angle_rad),
                   center.y() + radius * qSin(angle_rad));
}
