#ifndef LIST_H
#define LIST_H
#include <vector>

using namespace std;

class Node {  //узел списка(в данной задаче-вершина многоугольника)
public:
    double x, y; //координаты текущей точки
    Node *prev, *next; //указатели на предыдущую и следующую вершины
    //конструктор по умолчанию
    Node (double a = 0.0, double b = 0.0): x(a), y(b), prev(0), next(0) {}
};

//направление, в котором заданы координаты(по часовой стрелке, против часовой стрелки)
typedef enum { clockwise, count_clockwise, error } type;

class List { //кольцевой двусвязный список(будет содержать координаты многоугольника)
public:
    int size; //количество вершин
    Node* cur; //указатель на текущую вершину
    List (int s = 0, Node* tmp = 0): size(s), cur(tmp) {}
    bool is_empty () {
      return ( size == 0 ? true : false );
    }
    void insert (double a, double b);//вставка одной вершины после текущей
    void insert (double *a, int n); //вставка массива координат
    Node* find (double a, double b); //поиск вершины с заданными координатами
    void delete_node (double a, double b); //удаление вершины с заданными координатами
    double cross_prod (Node *first, Node *second, Node *third);//векторное произведение
    type direction (); //направление обхода многоугольника
    //принадлежность остальных вершин многоугольника треугольнику с вершинами {first, second, third}
    bool is_in_triangle (Node *first, Node *second, Node *third);
    vector <List> triangulation (); //триангуляция
};

#endif /* LIST_H*/v
