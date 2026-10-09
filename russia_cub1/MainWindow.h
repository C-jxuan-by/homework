#ifndef MainWindow_H
#define MainWindow_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void gameStart();

    void gameEnd();

    void on_startButton_clicked();

    void on_pauseButton_clicked();

    void on_endButton_clicked();

    void on_scoreShow_overflow();

    void on_leftMoveButton_clicked();

    void on_rightMoveButton_clicked();

    void on_rotateButton_clicked();

    void on_setButton_clicked();

    void show_new_map();

    void show_new_thiscub(int y,int x);

    void show_nextcub();

    void change_cub_show(QWidget *thecub,int state);

    QWidget* match_mapcub_qwidget(int y,int x);

    QWidget* match_thiscub_qwidget(int y,int x);
    void on_restartButton_clicked();

    void on_exitButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MainWindow_H
