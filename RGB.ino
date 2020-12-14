#include <SPI.h>
#include <Ethernet.h>

# define NUM_LEDS 50

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xA2, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(xxx, xxx, xxx,xxx); // ip arduino
byte gateway[] = { xxx, xxx, xxx, xxx}; 
// the subnet:
byte subnet[] = { 255, 255, 255, 0 };
// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);
unsigned int localPort = 1234; // set the port you want

char packetBuffer[UDP_TX_PACKET_MAX_SIZE];                   // buffer to hold incoming packet
EthernetUDP Udp; 

int red = 6; /// add pin led red of your choice
int green = 7; /// add pin led green of your choice
int blue = 5;/// add pin blue red of your choice



char input[30];
char aa[32]; // make this at least big enough for the whole string
String myData,xval,yval,zval;
int r,b,g;

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = { 0, -1 };
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

 void mode(String modes){

if (modes.equals("strobe")) {
      
    for(int j = 0; j < 10; j++) {

      analogWrite(red, 200);
      analogWrite(green, 200);
      analogWrite(blue, 200);
      delay(50);
      analogWrite(red, 0);
      analogWrite(green, 0);
      analogWrite(blue, 0);
      delay(100);

    }

    }
    if (modes.equals("blinking")) {

    for(int j = 0; j < 10; j++) {

      analogWrite(red, 255);
      analogWrite(green, 0);
      analogWrite(blue, 0);
      delay(100);
      analogWrite(red, 0);
      analogWrite(green, 255);
      analogWrite(blue, 0);
      delay(100);
      analogWrite(red, 0);
      analogWrite(green, 0);
      analogWrite(blue, 255);
      delay(100);

    }

    }

    if (modes.equals("rain")) {
    rainbowCycle(15);
    }
    if (modes.equals("fadedin")) {
      for (int j = 0; j < 3; j++ ) {

        // Fade IN
        for (int k = 0; k < 256; k++) {
          switch (j) {
            case 0: analogWrite(red, k);  analogWrite(green, 0); analogWrite(blue, 0); break;
            case 1: analogWrite(red, 0);  analogWrite(green, k); analogWrite(blue, 0); break;
            case 2: analogWrite(red, 0);  analogWrite(green, 0); analogWrite(blue, k); break;
          }

          delay(3);
        }
      }
    }
    if (modes.equals("fadedout")) {
      for (int j = 0; j < 3; j++ ) {
        // Fade OUT
        for (int k = 255; k >= 0; k--) {
          switch (j) {
            case 0: analogWrite(red, k);  analogWrite(green, 0); analogWrite(blue, 0); break;
            case 1: analogWrite(red, 0);  analogWrite(green, k); analogWrite(blue, 0); break;
            case 2: analogWrite(red, 0);  analogWrite(green, 0); analogWrite(blue, k); break;
          }

          delay(3);
        }
      }
    }
  
  }

  void rainbowCycle(int SpeedDelay) {

  byte *c;
  uint16_t i, j;
  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< NUM_LEDS; i++) {
      c=Wheel(((i * 256 / NUM_LEDS) + j) & 255);
      analogWrite(red, *c);
      analogWrite(green, *(c+1));
      analogWrite(blue, *(c+2));
    }
    delay(SpeedDelay);
  }
}

byte * Wheel(byte WheelPos) {
  static byte c[3];
  if(WheelPos < 85) {
   c[0]=WheelPos * 3;
   c[1]=255 - WheelPos * 3;
   c[2]=0;
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   c[0]=255 - WheelPos * 3;
   c[1]=0;
   c[2]=WheelPos * 3;
  } else {
   WheelPos -= 170;
   c[0]=0;
   c[1]=WheelPos * 3;
   c[2]=255 - WheelPos * 3;
  }

  return c;

}

void setup() {
 
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  delay(10);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  digitalWrite(red, LOW);
  digitalWrite(green, LOW);
  digitalWrite(blue, LOW);

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
  Udp.begin(localPort); 
}


void loop() {
  
  int packetSize = Udp.parsePacket();                        // if there's data available, read a packet
  if(packetSize)
  {
  int read = Udp.read(packetBuffer,UDP_TX_PACKET_MAX_SIZE);  // read the packet into packetBufffer
    
    packetBuffer[read] = 0;
    /// make with the packet everythin you want the format of string is R: 154 G: 0 B: 157 if not set pins for color
    ///if set pins is coming in this format R(14):58 G(15):202 B(19)
    Serial.println(packetBuffer);
    if (getValue(packetBuffer, ':', 0) == "R")
    {
    xval = getValue(packetBuffer, ' ', 0);
    xval = getValue(xval, ':', 1);
    r = atoi(xval.c_str());
      if (r == 0) {
        digitalWrite(red, LOW);
      } else {
        analogWrite(red, r);
      }
    yval = getValue(packetBuffer, ' ', 1);
    yval = getValue(yval, ':', 1);
    g = atoi(yval.c_str());
      if (g == 0) {
        digitalWrite(green, LOW);
      } else {
        analogWrite(green, g);
      }
    zval = getValue(packetBuffer, ':', 3);
    b = atoi(zval.c_str());
      if (b == 0) {
        digitalWrite(blue, LOW);
      } else {
        analogWrite(blue, b);
      }
    Serial.println(xval);
    Serial.println(yval);
    Serial.println(zval);
    }else{
    mode(packetBuffer);
    }
    
  }
  
}
