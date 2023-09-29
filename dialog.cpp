#include "dialog.h"
#include "ui_dialog.h"


Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint); //minimize button
    setWindowState(Qt::WindowActive); //pokreni maksimizovanog

    //esp32_id_print(); //debug

    check_if_connected = new QTimer(this);
    connect(check_if_connected,&QTimer::timeout,this,&Dialog::checkifdisconnected);
    check_if_connected->start(CHECKING_RATE);

    connect(this, &Dialog::value_changed_signal, this, &Dialog::value_changed);
    connect(ui->pushButton,&QPushButton::clicked, this, &Dialog::pushButton_clicked);


  // test

   // write_diode(plava,9000,100);

    connect(ui->can0_chooseColor,&QPushButton::clicked, this, &Dialog::pushButton_2_clicked);

    changecolor_C0(to_darker_color(C0_color,3));

    can0_colordialog_declare();

    ui->lineEdit_1->setMaxLength(MAX_DELAY_LENGTH); // max delay 9999ms
    ui->lineEdit_1->setText(QString::number(default_delay)); // pod. vrednost

    connect(ui->lineEdit_1,&QLineEdit::textEdited,this,&Dialog::delay_updated);

    ui->label_green->setPixmap(changeImageColor(REDDIODE_IMG,Qt::green));
    ui->label_blue->setPixmap(changeImageColor(REDDIODE_IMG,Qt::blue));
    ui->label_rgb->setPixmap(changeImageColor(REDDIODE_IMG,default_rgb));

    connect(ui->pushButton_red,&QPushButton::clicked,this,&Dialog::red_butt_clicked);

   // ui->pushButton_red_over->setStyleSheet("background-color: rgb(255,0,0);");
    ui->pushButton_red_over->setStyleSheet("* { background-color: rgba(0,125,0,0) }");

    connect(ui->pushButton_red_over,&QPushButton::clicked,this,&Dialog::redled_pic_clicked);
    connect(ui->spinBox_red,QOverload<int>::of(&QSpinBox::valueChanged),this,&Dialog::red_jacina_change);

    connect(ui->pushButton_3,&QPushButton::clicked,this,&Dialog::popup_change_color_rgb);

    rgb_dialog = new QColorDialog(default_rgb,this);
    connect(rgb_dialog,&QColorDialog::currentColorChanged,this,&Dialog::changeRgbLabelColor);

    connect(rgb_dialog,&QColorDialog::colorSelected,this,&Dialog::colordialog1_ok);
    connect(rgb_dialog,&QColorDialog::finished,this,&Dialog::colordialog1_cancel);

    ui->pot_link->insertItems(0,{"red","green","blue","rgb"});
    ui->pot_link->setCurrentIndex(2); //def plava

    connect(ui->pot_link,&QComboBox::currentTextChanged,this,&Dialog::linkLedToPot);

    ui->red_delay->setText(QString::number(default_delay));
    ui->red_delay->setMaxLength(MAX_DELAY_LENGTH);

    connect(ui->red_delay,&QLineEdit::textChanged,this,&Dialog::red_delay_updated);

    ui->volume_slider->setDisabled(true);

    connect(ui->can0_onoff,&QPushButton::clicked,this,&Dialog::iskljuci_C0);



    connect(ui->C0_click,&QPushButton::clicked,this,&Dialog::C0SijaISvira);
    ui->C0_click->setStyleSheet("* { background-color: rgba(0,125,0,0) }");

    connect(ui->C1_click,&QPushButton::clicked,this,&Dialog::C1SijaISvira);
    ui->C1_click->setStyleSheet("* { background-color: rgba(0,125,0,0) }");

    connect(ui->C2_click,&QPushButton::clicked,this,&Dialog::C2SijaISvira);
    ui->C2_click->setStyleSheet("* { background-color: rgba(0,125,0,0) }");

    connect(ui->C3_click,&QPushButton::clicked,this,&Dialog::C3SijaISvira);
    ui->C3_click->setStyleSheet("* { background-color: rgba(0,125,0,0) }");

    connect(ui->C4_click,&QPushButton::clicked,this,&Dialog::C4SijaISvira);
    ui->C4_click->setStyleSheet("* { background-color: rgba(0,125,0,0) }");

    connect(ui->C5_click,&QPushButton::clicked,this,&Dialog::C5SijaISvira);
    ui->C5_click->setStyleSheet("* { background-color: rgba(0,125,0,0) }");

    connect(ui->C6_click,&QPushButton::clicked,this,&Dialog::C6SijaISvira);
    ui->C6_click->setStyleSheet("* { background-color: rgba(0,125,0,0) }");



    choose_note(ui->can0_changeNote); // puni pad. meni

    ui->can0_changeNote->setCurrentIndex(0) ; // podraz. vrednost (C dur)
    connect(ui->can0_changeNote,&QComboBox::currentTextChanged,this,&Dialog::izabranaNotaUPad);

    ui->comboBox_samples->addItems(sample_names);
    ui->comboBox_samples->setCurrentIndex(0);

    connect(ui->can0_sharpen,&QPushButton::clicked,this,&Dialog::can0_sharp_pressed);
    connect(ui->can0_flatten,&QPushButton::clicked,this,&Dialog::can0_flat_pressed);

    connect(ui->pushButton_major,&QPushButton::clicked,this,&Dialog::to_major);
    to_major(true); // inicijalizacija

    connect(ui->pushButton_minor,&QPushButton::clicked,this,&Dialog::to_minor);

    connect (this,&Dialog::major,this,&Dialog::puni_p_meni_major);
    connect (this,&Dialog::minor,this,&Dialog::puni_p_meni_minor);

    puni_p_meni_major(); // inic vrednost p menija
    ui->comboBox_scale->setCurrentIndex(0); // inic vr

    ui->horizontalSlider_octave->setRange(-1,7); //octave slider
    ui->horizontalSlider_octave->setValue(3); // pod. vr.
    ui->horizontalSlider_octave->setTickPosition(QSlider::TicksAbove);
    ui->horizontalSlider_octave->setTickInterval(1);
    ui->horizontalSlider_octave->setPageStep(1);

    ui->label_octave->setText("3"); //def

    connect(ui->horizontalSlider_octave, &QSlider::valueChanged, this, &Dialog::setuj_tekst);

    connect(ui->radioButton_sharp,&QRadioButton::clicked,this,&Dialog::radioButton_sharp_clicked);
    connect(ui->radioButton_flat,&QRadioButton::clicked,this,&Dialog::radioButton_flat_clicked);

    connect(ui->input_on_off,&QCheckBox::stateChanged,this,&Dialog::input_on_off_stateChanged);
    connect(ui->output_on_off,&QCheckBox::stateChanged,this,&Dialog::output_on_off_stateChanged);

}

