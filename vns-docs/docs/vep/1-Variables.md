# VEP 1 - Variables
![Static Badge](https://img.shields.io/badge/vns-2.2-blue?style=for-the-badge) ![Static Badge](https://img.shields.io/badge/Status-Final-green?style=for-the-badge "Accepted and implementation complete, or no longer active")

[TOC]

## Abstract:

This VEP proposes a standardized system for variable creation and management within the Visual Novel Script (VNS). By defining clear rules for local, global, and persistent variables, developers can streamline their code and facilitate future feature implementation within VNS.

## Rationale and Goals:
Variables are fundamental components of any programming language, enabling developers to store and manipulate data. The primary objective of this proposal is to establish a unified approach to variable handling in VNS. This will not only enhance code readability and maintainability but also pave the way for implementing advanced features like conditional statements and explicit branching.

## Definition:
In VNS, variables will be categorized into three types:
- **Local Variables**: These variables are confined to specific sections and are not shared between sections. Each section can have its own set of local variables, allowing for encapsulation and modularization of code. However, developers must implement a robust system for persisting local variables if progress saving is enabled.
- **Global Variables**: Global variables persist across sections within a particular dialogue. They are visible and accessible throughout the entire dialogue, facilitating data sharing between sections. Global variables should be carefully managed, especially when implementing progress-saving functionality.
- **Persistent Variables**: Persistent variables transcend individual dialogues and persist across sessions or even across the entire program/system. These variables require explicit management and should be saved persistently, typically on the client's device.

## Syntax:
Variables in VNS can store signed numbers (floats, ints), boolean, or strings. The syntax for creating and updating variables is as follows:

    // Local Variable Syntax:
    variable_name = 1
    does_variable_exist = false
    // Global Variable Syntax:
    @variable_name = -3.1415926 // (Global variables must start with '@')
    // Persistent Variable Syntax:
    &variable_name = "Hello world" // (Persistent variables must start with '&')

## Optional Syntax:
Additional syntax options are provided for convenience and flexibility:

    variable_name ++ // Increment/Decrement: Use ++ and -- to increment or decrement number variables.
    variable_name += 2  // Comment Number Operations
    variable1 = variable2  // Variable Assignment
    variable1 = variable2 + 21 // Complex Math Operations

## Rejected Idea(s):
### Persistent variables should start with '%'
When considering the proposal that persistent variables should start with the symbol '%', it's important to acknowledge the conflict this introduces with established conventions in many programming languages. The '%' symbol is widely recognized as the modulo operator, a fundamental mathematical operator used to find the remainder of a division between two numbers. This operator is a staple in programming languages such as Python, Java, C++, and many others, serving as an essential tool for various calculations and logic operations.

Introducing '%' as a prefix for persistent variables could lead to confusion and readability issues in code. Developers accustomed to seeing '%' used in the context of arithmetic operations might misinterpret its purpose when used as a variable prefix. This could hinder the understanding of code, especially for beginners or those working in a collaborative environment where clear and intuitive code is crucial.

Furthermore, repurposing common symbols can complicate the parsing and interpretation of code by compilers and interpreters. These tools are designed to recognize specific patterns and symbols with predefined meanings. Introducing a dual role for '%' could require significant adjustments in the parsing logic, potentially leading to increased complexity and a higher likelihood of bugs in language processors.

## Conclusion:
By adopting this standardized variable naming system and syntax, developers can improve code organization, readability, and maintainability in VNS projects. This proposal also lays the foundation for future enhancements and feature implementations within VNS.

## VEP Development Process
A live draft for this VEP lives on GitHub. There is also an issue tracker, where much of the technical discussion takes place.
The draft on GitHub is updated regularly in small increments. The official VEPS repo is (usually) only updated when a new draft is posted to the Linpg Foundation.

## Copyright
This document has been placed in the public domain.