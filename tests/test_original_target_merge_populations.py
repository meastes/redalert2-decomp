"""merge_populations.py plan contract on a synthetic catalog; no game bytes, no compiler."""
import pathlib
import sys
import unittest

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parents[1] / 'tools' / 'original_target'))
import merge_populations


def row(entry, population, source, order, **extra):
    base = dict(entry=entry, population=population, source=source, source_order=order,
                symbol_prefix='?F%x@@' % entry, options=['/O2', '/Ob2', '/G5', '/Gy'])
    base.update(extra)
    return base


class MergePopulationsTest(unittest.TestCase):
    def catalog(self, *rows):
        return dict(regions=list(rows))

    def plan(self, catalog, pids=('population-1', 'population-2')):
        return merge_populations.build_plan(catalog, list(pids), 'm', 'build/merge-m/drafts',
                                            lambda path: path.encode())

    def test_rows_in_source_order_with_flags_and_names(self):
        catalog = self.catalog(
            row(0x469000, 'population-2', 'src/recovered/a/Error_00469000.cpp', 5,
                semantic_name='Error_00469000'),
            row(0x468f50, 'population-1', 'src/recovered/a/Ctor_00468f50.cpp', 9,
                count_calibrated=True, count_calibrated_note='n'),
            row(0x468f90, 'population-1', 'src/recovered/a/Ctor_00468f90.cpp', 10),
            row(0x500000, 'population-3', 'src/recovered/b/Other_00500000.cpp', 1))
        plan, drafts = self.plan(catalog)
        self.assertEqual([i['entry'] for i in plan['sources']], ['00469000', '00468f50', '00468f90'])
        self.assertTrue(plan['all_or_nothing'])
        self.assertEqual(plan['merged_populations'], ['population-1', 'population-2'])
        self.assertNotIn('options', plan)
        first, second = plan['sources'][:2]
        self.assertEqual(first['semantic_name'], 'Error_00469000')
        self.assertEqual(first['draft'], 'build/merge-m/drafts/Error_00469000.cpp')
        self.assertEqual(first['source'], 'src/recovered/a/Error_00469000.cpp')
        self.assertTrue(second['count_calibrated'])
        self.assertEqual(second['count_calibrated_note'], 'n')
        self.assertEqual(drafts['build/merge-m/drafts/Ctor_00468f50.cpp'], b'src/recovered/a/Ctor_00468f50.cpp')

    def test_recorded_option_set_is_carried(self):
        gr = ['/O2', '/Ob2', '/G5', '/Gy', '/GR']
        catalog = self.catalog(row(1, 'population-1', 'src/recovered/x_00000001.cpp', 1, options=gr),
                               row(2, 'population-2', 'src/recovered/y_00000002.cpp', 2, options=gr))
        plan, _ = self.plan(catalog)
        self.assertEqual(plan['options'], [gr])

    def test_refuses_different_options(self):
        catalog = self.catalog(row(1, 'population-1', 'src/recovered/x_00000001.cpp', 1),
                               row(2, 'population-2', 'src/recovered/y_00000002.cpp', 2,
                                   options=['/O2', '/Ob2', '/G5', '/Gy', '/GR']))
        with self.assertRaises(SystemExit):
            self.plan(catalog)

    def test_refuses_module_sources(self):
        catalog = self.catalog(row(1, 'population-1', 'src/recovered/random.cpp', 1),
                               row(2, 'population-1', 'src/recovered/random.cpp', 2),
                               row(3, 'population-2', 'src/recovered/y_00000003.cpp', 3))
        with self.assertRaises(SystemExit):
            self.plan(catalog)

    def test_refuses_empty_or_single_population(self):
        catalog = self.catalog(row(1, 'population-1', 'src/recovered/x_00000001.cpp', 1))
        with self.assertRaises(SystemExit):
            self.plan(catalog, ('population-1',))
        with self.assertRaises(SystemExit):
            self.plan(catalog, ('population-1', 'population-9'))

    def test_refuses_colliding_file_names(self):
        catalog = self.catalog(row(1, 'population-1', 'src/recovered/a/Same.cpp', 1),
                               row(2, 'population-2', 'src/recovered/b/Same.cpp', 2))
        with self.assertRaises(SystemExit):
            self.plan(catalog)


if __name__ == '__main__':
    unittest.main()
