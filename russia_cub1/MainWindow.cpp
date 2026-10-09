#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <qtimer.h>
#include <QShortcut>
#include <gameData.h>
#include <gameFunctions.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->endButton->setShortcut(QKeySequence("Q"));
    ui->pauseButton->setShortcut(QKeySequence(" "));
    ui->leftMoveButton->setShortcut(QKeySequence("A"));
    ui->rightMoveButton->setShortcut(QKeySequence("D"));
    ui->rotateButton->setShortcut(QKeySequence("R"));
    ui->setButton->setShortcut(QKeySequence("S"));
    ui->Paused->setVisible(false);
    ui->game_Over->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::gameStart()
{
    sumscore = 0;
    show_new_map();
    ui->scoreShow->display(sumscore);
    random_cub(&cub);
    random_cub(&next_cub);
    QPoint pos=ui->thisBlock->pos();
    ui->thisBlock->move(cub.position*29+9,pos.y());
    show_nextcub();
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
            show_new_thiscub(i,j);
    }

    ui->pauseButton->setEnabled(true);
    ui->leftMoveButton->setEnabled(true);
    ui->rightMoveButton->setEnabled(true);
    ui->rotateButton->setEnabled(true);
    ui->setButton->setEnabled(true);
    ui->endButton->setEnabled(true);
    ui->startButton->setEnabled(false);
    ui->Paused->setVisible(false);
    ui->game_Over->setVisible(false);
}

void MainWindow::gameEnd()
{
    ui->pauseButton->setEnabled(false);
    ui->leftMoveButton->setEnabled(false);
    ui->rightMoveButton->setEnabled(false);
    ui->rotateButton->setEnabled(false);
    ui->setButton->setEnabled(false);
    ui->game_Over->setVisible(true);
    ui->last_score->setText(QString::number(sumscore));
}

void MainWindow::on_startButton_clicked()
{
    //开启游戏进程（函数封装）
    //随机生成两个方块
    gameStart();
}


void MainWindow::on_pauseButton_clicked()
{
    //对事件的侦听状态取反（考虑设置某个布尔值）
    bool buttonstate=ui->leftMoveButton->isEnabled();
    ui->leftMoveButton->setEnabled(!buttonstate);
    ui->rightMoveButton->setEnabled(!buttonstate);
    ui->rotateButton->setEnabled(!buttonstate);
    ui->setButton->setEnabled(!buttonstate);
    ui->Paused->setVisible(buttonstate);
}


void MainWindow::on_endButton_clicked()
{
    //结束进程，弹出结算窗口
    gameEnd();
}

void MainWindow::on_leftMoveButton_clicked()
{
    //调用左移函数,将结果传给moved
    int moved = leftMove_cub();
    if(moved)
    {
        QPoint pos=ui->thisBlock->pos();
        ui->thisBlock->move(pos.x()-29,pos.y());
    }
}

void MainWindow::on_rightMoveButton_clicked()
{
    //调用右移函数,将结果传给moved
    int moved = rightMove_cub();
    if(moved)
    {
        QPoint pos=ui->thisBlock->pos();
        ui->thisBlock->move(pos.x()+29,pos.y());
    }
}

void MainWindow::on_rotateButton_clicked()
{
    //调用旋转函数
    rotate_cub();
    QPoint pos=ui->thisBlock->pos();
    ui->thisBlock->move(cub.position*29+9,pos.y());
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
            show_new_thiscub(i,j);
    }
}

void MainWindow::on_setButton_clicked()
{

    //调用放置函数,并更新地图
    set_cub();
    show_new_map();
    //更新两个方块
    cub=next_cub;
    random_cub(&next_cub);
    //更新对应小块的样式
    show_nextcub();
    QPoint pos=ui->thisBlock->pos();
    ui->thisBlock->move(cub.position*29+9,pos.y());
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
            show_new_thiscub(i,j);
    }

    //调用检查得分函数
    int thisScore=clear_ful();
    //若此轮得分不为零：
    if(thisScore)
    {
        QTimer::singleShot(500,this,[this](){show_new_map();});
        sumscore += thisScore;
        ui->scoreShow->display(sumscore);
        //更新地图
    }
    //检查是否结束
    if(mapp[DIE])
    {
        gameEnd();
    }
}

