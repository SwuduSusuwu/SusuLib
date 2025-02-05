(C) 2024 Swudu Susuwu, dual licenses: choose [_GPLv2_](./LICENSE_GPLv2) or [_Apache 2_](./LICENSE) (allows all uses).
# [Table of Contents](../README.md#table-of-contents)
- [Purposes](#purposes)
# Purposes
[`./hooks/`](./) is `git` scripts ([`man githooks`](https://git-scm.com/docs/githooks)) which assist you; install with `cp -ra ./hooks/* ./.git/hooks/`.
- [`./hooks/post-checkout`](./post-checkout) is custom `post-checkout` (produces `./tags`.)
- [`./hooks/post-merge`](./post-merge) is custom `post-merge` (produces `./tags`.)
- [`./hooks/pre-commit`](./pre-commit) is [custom `pre-commit`](https://git-scm.com/book/en/v2/Customizing-Git-Git-Hooks) (`git add ./tags` if tracked, + `SUSUWU_GIHUB_WORKSPACE_JSON && git add ./compile_commands.json`) + [`.git/hooks/pre-commit.sample` (scans for non-ASCII filenames, conflict markers or whitespace errors)](https://github.com/auth0/gitzero/blob/master/tests/example/_git/hooks/pre-commit.sample)
- [`./hooks/pre-rebase`](./pre-rebase) is [custom `pre-rebase`](https://git-scm.com/book/en/v2/Customizing-Git-Git-Hooks) (restores `./tags` to avoid `error: cannot rebase: You have unstaged changes.`)
- [`./hooks/prepare-commit-message`](./prepare-commit-message) is [custom `prepare-commit-msg`](https://git-scm.com/book/en/v2/Customizing-Git-Git-Hooks) (`git add ./tags` if tracked, + `SUSUWU_GIHUB_WORKSPACE_JSON && git add ./compile_commands.json`.)

[`./compile_commands.json`](./compile_commands.json) is [`clang-tidy`'s compilation database](https://clang.llvm.org/docs/JSONCompilationDatabase.html#build-system-integration). Included for tools which execute before `./build.sh` (which produces this).
[`./tags`](../tags) is [an _IntelliSense_ alternative for editors such as `vim`](https://vi.stackexchange.com/a/45044) (stores data for [autocompletion use](https://vim.fandom.com/wiki/Any_word_completion), or to [jump to declarations through tags](https://dev.to/iggredible/how-to-use-tags-in-vim-to-jump-to-definitions-quickly-2g28)). Included for systems without [`ctags`](https://github.com/universal-ctags/ctags?tab=readme-ov-file#universal-ctags) (which produces this).

