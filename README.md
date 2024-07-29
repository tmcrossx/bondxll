# Excel xll add-in for bondlib

## Naming conventions.

Functions returning an C++ object handle start with a backslash (`\\`).
The corresponding function without the backslash returns the contents of the object.

## Enumerated types

We use `XLL_CONST(name, type, value, help, category, topic)` to create enumerated types.
To get the corresponding value, call `=name()` in Excel.
The function `ENUM("name")` also returns the value.