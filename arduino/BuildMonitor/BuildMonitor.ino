#include <FileIO.h>

const int latchPin = 10;
const int clockPin = 13;
const int dataPin = 11;

const int red = 8;
const int amber = 4;
const int green = 2;

const byte testValues[6] = { 128, 64, 32, 8, 4, 2 };

const String jobDirPrefix = "/mnt/sd/job-";
const String jobStatusFileName = "status.txt";
const String jobRunningFileName = "running.txt";

void setup() {
  
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  
  Bridge.begin();
  FileSystem.begin();
  
  selfTest();
  
}

void loop() {
  
  displayJobStatus(1);  
  delay(1000);
  displayJobStatus(0);  
  delay(1000);
  
}

void displayJobStatus(byte toggle) {
  
  byte status3 = calculateJobStatus(3, toggle);
  byte status2 = calculateJobStatus(2, toggle);
  byte lsb = status3 + (status2 * 16);
  byte status1 = calculateJobStatus(1, toggle);
  byte status0 = calculateJobStatus(0, toggle);
  byte msb = status1 + (status0 * 16);
  
  refresh(msb, lsb);

}

byte calculateJobStatus(int jobNo, byte toggle) {
  return retrieveJobStatus(jobNo) * (retrieveJobRunning(jobNo) == 0 ? 1 : toggle);
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
  
  refresh(0, 0);
  
  for (int index = 0; index < 12; index++) {
    if (index < 6) {
      refresh(testValues[index], 0);
    }
    else {
      refresh(0, testValues[index - 6]);
    }
    delay(500);
  } 
  
  refresh(0, 0);
  delay(2000);
  
}

void refresh(byte msb, byte lsb) {
  
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, lsb);
  shiftOut(dataPin, clockPin, LSBFIRST, msb);
  digitalWrite(latchPin, HIGH);
  
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
