/*******
 * setting divergence via BT enabled
 * "STEINS;GATE" added
 * BT module added
 * switch added
 * RTC added
 * botton added
 * dot of the second tube added
 * Nixie tubes show number randomly in different times
********/

#include <virtuabotixRTC.h>
#include <SoftwareSerial.h>
#include <Wire.h>

//4028
int d = 11;
int c = 10;
int b = 9;
int a = 8;

//74141
int h = 6;
int g = 5;
int f = 4;
int e = 3;

//ds1302
int sclk = 26;
int io = 28;
int rst = 30;

int rightDot = 13;
int numSwitch = 22;
int button = 24;
int SWITCH = 32;

int i, j, k, l, x, y, z;
int BTIndex = 0, strIndex;
int temp[9], runtime[9];
int continueFlash;
int buttonState = 0;
int switchState = 0;
int clockState = 0;
int year, month, dayOfMonth, dayOfWeek, hour, minute, second;
char sg[] = {'S','T','E','I','N','S',';','G','A','T','E','\0'};
char str[20], getByte;

// Creation of the Real Time Clock Object
virtuabotixRTC myRTC(sclk, io, rst);
//SoftwareSerial BTSerial(rx,tx); // Arduino RX/TX

void setup() {

  Serial.begin(19200); //Serial Monotor
  
  Serial1.begin(9600);//BT rx1 tx1
  
  pinMode(d, OUTPUT);// 4028-D
  pinMode(c, OUTPUT);// 4028-C
  pinMode(b, OUTPUT);// 4028-B
  pinMode(a, OUTPUT);// 4028-A
  
  pinMode(h, OUTPUT);// 74141-D
  pinMode(g, OUTPUT);// 74141-C
  pinMode(f, OUTPUT);// 74141-B
  pinMode(e, OUTPUT);// 74141-A

  pinMode(rightDot, OUTPUT);
  pinMode(numSwitch, OUTPUT);
  
  pinMode(button, INPUT);
  pinMode(SWITCH, INPUT);

  // Set the current date, and time in the following format:
  // seconds, minutes, hours, day of the week, day of the month, month, year
 // myRTC.setDS1302Time(00, 27, 12, 5, 30, 12, 2016);

  for(i=1; i<9; i++)
    temp[i] = random(0, 10);

  if(temp[1] != 1)
    temp[1] = 0;

}


