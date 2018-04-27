#include <QLabel>

class MyLabel : public QLabel {

    Q_OBJECT

    QString original;

    public:
        MyLabel(QWidget* parent = 0);

    public slots:
        void Truncar(int);
        void SetOriginal(QString);

    signals:
        void enviaText (const QString &);

};
