**\[Preview\] How to improve how fast desktops/laptops/phones execute code**

\[[This post](https://swudususuwu.substack.com/p/howto-run-devices-phones-laptops) allows [all uses](https://creativecommons.org/licenses/by/2.0/).\] \[This post is a work-in-progress.\]

# Table of Contents
- [Intro](#intro)
- [**SIMD** (Single Instruction Multiple Data)](#simd-single-instruction-multiple-data)
- [**GPGPU**s (General Purpose Graphics Processor Unit)](#gpgpus-general-purpose-graphics-processor-units)
- [**TPU**s (Tensor Processor Unit)](#tpus-tensor-processor-units)
- [Synopsis + related posts](#synopsis--related-posts)
  - [Compiler flags (`CXXFLAGS`)](#cxxflags)

# Intro
Since around the year _2002_, the physical [**CMOS**](https://wikipedia.org/wiki/CMOS) limits of transistors have meant that [the **ghz** (gigahertz) of **CPU**'s can not improve](https://forums.tomshardware.com/threads/why-are-we-stuck-at-5-ghz-for-almost-18-years.3718547/), and thus [multicore (also known as **SMP** (_Symmetric Multiprocessing_)) / **SIMD** (_Single Instruction Multiple Data_) is required for throughput to continue to improve](https://poe.com/s/XSpIAY48coBq2rHjIuwQ).

To improve:
- Switch to **CPU**'s with more cores and new [**SIMD**](#simd-single-instruction-multiple-data) opcodes to use.
- Switch to **CPU**'s / **GPU**'s / **RAM** which uses more small transistors (lower "**nm**" values), as those have more compute units, plus those improve power use.
- For closed source program use:
  - Search for new versions of programs, which use _OpenMP_ (or which use other such tools which produce **SMP** & **SIMD** code flows).
  - Search for versions which list the newest **CPU** which is not more new than the **CPU** in use.
  - _Microsoft Windows_ has [**WOW64** to execute programs as x32 or x64 based on your current **CPU**](https://wikipedia.org/wiki/WOW64#Performance).
    - _Linux_'s equivalent is [_MultiArch_](https://help.ubuntu.com/community/MultiArch) (which [is similar to _Microsoft Windows_’ **WOW64**](https://stackoverflow.com/questions/40343790/how-does-a-64b-linux-knows-to-manage-a-32b-application).
    - If the **CPU** in use supports _64-bit_ execution, choose program versions which list "_AMD64_", "_Intel64_", "_x86_64_", "_aarch64_", or "_Arm64_"; _64-bit_ executables all use **SIMD** opcodes.

- For open source ([**FLOSS**](https://wikipedia.org/wiki/FLOSS)) programs use:
  - Recompile the source code with `--march=native` (insert into [compiler flags](#cxxflags)) to produce the newest **SIMD** code which the currrent **CPU** can use.
    - **MSVC** (_MicroSoft Visual Compiler_) [has auto-vectorization (produces executables which use **SIMD** opcodes on compatible **CPU**s)](https://devblogs.microsoft.com/cppblog/avx-512-auto-vectorization-in-msvc/).
    - **GCC** (_GNU Compiler Collection_) [also has auto-vectorization](https://gcc.gnu.org/projects/tree-ssa/vectorization.html).
    - _Clang_ / **LLVM** (_Low-Level Virtual Machine_) [also has auto-vectorization](https://llvm.org/devmtg/2014-02/slides/golin-AutoVectorizationLLVM.pdf).
  - Recompile the source code with `--openmp` (insert into [compiler flags](#cxxflags)), which enables `#pragma omp <command>` (where `<command>` is the specific subset of [_OpenMP_](https://codezup.com/boost-c-performance-openmp-case-study/) to use, such as `parallel` or `simd`).
    - Improve source code: if the source code does not have **SIMD** directives, ensure the code [is amenable (does not have inter-element dependencies of tensors)](https://stackoverflow.com/questions/70855607/how-to-properly-use-pragma-omp-simd) (remember to consider how the compiler's [_Abstract syntax tree_](https://wikipedia.org/wiki/Abstract_Syntax_Tree) differs from human-readable source code) + insert `#pragma omp simd` above amenable loops, or use [_intrinsic functions_](https://wikipedia.org/wiki/Intrinsic_function) (which allow manual insertion of **SIMD** opcodes).
      - Example: [`strchr` through **SSE2**, plus `strstr` through **SSE4.2**](https://poe.com/s/VV1Smbo3pGiEuLLxeRTp).
      - Example: [tensor transpose through **AVX2**, **AVX-512** and **AMX**](https://poe.com/s/nkPauyliePYo5Vl0avi4).
    - Improve Source code: if the source code does not have directives for multiple **CPU** core use, insert [`#pragma omp` directives which instruct the compiler to prefer multiple core use](https://www.ibm.com/docs/en/zos/3.1.0?topic=descriptions-pragma-directives-parallel-processing) above the start of loops which most suit multiple core use:
      - Loops which do not have dependencies on the previous iteration of the loop.
      - Loops total whose total iterations count can absorb (ammeliorate) thread (or process) startup costs. Unless the architecture is similar to **GPU**s (which have minimal thread startup costs), this requires iteration counts in the thousands (or more numerous).
- Disable stylistic features (such as shadows or animations) — unless your workflow has use for those (such as school classes about graphics do) — and measure (to determine if the resource use goes down or if responsiveness improves; if so, put a note to disable those in the future).

![Linaro autovectorization flow chart](https://substackcdn.com/image/fetch/$s_!Zqm6!,w_1456,c_limit,f_webp,q_auto:good,fl_progressive:steep/https%3A%2F%2Fsubstack-post-media.s3.amazonaws.com%2Fpublic%2Fimages%2Fa79087a5-f0ff-4fe9-9f09-1a3d800a9329_991x611.png)

**C++**'s "syntactic sugar" reduces source code sizes (due to classes, templates, and the **STL**, which allow more source code reuse),
plus are more abstract, which gives compilers more room to implement the source code through **SMP** or **SIMD** opcodes (_uops_):
- <https://stackoverflow.com/questions/13676172/optimization-expectations-in-the-stl>
- <https://devblogs.microsoft.com/cppblog/algorithm-optimizations-advanced-stl-part-2/>
- <https://stackoverflow.com/questions/6313730/does-const-correctness-give-the-compiler-more-room-for-optimization>


# **SIMD** ([_Single Instruction Multiple Data_](https://wikipedia.org/wiki/SIMD))
**SIMD**-compatible **CPU**s:
- All 64-bit _Intel_ / _AMD_ **CPU**s allow [**SSE2** (_Streaming SIMD Extensions 2_)](https://wikipedia.org/wiki/SSE2).
- Most _aarch64_ / _Arm64_ **CPU**s allow [**NEON**](https://wikipedia.org/wiki/ARM_architecture_family#Advanced_SIMD_(NEON)).

![SISD versus SIMD illustration](https://substackcdn.com/image/fetch/$s_!0G-J!,w_1456,c_limit,f_webp,q_auto:good,fl_progressive:steep/https%3A%2F%2Fsubstack-post-media.s3.amazonaws.com%2Fpublic%2Fimages%2F23662e32-b484-47a5-8c94-6e2a3eee19a7_1120x630.png)

**SIMD** resources:
- Common **SIMD** instruction sets include [**SSE2** (_Streaming SIMD Extensions 2_)](https://wikipedia.org/wiki/SSE2), [**SSE4.2**](https://wikipedia.org/wiki/SSE4.2) and [**AVX2** (_Advanced Vector Extensions 2_](https://wikipedia.org/wiki/Advanced_Vector_Extensions)\[[2](https://edc.intel.com/content/www/us/en/design/ipla/software-development-platforms/client/platforms/alder-lake-desktop/12th-generation-intel-core-processors-datasheet-volume-1-of-2/002/intel-advanced-vector-extensions-2-intel-avx2/)\].
- Newer _Intel64_ / _AMD64_ **CPUs** allow **AVX-512** (which is one of the newest **SIMD** instruction sets) opcodes to do operations on tensors (vectors, arrays or matrices) of 16 packed 32-bit `int`egers (or `float`s) at once (which uses just 1 microinstruction-cycle -- 1 _uop_ -- on the **CPU**). [**AVX-512** allows expansion to **AVX-1024**](https://community.intel.com/t5/Intel-ISA-Extensions/1024-bit-AVX/td-p/783641) (opcodes which compute packs of 32).
  - Newest **CPU**s allow [**AMX** (_Advanced Matrix Extensions_ ) opcodes, which compute 1024 _half-float_ operations per _uop_](https://wikipedia.org/wiki/Advanced_Matrix_Extensions#Tile_matrix_multiply_unit).
    - **AMX** use is similar to [**TPU**](#tpus-tensor-processor-units) use, but is through (new) opcodes on normal **CPU**s (opposed to dedicated **ASIC**s, which constitute **TPU**s).
    - [_Intel_ says how to use **AMX** for _TensorFlow_](https://www.intel.com/content/www/us/en/developer/articles/technical/accelerate-tensorflow-ml-performance-amx.html).
    - [_TensorFlow_'s blog says how to use **AMX** for _TensorFlow_](https://blog.tensorflow.org/2023/01/optimizing-tensorflow-for-4th-gen-intel-xeon-processors.html).
- [Comparison of how **AVX2** versus **AVX-512** versus **AMX** _intrinsic functions_  implement 4x4 tensor transpose](https://poe.com/s/nkPauyliePYo5Vl0avi4). Digital _Assistant_'s are not suitable to produce most code (such as code which involves high-level control flow or user interactions), but are suitable to produce specific compute kernels (such as tensor transpose).
- _Intel Performance Primitives_ (for _Microsoft Windows_ and _Linux_ / _Unix_) includes multiple **SIMD** versions of compute kernels, and uses uses `cpuid` [to do dynamic code dispatch to the most new instruction set compatible with the current **CPU**](https://www.reddit.com/r/simd/comments/6h9xy8/different_simd_codepaths_chosen_at_runtime_based/) (similar to **GCC**'s "function multi-versioning", but specific to _x86_ **CPU**s).
- _Solaris_ also uses `cpuid` [to choose the most performant code path (opcodes) which your **CPU** allows](https://device.report/m/69d161d7d4feb288dac686a25cf8ee8019426789f720e509abc6c64558e4edc7) and [has protocols which allow users to force use of advanced opcoodes](https://stackoverflow.com/questions/53210019/override-hwcap-2-in-mapfile-on-solaris-x86-platforms).

**GCC** / **LLVM** / _Clang_ [accept `march=native`](https://gcc.gnu.org/onlinedocs/gcc/x86-Options.html#index-march-15) to [recompile programs to use the most advanced opcodes](https://gcc.gnu.org/onlinedocs/gcc/x86-Options.html#index-march-15).
- _Linux_ / _Unix_ [allows you to recompile the whole](https://www.reddit.com/r/linux/comments/53p4uu/compiling_glibc_with_cpuspecific_optimizations/) [**OS**](https://www.reddit.com/r/linux/comments/53p4uu/compiling_glibc_with_cpuspecific_optimizations/), [to use the newest instruction set opcodes (uops) compatible with the current **CPU**](https://www.reddit.com/r/linux/comments/53p4uu/compiling_glibc_with_cpuspecific_optimizations/).
  - _ArchLinux_ was produced [to reduce the effort to recompile all packages, with custom flags](https://bbs.archlinux.org/viewtopic.php?id=48957) [(the _Linux_ ecosystem calls programs "packages")](https://bbs.archlinux.org/viewtopic.php?id=48957) to use your **CPU**’s most suitable opcodes (_uops_).
- New versions of **GCC** / **LLVM** / _Clang_ have flags to produce multiple code **SIMD** versions of compute kernels and use `cpuid` to choose the path which uses the newest opcodes (_uops_) compatible with the current **CPU**, similar to _Intel Performance Primitives_ (except not specific to _x86_ **CPU**s). **GCC**'s version of this is ["function multi-versioning"](https://lwn.net/Articles/691932/)

# **GPGPU**s (General Purpose Graphics Processor Units)
Auto-parallelization produces threaded (multicore) code (searches for code with lots of loops, distributes those loads across all local **CPU**s or **GPU**s):
- <https://wikipedia.org/wiki/Automatic_parallelization>
- <https://www.intel.com/content/www/us/en/developer/articles/technical/automatic-parallelization-with-intel-compilers.html>;  “Adding the `-Qparallel` (_Windows_\*) or `-parallel` (_Linux_\* or _macOS_\*) option to the compile command is the only action required of the programmer. However, successful parallelization is subject to certain conditions”
- <https://gcc.gnu.org/wiki/AutoParInGCC> (`gcc` or `g++`) "You can trigger it by 2 flags `-floop-parallelize-all -ftree-parallelize-loops=4`"
- <https://polly.llvm.org/docs/UsingPollyWithClang.html> “To automatically detect parallel loops and generate _OpenMP_ code for them you also need to add `-mllvm -polly-parallel -lgomp` to your `CFLAGS`. `clang -O3 -mllvm -polly -mllvm -polly-parallel -lgomp file.c`"
- <https://link.springer.com/chapter/10.1007/978-3-030-64616-5_38> "_LLVM_ Based Parallelization of **C** Programs for **GPU**"
- <https://stackoverflow.com/questions/41553533/auto-parallelization-of-simple-do-loop-memory-reference-too-complex> (distributes _Fortran_ tasks).

# **TPU**s ([Tensor Processor Units](https://wikipedia.org/wiki/Tensor_Processing_Unit))
The subset of **ASIC**s ([_Application-Specific Integrated Circuits_](https://wikipedia.org/wiki/Application-specific_integrated_circuit)) known as **TPU**s are processors which are specific to the _tensor_ workloads which the [**SIMD**](#simd-single-instruction-multiple-data) section mentions, such as **AMX**'s tensor transpose _uop_.
- Most **TPU**s were limited to specific commercial users, but new consumer computers (and some smartphones) include **TPU**s.
  - [Most smartphone **TPU**s sacrifice integer (and float) precision, to reduce die area (square inches) and power use (watts), which limits those to inference use. It is possible that future chips will use some new fused architecture which allows training (back-propagation) + inference (forward-propagation) from shared transistors.](https://poe.com/s/AQwKDX60fOyIEYVprb7h).
  - [_Google_'s **Edge TPU**s can does inference through _TensorFlow_](https://wikipedia.org/wiki/Tensor_Processing_Unit#Edge_TPU). [_Coral_ says the original **Edge TPU** processes 4 quarter-precision **teraOPS** (trillion opcodes per second) with 2W power use](https://coral.ai/docs/edgetpu/benchmarks/)
  - The _Google Pixel 2_'s [_Pixel Visual Core_](https://wikipedia.org/wiki/Pixel_Visual_Core)... [is an **edgeTPU** (mobile chip which does inference through _TensorFlow_)](https://blog.google/products/pixel/pixel-visual-core-image-processing-and-machine-learning-pixel-2/)).
  - The _Google Pixel 4_'s [_Pixel Neural Core_](https://wikipedia.org/wiki/Pixel_4#Design_and_hardware)... is also an [**edgeTPU**](https://research.google/blog/introducing-the-next-generation-of-on-device-vision-models-mobilenetv3-and-mobilenetedgetpu/) (can also do inference through _TensorFlow_). [_Pixel Neural Core_ uses the original **Edge TPU**](https://wikipedia.org/wiki/Tensor_Processing_Unit#Pixel_Neural_Core)
  - The _Google Pixel 6_'s _Google Tensor_ **TPU** is an [**edgeTPU**](https://research.google/blog/improved-on-device-ml-on-pixel-6-with-neural-architecture-search/) which [_Claude-3-Haiku_ says can process 5.2 **teraFLOPS** with average 6-watt power use (if average load is 62%)](https://poe.com/s/P50pXM92KDn8YBG19uBV)
  - The [_iPhone X_'s _Apple Neural Engine_ can do 0.6 half-precision teraFLOPS. The _iPhonoe 13 Pro_'s 5th-gen _Apple Neural Engine_ (part of "_A15_") can do 15.8 (unspecified precision) teraFLOPS](https://machinelearning.apple.com/research/neural-engine-transformers), but is limited to [_Core ML_](https://www.netguru.com/blog/coreml-vs-tensorflow-lite-mobile).
    - [_Core ML_ has frontends to convert from _PyTorch_ and _TensorFlow_](https://apple.github.io/coremltools/docs-guides/).
- [_TensorFlow Lite_ can now use smartphone **TPU**s](https://research.google/blog/improved-on-device-ml-on-pixel-6-with-neural-architecture-search/). [Standard _TensorFlow_ can use desktop / laptop / server **TPU**s](https://www.tensorflow.org/guide/tpu).
  - [_TensorFlow.js_ is still limited to **CPU**s and **GPGPU**s](https://discuss.ai.google.dev/t/can-tfjs-make-use-of-the-tpu-on-a-pixel-6/31842). [New **W3C** standards are required for browsers to support **TPU**s.](https://www.w3.org/TR/webnn/#programming-model-device-selection)
  - [_Assistant_ says how to use _TensorFlow_ on _Pixel 6_'s **edgeTPU**](https://poe.com/s/yVoGIRFdvLOPhknnQ9lH).

# Synopsis + related posts
_TensorFlow_'s `MapReduce` (<https://www.tensorflow.org/federated/api_docs/python/tff/backends/mapreduce>) distributes loads across clouds of **CPUs** / **GPGPUs** / **TPUs**.
The sort of **SW** (programs) which improve the most through use of `MapReduce` is artificial neural tissue (which can distribute execution through billions of processes), such as:
- [`./posts/ArduinoElegooTools.md`](./ArduinoElegooTools.md)
- [`./posts/VirusAnalysis.md`](./VirusAnalysis.md)
- [`./posts/CnsCompress.md`](./CnsCompress.md)
- [How to produce general-use autonomous tools through calculus (continuous formulas), + use _TensorFlow_ for synthesis of close-to-human consciousness.](https://github.com/SwuduSusuwu/SusuPosts/blob/preview/posts/Autonomous-tools_+_human-consciousness.md)
- [`./posts/AlbatrossCNS.md`](./AlbatrossCNS.md)
- [Destructive (scan uses nanoscopic decomposition) upload of human's consciousness](https://swudususuwu.substack.com/p/destructive-unreversible-upload-of)

******************************************

Unknown sources (can not discern truth (fitness-to-use) of those), which have to do with neuromorphic (bio-inspired [**TPU**](#tpus-tensor-processor-units)) computers:
* [International Centre for Neuromorphic Systems](https://www.westernsydney.edu.au/icns)
* [Neuromorphic Cognitive Systems | Institute of Neuroinformatics](https://www.ini.uzh.ch/en/research/groups/ncs.html)
* [Neuromorphic systems | NIST](https://www.nist.gov/programs-projects/neuromorphic-systems)

******************************************

## `CXXFLAGS`
* Some **IDE**s (*Integrated Development Environment*s, such as *Microsoft Visual Studio*) have custom menus (which the user stores compiler flag values into); those values are then sent to the compiler which produces executables.
* Most *Unix* tools use *Environment Variables* (*envvars*) such as `CXXFLAGS` to store compiler flag values (values such as `-O2`).
  * Most console tools (regardless of **OS**) which do not use *envvars*, use config files (such as `CMakeLists.txt:CMAKE_CXX_FLAGS`) to store compiler flag values.

