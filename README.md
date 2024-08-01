# bondxll

Excel add-in for [bondlib](https://github.com/tmcrossx/bondlib/).

C++ objects are made available in Excel via _handles_.
A handle is a pointer to the underlying C++ object
and makes object lookup very efficient.
The funny looking number in Excel has the same
bits as the pointer to the C++ object.

All functions that create C++ objects start with a backslash (`\\`).
The corresponding function without the backslash returns the
data used to create the object.

## Ctrl-Shift-A

After typing `=` and the name of a function, and optionally using `<Tab>` 
to complete the name, then pressing `Ctrl-Shift-A`
will produce the names of the arguments of the function.

I don't have access to the source code of Excel, but there is a
simple way to extend this functionality. Instead of pressing
`Ctrl-Shift-A` you can press `<Backspace>` to remove the
trailing left parenthesis and then press `<Enter>`.
You will see another funny looking number, but it is not a handle. 
It is the [register id](https://learn.microsoft.com/en-us/office/client-developer/excel/xlfregisterid)
Excel uses to keep track
user defined functions.

With your cursor in the cell, pressing `Ctrl-Shift-B` will
replace the arguments you see with `Ctrl-Shift-A` with their
default values.

Pressing `Ctrl-Shift-C` will enter the default values below the cell
and provide the function corresponding to the register id to call those.
You can change the values below the cell to provide new arguments.

Pressing `Ctrl-Shift-D` will create the names you see from `Ctrl-Shift-A`
below the cell and provide the function corresponding to the register id
with those names as arguments. The function has the `Output` style
applied and the arguments have the `Input` style applied.

