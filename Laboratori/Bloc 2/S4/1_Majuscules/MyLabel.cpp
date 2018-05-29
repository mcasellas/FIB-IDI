#include "MyLabel.h"

MyLabel::MyLabel(QWidget *parent):QLabel(parent){}

void MyLabel::setTextMajuscules(QString s){
    QString str = s.toUpper();
    setText(str);
}
