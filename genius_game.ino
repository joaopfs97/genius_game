//int ledVermelho = 2;
#define LED_VERDE 2
#define LED_AMARELO 3
#define LED_VERMELHO 4
#define LED_AZUL 5
#define INDEFINIDO -1

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
  pinMode(LED_VERMELHO,OUTPUT);
  pinMode(LED_VERDE,OUTPUT);
  pinMode(LED_AMARELO,OUTPUT);
  pinMode(LED_AZUL,OUTPUT);

  pinMode(BOTAO_VERMELHO,INPUT_PULLUP);
  pinMode(BOTAO_VERDE,INPUT_PULLUP);
  pinMode(BOTAO_AMARELO,INPUT_PULLUP);
  pinMode(BOTAO_AZUL,INPUT_PULLUP);
}

void iniciaJogo() {
  int jogo = analogRead(0);
  randomSeed(jogo);
  
  for(int j = 0; j < TAMANHO_RODADA; j++){
  	listaLuzes[j] = sorteiaCor();  
  }
}  

int sorteiaCor(){
  return random(LED_VERDE,LED_AZUL + 1);
}

void loop() {
  switch(estadoAtual()){
  	case PRONTO_PARA_PROXIMA_RODADA:
    	Serial.println("Pronto para proxima rodada");
    	preparaNovaRodada();
    	break;
    case USUARIO_RESPONDENDO:
    	Serial.println("Usuario respondendo");
    	break;
 	case JOGO_FINALIZADO_SUCESSO:
    	Serial.println("Jogo finalizado com sucesso");
    	break;
 	case JOGO_FINALIZADO_FALHA:
    	Serial.println("Jogo finalizado com falha");
    	break;
  }  
  delay(MEIO_SEGUNDO);
}

void tocaRodadaAtual(){
  for(int i = 0;i < rodada;i++){
    piscaLed(listaLuzes[i]);
  }
}  

int estadoAtual(){
  if(rodada < TAMANHO_RODADA){
    return PRONTO_PARA_PROXIMA_RODADA;
  } else{
  		return JOGO_FINALIZADO_SUCESSO;
  	}
  return JOGO_FINALIZADO_FALHA;
}

void preparaNovaRodada(){
  rodada++;
  tocaRodadaAtual();
}

int checaRespostaJogador(){
  if(digitalRead(BOTAO_VERDE) == LOW){
    return piscaLed(LED_VERDE);
  }

  if(digitalRead(BOTAO_VERMELHO) == LOW){
    return piscaLed(LED_VERMELHO);
  }

  if(digitalRead(BOTAO_AMARELO) == LOW){
    return piscaLed(LED_AMARELO);
  }

  if(digitalRead(BOTAO_AZUL) == LOW){
    return piscaLed(LED_AZUL);
  }

  return INDEFINIDO;
}

int piscaLed(int portaLed){
  digitalWrite(portaLed, HIGH);
  delay(UM_SEGUNDO);
  digitalWrite(portaLed, LOW);
  delay(MEIO_SEGUNDO);
  return portaLed;
}