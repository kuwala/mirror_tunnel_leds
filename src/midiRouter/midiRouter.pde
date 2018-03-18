import themidibus.*;

MidiBus myBus;
int pitch = 64;
int noteLength = 200;
int delayBetweenNotes = 100;
int xx = 0;
int yy = 0;

void setup() {
  MidiBus.list();
  // Patch midi in from op-1 
  // to midi out to teensy
  // use -1 for no input/output
  myBus = new MidiBus(this, 1, 3);
  // Change LED strip State
  myBus.sendNoteOff(0, 2, 127);
  
}
void draw() {
  background(0,0,0,0.01);
  fill(random(255));
  rect(xx*10,yy*10,10,10); 
  //randomMidi();
}

void randomMidi() {
  
  int channel = 0;
  pitch = pitch + int(random(10) - 5);
  int velocity = 127;
  
  myBus.sendNoteOn(channel, pitch, velocity);
  delay(noteLength);
  myBus.sendNoteOff(channel, pitch, velocity);
  int number = 0;
  int value = 90;
  
  //myBus.sendControllerChange(channel, number, value);
  print("sending random Midi ");
  println(pitch);
  delay(delayBetweenNotes);
}


void noteOn(int channel, int pitch, int velocity) {
  print("Routing note !!");
  xx = pitch %8;
  yy = (int)random(8);
  println(pitch);
  myBus.sendNoteOn(channel, pitch, velocity);
  
  //print("Note On -- ");
  //println("Pitch: "+pitch);
  
}
void noteOff(int channel, int pitch, int velocity) {
   print("Routing note !!");
  xx = pitch %8;
  yy = (int)random(8);
  println(pitch);
  myBus.sendNoteOff(channel, pitch, velocity);
  
}
void keyPressed() {
  if(key == '1') {
     noteOn(0,0, 127);
   noteOff(0,0, 127);
  } else if (key == '2' ) {
     noteOn(0,1, 127);
   noteOff(0,1, 127);
  } else if (key == '3' ) {
     noteOn(0,2, 127);
   noteOff(0,2, 127);
  }
  
  if(key == ' ') {
   int note = 4 +(int)random(250);
   noteOn(0,note, 127);
   //delay(5);
   noteOff(0,note, 127);
  }
  
}
void mousePressed() {
 

  int note = 62 +(int)random(12);
 noteOn(0,note, 127);
 delay(5);
 noteOff(0,note, 127);
 //randomMidi();
 // map mouse x to cc
 //int cc = map(mouseX, 0, 100, 0, 127);
 

}