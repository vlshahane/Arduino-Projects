
int incomingByte;      // a variable to read incoming serial data into
int counter = 0;
float tempC = 0.00;
//char charTempBuf[5];
//String tempStr[5];

float alarmThreshold_C = 24.00;   // Temperature threshold (in C)
int alarmFreq = 1000;     //in Hz
int alarmDuration = 1000; //in ms
String received = "";
int pinSpeaker = 13;     // digital channel

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
}

void loop() {
  // see if there's incoming serial data:
  if (Serial.available()) {
    // read the oldest byte in the serial buffer:
    incomingByte = Serial.read();
    
    switch(incomingByte){
      case 46:
        Serial.print('.');
        received.concat(".");
        break;
      case 48:
        Serial.print('0');
        received.concat(0);
        break;
      case 49:
        Serial.print('1');
        received.concat(1);
        break;
      case 50:
        Serial.print('2');
        received.concat(2);
        break;
      case 51:
        Serial.print('3');
        received.concat(3);
        break;
      case 52:
        Serial.print('4');
        received.concat(4);
        break;
      case 53:
        Serial.print('5');
        received.concat(5);
        break;
      case 54:
        Serial.print('6');
        received.concat(6);
        break;
      case 55:
        Serial.print('7');
        received.concat(7);
        break;
      case 56:
        Serial.print('8');
        received.concat(8);
        break;
      case 57:
        Serial.print('9');
        received.concat(9);
        break;
      default:
        Serial.print(' ');
        tempC = received.toFloat();
        if (tempC > 0)
        Serial.println(tempC);
        received = "";
        break;
    }
  }
  if(tempC >= alarmThreshold_C){
    tone(pinSpeaker,alarmFreq,alarmDuration); //tone(pin, frequency, duration)
  }
}


