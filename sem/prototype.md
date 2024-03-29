# Что нужно сделать

До второго промежуточного дедлайна вам необходимо реализовать прототип
rsync-a c очень урезанной функциональностью. Должен получиться
исполняемый файл, который принимает 2 аргумента - имя директории с
сходными файлами и имя директории, которую следует синхронизировать с
исходной. После запуска, утилита должна выдавать в стандартный поток
вывода список действий, которые необходимо совершить, чтобы вторая
директория синхронизировалась с исходной.

Например, если есть следующая структура каталогов и файлов:

```
source
source/file1
source/file2
source/file3

dest
dest/file2
dest/file4
```

Команда `rsync source dest` должна выдать:

```
rm file3
cp file1
cp file3
```

При этом, внутри программа должна эмулировать взаимодействие по
сети (**Однопоточная программа, которая просто вычисляет разницу
между двумя каталогами засчитана не будет**).

Чтобы сэмулировать взаимодействие, предлагается использовать системный
вызов `socketpair`, который создаёт пару сокетов соединённых между
собой (пример в файле `test/rsync_test.cpp`).

Ваша программа должна создавать такую пару сокетов, а потом запускать
2 потока (пример запуска потоков в файле с тестами). Первый поток
будет исполнять код Sender-а, а второй - код Receiver-а.

Sender и Receiver должны обмениваться данными через сокет, согласно
протоколу, который обсуждался на семинаре.

**До первого промежуточного дедлайна**(через один семинар) нужно
пройти review на дизайн вашего кода. Репозиторий должен содержать
интерфейсы всех необходимых функций и классов, вместе с описанием
того, как эти классы между собой взаимодействуют. *Пройти review*
значит получить от меня OK. Если вы закоммитите что-то в последнюю
ночь и пришлёте мне ссылку, review не считается пройденным.

# Замечания

1) Весь код должен быть разбит на классы. У каждого класса должна быть
огранниченная область ответственности.

Решение, в котором main() содержит какую-то нетривиальную логику,
кроме ввода-вывода и разбора аргументов командной строки, засчитано не
будет.

Методы или функции длинной больше 50 строк, необходимо будет отдельно
защищать на code review.

2) В помощь студентам, неуверенным в своих навыках объектно
ориентированного дизайна, дан интерфейс классов отвечающих за
взаимодействие с сетью (`Connection` и `Protocol`). Если у вас
возникнут вопросы, по поводу того, как разбить на классы оставщуюся
часть программы, можете обращаться ко мне или обсуждать это в общем
чате (делиться кусками кода с другими студентами, очевидно, нельзя).

За плохой дизайн в конце семестра будут сниматься баллы. Критерии
*хорошести* дизайна слабо формализуются, поэтому нужно писать код в
течении семестра и регулярно присылать его на review.

3) На каждый нетривиальный кусок функциональности должны быть написаны тесты.
За недостаточное покрытие кода тестами будут сниматься баллы.

Это значит, что если вы пишите какой-то кусок функциональности, то вам
сначала нужно подумать о том, как его можно протестировать. После
этого написать тесты. И только потом приступать к реализации самой
функциональности.

Если вы не уверены, что протестировали код хорошо, или не знаете, как
протестировать ваш класс, можете задавать вопросы мне лично.

4) Программа должна корректно обрабатывать все ошибки и в случае
возникновения исключительной ситуации завершаться с осмысленным сообщением.

Чтобы добиться этого, предлагается использовать механизм обработки
исключений C++. Нужно проверять возвращаемое значение каждого
системного вызова и в случае ошибки бросать исключение. На самом
верхнем уровене в программе предлагается поставить один try-catch
блок, который эти исключения ловит и печатает.
