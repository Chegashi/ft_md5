#!/bin/sh
#
# Usage: sh tests/run_tests.sh <path/to/ft_ssl> [algo ...]
#   (normally run through `make test` or `make test md5 sha256`)
#
# Every algorithm is checked against the `openssl dgst` reference, and md5 /
# sha256 additionally against the expected outputs printed in the subject.

ALL_ALGOS="md5 sha224 sha256 sha384 sha512 whirlpool"

FT_SSL="$(cd "$(dirname "$1")" && pwd)/$(basename "$1")"
shift
ALGOS="${*:-$ALL_ALGOS}"

if [ ! -x "$FT_SSL" ]; then
	echo "run_tests: $FT_SSL: not found, run make first" >&2
	exit 1
fi
if ! command -v openssl >/dev/null 2>&1; then
	echo "run_tests: openssl is needed as the reference implementation" >&2
	exit 1
fi

TMP="$(mktemp -d)"
trap 'rm -rf "$TMP"' EXIT
cd "$TMP" || exit 1

if [ -t 1 ]; then
	GREEN="$(printf '\033[32m')"; RED="$(printf '\033[31m')"
	BOLD="$(printf '\033[1m')"; RESET="$(printf '\033[0m')"
fi

PASS=0
FAIL=0
SUITE=""
RESULTS="$TMP/.results"
FAILURES="$TMP/.failures"
: > "$RESULTS"
: > "$FAILURES"

# ---------------------------------------------------------------- helpers --

# check <description> <expected> <actual>
# Results are only recorded here; they are printed as tables at the end.
check() {
	if [ "$2" = "$3" ]; then
		PASS=$((PASS + 1))
		result=OK
	else
		FAIL=$((FAIL + 1))
		result=KO
		{
			printf '%s%s > %s%s\n' "$BOLD" "$SUITE" "$1" "$RESET"
			printf '    expected: %s\n' "$2" | sed '2,$s/^/              /'
			printf '    got:      %s\n' "$3" | sed '2,$s/^/              /'
			echo
		} >> "$FAILURES"
	fi
	printf '%s\t%s\t%s\n' "$SUITE" "$1" "$result" >> "$RESULTS"
}

# check_status <description> <expected status> <actual status>
check_status() {
	check "$1 (exit status)" "$2" "$3"
}

# ft_ssl with stderr merged into stdout, so error lines are compared too
run() {
	"$FT_SSL" "$@" 2>&1
}

# ref <algo>: reference digest of stdin, bare hex
ref() {
	if [ "$1" = whirlpool ]; then
		openssl dgst -provider legacy -provider default -whirlpool -r
	else
		openssl dgst -"$1" -r
	fi | cut -d' ' -f1
}

upper() {
	printf '%s' "$1" | tr '[:lower:]' '[:upper:]'
}

title() {
	SUITE="$1"
	[ -t 1 ] && printf '\r\033[K  running %s ...' "$1"
}

# Renders .results as one table per suite, then a summary table.
print_tables() {
	awk -F '\t' -v G="$GREEN" -v R="$RED" -v B="$BOLD" -v Z="$RESET" '
	function rep(s, k,    o) { o = ""; while (k-- > 0) o = o s; return o }
	function bar(l, m, r, a, b, c) {
		return l rep("─", a) m rep("─", b) m rep("─", c) r
	}
	function bar4(l, m, r) {
		return l rep("─", ws + 2) m rep("─", 8) m rep("─", 8) m \
			rep("─", 7) r
	}
	{
		n++; S[n] = $1; D[n] = $2; V[n] = $3
		if (length($2) > w) w = length($2)
		if (length($1) > ws) ws = length($1)
		if (!($1 in seen)) { seen[$1] = 1; order[++ns] = $1 }
		if ($3 == "OK") ok[$1]++; else ko[$1]++
	}
	END {
		if (w < 24) w = 24
		if (ws < 5) ws = 5
		fd = "%-" w "s"; fs = "%-" ws "s"
		for (i = 1; i <= ns; i++) {
			s = order[i]; k = 0
			print bar("┌", "┬", "┐", 6, w + 2, 8)
			printf "│    # │ %s" fd "%s │ Result │\n", B, s, Z
			print bar("├", "┼", "┤", 6, w + 2, 8)
			for (j = 1; j <= n; j++) {
				if (S[j] != s) continue
				c = (V[j] == "OK") ? G : R
				printf "│ %4d │ " fd " │ %s  %s  %s │\n", ++k, D[j], c, V[j], Z
			}
			print bar("└", "┴", "┘", 6, w + 2, 8)
			print ""
		}
		print bar4("┌", "┬", "┐")
		printf "│ %s" fs "%s │ Passed │ Failed │ Total │\n", B, "Suite", Z
		print bar4("├", "┼", "┤")
		for (i = 1; i <= ns; i++) {
			s = order[i]; c = (ko[s] > 0) ? R : G
			printf "│ %s" fs "%s │ %6d │ %s%6d%s │ %5d │\n", c, s, Z, \
				ok[s], (ko[s] > 0 ? R : ""), ko[s], Z, ok[s] + ko[s]
			tok += ok[s]; tko += ko[s]
		}
		print bar4("├", "┼", "┤")
		printf "│ %s" fs "%s │ %6d │ %6d │ %5d │\n", B, "Total", Z, \
			tok, tko, tok + tko
		print bar4("└", "┴", "┘")
	}' "$RESULTS"
}

