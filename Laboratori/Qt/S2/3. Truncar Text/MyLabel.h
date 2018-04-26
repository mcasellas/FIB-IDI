#include <QLabel>

class MyLabel : public QLabel {

    Q_OBJECT

    public:
        MyLabel(QWidget* parent = 0);

    public slots:
        void Truncar(int);

};
