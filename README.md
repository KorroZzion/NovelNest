# NovelNest
 Aurora OS ebook reader

QML
Mainpage.qml: отвечает за отрисовку визуальных элементов на главной странице.
BookDetailPage.qml: отвечает за вывод информации о книге.
ReadingPage.qml: отображение всей книги.
SearchPage.qml: список доступных книг на устройстве.
Element.qml: заглушка обложки.
.h
Bookdatabasemanager: класс описывающий взаимодействие с базой данных (удаление, сохранение и выгрузка содержимого).
Bookfinder: класс для нахождения файлов формата fb2 и txt и изъятия из них названия и автора.
Bookloader: класс для чтения файла.
Cpp.
Main: подключение объектов c++ к qml.
