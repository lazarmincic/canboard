#ifndef DIALOG_H
#define DIALOG_H

#define BAUD 250000
#define default_delay 1000
#define default_rgb Qt::red
#define default_can Qt::black
#define pot_margin 2
#define timer_ms 250
#define CANRED_IMG "C:/Users/Lazar/Desktop/can_red.png"
#define REDDIODE_IMG "C:/Users/Lazar/Desktop/all_red_diode.png"
#define CHECKING_RATE 400 //ms
#define MAX_DELAY_LENGTH 4
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define SAMPLE_LOC "C:/Users/Lazar/Desktop/canboard_files/"


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

    QColor C0_color = Qt::red;
    bool colordialog_ok_pressed = false;

    void write_rgb (const QColor &color,int delay= default_delay ,int jacina_rgb= -1);

    int delay_ms = default_delay;
    int red_delay_ms = default_delay;

    QPixmap changeImageColor(const QString &path,const QColor &color);
    void changeRgbLabelColor(const QColor& color);

    bool en_red = 1; //enable za led
    bool en_green = 1;
    bool en_blue = 1;
    bool en_rgb = 1;

    int jacina_red = 100;

    int doubleMap(double x, double in_min, double in_max, double out_min,double out_max);

    void pot_value_is_changing(int pot_value);

    QString esp32_port_name;

    void esp32_id_print();

    QTimer* check_if_connected;

    QColorDialog* rgb_dialog;

    QColor rgb_color = Qt::red;

    bool colordialog1_ok_pressed = false;

    led_enum pot_led = plava; //default

    void errorCheckDelay(const QString& text, QLineEdit *&qLineEdit_pointer, QLabel *&errorLabel_pointer, int& delay_);

    void can_pressedOrReleased(bool* pressed,QVector<int> c_changed);

    void changeVolumeSlider(int new_value);
    void can0_colordialog_declare();
    QColor to_darker_color(QColor oc,int times_darker);

    bool C0_on = true;

    void writeOnImage(QPixmap *pxmp, const QString &note_name, const QColor &font_color);
    QColor findTextColor(const QColor &color);

   /* quint32 seed_r;
    quint32 seed_g;
    quint32 seed_b;

    QRandomGenerator* rand_r;
    QRandomGenerator* rand_g;
    QRandomGenerator* rand_b;
*/

    void choose_note(QComboBox* c );

    note_mode sharp_or_flat = sharp;

    QString C0_nota = "C";

    void svirajNotuCB (QComboBox *c);


    void puni_p_meni_major();
    void puni_p_meni_minor();

    QSoundEffect* zvuk = new QSoundEffect ;

    void changecolor(const QColor &color, QString nota_str, QLabel* can_pixmap_p);
    void doWhenPressedOrReleased(bool c_pressed);

