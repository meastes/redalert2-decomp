#!/bin/zsh
# yr_run.sh <native build dir> [seconds] [run-to]: run yr-native against the user's RA2+YR install
# (read only; $YR_DATA_DIR overrides) with writes under <build>/yr-run and the window dumped every
# second to <build>/yr-run/shots/win.<n>.bmp; prints the milestones, traps and the stack (lane Y3).
B=${1:?native build dir}
D=${YR_DATA_DIR:-"/Volumes/Lesalia/Command and Conquer Red Alert II"}
O=$B/yr-run
mkdir -p $O/w $O/shots
find $O/shots -name 'win.*.bmp' -delete
RA2_NATIVE_WINDOW_DUMP=$O/shots/win RA2_NATIVE_WINDOW_DUMP_MS=1000 RA2_NATIVE_MOVIE_SKIP=1 RA2_NATIVE_IGNORE_FOCUS=1 \
  $B/yr-native --data-dir "$D" --run-to=${3:-never} --run-seconds=${2:-30} --write-dir=$O/w "--game-args=-noaudio -CD." >| $O/run.log 2>&1
echo rc $?
grep -v "^progress: present" $O/run.log | grep -E "run-to|unrecovered|signal|^[0-9]+ " | head -40 | cut -c1-200
