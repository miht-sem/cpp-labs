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
private:
    double x, y;

public:
    explicit Dot(const double &x = 0.0, const double &y = 0.0) : x(x), y(y) {}
    Dot(const Dot &dot) : x(dot.x), y(dot.y) {}
    Dot &operator=(const Dot &dot)
    {
        if (&dot == this)
            return *this;
        x = dot.x;
        y = dot.y;
        return *this;
    }
    double getX() const
    {
        return this->x;
    }
    double getY() const
    {
        return this->y;
    }
    friend std::ostream &operator<<(std::ostream &out, const Dot &dot)
    {
        return out << "(x: " << dot.x << ", y: " << dot.y << ")\n";
    }
};

class Polyline
{
protected:
    std::vector<Dot> m_dot_segments;

public:
    explicit Polyline(const std::vector<Dot> &dot_segments = {Dot(0.0, 0.0), Dot(1.0, 1.0)}) : m_dot_segments(dot_segments) {}
    Polyline(const Polyline &polyline) : m_dot_segments(polyline.m_dot_segments) {}
    Polyline &operator=(const Polyline &polyline)
    {
        if (&polyline == this)
            return *this;
        this->m_dot_segments = polyline.m_dot_segments;
        return *this;
    }
    int size() const
    {
        return this->m_dot_segments.size();
    }
    double perimeter() const
    {
        double sum_ = 0;
        for (int number_of_dot = 0; number_of_dot < m_dot_segments.size() - 1; number_of_dot++)
        {
            double start_x = m_dot_segments[number_of_dot].getX();
            double start_y = m_dot_segments[number_of_dot].getY();
            double end_x = m_dot_segments[number_of_dot + 1].getX();
            double end_y = m_dot_segments[number_of_dot + 1].getY();
            sum_ += sqrt(pow((end_x - start_x), 2) + pow((end_y - start_y), 2));
        }
        return sum_;
    }
};

class ClosedPolyline : public Polyline
{
public:
    explicit ClosedPolyline(std::vector<Dot> dot_segments = {Dot(0.0, 0.0), Dot(1.0, 1.0)})
    {
        dot_segments.push_back(dot_segments[0]);
        m_dot_segments = dot_segments;
    }
    ClosedPolyline(const ClosedPolyline &closed_polyline) : Polyline(closed_polyline.m_dot_segments) {}
    ClosedPolyline &operator=(const ClosedPolyline &closed_polyline)
    {
        if (&closed_polyline == this)
            return *this;
        m_dot_segments = closed_polyline.m_dot_segments;
        return *this;
    }
    Dot operator[](const int &index) const
    {
        if (index >= 0 && index < this->m_dot_segments.size())
            return this->m_dot_segments[index];
        else
        {
            std::cout << "INDEX OUT OF BOUNDS!!!";
            exit(1);
        }
    }
};

class Polygon
{
protected:
    ClosedPolyline m_closed_polyline;

public:
    explicit Polygon(const ClosedPolyline &closed_polyline = ClosedPolyline()) : m_closed_polyline(closed_polyline) {}
    explicit Polygon(const std::vector<Dot> &polyline) : m_closed_polyline(ClosedPolyline(polyline)) {}
    Polygon(const Polygon &polygon) : m_closed_polyline(polygon.m_closed_polyline) {}
    Polygon &operator=(const Polygon &polygon)
    {
        if (&polygon == this)
            return *this;
        m_closed_polyline = polygon.m_closed_polyline;
        return *this;
    }
    double perimeter() const
    {
        std::cout << name();
        return this->m_closed_polyline.perimeter();
    }
    virtual double square() const
    {
        std::cout << name();
        double square_sum = 0.0;
        int dot2 = 1;
        for (int dot1 = 0; dot1 < this->m_closed_polyline.size() - 2; dot1++)
        {
            double x1 = this->m_closed_polyline[dot1].getX();
            double y1 = this->m_closed_polyline[dot1].getY();
            double x2 = this->m_closed_polyline[dot2].getX();
            double y2 = this->m_closed_polyline[dot2].getY();
            square_sum += ((x1 + x2) * (y1 - y2));
            dot2 += 1;
        }
        return abs(square_sum) / 2.0;
    }
    virtual std::string name() const
    {
        return "Polygon: ";
    }
};

class Triangle : public Polygon
{
public:
    Triangle(const Dot &a, const Dot &b, const Dot &c) : Polygon({a, b, c}) {}
    Triangle(const Triangle &triangle) : Polygon(triangle.m_closed_polyline) {}
    Triangle &operator=(const Triangle &triangle)
    {
        if (&triangle == this)
            return *this;
        m_closed_polyline = triangle.m_closed_polyline;
        return *this;
    }
    double square() const override
    {
        return Polygon::square();
    }
    std::string name() const override
    {
        return "Triangle: ";
    }
};

class Trapezoid : public Polygon
{
public:
    Trapezoid(const Dot &a, const Dot &b, const Dot &c, const Dot &d) : Polygon({a, b, c, d}) {}
    Trapezoid(const Trapezoid &trapezoid) : Polygon(trapezoid.m_closed_polyline) {}
    Trapezoid &operator=(const Trapezoid &trapezoid)
    {
        if (&trapezoid == this)
            return *this;
        m_closed_polyline = trapezoid.m_closed_polyline;
        return *this;
    }
    double square() const override
    {
        return Polygon::square();
    }
    std::string name() const override
    {
        return "Trapezoid: ";
    }
};

class RegularPolygon : public Polygon
{
public:
    RegularPolygon(const std::vector<Dot>& polyline = {Dot(0.0, 0.0), Dot(1.0, 1.0)}) : Polygon(polyline) {}
    RegularPolygon(const RegularPolygon &regularPolygon) : Polygon(regularPolygon.m_closed_polyline) {}
    RegularPolygon &operator=(const RegularPolygon &regularPolygon)
    {
        if (&regularPolygon == this)
            return *this;
        m_closed_polyline = regularPolygon.m_closed_polyline;
        return *this;
    }
    double square() const override
    {
        return Polygon::square();
    }
    std::string name() const override
    {
        return "Regular polygon: ";
    }
};

int main()
{

    Dot a1(0.0, 0.0), a2(3.0, 3.0), a3(0.0, 7.0);
    Dot b1(0, 0), b2(0, 1), b3(2, 2), b4(1, 0);
    Dot c1(0, 0), c2(2, 3), c3(5, 3), c4(7, 0);
    Dot d1(0, 0), d2(0, 1), d3(1, 1), d4(1, 0);
    Polygon *polygon = new Polygon({b1, b2, b3, b4});
    Triangle *triangle = new Triangle(a1, a2, a3);
    Trapezoid *trapezoid = new Trapezoid(c1, c2, c3, c4);
    RegularPolygon *regular_polygon = new RegularPolygon({d1, d2, d3, d4});
    std::vector<Polygon *> polygons{polygon, triangle, trapezoid, regular_polygon};
    for (int i = 0; i < polygons.size(); i++)
    {
        std::cout << "Perimeter " << polygons[i]->perimeter() << "; Square " << polygons[i]->square() << ";\n";
    }

    return 0;
}