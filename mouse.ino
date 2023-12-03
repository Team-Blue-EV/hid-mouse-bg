// библиотека която емулира мишка
#include "Mouse.h"

// set pin numbers for switch, joystick axes, and LED:
// const int switchPin = 2;    // switch to turn on and off mouse control
// const int mouseButton = 3;  // input pin for the mouse pushButton

const int mouseClick = A0;  // Вход от сензора за духане
const int xAxis = A1;       // joystick X axis пин към Ардуино за ос X 
const int yAxis = A2;       // joystick Y axis пин към Ардуино за ос Y
//const int ledPin = 5;       // Mouse control LED

int InitBlow = 0;     // инициализация сензора за духане
int BlowRange=200;    // праг на духане

// параметри на joystick, те служат за калибриране:
int range = 8;
int responseDelay = 5;      // време за забавяне, в ms
int threshold = range / 4;  // resting threshold
int center = range / 2;     // resting position value

bool mouseIsActive = true;  // Активиране на мишката
int lastSwitchState = LOW;  // в момента не се използва

void setup() {
  // не се използва
  /*
  pinMode(switchPin, INPUT);  // the switch pin
  pinMode(ledPin, OUTPUT);    // the LED pin
  */

  // Прочитане на аналоговия сензор за душане
  InitBlow = analogRead (mouseClick);

  // Инициализиране на сериния изход
  Serial.begin(9600);

  // Инициализиране на библиотеката на мишката
  Mouse.begin();

  // Разпечатване на стойноста на сензора.
  Serial.println(InitBlow);

}

void loop() {
  // Прочитане на ключа, тука ти не го ползвах, затова е коментирано
  /*
  int switchState = digitalRead(switchPin);
  if (switchState != lastSwitchState) {
    if (switchState == HIGH) {
      digitalWrite(ledPin, mouseIsActive);
    }
  }
  lastSwitchState = switchState;
  */
  
  // Четеме кординати на джойстика за X,Y
  int xReading = readAxis(xAxis);
  int yReading = readAxis(yAxis);

  // Ако флаг mouseIsActive е TRUE преместваме мишката, той по подразбиране е TRUE
  if (mouseIsActive) {
    Mouse.move(xReading, yReading, 0);
  }

  // Прочитаме стойноста на душащия сенсор
  int temp =analogRead(mouseClick);

  // ако стойноста (temp - BlowRange) > InitBlow, тогава налягането е по-голямо и натискаме левия бутон на мишката 
  if ((temp - BlowRange) > InitBlow) {
    // Проверяваме дали бутона е натиснат, ако не е го натискаме.
    if (!Mouse.isPressed(MOUSE_LEFT)) {
      / казваме на мишката че бутона е натиснат
      Mouse.press(MOUSE_LEFT);
    }
  }
  // ако налягането е по-малко
  else {
    // проверяваме дали бутона е натиснат, ако е натиснат, казваме на мишката че бутона се освобождава
    if (Mouse.isPressed(MOUSE_LEFT)) {
      // казваме на мишката че бутона се освобождава
      Mouse.release(MOUSE_LEFT);
    }
  }

  // Правим едно забавяне от порядъка на 5 мили секунди
  delay(responseDelay);
}

// Функция която чете дани от джоистика
int readAxis(int thisAxis) {
  // Прочита аналогия пин
  int reading = analogRead(thisAxis);

  // преобразува стойноста reading, тука range го задаваме в началото на програмата, тя служи като калибриране
  reading = map(reading, 0, 1023, 0, range);

  // определяне на положението
  int distance = reading - center;

  if (abs(distance) < threshold) {
    distance = 0;
  }

  // връщане на стойноста:
  return distance;
}
