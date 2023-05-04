#!/bin/sh

TOPDIR=${TOPDIR:-$(git rev-parse --show-toplevel)}
SRCDIR=${SRCDIR:-$TOPDIR/src}
MANDIR=${MANDIR:-$TOPDIR/doc/man}

GAMEPASSD=${GAMEPASSD:-$SRCDIR/gamepassd}
GAMEPASSCLI=${GAMEPASSCLI:-$SRCDIR/gamepass-cli}
GAMEPASSTX=${GAMEPASSTX:-$SRCDIR/gamepass-tx}
GAMEPASSQT=${GAMEPASSQT:-$SRCDIR/qt/gamepass-qt}

[ ! -x $GAMEPASSD ] && echo "$GAMEPASSD not found or not executable." && exit 1

# The autodetected version git tag can screw up manpage output a little bit
GPNVER=($($GAMEPASSCLI --version | head -n1 | awk -F'[ -]' '{ print $6, $7 }'))

# Create a footer file with copyright content.
# This gets autodetected fine for gamepassd if --version-string is not set,
# but has different outcomes for gamepass-qt and gamepass-cli.
echo "[COPYRIGHT]" > footer.h2m
$GAMEPASSD --version | sed -n '1!p' >> footer.h2m

for cmd in $GAMEPASSD $GAMEPASSCLI $GAMEPASSTX $GAMEPASSQT; do
  cmdname="${cmd##*/}"
  help2man -N --version-string=${GPNVER[0]} --include=footer.h2m -o ${MANDIR}/${cmdname}.1 ${cmd}
  sed -i "s/\\\-${GPNVER[1]}//g" ${MANDIR}/${cmdname}.1
done

rm -f footer.h2m
