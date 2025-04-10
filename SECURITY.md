# Security Policy
Asof 2024-12-24, all [commit signatures](https://docs.github.com/en/authentication/managing-commit-signature-verification/signing-commits) shall match [`./.ssh/sha256.sig`](./.ssh/sha256.sig) values. [./README.md#signaturecertificate](./README.md#signaturecertificate) shows how to test this on your own.

Asof 2025-04-09, [`./.ssh/allowed_signers.old`](./.ssh/allowed_signers.old) holds old certificates[^3] (not known as "compromised"; just no longer used). **TODO**; warn if new commits use old certificates.
## Supported versions
Users can expect that past 2024-06-26, [`trunk`](https://github.com/SwuduSusuwu/SubStack) passes [`susuwuUnitTests()`, which uses `Susuwu::unitTestsCxx()`](./cxx/main.cxx#L22-L82)[^1] + [_GitHub_'s code scans](https://github.com/SwuduSusuwu/SubStack/security/code-scanning?query=branch%3Atrunk+is%3Aopen+severity%3Amedium%2Clow%2Ccritical%2Cerror%2Chigh)[^2].

## Sensitive issues
First, view [How to contribute](./README.md#how-to-contribute) for information on issues (to ensure that what you found is not a normal issue).

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
[^3]: Asof 2025-04-09, [`./.ssh/sha256.sig`](./.ssh/sha256.sig) (and the [`./.ssh/allowed_signers`](./.ssh/allowed_signers) which `sha256.sig` produces) have a new certificate. **TODO**; warn if new commits use old certificates.

