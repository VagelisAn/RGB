#include <SPI.h>
#include <Ethernet.h>

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

void setup() {
 
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }


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
    Serial.println(read);
    packetBuffer[read] = 0;
    /// make with the packet everythin you want the format of string is R: 154 G: 0 B: 157 if not set pins for color
    ///if set pins is coming in this format R(14):58 G(15):202 B(19)


    Serial.println(packetBuffer);
  }
  
} 
