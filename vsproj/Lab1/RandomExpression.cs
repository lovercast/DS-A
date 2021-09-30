using System;
namespace Lab1
{

    /* Generate a random arithmetic expression and quiz the user.
     * Display a message that fits their result. */
    /* 1 <= a, b <= 15, operators are in {+, -, *, /} */
    public class RandomExpression
    {

        public static int NumOperators = 4;

        public struct opAnswer {

            public char op;
            public int answer;

            public opAnswer(char op, int answer)
            {
                this.op = op;
                this.answer = answer;
            }
        }

        public static opAnswer O;

        private static void GetResult(int a, int b, int op)
        {
            opAnswer result = new opAnswer();
            switch(op) {
            case 0:
                    result.op = '*';
                    result.answer = a * b;
                    break;
            case 1:
                    if (b == 0)
                        throw new DivideByZeroException("You tried to divide divide by zero!");
                    result.op = '/';
                    result.answer = a / b;
                    break;
            case 2:
                    result.op = '+';
                    result.answer = a + b;
                    break;
            case 3:
                    result.op = '-';
                    result.answer =  a - b;
                    break;
            default:
                    Console.WriteLine("illegal operator");
                    result.op = '\0';
                    return;
            }
            O = result;
        }

           
        /// <summary>
        /// Generate two random numbers in the given range, 
        /// and generate a random operator from the set.
        /// Quiz the user.
        /// </summary>
        public static void Quiz()
        {
            
            Random rng = new Random();
            int a, b;
            a = rng.Next(1, 16);
            b = rng.Next(1, 16);

            int op = rng.Next(4);

            GetResult(a, b, op);

            if (O.op == '\0') {
                return;
            }

            /* Display problem and prompt for input */
            Console.Write($"Problem: {a}{O.op}{b} = ");

            int userAnswer;
            while (!int.TryParse(Console.ReadLine(), out userAnswer))
            {
                Console.WriteLine("Enter an INTEGER");
                Console.Write($"Problem: {a}{O.op}{b} = ");
            }

            if (userAnswer == O.answer) {
                Console.WriteLine("Yay! You nailed it!");
            } else {
                Console.WriteLine("SORRY 😭 you got it wrong.");
            }
        }
    }
}
