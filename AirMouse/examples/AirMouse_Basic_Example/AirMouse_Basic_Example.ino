// BasicExample.ino (Ajustado para as modificações da AirMouse)
#include "AirMouse.h"

AirMouse mouse;

void setup() {
  Serial.begin(115200);
  mouse.begin("ESP32 Air Mouse");
}

void loop() {
  if (mouse.isConnected()) {
    Serial.println("Mouse conectado!");

    // Exemplo de movimento do cursor
    mouse.move(10, 0); // Move 10 pixels para a direita
    delay(500);
    mouse.move(-10, 0); // Move 10 pixels para a esquerda
    delay(500);

    // Exemplo de clique normal (pressiona e solta automaticamente)
    mouse.click(0x01); // Clique com o botão esquerdo
    delay(1000);

    // Exemplo de arrastar um item:
    mouse.press(0x01); // Pressiona o botão esquerdo
    delay(1000);       // Mantém pressionado por 1 segundo (arrastando)
    mouse.release();   // Solta o botão esquerdo

    delay(2000); // Pausa antes do próximo loop
  }
}