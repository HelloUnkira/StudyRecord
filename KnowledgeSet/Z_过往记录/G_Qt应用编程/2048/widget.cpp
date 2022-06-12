#include <string>
#include <QDebug>
#include <QPainter>
#include <QTimer>
#include <QKeyEvent>
#include <ctime>
#include <QFont>
#include "widget.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    //固定窗口大小
    setFixedSize(max_width, max_height);
    //设置标题
    setWindowTitle("2048v1.0");
    //设置背景图
    setAutoFillBackground(true);
    QPalette palette;
    //设置为纯色
    setPalette(Qt::white);
    //将背景图片设置好，相对路径是以项目的 build-Snake... 这个文件夹为基准
    palette.setBrush(QPalette::Background, QBrush(QPixmap(\
                                "../2048/picture/background.jpg")));
    setPalette(palette);

    //设置定时器，做闹钟
    //超时后发送信号 ---》 到槽函数
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
    timer->start(overtime);//超时时间
    timer->stop();

    //初始化矩阵
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            table[i][j] = 0;

    srand((unsigned int)(time(NULL)));
    //刷俩个格子
    if (16 - used_cell > 0)
        table[rand() % 4][rand() % 4] = 2;
    used_cell++;
    if (16 - used_cell > 0)
        table[rand() % 4][rand() % 4] = 2;
    used_cell++;

}

Widget::~Widget()
{
}

void Widget::paintEvent(QPaintEvent *event)
{
    //画家
    //画图对象，直到画的界面为当前界面
    QPainter painter(this);
    //画图片
    //QPixmap pix;
    //pix.load("../snake/background.jpg");
    //painter.drawPixmap(0,0,800,600,pix);
    //画笔
    QPen pen;
    pen.setColor(Qt::black);
    painter.setPen(pen);
    //画刷
    QBrush brush;
    brush.setColor(Qt::red);
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);

    //quit
    if (success == true && dont_stop == false) {

    } else if (quit == true && dont_stop == false) {
        //画图片
        QPixmap pix;
        pix.load(QString::fromStdString(\
                     "../2048/picture/game_over.bmp"));
        painter.drawPixmap(offset + cell_height * 2, offset, 400, 400, pix);
    } else {
        //画格子即可了
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++) {
               //画图片
               QPixmap pix;
               pix.load(QString::fromStdString("../2048/picture/" + \
                    std::to_string((int)table[i][j]) + ".bmp"));
               painter.drawPixmap(offset + cell_height * 2 + j * cell_height, \
                    offset + i * cell_width, cell_height, cell_width, pix);
            }
    }
        //画分数
        QFont font("方正舒体", 30, QFont::ExtraLight,false);
        painter.setFont(font);
        painter.drawText(0, 50, QString::fromStdString(
                             "分数：" + std::to_string(scord)));
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    int temp = 0;
    switch (event->key()) {
    case Qt::Key_Left:
    case Qt::Key_A:
        current_state = left;
        break;
    case Qt::Key_Right:
    case Qt::Key_D:
        current_state = right;
        break;
    case Qt::Key_Up:
    case Qt::Key_W:
        current_state = up;
        break;
    case Qt::Key_Down:
    case Qt::Key_S:
        current_state = down;
        break;
    case Qt::Key_P:
        //矩阵表
        used_cell = 0;
        quit = false;
        success = false;
        scord = 0;
        //设置定时器
        current_state = stop;
        //初始化矩阵
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                table[i][j] = 0;
        //刷俩个格子
        if (16 - used_cell > 0)
            table[rand() % 4][rand() % 4] = 2;
        used_cell++;
        if (16 - used_cell > 0)
            table[rand() % 4][rand() % 4] = 2;
        used_cell++;
        break;
    case Qt::Key_Q:
        dont_stop = false;
        break;
    case Qt::Key_M:
        //去掉一个格子
        table[x][y] = 0;
        used_cell--;
        use_x = false;
        use_y = false;
        break;
    case Qt::Key_N:
        use_x = false;
        use_y = false;
        break;
    case Qt::Key_B:
        get_x1y1 = false;
        get_x2y2 = false;
        break;
    case Qt::Key_V:
        if (get_x1y1 == false) {
            x1 = x;
            y1 = y;
            get_x1y1 = true;
        } else if (get_x2y2 == false) {
            x2 = x;
            y2 = y;
            get_x2y2 = true;
        }
        use_x = false;
        use_y = false;
        break;
    case Qt:: Key_C:
        //交换俩个数据
        if (get_x1y1 == true && get_x2y2 == true) {
            temp = table[x1][y1];
            table[x1][y1] = table[x2][y2];
            table[x2][y2] = temp;
            get_x1y1 = false;
            get_x2y2 = false;
        }
        break;
    case Qt::Key_1:
        if (use_x == false) {
            use_x = true;
            x = 0;
        } else if (use_y == false) {
            use_y = true;
            y = 0;
        }
        break;
    case Qt::Key_2:
        if (use_x == false) {
            use_x = true;
            x = 1;
        } else if (use_y == false) {
            use_y = true;
            y = 1;
        }
        break;
    case Qt::Key_3:
        if (use_x == false) {
            use_x = true;
            x = 2;
        } else if (use_y == false) {
            use_y = true;
            y = 2;
        }
        break;
    case Qt::Key_4:
        if (use_x == false) {
            use_x = true;
            x = 3;
        } else if (use_y == false) {
            use_y = true;
            y = 3;
        }
        break;
    default:
        current_state = stop;
        break;
    }
    timer->start(overtime);
}

