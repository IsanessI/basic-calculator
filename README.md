# basic-calculator
This class is working with math problems like 1+2+3. For using it, you just need create string object. Constructor of this class is working only for string and interpreted types(char*, etc).
For calculate just use method calculate(). Answer will return into double type.

For example,
{
  string obj = "1+2+3";
  calculator calc(obj);
  double answer = calc.calculate();
}

This version supports addition, subtraction, multiplication, division and exponentiation.
