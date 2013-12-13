package ast.handlers;

import java.util.ArrayList;
import java.util.List;

import org.eclipse.core.commands.AbstractHandler;
import org.eclipse.core.commands.ExecutionEvent;
import org.eclipse.core.commands.ExecutionException;
import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.IWorkspace;
import org.eclipse.core.resources.IWorkspaceRoot;
import org.eclipse.core.resources.ResourcesPlugin;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.jdt.core.ICompilationUnit;
import org.eclipse.jdt.core.IPackageFragment;
import org.eclipse.jdt.core.IPackageFragmentRoot;
import org.eclipse.jdt.core.JavaCore;
import org.eclipse.jdt.core.JavaModelException;
import org.eclipse.jdt.core.dom.AST;
import org.eclipse.jdt.core.dom.ASTParser;
import org.eclipse.jdt.core.dom.ArrayAccess;
import org.eclipse.jdt.core.dom.Block;
import org.eclipse.jdt.core.dom.BreakStatement;
import org.eclipse.jdt.core.dom.CompilationUnit;
import org.eclipse.jdt.core.dom.ContinueStatement;
import org.eclipse.jdt.core.dom.ExpressionStatement;
import org.eclipse.jdt.core.dom.ForStatement;
import org.eclipse.jdt.core.dom.IfStatement;
import org.eclipse.jdt.core.dom.ImportDeclaration;
import org.eclipse.jdt.core.dom.InfixExpression;
import org.eclipse.jdt.core.dom.MethodDeclaration;
import org.eclipse.jdt.core.dom.MethodInvocation;
import org.eclipse.jdt.core.dom.QualifiedName;
import org.eclipse.jdt.core.dom.ReturnStatement;
import org.eclipse.jdt.core.dom.SingleVariableDeclaration;
import org.eclipse.jdt.core.dom.Statement;
import org.eclipse.jdt.core.dom.StringLiteral;
import org.eclipse.jdt.core.dom.VariableDeclarationStatement;
import org.eclipse.jdt.core.dom.WhileStatement;
import org.eclipse.jdt.core.dom.rewrite.ASTRewrite;
import org.eclipse.jdt.core.dom.rewrite.ListRewrite;
import org.eclipse.jface.text.Document;
import org.eclipse.text.edits.TextEdit;



public class SampleHandler extends AbstractHandler {
	public static CompilationUnit parse;
	  public static List<Integer> lineNumbers=new ArrayList<Integer>();
	  public static ICompilationUnit currUnit;
	  public static ASTRewrite rewriter;
	  
	  
	  public ArrayAccess getNewArrayAccess(AST ast, String name, int i){
		  //AST ast=parse.getAST();
		  ArrayAccess aa=ast.newArrayAccess();
			aa.setArray(ast.newSimpleName(name));
			aa.setIndex(ast.newNumberLiteral(String.valueOf(i)));
			return aa;
	  }
	  
