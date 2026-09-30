# Coding Standards. 

These are the coding standards throughout this project.

## CPP

### General Principles

* **Write Readable Code**
    
    The code should be easy to understand and self documenting to an extent

* **Keep It Simple**

    Avoid unnecessary complexity - favour clear and maintainable solutions

* **Use meaningful names**

    choose descriptive names for variables, functions, struct and classes

### Formatting and Style

* Use **Tab** for indentation level. 

* Use **braces** even for single-line if statements or loops

* **Always** Place braces on the next line.

* Use **blank lines** to separate logical sections.

### General Rules

* **Classes and Structs**

    **Structs** should only be used as Data Containers and should not include Functions, **except** from operation overloads.

    **Classes** should only be used for logic. 

    They should use `Pascal Case` for example `struct PlayerData` and not `struct playerData`.  
    
    If they use templates they must be prefixed with `T` as `class TObject`.

    If they are Abstract they must be prefixed with `I` as `IGame`.

    Public members and functions must be first, followed by protected and finally private. 

    Use Multiple public/protected/private blocks to separate members and functios.

    Use the `default` keyword for constructor and destructor where necessary.

    Getter or Setter functions should be at the bottom of the public block.

    All non-templated functions must be implemented inside the `.cpp` file.

    Functions that return values should be marked with the `[[nodiscard]]` attribute.

* **Variables**

    Public variables must be in `Pascal Case`.

    Private variables must be prefixed with `m_`.

    Global variables must be prefixed with `g_`

    Constants should be written in upper snake case for example `MAX_SPEED`. 

    Private & Global variables must have thier type in the name: `m_bName` for bool, `m_fName` for float, `m_eName` for enum, `m_vName` for vector, `m_iName` for int, `m_mName` for map, `m_pName` for raw pointer, **Don't** use this for smart pointers, any other variable should just use `m_Name`.    

* **Functions**

    Functions should be written in pascal case. 

    Parameters inside fucntions must be snake case. 

    
* **Header Files & Includes**

    * Use `#pragma once` at the top of all `.h` files
    * Use forward declarations where possible
    * Sort includes in order of:

        1. Standard library headers using `#include <>`

        2. Third-party library headers using `#include ""`

        3. Project-specific headers using `#include ""`

    * Avoid unnecessary includes in headers and prefer forward  declaration
    * For project specific headers use the full path, from the  root as `Foo\Bar\Application.h` rather than  `Application.h`


* **Misc**

    * Prefer `using` over `typedef`

    * `auto` is okay but prefer to always use the type

    * Prefer using early outs over heavily nested code

    * Prefer C++ cast over C-style cast. 
    
    * Try to avoid the using keyword for types with new names for convienience. 
    
    * Prefer using `inline static` over adding in the type in   the `.cpp` file. 