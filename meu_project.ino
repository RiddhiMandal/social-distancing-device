#include <BLEUtils.h>
#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BLEServer.h>
int buzzer = 4 ;
int LED = 2 ;
#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
int scanTime = 1;     // In seconds
const int CUTOFF = -82;
bool deviceConnected = false;
bool oldDeviceConnected = false;
BLEScan *pBLEScan;
BLEDevice bledevice;
BLEAdvertisedDevice advertisedDevice;
class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{
  void onResult(BLEAdvertisedDevice advertisedDevice)
  {
    if (advertisedDevice.getName() == "MEU")
    {
      
      int ble_rssi = advertisedDevice.getRSSI();
      Serial.println("RSSI: " + String(ble_rssi>CUTOFF));
        Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
       digitalWrite(buzzer,ble_rssi>CUTOFF? HIGH:LOW);
        digitalWrite(LED,ble_rssi>CUTOFF? HIGH:LOW);
       
      }
      else{
        digitalWrite(buzzer, LOW);
          digitalWrite(LED, LOW);
        
         }
  }
};

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  BLEDevice::init("MEU");
  
  pBLEScan = BLEDevice::getScan(); //create new scan
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ |
          BLECharacteristic::PROPERTY_WRITE);

  pCharacteristic->setValue("MEU");
  pService->start();
  
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  BLEDevice::startAdvertising();
 BLEScan* pBLEScan = BLEDevice::getScan();
  Serial.println("Characteristic defined!");
  Serial.println("Scanning...");
  pinMode(buzzer, OUTPUT); // set pin to output mode
  BLEDevice::init("MEU");
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99); // less or equal setInterval value
  
 
}
void loop()
{
  
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
   //BLEScanResults scanResults = pBLEScan->start(0);
  BLEScan *scan = BLEDevice::getScan();
  scan->setActiveScan(true);
  BLEScanResults results = scan->start(1);
  pBLEScan->clearResults();
  int best = CUTOFF;
  for (int i = 0; i < foundDevices.getCount(); i++) {
    BLEAdvertisedDevice advertiseddevice = results.getDevice(i);
    int ble_rssi = advertiseddevice.getRSSI();
    if (ble_rssi > best) {
      best = ble_rssi;
      
    }
}
//pBLEScan->clearResults();
}
 