void loop(){

  buttonState = digitalRead(button);
  switchState = digitalRead(SWITCH);

  if(switchState == HIGH){          //divergence mode

    //Serial.println("divergence");

    /////////////////////////////////////////////////////  BT
    while(Serial1.available()>0){   // get BT data

             getByte = Serial1.read();
             str[BTIndex] = getByte;
             BTIndex++;
    }

    if(getByte == '\n'){

      for(i=0; i<9; i++){
        
        Serial.print(i);
        Serial.print("  ");
        Serial.println(str[i]);
        }

        Serial.print("BTIndex = ");
        Serial.println(BTIndex);

        if(BTIndex==10 && str[0]=='d'){                  //divergence seeting

              changeDivergence(str[1]-'0', str[3]-'0', str[4]-'0', str[5]-'0', str[6]-'0', str[7]-'0', str[8]-'0');              
        }

        for(strIndex=0; strIndex<11; strIndex++){        // "STEINS;GATE" setting

            if(str[strIndex] == sg[strIndex]){}
            else 
               break;

              if(strIndex == 10){
                changeDivergence(1, 0, 4, 8, 5, 9, 6);       //STEINS;GATE
                Serial.println("STEINS;GATE   1.048596");     //STEINS;GATE 
                Serial.println("here2");
                
               }     
        }
              
      BTIndex = 0;
      strIndex = 0;
      getByte = ' ';
      Serial.println("here1");
     }
    
    
    if(buttonState == HIGH)
       divergenceDetect();

    else{
    
      digitalWrite(numSwitch, HIGH);
      digitalWrite(rightDot, LOW);
      light(1, temp[1]);
      delay(2);

      digitalWrite(numSwitch, LOW);
      lightDot(2);
      digitalWrite(rightDot, HIGH);
      delay(2);

      digitalWrite(numSwitch, HIGH);
      digitalWrite(rightDot, LOW);
      for(x=3; x<9; x++){
      light(x, temp[x]);
      delay(2);
      }
       

     }  // end of else


  }else{                            //clock mode

    //Serial.println("clock");

    while(Serial1.available()>0){   // get BT data

             getByte = Serial1.read();
             str[BTIndex] = getByte;
             BTIndex++;
    }

    if(getByte == '\n'){

      for(i=0; i<15; i++){
        
        Serial.print(i);
        Serial.print("  ");
        Serial.println(str[i]);
        }

        Serial.print("BTIndex = ");
        Serial.println(BTIndex);

        if(BTIndex==15 && str[0]=='t'){                  //time seting

            year = (str[1]-'0')*1000 + (str[2]-'0')*100 + (str[3]-'0')*10 + (str[4]-'0');
            month = (str[5]-'0')*10 + (str[6]-'0');
            dayOfMonth = (str[7]-'0')*10 + (str[8]-'0');
            dayOfWeek = (str[9]-'0');
            hour = (str[10]-'0')*10 + (str[11]-'0');
            minute = (str[12]-'0')*10 + (str[13]-'0');

            myRTC.setDS1302Time(second, minute, hour, dayOfWeek, dayOfMonth, month, year);                
        }
        
      BTIndex = 0;
      strIndex = 0;
      getByte = ' ';
      Serial.println("here2");
     }
    
    myRTC.updateTime();

    digitalWrite(numSwitch, HIGH);
    digitalWrite(rightDot, LOW);

    light(1, myRTC.hours/10);
    delay(2);

    light(2, myRTC.hours%10);
    delay(2);

    digitalWrite(numSwitch, LOW);
    lightDot(3);
    digitalWrite(rightDot, HIGH);
    delay(2);

    digitalWrite(numSwitch, HIGH);
    digitalWrite(rightDot, LOW);
  
    light(4, myRTC.minutes/10);
    delay(2);

    light(5, myRTC.minutes%10);
    delay(2);

    digitalWrite(numSwitch, LOW);
    lightDot(6);
    digitalWrite(rightDot, HIGH);
    delay(2);

    digitalWrite(numSwitch, HIGH);
    digitalWrite(rightDot, LOW);

    light(7, myRTC.seconds/10);
    delay(2);

    light(8, myRTC.seconds%10);
    delay(2);
  
    }
  
}   // end of  loop



 
void divergenceDetect() {

x=0; y=0; z=0;

i=0; j=0; k=0; l=0;

    runtime[1] = random(100, 200);  //randomly deciding flash time of first tube

    for(i=3; i<9; i++)               //randomly deciding flash time of each tube
      runtime[i] = random(100, 200);

    for(i=1; i<9; i++)
      temp[i] = 10;

    continueFlash = 1;
       
    while(continueFlash != 0){         //flash

     /*******code of tube 1**********/
     digitalWrite(numSwitch, HIGH);
     digitalWrite(rightDot, LOW);
      if(runtime[1] != 0){

        if(random(0, 10) != 1)
          light(1, 0);
        else 
          light(1, 1);
          
        delay(2);
        runtime[1]--;
      
      }else{
        
        light(1, temp[1]);
        delay(2);
        }

      /****code of tube 2******/
      digitalWrite(numSwitch, LOW);
      lightDot(2);
      digitalWrite(rightDot, HIGH);
      delay(2);

      /******code of tube 3-8*********/
      digitalWrite(numSwitch, HIGH);
      digitalWrite(rightDot, LOW);
      
      for(y=3; y<9; y++){

        if(runtime[y] != 0){
          light(y, random(0, 10));
          delay(2);
          runtime[y]--;
        
        }else{
          
          light(y, temp[y]);
          delay(2);
         }
       
        }    ////////////////end for y


    if(runtime[1]==0 && temp[1]==10){  //deciding the final number of tube 1   
      
      temp[1] = random(0, 10);
      
      if(temp[1] != 1)              //percentage of 1 is 10%
        temp[1] = 0;
      }
      

      for(i=3; i<9; i++){             //deciding the final number of tube 3-8

           if(runtime[i]==0 && temp[i]==10){
          
            temp[i] = random(0, 10);
            }          
         }    //end for


       continueFlash = runtime[1];   //counting the sum of runtime
                                     //continue while loop if sum != 0
       for(i=3; i<9; i++)            //
         continueFlash = continueFlash + runtime[i];//
      
      }     //end while


      
    /*********showing the final number of the tubes*************
    for(z=0; z<100; z++){         

      digitalWrite(numSwitch, HIGH);
      digitalWrite(rightDot, LOW);
      light(1, temp[1]);
      delay(2);

      digitalWrite(numSwitch, LOW);
      lightDot(2);
      digitalWrite(rightDot, HIGH);
      delay(2);

      digitalWrite(numSwitch, HIGH);
      digitalWrite(rightDot, LOW);
      for(x=3; x<9; x++){
      light(x, temp[x]);
      delay(2);
      }
    }    */

    
}


