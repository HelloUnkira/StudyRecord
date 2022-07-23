#include "widget.h"
#include <QDebug>
#include <QPainter>
#include <QTimer>
#include <QKeyEvent>
#include <ctime>
#include <QSound>
#include <QFont>

#include <ctime>
#include <string>

//游戏界面暂时需要一个窗口就够了
//则创建窗口全部在构造函数完成即可
Widget::Widget(QWidget *parent) :
    QWidget(parent)
{
    //固定窗口大小
    setFixedSize(max_width, max_height);
    //设置标题
    setWindowTitle("像素贪吃蛇v1.0");
    //设置图标
    //QIcon icon = QIcon("../snake/icon.bmp")
    //setWindowIcon(icon);
    //设置背景图
    setAutoFillBackground(true);
    QPalette palette;
    //设置为纯色
    //setPalette(Qt::white);
    //将背景图片设置好，相对路径是以项目的 build-Snake... 这个文件夹为基准
    palette.setBrush(QPalette::Background, QBrush(QPixmap("../snake/background.jpg")));
    setPalette(palette);

    //设置定时器，做闹钟
    //超时后发送信号 ---》 到槽函数
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
    timer->start(overtime);//超时时间

    srand((unsigned int)(time(NULL)));

    //使用链表初始化蛇身
    for (int i = 4; i >= 0; i--) {
        QRectF rect(100 + snake_width * i, 100, snake_width, snake_height);
        snake.append(rect);
    }

    //舒适化食物
    add_food();

    //添加背景音乐
    QSound * sound = new QSound(":/background.wav");
    sound->setLoops(QSound::Infinite);
    sound->play();

    //设置起始时间
    current_time = time(NULL);
}

//重写画图事件函数
//刷新图片上的蛇的位置
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

    //如果你犯了规
    if (god_mode == true)
        is_dead = false;
    if (is_dead == true || rest_time <= 0) {
        QSound::play("../snake/death.wav");
        QFont font("方正舒体", 20, QFont::ExtraLight,false);
        painter.setFont(font);
        QPixmap pix;
        pix.fill(Qt::white);
        painter.drawPixmap(0,0,snake_width,snake_height,pix);
        //pix.fill("../snake/background.jpg");
        //painter.drawPixmap(0,0,snake_width,snake_height,pix);

        painter.drawText((this->width() - 250) / 2,
                         (this->height() + 15) / 2,
                         "哈哈，你已经死掉了");
        //可以设置背景为游戏结束
        timer->stop();
    }

    if (start == true) {
        QFont font("方正舒体", 20, QFont::ExtraLight,false);
        painter.setFont(font);
        painter.drawText((this->width() - 250) / 2,
                         (this->height() + 15) / 2,
                         "你准备好了吗？(空格开始哦)");
        start = false;
        timer->stop();
    }

    if (restart == true) {
        QFont font("方正舒体", 20, QFont::ExtraLight,false);
        painter.setFont(font);
        painter.drawText((this->width() - 250) / 2,
                         (this->height() + 15) / 2,
                         "还来？随便你（空格继续）");
        restart = false;
        timer->stop();
    }

    //利用此稳定调用去确定是否达到预期时间
    //如果程序停下来就更需要不停更新当前时间
    if (timer->isActive() == false)
        current_time = time(NULL);
    //如果程序运行就更需要不停更新当前剩余时间
    if (timer->isActive() == true)
        rest_time -= (int)difftime(time(NULL), current_time);
    current_time = time(NULL);

    if (timer->isActive() == true) {
        //时间启动标志
        if (run_time_flag == true) {
            run_time_flag = false;
            rest_time = last_stop_time;
            current_time = time(NULL);
        }

        QFont font("方正舒体", 50, QFont::ExtraLight,false);
        painter.setFont(font);
        painter.drawText(0, 50, QString::fromStdString(
                             "分数：" + std::to_string(scord)));
        painter.drawText(0, this->height() - 50,QString::fromStdString(
                             "速度：" + std::to_string(110 - overtime)));
        painter.drawText(0, this->height(), QString::fromStdString(
                             "剩余时间：" + std::to_string(rest_time)));

        //画背景格子
        for (int i = 0; i < this->height(); i += snake_height)
            painter.drawLine(QPointF(0, i),
                             QPointF(this->width(), i));
        for (int i = 0; i < this->width(); i += snake_height)
            painter.drawLine(QPointF(i, 0),
                             QPointF(i, this->height()));

        //画蛇
        for (int i = 0; i < snake.length(); i++)
            painter.drawRect(snake[i]);
        //画食物
        QColor color;
        srand((unsigned int)time(NULL));
        for (int i = 0; i < food_set.length(); i++) {
            color = QColor(rand()%255,rand()%255,rand()%255);
            brush.setColor(color);
            //brush.setColor(Qt::green);
            brush.setStyle(Qt::SolidPattern);
            painter.setBrush(brush);
            painter.drawRect(food_set[i]);
        }
    }
}

