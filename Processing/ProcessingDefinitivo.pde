import processing.serial.*;
import de.bezier.data.sql.*;

Serial myPort;
String val;
MySQL db;
int end = 10;
int b = 0;
int contador = 0;
String serial, fecha;

void setup()
{
  // On Windows machines, this generally opens COM1.
  printArray(Serial.list()[0]);
  myPort = new Serial(this, Serial.list()[0], 9600);
  
  String user     = "root";
  String pass     = "";
  String database = "bdpython";
  db = new MySQL(this, "127.0.0.1", database, user, pass);
}

void draw()
{  
   if  (myPort.available() > 0 && b == 0){
        myPort.write("T1496245107");
        b++;
        }
        
   val = myPort.readStringUntil('\n');  
   
   if (val != null){
     
         val = trim (val);
         
         if (val.contains("las")){
             fecha = val;
             }
          else {
             serial = val;
          }

          if (contador %2 !=0){
             db.connect();
             db.execute("INSERT INTO example (data, hora) VALUES ('%s', '%s')", serial,fecha);
             }
         contador++;
         }  
       } 