# ------------------------------------------------------------ generic part --

suite_generic() {
	title "ft_ssl"
	commands=$(for a in $ALL_ALGOS; do printf '%s\n' "$a"; done)
	invalid=$(printf "ft_ssl: Error: 'foobar' is an invalid command.\n\nCommands:\n%s\n\nFlags:\n-p -q -r -s" "$commands")

	out=$(run); st=$?
	check "no argument prints usage" \
		"usage: ft_ssl command [flags] [file/string]" "$out"
	check_status "no argument" 1 "$st"

	out=$(run foobar); st=$?
	check "invalid command" "$invalid" "$out"
	check_status "invalid command" 1 "$st"

	out=$(run md5 -x); st=$?
	check "illegal option" "$(printf '%s\n%s' \
		'ft_ssl: md5: illegal option -- x' \
		'usage: ft_ssl md5 [-pqr] [-s string] [files ...]')" "$out"
	check_status "illegal option" 1 "$st"

	out=$(run md5 -s); st=$?
	check "-s without argument" "$(printf '%s\n%s' \
		'ft_ssl: md5: option requires an argument -- s' \
		'usage: ft_ssl md5 [-pqr] [-s string] [files ...]')" "$out"
	check_status "-s without argument" 1 "$st"
}

# -------------------------------------------------------- per-algo checks --

