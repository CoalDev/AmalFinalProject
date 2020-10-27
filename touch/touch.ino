const int px = 15;
const int dis = 10;
const int w = 90;
const int h = 90;
const int py = 100;
const int SD_CS = 4;

#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN            A0
#define NUMPIXELS      10
#define ARDUINO_RX     5
#define ARDUINO_TX     6

#define CMD_NEXT_SONG     0X01  // Play next song.
#define CMD_PREV_SONG     0X02  // Play previous song.
#define CMD_PLAY_W_INDEX  0X03
#define CMD_VOLUME_UP     0X04
#define CMD_VOLUME_DOWN   0X05
#define CMD_SET_VOLUME    0X06

#define CMD_SNG_CYCL_PLAY 0X08  // Single Cycle Play.
#define CMD_SEL_DEV       0X09
#define CMD_SLEEP_MODE    0X0A
#define CMD_WAKE_UP       0X0B
#define CMD_RESET         0X0C
#define CMD_PLAY          0X0D
#define CMD_PAUSE         0X0E
#define CMD_PLAY_FOLDER_FILE 0X0F

#define CMD_STOP_PLAY     0X16
#define CMD_FOLDER_CYCLE  0X17
#define CMD_SHUFFLE_PLAY  0x18 //
#define CMD_SET_SNGL_CYCL 0X19 // Set single cycle.

#define CMD_SET_DAC 0X1A
#define DAC_ON  0X00
#define DAC_OFF 0X01

#define CMD_PLAY_W_VOL    0X22
#define CMD_PLAYING_N     0x4C
#define CMD_QUERY_STATUS      0x42
#define CMD_QUERY_VOLUME      0x43
#define CMD_QUERY_FLDR_TRACKS 0x4e
#define CMD_QUERY_TOT_TRACKS  0x48
#define CMD_QUERY_FLDR_COUNT  0x4f

#define DEV_TF            0X02

static int8_t Send_buf[8] = {0}; // Buffer for Send commands.  // BETTER LOCALLY
static uint8_t ansbuf[10] = {0}; // Buffer for the answers.    // BETTER LOCALLY

String mp3Answer;
String p1 = "";
String p2 = "";
String p3 = "";
String t = "";

int delayval = 1000;
int player;
int screen = 1;
int seconds = -1;
int minutes = 0;

int pos[3] = {55 + 0 * 77, 55 + 1 * 77, 55 + 2 * 77} ;

bool timerRun = true;

#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>
#include <SdFat.h>
#include <SPI.h>
#include "spiUnit.h"
#include "TFT9341.h"
#include "touch.h"
#include <SPI.h>
#include "bitmap.h"

SdFat sd;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
SoftwareSerial mp3(ARDUINO_RX, ARDUINO_TX);

word x, y;
void setup() {


#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  pixels.begin();
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    pixels.show();
  }
  mp3.begin(9600);
  sendCommand(CMD_PAUSE, 0);
  setFirstRecords();
  sd.begin(SD_CS, SPI_QUARTER_SPEED);
  Serial.begin(2000000);
  lcd.begin();
  lcdtouch.begin();
  lcdtouch.InitTypeTouch(2);
  bmp::draw("screen1.bmp", 0, 0);

}

void loop() {
  checkPress();
}


