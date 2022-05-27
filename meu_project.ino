#include <BLEUtils.h>
#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BLEServer.h>
int buzzer = 4 ;
int LED = 2 ;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

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

/*Create a BLE server that, once we receive a connection, will send periodic notifications.
   The service advertises itself as: 
   And has a characteristic of: 
   The design of creating the BLE server is:
   1. Create a BLE Server
   2. Create a BLE Service
   3. Create a BLE Characteristic on the Service
   4. Create a BLE Descriptor on the characteristic
   5. Start the service.
   6. Start advertising.
   A connect hander associated with the server starts a background task that performs notification
   every couple of seconds.*/
     
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
  pService->start();   // restart advertising
  
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising(); //Called when a new scan result is detected
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  BLEDevice::startAdvertising();
 BLEScan* pBLEScan = BLEDevice::getScan();
  Serial.println("Characteristic defined!");
  Serial.println("Scanning...");
  pinMode(buzzer, OUTPUT); // set pin to output mode
  BLEDevice::init("MEU");
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());  //Called when a new scan result is detected.
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
 
