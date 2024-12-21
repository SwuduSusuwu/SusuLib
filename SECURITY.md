# Security Policy

## Supported versions
Users can expect that past 2024 June 26, [`trunk`](https://github.com/SwuduSusuwu/SubStack) passes [`susuwuUnitTests()`, which uses `Susuwu::unitTestsCxx()`](./cxx/main.cxx#L22-L82)[^1] + [_GitHub_'s code scans](https://github.com/SwuduSusuwu/SubStack/security/code-scanning?query=branch%3Atrunk+is%3Aopen+severity%3Amedium%2Clow%2Ccritical%2Cerror%2Chigh)[^2].

## Sensitive issues
First, view [How to contribute](./README.md#How-to-contribute) for information on issues (to ensure that what you found is not a normal issue).

If you found [normal issue(s), such as this](https://github.com/SwuduSusuwu/SubStack/security/code-scanning/1882), use [this normal route to post about new issues](https://github.com/SwuduSusuwu/SubStack/issues/new).

But if you found [sensitive issue(s), such as this](https://github.com/SwuduSusuwu/SubStack/security/code-scanning/1277), you have a few options to report the issue:
- through a [new private advisory](https://github.com/SwuduSusuwu/SubStack/security/advisories/new),
- through private message to [https://github.com/SwuduSusuwu](https://github.com/SwuduSusuwu) (if _GitHub_ now allows private messages),
- or [mailto:2002swudususuwu@gmail.com](mailto:2002swudususuwu@gmail.com).
- If there is no response soon, you can also contact [https://substack.com/@swudususuwu](https://substack.com/@swudususuwu).

You can expect:
- Best effort to address the issue(s),
- with you anonymous (unless you ask to publish credits to you.)

## Errata/footnotes
[^1]: Asof [commmit 7a9f52b2301f16807485b6701bec883404b4bd29](https://github.com/SwuduSusuwu/SubStack/commit/7a9f52b2301f16807485b6701bec883404b4bd29) (+[`cxx/main.hxx`](./cxx/main.hxx): for issues #3, #14: cross-language), `testHarnesses` is now `susuwuUnitTests`.
[^2]: Asof [commmit 36fa8a54a2a56d6e5bf21899980b48b462c15bde](https://github.com/SwuduSusuwu/SubStack/commit/36fa8a54a2a56d6e5bf21899980b48b462c15bde) (+[`.github/workflows/codacy.yml`](./.github/workflows/codacy.yml) New _GitHub_ analysis.), the code scans now include all of _Codacy_'s test results; before this, [just _GitHub_'s _CodeQL_ produced code scans](https://github.com/SwuduSusuwu/SubStack/security/code-scanning?query=is%3Aopen+tool%3ACodeQL+).
