class BTNode{
    public int[] keys;
    public int t;
    public BTNode[] child;
    public int current;
    public boolean leaf;
    
    public BTNode(int t, boolean leaf)
    {
        this.t = t;
        this.leaf = leaf;
        
        keys = new int[2*t -1];
        child = new BTNode[2*t];
        current = 0;
    }
    
    public void traverse()
    {
        int i;
        for(i = 0; i< current; i++)
        {
            if(leaf == false)
            {
                child[i].traverse();
            }
            System.out.print(" ");
            System.out.print(keys[i]);
        }
        if(leaf == false)
        { 
            child[i].traverse();
        }
    }
    
    public BTNode search(int k)
    {
        int i=0;
        while(i<current && k>keys[i])
        {
            i++;
        }
        if(keys[i] == k)
            return this;
        
        if(leaf == true)
            return null;
        
        return child[i].search(k);
    }
    
    public int findKey(int k)
    {
        int idx = 0;
        while(idx<current && keys[idx] <k)
            ++idx;
        return idx;
    }
    
    public BTNode remove(int k)
    {
        int idx = findKey(k);
        if(idx < current && keys[idx] == k)
        {
            if(leaf)
                removeFromLeaf(idx);
            else
                removeFromNonLeaf(idx);
        }
        else
        {
            if(leaf)
            {
                System.out.println("The key" + k +" is does not exist in the tree");
                return null;
            }
            boolean flag = ((idx == current) ? true : false);
            /*{
                if(idx = current)
                    return(true);
                return(false);
            }*/
            if(child[idx].current < t)
                fill(idx);
            
            if(flag && idx > current)
                child[idx-1].remove(k);
            else
                child[idx].remove(k);
        }
        return null;
    }
    
    public BTNode removeFromLeaf(int idx)
    {
        for(int i=idx+1; i<current; ++i)
            keys[i-1] = keys[i];
        
        current--;
        return null;
    }
    
    public BTNode removeFromNonLeaf(int idx)
    {
        int k = keys[idx];
        if(child[idx].current >= t)
        {
            int pred = getPred(idx);
            keys[idx] = pred;
            child[idx].remove( pred);
        }
        else if(child[idx+1].current >= t)
        {
            int succ = getSucc(idx);
            keys[idx] = succ;
            child[idx+1].remove(succ);
        }
        else{
            merge(idx);
            child[idx].remove(k);
        }
        return null;
    }
    
    public int getPred(int idx)
    {
        BTNode pos = child[idx];
        while(!pos.leaf)
        {
            pos = pos.child[pos.current];
        }
        return(pos.keys[pos.current-1]);
    }
    
    public int getSucc(int idx)
    {
        BTNode pos = child[idx+1];
        while(!pos.leaf)
            pos = pos.child[0];
        
        return(pos.keys[0]);
    }
    
    public BTNode fill(int idx)
    {
        if(idx!=0 && child[idx-1].current == t)
            borrowFromPrev(idx);
        
        else if(idx!=current && child[idx+1].current == t)
            borrowFromNext(idx);
        
        else{
            if(idx != current)
                merge(idx);
            else
                merge(idx-1);
        }
        return null;
    }
    
    public BTNode borrowFromPrev(int idx)
    {
        BTNode nchild = child[idx];
        BTNode sibling = child[idx-1];
        
        for(int i = nchild.current-1; i>=0; --i)
            nchild.keys[i+1] = nchild.keys[i];
        
        if(!nchild.leaf)
        {
            for(int i = nchild.current; i>=0; --i)
                nchild.child[i+1] = nchild.child[i];
        }
        nchild.keys[0] = keys[idx-1];
        
        if(!leaf)
            nchild.child[0] = sibling.child[sibling.current];
        
        keys[idx-1] = sibling.keys[sibling.current-1];
        nchild.current += 1;
        sibling.current -= 1;
        return null;
    }
    
    public BTNode borrowFromNext(int idx)
    {
        BTNode nchild = child[idx];
        BTNode sibling = child[idx+1];
        nchild.keys[(nchild.current)] = keys[idx];
        
        if(!(nchild.leaf))
            nchild.child[(nchild.current)+1] = sibling.child[0];
        
        keys[idx] = sibling.keys[0];
        
        for(int i=1; i<sibling.current; ++i)
            sibling.keys[i-1] = sibling.keys[i];
        
        if(!sibling.leaf)
        {
            for(int i=1; i<= sibling.current; ++i)
                sibling.child[i-1] = sibling.child[i];
        }
        
        nchild.current += 1;
        sibling.current -= 1;
        
        return null;
    }
    
    public BTNode merge(int idx)
    {
        BTNode nchild = child[idx];
        BTNode sibling = child[idx+1];
        
        nchild.keys[t-1] = keys[idx];
        
        for(int i=0; i<sibling.current; ++i)
            nchild.keys[i+t] = sibling.keys[i];
        
        if(!nchild.leaf)
        {
            for(int i=0; i<=sibling.current; ++i)
                nchild.child[i+t] = sibling.child[i];
        }
        
        for(int i=idx+1; i<current; ++i)
            keys[i-1] = keys[i];
        
        for(int i=idx+2; i<current; ++i)
            child[i-1] = child[i];
        
        nchild.current += sibling.current+1;
        current--;
        
        sibling = null;
        return null;
    }
   
}
    
            
            
    
    
    
    
    