void changeDivergence(int a, int b, int c, int d, int e, int f, int g){
  
   x=0; y=0; z=0;
   i=0; j=0; k=0; l=0;

   runtime[1] = 50;
   
    for(i=3; i<9; i++)               //deciding flash time of each tube
      runtime[i] = (i-1) * 50;

    temp[1] = a;                    
    temp[3] = b;
    temp[4] = c;
    temp[5] = d;
    temp[6] = e;
    temp[7] = f;
    temp[8] = g;
    
    continueFlash = 1;
       
    while(continueFlash != 0){         //flash

     /*******code of tube 1**********/
     digitalWrite(numSwitch, HIGH);
     digitalWrite(rightDot, LOW);
      if(runtime[1] != 0){

        if(random(0, 10) != 1)
          light(1, 0);
        else 
          light(1, 1);
          
        delay(2);
        runtime[1]--;
      
      }else{
        
        light(1, temp[1]);
        delay(2);
        }

      /****code of tube 2******/
      digitalWrite(numSwitch, LOW);
      lightDot(2);
      digitalWrite(rightDot, HIGH);
      delay(2);

      /******code of tube 3-8*********/
      digitalWrite(numSwitch, HIGH);
      digitalWrite(rightDot, LOW);
      
      for(y=3; y<9; y++){

        if(runtime[y] != 0){
          light(y, random(0, 10));
          delay(2);
          runtime[y]--;
        
        }else{
          light(y, temp[y]);
        }
          delay(2);
         }
       
       continueFlash = runtime[1];   //counting the sum of runtime
                                     //continue while loop if sum != 0
       for(i=3; i<9; i++)            //
         continueFlash = continueFlash + runtime[i];//
      
      }     //end while
  
  }