suite_algo() {
	a="$1"
	L="$(upper "$a")"
	title "$a"

	if ! printf '' | ref "$a" | grep -q '^[0-9a-f]\{16,\}$'; then
		check "reference \`openssl dgst -$a\` available" yes no
		return
	fi

	printf 'And above all,\n' > file
	mkdir -p dir
	h_in=$(printf '42 is nice\n' | ref "$a")
	h_file=$(ref "$a" < file)
	h_foo=$(printf 'foo' | ref "$a")
	h_bar=$(printf 'bar' | ref "$a")
	h_empty=$(printf '' | ref "$a")

	# stdin
	check "stdin" "(stdin)= $h_in" "$(printf '42 is nice\n' | run "$a")"
	check "stdin -q" "$h_in" "$(printf '42 is nice\n' | run "$a" -q)"
	check "stdin -r" "$h_in *stdin" "$(printf '42 is nice\n' | run "$a" -r)"
	check "stdin -q -r" "$h_in" "$(printf '42 is nice\n' | run "$a" -q -r)"
	check "empty stdin" "(stdin)= $h_empty" "$(printf '' | run "$a")"

	# -p
	check "-p" "(\"42 is nice\")= $h_in" \
		"$(printf '42 is nice\n' | run "$a" -p)"
	check "-p -r (no effect)" "(\"42 is nice\")= $h_in" \
		"$(printf '42 is nice\n' | run "$a" -p -r)"
	check "-p -q echoes raw input" "$(printf '42 is nice\n%s' "$h_in")" \
		"$(printf '42 is nice\n' | run "$a" -p -q)"
	check "-p without trailing newline" "(\"foo\")= $h_foo" \
		"$(printf 'foo' | run "$a" -p)"
	check "-p empty stdin" "(\"\")= $h_empty" "$(printf '' | run "$a" -p)"

	# -s
	check "-s" "$L (\"foo\") = $h_foo" "$(run "$a" -s foo)"
	check "-s -r" "$h_foo \"foo\"" "$(run "$a" -r -s foo)"
	check "-s -q" "$h_foo" "$(run "$a" -q -s foo)"
	check "-s -q -r" "$h_foo" "$(run "$a" -q -r -s foo)"
	check "-s empty string" "$L (\"\") = $h_empty" "$(run "$a" -s '')"
	check "-s twice" "$(printf '%s\n%s' "$L (\"foo\") = $h_foo" \
		"$L (\"bar\") = $h_bar")" "$(run "$a" -s foo -s bar)"
	check "-s does not read stdin" "$L (\"foo\") = $h_foo" \
		"$(printf 'x\n' | run "$a" -s foo)"

	# files
	check "file" "$L (file) = $h_file" "$(run "$a" file)"
	check "file -r" "$h_file file" "$(run "$a" -r file)"
	check "file -q" "$h_file" "$(run "$a" -q file)"
	check "file does not read stdin" "$L (file) = $h_file" \
		"$(printf 'x\n' | run "$a" file)"
	check "two files" "$(printf '%s\n%s' "$L (file) = $h_file" \
		"$L (file) = $h_file")" "$(run "$a" file file)"

	# order and mixing
	check "-p -s file: stdin, string, file" "$(printf '%s\n%s\n%s' \
		"(\"42 is nice\")= $h_in" "$L (\"foo\") = $h_foo" \
		"$L (file) = $h_file")" \
		"$(printf '42 is nice\n' | run "$a" -p -s foo file)"
	check "flags apply wherever they appear" "$(printf '%s\n%s' \
		"$h_foo \"foo\"" "$h_file file")" "$(run "$a" -s foo -r file)"

	# errors
	out=$(run "$a" file -s bar); st=$?
	check "flags after a file are files" "$(printf '%s\n%s\n%s' \
		"$L (file) = $h_file" \
		"ft_ssl: $a: -s: No such file or directory" \
		"ft_ssl: $a: bar: No such file or directory")" "$out"
	check_status "flags after a file" 1 "$st"

	out=$(run "$a" nope file); st=$?
	check "missing file, keeps going" "$(printf '%s\n%s' \
		"ft_ssl: $a: nope: No such file or directory" \
		"$L (file) = $h_file")" "$out"
	check_status "missing file" 1 "$st"

	out=$(run "$a" dir); st=$?
	check "directory" "ft_ssl: $a: dir: Is a directory" "$out"
	check_status "directory" 1 "$st"

	out=$(run "$a" file); st=$?
	check_status "success" 0 "$st"

	# lengths around the padding / block boundaries
	for n in 1 55 56 57 63 64 65 111 112 113 127 128 129 1000; do
		printf "%${n}s" '' | tr ' ' 'a' > blk
		check "$n bytes" "$(ref "$a" < blk)" "$(run "$a" -q blk)"
	done

	# binary data (NUL bytes) and a large input through stdin
	head -c 100000 /dev/urandom > bin
	check "binary file" "$(ref "$a" < bin)" "$(run "$a" -q bin)"
	head -c 5000000 /dev/urandom > big
	check "5 MB through stdin" "$(ref "$a" < big)" "$(run "$a" -q < big)"
}

# ------------------------------------------- examples given by the subject --

