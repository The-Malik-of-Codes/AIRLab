// include the library code:
#include <LiquidCrystal.h>
#include <Adafruit_Fingerprint.h>
#include <Keypad.h>
#include <List.hpp>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(A5, A4, A3, A2, A1, A0);

byte smiley[8] = {
  0b00000,
  0b00000,
  0b01010,
  0b00000,
  0b00000,
  0b10001,
  0b01110,
  0b00000
};

#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)
// For UNO and others without hardware serial, we must use software serial...
// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
// Set up the serial port to use softwareserial..
SoftwareSerial mySerial(2, 3);

#else
// On Leonardo/M0/etc, others with hardware serial, use hardware serial!
// #0 is green wire, #1 is white
#define mySerial Serial1

#endif

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

int redLed = 13;
int greenLed = 0;
//bool IsAttendanceInProgress = false;

//initially we assume an attendance just ended(true), and a new attendance hasn't started(false)
bool IsAttendanceStarted = false;
bool IsAttendanceEnded = true;
String result;
//from red to yellow
//pin 13 - 6

byte rowPins[ROWS] = {4, 5, 6, 7}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {8, 9, 11, 12}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

String previousLCDTitle = "";
String previousLCDmsg = "";

List<String> attendance(true);
byte NumOfRegisteredStudents = 120;
byte fingerID[120] = {1,2,11,12,13,14,15}; 
String studentID[120] = {"160407038","190407055", "221028391", "220401031", "190404094", "20EG01009", "170404085"}; 
//String studentName[3] = {"Adewole Benard","Ahmad-Mosaku","Abdul Azzez"}; 
void displayTextLCD(String title,String msg, bool IsScrollable,int ScrollSpeed = 300, int timeout = 0)
{
  if(previousLCDTitle == title && previousLCDmsg == msg)
  {
    return;
    //To avoid printing same message to the LCD Multiple times 
  }
  
  lcd.clear();
  
  
  lcd.setCursor(0, 0);
  lcd.print(title);
  lcd.setCursor(0, 1);
  lcd.print(msg);

  //Serial.println(title);
  //Serial.println(msg);
  //Serial.println();
  previousLCDTitle = title;
  previousLCDmsg = msg;
  //
  if(IsScrollable)
  {
    for(byte i = 0; i < 20; ++i){
    delay(IsScrollable);
    lcd.scrollDisplayLeft();
    }
  }
  delay(timeout);
  
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  // found a match!
  //Serial.print("Found ID #"); Serial.print(finger.fingerID);
  //Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID;
}

bool attendaceTaken(String matric)
{
  for(byte i = 0; i < attendance.getSize(); i++)
  {
    if(attendance.getValue(i) == matric)
    {
      return true;
      }
  }
return false;
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      //Serial.println("No finger detected");
      //Serial.println(attendance.getSize());

      result = "Attendance: " + String(attendance.getSize());
      displayTextLCD(result,"In-Progress", false);
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      //Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      //Serial.println("Imaging error");
      return p;
    default:
      //Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      //Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      //Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      //Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      //Serial.println("Could not find fingerprint features");
      return p;
    default:
      //Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    //Serial.println("Found a print match!");
    
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    //Serial.println("Communication error");
    digitalWrite(redLed,HIGH);
    displayTextLCD("Failed","Try Again", false,300, 500);
    digitalWrite(redLed,LOW);
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    //Serial.println("Did not find a match");
    digitalWrite(redLed,HIGH);
    displayTextLCD("Failed","Not Registered", false,300, 1000);
    digitalWrite(redLed,LOW);
    return p;
  } else {
    //Serial.println("Unknown error");
    displayTextLCD("Remove Finger","------ ------", false,300, 500);
    return p;
  }
  
  // found a match!
  digitalWrite(greenLed,HIGH);
  
  
  //Serial.print("Found ID #"); Serial.print(finger.fingerID);
  //Serial.print(" with confidence of "); Serial.println(finger.confidence);
  for (byte i = 0; i < NumOfRegisteredStudents; i++)
  {
    if(finger.fingerID == fingerID[i])
    {
        //Serial.println(studentID[i]);
        displayTextLCD("User Verified",studentID[i], false,300, 1000);

        //if attendace hasn't been recorded, record it
        if(!attendaceTaken(studentID[i]))
        {
          attendance.add(studentID[i]);
          }
        
        
    }
  }
  
  digitalWrite(greenLed,LOW);
  return finger.fingerID; 
}



String keyPass = "";

