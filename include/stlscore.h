/*
**	Command & Conquer Red Alert 2
**	STLSCORE.H: the score screen of a single player mission.
*/

#pragma once

/*
**	The score screen shown when a single player mission is won: the art is loaded, the
**	dialog (resource 0x103) reports the par time, the time taken and the kills, and the
**	screen is drawn once more as the dialog closes.
*/
class STLScoreScreen
{
	public:
		STLScoreScreen(void);

		void Present(void);
};
