#!/bin/sh
#
# Hash one input with ft_ssl and with the reference (openssl) and compare.
#
#   sh tests/compare.sh -s "42 is nice"              every algorithm
#   sh tests/compare.sh md5 sha256 -s "42 is nice"   only those
#   sh tests/compare.sh whirlpool -f Makefile        a file
#   echo "42 is nice" | sh tests/compare.sh md5      stdin
#
# Or through make:  make compare S="42 is nice" A="md5 sha256"
#                   make compare F=Makefile

ALL_ALGOS="md5 sha224 sha256 sha384 sha512 whirlpool"
FT_SSL="${FT_SSL:-./ft_ssl}"

usage() {
	sed -n '3,12s/^# \{0,1\}//p' "$0"
}

ALGOS=""
MODE=""
INPUT=""
while [ $# -gt 0 ]; do
	case "$1" in
		-s) [ $# -ge 2 ] || { usage; exit 1; }
			MODE=string; INPUT="$2"; shift 2 ;;
		-f) [ $# -ge 2 ] || { usage; exit 1; }
			MODE=file; INPUT="$2"; shift 2 ;;
		-h|--help) usage; exit 0 ;;
		*) ALGOS="$ALGOS $1"; shift ;;
	esac
done
ALGOS="${ALGOS:-$ALL_ALGOS}"

if [ -z "$MODE" ]; then
	if [ -t 0 ]; then
		usage
		exit 1
	fi
	MODE=stdin
fi
if [ ! -x "$FT_SSL" ]; then
	echo "compare: $FT_SSL not found, run make first" >&2
	exit 1
fi
if [ "$MODE" = file ] && [ ! -r "$INPUT" ]; then
	echo "compare: $INPUT: cannot read file" >&2
	exit 1
fi

TMP="$(mktemp)"
trap 'rm -f "$TMP"' EXIT
case "$MODE" in
	string) printf '%s' "$INPUT" > "$TMP"; SHOWN="string \"$INPUT\"" ;;
	file)   cat "$INPUT" > "$TMP"; SHOWN="file $INPUT" ;;
	stdin)  cat > "$TMP"; SHOWN="stdin" ;;
esac

if [ -t 1 ]; then
	GREEN="$(printf '\033[32m')"; RED="$(printf '\033[31m')"
	DIM="$(printf '\033[2m')"; RESET="$(printf '\033[0m')"
fi

# ref <algo>: reference digest of stdin, bare hex
ref() {
	if [ "$1" = whirlpool ]; then
		openssl dgst -provider legacy -provider default -whirlpool -r
	else
		openssl dgst -"$1" -r
	fi 2>/dev/null | cut -d' ' -f1
}

printf 'input: %s (%s bytes)\n\n' "$SHOWN" "$(wc -c < "$TMP" | tr -d ' ')"

FAIL=0
for a in $ALGOS; do
	case " $ALL_ALGOS " in
		*" $a "*) ;;
		*) printf '%s%-10s unknown algorithm (have: %s)%s\n\n' \
			"$RED" "$a" "$ALL_ALGOS" "$RESET"
		   FAIL=1; continue ;;
	esac
	case "$MODE" in
		string) mine=$("$FT_SSL" "$a" -q -s "$INPUT" 2>&1)
				cmd="./ft_ssl $a -s '$INPUT'" ;;
		file)   mine=$("$FT_SSL" "$a" -q "$INPUT" 2>&1)
				cmd="./ft_ssl $a $INPUT" ;;
		stdin)  mine=$("$FT_SSL" "$a" -q < "$TMP" 2>&1)
				cmd="... | ./ft_ssl $a" ;;
	esac
	theirs=$(ref "$a" < "$TMP")
	if [ -z "$theirs" ]; then
		status="${RED}NO REFERENCE${RESET}"; FAIL=1
	elif [ "$mine" = "$theirs" ]; then
		status="${GREEN}OK${RESET}"
	else
		status="${RED}KO${RESET}"; FAIL=1
	fi
	printf '%-10s %s\n' "$a" "$status"
	printf '  ft_ssl : %s\n' "$mine"
	printf '  openssl: %s\n' "$theirs"
	printf '  %s%s%s\n\n' "$DIM" "$cmd" "$RESET"
done
exit "$FAIL"
