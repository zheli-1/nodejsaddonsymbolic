const addon = require('./build/Release/symbolic_interface');

var str_var = "zheli";
var obj = { sym_str:str_var };
console.log(obj.sym_str);

// save the target JS object in the addon
addon.setup(obj);
console.log(obj);

//addon.startAnalysis();
var sym_buffer = addon.makeConcolicVar(obj.sym_str);
var buf1=Buffer.from('TestS');

console.log("content should be changed");
console.log(sym_buffer);

if (sym_buffer.equals(buf1)){ 
  console.log('1')
}else{
  console.log('2')
}

//addon.makeConcolicVar(obj.sym_str);
addon.endAnalysis();


  
