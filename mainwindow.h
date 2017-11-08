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
signals:
    void brushColorChanged(QColor color);
    void brushSizeChanged(int size);
protected:
    void mouseMoveEvent(QMouseEvent *event) override;
private slots:
    void on_action_open_file_triggered();
    void on_action_save_file_triggered();

    void on_action_quit_triggered();

private:
    Ui::MainWindow *ui;
    QLabel *mLabelStatus;
    QLabel *mLabelCursor;

};

#endif // MAINWINDOW_H
