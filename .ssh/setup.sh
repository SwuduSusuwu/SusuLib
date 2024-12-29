#!/bin/sh
set -x
NAME_OF_CONTRIBUTOR="SwuduSusuwu" # if you fork this, substitute your own
DIR="$(dirname "$(git rev-parse --git-dir)")/.ssh" # `DIR=$(dirname "$0")` gives `/usr/bin` for `. setup.sh`
if [ "${NAME_OF_CONTRIBUTOR}" = "$(git config --get user.name)" ]; then
	git config gpg.format "ssh"
#	git config user.signingkey "${DIR}/id_ed25519.pub" # for `git commit -S`
	git config commit.gpgsign true
	git config --get user.email > "${DIR}/email_of_contributor"
fi
EMAIL_OF_CONTRIBUTOR=$(cat "${DIR}/email_of_contributor")
echo "${EMAIL_OF_CONTRIBUTOR} namespaces=\"git\" $(cat "${DIR}/id_ed25519.pub")" > "${DIR}/allowed_signers"
git config gpg.ssh.allowedSignersFile "${DIR}/allowed_signers" # for `git log --show-signature`, `git-verify`
git log --show-signature
ssh-keygen -lf "${DIR}/id_ed25519.pub" -E sha256 > "${DIR}/sha256.sig" && cat "${DIR}/sha256.sig" #GitHub converts fingerprint to sha256 hash
set +x

