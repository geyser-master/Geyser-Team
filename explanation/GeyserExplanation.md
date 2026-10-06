# The Geyser Programming Language (.gy)

> *(Important copyright notes: This project incorporates components from LLVM (targets/x86_64-windows/...), licensed under the Apache License 2.0 with LLVM Exceptions. See targets/x86_64-windows/include/llvm/Support/LICENSE.TXT for full license text. Original source: https://llvm.org)*

**Core Vision:** A strict, explicit, highly predictable, and blistering fast alternative to C++.

Geyser is a systems programming language engineered from the ground up for high-performance software, real-time engines, robotics, and advanced simulators (like *Fortnite*, *Cyberpunk 2077*). By dropping 40 years of legacy backward-compatibility baggage and utilizing a native, highly optimized C++-powered compiler backend linked with LLVM-22, Geyser achieves maximum hardware execution speed. It has a built-in Geyser Debugger (GYDB), its dedicated debugger and error-handling tool which works alongside `gy.exe` to provide detailed diagnostics and debugging information when Geyser programs fail. In this following code examples I will be using x86_64-windows. Currently this is how the main Geyser package is currently now planned as:

```text
Geyser-x86_64-windows /
    bin /
        engine /
            codegen.exe
            detect_target.exe
            lexer.exe
            parser.exe
        cmdlet /
            gy.exe
            gpm.exe
            gybuild.exe
            gydb.exel
        include /
            tokens.h
        lib /
            3party /
                GPM downloaded files...
            geyser /
                standard library...
    explanation /
        GeyserExplanation.md
    targets /
        x86-64-windows /
            ...
    README.md
```

---
## 1. Terminal commands

### Downloading something
Geyser does this by calling the MSYS2 pacman to fetch the required package:

```bash
gpm install vulkan, raylib, opengl, imgui # GPM means Geyser Package Manager
```

### Running a file
Geyser has two options:
*Path 1* — If you had an IDE, Simply click the run button
*Path 2* — By terminal, run `gy run "path to your file"`

### Building a `*.exe` file
We do this by geyser's `gybuild`, to convert it, run
```bash
gybuild build "path/to/your/file.gy" --output "keep/the/file/at/here.exe" --architecture architecture --optimize=O_plus_number_from_0_to_3
```

An example command to convert to a `*.exe`(win) file is
```bash
gybuild build "C:/Users/Dell/game.gy" --output "C:/Users/Dell/game.exe" --architecture arm64 --optimize=O0
```

The path could significantly vary based on the hardware's OS.

### Updating Geyser
Sometimes you don't want the old geyser and want the latest geyser in the repo, to do that, run:
```bash
geyser update
```

This asks to close the window, runs a script after the window is closed to execute after 2 seconds to make sure the window is closed, rm -rf's the root geyser-...-version, downloads the latest version in the repo, unzips it in the background, and voila! done

---
## 2. Core Architectural Pillars

### Explicit over Implicit (No Guessing Games)
Geyser outlaws compiler guesswork. Every data type, scope boundary, statement terminator, and logical evaluation must be completely explicit. If the code breaks geyser rules or contains ambiguity, the compiler halts instantly at build time. 

### Features outside string literals to change things(Usually for printing and stuff)
* **`r`**  
  Treats the string everything as literal
* **`iq`**  
  Ignores in-between quotes inside string literals
* **`f`**  
  Formats curly brackets (Evaluates dynamic tokens into flat string literals at compile-time with zero execution penalty)
  > *Note, If you want to add multiple string literal changers, you need to use commas (Eg: (r, iq"\" 'e' """))

```java
// Sane Modifier Rule: You can combine (f, iq) or (iq, r), but mixing formatting and raw text behavior is illegal.
String invalidCombination = (f, r"Path: {user}"); // FormatError: combination of 'f' and 'r' is invalid
```

### Existing attributes currently in **`Geyser`**
* `toLowerCase()`  
  Makes a string lowercase
* `toUpperCase()`  
  Makes a string uppercase
* `replaceAll(old, new)` 
  Replaces the old string with the new string
* `capitalizeFirstLetter()`  
  Makes a certain string first letter capitalized
* ~~`lengthOf()/byteLengthOf()/bitLengthOf()`~~  
  Removed lengthOf feature

### The "Yeet the GC" Memory Strategy
The Garbage Collector is permanently banished to eliminate background lag spikes and unpredictable stuttering frames. Geyser utilizes **Automatic Scope-Based Cleanup**:
* Local data sits flat inside memory stack slots.
* Constant top-level objects stay active in the root file scope until the execution terminates, at which point the Operating System reclaims the entire layout at EOF (End of File).

### Introducing 'variable deletion'
We are introducing manual 'variable deletion' ability, the use of it is to help clearing data to prevent a memory leak, it clears the **pointer** and **data** basically everything, it is used via 'name.delete()'
Here is an code example

```java
// Compile-Time Eviction
// When .delete() is called, the variable name is scrubbed from the compiler's symbol table.
// Any attempt to use it again in the same scope results in an immediate build error.
int x = 10;
x.delete();
IO.output(cast(x, String)); // NameError: undefined name 'x'

// Aliases: Assigning one variable to another object variable creates an alias to the original variable/object rather than copying its underlying data. The alias acts as an additional name for the original target. Deleting the original target does not magically rewrite every alias; aliases referring to the destroyed target may therefore become dangling.
Player p1 = new Player("Surgeon", 100);
Player p2 = p1; // p2 is an alias (additional name) referring to p1
p1.delete();
p2.heal(50); // NameError: no such function 'heal' in p2 (OR in some cases a SIGSEGV or misinterpreting the bytes as code)
IO.output(cast(p2, String)); // Garbage data, SIGSEGV or misinterpreting the bytes as code
```

### Hardware cache-ing
Using the linear-data ram-data arrangement, Geyser's memory layout and data locality may allow the compiler to generate cache-friendly access patterns. Actual cache behavior is determined by the target CPU's cache hierarchy and hardware policies.

---
## 3. Core Data Types

Variables are raw physical memory slots. They never compile into heavy, tracking object layers or dynamic wrappers. Once assigned, a variable remains that type permanently.

* `unsigned` — Makes the target binary value unsigned(only workable for ints)
* `int` — Flat whole number hardware blocks. By default, its signed(Optional: If you want to be more precise, you can join a bit's number to the 'int' prefix(e.g: unsigned int32), it can scale as large as they want by 64-bit joining if exceeding 64-bit, When an integer operation exceeds the representable range of its declared width, the value wraps around to the opposite end of that range. This behavior is deterministic and defined by Geyser).
* `decimal` — High-precision fractional numbers for physics and fluid simulations(The same 'number' rule is applied but by the pattern 'decnumber'(e.g: 'dec32'), dec32/dec64 etc. are floating-point precision numbers specifically, dec follow standard IEEE-754 rules).
* `String` — Strict, flat text character sequences.
* `bool` — Boolean values that evaluate to `true` or `false`.
* `const` — Makes a variable permanently immutable after initialization.
* `null` — The null value, representing the absence of a valid object or memory target so it can be used on any variable's value (Truly its a damn null, not sneaky ((void *)0)), Geyser performs no implicit null-safety check. Dereferencing null constitutes an invalid memory access and may terminate the process through the target platform's memory-protection mechanisms.
* `comptime` — A keyword to make a specific variable/function/class etc. evaluated at compile-time.
* `auto` — Automatically determines a variable's type from its initializer at compile time. Once inferred, the variable behaves as though its inferred type had been explicitly declared.
* `pointer` — A raw memory-address type whose width and representation are determined by the target architecture.

---
## 4. Syntax & Grammar Guide

### The Main Scope
The root of a `.gy` file *is* the main execution area. There are no useless class wrappers or mandatory `main` methods required just to say hello. Extra functions and classes simply create explicit inner scopes when called.

### Module Import Tools & Explicit Options
Geyser mades that **Wildcard imports (e.g., `import package.*;`) are strictly banned.** This prevents namespace pollution, and ensures no hidden names are snuck into your file scope.

### The Dictionary
```java
import geyser.lang.Dict;
import geyser.lang.IO;
Dict profile = {
    "name": "Alex",
    "userid": 10452, // A trailing comma after the final dictionary entry is permitted and removed during compilation.
};

// Printing the value with the name
IO.output(cast(profile["name"], String));
```

### Try/Catch block
```java
import geyser.lang.IO;

try {
    error; // Intercepts the error, checks the catch on what its trying to catch, if the erro matches what catch is trying to catch, it skips the entire try block, executes the script in catch and done, else ignores the error
} catch (NameError as e) { // Assigns a alias to the passen name-error in the try-block, in here I used 'e'
    IO.output("Error caught: {cast(e, String)}");
}
```

### Comments
```java
// This is a comment, it does nothing
// Its only for notes and helper identifier
// An example is
auto i = 0; // Index
// Thats it! Comments does nothing but help the programmer, it automatically gets stripped out during compile time
```

### The System library
All the System functions, all of these are examples

```java
import geyser.lang.System;

System.time.changeSystemTime(ymdhms="2026.1.1 00:00:00"); // Changes system clock
System.time.changeTimezone("+12"); // Changes timezone to a specific territory
System.geyser.IO.streamEncoding("utf-8"); // Sets encoding for Input/Output stream
System.geyser.IO.output.flush(); // Flushes the buffer
System.geyser.IO.input.maskUserInputWith('*'); // Masks the user input with a specific character/string
System.geyser.IO.input.ignoreCharacters("\n", " ", "\r"); // Ignores specific character signals sended by the keyboard
System.core.getLatestGeyserVersion(); // Gets the latest geyser version
System.core.getCurrentGeyserVersion(); // Gets the current geyser version on the system the user is using Geyser
System.core.getLanguageName(); // Returns the language name 'Geyser'
System.core.exitProcessWithReturnCode(0); // Returns a exit code and terminates the program
```

### The Math library
All the Math functions, all of these are examples

```java
import geyser.lang.Math;


```

### Time sleeps
```java
import geyser.lang.Time;
Time.wait(1, unit="second");
```

### Memory addresses
```java
import geyser.lang.IO;
int x = 10;
int y = 20;
pointer ptrX = addressOf(x); // e.g: 0x1000
pointer manualPtrX = 0x1000;
ptrX = addressOf(y); // Changes the memory address to 'y'
valueOf(manualPtrX) = 30; // Changes the value of the address to 30
IO.output(f"X: {cast(x, String)} | Y: {cast(y, String)}");
```

### Multi-Threading
```java
import geyser.lang.Threading;
import geyser.lang.Time;
import geyser.lang.IO;
void func calculatePizzaArrival(int BOOB) {
    for (int i = 0; i <= 3; i += 1) {
        Time.wait(1, unit="second");
    }
    IO.output("Calculated pizza arrival time: 5022 seconds");
}
void func calculateEatingTime() {
    for (int i = 0; i <= 3; i += 1) {
        Time.wait(1, unit="second");
    }
    IO.output("Calculated dinner time: 3544 seconds");
}

// If two threads try to modify an address at the exact same time, who was first is allowed to modify, the second has to wait
// If a thread encounters a error, it gets immediately destroyed
Thread workerA = Threading.newThread(task=calculatePizzaArrival, args=(10), daemon=true); // The args argument passes required argument to the functions in a specific order, this argument is optional
Thread workerB = Threading.newThread(task=calculateEatingTime, daemon=true);
Thread workerC = Threading.newThread(task=calculateEatingTime, daemon=true, priority=10); // priority argument is optional to 'set' a priority to a thread
workerC.startThread();
Time.wait((workerC.timeBeforeFinish / 2), unit="seconds");
if (workerC.isFinished() == false) {
    IO.output(f"Grrh! Im impatient on the {cast(workerC.ID, String)}");
}
workerA.startThread();
Time.wait(2, unit="seconds");
workerA.stopThread(); // Pauses the thread to be started again
Time.wait(10, unit="seconds");
workerB.startThread();
workerA.waitForThread(workerB);
Time.wait(10, unit="seconds");
workerA.killThread(); // Kills the thread cleaing it up
```

### Powerful binary tools, math, and value type prefixes
```java
int xor = 10 ^ 9; // OUTPUT: 3
int amp = 10 & 9; // OUTPUT: 8
int pip = 10 | 9; // OUTPUT: 11
int tid = ~10; // OUTPUT: -11
int mod = 10 % 9; // OUTPUT: 1
dec64 sum = (10 + 10) - 9 + 8 * 7 / 6 + (5 ** 4) / 3 * ~2 + 1; // OUTPUT: -646.1666666666
hex hexadecimal_num = 0xFF; // Chooses the minimum length required for the following data if no angle-bracket length specifier is passed
bin<16> binary_num = 0b11111111; // An optional <N> parameter explicitly specifies the storage width. When no width is supplied, Geyser selects the minimum width required to represent the value.
```

### Reading other .gy files gossips and secrets
```java
import c.Users.Dell.main; // its a GY file, for paths, if the cat starts with a root drive name, it automatically starts from it, else defaults to the root ~ on linux, . on windows
main.Player myPlayer = new Player("Surgeon", 100); // ANOTHER SURGEON???
myPlayer.heal(100);
```

### Code Formatting
```java
// Semicolons at the end of a statement are strictly mandatory
import geyser.lang.IO;
import geyser.lang.string.concatenate;

// Global prefixes are unnesessary to prevent damn pain-in-the-ahh 'restricted area' errors
String gameTitle = "DocItOut";

// Indentation does not matter to the compiler; it is strictly for human beauty
String part1 = "Patient status: ";
String part2 = "Stable\n";
String status = part1.concatenate(part2); // The '+' operator is purified strictly for math

// cast(value, type) explicitly converts a value when a valid conversion exists. Invalid or meaningless conversions are rejected at compile time.
IO.output(cast(status, String));

// Double quotes for Strings, single quotes for clean single-byte character literals
System.IO.input.ignoreCharacters("\n", " ", "\r");
String username = IO.input("Enter your username: ");

// Tuples are default and allowed
System.IO.input.maskUserInputWith('*');
String password = IO.input("Enter secure password: ");

System.IO.input.ignoreCharacters(null);
System.IO.input.maskUserInputWith(null);

// ANSI
IO.output("\033[38;2;255;255;0mHello World\033[0m\n");
```

### Strict Mathematical Rules
1. An existing, declared variable name cannot be re-declared.
2. Modification of an existing slot must use explicit compound mutation operators (`+=`, `-=`, `*=`, `/=` etc.), reassignment without declaring type again or reassignment with math operators etc..
3. Slot type must match the value else (TypeError: mismatched types between slot type and value)
4. Truncation in values are guaranted to not happen unless explicitly told to do so
5. Math is evaluated left-to-right instead of PEMDAS/BODMAS, Parenthetical expressions are solved first, then left-to-right math, decimal precision length for printing is 10, can be increased by 'System.math.increaseDecimalPrecisionBy(N);'

```java
int patientPulse = 70;
patientPulse += 5; // Valid: mutates the hardware slot directly

int patientHB = 80 + 10; // Also valid: Constant folding handles this at build-time with zero runtime penalty

10 + 10; // Gets removed by optimizer
```

### Index
1. Index cannot be over the string/list etc. range
2. Index requires a strict square brackets formatting
3. Index is just the standard `[start:stop:step]` rules(also it also has negative indexing, same rules, starts at 0 in positive, starts at -1 in negative)
4. Index here is not too confuzing

```java
String text = "Motherfather";
String result = text[0:6]; // Starts: 0, Ends: before 6
```

### Ultra-Strict Control Flow & Loops
Conditions inside `if` statements require explicit true/false comparison operators. Implicit shortcut evaluations are illegal. The third instruction in a for loop header may optionally be followed by one trailing semicolon. The semicolon is redundant and removed during compilation. More than one trailing semicolon is invalid syntax with a 'SyntaxError: more than 1 trailing semicolon in for-loop', Machine code doesnt care about semicolons so the entire statement is anyways... going to be converted to machine code.

```java
bool patientBleeding = true;
int emergencyLevel = 5;

// .exists() is a compiler-recognized memory-presence check. Unlike ordinary variable access, it may be evaluated even when the referenced variable has been deleted or is otherwise unavailable by normal name lookup. It returns true when the referenced memory/object is present and false when it is absent.
if (emergencyLevel.exists() and patientBleeding == true) {
    IO.output("Initiate surgery\n");
} elseif (patientBleeding == false) {
    IO.output("Vitals stable\n");
} else {
    IO.output("Evaluating\n");
}

// Logical text operators ('and', 'or') are used instead of confusing '&&' or '||'
// Compiler quietly optimizes the redundant semicolons, adding it is no use, machine code doesn't care of semicolons
for (int i = 0; i < 100; i += 1;) {
    IO.output(cast(i, String));
}
```

### Meet randomness and cryptography
```java
import geyser.lang.Random;
import geyser.lang.CryptoHash;
import geyser.lang.IO;

// Testing all types of randomness in geyser
// 1. randomNum: randomly generates an number based on the type, returnType, from and to
// type param values: (secure, true, normal)
// returnType param values: (int, decimal, bool)
// from param values: int: any
// to param values: int: any
int rand_num = Random.randomNum(type="secure", returnType="int", from=1, to=2);

// 2. randomChoice: randomly guesses a choice on the given list
// choice param values: (int, String, bool, decimal): any
String func generateRandomChoice() { 
    return Random.randomChoice(choice=("Yes", "No"));
}

// 3. randomHex/randomBin: randomly seeds a hex/bin based on the length by CSPRNG
// length param values: int: any
hex hex_salad = Random.randomHex(length=12);
bin bin_salad = Random.randomBin(length=24);

// 4. encrypt/hash: encrypts/hashes with the following algorithm
// algorithm param values: encrypter/hasher name(lowercase, e.g: tls/sha256)
// value param values: any: any
hex encrypted_string = CryptoHash.encrypt(algorithm="aes256gcm", value="TOP SECRET!!!");
hex hashed_string = CryptoHash.hash(algorithm="sha256", value="HYPER SECRET!!!");
```

### System and OS'es
```java
import geyser.lang.os.windows; // Imports all the OS features of windows
import geyser.lang.os.linux; // Imports all the OS features of linux
import geyser.lang.os.unix; // Imports all the OS features of unix
```

### Collections, Arrays, Matrixes & Objects
```java
// Lists use flags to ease human pain and add more features while still keeping the code blazing fast, lists can hold any data type
// List can only hold a pre-initialized variable OR a raw value
// Lists have a default initialization flags of 'mutable' and 'resizable' if explicit <> flags are not passed
List inventory = ["Apple", "Banana"];

// EXPLICIT INDEX REQUIREMENT RULE:
// If a list already contains elements, adding an item requires an index target parameter
// to explicitly state where the tail placement or offset is verified. The list must be mutable and resizable in order to add or pop else (SyntaxError: cannot add/remove item in immutable/resizable list). If missing the 'at' argument, the compiler throws an error instantly, it works like
//  0  1
// -2 -1
//  A  B
// In here, A means Apple, B means Banana
// Now we want to add 'Prunes', we name it 'P' in this example
//  0  1  2 <-- Look! New index
// -3 -2 -1 <-- geyser recalculates the negative index
//  A  B  C
// Notice? Index works like
// e.g: we add prune in '-1' so to do it geyser
//  0  1  2 <-- Adds a new slot index
// -3 -2 -1 <-- Updates the negative index
//  A  B []
// In here we need to add Prune in that empty -1, but in geyser adding an item, its index gets incremented to make the item go to the right empty slot, so we need to use -2 in here where it goes automatically to the right which is -1 and places Prunes in there! now out list is
//  A  B  P
// Now you think "What about in between!?"
// It works the same
// So now if we want to add 'Oranges', lets name them 'O'
// We want to place it in '-2' shifting everything to the right to reserve a space, so we need to use '-3' to make it go to the right to become '-2', pushing the things after it a right to reserve a slot, update the negative and positive index, and place 'O' in the new slot, so now the array is
//  A  O  B  P
inventory.add("Prunes", at=(-2)); // Added the '()' so the = and - don't get confused and the () evaluates first

// Matrixes can be any dimension as they want
List<mutable, unresizable> matrix = [
    [0, 1, 0],
    [0, 0, 0],
    [1, 0, 1],
];

// List can contain practically a lot
List<mutable, unresizable> randomness_poop = [
    [p1, p2, p3],
    [func1, func2, func3],
    [1, "Poop", true],
];

// Functions in geyser: void, int, String, decimal, bool, basically anything, Period
bool func calculateScoreIfFailElseCreateID(int score) {
    if (score > 100) {
        return false;
    } elseif (score < 35) {
        return true;
    } else {
        return false;
    }
}

// Data-Escaping, nested functions, and nested classes
int func nightmare() {
    int func collosal() {
        int Haha = 10;
        return Haha; // If Haha wasn't freed, it would be cleared when this function was called, but since it can return, if it was called, the variable gets released to that scope
    }
    return -1;
}

comptime class NestedNightmare {
    class NestedChild {
        int data = nightmare.collosal();
    }
}

// Boilerplate-free classes with explicit constructors
// Code outside private/public {} are invalid with a 'SyntaxError: cannot decide variable/function/class is public or private'
// private and public {} definition is mandatory, if unneeded, simply keep them empty
class Animal {
    private {
        
    }

    public {
        String name;
        int health;
        
        // Constructor executes code right-after class is initialized
        // Constructor orders is the root parent being constructed first, then the childs 1 level down, and so on until the last childs with no childs of themselfs (leafs)
        constructor Animal(String newName, int newHealth) {
            name = newName;
            health = newHealth;
        }

        pointer ptr = addressOf(health);

        void func bark() {
            IO.output("Woof!");
        }

        // Destructor executes code right-before class gets sent to the shadow-realm
        // Destructor orders is the child with no child of its own is destroyed first, then the childs/parent 1 level up, and so on until the main root parent, then the entire class tolded to be deleted (.delete()) is reclaimed
        destructor Animal() {
            ptr = null;
        }
    }
}

// Class can inherit more than 1 classes(e.g: Class Player inherits Speech, Animal), A class only inherits the extra feature from the inherited class, not the entire class
class Player inherits Animal {
    private {
        String secret;
    }

    public {
        int health;
        String name;

        constructor Player(String newName, int newHealth) {
            name = newName;
            health = newHealth;
        }

        void func heal(int amount) {
            health += amount;
        }

        override void func bark() {
            IO.output("Uhh Bark? Im a human, sorry");
        }
    }
}



Player myPlayer = new Player("Surgeon", 100);
IO.output(cast(myPlayer.health, String));
myPlayer.heal(100);
IO.output(cast(myPlayer.health, String)); // Testing if it really increased

// Dual-Track Instantiation Matrix
// Track 1: Arguments map to the top-to-bottom physical order of class fields.
Player workerA = new Player("Surgeon", 100);

// Track 2: Arguments can use explicit field keys in any layout order.
Player workerB = new Player(health=100, name="Architect");

// Error Gate: Mixing positional and named arguments is confusing for a programmer so its banned
Player brokenWorker = new Player("Surgeon", health=100); // ArgumentError: cannot mix named and non-named arguments
// Is it a hospital!? THREE PLAYERS ARE SURGEONS!!!

IO.output(cast(workerA, String)); // Output:-
// Instance of 'Player' with name 'workerA' at [HEX_ADDRESS]
// Data: {
//     name: "Surgeon",
//     health: 100
// }
// Type: Class

// Enumerators
enum TemperatureF {
    HOT = 108;
    NORMAL = 99;
    COLD = 88;
}

IO.output(f"Ah! Its fricking {cast(TemperatureF.HOT, String)}!");
```

---
## 5. Build Integrity & The Error Engine

Geyser blocks bugs before they can ever execute on hardware by throwing descriptive compile-time errors instantly and smart explicit warnings

> Note: Also during compilation if a warning pops up it throws an e/y/N prompt to continue compilation, e simply means to display all the other remaining warnings, clicking y after e means agreeing to all warnings, if a single N prompt appears, the entire compilation is halted, just y means aggreeing on the current warning continuing to display the rest one by one

---
## 6. Organizational Project Layout

Development moves in a strictly disciplined pipeline under the command of the Chief Architect:
* **Team 1 (Compiler Thinkers):** Maps specifications to native hardware logic and handles register layouts within the C/LLVM backend.
* **Team 2 (Syntax Developers):** Builds the actual C tokenizer, lexer, and parser within CLion to read `.gy` source text and enforce compile-time error gates.
* **Team 3 (Code Breakers):** Aggressively stress-tests the system by writing broken code AND correct to find compiler exploits, racing bugs, or memory leakage flaws to report it to `Team 2`.
* **Team 4 (Launch & Media):** Manages the official website, syntax highlighters, documentation, and handles public advertisements to drive industry adoption.
* **Team 5 (OS and CPU archaeological specialists):** Handles advanced cross-platform OS layers mapping and target instructions to ensure native binary efficiency.

---
### Geyser specifications

  * Geyser is a compiled language designed for lightning fast execution.
  * Geyser is growing and being planned so geyser might replace `Python` and `C++` uses (Not duck-typing, i mean their uses (Like how Python is used for Data science)).
  * Geyser has a real use and is not your everyday **`esolang`**