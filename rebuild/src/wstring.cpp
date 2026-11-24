/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2025 Electronic Arts Inc.
 * Copyright 2026 OpenTS contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/

/****************************************************************************\
*        C O N F I D E N T I A L --- W E S T W O O D   S T U D I O S         *
******************************************************************************
Project Name: Carpenter  (The RedAlert ladder creator)
File Name   : string.cpp
Author      : Neal Kettler
Start Date  : June 1, 1997
Last Update : June 17, 1997

A fairly typical string class.  This string class always copies any input
string to it's own memory (for assignment or construction).
\***************************************************************************/

#include "always.h"

#include "wstring.h"
#include "widewstring.h" // RA2: the wide string class shares this file

#include "win.h" /// needed for NULL, TRUE and FALSE

#include <cctype>
#include <cstring>
#include <cwchar> // RA2: wide string functions


/// <summary>
/// Constructs an empty string.
/// </summary>
Wstring::Wstring(void) :str(NULL)
{
}


/// <summary>
/// Constructs a string from a block of text.
/// The text is copied into memory this string owns, so the source may be discarded
/// once construction is complete.
/// </summary>
Wstring::Wstring(IN char const* string) :str(NULL)
{
	set(string);
}


/// <summary>
/// Constructs a copy of another string.
/// The text is duplicated, so the two strings share no memory afterwards.
/// </summary>
Wstring::Wstring(IN const Wstring& other) :str(NULL)
{
	if (other.str != NULL)
	{
		str = new char[strlen(other.str) + 1];
		strcpy(str, other.str);
	}
}


/// <summary>
/// Destroys the string and frees the text it owns.
/// </summary>
Wstring::~Wstring(void)
{
	clear();
}


/// <summary>
/// Compares this string against a block of text for equality.
/// </summary>
/// <returns>bool; Does the text match this string?</returns>
/// <remarks>Unless both are NULL, this string and the text supplied must each have been
/// set.</remarks>
bool Wstring::operator==(IN char const* other) const
{
	if ((str == NULL) && (other == NULL))
		return(TRUE);

	if ((str == NULL) || (other == NULL)) // RA2
		return(FALSE);

	if (strcmp(str, other) != 0)
		return(FALSE);
	else
		return(TRUE);
}


/// <summary>
/// Compares this string against another for equality.
/// Two strings that have never been set count as equal, and a string that has been set
/// never matches one that has not.
/// </summary>
/// <returns>bool; Are the two strings the same?</returns>
bool Wstring::operator==(IN Wstring const& other) const
{
	if ((str == NULL) && (other.str == NULL))
		return(TRUE);

	if ((str == NULL) || (other.str == NULL))
		return(FALSE);

	if (strcmp(str, other.str) != 0)
		return(FALSE);
	else
		return(TRUE);
}


/// <summary>
/// Compares this string against a block of text for inequality.
/// </summary>
/// <returns>bool; Does the text differ from this string?</returns>
/// <remarks>This string and the text supplied must each have been set; neither may be
/// NULL.</remarks>
bool Wstring::operator!=(IN char const* other) const
{
	if ((str == NULL) && (other == NULL)) // RA2
		return(FALSE);

	if ((str == NULL) || (other == NULL)) // RA2
		return(TRUE);

	if (strcmp(str, other) != 0)
		return(TRUE);
	else
		return(FALSE);
}


/// <summary>
/// Compares this string against another for inequality.
/// Two strings that have never been set count as equal, and a string that has been set
/// always differs from one that has not.
/// </summary>
/// <returns>bool; Do the two strings differ?</returns>
bool Wstring::operator!=(IN Wstring const& other) const
{
	if ((str == NULL) && (other.str == NULL))
		return(FALSE);

	if ((str == NULL) || (other.str == NULL))
		return(TRUE);

	if (strcmp(str, other.str) != 0)
		return(TRUE);
	else
		return(FALSE);
}


/// <summary>
/// Assigns a block of text to this string.
/// The text is copied, so the source may be discarded afterwards.
/// </summary>
/// <returns>Returns with a reference to this string.</returns>
Wstring& Wstring::operator=(char const* other)
{
	set(other);
	return(*this);
}


/// <summary>
/// Assigns another string to this one.
/// The text is copied, so the two strings share no memory afterwards.
/// </summary>
/// <returns>Returns with a reference to this string.</returns>
Wstring& Wstring::operator=(IN Wstring const& other)
{
	if (*this == other)
		return(*this);

	set(other.get());
	return(*this);
}


/// <summary>
/// Appends a block of text onto the end of the string.
/// A NULL source is quietly ignored rather than treated as an error.
/// </summary>
/// <param name="s">The text to append.</param>
char Wstring::cat(IN char const* s)
{
	char* oldStr;
	unsigned int   len;

	if (s == NULL)   // it's OK to cat nothing
		return(TRUE);

	// Save the contents of the string.
	oldStr = str;

	// Determine the length of the resultant string.
	len = strlen(s) + 1;
	if (oldStr)
		len += strlen(oldStr);

	// Allocate memory for the new string.
	str = new char[(len * sizeof(char))];

	// Copy the contents of the old string and concatenate the passed
	// string.
	if (oldStr)   strcpy(str, oldStr);
	else         str[0] = 0;

	strcat(str, s);

	// delete the old string.
	if (oldStr)
		delete[](oldStr);

	return(TRUE);
}


