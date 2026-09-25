# REPORT.md

## Part 2: Multi-file Project using Make Utility

### Q1: Explain the linking rule bin/client: obj/main.o obj/mystrfunctions.o obj/myfilefunctions.o. How does it differ from a Makefile rule that links against a library?

This rule tells Make that the target bin/client depends on three object files. When all three .o files exist and are up to date, Make runs the linking command, which passes all three object files directly to gcc and produces the final executable. Every function's actual compiled code lives inside these .o files, and the linker combines them all into one binary.

A rule that links against a library looks different. Instead of listing individual .o files, it lists a single library file as a dependency and uses -L (library path) and -l (library name) flags, for example:

bin/client_static: obj/main.o lib/libmyutils.a
	gcc obj/main.o -Llib -lmyutils -o bin/client_static

Here, main.o is compiled on its own, but the string and file functions are not linked in as separate .o files. Instead, they are already bundled inside libmyutils.a, and the linker pulls in only the specific functions that main.o actually calls. This is the core difference: linking straight from object files pulls in everything unconditionally, while linking against a library lets the linker be selective about what gets included from that library.

### Q2: What is a git tag and why is it useful in a project? What is the difference between a simple tag and an annotated tag?

A git tag is a fixed marker attached to one specific commit in a project's history. Unlike branches, which keep moving forward as new commits are added, a tag stays permanently pointed at the exact commit it was created on. This makes tags useful for marking specific, meaningful points in a project, such as a stable release version, so that you or anyone else can always find and check out that exact state of the code later, even after hundreds of new commits have been made.

A simple tag is just a name pointing to a commit, nothing else. It is created with git tag tagname and carries no additional information.

An annotated tag, created with git tag -a tagname -m "message", stores extra metadata: the tagger's name, email, date, and a message describing the tag. Annotated tags are treated as full objects in git's database, and they are the recommended type for marking releases since the message gives context about what that version represents, which a simple tag cannot do.

### Q3: What is the purpose of creating a "Release" on GitHub? What is the significance of attaching binaries (like your client executable) to it?

A GitHub Release turns a git tag into a user-facing, documented version of a project. It gives the version a title, a description explaining what changed or what that version includes, and a dedicated page other people can visit to see the project's version history over time. Releases make it easy for anyone to find and download specific past versions without needing to dig through commit history or clone the entire repository.

Attaching binaries, such as the compiled bin/client executable, means an end user does not need to have a compiler, the source code, or any build tools installed to run the program. They can simply download the prebuilt binary from the release page and run it directly. This is the standard way software is distributed to non-developers, and it demonstrates the final, practical outcome of the whole build process: a runnable artifact, not just source code.


## Part 3: Creating and using Static Library

### Q1: Compare the Makefile from Part 2 and Part 3. What are the key differences in the variables and rules that enable the creation of a static library?

In Part 2, the final executable was built directly from all three object files: bin/client: obj/main.o obj/mystrfunctions.o obj/myfilefunctions.o, and the link command passed all three .o files straight to gcc.

In Part 3, a new intermediate target was introduced: lib/libmyutils.a: obj/mystrfunctions.o obj/myfilefunctions.o, with a command using ar rcs to bundle mystrfunctions.o and myfilefunctions.o into a single archive file. The final executable's rule changed to bin/client_static: obj/main.o lib/libmyutils.a, meaning it now depends on the archive instead of the two object files individually. The link command also changed from listing every .o file to using -Llib -lmyutils, telling gcc to search the lib directory and link against libmyutils.a specifically. The key difference is that Part 3 adds a library-creation step in between compiling and linking, and the final link command uses library search/link flags instead of listing object files directly.

### Q2: What is the purpose of the ar command? Why is ranlib often used immediately after it?

ar is the archiver utility used to create, modify, and extract from archive files, most commonly used to bundle multiple .o object files into a single .a static library file. It essentially packages several compiled object files together into one file that the linker can later search through.

ranlib generates or updates an index inside the archive that lists which symbols (functions and variables) are defined in which object file within the archive. This index lets the linker quickly locate the correct object file for a given symbol without having to scan through every object file in the archive sequentially. Without this index, older linkers could fail to resolve symbols correctly depending on the order objects were added. In this project, the s flag was passed directly to ar (ar rcs), which performs the same indexing that ranlib would do, making a separate ranlib call unnecessary.

### Q3: When you run nm on your client_static executable, are the symbols for functions like mystrlen present? What does this tell you about how static linking works?

