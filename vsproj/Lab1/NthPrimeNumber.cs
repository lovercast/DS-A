using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Text;

/* Generate the nth prime number given n from the user. */

/* Method 1 sieve */
/* how big to make the sieve array -- ?
 * What if the user wants to request a number which is way bigger than the input number/
 * What if I prompt the user for input in a range of exponents.
 * Or you could use a moving average of the inputs of the user --
*/

/* Method 2 iterative prime number generation. 
 * phi(n) ~ n/log(n) = k 
 * what is an upper bound for the nth prime? ( Stirling)
 */
namespace Lab1
{
    public class NthPrimeNumber
    {

        public static void TestSieve()
        {
            // Console.WriteLine(primes[0]);
            int[] indices = { 1, 5, 100, 1000 };
            int[] want = { 2, 11, 541, 7919 };

            for (int i = 0; i < indices.Length; i++)
            {
                List<int> primes = NthPrimeNumber.Sieve(indices[i]);
                Debug.Assert(primes[indices[i]-1] == want[i]); // works!
            }
            return;
        }

        public static List<int> Sieve(int n)
        {
            if (n < 1)
                return null;

            // this bound doesn't work! We need the inverse of this function!
            int lim2 = 2 * n * (int)Math.Log(n)+ n; // how long does the sieve need to be? phi(x) ~ x/log(x) -- so sieve needs to be ceiling  -- c x n log n
            int lim3 = n * n;
            int lim = n > 10 ?  n * (int) Math.Log(n) * (int) Math.Log(n) : lim2;


            List<int> primes = new List<int>();
            // create a sieve --
            // write output to a file to save the primes
            int i;

            int[] sieve = new int[lim]; 
            for (i = 0; i < sieve.Length; i++) {
                sieve[i] = i+2;
            }

            // upper limit of testing is ceil(sqrt(n))
            for (i = 0; i < sieve.Length; i++)
            {
                if (sieve[i] != 0)
                {
                    primes.Add(sieve[i]);
                    // if (primes.Count > n)
                        // return primes;
                    if (i+2 < Math.Sqrt(sieve.Length))
                        SieveNum(sieve, sieve[i]);
                }
            }

            /* all remaining non-zero numbers in the array are prime.
             */
            Debug.Assert(primes.Count >= n);
            // Console.WriteLine(primes.Count);
            return primes;
        }

        /// <summary>
        /// Sieve all the multiples of x from the sieve array. 
        /// </summary>
        /// <param name="sieve">An array containing candidate primes. </param>
        /// <param name="x"></param>
        public static void SieveNum(int[] sieve, int x)
        {
            int acc = x + x; 
            while (acc-2 < sieve.Length) // TODO fix this -- what should the bound be for this? acc-2 maps a number onto its index in sieve
            {
                sieve[acc-2] = 0;
                acc += x;
            }
        }

        /// <summary>
        /// Calculate the primes up to lim and write the results into csv.
        /// </summary>
        /// <param name="lim"></param>
        public static void WritePrimesToFile(int lim)
        {
            const int rowlength = 100;
            string numstr;
            StringBuilder row;

            string filename = "/Users/levio/dsa/primes.csv";
            StreamWriter sw;
            using (sw = new StreamWriter(filename))
            {
                List<int> primes = Sieve(lim);
                row = new StringBuilder();
                for (int i = 0; i < primes.Count; i++) {

                    numstr = primes[i].ToString();

                    if (i == 0)
                        row = row.Append($"{numstr}");
                    else
                        row = row.Append($",{numstr}");

                    if (i % rowlength == rowlength - 1)
                    {
                        sw.WriteLine(row.ToString());
                        row = new StringBuilder();
                    }
                }
            }
        }
    }
}
