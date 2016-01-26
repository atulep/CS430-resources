
public class Factors {


	public static boolean isPrime(long n) 
	{
        long sqrtn = (long) Math.ceil(Math.sqrt(n));
		for (int i = 2; i < sqrtn; i++) {
			if (n % i == 0)  return false;
		}
		return true;
	}

  	public static String primeFactorization(long n)
    {
        if (n == 1) return "1";

        String result = "";
        long sqrtn = (long) Math.ceil(Math.sqrt(n));
        long number = n;

        for (int i=2; i<=sqrtn; i++)
        {
            // if i is prime, see how many times it divides into number
            if (isPrime(i))
            {
                while (number % i == 0)
                {
                    result = result + i;
                    number = number/i;
                    if (number != 1) 
                        result = result + " * ";
                }
            }
            if (number == 1) break;
        }
        if (number > 1) 
            result = result + number;
        return result;
    }

  	public static void main (String args[]) 
	{

			if (args.length == 0)  {
				System.out.println("Usage: java Factors <n>");
				System.exit(1);
			}
			System.out.println(primeFactorization(Long.parseLong(args[0])));
	}

}
