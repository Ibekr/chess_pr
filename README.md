# chess_pr
Chess problem resolver (Программа, решающая шахматные задачи)


~~~~~
1) Король    K (king)
2) Ферзь     Q (queen)
3) Ладья     R (rook)
4) Конь      N (kNight)
5) Слон      B (bishop)
6) Пешка     P(pawn)

Chess::Desk - вложенный класс, описывает текущую ситуацию на поле
Формат входных данных в файле:
целые числа, разделенные пробелом и переводом строки
Первая строка вида:
P W B
где
P - количество ходов из условия задачи
W - количество белых фигур на доске
B - количество черных фигур на доске
далее N строк вида:
A X Y
где
A - номер фигуры (тип)
X - положение фигуры на доске по координате X (от левого края)
Y - положение фигуры на доске по координате Y (от нижнего ряда)
далее M строк вида:
A X Y
Поле 1 1 (A1) находится в нижнем левом углу доски
Фигуры нумеруются от одного до семи в независимости от цвета
1) Король    K (king)
2) Ферзь     Q (queen)
3) Ладья     R (rook)
4) Конь      N (kNight)
5) Слон      B (bishop)
6) Пешка     P (pawn)
7) Пешка, последний ход которой был двухшаговым
   (Возможно взятие на проходе)
~~~~~

Имя файла с описанием задачи задается в функции `loadChessProblemFromFile()`