/// <summary>
/// Appends a fixed number of characters onto the end of the string.
/// Use this routine when the source is not necessarily terminated, such as a run of
/// characters carved out of a larger buffer.
/// </summary>
/// <param name="size">The number of characters to take from the source.</param>
/// <param name="s">The characters to append.</param>
char Wstring::cat(unsigned int size, IN char const* s)
{
	char* oldStr;
	unsigned int   len;

	// Save the contents of the string.
	oldStr = str;

	// Determine the length of the resultant string.
	len = size + 1;
	if (oldStr)
		len += strlen(oldStr);

	// Allocate memory for the new string.
	str = new char[(len * sizeof(char))];

	// Copy the contents of the old string and concatenate the passed
	// string.
	if (oldStr)
		strcpy(str, oldStr);
	else
		str[0] = 0;

	if (s)
	strncat(str, s, size);

	// delete the old string.
	if (oldStr)
		delete[](oldStr);

	return(TRUE);
}


/// <summary>
/// Appends another string onto the end of this one.
/// </summary>
char Wstring::cat(IN Wstring const& other)
{
	return(cat(other.get()));
}


/// <summary>
/// Appends a block of text onto the end of this string.
/// </summary>
/// <returns>Returns with a reference to this string, so that appends may be chained.</returns>
Wstring& Wstring::operator+=(IN char const* string)
{
	cat(string);
	return(*this);
}


/// <summary>
/// Appends another string onto the end of this one.
/// </summary>
/// <returns>Returns with a reference to this string, so that appends may be chained.</returns>
Wstring& Wstring::operator+=(IN Wstring const& other)
{
	cat(other.get());
	return(*this);
}


/// <summary>
/// Joins this string and a block of text into a new string.
/// Neither operand is disturbed.
/// </summary>
/// <returns>Returns with a new string holding this string followed by the text.</returns>
Wstring Wstring::operator+(IN char const* string) const
{
	Wstring temp = *this;
	temp.cat(string);
	return(temp);
}


/// <summary>
/// Joins two strings into a new string.
/// Neither operand is disturbed.
/// </summary>
/// <returns>Returns with a new string holding this string followed by the other.</returns>
Wstring Wstring::operator+(IN Wstring const& s) const
{
	Wstring temp = *this;
	temp.cat(s);
	return(temp);
}


//
// This function deletes 'count' characters indexed by `pos' from the Wstring.
// If `pos'+'count' is > the length of the array, the last 'count' characters
// of the string are removed.  If an error occurs, FALSE is returned.
// Otherwise, TRUE is returned.  Note: count has a default value of 1.
//
//
char Wstring::remove(int pos, int count)
{
	char* s;
	int   len;


	len = (int)strlen(str);

	if (pos + count > len)
		pos = len - count;
	if (pos < 0)
	{
		count += pos;    // If they remove before 0, ignore up till beginning
		pos = 0;
	}
	if (count <= 0)
		return(FALSE);

	s = new char[len - count + 1];

	///////DBGMSG("Wstring::remove  POS: "<<pos<<"  LEN: "<<len);

	// put nulls on both ends of substring to be removed
	str[pos] = 0;
	str[pos + count - 1] = 0;

	strcpy(s, str);
	strcat(s, &(str[pos + count]));
	delete[](str);
	str = s;

	return(TRUE);
}


// Remove all instances of a char from the string
char Wstring::removeChar(char c)
{
	int     len = 0;
	char* cptr = NULL;
	char    removed = FALSE;

	if (str == NULL)
		return(FALSE);

	len = strlen(str);
	while ((cptr = strchr(str, c)) != NULL)
	{
		memmove(cptr, cptr + 1, len - 1 - ((int)(cptr - str)));
		len--;
		str[len] = 0;
		removed = TRUE;
	}
	if (removed)
	{
		char* newStr = new char[strlen(str) + 1];
		strcpy(newStr, str);
		delete[](str);
		str = newStr;
	}
	return(removed);
}


/// <summary>
/// Removes all white space from the string.
/// Both spaces and tabs are stripped out, wherever in the string they occur.
/// </summary>
void Wstring::removeSpaces(void)
{
	removeChar(' ');
	removeChar('\t');
}


/// <summary>
/// RA2: Removes the leading characters that occur in the delimiter string.
/// </summary>
void Wstring::trimLeading(IN Wstring const& delim)
{
	int i = 0;

	while (i < (int)length() && strchr(delim.get(), str[i]))
		i++;
	if (i > 0)
		remove(0, i);
}


/// <summary>
/// RA2: Removes the trailing characters that occur in the delimiter string.
/// </summary>
void Wstring::trimTrailing(IN Wstring const& delim)
{
	unsigned int i = length();

	while (i--)
	{
		if (!strchr(delim.get(), str[i]))
			break;
	}
	truncate(i + 1);
}


/// <summary>
/// Frees the text and leaves the string empty.
/// </summary>
void Wstring::clear(void)
{
	if (str)
		delete[](str);
	str = NULL;
}


/// <summary>
/// Resizes the string to an empty buffer of the size requested.
/// Any previous contents are discarded. Use this routine when the string is about to be
/// filled in by other code writing through the raw text pointer.
/// </summary>
/// <param name="size">The number of bytes to reserve, terminator included.</param>
void Wstring::setSize(int size)
{
	clear();
	if (size < 0)
		return;
	str = new char[size];
	memset(str, 0, size);
}


