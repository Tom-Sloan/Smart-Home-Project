import processing.serial.*;
#include <stdio.h>
#include <unistd.h>

import java.io.*;

Serial mySerial;
PrintWriter output;
int count = 0;
int prev_count = 0;
String file_name;
void setup() {
   prev_count = count;
   mySerial = new Serial( this, Serial.list()[0], 115200 );
   Thread.sleep(1000);
   System.out.println("Recording");
   
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
      String file_name = "data_"+count+".txt";
      output = createWriter(file_name);
      
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
    
    if (key == ' '){ // Clicking space restarts the program
      count++;
      setup();
      System.out.println("Checking file");
      boolean file_status = check_file(file_name);
      Thread.sleep(3000);
      if (file_status){
        System.out.println("File is good/ has content");
      }
      else{
        System.out.println("Nothing written to file, exitting");
        exit();
      }
      
    }
    if (key == 'x'){ // Clicking x stops the program
      exit();  // Stops the program
    }
} 

boolean check_file(String file_name){
  try{
  BufferedReader br = new BufferedReader(new FileReader(file_name));
  if (br.readLine() != null){
    return true;
  }
  }
      catch (FileNotFoundException e){
        System.out.println(e);
      }
      catch (Exception e){
        System.out.println(e);
      }
  return false;


}
