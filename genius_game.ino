#include "pitches.h"

#define LED_VERDE 2
#define LED_AMARELO 3
#define LED_VERMELHO 4
#define LED_AZUL 5
#define INDEFINIDO -1
#define BUZZER 13

#define BOTAO_VERDE 8
#define BOTAO_AMARELO 9
#define BOTAO_VERMELHO 10
#define BOTAO_AZUL 11

#define UM_SEGUNDO 1000
#define MEIO_SEGUNDO 500
#define TRES_SEGUNDOS 3000
#define TAMANHO_RODADA 4
int listaLuzes[TAMANHO_RODADA];
int rodada = 0;
int ledsRespondidos = 0;

enum Estados {
  PRONTO_PARA_PROXIMA_RODADA,
  USUARIO_RESPONDENDO,
  JOGO_FINALIZADO_SUCESSO,
  JOGO_FINALIZADO_FALHA
};

void setup() {
  iniciaPortas();
  Serial.begin(9600);
  iniciaJogo();
}

void iniciaPortas() {
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AMARELO, OUTPUT);
  pinMode(LED_AZUL, OUTPUT);

  pinMode(BOTAO_VERMELHO, INPUT_PULLUP);
  pinMode(BOTAO_VERDE, INPUT_PULLUP);
  pinMode(BOTAO_AMARELO, INPUT_PULLUP);
  pinMode(BOTAO_AZUL, INPUT_PULLUP);

  pinMode(BUZZER, OUTPUT);
}

void iniciaJogo() {
  int jogo = analogRead(0);
  randomSeed(jogo);

  for (int j = 0; j < TAMANHO_RODADA; j++) {
    listaLuzes[j] = sorteiaCor();
  }
}

int sorteiaCor() {
  return random(LED_VERDE, LED_AZUL + 1);
}

void loop() {
  switch (estadoAtual()) {
    case PRONTO_PARA_PROXIMA_RODADA:
      Serial.println("Pronto para proxima rodada");
      preparaNovaRodada();
      break;
    case USUARIO_RESPONDENDO:
      Serial.println("Usuario respondendo");
      processaRespostaUsuario();
      break;
    case JOGO_FINALIZADO_SUCESSO:
      Serial.println("Jogo finalizado com sucesso");
      piscaLedSucesso();
      tocaFimDeJogoSucesso();
      for (;;)
        ;
      break;
    case JOGO_FINALIZADO_FALHA:
      Serial.println("Jogo finalizado com falha");
      piscaLedFalha();
      piscaLedFalha();
      piscaLedFalha();
      for (;;)
        ;
      break;
  }
  delay(MEIO_SEGUNDO);
}

void tocaRodadaAtual() {
  for (int i = 0; i < rodada; i++) {
    piscaLed(listaLuzes[i]);
  }
}

int estadoAtual() {
  if (rodada <= TAMANHO_RODADA) {
    if (rodada == ledsRespondidos) {
      return PRONTO_PARA_PROXIMA_RODADA;
    } else {
      return USUARIO_RESPONDENDO;
    }
  } else if (rodada == TAMANHO_RODADA + 1) {
    return JOGO_FINALIZADO_SUCESSO;
  } else {
    return JOGO_FINALIZADO_FALHA;
  }
}

void preparaNovaRodada() {
  rodada++;
  ledsRespondidos = 0;
  if (rodada <= TAMANHO_RODADA) {
    tocaRodadaAtual();
  }
}

void processaRespostaUsuario() {
  int resposta = checaRespostaJogador();

  if (resposta == INDEFINIDO) {
    return;
  }

  if (resposta == listaLuzes[ledsRespondidos]) {
    ledsRespondidos++;
  } else {
    rodada = TAMANHO_RODADA + 2;
  }
}

int checaRespostaJogador() {
  if (digitalRead(BOTAO_VERDE) == LOW) {
    return piscaLed(LED_VERDE);
  }

  if (digitalRead(BOTAO_VERMELHO) == LOW) {
    return piscaLed(LED_VERMELHO);
  }

  if (digitalRead(BOTAO_AMARELO) == LOW) {
    return piscaLed(LED_AMARELO);
  }

  if (digitalRead(BOTAO_AZUL) == LOW) {
    return piscaLed(LED_AZUL);
  }

  return INDEFINIDO;
}

int piscaLed(int portaLed) {
  verificaSomLed(portaLed);

  digitalWrite(portaLed, HIGH);
  delay(UM_SEGUNDO);
  digitalWrite(portaLed, LOW);
  delay(MEIO_SEGUNDO);
  return portaLed;
}

void piscaLedSucesso() {
  for (int i = 0; i < TAMANHO_RODADA; i++) {
    piscaLed(listaLuzes[i]);
  }
  piscaTodasAsLuzes();
}

void piscaLedFalha() {
  piscaTodasAsLuzes();
}

void piscaTodasAsLuzes() {
  delay(MEIO_SEGUNDO);
  digitalWrite(LED_VERMELHO, HIGH);
  digitalWrite(LED_VERDE, HIGH);
  digitalWrite(LED_AMARELO, HIGH);
  digitalWrite(LED_AZUL, HIGH);
  delay(UM_SEGUNDO);
  digitalWrite(LED_VERMELHO, LOW);
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_AMARELO, LOW);
  digitalWrite(LED_AZUL, LOW);
}

void tocaSom(int frequencia) {
  tone(BUZZER, frequencia, 100);
}

void verificaSomLed(int portaLed) {
  switch (portaLed) {
    case LED_VERDE:
      tocaSom(200);
      break;
    case LED_VERMELHO:
      tocaSom(220);
      break;
    case LED_AMARELO:
      tocaSom(240);
      break;
    case LED_AZUL:
      tocaSom(260);
      break;
  }
}

void tocaFimDeJogoSucesso() {
  int melody[] = { NOTE_G5, NOTE_FS5, NOTE_DS5, NOTE_G4, NOTE_FS4, NOTE_E5, NOTE_GS5, NOTE_C6 };
  int noteDurations[] = { // Note Durations (4 = 1/4 Note, 8 = 1/8 Note, etc...)
                          6, 6, 6, 6, 6, 6, 6, 2
  };
  for (int thisNote = 0; thisNote < 8; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(13, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(13);
  }
}