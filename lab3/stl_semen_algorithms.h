namespace semen_algorithms
{
    template <typename _InputIterator, typename _Predicate>
    inline bool all_of(_InputIterator __first, _InputIterator __last, _Predicate __pred)
    {
        return __last == std::find_if_not(__first, __last, __pred);
    }

    template <typename _InputIterator, typename _Predicate>
    inline bool any_of(_InputIterator __first, _InputIterator __last, _Predicate __pred)
    {
        return !std::none_of(__first, __last, __pred);
    }

    template <typename _InputIterator, typename _Predicate>
    inline bool none_of(_InputIterator __first, _InputIterator __last, _Predicate __pred)
    {
        return __last == std::find_if(__first, __last, __pred);
    }

    template <typename _InputIterator, typename _Predicate>
    bool one_of(_InputIterator __first, _InputIterator __last, _Predicate __pred)
    {
        _InputIterator i = std::find_if(__first, __last, __pred);
        if (i == __last)
            return false;
        return std::none_of(++i, __last, __pred);
    }

    template <typename _ForwardIterator, typename _Compare>
    inline bool is_sorted(_ForwardIterator __first, _ForwardIterator __last, _Compare __comp)
    {
        return std::is_sorted_until(__first, __last, __comp) == __last;
    }

    template <typename _InputIterator, typename _Predicate>
    inline bool is_partitioned(_InputIterator __first, _InputIterator __last, _Predicate __pred)
    {
        __first = std::find_if_not(__first, __last, __pred);
        if (__first == __last)
            return true;
        ++__first;
        return std::none_of(__first, __last, __pred);
    }

    template <typename _InputIterator, typename _Type>
    _InputIterator find_not(_InputIterator __first, _InputIterator __last, const _Type &value)
    {
        while (__first != __last)
        {
            if (*__first != value)
                return __first;
            __first++;
        }
        return __last;
    }

    template <typename _InputIterator, typename _Type>
    _InputIterator find_backward(_InputIterator __first, _InputIterator __last, const _Type &value)
    {
        auto temp_last_it = __last;
        while (temp_last_it != __first)
        {
            temp_last_it--;
            if (*temp_last_it == value)
                return temp_last_it;
        }
        return __last;
    }

    template <typename _InputIterator, typename _Predicate>
    bool is_palindrom(_InputIterator __first, _InputIterator __last, _Predicate __pred)
    {
        while (__first != __last)
        {
            __last--;
            if (!__pred(*__first, *__last))
                return false;
            if (__first == __last)
                return true;
            __first++;
        }
        return true;
    }
}