void MainWindow::on_scoreShow_overflow()
{
    //将关联的score变量重置为0
    sumscore = 0;
    ui->scoreShow->display(sumscore);
}

void MainWindow::show_new_map()
{
    for(int i=0;i<Y;i++)
    {
        for(int j=0;j<X;j++)
        {
            int state=get_mapcub_data(i,j);//获取该坐标的状态，此函数放在功能文件中
            QWidget *thecub=match_mapcub_qwidget(i,j);//将坐标与板块一一对应
            change_cub_show(thecub,state);
        }
    }
}

void MainWindow::show_new_thiscub(int y,int x)
{
    int state=get_thiscub_data(y,x);//获取该坐标的状态，此函数放在功能文件中
    QWidget *thecub=match_thiscub_qwidget(y,x);//将坐标与板块一一对应
    change_cub_show(thecub,state);
}

void MainWindow::show_nextcub()
{
    switch (next_cub.type) {
    case 'I':
        ui->next->setStyleSheet("image: url(:/new/prefix1/sorce/I.png);\nborder: 2px solid black;\nbackground-color: rgb(147, 147, 147);");
        break;
    case 'J':
        ui->next->setStyleSheet("image: url(:/new/prefix1/sorce/J.png);\nborder: 2px solid black;\nbackground-color: rgb(147, 147, 147);");
        break;
    case 'O':
        ui->next->setStyleSheet("image: url(:/new/prefix1/sorce/O.png);\nborder: 2px solid black;\nbackground-color: rgb(147, 147, 147);");
        break;
    case 'L':
        ui->next->setStyleSheet("image: url(:/new/prefix1/sorce/L.png);\nborder: 2px solid black;\nbackground-color: rgb(147, 147, 147);");
        break;
    case 'S':
        ui->next->setStyleSheet("image: url(:/new/prefix1/sorce/S.png);\nborder: 2px solid black;\nbackground-color: rgb(147, 147, 147);");
        break;
    case 'Z':
        ui->next->setStyleSheet("image: url(:/new/prefix1/sorce/Z.png);\nborder: 2px solid black;\nbackground-color: rgb(147, 147, 147);");
        break;
    case 'T':
        ui->next->setStyleSheet("image: url(:/new/prefix1/sorce/T.png);\nborder: 2px solid black;\nbackground-color: rgb(147, 147, 147);");
        break;
    default:
        break;
    }
}

void MainWindow::change_cub_show(QWidget *thecub,int state)
{
    if(state){
        thecub ->setStyleSheet("background-color:rgb(190, 190, 190);\nborder: 1px solid grey;");
    }
    else{
        if(thecub->parent()==ui->thisBlock)
            thecub ->setStyleSheet("border: 0px;");
        else if(thecub->pos().y()<=155)
            thecub ->setStyleSheet("border: 1px solid rgb(237, 136, 141);");
        else
            thecub ->setStyleSheet("border: 1px solid grey;");
    }
}

