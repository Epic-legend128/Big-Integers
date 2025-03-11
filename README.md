# Big-Integers
Handling Big Integers in C++.
Only supports addition and subtraction from the 4 basic operations. Support for multiplication and division may be added in the future.
<br>
Of course you can make a very naive multiplication and division using the addition and subtraction but they won't be very efficient.


## Supported Operations
### Value Assignment
Assignment can be made using strings which are already saved in a variable or using other big integers either on instatiation or using the `=` operator. However, you can also use the `<<` operator to directly set the value using a string which is not stored in a variable.
```c++
std::string value = "100";
BigInteger num(value);
value = "101";
num = value;
num << "200";
```

### Addition and Subtraction
Addition and subtraction can be used the same way as used for a normal int type by using the `+` and `-` operators like so:
```c++
BigInteger d = a + b - c;
```
You can also use the `+=`, `++`, `--` and `-=` operators if needed.

### Comparison
The following comparison operators are supported: `<`, `>`, `==`, `<=`, `>=`, `!=`.

### Accessing the value
To access the value of the BigInteger for displaying or other purposes you can use the `.value()` method which returns a string representation of the number. Or you can use the `.getArray()` which returns a vector representation where each cell holds 18 digits. You can also access the individual cells of this vector by using the `[]` notation. 
<br>
Also the `<<` operator for the `std::ostream` has been overloaded so that it can output the string representation of BigInteger automatically.

### Size
To view how many digits the number is just use the `.len()` method and you can also use the `.size()` to view how many cells the vector representation has.



