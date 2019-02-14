#include <Time.h> 
#include <SPI.h>   
#include <RFID.h> 
#include <LiquidCrystal.h>

#define TIME_HEADER  "T"   // Cabecera para el mensaje que pasa por Serial
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message 
#define SS_PIN 53 
#define RST_PIN 6 
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int led1 = 30;
int led2 = 31;
int numero_serie[5]; 
int usuario[3][5]={ {227,93,65,197,58},{182,48,0,73,207},{134,249,190,50,243} };   //E3 5D 41 C5 3A        B6 30 00 49 CF       86 F9 BE 32 F3       
int i=0;
int contador=1;
int contador2=1;
int contador3=1;
RFID rfid(SS_PIN,RST_PIN);
time_t TFinal, TInicial, TFinal2, TInicial2, TFinal3, TInicial3, TMal, TAforo;
unsigned long x,y,x2,y2,x3,y3;
int aforo = 0;
byte customChar[8] = { //é
    B00010,
    B00100,
    B01110,
    B10001,
    B11111,
    B10000,
    B01110,
    B00000
};
byte customChar2[8] = { //á
    B00010,
    B00100,
    B01100,
    B00010,
    B01110,
    B10010,
    B01110,
    B00000
};
byte customChar3[8] = { //í
    B00000,
    B00010,
    B00100,
    B00000,
    B00100,
    B00100,
    B00100,
    B00000
};

void setup()  {
  Serial.begin(9600);
  setSyncProvider (requestSync);  //set function to call when sync required
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(13, OUTPUT);
  SPI.begin(); 
  rfid.init();
  
}

void loop(){    
  if (Serial.available()) {
    processSyncMessage();
  }
  if (timeStatus()!= timeNotSet) {       //timeNotSet = La hora no ha sido programada
    digitalClockDisplay();               //Llama a función digitalClockDisplay
  }
  if (timeStatus() == timeSet) {         //timeSet = La hora está programada y sincronizada
    digitalWrite(13, HIGH); 
    Tarjetas();             
  } else {
    digitalWrite(13, LOW);               // LED off si se necesita actualizar 
  }
  delay(1000);
}

void digitalClockDisplay(){
  lcd.begin(16, 2);
  lcd.print(day());
  lcd.print("/");
  lcd.print(month());
  lcd.print("/");
  lcd.print(year());
  lcd.setCursor(0,1);
  if (hour() < 10){
  lcd.print("0");
  }
  lcd.print(hour());
  lcd.print(":");
  if (minute() < 10){
  lcd.print("0");
  }
  lcd.print(minute());
  lcd.print(":");
  if (second() < 10){
  lcd.print("0");
  }
  lcd.print(second());
  lcd.setCursor(0,0);
}
  
