using System;
using System.Diagnostics;
using System.Numerics;
using System.Collections.Generic;


/* Miller Rabin Primality Test */
/* input n > 3 -- an odd integer to test for primality 
 * input k, the number of rounds of testing to perform
 * output "composite" if n is found to be composite, 
 * output "prime" if n is "probably prime"
 * let 
*/
/* Write n as 2^r d + 1 -- by factoring out powers of 2 from n-1 */
namespace PrimeTests
{
    /// <summary>
        /// Input a candidate prime n.
        /// Get d and s such that n-1 = 2^s * d. 
        /// Generate several random witnesses a.
        /// Evaluate several witnesses against n:
        /// If a^d not congruent to +-1 mod n and 
        /// if a^(2^r d) not congruent to -1 for all 1< r < s, 
        /// then n is definitely composite and a is a witness for 
        /// n's compositeness.
        /// </summary>
    public class MillerRabin
    {
        static List<int> primes;
        static List<int> pseudos;
        static List<int> comps; // TODO add list of composites

        static int d;
        static int s;
        static int numWitnesses = 100;

        public MillerRabin() { }

        public List<Tuple<int, bool>> GetResults(List<int> data)
        {
            List<Tuple<int, bool>> table = new List<Tuple<int, bool>>();
            Tuple<int, bool> result;
            bool isprime;
            foreach (int datum in data) {
                isprime = IsPrime(datum);
                result = new Tuple<int, bool>(datum, isprime);
                table.Add(result);
            }
            return table;
        }

        public static bool IsPrime(int n)
        {
            /* trivial */
            if (n == 2)
                return true;
            if (n % 2 == 0)
                return false;

            int a, i, r;
            Random rng = new Random(); 

            CalcParams(n); // get s and d

            for (i = 0; i < numWitnesses; i++)
            {
                r = 0;
                a = rng.Next(2, n - 1); // a in [2, n-2]. 0^x is 0 mod y for all y, 1 and n-1 are bad witnesses, and anything > n-1 we can reduce mod n.
                a = PowMod(a, d, n);

                while (r < s && a != 1 && a != n-1)
                {
                    a = PowMod(a, 2, n);
                    ++r;
                }

                if (r == s) 
                    return false; // mult by a mod n is not a cyclic subgroup, a and n are not relatively prime. And since a < n, n composite.
            }

            return true; // What is the probability that n is prime given numWitnesses?
        }

        public static void TestPowMod()
        {
            // 17 ^ 44 % 23 = 1.
            int want = 1;
            int got = PowMod(17, 44, 23);
            Debug.Assert(want == got);
        }

        /// <summary>
        /// O(lg(exp)) time b^exp mod n
        /// </summary>
        /// <param name="b"></param>
        /// <param name="exp"></param>
        /// <param name="mod"></param>
        /// <returns></returns>
        public static int PowMod(int b, int exp, int n)
        {
            if (b == 0 || exp < 0)
                return 0; 

            int result = 1;
            int power = b;

            while (exp > 0) {
                power *= power;
                power %= n;
                if (exp % 2 == 1)
                {
                    result *= power;
                    result %= n;
                }
                exp >>= 1;
            }

            if (result < 0)
                return result + n;
            return result;
        }

        public static void TestCalcParams()
        {
            int n = 1 + 16 * 51;
            int swant = 4;
            int dwant = 51;
            CalcParams(n);
            Debug.Assert(swant == s);
            Debug.Assert(dwant == d);
        }

        /// <summary>
        /// Fill the s and d parameters.
        /// 2^s * d = n-1 for d odd.
        /// </summary>
        static void CalcParams(int n)
        {
            n -= 1;

            int count = 0;
            while (n % 2 == 0) {
                n >>= 1;
                ++count;
            }

            s = count;
            d = n;
        }

    }

}
