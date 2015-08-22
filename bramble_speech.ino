
#define PHRASE_MAX_LENGTH 60
char phrase[PHRASE_MAX_LENGTH];

enum syllable {
  PHRASE_END = 0,
  SYL_LIGHT,
  SYL_DBLLIGHT,
  SYL_HEAVY,
  SYL_HEAVIER,
  SYL_HEAVIEST,
  SYL_CAESURA
};

// milliseconds per syllable
// based on SPM of TED talks, from http://sixminutes.dlugan.com/speaking-rate/
int syllable_time_max = 400; 
//int syllable_time_max = 300; // 200 SPM (Al Gore)
//int syllable_time_min = 221; // 271 SPM (Jacqueline Novogratz)
int syllable_time_min = 240;

int light_intensity = 30;
int light_duration = 75;
int heavy_intensity = 55;
int heavy_duration = 100;
int heavier_intensity = 75;
int heavier_duration = 110;
int heaviest_intensity = 100;
int heaviest_duration = 140;
int caesura_duration = 270;

int solenoid_open_time = 200;
int thinking_time_min = 1500;
int thinking_time_max = 5500;
int refill_time = 2000;

enum modeType {
  MODE_SPEECH,
  MODE_MANUAL
};

modeType mode = MODE_SPEECH;

#define SOLENOID_PIN 4
#define BUTTON_PIN 2
#define BUTTON_LED_PIN 5

bool coinflip() {
  if (random(0, 1) == 0) {
    return true;
  } else {
    return false;
  }
}

int pickFromTwo(int a, int b) {
  return coinflip() ? a : b;
}

int pickFromThree(int a, int b, int c) {
  int r = random(0, 3);
  if (r == 0) { 
    return a;
  } else if (r == 1) { 
    return b;
  } else {
    return c;
  }
}

void create_rhythm_saturnian(char* phrase, int lines) {
  // e.g. Old Latin, Livius Andronicus 
  // https://en.wikipedia.org/wiki/Saturnian_(poetry)
  char index = 0;
  for (int line = 0; line < lines; line++) {
    phrase[index++] = pickFromThree(SYL_LIGHT, SYL_HEAVY, SYL_DBLLIGHT);
    phrase[index++] = pickFromTwo(SYL_HEAVY, SYL_HEAVIER);
    phrase[index++] = pickFromThree(SYL_LIGHT, SYL_HEAVY, SYL_DBLLIGHT);
    phrase[index++] = pickFromTwo(SYL_LIGHT, SYL_HEAVY);
    phrase[index++] = SYL_CAESURA;
    phrase[index++] = pickFromThree(SYL_LIGHT, SYL_HEAVY, SYL_DBLLIGHT);
    phrase[index++] = pickFromThree(SYL_HEAVY, SYL_HEAVIER, SYL_HEAVIEST);
    phrase[index++] = pickFromTwo(SYL_LIGHT, SYL_HEAVY);
    phrase[index++] = SYL_CAESURA;
    if (coinflip()) {
      phrase[index++] = SYL_HEAVY;
      phrase[index++] = SYL_LIGHT;
      phrase[index++] = pickFromTwo(SYL_LIGHT, SYL_HEAVY);
      phrase[index++] = SYL_CAESURA;
      phrase[index++] = pickFromThree(SYL_LIGHT, SYL_HEAVY, SYL_DBLLIGHT);
      phrase[index++] = pickFromThree(SYL_HEAVY, SYL_HEAVIER, SYL_HEAVIEST);
      phrase[index++] = pickFromTwo(SYL_LIGHT, SYL_HEAVY);
    } else {
      phrase[index++] = pickFromThree(SYL_LIGHT, SYL_HEAVY, SYL_DBLLIGHT);
      phrase[index++] = SYL_HEAVY;
      phrase[index++] = pickFromThree(SYL_LIGHT, SYL_HEAVY, SYL_DBLLIGHT);
      phrase[index++] = pickFromThree(SYL_HEAVY, SYL_HEAVIER, SYL_HEAVIEST);
      phrase[index++] = pickFromTwo(SYL_LIGHT, SYL_HEAVY);
    }
  }
  phrase[index++] = PHRASE_END;
}

