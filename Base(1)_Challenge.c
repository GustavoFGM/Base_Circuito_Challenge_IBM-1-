// Codigo base para callenge da IBM prototio inicial
//funcionalidades:
//estabele uma tela de lcd 16*2 para mostar algumas informaçoes basica
//que sepode ser usado como base num prototipo em escala do jeito que esta(basico)
//estabele um servo(representando a movimentação da placa de energia solar)
//que se move durante o dia 15° a cada hora do dia para maior eficiencia


#include <LiquidCrystal.h>   // Inclui a biblioteca LiquidCrystal para controle do display LCD
#include <Servo.h>           // Inclui a biblioteca Servo para controle do servo motor

#define LCD_PIN_REGSELECT 12 // Define o pino de seleção do registrador do display LCD como 12
#define LCD_PIN_ENABLE 11    // Define o pino de habilitação do display LCD como 11
#define LCD_PIN_DB1 7        // Define o pino DB1 do display LCD como 7
#define LCD_PIN_DB2 6        // Define o pino DB2 do display LCD como 6
#define LCD_PIN_DB3 5        // Define o pino DB3 do display LCD como 5
#define LCD_PIN_DB4 4        // Define o pino DB4 do display LCD como 4
#define LCD_WIDTH 16         // Define a largura do display LCD como 16 caracteres
#define LCD_HEIGHT 2         // Define a altura do display LCD como 2 linhas

#define TEMPERATURE_CONTROLLER_PIN A0 // Define o pino de leitura do sensor de temperatura como A0
#define BRIGHTNESS_CONTROLLER_PIN A1  // Define o pino de leitura do sensor de luminosidade como A1
#define SERVO_PIN 9                   // Define o pino de controle do servo motor como 9
#define SERVO_ANGLE_INCREMENT 15      // Define o incremento de ângulo do servo motor como 15 graus
#define SERVO_MOVE_INTERVAL 60000     // Define o intervalo de tempo para movimentação do servo motor como 1 minuto (60000 ms)
#define MIN_BRIGHTNESS 450            // Define o valor mínimo de leitura do sensor de luminosidade como 450
#define MAX_BRIGHTNESS 550            // Define o valor máximo de leitura do sensor de luminosidade como 550

Servo servo1;  

  // Declaração do objeto servo1 da classe Servo

byte custom_degree_char[] =
  
  // Definição do caractere customizado para o símbolo de grau Celsius
{
  B00110, B01001,
  B01001, B00110,
  B00000, B00000,
  B00000, B00000
};

LiquidCrystal lcd(LCD_PIN_REGSELECT, LCD_PIN_ENABLE, LCD_PIN_DB1, LCD_PIN_DB2, LCD_PIN_DB3, LCD_PIN_DB4); // Declaração do objeto lcd da classe LiquidCrystal

unsigned long last_servo_move_time = 0;  
// Variável que armazena o tempo da última movimentação do servo motor

void setup() {
  Serial.begin(9600); 
  
  // Inicia a comunicação serial a uma taxa de 9600 bps

  servo1.attach(SERVO_PIN);          
  
  // Anexa o objeto servo1 ao pino de controle do servo motor

  lcd.begin(LCD_WIDTH, LCD_HEIGHT);      
  
  // Inicializa o objeto lcd com a largura e altura do display LCD
  
  lcd.createChar(0, custom_degree_char);
  
  // Cria o caractere customizado do símbolo de grau Celsius

  pinMode(TEMPERATURE_CONTROLLER_PIN, INPUT);
  
  // Define o pino do sensor de temperatura como entrada
  
  pinMode(BRIGHTNESS_CONTROLLER_PIN, INPUT);  
  
  // Define o pino do sensor de luminosidade como entrada
  
}

// Loop principal

void loop() {
//Lê a tensão do controlador de temperatura e converte para grau celsius e farenheit
  
  float temperature_voltage = analogRead(TEMPERATURE_CONTROLLER_PIN) * 5.0f / 1024; // Lê a tensão do sensor de temperatura e converte para Volts
  float temperature_C = (temperature_voltage - 0.5f) * 100;
  float temperature_F = temperature_C * 9.0f / 5.0f + 32.0f;
  
//Imprime a temperatura no lcd
  
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature_C);
  lcd.write(byte(0));
  
// le a tensão do controlador de brilho
  
  int current_brightness = analogRead(BRIGHTNESS_CONTROLLER_PIN);

//Move o servo a cada SERVO_MOVE_INTERVAL milissegundos
  
  if (millis() - last_servo_move_time >= SERVO_MOVE_INTERVAL) {
    last_servo_move_time = millis();
    
//Atualiza o último movimento do serve
    
    static int servo_angle = 0;
    servo_angle += SERVO_ANGLE_INCREMENT;
    if (servo_angle > 180) {
      servo_angle = 0;
    }
    servo1.write(servo_angle);
  }

  lcd.setCursor(0, 1);
  lcd.print("Light: ");
  lcd.print(current_brightness);

  delay(100); // Aguarda 100ms antes de atualizar novamente
}
