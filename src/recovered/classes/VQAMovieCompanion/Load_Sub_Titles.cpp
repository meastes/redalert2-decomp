// bool VQAMovieCompanion::Load_Sub_Titles(char const *moviename) (0x0069B300): the movie's section of
// subtitle.txt; false when there is none.
bool VQAMovieCompanion::Load_Sub_Titles(char const *moviename)
{
    if (moviename != 0 && strlen(moviename) != 0) {
        CCFileClass file("subtitle.txt");
        if (file.Is_Available()) {
            FileStraw input(file);
            SubTitleParser parser(input);
            SubTitles = parser.Get_Sub_Titles(moviename);
            if (SubTitles == 0) {
                return false;
            }
            return true;
        }
    }
    return false;
}
