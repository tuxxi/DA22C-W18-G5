#pragma once

#include <cmath>
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

    static PrimeNumbers Primes;
}
