#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <array>

char plus_or_minus(int num)
{
    return num > 0 ? '+' : '-';
}

template<int size_of_array>
class Polynomial
{
private:
    std::array<int, size_of_array> m_coefficients;
    constexpr int pow(int x, int degree) const {
		int num = 1;
        while (degree--) num *= x;
		return num;
	}
public:
    constexpr Polynomial(const std::array<int, size_of_array> &coefficients = {}) : m_coefficients(coefficients) {}
    Polynomial(const Polynomial &polynomial) : m_coefficients(polynomial.m_coefficients){}
    ~Polynomial()=default;
    Polynomial &operator=(const Polynomial &polynomial)
    {
        if (&polynomial == this)
            return *this;
        this->m_coefficients = polynomial.m_coefficients;
        return *this;
    }

    constexpr int calculate_x(int x) const
    {
        int calculated_num = m_coefficients[0];
        for (int i = 1; i < m_coefficients.size(); ++i)
        {
            calculated_num += m_coefficients[i] * pow(x, i);
        }
        return calculated_num;
    }

    friend std::ostream &operator<<(std::ostream &out, const Polynomial &polynomial)
    {
        out << "Your Polynomial: ";
        int length_of_polynomial = polynomial.m_coefficients.size() - 1;
        bool first = false;
        for (; length_of_polynomial > 1; length_of_polynomial--)
        {
            if (polynomial.m_coefficients[length_of_polynomial] != 0 && abs(polynomial.m_coefficients[length_of_polynomial]) != 1)
            {
                if (polynomial.m_coefficients[length_of_polynomial] > 0)
                {
                    out << polynomial.m_coefficients[length_of_polynomial] << "x^" << length_of_polynomial << " ";
                    first = true;
                    length_of_polynomial -= 1;
                    break;
                }
                else
                {
                    out << "-" << abs(polynomial.m_coefficients[length_of_polynomial]) << "x^" << length_of_polynomial << " ";
                    first = true;
                    length_of_polynomial -= 1;
                    break;
                }
            }
            else if (abs(polynomial.m_coefficients[length_of_polynomial]) == 1)
            {
                out << "x^" << length_of_polynomial << " ";
                first = true;
                length_of_polynomial -= 1;
                break;
            }
        }
        for (; length_of_polynomial > 1; length_of_polynomial--)
        {
            int coefficient = polynomial.m_coefficients[length_of_polynomial];
            if (coefficient != 0 && abs(coefficient) != 1)
            {
                out << plus_or_minus(coefficient) << " " << abs(coefficient) << "x^" << length_of_polynomial << " ";
            }
            else if (abs(coefficient) == 1)
            {
                out << plus_or_minus(coefficient) << " x^" << length_of_polynomial << " ";
            }
        }
        if (polynomial.m_coefficients[1] != 0 && first)
            out << (polynomial.m_coefficients[1] < 0 ? "-" : "+") << " " << (abs(polynomial.m_coefficients[1]) != 1 ? std::to_string(abs(polynomial.m_coefficients[1])) : "") << "x ";
        else if (polynomial.m_coefficients[1] != 0)
        {
            out << (polynomial.m_coefficients[1] < 0 ? "-" : "") << (abs(polynomial.m_coefficients[1]) != 1 ? std::to_string(abs(polynomial.m_coefficients[1])) : "") << "x ";
            first = true;
        }
        if (polynomial.m_coefficients[0] != 0 && first)
            out << plus_or_minus(polynomial.m_coefficients[0]) << " " << abs(polynomial.m_coefficients[0]) << " ;";
        else if (polynomial.m_coefficients[0] != 0)
            out << polynomial.m_coefficients[0] << " ;";
        else
            out << ";\n";
        return out;
    }
};

// int main()
// {
//     constexpr Polynomial<5> pol(std::array<int, 5> { 1, 2, 3, 4, 5 });
//     constexpr int calculated_num = pol.calculate_x(2);
//     static_assert(calculated_num == 129, "calculated_num is correct");
// 	std::cout << pol << " " << calculated_num << "\n";
//     return 0;
// }