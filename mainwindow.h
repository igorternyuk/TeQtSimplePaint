#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPointF>
#include <QPen>
#include "model.h"

namespace Ui
{
    class MainWindow;
}

class QPainter;
class QImage;
class QLabel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
private:
    Model mModel;
    Ui::MainWindow *ui;    
    QImage *mBufferFront, *mBufferBack;
    QPainter *mPainterFront, *mPainterBack;
    QPointF mFirstPoint, mSecondPoint;
    QColor mPenColor;
    QPen mPen;
    bool mEnabled {false};
    bool mIsFirstPointSelected{false};
    bool mIsSecondPointSelected{false};
    QLabel *mLabelStatus;
    QLabel *mLabelCursor;

    void drawParabola(QPainter *painter, const QPointF &begin,
                      const QPointF &end, double a, double b, double c);
};

#endif // MAINWINDOW_H
