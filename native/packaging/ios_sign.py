#!/usr/bin/env python3
"""Sign the iOS app for the user's own device (build-ios.command device; Scout1248).

    python3 native/packaging/ios_sign.py <RA2.app> --team <team id> [--profile <file.mobileprovision>]

The team id comes from the command line (RA2_IOS_TEAM), never from a tracked file.
Without --profile, the newest unexpired development provisioning profile of that
team whose app id covers io.github.ra2native.RA2 (exact or the team wildcard) is
taken from Xcode's profile folders. The signing identity is the keychain's code
signing identity whose certificate the profile lists. The app gets the profile as
embedded.mobileprovision and entitlements made from it (application identifier,
team, get-task-allow), then codesign signs it. Nothing is downloaded and no
account or keychain setting is changed: when no usable profile or identity is
found, it says so and stops (Xcode, signed in with the team, makes them).
"""
import argparse
import datetime
import glob
import hashlib
import os
import plistlib
import shutil
import subprocess
import sys
import tempfile

BUNDLE_ID = 'io.github.ra2native.RA2'
FOLDERS = ['~/Library/Developer/Xcode/UserData/Provisioning Profiles', '~/Library/MobileDevice/Provisioning Profiles']


def decode(path):
    out = subprocess.run(['security', 'cms', '-D', '-i', path], capture_output=True)
    return plistlib.loads(out.stdout) if out.returncode == 0 and out.stdout else None


def identities():
    """{sha1: name} of the keychain's valid code signing identities."""
    out = subprocess.run(['security', 'find-identity', '-v', '-p', 'codesigning'], capture_output=True, text=True).stdout
    found = {}
    for line in out.splitlines():
        parts = line.split()
        if len(parts) >= 3 and len(parts[1]) == 40:
            found[parts[1].upper()] = line.split('"')[1] if '"' in line else parts[2]
    return found


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('app')
    ap.add_argument('--team', required=True)
    ap.add_argument('--profile')
    args = ap.parse_args()
    keys = identities()
    candidates = [args.profile] if args.profile else [p for d in FOLDERS for p in glob.glob(os.path.join(os.path.expanduser(d), '*.mobileprovision'))]
    now = datetime.datetime.now(datetime.timezone.utc)
    best = None
    for path in candidates:
        p = decode(path)
        if not p or args.team not in p.get('TeamIdentifier', []):
            continue
        ent = p.get('Entitlements', {})
        app_id = ent.get('application-identifier', '')
        if app_id not in ('%s.%s' % (args.team, BUNDLE_ID), '%s.*' % args.team) or not ent.get('get-task-allow'):
            continue
        expires = p['ExpirationDate'].replace(tzinfo=datetime.timezone.utc)
        if expires <= now:
            continue
        signers = [hashlib.sha1(c).hexdigest().upper() for c in p.get('DeveloperCertificates', [])]
        usable = [s for s in signers if s in keys]
        if not usable:
            continue
        if best is None or (app_id.endswith(BUNDLE_ID), p['CreationDate']) > (best[2].endswith(BUNDLE_ID), best[1]['CreationDate']):
            best = (path, p, app_id, usable[0])
    if not best:
        sys.exit('ios_sign: no unexpired development profile of the team covering %s with a signing identity in the '
                 'keychain: open Xcode signed in with the team (Settings, Accounts) and let it make one' % BUNDLE_ID)
    path, profile, app_id, identity = best
    print('ios_sign: profile "%s" (expires %s), identity %s' % (profile.get('Name'), profile['ExpirationDate'].date(), keys[identity]))
    shutil.copyfile(path, os.path.join(args.app, 'embedded.mobileprovision'))
    entitlements = {
        'application-identifier': '%s.%s' % (args.team, BUNDLE_ID),
        'com.apple.developer.team-identifier': args.team,
        'get-task-allow': True,
        'keychain-access-groups': ['%s.%s' % (args.team, BUNDLE_ID)],
    }
    with tempfile.NamedTemporaryFile('wb', suffix='.plist', delete=False) as f:
        plistlib.dump(entitlements, f)
        ent_path = f.name
    try:
        subprocess.run(['codesign', '--force', '--sign', identity, '--entitlements', ent_path, '--timestamp=none',
                        '--generate-entitlement-der', args.app], check=True)
        subprocess.run(['codesign', '--verify', '--strict', args.app], check=True)
    finally:
        os.unlink(ent_path)
    print('ios_sign: signed %s' % args.app)


if __name__ == '__main__':
    main()