void create_rhythm_iambic_pentameter(char* phrase, int lines) {
  // e.g. Shakespeare, John Donne
  // https://en.wikipedia.org/wiki/Iambic_pentameter
  char index = 0;
  for (int line = 0; line < lines; line++) {
    if (coinflip()) {
      phrase[index++] = SYL_LIGHT;
      phrase[index++] = pickFromTwo(SYL_HEAVY, SYL_HEAVIER);
    } else {
      phrase[index++] = pickFromTwo(SYL_HEAVY, SYL_HEAVIER);
      phrase[index++] = SYL_LIGHT;
    }
    phrase[index++] = SYL_LIGHT;
    phrase[index++] = pickFromThree(SYL_HEAVY, SYL_HEAVIER, SYL_HEAVIEST);
    if (random(0, 4) == 0) {
      phrase[index++] = SYL_CAESURA;
    }
    phrase[index++] = pickFromTwo(SYL_LIGHT, SYL_HEAVY);
    phrase[index++] = pickFromTwo(SYL_HEAVY, SYL_HEAVIER);
    phrase[index++] = pickFromThree(SYL_LIGHT, SYL_LIGHT, SYL_DBLLIGHT);
    phrase[index++] = pickFromThree(SYL_HEAVY, SYL_HEAVIER, SYL_HEAVIEST);
    phrase[index++] = pickFromThree(SYL_LIGHT, SYL_LIGHT, SYL_DBLLIGHT);
    phrase[index++] = pickFromThree(SYL_HEAVY, SYL_HEAVIER, SYL_HEAVIEST);
    if (random(0, 4) != 0) {
      phrase[index++] = SYL_CAESURA;
    }
  }
  phrase[index++] = PHRASE_END;
}

void create_rhythm_common_metre(char* phrase, int lines) {
  // e.g. John Newton, Edna St. Vincent Millay
}

void setup() {
  pinMode(SOLENOID_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  pinMode(BUTTON_LED_PIN, OUTPUT);
  
  digitalWrite(SOLENOID_PIN, LOW);
  digitalWrite(BUTTON_PIN, LOW);
  digitalWrite(BUTTON_LED_PIN, LOW);
}

void toggleSolenoid(int ms_on, int ms_off) {
  digitalWrite(SOLENOID_PIN, HIGH);
  delay(ms_on);
  digitalWrite(SOLENOID_PIN, LOW);
  delay(ms_off);
}

void speakSolenoidSyllable(int syllableNominalLength, int intensity, int duration) {
  int ms_on = solenoid_open_time * (intensity / 100.0);
  int ms_off = (duration / 100.0) * syllableNominalLength - ms_on;
  toggleSolenoid(ms_on, ms_off);
}

void loop() {
  if (mode == MODE_MANUAL) {
    if (digitalRead(BUTTON_PIN) == HIGH) {
      digitalWrite(SOLENOID_PIN, HIGH);
    } else {
      digitalWrite(SOLENOID_PIN, LOW);
    }
  } else if (mode == MODE_SPEECH) {
    
    while (digitalRead(BUTTON_PIN) == LOW) {
    }
    digitalWrite(BUTTON_LED_PIN, LOW);
    
    delay(thinking_time_min + random(0, thinking_time_max - thinking_time_min));
    
    randomSeed(analogRead(0));
    
    int meterChoice = random(0, 3);
    if (meterChoice == 0) {
      create_rhythm_saturnian(phrase, 2);
    } else if (meterChoice == 1) {
      create_rhythm_iambic_pentameter(phrase, 2);
    } else {
      create_rhythm_iambic_pentameter(phrase, 4);
    }
    
    int syllableLength = random(syllable_time_min, syllable_time_max);
    
    int i = 0;
    while (phrase[i] != PHRASE_END && i < PHRASE_MAX_LENGTH) {
      switch (phrase[i]) {
      case SYL_LIGHT:
        {
          speakSolenoidSyllable(syllableLength, light_intensity, light_duration);
        }
        break;
      case SYL_DBLLIGHT:
        {
          speakSolenoidSyllable(syllableLength, light_intensity, heavy_duration / 2);
          speakSolenoidSyllable(syllableLength, light_intensity, heavy_duration / 2);
        }
        break;
      case SYL_HEAVY:
        {
          speakSolenoidSyllable(syllableLength, heavy_intensity, heavy_duration);
        }
        break;
      case SYL_HEAVIER:
        {
          speakSolenoidSyllable(syllableLength, heavier_intensity, heavier_duration);
        }
        break;
      case SYL_HEAVIEST:
        {
          speakSolenoidSyllable(syllableLength, heaviest_intensity, heaviest_duration);
        }
        break;
      case SYL_CAESURA:
        {
          delay(caesura_duration / 100.0 * syllableLength);
        }
        break;
      }
      i++;
    }
    
    delay(refill_time);
    digitalWrite(BUTTON_LED_PIN, HIGH);
  }
}
