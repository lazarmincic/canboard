#ifndef DIALOG_H
#define DIALOG_H




#define BAUD 250000
#define default_delay 1000
#define default_rgb Qt::red
#define default_can Qt::black
#define pot_margin 3
#define timer_ms 250
#define CANRED_IMG "C:/Users/Lazar/Desktop/can_red.png"
#define REDDIODE_IMG "C:/Users/Lazar/Desktop/all_red_diode.png"
#define CHECKING_RATE 400 //ms
#define MAX_DELAY_LENGTH 4
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define SAMPLE_LOC "C:/Users/Lazar/Desktop/canboard_files/"
#define NOTE_SPAN 900 //ms test mod

#include <qdebug.h>
#include <qstring.h>
#include <qmessagebox.h>
#include <qstringlist.h>


#include <QDialog>  // sve include u h fajl
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QByteArray>
#include <QIODevice>
#include <QMessageBox>
#include <QtGlobal>
#include <QPixmap>
#include <QBitmap>
#include <QPainter>
#include <QColorDialog>
#include <QColor>
#include <QLineEdit>
#include <QRegExp>
#include <QSpinBox>
#include <QTimer>
#include <QVector>
#include <QRandomGenerator>
#include <QComboBox>
#include <QImage>
#include <QSoundEffect>
#include <QAudio>
#include <QElapsedTimer>
#include <QDeadlineTimer>
#include <QDateTime>


//qt verzija 5.15

enum led_enum {gasi,zelena = 1,crvena = 2,plava = 3,rgb_R =4,rgb_G=5,rgb_B=6,rgb};

enum note_mode {sharp, flat};


QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE



class Dialog : public QDialog
{
    Q_OBJECT


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

    int esp32_prikacen = -1;

    QStringList buffer_split_list;

    bool prvi_put_usao = 0;

    // jer pocinje sa ni jednim K pritisnutim i sa najacim zvukom
    bool pressed_prethodna [7] = {0,0,0,0,0,0,0};
    int pot_value_prethodna = 100;

    QColorDialog* can0_colordialog;
    QColorDialog* can1_colordialog;
    QColorDialog* can2_colordialog;
    QColorDialog* can3_colordialog;
    QColorDialog* can4_colordialog;
    QColorDialog* can5_colordialog;
    QColorDialog* can6_colordialog;

    QColor C0_color = Qt::red;
    QColor C1_color = Qt::red;
    QColor C2_color = Qt::red;
    QColor C3_color = Qt::red;
    QColor C4_color = Qt::red;
    QColor C5_color = Qt::red;
    QColor C6_color = Qt::red;
    QColor C7_color = Qt::red;

    bool can0_colordialog_ok_pressed = false;
    bool can1_colordialog_ok_pressed = false;
    bool can2_colordialog_ok_pressed = false;
    bool can3_colordialog_ok_pressed = false;
    bool can4_colordialog_ok_pressed = false;
    bool can5_colordialog_ok_pressed = false;
    bool can6_colordialog_ok_pressed = false;

    void write_rgb (const QColor &color,int delay= default_delay ,int jacina_rgb= -1);

    int delay_ms = default_delay;
    int red_delay_ms = default_delay;
    int green_delay_ms = default_delay;
    int blue_delay_ms = default_delay;
    int rgb_delay_ms = default_delay;

    QPixmap changeImageColor(const QString &path,const QColor &color);
    void changeRgbLabelColor(const QColor& color);

    bool en_red = 1; //enable za led
    bool en_green = 1;
    bool en_blue = 1;
    bool en_rgb = 1;

    int jacina_red = 100;
    int jacina_green = 100;
    int jacina_blue = 100;
    int jacina_rgb = 100;

    int doubleMap(double x, double in_min, double in_max, double out_min,double out_max);

    void pot_value_is_changing(int pot_value);

    QString esp32_port_name;

    void esp32_id_print();

    QTimer* check_if_connected;

    QColorDialog* rgb_dialog;

    QColor rgb_color = Qt::red;

    bool rgb_colordialog_ok_pressed = false;

    led_enum pot_led = plava; //default

    void errorCheckDelay(const QString& text, QLineEdit *x_delay_p, QLabel *label_error_x_p, int *x_delay_ms_g);

    void can_pressedOrReleased(bool* pressed,QVector<int> c_changed);

    void changeVolumeSlider(int new_value);
    void can0_colordialog_declare();
    void can1_colordialog_declare();
    void can2_colordialog_declare();
    void can3_colordialog_declare();
    void can4_colordialog_declare();
    void can5_colordialog_declare();
    void can6_colordialog_declare();

