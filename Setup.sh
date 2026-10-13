#!/usr/bin/env bash
# Registers this folder as the Sengine install. Game projects find the engine through SENGINE_ROOT.
set -e
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROFILE="$HOME/.profile"

touch "$PROFILE"
grep -v '^export SENGINE_ROOT=' "$PROFILE" > "$PROFILE.tmp" || true
echo "export SENGINE_ROOT='$ROOT'" >> "$PROFILE.tmp"
mv "$PROFILE.tmp" "$PROFILE"

echo "SENGINE_ROOT set to $ROOT"
echo "Log out and back in (or run: source ~/.profile) for it to take effect."
