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
    connect(ui->pushButton,&QPushButton::clicked, this, &Dialog::pushButton_clicked); // test dugme


  // test

   // write_diode(plava,9000,100);

    connect(ui->can0_chooseColor,&QPushButton::clicked, this, &Dialog::show_can0_colordialog);
    connect(ui->can1_chooseColor,&QPushButton::clicked,this,&Dialog::show_can1_colordialog);
    connect(ui->can2_chooseColor,&QPushButton::clicked,this,&Dialog::show_can2_colordialog);
    connect(ui->can3_chooseColor,&QPushButton::clicked,this,&Dialog::show_can3_colordialog);
    connect(ui->can4_chooseColor,&QPushButton::clicked,this,&Dialog::show_can4_colordialog);
    connect(ui->can5_chooseColor,&QPushButton::clicked,this,&Dialog::show_can5_colordialog);
    connect(ui->can6_chooseColor,&QPushButton::clicked,this,&Dialog::show_can6_colordialog);

 /*
    changecolor_C0(to_darker_color(C0_color,3));
    changecolor_C1(to_darker_color(C1_color,3));
    changecolor_C2(to_darker_color(C2_color,3));
    changecolor_C3(to_darker_color(C3_color,3));
    changecolor_C4(to_darker_color(C4_color,3));
    changecolor_C5(to_darker_color(C5_color,3));
    changecolor_C6(to_darker_color(C5_color,3));
*/
    changeAllAtOnce(to_darker_color(Qt::red,3));

    can0_colordialog_declare();
    can1_colordialog_declare();
    can2_colordialog_declare();
    can3_colordialog_declare();
    can4_colordialog_declare();
    can5_colordialog_declare();
    can6_colordialog_declare();

    ui->lineEdit_delay->setMaxLength(MAX_DELAY_LENGTH); // max delay 9999ms
    ui->lineEdit_delay->setText(QString::number(default_delay)); // pod. vrednost

    connect(ui->lineEdit_delay,&QLineEdit::textEdited,this,&Dialog::delay_updated);

    ui->label_green->setPixmap(changeImageColor(REDDIODE_IMG,Qt::green));
    ui->label_blue->setPixmap(changeImageColor(REDDIODE_IMG,Qt::blue));
    ui->label_rgb->setPixmap(changeImageColor(REDDIODE_IMG,default_rgb));

    connect(ui->pushButton_red,&QPushButton::clicked,this,&Dialog::red_onoff);
    connect(ui->pushButton_green,&QPushButton::clicked,this,&Dialog::green_onoff);
    connect(ui->pushButton_blue,&QPushButton::clicked,this,&Dialog::blue_onoff);
    connect(ui->pushButton_rgb,&QPushButton::clicked,this,&Dialog::rgb_onoff);

   // ui->pushButton_red_over->setStyleSheet("background-color: rgb(255,0,0);");
    ui->pushButton_red_over->setStyleSheet("* { background-color: rgba(0,125,0,0) }");
    ui->pushButton_green_over->setStyleSheet("* { background-color: rgba(0,125,0,0) }");
    ui->pushButton_blue_over->setStyleSheet("* { background-color: rgba(0,125,0,0) }");
    ui->pushButton_rgb_over->setStyleSheet("* { background-color: rgba(0,125,0,0) }");

    connect(ui->pushButton_red_over,&QPushButton::clicked,this,&Dialog::redled_pic_clicked);
    connect(ui->pushButton_green_over,&QPushButton::clicked,this,&Dialog::greenled_pic_clicked);
    connect(ui->pushButton_blue_over,&QPushButton::clicked,this,&Dialog::blueled_pic_clicked);
    connect(ui->pushButton_rgb_over,&QPushButton::clicked,this,&Dialog::rgbled_pic_clicked);

    connect(ui->spinBox_red,QOverload<int>::of(&QSpinBox::valueChanged),this,&Dialog::red_jacina_change);
    connect(ui->spinBox_green,QOverload<int>::of(&QSpinBox::valueChanged),this,&Dialog::green_jacina_change);
    connect(ui->spinBox_blue,QOverload<int>::of(&QSpinBox::valueChanged),this,&Dialog::blue_jacina_change);
    connect(ui->spinBox_rgb,QOverload<int>::of(&QSpinBox::valueChanged),this,&Dialog::rgb_jacina_change);


    connect(ui->izaberiBoju_rgb,&QPushButton::clicked,this,&Dialog::popup_change_color_rgb);

    rgb_dialog = new QColorDialog(default_rgb,this);
    connect(rgb_dialog,&QColorDialog::currentColorChanged,this,&Dialog::changeRgbLabelColor);
    connect(rgb_dialog,&QColorDialog::currentColorChanged,this,&Dialog::light_rgb);

    connect(rgb_dialog,&QColorDialog::colorSelected,this,&Dialog::rgb_colordialog_ok);
    connect(rgb_dialog,&QColorDialog::finished,this,&Dialog::rgb_colordialog_cancel);

    ui->pot_link->insertItems(0,{"red","green","blue","rgb"});
    ui->pot_link->setCurrentIndex(2); //def plava

    connect(ui->pot_link,&QComboBox::currentTextChanged,this,&Dialog::linkLedToPot);

    ui->red_delay->setText(QString::number(default_delay));
    ui->red_delay->setMaxLength(MAX_DELAY_LENGTH);

    ui->green_delay->setText(QString::number(default_delay));
    ui->green_delay->setMaxLength(MAX_DELAY_LENGTH);

    ui->blue_delay->setText(QString::number(default_delay));
    ui->blue_delay->setMaxLength(MAX_DELAY_LENGTH);

    ui->rgb_delay->setText(QString::number(default_delay));
    ui->rgb_delay->setMaxLength(MAX_DELAY_LENGTH);

    connect(ui->red_delay,&QLineEdit::textChanged,this,&Dialog::red_delay_updated);
    connect(ui->green_delay,&QLineEdit::textChanged,this,&Dialog::green_delay_updated);
    connect(ui->blue_delay,&QLineEdit::textChanged,this,&Dialog::blue_delay_updated);
    connect(ui->rgb_delay,&QLineEdit::textChanged,this,&Dialog::rgb_delay_updated);

    ui->volume_slider->setDisabled(true);

    connect(ui->can0_onoff,&QPushButton::clicked,this,&Dialog::iskljuci_C0);
    connect(ui->can1_onoff,&QPushButton::clicked,this,&Dialog::iskljuci_C1);
    connect(ui->can2_onoff,&QPushButton::clicked,this,&Dialog::iskljuci_C2);
    connect(ui->can3_onoff,&QPushButton::clicked,this,&Dialog::iskljuci_C3);
    connect(ui->can4_onoff,&QPushButton::clicked,this,&Dialog::iskljuci_C4);
    connect(ui->can5_onoff,&QPushButton::clicked,this,&Dialog::iskljuci_C5);
    connect(ui->can6_onoff,&QPushButton::clicked,this,&Dialog::iskljuci_C6);


    connect(ui->C0_click,&QPushButton::pressed,this,&Dialog::can0_pressed);
    connect(ui->C0_click,&QPushButton::released,this,&Dialog::can0_released);
    ui->C0_click->setStyleSheet("* { background-color: rgba(0,125,0,0) }");

    connect(ui->C1_click,&QPushButton::pressed,this,&Dialog::can1_pressed);
    connect(ui->C1_click,&QPushButton::released,this,&Dialog::can1_released);
    ui->C1_click->setStyleSheet("* { background-color: rgba(0,125,0,0) }");

    connect(ui->C2_click,&QPushButton::pressed,this,&Dialog::can2_pressed);
    connect(ui->C2_click,&QPushButton::released,this,&Dialog::can2_released);
    ui->C2_click->setStyleSheet("* { background-color: rgba(0,125,0,0) }");

    connect(ui->C3_click,&QPushButton::pressed,this,&Dialog::can3_pressed);
    connect(ui->C3_click,&QPushButton::released,this,&Dialog::can3_released);
    ui->C3_click->setStyleSheet("* { background-color: rgba(0,125,0,0) }");

    connect(ui->C4_click,&QPushButton::pressed,this,&Dialog::can4_pressed);
    connect(ui->C4_click,&QPushButton::released,this,&Dialog::can4_released);
    ui->C4_click->setStyleSheet("* { background-color: rgba(0,125,0,0) }");

    connect(ui->C5_click,&QPushButton::pressed,this,&Dialog::can5_pressed);
    connect(ui->C5_click,&QPushButton::released,this,&Dialog::can5_released);
    ui->C5_click->setStyleSheet("* { background-color: rgba(0,125,0,0) }");

    connect(ui->C6_click,&QPushButton::pressed,this,&Dialog::can6_pressed);
    connect(ui->C6_click,&QPushButton::released,this,&Dialog::can6_released);
    ui->C6_click->setStyleSheet("* { background-color: rgba(0,125,0,0) }");



    puniPadMeni(ui->can0_changeNote); // puni pad. meni
    puniPadMeni(ui->can1_changeNote);
    puniPadMeni(ui->can2_changeNote);
    puniPadMeni(ui->can3_changeNote);
    puniPadMeni(ui->can4_changeNote);
    puniPadMeni(ui->can5_changeNote);
    puniPadMeni(ui->can6_changeNote);

    ui->can0_changeNote->setCurrentIndex(0) ; // podraz. vrednost (C dur)
    connect(ui->can0_changeNote,&QComboBox::currentTextChanged,this,&Dialog::izabranaNotaUPad_C0);

    ui->can1_changeNote->setCurrentIndex(2) ;
    connect(ui->can1_changeNote,&QComboBox::currentTextChanged,this,&Dialog::izabranaNotaUPad_C1);

    ui->can2_changeNote->setCurrentIndex(4) ;
    connect(ui->can2_changeNote,&QComboBox::currentTextChanged,this,&Dialog::izabranaNotaUPad_C2);

    ui->can3_changeNote->setCurrentIndex(5) ;
    connect(ui->can3_changeNote,&QComboBox::currentTextChanged,this,&Dialog::izabranaNotaUPad_C3);

    ui->can4_changeNote->setCurrentIndex(7) ;
    connect(ui->can4_changeNote,&QComboBox::currentTextChanged,this,&Dialog::izabranaNotaUPad_C4);

    ui->can5_changeNote->setCurrentIndex(9) ;
    connect(ui->can5_changeNote,&QComboBox::currentTextChanged,this,&Dialog::izabranaNotaUPad_C5);

    ui->can6_changeNote->setCurrentIndex(11) ;
    connect(ui->can6_changeNote,&QComboBox::currentTextChanged,this,&Dialog::izabranaNotaUPad_C6);

    ui->comboBox_samples->addItems(sample_names);
    ui->comboBox_samples->setCurrentIndex(0); //grand piano default

    connect(ui->can0_sharpen,&QPushButton::clicked,this,&Dialog::can0_sharp_pressed);
    connect(ui->can0_flatten,&QPushButton::clicked,this,&Dialog::can0_flat_pressed);

    connect(ui->can1_sharpen,&QPushButton::clicked,this,&Dialog::can1_sharp_pressed);
    connect(ui->can1_flatten,&QPushButton::clicked,this,&Dialog::can1_flat_pressed);

    connect(ui->can2_sharpen,&QPushButton::clicked,this,&Dialog::can2_sharp_pressed);
    connect(ui->can2_flatten,&QPushButton::clicked,this,&Dialog::can2_flat_pressed);

    connect(ui->can3_sharpen,&QPushButton::clicked,this,&Dialog::can3_sharp_pressed);
    connect(ui->can3_flatten,&QPushButton::clicked,this,&Dialog::can3_flat_pressed);

    connect(ui->can4_sharpen,&QPushButton::clicked,this,&Dialog::can4_sharp_pressed);
    connect(ui->can4_flatten,&QPushButton::clicked,this,&Dialog::can4_flat_pressed);

    connect(ui->can5_sharpen,&QPushButton::clicked,this,&Dialog::can5_sharp_pressed);
    connect(ui->can5_flatten,&QPushButton::clicked,this,&Dialog::can5_flat_pressed);

    connect(ui->can6_sharpen,&QPushButton::clicked,this,&Dialog::can6_sharp_pressed);
    connect(ui->can6_flatten,&QPushButton::clicked,this,&Dialog::can6_flat_pressed);

    connect(ui->pushButton_major,&QPushButton::clicked,this,&Dialog::emitMajorMinor); //emit major/minor
    emitMajorMinor(true); // inicijalizacija

    connect(ui->pushButton_minor,&QPushButton::clicked,this,&Dialog::emitMinorMajor);

    connect (this,&Dialog::major,this,&Dialog::puni_p_meni_major);
    connect (this,&Dialog::minor,this,&Dialog::puni_p_meni_minor);

    puni_p_meni_major(); // inic vrednost p menija
    ui->comboBox_scale->setCurrentIndex(0); // inic vr
    connect (ui->comboBox_scale,&QComboBox::currentTextChanged,this,&Dialog::menjaj_skalu);

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

    connect(ui->colorBox,&QComboBox::currentTextChanged,this,&Dialog::changeAllColors);
    ui->colorBox->insertItems(0,{"All Red","All White","Rainbow"});
    ui->colorBox->setCurrentIndex(2); //def duga

    connect(ui->record,&QPushButton::toggled,this,&Dialog::start_recording);
    e_timer = new QElapsedTimer;

    connect(ui->play,&QPushButton::toggled,this,&Dialog::start_playing);
    p_timer = new QTimer(this);
    p_timer->setSingleShot(true);
    connect(p_timer,&QTimer::timeout,this,&Dialog::play_again);

    connect(ui->test,&QPushButton::toggled,this,&Dialog::start_test);
    t_timer = new QTimer(this);
    t_timer->setSingleShot(true);
    connect(t_timer,&QTimer::timeout,this,&Dialog::svirajNotuT);

    test_result = new QMessageBox (this);
}

