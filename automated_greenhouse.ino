#include <LCD_I2C.h>
LCD_I2C lcd(0x27, 16, 2);  // Default address of most PCF8574 modules, change according

#define GLED 3
#define RLED 2
#define PUMP 12

#define PLUS_BUTTON 5
#define MIN_BUTTON 4

#define HUM A2
#define HUM_POWER 7
#define LEVEL A1
#define LEVEL_POWER 10

//practic fct asta permite sa nu corodem senzorul de nvl
int readLevel() {
  digitalWrite(LEVEL_POWER, HIGH);  // Turn the sensor ON
  delay(10);                        // wait 10 milliseconds
  int val = analogRead(LEVEL);      // Read the analog value form sensor
  digitalWrite(LEVEL_POWER, LOW);   // Turn the sensor OFF
  return val;                       // send current reading
}

// la fel dar pt soil senzor
int readHum() {
  digitalWrite(HUM_POWER, HIGH);  // Turn the sensor ON
  delay(10);                      // wait 10 milliseconds
  int val = analogRead(HUM);      // Read the analog value form sensor
  digitalWrite(HUM_POWER, LOW);   // Turn the sensor OFF
  return val;                     // send current reading
}

int target_hum = 50;

void setup() {

  pinMode(PLUS_BUTTON, INPUT_PULLUP);
  pinMode(MIN_BUTTON, INPUT_PULLUP);

  pinMode(GLED, OUTPUT);
  pinMode(RLED, OUTPUT);
  pinMode(PUMP, OUTPUT);

  digitalWrite(GLED, LOW);
  digitalWrite(RLED, LOW);
  digitalWrite(PUMP, LOW);
  //senzor hum
  pinMode(HUM, INPUT);
  pinMode(HUM_POWER, OUTPUT);
  digitalWrite(HUM_POWER, LOW);
  //senzor nvl apa
  pinMode(LEVEL, INPUT);
  pinMode(LEVEL_POWER, OUTPUT);
  digitalWrite(LEVEL_POWER, LOW);

  //pornim ecranul
  lcd.begin();      // If you are using more I2C devices using the Wire library use lcd.begin(false)
  lcd.backlight();  //back light on
}

void loop() {
  int hum = readHum();
  int level = readLevel();
  char sHum[16] = "";
  //convert int in str
  sprintf(sHum, "H:%2d%%  ", hum);

  if (isnan(hum) || isnan(level)) {  // Is Not A Number
    //verif daca senzorele merg
    lcd.clear();
    lcd.setCursor(0, 0);  //  setting the cursor in the desired position.
    lcd.print("check senzors");
    digitalWrite(RLED, HIGH);
    digitalWrite(GLED, LOW);
    while (isnan(hum) || isnan(level)) {
      delay(1);
    }
  }
  digitalWrite(RLED, LOW);
  digitalWrite(GLED, HIGH);

  lcd.clear();
  lcd.setCursor(0, 0);  //  setting the cursor in the desired position.
  lcd.print(sHum);

  lcd.setCursor(1, 0);  //  setting the cursor in the desired position.
  lcd.print("target:");
  lcd.print(target_hum);

  if (digitalRead(PLUS_BUTTON) == LOW && target_hum < 100) {
    target_hum += 5;
  }

  if (digitalRead(MIN_BUTTON) == LOW && target_hum > 0) {
    target_hum -= 5;
  }

  if (map(hum, 0, 1023, 100, 0) > target_hum)
    digitalWrite(PUMP, LOW);
  else
    digitalWrite(PUMP, HIGH);

  delay(1000);
}