# KNU_Paralelism
Interaction of processes. Parallelism. Controls standard I / O.                   
Взаємодія процесів. Паралелізм. Управління стандартним вводом-виводом. Обчислити f(x) * g(x), використовуючи 2 допоміжні процеси: один обчислює f(x), а інший – g(x). Основна програма виконує ввод-вивід та операцію *. Не використовувати обмін повідомленнями між процесами та порти. Процеси f та g читають дані з stdin, а результати пишуть в stdout, але не безпосередньо – вводом і виводом керує основна програма, вона посилає на вхід f і g дані та отримує від них результати. Забороняється використовувати допоміжні файли для обміну інформацією між процесами.
