#include <cstdlib>
#include <ctime>
#include <vector>
#include "list.h"
#include <GL/glut.h>
#define width 800
#define height 640

using namespace std;

//координаты многоугольника(заданы по часовой стрелке)
double coords[] = { 140.0, 90.0, 120.0, 170.0, 100.0, 180.0, 180.0, 250.0, 110.0, 230.0,200.0, 300.0,
300.0, 280.0, 280.0, 220.0, 300.0, 170.0, 360.0, 130.0, 300.0, 130.0, 250.0, 60.0, 180.0, 150.0};
int size = sizeof(coords) / sizeof(coords[0]);

void init () {
    glClearColor (1.0, 1.0, 1.0, 1.0); //цвет фона(RGBA, белый)
    glViewport (0, 0, width, height); //область вывода
    glMatrixMode (GL_PROJECTION); //матрица преобразований(матрица проекций)
    glLoadIdentity (); //замена текущей матрицы на единичную
    /*матрица двухмерной ортографической проекции,
    параметры функции определяют левую, правую, нижнюю и верхнюю плоскости отсечения*/
    gluOrtho2D (0, width / 2,  0, height / 2);
}

void draw_polygon () {
/*1-й параметр-число координат вершины, 2-й-тип данных,
3-й-смещение от координат одной вершины до координат следующей,4-й-указатель на массив с координатами*/
    glVertexPointer(2, GL_DOUBLE, 0, coords);
    glEnableClientState (GL_VERTEX_ARRAY); //тип массива(массив вершин)
    //вывод примитивов типа замкнутая ломанная
    glDrawArrays(GL_LINE_LOOP, 0, size / 2);
    glFlush (); //вывод на экран
}
void draw_triangle (double x1, double y1, double x2, double y2, double x3, double y3) {
    glColor3ub (0, 0, 0);
    glBegin(GL_LINE_LOOP); //тип примитива(замкнутая ломанная)
    //координаты вершин
    glVertex2d (x1, y1);
    glVertex2d (x2, y2);
    glVertex2d (x3, y3);
    glEnd();
    glFlush();
}
void fill_triangle (double x1, double y1, double x2, double y2, double x3, double y3) {
    glColor3ub (rand () % 254, rand() % 254, rand() % 254);
    glBegin(GL_TRIANGLES);
    glVertex2d (x1, y1);
    glVertex2d (x2, y2);
    glVertex2d (x3, y3);
    glEnd();
    glFlush();
}
void triangulate () {
    draw_polygon();
    List *polygon;
    polygon = new List;
    polygon->insert (coords, size); //инициализация многоугольника
    //применение триангуляции
    vector <List> triangles = polygon->triangulation();
    double x1, y1, x2, y2, x3, y3;
    srand (time(0));
    int i = 1; //номер треугольника
    for (auto now : triangles) {
        x1 = now.cur->x, y1 = now.cur->y, x2 = now.cur->next->x, y2 = now.cur->next->y,
        x3 = now.cur->prev->x, y3 = now.cur->prev->y;
        if ( i % 2 ) //закрашиваем треугольники с нечётными номерами
            fill_triangle(x1, y1, x2, y2, x3, y3);
        //прорисовываем границы
        draw_triangle(x1, y1, x2, y2, x3, y3);
        i++;
    }
    delete polygon;
}

void display () {
    glClear (GL_COLOR_BUFFER_BIT); //очистка текущего буфера цвета
    glColor3ub (0, 0, 0); //установка цвета рисования(RGB, черный)
    glLineWidth (2); //ширина линии в пикселях
    //построение и триангуляция многоугольника с координатами из массива coords
    triangulate();
}

int main ( int argc, char** argv ) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); //режим окна(одиночный буфер и RGB-палитра)
    glutInitWindowSize (width, height); //установка размеров окна(ширина и высота в пикселях)
    glutInitWindowPosition (200, 30);//положение окна относительно левого верхнего угла экрана
    glutCreateWindow ("triangulation");//создание окна c указанным заголовком
    glutDisplayFunc(display); //вывод на экран
    init (); //дополнительные параметры
    glutMainLoop();
    return 0;
}
