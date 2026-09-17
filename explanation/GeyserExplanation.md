# The Geyser Programming Language (.gy)

> *(Important copyright notes: This project incorporates components from LLVM (targets/x86_64-windows/...), licensed under the Apache License 2.0 with LLVM Exceptions. See targets/x86_64-windows/include/llvm/Support/LICENSE.TXT for full license text. Original source: https://llvm.org)

**Core Vision:** A strict, explicit, highly predictable, and blistering fast alternative to C++.

Geyser is a systems programming language engineered from the ground up for high-performance software, real-time engines, robotics, and advanced simulators (like *Fortnite*, *Cyberpunk 2077*). By dropping 40 years of legacy backward-compatibility baggage and utilizing a native, highly optimized C-powered compiler backend linked with LLVM-22, Geyser achieves maximum hardware execution speed, if you can't handle the tiny boilerplate, **go back to python**.

---
## 1. Terminal commands

### Downloading something
Geyser does this by fetching from the net to download the modules via:
```bash
geyser install vulkanapi
```

### Running a file
Geyser has two options:
*Path 1* — If you had an IDE, Simply click the run button
*Path 2* — By terminal, run `geyser run "path to your file"`

### Building a `*.exe` file
We do this by geyser's `gyexe`(need to be geyser installed via `geyser install gyexe)
And to convert it, run
```bash
gybuild build "path/to/your/file.gy" --output "path/to/your/file.exe" --architecture architecture --optimize=O_plus_number_from_0_to_3
```
*--noconsole/--console:* Optional  
*--collect-all modules_by_geyser_install:* Recommended(Only if your project contains modules by geyser install)  
*--onefile/--onedir:* Mandatory  
*--file="file_name.gy":* Mandatory  
*--at:* Mandatory  
*--target:* Mandatory

An example command to convert to a `*.exe`(win) file is
```bash
geyser install gybuild
gybuild build "C:/Users/Dell/game.gy" --output "C:/Users/Dell/game.exe" --architecture arm64 --optimize=O0
```

The path could significantly vary based on the hardware's OS.

---
## 2. Core Architectural Pillars

### Explicit over Implicit (No Guessing Games)
Geyser outlaws compiler guesswork. Every data type, scope boundary, statement terminator, and logical evaluation must be completely explicit. If the code contains ambiguity, the compiler halts instantly at build time. 

If a programmer cannot handle explicitness, they can go back to Python.

### Features outside string literals to change things
* **`r`**  
  Treats the string everything as literal
* **`iq`**  
  Ignores in-between quotes inside string literals
* **`f`**  
  Formats curly brackets (Evaluates dynamic tokens into flat string literals at compile-time with zero execution penalty)
  > *Note, If you want to add multiple string literal changers, you need to use comma's(Eg: (f, iq, r"Hello ""\ \ "))

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
To protect tight-loop latency, Geyser enforces a default variable initializing-after division at the hardware processor layer:
* **Loops are Cached:** Loop execution blocks and branching mechanisms are held natively in the CPU Instruction Cache (I-Cache) to repeat at maximum processor clock speeds.
* **Variables are Uncached:** To solve multi-threaded data synchronization bugs, variable reads and writes completely bypass the local CPU Data Cache (D-Cache). Operations interact directly with physical memory slots, ensuring 100% real-time data transparency across all cores. If a variable is marked as `cache`, it is retained in the D-Cache, and the program will still read from the cache unless manual `uncache variable;` is invoked, we are not holding your hand.

---
## 3. Core Data Types

Variables are raw physical memory slots. They never compile into heavy, tracking object layers or dynamic wrappers. Once assigned, a variable remains that type permanently.

* `unsigned` — Makes the target binary value unsigned
* `int` — Flat whole number hardware blocks. By default, its signed(Optional: If you want to be more precise, you can join a bit's number to the 'int' prefix(e.g: unsigned int32, int1024), can go up to much as you want, just... i hope you have that much memory if you are allocating a lot).
* `decimal` — High-precision fractional numbers for physics and fluid simulations(The same 'number' rule is applied but by 'decnumber'(e.g: 'dec32')).
* `String` — Strict, flat text character sequences.
* `boolean` — Evaluation literals (`true` or `false`).
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
Geyser mades that **Wildcard imports (e.g., `import package.*;`) are strictly banned.** This prevents namespace pollution, and ensures no hidden names are snuck into your file scope.

### The Dictionary
```java
import geyser.lang.Dictionary;
import geyser.lang.System;
import geyser.lang.conversion.toString;
Dict profile = {
    "name": "Alex",
    "userid": 10452 // Loose trailing ',' are forbidden with a "ValueError: expected value after  ','"
};

// Printing the value with the name
System.print(toString(profile["name"]));
```

### Comments
```java
// This is a comment, it does nothing
// Its only for notes and helper identifier
// An example is
int i = 0; // Index
// Thats it! Comments does nothing but helps the programmer
```

### System exits
```java
import geyser.lang.System;
System.exitWithReturnCode(1);
```

### Code Formatting
```java
// Semicolons at the end of a statement are strictly mandatory
import geyser.lang.System;
import geyser.lang.Prompt;
import geyser.lang.string.concatenate;

// Global variables are visible everywhere, avoiding nested local-scope inheritance locks
global String gameTitle = "DocItOut";

// Indentation does not matter to the compiler; it is strictly for human beauty
String part1 = "Patient status: ";
String part2 = "Stable\n";
String status = part1.concatenate(part2); // The '+' operator is purified strictly for math

System.print(status);

// Double quotes for Strings, single quotes for clean single-byte character literals
String username = Prompt("Enter surgeon name: ").toLowerCase().replaceAll(' ', '');

// Tuples are default and allowed
Tuple patients = ("101", 999, true, 10.5);
String password = Prompt("Enter secure password: ", maskWith='*', ignore=(' ', '\r', '\n'))

// ANSI
System.print("\033[38;2;255;255;0mHello World\033[0m\n");
```

### Strict Mathematical Rules
1. Math cannot be loosely calculated without a memory container slot to receive the output.
2. An existing, declared variable name cannot be re-declared.
3. Modification of an existing slot must use explicit compound mutation operators (`+=`, `-=`, `*=`, `/=` etc.), reassignment without declaring type again or reassignment with math operators etc..
4. Slot type must match the value else (TypeError: mismatched types between slot type and value)
5. Truncation in values are gurranted to not happen unless explicitly told to do so

```java
int patientPulse = 70;
patientPulse += 5; // Valid: mutates the hardware slot directly

int patientHB = 80 + 10; // Also valid: Constant folding handles this at build-time with zero runtime penalty

10 + 10; // Invalid
```

### Index
1. Index cannot be over the string/list etc. range
2. Index requires a strict square brackets formatting
3. Index is just the standard `[start:stop:step]` rules(also it also has negative indexing, same rules, starts at 0 in positive, starts at -1 in negative)
4. Index guarantees it does not unexpectedly give up just because the output is *bad* to it no matter what(Unlike Python)

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
import geyser.lang.string.concatenate as concat;

// Testing CSPRNG
int number = Random.secureRandomInt(0, 10);

// Testing Standard PRNG
int number1 = Random.randomInt(0, 10);

// Testing true randomness from OS
int number2 = Random.trueRandomInt(0, 10);

System.print(number.concat(' ').concat(number1).concat(' ').concat(number2)); // OUTPUT: 7 3 9
```

### Collections, Arrays & Objects
```java
// Lists use flags to ease human pain and add more features while still keeping the code blazing fast
List<mutable, resizable> inventory = [];

// EXPLICIT INDEX REQUIREMENT RULE:
// If a list already contains elements, adding an item requires an index target parameter
// to explicitly state where the tail placement or offset is verified. The list must be mutable and resizable in order to add or pop else (SyntaxError: cannot add/remove item in immutable/resizable list). If missing the index brackets entirely, the compiler throws an error instantly.
inventory.add("Prunes")[-1];

// Functions in geyser: void, int, String, decimal, boolean are the return types, use all if a function has mixed in return types
all func calculateScoreIfFailOrPass(int score) {
    if (score > 100) {
        return "TOO_HIGH";
    } elseif (score < 35) {
        return true;
    } else {
        return false;
    }
}
// Boilerplate-free classes with automatically mapped constructors
class Player {
    String name;
    int health;
    void func heal(int amount) {
        health += amount;
    }
}

Player myPlayer = new Player("Surgeon", 100);
System.print(myPlayer.health);
myPlayer.heal(100);
System.print(myPlayer.health); // Testing if it really increased
```

---
## 5. Build Integrity & The Error Engine

Geyser blocks bugs before they can ever execute on hardware by throwing descriptive compile-time errors instantly and smart explicit warnings:

### Errors
* **`SyntaxError`** — Disrespecting language grammar (e.g., leaving out a mandatory trailing semicolon `;`, attempting an unauthorized wildcard `*` import, or omitting list element injection index parameters `[]`).
  *Output:* `SyntaxError: explicit index placement required for collection mutation`
* **`ConditionalError`** — Attempting to evaluate a raw condition without an explicit comparison check.
  *Output:* `ConditionalError: cannot reference true or false to variable with no true check or false check`
* **`NameError`** — referencing an unknown identifier, using a variable before it exists, or trying to access a class layout without instantiating it.
  *Output:* `NameError: cannot access class 'Player' without creating it`
* **`ArgumentError`** — Violating arguments rules where you dont pass a critical argument.
  *Output:* `ArgumentError: expected argument got none`
* **`AssignmentError`** — Collision violations (e.g., trying to duplicate a variable declaration inside the same scope boundary).
  *Output:* `AssignmentError: cannot reassign variable 'currentVelocity'`
* **`AttributeError`** — Unknown attributes
  *Output:* `AttributeError: module 'System' does not have attribute 'Wipe'`
* **`TypeError`** — Attempting to push mismatched data types into an explicit hardware slot.
  *Output:* `TypeError: expected value 'int' got 'decimal' in type 'int'`
* **`ValueError`** — Encountering an unbound data transformation or failing to secure a raw layout slot for conversion.
  *Output:* `ValueError: expected variable to store output`
* **`IndexError`** — Index violation(e.g., trying to read over the index limit).
* **`ModuleError`** — Unknown module name
  *Output:* `ModuleError: no module named 'Bloat'`
* **`FormatError`** — Occurs when the string formatting is logically impossible
  *Output:* `FormatError: combination of 'f' and 'r' is invalid`
* **`ExitError`** — Commonly a invalid exit code
  *Output:* `ExitError: exit code 'Crazy' is invalid`

### Warnings
* **`SyntaxWarning`** — Things that might break in the future or violate syntax in the future or a warning that code might break in runtime
* **`DeprecatedWarning`** — Warning of deprecated feature, builtins or modules .etc (Example: `DeprecatedWarning: feature/module/builtin 'name' is deprecated`)

> Note: Also during compilation if this error pops up it throws an y/N prompt to continue compilation

---
## 6. Organizational Project Layout

Development moves in a strictly disciplined pipeline under the command of the Chief Architect:
* **Team 1 (Compiler Thinkers):** Maps specifications to native hardware logic and handles register layouts within the C/LLVM backend.
* **Team 2 (Syntax Developers):** Builds the actual C tokenizer, lexer, and parser within CLion to read `.gy` source text and enforce compile-time error gates.
* **Team 3 (Code Breakers):** Aggressively stress-tests the system by writing broken code to find compiler exploits, racing bugs, or memory leakage flaws.
* **Team 4 (Launch & Media):** Manages the official website, syntax highlighters, documentation, and handles public advertisements to drive industry adoption.
* **Team 5 (OS and CPU archaeological specialists):** Handles advanced cross-platform OS layers and target instructions to ensure native binary efficiency.

---
### Geyser specifications

  * Geyser is a compiled language designed for lightning fast execution.
  * Geyser is growing and being planned so geyser might replace `Python` and `C++` uses (Not duck-typing, i mean their uses (Like how Python is used for Data science)).
  * Geyser has a real use and is not your everyday **`esolang`**.

---
### Example geyser script

```java
import geyser.lang.System;
import geyser.lang.Prompt;
import geyser.lang.Time;
import geyser.lang.List;
import geyser.time.Datetime;
import geyser.time.MeasureTimeByTask;
import geyser.lang.string.concatenate;
import geyser.lang.conversion.toString; // and also toInt, toBool and toDecimal

// 1. Outputing text
System.print("Hello World");

// 2. Variables
int age = 9;
decimal temp = 37.8;
String language = "Geyser";
boolean isEsolang = false;

// 3. Arrays
List<mutable, resizable> inventory = ["Apple", "Banana", "Grapes"];

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

// 7. Concatenation, Original but tedious to read version, its going to compile it into an single string anyways, the con is that this is absolutely frustrating and tedious to type
System.print("Hello! I am ".concatenate(status));

// 8. Recommended: Curly bracket formating, Alternative professional version, its going to compile it into an single string anyways, the only con is that for this compilation tinily slows down but its harmless, the real execution speed is what matters the most
System.print(f"Hello! I am {status}");

// 9. Measuring time
decimal timeToPrint = MeasureTimeByTask(task=iq"System.print("Hello")", measureBy="picoseconds");
System.print(f"Time took to print 'Hello' was {timeToPrint}");
```