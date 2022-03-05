/*
1.Классы, наследование
Спроектировать и реализовать следующие классы:
1. Точка DONE
2. Ломаная DONE
3. Замкнутая ломаная DONE
4. Многоугольник DONE
5. Треугольник DONE
6. Трапеция DONE
7. Правильный многоугольник DONE
Для каждого из классов реализовать следующие методы:
1. Конструктор(ы) DONE
2. Конструктор копирования DONE
3. Оператор присваивания DONE
4. Расчет периметра, если применимо DONE
5. Расчет площади, если применимо DONE
6. Другие приватные и публичные метода по усмотрению. DONE
Организовать иерархию классов, там, где это имеет смысл. DONE
!Подумать.Какие объекты этих классов могут быть объединены в один массив, где
применим динамический полиморфизм? Продемонстрировать это DONE
*/
#include <iostream>
#include <vector>
#include <cmath>

class Dot
{
public:
    float x, y;
    Dot(float x = 0, float y = 0) : x(x), y(y) {}
    Dot(const Dot &dot) : x(dot.x), y(dot.y) {}
    Dot &operator=(const Dot &dot)
    {
        if (&dot == this)
            return *this;
        x = dot.x;
        y = dot.y;
        return *this;
    }
    friend std::ostream &operator<<(std::ostream &out, const Dot &dot)
    {
        return out << "(x: " << dot.x << ", y: " << dot.y << ")\n";
    }
};

class Polyline
{
public:
    int size = 2;
    std::vector<Dot> dot_segments;
    Polyline(std::vector<Dot> dot_segments = {Dot(0.0, 0.0), Dot(1.0, 1.0)}, int size = 2) : dot_segments(dot_segments), size(size) {}
    Polyline(const Polyline &polyline) : dot_segments(polyline.dot_segments), size(polyline.size) {}
    Polyline &operator=(const Polyline &polyline)
    {
        if (&polyline == this)
            return *this;
        dot_segments = polyline.dot_segments;
        size = polyline.size;
        return *this;
    }
};

class ClosedPolyline : public Polyline
{
public:
    ClosedPolyline(std::vector<Dot> dot_segments = {Dot(0.0, 0.0), Dot(1.0, 1.0)}, int size_ = 2) : Polyline(dot_segments, size_)
    {
        dot_segments.push_back(dot_segments[0]);
        size += 1;
    }
    ClosedPolyline(const ClosedPolyline &closed_polyline) : Polyline(closed_polyline.dot_segments, closed_polyline.size) {}
    ClosedPolyline &operator=(const ClosedPolyline &closed_polyline)
    {
        if (&closed_polyline == this)
            return *this;
        dot_segments = closed_polyline.dot_segments;
        size = closed_polyline.size;
        return *this;
    }
};

class Polygon
{
protected:
    ClosedPolyline m_closed_polyline;

public:
    Polygon(ClosedPolyline closed_polyline = ClosedPolyline()) : m_closed_polyline(closed_polyline) {}
    Polygon(std::vector<Dot> polyline = {Dot(0.0, 0.0), Dot(1.0, 1.0)}) : m_closed_polyline(ClosedPolyline(polyline, polyline.size())) {}
    Polygon(const Polygon &polygon) : m_closed_polyline(polygon.m_closed_polyline) {}
    Polygon &operator=(const Polygon &polygon)
    {
        if (&polygon == this)
            return *this;
        m_closed_polyline = polygon.m_closed_polyline;
        return *this;
    }
    float perimeter()
    {
        float sum_ = 0;
        for (int number_of_dot = 0; number_of_dot < m_closed_polyline.size - 1; number_of_dot++)
        {
            int start_x = m_closed_polyline.dot_segments[number_of_dot].x;
            int start_y = m_closed_polyline.dot_segments[number_of_dot].y;
            int end_x = m_closed_polyline.dot_segments[number_of_dot + 1].x;
            int end_y = m_closed_polyline.dot_segments[number_of_dot + 1].y;
            sum_ += sqrt(pow((end_x - start_x), 2) + pow((end_y - start_y), 2));
        }
        return sum_;
    }
    float square()
    {
        float square_sum = 0;
        int dot2 = 1;
        for (int dot1 = 0; dot1 < m_closed_polyline.size - 2; dot1++)
        {

            int x1 = m_closed_polyline.dot_segments[dot1].x;
            int y1 = m_closed_polyline.dot_segments[dot1].y;
            int x2 = m_closed_polyline.dot_segments[dot2].x;
            int y2 = m_closed_polyline.dot_segments[dot2].y;
            square_sum += ((x1 + x2) * (y1 - y2));
            dot2 += 1;
        }
        return square_sum / 2.0;
    }
};

class Triangle : public Polygon
{
public:
    Triangle(const Dot &a, const Dot &b, const Dot &c) : Polygon(ClosedPolyline({a, b, c}, 3)) {}
    Triangle(const Triangle &triangle) : Polygon(triangle.m_closed_polyline) {}
    Triangle &operator=(const Triangle &triangle)
    {
        if (&triangle == this)
            return *this;
        m_closed_polyline = triangle.m_closed_polyline;
        return *this;
    }
};

class Trapezoid : public Polygon
{
public:
    Trapezoid(const Dot &a, const Dot &b, const Dot &c, const Dot &d) : Polygon(ClosedPolyline({a, b, c, d}, 4)) {}
    Trapezoid(const Trapezoid &trapezoid) : Polygon(trapezoid.m_closed_polyline) {}
    Trapezoid &operator=(const Trapezoid &trapezoid)
    {
        if (&trapezoid == this)
            return *this;
        m_closed_polyline = trapezoid.m_closed_polyline;
        return *this;
    }
};

class RegularPolygon : public Polygon
{
public:
    RegularPolygon(const Dot &a, const Dot &b, const Dot &c, const Dot &d) : Polygon(ClosedPolyline({a, b, c, d}, 4)) {}
    RegularPolygon(const RegularPolygon &regularPolygon) : Polygon(regularPolygon.m_closed_polyline) {}
    RegularPolygon &operator=(const RegularPolygon &regularPolygon)
    {
        if (&regularPolygon == this)
            return *this;
        m_closed_polyline = regularPolygon.m_closed_polyline;
        return *this;
    }
};

int main()
{

    Dot a1(0, 0), a2(2, 3), a3(4, 0);
    Dot b1(0, 0), b2(0, 1), b3(2, 2), b4(1, 0);
    Dot c1(0, 0), c2(2, 3), c3(5, 3), c4(7, 0);
    Dot d1(0, 0), d2(0, 1), d3(1, 1), d4(1, 0);
    std::vector<Polygon> polygon{Polygon({b1, b2, b3, b4}), Triangle(a1, a2, a3), Trapezoid(c1, c2, c3, c4), RegularPolygon(d1, d2, d3, d4)};
    for (int i = 0; i < polygon.size(); i++)
    {
        std::cout << "Perimeter: " << polygon[i].perimeter() << "; Square: " << polygon[i].square() << ";\n";
    }

    return 0;
}