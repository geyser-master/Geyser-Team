Welcome to the geyser team! you are helping to change the gaming tech industry with a 9 year old!

# The Geyser Programming Language (.gy)

**Chief Architect:** A 9 year old kid  
**Core Vision:** A strict, explicit, highly predictable, and blistering fast alternative to C++.

Geyser is a systems programming language engineered from the ground up for high-performance software, real-time engines, robotics, and advanced simulators (like *Fortnite*, *Cyberpunk 2077*). By dropping 40 years of legacy backward-compatibility baggage and utilizing a native, highly optimized C-powered compiler backend, Geyser achieves maximum hardware execution speed with a modern, non-boilerplate layout.

---
## 1. Terminal commands

### Downloading something
Geyser does this by fetching from the net to download the modules via:
```bash
geyser install vulkanapi
```

### Running a file
Geyser has two options:
*Path 1* — If you had a IDE, Simply click the run button
*Path 2* — By terminal, run `geyser run main.gy`

### Building a `*.exe` file
We do this by geyser's `gyexe`(need to be geyser installed via `geyser install gyexe)
And to convert it, run
```bash
gyexe --noconsole --onefile file="fileName.gy" --at=r"path" --target="literal_exact_processor_name_with_the_ghz"
```
*--noconsole/--console:* Optional
*--collect-all modules_by_geyser_install:* Mandatory(Only if your project contains modules by geyser install)
*--onefile/--onedir:* Mandatory
*file="file_name.gy":* Mandatory
*--at:* Mandatory
*--target:* Mandatory

An example command to convert to a `*.exe` file is
```bash
geyser install gyexe
gyexe --noconsole --onefile --collect-all numpy math gui file="calculator.gy" --at="C:\Users\Dell\MyApp.exe" --target="intelr_coretm_i58365U_CPU@1.60ghz"
```

## 2. Core Architectural Pillars

### Explicit over Implicit (No Guessing Games)
Geyser outlaws compiler guesswork. Every data type, scope boundary, statement terminator, and logical evaluation must be completely explicit. If the code contains ambiguity, the compiler halts instantly at build time.

### Features outside string literals to change things
* **`r`**  
  Treats the string everything as literal
* **`iq`**  
  Ignores in-between quotes inside string literals
* **`f`**  
  Formats curly brackets(planned to be removed)
  > *Note, If you want to add multiple string literal changers, you need to use comma's(Eg: (f, iq, r"Hello ""\\ \ "))

### Existing attributes currently in **`Geyser`**
* `toLowerCase()`  
  Makes a string lowercase
* `toUpperCase()`  
  Makes a string uppercase
* `replaceAll(old, new)`(RegEx also allowed)  
  Replaces the old string with the new string
* `capitalizeFirstLetter()`  
  Makes a certain string first letter capitalized
* `lengthOf()/byteLengthOf()/bitLengthOf()`  
  Checks a length of a given value. if string, checks characters; if int, error; if array, checks the total element count. for byteLengthOf, checks how much bytes occupies so int is valid. for bitLengthOf, same logic, but it checks the bits

### The "Yeet the GC" Memory Strategy
The Garbage Collector is permanently banished to eliminate background lag spikes and unpredictable stuttering frames. Geyser utilizes **Automatic Scope-Based Cleanup**:
* Local data sits flat inside memory stack slots.
* The exact millisecond code execution exits a block and encounters a closing curly brace `}`, the compiler automatically inserts an instruction to destroy those memory slots.
* Constant top-level objects stay active in the root file scope until the execution terminates, at which point the Operating System reclaims the entire layout at EOF (End of File).