/// <summary>
/// Copies the string into a fixed width field.
/// This routine is used to lay text out in columns -- the destination is padded out to
/// the requested width with spaces.
/// </summary>
/// <param name="dest">The buffer to copy the text into.</param>
/// <param name="len">The width of the field to fill.</param>
/// <remarks>Be sure the destination buffer is big enough for the field width plus a
/// terminator.</remarks>
void Wstring::cellCopy(char* dest, unsigned int len) const
{
	unsigned int i;

	strncpy(dest, str, len);
	for (i = (unsigned int)strlen(str); i < len; i++)
		dest[i] = ' ';
	dest[len] = 0;
}


/// <summary>
/// Fetches the raw text of the string.
/// Use this routine to hand the string to the C library, or to any other code that deals
/// in plain character pointers.
/// </summary>
/// <returns>Returns with a pointer to the text. A string that was never set yields an
/// empty string rather than NULL.</returns>
char* Wstring::get(void) const
{
	if (!str)
		return((char *)"");
	return(str);
}


/// <summary>
/// Fetches a single character out of the string.
/// </summary>
/// <param name="index">The position of the character to fetch.</param>
/// <returns>Returns with the character found, or zero if the position lies past the end
/// of the string.</returns>
char Wstring::get(unsigned int index)
{
	if (index < strlen(str))
		return(str[index]);
	return(0);
}


/// <summary>
/// RA2: Fetches the text, or an empty string when there is none.
/// </summary>
char* Wstring::text(void) const
{
	if (!str)
		return((char *)"");
	return(str);
}


/// <summary>
/// RA2: Fetches the character at the index, or zero past the end.
/// </summary>
char Wstring::operator[](unsigned int index) const
{
	if (index < strlen(str))
		return(str[index]);
	return(0);
}


/// <summary>
/// RA2: Fetches the character at the index, or zero past the end.
/// </summary>
char Wstring::getChar(unsigned int index) const
{
	if (index < strlen(str))
		return(str[index]);
	return(0);
}


/// <summary>
/// Fetches the length of the string.
/// </summary>
/// <returns>Returns with the number of characters in the string.</returns>
unsigned int Wstring::length(void) const
{
	if (str == NULL)
		return(0);
	return((unsigned int)strlen(str));
}


// Insert at given position and shift old stuff to right
char Wstring::insert(char const* instring, unsigned int pos)
{
	if (str == NULL)
		return(set(instring));
	if (pos > strlen(str))
		pos = strlen(str);
	char* newstr = new char[strlen(str) + strlen(instring) + 1];
	memset(newstr, 0, strlen(str) + strlen(instring) + 1);
	strcpy(newstr, str);
	// move the old data out of the way
	int bytes = strlen(str) + 1 - pos;
	memmove(newstr + pos + strlen(instring), newstr + pos, bytes);
	// move the new data into place
	memmove(newstr + pos, instring, strlen(instring));
	delete[](str);
	str = newstr;
	return(TRUE);
}


// This function inserts the character specified by `k' into the string at the
// position indexed by `pos'.  If `pos' is >= the length of the string, it is
// appended to the string.  If an error occurs, FALSE is returned.  Otherwise,
// TRUE is returned.
char Wstring::insert(char k, unsigned int pos)
{
	char* s;
	unsigned int   len;
	char     c[2];

	if (!str)
	{
		c[0] = k;
		c[1] = 0;
		return(set(c));
	}

	len = (unsigned int)strlen(str);

	if (pos > len)
		pos = len;

	s = (char*)new char[(len + 2)];

	c[0] = str[pos];
	str[pos] = 0;
	c[1] = 0;
	strcpy(s, str);
	str[pos] = c[0];
	c[0] = k;
	strcat(s, c);
	strcat(s, &(str[pos]));
	delete[](str);
	str = s;

	return(TRUE);
}


// This function replaces any occurences of the string pointed to by
// `replaceThis' with the string pointed to by `withThis'.  If an error
// occurs, FALSE is returned.  Otherwise, TRUE is returned.
char Wstring::replace(char const* replaceThis, char const* withThis)
{
	Wstring  dest;
	char* foundStr, * src;
	unsigned int   len;

	src = get();
	while (src && src[0])
	{
		foundStr = strstr(src, replaceThis);
		if (foundStr)
		{
			len = (unsigned int)foundStr - (unsigned int)src;
			if (len)
			{
				if (!dest.cat(len, src))
					return(FALSE);
			}
			if (!dest.cat(withThis))
				return(FALSE);
			src = foundStr + strlen(replaceThis);
		}
		else
		{
			if (!dest.cat(src))
				return(FALSE);

			src = NULL;
		}
	}
	return(set(dest.get()));
}


/// <summary>
/// Sets the string to a copy of the text supplied.
/// The string always owns its own memory, so the source may be discarded once this
/// routine returns. Any previous contents are freed.
/// </summary>
/// <param name="s">The text to copy.</param>
char Wstring::set(IN char const* s)
{
	unsigned int len;

	clear();

	len = s ? (unsigned int)strlen(s) + 1 : 1; // RA2: a NULL string sets an empty one

	str = new char[len];
	str[0] = 0; // RA2
	if (s)
		strcpy(str, s);

	return(TRUE);
}