void setFirstRecords()
{
  for (int i = 7; i <= 9; i++)
    if (EEPROM.read(i) != 1)
      EEPROM.write(i, 0);
}
void checkPress()
{
  if (digitalRead(2) == 0) {
    lcdtouch.readxy();
    x = lcdtouch.readx();
    y = lcdtouch.ready();


    if (screen == 1) {


      if ((x > px) && (x < (px + w)) && (y > py) && (y < (py + h))) {
        player = 1;
        screen = 2;
        startGame();
      }
      else if ((x > (px + w + dis)) && (x < (2 * w + dis)) && (y > py) && (y < (py + h))) {
        player = 2;
        screen = 2;
        startGame();
      }
      else if ((x > (2 * dis + 2 * w)) && (x < (2 * dis + 3 * w)) && (y > py) && (y < (py + h))) {
        player = 3;
        screen = 2;
        startGame();
      }
    }
    else if (screen == 2) {
      if (x > 70 && x < 150 && y > 182 && y < 222) { //finish
        sendCommand(CMD_PAUSE, 0);
        screen = 3;
        timerRun = false;
        bmp::draw("screen3.bmp", 0, 0);
        // stop timer && save
      }
      else if (x > 174 && x < 254 && y > 182 && y < 222) {
        bmp::draw("screen2.bmp", 0, 0);
        seconds = -1;
        minutes = 0;
        countDown(5);
        // reset  timer
      }
    }
    else if (screen == 3) {

      if (x > 70 && x < 150 && y > 182 && y < 222) { //finish
        screen = 1;
        for (int i = 0; i < NUMPIXELS; i++) {
          pixels.setPixelColor(i, pixels.Color(0, 0, 0));
          pixels.show();
        }
        bmp::draw("screen1.bmp", 0, 0);
      }
      else if (x > 174 && x < 254 && y > 182 && y < 222) {
        screen = 2;
        startGame();
      }
    }
  }
  String playerCheck = "Player: " + String(player) + " " + "Screen: " + String(screen);
}


void timer() {
  lcd.gotoxy(60, 70);

  t = "";
  if (minutes < 10)
    t += "0";
  t += String(minutes) + ":";
  if (seconds < 10)
    t += "0";
  t += String(seconds);
  lcd.setColor(BLACK);
  lcd.print(t);
}



void countDown(int num) {
  lcd.setColor(BLACK);

  lcd.setFont(7);
  for (int i = num; i > 0; i--) {
    lcd.gotoxy(140, 70);
    lcd.print(i);
    delay(1000);
  }
  lcd.gotoxy(140, 70);
  lcd.print(" ");
}

void startGame() {
  ColorFlash(player);
  timerRun = true;
  bmp::draw("screen2.bmp", 0, 0);
  countDown(5);
  sendCommand(CMD_FOLDER_CYCLE, 0x0101);
  lcd.setColor(BLACK);
  lcd.setFont(7);
  while (timerRun)
  {

    seconds++;
    timer();
    for (int i = 0; i < 20; i++)
    {
      delay(50);
      checkPress();
    }
    if (seconds == 60) {
      minutes++;
      seconds = 0;
    }
    if (minutes == 60) {
      minutes = 0;// maybe gameover
    }
  }
  UpdateRecord();
  seconds = -1;
  minutes = 0;
  timerRun = false;
}

