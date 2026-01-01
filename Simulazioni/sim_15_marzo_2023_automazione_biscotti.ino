#include <Arduino.h>

/*Definizione dei pin digitali */
const int startButtonPin = 4; //Pulsante di Start
const int pinSensoreBiscotti = 2; //il cambio di stato innesca un interrupt del micro
const int pinSensoreFlussoAcqua = 3; //il cambio di stato innesca un interrupt del micro
const int pincontrolloImmissionmeFarina = 5; 
const int scivoloFarina = 6;

/*Definizione dei pin Analogici */
const int pinAnalogicoBilancia = A0;

/*Definizione delle variabili*/
int contatoreBiscotti = 0;
int contaImpulsiAcqua = 0;
bool impastoFinito = true;
byte pesoFarina =0;  

//Ogni volta che cade un biscotto arriva un impulso di conteggio
void contaBiscotti()
{
     contatoreBiscotti++;
    }


//ogni 100 impulsi abbiamo un litro di acqua immesso nell'impasto
void contaImpulsiFlussimetro()
{
     contaImpulsiAcqua++;
    }


void setup() {

pinMode(startButtonPin, INPUT);
pinMode(pinSensoreBiscotti, INPUT);
pinMode(pinSensoreFlussoAcqua, INPUT);
pinMode(scivoloFarina, OUTPUT);

//associamo le routine di interrupt ai piedini sensibili ai fronti di discesa/salita
attachInterrupt(digitalPinToInterrupt(pinSensoreBiscotti), contaBiscotti, FALLING); 
attachInterrupt(digitalPinToInterrupt(pinSensoreFlussoAcqua), contaImpulsiFlussimetro, FALLING); 

}

void loop() {
  
while(digitalRead(startButtonPin)); //pull-up resistor -> se non premuto HIGH   

//Controllo se devo preparare l'impasto 
if(impastoFinito){
digitalWrite(pincontrolloImmissionmeFarina, HIGH); //Scarico la farina sulla bilancia

while(pesoFarina<80);
{
  int x = analogRead(A0);
  pesoFarina = (x/1023)*100; //Peso massimo 100 Newton
} 

digitalWrite(pincontrolloImmissionmeFarina, LOW);

digitalWrite(scivoloFarina, HIGH); //Inserisco la farina nel miscelatore
digitalWrite(pinSensoreFlussoAcqua, HIGH); // Inserisco l'acqua nel miscelatore

while(contaImpulsiAcqua<=500); //Continuo a immettere acqua fino a raggiungere 5 Litri
 
digitalWrite(pinSensoreFlussoAcqua, LOW); //Basta acqua, tocca al lievito

     
     
}

}
