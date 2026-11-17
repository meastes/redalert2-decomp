// void unknown_open_tournament_page() -- 0x0073C2A0
// Purpose: opens the Westwood Online Red Alert 2 tournaments page through
// 0x0073C580 (a URL opener; second argument 0).
// Assumptions: the opener's second argument is unknown.

void __fastcall unknown_open_url_73c580(char const *url, int arg2);    // 0x0073C580

void unknown_open_tournament_page()
{
    unknown_open_url_73c580("http://www.westwood.com/westwoodonline/tournaments/redalert2/", 0);
}
