#include <QLabel>

class MyLabel : public QLabel {

    Q_OBJECT

    int hora = 0;
    int minut = 0;

    bool mode = false;

    public:
        MyLabel(QWidget* parent = 0);

    public slots:
        void activarAlarma();
        void desactivarAlarma();
        void rebreHora(int);
        void rebreMinut(int);
        void reset();


    signals:
        void activaCheck(bool);
        void assignaZero(QString);

};