QWidget* MainWindow::match_mapcub_qwidget(int y,int x)
{
    switch (y) {
    case 0:
        switch (x) {
        case 0:
            return ui->m0_0;
            break;
        case 1:
            return ui->m0_1;
            break;
        case 2:
            return ui->m0_2;
            break;
        case 3:
            return ui->m0_3;
            break;
        case 4:
            return ui->m0_4;
            break;
        case 5:
            return ui->m0_5;
            break;
        case 6:
            return ui->m0_6;
            break;
        case 7:
            return ui->m0_7;
            break;
        case 8:
            return ui->m0_8;
            break;
        case 9:
            return ui->m0_9;
            break;
        default:
            break;
        }
        break;
    case 1:
        switch (x) {
        case 0:
            return ui->m1_0;
            break;
        case 1:
            return ui->m1_1;
            break;
        case 2:
            return ui->m1_2;
            break;
        case 3:
            return ui->m1_3;
            break;
        case 4:
            return ui->m1_4;
            break;
        case 5:
            return ui->m1_5;
            break;
        case 6:
            return ui->m1_6;
            break;
        case 7:
            return ui->m1_7;
            break;
        case 8:
            return ui->m1_8;
            break;
        case 9:
            return ui->m1_9;
            break;
        default:
            break;
        }
        break;
    case 2:
        switch (x) {
        case 0:
            return ui->m2_0;
            break;
        case 1:
            return ui->m2_1;
            break;
        case 2:
            return ui->m2_2;
            break;
        case 3:
            return ui->m2_3;
            break;
        case 4:
            return ui->m2_4;
            break;
        case 5:
            return ui->m2_5;
            break;
        case 6:
            return ui->m2_6;
            break;
        case 7:
            return ui->m2_7;
            break;
        case 8:
            return ui->m2_8;
            break;
        case 9:
            return ui->m2_9;
            break;
        default:
            break;
        }
        break;
    case 3:
        switch (x) {
        case 0:
            return ui->m3_0;
            break;
        case 1:
            return ui->m3_1;
            break;
        case 2:
            return ui->m3_2;
            break;
        case 3:
            return ui->m3_3;
            break;
        case 4:
            return ui->m3_4;
            break;
        case 5:
            return ui->m3_5;
            break;
        case 6:
            return ui->m3_6;
            break;
        case 7:
            return ui->m3_7;
            break;
        case 8:
            return ui->m3_8;
            break;
        case 9:
            return ui->m3_9;
            break;
        default:
            break;
        }
        break;
    case 4:
        switch (x) {
        case 0:
            return ui->m4_0;
            break;
        case 1:
            return ui->m4_1;
            break;
        case 2:
            return ui->m4_2;
            break;
        case 3:
            return ui->m4_3;
            break;
        case 4:
            return ui->m4_4;
            break;
        case 5:
            return ui->m4_5;
            break;
        case 6:
            return ui->m4_6;
            break;
        case 7:
            return ui->m4_7;
            break;
        case 8:
            return ui->m4_8;
            break;
        case 9:
            return ui->m4_9;
            break;
        default:
            break;
        }
        break;
    case 5:
        switch (x) {
        case 0:
            return ui->m5_0;
            break;
        case 1:
            return ui->m5_1;
            break;
        case 2:
            return ui->m5_2;
            break;
        case 3:
            return ui->m5_3;
            break;
        case 4:
            return ui->m5_4;
            break;
        case 5:
            return ui->m5_5;
            break;
        case 6:
            return ui->m5_6;
            break;
        case 7:
            return ui->m5_7;
            break;
        case 8:
            return ui->m5_8;
            break;
        case 9:
            return ui->m5_9;
            break;
        default:
            break;
        }
        break;
    case 6:
        switch (x) {
        case 0:
            return ui->m6_0;
            break;
        case 1:
            return ui->m6_1;
            break;
        case 2:
            return ui->m6_2;
            break;
        case 3:
            return ui->m6_3;
            break;
        case 4:
            return ui->m6_4;
            break;
        case 5:
            return ui->m6_5;
            break;
        case 6:
            return ui->m6_6;
            break;
        case 7:
            return ui->m6_7;
            break;
        case 8:
            return ui->m6_8;
            break;
        case 9:
            return ui->m6_9;
            break;
        default:
            break;
        }
        break;
    case 7:
        switch (x) {
        case 0:
            return ui->m7_0;
            break;
        case 1:
            return ui->m7_1;
            break;
        case 2:
            return ui->m7_2;
            break;
        case 3:
            return ui->m7_3;
            break;
        case 4:
            return ui->m7_4;
            break;
        case 5:
            return ui->m7_5;
            break;
        case 6:
            return ui->m7_6;
            break;
        case 7:
            return ui->m7_7;
            break;
        case 8:
            return ui->m7_8;
            break;
        case 9:
            return ui->m7_9;
            break;
        default:
            break;
        }
        break;
    case 8:
        switch (x) {
        case 0:
            return ui->m8_0;
            break;
        case 1:
            return ui->m8_1;
            break;
        case 2:
            return ui->m8_2;
            break;
        case 3:
            return ui->m8_3;
            break;
        case 4:
            return ui->m8_4;
            break;
        case 5:
            return ui->m8_5;
            break;
        case 6:
            return ui->m8_6;
            break;
        case 7:
            return ui->m8_7;
            break;
        case 8:
            return ui->m8_8;
            break;
        case 9:
            return ui->m8_9;
            break;
        default:
            break;
        }
        break;
    case 9:
        switch (x) {
        case 0:
            return ui->m9_0;
            break;
        case 1:
            return ui->m9_1;
            break;
        case 2:
            return ui->m9_2;
            break;
        case 3:
            return ui->m9_3;
            break;
        case 4:
            return ui->m9_4;
            break;
        case 5:
            return ui->m9_5;
            break;
        case 6:
            return ui->m9_6;
            break;
        case 7:
            return ui->m9_7;
            break;
        case 8:
            return ui->m9_8;
            break;
        case 9:
            return ui->m9_9;
            break;
        default:
            break;
        }
        break;
    case 10:
        switch (x) {
        case 0:
            return ui->m10_0;
            break;
        case 1:
            return ui->m10_1;
            break;
        case 2:
            return ui->m10_2;
            break;
        case 3:
            return ui->m10_3;
            break;
        case 4:
            return ui->m10_4;
            break;
        case 5:
            return ui->m10_5;
            break;
        case 6:
            return ui->m10_6;
            break;
        case 7:
            return ui->m10_7;
            break;
        case 8:
            return ui->m10_8;
            break;
        case 9:
            return ui->m10_9;
            break;
        default:
            break;
        }
        break;
    case 11:
        switch (x) {
        case 0:
            return ui->m11_0;
            break;
        case 1:
            return ui->m11_1;
            break;
        case 2:
            return ui->m11_2;
            break;
        case 3:
            return ui->m11_3;
            break;
        case 4:
            return ui->m11_4;
            break;
        case 5:
            return ui->m11_5;
            break;
        case 6:
            return ui->m11_6;
            break;
        case 7:
            return ui->m11_7;
            break;
        case 8:
            return ui->m11_8;
            break;
        case 9:
            return ui->m11_9;
            break;
        default:
            break;
        }
        break;
    case 12:
        switch (x) {
        case 0:
            return ui->m12_0;
            break;
        case 1:
            return ui->m12_1;
            break;
        case 2:
            return ui->m12_2;
            break;
        case 3:
            return ui->m12_3;
            break;
        case 4:
            return ui->m12_4;
            break;
        case 5:
            return ui->m12_5;
            break;
        case 6:
            return ui->m12_6;
            break;
        case 7:
            return ui->m12_7;
            break;
        case 8:
            return ui->m12_8;
            break;
        case 9:
            return ui->m12_9;
            break;
        default:
            break;
        }
        break;
    case 13:
        switch (x) {
        case 0:
            return ui->m13_0;
            break;
        case 1:
            return ui->m13_1;
            break;
        case 2:
            return ui->m13_2;
            break;
        case 3:
            return ui->m13_3;
            break;
        case 4:
            return ui->m13_4;
            break;
        case 5:
            return ui->m13_5;
            break;
        case 6:
            return ui->m13_6;
            break;
        case 7:
            return ui->m13_7;
            break;
        case 8:
            return ui->m13_8;
            break;
        case 9:
            return ui->m13_9;
            break;
        default:
            break;
        }
        break;
    case 14:
        switch (x) {
        case 0:
            return ui->m14_0;
            break;
        case 1:
            return ui->m14_1;
            break;
        case 2:
            return ui->m14_2;
            break;
        case 3:
            return ui->m14_3;
            break;
        case 4:
            return ui->m14_4;
            break;
        case 5:
            return ui->m14_5;
            break;
        case 6:
            return ui->m14_6;
            break;
        case 7:
            return ui->m14_7;
            break;
        case 8:
            return ui->m14_8;
            break;
        case 9:
            return ui->m14_9;
            break;
        default:
            break;
        }
        break;
    case 15:
        switch (x) {
        case 0:
            return ui->m15_0;
            break;
        case 1:
            return ui->m15_1;
            break;
        case 2:
            return ui->m15_2;
            break;
        case 3:
            return ui->m15_3;
            break;
        case 4:
            return ui->m15_4;
            break;
        case 5:
            return ui->m15_5;
            break;
        case 6:
            return ui->m15_6;
            break;
        case 7:
            return ui->m15_7;
            break;
        case 8:
            return ui->m15_8;
            break;
        case 9:
            return ui->m15_9;
            break;
        default:
            break;
        }
        break;
    case 16:
        switch (x) {
        case 0:
            return ui->m16_0;
            break;
        case 1:
            return ui->m16_1;
            break;
        case 2:
            return ui->m16_2;
            break;
        case 3:
            return ui->m16_3;
            break;
        case 4:
            return ui->m16_4;
            break;
        case 5:
            return ui->m16_5;
            break;
        case 6:
            return ui->m16_6;
            break;
        case 7:
            return ui->m16_7;
            break;
        case 8:
            return ui->m16_8;
            break;
        case 9:
            return ui->m16_9;
            break;
        default:
            break;
        }
        break;
    case 17:
        switch (x) {
        case 0:
            return ui->m17_0;
            break;
        case 1:
            return ui->m17_1;
            break;
        case 2:
            return ui->m17_2;
            break;
        case 3:
            return ui->m17_3;
            break;
        case 4:
            return ui->m17_4;
            break;
        case 5:
            return ui->m17_5;
            break;
        case 6:
            return ui->m17_6;
            break;
        case 7:
            return ui->m17_7;
            break;
        case 8:
            return ui->m17_8;
            break;
        case 9:
            return ui->m17_9;
            break;
        default:
            break;
        }
        break;
    case 18:
        switch (x) {
        case 0:
            return ui->m18_0;
            break;
        case 1:
            return ui->m18_1;
            break;
        case 2:
            return ui->m18_2;
            break;
        case 3:
            return ui->m18_3;
            break;
        case 4:
            return ui->m18_4;
            break;
        case 5:
            return ui->m18_5;
            break;
        case 6:
            return ui->m18_6;
            break;
        case 7:
            return ui->m18_7;
            break;
        case 8:
            return ui->m18_8;
            break;
        case 9:
            return ui->m18_9;
            break;
        default:
            break;
        }
        break;
    case 19:
        switch (x) {
        case 0:
            return ui->m19_0;
            break;
        case 1:
            return ui->m19_1;
            break;
        case 2:
            return ui->m19_2;
            break;
        case 3:
            return ui->m19_3;
            break;
        case 4:
            return ui->m19_4;
            break;
        case 5:
            return ui->m19_5;
            break;
        case 6:
            return ui->m19_6;
            break;
        case 7:
            return ui->m19_7;
            break;
        case 8:
            return ui->m19_8;
            break;
        case 9:
            return ui->m19_9;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
    return NULL;
}

QWidget* MainWindow::match_thiscub_qwidget(int y,int x)
{
    switch (y) {
    case 0:
        switch (x) {
        case 0:
            return ui->t0_0;
            break;
        case 1:
            return ui->t0_1;
            break;
        case 2:
            return ui->t0_2;
            break;
        case 3:
            return ui->t0_3;
            break;
        default:
            break;
        }
        break;
    case 1:
        switch (x) {
        case 0:
            return ui->t1_0;
            break;
        case 1:
            return ui->t1_1;
            break;
        case 2:
            return ui->t1_2;
            break;
        case 3:
            return ui->t1_3;
            break;
        default:
            break;
        }
        break;
    case 2:
        switch (x) {
        case 0:
            return ui->t2_0;
            break;
        case 1:
            return ui->t2_1;
            break;
        case 2:
            return ui->t2_2;
            break;
        case 3:
            return ui->t2_3;
            break;
        default:
            break;
        }
        break;
    case 3:
        switch (x) {
        case 0:
            return ui->t3_0;
            break;
        case 1:
            return ui->t3_1;
            break;
        case 2:
            return ui->t3_2;
            break;
        case 3:
            return ui->t3_3;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
    return NULL;
}

void MainWindow::on_restartButton_clicked()
{

    for(int i=0;i<Y;i++)
        mapp[i]=0x0;
    gameStart();
}


void MainWindow::on_exitButton_clicked()
{
    this->close();
}