void UpdateRecord() {
  t = "";
  p1 = "";
  p2 = "";
  p3 = "";
  if (player == 1) {


    if ((minutes * 60 + seconds) < (EEPROM.read(2 * player - 1) * 60 + EEPROM.read(2 * player)))
    {
      EEPROM.write(2 * player - 1, minutes);
      EEPROM.write(2 * player, seconds);
      lcd.setColor(GREEN); // Set color of score depending if the score was broken
      ColorBR(true);
    } else {
      lcd.setColor(RED); // Set color of score depending if the score wasn't broken
      ColorBR(false);
    }
    if (EEPROM.read(1) < 10)
      p1 += "0";
    p1 += String(EEPROM.read(1)) + ":";
    if (EEPROM.read(2) < 10)
      p1 += "0";
    p1 += String(EEPROM.read(2));

    lcd.gotoxy(55 + 0 * 77, 130);
    lcd.setFont(2);
    if (EEPROM.read(1) < 59 && EEPROM.read(2) < 59) {
      lcd.print(p1);
    }

    if (EEPROM.read(3) < 10)
      p2 += "0";
    p2 += String(EEPROM.read(3)) + ":";
    if (EEPROM.read(4) < 10)
      p2 += "0";
    p2 += String(EEPROM.read(4));
    lcd.setColor(BLACK);
    lcd.gotoxy(55 + 1 * 77, 130);
    lcd.setFont(2);
    if (EEPROM.read(3) < 59 && EEPROM.read(4) < 59) {
      lcd.print(p2);
    }

    if (EEPROM.read(5) < 10)
      p3 += "0";
    p3 += String(EEPROM.read(5)) + ":";
    if (EEPROM.read(6) < 10)
      p3 += "0";
    p3 += String(EEPROM.read(6));
    lcd.setColor(BLACK);
    lcd.gotoxy(55 + 2 * 77, 130);
    lcd.setFont(2);
    if (EEPROM.read(5) < 59 && EEPROM.read(6) < 59) {
      lcd.print(p3);
    }
  } else if (player == 2) {
    if (EEPROM.read(1) < 10)
      p1 += "0";
    p1 += String(EEPROM.read(1)) + ":";
    if (EEPROM.read(2) < 10)
      p1 += "0";
    p1 += String(EEPROM.read(2));
    lcd.setColor(BLACK);
    lcd.gotoxy(55 + 0 * 77, 130);
    lcd.setFont(2);
    if (EEPROM.read(1) < 59 && EEPROM.read(2) < 59) {
      lcd.print(p1);
    }


    if ((minutes * 60 + seconds) < (EEPROM.read(2 * player - 1) * 60 + EEPROM.read(2 * player)))
    {
      EEPROM.write(2 * player - 1, minutes);
      EEPROM.write(2 * player, seconds);
      ColorBR(true);
      lcd.setColor(GREEN); // Set color of score depending if the score was broken
    } else {
      lcd.setColor(RED); // Set color of score depending if the score wasn't broken
      ColorBR(false);
    }
    if (EEPROM.read(3) < 10)
      p2 += "0";
    p2 += String(EEPROM.read(3)) + ":";
    if (EEPROM.read(4) < 10)
      p2 += "0";
    p2 += String(EEPROM.read(4));

    lcd.gotoxy(55 + 1 * 77, 130);
    lcd.setFont(2);
    if (EEPROM.read(3) < 59 && EEPROM.read(4) < 59) {
      lcd.print(p2);
    }

    if (EEPROM.read(5) < 10)
      p3 += "0";
    p3 += String(EEPROM.read(5)) + ":";
    if (EEPROM.read(6) < 10)
      p3 += "0";
    p3 += String(EEPROM.read(6));
    lcd.setColor(BLACK);
    lcd.gotoxy(55 + 2 * 77, 130);
    lcd.setFont(2);
    if (EEPROM.read(5) < 59 && EEPROM.read(6) < 59) {
      lcd.print(p3);
    }
  } else if (player == 3) {
    if (EEPROM.read(1) < 10)
      p1 += "0";
    p1 += String(EEPROM.read(1)) + ":";
    if (EEPROM.read(2) < 10)
      p1 += "0";
    p1 += String(EEPROM.read(2));
    lcd.setColor(BLACK);
    lcd.gotoxy(pos[player - 1], 130);
    lcd.setFont(2);
    if (EEPROM.read(1) < 59 && EEPROM.read(2) < 59) {
      lcd.print(p1);
    }

    if (EEPROM.read(3) < 10)
      p2 += "0";
    p2 += String(EEPROM.read(3)) + ":";
    if (EEPROM.read(4) < 10)
      p2 += "0";
    p2 += String(EEPROM.read(4));
    lcd.setColor(BLACK);
    lcd.gotoxy(pos[player - 1], 130);
    lcd.setFont(2);
    if (EEPROM.read(3) < 59 && EEPROM.read(4) < 59) {
      lcd.print(p2);
    }

    if ((minutes * 60 + seconds) < (EEPROM.read(2 * player - 1) * 60 + EEPROM.read(2 * player)))
    {
      EEPROM.write(2 * player - 1, minutes);
      EEPROM.write(2 * player, seconds);
      ColorBR(true);
      lcd.setColor(GREEN); // Set color of score depending if the score was broken
    } else {
      ColorBR(false);
      lcd.setColor(RED); // Set color of score depending if the score wasn't broken
    }
    if (EEPROM.read(5) < 10)
      p3 += "0";
    p3 += String(EEPROM.read(5)) + ":";
    if (EEPROM.read(6) < 10)
      p3 += "0";
    p3 += String(EEPROM.read(6));
    String sm = "Seconds:" + String(seconds + minutes * 60);
    String Esm = "ESeconds: " + String(EEPROM.read(2 * player - 1) * 60 + EEPROM.read(2 * player));
    Serial.println(sm);
    Serial.println(Esm);

    lcd.gotoxy(pos[player - 1], 130);
    lcd.setFont(2);
    if (EEPROM.read(5) < 59 && EEPROM.read(6) < 59) {
      lcd.print(p3);
    }
  }
}