	  public StringLiteral GetSpace(AST ast){
			StringLiteral sl=ast.newStringLiteral();
			sl.setLiteralValue(" ");
			return sl;
	  }
  public Object execute(ExecutionEvent event) throws ExecutionException {
    // Get the root of the workspace
    IWorkspace workspace = ResourcesPlugin.getWorkspace();
    IWorkspaceRoot root = workspace.getRoot();
    // Get all projects in the workspace
    IProject[] projects = root.getProjects();
    // Loop over all projects
    for (IProject project : projects) {
      try {    	     	  
    	  //analyseMethods(project);
    	  IPackageFragment[] packages = JavaCore.create(project)
    		        .getPackageFragments();
    	  for (IPackageFragment mypackage : packages) {
    		  if (mypackage.getKind() == IPackageFragmentRoot.K_SOURCE) {
		    	  for (ICompilationUnit unit : mypackage.getCompilationUnits()) {
		    		  currUnit=unit;
		    		  ASTParser parser = ASTParser.newParser(AST.JLS3);
		    		    parser.setKind(ASTParser.K_COMPILATION_UNIT);
		    		    parser.setSource(unit);
		    		    parser.setResolveBindings(true);
		    		    MethodVisitor visitor = new MethodVisitor();
		    		     parse = (CompilationUnit) parser.createAST(null);
		    		    parse.accept(visitor);
		    		    //Get global variables
		    		    
		    		    GlobalVariableVisitor gv=new GlobalVariableVisitor();
		    		    parse.accept(gv);
		    		    
		    		    List<String> variables=new ArrayList<String>();
		    		    
		    		    List<String> globalVariables=gv.getGlobalVariables();
		    		    //=====In each methods====
		    		    

		    		    List<MethodDeclaration> list=visitor.getMethods();
		    		    for(MethodDeclaration md:visitor.getMethods()){
		    		    	
		    		    	try{
		    		    	List<SingleVariableDeclaration> ld = md.parameters();
		    		    	//for(int i=0;i<ld.size();i++){
		    		    		//variables.add(ld.get(i).getName().toString());
		    		    		
		    		    	//}
		    		    	//ExploreBlock(md.getBody());
		    		    	Block stmt=md.getBody();
		    		    	
		    		    	AST ast=parse.getAST();
		    				MethodInvocation newInvocation = ast.newMethodInvocation();
		    				//==only add once===
		    				ImportDeclaration importDeclaration = ast.newImportDeclaration();
		    		    	QualifiedName qname = 
		    						ast.newQualifiedName(
		    							ast.newSimpleName("System"),
		    							ast.newSimpleName("out"));
		    				newInvocation.setExpression(qname);
		    				newInvocation.setName(ast.newSimpleName("println"));
		    				
		    				InfixExpression infixExpression0 = ast.newInfixExpression();
		    				infixExpression0.setOperator(InfixExpression.Operator.PLUS);	
		    				infixExpression0.setRightOperand(ast.newSimpleName("max"));			    				
		    				infixExpression0.setLeftOperand(GetSpace(ast));
		    				
		    				
		    				 ArrayAccess aa=ast.newArrayAccess();
		    					aa.setArray(ast.newSimpleName("arr"));
		    					aa.setIndex(ast.newNumberLiteral(String.valueOf(2)));
		    					
		    					
		    				InfixExpression infixExpression1 = ast.newInfixExpression();
		    				infixExpression1.setOperator(InfixExpression.Operator.PLUS);	
		    				infixExpression1.setRightOperand(infixExpression0);			    				
		    				infixExpression1.setLeftOperand(getNewArrayAccess(ast, "arr", 2));
		    				
		    				InfixExpression infixExpression2 = ast.newInfixExpression();
		    				infixExpression2.setOperator(InfixExpression.Operator.PLUS);	
		    				infixExpression2.setRightOperand(infixExpression1);			    				
		    				infixExpression2.setLeftOperand(GetSpace(ast));
		    				
		    				InfixExpression infixExpression3 = ast.newInfixExpression();
		    				infixExpression3.setOperator(InfixExpression.Operator.PLUS);	
		    				infixExpression3.setRightOperand(infixExpression2);			    				
		    				infixExpression3.setLeftOperand(getNewArrayAccess(ast, "arr", 1));
		    				
		    				
		    				InfixExpression infixExpression4 = ast.newInfixExpression();
		    				infixExpression4.setOperator(InfixExpression.Operator.PLUS);	
		    				infixExpression4.setRightOperand(infixExpression3);			    				
		    				infixExpression4.setLeftOperand(GetSpace(ast));
		    				
		    				InfixExpression infixExpression5 = ast.newInfixExpression();
		    				infixExpression5.setOperator(InfixExpression.Operator.PLUS);	
		    				infixExpression5.setRightOperand(infixExpression4);			    				
		    				infixExpression5.setLeftOperand(getNewArrayAccess(ast, "arr", 0));
//		    				
//		    				InfixExpression infixExpression2 = ast.newInfixExpression();
//		    				infixExpression2.setOperator(InfixExpression.Operator.PLUS);	
//		    				infixExpression2.setRightOperand(infixExpression1);			    				
//		    				infixExpression2.setLeftOperand(sl);
		    				
		    				
		    				
		    				
		    				
//		    				getNewArrayAccess("arr",2)
//		    				InfixExpression infixExpression2 = ast.newInfixExpression();
//		    				infixExpression2.setOperator(InfixExpression.Operator.PLUS);
//		    				infixExpression2.setRightOperand(infixExpression3);
//		    				infixExpression2.setLeftOperand(ast.newSimpleName("arr[2]"));
//		    				
//		    				InfixExpression infixExpression1 = ast.newInfixExpression();
//		    				infixExpression1.setOperator(InfixExpression.Operator.PLUS);
//		    				infixExpression1.setRightOperand(infixExpression2);
//		    				infixExpression1.setLeftOperand(ast.newSimpleName("arr[1]"));
//		    				
//		    				InfixExpression infixExpression0 = ast.newInfixExpression();
//		    				infixExpression0.setOperator(InfixExpression.Operator.PLUS);
//		    				infixExpression0.setRightOperand(infixExpression1);
//		    				infixExpression0.setLeftOperand(ast.newSimpleName("arr[0]"));
		    				
//		    				InfixExpression infixExpression = ast.newInfixExpression();
//		    				infixExpression.setOperator(InfixExpression.Operator.PLUS);
//		    				StringLiteral literal = ast.newStringLiteral();
//		    				literal.setLiteralValue("Hello");
//		    				infixExpression.setLeftOperand(literal);
//		    				literal = ast.newStringLiteral();
//		    				literal.setLiteralValue(" world");
//		    				infixExpression.setRightOperand(literal);
		    				
		    				
		    				newInvocation.arguments().add(infixExpression5);
		    				
		    				Statement newStatement = ast.newExpressionStatement(newInvocation);
		    		 
		    				//create ListRewrite
		    				if(rewriter==null){
		    					rewriter = ASTRewrite.create(ast);
		    				}
		    				//ASTRewrite rewriter = ASTRewrite.create(ast);
		    				ListRewrite listRewrite = rewriter.getListRewrite(stmt, Block.STATEMENTS_PROPERTY);
		    		    	
		    		    	//for(int i=stmt.statements().size();i>=0;i--){
		    		    		
		    		    	//}
		    		    	
	    		    		if(stmt.statements().get(stmt.statements().size()-1) instanceof ReturnStatement){
	    						listRewrite.insertAt(newStatement,stmt.statements().size()-1, null);
	    						//break;
	    					}
		    		    	
//		    		    	AddStatements(md.getBody());
		    		    	}catch(Exception e){
		    		    		 e.printStackTrace();
		    		    	}

		    		    	
		    		    }
		    		     
		    		    //=====In each methods====
		    			
		    	  }
    		  }
    	  }
    		try{
    	  CommitChange();
    		}catch(Exception e){
    			 e.printStackTrace();
	    	}
    	  System.out.println("===print numbers===");
    	 for(Integer i: lineNumbers){
    		 System.out.println(i);
    	 }
    	 System.out.println("===print numbers end===");
      } catch (CoreException e) {
        e.printStackTrace();
      }
    }
    return null;
  }
  
