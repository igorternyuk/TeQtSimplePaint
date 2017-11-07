#include "model.h"
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