Dialog::~Dialog()
{

    if(esp32->isOpen()){
        esp32->close(); //    zatvori serijski port ako je otvoren
    }

    delete esp32;
    delete can0_colordialog;
    delete can1_colordialog;
    delete can2_colordialog;
    delete can3_colordialog;
    delete can4_colordialog;
    delete can5_colordialog;
    delete can6_colordialog;
    delete check_if_connected;
    delete rgb_dialog;
    delete can0_zvuk;
    delete can1_zvuk;
    delete can2_zvuk;
    delete can3_zvuk;
    delete can4_zvuk;
    delete can5_zvuk;
    delete can6_zvuk;
    delete e_timer;
    delete p_timer;
    delete t_timer;
    delete efekat;
    delete test_result;

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
    if (l==zelena && en_green == 0) return;
    if (l==plava && en_blue == 0) return;

    if (l==crvena) delay_ms =red_delay_ms;
    else if (l==zelena) delay_ms =green_delay_ms;
    else if (l==plava) delay_ms =blue_delay_ms;

    string_to_write = (QByteArray::number(l)).append(" , ").append(QByteArray::number(delay_ms)).append(" , ").append(QByteArray::number(jacina)).append('\0');
    if ( (esp32->write(string_to_write)) == -1 )   //ako nije mogao upisati, '\0'
    {
       // qDebug()<< "Nije moguce upaliti diodu";
        if (esp32_prikacen)
        {
        //QMessageBox prozor_greske (QMessageBox::Critical,"Error!","Device currently in use.",QMessageBox::Ok,this,Qt::Widget); //moze se namestiti da prikazuje eror
        QMessageBox* prozor_greske = new QMessageBox(QMessageBox::Critical,"Serial Port Error","Couldn't write to device.",QMessageBox::NoButton,this);
        prozor_greske->setStandardButtons(QMessageBox::NoButton);
        prozor_greske->setInformativeText("Retrying...If persists, try reconnecting the device");
        prozor_greske->setWindowModality(Qt::NonModal);

        // napravi tajmer koji ce pokusavati da upise stalno
        QTimer *timer_temp = new QTimer (this);
        connect(timer_temp,&QTimer::timeout,this,&Dialog::pokusajUpisatiOpet);

        connect(this,&Dialog::upis_uspesan,timer_temp,&QTimer::stop);
        connect(this,&Dialog::upis_uspesan,prozor_greske,&QMessageBox::accept);
        connect(this,&Dialog::upis_uspesan,timer_temp,&QTimer::deleteLater);
        connect(this,&Dialog::upis_uspesan,prozor_greske,&QMessageBox::deleteLater);
        write_diode(crvena,delay_ms,jacina_red);
        timer_temp->start(CHECKING_RATE);
        prozor_greske->exec();
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
    write_diode(zelena,delay_ms,jacina_green); // od 0 do 100 jacine
    write_diode(crvena,delay_ms,jacina_red);
    write_diode(plava,delay_ms,jacina_blue);
    write_diode(rgb_R,delay_ms,201); //C900FF pink
    write_diode(rgb_G,delay_ms,0);
    write_diode(rgb_B,delay_ms,255);
}

void Dialog::show_can0_colordialog()
{

    can0_colordialog->show();
}

void Dialog::show_can1_colordialog()
{
    can1_colordialog->show();
}

void Dialog::show_can2_colordialog()
{
    can2_colordialog->show();
}

void Dialog::show_can3_colordialog()
{
    can3_colordialog->show();
}

void Dialog::show_can4_colordialog()
{
    can4_colordialog->show();
}

void Dialog::show_can5_colordialog()
{
    can5_colordialog->show();
}

void Dialog::show_can6_colordialog()
{
    can6_colordialog->show();
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

void Dialog::changecolor_C1(const QColor &color)
{
    changecolor(color,C1_nota,ui->can1_pixmap);
}

void Dialog::changecolor_C2(const QColor &color)
{
    changecolor(color,C2_nota,ui->can2_pixmap);
}

void Dialog::changecolor_C3(const QColor &color)
{
    changecolor(color,C3_nota,ui->can3_pixmap);
}

void Dialog::changecolor_C4(const QColor &color)
{
    changecolor(color,C4_nota,ui->can4_pixmap);
}

void Dialog::changecolor_C5(const QColor &color)
{
    changecolor(color,C5_nota,ui->can5_pixmap);
}

void Dialog::changecolor_C6(const QColor &color)
{
    changecolor(color,C6_nota,ui->can6_pixmap);
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


void Dialog::can_colordialog_ok(const QColor &color,void_noarg can_colordialog_declare_f,QColor* can_color_g,
                                void_constqcolor changecolor_can,bool* can_colordialog_ok_pressed)
{
    // qDebug()<<"Ok pritisnuto!";
     (this->*can_colordialog_declare_f)();
     *can_color_g = color; // cuva u mem
     (this->*changecolor_can)(to_darker_color(*can_color_g,3));//255/3
     changeRgbLabelColor(rgb_color);
     *can_colordialog_ok_pressed = true;
}


void Dialog::can0_colordialog_ok(const QColor &color)
{
/*
    // qDebug()<<"Ok pritisnuto!";
     can0_colordialog_declare();
     C0_color = color; // cuva u mem
     changecolor_C0(to_darker_color(C0_color,3));//255/3
     changeRgbLabelColor(rgb_color);
     can0_colordialog_ok_pressed = true;
*/
     can_colordialog_ok(color,can0_colordialog_declare,&C0_color,changecolor_C0,&can0_colordialog_ok_pressed);
}

void Dialog::can1_colordialog_ok(const QColor &color)
{
     can_colordialog_ok(color,can1_colordialog_declare,&C1_color,changecolor_C1,&can1_colordialog_ok_pressed);
}

void Dialog::can2_colordialog_ok(const QColor &color)
{
     can_colordialog_ok(color,can2_colordialog_declare,&C2_color,changecolor_C2,&can2_colordialog_ok_pressed);
}

void Dialog::can3_colordialog_ok(const QColor &color)
{
     can_colordialog_ok(color,can3_colordialog_declare,&C3_color,changecolor_C3,&can3_colordialog_ok_pressed);
}

void Dialog::can4_colordialog_ok(const QColor &color)
{
     can_colordialog_ok(color,can4_colordialog_declare,&C4_color,changecolor_C4,&can4_colordialog_ok_pressed);
}

void Dialog::can5_colordialog_ok(const QColor &color)
{
     can_colordialog_ok(color,can5_colordialog_declare,&C5_color,changecolor_C5,&can5_colordialog_ok_pressed);
}

void Dialog::can6_colordialog_ok(const QColor &color)
{
     can_colordialog_ok(color,can6_colordialog_declare,&C6_color,changecolor_C6,&can6_colordialog_ok_pressed);
}

void Dialog::can_colordialog_cancel(bool* can_colordialog_ok_pressed,void_noarg can_colordialog_declare_f,void_constqcolor changecolor_can_f
                                    ,QColor* can_color_g)
{
     if (!(*can_colordialog_ok_pressed))
     {
        //qDebug()<<"Cancel ili X pritisnuto!";
        (this->*can_colordialog_declare_f)();
        (this->*changecolor_can_f)(to_darker_color(*can_color_g,3));
        changeRgbLabelColor(rgb_color);
     }
     else
        *can_colordialog_ok_pressed = false;
}


void Dialog::can0_colordialog_cancel()
{
/*
     if (!can0_colordialog_ok_pressed)
     {
      qDebug()<<"Cancel ili X pritisnuto!";
      can0_colordialog_declare();
      changecolor_C0(to_darker_color(C0_color,3));
      changeRgbLabelColor(rgb_color);
     }
     else
      can0_colordialog_ok_pressed = false;
*/
     can_colordialog_cancel(&can0_colordialog_ok_pressed,can0_colordialog_declare,changecolor_C0,&C0_color);
}

void Dialog::can1_colordialog_cancel()
{
     can_colordialog_cancel(&can1_colordialog_ok_pressed,can1_colordialog_declare,changecolor_C1,&C1_color);
}

void Dialog::can2_colordialog_cancel()
{
     can_colordialog_cancel(&can2_colordialog_ok_pressed,can2_colordialog_declare,changecolor_C2,&C2_color);
}

void Dialog::can3_colordialog_cancel()
{
     can_colordialog_cancel(&can3_colordialog_ok_pressed,can3_colordialog_declare,changecolor_C3,&C3_color);
}

void Dialog::can4_colordialog_cancel()
{
     can_colordialog_cancel(&can4_colordialog_ok_pressed,can4_colordialog_declare,changecolor_C4,&C4_color);
}

void Dialog::can5_colordialog_cancel()
{
     can_colordialog_cancel(&can5_colordialog_ok_pressed,can5_colordialog_declare,changecolor_C5,&C5_color);
}

void Dialog::can6_colordialog_cancel()
{
     can_colordialog_cancel(&can6_colordialog_ok_pressed,can6_colordialog_declare,changecolor_C6,&C6_color);
}

void Dialog::write_rgb(const QColor &color, int delay , int jacina_rgb ) //max 100
{

     if (!en_rgb) return;
     delay = rgb_delay_ms;

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

     errorCheckDelay(text,ui->lineEdit_delay,ui->label_error,&delay_ms); //delay_ms se menja
     ui->red_delay->setText(QString::number(delay_ms));
     ui->green_delay->setText(QString::number(delay_ms));
     ui->blue_delay->setText(QString::number(delay_ms));
     ui->rgb_delay->setText(QString::number(delay_ms));
}

void Dialog::changeRgbLabelColor(const QColor& color)
{
    ui->label_rgb->setPixmap(changeImageColor(REDDIODE_IMG,color));
}


void Dialog::x_onoff (bool* en_x_g,QPushButton* onoff_but_p,QLabel* label_x_p,led_enum led_boja,int* jacina_x_g) // pressed = true - on
{
    //clicked - jedino na korisnikove klikove se poziva
    *en_x_g=*en_x_g^1;
    //  qDebug()<<en_red;
    if (!(*en_x_g))
    {
      onoff_but_p->setText("OFF");
      int r = 0, g = 0, b = 0;
      if (led_boja == crvena) r = 128;
      else if (led_boja == zelena) g = 128;
      else if (led_boja == plava) b = 128; //255/2 ne vidi se promena ispod 128
      else if (led_boja == rgb)
            {
                QColor color_temp = rgb_color;
                color_temp.setHsv(color_temp.hue(),color_temp.saturation(),color_temp.value()/2);
                r=color_temp.red(); g=color_temp.green(); b=color_temp.blue();
            }
      label_x_p->setPixmap(changeImageColor(REDDIODE_IMG,QColor::fromRgb(r,g,b)));
    }
    else
    {
      onoff_but_p->setText("ON");
      qreal logBrightness = QAudio::convertVolume(*jacina_x_g / qreal(100.0), //0-100 -> 0.0-1.0
                                                     QAudio::LinearVolumeScale,
                                                    QAudio::LogarithmicVolumeScale);
      int logBrightness_int = qRound(logBrightness*100); // u linearnu skalu
      int x_max_255 = doubleMap(logBrightness_int,0,100,128,255); //pretvori
      int r = 0, g = 0, b = 0;
      if (led_boja == crvena) r = x_max_255;
      else if (led_boja == zelena) g = x_max_255;
      else if (led_boja == plava) b = x_max_255;
      else if (led_boja == rgb)
      {
            x_max_255 = doubleMap(logBrightness_int,0,100,0,255);
            QColor color_temp = rgb_color;
            color_temp.setHsv(color_temp.hue(),color_temp.saturation(),x_max_255);
            r=color_temp.red(); g=color_temp.green(); b=color_temp.blue();
      }
      label_x_p->setPixmap(changeImageColor(REDDIODE_IMG,QColor::fromRgb(r,g,b)));
    }
}


void Dialog::red_onoff () // pressed = true - on
{
 /*
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
*/
    x_onoff(&en_red,ui->pushButton_red,ui->label_red,crvena,&jacina_red);
}
void Dialog::green_onoff ()
{
    x_onoff(&en_green,ui->pushButton_green,ui->label_green,zelena,&jacina_green);
}

void Dialog::blue_onoff ()
{
    x_onoff(&en_blue,ui->pushButton_blue,ui->label_blue,plava,&jacina_blue);
}

void Dialog::rgb_onoff ()
{
    x_onoff(&en_rgb,ui->pushButton_rgb,ui->label_rgb,rgb,&jacina_rgb);
}


void Dialog::redled_pic_clicked()
{
    write_diode(crvena,delay_ms,jacina_red); // jacina
}

void Dialog::greenled_pic_clicked()
{
    write_diode(zelena,delay_ms,jacina_green);
}

void Dialog::blueled_pic_clicked()
{
    write_diode(plava,delay_ms,jacina_blue);
}

void Dialog::rgbled_pic_clicked()
{
    write_rgb(rgb_color,delay_ms,jacina_rgb);
}




void Dialog::x_jacina_change(int x_jacina_novo,int* jacina_x_g,led_enum boja,QLabel* label_x_p)
{

    *jacina_x_g = x_jacina_novo; //jacina_x - 0 do 100

    qreal linearBrightness = QAudio::convertVolume(*jacina_x_g / qreal(100.0), //0-100 -> 0.0-1.0
                                                   QAudio::LogarithmicVolumeScale,
                                                   QAudio::LinearVolumeScale);
    *jacina_x_g = qRound(linearBrightness*100); // u log skalu

    switch (boja)
    {
        case crvena: if (!en_red) return; break;
        case zelena: if (!en_green) return; break;
        case plava: if (!en_blue) return; break;
        case rgb: if (!en_rgb) return; break;
        default: break;
    }
    if (ui->lightWhileChanging_B->isChecked())
    {
      if (boja!=rgb)
        write_diode(boja,delay_ms,*jacina_x_g);
      else
        write_rgb(rgb_color,delay_ms,*jacina_x_g);
    }

    int x_max_255 = doubleMap(x_jacina_novo,0,100,128,255); //pretvori
    int r=0,g=0,b=0;
    QColor boja_pom = rgb_color;
    switch(boja)
    {
        case crvena: r=x_max_255; break;
        case zelena: g=x_max_255; break;
        case plava: b=x_max_255; break;
        case rgb:
            x_max_255 = doubleMap(x_jacina_novo,0,100,0,255);
            boja_pom.setHsv(boja_pom.hue(),boja_pom.saturation(),x_max_255);
            r= boja_pom.red(); g= boja_pom.green(); b= boja_pom.blue();
            break;
        default: break;
    }
    label_x_p->setPixmap(changeImageColor(REDDIODE_IMG,QColor::fromRgb(r,g,b)));

}



void Dialog::red_jacina_change(int red_jacina_novo)
{
/*
    jacina_red = red_jacina_novo; //jacina_red - 0 do 100

    qreal linearBrightness = QAudio::convertVolume(jacina_red / qreal(100.0), //0-100 -> 0.0-1.0
                                               QAudio::LogarithmicVolumeScale,
                                               QAudio::LinearVolumeScale);
    jacina_red = qRound(linearBrightness*100); // u log skalu

    if (!en_red) return;
    if (ui->lightWhileChanging_B->isChecked())
        write_diode(crvena,delay_ms,jacina_red); // bice opcija za ovo
    int red_max_255 = doubleMap(red_jacina_novo,0,100,128,255); //pretvori
    ui->label_red->setPixmap(changeImageColor(REDDIODE_IMG,QColor::fromRgb(red_max_255,0,0)));
*/
    x_jacina_change(red_jacina_novo,&jacina_red,crvena,ui->label_red);
}

void Dialog::green_jacina_change(int green_jacina_novo)
{
    x_jacina_change(green_jacina_novo,&jacina_green,zelena,ui->label_green);
}

void Dialog::blue_jacina_change(int blue_jacina_novo)
{
    x_jacina_change(blue_jacina_novo,&jacina_blue,plava,ui->label_blue);
}

void Dialog::rgb_jacina_change(int rgb_jacina_novo)
{
    x_jacina_change(rgb_jacina_novo,&jacina_rgb,rgb,ui->label_rgb);
}

int Dialog::doubleMap(double x, double in_min, double in_max, double out_min,double out_max)
{
    return int((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min);
}

void Dialog::pot_value_is_changing(int pot_value)
{
    changeVolumeSlider(pot_value);

    qreal linearBrightness = QAudio::convertVolume(pot_value / qreal(100.0), //0-100 -> 0.0-1.0
                                                   QAudio::LogarithmicVolumeScale,
                                                   QAudio::LinearVolumeScale);
    pot_value = qRound(linearBrightness*100); // u log skalu
    if (pot_led!=rgb)
        write_diode(pot_led,delay_ms,pot_value);
    else
        write_rgb(rgb_color,delay_ms,pot_value);


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
                write_diode(zelena,delay_ms,jacina_green);
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
            write_diode(crvena,delay_ms,jacina_red);
            msbx->show();
        }

    }
}

void Dialog::popup_change_color_rgb()
{
    rgb_dialog->show();

}

void Dialog::rgb_colordialog_ok(const QColor& color)
{
    //Ok pritisnuto
    rgb_color = color;
    changeRgbLabelColor(rgb_color);
    int value_max100 = doubleMap(color.value(),0,255,0,100);
    ui->spinBox_rgb->setValue(value_max100);
    rgb_colordialog_ok_pressed = true;
}

void Dialog::rgb_colordialog_cancel()
{
    if (!rgb_colordialog_ok_pressed)
    {
        //Cancel ili X pritisnuto

        changeRgbLabelColor(rgb_color);
        disconnect(rgb_dialog,&QColorDialog::currentColorChanged,this,&Dialog::light_rgb);
        rgb_dialog->setCurrentColor(rgb_color);
        connect(rgb_dialog,&QColorDialog::currentColorChanged,this,&Dialog::light_rgb);
    }
    else
        rgb_colordialog_ok_pressed = false;
}

void Dialog::linkLedToPot(const QString &text) // tekst- red green blue rgb
{
    if(text=="red")pot_led = crvena;
    if(text=="green")pot_led = zelena;
    if(text=="blue")pot_led = plava;
    if(text=="rgb")pot_led = rgb;
}

void Dialog::x_delay_updated(const QString &text,QLineEdit* x_delay_p,QLabel* label_error_x_p,int* x_delay_ms_g,QLabel* label_x,
                             led_enum boja,bool* en_x_g,int* jacina_x_g) //text = vrednost delay za red
{
    errorCheckDelay(text,x_delay_p,label_error_x_p,x_delay_ms_g);
    if (*x_delay_ms_g == 0)
    {
        int r=0,g=0,b=0;
        switch(boja)
        {
            case crvena:r=128;break;
            case zelena:g=128;break;
            case plava:b=128;break;
            case rgb: break;
            default:break;
        }
            label_x->setPixmap(changeImageColor(REDDIODE_IMG,QColor::fromRgb(r,g,b)));
    }
    else if(*en_x_g)
    {
        int xMax_255 = doubleMap(*jacina_x_g,0,100,128,255); //pretvori
        int r=0,g=0,b=0;
        QColor temp_c = rgb_color;
        switch(boja)
        {
        case crvena:r=xMax_255;break;
        case zelena:g=xMax_255;break;
        case plava:b=xMax_255;break;
        case rgb:
            xMax_255 = doubleMap(*jacina_x_g,0,100,0,255);
            temp_c.setHsv(temp_c.hue(),temp_c.saturation(),xMax_255);
            r= temp_c.red(); g= temp_c.green(); b= temp_c.blue();
            break;
        default: break;
        }
        label_x->setPixmap(changeImageColor(REDDIODE_IMG,QColor::fromRgb(r,g,b)));
    }
}

void Dialog::red_delay_updated(const QString &text) //text = vrednost delay za red
{
/*
    errorCheckDelay(text,ui->red_delay,ui->label_error_red,&red_delay_ms);
    if (red_delay_ms == 0)
        ui->label_red->setPixmap(changeImageColor(REDDIODE_IMG,QColor::fromRgb(128,0,0)));
    else if(en_red)
    {
        int redMax_255 = doubleMap(jacina_red,0,100,128,255); //pretvori
        ui->label_red->setPixmap(changeImageColor(REDDIODE_IMG,QColor::fromRgb(redMax_255,0,0)));
    }
*/
    x_delay_updated(text,ui->red_delay,ui->label_error_red,&red_delay_ms,ui->label_red,crvena,&en_red,&jacina_red);
}

void Dialog::green_delay_updated(const QString &text)
{
    x_delay_updated(text,ui->green_delay,ui->label_error_green,&green_delay_ms,ui->label_green,zelena,&en_green,&jacina_green);
}

void Dialog::blue_delay_updated(const QString &text)
{
    x_delay_updated(text,ui->blue_delay,ui->label_error_blue,&blue_delay_ms,ui->label_blue,plava,&en_blue,&jacina_blue);
}

void Dialog::rgb_delay_updated(const QString &text)
{
    x_delay_updated(text,ui->rgb_delay,ui->label_error_rgb,&rgb_delay_ms,ui->label_rgb,rgb,&en_rgb,&jacina_rgb);
}

void Dialog::errorCheckDelay(const QString& text, QLineEdit* x_delay_p, QLabel* label_error_x_p, int* x_delay_ms_g)

{

    QRegExp re ("\\d*"); //jeste broj
    if (re.exactMatch(text))
    {
        //qDebug()<<"Uneti string je broj!";
        bool ok;                          //ako su sve nule
        int text_int = text.toInt(&ok);
        if (ok) x_delay_p->setText(QString::number(text_int));
        if (text_int==0 && ok)
        {
            QString temp_s;
            if (x_delay_p == ui->lineEdit_delay) temp_s = "<font color='purple'>Diodes off.</font>";
            else temp_s = "<font color='purple'>Diode off.</font>";
                label_error_x_p->setText(temp_s);

            *x_delay_ms_g = text_int;
        }
        else
        {
            label_error_x_p->setText(""); //validna vrednost uneta
            *x_delay_ms_g = text_int;
        }
    }
    else
    {
        // qDebug()<<"Uneti string nije broj!";
        label_error_x_p->setText("<font color='red'>Input invalid!</font>");
        x_delay_p->setText("");
        *x_delay_ms_g = 0;
    }

    if (text=="")
    {
        label_error_x_p->setText("<font color='red'>Insert a value!</font>");
        *x_delay_ms_g = 0;
    }

}


void Dialog::can_pressedOrReleased(bool* pressed,QVector<int> c_changed) //kao interapt
{
    //qDebug()<<"pressed:"<<pressed;

    //changecolor_C0(C0_color);

    foreach(int i, c_changed)
    {
        bool is_pressed = pressed[i]==1;
        if (i == 0 && C0_on) //first can
        {
            doWhenPressedOrReleased(is_pressed,ui->can0_onoff,changecolor_C0,C0_color,ui->can0_changeNote);
        }
        if (i == 1 && C1_on)
        {
            doWhenPressedOrReleased(is_pressed,ui->can1_onoff,changecolor_C1,C1_color,ui->can1_changeNote);
        }
        if (i == 2 && C2_on)
        {
            doWhenPressedOrReleased(is_pressed,ui->can2_onoff,changecolor_C2,C2_color,ui->can2_changeNote);
        }
        if (i == 3 && C3_on)
        {
            doWhenPressedOrReleased(is_pressed,ui->can3_onoff,changecolor_C3,C3_color,ui->can3_changeNote);
        }
        if (i == 4 && C4_on)
        {
            doWhenPressedOrReleased(is_pressed,ui->can4_onoff,changecolor_C4,C4_color,ui->can4_changeNote);
        }
        if (i == 5 && C5_on)
        {
            doWhenPressedOrReleased(is_pressed,ui->can5_onoff,changecolor_C5,C5_color,ui->can5_changeNote);
        }
        if (i == 6 && C6_on)
        {
            doWhenPressedOrReleased(is_pressed,ui->can6_onoff,changecolor_C6,C6_color,ui->can6_changeNote);
        }
    }
}

void Dialog::changeVolumeSlider(int new_value)
{
    ui->volume_slider->setEnabled(true);
    ui->volume_slider->setValue(new_value);
    ui->volume_slider->setDisabled(true);
}

void Dialog::can_colordialog_declare(QColorDialog** can_colordialog_g, void_constqcolor changecolor_can_f, void_constqcolor can_colordialog_ok_f, void_noarg can_colordialog_cancel_f)
{
    *can_colordialog_g = new QColorDialog(default_rgb,this); // can1
    //(*can_colordialog_g)->setOption(QColorDialog::DontUseNativeDialog);
    connect(*can_colordialog_g,&QColorDialog::currentColorChanged,this,changecolor_can_f);

    connect(*can_colordialog_g,&QColorDialog::currentColorChanged,this,&Dialog::changeRgbLabelColor);
    connect(*can_colordialog_g,&QColorDialog::currentColorChanged,this,&Dialog::light_rgb);

    connect(*can_colordialog_g,&QColorDialog::colorSelected,this,can_colordialog_ok_f);
    connect(*can_colordialog_g,&QColorDialog::finished,this,can_colordialog_cancel_f);

}


void Dialog::can0_colordialog_declare()
{
    /*
    can0_colordialog = new QColorDialog(default_rgb,this); // can1
    //can0_colordialog->setOption(QColorDialog::DontUseNativeDialog);
    connect(can0_colordialog,&QColorDialog::currentColorChanged,this,&Dialog::changecolor_C0);
    connect(can0_colordialog,&QColorDialog::currentColorChanged,this,&Dialog::changeRgbLabelColor);
    connect(can0_colordialog,&QColorDialog::currentColorChanged,this,&Dialog::light_rgb);

    connect(can0_colordialog,&QColorDialog::colorSelected,this,&Dialog::colordialog_ok);
    connect(can0_colordialog,&QColorDialog::finished,this,&Dialog::colordialog_cancel);
*/
    can_colordialog_declare(&can0_colordialog,changecolor_C0,can0_colordialog_ok,can0_colordialog_cancel);
}

void Dialog::can1_colordialog_declare()
{
    can_colordialog_declare(&can1_colordialog,changecolor_C1,can1_colordialog_ok,can1_colordialog_cancel);
}

void Dialog::can2_colordialog_declare()
{
    can_colordialog_declare(&can2_colordialog,changecolor_C2,can2_colordialog_ok,can2_colordialog_cancel);
}

void Dialog::can3_colordialog_declare()
{
    can_colordialog_declare(&can3_colordialog,changecolor_C3,can3_colordialog_ok,can3_colordialog_cancel);
}

void Dialog::can4_colordialog_declare()
{
    can_colordialog_declare(&can4_colordialog,changecolor_C4,can4_colordialog_ok,can4_colordialog_cancel);
}

void Dialog::can5_colordialog_declare()
{
    can_colordialog_declare(&can5_colordialog,changecolor_C5,can5_colordialog_ok,can5_colordialog_cancel);
}

void Dialog::can6_colordialog_declare()
{
    can_colordialog_declare(&can6_colordialog,changecolor_C6,can6_colordialog_ok,can6_colordialog_cancel);
}

void Dialog::iskljuci_can(QPushButton* can_onoff_p,bool* can_on_g)
{
    bool on;
    if (can_onoff_p->text()=="ON") //gasi se
    {
        can_onoff_p->setText("OFF");

        on = false;
    }
    else  // pali se
    {
        can_onoff_p->setText("ON");
        on = true;
    }
    *can_on_g = on;

}


void Dialog::iskljuci_C0()
{
/*
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
*/
    iskljuci_can(ui->can0_onoff,&C0_on);
}

void Dialog::iskljuci_C1()
{
    iskljuci_can(ui->can1_onoff,&C1_on);
}

void Dialog::iskljuci_C2()
{
    iskljuci_can(ui->can2_onoff,&C2_on);
}

void Dialog::iskljuci_C3()
{
    iskljuci_can(ui->can3_onoff,&C3_on);
}

void Dialog::iskljuci_C4()
{
    iskljuci_can(ui->can4_onoff,&C4_on);
}

void Dialog::iskljuci_C5()
{
    iskljuci_can(ui->can5_onoff,&C5_on);
}

void Dialog::iskljuci_C6()
{
    iskljuci_can(ui->can6_onoff,&C6_on);
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

void Dialog::can0_pressed()
{
    doWhenPressedOrReleased(true,ui->can0_onoff,changecolor_C0,C0_color,ui->can0_changeNote);
}

void Dialog::can0_released()
{
    doWhenPressedOrReleased(false,ui->can0_onoff,changecolor_C0,C0_color,ui->can0_changeNote);
}


void Dialog::can1_pressed()
{
    doWhenPressedOrReleased(true,ui->can1_onoff,changecolor_C1,C1_color,ui->can1_changeNote);
}

void Dialog::can1_released()
{
    doWhenPressedOrReleased(false,ui->can1_onoff,changecolor_C1,C1_color,ui->can1_changeNote);
}


void Dialog::can2_pressed()
{
    doWhenPressedOrReleased(true,ui->can2_onoff,changecolor_C2,C2_color,ui->can2_changeNote);
}

void Dialog::can2_released()
{
    doWhenPressedOrReleased(false,ui->can2_onoff,changecolor_C2,C2_color,ui->can2_changeNote);
}


void Dialog::can3_pressed()
{
    doWhenPressedOrReleased(true,ui->can3_onoff,changecolor_C3,C3_color,ui->can3_changeNote);
}

void Dialog::can3_released()
{
    doWhenPressedOrReleased(false,ui->can3_onoff,changecolor_C3,C3_color,ui->can3_changeNote);
}


void Dialog::can4_pressed()
{
    doWhenPressedOrReleased(true,ui->can4_onoff,changecolor_C4,C4_color,ui->can4_changeNote);
}

void Dialog::can4_released()
{
    doWhenPressedOrReleased(false,ui->can4_onoff,changecolor_C4,C4_color,ui->can4_changeNote);
}


void Dialog::can5_pressed()
{
    doWhenPressedOrReleased(true,ui->can5_onoff,changecolor_C5,C5_color,ui->can5_changeNote);
}

void Dialog::can5_released()
{
    doWhenPressedOrReleased(false,ui->can5_onoff,changecolor_C5,C5_color,ui->can5_changeNote);
}


void Dialog::can6_pressed()
{
    doWhenPressedOrReleased(true,ui->can6_onoff,changecolor_C6,C6_color,ui->can6_changeNote);
}

void Dialog::can6_released()
{
    doWhenPressedOrReleased(false,ui->can6_onoff,changecolor_C6,C6_color,ui->can6_changeNote);
}


void Dialog::puniPadMeni(QComboBox* can_changeNote) // ubacivanje imena nota u padajuci meni
{

    int old_index = can_changeNote->currentIndex();

    if (can_changeNote->count()!=0) can_changeNote->clear();
    for (int i=0;i<12;i++)
    {
        if (sharp_or_flat==sharp)
        {
            QString string_sharp (note_names_sharp[i]);
            can_changeNote->insertItem(i+1,string_sharp,0);
        }
        if (sharp_or_flat==flat)
        {
            QString string_flat (note_names_flat[i]);
            can_changeNote->insertItem(i+1,string_flat,0);
        }
    }
    can_changeNote->setCurrentIndex(old_index);
}

void Dialog::izabranaNotaUPad(const QString &note,QString* nota_g_p,QLabel* can_pixmap_p,void_constqcolor func)
{
    *nota_g_p = note;
    QImage can_image = can_pixmap_p->pixmap(Qt::ReturnByValue).toImage();
    int img_width = can_pixmap_p->pixmap(Qt::ReturnByValue).rect().width();
    int img_height = can_pixmap_p->pixmap(Qt::ReturnByValue).rect().height();
    QColor img_color = can_image.pixelColor(img_width/2,img_height/5);
    (this->*func)(img_color);     // ista boja ali razl. tekst

}

void Dialog::izabranaNotaUPad_C0(const QString &note)
{
    izabranaNotaUPad(note,&C0_nota,ui->can0_pixmap,changecolor_C0);
}

void Dialog::izabranaNotaUPad_C1(const QString &note)
{
    izabranaNotaUPad(note,&C1_nota,ui->can1_pixmap,changecolor_C1);
}

void Dialog::izabranaNotaUPad_C2(const QString &note)
{
    izabranaNotaUPad(note,&C2_nota,ui->can2_pixmap,changecolor_C2);
}

void Dialog::izabranaNotaUPad_C3(const QString &note)
{
    izabranaNotaUPad(note,&C3_nota,ui->can3_pixmap,changecolor_C3);
}

void Dialog::izabranaNotaUPad_C4(const QString &note)
{
    izabranaNotaUPad(note,&C4_nota,ui->can4_pixmap,changecolor_C4);
}

void Dialog::izabranaNotaUPad_C5(const QString &note)
{
    izabranaNotaUPad(note,&C5_nota,ui->can5_pixmap,changecolor_C5);
}

void Dialog::izabranaNotaUPad_C6(const QString &note)
{
    izabranaNotaUPad(note,&C6_nota,ui->can6_pixmap,changecolor_C6);
}

void Dialog::svirajNotuCB (QComboBox *can_changeNote_p) // cb - combobox jer je u cb ime note
{
    QString ime_sampla;
    int sample_izabran = ui->comboBox_samples->currentIndex();

    if (sample_izabran==0)  ime_sampla = "grand piano/grand piano  - ";
    else if (sample_izabran==1)  ime_sampla = "modern upright/modern upright - ";
    else if (sample_izabran==2)  ime_sampla = "maple hill funk/maple hill funk - ";
    else if (sample_izabran==3)  ime_sampla = "voodoo magic/voodoo magic - ";
    else if (sample_izabran==4)  ime_sampla = "industrial pad/industrial pad - ";
    else if (sample_izabran==5)  ime_sampla = "current value h/current value h - ";

    QString temp (can_changeNote_p->currentText());

    if (temp.contains("b"))
        temp = note_names_sharp[can_changeNote_p->currentIndex()];

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
                if (all_scales[i][j]==can_changeNote_p->currentText())
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
    QSoundEffect* can_zvuk;
    if      (can_changeNote_p == ui->can0_changeNote)
        can_zvuk =  can0_zvuk;
    else if (can_changeNote_p == ui->can1_changeNote)
        can_zvuk =  can1_zvuk;
    else if (can_changeNote_p == ui->can2_changeNote)
        can_zvuk =  can2_zvuk;
    else if (can_changeNote_p == ui->can3_changeNote)
        can_zvuk =  can3_zvuk;
    else if (can_changeNote_p == ui->can4_changeNote)
        can_zvuk =  can4_zvuk;
    else if (can_changeNote_p == ui->can5_changeNote)
        can_zvuk =  can5_zvuk;
    else if (can_changeNote_p == ui->can6_changeNote)
        can_zvuk =  can6_zvuk;

    can_zvuk->setSource(url_sampla);

    int volumeSliderValue = ui->volume_slider->value();
    qreal linearVolume /* = QAudio::convertVolume(volumeSliderValue / qreal(100.0),
                                               QAudio::LogarithmicVolumeScale,
                                               QAudio::LinearVolumeScale)*/;
    linearVolume = volumeSliderValue / qreal(100.0);
    can_zvuk->setVolume(linearVolume);
    //qDebug ()<<"Jacina note je"<<linearVolume;
    can_zvuk->play();

    if (korisnik_unosi) //deo gde korisnik pritiska
    {
        korisnik_odgovor.append(CB_to_Number(can_changeNote_p));
        if (korisnik_odgovor.size() >= ui->test_level->value())
        {
            // kraj
           // qDebug()<<"k_o:"<<korisnik_odgovor;
           // qDebug()<<"t_o:"<<tacan_odgovor;
            korisnik_unosi = false;


            QString string (SAMPLE_LOC);
            QUrl url;
            string.prepend("file:///");

            if (korisnik_odgovor == tacan_odgovor) //tacno
            {
                string.append("correct.wav");
                url = QUrl (string);
                efekat->setSource(url);
                efekat->play();
                write_diode(zelena,delay_ms,jacina_green);
               // QMessageBox::about(this,"Congrats!","That is the correct answer"); NE RADI!!
                test_result->setWindowTitle("Congrats!");
                test_result->setText("That is the correct answer");
                test_result->show();

            }
            else // netacno
            {
                string.append("wrong.wav");
                url = QUrl (string);
                efekat->setSource(url);
                efekat->play();
                write_diode(crvena,delay_ms,jacina_red);
                //QMessageBox::about(this,"Too bad!","Better luck next time!");
                test_result->setWindowTitle("Too bad!");
                test_result->setText("Better luck next time!");
                test_result->show();
            }
            korisnik_odgovor.clear();
            ui->test->setChecked(false);
            return;


        }
        else
        {
        }

    }

}


void Dialog::doWhenPressedOrReleased(bool c_pressed, QPushButton* can_onoff_p, void_constqcolor f, QColor& can_color_g, QComboBox* can_changeNote_p)   //,QOushButton* can_onoff_p,void (*void_constqcolor)(QColor& can_color),QColor& can_color
{               //g - global f - funkcija

    if (c_pressed) //pressed
    {
        if (can_onoff_p->text()=="ON") // on
        {
            (this->*f)(can_color_g);
            if (ui->syncWithPress->isChecked())
            {
                if (delay_ms!=0)
                {
                            int podsesno_ms = delay_ms;
                            delay_ms = 99999;
                            light_rgb(can_color_g);
                            delay_ms = podsesno_ms;
                }

            }
            else
            {
                light_rgb(can_color_g);
            }
            changeRgbLabelColor(can_color_g);
            svirajNotuCB(can_changeNote_p);
        }
    }
    else if (!c_pressed) //released
    {
        (this->*f)(to_darker_color(can_color_g,3));
        light_rgb(Qt::black);
        changeRgbLabelColor(rgb_color);

    }

    if (rec_mode_on)
    {
        recorded_times.append(e_timer->restart());
        //qDebug()<<recorded_times.last();
        c_pressed_m.append(c_pressed);
        can_onoff_m.append(can_onoff_p);
        f_m.append(f);
        can_color_m.append(can_color_g);
        can_changeNote_m.append(can_changeNote_p);
    }
}


void Dialog::can_sharp_pressed(QComboBox* can_changeNote_p)
{

    int new_index = can_changeNote_p->currentIndex();
    new_index++;
    if (new_index > 11 ) new_index = 0;
    can_changeNote_p->setCurrentIndex(new_index);

}


void Dialog::can0_sharp_pressed()
{
/*
    int new_index = ui->can0_changeNote->currentIndex();
    new_index++;
    if (new_index > 11 ) new_index = 0;
    ui->can0_changeNote->setCurrentIndex(new_index);
*/
    can_sharp_pressed(ui->can0_changeNote);

}

void Dialog::can1_sharp_pressed()
{
    can_sharp_pressed(ui->can1_changeNote);
}

void Dialog::can2_sharp_pressed()
{
    can_sharp_pressed(ui->can2_changeNote);
}

void Dialog::can3_sharp_pressed()
{
    can_sharp_pressed(ui->can3_changeNote);
}

void Dialog::can4_sharp_pressed()
{
    can_sharp_pressed(ui->can4_changeNote);
}

void Dialog::can5_sharp_pressed()
{
    can_sharp_pressed(ui->can5_changeNote);
}

void Dialog::can6_sharp_pressed()
{
    can_sharp_pressed(ui->can6_changeNote);
}

void Dialog::can_flat_pressed(QComboBox* can_changeNote_p)
{
    int new_index = can_changeNote_p->currentIndex();
    new_index--;
    if (new_index < 0 ) new_index = 11;
    can_changeNote_p->setCurrentIndex(new_index);
}


void Dialog::can0_flat_pressed()
{
/*
    int new_index = ui->can0_changeNote->currentIndex();
    new_index--;
    if (new_index < 0 ) new_index = 11;
    ui->can0_changeNote->setCurrentIndex(new_index);
*/
    can_flat_pressed(ui->can0_changeNote);

}

void Dialog::can1_flat_pressed()
{
    can_flat_pressed(ui->can1_changeNote);
}

void Dialog::can2_flat_pressed()
{
    can_flat_pressed(ui->can2_changeNote);
}

void Dialog::can3_flat_pressed()
{
    can_flat_pressed(ui->can3_changeNote);
}

void Dialog::can4_flat_pressed()
{
    can_flat_pressed(ui->can4_changeNote);
}

void Dialog::can5_flat_pressed()
{
    can_flat_pressed(ui->can5_changeNote);
}

void Dialog::can6_flat_pressed()
{
    can_flat_pressed(ui->can6_changeNote);
}


void Dialog::emitMajorMinor(bool checked)
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

void Dialog::emitMinorMajor(bool checked)
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
        puniPadMeni(ui->can0_changeNote);
        puniPadMeni(ui->can1_changeNote);
        puniPadMeni(ui->can2_changeNote);
        puniPadMeni(ui->can3_changeNote);
        puniPadMeni(ui->can4_changeNote);
        puniPadMeni(ui->can5_changeNote);
        puniPadMeni(ui->can6_changeNote);


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
        puniPadMeni(ui->can0_changeNote);
        puniPadMeni(ui->can1_changeNote);
        puniPadMeni(ui->can2_changeNote);
        puniPadMeni(ui->can3_changeNote);
        puniPadMeni(ui->can4_changeNote);
        puniPadMeni(ui->can5_changeNote);
        puniPadMeni(ui->can6_changeNote);

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



void Dialog::pokusajUpisatiOpet()
{
    esp32 -> open(QSerialPort::ReadWrite);
    if ( (esp32->write(string_to_write)) != -1 )
        emit upis_uspesan();
}


void Dialog::changeAllColors(const QString &text)
{
    if (text=="All Red")
    {
        C0_color = Qt::red;
        C1_color = Qt::red;
        C2_color = Qt::red;
        C3_color = Qt::red;
        C4_color = Qt::red;
        C5_color = Qt::red;
        C6_color = Qt::red;
        changeAllAtOnce(to_darker_color(Qt::red,3));
    }
    else if (text=="All White")
    {
        C0_color = Qt::white;
        C1_color = Qt::white;
        C2_color = Qt::white;
        C3_color = Qt::white;
        C4_color = Qt::white;
        C5_color = Qt::white;
        C6_color = Qt::white;
        changeAllAtOnce(to_darker_color(Qt::white,3));
    }
    else if (text=="Rainbow")
    {
        C0_color = QColor("#E81416"); //red
        C1_color = QColor("#FFA500"); //orange
        C2_color = QColor("#FAEB36"); //yellow
        C3_color = QColor("#79C314"); //green
        C4_color = QColor("#487DE7"); //blue
        C5_color = QColor("#4B369D"); //indigo
        C6_color = QColor("#70369D"); //violet
        changecolor_C0(to_darker_color(C0_color,3));
        changecolor_C1(to_darker_color(C1_color,3));
        changecolor_C2(to_darker_color(C2_color,3));
        changecolor_C3(to_darker_color(C3_color,3));
        changecolor_C4(to_darker_color(C4_color,3));
        changecolor_C5(to_darker_color(C5_color,3));
        changecolor_C6(to_darker_color(C6_color,3));
    }
}

void Dialog::changeAllAtOnce(const QColor &c)
{
    changecolor_C0(c);
    changecolor_C1(c);
    changecolor_C2(c);
    changecolor_C3(c);
    changecolor_C4(c);
    changecolor_C5(c);
    changecolor_C6(c);

}

void Dialog::menjaj_skalu(const QString &arg1)
{

    for (int i=0; i<30;i++) // 30 skala postoji
    {
        QString text;

        if (all_scales [i][0] == arg1)
        {
            text = all_scales [i][1];
            ui->can0_changeNote->setCurrentText(text);
            text.clear();

            text = all_scales [i][2];
            ui->can1_changeNote->setCurrentText(text);
            text.clear();

            text = all_scales [i][3];
            ui->can2_changeNote->setCurrentText(text);
            text.clear();

            text = all_scales [i][4];
            ui->can3_changeNote->setCurrentText(text);
            text.clear();

            text = all_scales [i][5];
            ui->can4_changeNote->setCurrentText(text);
            text.clear();

            text = all_scales [i][6];
            ui->can5_changeNote->setCurrentText(text);
            text.clear();

            text = all_scales [i][7];
            ui->can6_changeNote->setCurrentText(text);
            text.clear();
        }
    }
}

void Dialog::start_recording(bool pressed)
{
    if (pressed)
    {
        ui->play->setChecked(false);
        ui->test->setChecked(false);

        rec_mode_on = true;

        play_mode_on = false;
        // treba zapoceti brojanje ovde
        e_timer->start();

        recorded_times.clear();
        c_pressed_m.clear();
        can_onoff_m.clear();
        f_m.clear();
        can_color_m.clear();
        can_changeNote_m.clear();
    }
    else
    {
        rec_mode_on = false;
    }
}

void Dialog::start_playing(bool pressed)
{
    if (pressed)
    {
        ui->record->setChecked(false);
        ui->test->setChecked(false);

        play_mode_on = true;

        rec_mode_on = false;


        if (!recorded_times.isEmpty()) //nesto je snimljeno
        {
                i_m = 0;
                p_timer->start(static_cast<int>(recorded_times[i_m]));
        }
    }
    else
    {
        play_mode_on = false;
    }

}

void Dialog::play_again()
{
    doWhenPressedOrReleased(c_pressed_m[i_m],can_onoff_m[i_m],f_m[i_m],can_color_m[i_m],can_changeNote_m[i_m]);
    //qDebug()<<recorded_times[i_m];
    i_m++;
    if (i_m>=recorded_times.size())
    {
        i_m = 0;
        ui->play->setChecked(false);
        return;
    }
    p_timer->start(int(recorded_times[i_m]));
}

void Dialog::start_test(bool pressed)
{
    if (pressed)
    {

        ui->record->setChecked(false);
        ui->play->setChecked(false);


        rec_mode_on = false;
        play_mode_on = false;

        tacan_odgovor.clear();
        svirajNotuT();
    }
    else
    {
        t_timer->stop();
        tacan_odgovor.clear();
        korisnik_unosi = false;
        korisnik_odgovor.clear();
    }

}

void Dialog::svirajNotuN(int can_number)
{
    QComboBox* c = NULL;
    switch (can_number)
    {
        case 0: c=ui->can0_changeNote;break;
        case 1: c=ui->can1_changeNote;break;
        case 2: c=ui->can2_changeNote;break;
        case 3: c=ui->can3_changeNote;break;
        case 4: c=ui->can4_changeNote;break;
        case 5: c=ui->can5_changeNote;break;
        case 6: c=ui->can6_changeNote;break;
        default: break;

    }
    svirajNotuCB(c);
}

void Dialog::svirajNotuT()
{
    int level = ui->test_level->value();
    if (tacan_odgovor.size()>=level )
    {

        QString s;
        s.append("Play the ");
        s.append(QString::number(level));
        s.append(" note");
        if (level>1) s.append("s");
        s.append(" you just heard.");

        QMessageBox::information(this,"Test Mode",s);
        korisnik_unosi = true;
        korisnik_odgovor.clear();
        return;
    }
    else
    {
        tacan_odgovor.append(QRandomGenerator::global()->bounded(0,7)); //sa 0 bez 7
        svirajNotuN(tacan_odgovor.last()); // n-number
        write_diode(plava,delay_ms,jacina_blue);
        t_timer->start(NOTE_SPAN);
    }

}

int Dialog::CB_to_Number (QComboBox* c)
{
    int ret;
         if (c==ui->can0_changeNote) ret = 0;
    else if (c==ui->can1_changeNote) ret = 1;
    else if (c==ui->can2_changeNote) ret = 2;
    else if (c==ui->can3_changeNote) ret = 3;
    else if (c==ui->can4_changeNote) ret = 4;
    else if (c==ui->can5_changeNote) ret = 5;
    else if (c==ui->can6_changeNote) ret = 6;
    return ret;
}
