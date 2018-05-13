#include "MyLabel.h"

MyLabel::MyLabel(QWidget *parent):QLabel(parent){}

void MyLabel::activarAlarma() {
    QString hour = QString::number(hora);
    QString minute = QString::number(minut);

    if (hour < 10) hour = hour + '0';
    if (minut < 10) minute = '0' + minute;

    setText("Alama - " + hour + ":" + minute);
    MyLabel::setStyleSheet("background-color:green; color:white;");
    mode = true;
}

void MyLabel::desactivarAlarma(){
    setText("Alama desactivada");
    MyLabel::setStyleSheet("background-color:red; color:white;");
    mode = false;
}

void MyLabel::rebreHora(int h) {
    hora = h;
    if (mode) activarAlarma();
}

void MyLabel::rebreMinut(int m) {
    minut = m;
    if (mode) activarAlarma();
}

void MyLabel::reset(){
    desactivarAlarma();
    emit assignaZero("0");
    emit activaCheck(1);

    hora = 0;
    minut = 0;
}
