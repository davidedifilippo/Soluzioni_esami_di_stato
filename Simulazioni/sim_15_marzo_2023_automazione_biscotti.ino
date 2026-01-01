#include <Arduino.h>

/*Definizione dei pin digitali d'ingresso*/
const int startButtonPin = 4; //Pulsante di Start
const int pinSensoreBiscotti = 2; //il cambio di stato innesca un interrupt del micro
const int pinSensoreFlussoAcqua = 3; //il cambio di stato innesca un interrupt del micro
const int pinSensoreTegliaPresente = 4;

/*Definizione dei pin digitali di uscita*/
const int pinControlloImmissioneFarina = 5; 
const int pinControlloImmissioneAcqua= 6;
const int pinControlloImmissioneLievito= 7;
const int pinControlloScivoloFarina = 8;
const int pinControlloFrullino = 9;
const int pinControlloCoclea = 10;
const int pinControlloPinzaSigillatura = 11;


/*Definizione dei pin Analogici */
const int pinAnalogicoBilancia = A0;

/*Definizione delle variabili*/
int contatoreBiscotti = 0;
int contaImpulsiAcqua = 0;
int contaErogazioni = 0;
int maxErogazioni = 200;

bool impastoFinito = true;
bool tegliaPresente = false;
byte pesoFarina =0;  

long timeoutImmissioneLievito =  5000;
long timeoutFrullino =  5*60*1000; //5 miniuti in millisecondi
long timeoutCoclea =  4000; //5 miniuti in millisecondi
long segnaTempo = 0; 

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
pinMode(pinSensoreTegliaPresente, INPUT);

pinMode(pinControlloImmissioneFarina, OUTPUT);
pinMode(pinControlloImmissioneAcqua, OUTPUT);
pinMode(pinControlloImmissioneLievito, OUTPUT);
pinMode(pinControlloFrullino, OUTPUT);
pinMode(pinControlloCoclea, OUTPUT);
pinMode(pinControlloPinzaSigillatura, OUTPUT);

//associamo le routine di interrupt ai piedini sensibili ai fronti di discesa/salita
attachInterrupt(digitalPinToInterrupt(pinSensoreBiscotti), contaBiscotti, FALLING); 
attachInterrupt(digitalPinToInterrupt(pinSensoreFlussoAcqua), contaImpulsiFlussimetro, FALLING); 

}

void loop() {
  
while(digitalRead(startButtonPin)); //pull-up resistor -> se non premuto HIGH   

//Controllo se devo preparare l'impasto 
if(impastoFinito){
digitalWrite(pinControlloImmissioneFarina, HIGH); //Scarico la farina sulla bilancia

while(pesoFarina<80);
{
  int x = analogRead(A0);
  pesoFarina = (x/1023)*100; //Peso massimo 100 Newton
} 

digitalWrite(pinControlloImmissioneFarina, LOW);

digitalWrite(pinControlloScivoloFarina, HIGH); //Inserisco la farina nel miscelatore
digitalWrite(pinControlloImmissioneAcqua, HIGH); // Inserisco l'acqua nel miscelatore

while(contaImpulsiAcqua<=500); //Continuo a immettere acqua fino a raggiungere 5 Litri

digitalWrite(pinControlloImmissioneAcqua, LOW); //Basta acqua, tocca al lievito
contaImpulsiAcqua = 0; //resetto il contaimpulsi acqua

digitalWrite(pinControlloImmissioneLievito, HIGH); //Basta acqua, tocca al lievito

segnaTempo = millis(); //Memorizzo l'attimo di inizio immissione lievito 
while (millis()- segnaTempo <= timeoutImmissioneLievito); //Continuo a immettere lievito per 5 secondi

digitalWrite(pinControlloImmissioneLievito, LOW); //Basta lievito

digitalWrite(pinControlloFrullino, HIGH); //Impastiamo per 5 minuti

segnaTempo = millis(); //Memorizzo l'attimo di inizio immissione lievito 
while (millis()- segnaTempo <= timeoutFrullino); //Continuo a immettere lievito per 5 secondi
digitalWrite(pinControlloFrullino, LOW); //Fermo il frullino


} //Preparazione impasto conclusa


while(!tegliaPresente) 
  tegliaPresente = digitalRead(pinSensoreTegliaPresente);

digitalWrite(pinControlloCoclea, HIGH); //Aziono la coclea per 4 secondi
segnaTempo = millis(); //Memorizzo l'attimo di inizio immissione lievito 
while (millis()- segnaTempo <= timeoutCoclea); //Continuo a immettere lievito per 5 secondi
digitalWrite(pinControlloCoclea, LOW); //Fermo il frullino

contaErogazioni++;

if (contaErogazioni > maxErogazioni ) impastoFinito = true;

if (contatoreBiscotti == 10) {
   digitalWrite(pinControlloPinzaSigillatura, HIGH);
   contatoreBiscotti = 0;
  }
else 
  digitalWrite(pinControlloPinzaSigillatura, LOW);


}
