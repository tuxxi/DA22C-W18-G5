#pragma once

#include "Vector.h"
namespace Util
{
    class PrimeNumbers
    {
    public:
        PrimeNumbers() noexcept; //declare the ctor noexcept so clang-tidy doesn't yell at me for static storage allocation
        int getNearestPrime(int n);
    private:
        // modified binary search to find nearest prime less than N
        int binarySearch(int left, int right, int n);
        // Utility function, Sieve of Sundaram
        void CreatePrimes();
        // array to store all primes
        Vector<int> primes;
        static const int MAX_PRIME = 100000;
    };

#if __cplusplus > 201103L //C++14 only in these parts

    namespace Detail
    {
        // C++14 compile time square root using binary search
        //from https://baptiste-wicht.com/posts/2014/07/compile-integer-square-roots-at-compile-time-in-cpp.html
        template <typename T>
        constexpr T sqrt_helper(T x, T lo, T hi)
        {
            if (lo == hi)
                return lo;

            const T mid = (lo + hi + 1) / 2;

            if (x / mid < mid)
                return sqrt_helper(x, lo, mid - 1);

            return sqrt_helper(x, mid, hi);
        }
    }
    template <typename T>
    constexpr T ct_sqrt(T x)
    {
        return Detail::sqrt_helper<T>(x, 0, x / 2 + 1);
    }
#else // <= 201103L
    //C++11 compile time square root using simple iteration
    //much slower than C++14 version
    template <typename T>
    constexpr T ct_sqrt(T n, T i = 1) {
        return n == i ? n : (i * i < n ? ct_sqrt(n, i + 1) : i);
    }
#endif //__cplusplus

    static PrimeNumbers Primes;
}
