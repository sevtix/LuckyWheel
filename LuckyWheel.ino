#include <Wire.h>

bool HallStatus = false;

void setup() {
  Wire.begin(1, 0);
  Serial.begin(115200);
  pinMode(18, INPUT);
  SetupRelay();
}

void loop() {
  if (digitalRead(18) && HallStatus == false) {
    OnHallRising();
    HallStatus = true;
  }

  if (!digitalRead(18) && HallStatus == true) {
    OnHallFalling();
    HallStatus = false;
  }

  CheckTimer();
}

void OnHallFalling() {
  Serial.println("OnHallFalling");
  StartTimer();
}

void OnHallRising() {
  Serial.println("OnHallRising");
  StopTimer();
}

// RELAY

void SetupRelay() {
  Wire.beginTransmission(0x26);
  Wire.write(0x10);
  Wire.write(1);
  Wire.endTransmission();
}

void WriteRelay(uint8_t data) {
  Wire.beginTransmission(0x26);
  Wire.write(0x11);
  Wire.write(data);
  Wire.endTransmission();
  Serial.println(data);
}

// TIMER

long TimerStartTimestamp = 0L;
int TimerMinimumDuration = 2000;
bool TimerActive = false;

void StartTimer() {
  if (!TimerActive) {
    TimerStartTimestamp = millis();
    TimerActive = true;
    Serial.println("Timer started");
  } else {
    Serial.println("Timer already running");
  }
}

void StopTimer() {
  if (TimerActive) {    
    TimerActive = false;
    Serial.println("Timer stopped");
  } else {
    Serial.println("Timer not running");    
  }
}

void CheckTimer() {
  if (TimerActive) {
    if (millis() - TimerStartTimestamp >= TimerMinimumDuration) {
      Win();
      TimerActive = false;
    }
  }
}

void Win() {
  Serial.println("ExecuteTimerTask");
  WriteRelay(0b00000001);
  delay(2000);
  WriteRelay(0b00000000);
}