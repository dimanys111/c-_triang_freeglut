#include <vector>
#include "list.h"

using namespace std;

void List :: insert (double a, double b) {
    Node *tmp = new Node;
    tmp->x = a; tmp->y = b;
    if ( is_empty() ) {
        tmp->next = tmp;
        tmp->prev = tmp;
    }
    else {
        tmp->next = cur->next;
        cur->next = tmp;
        tmp->prev = cur;
        tmp->next->prev = tmp;
    }
    cur = tmp; //меняем указатель на текущую вершину(вставленная вершина)
    size++;
}

void List :: insert (double *a, int n) {
    Node *tail = 0;
    /*добавлять массив из координат можно только в пустой список, иначе
    он будет замкнут неправильно*/
    if ( is_empty() ) {
        for ( int i = 0; i < n; i += 2 ) {
            insert (a[i], a[i + 1]);
            //сохраняем указатель на начало списка, чтобы потом его замкнуть
            if ( i == 0 )
                tail = cur;
        }
        //замыкаем список
        cur->next = tail;
        cur->next->prev = cur;
        cur = cur->next;
    }
}

Node* List :: find (double a, double b) {
    Node *tmp = cur;
    if ( !is_empty() ) {
        do {
            if ( a == tmp->x && b == tmp->y )
              return tmp;
            else
                tmp = tmp->next;
        } while ( tmp != cur );
      }
    return 0;
}
void List :: delete_node (double a, double b) {
    Node *tmp = find (a, b);
    if (tmp) { //вершина с такими координатами найдена
        //если удаляется текущая вершина, смещаем указатель на предыдущую
        if ( tmp == cur )
            cur = cur->prev;
        tmp->prev->next = tmp->next;
        tmp->next->prev = tmp->prev;
        delete tmp;
        size--;
    }
}

//векторное произведение векторов, заданных точками {first, second} и {first, third}
double List :: cross_prod (Node *first, Node *second, Node *third) {
    double x1 = second->x - first->x, x2 = third->x - first->x,
            y1 = second->y - first->y, y2 = third->y - first->y;
    return (x1 * y2 - x2 * y1);
}

type List :: direction () {
    Node *tmp = cur, *min = cur;
    double min_x = tmp->x, min_y = tmp->y;
    /*поиск самой левой точки(с минимальным значением абсциссы)
    если таких несколько, выберем нижнюю из них(с минимальным значением ординаты)*/
    if ( !is_empty() ) {
        do {
            if ( tmp->x < min_x ) {
                min_x = tmp->x;
                min = tmp;
            }
            if ( tmp->x == min_x && tmp->y < min_y ) {
                min_y = tmp->y;
                min = tmp;
            }
            tmp = tmp->next;
        } while ( tmp != cur );
        /*направление обхода зависит от знака векторного произведения между векторами,
        задаваемыми "минимальной" вершиной и двумя соседними с ней*/
        return ( ( cross_prod ( min, min->next, min->prev ) < 0) ? clockwise : count_clockwise );
    }
    return error;
}

bool List :: is_in_triangle ( Node *first, Node *second, Node *third ) {
    double a, b, c;
    Node *tmp = cur;
    do {
        if ( tmp != first && tmp != second && tmp != third ) {
            a = cross_prod(tmp, first, second);
            b = cross_prod(tmp, second, third);
            c = cross_prod(tmp, third, first);
            //какая-либо из точек многоугольника попадает внутрь треугольника
            if ( ( a > 0 && b > 0 && c > 0 ) || ( a < 0 && b < 0 && c < 0 ) )
                return true;
        }
        tmp = tmp->next;
    } while ( tmp != cur );
    return false;
}
vector <List> List :: triangulation () {
    //массив, c координатами треугольников, образующими триангуляцию
    vector <List> triangles, empty(0, 0); //empty будет возвращен в случае ошибки
    List *l; //список координат треугольника
    //берем три последовательно расположенные вершины
    Node *first = cur, *second = cur->next, *third = cur->next->next;
    //определяем направление обхода(по часовой стрелке/против часовой стрелки)
    type cond = direction();
    double prod;
    if ( cond != error ) {
        while (size > 3) {
            prod = cross_prod(first, second, third);
            /*вторая вершина должна лежать левее прямой, соединяющей first и third, если вершины
            заданы по часовой стрелке и правее прямой, если против часовой стрелки*/
            if ( ( cond == clockwise && prod < 0 ) || ( cond == count_clockwise && prod > 0 ) ) {
                //внутри треугольника нет других вершин многоугольника
                if ( !is_in_triangle ( first, second, third ) ) {
                    l = new List;
                    l->insert(first->x, first->y);
                    l->insert(second->x, second->y);
                    l->insert(third->x, third->y);
                    triangles.push_back(*l);
                    //исключаем вершину second из рассмотрения
                    delete_node(second->x, second->y);
                    delete l;
                }
                second = third;
                third = third->next;
            }
            else {
                first = second;
                second = third;
                third = third->next;
            }
        }
        if (size == 3) { //добавляем последний треугольник
            l = new List;
            l->insert(first->x, first->y);
            l->insert(second->x, second->y);
            l->insert(third->x, third->y);
            triangles.push_back(*l);
            delete l;
        }
        return triangles;
    }
    return empty;
}
