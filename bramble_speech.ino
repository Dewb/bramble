
#define PHRASE_MAX_LENGTH 25
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

int light_on_time = 40;
int light_total_time = 200;
int heavy_on_time = 80;
int heavy_total_time = 240;
int heavier_on_time = 100;
int heavier_total_time = 240;
int heaviest_on_time = 140;
int heaviest_total_time = 240;
int dbllight_on_time = 40;
int dbllight_gap_time = 30;
int dbllight_total_time = 240;
int caesura_time = 400;
int thinking_time_min = 1500;
int thinking_time_max = 5500;
int refill_time = 5000;

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

void create_rhythm_saturnian(char* phrase) {
  // e.g. Old Latin, Livius Andronicus 
  char index = 0;
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
  phrase[index++] = PHRASE_END;
}

void create_rhythm_iambic_pentameter(char* phrase) {
  // e.g. Shakespeare, John Donne
  char index = 0;
  for (int line = 0; line < 2; line++) {
    if (coinflip()) {
      phrase[index++] = SYL_LIGHT;
      phrase[index++] = pickFromTwo(SYL_HEAVY, SYL_HEAVIER);
    } else {
      phrase[index++] = pickFromTwo(SYL_HEAVY, SYL_HEAVIER);
      phrase[index++] = SYL_LIGHT;
    }
    phrase[index++] = SYL_LIGHT;
    phrase[index++] = pickFromThree(SYL_HEAVY, SYL_HEAVIER, SYL_HEAVIEST);
    if (coinflip()) {
      phrase[index++] = SYL_CAESURA;
    }
    phrase[index++] = pickFromTwo(SYL_LIGHT, SYL_HEAVY);
    phrase[index++] = pickFromTwo(SYL_HEAVY, SYL_HEAVIER);
    phrase[index++] = pickFromThree(SYL_LIGHT, SYL_LIGHT, SYL_DBLLIGHT);
    phrase[index++] = pickFromThree(SYL_HEAVY, SYL_HEAVIER, SYL_HEAVIEST);
    phrase[index++] = pickFromThree(SYL_LIGHT, SYL_LIGHT, SYL_DBLLIGHT);
    phrase[index++] = pickFromThree(SYL_HEAVY, SYL_HEAVIER, SYL_HEAVIEST);
    phrase[index++] = SYL_CAESURA;
  }
  phrase[index++] = PHRASE_END;
}

void create_rhythm_common_metre(char* phrase) {
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
      create_rhythm_saturnian(phrase);
    } else if (meterChoice == 1) {
      create_rhythm_iambic_pentameter(phrase);
    } else {
      create_rhythm_iambic_pentameter(phrase);
    }
    
    int i = 0;
    while (phrase[i] != PHRASE_END && i < PHRASE_MAX_LENGTH) {
      switch (phrase[i]) {
      case SYL_LIGHT:
        {
          toggleSolenoid(light_on_time, light_total_time - light_on_time);
        }
        break;
      case SYL_DBLLIGHT:
        {
          toggleSolenoid(dbllight_on_time, dbllight_gap_time);
          toggleSolenoid(dbllight_on_time, dbllight_total_time - 2 * dbllight_on_time - dbllight_gap_time);
        }
        break;
      case SYL_HEAVY:
        {
          toggleSolenoid(heavy_on_time, heavy_total_time - heavy_on_time);
        }
        break;
      case SYL_HEAVIER:
        {
          toggleSolenoid(heavier_on_time, heavier_total_time - heavier_on_time);          
        }
        break;
      case SYL_HEAVIEST:
        {
          toggleSolenoid(heaviest_on_time, heaviest_total_time - heaviest_on_time);
        }
        break;
      case SYL_CAESURA:
        {
          delay(caesura_time);
        }
        break;
      }
      i++;
    }
    
    delay(refill_time);
    digitalWrite(BUTTON_LED_PIN, HIGH);
  }
}