/// <summary>
/// Sets a single character within the string.
/// </summary>
/// <param name="c">The character to store.</param>
/// <param name="index">The position within the string to store it at.</param>
/// <returns>Returns true if the character was stored, false if the position lies past the
/// end of the string.</returns>
char Wstring::set(char c, unsigned int index)
{
	if (index >= (unsigned int)strlen(str))
		return(FALSE);

	str[index] = c;

	return(TRUE);
}


/// <summary>
/// Sets the string from a fixed number of characters.
/// Use this routine when the source is not necessarily terminated, such as a run of
/// characters carved out of a larger buffer. The copy is always terminated.
/// </summary>
/// <param name="size">The number of characters to copy from the source.</param>
/// <param name="string">The characters to copy.</param>
char Wstring::set(unsigned int size, IN char const* string)
{
	unsigned int len;

	clear();
	len = size + 1;

	str = new char[len];

	// Copy the bytes in the string, and NULL-terminate it.
	str[0] = 0; // RA2
	if (string)
	strncpy(str, string, size);
	str[size] = 0;

	return(TRUE);
}


// This function converts all alphabetical characters in the string to lower
// case.
void Wstring::toLower(void)
{
	unsigned int i;

	for (i = 0; i < length(); i++)
	{
		if ((str[i] >= 'A') && (str[i] <= 'Z'))
			str[i] = tolower(str[i]);
	}
}


/// <summary>
/// RA2: Fetches a copy of the text in lower case.
/// </summary>
Wstring Wstring::toLower(void) const
{
	Wstring tmp = *this;
	tmp.toLower();
	return(tmp);
}


// This function converts all alphabetical characters in the string to upper
// case.
void Wstring::toUpper(void)
{
	unsigned int i;

	for (i = 0; i < length(); i++)
	{
		if ((str[i] >= 'a') && (str[i] <= 'z'))
			str[i] = toupper(str[i]);
	}
}


/// <summary>
/// RA2: Fetches a copy of the text in upper case.
/// </summary>
Wstring Wstring::toUpper(void) const
{
	Wstring tmp = *this;
	tmp.toUpper();
	return(tmp);
}


//  This function truncates the string so its length will match the specified
// `len'.  If an error occurs, FALSE is returned.  Otherwise, TRUE is returned.
char Wstring::truncate(unsigned int len)
{
	Wstring tmp;
	if (!tmp.set(len, get()) || !set(tmp.get()))
		return(FALSE);
	return(TRUE);
}


// Truncate the string after the character 'c' (gets rid of 'c' as well)
//   Do nothing if 'c' isn't in the string
char Wstring::truncate(char c)
{
	int  len;

	if (str == NULL)
		return(FALSE);

	char* cptr = strchr(str, c);
	if (cptr == NULL)
		return(FALSE);
	len = (int)(cptr - str);
	truncate((unsigned int)len);
	return(TRUE);
}


// Get a token from this string that's seperated by one or more
//  chars from the 'delim' string , start at offset & return offset
int Wstring::getToken(int offset, char const* delim, Wstring& out) const
{
	int i;
	int start;
	int stop;

	if (offset < 0)
		return(-1);

	for (i = offset; i < (int)length(); i++) {
		if (strchr(delim, str[i]) == NULL)
			break;
	}
	if (i >= (int)length())
		return(-1);
	start = i;

	for (; i < (int)length(); i++) {
		if (strchr(delim, str[i]) != NULL)
			break;
	}
	stop = i - 1;
	out.set(str + start);
	out.truncate((unsigned int)stop - start + 1);
	return(stop + 1);
}


// Get the first line of text after offset.  Lines are terminated by '\r\n' or '\n'
int Wstring::getLine(int offset, Wstring& out) const
{
	int i;
	int start;
	int stop;

	start = i = offset;
	if (start >= (int)length())
		return(-1);

	for (; i < (int)length(); i++) {
		if (strchr("\r\n", str[i]) != NULL)
			break;
	}
	stop = i;
	if ((str[stop] == '\r') && (str[stop + 1] == '\n'))
		stop++;

	out.set(str + start);
	out.truncate((unsigned int)stop - start + 1);
	return(stop + 1);
}

/// <summary>
/// RA2: Converts the text to an integer.
/// </summary>
int Wstring::toInt(void) const
{
	return(atoi(str));
}

/// <summary>
/// RA2: Builds a string from an integer.
/// </summary>
Wstring Wstring_FromInt(int value)
{
	char buf[16];
	sprintf(buf, "%d", value);
	return(Wstring(buf));
}

/// <summary>
/// RA2: Builds a string from an integer, zero padded to the width.
/// </summary>
Wstring Wstring_FromInt(int value, int width)
{
	char buf[16];
	sprintf(buf, "%%0%dd", width);
	sprintf(buf, buf, value);
	return(Wstring(buf));
}

/// <summary>
/// RA2: Joins a plain string and a Wstring.
/// </summary>
Wstring operator+(char const* a, Wstring const& b)
{
	Wstring tmp(a);
	return(tmp + b);
}

/// <summary>
/// RA2: Joins two strings.
/// </summary>
Wstring operator+(Wstring const& a, Wstring const& b)
{
	Wstring tmp = a;
	tmp.cat(b);
	return(tmp);
}

/// <summary>
/// RA2: Checks whether the text holds any of the characters.
/// </summary>
bool Wstring::containsAny(char const* chars) const
{
	if (chars == NULL || str == NULL)
		return(false);
	unsigned int len = strlen(str);
	return(strcspn(str, chars) < len);
}

