In general, follow the Ogre coding style. If some code requires formatting not covered by this page, please add the appropriate guidelines for such code to this list.

    * Indent with four spaces: no tabs. (Note that this is different from Ogre.)
    * Use Unix linebreaks (LF).
    * End each file with a newline, and remove all trailing whitespace from individual lines.
    * Endeavor to be const correct everywhere: modern compilers can use this to a huge advantage.
    * Opening braces are always on the same line as the keyword, except for class and function definitions. 

namespace MyNamespace {
//m is for 'member' 
class MyClass {
  int mVar1;
  int mVar2;
  MyClass(int val1, int val2);
};
 
MyClass::MyClass(int val1, int val2)
: mVar1(val1), mVar2(val2)
{
    if(a) {
        myFunctionOne();
        myFunctionTwo();
    }else {
        myFunctionThree();        
    }
}
 
}  // namespace MyNamespace

    * Functions definitions that are sufficiently short to inline should be defined in the with the class declaration (usually in the .hpp) and live entirely on one line, as in myFunctionOne() below.
    * Function definitions that are longer than a few lines should not be implemented within the class definition, as in myErf() below. There are several reasons for this: (1) the class definition is used by programmers as an API reference, and implementation details make it difficult to find the information; (2) functions defined within the class declaration are considered to be inline functions, and if longer than a few lines will lead to code bloat and be difficult to trace in a debugger. 

class MyClass
{
    int myFunctionOne() const { return 1; }
    double myErf(double x) const;
};
 
double MyClass::myErf(double x) const
{
    if (x < 0)
        return -myErf(-x);
    const double delta = 0.01;
    double acc = 0;
    for (double y = delta; y < x; y += delta)
        acc += delta * exp(-y * y);
    y -= delta;
    acc += (x - y) * exp(-x * x);
    acc *= 2. / M_PI;
    return acc;
}

    * All names are in camel notation, e.g. MyClass, MyNamespace, myFunction, mMemberVariable and should be maximally descriptive. That is, a function that computes the length of something should be called myLength() instead of Len(), and a loop that iterates over a list of Spimes should look like for (int spime = 0 ; spime < spimeLength; spime++) instead of for (int i = 0; i < spimeLength; i++).
    * All function names start with a lower-case letter.
    * Member variables start with the lowercase letter 'm'.
    * Stick to 155-character screen widths as far as possible.
    * Write the minimum amount of code that does what you need to do. This means that 

if (condition) value = myFunction() + 1. / 3 * 5;

is much preferable to

if (condition == true) {
    value = (this->myFunction() + (1.0 / 3.0) * 5.0));
}

In the vast majority of cases, if the compiler doesn't need parentheses or brackets, you don't need them either.

    * Use whitespace where appropriate. In particular, clearly separate successive blocks, function declarations etc (a good model is the MyClass code to illustrate indentation below). Also, put spaces in mathematical expressions, e.g. x = a + b + c;, not x=a+b+c;, and after delimiters in general, especially commas (e.g. int a, b;, not int a,b;).
    * In general, you should put comments wherever you feel that they enhance the readability of the code. However, if you're writing good 

code, there should be very few instances where this is the case. Redundant comments (like the one below) are pernicious and should be avoided:

if (programOptions[OPTION_MODE] == "server") { // server

    * Doxygen comments should be written like this: 

/** Short description of function foo.
 *  Longer description of function foo that may span many lines
 *  like this blah blah blah...  Return type should be on same line
 *  as start of function name to support eventDispatch lookups
 */
void foo()
{
    ...
}

    * Non-Doxygen comments should be written like this: 

// Let x be the sum of a and b.
x = a + b;
 
// Now loop over values of i from 0 to 9, adding
// 1 to x at each stage.
for (int i = 0; i < 10; ++i)
    ++x;  // Faster than x = x + 1?

    * Function definitions should always be preceded by a comment of the following form: 

//#####################################################################
// myFunction
//#####################################################################
void myFunction()
{
    ...
}

    * A namespace declaration does not induce a level of indentation.
    * Indent within a class as follows: 

class MyClass
{
 
  public:
 
    /** Short description of foo.
     *  Long description of foo.
     */
    void foo();
 
    /** smallFoo has only a short description. */
    void smallFoo();
 
 
  private:
 
    /** Short description of bar.
     *  Long description of bar.
     */
    void bar();
 
};  // class MyClass

    * When defining a function (except when it is inlined within a class declaration), put its name and return type on a separate line, indented as little as possible. That is: 

[template <class T>] [inline]
void MyClass::foo(int n)
{
    ...
}

    * Initialization lists are written like this: 

MyClass::MyClass(std::string const & name, int age, int uuid)
: ParentClass(uuid), mName(name), mAge(age)
{
    ...
}

    * Write typenames so they can be read right-to-left, e.g. std::string const & name can be read as: "name is a reference (&) to an immutable (const) string", and int * const p can be read as: "p is an immutable (const) pointer (*) to an integer". Note that in the latter case, the integer can be hanged through p, but p cannot be reseated to point to something else, which is made clear by the right-to-left reading.
    * When a function has so many arguments that it cannot fit in 155 columns, write its declaration like this: 

void foo(int a, int b, int c, int d, int e,
         int f, int g, int h, int i, int j);

... and its definition like this:

void foo(int a, int b, int c, int d, int e,
         int f, int g, int h, int i, int j)
{
    ...
}

    * Use "get" and "set" as prefixes to accessor and modifier functions. i.e. use int getAge() const and void setAge(int age), not int age() const and void age(int a).
    * Prefer clear, well written code that reads naturally to documentation. Document all functions for Doxygen and reference purposes, but within functions or for private functions, document only 

as necessary. However, don't under-document code. Prefer over-documentation if you are unsure if documentation is necessary.

    * Prefer references to smart pointers, and smart pointers to raw pointers.
    * All text (comments, identifiers etc) should be in English except for technical terms for which it's conventional to use another language. For consistency, prefer the American spelling for identifiers.
    * Don't write if (a) delete a;. delete may safely be called on NULL. 

Logging

    * All debugging printouts should use our new threadsafe logging mechanism which is customizable on a per-user basis through config files and/or command line options.
    * The Logging.hpp contains macros that should be used to construct the output stream to write logs to.
    * Prefer your subsystem macro:
    * Let's say you are working on a plugin called "mymodule". If you wish to write a log that will be seen by default, write: 

     SILOG(mymodule,info,"Everyone will see this")

    * If you are working on graphics and wish to write a log that will assist you in debugging and should only be seen by others working on 

the graphics subsystem do:

     SILOG(mymodule,debug,"Only Ogre graphics developers will see this in the meru system")

    * If you are working on graphics and need to spam the console with information to get at a bug use: 

     SILOG(mymodule,insane,"This is only for the folks willing to sacrifice serious performance to view logs")

    * In general, there will be about one subsystem per directory.
    * Level can be one of [insane,debug,info,warning,error,fatal]. 

Here's an example:

SILOG(mymodule, debug, "Class Hierarchy Ready, go to loading phase");
SILOG(mymodule, info, "Server ready to accept clients");
SILOG(mymodule, warning, "Value of ProxyObject should not be null");
SILOG(mymodule, error, "Empty config file. Wrong path? "<< mypath);
SILOG(mymodule, fatal, "Cannot recover from "<<err<<": aborting");

    * There are macros for each of these debug levels
    * FIXME: How do do this in sirikata? Turn on logging with --loglevel debug or turn it down to serious only errors with --loglevel error
    * Turning on subsystem logs is easy: just run with the command line argument --moduleloglevel module1=debug,module2=warning for example.
    * For example, if you only want to display networking prints, you can add a command line option: "--moduleloglevel transfer=fatal,ogre=fatal,task=fatal,resource=fatal" 

/*

    * You can tune what and where prints out in logs by selecting various options from
          o --logfile=log.txt Choose which file to write to
          o --log-write-stdout=0 Choose not to write to stdout terminal
          o --log-write-stderr=1 Choose to write to stderr terminal
          o --log-time=0 Choose not to write out timestamps
          o --log-thread=0 Choose not to write threadid out
          o --log-index=0 Choose not to write index # to log
          o --log-function=1 Choose to log which function printed the message
          o --log-fileline=0 Choose not to log which file and line number the message was printed on
          o --log-leveltype=0 Choose not to log the level of the log (debug,warning,info,etc) 
    * / 