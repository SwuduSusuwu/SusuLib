(C) 2024 Swudu Susuwu, dual licenses: choose [_GPLv2_](./LICENSE_GPLv2) or [_Apache 2_](./LICENSE) (allows all uses).
# [Table of contents](../README.md#table-of-contents)
- [Purposes](#purposes)
- [Issues/workarounds](#issuesworkarounds)
# Purposes
[`./sh/`](./) is [`/bin/sh`](https://wikipedia.org/wiki/Bourne_shell) scripts.
- [`./sh/Macros.sh`](./Macros.sh) is a standalone lib for common console tasks (can do most of what [ncurses](https://wikipedia.org/wiki/Ncurses) can do). `./build.sh` and `./sh/make.sh` use this.
- [`./sh/make.sh`](./make.sh) is a standalone (just imports `Macros.sh`) port of [`make`](https://wikipedia.org/wiki/Make_(software)) to `/bin/sh`. `./build.sh` uses this.

[`./build.sh`](../build.sh) does what {`./configure`, `make`} often do, can use most of [_GNU_ `make`'s options](https://maketools.com/). (View [Options/setup](../README.md#optionssetup) for options).
# Issues/workarounds
Issues:
- TODO: [produce alias (such as {`--silent`, `--quiet`} -> `-s`) groups of options/flags, for `SUSUWU_PROCESS_*` functions.](https://github.com/SwuduSusuwu/SubStack/issues/23)
- TODO: [map options/flags (which `SUSUWU_PROCESS_*` functions use) to descriptions (for `--help` output.)](https://github.com/SwuduSusuwu/SubStack/issues/24)

Workaround: [`git switch experimental`](../README.md#beta-testexperimental-builds)

