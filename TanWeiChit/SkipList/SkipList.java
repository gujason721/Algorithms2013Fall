public class SkipList{
    
    private SkipListNode head;
    private SkipListNode tail;
    private int h;
    public int maxH;
    public static final double P=0.5;
    public static final int Max = 6;
    
    private SkipListNode[] update;
    
    public SkipList(){
        h = 0;
        maxH = 0;
        
        head = new SkipListNode(Integer.MIN_VALUE, 0);
        tail = new SkipListNode(Integer.MAX_VALUE, 0);
        
        head.next[0] = tail;
        update = new SkipListNode[h+1];
    }
    
    private int randheight(){
        int h = 0;
        h = (int)(Math.log(1.-Math.random())/Math.log(1.-P));
        return Math.min(h, Max);
    }
    
    public SkipListNode search(int key){
        SkipListNode p = head;
        for(int i =h; i >= 0; i--){
            while(p.next[i].key < key){
                p=p.next[i];
            }
            p = p.next[0];
            if(p.key == key && p!= tail)
                return p;
            else
                return null;
        }
        return null;
    }
    
    public void insert(int key){
        SkipListNode p = head;
        for(int i = h; i>=0; i--){
            while(p.next[i].key<key)
            {
                p=p.next[i];
            }
            update[i] = p;
        }
        p = p.next[0];
        /*if(p.key == key){
            System.out.println("KEY"+key+"FOUND");
            return;
        }*/
        int newH = randheight();
        if(newH > maxH){
            SkipListNode oldHead = head;
            head = new SkipListNode(Integer.MIN_VALUE, newH);
            for(int i=0; i<=maxH; i++)
                head.next[i] = oldHead.next[i];
            for(int i = maxH+1; i<=newH; i++)
                head.next[i] = tail;
            
            maxH = newH;
            
            for(int i = h; i>=0 && update[i] == oldHead; i--)
                update[i] = head;
            
            SkipListNode[] oldUpdate = update;
            update = new SkipListNode[newH + 1];
            for(int i =0; i<= h; i++)
                update[i] = oldUpdate[i];
        }
        if(newH > h){
            for(int i = h+1; i <= newH; i++)
                update[i] = head;
            h = newH;
        }
        
        p = new SkipListNode(key, newH);
        
        for(int i=0; i<= newH; i++)
        {
            p.next[i] = update[i].next[i];
            update[i].next[i] =p;
        }
    }
    
    public void delete(int key){
        SkipListNode p =head;
        SkipListNode[] update = new SkipListNode[h+1];
        
        for(int i =h; i>=0; i--)
        {
            while(p.next[i].key < key)
            {
                p= p.next[i];
            }
            update[i] = p;
        }
        p = p.next[0];
        
        if(p.key != key)
        {
            System.out.println("KEY " + key+ " NOT FOUND");
            return;
        }
        for(int i =0; i< p.next.length; i++)
        {
            update[i].next[i] = update[i].next[i].next[i];
        }
        
        while(h>=0 && head.next[h] == tail)
            h--;
        
        if(4*h <= maxH){
            maxH = 2*h;
            SkipListNode oldHead = head;
            head = new SkipListNode(Integer.MIN_VALUE, maxH);
            for(int i=0; i<=maxH; i++)
                head.next[i] = oldHead.next[i];
            
            update = new SkipListNode[maxH+1];
        }
    }
        public void print(){
            for(int i = h; i>=0; i--){
                SkipListNode p = head.next[i];
                SkipListNode q = head.next[0];
                while(p!=tail){
                    if(q == p){
                        System.out.print(p.key+" ");
                        p = p.next[i];
                    }
                    else System.out.print("   ");
                    q = q.next[0];
                }
                System.out.println();
            }
            System.out.println();
        }
    }
                    
            
        
        
        