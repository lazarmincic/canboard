

const int pins [] = {4,32,33,27,14,12,13}; // redosled pinova sa leva na desno
const int pot = 36; // pot na gpio36

int floatMap(float x, float in_min, float in_max, float out_min, float out_max) {
  return int((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min); }

const int tresh = 80; //eksperimentalno

#define LEDC_TIMER_12_BIT  12
#define LEDC_BASE_FREQ     5000

#define BAUD 250000

const int leds [6] = {26,25,23,22,21,19};

unsigned long when_delay_started [6]= {0,0,0,0,0,0};
int delay_ms [6] = {0,0,0,0,0,0};

const unsigned long kec = 1;

String led_s;

void ledcAnalogWrite(uint8_t channel, uint32_t value) 
{
  bool is_rgb = false;
  uint32_t valueMax = 100;
  if (channel<=5 && channel>=3) 
  {
     is_rgb = true;
     valueMax = 255; 
  }
  uint32_t duty = ((pow(2,LEDC_TIMER_12_BIT)-1) / valueMax) * min(value, valueMax); //  4095 od 2 ^ 12 - 1
  if (!is_rgb)
    ledcWrite(channel, duty);
  else
    ledcWrite(channel, ((pow(2,LEDC_TIMER_12_BIT)-1)-duty)); // obrnuta logika
}

TaskHandle_t serialwrite;
TaskHandle_t serialread;

void setup()
{
  Serial.begin(BAUD,SERIAL_8O2); // 8 bit, odd parity, 2 stop bita
  delay(2000);
  touchSetCycles(0x3500 ,0x3500); 
  for (int i=0;i<6;i++) // 0-5
  {
      ledcSetup(i, LEDC_BASE_FREQ, LEDC_TIMER_12_BIT);
      ledcAttachPin(leds[i], i); 
  }

  xTaskCreatePinnedToCore(
  serialwrite_f,"serialwrite",10000,NULL,1,&serialwrite,0);
  xTaskCreatePinnedToCore(
  serialread_f, "serialread",10000,NULL,1,&serialread,1);

}

void serialwrite_f(void * pvParameters ) {
  for(;;) {
      // pisanje

  String ispis;
  
  for (int i=0;i<7;i++) 
  {
    if (touchRead(pins[i])<tresh) // uslov -  dodirnut
      ispis.concat('1');
    else 
      ispis.concat('0');
      
    if (i==6) 
      ispis.concat('|');
    else
      ispis.concat(',');
  }
  
// format: ukljucen_prvi(0/1) ukljucen_drugi,...,ukljucen_sedmi|vrednost_pot(0-100)'\0'
// prva konzerva gubi kontakt?
  int pot_vr = floatMap(analogRead(pot), 0, (pow(2,LEDC_TIMER_12_BIT)-1), 0, 100.0); //vrednost od 0 do 100
  if (pot_vr <= 9) ispis.concat("00");
  else if (pot_vr <= 99) ispis.concat('0'); // format uvek trocifren
  ispis.concat(pot_vr);
   
  Serial.println(ispis);  // u qt

  // funkcija koja cita serijski port u qt treba da bude jednostavna

  }
}

void serialread_f( void * pvParameters ){
  for(;;)
  {
      // citanje

  // format: led , vreme , jacina  
  // vreme u ms, jacina 0-100, led 0-2
  // ako je nula, vreme nije bitno, ali se mora poslati

    if(Serial.available()) 
  {
    String vreme_s = Serial.readStringUntil('\0'); 
    vreme_s.concat('\0'); // jer readstring odstrani \0
   
    led_s = vreme_s;
    String jacina_s = vreme_s;
    
    led_s.remove(led_s.indexOf(",")-1); //ostace samo ono od pocetka - led +1 zbog space-a

    if (led_s!="0")
    {
       vreme_s.remove(vreme_s.indexOf(",",vreme_s.indexOf(",")+1)-1);
       vreme_s.remove(0,vreme_s.indexOf(",")+2); // ostaje od kraja - vreme
       delay_ms [led_s.toInt()-1] = vreme_s.toInt();
    }

    jacina_s.remove(0,jacina_s.indexOf(",",jacina_s.indexOf(",")+1)+2);
    int pwm_val = jacina_s.toInt();

       if (led_s == "0")
            for (int i=0;i<6;i++)
              ledcAnalogWrite(i,0);
       else
       {
        ledcAnalogWrite((led_s.toInt()-1),pwm_val);
        when_delay_started[led_s.toInt()-1] = millis();
       }
  }
    if (led_s != "0")
       for (int i=0;i<6;i++)
         {
          if ((millis() - when_delay_started[i]) >= (delay_ms[i]*kec))
             ledcAnalogWrite (i, 0); 
         }
  
  }
}


void loop()
{
}
