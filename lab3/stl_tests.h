#include "stl_semen_algorithms.h"
#include <math.h>

void buffer_test()
{
    Cyclic_Buffer<int> cyc_buf(7);

    std::cout << "\n----------- Cyclic Buffer Test -----------\n" << cyc_buf;
    cyc_buf.push_back(6);
    std::cout << "Command: push_back(6)"<< cyc_buf;
    cyc_buf.push_back(5);
    std::cout << "Command: push_back(5)"<< cyc_buf;
    cyc_buf.push_back(4);
    std::cout << "Command: push_back(4)"<< cyc_buf;
    cyc_buf.push_front(3);
    std::cout << "Command: push_front(3)"<< cyc_buf;
    cyc_buf.change_max_size(6);
    std::cout << "Command: change_max_size(6)"<< cyc_buf;
    cyc_buf.push_front(2);
    std::cout << "Command: push_front(2)"<< cyc_buf;
    cyc_buf.push_front(1);
    std::cout << "Command: push_front(1)"<< cyc_buf;

    std::cout << "Back element: " << cyc_buf.back() << '\n';
    std::cout << "Front element: " << cyc_buf.front() << '\n';
    std::cout << "Min element in Buffer: " << *std::min_element(cyc_buf.begin(), cyc_buf.end()) << '\n';
    std::cout << "Max element in Buffer: " << *std::max_element(cyc_buf.begin(), cyc_buf.end()) << '\n';
    std::cout << "There are only positive elements (0/1): " << std::none_of(cyc_buf.begin(), cyc_buf.end(), [](int elem){ return elem < 0; }) << '\n';
    std::sort(cyc_buf.begin(), cyc_buf.end());
    std::cout << "Sort Buffer: " << cyc_buf;
    std::cout << "Is Buffer sorted (0/1): " << std::is_sorted(cyc_buf.begin(), cyc_buf.end()) << '\n';

    std::cout << "Command: pop_back(): " << cyc_buf.pop_back() << cyc_buf;
    std::cout << "Command: pop_back(): " << cyc_buf.pop_back() << cyc_buf;
    std::cout << "Command: pop_back(): " << cyc_buf.pop_back() << cyc_buf;
    std::cout << "Command: pop_front(): " << cyc_buf.pop_front() << cyc_buf;
    std::cout << "Command: pop_front(): " << cyc_buf.pop_front() << cyc_buf;
    std::cout << "Command: pop_front(): " << cyc_buf.pop_front() << cyc_buf;
}

template <typename Tp, int value>
bool more_than(Tp num) { return num > value; }

template <typename Tp>
bool more(Tp lhs, Tp rhs) { return lhs > rhs; }

template <size_t num>
bool equals_more_than(int lhs, int rhs) { return lhs > num == rhs > num; }

void test_algo()
{
    std::cout << "\n----------- Alghorithms Test -----------\n";
    Cyclic_Buffer<int> cyc_buf{10, 8, 6, 4, 2, 2};
    std::cout << cyc_buf;

    std::cout << "All elements more than 1 (0/1): " << semen_algorithms::all_of(cyc_buf.begin(), cyc_buf.end(), more_than<int, 1>) << '\n';

    std::cout << "Any element more than 100 (0/1): " << semen_algorithms::any_of(cyc_buf.begin(), cyc_buf.end(), more_than<int, 100>) << '\n';

    std::cout << "None elements more than 100 (0/1): " << semen_algorithms::none_of(cyc_buf.begin(), cyc_buf.end(), more_than<int, 100>) << '\n';

    std::cout << "One element more than 8 (0/1): " << semen_algorithms::one_of(cyc_buf.begin(), cyc_buf.end(), more_than<int, 8>) << '\n';

    std::cout << "Is sorted by decreasing (0/1): " << semen_algorithms::is_sorted(cyc_buf.begin(), cyc_buf.end(), more<int>) << '\n';

    std::cout << "Is partiotioned by more than 5 (0/1): " << semen_algorithms::is_partitioned(cyc_buf.begin(), cyc_buf.end(), more_than<int, 5>) << '\n';

    std::cout << "Find first not equal to 2: " << *semen_algorithms::find_not(cyc_buf.begin(), cyc_buf.end(), 2) << '\n';

    std::cout << "Find last equal to 8: " << *(semen_algorithms::find_backward(cyc_buf.begin(), cyc_buf.end(), 8)) << '\n';

    std::cout << "Is palindrom by more than 0 (0/1): " << semen_algorithms::is_palindrom(cyc_buf.begin(), cyc_buf.end(), equals_more_than<0>) << '\n';
}

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
    double get_x() const { return this->x; }

    double get_y() const { return this->y; }

    double length() const { return sqrt(x * x + y * y); }

    bool operator>(Dot value) { return length() > value.length(); }

    bool operator>(int value) { return length() > value; }

    friend std::ostream &operator<<(std::ostream &out, const Dot &dot)
    { 
        return out << "\n(x: " << dot.x << ", y: " << dot.y << ")\n"; 
    }
};

void test_algo_with_dots()
{
    std::cout << "\n----------- Alghorithms Test With Dots -----------\n";
    Dot a(0.0, 0.0), b(3.0, 3.0), c(0.0, 7.0);
    Dot d(2.0, 2.0), e(1.0, 0.0), f(5.0, 3.0);

    Cyclic_Buffer<Dot> cyc_buf{a, b, c, d, e, f};
    std::cout << cyc_buf;

    std::cout << "Is sorted by decreasing (0/1): " << semen_algorithms::is_sorted(cyc_buf.begin(), cyc_buf.end(), more<Dot>) << '\n';
    
    std::cout << "Is sorted by x: " << semen_algorithms::is_sorted(cyc_buf.begin(), cyc_buf.end(), [](Dot rhs, Dot lhs) { return rhs.get_x() <= lhs.get_x(); }) << '\n';
    
    std::cout << "Is sorted by y: " << semen_algorithms::is_sorted(cyc_buf.begin(), cyc_buf.end(), [](Dot rhs, Dot lhs) { return rhs.get_y() <= lhs.get_y(); }) << '\n';
    
    std::cout << "All lengths of Dots more than 0: " << semen_algorithms::all_of(cyc_buf.begin(), cyc_buf.end(), more_than<Dot, 0>) << '\n';
    
    std::cout << "Any Dot length more than -3: " << semen_algorithms::any_of(cyc_buf.begin(), cyc_buf.end(), more_than<Dot, -3>) << '\n';
    
    std::cout << "None of Dots length more than 0: " << semen_algorithms::none_of(cyc_buf.begin(), cyc_buf.end(), more_than<Dot, 0>) << '\n';
    
    std::cout << "Only one Dot length more than 6: " << semen_algorithms::one_of(cyc_buf.begin(), cyc_buf.end(), more_than<Dot, 6>) << '\n';

}
