#include "MyLabel.h"

MyLabel::MyLabel(QWidget *parent):QLabel(parent){}

void MyLabel::Truncar(int i){
    QString str = text();
    str = str.truncate(i);
    setText(str);
}