Dialog::~Dialog()
{

    if(esp32->isOpen()){
        esp32->close(); //    zatvori serijski port ako je otvoren
    }

    delete esp32;
    delete can0_colordialog;
    delete check_if_connected;
    delete rgb_dialog;
    delete zvuk;

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
    if (delay_ms<=0)return;
    if (l==crvena && en_red == 0) return;
    if (l==crvena) delay_ms =red_delay_ms;

    if ( (esp32->write((QByteArray::number(l)).append(" , ").append(QByteArray::number(delay_ms)).append(" , ").append(QByteArray::number(jacina)).append('\0'))) == -1 )   //ako nije mogao upisati, '\0'
    {
       // qDebug()<< "Nije moguce upaliti diodu";
        if (esp32_prikacen)
        {
          QMessageBox prozor_greske (QMessageBox::Critical,"Error!","Device currently in use.",QMessageBox::Ok,this,Qt::Widget); //moze se namestiti da prikazuje eror
          prozor_greske.setWindowModality(Qt::NonModal);
          prozor_greske.exec(); //show
        }

    }
}

void Dialog::value_changed(bool* pressed,int pot_value) //stalno ce se izvrsavati
{
 /*   debug_n++;
    qDebug()<<"Usao u slot "<<debug_n;

    if (pot_value!=100) qFatal("Pogresno ocitan pot");*/

   // for (int i=0;i<7;i++)
    {
      //  qDebug().nospace()<<"K"<<0<<" "<<pressed[0];
       // if (pressed[i]==true)
         // qFatal("Pritisnuta K");
    }
    if (prvi_put_usao==0)
    {
        //qDebug()<<"Ovo treba da se ispise samo jednom"; //ok
        prvi_put_usao = 1;
        pot_value_prethodna = pot_value;  // na pocetku programa da ne sija
        changeVolumeSlider(pot_value);
    }

    //qDebug()<<"Vrednost pot: "<<pot_value;

    if (!(pot_value_prethodna<=pot_value+pot_margin && pot_value_prethodna>=pot_value-pot_margin))
    {
        //qDebug()<<"Ispunjen uslov";
        pot_value_is_changing(pot_value); //ako treba jos stvari da se radi, emituj signal
        pot_value_prethodna = pot_value;
    }


    QVector<int> c_changed;
    for (int i =0;i<7;i++)
        if (pressed[i]!=pressed_prethodna[i])
            c_changed.append(i);
    if (c_changed.size()!=0)
    {
        can_pressedOrReleased(pressed,c_changed);
        for (int i =0;i<7;i++)
            pressed_prethodna[i]=pressed[i];
    }

}




