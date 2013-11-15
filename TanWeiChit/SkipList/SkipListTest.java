import java.util.*;
public class SkipListTest {
   public static void main(String args[]){
      Random rand = new Random();
      SkipList L = new SkipList ();
      Scanner in = new Scanner(System.in);
      for (int i = 0; i < 10; i++) {
          int low = 10;
          int high = 100;
          int R = rand.nextInt(high-low) +low;
          int key = R;
          L.insert(key);
      }
      L.print();
      while(true){
      System.out.println("Enter a key to Insert:\n");
      int num = in.nextInt();
      L.insert(num);
      L.print();
      System.out.println("Enter a key to Delete:\n");
      int nom = in.nextInt();
      L.delete(nom);
      L.print();
      }
   }   
}