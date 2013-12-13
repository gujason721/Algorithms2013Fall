package china.zhouyiren.skip_list;

import java.util.*;





public class skip_list {
	
	
	
	//test code here
	public static void main(String[] args) {
		skip_list list=new skip_list();
		list.createskip_list(head,max_level);
		
		//data list
		int[] array = {39,58,82,26,47,63,27,67,51,94};
		
		//input list
		for (int i = 0; i < array.length; i++){
			list.insert_node(array[i]);
			
		}
		
		//show structure
        int[] data=list.traverse(head,array.length);
        int[] level=new int[data.length];
        for (int i = 0; i < data.length; i++){
        	level[i]=list.find_level(head,data[i]);
        	System.out.println("data:"+data[i]+"	level:"+level[i]);
        }
        
        //search for one node
        skipnode x=list.search_node(head,data[6]);
        System.out.println("data:"+x.getvalue()+"	next:"+x.getnext().getvalue());
        
        //delete this node
        list.delete_node(data[6]);
        
        //show strcture again
        int[] data1=list.traverse(head,data.length-1);
        int[] level1=new int[data1.length];
        for (int i = 0; i < data1.length; i++){
        	level1[i]=list.find_level(head,data1[i]);
        	System.out.println("delete data:"+data1[i]+"	level:"+level1[i]);
        }
	}
	
	private static int max_level=8;
	private static skipnode head;
	
	private final skipnode NIL= new skipnode(-2);
	
	public skip_list(){
		head=new skipnode(-1);
	}
	
	public skip_list(skipnode head){
		skip_list.head=head;
	}
	
	public void createskip_list(skipnode head,int level){
		head.setvalue(-1);
		int k=level;
		skipnode x=new skipnode(head.getvalue());
		head.setdown(x);
		while(k>2){
			skipnode y=new skipnode(x.getvalue());
			x.setdown(y);
			x=y;
			k=k-1;
		}
		x=NIL;
	}
	
	//define skipnode
	class skipnode{
		public int value;
		public skipnode next;
		public skipnode down;
		
		public skipnode(int value){
			super();
			this.value=value;
			this.next=NIL;
			this.down=NIL;
		}
		
		public skipnode(){
			
		}
		
		public skipnode getnext(){
			return next;
		}
		
		public skipnode getdown(){
			return down;
		}
		
		public int getvalue(){
			return value;
		}
		
		public void setnext(skipnode next){
			this.next=next;
		}
		
		public void setdown(skipnode down){
			this.down=down;
		}
		
		public void setvalue(int value){
			this.value=value;
		}
		
	}
	
	//random level
	private static int random_level(int max_level){
		int k=1;
		while(k<max_level && (Math.random()>0.6?1:0)==0){
			k++;
		}
		return k;
	}
	
	//insert
	private void insert_node(int value){
		int k=random_level(max_level);
		int i=max_level;
		skipnode x=head;
		while(i>k){
			x=x.getdown();
			i--;
		}
		skipnode y=new skipnode(value);
		while(i>0){
			if(x.getnext().getvalue()==-2){
				x.setnext(y);
				y.setnext(NIL);
			}
			else{
				skipnode z=x;
				while(z.getnext().getvalue()!=-2 && z.getnext().getvalue()<value){
					z=z.getnext();
				}
				if(z.getnext().getvalue()==value){
					System.out.println("Insert: Duplicate value!"+i);
					return;
				}
				skipnode a=z.getnext();
				z.setnext(y);
				y.setnext(a);
			}
			x=x.getdown();
			skipnode b=new skipnode(value);
			y.setdown(b);
			y=b;
			i--;
			
		}
		y.setvalue(-2);
	}
	
	//traverse
	private int[] traverse(skipnode x,int size){
		int i=max_level;
		while(i>1){
			x=x.getdown();
			i--;
		}
		int[] a=new int[size];
		int j=0;
		while(x.getnext().getvalue()!=-2){
			a[j]=x.getnext().getvalue();
			x=x.getnext();
			j++;
		}
		return a;
	}
	
	//find level
	private int find_level(skipnode x,int value){
		int i=max_level;
		while(i>1){
			if(!find_thislevel(x,value)){
				x=x.getdown();
				i--;
			}
			else{
				return i;
			}
		}
		if(find_thislevel(x,value)){
			return i;
		}
		else{
			return -1;
		}
	}
	
	//find this level
	private boolean find_thislevel(skipnode x,int value){
		while(x.getnext().getvalue()<value){
			if(x.getnext().getvalue()==-2){
				return false;
			}
			else{
				x=x.getnext();
			}
		}
		if(x.getnext().getvalue()==value){
			return true;
		}
		else{
			return false;
		}
	}
	
	//search prenode
	private skipnode search_prenode(skipnode x,int value){
		while(x.getnext().getvalue()==-2){
			x=x.getdown();
		}
		while(x.getdown().getvalue()!=-2){
			while(x.getnext().getvalue()!=-2 && x.getnext().getvalue()<value){
				x=x.getnext();
			}
			if(x.getnext().getvalue()==value){
				return x;
			}
			else{
				x=x.getdown();
			}
		}
		while(x.getnext().getvalue()!=-2 && x.getnext().getvalue()<value){
			x=x.getnext();
		}
		if(x.getnext().getvalue()==value){
			return x;
		}
		else{
			return NIL;
		}
	}
	
	//search node
	private skipnode search_node(skipnode x,int value){
		skipnode y=search_prenode(x,value);
		if(y.getvalue()!=-2 && y.getnext().getvalue()!=-2){
			return y.getnext();
		}
		return NIL;
	}
	
	//search next prenode
	private skipnode search_nextprenode(skipnode x,int value){
		if(x.getdown().getvalue()==-2){
			return NIL;
		}
		x=x.getdown();
		while(x.getnext().getvalue()<value){
			x=x.getnext();
		}
		return x;
	}
	
	//delete
	private void delete_node(int value){
		skipnode x=search_prenode(head,value);
		x.setnext(x.getnext().getnext());
		while(x.getdown().getvalue()!=-2){
			x=search_nextprenode(x,value);
			x.setnext(x.getnext().getnext());
		}
	}

}
