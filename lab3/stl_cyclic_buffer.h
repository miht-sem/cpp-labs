#include <iostream>
#include <algorithm>
#include <vector>
#include <ostream>

template <class Tp, class Allocator = std::allocator<Tp>>
class Cyclic_Buffer
{
private:
    Tp *_buffer;
    size_t _head = 0;
    size_t _tail = 0;
    size_t _max_size;
    Allocator _alloc;

    struct iterator : std::iterator<std::random_access_iterator_tag, Tp>
    {
        int index;
        friend class Cyclic_Buffer;
        Cyclic_Buffer *cyc_buf;

        iterator(size_t index, Cyclic_Buffer *cyc_buf) : index(index), cyc_buf(cyc_buf) {}

        Tp &operator*() const { return cyc_buf->_buffer[index]; }

        Tp *operator->() const { return cyc_buf->_buffer[index]; }

        iterator operator++(int)
        {
            auto before = *this;
            ++(*this);
            return before;
        }

        iterator &operator++()
        {
            index = (index + 1) % cyc_buf->_max_size;
            return *this;
        }

        iterator &operator+=(int n)
        {
            index = (index + n) % cyc_buf->_max_size;
            return *this;
        }

        iterator operator--(int)
        {
            auto before = *this;
            --(*this);
            return before;
        }

        iterator &operator--()
        {
            index = index > 0 ? index - 1 : cyc_buf->_max_size - 1;
            return *this;
        }

        iterator &operator-=(int n) { return *this += -n; }

        Tp &operator[](int n) const { return cyc_buf->_buffer[n]; }
    };

    struct const_iterator : std::iterator<std::random_access_iterator_tag, const Tp>
    {
        size_t index;
        friend class Cyclic_Buffer;
        Cyclic_Buffer *cyc_buf;

        const_iterator(size_t index, Cyclic_Buffer *cyc_buf) : index(index), cyc_buf(cyc_buf) {}

        const_iterator(iterator it) : const_iterator(it.index, it.cyc_buf) {}

        const Tp &operator*() const { return cyc_buf->_buffer[index]; }

        const Tp *operator->() const { return cyc_buf->_buffer[index]; }

        const_iterator operator++(int)
        {
            auto before = *this;
            ++(*this);
            return before;
        }

        const_iterator &operator++()
        {
            index = (index + 1) % cyc_buf->_max_size;
            return *this;
        }

        const_iterator &operator+=(int n)
        {
            index = (index + n) % cyc_buf->_max_size;
            return *this;
        }

        const_iterator operator--(int)
        {
            auto before = *this;
            --(*this);
            return before;
        }

        const_iterator &operator--()
        {
            index = index > 0 ? index - 1 : cyc_buf->_max_size - 1;
            return *this;
        }

        const_iterator &operator-=(int n) { return *this += -n; }

        const Tp &operator[](int n) const { return cyc_buf->_buffer[n]; }
    };

public:
    using allocator_type = Allocator;
    using allocator_pointer = typename std::allocator_traits<Allocator>::pointer;
    using alloc_traits = std::allocator_traits<Allocator>;

    Cyclic_Buffer<Tp, Allocator>(size_t max_size, Allocator allocator = std::allocator<Tp>()) : _max_size(max_size + 1), _alloc(allocator)
    {
        _buffer = allocator.allocate(max_size + 1);
    };

    Cyclic_Buffer<Tp, Allocator>(const Cyclic_Buffer<Tp> &cyc_buf) : Cyclic_Buffer<Tp, Allocator>(cyc_buf._max_size - 1)
    {
        _alloc = std::allocator_traits<Allocator>::select_on_container_copy_construction(_alloc);
        for (size_t i = 0; i <= _max_size; ++i) _alloc.construct(_buffer + i, cyc_buf._buffer[i]);
    };

    Cyclic_Buffer<Tp, Allocator>(std::initializer_list<Tp> list) : Cyclic_Buffer<Tp, Allocator>(list.size())
    {
        for (auto el : list) push_back(el);
    }

