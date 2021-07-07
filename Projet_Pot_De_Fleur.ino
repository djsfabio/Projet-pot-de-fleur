//-----------------IMPORT DE BIBLIOTHEQUE NECESSAIRES-----------------------------------------------------------------

#include <LowPower.h>

//-----------------FIN IMPORT-----------------------------------------------------------------------------------------

//-----------------INITIALISATION DE TOUTES LES VARIABLES NECESSAIRES-------------------------------------------------

int sensor=A0; //initializing the analog pin

int sensorLevel =A1;


int valueA;
int valueD; 

int sensor_digital=13; // initializing the digital pin

int led2 = 3;

int valeurPourcen = 0 ; 

int cent= 100;

int quarante = 40; 

int deuxcentquinze = 215 ; 

int valueLevel; 

//-----------------FIN DE L'INITIALISATION DE TOUTES LES VARIABLES NECESSAIRES-----------------------------------------


void setup(){
  Serial.begin(9600);

//----------------SETUP DE TOUS LES PINS-------------------------------------------------------------------------------

  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,LOW);
  
  pinMode(11, OUTPUT); //On envoie de l'énergie pour le capteur de niveau d'eau
  pinMode(sensorLevel, INPUT); //On reçoit un signal analogique pour le capteur de niveau d'eau
  
  pinMode(12,OUTPUT); //On envoie de l'energie au niveau de la PIN 12 qui correspond à ce qui est branché sur la bread board
  
  pinMode(sensor, INPUT); // On reçoit un signal analogique pour le capteur d'humidité 
  pinMode(sensor_digital, INPUT); // On reçoit un signal numérique pour le capteur d'humidité

//-----------------FIN DE SETUP DE TOUS LES PINS-----------------------------------------------------------------------
}

void loop(){
  //On envoie constamment de l'énergie dans le capteur de niveau d'eau
  digitalWrite(11, HIGH);
 
//-----------------CAPTATION DES VALEURS DE L'ENVIRONNEMENT------------------------------------------------------------
  valueLevel = analogRead(sensorLevel); //Lecture du niveau d'eau en analog
  valueLevel = map(valueLevel,0,1023,0,40); //On remap les valeurs lues pour qu'elles soient comprises entre 0 et 40

  valueA=analogRead(sensor); //On lit la valeur analog du capteur d'humidité
  valueA =map(valueA,0,1023,0,255); //On remap la valeur analog du capteur d'humidité pour qu'elle aille de 0 à 255 à la place de 0 à 1023.
  
  valueD=digitalRead(sensor_digital); //On lit la valeur digitale du capteur d'humidité
  
  valeurPourcen = 100 - (valueA/2.55);//Permet de mettre la valeur Analog en pourcentage, et ainsi avoir une valeur maniable. 

  //Ecriture des valeurs captées dans les logs
  Serial.print("Valeur du niveau d'eau : ");
  Serial.println(valueLevel);
  Serial.print("Captation d'environnement : "); 
  Serial.println(valueD); 
  Serial.print("Pourcentage d'humidité dans le milieu : ");
  Serial.print(valeurPourcen);
  Serial.println(" %");
  
//-----------------FIN DE CAPTATION------------------------------------------------------------------------------------

//-----------------PARTIE QUI TRAITE DU L'HUMIDITE ET DU NIVEAU D'EAU--------------------------------------------------

// Si l'humidité est inférieur à 70% et qu'au même moment le niveau d'eau est supérieur à 10, alors on arrose
  if(valeurPourcen<70 && valueLevel>=10){
    Serial.println("L'humidité étant passée en dessous de 70% d'humidité on arrose.");
    //Ici on envoie du courant dans la PIN 12 qui équivaut à la pompe
    digitalWrite(12,HIGH);
    delay(1000);
  }
// Dans le cas contraire, alors l'arrosage est mis en pause et on indique les conditions qui ne sont pas remplies
  else{
    //Ici on désactive le courant envoyé à la pompe
    digitalWrite(12,LOW);
    Serial.println("Arrosage mis en pause car les conditions nécessaires ne sont pas remplies.");
    Serial.println("Voici les conditions non remplies :");
    if(valueLevel<10){
      Serial.println("  - Valeur critique du niveau d'eau, veuillez recharger.");
    }
    else{
      Serial.println("  - Valeur d'humidité supérieure à 70%.");
    }
    
//-----------------PARTIE CONCERNANT LE SLEEP MODE DE L'ARDUINI, OBLIGATOIREMENT 8 SECONDES----------------------------
    
    Serial.println("Lancement du mode économie d'énergie pendant 8 secondes.");
    delay(1000);
    digitalWrite(LED_BUILTIN,LOW);
    LowPower.idle(SLEEP_8S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF,
                   SPI_OFF, USART0_OFF, TWI_OFF);
    Serial.println("Désactivation du mode économie d'énergie.");
    
//-----------------FIN DE LA PARTIE ECONOMIE D'ENERGIE-----------------------------------------------------------------
  }
  //Saut de ligne pour séparer les blocs de logs
  Serial.println("");
  Serial.println("");
  Serial.println("");

}