void ColorFlash(int player) {
  if (player == 1) {
    Color(159, 126, 184);
  } else if (player == 2) {
    Color(85, 188, 236);
  } else if (player == 3) {
    Color(221, 137, 145);
  }
}

void Color(int r, int g, int b) {
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(r, g, b));
    pixels.show();
  }
  delay(delayval);
}

void ColorBR(bool check) {
  if (check) {
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 255, 0));
      pixels.show();
    }
  } else {
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(255, 0, 0));
      pixels.show();
    }
  }
}

/*
   lose red
   win green
   every timer tick flash the players color
   ulay dagesh al ulay when the record is close to not being beaten flash in the color red

*/

void sendMP3Command(char c) {
  switch (c) {
    case '?':
    case 'h':
      Serial.println("HELP  ");
      Serial.println(" p = Play");
      Serial.println(" P = Pause");
      Serial.println(" > = Next");
      Serial.println(" < = Previous");
      Serial.println(" + = Volume UP");
      Serial.println(" - = Volume DOWN");
      Serial.println(" c = Query current file");
      Serial.println(" q = Query status");
      Serial.println(" v = Query volume");
      Serial.println(" x = Query folder count");
      Serial.println(" t = Query total file count");
      Serial.println(" 1 = Play folder 1");
      Serial.println(" 2 = Play folder 2");
      Serial.println(" 3 = Play folder 3");
      Serial.println(" 4 = Play folder 4");
      Serial.println(" 5 = Play folder 5");
      Serial.println(" S = Sleep");
      Serial.println(" W = Wake up");
      Serial.println(" r = Reset");
      break;


    case 'p':
      Serial.println("Play ");
      sendCommand(CMD_PLAY, 0);
      break;

    case 'P':
      Serial.println("Pause");
      sendCommand(CMD_PAUSE, 0);
      break;

    case '>':
      Serial.println("Next");
      sendCommand(CMD_NEXT_SONG, 0);
      sendCommand(CMD_PLAYING_N, 0x0000); // ask for the number of file is playing
      break;

    case '<':
      Serial.println("Previous");
      sendCommand(CMD_PREV_SONG, 0);
      sendCommand(CMD_PLAYING_N, 0x0000); // ask for the number of file is playing
      break;

    case '+':
      Serial.println("Volume Up");
      sendCommand(CMD_VOLUME_UP, 0);
      break;

    case '-':
      Serial.println("Volume Down");
      sendCommand(CMD_VOLUME_DOWN, 0);
      break;

    case 'c':
      Serial.println("Query current file");
      sendCommand(CMD_PLAYING_N, 0);
      break;

    case 'q':
      Serial.println("Query status");
      sendCommand(CMD_QUERY_STATUS, 0);
      break;

    case 'v':
      Serial.println("Query volume");
      sendCommand(CMD_QUERY_VOLUME, 0);
      break;

    case 'x':
      Serial.println("Query folder count");
      sendCommand(CMD_QUERY_FLDR_COUNT, 0);
      break;

    case 't':
      Serial.println("Query total file count");
      sendCommand(CMD_QUERY_TOT_TRACKS, 0);
      break;

    case '1':
      Serial.println("Play folder 1");
      sendCommand(CMD_FOLDER_CYCLE, 0x0101);
      break;

    case '2':
      Serial.println("Play folder 2");
      sendCommand(CMD_FOLDER_CYCLE, 0x0102);
      break;

    case '3':
      Serial.println("Play folder 3");
      sendCommand(CMD_FOLDER_CYCLE, 0x0103);
      break;

    case '4':
      Serial.println("Play folder 4");
      sendCommand(CMD_FOLDER_CYCLE, 0x0104);
      break;

    case '5':
      Serial.println("Play folder 5");
      sendCommand(CMD_FOLDER_CYCLE, 0x0105);
      break;

    case 'S':
      Serial.println("Sleep");
      sendCommand(CMD_SLEEP_MODE, 0x00);
      break;

    case 'W':
      Serial.println("Wake up");
      sendCommand(CMD_WAKE_UP, 0x00);
      break;

    case 'r':
      Serial.println("Reset");
      sendCommand(CMD_RESET, 0x00);
      break;
  }
}

