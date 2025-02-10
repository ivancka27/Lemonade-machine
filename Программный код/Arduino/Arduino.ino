#include <iarduino_KB.h>            // Подключаем библиотеку для работы с матричной клавиатурой
#include <LiquidCrystal.h> // подключаем библиотеку для дисплея
#include <iarduino_HC_SR04_int.h> //подключаем библиотеку для датчиков

byte bukva_B[8]   = {B11110,B10000,B10000,B11110,B10001,B10001,B11110,B00000,}; // Буква "Б"+
byte bukva_G[8]   = {B11111,B10001,B10000,B10000,B10000,B10000,B10000,B00000,}; // Буква "Г"
byte bukva_D[8]   = {B01111,B00101,B00101,B01001,B10001,B11111,B10001,B00000,}; // Буква "Д"
byte bukva_ZH[8]  = {B10101,B10101,B10101,B11111,B10101,B10101,B10101,B00000,}; // Буква "Ж"
byte bukva_Z[8]   = {B01110,B10001,B00001,B00010,B00001,B10001,B01110,B00000,}; // Буква "З"+
byte bukva_I[8]   = {B10001,B10011,B10011,B10101,B11001,B11001,B10001,B00000,}; // Буква "И"+
byte bukva_L[8]   = {B00011,B00111,B00101,B00101,B01101,B01001,B11001,B00000,}; // Буква "Л"+
byte bukva_P[8]   = {B11111,B10001,B10001,B10001,B10001,B10001,B10001,B00000,}; // Буква "П"+
byte bukva_Y[8]   = {B10001,B10001,B10001,B01010,B00100,B01000,B10000,B00000,}; // Буква "У"
byte bukva_F[8]   = {B00100,B11111,B10101,B10101,B11111,B00100,B00100,B00000,}; // Буква "Ф"
byte bukva_TS[8]  = {B10010,B10010,B10010,B10010,B10010,B10010,B11111,B00001,}; // Буква "Ц"
byte bukva_CH[8]  = {B10001,B10001,B10001,B01111,B00001,B00001,B00001,B00000,}; // Буква "Ч"
byte bukva_Sh[8]  = {B10101,B10101,B10101,B10101,B10101,B10101,B11111,B00000,}; // Буква "Ш"
byte bukva_Shch[8]= {B10101,B10101,B10101,B10101,B10101,B10101,B11111,B00001,}; // Буква "Щ"
byte bukva_Mz[8]  = {B10000,B10000,B10000,B11110,B10001,B10001,B11110,B00000,}; // Буква "Ь"
byte bukva_IYI[8] = {B10001,B10001,B10001,B11001,B10101,B10101,B11001,B00000,}; // Буква "Ы"+
byte bukva_Yu[8]  = {B10010,B10101,B10101,B11101,B10101,B10101,B10010,B00000,}; // Буква "Ю"
byte bukva_Ya[8]  = {B01111,B10001,B10001,B01111,B00101,B01001,B10001,B00000,}; // Буква "Я"

iarduino_KB KB(8,7,6,5,4,3,2);  // Объявляем переменную для работы с матричной клавиатурой, указывая номера arduino подключенные к шлейфу клавиатуры, начиная с первого вывода шлейфа.
LiquidCrystal lcd(14, 15, 16, 17, 18, 19);   // пины подклчюеиня дисплея
const int pump1Pin = 10; // управлеине реле для насоса 1 на 10 пине
const int pump2Pin = 11; // управлеине реле для насоса 2 на 11 пине
const int pump3Pin = 12; // управлеине реле для насоса 3 на 12 пине
constexpr uint8_t PIN_SENSOR_TRIG_1 = 22; //отправка сигнала на 22 пине
constexpr uint8_t PIN_SENSOR_ECHO_1 = 20; //прием сигнала на 20 пине
constexpr uint8_t PIN_SENSOR_TRIG_2 = 24; //отправка сигнала на 24 пине
constexpr uint8_t PIN_SENSOR_ECHO_2 = 21; //прием сигнала на 21 пине
iarduino_HC_SR04_int sensor1(PIN_SENSOR_TRIG_1, PIN_SENSOR_ECHO_1); // пины для сенсора 1
iarduino_HC_SR04_int sensor2(PIN_SENSOR_TRIG_2, PIN_SENSOR_ECHO_2); // пины для сенсора 2
// Enable: программное включение
// и отключение питания от двигателя
constexpr uint8_t EN_PIN = 7;
// Direction: управление направлением
// вращение шагового мотора
constexpr uint8_t DIR_PIN = 10;
// Step: шаг двигателя
constexpr uint8_t STEP_PIN = 6;
const int analogInPin = A5;               // Определяем номер вывода, к которому подключен модуль Холла
int sensorValue = 0;                      // Задаем переменную для значений, полученных с модуля
const double  ml_in_ms= 1.65; //константа накачивания насоса мл в секунду
const int distanceToCup = 4; //для датчика у налития
const int distanceToCup2 = 4; //для датчика у выдачи
// Выдержка для регулировки скорости вращения
// Диапазон значений от 800 до бесконечности
int delayTime = 1000;

