// AirMouse.cpp
#include "AirMouse.h"

// Callback para atualizar o estado da conexão BLE e reiniciar a publicidade
class MouseCallbacks : public BLEServerCallbacks {
  public:
    MouseCallbacks(bool* connectedFlag, BLEAdvertising* adv) : connected(connectedFlag), advertising(adv) {}
    bool* connected;
    BLEAdvertising* advertising;

    void onConnect(BLEServer* server) override {
      *connected = true;
    }

    void onDisconnect(BLEServer* server) override {
      *connected = false;
      delay(500); // Pequeno atraso para estabilidade
      advertising->start(); // Reinicia a publicidade para reconectar
    }
};

AirMouse::AirMouse() : hid(nullptr), inputReport(nullptr), connected(false) {}

void AirMouse::begin(const char* deviceName) {
  BLEDevice::deinit();  // Reseta o BLE para evitar problemas de cache
  delay(500);           // Pequeno atraso para estabilidade

  BLEDevice::init(deviceName);
  BLEServer* server = BLEDevice::createServer();
  BLEAdvertising* advertising = server->getAdvertising();
  server->setCallbacks(new MouseCallbacks(&connected, advertising));

  hid = new BLEHIDDevice(server);
  inputReport = hid->inputReport(0);

  hid->manufacturer()->setValue("ESP32 HID Mouse");
  hid->pnp(0x02, 0xe502, 0xa111, 0x0210);
  hid->hidInfo(0x00, 0x01);

  const uint8_t reportMap[] = {
      0x05, 0x01, 0x09, 0x02, 0xA1, 0x01, 0x09, 0x01, 0xA1, 0x00,
      0x05, 0x09, 0x19, 0x01, 0x29, 0x03, 0x15, 0x00, 0x25, 0x01,
      0x95, 0x03, 0x75, 0x01, 0x81, 0x02, 0x95, 0x01, 0x75, 0x05,
      0x81, 0x03, 0x05, 0x01, 0x09, 0x30, 0x09, 0x31, 0x15, 0x81,
      0x25, 0x7F, 0x75, 0x08, 0x95, 0x02, 0x81, 0x06, 0xC0, 0xC0
  };

  hid->reportMap((uint8_t*)reportMap, sizeof(reportMap));
  hid->startServices();

  advertising->addServiceUUID(hid->hidService()->getUUID());
  advertising->setScanResponse(false);
  advertising->start(); // Garante que a publicidade recomece corretamente

  hid->setBatteryLevel(100);
}


void AirMouse::sendReport(uint8_t buttons, int8_t x, int8_t y) {
  if (!connected) return;
  uint8_t report[3] = {buttons, (uint8_t)x, (uint8_t)y};
  inputReport->setValue(report, sizeof(report));
  inputReport->notify();
}

void AirMouse::move(int8_t deltaX, int8_t deltaY) {
  sendReport(0x00, deltaX, deltaY);
}

void AirMouse::press(uint8_t buttons) {
  sendReport(buttons, 0, 0);
}

void AirMouse::release() {
  sendReport(0x00, 0, 0);
}

void AirMouse::click(uint8_t buttons) {
  press(buttons);
  release();
}

bool AirMouse::isConnected() {
  return connected;
}
