/*
  caja = MIC1 = A2 == red;
  bombo = MIC2 = A4 == green;
  izquierda = MIC3 = A0 == pink;
  derecha = MIC4 = A1 == blue;
*/

#include <Adafruit_NeoPixel.h>

#define PIN 2 // pin de la tira de leds

#define NUMPIXELS 37 // cantidad de leds que tiene la tira

int estado = 0;

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

const int cantidadMics = 4; // cantidad de piezoeléctricos
const int pinMics[] = {A2, A4, A0, A1}; // pines de los piezos
int datosMics[cantidadMics];
int i;

// deficinición de colores en RGB
uint32_t red = pixels.Color(255, 0, 0);
uint32_t green = pixels.Color(0, 255, 0);
uint32_t pink = pixels.Color(255, 51, 221);
uint32_t blue = pixels.Color (6, 241, 249);

int note;
int velocity = 100;
int noteON = 144;
int noteOFF = 128;

const int delayOn = 100; 

int sensibilidadMic1 = 10;
int sensibilidadMic2 = 10;
int sensibilidadMic3 = 10;
int sensibilidadMic4 = 10;

// valor threshold de cada piezo
int pico1 = 300; //caja green
int pico2 = 55; //bombo red
int pico3 = 70; //mano izquierda pink
int pico4 = 170; //mano derecha blue

bool send1 = true;

unsigned long previousMillis;
unsigned long currentMillis;
const unsigned long intervalo = 1000;

int counter = 0;

void setup() {
  //Serial.begin(115200);

  for (int i = 0; i < cantidadMics; i++) {
    pinMode(pinMics[i], INPUT);
  }

  previousMillis = millis();

  pixels.begin();
  pixels.clear();
  pixels.show();
}

void loop() {

//imprimirDatos();

  leerPiezos();

}

void leerPiezos() {

  for (int i = 0; i < cantidadMics; i++) {
    pinMode(pinMics[i], INPUT);
    datosMics[i] = analogRead(pinMics[i]);
  }

  if (datosMics[0] > sensibilidadMic1 ||
      datosMics[1] > sensibilidadMic2 ||
      datosMics[2] > sensibilidadMic3 ||
      datosMics[3] > sensibilidadMic4 ) {

    disparo();
  }
}

void disparo() {
  
  for (int i = 0; i < cantidadMics; i++) {
    pinMode(pinMics[i], INPUT);
  }
  
   //imprimirDatos();

  if (datosMics[0] > datosMics[1] &&
      datosMics[0] > datosMics[2] &&
      datosMics[0] > datosMics[3]) {

    if (datosMics[0] > pico1) {
      mic1();
      frenarSensado();
    }
    estado = 1;
  }

  if (datosMics[1] > datosMics[0] &&
      datosMics[1] > datosMics[2] &&
      datosMics[1] > datosMics[3]) {

    if (datosMics[1] > pico2) {
      mic2();
      frenarSensado();
    }
    estado = 1;
  }

  if (datosMics[2] > datosMics[0] &&
      datosMics[2] > datosMics[1] &&
      datosMics[2] > datosMics[3]) {

    if (datosMics[2] > pico3) {
      mic3();
      frenarSensado();
    }
    estado = 1;
  }

  if (datosMics[3] > datosMics[0] &&
      datosMics[3] > datosMics[1] &&
      datosMics[3] > datosMics[2]) {

    if (datosMics[3] > pico4) {
      mic4();
      frenarSensado();
    }
    estado = 1;
  }

  switch (estado) {
    case 0:
      leerPiezos();
      break;

    case 1:
      frenarSensado();
      estado = 0;
      break;
  }
  

}

void mic1() {

  note = 38; // KICK ABLETON
  MIDImessage(noteON, note, velocity);

  pixels.fill(green);
  pixels.show();

  delay(delayOn);

  pixels.clear();
  pixels.show();

  MIDImessage(noteOFF, note, velocity);
}

void mic2() {

  note = 36; // SNARE ABLETON
  MIDImessage(noteON, note, velocity);

  pixels.fill(red);
  pixels.show();

  delay(delayOn);

  pixels.clear();
  pixels.show();

  MIDImessage(noteOFF, note, velocity);
}

void mic3() {

  note = 40; // HI HAT CLOSED
  MIDImessage(noteON, note, velocity);

  pixels.fill(pink);
  pixels.show();

  delay(delayOn);

  pixels.clear();
  pixels.show();

  MIDImessage(noteOFF, note, velocity);
}

void mic4() {

  note = 42; // CLAVE 808
  MIDImessage(noteON, note, velocity);

  pixels.fill(blue);
  pixels.show();

  delay(delayOn);

  pixels.clear();
  pixels.show();

  MIDImessage(noteOFF, note, velocity);
}

void frenarSensado() {
  for (int i = 0; i < cantidadMics; i++) {
    pinMode(pinMics[i], OUTPUT);
  }
}

void imprimirDatos() {
  Serial.print(" 1:");
  Serial.print(datosMics[0]);
  Serial.print(" 2:");
  Serial.print(datosMics[1]);
  Serial.print(" 3:");
  Serial.print(datosMics[2]);
  Serial.print(" 4:");
  Serial.println(datosMics[3]);
}

void MIDImessage(int command, int MIDInote, int MIDIvelocity) {
  Serial.write(command);
  Serial.write(MIDInote);
  Serial.write(MIDIvelocity);
}
