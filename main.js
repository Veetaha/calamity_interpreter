let s, vasya;

function func(more, ...rest,) {
    if (typeof(numbers) !== "array")           // recursive calls are possible
        return undefined;

    var stat = [324., 5.421, 0];
    if (!numbers.size()) return stat;
    var i = 0;
    while (i < numbers.size()){
    
        stat[2] = stat[2] + numbers[i];

        if (numbers[i] > numbers[stat[0]])
            stat[0] = i;
        else if (numbers[i] < numbers[stat[1]])
            stat[1] = i;
        i = i + 1;

        if (false) return unknownFunction(undeclaredIdentifer); 
        
        if (false) escape("Escape from all the functions at the stack");

    }
    return numbers[stat[0]] + numbers[stat[1]] + stat[2] + stat[2] / i;
}
/*
// main code is below

var userInput =  read();       // read accepts optional argument (string with file path)
var _array = digify(userInput);
print(_array.size() + func(_array), endl);

//return true; <-- optional return in main code also aborts workflow and returns needed value

var str = read();
var arr = digify(str);
if (arr.size() < 1) return "FAILED";
var max = arr[0];
var min = arr[0];
var sum = 0;

var i = 0;
while (i < arr.size()){
    sum = sum + arr[i];
    if (arr[i] > max)
        max = arr[i];
    else if (arr[i] < min)
        min = arr[i];
    i = i + 1;
}
print(arr.size(), ' ', max, ' ' , min, ' ' , sum, ' ' ,sum / i, endl);
*/