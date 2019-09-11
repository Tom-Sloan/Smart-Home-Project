import processing.serial.*;


void setup(){
 System.out.println("In setup"); 
}  

void draw() {
  //System.out.println("In draw");
    rect(25, 25, 50, 50);
}

void keyPressed() {
    System.out.println(key);
    if (key == ' '){
      setup();
    }
    if (key == 'x'){
      System.out.println("Exitting");
      exit();  // Stops the program
    }
} 