void enterKeyPass()
{
  char key = keypad.getKey();
    // just print the pressed key
   // this checkes if A is pressed, validate keyPass.
   //Disables the following buttons from concatenation
  if (key =='C' || key == '#'){
    return;
  }
  
  if (key =='A'){
    beep();
    if(keyPass == "1234")
    {
      //if attendace didn't just end earlier, and attendance hasn't started, start a new attendance with Accept button
      if(IsAttendanceEnded && !IsAttendanceStarted)
      {
        //Serial.println("Accepted");
      digitalWrite(greenLed,HIGH);
      displayTextLCD("Valid Pass","Please Begin", false, 300, 3000);
      digitalWrite(greenLed,LOW);
        IsAttendanceStarted = true;
        IsAttendanceEnded = false;
        return;
        }
        
      if(IsAttendanceEnded && IsAttendanceStarted)
      {
        //Serial.println("Saved");
      keyPass = "";
      digitalWrite(greenLed,HIGH);
      result = "Total Att.: " + String(attendance.getSize());
      displayTextLCD("Valid Pass",result, false, 300, 3000);
    displayTextLCD("Signing Out","---- ---- ----", false, 300, 3000);
    displayTextLCD("New Attendance","Getting Ready", false, 300, 1000);
    displayTextLCD("New Attendance","Getting Ready.", false, 300, 1000);
    displayTextLCD("New Attendance","Getting Ready..", false, 300, 1000);
    displayTextLCD("New Attendance","Getting Ready...", false, 300, 1000);
    attendance.clear();
    digitalWrite(greenLed,LOW);
      digitalWrite(greenLed,LOW);
        IsAttendanceStarted = false;
        return;
        }
      
    }
    else
    {
      digitalWrite(redLed,HIGH);
      displayTextLCD("Invalid Pass","Try Again", false, 300, 4000);
      digitalWrite(redLed,LOW);
      keyPass = "";
      return;
    }
  }
  if (key =='B'){
    beep();
    if(keyPass.length() != 0)
    {
      
      keyPass = keyPass.substring(0,keyPass.length()-1);
      return;
    }
  }
  //this checkes if D is pressed, deletes all character stored in the keyPass variable
  if (key =='D'){
    beep();
    keyPass = "";
    return;
  }

  if (key =='*' && IsAttendanceEnded && IsAttendanceStarted){
    beep();
    keyPass = "";
    IsAttendanceEnded = false;
    IsAttendanceStarted = true;
    return;
  }
  if(key =='*'){
    return;
  }
  
   if (key){
    beep();
    keyPass = keyPass + key;
    //Serial.println(keyPass);
    
  } 
  
  if(keyPass == "" && IsAttendanceEnded && !IsAttendanceStarted)
  {
    // if no key has been pressed display
    displayTextLCD("SIGN IN","Enter Your ID", false);
  }
  else if (keyPass == "" && IsAttendanceEnded && IsAttendanceStarted)
  {
    displayTextLCD("To Save:","Enter Your ID", false);
    }
  else
  {
    displayTextLCD("ID Number",keyPass, false);
  }
  
}

void setup() {
  // put your setup code here, to run once:
  //Serial.begin(9600);
  pinMode(greenLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  
  // create a new character
  /*lcd.createChar(1, smiley);

  // Print a message to the LCD.
  lcd.write(1); //Print custom character smiley
  lcd.print("hello, world!");
  lcd.write(1);*/ //Print custom character smiley

  
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  //Serial.println("\n\nAdafruit finger detect test");

  // set the data rate for the sensor serial port
  finger.begin(57600);
  delay(5);
  if (finger.verifyPassword()) {
    //Serial.println("Found fingerprint sensor!");
  } else {
    //Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }

  //Serial.println(F("Reading sensor parameters"));
  finger.getParameters();
  //Serial.print(F("Status: 0x")); Serial.println(finger.status_reg, HEX);
  //Serial.print(F("Sys ID: 0x")); Serial.println(finger.system_id, HEX);
  //Serial.print(F("Capacity: ")); Serial.println(finger.capacity);
  //Serial.print(F("Security level: ")); Serial.println(finger.security_level);
  //Serial.print(F("Device address: ")); Serial.println(finger.device_addr, HEX);
  //Serial.print(F("Packet len: ")); Serial.println(finger.packet_len);
  //Serial.print(F("Baud rate: ")); Serial.println(finger.baud_rate);

  finger.getTemplateCount();

  if (finger.templateCount == 0) {
    //Serial.print("Sensor doesn't contain any fingerprint data. Please run the 'enroll' example.");
  }
  else {
    //Serial.println("Waiting for valid finger...");
      //Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  }
}


void beep()
{
  tone(10, 1000, 50); 
}

void loop() {
  // put your main code here, to run repeatedly:
  
  if((IsAttendanceEnded && !IsAttendanceStarted) || (IsAttendanceEnded && IsAttendanceStarted))
  {
    enterKeyPass();
  }
  else
  {
    getFingerprintID();

    //Checks if Lecturer wants to ternimate the  attendance
    char key = keypad.getKey();
    if (key =='C'){
      keyPass = "";
      beep();
      IsAttendanceEnded = true;
      
    
  }
    
    delay(50);            //don't ned to run this at full speed.
  }
  
}
