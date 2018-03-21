//
// Created by tuxxi on 3/20/18.
//
#include "Util.h"
#include <cmath>

using namespace Util;

PrimeNumbers::PrimeNumbers() noexcept
{
    CreatePrimes();
}
int PrimeNumbers::getNearestPrime(int n)
{
    return binarySearch(0, int(primes.size() - 1), n);
}
int PrimeNumbers::binarySearch(int left, int right, int n)
{
    if (left<=right)
    {
        int mid = (left + right)/2;

        // base condition is, if we are reaching at left
        // corner or right corner of primes[] array then
        // return that corner element because before or
        // after that we don't have any prime number in
        // primes array
        if (mid == 0 || mid == primes.size()-1)
            return primes[mid];

        // now if n is itself a prime so it will be present
        // in primes array and here we have to find nearest
        // prime less than n so we will return primes[mid-1]
        if (primes[mid] == n)
            return primes[mid-1];

        // now if primes[mid]<n and primes[mid+1]>n that
        // mean we reached at nearest prime
        if (primes[mid] < n && primes[mid+1] > n)
            return primes[mid];
        if (n < primes[mid])
            return binarySearch(left, mid-1, n);
        else
            return binarySearch(mid+1, right, n);
    }
    return 0;
}
// Utility function of Sieve of Sundaram
void PrimeNumbers::CreatePrimes()
{
    constexpr int n = MAX_PRIME; //largest number we want to have support for

    // In general Sieve of Sundaram, produces primes
    // smaller than (2*x + 2) for a given number x
    constexpr auto nNew = int(ct_sqrt(n));

    // This array is used to separate numbers of the
    // form i+j+2ij from others where  1 <= i <= j
    int flags[n/2+500] = {0};

    // eliminate indexes which does not produce primes
    for (int i=1; i<=(nNew-1)/2; i++)
        for (int j=(i*(i+1))<<1; j<=n/2; j=j+2*i+1)
            flags[j] = 1;

    // Since 2 is a prime number
    primes.add(2);

    // Remaining primes are of the form 2*i + 1 such
    // that flags[i] is false.
    for (int i=1; i<=n/2; i++)
        if (flags[i] == 0)
            primes.add(2*i + 1);
}
