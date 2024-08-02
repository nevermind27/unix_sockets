Задача: реализовать две программы, которые передают друг другу данные с использованием сокетов.
#Программа 1 
Состоит из двух потоков и одного общего буфера.
Поток 1. Принимает строку, которую введет пользователь. Должна быть проверка, что строка состоит только из цифр и не превышает 64 символа. После проверки строка должна быть отсортирована по убыванию и все элементы, значение которых чётно, заменены на латинские буквы «КВ». После данная строка помещается в общий буфер и поток должен ожидать дальнейшего ввода пользователя.
Поток 2. Должен обрабатывать данные которые помещаются в общий буфер. После получения данных общий буфер затирается.
Поток должен вывести полученные данные на экран, рассчитать общую сумму всех элементов , которые являются численными значениями. Полученную сумму передать в Программу №2. После этого поток ожидает следующие данные.
(Для синхронизации потоков и защиты от одновременного стирания потоком 2 и записи потоком 1 применяются мьютексы)
#Программа №2.
Ожидает данные от Программы №1. При получении данных происходит анализ из скольки символов состоит переданное значение. Если оно больше 2-ух символов и если оно кратно 32 выводит сообщение о полученных данных, иначе выводится сообщение об ошибке. Далее программа продолжает ожидать данные.

Примечание: Возможно, стоило вынести объявление класса и функций в отдельные файлы, сделать заголовочные файлы, однако, на мой взгляд, их не так много, поэтому всё собрано в main.cpp