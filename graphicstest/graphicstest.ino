
/***************************************************
  This is a library for the Adafruit 1.8" SPI display.

This library works with the Adafruit 1.8" TFT Breakout w/SD card
  ----> http://www.adafruit.com/products/358
The 1.8" TFT shield
  ----> https://www.adafruit.com/product/802
The 1.44" TFT breakout
  ----> https://www.adafruit.com/product/2088
as well as Adafruit raw 1.8" TFT display
  ----> http://www.adafruit.com/products/618

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ESP8266HTTPClient.h>

#include <Adafruit_GFX.h>    // Core graphics library
//#include <Adafruit_ST7735.h> // Hardware-specific library
#include <TFT_ILI9163C.h>
#include <SPI.h>
#include <ClassicBot.h>

// PINS
// ESP to Arduino
static const uint8_t D0   = 16;
static const uint8_t D1   = 5;
static const uint8_t D2   = 4;
static const uint8_t D3   = 0;
static const uint8_t D4   = 2;
static const uint8_t D5   = 14;
static const uint8_t D6   = 12;
static const uint8_t D7   = 13;
static const uint8_t D8   = 15;
static const uint8_t D9   = 3;
static const uint8_t D10  = 1;

// TFT pins
#define TFT_RST    D0
#define TFT_CS     D1
#define TFT_DC     D2
#define TFT_MOSI   D7
#define TFT_SCLK   D5
#define TFT_LITE   D8

// Network stuffs

char ssid[] = "";  //  your network SSID (name)
char pass[] = "";       // your network password

unsigned int localPort = 2390;      // local port to listen for UDP packets
IPAddress timeServerIP; // time.nist.gov NTP server address
const char* ntpServerName = "time.nist.gov";
const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets 
// A UDP instance to let us send and receive packets over UDP
WiFiUDP udp;


char* string2char(String command){
    if(command.length()!=0){
        char *p = const_cast<char*>(command.c_str());
        return p;
    }
}

TFT_ILI9163C tft = TFT_ILI9163C(TFT_CS, TFT_DC, TFT_RST);


Bot *bot = new Bot(&tft);
//Eye eyes[2];


void setup(void) {

  Serial.begin(115200);
  Serial.print("Hello! Classicbot!"); 
  tft.begin();
  tft.setRotation(1);
  Serial.println("Initialized");
  bot->sayHello();
  
 // We start by connecting to a WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);

  // Reset all the Wifi settings...
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  #if defined(DEBUG)
    IPAddress ip = WiFi.localIP();
    Serial.println(ip);
    String msg = ip.toString();
    bot->drawTextAt(string2char(msg), RED, 32, 32, 1);
  #endif
  
  bot->bootSequence();  
  Serial.println("Starting UDP");
  udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(udp.localPort());

}

void loop() {

  //bot->loop();
  bot->resetDisplay();  
  delay(3000);
  do_net_stuff();
  
  Serial.println("Loop complete!");

  
}


// send an NTP request to the time server at the given address
unsigned long sendNTPpacket(IPAddress& address)
{
  Serial.println("sending NTP packet...");
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udp.beginPacket(address, 123); //NTP requests are to port 123
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}


void do_net_stuff(){

     //get a random server from the pool
  WiFi.hostByName(ntpServerName, timeServerIP); 

  sendNTPpacket(timeServerIP); // send an NTP packet to a time server
  // wait to see if a reply is available
  delay(1000);
  
  int cb = udp.parsePacket();
  if (!cb) {
    Serial.println("no packet yet");
  }
  else {
    Serial.print("packet received, length=");
    Serial.println(cb);
    // We've received a packet, read the data from it
    udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

    //the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, esxtract the two words:

    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    Serial.print("Seconds since Jan 1 1900 = " );
    Serial.println(secsSince1900);

    // now convert NTP time into everyday time:
    Serial.print("Unix time = ");
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    // subtract seventy years:
    unsigned long epoch = secsSince1900 - seventyYears;
    // print Unix time:
    Serial.println(epoch);

    // Build the hour, minute and second string:
    String htime = "";
    int hour = (epoch  % 86400L) / 3600;
    int m = ((epoch % 3600) / 60);
    int s = (epoch % 60);
    
    String minute = (m < 10) ? m : "0" + (String) m; 
    String second = (s < 10) ? s : "0" + (String) s;

    htime = hour + ":" + minute + ":" + second;

    Serial.println(htime);
    bot->drawText(string2char(htime),BLACK);

  }
  // wait ten seconds before asking for the time again
  delay(10000);
 
 }

 /*

void draw_eyes() {
  // Wait some time...
  //delay(3000);

  //Blink
  blink_eyes(random(1,2),random(1, 5));
  
  // Wait some time...
  delay(random(1,10) * 1000);

  //Look around the room
  look_a round(random(1,2));
  
}

void blink_eyes(int repeat, int wait) {
  
  int color = CYAN;
  int i;
  int t;
  int eye;
  int loop_count = 0;

  tft.fillScreen(DARK_GREY);

  while(loop_count <= repeat) {
    
    // Open Eye Base Dimensions
    int x = 10; //(eye == 0) ? 10 : 110;
    int y = LEFT_EYE_Y;
    int w = 10;
    int h = 1;
    int eye_top = y - 2;
    int eyelid_height = 0;
    
      while(eyelid_height <= 40){
      //for(i = 0 ; i <= 40; i+=1) {
        // Close Eye
        tft.fillRect(LEFT_EYE_X, eye_top , w, eyelid_height, DARK_GREY);
        tft.fillRect(RIGHT_EYE_X, eye_top, w, eyelid_height, DARK_GREY);
        //eye_top -= 1;
        eyelid_height +=2;        
      }
      delay(random(1,5) * 100);
      
      y = LEFT_EYE_Y + 30;
      while(y >= LEFT_EYE_Y){
        // Open Eye
        tft.fillRoundRect(LEFT_EYE_X, y, w, h, 2, color);
        tft.fillRoundRect(RIGHT_EYE_X, y, w, h, 2, color);
        
        y -=1;
        h +=1;
        
      }
      
    loop_count++;
    
    eyelid_height = 0;
    y = LEFT_EYE_Y;
    h = 1;
    
    delay(wait * 500);    
    }
  }


void look_around(int direct) {

  int color = CYAN;
  int i;
  int t;

      int x = 0;
      int y = 80;
      int w = 10;
      int h = 30;
      int travel = random(2,30);
      int travelled = 0;

      while (travelled <= travel){
        tft.fillRect((LEFT_EYE_X - w) + travelled, y, w, h + 2, DARK_GREY);
        tft.fillRect((RIGHT_EYE_X - w) + travelled, y, w, h + 2, DARK_GREY);

        tft.fillRoundRect(LEFT_EYE_X + travelled, y, w, h, 2, color);
        tft.fillRoundRect(RIGHT_EYE_X + travelled, y, w, h, 2, color);
        travelled++;
        
        }
      delay(1000);
      while (travelled >= 0){
        tft.fillRect((LEFT_EYE_X + travelled + w), y, w, h, DARK_GREY);
        tft.fillRect((RIGHT_EYE_X + travelled + w), y, w, h, DARK_GREY);
        
        tft.fillRoundRect(LEFT_EYE_X + travelled, y, w, h, 2, color);
        tft.fillRoundRect(RIGHT_EYE_X + travelled, y, w, h, 2, color);
        travelled--; 
        
        }
  }


void drawtext(char *text, uint16_t color) {
  tft.setCursor(1, 1);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.setTextSize(2);

  tft.print(text);
}

*/

