# C-With-Classes
A Transpiler that can turn a vaguely C++ class structure into pure C code for Object Oriented Programming.

# Background
## History
C with Classes was a language that was created by Bjarne Stroustrup at Bell Labs in 1979 to be an extension to the C programming language. 
It was intended to introduce higher level language features to C that were still fast due to the compiled nature of C.
C with Classes was renamed to C++ in 1983 and was standardized in 1998. Since then, C++ has gained many features and has a very large Standard Template Library.


This is a recreation of C with Classes that seeks to be an experiment as well as a teaching tool in how to write object oriented code in C. All code created by
this transpiler should be valid C code that can be compiled by a compiler of your chosing.

## How it works
The way object oriented programming works in C is somewhat of a hack. The closest the C has to objects is structs which are a way to store groups of data together.
In addition to structs, C also allows us to set pointers to functions and call them like functions in code. With these two features we can create something
close to an object and have encapsulation. 

# How to Declare a Class with C with Classes
You can declare a class by putting it in any file type you like. You can declare a class by leading with the `class` keyword, followed by the name of the class
you wish to declare. Then lead with an opening curly brace; You can declare your class member data and methods here. Just know that there are no public or 
private members or functions. Private is just checked at compile time and since this transpiler only makes C code, there is no way to check for public or 
private access. For methods it is important to know that to access class data and to call methods within a method you must use `this` and the accessor 
operator `->` to be able to ensure that your code works. To close the class definition, you end with a closing curly brace and a semicolon.

## Example Class
```
class Test {
  int data;

  void setData(int data) {
    this->data = data;
  }
  int getData() {
    return this->data;
  }
};
```
# How to Use it
Run the command and give it your files to be converted and it will output the files in a folder called output and will create a header and source file for
each file you feed it.
