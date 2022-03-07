/*
2. Классы, перегрузка операторова
Спроектировать и реализовать класс для описания сущности многочлен (полином),
раздела математики - Алгебра.
Реализовать конструктор(ы), конструктор копирования, деструктор, а также следующие DONE
операторы:
1. = DONE
2. ==, != DONE
3. +, - (унарный и бинарный), +=, -= DONE
4. * , / (на число), *=, /= DONE
5. <<, >>  DONE
6. [] (для получения коэффициента i-го члена) DONE
*/
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

char plus_or_minus(double num)
{
    return num > 0 ? '+' : '-';
}

class Polynomial
{
private:
    std::vector<double> m_coefficients;

public:
    Polynomial(const std::vector<double> &coefficients = {}) : m_coefficients(coefficients) {}
    Polynomial(const Polynomial &polynomial) : m_coefficients(polynomial.m_coefficients) {}
    ~Polynomial() { m_coefficients.clear(); }
    Polynomial &operator=(const Polynomial &polynomial)
    {
        if (&polynomial == this)
            return *this;
        this->m_coefficients = polynomial.m_coefficients;
        return *this;
    }
    friend std::ostream &operator<<(std::ostream &out, const Polynomial &polynomial)
    {
        out << "Your Polynomial: ";
        double length_of_polynomial = polynomial.m_coefficients.size() - 1;
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
            double coefficient = polynomial.m_coefficients[length_of_polynomial];
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
    friend std::istream &operator>>(std::istream &in, Polynomial &polynomial)
    {
        double n;
        in >> n;
        polynomial.m_coefficients.resize(n);
        for (int coefficients = 0; coefficients < n; coefficients++)
        {
            in >> polynomial.m_coefficients[coefficients];
        }
        return in;
    }
    double &operator[](const int &index)
    {
        if (index >= this->m_coefficients.size())
        {
            std::cout << "Array index out of bound, exiting";
            exit(1);
        }
        return this->m_coefficients[index];
    }
    bool operator==(const Polynomial &polynomial2) const
    {
        if (this->m_coefficients.size() != polynomial2.m_coefficients.size())
        {
            return false;
        }
        for (int i = 0; i <= this->m_coefficients.size(); i++)
        {
            if (abs(polynomial2.m_coefficients[i] - this->m_coefficients[i]) > 0.001)
                return false;
        }
        return true;
    }
    bool operator!=(const Polynomial &polynomial2) const
    {
        return !(this->m_coefficients == polynomial2.m_coefficients);
    }
    Polynomial operator*(const double &multiply)
    {
        std::transform(this->m_coefficients.begin(), this->m_coefficients.end(), this->m_coefficients.begin(), std::bind(std::multiplies<int>(), std::placeholders::_1, multiply));
        return *this;
    }
    Polynomial operator*(const Polynomial &polynomial2)
    {
        std::vector<double> polynomial_multiply(this->m_coefficients.size() + polynomial2.m_coefficients.size() - 1);
        for (int i = 0; i < this->m_coefficients.size(); i++)
        {
            for (int j = 0; j < polynomial2.m_coefficients.size(); j++)
            {
                polynomial_multiply[i + j] += this->m_coefficients[i] * polynomial2.m_coefficients[j];
            }
        }
        return Polynomial(polynomial_multiply);
    }
    Polynomial operator/(const double &division)
    {
        std::transform(this->m_coefficients.begin(), this->m_coefficients.end(), this->m_coefficients.begin(), std::bind(std::divides<int>(), std::placeholders::_1, division));
        return *this;
    }
    Polynomial operator+(const Polynomial &polynomial2) const
    {
        std::vector<double> sum(this->m_coefficients.size());
        std::transform(this->m_coefficients.begin(), this->m_coefficients.end(), polynomial2.m_coefficients.begin(), sum.begin(), std::plus<int>());
        return Polynomial(sum);
    }
    Polynomial operator-(const Polynomial &polynomial2) const
    {
        std::vector<double> not_sum(this->m_coefficients.size());
        std::transform(this->m_coefficients.begin(), this->m_coefficients.end(), polynomial2.m_coefficients.begin(), not_sum.begin(), std::minus<int>());
        return Polynomial(not_sum);
    }
    Polynomial operator+()
    {
        return *this;
    }
    Polynomial operator-()
    {
        return *this * -1;
    }
    void operator+=(const Polynomial &polynomial2)
    {
        *this = *this + polynomial2;
    }
    void operator-=(const Polynomial &polynomial2)
    {
        *this = *this - polynomial2;
    }
    void operator*=(const double &multiply)
    {
        *this = *this * multiply;    
    }
    void operator*=(const Polynomial &polynomial2)
    {
        std::vector<double> polynomial_multiply(this->m_coefficients.size() + polynomial2.m_coefficients.size() - 1);
        for (int i = 0; i < this->m_coefficients.size(); i++)
        {
            for (int j = 0; j < polynomial2.m_coefficients.size(); j++)
            {
                polynomial_multiply[i + j] += this->m_coefficients[i] * polynomial2.m_coefficients[j];
            }
        }
        *this = polynomial_multiply;
    }
    void operator/=(const double &division)
    {
        *this = *this / division;
    }
};

int main()
{
    Polynomial pol{{2, 2, 2, 2, 2}};
    Polynomial pol2{{1, 1, 1, 1, 1}};
    pol = pol / 2;
    std::cout << pol;
    return 0;
}