    ~Cyclic_Buffer<Tp, Allocator>() { _alloc.deallocate(_buffer, _max_size); }

    Cyclic_Buffer<Tp, Allocator> &operator=(const Cyclic_Buffer<Tp> &cyc_buf)
    {
        if (this == &cyc_buf)
            return *this;

        Tp *new_cyc_buf = _alloc.allocate(cyc_buf._max_size);
        size_t old_size = _max_size;
        _max_size = cyc_buf._max_size;

        for (size_t i = 0; i < _max_size; ++i)
        {
            _alloc.construct(new_cyc_buf + i, cyc_buf._buffer[i]);
            _alloc.destroy(_buffer + i);
        }

        _alloc.deallocate(_buffer, old_size);
        _buffer = new_cyc_buf;
        return *this;
    }

    void push_back(Tp item)
    {
        if (is_full()) change_max_size(_max_size * 2);
        _alloc.construct(_buffer + _tail, item);
        _tail = (_tail + 1) % _max_size;
    }

    void push_front(Tp item)
    {
        if (is_full()) change_max_size(_max_size * 2);
        size_t new_head = _head > 0 ? _head - 1 : _max_size - 1;
        _alloc.construct(_buffer + new_head, item);
        _head = new_head;
    }

    Tp pop_front()
    {
        if (is_empty()) throw std::runtime_error("Error: Buffer is empty!!!");
        Tp destroyed = _buffer[_head];
        _alloc.destroy(_buffer + _head);
        _head = (_head + 1) % _max_size;
        return destroyed;
    }

    Tp pop_back()
    {
        if (is_empty()) throw std::runtime_error("Error: Buffer is empty!!!");
        size_t new_tail = _tail > 0 ? _tail - 1 : _max_size - 1;
        Tp destroyed = _buffer[new_tail];
        _alloc.destroy(_buffer + new_tail);
        _tail = new_tail;
        return destroyed;
    }

    size_t size()
    {
        if (_tail >= _head) return _tail - _head;
        return _max_size - (_head - _tail);
    }


    void change_max_size(size_t new_max_size)
    {
        new_max_size++;
        if (new_max_size < size())  throw std::runtime_error("Error: You cannot make the buffer size smaller than the current number of elements in it!");
        if (_max_size == new_max_size) return;

        Tp *new_cyc_buf = _alloc.allocate(new_max_size);
        for (size_t i = 0; i < size(); ++i)
        {
            _alloc.construct(new_cyc_buf + i, operator[](i));
            _alloc.destroy(_buffer + ((_head + i) % new_max_size));
        }

        _head = 0;
        _tail = size() + 1;
        _alloc.deallocate(_buffer, _max_size);
        _buffer = new_cyc_buf;
        _max_size = new_max_size;
    }

    friend std::ostream &operator<<(std::ostream &out, Cyclic_Buffer &_buffer)
    {
        out << "\nBuffer Stat:";
        out << "\nSize/Max_Size: " << _buffer.size() << " / " << _buffer._max_size - 1 << '\n';
        if (_buffer.size() == 0) 
        {
            out << '\n';
            return out;
        }

        out << "Elements in buffer: ";
        for (size_t i = 0; i < _buffer.size(); ++i)
            out << _buffer[i] << (i == _buffer.size() - 1 ? "\n\n" : " ");
        return out;
    }

    Tp &operator[](size_t index) { return _buffer[(_head + index) % _max_size]; }

    const Tp &operator[](size_t index) const { return _buffer[(_head + index) % _max_size]; }

    bool is_empty() const { return _head == _tail; }

    bool is_full() const { return (_tail + 1) % _max_size == _head; }

    const Tp &front() const { return _buffer[_head]; }

    const Tp &back() const { return _buffer[_tail > 0 ? _tail - 1 : _max_size - 1]; }