void light(int i, int j){    //i:4028   j:74141

  switch(i){                           //////////////////////////////4028  a b c d

    case 0:
      digitalWrite(d, LOW);  //D
      digitalWrite(c, LOW);  //C
      digitalWrite(b, LOW);  //B
      digitalWrite(a, LOW);  //A
      break;
  
    case 1:
       digitalWrite(d, LOW);  //D
       digitalWrite(c, LOW);  //C
       digitalWrite(b, LOW);  //B
       digitalWrite(a, HIGH); //A
       break;
  
    case 2:
      digitalWrite(d, LOW);  //D
      digitalWrite(c, LOW);  //C
      digitalWrite(b, HIGH); //B
      digitalWrite(a, LOW);  //A
      break;
 
    case 3:
      digitalWrite(d, LOW);  //D
      digitalWrite(c, LOW);  //C
      digitalWrite(b, HIGH); //B
      digitalWrite(a, HIGH); //A
      break;
 
    case 4:
      digitalWrite(d, LOW);  //D
      digitalWrite(c, HIGH); //C
      digitalWrite(b, LOW);  //B
      digitalWrite(a, LOW);  //A
      break;
 
    case 5:
      digitalWrite(d, LOW);  //D
      digitalWrite(c, HIGH); //C
      digitalWrite(b, LOW);  //B
      digitalWrite(a, HIGH); //A
      break;
      
    case 6:
      digitalWrite(d, LOW);  //D
      digitalWrite(c, HIGH); //C
      digitalWrite(b, HIGH); //B
      digitalWrite(a, LOW);  //A
      break;
      
    case 7:
      digitalWrite(d, LOW);  //D
      digitalWrite(c, HIGH); //C
      digitalWrite(b, HIGH); //B
      digitalWrite(a, HIGH); //A
      break;
    
    case 8:
      digitalWrite(d, HIGH); //D
      digitalWrite(c, LOW);  //C
      digitalWrite(b, LOW);  //B
      digitalWrite(a, LOW);  //A
      break;
      
    case 9:
      digitalWrite(d, HIGH); //D
      digitalWrite(c, LOW);  //C
      digitalWrite(b, LOW); //B
      digitalWrite(a, HIGH);  //A
      break;
      
     
  }

  switch(j){        /////////////////////////////////74141

    case 0:
      digitalWrite(h, LOW);  //D
      digitalWrite(g, LOW);  //C
      digitalWrite(f, LOW);  //B
      digitalWrite(e, LOW);  //A
      break;
  
    case 1:
       digitalWrite(h, LOW);  //D
       digitalWrite(g, LOW);  //C
       digitalWrite(f, LOW);  //B
       digitalWrite(e, HIGH); //A
       break;
  
    case 2:
      digitalWrite(h, LOW);  //D
      digitalWrite(g, LOW);  //C
      digitalWrite(f, HIGH); //B
      digitalWrite(e, LOW);  //A
      break;
 
    case 3:
      digitalWrite(h, LOW);  //D
      digitalWrite(g, LOW);  //C
      digitalWrite(f, HIGH); //B
      digitalWrite(e, HIGH); //A
      break;
 
    case 4:
      digitalWrite(h, LOW);  //D
      digitalWrite(g, HIGH); //C
      digitalWrite(f, LOW);  //B
      digitalWrite(e, LOW);  //A
      break;
 
    case 5:
      digitalWrite(h, LOW);  //D
      digitalWrite(g, HIGH); //C
      digitalWrite(f, LOW);  //B
      digitalWrite(e, HIGH); //A
      break;
      
    case 6:
      digitalWrite(h, LOW);  //D
      digitalWrite(g, HIGH); //C
      digitalWrite(f, HIGH); //B
      digitalWrite(e, LOW);  //A
      break;
      
    case 7:
      digitalWrite(h, LOW);  //D
      digitalWrite(g, HIGH); //C
      digitalWrite(f, HIGH); //B
      digitalWrite(e, HIGH); //A
      break;
    
    case 8:
      digitalWrite(h, HIGH); //D
      digitalWrite(g, LOW);  //C
      digitalWrite(f, LOW);  //B
      digitalWrite(e, LOW);  //A
      break;
      
    case 9:
      digitalWrite(h, HIGH); //D
      digitalWrite(g, LOW);  //C
      digitalWrite(f, LOW); //B
      digitalWrite(e, HIGH);  //A
      break;
      
     
  }

}

void lightDot(int i){
  
  switch(i){
    
      case 1:
        digitalWrite(d, LOW); //D
        digitalWrite(c, LOW);  //C
        digitalWrite(b, LOW);  //B
        digitalWrite(a, HIGH);  //A
        break;

      case 2:
        digitalWrite(d, LOW); //D
        digitalWrite(c, LOW);  //C
        digitalWrite(b, HIGH);  //B
        digitalWrite(a, LOW);  //A
        break;

      case 3:
        digitalWrite(d, LOW); //D
        digitalWrite(c, LOW);  //C
        digitalWrite(b, HIGH);  //B
        digitalWrite(a, HIGH);  //A
        break;

      case 4:
        digitalWrite(d, LOW); //D
        digitalWrite(c, HIGH);  //C
        digitalWrite(b, LOW);  //B
        digitalWrite(a, LOW);  //A
        break;

      case 5:
        digitalWrite(d, LOW); //D
        digitalWrite(c, HIGH);  //C
        digitalWrite(b, LOW);  //B
        digitalWrite(a, HIGH);  //A
        break;

      case 6:
        digitalWrite(d, LOW); //D
        digitalWrite(c, HIGH);  //C
        digitalWrite(b, HIGH);  //B
        digitalWrite(a, LOW);  //A
        break;

      case 7:
        digitalWrite(d, LOW); //D
        digitalWrite(c, HIGH);  //C
        digitalWrite(b, HIGH);  //B
        digitalWrite(a, HIGH);  //A
        break;

      case 8:
        digitalWrite(d, HIGH); //D
        digitalWrite(c, LOW);  //C
        digitalWrite(b, LOW);  //B
        digitalWrite(a, LOW);  //A
        break;

    }
  }
