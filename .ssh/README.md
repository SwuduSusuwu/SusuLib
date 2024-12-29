(C) 2024 Swudu Susuwu, dual licenses: choose [_GPLv2_](../LICENSE_GPLv2) or [_Apache 2_](../LICENSE) (allows all uses).

# [Table of Contents](../README.md#table-of-contents)
- [Purposes](#purposes)

# Purposes
[`./.ssh/`](./) is to [compute signatures/certificates](../README.md#signaturecertificate).
- [`./.ssh/setup.sh`](./setup.sh) is to setup `gpg.ssh.allowedSignersFile` (allows to use `git verify <ref>` or `git log --show-signature`)
- [`./.ssh/id_ed25519.pub`](./id_ed25519.pub) is used at `.ssh/setup.sh` to produce `.ssh/allowed_signers` + `.ssh/sha256.sig`.
- [`./.ssh/email_of_contributor`](./email_of_contributor) is used at `.ssh/setup.sh` to produce `.ssh/allowed_signers`.
- [`./.ssh/allowed_signers`](./allowed_signers) is required for `gpg.ssh.allowedSignersFile` (included in case `.ssh/setup.sh` fails to produce this.)
- [`./.ssh/sha256.sig`](./sha256.sig) is for comparison to the results of `git verify <ref>` or `git log --show-signature` (included in case `.ssh/setup.sh` fails to produce this.)

