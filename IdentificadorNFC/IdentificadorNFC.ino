#include <Arduino.h>
#include <U8x8lib.h>
#include <SPI.h>
#include <MFRC522.h>

#define redLed 7    // Definir os LEDs
#define greenLed 6

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);    

 
void setup() 
{
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  digitalWrite(redLed, HIGH);  // Led vermelho começa ligado
  digitalWrite(greenLed, LOW);  // Led verde começa desligado

  u8x8.begin();
  
  Serial.begin(9600);   // Inicia a serial
  SPI.begin();      // Inicia  SPI bus
  mfrc522.PCD_Init();   // Inicia MFRC522
  Serial.println("Aproxime o cartão do leitor");
  splashscr();
}
 
void loop() 
{
  
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    aguardarcartao();
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Mostra UID na serial
  Serial.print("UID da tag :");
  String conteudo= "";
  byte letra;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  conteudo.toUpperCase();

  if (conteudo.substring(1) == "4A F1 7F 39") //UID 1 - Renan TAG azul
  {
    Serial.println("Renan identificado");
    Serial.println();
    permitido("Renan Gomes");
    delay(50);
  }
  
  else if (conteudo.substring(1) == "E1 CE CA E3") //UID 2 - José BOM
  {
    Serial.println("José Identificado");
    Serial.println();
    permitido("José");
    delay(50);
  }

  else if (conteudo.substring(1) == "49 2A B9 CD") //UID 3 - NuBank
  {
    Serial.println("Nubank Identificado");
    Serial.println();
    permitido("NuBank");
    delay(50);
  }

  else 
  {
  negado();
  }
} 


/////////////////////////////////////////  Acesso permitido    ///////////////////////////////////
void permitido(char str[]) {
  digitalWrite(redLed, LOW); 
  digitalWrite(greenLed, HIGH);  
  u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
  u8x8.clearDisplay();
  u8x8.drawUTF8(0, 0, "ACESSO PERMITIDO");
  u8x8.drawUTF8(0, 3, "Seja bem-vindo");
  u8x8.drawUTF8(0, 5, str);
  delay(3000);            // Mantém o LED verde aceso
  u8x8.clearDisplay();
}

///////////////////////////////////////// Acesso negado  ///////////////////////////////////
void negado() {
  digitalWrite(greenLed, LOW);  
  digitalWrite(redLed, HIGH);   
  u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
  u8x8.clearDisplay();
  u8x8.drawUTF8(0, 0, "ACESSO NEGADO");
  delay(1000);
  u8x8.clearDisplay();
}

///////////////////////////////////////// Aguardar cartão  ///////////////////////////////////
void aguardarcartao()
{
  digitalWrite(greenLed, LOW);  
  digitalWrite(redLed, HIGH);  
  u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
//  u8x8.clearDisplay();
  u8x8.drawUTF8(0, 0, "Aproxime o");
  u8x8.drawUTF8(0, 2, "cartão");
  
}

///////////////////////////////////////// Splash Screen  ///////////////////////////////////
void splashscr()
{
  u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
  u8x8.drawUTF8(0, 0, "Renan Gomes");
  delay(500);
  u8x8.clearDisplay();
}