  public void AddStatements(Statement blk) throws Exception{
	// create new statements for insertion
	  if(!(blk instanceof Block)) return;
	  Block stmt=(Block)blk;
		AST ast=parse.getAST();
			MethodInvocation newInvocation = ast.newMethodInvocation();
			//==only add once===
			ImportDeclaration importDeclaration = ast.newImportDeclaration();
	
//			QualifiedName name = 
//				ast.newQualifiedName(
//					ast.newSimpleName("java"),
//					ast.newSimpleName("util"));
//			importDeclaration.setName(name);
//			importDeclaration.setOnDemand(true);
//			parse.imports().add(importDeclaration);
			
			//==only add once===
			
			QualifiedName qname = 
					ast.newQualifiedName(
						ast.newSimpleName("System"),
						ast.newSimpleName("out"));
			newInvocation.setExpression(qname);
			newInvocation.setName(ast.newSimpleName("println"));
			
			InfixExpression infixExpression = ast.newInfixExpression();
			infixExpression.setOperator(InfixExpression.Operator.PLUS);
			StringLiteral literal = ast.newStringLiteral();
			literal.setLiteralValue("Hello");
			infixExpression.setLeftOperand(literal);
			literal = ast.newStringLiteral();
			literal.setLiteralValue(" world");
			infixExpression.setRightOperand(literal);
			
			newInvocation.arguments().add(infixExpression);
			
			Statement newStatement = ast.newExpressionStatement(newInvocation);
	 
			//create ListRewrite
			if(rewriter==null){
				rewriter = ASTRewrite.create(ast);
			}
			//ASTRewrite rewriter = ASTRewrite.create(ast);
			ListRewrite listRewrite = rewriter.getListRewrite(stmt, Block.STATEMENTS_PROPERTY);
			//for(int i=stmt.statements().size();i>=0;i--){
//				if(i==0 || (i>=1 && !(stmt.statements().get(i-1) instanceof ReturnStatement))){
//				listRewrite.insertAt(newStatement, i, null);
//				}				
				if(stmt.statements().get(stmt.statements().size()-1) instanceof ReturnStatement){
					listRewrite.insertAt(newStatement, stmt.statements().size()-1, null);
					//break;
				}
			//}
			
			//listRewrite.insertFirst(newStatement, null);
	 
			
  }
  
