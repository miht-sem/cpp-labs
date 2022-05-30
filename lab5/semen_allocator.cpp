#include <iostream>
#include <cassert>
#include <memory>
#include <deque>
#include <functional>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>

class Allocator_Helper
{
public:
    Allocator_Helper(const std::size_t count_of_blocks = 100, const std::size_t size_of_block = 8192)
    {

        for (int i = 0; i < count_of_blocks; ++i)
            m_chunks.emplace_back(size_of_block, size_of_block);
        m_available_blocks = count_of_blocks - 1;
        m_size_of_block = size_of_block;
        m_memory = &m_chunks.back().first.front();
    }
    ~Allocator_Helper() {}

    void *allocate(std::size_t n)
    {
        if (m_available_blocks == 0)
        {
            throw std::runtime_error("Error: Memory is end!");
        }
        if (n > m_size_of_block)
        {
            throw std::runtime_error("Error: There is not enough memory in block!!!!");
        }
        if (n <= m_chunks[m_available_blocks].second)
        {
            auto mem = m_memory;
            m_memory += n;
            m_chunks[m_available_blocks].second -= n;
            std::cout << "Allocated: " << n << " bytes. Count Of Available Blocks: " << m_available_blocks << "\n";
            return mem;
        }

        m_available_blocks -= 1;
        m_memory = &m_chunks[m_available_blocks].first.front();
        m_chunks[m_available_blocks].second -= n;
        auto mem = m_memory;
        m_memory += n;
        std::cout << "Allocated: " << n << " bytes. Count Of Available Blocks: " << m_available_blocks << "\n";
        return mem;
    }

    void deallocate(void *p, std::size_t n)
    {
        auto mem = (unsigned char *)p;

        if (m_chunks[m_available_blocks].second + n == m_size_of_block)
        {
            m_chunks[m_available_blocks].second += n;
            m_available_blocks += 1;
            if (m_available_blocks == m_chunks.size())
            {
                m_available_blocks -= 1;
            }
            m_memory = &m_chunks[m_available_blocks].first.front();
        }
        else
        {
            m_chunks[m_available_blocks].second += n;
            m_memory = mem;
        }
        std::cout << "Deallocated: " << n << " bytes. Count Of Available Blocks: " << m_available_blocks << "\n";
        std::cout << "Available memory in block: " << m_chunks[m_available_blocks].second << " bytes." << '\n';
    }

private:
    std::vector<std::pair<std::vector<unsigned char>, int>> m_chunks;
    std::size_t m_available_blocks = 0;
    std::size_t m_size_of_block = 0;
    unsigned char *m_memory;
};

template <class T>
class Semen_Allocator
{
public:
    Allocator_Helper *m_alloc_helper;

    using value_type = T;

    using pointer = T *;
    using const_pointer = const T *;

    using void_pointer = void *;
    using const_void_pointer = const void *;

    using size_type = size_t;

    using difference_type = std::ptrdiff_t;

    Semen_Allocator() = default;

    explicit Semen_Allocator(Allocator_Helper &allocator_helper) : m_alloc_helper(&allocator_helper) {}
    template <class U>
    Semen_Allocator(const Semen_Allocator<U> &other) : m_alloc_helper(other.m_alloc_helper) {}

    ~Semen_Allocator() = default;

    pointer allocate(size_type count_objects)
    {
        if (count_objects > std::numeric_limits<std::size_t>::max() / sizeof(T))
            throw std::bad_array_new_length();

        if (auto p = (T *)m_alloc_helper->allocate(count_objects * sizeof(T)))
        {
            return p;
        }

        throw std::bad_alloc();
        ;
    }

    void deallocate(pointer p, size_type count_objects)
    {
        m_alloc_helper->deallocate(p, count_objects * sizeof(T));
    }
};

template <class T, class U>
bool operator==(const Semen_Allocator<T> &, const Semen_Allocator<U> &);
template <class T, class U>
bool operator!=(const Semen_Allocator<T> &, const Semen_Allocator<U> &);

using alloc_string = std::basic_string<char, std::char_traits<char>, Semen_Allocator<char>>;
template <class T>
using alloc_vector = std::vector<T, Semen_Allocator<T>>;
template <class T>
using alloc_deque = std::deque<T, Semen_Allocator<T>>;
template <class T>
using alloc_list = std::list<T, Semen_Allocator<T>>;
template <class T>
using alloc_set = std::set<T, std::less<T>, Semen_Allocator<T>>;
template <class T, class V>
using alloc_map = std::map<T, V, std::less<T>, Semen_Allocator<std::pair<const T, V>>>;
template <class T>
using alloc_unordered_set = std::unordered_set<T, std::hash<T>, std::equal_to<T>, Semen_Allocator<T>>;
template <class T, class V>
using alloc_unordered_map = std::unordered_map<T, std::hash<T>, std::equal_to<T>, Semen_Allocator<std::pair<const T, V>>>;

struct F
{
};

int main()
{
    Allocator_Helper allocator_helper(100, 4096);
    Semen_Allocator<char> semen_allocator(allocator_helper);
    alloc_string string_123("123", semen_allocator);
    alloc_vector<int> semens({1, 2, 3}, semen_allocator);
    alloc_vector<alloc_string> v_str({string_123, string_123}, semen_allocator);
    alloc_vector<alloc_string> v_str_copy(v_str, semen_allocator);
    alloc_vector<F> v_x(24, F{}, semen_allocator);
    alloc_deque<int> d_int({1, 2, 3}, semen_allocator);
    alloc_list<int> l_int({1, 2, 3}, semen_allocator);
    alloc_set<int> s_int({1, 2, 3}, std::less<int>{}, semen_allocator);
    alloc_map<alloc_string, int> m_str_int(semen_allocator);
    alloc_unordered_set<int> us_int(semen_allocator);
    return 0;
}