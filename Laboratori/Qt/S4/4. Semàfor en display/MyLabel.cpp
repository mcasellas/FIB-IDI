#include "MyLabel.h"

MyLabel::MyLabel(QWidget *parent):QLabel(parent){}

void MyLabel::Truncar(int i){
    QString str = original;
    str.truncate(i);
    emit enviaText(str);
}

void MyLabel::SetOriginal(QString orig) {
    original = orig;
    Truncar(orig.size());
}