void Dialog::pushButton_clicked()
{
    write_diode(zelena,delay_ms,100); // od 0 do 100 jacine
    write_diode(crvena,delay_ms,jacina_red);
    write_diode(plava,delay_ms,100);
    write_diode(rgb_R,delay_ms,201); //C900FF
    write_diode(rgb_G,delay_ms,0);
    write_diode(rgb_B,delay_ms,255);
}

void Dialog::pushButton_2_clicked()
{

    can0_colordialog->show();

}

QPixmap Dialog::changeImageColor(const QString &path,const QColor &color)
{
    QPixmap px( path ); //
    QPixmap pxr( px.size() );
    pxr.fill(color); // boja u koju ide, napravi funkciju (201, 0, 255)
    pxr.setMask(px.createMaskFromColor( Qt::red, Qt::MaskOutColor));
    QPainter painter(&px);
    painter.drawPixmap(0,0,pxr);
    return px;
}

void Dialog::writeOnImage(QPixmap *pxmp, const QString &note_name, const QColor& font_color)
{

    QPainter p(pxmp); //p - pozadina
    p.setPen(QPen(font_color));
    p.setFont(QFont("Fantasy", 192));
    p.drawText(pxmp->rect(), Qt::AlignCenter, note_name);
     // nije ga centriralo kako ja zelim
    /*
    QPixmap slova( pxmp->size() );
    slova.load("C:/Users/Lazar/Desktop/canboard_files/note_text_nobkg/C.png");
    QPainter p(pxmp);
    p.drawPixmap(pxmp->rect(),slova); //slova
    */

}


void Dialog::changecolor_C0(const QColor &color)
{
    changecolor(color,C0_nota,ui->can0_pixmap);
}

void Dialog::changecolor(const QColor &color, QString nota_str, QLabel* can_pixmap_p) //p - pointer
{
    QPixmap temp_pix(changeImageColor(CANRED_IMG,color));
    writeOnImage(&temp_pix,nota_str,findTextColor(color));
    can_pixmap_p->setPixmap(temp_pix); // u arg
}


void Dialog::light_rgb(const QColor &color)
{
     write_rgb(color,delay_ms);
}

QColor Dialog::to_darker_color(QColor oc,int times_darker) //oc - old color
{
     return QColor::fromHsv(oc.hue(),oc.saturation(),(oc.value()/times_darker));
}


void Dialog::colordialog_ok(const QColor &color)
{
     qDebug()<<"Ok pritisnuto!";
     can0_colordialog_declare();
     C0_color = color; // cuva u mem
     changecolor_C0(to_darker_color(C0_color,3));//255/3
     changeRgbLabelColor(rgb_color);
     colordialog_ok_pressed = true;
}

void Dialog::colordialog_cancel()
{
     if (!colordialog_ok_pressed)
     {
      qDebug()<<"Cancel ili X pritisnuto!";
      can0_colordialog_declare();
      changecolor_C0(to_darker_color(C0_color,3));
      changeRgbLabelColor(rgb_color);
     }
     else
      colordialog_ok_pressed = false;
}

void Dialog::write_rgb(const QColor &color, int delay , int jacina_rgb ) //max 100
{
     QColor color_to_write = color;
     if ( jacina_rgb == -1) // ako nije poslata jacina
      jacina_rgb = color_to_write.value();
     else
      jacina_rgb = doubleMap(jacina_rgb,0,100,0,255);

     color_to_write.setHsv(color_to_write.hue(),color_to_write.saturation(),jacina_rgb);

     int red,green,blue;
     color_to_write.getRgb(&red,&green,&blue);
     write_diode(rgb_R,delay,red); //C900FF pink
     write_diode(rgb_G,delay,green);
     write_diode(rgb_B,delay,blue);

     if (color == Qt::black && delay == 0)
     {
      write_diode(rgb_R,1,0);
      write_diode(rgb_G,1,0);
      write_diode(rgb_B,1,0);
     }
}

