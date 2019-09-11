import processing.serial.*;
#include <stdio.h>
#include <unistd.h>


Serial mySerial;
PrintWriter output;
int count = 0;
void setup() {
   mySerial = new Serial( this, Serial.list()[0], 115200 );
   sleep(1)
   
}
void draw() {
    if (count == prev_count){
      if (mySerial.available() > 0 ) {
         String value = mySerial.readString();
         if ( value != null ) {
              output.println( value );
         }
      }
    }
    else{
      
      output = createWriter( "data_"+count+".txt" );
      
      if (mySerial.available() > 0 ) {
           String value = mySerial.readString();
           if ( value != null ) {
                output.println( value );
           }
      }
    }
}

void keyPressed() {
    output.flush();  // Writes the remaining data to the file
    output.close();  // Finishes the file
    count++;
    if (key == ' '){
      setup();
    }
    if (key == 'x'){
      exit();  // Stops the program
    }
} 