//重写按键事件函数
void Widget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_X:
        if (god_mode == true)
            god_mode = false;
        else
            god_mode = true;
        break;
    case Qt::Key_W:
    case Qt::Key_Up:
        if (move_direct != move_up && move_direct != move_down)
            move_direct = move_up;
        break;
    case Qt::Key_S:
    case Qt::Key_Down:
        if (move_direct != move_up && move_direct != move_down)
            move_direct = move_down;
        break;
    case Qt::Key_A:
    case Qt::Key_Left:
        if (move_direct != move_left && move_direct != move_right)
            move_direct = move_left;
        break;
    case Qt::Key_D:
    case Qt::Key_Right:
        if (move_direct != move_left && move_direct != move_right)
            move_direct = move_right;
        break;
    case Qt::Key_Space:
        if (game_run == false) {
            game_run = true;
            timer->start(overtime);
            run_time_flag = true;
        } else {
            game_run = false;
            timer->stop();
            last_stop_time = rest_time;
        }
        break;
    case Qt::Key_P:
        //回溯状态
        scord = 0;
        is_dead = false;
        overtime = 100;
        last_stop_time = 100;
        rest_time = 100;
        restart = true;
        move_direct = move_right;
        //清除蛇，食物
        snake.clear();
        food_set.clear();
        //使用链表初始化蛇身
        for (int i = 4; i >= 0; i--) {
            QRectF rect(100 + snake_width * i, 100,
                        snake_width, snake_height);
            snake.append(rect);
        }
        //舒适化食物
        add_food();
        //重新开始
        timer->start(overtime);
        break;
    default:
        break;
    }
}

//上下左右移动
void Widget::left_move()
{
    //获取头节点,下次移动按头节点加方向移动即可
    QRectF first = snake.first();
    QPointF left_top;
    QPointF right_bottom;

    if (first.x() + snake_width < 0) {
        if (god_mode == true) {
            left_top = QPoint(this->width() - snake_width, first.y());
            right_bottom = QPoint(this->width(), first.y()+snake_height);
        }
        is_dead = true;
     } else {
        left_top =QPointF(first.x() - snake_width, first.y());
        right_bottom = first.bottomLeft();
    }
    snake.insert(snake.indexOf(first), QRectF(left_top, right_bottom));
    //snake.append(QRectF(left_top,right_bottom));
}

void Widget::right_move()
{
    //获取头节点,下次移动按头节点加方向移动即可
    QRectF first = snake.first();
    QPointF left_top;
    QPointF right_bottom;

    if (first.x() + snake_width >= this->width()) {
        if (god_mode == true) {
            left_top = QPoint(0, first.y());
            right_bottom = QPoint(snake_width, first.y() + snake_height);
        }
        is_dead = true;
    } else {
        left_top = first.topRight();
        right_bottom = QPointF(first.x() + snake_width * 2, first.y() + snake_height);
    }
    snake.insert(snake.indexOf(first), QRectF(left_top, right_bottom));
}

void Widget::up_move()
{
    //获取头节点,下次移动按头节点加方向移动即可
    QRectF first = snake.first();
    QPointF left_top;
    QPointF right_bottom;

    if (first.y() - snake_height < 0) {
        if (god_mode == true) {
            left_top = QPoint(first.x(), this->height() - snake_height);
            right_bottom = QPoint(first.x() + snake_width, this->height());
        }
        is_dead = true;
    } else {
        left_top = QPointF(first.x(), first.y() - snake_height);
        right_bottom = first.topRight();
    }
    snake.insert(snake.indexOf(first), QRectF(left_top,right_bottom));
}

void Widget::down_move()
{
    //获取头节点,下次移动按头节点加方向移动即可
    QRectF first = snake.first();
    QPointF left_top;
    QPointF right_bottom;

    if (first.y() + snake_height > this->height()) {
        if (god_mode == true) {
            left_top = QPoint(first.x(), 0);
            right_bottom = QPoint(first.x() + snake_width, snake_height);
        }
        is_dead = true;
    } else {
        left_top = first.bottomLeft();
        right_bottom = QPointF(first.x() + snake_width, first.y() + snake_height * 2);
    }
    snake.insert(snake.indexOf(first), QRectF(left_top, right_bottom));
}

void Widget::add_food()
{
    QPointF left_top = QPoint((rand() * snake_width % (this->width() - snake_width)) ,
                             (rand() * snake_height % (this->height() - snake_height)));
    QPointF right_bottom = QPoint(left_top.x() + snake_width,
                                 left_top.y() + snake_height);
    food = QRectF(left_top, right_bottom);
    food_set.append(food);
}


void Widget::timeout()
{
    switch (move_direct) {
    case move_up:
        up_move();
        break;
    case move_down:
        down_move();
        break;
    case move_left:
        left_move();
        break;
    case move_right:
        right_move();
        break;
    default:
        break;
    }
    update();
    bool flag = false;
    for (int i = 0; i < food_set.length(); i++)
        if (snake[0].intersects(food_set[i])) {
            QSound::play(":/eat.wav");
            scord += 10;
            food_set.removeAt(food_set.indexOf(food_set[i]));
            if (overtime > 1)
                overtime -= 10;
            timer->start(overtime);
            if (food_set.length() == 0)
                add_food();
            if (food_set.length() < 5)
            for (int j = 0; j < rand() % 4; j++)
                add_food();
            flag = true;
            //重置时间
            rest_time = 100;
            break;
        }
    if (flag == false)
        snake.removeLast();

    //撞到了自己？？
    for (int i = 1; i < snake.length(); i++)
            if (snake[0].intersects(snake[i]))
                is_dead = true;
}

Widget::~Widget()
{
}
