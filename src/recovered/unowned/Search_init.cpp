// Search (0x008315C8), defined by the unit at 0x0040ACB0: its dynamic initializer
// calls the out-of-line constructor (0x427FB0) and registers the atexit
// destructor (the next draft), which jumps to the destructor (0x4281E0).
// @ref ??0AStarClass@@QAE@XZ=00427fb0
// @ref ??1AStarClass@@QAE@XZ=004281e0
// @ref ?Search@@3UUnknownPathEstimator@@A=008315c8
#include "shared/path_estimator.hpp"

AStarClass Search;
