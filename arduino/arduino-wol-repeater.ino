/**
 * Arduino Wake on LAN repeater
 * + IP change notifier
 * © www.finalclap.com
 */
 
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
 
byte arduinoMAC[] = { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 }; // MAC address (read it on the Arduino ethernet shield sticker)
IPAddress arduinoIP(192, 168, 1, 200);                      // Arduino local IP address (LAN)
unsigned int arduinoPort = 9;                               // Port listened by the Arduino (your router must forward UDP 9 or UDP 7 here to root magic packets to the Arduino)
IPAddress broadcastIP(192, 168, 1, 255);                    // LAN broadcast IP address
 
// Buffer
#define MAGIC_PACKET_SIZE 102
byte packetBuffer[MAGIC_PACKET_SIZE];
 
EthernetUDP udp;          // Used to manage UDP packets
EthernetClient tcpClient; // TCP client used to call logmyip.php
 
// IP change log server settings
char notifierHostname[] = "yourwebsite.com";
char nofifierPath[]     = "/webserver/logmyip.php";
int notifierPeriod      = 600; // Loop delay = 100ms, server notification period : 60s => 60/0.1 = 600
int loopCount           = 0;
 
void setup() {
  // Connecting to local network (router)
  Ethernet.begin(arduinoMAC,arduinoIP);
 
  // Listening for magic packets forwarded by the router
  udp.begin(arduinoPort);
}
 
void loop() {
  // Job 1
  forward_wol_magic_packet();
 
  // Job 2
  if( loopCount >= notifierPeriod ){
    ip_change_notify(notifierHostname, nofifierPath);
    loopCount = 0;
  }
 
  loopCount++;
  delay(100);
}
 
/*
 * Job #1 : Wake On Lan magic packet forwarding
**/
void forward_wol_magic_packet(){
  // Check if data available (magic packet)
  int packetSize = udp.parsePacket();
  if(packetSize)
  {
    // Reading magic packet
    udp.read(packetBuffer, MAGIC_PACKET_SIZE);
 
    // Forwarding magic packet to broadcast IP address
    udp.beginPacket(broadcastIP, 9);
    udp.write(packetBuffer, MAGIC_PACKET_SIZE);
    udp.endPacket();
  }
}
 
/*
 * Job #2 : IP change notifier
**/
void ip_change_notify(char* host, char* path){
  if (tcpClient.connect(host, 80)) {
    tcpClient.println("GET " + (String)path + " HTTP/1.0");
    tcpClient.println("Host: " + (String)host);
    tcpClient.println("User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:8.0.1) Gecko/20100101 Firefox/8.0.1");
    tcpClient.println("Accept: text/html,application/xhtml+xml,application/xml;q=0.9");
    tcpClient.println("Accept-Language: fr,fr-fr;q=0.8,en-us;q=0.5,en;q=0.3");
    tcpClient.println("Accept-Encoding: gzip, deflate");
    tcpClient.println("Accept-Charset: ISO-8859-1,utf-8;q=0.7");
    tcpClient.println("Connection: close");
    tcpClient.println(); // HTTP header end (empty line)
  }
  tcpClient.stop();
}