#include <FileIO.h>
#include <Adafruit_NeoPixel.h>

#define NEOPIXEL_OUTPUT_PIN 6

#define FAILED { 16, 0, 0 }
#define TESTS_FAILED { 0, 16, 16 }
#define PASSED { 0, 16, 0 }

const String panelLayoutFile = "/mnt/sd/layout.txt";
const String jobStatusFileName = "/mnt/sd/status.txt";

Adafruit_NeoPixel strip = Adafruit_NeoPixel(128, NEOPIXEL_OUTPUT_PIN, NEO_GRB + NEO_KHZ800);

int baseIndexes[2][4] = {
  { 100, 96, 36, 32 },
  { 68, 64, 4, 0 }
};

byte stateColors[3][3] = { FAILED, TESTS_FAILED, PASSED };

void setup() {
  
  Bridge.begin();
  FileSystem.begin();
  
  strip.begin();
  strip.show();
  
  selfTest();
  
}

void loop() {

  for (int phase = 0; phase <= 1; phase++) {  
    for (int step = 1; step <= 4; step++) {
      displayJobStatus(phase, step * 2);  
      delay(30);
    }
  }
  
}

void displayJobStatus(int phase, int step) {
  
  int state[8] = {
    retrieveJobStatus(0),
    retrieveJobStatus(1),
    retrieveJobStatus(2),
    retrieveJobStatus(3),
    retrieveJobStatus(4),
    retrieveJobStatus(5),
    retrieveJobStatus(6),
    retrieveJobStatus(7)
  };
  
  byte running[8] = {
    retrieveJobRunning(0),
    retrieveJobRunning(1),
    retrieveJobRunning(2),
    retrieveJobRunning(3),
    retrieveJobRunning(4),
    retrieveJobRunning(5),
    retrieveJobRunning(6),
    retrieveJobRunning(7)
  };
  
  File file = openR(panelLayoutFile);
  if (file) {
    for (byte row = 0; row <= 1; row++) {
      for (byte col = 0; col <= 3; col++) {
        byte jobNo = file.read() - 48;
        updateCell(baseIndexes[row][col], state[jobNo], running[jobNo], phase, step);
      }
      file.read(); // Read the trailing LF
    }
    file.close();
    strip.show();
  }
  
}

void updateCell(int baseIndex, int state, byte running, int phase, int step) {
  
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

int retrieveJobStatus(int jobNo) {
  
  File file = openR(jobStatusFileName);
  char statusChar;
  if (file) {
    int index = (jobNo * 2) + 1;
    while (index > 0) {
      index--;
      statusChar = file.read() - 48;
    }
    file.close();
    return statusChar;
  }
  else {
    file.close();
    return -1;
  }
  
}

byte retrieveJobRunning(int jobNo) {
  
  File file = openR(jobStatusFileName);
  char statusChar;
  if (file) {
    int index = (jobNo + 1) * 2;
    while (index > 0) {
      index--;
      statusChar = file.read() - 48;
    }
    file.close();
  }
  else {
    file.close();
    statusChar = 0;
  }
  
  return (byte) statusChar;
  
}

void selfTest() {
  
}

File openR(String path) {
  
  char filename[path.length() + 1];
  path.toCharArray(filename, path.length() + 1);
  filename[path.length()] = 0;

  return FileSystem.open(filename);  
  
}
