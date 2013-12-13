package ast.handlers;

import java.util.ArrayList;
import java.util.List;

import org.eclipse.jdt.core.dom.ASTVisitor;
import org.eclipse.jdt.core.dom.MethodDeclaration;
import org.eclipse.jdt.core.dom.Statement;

public class StatementVisitor extends ASTVisitor {
	List<Statement> statements = new ArrayList<Statement>();
	 @Override
	  public boolean visit(MethodDeclaration node) {
		// statements.add(node);
	    return super.visit(node);
	  }
	 public List<Statement> getStatements() {
		    return statements;
		  }

}
