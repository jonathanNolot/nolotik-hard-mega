/*
 * WebServerParsing
 *
 * Respond to requests in the URL to change digital and analog output ports
 * show the number of ports changed and the value of the analog input pins.
 *
 * the following url turns digital pin 2 on and 3 off, analog output on pin 9 is set to 128, pin 11 to 255 
 * only onen parm http://192.168.1.177/2
 *
 * http://192.168.1.177/?pinD2=1&pinD3=0&pinA9=128&pinA11=255
 */
 
#if ARDUINO > 18
#include <SPI.h>         // needed for Arduino versions later than 0018 
#endif

#include <Ethernet.h>
#include <string.h>
#include <TextFinder.h>
#include <EEPROM.h>
#include <OneWire.h> // Inclusion de la librairie OneWire

#define S1 22
#define S2 23
#define S3 24
#define S4 25
#define S1_EEPROM_ADRESS 0
#define S2_EEPROM_ADRESS 1
#define S3_EEPROM_ADRESS 2
#define S4_EEPROM_ADRESS 3
#define DS18B20 0x28     // Adresse 1-Wire du DS18B20
#define BROCHE_ONEWIRE_1 26 // Broche utilisée pour le bus 1-Wire
#define BROCHE_ONEWIRE_2 27 // Broche utilisée pour le bus 1-Wire
#define BROCHE_ONEWIRE_3 28 // Broche utilisée pour le bus 1-Wire
#define BROCHE_ONEWIRE_4 29 // Broche utilisée pour le bus 1-Wire
#define BROCHE_ONEWIRE_5 30 // Broche utilisée pour le bus 1-Wire
#define BROCHE_ONEWIRE_6 31 // Broche utilisée pour le bus 1-Wire
#define i1 32
#define i2 33
#define i3 34


byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 192, 168, 1, 70 }; // 
byte gateway[] = { 192, 168, 1, 1 }; // 
byte subnet[] = { 255, 255, 255, 0 }; //
EthernetServer server(80);
EthernetClient client;
OneWire ds1(BROCHE_ONEWIRE_1); // Création de l'objet OneWire ds
OneWire ds2(BROCHE_ONEWIRE_2); // Création de l'objet OneWire ds
OneWire ds3(BROCHE_ONEWIRE_3); // Création de l'objet OneWire ds
OneWire ds4(BROCHE_ONEWIRE_4); // Création de l'objet OneWire ds
OneWire ds5(BROCHE_ONEWIRE_5); // Création de l'objet OneWire ds
OneWire ds6(BROCHE_ONEWIRE_6); // Création de l'objet OneWire ds
TextFinder  finder(client);
int digitalRequests;
int analogRequests;
int s1,s2,s3,s4;
float tempVal;
char tempValChar[5];

void httpRequestProcess(){
        digitalRequests = 0;  // counters to show the number of pin change requests
        analogRequests = 0;
        if( finder.find("GET /") ) {            
          // find tokens starting with "pin" and stop on the first blank line  
          while(finder.findUntil("s", "\n\r")){  
            int sortie = finder.getValue();
            int val = finder.getValue();
              
			if(sortie == 1){
				Serial.print("Sortie 1 ");
				pinMode(S1, OUTPUT);
				digitalWrite(S1, !val);
				EEPROM.write(S1_EEPROM_ADRESS, !val);
				s1 = val;
				Serial.print("= ");
				Serial.println(val);
			}
			
			if(sortie == 2){
				Serial.print("Sortie 2 ");
				pinMode(S2, OUTPUT);
				digitalWrite(S2, !val);
				EEPROM.write(S2_EEPROM_ADRESS, !val);
				s2 = val;
				Serial.print("= ");
				Serial.println(val);
			}
			
			if(sortie == 3){
				Serial.print("Sortie 3 ");
				pinMode(S3, OUTPUT);
				digitalWrite(S3, !val);
				EEPROM.write(S3_EEPROM_ADRESS, !val);
				s3 = val;
				Serial.print("= ");
				Serial.println(val);
			}
			
			if(sortie == 4){
				Serial.print("Sortie 4 ");
				pinMode(S4, OUTPUT);
				digitalWrite(S4, !val);
				EEPROM.write(S4_EEPROM_ADRESS, !val);
				s4 = val;
				Serial.print("= ");
				Serial.println(val);
			}
          }                         
        }        
        Serial.println();
}