### Hardware Cache Separation
To protect tight-loop latency, Geyser enforces a strict division at the hardware processor layer(And by cache, I mean that for variables if it was set to `cached` it will be in the D-Cache and whenever the program is accessing the variable but it was'nt changed, Geyser uses the cache unless the variable has changed, if yes, it will clear the cache and make a new one unless the user ran `uncache variable;`):
* **Loops are Cached:** Loop execution blocks and branching mechanisms are held natively in the CPU Instruction Cache (I-Cache) to repeat at maximum processor clock speeds.
* **Variables are Uncached:** To solve multi-threaded data synchronization bugs, variable reads and writes completely bypass the local CPU Data Cache (D-Cache). Operations interact directly with physical memory slots, ensuring 100% real-time data transparency across all cores.

---

## 2. Lexical Scoping: The Two-Line Law

Variable access and lifecycle visibility are determined by a simple, non-negotiable two-line law:
1. **`global` keyword:** Any code, nested function, or separate class anywhere in the application can read or write to it.
2. **No `global` keyword:** The variable is strictly locked to its immediate outer scope block. Inner functions and nested classes are completely barred from accessing it.

---

## 3. Core Data Types

Variables are raw physical memory slots. They never compile into heavy, tracking object layers or dynamic wrappers. Once assigned, a variable remains that type permanently.

* `int` — Flat whole number hardware blocks.
* `decimal` — High-precision fractional numbers for physics and fluid simulations.
* `String` — Strict, flat text character sequences.
* `boolean` — Evaluation literals (`true` or `false`).
* `Tuple` — A static tuple, Immutable, can hold anything mixed
* `const` modifier — Makes a variable permanently immutable after initialization.
* `cache` — Allows a variable's value to be retained in the CPU data cache when possible; the cache is invalidated when the value changes.
* `uncache` — Makes a variable's cache cleared in the D-Cache

```java
cache int x = 10; // Makes int x have a cache in D-Cache
uncache x; // Uncaches x
x += 10;
cache x; // Recaches x with a fresh copy
```

---

## 4. Syntax & Grammar Guide

### The Main Scope
The root of a `.gy` file *is* the main execution area. There are no useless class wrappers or mandatory `main` methods required just to say hello. Extra functions and classes simply create explicit inner scopes when called.

### Module Import Tools & Explicit Options
Geyser provides two highly disciplined options for loading external tools into a workspace. **Wildcard imports (e.g., `import package.*;`) are strictly banned.** This prevents namespace pollution, guarantees instant compile times, and ensures no hidden names are snuck into your file scope.

* **Option 1: Direct Tool Import**
  Imports a single, specific tool from a library namespace into the local scope.
  ```java
  import geyser.lang.System;
  import geyser.lang.Prompt;
  ```
* **Option 2: Namespace Alias (`as` keyword)**
  Imports an entire tool module but forces it under an explicit, short nickname to prevent typing fatigue while remaining completely traceable.
  ```java
  import geyser.gui.GeyserUI as ui;
  ```

### Comments
```java
// This is a comment, it does nothing
// Its only for notes and helper identifier
// An example is
int i = 0; // Index
// Thats it! Comments does nothing but help the programmer
```

### Code Formatting
```java
// Semicolons at the end of a statement are strictly mandatory
import geyser.lang.System;
import geyser.lang.Prompt;
import geyser.lang.string.concatenate;

// Global variables are visible everywhere
global String gameTitle = "DocItOut";

// Indentation does not matter to the compiler; it is strictly for human beauty
String part1 = "Patient status: ";
String part2 = "Stable\n";
String status = part1.concatenate(part2); // The '+' operator is purified strictly for math

System.print(status);

// Double quotes for Strings, single quotes for clean single-byte character literals
String username = Prompt("Enter surgeon name: ").toLowerCase().replaceAll(' ', '');

// Tuples are default and allowed
String password = Prompt("Enter secure password: ", maskWith='*', ignore=(' ', '\r', '\n'))
```

### Strict Mathematical Rules
1. Additions cannot be loosely calculated without a dedicated memory container slot to receive the output.
2. An existing, declared variable name cannot be re-declared to perform math.
3. Modification of an existing slot must use explicit compound mutation operators (`+=`, `-=`, `*=`, `/=`) or reassignment without declaring type again.
4. Slot type must match the value else if was assigning(AssignmentError: mismatching type between slot type and value) else if it was already declared and it came from some sort of addition or math stuff(TypeError: mismatched types between slot type and value)
5. Division divisor is strictly a int, either `int / int` or `decimal / int`, Same for multiplication multiplier
6. Truncation in values are prevented and are banned

```java
int patientPulse = 70;
patientPulse += 5; // Valid: mutates the hardware slot directly
```

### Index
1. Index cannot be over the string/list .etc range
2. Index requires a strict square brackets formatting
3. Index is just the standard `[start:stop:step]` rules(also it also has negative indexing, same rules, starts at 0 in positive, starts at -1 in negative)

```java
String text = "Motherfather";
String result = text[0:6]; // Starts: 0, Ends: before 6
```

### Ultra-Strict Control Flow & Loops
Conditions inside `if` statements require explicit true/false comparison operators. Implicit shortcut evaluations are illegal. Semicolons at the exact end of a third instruction inside `for` loop headers are redundant and forbidden.

```java
boolean patientBleeding = true;
int emergencyLevel = 5;

// Variables check presence explicitly using .exists()
if (emergencyLevel.exists() and patientBleeding == true) {
    System.print("Initiate surgery\n");
} elseif (patientBleeding == false) {
    System.print("Vitals stable\n");
} else {
    System.print("Evaluating\n");
}

// Logical text operators ('and', 'or') are used instead of confusing '&&' or '||'
for (int i = 0; i < 100; i += 1) {
    System.print(i);
}
```

### Meet randomness
```java
import geyser.lang.Random;
import geyser.lang.System;
import geyser.lang.string.concatenate;

// Testing CSPRNG
int number = Random.secureRandomInt(0, 10);

// Testing Standard PRNG
int number1 = Random.randomInt(0, 10);

// Testing true randomness from OS
int number2 = Random.trueRandomInt(0, 10);

System.print(number.concatenate(number1).concatenate(number2)); // OUTPUT: 739, why? it was concatenated without any spaces
```

### Collections, Arrays & Objects
```java
// Dynamic lists use explicit type tags to eliminate runtime alignment guessing
ArrayList<String> inventory = ["Apple", "Banana", "Grapes"];

// EXPLICIT INDEX REQUIREMENT RULE:
// If a list already contains elements, adding an item requires an index target parameter
// to explicitly state where the tail placement or offset is verified.
inventory.add("Prunes")[-1];

// Boilerplate-free classes with automatically mapped constructors
class Player {
    String name;
    int health;
    void func heal(int amount) {
        health += amount
    }
}

Player myPlayer = new Player("Surgeon", 100);
System.print(myPlayer.health);
myPlayer.health(100);
System.print(myPlayer.health); // Testing if it really increased
```

---

## 5. Currently Implemented Import Modules

These are the foundational standard library namespaces hardcoded into the initial Geyser ecosystem compiler framework. Attempting to call an module outside of this verified tool dictionary will instantly flag an installation error.

* **`geyser.lang.System`**
  Handles native command pipeline environments and console output actions via `System.print();`.
* **`geyser.lang.Random`**
  Meet the random generator, generated randomness via `Random.attribute()`
* **`geyser.lang.Prompt`**
  Grabs explicit user inputs via the terminal console layer directly into a mutating string execution block.
* **`geyser.lang.ArrayList`**
  Dynamic arrays with a strict `<type>` rule via `ArrayList<type> name = [];`
* **`geyser.lang.string.concatenate`**
  The explicit string manipulator that safely joins two flat text character sequences into a single container without dynamic memory guessing.
* **`geyser.lang.Time`**
  Handles delays in time via `Time.wait(1000);`.
* **`geyser.lang.conversion.toString/toBool/toInt/toDecimal`**
  Converts given argument to the specific argument type via `toString/toBool/toDecimal/toInt();`.
* **`geyser.time.Datetime`**
  The explicit short module to get the system time via `Datetime.getSystemTime();`, Must be saved in a variable in `DatetimeObject` variable type, datetime returns  a list, so
  * [0]: The hours
  * [1]: The minutes
  * [2]: The seconds
  `datetime.getSystemTime();` expects a argument, A example is `H:M:S`.
* **`geyser.time.MeasureTimeByTask`**
  It measures time by how long a task takes to finish, You must give a task argument without `"` around it  
  **Two specifications**
  * 1: If the program is not running any background tasks on loop, It waits for the timing to finish based on how long the task takes to finish
  * 2: In other cases, It handled the background tasks while the timer is still benchmarking, Kinda like a `async`
  The argument given does not execute and give the result, it runs silently in the background without interfering on the code execution, and it returns a `ArrayList<int>`
---

## 6. Build Integrity & The Error Engine

Geyser blocks bugs before they can ever execute on hardware by throwing descriptive compile-time errors instantly and smart explicit warnings:

### Errors
* **`SyntaxError`** — Disrespecting language grammar (e.g., leaving out a mandatory trailing semicolon `;` or attempting an unauthorized wildcard `*` import), Sometimes these are too literal.
  * *Output:* `SyntaxError: unterminated string literal but reached EOF`
* **`ConditionalError`** — Attempting to evaluate a raw condition without an explicit comparison check.
  * *Output:* `ConditionalError: cannot reference true or false to variable with no true check or false check`
* **`NameError`** — referencing an unknown identifier, using a variable before it exists, or trying to access a class layout without instantiating it.
  * *Output:* `NameError: cannot access class 'Player' without creating it`
* **`ArgumentError`** — Violating arguments rules where you dont pass a critical argument.
  * *Output:* `ArgumentError: expected argument got none`
* **`AssignmentError`** — Collision violations (e.g., trying to duplicate a variable declaration inside the same scope boundary).
  * *Output:* `AssignmentError: 
* **`TypeError`** — Attempting to push mismatched data types into an explicit hardware slot.
  * *Output:* `TypeError: expected value 'int' got 'decimal' in type 'int'
* **`ValueError`** — Encountering an unbound data transformation or failing to secure a raw layout slot for conversion.
  * *Output:* `ValueError: expected variable to store output`
* **`MathError`** — Emitted when a mathematical expression is unresolvable.
  * *Output:* `MathError: invalid operation '0 ** 0'`
* **`IndexError`** — Index violation(e.g., trying to read over the index limit).
* **`TaskError`** — A floating builtins or smth else without giving a task(Eg: 'System' without any runnable attribute or task to do)
  * *Output:* `TaskError: expected task from module 'System' got floating module  `
* **`ModuleError`** — Unknown module name
  *Output:* `ModuleError: no module named 'poopington'`
* **`FormatError`** — Occurs when the string formatting is logically impossible
  *Output:* `FormatError: combination of 'f' and 'r' is invalid`

### Warnings
* **`SyntaxWarning`** — Things that might break in the future or violate syntax in the future or a warning that code might break in runtime
* **`DeprecatedWarning`** — Warning of deprecated feature, builtins or modules .etc(Example: `DeprecatedWarning: feature/module/builtin 'name' is deprecated
* **`MemoryWarning`** — Allocating a massive block more than 4MB(Example: MemoryWarning: memory allocation exceeded more than 4MB, proceed with caution)
* **`


---

## 7. Organizational Project Layout(Planning, Still empty, Feel free to join the Geyser army! But to join you need to know C and not just little, like a lot to make that C compiler(only for Team 2))

Development moves in a strictly disciplined pipeline under your direct command:
* **Team 1 (Compiler Thinkers):** Maps specifications to native hardware logic and handles register layouts within the C compiler backend(Need to know - Compiler making and ideas / System / OS a lot).
* **Team 2 (Syntax Developers):** Builds the actual C tokenizer, lexer, and parser to read `.gy` source text and enforce compile-time error gates.
* **Team 3 (Code Breakers):** Aggressively stress-tests the system by writing broken code to find compiler exploits, racing bugs, or memory leakage flaws(Need to know - Find the absolute ways and every way to break my compiler for fun).
* **Team 4 (Launch & Media):** Manages the official website, syntax highlighters, documentation, and handles public advertisements to drive industry adoption(Need to know - Professional designer and influencer).
* **Team 5 (OS and CPU archiealogical specialists):** Handles advanced OS thinking and CPU's to make Geyser suitable and make it support devices
,
---

### Geyser specifications

  * Geyser is a compiled language designed for lightning fast execution
  * Geyser is growing and being planned so geyser might replace `Python` and `C++`
  * Geyser has a real use and is not your everyday **`esolang`**

---

### Example geyser script

```java
import geyser.lang.System;
import geyser.lang.Prompt;
import geyser.lang.Time;
import geyser.lang.ArrayList;
import geyser.time.Datetime;
import geyser.time.MeasureTimeByTask;
import geyser.lang.string.concatenate;
import geyser.lang.conversion.toString; // and also toInt, toBool and toDecimal
import geyser.lang.format.formatCurlyBrackets; // deprecated

// 1. Outputing text
System.print("Hello World");

// 2. Variables
int age = 9;
decimal temp = 37.8;
String language = "Geyser";
boolean isEsolang = false;

// 3. Arrays
ArrayList<String> inventory = ["Apple", "Banana", "Grapes"];

// 4. Time sleeps
System.print("Hello...\n");
Time.wait(1000);
System.print("... World");

// 5. Asking input
String name = Prompt("Enter your name: ").toLowerCase().replaceAll(' ', '');
System.print("Hello ".concatenate(name));

// 6. Getting system time and data conversion
DatetimeObject currentTime =  new Datetime();
currentTime.getSystemTime("H:M:S");
String status = "Vibing"; // Quick variable inserting
System.print("Current time is ".concatenate(currentTime.toString()));

// 7. Recommended: Concatenation, Original easy to read and fast version
System.print("Hello! I am ".concatenate(status));

// 8. Deprecated: Curly bracket formating, Alternative professional version yet slow
System.print(formatCurlyBrackets("Hello! I am {status}"));

// 9. Measuring time
decimal timeToPrint = MeasureTimeByTask(task=iq"System.print("Hello")", measureBy="picoseconds");
System.print(formatCurlyBrackets("Time took to print 'Hello' was {timeToPrint}"));
```

If you had any feedback, Please share it out, we are always ears out
