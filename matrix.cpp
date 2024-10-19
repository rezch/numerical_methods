#include <iostream>
#include "utils.h"

#define EPS 1e-8


template <class _Tp = int, int ROWS = 2, int COLS = 2>
struct Matrix {

    using _Mt = Matrix<_Tp, ROWS, COLS>;

#ifndef EPS
    #define _EPS 1e-9
#else
    #define _EPS EPS
#endif

    _Tp* operator [] (int index) {
        return data_[index];
    }

    struct iterator {
        using iterator_category = std::random_access_iterator_tag;
        using value_type        = _Tp;
        using difference_type   = ssize_t;
        using pointer           = _Tp*;
        using reference         = _Tp&;
        using const_reference   = const _Tp&;

        iterator(_Tp* m) : _pos(0), _data(m) { };
        iterator(_Tp* m, difference_type pos) : _pos(pos), _data(m) { };

        inline bool operator == (const iterator& other) const {
            return _pos == other._pos;
        }
        inline bool operator != (const iterator& other) const {
            return _pos != other._pos;
        }
        inline bool operator < (const iterator& other) const {
            return _pos < other._pos;;
        }
        inline bool operator <= (const iterator& other) const {
            return _pos <= other._pos;;
        }
        inline bool operator > (const iterator& other) const {
            return _pos > other._pos;;
        }
        inline bool operator >= (const iterator& other) const {
            return _pos >= other._pos;;
        }

        iterator& operator += (const difference_type& movement) {
            _pos += movement;
            return *this;
        }
        iterator& operator -= (const difference_type& movement) {
            _pos -= movement;
            return *this;
        }
        iterator& operator ++ () {
            ++_pos;
            return *this;
        }
        iterator& operator -- () {
            --_pos;
            return *this;
        }
        iterator operator ++ (int) {
            auto tmp(*this);
            ++_pos;
            return tmp;
        }
        iterator operator -- (int) {
            auto tmp(*this);
            --_pos;
            return tmp;
        }
        iterator operator + (const difference_type& movement) const {
            auto tmp(*this);
            tmp += movement;
            return tmp;
        }
        iterator operator - (const difference_type& movement) const {
            auto tmp(*this);
            tmp -= movement;
            return tmp;
        }

        difference_type operator + (const iterator& other) const {
            return _pos + other._pos;
        }
        difference_type operator - (const iterator& other) const {
            return _pos - other._pos;
        }

        reference operator * () {
            return _data[_pos];
        }
        const_reference operator * () const {
            return _data[_pos];
        }
        pointer operator-> () {
            return &_data[_pos];
        }

    private:
        difference_type _pos{};
        pointer         _data;
    };
    
    iterator begin() const {
        return { (_Tp*)data_, 0 };
    }

    iterator end() const {
        return { (_Tp*)data_, ROWS * COLS };
    }

    void T() {
        for (int i = 0; i < ROWS; ++i)
            for (int j = i + 1; j < COLS; ++j)
                std::swap(data_[i][j], data_[j][i]);
    }

    template <int R, int C>
    _Mt operator * (Matrix<_Tp, R, C>& other) const {
        static_assert(COLS == R);
        Matrix<_Tp, ROWS, C> tmp;
        tmp.to_zeros();
        other.T();
        for (int i = 0; i < ROWS; ++i)
            for (int j = 0; j < C; ++j)
                for (int k = 0; k < R; ++k)
                    tmp[i][j] += data_[i][k] * other.data_[j][k];
        other.T();
        return tmp;
    }

    template <int R, int C>
    _Mt operator * (const Matrix<_Tp, R, C>& other) const {
        static_assert(COLS == R);
        Matrix<_Tp, R, C> otherCopy(other);
        return *this * otherCopy;
    }

#define FOR for (int i = 0; i < ROWS; ++i) for (int j = 0; j < COLS; ++j)

    void to_zeros() { FOR data_[i][j] = 0; }

    void to_ones() {
        to_zeros();
        for (int i = 0; i < ROWS; ++i)
            data_[i][i] = 1;
    }

    _Mt& operator + (const _Mt& other) const {
        _Mt tmp(*this);
        FOR tmp[i][j] += other.data_[i][j];
        return tmp;
    }

    _Mt& operator += (const _Mt& other) {
        FOR data_[i][j] += other.data_[i][j];
        return *this;
    }

    _Mt& operator - (const _Mt& other) const {
        _Mt tmp(*this);
        FOR tmp[i][j] -= other.data_[i][j];
        return tmp;
    }

    _Mt& operator -= (const _Mt& other) {
        FOR data_[i][j] -= other.data_[i][j];
        return *this;
    }

#undef FOR
#undef _EPS

    _Tp data_[ROWS][COLS];
    static constexpr int rows = ROWS;
    static constexpr int cols = COLS;
};


signed main() {
    Matrix<float, 3, 3> a = {{
        {4, 1, 5},
        {2, 3, 2},
        {3, 0, 4},
    }};
    Matrix<float, 3, 3> b = {{
        {1, 1, 3},
        {2, 5, 2},
        {4, 2, 1},
    }};
    auto m = a * b;
    for (int i = 0; i < m.rows; ++i) {
        for (int j = 0; j < m.cols; ++j)
            std::cout << m[i][j] << ' ';
        write();
    }
    write();
    std::sort(m.begin(), m.end());
    for (int i = 0; i < m.rows; ++i) {
        for (int j = 0; j < m.cols; ++j)
            std::cout << m[i][j] << ' ';
        write();
    }
}
