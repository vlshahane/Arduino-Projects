#define hampelWindowSize 23 
#define hampelHalfWindowSize 11
#define HampeThreshold 1 
#define Threshold 320

const int motorPinDir1  = 2;  // vibration motor transistor for direction 1 is connected to pin 2
const int motorPinDir2  = 3;  // vibration motor transistor for direction 2 is connected to pin 3
const int motorPinDir3  = 4;  // vibration motor transistor for direction 3 is connected to pin 4

const int trigPin1 = 7;
const int echoPin1 = 8;

const int trigPin2 = 9;
const int echoPin2 = 10;

const int trigPin3 = 11;
const int echoPin3 = 12;

int s1_cm=0;
int s2_cm=0;
int s3_cm=0;

// Hampel filter related declaration
int hampelFilterInSeneor1 [hampelWindowSize];
int hampelFilterInSeneor2 [hampelWindowSize];
int hampelFilterInSeneor3 [hampelWindowSize];
int hampelFilterBufSensor1 [hampelWindowSize];
int hampelFilterBufSensor2 [hampelWindowSize];
int hampelFilterBufSensor3 [hampelWindowSize];
int startIndex = 0;
int endIndex = 22;
int sensor1InitDone = 0;  //O means still waiting for letting array content to be full
int sensor2InitDone = 0;  //O means still waiting for letting array content to be full
int sensor3InitDone = 0;  //O means still waiting for letting array content to be full
int sensor1InitTime = 0;
int sensor2InitTime = 0;
int sensor3InitTime = 0;
int i;
int hampelFilterOutSensor1;  //This is the output of hampelfilter, which is also the moving average input (only one sample point)
int hampelFilterOutSensor2;
int hampelFilterOutSensor3;

// AMA related parameters
#define adj_window_length  11
int adj_idx = 10;
int AMABufSeneor1 [adj_window_length];
int AMABufSeneor2 [adj_window_length];
int AMABufSeneor3 [adj_window_length];
int AMAInput1 [adj_window_length];
int AMAInput2 [adj_window_length];
int AMAInput3 [adj_window_length];
int AMAOutSensor1;  //This is the output of AMA (only one sample point)
int AMAOutSensor2;
int AMAOutSensor3;
int hampel1Flag = 0;  //O means still waiting for letting array content to be full
int hampel2Flag = 0;
int hampel3Flag = 0;
int hampelcnt1 = 0;
int hampelcnt2 = 0;
int hampelcnt3 = 0;



void setup() 
{
  // initialize serial communication:
  Serial.begin(9600);
  pinMode(motorPinDir1, OUTPUT);
  pinMode(motorPinDir2, OUTPUT);
  pinMode(motorPinDir3, OUTPUT);
}
 
