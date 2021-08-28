"use strict";

var dirname = '/home/zheli/crete-run-scripts/dispatch/2021-Mar-10_00.53.37.xml/pure-testcase-data/';
//zl3 read test case from directory


const fs = require('fs');

fs.readdir(dirname, (err, files) => {
	if (err)
		console.log(err);
	else {
		console.log("\nCurrent directory filenames:");
		var readFiles = function (index) {
            console.log(index);
			if (index == files.length) {
				console.log("Done reading all the files.");
			}
			else {

				// For each file we use readFile() to get 
				// the contents of the file 
                var full_path_file = dirname + files[index];
                console.log(full_path_file);
				console.log("path_file:" + files[index] + "index is " + index);
				fs.readFile(full_path_file, function (err, data) {
					if (err) {
						console.log("Error reading the file", err);
					} else {
						console.log("content is " + data.toString('utf-8', 0, data.length));
                        readFiles(index + 1);
                    }
                });
            }
        }
        readFiles(0);
    }
});