void setup(){
   Serial.begin(9600);              // инициируем подключение к COM-порту на скорости 9600 бот
   KB.begin(KB3); // Инициализируем клавиатуру тип KB3 10+2
   pinMode(pump1Pin, OUTPUT);                   // пин для реле насоса газ вода
   pinMode(pump2Pin, OUTPUT);                   // пин для реле насоса сироп мятный
   pinMode(pump3Pin, OUTPUT);                   // пин для реле насоса апельсиновый сок

   // Настраиваем пины мотора на выход
  pinMode(EN_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  // Подаём питания на обмотки двигателя
  digitalWrite(EN_PIN, LOW);

    lcd.begin(16, 2);                 //  Инициируем работу с LCD дисплеем, указывая количество (столбцов, строк)
    lcd.createChar(1, bukva_P);      // Создаем символ под номером 1 = П
    lcd.createChar(2, bukva_I);      // Создаем символ под номером 2 = И
    lcd.createChar(3, bukva_IYI);      // Создаем символ под номером 3 = Ы
    lcd.createChar(4, bukva_B);      // Создаем символ под номером 4 = Б
    lcd.createChar(5, bukva_L);      // Создаем символ под номером 5 = Л
    lcd.createChar(6, bukva_Z);      // Создаем символ под номером 6 = З
   lcd.begin(16, 2);                 //  Инициируем работу с LCD дисплеем, указывая количество (столбцов, строк)
    lcd.setCursor(0, 0);              //  Устанавливаем курсор в позицию (0 столбец, 0 строка)
    lcd.print("B\3\4EP\2TE HA\1\2TK\2"); // Выводим приветственное сообщение "Выбирите напитки"
}

// блок функций

void runPump(int pumpPin, int volume) { // функция наливания. принемает номер насоса(реле) и объем воды
  int currentTime = millis(); //объявление времени, когда получена команда наливать
  digitalWrite(pumpPin, LOW); // наливаем порциями по 50мл, т.к. если больше - то залипает реле
  while (volume > 50){
    volume = volume - 50;
    int time = (50/ml_in_ms) * 1000;
    while (millis()- currentTime < 100 & millis()-currentTime < time & time > 0){
      int sensorDistance = sensor1.distance(); 
      if (sensorDistance > distanceToCup){
        currentTime = currentTime + cupIsNotInPlace(pumpPin);
      }
      if (millis()- currentTime > 100){
        currentTime = millis();
        time = time - 100;
      }
    }
  }
  int time2 = (volume/ml_in_ms) * 1000; // доливаем остаток
  while (millis()- currentTime < 100 & millis()-currentTime < time2 & time2 > 0){
    int sensorDistance = sensor1.distance(); 
      if (sensorDistance > distanceToCup){
        currentTime = currentTime + cupIsNotInPlace(pumpPin);
      }
      if (millis()- currentTime > 100){
        currentTime = millis();
        time2 = time2 - 100;
      }
    } 
  digitalWrite(pumpPin, HIGH);
}

int cupIsNotInPlace(int pumpPin) {
  int timer = millis();
  digitalWrite(pumpPin, HIGH);
  Serial.print(" Стакан не на месте");
  int sensorDistance = sensor1.distance();
  while(sensorDistance > distanceToCup){
    sensorDistance = sensor1.distance(); 
    if(sensorDistance < distanceToCup){
      digitalWrite(pumpPin, LOW);
      Serial.print(" Стакан на месте");
      timer = millis() - timer;
    }
  }
  return timer;
}




void loop(){  //  основная часть
int value;
  digitalWrite(pump1Pin, HIGH);   // чтоб реле не включалось при старте
  digitalWrite(pump2Pin, HIGH);   // чтоб реле не включалось при старте
  digitalWrite(pump3Pin, HIGH);   // чтоб реле не включалось при старте

  int order[4]= {0,0,0,0}; // Массив с выбором напитков
  for (int i = 0; i < 4; i++){        // считываем нажатия кнопок с клавиатуры
    
    while(!KB.check(KEY_UP)) { // тут проблема с логикой обработки - мы полюбому ждем что на клавиатуру что-то нажмут и только после этого начинаем делать
      value = KB.getNum;
      if((value >0 && value <7) || value == 15) { // фильтруем ввод с клавы
        Serial.print(" i=");
        Serial.print(i);
        Serial.print(" знач=");
        Serial.println(value); // отладка
        lcd.setCursor(0, 0);              //  Устанавливаем курсор в позицию (0 столбец, 0 строка)
        lcd.print("B\3\4PAH\3:"); // Выводим  сообщение "Выбраны:"
        lcd.setCursor(9+i*2, 0); lcd.print(value);             //  Устанавливаем курсор в позицию (0 столбец, 0 строка)
        lcd.setCursor(10+i*2, 0); lcd.print("_");             //  Устанавливаем курсор в позицию (0 столбец, 0 строка)
      if(value == 15) { goto breakAll;}  // нажата решетка -  прерываемся       
        order[i] = value; // записываем выбор в массив

      }
    }
  }
  breakAll:
  Serial.println("Завешение цикла выбора"); // отладка

  Serial.print(order[0]); // отладка
  Serial.print(order[1]); // отладка
  Serial.print(order[2]); // отладка
  Serial.print(order[3]); // отладка
  
  lcd.clear();
  lcd.print("B\3\4PAH\3:"); // Выводим  сообщение "Выбраны"
  lcd.setCursor(9, 0); lcd.print(order[0]); // Выводим номер первого
  if (order[1] >0){lcd.setCursor(11, 0); lcd.print(order[1]);} // // Выводим номер второго
  if (order[1] >0){lcd.setCursor(13, 0); lcd.print(order[2]);} // // Выводим номер третьего
  if (order[1] >0){lcd.setCursor(15, 0); lcd.print(order[3]);} // // Выводим номер четвертого

  
  for (int j = 0; j < 4; j++){  // наливаем - добавить проверку наличия стакана по датчику + между наливками делать поворот моторна на N градусов для следующего стакана
    switch(order[j]) {
      case 1:
        Serial.println("наливаем 1");
        lcd.setCursor(0, 1); lcd.print("HA\5\2BAEM:"); lcd.setCursor(10,1);  lcd.print(order[j]); // Выводим номер напитка, который наливаем сейчас
        runPump(pump1Pin, 50);
        break;
      case 2:
        Serial.println("наливаем 2");
        lcd.setCursor(0, 1); lcd.print("HA\5\2BAEM:"); lcd.setCursor(10,1);  lcd.print(order[j]); // Выводим номер напитка, который наливаем сейчас
        runPump(pump2Pin, 10);
        break;
      case 3:
        Serial.println("наливаем 3");
        lcd.setCursor(0, 1); lcd.print("HA\5\2BAEM:"); lcd.setCursor(10,1);  lcd.print(order[j]); // Выводим номер напитка, который наливаем сейчас
        runPump(pump3Pin, 40);
        break;
      case 4:
        Serial.println("наливаем 4");
        lcd.setCursor(0, 1); lcd.print("HA\5\2BAEM:"); lcd.setCursor(10,1);  lcd.print(order[j]); // Выводим номер напитка, который наливаем сейчас
        runPump(pump1Pin, 80);
        runPump(pump2Pin, 20);
        break;
      case 5:
        Serial.println("наливаем 5");
        lcd.setCursor(0, 1); lcd.print("HA\5\2BAEM:"); lcd.setCursor(10,1);  lcd.print(order[j]); // Выводим номер напитка, который наливаем сейчас
        runPump(pump1Pin, 30);
        runPump(pump3Pin, 50);
        break;
      case 6:
        Serial.println("наливаем 6");
        lcd.setCursor(0, 1); lcd.print("HA\5\2BAEM:"); lcd.setCursor(10,1);  lcd.print(order[j]); // Выводим номер напитка, который наливаем сейчас
        runPump(pump1Pin, 35);
        runPump(pump2Pin, 10);
        runPump(pump3Pin, 45);
        break;
      default:
        Serial.println("ERROR!");
        lcd.setCursor(0, 1); lcd.print("ERROR!"); // Вывод "ERROR"
      break;
    }
    // Задаём направления мотора по часовой стрелке
    digitalWrite(DIR_PIN, HIGH);
    // Делаем 5000 шагов в заданном направлении
    for (int i = 0; i < 5000; ++i) {
      digitalWrite(STEP_PIN, HIGH);
      delayMicroseconds(delayTime);
      digitalWrite(STEP_PIN, LOW);
      delayMicroseconds(delayTime);
    }
  }
  
  int sensorDistance2 = sensor2.distance();

  //движем первый стаканчик до зоны выдачи
  while(sensorDistance2 != distanceToCup2){
    sensorDistance2 = sensor2.distance();
    for (int i = 0; i < 5000; ++i) {
      digitalWrite(STEP_PIN, HIGH);
      delayMicroseconds(delayTime);
      digitalWrite(STEP_PIN, LOW);
      delayMicroseconds(delayTime);
    }
  }

  //ждём забирание стаканчика
  while(sensorDistance2 < distanceToCup2){
    sensorDistance2 = sensor2.distance();
    if(sensorDistance2 > distanceToCup2){
      for (int i = 0; i < 5000; ++i) {
        digitalWrite(STEP_PIN, HIGH);
        delayMicroseconds(delayTime);
        digitalWrite(STEP_PIN, LOW);
        delayMicroseconds(delayTime);
      }
    }
  }

  if(order[1] > 0){ //если есть в заказе второй напиток
    //движем второй стаканчик до зоны выдачи
    while(sensorDistance2 != distanceToCup2){
      for (int i = 0; i < 5000; ++i) {
        digitalWrite(STEP_PIN, HIGH);
        delayMicroseconds(delayTime);
        digitalWrite(STEP_PIN, LOW);
        delayMicroseconds(delayTime);
      }
    }

    //ждём забирание стаканчика
    while(sensorDistance2 < distanceToCup2){
      sensorDistance2 = sensor2.distance();
      if(sensorDistance2 > distanceToCup2){
        for (int i = 0; i < 5000; ++i) {
          digitalWrite(STEP_PIN, HIGH);
          delayMicroseconds(delayTime);
          digitalWrite(STEP_PIN, LOW);
          delayMicroseconds(delayTime);
        }
      }
    }
  }

  if(order[2] > 0){ //если есть в заказе третий напиток
    //движем третий стаканчик до зоны выдачи
    while(sensorDistance2 != distanceToCup2){
      for (int i = 0; i < 5000; ++i) {
        digitalWrite(STEP_PIN, HIGH);
        delayMicroseconds(delayTime);
        digitalWrite(STEP_PIN, LOW);
        delayMicroseconds(delayTime);
      }
    }

    //ждём забирание стаканчика
    while(sensorDistance2 < distanceToCup2){
      sensorDistance2 = sensor2.distance();
      if(sensorDistance2 > distanceToCup2){
        for (int i = 0; i < 5000; ++i) {
          digitalWrite(STEP_PIN, HIGH);
          delayMicroseconds(delayTime);
          digitalWrite(STEP_PIN, LOW);
          delayMicroseconds(delayTime);
        }
      }
    }
  }

  if(order[3] > 0){ //если есть в заказе четвёртый напиток
    //движем четвёртый стаканчик до зоны выдачи
    while(sensorDistance2 != distanceToCup2){
      sensorDistance2 = sensor2.distance();
      for (int i = 0; i < 5000; ++i) {
        digitalWrite(STEP_PIN, HIGH);
        delayMicroseconds(delayTime);
        digitalWrite(STEP_PIN, LOW);
        delayMicroseconds(delayTime);
      }
    }

    //ждём забирание стаканчика
    while(sensorDistance2 < distanceToCup2){
      sensorDistance2 = sensor2.distance();
      if(sensorDistance2 > distanceToCup2){
        for (int i = 0; i < 5000; ++i) {
          digitalWrite(STEP_PIN, HIGH);
          delayMicroseconds(delayTime);
          digitalWrite(STEP_PIN, LOW);
          delayMicroseconds(delayTime);
        }
      }
    }
  }

  Serial.println("заказ закончен");
  lcd.clear(); lcd.print("\6AKA\6 B\3\1O\5HEH"); // Вывод "Заказ выполнен"
  
  sensorValue = analogRead(analogInPin);  // Записываем в переменную значения с модуля
  digitalWrite(STEP_PIN, HIGH);
  while(sensorValue > 500){
    sensorValue = analogRead(analogInPin);
    if(sensorValue < 500){
      digitalWrite(STEP_PIN, LOW);
    }
  }

  delay(8000);
  // возвращаемся к исходному состоянию
  lcd.clear();lcd.print("B\3\4EP\2TE HA\1\2TK\2"); // Выводим приветственное сообщение "Выберите напитки"
}