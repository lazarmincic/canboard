#ifndef SERIALREADWORKER_H
#define SERIALREADWORKER_H

#include <QObject>
#include <QDebug>

class SerialReadWorker : public QObject
{
    Q_OBJECT

public slots:
    void read_serial();

signals:
    void promena_na_ulazu(bool*,int);
};

#endif // SERIALREADWORKER_H
