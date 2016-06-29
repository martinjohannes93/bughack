#Easy Stackoverflow

#About
Stackoverflow are dangereous and are often the result of copy arguments to variables without checking the size of them.

#Assignment
Please compile the source code with the following commands:<br />
gcc -o affected stackOverflow_affected.c<br />

Then start the gdb or other analysis tools to find the bug, e.g.:<br />
gdb -q affected

After that, copy the source code and correct it<br />
cp stackOverflow_affected.c stackOverflow_myBugfix_MyName<br />
nano stackOverflow_myBugfix_MyName.c<br />

Than verify your bugfix through tests. You can also use tools for it, if you know some.

#Important
Don´t lock at the source code. Only if you have problems to find the bug or to find a solution, you can look at them.<br />
Please don´t overwrite the solution_example.c
