"""reset_refs.py UNIT...: drop the unit's learned per-unit data bindings (header statics, floating
constants, function statics) from units.json, keeping string literals, so the next `check --learn`
infers them again from the current text. Bindings learned under an earlier include order otherwise
survive and pin the statics to the wrong copies (lane R2)."""
import sys
import rebuild as R
units = R.load_json(R.UNITS, {})
for unit in sys.argv[1:]:
    refs = units[unit].get('references', {})
    drop = [k for k in refs if not k.startswith('??_C@')]
    for k in drop:
        del refs[k]
    print('%s: %d bindings dropped' % (unit, len(drop)))
R.save_units(units, sys.argv[1:])