suite_subject_md5() {
	title "md5 (subject examples)"
	printf 'And above all,\n' > file
	check "echo | md5" "(stdin)= 35f1d6de0302e2086a4e472266efb3a9" \
		"$(echo "42 is nice" | run md5)"
	check "echo | md5 -p" \
		"(\"42 is nice\")= 35f1d6de0302e2086a4e472266efb3a9" \
		"$(echo "42 is nice" | run md5 -p)"
	check "echo | md5 -q -r" "e20c3b973f63482a778f3fd1869b7f25" \
		"$(echo "Pity the living." | run md5 -q -r)"
	check "md5 file" "MD5 (file) = 53d53ea94217b259c11a5a2d104ec58a" \
		"$(run md5 file)"
	check "md5 -r file" "53d53ea94217b259c11a5a2d104ec58a file" \
		"$(run md5 -r file)"
	check "md5 -s" "MD5 (\"pity those that aren't following baerista on spotify.\") = a3c990a1964705d9bf0e602f44572f5f" \
		"$(run md5 -s "pity those that aren't following baerista on spotify.")"
	check "echo | md5 -p file" "$(printf '%s\n%s' \
		'("be sure to handle edge cases carefully")= 3553dc7dc5963b583c056d1b9fa3349c' \
		'MD5 (file) = 53d53ea94217b259c11a5a2d104ec58a')" \
		"$(echo "be sure to handle edge cases carefully" | run md5 -p file)"
	check "echo | md5 file" "MD5 (file) = 53d53ea94217b259c11a5a2d104ec58a" \
		"$(echo "some of this will not make sense at first" | run md5 file)"
	check "echo | md5 -p -r file" "$(printf '%s\n%s' \
		'("but eventually you will understand")= dcdd84e0f635694d2a943fa8d3905281' \
		'53d53ea94217b259c11a5a2d104ec58a file')" \
		"$(echo "but eventually you will understand" | run md5 -p -r file)"
	check "echo | md5 -p -s foo file" "$(printf '%s\n%s\n%s' \
		"(\"GL HF let's go\")= d1e3cc342b6da09480b27ec57ff243e2" \
		'MD5 ("foo") = acbd18db4cc2f85cedef654fccc4a4d8' \
		'MD5 (file) = 53d53ea94217b259c11a5a2d104ec58a')" \
		"$(echo "GL HF let's go" | run md5 -p -s "foo" file)"
	check "echo | md5 -r -p -s foo file -s bar" "$(printf '%s\n%s\n%s\n%s\n%s' \
		'("one more thing")= a0bd1876c6f011dd50fae52827f445f5' \
		'acbd18db4cc2f85cedef654fccc4a4d8 "foo"' \
		'53d53ea94217b259c11a5a2d104ec58a file' \
		'ft_ssl: md5: -s: No such file or directory' \
		'ft_ssl: md5: bar: No such file or directory')" \
		"$(echo "one more thing" | run md5 -r -p -s "foo" file -s "bar")"
	check "echo | md5 -r -q -p -s foo file" "$(printf '%s\n%s\n%s\n%s' \
		'just to be extra clear' \
		'3ba35f1ea0d170cb3b9a752e3360286c' \
		'acbd18db4cc2f85cedef654fccc4a4d8' \
		'53d53ea94217b259c11a5a2d104ec58a')" \
		"$(echo "just to be extra clear" | run md5 -r -q -p -s "foo" file)"
}

suite_subject_sha256() {
	title "sha256 (subject examples)"
	echo "https://www.42.fr/" > website
	check "sha256 -q website" \
		"1ceb55d2845d9dd98557b50488db12bbf51aaca5aa9c1199eb795607a2457daf" \
		"$(run sha256 -q website)"
	check "sha256 -s" \
		"SHA256 (\"42 is nice\") = b7e44c7a40c5f80139f0a50f3650fb2bd8d00b0d24667c4c2ca32c88e13b758f" \
		"$(run sha256 -s "42 is nice")"
}

# ------------------------------------------------------------------- main --

for a in $ALGOS; do
	case " $ALL_ALGOS " in
		*" $a "*) ;;
		*) echo "run_tests: unknown algorithm '$a' (have: $ALL_ALGOS)" >&2
		   exit 1 ;;
	esac
done

suite_generic
for a in $ALGOS; do
	suite_algo "$a"
	case "$a" in
		md5) suite_subject_md5 ;;
		sha256) suite_subject_sha256 ;;
	esac
done

[ -t 1 ] && printf '\r\033[K'
print_tables

TOTAL=$((PASS + FAIL))
if [ "$FAIL" -eq 0 ]; then
	printf '\n%s%d/%d tests passed%s\n' "$GREEN" "$PASS" "$TOTAL" "$RESET"
	exit 0
fi
printf '\n%sFailures:%s\n\n' "$RED" "$RESET"
cat "$FAILURES"
printf '%s%d/%d tests failed%s\n' "$RED" "$FAIL" "$TOTAL" "$RESET"
exit 1
