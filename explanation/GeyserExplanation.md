# The Geyser Programming Language (.gy)

> *(Important copyright notes: This project incorporates components from LLVM (targets/x86_64-windows/...), licensed under the Apache License 2.0 with LLVM Exceptions. See targets/x86_64-windows/include/llvm/Support/LICENSE.TXT for full license text. Original source: https://llvm.org)*

**Core Vision:** A strict, explicit, highly predictable, and blistering fast alternative to C++.

Geyser is a systems programming language engineered from the ground up for high-performance software, real-time engines, robotics, and advanced simulators (like *Fortnite*, *Cyberpunk 2077*). By dropping 40 years of legacy backward-compatibility baggage and utilizing a native, highly optimized C-powered compiler backend linked with LLVM-22, Geyser achieves maximum hardware execution speed, In this following code examples i will be using x86_64-windows. Currently this is how the main Geyser package is currently now:

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
        include /
            tokens.h
        lib /
            3party /
                empty
            geyser /
                empty
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
Geyser does this by fetching from the net to download the modules via:

```bash
gpm install vulkan # GPM means Geyser Package Manager
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
Sometimes you don't want the old geyser and wan't the latest geyser in the repo, to do that, run:
```bash
geyser update
```

This askes to close the window, runs a script to execute after 2 seconds to make sure the window is closed, rm -rf's the root geyser-...-version, downloads the latest version in the repo, unzips it in the background, and voila! done

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
  > *Note, If you want to add multiple string literal changers, you need to use comma's(Eg: (r, iq"\" 'e' """))

```java
// Sane Modifier Rule: You can combine (f, iq) or (iq, r), but mixing formatting and raw text behavior is illegal.
String invalidCombination = (f, r"Path: {user}"); // FormatError: combination of 'f' and 'r' is invalid
```

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
* Constant top-level objects stay active in the root file scope until the execution terminates, at which point the Operating System reclaims the entire layout at EOF (End of File).

### Introducing 'variable deletion'
We are introducing manual 'variable deletion' ability, the use of it is to help clearing data to prevent a memory leak, it clears the **pointer** and **data** basically everything, it is used via 'name.delete()'
Here is an code example

```java
// Compile-Time Eviction (Option C)
// When .delete() is called, the variable name is scrubbed from the compiler's symbol table.
// Any attempt to use it again in the same scope results in an immediate build error.
int x = 10;
x.delete();
System.print(cast(x, String)); // NameError: undefined name 'x'

// Alias tracking behavior
Player p1 = new Player("Surgeon", 100);
Player p2 = p1; // p2 is strictly a memory alias (pointer reference) to p1
p1.delete();
p2.heal(50); // NameError: no such function 'heal' in p2
System.print(cast(p2, String)); // NameError: undefined name 'p2'
```

### Hardware cache-ing
It is automated by the CPU, but with the linear-data ram-data arrangement, we can make it think "The program might need the next data on next round, lets keep it in the cache" and successfully make it cache the things we want

---
## 3. Core Data Types

Variables are raw physical memory slots. They never compile into heavy, tracking object layers or dynamic wrappers. Once assigned, a variable remains that type permanently, variables are arranged in a linear-array commonly first in the RAM default, so the CPU can assume that the program might need the next few of the slots and keep it in the fast cache-line.

* `unsigned` — Makes the target binary value unsigned
* `int` — Flat whole number hardware blocks. By default, its signed(Optional: If you want to be more precise, you can join a bit's number to the 'int' prefix(e.g: unsigned int32), if you cause a Overflow/Underflow, we dont care, you asked for it).
* `decimal` — High-precision fractional numbers for physics and fluid simulations(The same 'number' rule is applied but by 'decnumber'(e.g: 'dec32')).
* `String` — Strict, flat text character sequences.
* `boolean` — Evaluation literals (`true` or `false`).
* `const` modifier — Makes a variable permanently immutable after initialization.

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
Dict profile = {
    "name": "Alex",
    "userid": 10452 // Loose trailing ',' are optimized and removed at compile time IF its the final parameter
};

// Printing the value with the name
System.print(cast(profile["name"], String));
```

### Comments
```java
// This is a comment, it does nothing
// Its only for notes and helper identifier
// An example is
int i = 0; // Index
// Thats it! Comments does nothing but helps the programmer, it automatically gets stripped out during compile time
```

### Time sleeps
```java
import geyser.lang.Time;
Time.wait(1, unit="second");
```

### Memory addresses
```java
import geyser.lang.System;
int x = 10;
int y = 20;
pointer ptrX = addressOf(x); // e.g: 0x1000
pointer manualPtrX = 0x1000;
ptrX = addressOf(y); // Changes the memory address to 'y'
valueOf(manualPtrX) = 30; // Changes the value of the address to 30
System.print(f"X: {cast(x, String)} | Y: {cast(y, String)}");
```

### Multi-Threading
```java
import geyser.lang.Threading;
import geyser.lang.Time;
import geyser.lang.System;
void func calculatePizzaArrival() {
    for (int i = 0; i <= 3; i += 1) {
        Time.wait(1, unit="second");
    }
    System.print("Calculated pizza arrival time: 5022 seconds");
}
void func calculateEatingTime() {
    for (int i = 0; i <= 3; i += 1) {
        Time.wait(1, unit="second");
    }
    System.print("Calculated dinner time: 3544 seconds");
}

// If two threads try to modify an address at the exact same time, who was first is allowed to modify, the second has to wait
// If a thread encounters a error, it gets immeadly destroyed
Thread workerA = Threading.newThread(task=calculatePizzaArrival, daemon=true);
workerA.startThread();
Time.wait(2, unit="second");
workerA.stopThread(); // Pauses the thread to be started again
Time.sleep(10);
workerA.killThread(); // Kills the thread cleaing it up
System.exitWithReturnCode(0);
```

### System exits
```java
import geyser.lang.System;
System.exitWithReturnCode(1); // 1: Problem, 0: Success, -1: User interrupt, these doesn't matter, just a fun thing to remember
```

### Powerful binary tools, math, and value type prefixes
```java
int xor = 10 ^ 9; // OUTPUT: 3
int amp = 10 & 9; // OUTPUT: 8
int pip = 10 | 9; // OUTPUT: 11
int tid = ~10; // OUTPUT: -11
int mod = 10 % 9; // OUTPUT: 1
dec64 sum = (10 + 10) - 9 + 8 * 7 / 6 + (5 ** 4) / 3 * ~2 + 1; // OUTPUT: -601.8333
hex hexadecimal_num = 0xFF;
bin binary_num = 0b11111111;
```

### File IO's
```java
import geyser.lang.file;
import geyser.lang.System;
file.make("main.txt"); // Vague names also work also takes some time
file.write((f, iq"import geyser.lang.System;System.print("Hi");"), "main.txt") // Invalid file paths are stopped with an "FileError: no such file or directory 'path'"
System.print(cast(file.read("main.txt", String)));
file.delete("C:/User/Dell/main.txt"); // Explicit paths also work
```

### Reading other .gy files gossips and secrets
```java
import c.Users.Dell.main; // its a GY file, for paths, if the cat starts with a root drive name, it automatically starts from it, else defaults to the root ~ on linux, . on windows
main.Player myPlayer = new Player("Surgeon", 100); // ANOTHER SURGEON???
myPlayer.heal(100);
```


### Introducing GUI
Introducing GUI, we added a native Vulkan tool, same Geyser syntax, but original Vulkan personality, its like 'Vulkan syntax to Geyser syntax', exact same strucutre and logic, just different syntax. So devs still need to struggle by the way, its just for manualism so dves can make stunning games of their own.

```java
import geyser.gui.Vulkan;
// Vulkan code here..., Since vulkan is already widely known and in here the syntax is just 'geyser-fied', visit https://vulkan.org/learn
```

### Code Formatting
```java
// Semicolons at the end of a statement are strictly mandatory
import geyser.lang.System;
import geyser.lang.Prompt;
import geyser.lang.string.concatenate;

// Global prefixes are unnesecary to prevent damn pain-in-the-ahh 'restricted area' errors
String gameTitle = "DocItOut";

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
2. An existing, declared variable name cannot be re-declared.
3. Modification of an existing slot must use explicit compound mutation operators (`+=`, `-=`, `*=`, `/=` etc.), reassignment without declaring type again or reassignment with math operators etc..
4. Slot type must match the value else (TypeError: mismatched types between slot type and value)
5. Truncation in values are gurranted to not happen unless explicitly told to do so

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
Conditions inside `if` statements require explicit true/false comparison operators. Implicit shortcut evaluations are illegal. Semicolons at the exact end of a third instruction inside `for` loop headers are redundant.

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
// Compiler quietly optimizes the redundant semicolons and other semicolons to just a single raw machine code
for (int i = 0; i < 100; i += 1;) {
    System.print(cast(i, String);
}
```

### Meet randomness and cryptography
```java
import geyser.lang.Random;
import geyser.lang.CryptoHash;
import geyser.lang.System;

// Testing all types of randomness in geyser
// 1. randomNum: randomly generates an number based on the type, returnType, from and to
// type param values: (secure, true, normal)
// returnType param values: (int, decimal, boolean)
// from param values: int: any
// to param values: int: any
int rand_num = Random.randomNum(type="secure", returnType="int", from=1, to=2);

// 2. randomChoice: randomly guesses a choice on the given list
// choice param values: (int, String, boolean, decimal): any
String generateRandomChoice() { 
    return Random.randomChoice(choice=("Yes", "No"));
}

// 3. randomHex/randomBin: randomly seeds a hex/bin based on the length by CSPRNG
// length param values: int: any
hex hex_salad = Random.randomHex(length=12);
bin bin_salad = Random.randomBin(length=24);

// 4. encrypt/hash: encrypts/hashes with the following algorithm
// algorithm param values: encrypter/hasher name(lowercase, e.g: tls/sha256)
// value param values: any: any
hex encrypted_string = CryptoHash.encrypt(algorithm="tls", value="TOP SECRET!!!");
hex hashed_string = CryptoHash.hash(algorithm="sha128", value="HYPER SECRET!!!");
```

### Collections, Arrays, Matrixes & Objects
```java
// Lists use flags to ease human pain and add more features while still keeping the code blazing fast, lists can hold any data type
List<mutable, resizable> inventory = ["Apple", "Banana"];

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
    [
        [
            [
                [1, 1, 1],
                [1, 1, 1],
                [1, 1, 1],
            ]
            [
                [1, 1, 1],
                [1, 1, 1],
                [1, 1, 1],
            ]
            [
                [1, 1, 1],
                [1, 1, 1],
                [1, 1, 1],
            ]
        ]
        [
            [
                [1, 1, 1],
                [1, 1, 1],
                [1, 1, 1],
            ]
            [
                [1, 1, 1],
                [1, 1, 1],
                [1, 1, 1],
            ]
            [
                [1, 1, 1],
                [1, 1, 1],
                [1, 1, 1],
            ]
        ]
        [
            [
                [1, 1, 1],
                [1, 1, 1],
                [1, 1, 1],
            ]
            [
                [1, 1, 1],
                [1, 1, 1],
                [1, 1, 1],
            ]
            [
                [1, 1, 1],
                [1, 1, 1],
                [1, 1, 1],
            ]
        ]
    ]
    [
        [
            [
                [1, 1, 1],
                [1, 1, 1],
                [1, 1, 1],
            ]
            [
                [1, 1, 1],
                [1, 1, 1],
                [1, 1, 1],
            ]
            [
                [1, 1, 1],
                [1, 1, 1],
                [1, 1, 1],
            ]
        ]
        [
            [
                [1, 1, 1],
                [1, 1, 1],
                [1, 1, 1],
            ]
            [
                [1, 1, 1],
                [1, 1, 1],
                [1, 1, 1],
            ]
            [
                [1, 1, 1],
                [1, 1, 1],
                [1, 1, 1],
            ]
        ]
        [
            [
                [1, 1, 1],
                [1, 1, 1],
                [1, 1, 1],
            ]
            [
                [1, 1, 1],
                [1, 1, 1],
                [1, 1, 1],
            ]
            [
                [1, 1, 1],
                [1, 1, 1],
                [1, 1, 1],
            ]
        ]
    ]
    [
        [
            [
                [1, 1, 1],
                [1, 1, 1],
                [1, 1, 1],
            ]
            [
                [1, 1, 1],
                [1, 1, 1],
                [1, 1, 1],
            ]
            [
                [1, 1, 1],
                [1, 1, 1],
                [1, 1, 1],
            ]
        ]
        [
            [
                [1, 1, 1],
                [1, 1, 1],
                [1, 1, 1],
            ]
            [
                [1, 1, 1],
                [1, 1, 1],
                [1, 1, 1],
            ]
            [
                [1, 1, 1],
                [1, 1, 1],
                [1, 1, 1],
            ]
        ]
        [
            [
                [1, 1, 1],
                [1, 1, 1],
                [1, 1, 1],
            ]
            [
                [1, 1, 1],
                [1, 1, 1],
                [1, 1, 1],
            ]
            [
                [1, 1, 1],
                [1, 1, 1],
                [1, 1, 1],
            ]
        ]
    ]
];

// Functions in geyser: void, int, String, decimal, boolean, basically anything, Period
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
    func collosal() {
        int Haha = 10;
        return Haha
    }
    return -1
}

class NestedNightmare {
    class NestedChild {
        int data = nightmare.collosal();
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
System.print(cast(myPlayer.health, String));
myPlayer.heal(100);
System.print(cast(myPlayer.health, String)); // Testing if it really increased

// Dual-Track Instantiation Matrix
// Track 1: Arguments map to the top-to-bottom physical order of class fields.
Player workerA = new Player("Surgeon", 100);

// Track 2: Arguments can use explicit field keys in any layout order.
Player workerB = new Player(health=100, name="Architect");

// Error Gate: Mixing positional and named arguments is confuzing for a programmer so its banned
Player brokenWorker = new Player("Surgeon", health=100); // ArgumentError: cannot mix named and non-named arguments
// Is it a hospital!? THREE PLAYERS ARE SURGEONS!!!

System.print(cast(workerA, String)) // Output:-
// Instance of 'Player' with name 'workerA' at (address)
// Data: {
//     name: "Surgeon",
//     health: 100
// }
// Type: Class
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