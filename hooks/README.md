(C) 2024 Swudu Susuwu, dual licenses: choose [_GPLv2_](./LICENSE_GPLv2) or [_Apache 2_](./LICENSE) (allows all uses).
# [Table of Contents](../README.md#table-of-contents)
- [Purposes](#purposes)
# Purposes
[`./hooks/`](./) is `git` scripts ([`man githooks`](https://git-scm.com/docs/githooks)) which assist you; install with `cp -ra ./hooks/* ./.git/hooks/`.
- [`./hooks/pre-commit`](./pre-commit) is [`.git/hooks/pre-commit.sample` (scans for non-ASCII filenames, conflict markers or whitespace errors)](https://github.com/auth0/gitzero/blob/master/tests/example/_git/hooks/pre-commit.sample)

