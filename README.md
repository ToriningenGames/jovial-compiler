# Freestanding JOVIAL compiler

### Current status: No

## Why?

As in, why not just use, say, GCC? While other, better, more extensible
compilers exist, I felt it would be good exercise to write a language compiler
from the ground up. I'm reading through the Dragon Book anyways; may as well
prove I'm learning.

## Why JOVIAL?

Because there are no open-source implementations.
To quote [Peter Maurer](http://cs.ecs.baylor.edu/~maurer/SieveE/jovial.htm):

> If you really want a Jovial compiler you can get one. It might be expensive
> and you might have to go through a lot of rigamarole to get it, but a current
> Jovial compiler for modern PCs does indeed exist. However, the cost of such a
> compiler is beyond the resources of this little archive. It's probably also
> beyond the resources of the average programmer. So unless you've got a
> government contract, getting a Jovial compiler is probably not in your future.

Languages not preserved in implementation are doomed to be lost.
Plus, when deciding on a language to write a compiler for, an ancient language
designed for tiny archaic systems seemed like the best choice.

## Why in C?

Familiarity. And availability of tools.
Self-hosting would also be nice, but that would require a pre-existing compiler.
Failing this, C is an ALGOL-style language, which would make porting it to
JOVIAL a (hopefully) easy-ish task.

## Requirements

This project requires a C11 compiler to compile.
Its build system is done with make. Developed on a Windows 10 platform, with GCC

## TODO

* The entirety of code generation
* The entirety of parsing
* `DEFINE`s & co.
* Make names case insensitive
* Correct lexing of a few symbols
  * `!`
  * `(*`, `*)`