void Dialog::delay_updated (const QString &text)
{
 /*    QRegExp re ("\\d*"); //jeste broj
     if (re.exactMatch(text))
        {
              //qDebug()<<"Uneti string je broj!";
              bool ok;                          //ako su sve nule
              int text_int = text.toInt(&ok);
              if (ok) ui->lineEdit_1->setText(QString::number(text_int));
              if (text_int==0 && ok)
              {
                ui->label_error->setText("<font color='purple'>Diodes off.</font>");
                delay_ms = text_int;
              }
              else
              {
                ui->label_error->setText(""); //validna vrednost uneta
                delay_ms = text_int;
              }
        }
     else
        {
           // qDebug()<<"Uneti string nije broj!";
            ui->label_error->setText("<font color='red'>Input invalid!</font>");
            ui->lineEdit_1->setText("");
            delay_ms = 0;
        }

    if (text=="")
    {
        ui->label_error->setText("<font color='red'>Insert a value!</font>");
        delay_ms = 0;
    }

*/

     errorCheckDelay(text,ui->lineEdit_1,ui->label_error,delay_ms); //delay_ms se menja
     ui->red_delay->setText(QString::number(delay_ms));
}

void Dialog::changeRgbLabelColor(const QColor& color)
{
    ui->label_rgb->setPixmap(changeImageColor(REDDIODE_IMG,color));
}

void Dialog::red_butt_clicked () // pressed = true - on
{
    //clicked - jedino na korisnikove klikove se poziva
    en_red=en_red^1;
  //  qDebug()<<en_red;
    if (!en_red)
    {
        ui->pushButton_red->setText("OFF");
        ui->label_red->setPixmap(changeImageColor(REDDIODE_IMG,QColor::fromRgb(128,0,0))); //255/2 ne vidi se promena ispod 128
    }
    else
    {
        ui->pushButton_red->setText("ON");
        int red_max_255 = doubleMap(jacina_red,0,100,128,255); //pretvori
        ui->label_red->setPixmap(changeImageColor(REDDIODE_IMG,QColor::fromRgb(red_max_255,0,0)));
    }
}

void Dialog::redled_pic_clicked()
{
    write_diode(crvena,delay_ms,jacina_red); // jacina
}

void Dialog::red_jacina_change(int red_jacina_novo)
{

    jacina_red = red_jacina_novo; //jacina_red - 0 do 100

    qreal linearBrightness = QAudio::convertVolume(jacina_red / qreal(100.0), //0-100 -> 0.0-1.0
                                               QAudio::LogarithmicVolumeScale,
                                               QAudio::LinearVolumeScale);
    jacina_red = qRound(linearBrightness*100); // u log skalu

    if (!en_red) return;
    //write_diode(crvena,delay_ms,jacina_red); // bice opcija za ovo
    int red_max_255 = doubleMap(red_jacina_novo,0,100,128,255); //pretvori
    ui->label_red->setPixmap(changeImageColor(REDDIODE_IMG,QColor::fromRgb(red_max_255,0,0)));

}

int Dialog::doubleMap(double x, double in_min, double in_max, double out_min,double out_max)
{
    return int((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min);
}

void Dialog::pot_value_is_changing(int pot_value)
{
    if (pot_led!=rgb)
        write_diode(pot_led,delay_ms,pot_value);
    else
        write_rgb(rgb_color,delay_ms,pot_value);

    changeVolumeSlider(pot_value);
}



void Dialog::esp32_id_print()
{
    qDebug()<<"Broj portova: "<< QSerialPortInfo::availablePorts().length()<<"\n";
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
        qDebug() << "Opis: "<< serialPortInfo.description() << "\n";
        qDebug() << "Ima li id proizvodjaca?: "<<serialPortInfo.hasVendorIdentifier()<<"\n";
        qDebug() << "Ima li id proizvoda?: "<<serialPortInfo.hasProductIdentifier()<<"\n";
        qDebug() << "Product ID: "<< serialPortInfo.productIdentifier()<<"\n";
        qDebug() << "Vendor ID: "<<serialPortInfo.vendorIdentifier()<<"\n";
    }

    // ovim kodom nadji vendorid i productid od esp32
}

