#!/usr/bin/env bash
TOPLEVEL=`git rev-parse --show-toplevel`
HOOK_PATH="$TOPLEVEL/.git/hooks/"

echo "Installing hooks for current project in $HOOK_PATH"

cp $TOPLEVEL/git-hooks/pre-commit $HOOK_PATH