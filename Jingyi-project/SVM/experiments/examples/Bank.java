package examples;

import java.io.IOException;
import java.util.ArrayList;

public class Bank {
	
	private ArrayList<Customer> customers = new ArrayList<Customer>();
	private double totalMoney = 0;
	private double originalMoney;
	private double afterTransMoney;
	
	public void addCustomer(Customer newCustomer){
		customers.add(newCustomer);
	}
	
	public double getTotalMoney(){
		for(Customer customer : customers)
		{
			totalMoney = totalMoney + customer.myMoney;
		}
		return totalMoney;
	}
	
	public void transfer(Customer sender, Customer receiver) throws IOException, InterruptedException{
		sender.transferOut();
		wait(5000);
		if(sender.isConceled()==true){
			sender.retrieveMoney();
		}
		receiver.transferIn();
	}
	
}
