# How to keygen diablo2oo2's crackme #1

**No binary builds. Compile directly from source**

![Imgur](https://i.imgur.com/gjoeDOx.png)

**Meant to be a a simple tutorial in debugging with x64dbg and writing code in Rust**


Load up the crackme in x64dbg.

![Imgur](https://i.imgur.com/aj7F3IT.png)

Set breakpoints on functions like **GetDlgItemText**. This is needed so the debugger stops at the right moment when checking text input.

![Imgur](https://i.imgur.com/dZ87HFz.png)

Press the **Run** button so then the debugger steps at the right spot.

![Imgur](https://i.imgur.com/NnOn0N3.png)

Press **Step over** to step through the code in the crackme's execution. The code will first check if the entered name is less than 5 and greater than 32 characters. If the entered name meets either of those conditions a error message will show.

![Imgur](https://i.imgur.com/RQ9Cp3N.png)

The following screenshot shows the first part of the serial algorithm.

![Imgur](https://i.imgur.com/rl3N4Bk.png)

The code computes a buffer which is used for the final stage of the serial check. Basic arithmetic is used to compute the buffer as explained by the comments by the debugged code.

![Imgur](https://i.imgur.com/A8fNQqc.png)

The final checks are computed by this code. Using the computed buffer from earlier as a guide, some basic arithmetic is used to check against the following code. If each portion of the serial entered matches what was computed by this code, the serial is considered valid.

The code implemented in the keygen reimplements the x86 assembly code in Rust and also has modifications to the algorithm to generate portions as needed to fit the criteria used in the last section of the crackme.
