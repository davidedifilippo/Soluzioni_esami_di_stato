#include <Arduino.h>

const int startButtonPin = 4; //Pulsante di Start

const int pinSensoreBiscotti = 2; //Pulsante di Start
const int pinSensoreFlussoAcqua = 3; //Pulsante di Start
const int pincontrolloImmissionmeFarina = 5; 




int contaB = 0;
bool impastoFinito = true;
byte pesoFarina =0;  




void contaBiscotti()
{
     contaB++;
    }


void contaImpulsi()
{
     contaB++;
    }


void setup() {

pinMode(startButtonPin, INPUT);



 

pinMode(pinSensoreBiscotti, INPUT);

//ad ogni rilevazione di passaggio di un biscotto incrementa il contatore

attachInterrupt(digitalPinToInterrupt(pinSensoreBiscotti), contaBiscotti, RISING); 

pinMode(pinSensoreBiscotti, INPUT);

//ad ogni rilevazione di passaggio di un biscotto incrementa il contatore

attachInterrupt(digitalPinToInterrupt(pinSensoreFlussoAcqua), contaImpulsi, RISING); 

}

void loop() {
  
while(digitalRead(!startButtonPin)); //pull-up resistor -> se non premuto HIGH   



if(!impastoFinito){

digitalWrite(pincontrolloImmissionmeFarina, HIGH);

do{

int x = analogRead(0);

pesoFarina = (x/1023)*100;


} while(pesoFarina<80);




}

}