void Dialog::checkifdisconnected() // ponavlja se u reg. intervalima
{

  int esp32_prikacen_old = esp32_prikacen;
  esp32_prikacen = 0;

    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()) // proveravaj za svaki seriski port
    {
        if (serialPortInfo.hasProductIdentifier() && serialPortInfo.hasVendorIdentifier())
        {
                if ((serialPortInfo.productIdentifier() == esp32_product_id) &&
                    (serialPortInfo.vendorIdentifier()== esp32_vendor_id))
                {
                        esp32_prikacen = 1;
                        esp32_port_name = serialPortInfo.portName();
                }
        }
    }

    if (esp32_prikacen!=esp32_prikacen_old) // ako je doslo do promene (prvi put se sigurno izvrsava)
    {
        if (esp32_prikacen==1) // iz neprikacenog u prikaceno
        {
                emit esp32_connected_signal();

                esp32 = new QSerialPort (this);

                qDebug() << "Found esp32 port... \n";
                esp32 -> setPortName(esp32_port_name);
                esp32 -> open(QSerialPort::ReadWrite);
                esp32 -> setBaudRate(BAUD);

                esp32 -> setDataBits(QSerialPort::Data8);
                esp32 -> setFlowControl(QSerialPort::NoFlowControl);
                esp32 -> setParity(QSerialPort::OddParity); //
                esp32 -> setStopBits(QSerialPort::TwoStop); //
                QObject::connect(esp32, &QSerialPort::readyRead,this,&Dialog::readSerial);
        }
        else if (esp32_prikacen==0) //iz prikacenog u neprikaceno
        {
            qDebug() << "Couldn't find the correct port for the esp32.\n";
            QMessageBox* msbx = new QMessageBox(QMessageBox::Critical,"Serial Port Error","Couldn't open serial port to esp32.",QMessageBox::NoButton,this);// (QMessageBox::Critical,"Serial Port Error","Couldn't open serial port to esp32.",QMessageBox::NoButton,this);
            msbx->setStandardButtons(QMessageBox::NoButton);
            msbx->setInformativeText("Waiting for the connection...");
            //msbx.setWindowFlags(windowFlags() |Qt::WindowStaysOnTopHint);
            connect(this,&Dialog::esp32_connected_signal,msbx,&QMessageBox::accept);
            connect(this,&Dialog::esp32_connected_signal,msbx,&QMessageBox::deleteLater);
            msbx->show();
        }

    }
}

void Dialog::popup_change_color_rgb()
{
    rgb_dialog->show();

}

void Dialog::colordialog1_ok(const QColor& color)
{
    //Ok pritisnuto
    rgb_color = color;
    changeRgbLabelColor(rgb_color);
    colordialog1_ok_pressed = true;
}

void Dialog::colordialog1_cancel()
{
    if (!colordialog1_ok_pressed)
    {
        //Cancel ili X pritisnuto

        changeRgbLabelColor(rgb_color);
        rgb_dialog->setCurrentColor(rgb_color);
    }
    else
        colordialog1_ok_pressed = false;
}

void Dialog::linkLedToPot(const QString &text) // tekst- red green blue rgb
{
    if(text=="red")pot_led = crvena;
    if(text=="green")pot_led = zelena;
    if(text=="blue")pot_led = plava;
    if(text=="rgb")pot_led = rgb;
}

void Dialog::red_delay_updated(const QString &text) //text = vrednost delay za red
{
    errorCheckDelay(text,ui->red_delay,ui->label_error_red,red_delay_ms);
    if (red_delay_ms == 0)
        ui->label_red->setPixmap(changeImageColor(REDDIODE_IMG,QColor::fromRgb(128,0,0)));
    else if(en_red)
    {
        int redMax_255 = doubleMap(jacina_red,0,100,128,255); //pretvori
        ui->label_red->setPixmap(changeImageColor(REDDIODE_IMG,QColor::fromRgb(redMax_255,0,0)));
    }
}


void Dialog::errorCheckDelay(const QString& text, QLineEdit*& qLineEdit_pointer, QLabel*& errorLabel_pointer, int& delay_)

{

    QRegExp re ("\\d*"); //jeste broj
    if (re.exactMatch(text))
    {
        //qDebug()<<"Uneti string je broj!";
        bool ok;                          //ako su sve nule
        int text_int = text.toInt(&ok);
        if (ok) qLineEdit_pointer->setText(QString::number(text_int));
        if (text_int==0 && ok)
        {
            QString temp_s;
            if (qLineEdit_pointer == ui->lineEdit_1) temp_s = "<font color='purple'>Diodes off.</font>";
            else temp_s = "<font color='purple'>Diode off.</font>";
                errorLabel_pointer->setText(temp_s);

            delay_ = text_int;
        }
        else
        {
            errorLabel_pointer->setText(""); //validna vrednost uneta
            delay_ = text_int;
        }
    }
    else
    {
        // qDebug()<<"Uneti string nije broj!";
        errorLabel_pointer->setText("<font color='red'>Input invalid!</font>");
        qLineEdit_pointer->setText("");
        delay_ = 0;
    }

    if (text=="")
    {
        errorLabel_pointer->setText("<font color='red'>Insert a value!</font>");
        delay_ = 0;
    }

}