/// <summary>
/// RA2: Checks whether the text holds any of the characters.
/// </summary>
bool Wstring::containsAny(Wstring const& chars) const
{
	return(containsAny(chars.get()));
}


/// <summary>
/// RA2: Constructs an empty wide string.
/// </summary>
WideWstring::WideWstring(void) :str(NULL)
{
}


/// <summary>
/// RA2: Constructs a wide string from a block of text.
/// </summary>
WideWstring::WideWstring(wchar_t const * string) :str(NULL)
{
	*this = string; // RA2: copies through the assignment
}


/// <summary>
/// RA2: Constructs a copy of another wide string.
/// </summary>
WideWstring::WideWstring(WideWstring const & other) :str(NULL)
{
	if (other.str != NULL)
	{
		str = new wchar_t[wcslen(other.str) + 1];
		wcscpy(str, other.str);
	}
}


/// <summary>
/// RA2: Destroys the wide string and frees the text it owns.
/// </summary>
WideWstring::~WideWstring(void)
{
	if (str)
		delete (str);
	str = NULL;
}


/// <summary>
/// RA2: Compares against a block of text for equality.
/// </summary>
bool WideWstring::operator==(wchar_t const * other) const
{
	if ((str == NULL) && (other == NULL))
		return(true);

	if ((str != NULL) && (other != NULL))
		return(wcscmp(str, other) == 0 ? true : false);

	return(false);
}


/// <summary>
/// RA2: Compares against another wide string for equality.
/// </summary>
bool WideWstring::operator==(WideWstring const & other) const
{
	if ((str == NULL) && (other.str == NULL))
		return(true);

	if ((str != NULL) && (other.str != NULL))
		return(wcscmp(str, other.str) == 0 ? true : false);

	return(false);
}


/// <summary>
/// RA2: Compares against a block of text for inequality.
/// </summary>
bool WideWstring::operator!=(wchar_t const * other) const
{
	if ((str == NULL) && (other == NULL))
		return(false);

	if ((str == NULL) || (other == NULL))
		return(true);

	if (wcscmp(str, other) != 0)
		return(true);
	else
		return(false);
}


/// <summary>
/// RA2: Compares against another wide string for inequality.
/// </summary>
bool WideWstring::operator!=(WideWstring const & other) const
{
	if ((str == NULL) && (other.str == NULL))
		return(false);

	if ((str == NULL) || (other.str == NULL))
		return(true);

	if (wcscmp(str, other.str) != 0)
		return(true);
	else
		return(false);
}


/// <summary>
/// RA2: Replaces the text with an owned copy of a block of text.
/// </summary>
WideWstring & WideWstring::operator=(wchar_t const * other)
{
	set(other);
	return(*this);
}


/// <summary>
/// RA2: Replaces the text with an owned copy of another wide string.
/// </summary>
WideWstring & WideWstring::operator=(WideWstring const & other)
{
	if ((str == NULL) && (other.str == NULL)) // RA2: compares the pointers and wcscmp here instead of operator==
		return(*this);

	if ((str != NULL) && (other.str != NULL) && (wcscmp(str, other.str) == 0))
		return(*this);

	set(other.get());
	return(*this);
}


/// <summary>
/// RA2: Appends a block of text.
/// </summary>
char WideWstring::cat(wchar_t const * s)
{
	wchar_t * oldStr;
	unsigned int len;

	if (s == NULL)
		return(TRUE);

	oldStr = str;

	len = wcslen(s) + 1;
	if (oldStr)
		len += wcslen(oldStr);

	str = new wchar_t[len * 2]; // RA2: the allocation is twice the characters needed

	if (oldStr)
		wcscpy(str, oldStr);
	else
		str[0] = 0;
	wcscat(str, s);

	if (oldStr)
		delete (oldStr);

	return(TRUE);
}


/// <summary>
/// RA2: Appends a fixed number of characters of a block of text.
/// </summary>
char WideWstring::cat(unsigned int size, wchar_t const * s)
{
	wchar_t * oldStr;
	unsigned int len;

	oldStr = str;

	len = size + 1;
	if (oldStr)
		len += wcslen(oldStr);

	str = new wchar_t[len * 2];

	if (oldStr)
		wcscpy(str, oldStr);
	else
		str[0] = 0;
	if (s)
		wcsncat(str, s, size);

	if (oldStr)
		delete (oldStr);

	return(TRUE);
}


/// <summary>
/// RA2: Appends another wide string.
/// </summary>
char WideWstring::cat(WideWstring const & other)
{
	return(cat(other.get()));
}


/// <summary>
/// RA2: Appends a block of text.
/// </summary>
WideWstring & WideWstring::operator+=(wchar_t const * string)
{
	cat(string);
	return(*this);
}


/// <summary>
/// RA2: Appends another wide string.
/// </summary>
WideWstring & WideWstring::operator+=(WideWstring const & other)
{
	cat(other);
	return(*this);
}


/// <summary>
/// RA2: Joins the text and a block of text.
/// </summary>
WideWstring WideWstring::operator+(wchar_t const * string) const
{
	WideWstring temp = *this;
	temp.cat(string);
	return(temp);
}


/// <summary>
/// RA2: Joins the text and another wide string.
/// </summary>
WideWstring WideWstring::operator+(WideWstring const & s) const
{
	WideWstring temp = *this;
	temp.cat(s);
	return(temp);
}


