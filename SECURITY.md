# Security Policy
Asof 2024-12-24, all [commit signatures](https://docs.github.com/en/authentication/managing-commit-signature-verification/signing-commits) shall match [`./.ssh/sha256.sig`](./.ssh/sha256.sig) values. [./README.md#signaturecertificate](./README.md#signaturecertificate) shows how to test this on your own.

Asof 2025-04-09 ([commit a40d1ff013f3007384e4ed025d0e402364d189cb](https://github.com/SwuduSusuwu/SusuLib/commit/a40d1ff013f3007384e4ed025d0e402364d189cb)), [`./.ssh/allowed_signers.old`](./.ssh/allowed_signers.old) holds old certificates[^3] (not known as "compromised"; just no longer used). **TODO**; warn if new commits use old certificates.

Asof 2025-07-10 ([commit 4073fc729bdf7eda455a1a9914310e118efa5833](https://github.com/SwuduSusuwu/SusuLib/commit/4073fc729bdf7eda455a1a9914310e118efa5833)), this repo switches to a new certificate[^4]. The previous certificates are not known as "compromised", but were used on numerous devices and are no longer trustable.

Asof 2025-07-26 ([commit 585a6bb7c84cbb28ff106a1d8f5236c9cbc97082](https://github.com/SwuduSusuwu/SusuLib/commit/585a6bb7c84cbb28ff106a1d8f5236c9cbc97082)), this repo switches to a new certificate[^5]. The previous certificate is not known as "compromised", but is inaccessible for now. If possible, will restore that certificate, but the backup has a 32-character password which was forgotten, so vouched for this new certificate with `SHA256:1csQw8HZNJa7t2gbG9/usNZ6cXdlUlSMcA3dVb3j16c` (the certificate which was marked "not compromised" through [commit 4073fc729bdf7eda455a1a9914310e118efa5833](https://github.com/SwuduSusuwu/SusuLib/commit/4073fc729bdf7eda455a1a9914310e118efa5833), vouches for `SHA256:G6I98GU7oMXNmKB/k10nAlZo52bN9y17xvARXjblM2g`).

[Asof 2025-10-25](https://github.com/SwuduSusuwu/SusuLib/discussions/70#discussioncomment-14782700) ([`commit` 1cf751c10ee99dc3ed84e8dc7761358545486550](https://github.com/SwuduSusuwu/SusuLib/commit/1cf751c10ee99dc3ed84e8dc7761358545486550)), <https://github.com/SwuduSusuwu/> switches to new sigs[^6]. The previous sigs are not known as "compromised", but were used on numerous devices (thus are no longer trustable).

## Supported versions
Users can expect that past 2024-06-26, [`trunk`](https://github.com/SwuduSusuwu/SusuLib) passes [`susuwuUnitTests()`, which uses `Susuwu::unitTestsCxx()`](./cxx/main.cxx#L22-L82)[^1] + [_GitHub_'s code reviews](https://github.blog/changelog/2025-04-22-github-actions-workflow-security-analysis-with-codeql-is-now-generally-available/)[^2].
- If _GitHub_ gives advisories, <https://github.com/SwuduSusuwu/SusuLib/security/> shows those (the top just shows what is in `SECURITY.md`, so remember to scroll down).

## Sensitive issues
First, view [How to contribute](./README.md#how-to-contribute) for information on issues (to ensure that what you found is not a normal issue).

If you found [normal issue(s), such as this](https://github.com/SwuduSusuwu/SusuLib/security/code-scanning/1882), use [this normal route to post about new issues](https://github.com/SwuduSusuwu/SusuLib/issues/new).

But if you found [sensitive issue(s), such as this](https://github.com/SwuduSusuwu/SusuLib/security/code-scanning/1277), you have a few options to report the issue:
- through a [new private advisory](https://github.com/SwuduSusuwu/SusuLib/security/advisories/new),
- through private message to <https://github.com/SwuduSusuwu> (if _GitHub_ now allows private messages),
- or <mailto:2002swudususuwu@gmail.com>.
- If there is no response soon, you can also contact <https://substack.com/@swudususuwu>.

You can expect:
- Best effort to address the issue(s),
- with you anonymous (unless you ask to publish credits to you.)

## Errata/footnotes
**TODO**; have [`./.ssh/setup.sh`](./.ssh/setup.sh) do `git config` to warn if new commits use old certificates (don't know how to).

[^1]: Asof [commmit 7a9f52b2301f16807485b6701bec883404b4bd29](https://github.com/SwuduSusuwu/SusuLib/commit/7a9f52b2301f16807485b6701bec883404b4bd29) (+[`cxx/main.hxx`](./cxx/main.hxx): for issues #3, #14: cross-language), `testHarnesses` is now `susuwuUnitTests`.

[^2]: Asof [commmit 36fa8a54a2a56d6e5bf21899980b48b462c15bde](https://github.com/SwuduSusuwu/SusuLib/commit/36fa8a54a2a56d6e5bf21899980b48b462c15bde) (+[`.github/workflows/codacy.yml`](./.github/workflows/codacy.yml) New _GitHub_ analysis.), the code scans now include all of _Codacy_'s test results; before this, [just _GitHub_'s _CodeQL_ produced code scans](https://github.com/SwuduSusuwu/SusuLib/security/code-scanning?query=is%3Aopen+tool%3ACodeQL+).

[^3]: Asof [commmit a40d1ff013f3007384e4ed025d0e402364d189cb](https://github.com/SwuduSusuwu/SusuLib/commit/a40d1ff013f3007384e4ed025d0e402364d189cb) [`./.ssh/sha256.sig`](./.ssh/sha256.sig) (and the [`./.ssh/allowed_signers`](./.ssh/allowed_signers) which `sha256.sig` produces) have a new certificate (`SHA256:1csQw8HZNJa7t2gbG9/usNZ6cXdlUlSMcA3dVb3j16c`).

[^4]: Asof [commit 4073fc729bdf7eda455a1a9914310e118efa5833](https://github.com/SwuduSusuwu/SusuLib/commit/4073fc729bdf7eda455a1a9914310e118efa5833), [`./.ssh/sha256.sig`](./.ssh/sha256.sig) (and the [`./.ssh/allowed_signers`](./.ssh/allowed_signers) which `sha256.sig` produces) have a new certificate. All commits which follow that commit must use that new certificate (`SHA256:8MXQK2Ms1FI4X3BSNbLuYAAMO3MXPQ7GsGH4kcXNDiY`).

[^5]: Asof [commit 585a6bb7c84cbb28ff106a1d8f5236c9cbc97082](https://github.com/SwuduSusuwu/SusuLib/commit/585a6bb7c84cbb28ff106a1d8f5236c9cbc97082), [`./.ssh/sha256.sig`](./.ssh/sha256.sig) (and the [`./.ssh/allowed_signers`](./.ssh/allowed_signers) which `sha256.sig` produces) have a new certificate. All commits which follow that commit must use that new certificate (`SHA256:G6I98GU7oMXNmKB/k10nAlZo52bN9y17xvARXjblM2g`).

[^6]: Asof [`commit` 1cf751c10ee99dc3ed84e8dc7761358545486550](https://github.com/SwuduSusuwu/SusuLib/commit/1cf751c10ee99dc3ed84e8dc7761358545486550), [`./.ssh/sha256.sig`](./.ssh/sha256.sig) (plus the [`./.ssh/allowed_signers`](./.ssh/allowed_signers) which `sha256.sig` produces) use new sigs. All `commit`s which follow that `commit` must use those new sigs (hash is `SHA256:1ywl0RIdF6ZueX1qazgx1ihGDytfKYf9T97gONFroio`).