void Dialog::can_pressedOrReleased(bool* pressed,QVector<int> c_changed) //kao interapt
{
    //qDebug()<<c_changed;

    //changecolor_C0(C0_color);

    foreach(int i, c_changed)
    {
        if (i == 0 && C0_on) //first can
        {
            doWhenPressedOrReleased(pressed[i]==1);
        }
    }
}

void Dialog::changeVolumeSlider(int new_value)
{
    ui->volume_slider->setEnabled(true);
    ui->volume_slider->setValue(new_value);
    ui->volume_slider->setDisabled(true);
}

void Dialog::can0_colordialog_declare()
{
    can0_colordialog = new QColorDialog(default_rgb,this); // can1
    //can0_colordialog->setOption(QColorDialog::DontUseNativeDialog);
    connect(can0_colordialog,&QColorDialog::currentColorChanged,this,&Dialog::changecolor_C0);
    connect(can0_colordialog,&QColorDialog::currentColorChanged,this,&Dialog::changeRgbLabelColor);
    connect(can0_colordialog,&QColorDialog::currentColorChanged,this,&Dialog::light_rgb);

    connect(can0_colordialog,&QColorDialog::colorSelected,this,&Dialog::colordialog_ok);
    connect(can0_colordialog,&QColorDialog::finished,this,&Dialog::colordialog_cancel);
}

void Dialog::iskljuci_C0()
{
    bool on;
    if (ui->can0_onoff->text()=="ON") //gasi se
    {
        ui->can0_onoff->setText("OFF");

        on = false;
    }
    else  // pali se
    {
        ui->can0_onoff->setText("ON");
        on = true;
    }
    C0_on = on;

}


QColor Dialog::findTextColor(const QColor &color) // kako tekst uvek da se vidi u odnosu na boju u pozadini
{
    double r,g,b; // 0-1
    color.getRgbF(&r,&g,&b);
    double brightness = 0.299*r+0.587*g+0.114*b;
    QColor ret = (brightness > 0.5) ? Qt::black : Qt::white ;
/*
    int r = color.red(), g = color.green(), b=color.blue(), v=color.value(), s=color.saturation();
    QColor ret = QColor (255-r,255-g,255-b); //kompl
    v = (v<128) ? 255 : 0;
    s = (s<128) ? 255 : 0; // vibrant on pale
    ret.setHsv(ret.hue(),s,v);
*/
    return ret;
}

void Dialog::C0SijaISvira()
{
    static bool counter_C0 = 0;
    counter_C0 = counter_C0^1;
    doWhenPressedOrReleased(counter_C0);
}

void Dialog::choose_note(QComboBox *c) // ubacivanje imena nota u padajuci meni
{

    int old_index = c->currentIndex();

    if (c->count()!=0) c->clear();
    for (int i=0;i<12;i++)
    {
        if (sharp_or_flat==sharp)
        {
            QString string_sharp (note_names_sharp[i]);
            c->insertItem(i+1,string_sharp,0);
        }
        if (sharp_or_flat==flat)
        {
            QString string_flat (note_names_flat[i]);
            c->insertItem(i+1,string_flat,0);
        }
    }
    c->setCurrentIndex(old_index);
}

void Dialog::izabranaNotaUPad(const QString &note)
{

    C0_nota = note;
    QImage can_image = ui->can0_pixmap->pixmap(Qt::ReturnByValue).toImage();
    int img_width = ui->can0_pixmap->pixmap(Qt::ReturnByValue).rect().width();
    int img_height = ui->can0_pixmap->pixmap(Qt::ReturnByValue).rect().height();
    QColor img_color = can_image.pixelColor(img_width/2,img_height/5);

    changecolor_C0(img_color); // ista boja ali razl. tekst
}


