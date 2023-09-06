#include "dialog.h"
#include "ui_dialog.h"


Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);

    esp32 = new QSerialPort (this);

/*
    qDebug()<<"Broj portova: "<< QSerialPortInfo::availablePorts().length()<<"\n";
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
        qDebug() << "Opis: "<< serialPortInfo.description() << "\n";
        qDebug() << "Ima li id proizvodjaca?: "<<serialPortInfo.hasVendorIdentifier()<<"\n";
        qDebug() << "Ima li id proizvoda?: "<<serialPortInfo.hasProductIdentifier()<<"\n";
        qDebug() << "Product ID: "<< serialPortInfo.productIdentifier()<<"\n";
        qDebug() << "Vendor ID: "<<serialPortInfo.vendorIdentifier()<<"\n";
    }

    // ovim kodom nadji koji je port na kom je esp32!

*/

   // bool esp32_prikacen = false;
    QString esp32_port_name;

    // proveravaj za svaki seriski port
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
        if (serialPortInfo.hasProductIdentifier() && serialPortInfo.hasVendorIdentifier())
        {
            if ((serialPortInfo.productIdentifier() == esp32_product_id) &&
                    (serialPortInfo.vendorIdentifier()== esp32_vendor_id))
            {
                esp32_prikacen = true;
                esp32_port_name = serialPortInfo.portName();
            }
        }
    }

    if (esp32_prikacen)
    {
        qDebug() << "Found esp32 port... \n";
        esp32 -> setPortName(esp32_port_name);
        esp32 -> open(QSerialPort::ReadWrite);
        esp32 -> setBaudRate(BAUD);

        esp32 -> setDataBits(QSerialPort::Data8);
        esp32 -> setFlowControl(QSerialPort::NoFlowControl);
        esp32 -> setParity(QSerialPort::OddParity); //
        esp32 -> setStopBits(QSerialPort::TwoStop); //
        QObject::connect(esp32, SIGNAL (readyRead()),this,SLOT(readSerial()));
    }
    else
    {
        qDebug() << "Couldn't find the correct port for the esp32.\n";
        QMessageBox::information(this, "Serial Port Error", "Couldn't open serial port to esp32.");
    }

    connect(this, &Dialog::value_changed_signal, this, &Dialog::value_changed);
    connect(ui->pushButton,&QPushButton::clicked, this, &Dialog::pushButton_clicked);

    SerialReadWorker* worker = new SerialReadWorker;
    worker -> moveToThread(&SerialReadWorkerThread);

    connect(&SerialReadWorkerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(esp32,&QSerialPort::readyRead,worker,&SerialReadWorker::read_serial);
    connect(worker,&SerialReadWorker::promena_na_ulazu,this,&Dialog::read_serial_intr); // ---: metoda koja ce raditi nesto kad se promeni vrednost na ulazu

    SerialReadWorkerThread.start();
  // test



   // write_diode(plava,9000,100);


}

Dialog::~Dialog()
{

    if(esp32->isOpen()){
        esp32->close(); //    zatvori serijski port ako je otvoren
    }

    SerialReadWorkerThread.quit();
    SerialReadWorkerThread.wait();

    delete ui;
}

// ovo u novi tred
void Dialog::readSerial() // cita serijski port, nakon | ce biti vrednost Rpot
{



    serialBuffer = serialBuffer + QString::fromStdString(esp32 -> readAll().toStdString()); // je l ne dolaze u celom komadu
   // qDebug()<<"serialBuffer: "<<serialBuffer;


    int n_index = serialBuffer.indexOf("\r\n");
    if (n_index!=-1) // procitao je ceo, ali mozda i vise, podeli ga
    {
        buffer_split_list = serialBuffer.split("\r\n",Qt::SkipEmptyParts);
        //if ()
        serialBuffer.clear();
       // qDebug()<<"buffer_split_list: "<<buffer_split_list;
        int temp_n = buffer_split_list.size()-1 ;
        if (buffer_split_list.size()<=1) temp_n = buffer_split_list.size();

        for(int i=0;i<temp_n;i++) // obradjujemo
            {
                QStringList odvojeni_zarezima = buffer_split_list[i].split(',',Qt::SkipEmptyParts);
                if (odvojeni_zarezima.size()!=7) continue;
                QStringList temp = odvojeni_zarezima[6].split('|',Qt::SkipEmptyParts);
                if (temp.size()!=2) continue;
                odvojeni_zarezima.removeAt(6);
                odvojeni_zarezima.append(temp);
                if (odvojeni_zarezima.size()!=8) continue; //gomilu nekih provera u slucaju da podaci ne valjaju
                bool pressed [7] = {0,0,0,0,0,0,0};
                int pot_value = 100;
                if (odvojeni_zarezima[0].contains("\n")) odvojeni_zarezima[0].remove("\n");
                bool toint_ok = true;
                bool match = true; // velicina i unutrasnost
                for (int j=0;j<odvojeni_zarezima.size()-1;j++)
                {
                        if (odvojeni_zarezima[j].size()!=1 || (odvojeni_zarezima[j]!="0" && odvojeni_zarezima[j]!="1") )
                        {
                            match = false;
                            break;
                        }
                        pressed [j] = odvojeni_zarezima[j].toInt(&toint_ok) != 0;
                        if (!toint_ok) break;
                }
                if (!toint_ok || !match) continue;
                if (odvojeni_zarezima[odvojeni_zarezima.size()-1].size()!=3) continue;
                pot_value = odvojeni_zarezima[odvojeni_zarezima.size()-1].toInt(&toint_ok);
                if (!toint_ok) continue;

                // ovde radi nesto
                emit value_changed_signal(pressed,pot_value); //emituje signal sa vrednostima



            }
            serialBuffer.append((buffer_split_list[buffer_split_list.size()-1]));
            buffer_split_list.clear();
    }
    else return; // nije sve procitano, cekaj nove podatke


}



    void Dialog::write_diode(led_enum l, int delay_ms, int jacina)
{


    if ( (esp32->write((QByteArray::number(l)).append(" , ").append(QByteArray::number(delay_ms)).append(" , ").append(QByteArray::number(jacina)).append('\0'))) == -1 )   //ako nije mogao upisati, '\0'
    {
       // qDebug()<< "Nije moguce upaliti diodu";
        if (esp32_prikacen)
        {
          QMessageBox prozor_greske (QMessageBox::Critical,"Error!","Device currently in use.",QMessageBox::Ok,this,Qt::Widget); //moze se namestiti da prikazuje eror
          prozor_greske.setWindowModality(Qt::NonModal);
          prozor_greske.exec();
        }

    }
}

void Dialog::value_changed(bool* pressed,int pot_value)
{
 /*   debug_n++;
    qDebug()<<"Usao u slot "<<debug_n;


    if (pot_value!=100) qFatal("Pogresno ocitan pot");*/

   // for (int i=0;i<7;i++)
    {
        qDebug().nospace()<<"K"<<0<<" "<<pressed[0];
       // if (pressed[i]==true)
         // qFatal("Pritisnuta K");
    }


    //qDebug()<<"Vrednost pot: "<<pot_value;
}




void Dialog::pushButton_clicked()
{
    write_diode(zelena,1000,20); // od 0 do 100 jacine
    write_diode(crvena,1000,20);
    write_diode(plava,1000,20);
}

void Dialog::read_serial_intr(bool*,int)
{
    qDebug()<<"Usao u read_serial_intr";
}
