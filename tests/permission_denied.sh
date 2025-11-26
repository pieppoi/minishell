#!/usr/bin/env bash
set -euo pipefail

MINISHELL=${MINISHELL:-/workspace/minishell}
TMPDIR=$(mktemp -d)
cleanup() {
	rm -rf "$TMPDIR"
}
trap cleanup EXIT

mkdir -p "$TMPDIR/test_dir/sub_dir"
cat <<'EOF' >"$TMPDIR/test_dir/file.txt"
echo "should not execute"
EOF
chmod 644 "$TMPDIR/test_dir/file.txt"

INPUT=$(cat <<EOF
cd $TMPDIR/test_dir
cd sub_dir
../file.txt
exit
EOF
)

OUTPUT=$(printf "%s\n" "$INPUT" | "$MINISHELL" 2>&1 || true)

echo "$OUTPUT" | grep -q "Permission denied" && {
	echo "[PASS] Permission denied reproduced"
	exit 0
}

echo "[FAIL] Expected 'Permission denied' in output" >&2
echo "$OUTPUT" >&2
exit 1
