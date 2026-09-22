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
