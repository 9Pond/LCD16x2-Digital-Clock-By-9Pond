#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int day = 0;
int month = 0;
int year = 2000;

int hour = 0;
int minute = 0;
int second = 0;

bool editMode = false;
int editField = 0;

unsigned long lastTick = 0;
unsigned long blinkTimer = 0;
bool blinkState = true;

int readKey() {
  int adc = analogRead(A0);

  if (adc < 50) return 0;
  if (adc < 195) return 1;
  if (adc < 380) return 2;
  if (adc < 555) return 3;
  if (adc < 790) return 4;

  return 5;
}

int daysInMonth(int m, int y) {
  switch (m) {
    case 4:
    case 6:
    case 9:
    case 11:
      return 30;

    case 2:
      if ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0))
        return 29;
      return 28;

    default:
      return 31;
  }
}

void setup() {
  lcd.begin(16, 2);
}

void loop() {

  if (millis() - blinkTimer >= 500) {
    blinkTimer = millis();
    blinkState = !blinkState;
  }

  if (!editMode) {

    if (millis() - lastTick >= 1000) {
      lastTick += 1000;

      second++;

      if (second >= 60) {
        second = 0;
        minute++;
      }

      if (minute >= 60) {
        minute = 0;
        hour++;
      }

      if (hour >= 24) {
        hour = 0;
        day++;
      }

      if (day > daysInMonth(month, year)) {
        day = 1;
        month++;
      }

      if (month > 12) {
        month = 1;
        year++;
      }
    }
  }

  int key = readKey();

  if (key == 4) {
    delay(250);

    if (!editMode) {
      editMode = true;
      editField = 0;
    } else {
      editMode = false;
      lastTick = millis();
    }
  }

  if (editMode) {

    if (key == 0) {
      editField++;
      if (editField > 5) editField = 0;
      delay(200);
    }

    if (key == 3) {
      editField--;
      if (editField < 0) editField = 5;
      delay(200);
    }

    if (key == 1) {

      switch (editField) {

        case 0:
          day++;
          if (day > daysInMonth(month, year))
            day = 1;
          break;

        case 1:
          month++;
          if (month > 12)
            month = 1;
          break;

        case 2:
          year++;
          if (year > 2099)
            year = 2000;
          break;

        case 3:
          hour++;
          if (hour > 23)
            hour = 0;
          break;

        case 4:
          minute++;
          if (minute > 59)
            minute = 0;
          break;
        case 5:
          second++;
          if(second > 59) second = 0;
          break;
      }

      delay(150);
    }

    if (key == 2) {

      switch (editField) {

        case 0:
          day--;
          if (day < 1)
            day = daysInMonth(month, year);
          break;

        case 1:
          month--;
          if (month < 1)
            month = 12;
          break;

        case 2:
          year--;
          if (year < 2000)
            year = 2099;
          break;

        case 3:
          hour--;
          if (hour < 0)
            hour = 23;
          break;

        case 4:
          minute--;
          if (minute < 0)
            minute = 59;
          break;
        case 5:
          second--;
          if (second < 0) second = 59;
          break;
      }

      delay(150);
    }
  }

  char line1[17];
  char line2[17];

  sprintf(line1, "%02d/%02d/%04d", day, month, year);
  sprintf(line2, "%02d:%02d:%02d", hour, minute, second);

  if (editMode && !blinkState) {

    switch (editField) {

      case 0:
        sprintf(line1, "  /%02d/%04d", month, year);
        break;

      case 1:
        sprintf(line1, "%02d/  /%04d", day, year);
        break;

      case 2:
        sprintf(line1, "%02d/%02d/    ", day, month);
        break;

      case 3:
        sprintf(line2, "  :%02d:%02d", minute, second);
        break;

      case 4:
        sprintf(line2, "%02d:  :%02d", hour, second);
        break;
      case 5:
        sprintf(line2, "%02d:%02d:" , hour, minute);
        break;
    }
  }

  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 0);
  lcd.print(line1);

  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print(line2);

  delay(20);
}