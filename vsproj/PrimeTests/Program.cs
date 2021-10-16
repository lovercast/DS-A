using System;
using System.Text;
using System.Collections.Generic;
using System.IO;
using System.Diagnostics;
using BenchmarkDotNet.Attributes;
using BenchmarkDotNet.Running;

// need a function to get next prime and put it in data
// what does the class do?
// multithreading in c#

namespace PrimeTests
{
    public class Benchmarking
    {
        public static readonly int lim = 300;

        List<int> primes;
        List<int> pseudos;
        List<int> comps; // TODO add list of composites

        public static List<Tuple<int, bool>> primesresults;
        public static List<Tuple<int, bool>> pseudosresults;

        MillerRabin mr;
        int cand;
        int i;

        public int Next()
        {
            if (i < primes.Count) {
                return primes[i++];
            } else if (i < primes.Count + pseudos.Count) {
                return pseudos[i++ - primes.Count];
            } else if (i < primes.Count + pseudos.Count + comps.Count) {
                return comps[i++ - primes.Count - pseudos.Count];
            } else {
                return -1;
            }
        }

        public void Initialize()
        {
            i = 0;

            Numbers nums = new Numbers(lim);
            nums.LoadNums();
            primes = nums.primes;
            pseudos = nums.pseudos;
            comps = nums.comps;

            mr = new MillerRabin();
        }

        public Benchmarking()
        {
            // how do I tell benchmark to stop running when GetNextPrime hits -1?
            cand = Next();
        }

        [Benchmark]
        public bool BenchmarkIsPrime() => MillerRabin.IsPrime(cand);
    }


    class Program
    {
        // TODO add BigInteger support to avoid overflows -- how much performance cost?
        // TODO add list of composites to check against.
        // TODO commit to github
        // TODO integrate benchmarking

        public static readonly int lim = 1000000;

        /* control how big the output file is */
        static readonly int writelimit = 10000;

        static List<int> primes;
        static List<int> pseudos;
        static List<int> comps; // TODO add list of composites

        /* benchmark and write results */
        public static void Main(string[] args)
        {
            // var summary = BenchmarkRunner.Run<MillerRabin>();
            Numbers nums = new Numbers(lim);

            nums.GenerateComps();
            nums.WriteCompsToFile();

            return;

            nums.LoadNums();
            primes = nums.primes;
            pseudos = nums.pseudos;

            MillerRabin mr = new MillerRabin();

            List<Tuple<int, bool>> primesresults = mr.GetResults(primes);
            List<Tuple<int, bool>> pseudosresults = mr.GetResults(pseudos);

            string primesresultsfile = "primeresults.txt";
            string pseudosresultsfile = "pseudosresults.txt";

            /* analyze results */
            Tuple<int, int> primesanalysis = AnalyzeAndWriteResults(primesresultsfile, primesresults);
            Tuple<int, int> pseudosanalysis = AnalyzeAndWriteResults(pseudosresultsfile, pseudosresults);

            int denom;

            Console.WriteLine("==============");
            Console.WriteLine("Primes results");
            Console.WriteLine("==============");
            Console.WriteLine($"\tNumbers ran: {primesresults.Count}");
            Console.WriteLine($"\tPositives:\t{primesanalysis.Item1}");
            Console.WriteLine($"\tNegatives:\t{primesanalysis.Item2}");
            denom = primesanalysis.Item1 + primesanalysis.Item2;
            Console.WriteLine($"\tTrue Positive Rate:\t{100.0  * (float)primesanalysis.Item1/denom}%");
            Console.WriteLine($"\tTrue Positive Rate:\t{100.0 * (float)primesanalysis.Item2/denom}%");

            Console.WriteLine("==============");
            Console.WriteLine("Pseudos results");
            Console.WriteLine("==============");
            Console.WriteLine($"\tNumbers ran: {pseudosresults.Count}");
            Console.WriteLine($"\tPositives:\t{pseudosanalysis.Item1}");
            Console.WriteLine($"\tNegatives:\t{pseudosanalysis.Item2}");
            Console.WriteLine($"\tNegatives:\t{pseudosanalysis.Item2}");
            denom = pseudosanalysis.Item1 + pseudosanalysis.Item2;
            Console.WriteLine($"\tFalse Positive Rate:\t{100.0 * ((float)pseudosanalysis.Item1/denom)}%");
            Console.WriteLine($"\tTrue Negative Rate:\t{100.0 * ((float)pseudosanalysis.Item2/denom)}%");

            return;
        }

