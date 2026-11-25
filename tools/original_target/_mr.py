import sys, rebuild as R
lo, hi = int(sys.argv[1], 16), int(sys.argv[2], 16)
for r in sorted(R.main_catalog()['regions'], key=lambda r: r['entry']):
    if lo <= r['entry'] < hi: print('%08x %5d+%d %s %s' % (r['entry'], r['body_bytes'], r['padding_bytes'], r['symbol'][:60], r['source'][-45:]))