String jsonEncodeAttribut(char *nom, char* val){
	String jsontmp = String();
	jsontmp += "\"";
	jsontmp += nom;
	jsontmp += "\":\"";
	jsontmp += val;
	return jsontmp;
}

String jsonEncodeVirgule(){
	return "\",";
}

String jsonEncodeDebut(){
	return "{";
}

String jsonEncodeFin(){
	return "\"}";
}

void httpResponse(){

    String jsonOut = String();
	jsonOut += jsonEncodeDebut();
	

	//  assemble the json output

	char buffer[2];
	
	//t1
	if(getTemperature(&tempVal, ds1)) {
		Serial.println("valeur de t1 : "); 
		Serial.println(tempVal); 
		dtostrf(tempVal, 4, 3, tempValChar);
		jsonOut += jsonEncodeAttribut("t1",tempValChar);
	} else {jsonOut += jsonEncodeAttribut("t1","null");}
	jsonOut += jsonEncodeVirgule();
	//t2
	if(getTemperature(&tempVal, ds2)) {
		Serial.println("valeur de t2 : "); 
		Serial.println(tempVal); 
		dtostrf(tempVal, 4, 3, tempValChar);
		jsonOut += jsonEncodeAttribut("t2",tempValChar);
	} else {jsonOut += jsonEncodeAttribut("t2","null");}
	jsonOut += jsonEncodeVirgule();
	//t3
	if(getTemperature(&tempVal, ds3)) {
		Serial.println("valeur de t3 : "); 
		Serial.println(tempVal); 
		dtostrf(tempVal, 4, 3, tempValChar);
		jsonOut += jsonEncodeAttribut("t3",tempValChar);
	} else {jsonOut += jsonEncodeAttribut("t3","null");}
	jsonOut += jsonEncodeVirgule();
	//t4
	if(getTemperature(&tempVal, ds4)) {
		Serial.println("valeur de t4 : "); 
		Serial.println(tempVal); 
		dtostrf(tempVal, 4, 3, tempValChar);
		jsonOut += jsonEncodeAttribut("t4",tempValChar);
	} else {jsonOut += jsonEncodeAttribut("t4","null");}
	jsonOut += jsonEncodeVirgule();
	//t5
	if(getTemperature(&tempVal, ds5)) {
		Serial.println("valeur de t5 : "); 
		Serial.println(tempVal); 
		dtostrf(tempVal, 4, 3, tempValChar);
		jsonOut += jsonEncodeAttribut("t5",tempValChar);
	} else {jsonOut += jsonEncodeAttribut("t5","null");}
	jsonOut += jsonEncodeVirgule();
	//t6
	if(getTemperature(&tempVal, ds6)) {
		Serial.println("valeur de t6 : "); 
		Serial.println(tempVal); 
		dtostrf(tempVal, 4, 3, tempValChar);
		jsonOut += jsonEncodeAttribut("t6",tempValChar);
	} else {jsonOut += jsonEncodeAttribut("t6","null");}
	jsonOut += jsonEncodeVirgule();
	
	//
	int val = 0;
	//
	val = digitalRead(i1);
	jsonOut += jsonEncodeAttribut("i1",itoa(!val,buffer,10));
	jsonOut += jsonEncodeVirgule();
	val = digitalRead(i2);
	jsonOut += jsonEncodeAttribut("i2",itoa(!val,buffer,10));
	jsonOut += jsonEncodeVirgule();
	val = digitalRead(i3);
	jsonOut += jsonEncodeAttribut("i3",itoa(!val,buffer,10));
	jsonOut += jsonEncodeVirgule();
	//
	jsonOut += jsonEncodeAttribut("s1",itoa(s1,buffer,10));
	jsonOut += jsonEncodeVirgule();
	jsonOut += jsonEncodeAttribut("s2",itoa(s2,buffer,10));
	jsonOut += jsonEncodeVirgule();
	jsonOut += jsonEncodeAttribut("s3",itoa(s3,buffer,10));
	jsonOut += jsonEncodeVirgule();
	jsonOut += jsonEncodeAttribut("s4",itoa(s4,buffer,10));
	
	jsonOut += jsonEncodeFin();


	// when we get here the findUntil has detected the blank line (a lf followed by cr)
	// so the http request has ended and we can send a reply
	// send a standard http response header
	client.println("HTTP/1.1 200 OK");
	client.println("Content-Type: application/json");
	client.println("Access-Control-Allow-Origin: *");
	client.println();
	client.println(jsonOut);
}




