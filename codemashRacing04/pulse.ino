int pulseThreshold = 550;                                                                   // this is the switching threshold for a heartbeat
int beatCount = 10;                                                                         // how many beats should be counted?
long pulseTimes[2][10] = {{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}};  // if beatcount changes, this needs to get updated as well
float bpm[] = {70, 70};                                                                     // stores the bpms and starts with default value
int longestTime = 1200;                                                                     // longest plausible time between two heartbeats
long lastBeat[]={0,0};                                                                      // when happened the last beat

boolean pulses[] = {false, false};                                                          // is there a pulse?
boolean lastPulses[] = {false, false};                                                      // was there a pulse?
long lastPulseTimes[2];                                                                     // time counter

void pulseUpdate() {                                                                        // needs to be called as often as possible
  for (int j = 0; j < 2; j++) {
    if (analogRead(pulsePins[j]) > pulseThreshold) {
      pulses[j] = true;
      digitalWrite(ledPins[j],HIGH);
    } else {
      pulses[j] = false;
      digitalWrite(ledPins[j],LOW);
    }

    if (pulses[j] != lastPulses[j]) {
      if (pulses[j] == true) {
        if (millis() - lastBeat[j]<longestTime){
          // put time to array on the last position
          writeToArray(j, millis() - lastBeat[j]);
          lastPulseTimes[j] = millis();
          calculateHeartbeat(j);
        }
        lastBeat[j]=millis();
      }
    }
    lastPulses[j] = pulses[j];
  }
}

void writeToArray(int theIndex, long newValue) {
  for (int i = 0; i < beatCount - 1; i++) {
    pulseTimes[theIndex][i] = pulseTimes[theIndex][i + 1];
  }
  pulseTimes[theIndex][beatCount - 1] = newValue;
}

void calculateHeartbeat(int theIndex) {
  // Mittelwert aus zehn Sekunden mal sechs
  float average = 0.0;
  for (int i = 0; i < beatCount; i++) {
    average += pulseTimes[theIndex][i];
  }
  average = average / beatCount;
  bpm[theIndex] = (60.0 / average * 1000);

}
void sendBPMs() {
  Serial.print("BPM1 = "); Serial.print(bpm[0]);
  Serial.print("\t");
  Serial.print("BPM2 = "); Serial.println(bpm[1]);
}

void printValues() {
  for (int i = 0; i < 2; i++) {
    for (int j = 3; j < beatCount; j++) {
      Serial.print(pulseTimes[i][j]); Serial.print("\t");
    }
    Serial.print(bpm[0]);
    Serial.print("\t"); Serial.print("\t");
    Serial.print(bpm[1]); Serial.print("\t");
  }
  Serial.println("");
}