Yes. Running nm bin/client_static | grep mystrlen shows mystrlen defined inside the executable with a T symbol type, meaning it is defined in the executable's own text/code section.

This confirms that static linking physically copies the actual machine code for each required function out of the library archive and embeds it directly into the final executable at link time. The resulting binary is self-contained, it does not need libmyutils.a to exist anymore in order to run, since all the code it needs was already copied in during linking. This is different from dynamic linking, where the executable would instead reference an external shared library that must be present and loaded at runtime.


## Part 4: Creating and using Dynamic Library

### Q1: What is Position-Independent Code (-fPIC) and why is it a fundamental requirement for creating shared libraries?

Position-Independent Code is machine code that can run correctly no matter where in memory it gets loaded. Normally, compiled code can contain hardcoded memory addresses for variables and functions. This works fine for a static executable, since the OS always loads it at a predictable location. But a shared library can be loaded into many different processes at once, and each process may load it at a different memory address depending on what else is already loaded there. If the library's code contained hardcoded absolute addresses, it would break in any process where it wasn't loaded at that exact original address.

-fPIC tells the compiler to generate code that uses relative addressing instead of absolute addresses, so the same compiled .so file works correctly regardless of where it ends up in a process's memory space. This is why every object file that goes into a shared library must be compiled with -fPIC, without it, the resulting .so file would not be safely shareable across different programs.

### Q2: Explain the difference in file size between your static and dynamic clients. Why does this difference exist?

In theory, a statically linked executable should be larger than a dynamically linked one, since static linking copies the actual machine code of every used library function directly into the final binary, while dynamic linking only stores a reference to the shared library and resolves the actual function code at runtime.

In this project, however, client_static and client_dynamic came out to nearly identical sizes (17K each). This is because the utility library used here (mystrfunctions.c and myfilefunctions.c) contains only a handful of small functions. The actual compiled code for these functions is tiny compared to the fixed overhead every ELF executable carries regardless of linking method, such as headers, symbol tables, and section metadata. With such a small library, that fixed overhead dominates the file size and hides the difference that static vs dynamic linking would normally cause.

If the library contained significantly more code, such as a large real-world library with hundreds of functions, the size difference between static and dynamic linking would become much more visible, since static linking would need to embed all of that additional code directly into the executable.

### Q3: What is the LD_LIBRARY_PATH environment variable? Why was it necessary to set it for your program to run, and what does this tell you about the responsibilities of the operating system's dynamic loader?

LD_LIBRARY_PATH is an environment variable that tells the dynamic loader additional directories to search when looking for shared libraries at runtime. By default, the loader only checks a fixed set of standard system directories (like /lib and /usr/lib), it has no knowledge of custom project folders like this project's lib/ directory.

When client_dynamic was run without LD_LIBRARY_PATH set, it failed with "cannot open shared object file", because the loader could not find libmyutils.so anywhere in its default search paths. Setting LD_LIBRARY_PATH to include the project's lib/ folder gave the loader an additional place to look, allowing it to locate and load the library successfully.

This demonstrates that with dynamic linking, resolving where library code actually lives is deferred until the program is run, rather than being permanently baked into the executable at compile time like static linking does. The operating system's dynamic loader is responsible for finding, loading, and linking shared libraries into a process's memory at startup (or even later, for libraries loaded on demand), a responsibility that does not exist at all for statically linked programs.

## Part 5: Creating and Accessing Man Pages

### Q1: Man page content and structure

Two man pages were created under man/man3/: mystrfunctions.3 documenting mystrlen, mystrcpy, mystrncpy, and mystrcat, and myfilefunctions.3 documenting wordCount and mygrep. Each page follows standard groff conventions with a .TH title header, .SH NAME, .SH SYNOPSIS, .SH DESCRIPTION, and .SH AUTHOR sections, and was previewed locally using man -l before installation to confirm correct formatting.

### Q2: Makefile install target

The install target was added to the project's Makefile. It copies the compiled client_dynamic executable to /usr/local/bin/client so it can be run from anywhere on the system, copies libmyutils.so to /usr/local/lib/ and runs ldconfig so the dynamic loader can find it without needing LD_LIBRARY_PATH, and copies both man3 pages into /usr/local/share/man/man3/, followed by mandb to rebuild the man page index so they become accessible immediately.

### Q3: Git workflow

A separate man-pages branch was created from main. All man page files and the modified Makefile were committed to this branch, and it will be merged back into main in the final submission step, along with a v0.4.1-final tag and corresponding GitHub Release.
