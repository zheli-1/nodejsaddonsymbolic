var str_var = "zheli";

str_var = %SymbolicExecution(str_var);

if( str_var === "tests")
     return "tests";

if( str_var > 'tests1'){
  return "tests1";
}else{
  return "tests2"
}
