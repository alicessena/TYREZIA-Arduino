#define TRIG_ESQ 4
#define ECHO_ESQ 3
#define TRIG_DIR 6
#define ECHO_DIR 5
#define BUZZER_DIR 13
#define BUZZER_ESQ 12
#define PINO_POT A5

int frequenciaBase = 0; 

int lerDistancia(int pinoTrig, int pinoEcho, String nomeSensor) {
  digitalWrite(pinoTrig, LOW); 
  delayMicroseconds(2);
  digitalWrite(pinoTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinoTrig, LOW);

  long duration = pulseIn(pinoEcho, HIGH);
  
  int distance = duration * 0.034 / 2; 

  Serial.print(nomeSensor);
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm | Freq: ");
  Serial.print(frequenciaBase);
  Serial.println(" Hz"); 
  
  return distance;
}

void emitirSom(int distancia, int pinoBuzzer) {
  
  if (distancia < 100 && distancia > 0) {  
    tone(pinoBuzzer, frequenciaBase); 
    
    delay(50);
    
    noTone(pinoBuzzer);
    
    delay(distancia * 2);
  } else {
    noTone(pinoBuzzer);
  }
}

void setup() {
  Serial.begin(9600);
  
  pinMode(TRIG_ESQ, OUTPUT); 
  pinMode(ECHO_ESQ, INPUT); 
  
  pinMode(TRIG_DIR, OUTPUT);
  pinMode(ECHO_DIR, INPUT);
  
  pinMode(BUZZER_DIR, OUTPUT);
  pinMode(BUZZER_ESQ, OUTPUT);
  
  
  Serial.println("Sistema Iniciado. Ajuste o Potenciometro (A5).");
}

void loop() {
  int leituraPot = analogRead(PINO_POT); 
  
  frequenciaBase = map(leituraPot, 0, 1023, 300, 4000); 

  int distanciaEsq = lerDistancia(TRIG_ESQ, ECHO_ESQ, "Esq: ");
  int distanciaDir = lerDistancia(TRIG_DIR, ECHO_DIR, "Dir: ");
  
  emitirSom(distanciaEsq, BUZZER_ESQ);
  emitirSom(distanciaDir, BUZZER_DIR);
}