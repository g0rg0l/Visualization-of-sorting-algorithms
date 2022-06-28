# Визуализация работы алгоритмов сортировки

Темой проекта по предмету "Языки и методы программирования" была выбрана сфера работы алгоритмов сортировки одномерных массивов. Было принято решение продемонстрировать этот процесс наглядно. Весь представленный код был написан на языке **C**.

## Инструментарий

В написании проектной работы использовались следующие библиотеки:
  - *windows.h*
  - *OpenGL*
  - *string.h*
  - *stb_easy_font.h*

## Чтобы установить

Добавте в ваш Cmake-файл строчку линковки дефолтного пакета opengl с вашим проектом:

`target_link_libraries(NAME_OF_YOUR_PROJECT -lOpenGL32)`

## Описание интерфейса программы

Пользователя встречает интуитивно-понятный интерфейс, основанный на простых геометрических примитивах и пиксельном шрифте текста, которые неплохо сочетаются вместе.

![preview](https://github.com/g0rg0l/Visualization-of-sorting-algorithms/blob/master/imgs/1.jpg?raw=true)

Слева расположены функциональные кнопки, отвечающие за управление сортировщиком.

Немного правее располагаются кнопки управления скоростью сортировки, которую можно отслеживать и изменять прямо во время работы выбранного алгоритма.

Справа находятся 4 кнопки выбора сортировки. Были реализованы такие алгоритмы, как:
  - Пузырьковая сортировка ***(Bubble)***
  - Шейкер-сортировка ***(Shaker)***
  - Сортировка расчёской ***(Comb)***
  - Сортировка вставками ***(Insert)***

Немного ниже для каждого выбранного пользователем алгоритма представлена информация о его асимптотике - в худшем и в лучшем случаях.

## Управление

  - ***new array*** -> перезаполнение массива числами [1; 40]
  - ***begin sort*** -> запуск выбранной сортировки
  - ***stop sort*** -> остановка текущей сортировки
  - ***exit*** -> выход из программы

## Примеры работы всех четырёх алгоритмов

![bubble](https://github.com/g0rg0l/Visualization-of-sorting-algorithms/blob/master/imgs/bubble.gif?raw=true)

![shaker](https://github.com/g0rg0l/Visualization-of-sorting-algorithms/blob/master/imgs/shaker.gif?raw=true)

![comb](https://github.com/g0rg0l/Visualization-of-sorting-algorithms/blob/master/imgs/comb.gif?raw=true)

![insert](https://github.com/g0rg0l/Visualization-of-sorting-algorithms/blob/master/imgs/insert.gif?raw=true)
