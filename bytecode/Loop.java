public class Loop {
  static int myMethod(int x) {
    int sum = 0;
    for (int i = 0; i < x; i++) 
    {
      sum = sum + i;
    }
    return sum;
  }

  public static void main(String[] args) 
  {
    System.out.println(myMethod(100));
  }
}