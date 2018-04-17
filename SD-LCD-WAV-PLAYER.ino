#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SD.h>
#include <SPI.h>
#include <TMRpcm.h>

TMRpcm tmrpcm;   // create an object for use in this sketch

unsigned long time = 0;

const byte playPin = 9;
const byte fwdPin = 10;
const byte backPin = 8;
String filenames[10];
int count = 0;
File root;
int pos =0;
#define I2C_ADDR 0x3F

LiquidCrystal_I2C lcd(I2C_ADDR, 2, 1, 0, 4, 5, 6, 7);

int led = 13;

void setup () {
  tmrpcm.speakerPin = 46;
  pinMode(playPin, INPUT);
  pinMode(fwdPin, INPUT);
  pinMode(backPin, INPUT);
  lcd.begin(16, 2);//16 columns 2 rows for 1602A

  lcd.setBacklightPin(3, POSITIVE);
  //lcd.setBacklight(HIGH); // NOTE: You can turn the backlight off by setting it to LOW instead of HIGH
  lcd.setBacklight(HIGH);
  lcd.clear(); // clean screen and sets cursor to (0,0)
  // initialize the digital pin as an output.
  // will not blink without it
  pinMode(led, OUTPUT);
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
    Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  root = SD.open("/");

  printDirectory(root, 0);

  Serial.println("done!");
  
}



void loop() {
  if(digitalRead(playPin) == HIGH){
    lcd.clear();
      lcd.setCursor(0, 0);
      //lcd.print(filenames[1]);
      String lcdpr = "Play:";
      lcdpr = lcdpr + filenames[pos];
      lcd.print(lcdpr);
      delay(500);
      //tmrpcm.setVolume(0);
      tmrpcm.quality(1);
      String playfile = filenames[pos];
      Serial.println(playfile);
      char pfile[12];
      playfile.toCharArray(pfile, playfile.length() +1);
      tmrpcm.play(pfile);
  }
  if(digitalRead(fwdPin) == HIGH){
    if (pos < count -1){
      pos = pos + 1;
    } else {
      pos = 0;
    }
      Serial.begin(9600);
      Serial.println(filenames[pos]);
      lcd.clear();
      lcd.setCursor(0, 0);
      String lcdpr = (String)pos + ":";
      lcdpr = lcdpr + filenames[pos];
      lcd.print(lcdpr);
      delay(500);
    
  }
    if(digitalRead(backPin) == HIGH){
    if (pos > 0){
      pos = pos - 1;
    } else {
      pos = count -1;
    }
      Serial.begin(9600);
      Serial.println(filenames[pos]);
      lcd.clear();
      lcd.setCursor(0, 0);
      String lcdpr = (String)pos + ":";
      lcdpr = lcdpr + filenames[pos];
      lcd.print(lcdpr);
      delay(500);
  }
    //blink the LED manually to demonstrate music playback is independant of main loop
  if(tmrpcm.isPlaying() && millis() - time > 50 ) {      
      digitalWrite(13,!digitalRead(13));
      time = millis();    
  }else
  if(millis() - time > 500){     
    digitalWrite(13,!digitalRead(13)); 
    time = millis(); 
  }


  if(Serial.available()){    
    switch(Serial.read()){
    case 'd': tmrpcm.play("music"); break;
    case 'P': tmrpcm.play("temple"); break;
    case 't': tmrpcm.play("catfish"); break;
    case 'p': tmrpcm.pause(); break;
    case '?': if(tmrpcm.isPlaying()){ Serial.println("A wav file is being played");} break;
    case 'S': tmrpcm.stopPlayback(); break;
    case '=': tmrpcm.volume(1); break;
    case '-': tmrpcm.volume(0); break;
    case '0': tmrpcm.quality(0); break;
    case '1': tmrpcm.quality(1); break;
    default: break;
    }
  }
}
void printDirectory(File dir, int numTabs) {
  
   
    while (true) {
  
      File entry =  dir.openNextFile();
      if (! entry) {
        // no more files
        break;
      }
      //check if its a wav file
      if (isWav(entry.name())){
      //get the filename in the form of a string
      String strName = getName(entry.name());
      //add the filename to the filename array. 
      filenames[count] = strName;
      count = count + 1;
      delay(1000);
      lcd.setBacklight(HIGH);
      }
      entry.close();
    }
    //just to test the array worked
    for (int i = 0; i < count; i++){
      
      Serial.println(filenames[i]);
    }
      lcd.setCursor(0, 0);
      String lcdpr = (String)pos + ":";
      lcdpr = lcdpr + filenames[pos];
      lcd.print(lcdpr);
  }
  bool isWav(char* filename){
    int8_t len = strlen(filename);
    bool result;
    if ( strstr(strlwr(filename + (len - 4)), ".wav")){
      result = true;
    } else {
      result = false;
    } return result;
  }
  String getName(char* filename){
    String strName = (String) filename;
    return strName;
  }

