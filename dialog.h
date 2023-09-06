#ifndef DIALOG_H
#define DIALOG_H

#define BAUD 250000

#include <qdebug.h>
#include <qstring.h>
#include <qmessagebox.h>
#include <qstringlist.h>
#include "SerialReadWorker.h"

#include <QDialog>  // sve include u h fajl
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QByteArray>
#include <QIODevice>
#include <QMessageBox>
#include <QtGlobal>
#include <QThread>


enum led_enum {gasi,zelena = 1,crvena = 2,plava = 3};


QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT
    QThread SerialReadWorkerThread; //ovde ce biti tredovi

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private:
    Ui::Dialog *ui;

    QSerialPort* esp32;
    static const quint16 esp32_vendor_id = 4292;
    static const quint16 esp32_product_id = 60000;

    QString serialBuffer;



    void write_diode(led_enum l, int delay_ms, int jacina);

    QByteArray boja_u_string(led_enum);

    bool esp32_prikacen = false;

    QStringList buffer_split_list;

    int debug_n = 0;

    // jer pocinje sa ni jednim K pritisnutim i sa najacim zvukom
    bool pressed_prethodna [7] = {0,0,0,0,0,0,0};
    int pot_value_prethodna = 100;



signals:
    void value_changed_signal(bool* pressed,int pot_value);


private slots:
    void value_changed(bool* pressed,int pot_value);
    void readSerial();
    void pushButton_clicked();


public slots:
    void read_serial_intr(bool*,int);

};
#endif // DIALOG_H
