(C) 2024 Swudu Susuwu, dual licenses: choose [_GPLv2_](./LICENSE_GPLv2) or [_Apache 2_](./LICENSE) (allows all uses).
# [Table of contents](../README.md#table-of-contents)
- [Purposes](#purposes)
- [Progress](#progress)
- [Issue/workaround](#issueworkaround)
- [Progress](#progress)
## Purposes
[`./c/`](./c/) is [_C_](https://wikipedia.org/wiki/C_(programming_language)) source code (produced for [`./posts/`](./posts/) and for general use):
- [`./c/rfc6234/`](./c/rfc6234) is [vendored code](./.gitattributes) (direct from the official [_RFC6234_](https://www.rfc-editor.org/rfc/rfc6234#section-8)), which is used for {`classSha128()`, `classSha256()`, `classSha512()`}.
- TODO: [issue #3 produce analogous _C_ versions of source code](https://github.com/SwuduSusuwu/SusuLib/issues/3) which you can [contribute to](#how-to-contribute), or can request that more resources go to this issue

## Issue/workaround
If your project can not use [_C++_](../cxx/) includes,

and you cannot wait for [issue #3](https://github.com/SwuduSusuwu/SusuLib/issues/3) ([_C_](https://wikipedia.org/wiki/C_(programming_language)) port) to close (or [contribute to this](https://github.com/SwuduSusuwu/SusuLib?tab=readme-ov-file#How-to-contribute)),

you have numerous possible tools to use:
- [`cfront`](https://wikipedia.org/wiki/Cfront) (`git clone https://github.com/farisawan-2000/cfront-3.git`)
  - _Cfront_ may fail to cross-compile some _C++_-specific tools which require `libstdc++.so` (such as `std::exception`, _Standard Template Library_) so suggest to use _LLVM-Julia_.
- [_Comeau C/C++_](https://wikipedia.org/wiki/Comeau_C/C++) is the new `cfront` (supports `libstdc++.so` tools, but is not _FLOSS_.)
- _LLVM-Julia_ (is [_FLOSS_](https://wikipedia.org/wiki/FLOSS), `git clone https://github.com/JuliaHubOSS/llvm-cbe.git`) to cross-compile this to _C_ (to allow to import).
  - [Stock _LLVM_ removed `llvm-cbe` (which has `-march=c`, the "C-backend")](https://stackoverflow.com/questions/31960290/using-the-llvm-linker-to-produce-c-code), but _LLVM-Julia_ has this.

Chose to use _C++_ because:
- _C++_'s "syntactic sugar" reduces code sizes (due to classes, templates, and the _Standard Template Library_),
plus, this abstract syntax allows [more room for compiler optimizations of sources](https://swudususuwu.substack.com/p/howto-run-devices-phones-laptops).

Most languages allow you to include&use _C++_ functions.
- If yours does not, and `LLVM-Julia` is not good enough for you,
respond to [this issue](https://github.com/SwuduSusuwu/SusuLib/issues/3) as to how a _C_ version would assist you (will produce if enough ask for this).
## Progress
To replace _C++_'s _Standard Template Library_, [`../cxx/ClassObject.hxx`](../cxx/ClassObject.hxx) ports [_Java_](https://wikipedia.org/wiki/Java)'s [`java.lang.instrument.Instrumentation`](https://docs.oracle.com/javase/8/docs/api/java/lang/instrument/Instrumentation.html) to `class Instrumentation`, [`java.lang.Class`](https://docs.oracle.com/javase/8/docs/api/java/lang/Class.html) to `class Class : public Instrumentation`, [`java.lang.Object`](https://docs.oracle.com/javase/8/docs/api/java/lang/Object.html) to `class Object : Public Class`.
- It is simple to port those to _C_, as [all this project depends on is _C++98_'s {`throw`, single inheritance}, _Standard Template Library_, and _C++11_'s {`auto`, `class { bool defaultMemberInit = true; };`, `decltype`, `for(value: list) {}`}](../README.md#how-to-use-this)
  - Most functions which use `throw` have alternatives which `std::nothrow` (to allow `extern "C" {`).

