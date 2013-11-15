import java.util.*;
public class BT{    
    private BTNode root;
    private int t;
    public BT(int t)
    {
        root = null;
        this.t = t;
    }
    
    public final void traverse()
    {
        if(root != null)
            root.traverse();    
    }
    
    public BTNode search(int k)
    {
        if(root != null)
            root.search(k);  
        
        return(null);
    }
    
    public void insert(int k)
    {
        if(root == null){
            root = new BTNode(t, true);
            root.keys[0] = k;
            root.current = 1;
        }
        
        else{
            if(root.current == 2*t-1){
                BTNode s = new BTNode(t, false);
                s.child[0] = root;
                splitChild(s, 0, root);
                
                int i =0;
                if(s.keys[0] < k)
                    i++;
                
                insertNonFull(s.child[i], k);
                root = s;
            }
            else
            {
                insertNonFull(root, k);
            }
        }
    }
     public void insertNonFull(BTNode node,int k)
     {
         int i = node.current -1;
         if(node.leaf == true)
         {
             while(i>=0 && node.keys[i] >k)
             {
                 node.keys[i+1] = node.keys[i];
                 i--;
             }
             node.keys[i+1] = k;
             node.current = node.current+1;
         }
         else
         {
             while(i>=0 && node.keys[i] > k)
                 i--;
             
             if(node.child[i+1].current == 2*t-1)
             {
                 splitChild(node,i+1, node.child[i+1]);
                 if(node.keys[i+1] < k)
                     i++;
             }
             insertNonFull(node.child[i+1],k);
         }
     }
     
     public void splitChild(BTNode node, int i, BTNode y)
     {
         BTNode z = new BTNode(y.t, y.leaf);
         z.current = t-1;
         
         for(int j=0; j<t-1; j++)
             z.keys[j] = y.keys[j+t];
         
         if(y.leaf == false)
         {
             for(int j=0; j<t; j++)
                 z.child[j] = y.child[j+t];
         }
         
         y.current = t-1;
         
         for(int j= node.current; j >= i+1; j--)
             node.child[j+1] = node.child[j];
         
         node.child[i+1] = z;
         
         for(int j = node.current-1; j>=i; j--)
             node.keys[j+1] = node.keys[j];
         
         node.keys[i] = y.keys[t-1];
         node.current = node.current +1;
     }
     
     public void remove( int k)
     {
         if(root == null)
         {
             System.out.println("The tree is empty\n");
             return;
         }
         root.remove( k);
         if(root.current == 0)
         {
             BTNode temp = root;
             if(root.leaf)
                 root = null;
             else
                 root = root.child[0];
             
             temp = null;
         }
         return;
     }
         
     
     public static void main(String[] args)
     {
         BT t = new BT(3);
         Random r = new Random();
         /*for(int i=0; i<20; i++)
         {
             t.insert(r.nextInt(100));
         }*/
         Scanner in = new Scanner(System.in);
         
         int[] obj = new int[20];
         for(int i =0; i<20; i++)
         {
             obj[i] = r.nextInt(100);
             t.insert(obj[i]);
         }
      
         t.traverse();
         while(true){
         System.out.print("\n");
         System.out.println("Enter a key to delete:\n");
         int num = in.nextInt();
         t.remove(num);
         t.traverse();
         }
         
     }
}
         
         
         
                 
         
    

    