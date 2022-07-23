#include <QApplication>
#include <QWidget>
#include <QHBoxLayout>
#include <QSplitter>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>

int main(int argc,char** argv)
{
    QApplication app(argc,argv);
    QWidget widget;

    QVBoxLayout left_vbox;

    QHBoxLayout hbox1;
    QLabel label1("context");
    hbox1.addWidget(&label1);
    QTextEdit edit1;
    hbox1.addWidget(&edit1);
    left_vbox.addLayout(&hbox1);

    QHBoxLayout hbox2;
    QTextEdit edit2;
    hbox2.addWidget(&edit2);
    hbox2.addStretch();
    left_vbox.addLayout(&hbox2);

    QHBoxLayout hbox3;
    QTextEdit edit3;
    hbox3.addWidget(&edit3);
    hbox3.addStretch();
   left_vbox.addLayout(&hbox3);

    QHBoxLayout hbox4;
    QTextEdit edit4;
    hbox4.addWidget(&edit4);
    hbox4.addStretch();
    left_vbox.addLayout(&hbox4);

    QVBoxLayout right_vbox;
    QPushButton button1("search");
    QPushButton button2("close");
    QPushButton button3("show");
    right_vbox.addWidget(&button1);
    right_vbox.addWidget(&button2);
    right_vbox.addWidget(&button3);

    QHBoxLayout up_layout;
    up_layout.addLayout(&left_vbox);
    up_layout.addLayout(&right_vbox);

    QVBoxLayout vbox;
    vbox.addLayout(&up_layout);

    widget.setLayout(&vbox);
    widget.show();
    app.exec();
}

