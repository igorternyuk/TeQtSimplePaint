#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui
{
    class MainWindow;
}

class QImage;
class QLabel;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    void mouseMoveEvent(QMouseEvent *event) override;
private:
    Ui::MainWindow *ui;
    QLabel *mLabelStatus;
    QLabel *mLabelCursor;

};

#endif // MAINWINDOW_H
