package findMin;

public class FindMin {
	
	private static int little;
	private static int big;
	private final static int length = 100;
	private final static int max = 100;
	private static int[] newArray = new int[length];
	
	public static void main(String[] args) {
	
		int[] array = randomArray(length,max);
		sort(array);
		
		int j = 0;
		while(j==0){
			j= (int)(Math.random()*max);
		}
		for(int k=0;k<length;k++){
			if(j<length){
				newArray[j++] = array[k];
			}
			else{
				newArray[(j++)-length] = array[k];
			}
		}
		
		for(int s=0; s<newArray.length; s++){
			System.out.println(newArray[s] + "\n");
		}
		System.out.println("------------------------------------------------------------------------");
		System.out.println("------------------------------------------------------------------------");
		System.out.println("The minimum number in the circular array is " + findMin(newArray) + ".");
		
	}

	private static int findMin(int[] newList) {
		
		int start = 0;
		int second = newList.length/3;
		int third = newList.length*2/3;
		int middle;
		compare(start,second,third);
		if(little+big==newList.length*2/3 && little*big==0){
			little = newList.length-1;
		}
		
		while(little!=big){
			middle = (little+big)/2;
			if (newArray[middle]<=newArray[little]){
				little = middle;
				if(Math.abs(little-big)==1){
					return Math.min(newArray[little], newArray[big]);
				}
			}
			else{
				big = middle;
				if(Math.abs(little-big)==1){
					return Math.min(newArray[little], newArray[big]);
				}
					
			}
			}
			return newArray[little];
	}
	
	private static void sort(int[] array) {
		for(int j=1;j<array.length;j++){
			for(int i=0;i<j;i++){
				if(array[i]>array[j]){
					int key=array[j];
					array[j]=array[i];
					array[i]=key;
				}
			}
		}			
		
	}

	private static int[] randomArray(int length, int maxNum){
		
		// we can set the length and maximum number of the array
		int[] Array = new int[length];
		for(int i=0;i<length;i++){
			Array[i]=(int)(Math.random()*maxNum);
		}
		// we get a random array of integers only
		return Array;
	}
	
	
	private static void compare(int a, int b, int c) {
		
		int key1 = newArray[a];
		int key2 = newArray[b];
		int key3 = newArray[c];
		
		if(key1<key2){
			if(key2<key3){
			    little = a;
				big = c;
			}
			else{
				big = b;
				if(key1<key3){
					little = a;
				}
				else{
					little = c;
				}
			}
		}
		else{
			if(key1<key3){
				big = c;
				little = b;
			}
			else{
				big = a;
				if(key2<key3){
					little = b;
				}
				else{
					little = c;
				}
			}
		}
		
	}

}
