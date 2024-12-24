#!/bin/sh
set -x
echo "$(git config --get user.email) namespaces=\"git\" $(cat .ssh/id_ed25519.pub)" > .ssh/allowed_signers
git config gpg.ssh.allowedSignersFile ".ssh/allowed_signers"
if [ "uSwuduSusuwu" = "$(git config --get user.name)" ]; then
	git config gpg.format "ssh"
	git config user.signingkey ".ssh/id_ed25519.pub"
	git config commit.gpgsign true
fi
git log --show-signature
DIR=$(dirname "$0")
ssh-keygen -lf .ssh/id_ed25519.pub -E sha256 > "${DIR}/sha256.sig" && cat "${DIR}/sha256.sig" #GitHub converts fingerprint to sha256 hash
set +x