/// <summary>
/// RA2: Deletes count characters from a position (see Wstring::remove).
/// </summary>
wchar_t WideWstring::remove(int pos, int count)
{
	wchar_t * s;
	int len;

	len = (int)wcslen(str);
	if (pos + count > len)
		pos = len - count;
	if (pos < 0)
	{
		count += pos;
		pos = 0;
	}
	if (count <= 0)
		return(FALSE);

	s = new wchar_t[len - count + 1];

	str[pos] = 0;
	str[pos + count - 1] = 0;
	wcscpy(s, str);
	wcscat(s, &(str[pos + count]));
	delete (str);
	str = s;

	return(TRUE);
}


/// <summary>
/// RA2: Removes every instance of a character.
/// </summary>
char WideWstring::removeChar(wchar_t c)
{
	int len = 0;
	wchar_t * cptr = NULL;
	char removed = FALSE;

	if (str == NULL)
		return(FALSE);

	len = wcslen(str);
	while ((cptr = (wchar_t *)wcschr((wchar_t const *)str, c)) != NULL)
	{
		memmove(cptr, cptr + 1, (len - 1) * sizeof(wchar_t) - ((int)(cptr - str)));
		len--;
		str[len] = 0;
		removed = TRUE;
	}
	if (removed)
	{
		wchar_t * newStr = new wchar_t[wcslen(str) + 1];
		wcscpy(newStr, str);
		delete[] (str);
		str = newStr;
	}
	return(removed);
}


/// <summary>
/// RA2: Removes the spaces and tabs.
/// </summary>
void WideWstring::removeSpaces(void)
{
	removeChar(L' ');
	removeChar(L'\t');
}


/// <summary>
/// RA2: Removes the leading characters found in delim.
/// </summary>
void WideWstring::trimLeading(WideWstring const & delim)
{
	unsigned int i = 0;

	while (i < length())
	{
		wchar_t const * d = delim.str;
		if (!d) d = L"";
		if (!wcschr(d, str[i]))
			break;
		i++;
	}
	if (i > 0)
		remove(0, i);
}


/// <summary>
/// RA2: Removes the trailing characters found in delim.
/// </summary>
void WideWstring::trimTrailing(WideWstring const & delim)
{
	unsigned int i = length();

	while (i--)
	{
		wchar_t const * d = delim.str;
		if (!d) d = L"";
		if (!wcschr(d, str[i]))
			break;
	}
	truncate(i + 1);
}


/// <summary>
/// RA2: Frees the text.
/// </summary>
void WideWstring::clear(void)
{
	if (str)
		delete (str);
	str = NULL;
}


/// <summary>
/// RA2: Replaces the text with an empty buffer of the size given.
/// </summary>
void WideWstring::setSize(int size)
{
	clear();
	if (size < 0)
		return;
	str = new wchar_t[size];
	memset(str, 0, size * 2);
}


/// <summary>
/// RA2: Copies into a space padded cell of len characters.
/// </summary>
void WideWstring::cellCopy(wchar_t * dest, unsigned int len) const
{
	unsigned int i;

	wcsncpy(dest, str, len);
	for (i = (unsigned int)wcslen(str); i < len; i++)
		dest[i] = L' ';
	dest[len] = 0;
}


/// <summary>
/// RA2: The text, or an empty string for none.
/// </summary>
wchar_t * WideWstring::get(void) const
{
	if (!str)
		return((wchar_t *)L"");
	return(str);
}


/// <summary>
/// RA2: The character at an index, 0 past the end.
/// </summary>
wchar_t WideWstring::get(unsigned int index)
{
	if (index < wcslen(str))
		return(str[index]);
	return(0);
}


/// <summary>
/// RA2: The text, or an empty string for none.
/// </summary>
wchar_t * WideWstring::text(void) const
{
	if (!str)
		return((wchar_t *)L"");
	return(str);
}


/// <summary>
/// RA2: The character at an index, 0 past the end.
/// </summary>
wchar_t WideWstring::operator[](unsigned int index) const
{
	if (index < wcslen(str))
		return(str[index]);
	return(0);
}


/// <summary>
/// RA2: The character at an index, 0 past the end.
/// </summary>
wchar_t WideWstring::getChar(unsigned int index) const
{
	return(index < wcslen(str) ? str[index] : (wchar_t)0); // RA2: the conditional with a 16 bit zero gives xor eax, eax (the other index accessors give xor ax, ax)
}


/// <summary>
/// RA2: The number of characters.
/// </summary>
unsigned int WideWstring::length(void) const
{
	if (str == NULL)
		return(0);
	return((unsigned int)wcslen(str));
}


/// <summary>
/// RA2: Inserts a block of text at a position.
/// </summary>
char WideWstring::insert(wchar_t const * instring, unsigned int pos)
{
	if (str == NULL)
		return(set(instring));

	if (pos > wcslen(str))
		pos = wcslen(str);

	wchar_t * newstr = new wchar_t[wcslen(str) + wcslen(instring) + 1];
	memset(newstr, 0, (wcslen(str) + wcslen(instring) + 1) * 2); // RA2: the sizes count bytes
	wcscpy(newstr, str);

	int chars = wcslen(str) + 1 - pos;
	memmove(newstr + pos + wcslen(instring), newstr + pos, chars * 2);

	memmove(newstr + pos, instring, wcslen(instring) * 2);

	delete (str);
	str = newstr;
	return(TRUE);
}


