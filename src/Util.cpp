//
// Created by tuxxi on 3/20/18.
//

#include "Util.h"

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

CRC32Interface::CRC32Interface() noexcept
{
    CreateTable();
}

//Calcuates an ISO 3309 CRC-32 checksum for a given message and size
//see https://en.wikipedia.org/wiki/Cyclic_redundancy_check#CRC-32_algorithm
unsigned CRC32Interface::CalcCRC32(const char* message, unsigned int size)
{
    unsigned int crc = 0xFFFFFFFF; //initial value

    for (int i = 0; i < size; ++i)
    {
        const unsigned index = (crc ^ message[i]) & 0xFF;
        crc = (crc >> 8) ^ table[index]; //use the stored table to calcuate CRC
    }
    return ~crc;

}
// Creates the static lookup table for CRC-32, this makes computation extremely efficent
// since these table values are used in every cycle of CRC
void CRC32Interface::CreateTable()
{
    for (int byte = 0; byte < CRC_TABLE_SIZE; byte++)
    {
        unsigned crc = byte;
        for (int j = 7; j >= 0; j--)
        {
            const unsigned mask = -(crc & 1);
            crc = (crc >> 1) ^ (0xEDB88320 & mask); //0xEDB88320 is the polynomial for ISO 3309 CRC-32
            //x^32 + x^26 + x^23 + x^16 + x^12 + x^11 + x^10 + x^8 + x^7 + x^5 + x^4 + x^2 + x + 1 
        }
        table[byte] = crc;
    }
}