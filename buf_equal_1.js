const http = require('http');
const addon = require('./build/Release/symbolic_interface');

const hostname = '127.0.0.1';
const port = 3000;

const server = http.createServer((req, res) => {
  res.statusCode = 200;
  res.setHeader('Content-Type', 'text/plain');

  var obj = { sym_str:res.header};
  addon.setup(obj);
  
  var symheader = addon.makeConcolicVar(res.header);
  var buf1=Buffer.from('testingcontent');

  if (symheader.equals(buf1)){ 
    console.log('1')
  }else{
    console.log('2')
  }
  //res.end('Hello World');
  res.end(symheader);
});

server.listen(port, hostname, () => {
  console.log(`Server running at http://${hostname}:${port}/`);
});