    QColor to_darker_color(QColor oc,int times_darker);

    bool C0_on = true;
    bool C1_on = true;
    bool C2_on = true;
    bool C3_on = true;
    bool C4_on = true;
    bool C5_on = true;
    bool C6_on = true;

    void writeOnImage(QPixmap *pxmp, const QString &note_name, const QColor &font_color);
    QColor findTextColor(const QColor &color);

   /* quint32 seed_r;
    quint32 seed_g;
    quint32 seed_b;

    QRandomGenerator* rand_r;
    QRandomGenerator* rand_g;
    QRandomGenerator* rand_b;
*/

    void puniPadMeni(QComboBox* can_changeNote );

    note_mode sharp_or_flat = sharp;

    QString C0_nota = "C";
    QString C1_nota = "D";
    QString C2_nota = "E";
    QString C3_nota = "F";
    QString C4_nota = "G";
    QString C5_nota = "A";
    QString C6_nota = "B";

    void svirajNotuCB (QComboBox* can_changeNote_p);


    void puni_p_meni_major();
    void puni_p_meni_minor();

    QSoundEffect* can0_zvuk = new QSoundEffect ;
    QSoundEffect* can1_zvuk = new QSoundEffect ;
    QSoundEffect* can2_zvuk = new QSoundEffect ;
    QSoundEffect* can3_zvuk = new QSoundEffect ;
    QSoundEffect* can4_zvuk = new QSoundEffect ;
    QSoundEffect* can5_zvuk = new QSoundEffect ;
    QSoundEffect* can6_zvuk = new QSoundEffect ;

    void changecolor(const QColor &color, QString nota_str, QLabel* can_pixmap_p);

    typedef void (Dialog::*void_constqcolor)(const QColor &);
    typedef void (Dialog::*void_noarg)();
    void doWhenPressedOrReleased(bool c_pressed, QPushButton* can_onoff_p, void_constqcolor f, QColor &can_color_g, QComboBox* can_changeNote_p);
    void izabranaNotaUPad(const QString &note,QString* nota_g_p,QLabel* can_pixmap_p,void_constqcolor func);


    void can_colordialog_declare(QColorDialog** can_colordialog_g, void_constqcolor changecolor_can_f, void_constqcolor can_colordialog_ok_f, void_noarg can_colordialog_cancel_f);
    void can_colordialog_ok(const QColor &color,void_noarg can_colordialog_declare_f,QColor* can_color_g,
                                         void_constqcolor changecolor_can,bool* can_colordialog_ok_pressed);
    void can_colordialog_cancel(bool* can_colordialog_ok_pressed,void_noarg can_colordialog_declare_f,void_constqcolor changecolor_can_f
                                ,QColor* can_color_g);

    void x_onoff (bool* en_x_g,QPushButton* onoff_but_p,QLabel* label_x_p,led_enum led_boja,int* jacina_x_g);
    void x_jacina_change(int x_jacina_novo,int* jacina_x_g,led_enum boja,QLabel* label_x_p);
    void x_delay_updated(const QString &text, QLineEdit* x_delay_p, QLabel* label_error_x_p, int* x_delay_ms_g, QLabel* label_x,
                                 led_enum boja, bool *en_x_g, int* jacina_x_g);
    void iskljuci_can(QPushButton* can_onoff_p,bool* can_on_g);
    void can_sharp_pressed(QComboBox* can_changeNote_p);
    void can_flat_pressed(QComboBox* can_changeNote_p);

    QByteArray string_to_write;

    void changeAllAtOnce(const QColor &c);

    bool korisnik_unosi = 0;
    bool rec_mode_on = 0;
    bool play_mode_on = 0;

    QElapsedTimer* e_timer ;
    QVector<qint64> recorded_times; // svaki put kad se udje u dowhenpressedorreleased

    QVector<bool> c_pressed_m; // m - memory
    QVector<QPushButton*> can_onoff_m;
    QVector<void_constqcolor> f_m;
    QVector<QColor> can_color_m;
    QVector<QComboBox*> can_changeNote_m;

    int i_m;

    QTimer *p_timer ; // p - play
    QTimer *t_timer ; // t -test

    QVector<int> tacan_odgovor;

    void svirajNotuN(int can_number);


    int CB_to_Number (QComboBox* c);

    QVector<int> korisnik_odgovor;

    QSoundEffect* efekat = new QSoundEffect;
    QMessageBox* test_result;


protected:

    const char note_names_sharp [12][5] = {"C","C♯","D","D♯","E","F","F♯","G","G♯","A","A♯","B"}; // imena nota sarp mod
    const char note_names_flat [12][5] = {"C","Db","D","Eb","E","F","Gb","G","Ab","A","Bb","B"}; // imena nota flat mod
    const QStringList sample_names = {"Grand Piano","Modern Upright","Maple Hill Funk","Voodoo Magic","Industrial Pad","Current Value H"};
    const char major_scale_names_sharp [8][11] = {"C Major","G Major","D Major","A Major","E Major","B Major","F♯ Major","C♯ Major"}; // imena skala sharp mod
    const char major_scale_names_flat [8][11] = {"C Major","F Major","Bb Major","Eb Major","Ab Major","Cb Major","Gb Major","Db Major"}; // imena skala flat mod
    const char minor_scale_names_sharp [8][11] = {"A Minor","E Minor","B Minor","F♯ Minor","C♯ Minor","G♯ Minor","D♯ Minor","A♯ Minor"}; //////////////
    const char minor_scale_names_flat [8][11] = {"A Minor", "D Minor","G Minor","C Minor", "F Minor","Ab Minor","Eb Minor","Bb Minor"};
    const char all_scales [30][8][11] = {{"A Minor","A","B","C","D","E","F","G"},{"D Minor","D","E","F","G","A","Bb","C"},{"G Minor","G","A","Bb","C","D","Eb","F"},         // 30 skala ukupno, 15 major 15 minor
        {"C Minor","C","D","Eb","F","G","Ab","Bb"},{"F Minor","F","G","Ab","Bb","C","Db","Eb"},{"Ab Minor","Ab","Bb","B","Db","Eb","E","Gb"},
        {"Eb Minor","Eb","F","Gb","Ab","Bb","B","Db"},{"Bb Minor","Bb","C","Db","Eb","F","Gb","Ab"},{"E Minor","E","F♯","G","A","B","C","D"},
        {"B Minor","B","C♯","D","E","F♯","G","A"},{"F♯ Minor","F♯","G♯","A","B","C♯","D","E"},{"C♯ Minor","C♯","D♯","E","F♯","G♯","A","B"},
        {"G♯ Minor","G♯","A♯","B","C♯","D♯","E","F♯"},{"D♯ Minor","D♯","F","F♯","G♯","A♯","B","C♯"},{"A♯ Minor","A♯","C","C♯","D♯","F","F♯","G♯"},

        {"C Major","C","D","E","F","G","A","B"},{"F Major","F","G","A","Bb","C","D","E"},{"Bb Major","Bb","C","D","Eb","F","G","A"},
        {"Eb Major","Eb","F","G","Ab","Bb","C","D"},{"Ab Major","Ab","Bb","C","Db","Eb","F","G"},{"Cb Major","B","Db","Eb","E","Gb","Ab","Bb"},
        {"Gb Major","Gb","Ab","Bb","B","Db","Eb","F"},{"Db Major","Db","Eb","F","Gb","Ab","Bb","C"},    {"G Major","G","A","B","C","D","E","F♯"},
        {"D Major","D","E","F♯","G","A","B","C♯"},{"A Major","A","B","C♯","D","E","F♯","G♯"},{"E Major","E","F♯","G♯","A","B","C♯","D♯"},
        {"B Major","B","C♯","D♯","E","F♯","G♯","A♯"},{"F♯ Major","F♯","G♯","A♯","B","C♯","D♯","F"},{"C♯ Major","C♯","D♯","F","F♯","G♯","A♯","C"}
    };

    const bool scale_helper [30][7] = {{0,0,1,1,1,1,1},{0,0,0,0,0,0,1},{0,0,0,1,1,1,1},{0,0,0,0,0,0,0},{0,0,0,0,1,1,1},{0,0,0,1,1,1,1},
        {0,0,0,0,0,0,1},{0,1,1,1,1,1,1},{0,0,0,0,0,1,1},{0,1,1,1,1,1,1},{0,0,0,0,1,1,1},{0,0,0,0,0,0,0},{0,0,0,1,1,1,1},{0,0,0,0,0,0,1},{0,1,1,1,1,1,1},

        {0,0,0,0,0,0,0},{0,0,0,0,1,1,1},{0,1,1,1,1,1,1},{0,0,0,0,0,1,1},{0,0,1,1,1,1,1},{0,1,1,1,1,1,1},{0,0,0,0,1,1,1},{0,0,0,0,0,0,1},{0,0,0,1,1,1,1},
        {0,0,0,0,0,0,1},{0,0,1,1,1,1,1},{0,0,0,0,0,1,1},{0,1,1,1,1,1,1},{0,0,0,0,1,1,1},{0,0,0,0,0,0,1}
    }; //ide istim redosledom kao i all_scales, pa ne mora biti char. ako je 0 - skala se ne menja, ako je 1, menja se za +1



signals:
    void value_changed_signal(bool* pressed,int pot_value);