 private static void CommitChange() throws Exception{
	 TextEdit edits = rewriter.rewriteAST();
	 
		// apply the text edits to the compilation unit
		Document document = new Document(currUnit.getSource());

		edits.apply(document);

		// this is the code for adding statements
		currUnit.getBuffer().setContents(document.get());
 }
  
  private static int GetLineNumber(Statement s){
	  return parse.getLineNumber(s.getStartPosition());
  }
  private void ExploreBlock(Statement s) throws Exception{
	  lineNumbers.add(GetLineNumber(s));
	  //AddStatements(s);
	  if(s instanceof Block){
		  
		  Block myType=(Block)s;
		  for(Object blockStmt:myType.statements()){
			  //stack.push
			  ExploreBlock((Statement)blockStmt);
			  //stack.pop
		  }
		  
	  }else if (s instanceof IfStatement){
		  IfStatement myType=(IfStatement)s;
		  ExploreBlock(myType.getThenStatement());
		 if(myType.getElseStatement()!=null){
			 ExploreBlock(myType.getElseStatement());
		 }
		  
	  }else if (s instanceof ForStatement){
		  ForStatement myType=(ForStatement)s;
		  ExploreBlock(myType.getBody());
		  
	  }else if (s instanceof WhileStatement){
		  WhileStatement myType=(WhileStatement)s;
		  ExploreBlock(myType.getBody());
		  
	  }else if (s instanceof VariableDeclarationStatement){
		  VariableDeclarationStatement myType=(VariableDeclarationStatement)s;
		  
	  }else if (s instanceof ExpressionStatement){
		  ExpressionStatement myType=(ExpressionStatement)s;
		
		  
	  }else if (s instanceof BreakStatement){
		  BreakStatement myType=(BreakStatement)s;
		
		  
	  }else if (s instanceof ContinueStatement){
		  ContinueStatement myType=(ContinueStatement)s;
		  
		  
	  }
	  
  }
  
  
  
  private void analyseMethods(IProject project) throws JavaModelException {
	    IPackageFragment[] packages = JavaCore.create(project)
	        .getPackageFragments();
	    // parse(JavaCore.create(project));
	    for (IPackageFragment mypackage : packages) {
	      if (mypackage.getKind() == IPackageFragmentRoot.K_SOURCE) {
	        createAST(mypackage);
	      }

	    }
	  }

	  private void createAST(IPackageFragment mypackage)
	      throws JavaModelException {
	    for (ICompilationUnit unit : mypackage.getCompilationUnits()) {
	      // now create the AST for the ICompilationUnits
	      CompilationUnit parse = parse(unit);
	      MethodVisitor visitor = new MethodVisitor();
	      parse.accept(visitor);

	      for (MethodDeclaration method : visitor.getMethods()) {
	        System.out.print("Method name: " + method.getName()
	            + " Return type: " + method.getReturnType2());
	      }

	    }
	  }

	  
	/**
	   * Reads a ICompilationUnit and creates the AST DOM for manipulating the
	   * Java source file
	   * 
	   * @param unit
	   * @return
	   */


	  private static CompilationUnit parse(ICompilationUnit unit) {
	    ASTParser parser = ASTParser.newParser(AST.JLS3);
	    parser.setKind(ASTParser.K_COMPILATION_UNIT);
	    parser.setSource(unit);
	    parser.setResolveBindings(true);
	    return (CompilationUnit) parser.createAST(null); // parse
	  }

} 
