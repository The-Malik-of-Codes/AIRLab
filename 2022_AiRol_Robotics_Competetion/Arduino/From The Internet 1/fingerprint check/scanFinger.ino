#include <Keypad.h> //keypad library
#include <EEPROM.h> //rom library for save data
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

//settup -keypad
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
SoftwareSerial mySerial(10, 11); // RX, TX

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};


byte rowPins[ROWS] = {23, 25, 27, 29}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {22, 24, 26, 28}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

const int rs = 33, en = 31, d4 = 36, d5 = 34, d6 = 32, d7 = 30;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


String getIdFromKeyboard(){
byte  val=5;char key;
//while(!(val= keypad.getKey()));

String inString="";
  //Serial.println(val);

  Serial.print("Enter ID : ");
   lcd.print("Enter ID : ");
  byte a = 0;
  while (1) {
    
    while(!(key = keypad.getKey()));
    Serial.println(key);
   
    if(key == 'D'){
      String tmp = inString;
      tmp[a-1] = '\0';
      inString = "";
      for(int i=0;i<tmp.length()-1;i++){
        inString += tmp[i];
      }
      
      a--;
      Serial.println(inString);
      lcd.clear();
      lcd.print("Enter ID : ");
      lcd.setCursor(3,1);
      lcd.print(inString);
    }
    if (isDigit(key) && inString.length()<5) {
      inString += key;
      Serial.println(inString);
      lcd.setCursor(3,1);
      lcd.print(inString);
      a++;
    }
     if(a==val  && key =='A'){
      lcd.setCursor(10,1);
      lcd.print("OK");
      delay(3000);
      lcd.clear();
      return inString;
    }
   // lcd.setCursor(3, 1); //setCursur(col,row)
   // lcd.print(inString);
    delay(3000);
    
  }lcd.clear();
}
boolean initial(){
   Serial3.write("\rfingerMatch\r\n");
  
  Serial3.flush();
  Serial.println("done1");
  
  delay(1000);
  while(Serial3.available()==0);
   while(Serial3.available()){
    Serial.println("done2");
   String line= Serial3.readStringUntil("</html>");
   
    Serial.println(line);
    }
 return true;   
}
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
Serial3.begin(115200);
mySerial.begin(19200);
lcd.begin(16, 2);

// while( !initial());
  
}

int compare();
void loop() {
  // put your main code here, to run repeatedly:
/* String input =  getIdFromKeyboard();
 //lcd.setCursor(3,1);
 //  lcd.print(input);
 while(!input);
   Serial3.println(input);
   
   Serial3.flush();*/
   char key;
   lcd.print("Keep Your Finger");
   int id=compare();
   delay(3000);
   lcd.clear();
   if(id==0){
    Serial.println(compare());
    lcd.print("Retry");
    delay(3000);
   lcd.clear();
    }
   else{
   lcd.print(id);Serial.println(compare());
   delay(3000);
   lcd.clear();
   }
//   key = keypad.getKey();
//   if(key=='A'){
//    lcd.clear();
//    lcd.print("OK");
//   delay(3000);
//   lcd.clear();}
//   else{
//    
//    }
//   }
//    key = keypad.getKey();
//    if(key=='C'){
//      lcd.print(getIdFromKeyboard());
//      delay(3000);
//   lcd.clear();
//      }
   delay(2000);
   //Serial3.println("Hello");
   
}

