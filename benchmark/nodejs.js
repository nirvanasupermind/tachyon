var start = Date.now();
for(var i = 0; i < 10000000; i++) {
    var x = 2.0 + 2.0;
}
var end = Date.now();
console.log(end - start);