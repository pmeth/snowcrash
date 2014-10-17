//
//  TrimString.h
//  snowcrash
//
//  Created by Zdenek Nemec on 5/11/13.
//  Copyright (c) 2013 Apiary Inc. All rights reserved.
//
//  Credits:
//  http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring

#ifndef SNOWCRAH_TRIMSTRING_H
#define SNOWCRAH_TRIMSTRING_H

#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
#include <string>
#include <sstream>
#include <vector>

namespace snowcrash {

    // Check a character not to be an space of any kind
    inline bool isSpace(const std::string::value_type i){
        if(i == ' ' || i == '\t' || i == '\n' || i == '\v' || i == '\f' || i == '\r')
            return true;
        return false;
    }

    // Trim string from start
    inline std::string& TrimStringStart(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun(isSpace))));
        return s;
    }

    // Trim string from end
    inline std::string& TrimStringEnd(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun(isSpace))).base(), s.end());
        return s;
    }

    // Trim both ends of string
    inline std::string& TrimString(std::string &s) {
        return TrimStringStart(TrimStringEnd(s));
    }

    // Split string by delim
    inline std::vector<std::string>& Split(const std::string& s, char delim, std::vector<std::string>& elems) {
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim)) {
            elems.push_back(item);
        }
        return elems;
    }

    // Split string by delim
    inline std::vector<std::string> Split(const std::string& s, char delim) {
        std::vector<std::string> elems;
        Split(s, delim, elems);
        return elems;
    }

    // Split string on the first occurrence of delim
    inline std::vector<std::string> SplitOnFirst(const std::string& s, char delim) {
        std::string::size_type pos = s.find(delim);
        std::vector<std::string> elems;
        if (pos == std::string::npos) {
            elems.push_back(s);
        }
        else {
            elems.push_back(s.substr(0, pos));
            elems.push_back(s.substr(pos + 1, std::string::npos));
        }
        return elems;
    }

    // Make sure last two characters are newlines
    inline std::string& TwoNewLines(std::string& s) {

        if (s[s.length() - 1] != '\n') {
            s += "\n";
        }

        if (s[s.length() - 2] != '\n') {
            s += "\n";
        }

        return s;
    }

    /**
     *  \brief  Replace all occurrences of a string.
     *  \param  s       A string to search in.
     *  \param  find    A string to look for.
     *  \param  replace A string to replace with.
     *  \return A copy of %s with all occurrences of %find replaced by %replace.
     */
    inline std::string ReplaceString(const std::string& s,
                                     const std::string& find,
                                     const std::string& replace) {
        size_t pos = 0;
        std::string target(s);
        while ((pos = target.find(find, pos)) != std::string::npos) {
            target.replace(pos, find.length(), replace);
            pos += replace.length();
        }
        return target;
    }

    /**
     *  \brief  Extract the first line from a string.
     *
     *  \param  s   Subject of the extraction
     *  \param  r   Remaining content aftert the extraction
     *  \return First line from the subject string
     */
    inline std::string GetFirstLine(const std::string& s, std::string& r){
        std::vector<std::string> elem = SplitOnFirst(s, '\n');
        if (elem.empty())
            return std::string();
        if (elem.size() > 1)
            r = elem.back();
        return elem.front();
    }

    /**
     * \brief Retrieve strings enclosed by matching backticks
     *
     * \param subject Signature of the section that needs to be parsed
     * \param begin Character index representing the beginning of the escaped string
     * \param escapeChar Character used to escape the string
     *
     * \return Returns the escaped string, new subject will be from the end of the escaped string
     *
     * \example (begin = 1, escapeChar = "`", subject = "a```b```cd") ----> (return = "```b```", subject = "cd")
     */
    inline std::string RetrieveEscaped(std::string& subject,
                                       const size_t begin,
                                       const char escapeChar = '`') {

        size_t levels = 0, end;

        // Get the level of the backticks
        while (subject[levels + begin] == escapeChar) {
            levels++;
        }

        end = subject.substr(levels + begin).find(subject.substr(begin, levels));

        if (end == std::string::npos) {
            return "";
        }

        end = end + (2 * levels) + begin;

        std::string escapedString = subject.substr(begin, end - begin);
        subject = subject.substr(end);

        return escapedString;
    }
}

#endif
