package examples;

import java.io.*;

public class Customer {

	public double myMoney;
	public double transOutMoney;
	public double transInMoney;
	
	public void setTransOutMoney(double transMoney){
		this.transOutMoney = transMoney;
	}
	
	public void transferOut() {
		myMoney = myMoney - transOutMoney; 
	}
	
	public void cancelTransfer(){
		System.out.println("This customer wants to concel this money transfer and retrieve his/her money... ");
	}
	
	public boolean isConceled() throws IOException {
		System.out.println("Do you want to confirm this transfer? Press Y to continue and N to cancel...");
		BufferedReader cancelOrNot = new BufferedReader(new InputStreamReader(System.in));
		String isCancel = cancelOrNot.readLine();
		while(isCancel !="Y" && isCancel != "N"){
			System.out.println("Please enter the right choice...");
			isCancel = cancelOrNot.readLine();
		}
		if(isCancel == "Y"){
			retrieveMoney();
			return true;
		}
		return false;
	}
	
	public void transferIn() {
		myMoney = myMoney + transInMoney;
	}

	public void retrieveMoney() {
		myMoney = myMoney + transOutMoney;
	}

}
