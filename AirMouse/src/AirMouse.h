// AirMouse.h
#ifndef AIR_MOUSE_H
#define AIR_MOUSE_H

#include <BLEDevice.h>
#include <BLEHIDDevice.h>
#include <BLEUtils.h>
#include <Arduino.h>

class AirMouse {
  public:
    AirMouse();
    
    // Inicializa o BLE e o serviço HID (com nome opcional do dispositivo)
    void begin(const char* deviceName = "ESP32 Air Mouse");

    // Move o ponteiro do mouse de forma relativa (deltaX e deltaY)
    void move(int8_t deltaX, int8_t deltaY);

    // Simula o pressionamento de botão(s) (máscara de bits: 0x01 = esquerdo, 0x02 = direito)
    void press(uint8_t buttons);

    // Libera os botões (nenhum pressionado)
    void release();

    // Realiza um clique: apenas chama press() seguido de release()
    void click(uint8_t buttons);

    void SendControl(uint8_t buttons, int8_t deltaX, int8_t deltaY);

    // Retorna se o dispositivo BLE está conectado
    bool isConnected();

  private:
    BLEHIDDevice* hid;
    BLECharacteristic* inputReport;
    bool connected;

    // Envia um relatório HID com os parâmetros (botões e deslocamento X, Y)
    void sendReport(uint8_t buttons, int8_t x, int8_t y);
};

#endif
