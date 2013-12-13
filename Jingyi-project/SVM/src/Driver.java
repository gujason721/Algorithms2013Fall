import java.io.IOException;

import libsvm.*;

public class Driver {
	
	public static void main(String[] args) throws IOException {

		TestFindMax testMax = new TestFindMax();
		testMax.testIt();
		String[] arg ={"trainfile\\demo.txt", "trainfile\\model_r.txt"};
		svm_train t = new svm_train();    
		svm_model model=svm_train.main(arg); 
		svm_wapper result = new svm_wapper(model);
		result.generateDivider(model);
		result.printPredicate();
		System.out.println("\n");
		while(!svm_wapper.isFinal(svm_wapper.wVec)){
			System.out.println("**************This is not the final divider...try again**************");
			if(svm_wapper.loopNum>=2){
				System.out.println("**************This is the end, please check all the parameters involved...**************");
				break;
			}
			svm_wapper.selNewVec(svm_wapper.wVec);
			fileFactory.updateDataSb();
			fileFactory.preFile(svm_wapper.index, TestFindMax.lable, TestFindMax.varValue);
			model=svm_train.main(arg); 
			result.updateInfo(model);
			result.generateDivider(model);
			result.printPredicate();
			System.out.println("\n");
		}
		System.out.println("**************This is the end of the algorithm, Please check the generated predicate.**************");
	}
	
}