void Widget::merge_row(int x, int left_or_right)
{
    //合法格子
    if (x < 0 || x >= 4)
        return;
    if (left_or_right == left) {
        //先合并数据
        for (int i = 0; i < 4; i++)
            if (table[x][i] != 0)
                for (int j = i + 1; j < 4; j++)
                    if (0 != table[x][j]) {
                        if (table[x][i] == table[x][j]) {
                            table[x][i] *= 2;
                            table[x][j] = 0;
                            if (table[x][i] == 2048)
                                success = true;
                            scord++;
                            used_cell--;
                        }
                        break;
                }
        //向左挪移
        for (int i = 0; i < 4; i++)
            if (table[x][i] == 0)
                for (int j = i + 1; j < 4; j++)
                    if (table[x][j] != 0) {
                        table[x][i] = table[x][j];
                        table[x][j] = 0;
                        break;
                    }


    }
    if (left_or_right == right) {
        //先合并数据
        for (int i = 3; i >= 0; i--)
            if (table[x][i] != 0)
                for (int j = i - 1; j >= 0; j--)
                    if (0 != table[x][j]) {
                        if (table[x][i] == table[x][j]) {
                            table[x][i] *= 2;
                            table[x][j] = 0;
                            if (table[x][i] == 2048)
                                success = true;
                            scord++;
                            used_cell--;
                        }
                        break;
                }
        //向右挪移
        for (int i = 3; i >= 0; i--)
            if (table[x][i] == 0)
                for (int j = i - 1; j >= 0; j--)
                    if (table[x][j] != 0) {
                        table[x][i] = table[x][j];
                        table[x][j] = 0;
                        break;
                    }
    }
}

void Widget::merge_col(int x, int up_or_down)
{
    //合法格子
    if (x < 0 || x >= 4)
        return;
    if (up_or_down == up) {
        //先合并数据
        for (int i = 0; i < 4; i++)
            if (table[i][x] != 0)
                for (int j = i + 1; j < 4; j++)
                    if (0 != table[j][x]) {
                        if (table[i][x] == table[j][x]) {
                            table[i][x] *= 2;
                            table[j][x] = 0;
                            if (table[x][i] == 2048)
                                success = true;
                            scord++;
                            used_cell--;
                        }
                        break;
                }
        //向上挪移
        for (int i = 0; i < 4; i++)
            if (table[i][x] == 0)
                for (int j = i + 1; j < 4; j++)
                    if (table[j][x] != 0) {
                        table[i][x] = table[j][x];
                        table[j][x] = 0;
                        break;
                    }
    }
    if (up_or_down == down) {
        //先合并数据
        for (int i = 3; i >= 0; i--)
            if (table[i][x] != 0)
                for (int j = i - 1; j >= 0; j--)
                    if (0 != table[j][x]) {
                        if (table[i][x] == table[j][x]) {
                            table[i][x] *= 2;
                            table[j][x] = 0;
                            if (table[x][i] == 2048)
                                success = true;
                            scord++;
                            used_cell--;
                        }
                        break;
                }
        //向下挪移
        for (int i = 3; i >= 0; i--)
            if (table[i][x] == 0)
                for (int j = i - 1; j >= 0; j--)
                    if (table[j][x] != 0) {
                        table[i][x] = table[j][x];
                        table[j][x] = 0;
                        break;
                    }
    }
}

void Widget::help()
{

}

void Widget::timeout()
{
    switch (current_state) {
    case left:
        //行左进动
        for (int i = 0; i < 4; i++)
            merge_row(i, left);
        break;
    case right:
        //行右进动
        for (int i = 0; i < 4; i++)
            merge_row(i, right);
        break;
    case up:
        //列上进动
        for (int i = 0; i < 4; i++)
            merge_col(i, up);
        break;
    case down:
        //列下进动
        for (int i = 0; i < 4; i++)
            merge_col(i, down);
        break;
    default:
        break;
    }
    if (current_state != stop)
    //刷格子，8个以上考虑随机刷
        if (16 - used_cell > 3) {
            //刷一个
            while (1) {
                int i = rand() % 4;
                int j = rand() % 4;
                if (table[i][j] == 0) {
                    table[i][j] = 2;
                    used_cell++;
                    break;
                }
            }
        } else {
            //格子不多了，直接找个把
            for (int i = 3; i >= 0; i--)
                for (int j = 3; j >= 0; j--)
                    if (table[i][j] == 0) {
                        table[i][j] = 2;
                        used_cell++;
                        goto next;
                    }
            next:;
        }
    update();
    current_state = stop;
    //满了
    if (used_cell == 16) {
        bool flag = true;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++) {
                if (i + 1 < 4)
                    if (table[i][j] == table[i + 1][j]) {
                        flag = false;
                        break;
                    }
                if (j + 1 < 4)
                    if (table[i][j] == table[i][j + 1]) {
                        flag = false;
                        break;
                    }
            }
        if (flag == true)
            quit = true;
    }
    timer->stop();
}