String decodeMP3Answer() {
  String decodedMP3Answer = "";

  decodedMP3Answer += sanswer();

  switch (ansbuf[3]) {
    case 0x3A:
      decodedMP3Answer += " -> Memory card inserted.";
      break;

    case 0x3D:
      decodedMP3Answer += " -> Completed play num " + String(ansbuf[6], DEC);
      break;

    case 0x40:
      decodedMP3Answer += " -> Error";
      break;

    case 0x41:
      decodedMP3Answer += " -> Data recived correctly. ";
      break;

    case 0x42:
      decodedMP3Answer += " -> Status playing: " + String(ansbuf[6], DEC);
      break;

    case 0x48:
      decodedMP3Answer += " -> File count: " + String(ansbuf[6], DEC);
      break;

    case 0x4C:
      decodedMP3Answer += " -> Playing: " + String(ansbuf[6], DEC);
      break;

    case 0x4E:
      decodedMP3Answer += " -> Folder file count: " + String(ansbuf[6], DEC);
      break;

    case 0x4F:
      decodedMP3Answer += " -> Folder count: " + String(ansbuf[6], DEC);
      break;
  }

  return decodedMP3Answer;
}

void sendCommand(int8_t command, int16_t dat)
{
  delay(20);
  Send_buf[0] = 0x7e;   //
  Send_buf[1] = 0xff;   //
  Send_buf[2] = 0x06;   // Len
  Send_buf[3] = command;//
  Send_buf[4] = 0x01;   // 0x00 NO, 0x01 feedback
  Send_buf[5] = (int8_t)(dat >> 8);  //datah
  Send_buf[6] = (int8_t)(dat);       //datal
  Send_buf[7] = 0xef;   //
  Serial.print("Sending: ");
  for (uint8_t i = 0; i < 8; i++)
  {
    mp3.write(Send_buf[i]) ;
    Serial.print(sbyte2hex(Send_buf[i]));
  }
  Serial.println();
}

String sbyte2hex(uint8_t b)
{
  String shex;

  shex = "0X";

  if (b < 16) shex += "0";
  shex += String(b, HEX);
  shex += " ";
  return shex;
}

String sanswer(void)
{
  uint8_t i = 0;
  String mp3answer = "";

  // Get only 10 Bytes
  while (mp3.available() && (i < 10))
  {
    uint8_t b = mp3.read();
    ansbuf[i] = b;
    i++;

    mp3answer += sbyte2hex(b);
  }

  // if the answer format is correct.
  if ((ansbuf[0] == 0x7E) && (ansbuf[9] == 0xEF))
  {
    return mp3answer;
  }

  return "???: " + mp3answer;
}







/* IDEA LIST:
    1.a.When timer is running check if the timer didnt pass the record yet if didnt pass and still has a chance to beat then color Green timer
    1.b.else color Red
    2.a. if record was beat then score in the scoreboard will be Green
    2.b. else(Record wasn't beat) color will be Red
*/
