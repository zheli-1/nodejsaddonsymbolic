const addon = require('./build/Release/symbolic_interface');

var e=[];
e.push("foo");
e.push("bar");

addon.setup(e);
var symbuf  = addon.makeConcolicVarArray(e);

symbuf.forEach(element => {
    if(element == "bar"){
        console.log(element+" it is not foo");
    }

    if(element == " foo"){
        console.log(element+"it is foo");
    }
});