void saveToDatabase(int id,char eigenValues[]){

  byte addCmd[] = {0xF5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF5};
  byte checkSum = 0;
  byte reply[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  
  addCmd[0] = 0xF5;
  addCmd[7] = 0xF5;
  addCmd[1] = 0x41;
  addCmd[2] = 0;
  addCmd[3] = 196;
  addCmd[4] = 1;
  addCmd[5] = 0;
  checkSum = 0;
 // byte eigen[] ={12, 70, 153, 206, 65 ,71, 35, 164, 225, 79, 47 ,143, 1 ,85 ,171, 77, 225, 86, 26 ,171, 97, 113, 35, 146, 97, 123, 14 ,24 ,65, 128, 140, 194 ,1 ,129 ,19 ,192 ,193 ,119, 37, 41, 98, 120, 49 ,145, 2 ,146 ,42 ,106, 2 ,0 ,0};
    //char  eige[] ="12 70 153 206 65 71 35 164 225 79 47 143 1 85 171 77 225 86 26 171 97 113 35 146 97 123 14 24 65 128 140 194 1 129 19 192 193 119 37 41 98 120 49 145 2 146 42 106 2 0 0 ";
    char* eige=eigenValues;
   byte eigen[51];
    for(int i=1; i<6; i++){
    checkSum = checkSum^addCmd[i];
  }
  addCmd[6] = checkSum;int k=0;
  //while(eigen[k]!=NULL){Serial.print(eigen[k]+" ");k++;}
  for(int i=0; i<8; i++){
    mySerial.write(addCmd[i]);
    Serial.print(addCmd[i]);
    Serial.print(' ');
  }
  Serial.println();
  checkSum=0;
  mySerial.write((byte)0xF5);
  mySerial.write((byte)(id/256));checkSum = checkSum^(id/256);
  mySerial.write((byte)(id%256));checkSum = checkSum^(id%256);
  mySerial.write((byte)1);checkSum = checkSum^1;
 
  char* str = strtok(eige, " ");
 while (str != NULL){ // delimiter is the semicolon
  
    mySerial.write((byte)atoi(str));checkSum = checkSum^atoi(str);
  str = strtok(NULL, " ");
 
 
 } 

 
 
  for(int k=0;k<193-51;k++)mySerial.write((byte)0);
  mySerial.write(checkSum);
  mySerial.write((byte)0xF5);
  delay(1000);
  
  
  
  delay(1000);
  while(mySerial.available()<8){};
  int i=0;
  while(mySerial.available()>0 ){
    reply[i] = mySerial.read();
    Serial.print(reply[i]);
    Serial.print(' ');
    i++;
  }
  Serial.println();
  
 
}
int compare(){

  byte addCmd[] = {0xF5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF5};
  byte checkSum = 0;
  byte reply[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  
  addCmd[0] = 0xF5;
  addCmd[7] = 0xF5;
  addCmd[1] = 0x0C;
  addCmd[2] = 0;
  addCmd[3] = 0;
  addCmd[4] = 0;
  addCmd[5] = 0;
  checkSum = 0;
  
    for(int i=1; i<6; i++){
    checkSum = checkSum^addCmd[i];
  }
  addCmd[6] = checkSum;
  
  for(int i=0; i<8; i++){
    mySerial.write(addCmd[i]);
    Serial.print(addCmd[i]);
    Serial.print(' ');
  }
  Serial.println();
  delay(1000);
  
  delay(1000);
  while(mySerial.available()<8){};
  int i=0;
  while(mySerial.available()>0 ){
    reply[i] = mySerial.read();
    Serial.print(reply[i]);
    Serial.print(' ');
    i++;
  }Serial.println();
  if (reply[4]==1)
 return reply[2]*256+reply[3];
 else return 0;
  
}


void deleteUser(int id){

  byte addCmd[] = {0xF5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF5};
  byte checkSum = 0;
  byte reply[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  
  addCmd[0] = 0xF5;
  addCmd[7] = 0xF5;
  addCmd[1] = 0x04;
  addCmd[2] = id/256;
  addCmd[3] = id%256;
  addCmd[4] = 0;
  addCmd[5] = 0;
  checkSum = 0;
  
    for(int i=1; i<6; i++){
    checkSum = checkSum^addCmd[i];
  }
  addCmd[6] = checkSum;
  
  for(int i=0; i<8; i++){
    mySerial.write(addCmd[i]);
    Serial.print(addCmd[i]);
    Serial.print(' ');
  }
  Serial.println();
  delay(1000);
  
  delay(1000);
  while(mySerial.available()<8){};
  int i=0;
  while(mySerial.available()>0 ){
    reply[i] = mySerial.read();
    Serial.print(reply[i]);
    Serial.print(' ');
    i++;
  }Serial.println();
}

void addUser(int id){
  byte addCmd[] = {0xF5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF5};
  byte checkSum = 0;
  byte reply[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
 
 for(int k=1;k<4;k++){ 
  addCmd[0] = 0xF5;
  addCmd[7] = 0xF5;
  addCmd[1] = k;
  addCmd[2] = id/256;
  addCmd[3] = id%256;
  addCmd[4] = 1;
  addCmd[5] = 0;
  checkSum = 0;
  
    for(int i=1; i<6; i++){
    checkSum = checkSum^addCmd[i];
  }
  addCmd[6] = checkSum;
  
  for(int i=0; i<8; i++){
    mySerial.write(addCmd[i]);
    Serial.print(addCmd[i]);
    Serial.print(' ');
  }
  Serial.println();
  delay(1000);
  
  delay(1000);
  while(mySerial.available()<8){};
  int i=0;
  while(mySerial.available()>0 ){
    reply[i] = mySerial.read();
    Serial.print(reply[i]);
    Serial.print(' ');
    i++;
  }Serial.println();
 }
}

void deleteAll(){
   byte addCmd[] = {0xF5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF5};
  byte checkSum = 0;
  byte reply[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  
  addCmd[0] = 0xF5;
  addCmd[7] = 0xF5;
  addCmd[1] = 0x05;
  addCmd[2] = 0x00;
  addCmd[3] = 0x00;
  addCmd[4] = 0;
  addCmd[5] = 0;
  checkSum = 0;
  
    for(int i=1; i<6; i++){
    checkSum = checkSum^addCmd[i];
  }
  addCmd[6] = checkSum;
  
  for(int i=0; i<8; i++){
    mySerial.write(addCmd[i]);
    Serial.print(addCmd[i]);
    Serial.print(' ');
  }
  Serial.println();
  delay(1000);
  
  delay(1000);
  while(mySerial.available()<8){};
  int i=0;
  while(mySerial.available()>0 ){
    reply[i] = mySerial.read();
    Serial.print(reply[i]);
    Serial.print(' ');
    i++;
  }Serial.println();
 }

 void wipeData(){
   byte addCmd[] = {0xF5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF5};
  byte checkSum = 0;
  byte reply[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  
  addCmd[0] = 0xF5;
  addCmd[7] = 0xF5;
  addCmd[1] = 0x3A;
  addCmd[2] = 0;
  addCmd[3] = 0;
  addCmd[4] = 0;
  addCmd[5] = 0;
  checkSum = 0;
  
    for(int i=1; i<6; i++){
    checkSum = checkSum^addCmd[i];
  }
  addCmd[6] = checkSum;
  
  for(int i=0; i<8; i++){
    mySerial.write(addCmd[i]);
    Serial.print(addCmd[i]);
    Serial.print(' ');
  }
  Serial.println();
  delay(1000);
  
  delay(1000);
  while(mySerial.available()<8){};
  int i=0;
  while(mySerial.available()>0 ){
    reply[i] = mySerial.read();
    Serial.print(reply[i]);
    Serial.print(' ');
    i++;
  }Serial.println();
  }
