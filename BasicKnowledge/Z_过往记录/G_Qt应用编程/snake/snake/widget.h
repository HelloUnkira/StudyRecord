#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <ctime>

//编码风格：Linux内核编码风格

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    enum
    {
        move_up,
        move_down,
        move_left,
        move_right
    };
private:
    int max_height = 600;
    int max_width = 800;
    //定义一个链表，成员为矩形类，表示蛇身
    QList<QRectF> snake;
    int snake_width = 10;
    int snake_height = 10;
    //定义一个结点表示食物,以及食物集合
    QRectF food;
    QList<QRectF> food_set;
    //设置定时器
    QTimer* timer;
    int overtime = 100;
    int move_direct = move_right;
    bool is_dead = false;
    bool game_run = false;
    bool god_mode = false;
    int scord = 0;

    bool start = true;
    bool restart = false;

    int run_time_flag = true;
    time_t current_time = 0;
    int last_stop_time = 100;
    int rest_time = 100;
protected:
    //重写画图事件函数
    //特点，自动调用
    void paintEvent(QPaintEvent *event);
    //重写按键事件函数
    void keyPressEvent(QKeyEvent * event);

    //上下左右移动
    void left_move();
    void right_move();
    void up_move();
    void down_move();
    //添加食物
    void add_food();

//老版本槽函数专用存放点
protected slots:
    void timeout();

};


#endif // WIDGET_H
