# Sengine Coding Standards

## General Principles

* **Write Readable Code**
    
    The code should be easy to understand

* **Keep It Simple**

    Avoid unnecessary complexity - favour clear and maintainable solutions

* **Use meaningful names**

    choose descriptive names for variables, functions, struct and classes

## Naming Convections

## Naming Conventions
| Type            | Naming Convention | Example |
|----------------|------------------|---------|
| Variables      | `PascalCase`                                  | `PlayerHealth`|
| Constants      | `UPPER_SNAKE_CASE`                            | `MAX_SPEED` |
| Functions      | `PascalCase`                                  | `UpdatePhysics()` |
| Classes        | `PascalCase`                                  | `PhysicsEngine` |
| Structs        | `PascalCase`                                  | `PlayerStats` |
| Enums          | `PascalCase` prefix with `E`                  | `enum class EGameState { Running, Paused };` |
| Member Vars    | `PascalCase` Prefix with `m_`                 | `m_health` |
| Global Vars    | `PascalCase` Prefix with `g_` (use sparingly) | `g_windowManager` |
| Namespace      | `PascalCase`                                  | `namespace Sengine {}` |

### Additional Rules

Variables that use float must written as `m_Foo = 10.0f` instead of `m_Foo = 10`

Variables that use a pointer must be prefix with a **p** as `m_pApplicationInstance`

Template classes or structs must be prefix with Template

Abstract classes must be prefix with I as `ISengineApp`

## Formatting and Style

* Use **Spaces** per indentation level
* Use **braces** even for single-line if statements or loops
* **Always** Place braces on the next line [Allman Style](https://en.wikipedia.org/wiki/Indentation_style#Allman_style)
* Use **blank lines** to separate logical sections

### Class Formatting 

* Public members must be first, followed by protected and finally private. 
* Use Multiple public/protected/private blocks to separate members and functios
* Use the `default` keyword for constructor and destructor where necessary
* All implementation logic must be in a `.cpp` file
* Functions that return values should be marked with the `[[nodiscard]]` attribute, as well as const.

* Getter or Setter functions should be at the bottom of the public block
* All non-templated functions must be implemented inside the `.cpp` file

### Correct Example

```cpp

    //Player.h

    class Player
    {
    public:
        Player() = default;
        ~Player() = default;

        void Tick();

        [[nodiscard]] float GetPlayerHealth() const;
    private:
        [[nodiscard]] bool HasPlayerDied() const;
    private:
        float m_PlayerHealth = 100.0f;
    };

    //Player.cpp
    
    Player::Tick()
    {
        if(HasPlayerDied())
        {
            GameOver();
        }
        else
        {
            Move();
        }
    }

    Player::GetPlayerHealth() const
    {
        return m_PlayerHealth;
    }

    Player::HasPlayerDied() const
    {
        return m_PlayerHealth <= 0;
    }

```

### Bad Example

```cpp
    class Player()
    {

        float m_PlayerHealth = 10;

    public:
        void Tick()
        {
            if(m_PlayerHealth > 0) Move();
            else GameOver();
        }
    }

    
```

## Header Files & Includes

* Use `#pragma once` at the top of all `.h` files
* Use forward declarations where possible
* Sort includes in order:
    
    1. Standard library headers using `#include <>`

    2. Third-party library headers using `#include ""`

    3. Project-specific headers using `#include ""`

* Avoid unnecessary includes in headers
* Use a comment at the top of each file to explain what the file is. 

### Example

```cpp
    /**
     *	Filename: Application.h
     *	Date Created: 21/02/25
     *	Purpose: The main application class, handles the functionality from int main() to when the application is closed. 
     ***/
    #pragma once

    #include <string>

    #include "entt/entt.hpp"

    class player;
```

## Additional Notes

### Pointers

Use smart pointers type over raw pointers.

## Strings

If a function must return an empty string use the `EmptyString` static variable inside Sengine. 

## Magic Numbers

**AVOID THEM** prefer to use named contanst than hard coded values. 

## Constants 

Use `const` and `constexpr` where ever possible.

## Comments

Frequently used function should use Doxygen-style comments

```cpp
/**
 *  @brief Returns the player's current health.
 *  @return Player's health as a float.
 */
[[nodiscard]] float GetPlayerHealth() const;
```

## Error Handling

Prefer to use `SE_ASSERT` macro over `try-catch`. 

If `try-catch` is used, log using Sengine built in logger

```cpp
    try
    {
    	std::ifstream file(path);
    	Json json;
    	file >> json;

    	object.DeserializeData(json);
    }
    catch (const std::exception& e)
    {
    	const std::string errorMessage = e.what();
    	Logger::Log("Cannot Deserialize Json file " +  errorMessage, ELogType::Error, ELogAreaType::Engine);
    }
```

## General Guidance

* Prefer `using` over `typedef`

* `auto` is okay but prefer to always use the type

* Prefer using early outs over heavily nested code

### Correct

```cpp
void Foo()
{
    ...

    if( x )
    {
        return;
    }

    ...

    if( y )
    {
        return;
    }
}
```
### Incorrect
```cpp
void Foo()
{
    ...

    if( !x )
    {
        ...

        if( !y )
        {
            ...
        }
    }
}
```

* Prefer C++ cast over C-style cast. 

* Try to avoid using types with new names for convienience. 