protected:

    const char note_names_sharp [12][5] = {"C","C♯","D","D♯","E","F","F♯","G","G♯","A","A♯","B"}; // imena nota sarp mod
    const char note_names_flat [12][5] = {"C","Db","D","Eb","E","F","Gb","G","Ab","A","Bb","B"}; // imena nota flat mod
    const QStringList sample_names = {"Grand Piano","Modern Upright","Maple Hill Funk","Voodoo Magic","Industrial Pad","Current Value H"};
    const char major_scale_names_sharp [8][11] = {"C Major","G Major","D Major","A Major","E Major","B Major","F♯ Major","C♯ Major"}; // imena skala sharp mod
    const char major_scale_names_flat [8][11] = {"C Major","F Major","Bb Major","Eb Major","Ab Major","Cb Major","Gb Major","Db Major"}; // imena skala flat mod
    const char minor_scale_names_sharp [8][11] = {"A Minor","E Minor","B Minor","F♯ Minor","C♯ Minor","G♯ Minor","D♯ Minor","A♯ Minor"}; //////////////
    const char minor_scale_names_flat [8][11] = {"A Minor", "D Minor","G Minor","C Minor", "F Minor","Ab Minor","Eb Minor","Bb Minor"};
    const char all_scales [30][8][9] = {{"A Minor","A","B","C","D","E","F","G"},{"D Minor","D","E","F","G","A","Bb","C"},{"G Minor","G","A","Bb","C","D","Eb","F"},         // 30 skala ukupno, 15 major 15 minor
        {"C Minor","C","D","Eb","F","G","Ab","Bb"},{"F Minor","F","G","Ab","Bb","C","Db","Eb"},{"Ab Minor","Ab","Bb","B","Db","Eb","E","Gb"},
        {"Eb Minor","Eb","F","Gb","Ab","Bb","B","Db"},{"Bb Minor","Bb","C","Db","Eb","F","Gb","Ab"},{"E Minor","E","F#","G","A","B","C","D"},
        {"B Minor","B","C#","D","E","F#","G","A"},{"F# Minor","F#","G#","A","B","C#","D","E"},{"C# Minor","C#","D#","E","F#","G#","A","B"},
        {"G# Minor","G#","A#","B","C#","D#","E","F#"},{"D# Minor","D#","F","F#","G#","A#","B","C#"},{"A# Minor","A#","C","C#","D#","F","F#","G#"},

        {"C Major","C","D","E","F","G","A","B"},{"F Major","F","G","A","Bb","C","D","E"},{"Bb Major","Bb","C","D","Eb","F","G","A"},
        {"Eb Major","Eb","F","G","Ab","Bb","C","D"},{"Ab Major","Ab","Bb","C","Db","Eb","F","G"},{"Cb Major","B","Db","Eb","E","Gb","Ab","Bb"},
        {"Gb Major","Gb","Ab","Bb","B","Db","Eb","F"},{"Db Major","Db","Eb","F","Gb","Ab","Bb","C"},    {"G Major","G","A","B","C","D","E","F#"},
        {"D Major","D","E","F#","G","A","B","C#"},{"A Major","A","B","C#","D","E","F#","G#"},{"E Major","E","F#","G#","A","B","C#","D#"},
        {"B Major","B","C#","D#","E","F#","G#","A#"},{"F# Major","F#","G#","A#","B","C#","D#","F"},{"C# Major","C#","D#","F","F#","G#","A#","C"}
    };

    const bool scale_helper [30][7] = {{0,0,1,1,1,1,1},{0,0,0,0,0,0,1},{0,0,0,1,1,1,1},{0,0,0,0,0,0,0},{0,0,0,0,1,1,1},{0,0,0,1,1,1,1},
        {0,0,0,0,0,0,1},{0,1,1,1,1,1,1},{0,0,0,0,0,1,1},{0,1,1,1,1,1,1},{0,0,0,0,1,1,1},{0,0,0,0,0,0,0},{0,0,0,1,1,1,1},{0,0,0,0,0,0,1},{0,1,1,1,1,1,1},

        {0,0,0,0,0,0,0},{0,0,0,0,1,1,1},{0,1,1,1,1,1,1},{0,0,0,0,0,1,1},{0,0,1,1,1,1,1},{0,1,1,1,1,1,1},{0,0,0,0,1,1,1},{0,0,0,0,0,0,1},{0,0,0,0,0,0,1},
        {0,0,0,0,0,1,1},{0,0,0,1,1,1,1},{0,0,1,1,1,1,1},{0,1,1,1,1,1,1},{0,0,0,0,1,1,1},{0,0,0,0,0,0,1}
    }; //ide istim redosledom kao i all_scales, pa ne mora biti char. ako je 0 - skala se ne menja, ako je 1, menja se za +1


signals:
    void value_changed_signal(bool* pressed,int pot_value);

    void esp32_connected_signal();

    void major();
    void minor();



private slots:
    void value_changed(bool* pressed,int pot_value);
    void readSerial();
    void pushButton_clicked();
    void pushButton_2_clicked();
    void changecolor_C0(const QColor &color);
    void colordialog_ok(const QColor &color);
    void colordialog_cancel();
    void delay_updated (const QString &text);
    void red_butt_clicked ();
    void redled_pic_clicked();
    void red_jacina_change(int red_jacina_novo);
    void checkifdisconnected();
    void popup_change_color_rgb();
    void colordialog1_ok(const QColor& color);
    void colordialog1_cancel();
    void linkLedToPot(const QString &text);
    void red_delay_updated(const QString &text);
    void light_rgb(const QColor &color);
    void iskljuci_C0();

    void C0SijaISvira();
    void C1SijaISvira();
    void C2SijaISvira();
    void C3SijaISvira();
    void C4SijaISvira();
    void C5SijaISvira();
    void C6SijaISvira();

    void izabranaNotaUPad(const QString &note);
    void can0_sharp_pressed();
    void can0_flat_pressed();
    void to_major(bool checked);
    void to_minor(bool checked);
    void setuj_tekst (int broj);
    void radioButton_sharp_clicked(bool);
    void radioButton_flat_clicked(bool);
    void input_on_off_stateChanged(int arg1);
    void output_on_off_stateChanged(int);

public slots:


};
#endif // DIALOG_H
