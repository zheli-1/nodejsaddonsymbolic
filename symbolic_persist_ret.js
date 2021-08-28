const addon = require('./build/Release/symbolic_interface');

var str_var = "zheli";
var obj = { sym_str: str_var };
console.log(obj.sym_str);

// save the target JS object in the addon
addon.setup(obj);
console.log(obj);

//addon.startAnalysis();
var sym_buffer = addon.makeConcolicVar(obj.sym_str);
var buf1=Buffer.from('TestS');
var buf2=Buffer.from('ZELKd');

console.log("content should be changed");
console.log(sym_buffer);

if(Buffer.compare(sym_buffer, buf1)){
    console.log('First branch reached.')
  } else if (Buffer.compare(sym_buffer, buf2)) {
    console.log('Second branch reached.')
  }else {
    console.log('Third branch reached')
  }

//addon.makeConcolicVar(obj.sym_str);
addon.endAnalysis();


  