    void esp32_connected_signal();

    void major();
    void minor();

    void upis_uspesan();

private slots:
    void value_changed(bool* pressed,int pot_value);
    void readSerial();
    void pushButton_clicked();
    void show_can0_colordialog();
    void show_can1_colordialog();
    void show_can2_colordialog();
    void show_can3_colordialog();
    void show_can4_colordialog();
    void show_can5_colordialog();
    void show_can6_colordialog();
    void changecolor_C0(const QColor &color);
    void changecolor_C1(const QColor &color);
    void changecolor_C2(const QColor &color);
    void changecolor_C3(const QColor &color);
    void changecolor_C4(const QColor &color);
    void changecolor_C5(const QColor &color);
    void changecolor_C6(const QColor &color);
    void can0_colordialog_ok(const QColor &color);
    void can1_colordialog_ok(const QColor &color);
    void can2_colordialog_ok(const QColor &color);
    void can3_colordialog_ok(const QColor &color);
    void can4_colordialog_ok(const QColor &color);
    void can5_colordialog_ok(const QColor &color);
    void can6_colordialog_ok(const QColor &color);
    void can0_colordialog_cancel();
    void can1_colordialog_cancel();
    void can2_colordialog_cancel();
    void can3_colordialog_cancel();
    void can4_colordialog_cancel();
    void can5_colordialog_cancel();
    void can6_colordialog_cancel();
    void delay_updated (const QString &text);
    void red_onoff ();
    void green_onoff();
    void blue_onoff ();
    void rgb_onoff ();
    void redled_pic_clicked();
    void greenled_pic_clicked();
    void blueled_pic_clicked();
    void rgbled_pic_clicked();
    void red_jacina_change(int red_jacina_novo);
    void green_jacina_change(int green_jacina_novo);
    void blue_jacina_change(int blue_jacina_novo);
    void rgb_jacina_change(int rgb_jacina_novo);
    void checkifdisconnected();
    void popup_change_color_rgb();
    void rgb_colordialog_ok(const QColor& color);
    void rgb_colordialog_cancel();
    void linkLedToPot(const QString &text);
    void red_delay_updated(const QString &text);
    void green_delay_updated(const QString &text);
    void blue_delay_updated(const QString &text);
    void rgb_delay_updated(const QString &text);
    void light_rgb(const QColor &color);
    void iskljuci_C0();
    void iskljuci_C1();
    void iskljuci_C2();
    void iskljuci_C3();
    void iskljuci_C4();
    void iskljuci_C5();
    void iskljuci_C6();

    void can0_pressed();
    void can0_released();

    void can1_pressed();
    void can1_released();

    void can2_pressed();
    void can2_released();

    void can3_pressed();
    void can3_released();

    void can4_pressed();
    void can4_released();

    void can5_pressed();
    void can5_released();

    void can6_pressed();
    void can6_released();

    void izabranaNotaUPad_C0(const QString &note);
    void izabranaNotaUPad_C1(const QString &note);
    void izabranaNotaUPad_C2(const QString &note);
    void izabranaNotaUPad_C3(const QString &note);
    void izabranaNotaUPad_C4(const QString &note);
    void izabranaNotaUPad_C5(const QString &note);
    void izabranaNotaUPad_C6(const QString &note);
    void can0_sharp_pressed();
    void can1_sharp_pressed();
    void can2_sharp_pressed();
    void can3_sharp_pressed();
    void can4_sharp_pressed();
    void can5_sharp_pressed();
    void can6_sharp_pressed();
    void can0_flat_pressed();
    void can1_flat_pressed();
    void can2_flat_pressed();
    void can3_flat_pressed();
    void can4_flat_pressed();
    void can5_flat_pressed();
    void can6_flat_pressed();
    void emitMajorMinor(bool checked);
    void emitMinorMajor(bool checked);
    void setuj_tekst (int broj);
    void radioButton_sharp_clicked(bool);
    void radioButton_flat_clicked(bool);
    void input_on_off_stateChanged(int arg1);
    void output_on_off_stateChanged(int);

    void pokusajUpisatiOpet();
    void changeAllColors(const QString &text);
    void menjaj_skalu(const QString &arg1);

    void start_recording(bool pressed);
    void start_playing(bool pressed);
    void play_again();

    void start_test(bool pressed);
    void svirajNotuT();

public slots:


};




#endif // DIALOG_H
