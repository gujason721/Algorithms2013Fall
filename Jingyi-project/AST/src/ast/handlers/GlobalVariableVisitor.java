package ast.handlers;

import java.util.ArrayList;
import java.util.List;

import org.eclipse.jdt.core.dom.ASTVisitor;
import org.eclipse.jdt.core.dom.FieldDeclaration;
import org.eclipse.jdt.core.dom.TypeDeclaration;
import org.eclipse.jdt.core.dom.VariableDeclarationFragment;

public class GlobalVariableVisitor  extends ASTVisitor {
	List<String> variables= new ArrayList<String>();
	
//	 @Override
//	  public boolean visit(VariableDeclarationFragment node) {
//		 variables.add(node.getName().toString());
//	    return super.visit(node);
//	  }
	 @Override
	  public boolean visit(TypeDeclaration node) {
		FieldDeclaration[] fds=node.getFields();
		for(FieldDeclaration fd:fds){
			for(Object obj:fd.fragments()){
				variables.add(((VariableDeclarationFragment)obj).getName().toString());
				
			}
		}
	    return super.visit(node);
	  }
	 public List<String> getGlobalVariables() {
		    return variables;
		  }
}
