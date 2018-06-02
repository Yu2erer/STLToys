//
//  YY_iterator.h
//  STLToys
//
//  Created by ntian on 2018/5/2.
//  Copyright © 2018 ntian. All rights reserved.
//

#ifndef STLTOYS_YY_ITERATOR_H
#define STLTOYS_YY_ITERATOR_H

#include <cstddef>

namespace YY {

    struct input_iterator_tag { };
    struct output_iterator_tag { };
    struct forward_iterator_tag : public input_iterator_tag { };
    struct bidirectional_iterator_tag : public forward_iterator_tag { };
    struct random_access_iterator_tag : public bidirectional_iterator_tag { };

    // 迭代器最好继承这个 避免忘记
    template <typename Category, typename T,
            typename Distance = ptrdiff_t,
            typename Pointer = T*, typename Reference = T& >
    struct iterator {
        typedef Category    iterator_category;
        typedef T           value_type;
        typedef Distance    difference_type;
        typedef Pointer     pointer;
        typedef Reference   reference;
    };

    template <typename Iterator>
    struct iterator_traits {
        typedef typename Iterator::iterator_category    iterator_category;
        typedef typename Iterator::value_type           value_type;
        typedef typename Iterator::difference_type      difference_type;
        typedef typename Iterator::pointer              pointer;
        typedef typename Iterator::reference            reference;
    };

    // 对原生指针的 偏特化
    template <typename T>
    struct iterator_traits<T*> {
        typedef random_access_iterator_tag iterator_category;
        typedef T                          value_type;
        typedef ptrdiff_t                  difference_type;
        typedef T*                         pointer;
        typedef T&                         reference;
    };

    template <typename T>
    struct iterator_traits<const T*> {
        typedef random_access_iterator_tag iterator_category;
        typedef T                          value_type;
        typedef ptrdiff_t                  difference_type;
        typedef const T*                   pointer;
        typedef const T&                   reference;
    };

    template <typename Iterator>
    inline typename iterator_traits<Iterator>::iterator_category iterator_category(const Iterator&) {
        typedef typename iterator_traits<Iterator>::iterator_category category;
        return category();
    }

    template <typename Iterator>
    inline typename iterator_traits<Iterator>::value_type* value_type(const Iterator&) {
        return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
    }

    template <typename Iterator>
    inline typename iterator_traits<Iterator>::difference_type* distance_type(const Iterator&) {
        return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
    }

    template <typename InputIterator>
    inline typename iterator_traits<InputIterator>::difference_type __distance(InputIterator first, InputIterator last, input_iterator_tag) {
        typename iterator_traits<InputIterator>::difference_type n = 0;
        while (first != last) {
            ++first;
            ++n;
        }
        return n;
    }

    template <typename RandomAccessIterator>
    inline typename iterator_traits<RandomAccessIterator>::difference_type __distance(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag) {
        return last - first;
    }

    template <typename InputIterator>
    inline typename iterator_traits<InputIterator>::difference_type distance(InputIterator first, InputIterator last) {
        typedef typename iterator_traits<InputIterator>::iterator_category category;
        return __distance(first, last, category());
    }

    template <typename InputIterator, typename Distance>
    inline void __distance(InputIterator first, InputIterator last, Distance& n, input_iterator_tag) {
        while (first != last) { ++first; ++n; }
    }

    template <typename RandomAccessIterator, typename Distance>
    inline void __distance(RandomAccessIterator first, RandomAccessIterator last, Distance& n, random_access_iterator_tag) {
        n += last - first;
    }

    template <typename InputIterator, typename Distance>
    inline void distance(InputIterator first, InputIterator last, Distance& n) {
        typedef typename iterator_traits<InputIterator>::iterator_category category;
        return __distance(first, last, n, category());
    }

    template <typename InputIterator, typename Distance>
    inline void __advance(InputIterator &i, Distance n, input_iterator_tag) {
        while (n--) { ++i; }
    }

    template <typename BidirectionalIterator, typename Distance>
    inline void __advance(BidirectionalIterator &i, Distance n, bidirectional_iterator_tag) {
        if (n >= 0) {
            while (n--) { ++i; }
        } else {
            while (n++) { --i; }
        }
    }

    template <typename RandomAccessIterator, typename Distance>
    inline void __advance(RandomAccessIterator &i, Distance n, random_access_iterator_tag) {
        i += n;
    }

    template <typename InputIterator, typename Distance>
    inline void advance(InputIterator &i, Distance n) {
        typedef typename iterator_traits<InputIterator>::iterator_category category;
        __advance(i, n, category());
        // 也可以是 __advance(i, n, iterator_category(i));
    }

    template <typename Container>
    class back_insert_iterator {
    protected:
        Container* container;
    public:
        typedef output_iterator_tag iterator_category;
        typedef void                value_type;
        typedef void                difference_type;
        typedef void                pointer;
        typedef void                reference;

        explicit back_insert_iterator(Container& x) : container(x) {}
        back_insert_iterator<Container>& operator=(const typename Container::value_type& value) {
            container->push_back(value);
            return *this;
        }
        // 关闭功能
        back_insert_iterator<Container>& operator*() { return *this; }
        back_insert_iterator<Container>& operator++() { return *this; }
        back_insert_iterator<Container>& operator++(int) { return *this; }
    };
    // 辅助函数 为了方便使用 back_insert_iterator
    template <typename Container>
    inline back_insert_iterator<Container> back_inserter(Container &x) {
        return back_insert_iterator<Container>(x);
    }

