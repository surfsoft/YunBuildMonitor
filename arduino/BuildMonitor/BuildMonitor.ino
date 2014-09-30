#include <FileIO.h>
#include <Adafruit_NeoPixel.h>
#include <Bridge.h>
#include <Console.h>

#define NEOPIXEL_OUTPUT_PIN 6

#define RED { 16, 0, 0 }
#define ORANGE { 16, 8, 0}
#define YELLOW { 16, 16, 0 }
#define GREEN { 0, 16, 0 }
#define JENKINS_FLAG 64 + 10
#define NAGIOS_FLAG 64 + 14
#define DELIMITOR 44

const String panelLayoutFile = "/mnt/sd/layout.txt";
const String jenkinsStatusFileName = "/mnt/sd/jenkins-status.txt";
const String nagiosStatusFileName = "/mnt/sd/nagios-status.txt";

Adafruit_NeoPixel strip = Adafruit_NeoPixel(128, NEOPIXEL_OUTPUT_PIN, NEO_GRB + NEO_KHZ800);

int baseIndexes[2][4] = {
  { 100, 96, 36, 32 },
  { 68, 64, 4, 0 }
};

int baseLineIndexes[2][4] = {
  {127, 119, 111, 103},
  {95, 87, 79, 71}
};

int state[8];
int running[8];
int percent[8];

byte stateColors[3][3] = { RED, YELLOW, GREEN };
byte nagiosColors[16][3] = { GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, YELLOW, YELLOW, YELLOW, ORANGE, ORANGE, ORANGE, RED, RED, RED, RED };

void setup() {
  
  Bridge.begin();
  FileSystem.begin();
  
  strip.begin();
  strip.show();
  
}

void loop() {
  
  for (int phase = 0; phase <= 1; phase++) {  
    retrieveJenkinsStatus();
    retrieveNagiosStatus();
    for (int step = 1; step <= 8; step++) {
      displayJenkinsStatus(phase, step);  
      displayNagiosStatus(phase, step);  
      strip.show();
      delay(30);
    }
    delay((phase + 1) * 500);
  }
  
}

void displayJenkinsStatus(int phase, int step) {
  
  File file = openR(panelLayoutFile);
  if (file) {
    for (byte row = 0; row <= 1; row++) {
      boolean jenkinsRow = file.read() == JENKINS_FLAG;
      for (byte col = 0; col <= 3; col++) {
        byte jobNo = (byte) (file.read() - 48);
        if (jenkinsRow) {
          updateCell(baseIndexes[row][col], state[jobNo], running[jobNo], phase, step);
        }
      }
      file.read(); // Read the trailing LF
    }
    file.close();
  }
  
}

void updateCell(int baseIndex, int state, int running, int phase, int step) {
  
  for (byte rowIndex = 0; rowIndex <= 3; rowIndex++) {
    for (byte colIndex = 0; colIndex <= 3; colIndex++) {
      int pixelNo = baseIndex + (8 * (3 - rowIndex)) + ( 3 - colIndex);
      if (state >= 0) {
        if (running == 0) {
          strip.setPixelColor(pixelNo, strip.Color(stateColors[state][0],
            stateColors[state][1],
            stateColors[state][2]));
        }
        else {
          int red = stateColors[state][0];
          int green = stateColors[state][1];
          int blue = stateColors[state][2];
          if (phase == 0) {
            red = red - (step * 2);
            green = green - (step * 2);
            blue = blue - (step * 2);
          }
          else {
            red = red - 16 + (step * 2);
            green = green - 16 + (step * 2);
            blue = blue - 16 + (step * 2);
          }
          if (red < 0) red = 0;
          if (green < 0) green = 0;
          if (blue < 0) blue = 0;
          strip.setPixelColor(pixelNo, strip.Color(red, green, blue));
        }
      }
      else {
        strip.setPixelColor(pixelNo, strip.Color(0, 0, 0));
      }
    }
  }
  
}

void displayNagiosStatus(int phase, int step) {
  
  File file = openR(panelLayoutFile);
  if (file) {
    for (byte row = 0; row <= 1; row++) {
      boolean nagiosRow = file.read() == NAGIOS_FLAG;
      for (byte line = 0; line <= 3; line++) {
        byte jobNo = file.read() - 48;
        if (nagiosRow) {
          updateLine(row, line, percent[jobNo], phase, step);
        }
      }
      file.read(); // Read the trailing LF
    }
    file.close();
  }
  
}

void updateLine(byte row, byte line, int percentage, int phase, int step) {
  
  for (byte panel = 0; panel <= 1; panel++) {
    int baseIndex = baseLineIndexes[row][line];
    if (panel == 1) {
      baseIndex -= 64;
    }
    for (int pixelNo = 0; pixelNo <= 7; pixelNo++) {
      int actualPixelNo = pixelNo + (panel * 8);
      int threshold = (actualPixelNo * 100) / 16;
      int neoPixelNo = baseIndex - pixelNo;
      if (percentage > threshold) {
        if (percentage < 88) {
          strip.setPixelColor(neoPixelNo, strip.Color(
            nagiosColors[actualPixelNo][0],
            nagiosColors[actualPixelNo][1],
            nagiosColors[actualPixelNo][2]));
        }
        else {
          int red = nagiosColors[actualPixelNo][0];
          int green = nagiosColors[actualPixelNo][1];
          int blue = nagiosColors[actualPixelNo][2];
          if (phase == 0) {
            red = red - (step * 2);
            green = green - (step * 2);
            blue = blue - (step * 2);
          }
          else {
            red = red - 16 + (step * 2);
            green = green - 16 + (step * 2);
            blue = blue - 16 + (step * 2);
          }
          if (red < 0) red = 0;
          if (green < 0) green = 0;
          if (blue < 0) blue = 0;
          strip.setPixelColor(neoPixelNo, strip.Color(red, green, blue));
        }
      }
      else {
        strip.setPixelColor(neoPixelNo, strip.Color(0, 0, 0));
      }
    }
  }
  
}

void retrieveJenkinsStatus() {
  
  File file = openR(jenkinsStatusFileName);
  if (file) {
    for (int jobNo = 0; jobNo < 8; jobNo++) {
      state[jobNo] = (int) file.read() - 48;
      running[jobNo] = (int) file.read() - 48;
    }
  }
  file.close();
  
}

void retrieveNagiosStatus() {
  
  File file = openR(nagiosStatusFileName);
  if (file) {
    for (int jobNo = 0; jobNo < 8; jobNo++) {
      int value = 0;
      int b = (int) file.read();
      while (b != DELIMITOR) {
        value = (value * 10) + b - 48;
        b = file.read();
      }
      percent[jobNo] = value;
    }
  }
  file.close();
  
}

File openR(String path) {
  
  char filename[path.length() + 1];
  path.toCharArray(filename, path.length() + 1);
  filename[path.length()] = 0;

  return FileSystem.open(filename);  

}
