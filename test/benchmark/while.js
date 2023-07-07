var t1 = Date.now();
var i = 0;
while(i < 100000000) {
    i = i + 1;
}
var t2 = Date.now();
console.log(t2 - t1);