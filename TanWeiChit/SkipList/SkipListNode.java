public class SkipListNode{
    
    protected int key;
    protected SkipListNode [] next;
    
    SkipListNode(int key, int h){
        this.key = key;
        this.next = new SkipListNode[h+1];
    }
}
        