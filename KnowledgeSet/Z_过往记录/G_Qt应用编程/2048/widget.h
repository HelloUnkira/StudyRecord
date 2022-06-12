#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

//编码风格：Linux内核编码风格

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

    enum
    {
        stop,
        left,
        right,
        up,
        down
    };

private:
    int max_height = 600;
    int max_width = 800;
    int offset = 100;

    //矩阵表
    int table[4][4];
    int cell_width = 100;
    int cell_height = 100;
    int used_cell = 0;
    int quit = false;
    int success = false;
    int scord = 0;
    //不想停止
    int dont_stop = true;

    //设置定时器
    QTimer* timer;
    int overtime = 1;
    int current_state = stop;

    //修改数据
    int use_flag = 0;
    int x = -1;
    bool use_x = false;
    int y = -1;
    bool use_y = false;
    //交换俩个格子
    bool get_x1y1 = false;
    int x1 = -1;
    int y1 = -1;
    bool get_x2y2 = false;
    int x2 = -1;
    int y2 = -1;
protected:
    //重写画图事件函数
    //特点，自动调用
    void paintEvent(QPaintEvent *event);
    //重写按键事件函数
    void keyPressEvent(QKeyEvent * event);

    void merge_row(int x, int left_or_right);
    void merge_col(int x, int up_or_down);

    void help();

//槽函数专用存放点
protected slots:
    void timeout();
};

#endif // WIDGET_H












