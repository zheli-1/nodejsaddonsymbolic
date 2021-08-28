const addon = require('./build/Release/symbolic_interface');

var str_var = "zheli";
var obj = { sym_str:str_var};
console.log(obj.sym_str);

// save the target JS object in the addon
addon.setup(obj);
console.log(obj);

//addon.startAnalysis();
var sym_str1 = addon.makeConcolicVar(obj.sym_str);
console.log("content should be changed\n");
console.log(obj.sym_str);

if(sym_str1 == "symbolicTest"){
    console.log('First branch reached.')
  } else if (sym_str1 == "zheli1") {
    console.log('Second branch reached.')
  }else {
    console.log('Third branch reached')
  }

addon.makeConcolicVar(obj.sym_str);
addon.endAnalysis();


  