        public static Tuple<int, int> AnalyzeAndWriteResults(string filepath, List<Tuple<int, bool>> results)
        {   
            StreamWriter sw;
            int yescount = 0;
            int nocount = 0;
            int writecount = 0;
            using (sw = new StreamWriter(filepath))
            {
                foreach (Tuple<int, bool> result in results)
                {
                    if (result.Item2)
                        ++yescount;
                    else
                        ++nocount;

                    if (writecount < writelimit)
                    {
                        sw.WriteLine($"{result.Item1} {result.Item2}");
                        ++writecount;
                    }
                }
            }
            return new Tuple<int, int>(yescount, nocount);
        }
    }


    class Numbers
    {
        public List<int> primes;
        public List<int> pseudos;
        public List<int> comps;
        public int lim;

        public Numbers(int lim)
        {
            primes = new List<int>();
            pseudos = new List<int>();
            comps = new List<int>();
            this.lim = lim;
        }

        enum FileType {
            PRIME,
            PSEUDO,
            COMP
        };
        /// <summary>
        /// Writes lim primes to a csv file.
        /// </summary>
        public void GeneratePrimes()
        { 
            Lab1.NthPrimeNumber.WritePrimesToFile(lim);
        }

        /* Generate a list of odd composite numbers
         * based on the list of primes. */
        public void GenerateComps()
        {
            GetList("primes.csv", FileType.PRIME);
            
            /* get a primes from the list - first prime. */
            int i = 1;
            int c = primes[1];
            foreach (int p in primes) {
                while (c < p) {
                    c += 2;
                    if (c < p) 
                        comps.Add(c);
                }
            }
        }

        public void WriteCompsToFile()
        { 
            const int rowlength = 100;
            string numstr;
            StringBuilder row;

            string compfile = "comps.csv";
            string cwd = Directory.GetCurrentDirectory();
            compfile = $"{cwd}/{compfile}";
            StreamWriter sw;
            using (sw = new StreamWriter(compfile))
            {
                row = new StringBuilder();
                for (int i = 0; i < comps.Count; i++) {

                    numstr = comps[i].ToString();

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

        public void TestLoadNums()
        {
            LoadNums();
            Console.WriteLine(primes.Count);
            Console.WriteLine(pseudos.Count);
        }

        /// <summary>
        /// Get primes from primefile and pseudos from pseudofile.
        /// </summary>
        public void LoadNums()
        { 
            string cwd = Directory.GetCurrentDirectory();
            string primefile = "primes.csv";
            string pseudofile = "pseudo.csv"; // pseudoprimes credit to oeis.org
            string compfile = "comps.csv";

            if (OperatingSystem.IsWindows()) {
                primefile = $"{cwd}\\{primefile}";
                pseudofile = $"{cwd}\\{pseudofile}";
                compfile = $"{cwd}\\{compfile}";
            }
            else { 
                primefile = $"{cwd}/{primefile}";
                pseudofile = $"{cwd}/{pseudofile}";
                compfile = $"{cwd}/{compfile}";
            }

            Debug.Assert(File.Exists(primefile));
            Debug.Assert(File.Exists(pseudofile));
            Debug.Assert(File.Exists(compfile));

            GetList(primefile,  FileType.PRIME);
            GetList(pseudofile, FileType.PSEUDO);
            GetList(compfile, FileType.COMP);
        }

        /// <summary>
        /// Get numbers from a file up to a limit.
        /// </summary>
        /// <param name="filepath"></param>
        /// <param name="flag"></param>
        /// <param name="lim"></param>
        void GetList(string filepath, FileType flag)
        {
            StreamReader sr;
            List<int> l = new List<int>();
            string line;
            string[] tokens;
            int p;

            using (sr = new StreamReader(filepath)) {
                while (l.Count < lim && (line = sr.ReadLine()) != null) {
                    tokens = line.Split(',');
                    foreach (string token in tokens) {
                        if (l.Count >= lim)
                            break;
                        token.Trim();
                        if (!int.TryParse(token, out p))
                            continue;
                        l.Add(p);
                    }
                }
             }

            switch (flag) {
                case FileType.PRIME:
                    primes = l;
                    break;
                case FileType.PSEUDO:
                    pseudos = l;
                    break;
                case FileType.COMP:
                    comps = l;
                    break;

            }
        }
    }

}