void Tarjetas(){ 
 if(rfid.isCard())
{ 
  if(rfid.readCardSerial())
    { 
     for(i=0; i<=4 ; i++)
     {
      numero_serie[i] = rfid.serNum[i];
     }
     for(i=0; i<=4 ; i++)
                {
                  Serial.print(numero_serie[i],HEX);
                  Serial.print(" ");
                }
     Serial.println(""); 
     delay (500); 
     //
    } 
    
   if (((( rfid.serNum[0] ) == 227)) && ((( rfid.serNum[1] ) == 93)) && ((( rfid.serNum[2] ) == 65)) && ((( rfid.serNum[3] ) == 197)) && ((( rfid.serNum[4] ) == 58)))
   { 
    if (contador %2 !=0 && aforo<2) {
       digitalWrite(led1, HIGH);
       delay (1000);
       digitalWrite(led1, LOW);
       lcd.print("Bienvenido.");
       lcd.setCursor(0,1);
       lcd.print("Su ID ha sido comprobado y puede pasar");
       delay(1000);
             for (int positionCounter = 0; positionCounter < 22; positionCounter++) {
                  lcd.scrollDisplayLeft();
                  delay(250);
                  }
       delay(300);
       lcd.clear();
       lcd.setCursor(0,0);
       lcd.print("Ha accedido a la");
       lcd.setCursor(0,1);
       lcd.print("siguiente hora:");
       delay(2000);
       time_t TInicial = now();
       Serial.print("Ha entrado el ");
       Serial.print(day(TInicial));
       Serial.print("/");
       Serial.print(month(TInicial));
       Serial.print("/");
       Serial.print(year(TInicial));
       Serial.print(" a las ");
       Serial.print(hour(TInicial));
       Serial.print(":");
             if (minute(TInicial) < 10){
                 Serial.print("0");
                 }
       Serial.print(minute(TInicial));
       Serial.print(":");
             if (second(TInicial) < 10){
                 Serial.print("0");
                 }
       Serial.print(second(TInicial));
       lcd.clear();
       lcd.setCursor(0,0);
       lcd.print(day(TInicial));
       lcd.print("/");
       lcd.print(month(TInicial));
       lcd.print("/");
       lcd.print(year(TInicial));
       lcd.setCursor(0,1);
       lcd.print(hour(TInicial));
       lcd.print(":");
             if (minute(TInicial) < 10){
                 lcd.print("0");
                 }
       lcd.print(minute(TInicial));
       lcd.print(":");
             if (second(TInicial) < 10){
                 lcd.print("0");
                 }
       lcd.print(second(TInicial));
       delay(2000);
       x=TInicial;
       contador++;
       aforo++;
       }

    else if (contador %2 != 0 && aforo>=2){ 
        lcd.print("Lo sentimos.");
        lcd.setCursor(0,1);
        lcd.print("No puede acceder en estos momentos");
        delay(1000);
              for (int positionCounter = 0; positionCounter < 18; positionCounter++) {
                   lcd.scrollDisplayLeft();
                   delay(250);
                   }
        delay(300);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Int"); 
        lcd.createChar(0, customChar);
        lcd.setCursor(3,0); 
        lcd.write((byte)0); 
        lcd.setCursor(4,0);
        lcd.print("ntelo");
        lcd.setCursor(0,1);
        lcd.print("m");
        lcd.createChar(1,customChar2);
        lcd.setCursor(1,1);
        lcd.write((byte)1);
        lcd.setCursor(2,1);
        lcd.print("s tarde");
        delay(2000);
        time_t TAforo = now();
        Serial.print(day(TAforo));
        Serial.print("/");
        Serial.print(month(TAforo));
        Serial.print("/");
        Serial.print(year(TAforo));
        Serial.print(" a las ");
        Serial.print(hour(TAforo));
        Serial.print(":");
              if (minute(TAforo) < 10){
                  Serial.print("0");
                  }
        Serial.print(minute(TAforo));
        Serial.print(":");
              if (second(TAforo) < 10){
                  Serial.print("0");
                  }
        Serial.print(second(TAforo));
        Serial.print(" pero el aforo estaba lleno");
        }
                                         
    else if (contador %2 == 0) {
        digitalWrite(led1, HIGH);
        delay (1000);
        digitalWrite(led1, LOW);
        lcd.print("Su ID ha sido");
        lcd.setCursor(0,1);
        lcd.print("comprobado y puede salir");
        delay(1000);
              for (int positionCounter = 0; positionCounter < 8; positionCounter++) {
                  lcd.scrollDisplayLeft();
                  delay(250);
                  }
        delay(300);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Ha salido a la");
        lcd.setCursor(0,1);
        lcd.print("siguiente hora:");
        delay(2000);
        time_t TFinal = now();
        Serial.print("Ha salido el ");
        Serial.print(day(TFinal));
        Serial.print("/");
        Serial.print(month(TFinal));
        Serial.print("/");
        Serial.print(year(TFinal));
        Serial.print(" a las ");
        Serial.print(hour(TFinal));
        Serial.print(":");
               if (minute(TFinal) < 10){
                   Serial.print("0");
                   }
        Serial.print(minute(TFinal));
        Serial.print(":");
               if (second(TFinal) < 10){
                  Serial.print("0");
                  }
        Serial.print(second(TFinal));
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(day(TFinal));
        lcd.print("/");
        lcd.print(month(TFinal));
        lcd.print("/");
        lcd.print(year(TFinal));
        lcd.setCursor(0,1);
        lcd.print(hour(TFinal));
        lcd.print(":");
              if (minute(TFinal) < 10){
                  lcd.print("0");
                  }
        lcd.print(minute(TFinal));
        lcd.print(":");
              if (second(TFinal) < 10){
                  lcd.print("0");
                  }
        lcd.print(second(TFinal));
        delay(2000);
        y=TFinal;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Ha permanecido");
        lcd.setCursor(0,1);
        lcd.print("dentro del recinto un total de:");
        delay(1000);
              for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
                  lcd.scrollDisplayLeft();
                  delay(250);
                  }
        delay(300);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(String(day(y-x)-1)+ " d");
        lcd.createChar(2, customChar3); 
        lcd.setCursor(3,0);
        lcd.write((byte)2); 
        lcd.setCursor(4,0);
        lcd.print("a/s,");
        lcd.print(String(hour(y-x))+ " hora/s, ");
        lcd.setCursor(0,1);
        lcd.print(String(minute(y-x))+ " minuto/s y ");
        lcd.print(String(second(y-x))+ " segundo/s");
        delay(2000);
              for (int positionCounter = 0; positionCounter < 10; positionCounter++) {
                  lcd.scrollDisplayLeft();
                  delay(300);
                  }
        contador--;
        aforo--;
         }
        }
   
   else if (((( rfid.serNum[0] ) == 182)) && ((( rfid.serNum[1] ) == 48)) && ((( rfid.serNum[2] ) == 0)) && ((( rfid.serNum[3] ) == 73)) && ((( rfid.serNum[4] ) == 207)))
   { 
    if (contador2 %2 !=0 && aforo <2){
        digitalWrite(led1, HIGH);
        delay (1000);
        digitalWrite(led1, LOW);
        lcd.print("Bienvenido.");
        lcd.setCursor(0,1);
        lcd.print("Su ID ha sido comprobado y puede pasar");
        delay(1000);
              for (int positionCounter = 0; positionCounter < 22; positionCounter++) {
                   lcd.scrollDisplayLeft();
                   delay(250);
                   }
        delay(300);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Ha accedido a la");
        lcd.setCursor(0,1);
        lcd.print("siguiente hora:");
        delay(2000);
        time_t TInicial2 = now();
        Serial.print("Ha entrado el ");
        Serial.print(day(TInicial2));
        Serial.print("/");
        Serial.print(month(TInicial2));
        Serial.print("/");
        Serial.print(year(TInicial2));
        Serial.print(" a las ");
        Serial.print(hour(TInicial2));
        Serial.print(":");
               if (minute(TInicial2) < 10){
                   Serial.print("0");
                   }
        Serial.print(minute(TInicial2));
        Serial.print(":");
               if (second(TInicial2) < 10){
                   Serial.print("0");
                   }
        Serial.print(second(TInicial2));
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(day(TInicial2));
        lcd.print("/");
        lcd.print(month(TInicial2));
        lcd.print("/");
        lcd.print(year(TInicial2));
        lcd.setCursor(0,1);
        lcd.print(hour(TInicial2));
        lcd.print(":");
              if (minute(TInicial2) < 10){
                  lcd.print("0");
                  }
        lcd.print(minute(TInicial2));
        lcd.print(":");
              if (second(TInicial2) < 10){
                  lcd.print("0");
                  }
        lcd.print(second(TInicial2));
        delay(2000);
        x2=TInicial2;
        contador2++;
        aforo++;
        }

    else if (contador2 %2 != 0 && aforo>=2){
        lcd.print("Lo sentimos.");
        lcd.setCursor(0,1);
        lcd.print("No puede acceder en estos momentos");
        delay(1000);
              for (int positionCounter = 0; positionCounter < 18; positionCounter++) {
                   lcd.scrollDisplayLeft();
                   delay(250);
                   }
        delay(300);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Int"); 
        lcd.createChar(0, customChar);
        lcd.setCursor(3,0); 
        lcd.write((byte)0); 
        lcd.setCursor(4,0);
        lcd.print("ntelo");
        lcd.setCursor(0,1);
        lcd.print("m");
        lcd.createChar(1,customChar2);
        lcd.setCursor(1,1);
        lcd.write((byte)1);
        lcd.setCursor(2,1);
        lcd.print("s tarde");
        delay(2000);
        time_t TAforo = now();
        Serial.print(day(TAforo));
        Serial.print("/");
        Serial.print(month(TAforo));
        Serial.print("/");
        Serial.print(year(TAforo));
        Serial.print(" a las ");
        Serial.print(hour(TAforo));
        Serial.print(":");
              if (minute(TAforo) < 10){
                  Serial.print("0");
                  }
        Serial.print(minute(TAforo));
        Serial.print(":");
              if (second(TAforo) < 10){
                  Serial.print("0");
                  }
        Serial.print(second(TAforo));
        Serial.print(" pero el aforo estaba lleno");
        }
                       
    else if (contador2 %2 == 0){
        digitalWrite(led1, HIGH);
        delay (1000);
        digitalWrite(led1, LOW);
        lcd.print("Su ID ha sido");
        lcd.setCursor(0,1);
        lcd.print("comprobado y puede salir");
        delay(1000);
              for (int positionCounter = 0; positionCounter < 8; positionCounter++) {
                  lcd.scrollDisplayLeft();
                  delay(250);
                  }
        delay(300);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Ha salido a la");
        lcd.setCursor(0,1);
        lcd.print("siguiente hora:");
        delay(2000);
        time_t TFinal2 = now();
        Serial.print("Ha salido el ");
        Serial.print(day(TFinal2));
        Serial.print("/");
        Serial.print(month(TFinal2));
        Serial.print("/");
        Serial.print(year(TFinal2));
        Serial.print(" a las ");
        Serial.print(hour(TFinal2));
        Serial.print(":");
               if (minute(TFinal2) < 10){
                   Serial.print("0");
                   }
        Serial.print(minute(TFinal2));
        Serial.print(":");
               if (second(TFinal2) < 10){
                   Serial.print("0");
                   }
        Serial.print(second(TFinal2));
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(day(TFinal2));
        lcd.print("/");
        lcd.print(month(TFinal2));
        lcd.print("/");
        lcd.print(year(TFinal2));
        lcd.setCursor(0,1);
        lcd.print(hour(TFinal2));
        lcd.print(":");
              if (minute(TFinal2) < 10){
                  lcd.print("0");
                  }
        lcd.print(minute(TFinal2));
        lcd.print(":");
              if (second(TFinal2) < 10){
                  lcd.print("0");
                  }
        lcd.print(second(TFinal2));
        delay(2000);
        y2=TFinal2;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Ha permanecido");
        lcd.setCursor(0,1);
        lcd.print("dentro del recinto un total de:");
        delay(1000);
              for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
                  lcd.scrollDisplayLeft();
                  delay(250);
                  }
        delay(300);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(String(day(y2-x2)-1)+ " d");
        lcd.createChar(2, customChar3); 
        lcd.setCursor(3,0);
        lcd.write((byte)2); 
        lcd.setCursor(4,0);
        lcd.print("a/s,");
        lcd.print(String(hour(y2-x2))+ " hora/s, ");
        lcd.setCursor(0,1);
        lcd.print(String(minute(y2-x2))+ " minuto/s y ");
        lcd.print(String(second(y2-x2))+ " segundo/s");
        delay(2000);
              for (int positionCounter = 0; positionCounter < 10; positionCounter++) {
                  lcd.scrollDisplayLeft();
                  delay(300);
                  }
        contador2--;
        aforo--;
            }
         }

   else if (((( rfid.serNum[0] ) == 134)) && ((( rfid.serNum[1] ) == 249)) && ((( rfid.serNum[2] ) == 190)) && ((( rfid.serNum[3] ) == 50)) && ((( rfid.serNum[4] ) == 243)))
   { 
    if (contador3 %2 !=0 && aforo<2) {
       digitalWrite(led1, HIGH);
       delay (1000);
       digitalWrite(led1, LOW);
       lcd.print("Bienvenido.");
       lcd.setCursor(0,1);
       lcd.print("Su ID ha sido comprobado y puede pasar");
       delay(1000);
             for (int positionCounter = 0; positionCounter < 22; positionCounter++) {
                  lcd.scrollDisplayLeft();
                  delay(250);
                  }
       delay(300);
       lcd.clear();
       lcd.setCursor(0,0);
       lcd.print("Ha accedido a la");
       lcd.setCursor(0,1);
       lcd.print("siguiente hora:");
       delay(2000);
       time_t TInicial3 = now();
       Serial.print("Ha entrado el ");
       Serial.print(day(TInicial3));
       Serial.print("/");
       Serial.print(month(TInicial3));
       Serial.print("/");
       Serial.print(year(TInicial3));
       Serial.print(" a las ");
       Serial.print(hour(TInicial3));
       Serial.print(":");
              if (minute(TInicial3) < 10){
                  Serial.print("0");
                  }
       Serial.print(minute(TInicial3));
       Serial.print(":");
              if (second(TInicial3) < 10){
                  Serial.print("0");
                  }
       Serial.print(second(TInicial3));
       lcd.clear();
       lcd.setCursor(0,0);
       lcd.print(day(TInicial3));
       lcd.print("/");
       lcd.print(month(TInicial3));
       lcd.print("/");
       lcd.print(year(TInicial3));
       lcd.setCursor(0,1);
       lcd.print(hour(TInicial3));
       lcd.print(":");
             if (minute(TInicial3) < 10){
                 lcd.print("0");
                 }
       lcd.print(minute(TInicial3));
       lcd.print(":");
             if (second(TInicial3) < 10){
                 lcd.print("0");
                 }
       lcd.print(second(TInicial3));
       delay(2000);
       x3=TInicial3;
       contador3++;
       aforo++;
       }

    else if (contador3 %2 != 0 && aforo>=2){ 
        lcd.print("Lo sentimos.");
        lcd.setCursor(0,1);
        lcd.print("No puede acceder en estos momentos");
        delay(1000);
              for (int positionCounter = 0; positionCounter < 18; positionCounter++) {
                   lcd.scrollDisplayLeft();
                   delay(250);
                   }
        delay(300);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Int"); 
        lcd.createChar(0, customChar);
        lcd.setCursor(3,0); 
        lcd.write((byte)0); 
        lcd.setCursor(4,0);
        lcd.print("ntelo");
        lcd.setCursor(0,1);
        lcd.print("m");
        lcd.createChar(1,customChar2);
        lcd.setCursor(1,1);
        lcd.write((byte)1);
        lcd.setCursor(2,1);
        lcd.print("s tarde");
        delay(2000);
        time_t TAforo = now();
        Serial.print(day(TAforo));
        Serial.print("/");
        Serial.print(month(TAforo));
        Serial.print("/");
        Serial.print(year(TAforo));
        Serial.print(" a las ");
        Serial.print(hour(TAforo));
        Serial.print(":");
              if (minute(TAforo) < 10){
                  Serial.print("0");
                  }
        Serial.print(minute(TAforo));
        Serial.print(":");
              if (second(TAforo) < 10){
                  Serial.print("0");
                  }
        Serial.print(second(TAforo));
        Serial.print(" pero el aforo estaba lleno");
         }
                                         
    else if (contador3 %2 == 0) {
        digitalWrite(led1, HIGH);
        delay (1000);
        digitalWrite(led1, LOW);
        lcd.print("Su ID ha sido");
        lcd.setCursor(0,1);
        lcd.print("comprobado y puede salir");
        delay(1000);
              for (int positionCounter = 0; positionCounter < 8; positionCounter++) {
                  lcd.scrollDisplayLeft();
                  delay(250);
                  }
        delay(300);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Ha salido a la");
        lcd.setCursor(0,1);
        lcd.print("siguiente hora:");
        delay(2000);
        time_t TFinal3 = now();
        Serial.print("Ha salido el ");
        Serial.print(day(TFinal3));
        Serial.print("/");
        Serial.print(month(TFinal3));
        Serial.print("/");
        Serial.print(year(TFinal3));
        Serial.print(" a las ");
        Serial.print(hour(TFinal3));
        Serial.print(":");
               if (minute(TFinal3) < 10){
                   Serial.print("0");
                   }
        Serial.print(minute(TFinal3));
        Serial.print(":");
               if (second(TFinal3) < 10){
                   Serial.print("0");
                   }
        Serial.print(second(TFinal3));
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(day(TFinal3));
        lcd.print("/");
        lcd.print(month(TFinal3));
        lcd.print("/");
        lcd.print(year(TFinal3));
        lcd.setCursor(0,1);
        lcd.print(hour(TFinal3));
        lcd.print(":");
              if (minute(TFinal3) < 10){
                  lcd.print("0");
                  }
        lcd.print(minute(TFinal3));
        lcd.print(":");
              if (second(TFinal3) < 10){
                  lcd.print("0");
                  }
        lcd.print(second(TFinal3));
        delay(2000);
        y3=TFinal3;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Ha permanecido");
        lcd.setCursor(0,1);
        lcd.print("dentro del recinto un total de:");
        delay(1000);
              for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
                  lcd.scrollDisplayLeft();
                  delay(250);
                  }
        delay(300);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(String(day(y3-x3)-1)+ " d");
        lcd.createChar(2, customChar3); 
        lcd.setCursor(3,0);
        lcd.write((byte)2); 
        lcd.setCursor(4,0);
        lcd.print("a/s,");
        lcd.print(String(hour(y3-x3))+ " hora/s, ");
        lcd.setCursor(0,1);
        lcd.print(String(minute(y3-x3))+ " minuto/s y ");
        lcd.print(String(second(y3-x3))+ " segundo/s");
        delay(2000);
              for (int positionCounter = 0; positionCounter < 10; positionCounter++) {
                  lcd.scrollDisplayLeft();
                  delay(300);
                  }
        contador3--;
        aforo--;
         }
        }
   
    else {
        digitalWrite(led2, HIGH);
        delay (1000);
        digitalWrite(led2, LOW);
        lcd.print("Lo sentimos. Su");
        lcd.setCursor(0,1);
        lcd.print("ID no est");
        lcd.createChar(1,customChar2);
        lcd.setCursor(9,1);
        lcd.write((byte)1);
        lcd.setCursor(10,1);
        delay(1000);
        lcd.print(" registrado y no puede pasar");
              for (int positionCounter = 0; positionCounter < 23; positionCounter++) {
                   lcd.scrollDisplayLeft();
                   delay(250);
                   }
        delay(200);
        time_t TMal = now();
        Serial.print("El serial de usuario no se encuentra registrado. Ha intentado acceder el ");
        Serial.print(day(TMal));
        Serial.print("/");
        Serial.print(month(TMal));
        Serial.print("/");
        Serial.print(year(TMal));
        Serial.print(" a las ");
        Serial.print(hour(TMal));
        Serial.print(":");
               if (minute(TMal) < 10){
                   Serial.print("0");
                   }
        Serial.print(minute(TMal));
        Serial.print(":");
               if (second(TMal) < 10){
                   Serial.print("0");
                   }
        Serial.print(second(TMal));
        } 
   Serial.println("");
  }
rfid.halt();
}

void processSyncMessage() {
  unsigned long pctime;
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013

  if(Serial.find(TIME_HEADER)) {
     pctime = Serial.parseInt();
     if( pctime >= DEFAULT_TIME) { // check the integer is a valid time (greater than Jan 1 2013)
       setTime(pctime); // Sync Arduino clock to the time received on the serial port
     }
  }
}

time_t requestSync()
{
  Serial.write(TIME_REQUEST);  
  return 0; // the time will be sent later in response to serial mesg
}
