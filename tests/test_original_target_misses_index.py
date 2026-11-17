"""misses_index.py attribution tests on synthetic OPEN_MISSES text; no proprietary inputs or tools."""
from pathlib import Path
import sys
import tempfile
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools/original_target'))
import misses_index

# Shapes taken from real OPEN_MISSES.md lines that misattributed scores and drafts (Scout1059/1062/1067).
TEXT = '''# Open misses

## Scout708 (2026-11-04, closed; 0 B)
- Fire_At 0x6CA890 83.80, shape 99.83, 8 hunks (g5_8380_h8.cpp); best raw 87.06 (g2_8706_h15.cpp). Desired_Load_Dir 0x704730 78.93. Tunnel 0x6FAA30 45.37. Mission_Attack: the float mechanism does not explain its spills (80.68 at most; temps, not a struct).

## Scout723 (2026-11-05, closed; 0 B)
- Draw_Voxel 0x6FF690 52.54 (dv_h.cpp). Rock 0x6D5F40 64.37. Area_Modify 96.38 flat.

## Scout819 (2026-11-06, closed; 0 B)
- Parked: Tunnel depth 0x6CFB60 92.31 (w/tz2.cpp); Area_Modify 0x6C5270 96.38, Fire_At 83.80. Not attempted: 0x6CD340, 0x6D5F40.

## Scout524 (2026-11-02, closed; 0 B)
- Select_Game 0x5135F0: 93.08, the jump-table ceiling. SpotLight shapes 0x5DD760: 55.83. Made_A_Kill 0x713360: 63.16 (the original keeps VeteranCap as an FP rvalue).

## Scout824 (2026-11-06, closed; 0 B)
- Tunnel_AI 0x6FAA30 42.86 to 50.39 (keep/tunnel_tu5_5039.cpp; needs RA2_COORD_LENGTH; frame 0x90 vs 0x70).

## Scout955 (2026-11-07, closed; 0 B; TechnoClass Fire_At 0x6CA890 89.02 -> 99.14 natural; NOTES, keep/F_9914_natural.cpp, lab/ in build/class-work/Scout955/)
- Natural form.

## Scout960 (2026-11-07, closed; 0 B; Fire_At 0x6CA890 still 99.14 / 11 lines; NOTES, sw.sh, keep/lab_q1.cpp in build/class-work/Scout960/)
- Lab only.

## Scout1062 (2026-11-08, closed; 0 B; NOTES, keep/ in build/class-work/Scout1062/)
- Area_Modify 0x6C5270 96.38, Evaluate_Object 0x6C5570 91.81, Laser 99.46, Fire_At 0x6CA890 99.14 (Scout955 F_9914_natural.cpp), Get_Z_Adjust 0x6CFEA0 81.20; 0x6CD340 and Rock 0x6D5F40 drafts no longer compile. Errors: 0x6CA890 points at a lab file (keep/lab_q1.cpp).

## Scout1029 (2026-11-07, closed; 0 B; family 0x55DEF0/0x55E410 98.73 each; keep/ in build/class-work/Scout1029/)
- 0x55F020 (w/dm_EW2.cpp), 0x55F540 (w/dm_NS2.cpp) all 97.50 shape 100. Laser 0x6C9DF0 and Rock 0x6C9E00 64.00.
'''


def rows(text=TEXT, resolve=None):
    return {r['entry']: r for r in misses_index.build(text, None, resolve)}


class MissesIndexTests(unittest.TestCase):
    def test_fire_at_draft_is_the_best_scoring_lanes_file(self):
        r = rows()[0x6CA890]
        self.assertEqual(r['best'], 99.14)
        self.assertEqual(r['draft'], 'build/class-work/Scout955/keep/F_9914_natural.cpp')
        self.assertNotIn('build/class-work/Scout708/g2_8706_h15.cpp', r['drafts'])

    def test_rock_does_not_take_area_modify_score(self):
        r = rows()[0x6D5F40]
        self.assertEqual(r['best'], 64.37)
        self.assertEqual(r['attempts'], 3)
        self.assertEqual(r['last_lane'], 'Scout1062')

    def test_spotlight_does_not_take_made_a_kill_score(self):
        r = rows()
        self.assertEqual(r[0x5DD760]['best'], 55.83)
        self.assertEqual(r[0x713360]['best'], 63.16)
        self.assertEqual(r[0x5135F0]['best'], 93.08)

    def test_tunnel_does_not_take_fire_at_or_neighbour_scores(self):
        r = rows()
        self.assertEqual(r[0x6FAA30]['best'], 50.39)
        self.assertEqual(r[0x6FAA30]['draft'], 'build/class-work/Scout824/keep/tunnel_tu5_5039.cpp')
        self.assertEqual(r[0x704730]['best'], 78.93)
        self.assertEqual(r[0x6CA890]['history'][0]['score'], 83.80)

    def test_shared_scores_bind_only_when_the_note_distributes_them(self):
        r = rows()
        self.assertEqual(r[0x55DEF0]['best'], 98.73)
        self.assertEqual(r[0x55E410]['best'], 98.73)
        self.assertEqual(r[0x55F020]['best'], 97.50)
        self.assertEqual(r[0x55F020]['draft'], 'build/class-work/Scout1029/w/dm_EW2.cpp')
        self.assertIsNone(r[0x6C9DF0]['best'])
        self.assertTrue(r[0x6C9DF0]['best_ambiguous'])
        self.assertEqual(r[0x6C9E00]['best'], 64.00)

    def test_ambiguous_rows_render_question_marks(self):
        table = misses_index.render(misses_index.build(TEXT, None), True)
        line = next(l for l in table.splitlines() if l.startswith('| 0x6C9DF0 '))
        self.assertEqual(line.split(' | ')[2], '?')
        cd340 = rows()[0x6CD340]
        self.assertIsNone(cd340['best'])
        self.assertFalse(cd340['best_ambiguous'])

    def test_bare_draft_names_resolve_in_the_named_lane_folder(self):
        with tempfile.TemporaryDirectory() as root:
            keep = Path(root) / 'build/class-work/Scout955/keep'
            keep.mkdir(parents=True)
            (keep / 'F_9914_natural.cpp').write_text('')
            text = '## Scout1062 (2026-11-08, closed)\n- Fire_At 0x6CA890 99.14 (Scout955 F_9914_natural.cpp), Get_Z_Adjust 0x6CFEA0 81.20 (gz.cpp).\n'
            r = rows(text, misses_index.lane_resolver(Path(root)))
            self.assertEqual(r[0x6CA890]['draft'], 'build/class-work/Scout955/keep/F_9914_natural.cpp')
            self.assertEqual(r[0x6CA890]['best'], 99.14)
            self.assertIsNone(r[0x6CFEA0]['draft'])     # bare name not found in the lane folder
            self.assertEqual(r[0x6CFEA0]['best'], 81.20)


if __name__ == '__main__':
    unittest.main()