void loop()
{
  
  s1_cm = sensorReading(trigPin1, echoPin1);
  s2_cm = sensorReading(trigPin2, echoPin2);
  s3_cm = sensorReading(trigPin3, echoPin3);
  if(s1_cm > Threshold )
   s1_cm = Threshold; 
  if(s2_cm > Threshold )
   s2_cm = Threshold; 
  if(s3_cm > Threshold )
   s3_cm = Threshold;    
   
  /*
  Serial.print("Sensor1 Original Out: ");
  Serial.print(s1_cm);
  Serial.print(" cm | ");
  Serial.print("Sensor2 Original Out: ");
  Serial.print(s2_cm);
  Serial.print(" cm | ");
  Serial.print("Sensor3 Original Out: ");
  Serial.print(s3_cm);
  Serial.print(" cm ");
  Serial.println(); 
  */
  
  //In each operation, preparing the content of Sensor Array
  PrepareSeneorArray(hampelFilterInSeneor1, s1_cm,  startIndex, endIndex, sensor1InitDone, sensor1InitTime);
  PrepareSeneorArray(hampelFilterInSeneor2, s2_cm,  startIndex, endIndex, sensor2InitDone, sensor2InitTime);
  PrepareSeneorArray(hampelFilterInSeneor3, s3_cm,  startIndex, endIndex, sensor3InitDone, sensor3InitTime);
  
  // Sensor 1 initial array for tracking process
  if(sensor1InitDone == 0)
  {
    if(sensor1InitTime == endIndex)
    {
      sensor1InitTime = sensor1InitTime;
      sensor1InitDone = 1;    
    }
    else
      sensor1InitTime = sensor1InitTime + 1;    
  }
  else 
    sensor1InitDone = sensor1InitDone;
    
  // Sensor 2 initial array for tracking process
  if(sensor2InitDone == 0)
  {
    if(sensor2InitTime == endIndex)
    {
      sensor2InitTime = sensor2InitTime;
      sensor2InitDone = 1;    
    }
    else
      sensor2InitTime = sensor2InitTime + 1;    
  }
  else 
    sensor2InitDone = sensor2InitDone; 
    
  // Sensor 3 initial array for tracking process
  if(sensor3InitDone == 0)
  {
    if(sensor3InitTime == endIndex)
    {
      sensor3InitTime = sensor3InitTime;
      sensor3InitDone = 1;    
    }
    else
      sensor3InitTime = sensor3InitTime + 1;    
  }
  else 
    sensor3InitDone = sensor3InitDone;     
  
  if(sensor1InitDone == 1)
  {
    // This for loop is used to copy the stored contents of sensor sample array to the temporary space
    // for hampel filter usage
    for(i=startIndex;i<=endIndex;i++)
      hampelFilterBufSensor1[i] = hampelFilterInSeneor1[i];
    hampelFilterOutSensor1 = Hampel(hampelFilterBufSensor1, startIndex, endIndex);    
  
    //Serial.print("Sensor1: ");
    //Serial.print(hampelFilterOutSensor1);
    //Serial.print(" cm | ");  
  }

  if(sensor2InitDone == 1)
  {
    // This for loop is used to copy the stored contents of sensor sample array to the temporary space
    // for hampel filter usage
    for(i=startIndex;i<=endIndex;i++)
      hampelFilterBufSensor2[i] = hampelFilterInSeneor2[i];
    hampelFilterOutSensor2 = Hampel(hampelFilterBufSensor2, startIndex, endIndex);
    //Serial.print("Sensor2: ");
    //Serial.print(hampelFilterOutSensor2);
    //Serial.print(" cm | ");   
  }  
 
   if(sensor3InitDone == 1)
  {
    // This for loop is used to copy the stored contents of sensor sample array to the temporary space
    // for hampel filter usage
    for(i=startIndex;i<=endIndex;i++)
      hampelFilterBufSensor3[i] = hampelFilterInSeneor3[i];
    hampelFilterOutSensor3 = Hampel(hampelFilterBufSensor3, startIndex, endIndex);  
    //Serial.print("Sensor3: ");
    //Serial.print(hampelFilterOutSensor3);
    //Serial.print(" cm");
    //Serial.println(); 
  }   
  
  /*
  Serial.print("Sensor1 Hampel Out: ");
  Serial.print(hampelFilterOutSensor1);
  Serial.print(" cm | ");
  Serial.print("Sensor2  Hampel Out: ");
  Serial.print(hampelFilterOutSensor2);
  Serial.print(" cm | ");
  Serial.print("Sensor3 Hampel Out: ");
  Serial.print(hampelFilterOutSensor3);
  Serial.print(" cm");
  Serial.println();
  */
  
  
  /*
  if (hampelFilterOutSensor1 < 20) {
    vibrate1(motorPinDir1);
  }
  if (hampelFilterOutSensor2 < 20) {
    vibrate1(motorPinDir2);
  }
  if (hampelFilterOutSensor3 < 20) {
    vibrate1(motorPinDir3);
  }
  */
  
  /* Establish the input array for AMA algorithm */
  if(sensor1InitDone == 1)
    PrepareSeneorArray(AMABufSeneor1, hampelFilterOutSensor1,  startIndex, adj_idx, hampel1Flag, hampelcnt1);
  if(sensor2InitDone == 1)
    PrepareSeneorArray(AMABufSeneor2, hampelFilterOutSensor2,  startIndex, adj_idx, hampel2Flag, hampelcnt2);
  if(sensor3InitDone == 1)
    PrepareSeneorArray(AMABufSeneor3, hampelFilterOutSensor3,  startIndex, adj_idx, hampel3Flag, hampelcnt3);
  
  // AMA buffer 1 initial array for tracking process
  if(hampel1Flag == 0)
  {
    if(hampelcnt1 == adj_window_length)
    {
      hampelcnt1 = hampelcnt1;
      hampel1Flag = 1;    
    }
    else
      hampelcnt1 = hampelcnt1 + 1;    
  }
  else 
    hampel1Flag = hampel1Flag;
  
  // AMA buffer 2 initial array for tracking process
  if(hampel2Flag == 0)
  {
    if(hampelcnt2 == adj_window_length)
    {
      hampelcnt2 = hampelcnt2;
      hampel2Flag = 1;    
    }
    else
      hampelcnt2 = hampelcnt2 + 1;    
  }
  else 
    hampel2Flag = hampel2Flag;
  
  // AMA buffer 3 initial array for tracking process
  if(hampel3Flag == 0)
  {
    if(hampelcnt3 == adj_window_length)
    {
      hampelcnt3 = hampelcnt3;
      hampel3Flag = 1;    
    }
    else
      hampelcnt3 = hampelcnt3 + 1;    
  }
  else 
    hampel3Flag = hampel3Flag;
  
  /* Apply Adaptive Moving Average */
  if(hampel1Flag == 1)
  {
    for(i=startIndex;i<adj_window_length;i++)
      AMAInput1[i] = AMABufSeneor1[i];
    AMAOutSensor1 = Adj_Window_AMA(AMAInput1,adj_window_length);
    //Serial.print("AMA1: ");
    //Serial.print(AMAOutSensor1);
    //Serial.print(" cm | ");
  }
  if(hampel2Flag == 1)
  {
    for(i=startIndex;i<adj_window_length;i++)
      AMAInput2[i] = AMABufSeneor2[i];
    AMAOutSensor2 = Adj_Window_AMA(AMAInput2,adj_window_length);
    //Serial.print("AMA2: ");
    //Serial.print(AMAOutSensor2);
    //Serial.print(" cm | "); 
  }
  if(hampel3Flag == 1)
  {
    for(i=startIndex;i<adj_window_length;i++)
      AMAInput3[i] = AMABufSeneor3[i];
    AMAOutSensor3 = Adj_Window_AMA(AMAInput3,adj_window_length);
    //Serial.print("AMA3: ");
    //Serial.print(AMAOutSensor3);
    //Serial.print(" cm ");
    //Serial.println();
  }
  /*
  Serial.print("AMA1: ");
  Serial.print(AMAOutSensor1);
  Serial.print(" cm | ");
  Serial.print("AMA2: ");
  Serial.print(AMAOutSensor2);
  Serial.print(" cm | ");
  Serial.print("AMA3: ");
  Serial.print(AMAOutSensor3);
  Serial.print(" cm");
  Serial.println();
  */
  
  vibratorControl(hampel1Flag, AMAOutSensor1, s1_cm, motorPinDir1);
  vibratorControl(hampel2Flag, AMAOutSensor2, s2_cm, motorPinDir2);
  vibratorControl(hampel3Flag, AMAOutSensor3, s3_cm, motorPinDir3);
  
  delay(10);
}

