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

class Blocks
{
public:
    Blocks(std::pair<std::size_t, std::size_t> size_and_count)
    {
        for (std::size_t i = 0; i < size_and_count.second; ++i)
            m_blocks.push_back(new unsigned char[size_and_count.first]);
        m_available_blocks = size_and_count.second;
        size_of_block = size_and_count.first;
    }
    unsigned char *get_chunk()
    {
        if (is_empty())
            throw std::runtime_error("Error: There is no availible blocks!");
        m_available_blocks--;
        return m_blocks[m_available_blocks];
    }
    bool is_empty() const { return m_available_blocks == 0; }
    bool find_block(unsigned char *memory)
    {
        for (int i = m_blocks.size() - 1; i >= m_available_blocks && i > 0; i--)
            if (m_blocks[i] == memory)
            {
                if (i != m_available_blocks)
                    std::swap(m_blocks[i], m_blocks[m_available_blocks]);
                m_available_blocks++;
                return true;
            }
        return false;
    }

    std::size_t get_size_of_block() const { return size_of_block; }

private:
    std::vector<unsigned char *> m_blocks;
    std::size_t m_available_blocks;
    std::size_t size_of_block;
};

class Greater
{
private:
    std::size_t m_num;

public:
    Greater(std::size_t num) : m_num(num) {}
    bool operator()(const Blocks &block) const
    {
        return block.get_size_of_block() >= m_num;
    }
};

class Allocator_Helper
{
public:
    Allocator_Helper(const std::vector<std::pair<std::size_t, std::size_t>> size_and_count = {{1, 5}, {2, 5}, {4, 5}, {8, 5}, {16, 5}, {32, 5}, {64, 5}, {128, 5}, {256, 5}, {512, 5}})
    {
        for (auto i : size_and_count)
            m_chunks.emplace_back(i);
    }

    void *allocate(std::size_t n)
    {
        auto position_of_needed_chunk = std::find_if(m_chunks.begin(), m_chunks.end(), Greater(n));

        if (position_of_needed_chunk == m_chunks.end())
            throw std::runtime_error("Error: There is not such amount memory in chunks!!!!");

        while (position_of_needed_chunk->is_empty())
        {
            if (position_of_needed_chunk == m_chunks.end())
                throw std::runtime_error("Error: There is not such amount memory in chunks!!!!");

            position_of_needed_chunk++;
        }

        std::cout << "Allocated: " << n << " bytes.\n";
        return position_of_needed_chunk->get_chunk();
    }

    void deallocate(void *p, std::size_t n)
    {
        auto mem = (unsigned char *)p;
        auto position_of_needed_chunk = std::find_if(m_chunks.begin(), m_chunks.end(), Greater(n));
        while (position_of_needed_chunk != m_chunks.end())
        {
            if (position_of_needed_chunk->find_block(mem))
            {
                std::cout << "Deallocated: " << n << " bytes. \n";
                return;
            }
            position_of_needed_chunk++;
        }
    }

private:
    std::vector<Blocks> m_chunks;
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

        return (T *)m_alloc_helper->allocate(count_objects * sizeof(T));
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

template <class T>
using alloc_vector = std::vector<T, Semen_Allocator<T>>;
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


int main()
{
    Allocator_Helper allocator_helper;
    Semen_Allocator<char> semen_allocator(allocator_helper);
    alloc_vector<int> semens_vector({1, 2, 3}, semen_allocator);
    alloc_list<int> semens_list({1, 2, 3}, semen_allocator);
    alloc_set<int> semens_set({1, 2, 3}, std::less<int>{}, semen_allocator);
    alloc_unordered_set<int> semens_us(semen_allocator);
    return 0;
}