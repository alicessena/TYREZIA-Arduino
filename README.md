# 👓 TYREZIA - Óculos Guia Sonar

## ✨ Descrição do Projeto
O **EchoSense** é um projeto de eletrônica e programação desenvolvido como trabalho de conclusão de curso / projeto de faculdade.  
O objetivo é criar um dispositivo de **baixo custo** para auxiliar pessoas com deficiência visual na **detecção de obstáculos em tempo real**, fornecendo **feedback auditivo direcional**.

O sistema utiliza **dois sensores ultrassônicos** (esquerdo e direito) para medir a distância de objetos.  
Essas informações são convertidas em alertas sonoros por meio de **buzzers estéreo** (um para cada lado), indicando a proximidade e direção do obstáculo.

---

## 🔍 Funcionalidades Principais
- **Detecção Direcional:**  
  Cada lado emite seu próprio alerta (obstáculo à esquerda → buzzer esquerdo).
- **Feedback Proporcional:**  
  Quanto mais próximo o obstáculo, mais rápido o beep.
- **Ajuste de Frequência:**  
  Um potenciômetro controla o tom base do alerta sonoro (Hz).

---

## 🛠️ Componentes Necessários

| Componente                     | Quantidade | Observações                                      |
|-------------------------------|-----------|--------------------------------------------------|
| Arduino Uno (ou similar)      | 1         | Microcontrolador principal                      |
| Sensor Ultrassônico HC-SR04   | 2         | Um para o lado esquerdo e outro para o direito  |
| Buzzer Ativo/Passivo          | 2         | Emitirão os alertas estéreo                     |
| Potenciômetro (10kΩ)          | 1         | Controle de frequência sonora                   |
| Fios Jumper                   | Vários    | Para conexões                                   |

---

## 🔌 Diagrama de Conexão (Pinagem do Arduino)

| Componente                           | Pino Arduino | Constante no Código | Tipo             |
|--------------------------------------|--------------|----------------------|------------------|
| Sensor Ultrassônico ESQ (TRIG)       | D4           | `TRIG_ESQ`           | Saída Digital    |
| Sensor Ultrassônico ESQ (ECHO)       | D3           | `ECHO_ESQ`           | Entrada Digital  |
| Sensor Ultrassônico DIR (TRIG)       | D6           | `TRIG_DIR`           | Saída Digital    |
| Sensor Ultrassônico DIR (ECHO)       | D5           | `ECHO_DIR`           | Entrada Digital  |
| Buzzer DIREITO                       | D13          | `BUZZER_DIR`         | Saída Digital    |
| Buzzer ESQUERDO                      | D12          | `BUZZER_ESQ`         | Saída Digital    |
| Potenciômetro (pino central)         | A5           | `PINO_POT`           | Entrada Analógica |

---

## ⚙️ Código (Sketch Arduino)

```cpp
// --- Definição das Portas ---
#define TRIG_ESQ 4     // Trigger do Sensor Ultrassônico Esquerdo
#define ECHO_ESQ 3     // Echo do Sensor Ultrassônico Esquerdo
#define TRIG_DIR 6     // Trigger do Sensor Ultrassônico Direito
#define ECHO_DIR 5     // Echo do Sensor Ultrassônico Direito
#define BUZZER_DIR 13  // Buzzer do Lado Direito
#define BUZZER_ESQ 12  // Buzzer do Lado Esquerdo
#define PINO_POT A5    // Potenciômetro para controle de frequência

// --- Variável Global para Frequência ---
int frequenciaBase = 0; 

// --- Função: Ler Distância (Ultrassônico) ---
int lerDistancia(int pinoTrig, int pinoEcho) { 
  digitalWrite(pinoTrig, LOW); 
  delayMicroseconds(2);
  digitalWrite(pinoTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinoTrig, LOW);

  long duration = pulseIn(pinoEcho, HIGH);
  int distance = duration * 0.034 / 2; 
  return distance;
}

// --- Função: Emitir Som (Buzzer) ---
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

// --- Configuração Inicial (SETUP) ---
void setup() {
  Serial.begin(9600);

  pinMode(TRIG_ESQ, OUTPUT); pinMode(ECHO_ESQ, INPUT); 
  pinMode(TRIG_DIR, OUTPUT); pinMode(ECHO_DIR, INPUT);
  pinMode(BUZZER_DIR, OUTPUT); pinMode(BUZZER_ESQ, OUTPUT);

  Serial.println("Sistema de Deteccao de Obstaculos Iniciado. Ajuste o Potenciometro (A5).");
  Serial.println("-----------------------------------------------------------------------");
  Serial.println("  Distância Esq. (cm) | Distância Dir. (cm) | Frequência Base (Hz)  ");
  Serial.println("-----------------------------------------------------------------------");
}

// --- Loop Principal (LOOP) ---
void loop() {

  int leituraPot = analogRead(PINO_POT); 
  frequenciaBase = map(leituraPot, 0, 1023, 300, 4000); 

  int distanciaEsq = lerDistancia(TRIG_ESQ, ECHO_ESQ); 
  int distanciaDir = lerDistancia(TRIG_DIR, ECHO_DIR);

  Serial.print("      "); Serial.print(distanciaEsq);
  Serial.print("              |");
  Serial.print("      "); Serial.print(distanciaDir);
  Serial.print("              |");
  Serial.print("        "); Serial.println(frequenciaBase);

  emitirSom(distanciaEsq, BUZZER_ESQ);
  emitirSom(distanciaDir, BUZZER_DIR);
}