void Dialog::svirajNotuCB (QComboBox *c) // cb - combobox jer je u cb ime note
{
    QString ime_sampla;
    int sample_izabran = ui->comboBox_samples->currentIndex();

    if (sample_izabran==0)  ime_sampla = "grand piano/grand piano  - ";
    else if (sample_izabran==1)  ime_sampla = "modern upright/modern upright - ";
    else if (sample_izabran==2)  ime_sampla = "maple hill funk/maple hill funk - ";
    else if (sample_izabran==3)  ime_sampla = "voodoo magic/voodoo magic - ";
    else if (sample_izabran==4)  ime_sampla = "industrial pad/industrial pad - ";
    else if (sample_izabran==5)  ime_sampla = "current value h/current value h - ";

    QString temp (c->currentText());

    if (temp.contains("b"))
        temp = note_names_sharp[c->currentIndex()];

    if (temp.contains("♯"))
        temp.replace(QString("♯"),QString("s"));

    ime_sampla.append(temp);

    QString scale (ui->comboBox_scale->currentText());

    int octave = ui->label_octave->text().toInt();

    for (int i=0;i<30;i++)
        if(all_scales[i][0]==scale)
        {
            for (int j =0;j<8;j++)
            {
                if (all_scales[i][j]==c->currentText())
                {
                            if (scale_helper[i][j-1])
                            {
                                octave++;
                            }
                }
            }
        }

    if (octave>7) octave = 7;
    ime_sampla.append(QString::number(octave));  // slajder za oktavu, zavisi od izabrane oktave

    ime_sampla.append(".wav");
    ime_sampla.prepend(QString(SAMPLE_LOC));   // utice preset samples/effects
    ime_sampla.prepend("file:///");
    QUrl url_sampla= QUrl(ime_sampla);
    // url_sampla.toLocalFile();
    zvuk->setSource(url_sampla);

    int volumeSliderValue = ui->volume_slider->value();
    qreal linearVolume /* = QAudio::convertVolume(volumeSliderValue / qreal(100.0),
                                               QAudio::LogarithmicVolumeScale,
                                               QAudio::LinearVolumeScale)*/;
    linearVolume = volumeSliderValue / qreal(100.0);
    zvuk->setVolume(linearVolume);
    //qDebug ()<<"Jacina note je"<<linearVolume;
    zvuk->play();



}


void Dialog::doWhenPressedOrReleased(bool c_pressed)//,QOushButton* can_onoff_p,void (*changecolor_can)(QColor& can_color),QColor& can_color
{
    if (c_pressed) //pressed
    {
        if (ui->can0_onoff->text()=="ON") // on
        {
            changecolor_C0(C0_color);
            if (ui->syncWithPress->isChecked())
            {
                if (delay_ms!=0)
                {
                            int podsesno_ms = delay_ms;
                            delay_ms = 99999;
                            light_rgb(C0_color);
                            delay_ms = podsesno_ms;
                }

            }
            else
            {
                light_rgb(C0_color);
            }
            svirajNotuCB(ui->can0_changeNote);
        }
    }
    else if (!c_pressed) //released
    {
        changecolor_C0(to_darker_color(C0_color,3));
        light_rgb(Qt::black);
    }
}


void Dialog::can0_sharp_pressed()
{

    int new_index = ui->can0_changeNote->currentIndex();
    new_index++;
    if (new_index > 11 ) new_index = 0;
    ui->can0_changeNote->setCurrentIndex(new_index);

}

void Dialog::can0_flat_pressed()
{
    int new_index = ui->can0_changeNote->currentIndex();
    new_index--;
    if (new_index < 0 ) new_index = 11;
    ui->can0_changeNote->setCurrentIndex(new_index);
}

void Dialog::to_major(bool checked)
{
    if (checked)
    {
        ui->pushButton_minor->setChecked(0);
        emit major(); // signal
    }
    if (!checked)
    {
        ui->pushButton_minor->setChecked(1);
        emit minor();
    }
}

void Dialog::to_minor(bool checked)
{
    if (checked)
    {
        ui->pushButton_major->setChecked(0);
        emit minor(); // signal
    }
    if (!checked)
    {
        ui->pushButton_major->setChecked(1);
        emit major();
    }
}

void Dialog::puni_p_meni_major()
{
    QComboBox *c = ui->comboBox_scale;
    int index_stari = c->currentIndex();
    if (c->count()!=0) c->clear();
    for (int i=0;i<8;i++) // puni sa minor imenima
    {
        if (sharp_or_flat==sharp)
        {

            QString string_sharp (major_scale_names_sharp[i]);
            c->insertItem(i+1,string_sharp,0); // ubacivanje imena nota u padajuci meni
        }
        if (sharp_or_flat==flat)
        {
            QString string_flat (major_scale_names_flat[i]);
            c->insertItem(i+1,string_flat,0);
        }
    }

    c->setCurrentIndex(index_stari);

}