int vibrate1(int motorPin)
{
   digitalWrite(motorPin,  HIGH);
   delay(150);
   digitalWrite(motorPin,  LOW);
   delay(10);
}

int vibrate2(int motorPin)
{
   digitalWrite(motorPin,  HIGH);
   delay(100);
   digitalWrite(motorPin,  LOW);
   delay(10);
}

int vibrate3(int motorPin)
{
   digitalWrite(motorPin,  HIGH);
   delay(50);
   digitalWrite(motorPin,  LOW);
   delay(10);
}

void vibratorControl(int flag, int sensorValue, int originalValue, int motorPin)
{
  if((flag == 1)&&(sensorValue > 0))
  {
    if (sensorValue < 120)
    {
      vibrate1(motorPin);
    }
    if (sensorValue > 120 && sensorValue < 220)
    {
      vibrate2(motorPin);
    }
    if (sensorValue > 220 && sensorValue < 320)
    {
      vibrate3(motorPin);
    }
  }
  else
  {
    if (originalValue < 120)
    {
      vibrate1(motorPin);
    }
    if (originalValue > 120 && originalValue < 220)
    {
      vibrate2(motorPin);
    }
    if (originalValue > 220 && originalValue < 320)
    {
      vibrate3(motorPin);
    }
  }
}

int sensorReading(int trigPin, int echoPin){
  
  // establish variables for duration of the ping, 
  // and the distance result in inches and centimeters:
  long duration, inches, cm;
  
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
 
  // convert the time into a distance
  //inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);
  return cm;
}

void PrepareSeneorArray(int *sensorArray, int seneorReadout,  int startIndex, int endIndex, int initDone, int initTimeIndex)
{
  int i;
  if(initDone == 0) // Means array is not full
  {
   sensorArray[initTimeIndex] = seneorReadout; 
  } 
  else //means array is already full
  {
    for(i=startIndex;i<endIndex;i++)  // left the latest sesorReadout to be put in the array after the loop finish 
    {
      sensorArray[i] =  sensorArray[i+1];
    }
    sensorArray[endIndex] = seneorReadout;
  }
}


