#!/usr/bin/env python3
"""Placeholder views of classes that already exist (rename lane census, lane N4 2026-11-13).

    placeholder_census.py [--names CSV] [--all]

Lists accepted source that defines a member of a placeholder view (a class named Owner_*,
Unknown*, Leaf_*, Call_* or Target_*) at an address mischa85's names.csv attributes to a method
of a class src/shared already declares: the candidates for "merge the placeholder into the class
that already exists" (docs/DECISIONS.md "Placeholder type replacement"). One tab-separated line
per view: source, view::member, address, names.csv symbol, class. --all also lists views whose
class src/shared does not declare (marked "undeclared").

Text scan only: it reads src/recovered and src/shared and the names.csv of the private
mischa85 clone (private/reference/mischa85-RedAlert2/config/ra2-1.006/names.csv); nothing is
compiled and no catalog is read. A listed view is a candidate, not a ruling: a merge still has
to agree on offsets and pass apply_rename's strict audit with every accepted row unchanged.
"""
import argparse
import csv
import re
from pathlib import Path

REPOSITORY = Path(__file__).resolve().parents[2]
NAMES = REPOSITORY / 'private/reference/mischa85-RedAlert2/config/ra2-1.006/names.csv'
VIEW_MEMBER = re.compile(r'^[\w\s\*&:<>,]*?\b((?:Owner|Unknown|Leaf|Call|Target)\w*)::(\w+)\s*\(', re.M)
ADDRESS = re.compile(r'_?(?:0x)?([0-9a-fA-F]{6,8})\b')
DECLARED = re.compile(r'^(?:class|struct)\s+([A-Za-z_]\w*)\b(?!\s*;)', re.M)


def symbol_class(symbol):
    """The class of an MSVC member symbol (?Method@Class@@...), or None."""
    parts = symbol.lstrip('?').split('@')
    if len(parts) > 2 and parts[1] and not parts[1].startswith('$') and parts[2] == '':
        return parts[1]
    return None


def main():
    parser = argparse.ArgumentParser(description=__doc__.split('\n\n')[0])
    parser.add_argument('--names', default=str(NAMES))
    parser.add_argument('--all', action='store_true')
    args = parser.parse_args()
    names = {int(row['address'], 16): row['symbol'] for row in csv.DictReader(open(args.names))}
    declared = set()
    for header in (REPOSITORY / 'src/shared').rglob('*.hpp'):
        declared.update(DECLARED.findall(header.read_text(errors='replace')))
    for path in sorted((REPOSITORY / 'src/recovered').rglob('*.cpp')):
        text = path.read_text(errors='replace')
        for match in VIEW_MEMBER.finditer(text):
            view, member = match.groups()
            if view in declared:        # a real class whose name happens to start like a view (OwnerTalkClass)
                continue
            for token in ADDRESS.findall(view) + ADDRESS.findall(member):
                address = int(token, 16)
                symbol = names.get(address)
                cls = symbol and symbol_class(symbol)
                if not cls:
                    continue
                known = cls in declared
                if known or args.all:
                    print('\t'.join([str(path.relative_to(REPOSITORY)), '%s::%s' % (view, member), '%08X' % address,
                                     symbol, cls if known else cls + ' (undeclared)']))
                break


if __name__ == '__main__':
    main()
