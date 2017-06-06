#ifndef __MATH_HPP__
#define __MATH_HPP__

template <class T>
int sign(T x) {
    return (T(0) < x) - (x < T(0));
}

#endif // __MATH_HPP__