/// <summary>
/// RA2: Inserts a character at a position.
/// </summary>
char WideWstring::insert(wchar_t k, unsigned int pos)
{
	wchar_t * s;
	unsigned int len;
	wchar_t c[2];

	if (!str)
	{
		c[0] = k;
		c[1] = 0;
		return(set(c));
	}

	len = (unsigned int)wcslen(str);
	if (pos > len)
		pos = len;

	s = (wchar_t *)new wchar_t[(len + 2)];

	c[0] = str[pos];
	str[pos] = 0;
	c[1] = 0;
	wcscpy(s, str);
	str[pos] = c[0];
	c[0] = k;
	wcscat(s, c);
	wcscat(s, &(str[pos]));
	delete (str);
	str = s;
	return(TRUE);
}


/// <summary>
/// RA2: Replaces every occurrence of replaceThis with withThis.
/// </summary>
char WideWstring::replace(wchar_t const * replaceThis, wchar_t const * withThis)
{
	WideWstring dest;
	wchar_t *foundStr, *src;
	unsigned int len;

	src = get();
	while (src && src[0])
	{
		foundStr = wcsstr(src, replaceThis);
		if (foundStr)
		{
			len = (unsigned int)foundStr - (unsigned int)src;
			if (len)
			{
				if (!dest.cat(len, src)) // RA2: the byte distance is passed as the count
					return(FALSE);
			}
			if (!dest.cat(withThis))
				return(FALSE);
			src = (wchar_t *)((unsigned int)foundStr + wcslen(replaceThis) * 2);
		}
		else
		{
			if (!dest.cat(src))
				return(FALSE);

			src = NULL;
		}
	}
	return(set(dest.get()));
}


/// <summary>
/// RA2: Sets the text to an owned copy of a block of text.
/// </summary>
wchar_t WideWstring::set(wchar_t const * s)
{
	unsigned int len;

	clear();

	len = s ? (unsigned int)wcslen(s) + 1 : 1;

	str = new wchar_t[len];
	str[0] = 0;
	if (s)
		wcscpy(str, s);

	return(TRUE);
}


/// <summary>
/// RA2: Sets one character.
/// </summary>
char WideWstring::set(wchar_t c, unsigned int index)
{
	if (index >= (unsigned int)wcslen(str))
		return(FALSE);

	str[index] = c;

	return(TRUE);
}


/// <summary>
/// RA2: Sets the text from a fixed number of characters.
/// </summary>
wchar_t WideWstring::set(unsigned int size, wchar_t const * string)
{
	unsigned int len;

	clear();
	len = size + 1;

	str = new wchar_t[len];

	str[0] = 0;
	if (string)
		wcsncpy(str, string, size);
	str[size] = 0;

	return(TRUE);
}


/// <summary>
/// RA2: Converts the text to lower case.
/// </summary>
void WideWstring::toLower(void)
{
	unsigned int i;

	for (i = 0; i < length(); i++)
	{
		if ((str[i] >= L'A') && (str[i] <= L'Z'))
			str[i] = tolower(str[i]);
	}
}


/// <summary>
/// RA2: Fetches a copy of the text in lower case.
/// </summary>
WideWstring WideWstring::toLower(void) const
{
	WideWstring tmp = *this;
	tmp.toLower();
	return(tmp);
}


/// <summary>
/// RA2: Converts the text to upper case.
/// </summary>
void WideWstring::toUpper(void)
{
	unsigned int i;

	for (i = 0; i < length(); i++)
	{
		if ((str[i] >= L'a') && (str[i] <= L'z'))
			str[i] = toupper(str[i]);
	}
}


/// <summary>
/// RA2: Fetches a copy of the text in upper case.
/// </summary>
WideWstring WideWstring::toUpper(void) const
{
	WideWstring tmp = *this;
	tmp.toUpper();
	return(tmp);
}


/// <summary>
/// RA2: Truncates the text to len characters.
/// </summary>
char WideWstring::truncate(unsigned int len)
{
	WideWstring tmp;
	if (!tmp.set(len, get()) || !set(tmp.get()))
		return(FALSE);
	return(TRUE);
}


/// <summary>
/// RA2: Truncates the text at (and including) the character c.
/// </summary>
char WideWstring::truncate(wchar_t c)
{
	int len;

	if (str == NULL)
		return(FALSE);

	wchar_t * cptr = wcschr(str, c);
	if (cptr == NULL)
		return(FALSE);
	len = (int)(cptr - str);
	truncate((unsigned int)len);
	return(TRUE);
}


/// <summary>
/// RA2: Gets the token after an offset (see Wstring::getToken).
/// </summary>
int WideWstring::getToken(int offset, wchar_t const * delim, WideWstring & out) const
{
	int i;
	int start;
	int stop;

	if (offset < 0)
		return(-1);

	for (i = offset; i < (int)length(); i++) {
		if (wcschr(delim, str[i]) == NULL)
			break;
	}
	if (i >= (int)length())
		return(-1);
	start = i;

	for (; i < (int)length(); i++) {
		if (wcschr(delim, str[i]) != NULL)
			break;
	}
	stop = i - 1;
	out.set(str + start);
	out.truncate((unsigned int)stop - start + 1);
	return(stop + 1);
}