void initSorties(){


  Serial.println("init sorties");
  s1 = EEPROM.read(S1_EEPROM_ADRESS);
  s2 = EEPROM.read(S2_EEPROM_ADRESS);
  s3 = EEPROM.read(S3_EEPROM_ADRESS);
  s4 = EEPROM.read(S4_EEPROM_ADRESS);
  
  Serial.print("s1 = ");
  Serial.println(s1);
  Serial.print("s2 = ");
  Serial.println(s2);
  Serial.print("s3 = ");
  Serial.println(s3);
  Serial.print("s4 = ");
  Serial.println(s4);
  //
  pinMode(S1, OUTPUT);
  digitalWrite(S1, s1);
  //
  pinMode(S2, OUTPUT);
  digitalWrite(S2, s2);
  //
  pinMode(S3, OUTPUT);
  digitalWrite(S3, s3);
  //
  pinMode(S4, OUTPUT);
  digitalWrite(S4, s4);
}

void initEntrees(){
  pinMode(i1, INPUT);
  pinMode(i2, INPUT);
  pinMode(i3, INPUT);
}


// Fonction récupérant la température depuis le DS18B20
// Retourne true si tout va bien, ou false en cas d'erreur
boolean getTemperature(float *temp, OneWire ds18b20){
  byte data[9], addr[8];
  // data : Données lues depuis le scratchpad
  // addr : adresse du module 1-Wire détecté
 
  if (!ds18b20.search(addr)) { // Recherche un module 1-Wire
    ds18b20.reset_search();    // Réinitialise la recherche de module
    return false;         // Retourne une erreur
  }
   
  if (OneWire::crc8(addr, 7) != addr[7]) // Vérifie que l'adresse a été correctement reçue
    return false;                        // Si le message est corrompu on retourne une erreur
 
  if (addr[0] != DS18B20) // Vérifie qu'il s'agit bien d'un DS18B20
    return false;         // Si ce n'est pas le cas on retourne une erreur
 
  ds18b20.reset();             // On reset le bus 1-Wire
  ds18b20.select(addr);        // On sélectionne le DS18B20
   
  ds18b20.write(0x44, 1);      // On lance une prise de mesure de température
  delay(800);             // Et on attend la fin de la mesure
   
  ds18b20.reset();             // On reset le bus 1-Wire
  ds18b20.select(addr);        // On sélectionne le DS18B20
  ds18b20.write(0xBE);         // On envoie une demande de lecture du scratchpad
 
  for (byte i = 0; i < 9; i++) // On lit le scratchpad
    data[i] = ds18b20.read();       // Et on stock les octets reçus
   
  // Calcul de la température en degré Celsius
  *temp = ((data[1] << 8) | data[0]) * 0.0625; 
   
  // Pas d'erreur
  return true;
}

void setup()
{
  Serial.begin(9600);
  initSorties();
  initEntrees();
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.println("ready");
}


void loop()
{
  client = server.available();
  if (client) {
    //finder(client);  
    while (client.connected()) {      
      if (client.available()) {          
		httpRequestProcess();
		httpResponse();
		break;
      } 
    }
    delay(5);
    client.stop();
  }
}

