#!/bin/sh
set -x
NAME_OF_CONTRIBUTOR="SwuduSusuwu" # if you fork this, substitute your own
CRYPTO_MODE="id_ed25519"
DIR="$(dirname "$(git rev-parse --git-dir)")/.ssh" # `DIR=$(dirname "$0")` gives `/usr/bin` for `. setup.sh`
if [ "${NAME_OF_CONTRIBUTOR}" = "$(git config --get user.name)" ]; then
	git config gpg.format "ssh"
	cp ~/.ssh/${CRYPTO_MODE}.pub "${DIR}/${CRYPTO_MODE}.pub"
#	git config user.signingkey "${DIR}/${CRYPTO_MODE}.pub" # for `git commit -S`
	git config commit.gpgsign true
	git config --get user.email > "${DIR}/email_of_contributor"
fi
EMAIL_OF_CONTRIBUTOR=$(cat "${DIR}/email_of_contributor")
echo "${EMAIL_OF_CONTRIBUTOR} namespaces=\"git\" $(cat "${DIR}/${CRYPTO_MODE}.pub")" > "${DIR}/allowed_signers"
git config gpg.ssh.allowedSignersFile "${DIR}/allowed_signers" # for `git log --show-signature`, `git-verify`
cat "${DIR}/allowed_signers.old" >> "${DIR}/allowed_signers" # Old certificates (not revoked) # TODO: warn if old certificates are used for new commits
git log --show-signature
ssh-keygen -lf "${DIR}/${CRYPTO_MODE}.pub" -E sha256 > "${DIR}/sha256.sig" && cat "${DIR}/sha256.sig" #GitHub converts fingerprint to sha256 hash
set +x

