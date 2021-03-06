/*
   Пример использования библиотеки GyverButton, все возможности в одном скетче.
   - Опрос кнопки с программным антидребезгом контактов
   - Отработка нажатия, удерживания отпускания кнопки
   - Отработка одиночного, двойного и тройного нажатия (вынесено отдельно)
   - Отработка любого количества нажатий кнопки (функция возвращает число нажатий)
   - Отработка нажатия и удержания кнопки
   - Настраиваемый таймаут повторного нажатия/удержания
   - Функция изменения значения переменной с заданным шагом и заданным интервалом по времени
*/

#define PIN 3				// кнопка подключена сюда (PIN --- КНОПКА --- GND)

#include "GyverButton.h"
GButton butt1(PIN);
int value = 0;

void setup() {
  Serial.begin(9600);

  butt1.setDebounce(50);        // настройка антидребезга (по умолчанию 80 мс)
  butt1.setTimeout(300);        // настройка таймаута на удержание (по умолчанию 500 мс)
  butt1.setIncrStep(2);         // настройка инкремента, может быть отрицательным (по умолчанию 1)
  butt1.setIncrTimeout(500);    // настрйока интервала инкремента (по умолчанию 800 мс)
}

void loop() {
  butt1.tick();  // обязательная функция отработки. Должна постоянно опрашиваться

  if (butt1.isSingle()) Serial.println("Single");       // проверка на один клик
  if (butt1.isDouble()) Serial.println("Double");       // проверка на двойной клик
  if (butt1.isTriple()) Serial.println("Triple");       // проверка на тройной клик

  if (butt1.hasClicks())                                // проверка на наличие нажатий
    Serial.println(butt1.getClicks());                  // получить (и вывести) число нажатий

  if (butt1.isPress()) Serial.println("Press");         // нажатие на кнопку (+ дебаунс)
  if (butt1.isRelease()) Serial.println("Release");     // отпускание кнопки (+ дебаунс)
  if (butt1.isHolded()) Serial.println("Holded");       // проверка на удержание
  //if (butt1.isHold()) Serial.println("Hold");         // возвращает состояние кнопки

  if (butt1.isIncr()) {                                 // если кнопка была удержана (это для инкремента)
    value = butt1.getIncr(value);                       // увеличивать/уменьшать переменную value с шагом и интервалом
    Serial.println(value);      // для примера выведем в порт
  }
}