/*Perform the hampel filter algorithm here to calculate the :
* 1. Local Nominal Data Reference Value and the Local Scale of Natural Variation 
* 2. Replace the outlier if necessary
*/
int Hampel (int *sampleBuffer, int startIndex, int endIndex)
{
  int median;
  float variance = 1.4826;
  int i;
  int absoluteInput1;
  int absoluteInput2;
  int varianceTemp;
  int sampleBufferForVariance [hampelWindowSize];
  int sampleFiltered;
  for (i=startIndex;i<=endIndex;i++)
  {
    sampleBufferForVariance[i] = sampleBuffer[i];   
    if(i == (endIndex>>1))
     sampleFiltered = sampleBuffer[i];    
  }
  // Calculate the median of the input data sequence  
  // And after this function the contents in sampleBuffer are already sorted 
  median = Median (sampleBuffer, startIndex, endIndex);
  // Calculate the variance of the input data sequence
  for(i=startIndex;i<=endIndex;i++)
  {
    absoluteInput1 = sampleBufferForVariance[i] - median; 
    sampleBufferForVariance[i] = abs(absoluteInput1);
  }
  //After performing the Median function, the sampleBufferForVariance is already sorted
  varianceTemp = Median (sampleBufferForVariance, startIndex, endIndex);
  //variance here is floaint point declared as float
  variance = variance * float(varianceTemp) * HampeThreshold; 
  // Remove the outlier
  absoluteInput2 = sampleFiltered - median;
  if(abs(absoluteInput2) > int(variance)){
   sampleFiltered = median; 
  } 
 return sampleFiltered; 
}

   
/* Find the Median of the given input sample sequence*/
int Median (int *sampleBuffer, int startIndex, int endIndex)
{ 
  int median;
  int centerIndex;
  // Perform the Selection Sort
  SelectionSort (startIndex, endIndex, sampleBuffer);
  // Here we only use the odd numbe window size
  centerIndex = endIndex >> 1;
  median = sampleBuffer[centerIndex];
  return median;
}
 
/* Implement the selection sort algorithm Start*/
// Y is pointer point to the begining of the received sample array
void SelectionSort(int Start, int End, int *Y)
{
  int i,j;
  int minKey; 
  for (i=Start;i<=End;i++) 
  { 
    minKey = FindMin(i, End, Y);
    swap(minKey, i, Y);
  }
}

void swap(int a, int b, int *W)
{
    int temp;
    temp = W[a];
    W[a] = W[b];
    W[b] = temp;
}

int FindMin(int Start, int End, int *Z)
{
  int i;
  int minKey = Start,         
  minValue = Z[Start];    
  for (i=Start;i<=End;i++)
    {
      if (Z[i]<minValue)  
        {                   
          minKey = i;             
          minValue = Z[i];   
        }
    }
  return minKey;  
}
/* Implement the selection sort algorithm End*/

/* Variance Calculation */
double variance(int * input_array,int array_length)
{
  int sum = 0;
  double avg, sumOfSquare;
  // average calculation
  for(int i = 0;i < array_length;i++)
  {
    sum = sum + input_array[i];
  }
  avg = sum / array_length;
  
  for(int i = 0;i < array_length;i++)
  {
    sumOfSquare = sumOfSquare + (input_array[i] - avg)*(input_array[i] - avg);
  }
  
  return (sumOfSquare/array_length);
}

/* Adaptive Moving Average reading */
int AMA(int pre_value,int cur_reading,int window_len)
{
  float k, oneK;
  // Calculate the exponential percentage
  k = 2 / (window_len + 1);
  oneK = 1-k;
  return (cur_reading * k + (pre_value * oneK));
}

/* Adjust window size dynamically */
int Adj_Window_AMA(int * input_array,int init_window)
{
  int window_len, pre_value, cur_reading;
  int sampleInterval[init_window-1], newInterval[init_window];
  double oldVar, newVar;
  
  for (int i = 0;i < (init_window);i++)
  {
    if(i == init_window-1)
    {
      cur_reading = input_array[i];
      newInterval[i] = cur_reading;
    }
    else
    {
      sampleInterval[i] = input_array[i];
      newInterval[i] = input_array[i];
      if(i == init_window-2)
      {
        pre_value = input_array[i];
      }
    }
  }
  
  oldVar = variance(sampleInterval,init_window-1);
  newVar = variance(newInterval,init_window);
  
  if (newVar > 2*oldVar)
  {
    window_len = 3;
  }
  else if (newVar < oldVar)
  {
    window_len = 10;
  }
  else
  {
    window_len = window_len;
  }
  
  return AMA(pre_value,cur_reading,window_len);
}
/* End of Adaptive Moving Filter */


long microsecondsToInches(long microseconds)
{
  return microseconds / 74 / 2;
}
 
long microsecondsToCentimeters(long microseconds)
{
  return microseconds / 29 / 2;
}
