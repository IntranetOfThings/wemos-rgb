#include <Homie.h>
#include <Adafruit_NeoPixel.h>

#define FW_NAME       "wemos-rgb"
#define FW_VERSION    "0.0.1"

/* Magic sequence for Autodetectable Binary Upload */
const char *__FLAGGED_FW_NAME = "\xbf\x84\xe4\x13\x54" FW_NAME "\x93\x44\x6b\xa7\x75";
const char *__FLAGGED_FW_VERSION = "\x6a\x3f\x3e\x0e\xe1" FW_VERSION "\xb0\x30\x48\xd4\x1a";
/* End of magic sequence for Autodetectable Binary Upload */

#define RGB_PIN       D2

// listen for publishes to led/rgb/set (in format "R,G,B")
HomieNode ledNode("led", "rgb");

// when we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, RGB_PIN, NEO_GRB + NEO_KHZ800);

bool rgbHandler(String value) {
  String r_str = getValue(value, ',', 0);
  String g_str = getValue(value, ',', 1);
  String b_str = getValue(value, ',', 2);

  int r = r_str.toInt();
  int g = g_str.toInt();
  int b = b_str.toInt();

  pixels.setPixelColor(0, pixels.Color(r, g, b));
  pixels.show();
  
  return true;
}

void setupHandler() {
  pixels.begin();
}

void loopHandler() {
}

void setup() {
  Homie.setFirmware(FW_NAME, FW_VERSION);

  // publish "R,G,B" to led/rgb/set
  ledNode.subscribe("rgb", rgbHandler);  

  Homie.registerNode(ledNode);
    
  Homie.setSetupFunction(setupHandler);
  Homie.setLoopFunction(loopHandler);

  Homie.setup();
}

void loop() {
  Homie.loop();
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
        found++;
        strIndex[0] = strIndex[1] + 1;
        strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