/// <summary>
/// RA2: Gets the line after an offset (see Wstring::getLine).
/// </summary>
int WideWstring::getLine(int offset, WideWstring & out) const
{
	int i;
	int start;
	int stop;

	start = i = offset;
	if (start >= (int)length())
		return(-1);

	for (; i < (int)length(); i++) {
		if (strchr("\r\n", str[i]) != NULL) // RA2: the line break characters are searched as a narrow string (the character is widened to int)
			break;
	}
	stop = i;
	if ((str[stop] == L'\r') && (str[stop + 1] == L'\n'))
		stop++;

	out.set(str + start);
	out.truncate((unsigned int)stop - start + 1);
	return(stop + 1);
}


/// <summary>
/// RA2: Converts the text to an integer.
/// </summary>
int WideWstring::toInt(void) const
{
	return(wcstol(str, NULL, 10));
}


/// <summary>
/// RA2: Checks whether every character is below 0x100 (true for no text).
/// </summary>
bool WideWstring::Is_Narrow(void) const
{
	if (str != NULL)
	{
		int len = wcslen(str);
		for (int i = 0; i < len; i++)
		{
			if (str[i] >= 0x100)
				return(false);
		}
	}
	return(true);
}


/// <summary>
/// RA2: Builds a wide string from an integer.
/// </summary>
WideWstring __fastcall WideWstring_FromInt(int value)
{
	wchar_t buf[32];
	swprintf(buf, L"%d", value);
	return(WideWstring(buf));
}


/// <summary>
/// RA2: Builds a wide string from an integer, zero padded to the width.
/// </summary>
WideWstring __fastcall Int_To_WideString_Width(int value, int width)
{
	wchar_t buf[32];
	swprintf(buf, L"%%0%dd", width);
	swprintf(buf, buf, value);
	return(WideWstring(buf));
}


void __fastcall Char_To_Wide(wchar_t* dest, char const* source, int limit); // RA2: 0x006FA0C0 (campaign.cpp), widens a narrow string
namespace OwnerDraw { // RA2: as conquer.cpp
	void __fastcall WideToCharString(char* dest, wchar_t const* src, int count); // RA2: 0x006FA0F0, narrows a wide string
}
int __fastcall Bit_Stuff(unsigned char* source, unsigned char* dest, int count); // RA2: 0x00430DA0 (bfiofile2.cpp)
unsigned int __fastcall Bit_Unstuff_String(unsigned char* source, wchar_t* dest, unsigned int count); // RA2: 0x00430F30 (bfiofile2.cpp)


/// <summary>
/// RA2: Widens a narrow string: the text goes through a scratch buffer into the result.
/// </summary>
WideWstring __fastcall Wstring_ToWide(Wstring const & string)
{
	int len = string.length();
	wchar_t * buffer = new wchar_t[len + 1];
	Char_To_Wide(buffer, string.get(), len);
	WideWstring wide(buffer);
	delete (buffer);
	return(wide);
}


/// <summary>
/// RA2: Narrows a wide string: the text goes through a scratch buffer into the result.
/// </summary>
Wstring __fastcall Wstring_Narrow(WideWstring const & wide)
{
	int len = wide.length();
	char * buffer = new char[len + 1];
	OwnerDraw::WideToCharString(buffer, wide.get(), len);
	Wstring narrow(buffer);
	delete (buffer);
	return(narrow);
}


/// <summary>
/// RA2: Joins a block of text and a wide string.
/// </summary>
WideWstring __fastcall operator+(wchar_t const * a, WideWstring const & b)
{
	return(WideWstring(a).operator+(b));
}


/// <summary>
/// RA2: Joins two wide strings.
/// </summary>
WideWstring __fastcall operator+(WideWstring const & a, WideWstring const & b)
{
	WideWstring tmp(a);
	tmp.cat(b.get());
	return(tmp);
}


/// <summary>
/// RA2: Checks whether the text holds any of the characters.
/// </summary>
bool WideWstring::containsAny(wchar_t const * chars) const
{
	if (chars != NULL && str != NULL)
	{
		unsigned int len = wcslen(str);
		return(wcscspn(str, chars) < len);
	}
	return(false);
}


/// <summary>
/// RA2: Checks whether the text holds any of the characters.
/// </summary>
bool WideWstring::containsAny(WideWstring const & chars) const
{
	return(containsAny(chars.get()));
}


/// <summary>
/// RA2: Packs the 16 bit characters into a string of 7 bit characters (Bit_Stuff).
/// </summary>
Wstring __fastcall Wstring_FromWide(WideWstring const & wide)
{
	int bits = (wide.length() + 1) * 16;
	unsigned char * buffer = new unsigned char[(bits + 6) / 7];
	Bit_Stuff((unsigned char *)wide.get(), buffer, bits);
	Wstring packed((char *)buffer);
	delete (buffer);
	return(packed);
}


/// <summary>
/// RA2: Unpacks a string of 7 bit characters into 16 bit characters (Bit_Unstuff_String).
/// </summary>
WideWstring __fastcall Wide_From_7_Bit(Wstring const & string)
{
	int bytes = (string.length() + 1) * 7 / 8;
	int count = (bytes + 1) / 2;
	wchar_t * buffer = new wchar_t[count + 1];
	Bit_Unstuff_String((unsigned char *)string.get(), buffer, count);
	WideWstring wide(buffer);
	delete (buffer);
	return(wide);
}