    template <typename Container>
    class front_insert_iterator {
    protected:
        Container* container;
    public:
        typedef output_iterator_tag iterator_category;
        typedef void                value_type;
        typedef void                difference_type;
        typedef void                pointer;
        typedef void                reference;

        explicit front_insert_iterator(Container& x) : container(x) {}
        front_insert_iterator<Container>& operator=(const typename Container::value_type& value) {
            container->push_front(value);
            return *this;
        }
        // 关闭功能
        front_insert_iterator<Container>& operator*() { return *this; }
        front_insert_iterator<Container>& operator++() { return *this; }
        front_insert_iterator<Container>& operator++(int) { return *this; }
    };
    template <typename Container>
    inline front_insert_iterator<Container> front_inserter(Container& x) {
        return front_insert_iterator<Container>(x);
    }

    template <typename Container>
    class insert_iterator {
    protected:
        Container* container;
        typename Container::iterator iter;
    public:
        typedef output_iterator_tag iterator_category;
        typedef void                value_type;
        typedef void                difference_type;
        typedef void                pointer;
        typedef void                reference;

        insert_iterator(Container& x, typename Container::iterator i) : container(x), iter(i) {}
        insert_iterator<Container>& operator=(const typename Container::value_type& value) {
            iter = container->insert(iter, value);
            ++iter;
            return *this;
        }
        insert_iterator<Container>& operator*() { return *this; }
        insert_iterator<Container>& operator++() { return *this; }
        insert_iterator<Container>& operator++(int) { return *this; }
    };
    template <typename Container, typename Iterator>
    inline insert_iterator<Container> inserter(Container& x, Iterator i) {
        typedef typename Container::iterator iter;
        return insert_iterator<Container>(x, iter(i));
    }

    template <typename Iterator>
    class reverse_iterator {
    protected:
        Iterator current;
    public:
        typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
        typedef typename iterator_traits<Iterator>::value_type value_type;
        typedef typename iterator_traits<Iterator>::difference_type difference_type;
        typedef typename iterator_traits<Iterator>::pointer pointer;
        typedef typename iterator_traits<Iterator>::reference reference;

        typedef Iterator iterator_type;
        typedef reverse_iterator<Iterator> self;
    public:
        reverse_iterator() {}
        explicit reverse_iterator(iterator_type x) : current(x) {}
        reverse_iterator(const self& x) : current(x.current) {}
        iterator_type base() const { return current; }
        reference operator*() const {
            Iterator tmp = current;
            return *--tmp;
        }
        pointer operator->() const { return &(operator*()); }
        self& operator++() {
            --current;
            return *this;
        }
        self operator++(int) {
            self tmp = *this;
            --current;
            return tmp;
        }
        self& operator--() {
            ++current;
            return *this;
        }
        self operator--(int) {
            self tmp = *this;
            ++current;
            return tmp;
        }
        self operator+(difference_type n) const { return self(current - n); }
        self& operator+=(difference_type n) {
            current -= n;
            return *this;
        }
        self operator-(difference_type n) const { return self(current + n); }
        self& operator-=(difference_type n) {
            current += n;
            return *this;
        }
        reference operator[](difference_type n) const { return *(*this + n); }
    };

    template <typename T, typename Distance = ptrdiff_t>
    class istream_iterator {
        template <>
        friend bool operator==(const istream_iterator<T, Distance>& x, const istream_iterator<T, Distance>& y);
    protected:
        std::istream* stream;
        T value;
        bool end_marker;
        void read() {
            end_marker = (*stream) ? true : false;
            if (end_marker) {
                *stream >> value;
            }
            end_marker = (*stream) ? true : false;
        }
    public:
        typedef input_iterator_tag iterator_category;
        typedef T value_type;
        typedef Distance difference_type;
        typedef const T* pointer;
        typedef const T& reference;

        istream_iterator() : stream(&std::cin), end_marker(false) {}
        istream_iterator(std::istream& s) : stream(&s), end_marker(false) {}

        reference operator*() const { return value; }
        pointer operator->() const { return &(operator*()); }

        istream_iterator<T, Distance>& operator++() {
            read();
            return *this;
        }
        istream_iterator<T, Distance> operator++(int) {
            istream_iterator<T, Distance> tmp = *this;
            read();
            return tmp;
        }
    };
    template <typename T, typename Distance = ptrdiff_t>
    inline bool operator==(const istream_iterator<T, Distance> &x,
                                                   const istream_iterator<T, Distance> &y) {
        return x.stream == y.stream && x.end_marker == y.end_marker || x.end_marker == false && y.end_marker == false;
    }
    template <typename T>
    class ostream_iterator {
    protected:
        std::ostream* stream;
        const char* string;
    public:
        typedef output_iterator_tag iterator_category;
        typedef void                value_type;
        typedef void                difference_type;
        typedef void                pointer;
        typedef void                reference;

        ostream_iterator(std::ostream& s) : stream(&s), string(0) {}
        ostream_iterator(std::ostream& s, const char* c) : stream(&s), string(c) {}

        ostream_iterator<T>& operator=(const T& value) {
            *stream << value;
            if (string) { *stream << string; }
            return *this;
        }
        ostream_iterator<T>& operator*() { return *this; }
        ostream_iterator<T>& operator++() { return *this; }
        ostream_iterator<T>& operator++(int) { return *this; }
    };
}
#endif //STLTOYS_YY_ITERATOR_H