    iterator begin() { return iterator(_head, this); }

    iterator end() { return iterator(_tail, this); }

    const_iterator begin() const { return const_iterator(_head, this); }

    const_iterator end() const { return const_iterator(_tail, this); }

    friend bool operator==(const iterator &lhs, const iterator &rhs) { return lhs.index == rhs.index; }

    friend bool operator!=(const iterator &lhs, const iterator &rhs) { return !(lhs == rhs); }

    friend bool operator<(const iterator &lhs, const iterator &rhs)
    {
        if (lhs.index > lhs.cyc_buf->_head && rhs.index > rhs.cyc_buf->_head) return lhs.index < rhs.index;
        else if (lhs.index > lhs.cyc_buf->_head) return true;
        else if (rhs.index > rhs.cyc_buf->_head) return false;
        else return lhs.index < rhs.index;
    }

    friend bool operator<=(const iterator &lhs, const iterator &rhs) { return !(rhs > lhs); }

    friend bool operator>(const iterator &lhs, const iterator &rhs) { return rhs < lhs; }

    friend bool operator>=(const iterator &lhs, const iterator &rhs) { return !(lhs < rhs); }

    void swap(iterator &a, iterator &b) { std::swap(a.index, b.index); }
    
    void swap(const_iterator &a, const_iterator &b) { std::swap(a.index, b.index); }

    friend iterator operator+(iterator it, int n)
    {
        it += n;
        return it;
    }

    friend iterator operator-(iterator it, int n)
    {
        it -= n;
        return it;
    }

    friend iterator operator+(int n, iterator it) { return it + n; }

    friend int operator-(const iterator &lhs, const iterator &rhs)
    {
        int l = lhs.index >= lhs.cyc_buf->_head ? lhs.index - lhs.cyc_buf->_head : lhs.cyc_buf->_max_size - (lhs.cyc_buf->_head - lhs.index);
        int r = rhs.index >= rhs.cyc_buf->_head ? rhs.index - rhs.cyc_buf->_head : rhs.cyc_buf->_max_size - (rhs.cyc_buf->_head - rhs.index);
        return l - r;
    }

    friend bool operator==(const const_iterator &lhs, const const_iterator &rhs)
    {
        return lhs.index == rhs.index;
    }

    friend bool operator!=(const const_iterator &lhs, const const_iterator &rhs)
    {
        return !(lhs == rhs);
    }

    friend bool operator<(const const_iterator &lhs, const const_iterator &rhs)
    {
        if (lhs.index > lhs._buffer->_head && rhs.index > rhs._buffer->_head)
            return lhs.index < rhs.index;
        else if (lhs.index > lhs._buffer->_head)
            return true;
        else if (rhs.index > rhs._buffer->_head)
            return false;
        else
        {
            return lhs.index < rhs.index;
        }
    }

    friend bool operator>(const const_iterator &lhs, const const_iterator &rhs) { return rhs < lhs; }

    friend bool operator<=(const const_iterator &lhs, const const_iterator &rhs) { return !(rhs > lhs); }

    friend bool operator>=(const const_iterator &lhs, const const_iterator &rhs) { return !(lhs < rhs); }

    friend const_iterator operator+(const_iterator it, int n)
    {
        it += n;
        return it;
    }

    friend const_iterator operator+(int n, const_iterator it) { return it + n; }

    friend const_iterator operator-(const_iterator it, int n)
    {
        it -= n;
        return it;
    }

    friend int operator-(const const_iterator &lhs, const const_iterator &rhs)
    {
        int l = lhs.index >= lhs._buffer->_head ? lhs.index - lhs._buffer->_head : lhs._buffer->_max_size - (lhs._buffer->_head - lhs.index);
        int r = rhs.index >= rhs._buffer->_head ? rhs.index - rhs._buffer->_head : rhs._buffer->_max_size - (rhs._buffer->_head - rhs.index);
        return l - r;
    }
};
