#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QPointF>
#include "model.h"

int main(int argc, char *argv[])
{
    /*Model model;
    QPointF p1(-3, 9);
    QPointF p2(1, 1);
    QPointF p3(5, 25);
    std::cout << "sizeof(p1) = " << sizeof(p1) << std::endl;
    double a, b, c;
    model.parabolaByThreePoints(p1, p2, p3, a, b, c);*/
    QApplication app(argc, argv);
    app.setStyle("fusion");
    QRect desktop = QApplication::desktop()->geometry();
    MainWindow w;
    auto dx = (desktop.width() - w.width()) / 2;
    auto dy = (desktop.height() - w.height()) / 2;
    w.move(dx, dy);
    w.show();

    return app.exec();
}
