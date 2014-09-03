#include <FileIO.h>
#include <Adafruit_NeoPixel.h>

#define OUTPUT_COUNT 1

#define NEOPIXEL_OUTPUT_PIN_1 6
#define NEOPIXEL_OUTPUT_PIN_2 7
#define NEOPIXEL_OUTPUT_PIN_3 8
#define NEOPIXEL_OUTPUT_PIN_4 9
#define RED { 16, 0, 0 }
#define YELLOW { 16, 16, 0 }
#define GREEN { 0, 16, 0 }

const String jobDirPrefix = "/mnt/sd/job-";
const String jobStatusFileName = "status.txt";
const String jobRunningFileName = "running.txt";

Adafruit_NeoPixel strip[4] = {
    Adafruit_NeoPixel(64, NEOPIXEL_OUTPUT_PIN_1, NEO_GRB + NEO_KHZ800),
    Adafruit_NeoPixel(64, NEOPIXEL_OUTPUT_PIN_2, NEO_GRB + NEO_KHZ800),
    Adafruit_NeoPixel(64, NEOPIXEL_OUTPUT_PIN_3, NEO_GRB + NEO_KHZ800),
    Adafruit_NeoPixel(64, NEOPIXEL_OUTPUT_PIN_4, NEO_GRB + NEO_KHZ800) 
};

byte stateColors[3][3] = { RED, YELLOW, GREEN };

byte icons[3][2][8] = {
  {
    {
      B00111100,
      B01111110,
      B11111111,
      B11111111,
      B11111111,
      B11111111,
      B01111110,
      B00111100
    },
    {
      B11000011,
      B10000001,
      B00000000,
      B00000000,
      B00000000,
      B00000000,
      B10000001,
      B11000011
    }
  },
  {
    {
      B00111100,
      B01111110,
      B11111111,
      B11111111,
      B11111111,
      B11111111,
      B01111110,
      B00111100
    },
    {
      B11000011,
      B10000001,
      B00000000,
      B00000000,
      B00000000,
      B00000000,
      B10000001,
      B11000011
    }
  },
  {
    {
      B00111100,
      B01111110,
      B11111111,
      B11111111,
      B11111111,
      B11111111,
      B01111110,
      B00111100
    },
    {
      B11000011,
      B10000001,
      B00000000,
      B00000000,
      B00000000,
      B00000000,
      B10000001,
      B11000011
    }
  }
};

void setup() {
  
  Bridge.begin();
  FileSystem.begin();
  
  for (int index = 0; index < OUTPUT_COUNT; index++) {
    strip[index].begin();
    strip[index].show();
  }
  
  selfTest();
  
}

void loop() {
  
  displayJobStatus(1);  
  delay(1000);
  displayJobStatus(0);  
  delay(1000);
  
}

void displayJobStatus(byte toggle) {
  
  for (int jobNo = 0; jobNo < OUTPUT_COUNT; jobNo++) {
    byte state = retrieveJobStatus(jobNo);
    byte running = retrieveJobRunning(jobNo);
    byte phase = running * toggle;
    updateDisplay(jobNo, state, phase);
  }

}

void updateDisplay(byte displayNo, byte state, byte phase) {
  
  for (byte rowNo = 0; rowNo < 8; rowNo++) {
    byte row = icons[state][phase][rowNo];
    for (byte colNo = 0; colNo < 8; colNo++) {
      byte pixelNo = (rowNo * 8) + colNo;
      if ((row & (1 << colNo)) != 0) {
        strip[displayNo].setPixelColor(pixelNo, strip[displayNo].Color(stateColors[state][0], stateColors[state][1], stateColors[state][2]));
      }
      else {
        strip[displayNo].setPixelColor(pixelNo, strip[displayNo].Color(0, 0, 0));
      }
    }
  }
  
  strip[displayNo].show();  
  
}

byte retrieveJobStatus(int jobNo) {
  
  File file = openR(getJobStatusFilePath(jobNo));
  char statusChar;
  if (file) {
    statusChar = file.read();
    file.close();
  }
  else {
    statusChar = 48;
  }
  
  return (byte) statusChar - 48;
  
}

byte retrieveJobRunning(int jobNo) {
  
  File file = openR(getJobRunningFilePath(jobNo));
  char statusChar = file.read();
  file.close();
  
  return (byte) statusChar - 48;
  
}

void selfTest() {
  
}

String getJobStatusFilePath(int jobNo) {
  return getJobDir(jobNo) + "/" + jobStatusFileName;
}

String getJobRunningFilePath(int jobNo) {
  return getJobDir(jobNo) + "/" + jobRunningFileName;
}

String getJobDir(int jobNo) {
  return jobDirPrefix + String(jobNo);
}

File openR(String path) {
  
  char filename[path.length() + 1];
  path.toCharArray(filename, path.length() + 1);
  filename[path.length()] = 0;

  return FileSystem.open(filename);  
  
}

File openW(String path) {
  
  char filename[path.length() + 1];
  path.toCharArray(filename, path.length() + 1);
  filename[path.length()] = 0;

  return FileSystem.open(filename, FILE_WRITE);  
  
}