void Dialog::puni_p_meni_minor()
{
    QComboBox *c =  ui->comboBox_scale;
    int index_stari = c->currentIndex();
    if (c->count()!=0) c->clear();
    for (int i=0;i<8;i++) // puni sa minor imenima
    {
        if (ui->radioButton_sharp->isChecked()) //sharp
        {

            QString string_sharp (minor_scale_names_sharp[i]);
            c->insertItem(i+1,string_sharp,0); // ubacivanje imena nota u padajuci meni
        }
        else if (ui->radioButton_flat->isChecked())
        {
            QString string_flat (minor_scale_names_flat[i]);
            c->insertItem(i+1,string_flat,0);
        }
    }

    c->setCurrentIndex(index_stari);

}

void Dialog::setuj_tekst (int broj)
{
    QString temp = QString::number(broj);
    ui->label_octave->setText(temp);
}

void Dialog::radioButton_sharp_clicked(bool checked)
{
    if (checked)
    {
        sharp_or_flat = sharp;
        choose_note(ui->can0_changeNote);
        /*
        choose_note(ui->comboBox_change_note_2);
        choose_note(ui->comboBox_change_note_3);
        choose_note(ui->comboBox_change_note_4);
        choose_note(ui->comboBox_change_note_5);
        choose_note(ui->comboBox_change_note_6);
        choose_note(ui->comboBox_change_note_7);
        */ //kad prekopiras sve dodaj

        if(ui->pushButton_major->isChecked())
            puni_p_meni_major();
        else if(ui->pushButton_minor->isChecked())
            puni_p_meni_minor();


    }

}

void Dialog::radioButton_flat_clicked(bool checked)
{
    if (checked)
    {
        this->sharp_or_flat = flat;
        choose_note(ui->can0_changeNote);
        /*
        choose_note(ui->comboBox_change_note_2);
        choose_note(ui->comboBox_change_note_3);
        choose_note(ui->comboBox_change_note_4);
        choose_note(ui->comboBox_change_note_5);
        choose_note(ui->comboBox_change_note_6);
        choose_note(ui->comboBox_change_note_7);
        */

        if(ui->pushButton_major->isChecked())
            puni_p_meni_major();
        else if(ui->pushButton_minor->isChecked())
            puni_p_meni_minor();

    }


}

void Dialog::input_on_off_stateChanged(int arg1)
{
    /*
    if (arg1==Qt::Unchecked)
        disconnect(esp32, &QSerialPort::readyRead,this,&Dialog::readSerial);
    if (arg1 == Qt::Checked)
        connect(esp32, &QSerialPort::readyRead,this,&Dialog::readSerial);
*/ // ne, nego povezi sa svim on-off dugmadima

    if (arg1==Qt::Unchecked)
    {
        ui->can0_onoff->setText("OFF");
        ui->can1_onoff->setText("OFF");
        ui->can2_onoff->setText("OFF");
        ui->can3_onoff->setText("OFF");
        ui->can4_onoff->setText("OFF");
        ui->can5_onoff->setText("OFF");
        ui->can6_onoff->setText("OFF");
    }
    else if (arg1 == Qt::Checked)
    {
        ui->can0_onoff->setText("ON");
        ui->can1_onoff->setText("ON");
        ui->can2_onoff->setText("ON");
        ui->can3_onoff->setText("ON");
        ui->can4_onoff->setText("ON");
        ui->can5_onoff->setText("ON");
        ui->can6_onoff->setText("ON");
    }

}

void Dialog::output_on_off_stateChanged(int arg)
{
    if (arg == Qt::Checked)
    {
        ui->pushButton_red->setText("ON");
        ui->pushButton_green->setText("ON");
        ui->pushButton_blue->setText("ON");
        ui->pushButton_rgb->setText("ON");
    }
    else if (arg == Qt::Unchecked)
    {
        ui->pushButton_red->setText("OFF");
        ui->pushButton_green->setText("OFF");
        ui->pushButton_blue->setText("OFF");
        ui->pushButton_rgb->setText("OFF");

    }
}

void Dialog::C1SijaISvira()
{}

void Dialog::C2SijaISvira()
{}

void Dialog::C3SijaISvira()
{}

void Dialog::C4SijaISvira()
{}

void Dialog::C5SijaISvira()
{}

void Dialog::C6SijaISvira()
{}

