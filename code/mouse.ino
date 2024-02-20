// библиотека която емулира мишка
#include <Mouse.h>

const int sensorPin = A0;  // Вход от сензора за духане
const int xAxis = A1;       // joystick X axis пин към Ардуино за ос X 
const int yAxis = A2;       // joystick Y axis пин към Ардуино за ос Y


// параметри на joystick, те служат за калибриране:
int range = 8;
int responseDelay = 5;      // време за забавяне, в ms
int threshold = range / 4;  // resting threshold
int center = range / 2;     // resting position value

bool mouseIsActive = true;  // Активиране на мишката

unsigned long total = 0;
unsigned long numReadings = 0;
unsigned long previousMillis = 0;
unsigned long avarage = 0;
unsigned long tempValue = 0;

bool LeftMouse = false;
bool RightMouse = false;

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

void setup() {
  // Прочитане на аналоговия сензор за душане
  InitBlow = analogRead (sensorPin);

  // Инициализиране на сериния изход
  Serial.begin(9600);

  // Инициализиране на библиотеката на мишката
  Mouse.begin();

}

void loop() {
  // Четеме кординати на джойстика за X,Y
  int xReading = readAxis(xAxis);
  int yReading = readAxis(yAxis);

  // Ако флаг mouseIsActive е TRUE преместваме мишката, той по подразбиране е TRUE
  if (mouseIsActive) {
    Mouse.move(xReading, yReading, 0);
  }

  // прочитане на моментната стойност за налягането
  unsigned long sensorValue = analogRead(sensorPin);
  delay(3);

  // добавяме новата стойност към общата сума
  total += sensorValue;
  // вдигане на брояча
  numReadings++;
 
  // Проверка дали е изминало времето за проверка
  if ((millis() - previousMillis) >= 700) {
    previousMillis = millis();

    // намиране на средната стойност
    avarage=(long)(total/numReadings);

    // ако разликата в налягането е повече от 180 - силно духане клик десен
    if ((avarage-tempValue) >= 180) {
      LeftMouse=false;
      RightMouse=true;

    // ако разликата в налягането е повече от 60 - по слабо духане клик ляв
    }else if ((avarage-tempValue) >= 60) {
      LeftMouse=true;
      RightMouse=false;

    // нама десен лил ляв клик
    }else{
      LeftMouse=false;
      RightMouse=false;
    }

    // зануляване на стойностите
    tempValue=avarage;
    total=0;
    numReadings=0;
  }else{
    LeftMouse=false;
    RightMouse=false;
  }

  // ако флага е LeftMouse
  if (LeftMouse) {
    if (!Mouse.isPressed(MOUSE_LEFT)) {
      // казваме на мишката че бутона е натиснат
      Mouse.press(MOUSE_LEFT);
    }
    //Serial.println("LeftMouse");
  }

  // ако флага е RightMouse
  if (RightMouse) {
    if (!Mouse.isPressed(MOUSE_RIGHT)) {
      // казваме на мишката че бутона е натиснат
      Mouse.press(MOUSE_RIGHT);
    }
    //Serial.println("RightMouse");
  }

  
  // Правим едно забавяне от порядъка на 5 мили секунди
  delay(responseDelay);
}
