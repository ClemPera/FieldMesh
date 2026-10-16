#pragma once

#include <MeshCore.h>
#include <helpers/ui/DisplayDriver.h>
#include <helpers/ui/UIScreen.h>
#include <helpers/SensorManager.h>
#include <helpers/MultiSerialInterface.h>
#include <Arduino.h>

#ifdef PIN_BUZZER
  #include <helpers/ui/buzzer.h>
#endif

#include "NodePrefs.h"
#include "MyMesh.h"

enum class UIEventType {
    none,
    contactMessage,
    channelMessage,
    roomMessage,
    newContactMessage,
    ack
};

class AbstractUITask : public MyMesh::Listener {
protected:
  mesh::MainBoard* _board;
  MultiSerialInterface* _interfaceManager;

  AbstractUITask(mesh::MainBoard* board, MultiSerialInterface* interfaceManager) : _board(board), _interfaceManager(interfaceManager) {
  }

public:
  bool hasConnection() const { return _interfaceManager->isConnected(); }
  uint16_t getBattMilliVolts() const { return _board->getBattMilliVolts(); }
  // NOTE (merge v1.18.0): serial on/off + msgRead/newMsg moved to MyMesh::Listener
  // (default no-op there); the companion UITask still overrides newMsg/msgRead.
  bool isBluetoothEnabled() const { return _interfaceManager->isBluetoothEnabled(); }
  void enableBluetooth() { _interfaceManager->enableBluetooth(); }
  void disableBluetooth() { _interfaceManager->disableBluetooth(); }

  virtual void notify(UIEventType t = UIEventType::none) = 0;
  virtual void loop() = 0;
  virtual void triggerSOS(const char* from, const char* text) {}  // V3: default no-op (no display)
  virtual void refreshDisplay() {}                                 // Advert refresh without backlight change
  virtual bool isOnRecentOrTrackingPage() const { return false; } // Used for advert refresh check
  virtual void updateOnlineNode(const char* name, uint32_t timestamp) {}  // V5: default no-op
};
