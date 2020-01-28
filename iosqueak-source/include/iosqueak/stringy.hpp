/** Utility Functions [IOSqueak]
  * Version: 1.0
  *
  * NOTE: Rename/rework this class!
  *
  * Author(s): Jason C. McDonald, Scott Taylor
  */

/* LICENSE (BSD-3-Clause)
 * Copyright (c) 2016-2019 MousePaw Media.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 *
 * CONTRIBUTING
 * See https://www.mousepawmedia.com/developers for information
 * on how to contribute to our projects.
 */

#ifndef IOSQUEAK_UTILS_HPP
#define IOSQUEAK_UTILS_HPP

#include <algorithm>
#include <cmath>
#include <cstring>
#include <cstdio>
#include <cstdint>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <vector>
// Needed for stdsplit
#include <string>

/* NOTE: Due to bug 19439, if we're using GCC, this only works on g++ 5.3 or higher.
 * https://sourceware.org/bugzilla/show_bug.cgi?id=19439
 */
#if defined(__clang__) || defined(__INTEL_COMPILER) || __GNUC__ > 5 || (__GNUC__ == 5 && (__GNUC_MINOR__ > 3 || (__GNUC_MINOR__ == 3 && __GNUC_PATCHLEVEL__ > 0)))
using std::isinf;
using std::isnan;
#endif // __GNUC__

////const double PRECISION = 0.00000000000001;
////const int MAX_NUMBER_STRING_SIZE = 32;

namespace stringy
{
    /** Efficiently split a std::string by tokens.
    * \param the string to split
    * \param the character or string to split by
    * \param the vector to store the tokens in
    */
    inline void stdsplit(std::string str, std::string splitBy, std::vector<std::string>& tokens)
    {
        /* Store the original string in the array, so we can loop the rest
            * of the algorithm. */
        tokens.push_back(str);

        // Store the size of what we're splicing out.
        size_t splitLen = splitBy.size();
        // Create a string for temporarily storing the fragment we're processing.
        std::string frag;

        // Loop infinitely - break is internal.
        while(true)
        {
            // Store the split index in a 'size_t' (unsigned integer) type.
            size_t splitAt;

            /* Store the last string in the vector, which is the only logical
                * candidate for processing. */
            frag = tokens.back();
            /* The index where the split is. */
            splitAt = frag.find(splitBy);
            // If we didn't find a new split point...
            if(splitAt == std::string::npos)
            {
                // Break the loop and (implicitly) return.
                break;
            }
            /* Put everything from the left side of the split where the string
                * being processed used to be. */
            tokens.back() = frag.substr(0, splitAt);
            /* Push everything from the right side of the split to the next empty
                * index in the vector. */
            tokens.push_back(frag.substr(splitAt+splitLen, frag.size()-(splitAt+splitLen)));
        }
    }

    /** Reverse a C-string. Algorithm from
    * http://stackoverflow.com/a/784567/472647
    * \param the C-string to reverse.
    * \param the reversed C-string
    */
    inline char* strrev(char* str)
    {
        if (str != NULL)
        {
            char *src;
            size_t len;

            len = strlen (str);
            if (len > 1)
            {
                char *dst;
                src = str;
                dst = src + len - 1;
                while (src < dst)
                {
                    char tmp = *src;
                    *src++ = *dst;
                    *dst-- = tmp;
                }
            }
        }
        return str;
    }
    //TODO: Move this to onestring!

    template<typename T>
    inline void arev(T* arr, size_t start, size_t end)
    {
        T tmp;
        while(start < end)
        {
            memcpy(&tmp, (arr + end), sizeof(T));
            memcpy((arr+end), (arr+start), sizeof(T));
            memcpy((arr+start), &tmp, sizeof(T));
            ++start;
            --end;
        }
    }

    /**Count the number of digits/characters in an integer. Does not
     * count the null terminator.
     * Based on http://stackoverflow.com/a/1489873/472647
     * \param the number to count the digits in
     * \param the base, default 10
     * \param whether to count the symbols (ignored if unsigned)
     * \return the number of digits in the integer*/
    template<typename T>
    inline int intlen(T number, int base, bool count_neg)
    {
        if(base < 2 || base > 36)
        {
            throw std::invalid_argument("stringy::intlen: Invalid base. Must be between 2 and 36.");
        }
        int digits = 0;
        if (number < 0 && count_neg)
        {
            digits = 1;
        }
        //Else if the number is literally "0"...
        else if(number == 0)
        {
            digits = 1;
        }
        while (number)
        {
            number /= base;
            ++digits;
        }

        return digits;
    }

    template int intlen<char>(char, int, bool);
    template int intlen<unsigned char>(unsigned char, int, bool);
    template int intlen<int>(int, int, bool);
    template int intlen<unsigned int>(unsigned int, int, bool);
    template int intlen<short int>(short int, int, bool);
    template int intlen<unsigned short int>(unsigned short int, int, bool);
    template int intlen<long int>(long int, int, bool);
    template int intlen<unsigned long int>(unsigned long int, int, bool);
    template int intlen<long long int>(long long int, int, bool);
    template int intlen<unsigned long long int>(unsigned long long int, int, bool);


    /**Count the maximum (safest) estimated number of digits/characters
     * in a float/double. Does not count null terminator. WARNING: This
     * is designed to strategically overcount, to prevent fatal array size
     * errors. This is necessary, since floats are unpredictable by
     * nature.
     * http://stackoverflow.com/a/1489873/472647
     * \param the number to count the characters in
     * \param the number of significands (default 14)
     * \param whether to count the symbols
     * \return the number of digits in the float*/
    template<typename T>
    inline int floatlen(T val, int significand, int sci, bool count_sym)
    {
        int len = 0;

        //Flag if the number is negative.
        bool neg = val < 0;

        //If we have a negative value...
        if(neg)
        {
            //Flip the value.
            val = -val;
            //If we're counting symbols, add one for the negative sign.
            if(count_sym){len++;}
        }

        //Calculate the magnitude.
        int m = log10(val);

        //Determine if we're using scientific notation.
        bool useExp = (
            ((sci == 1) && (m >= 14 || (neg && m >= 9) || m <= -9)) ||
            (sci == 2)
        );

        /* The maximum number of digits is usually the significand.*/
        len += significand;

        /* Sometimes, the whole part goes past the size of the significand,
            * so add the intlen of the whole part of the float.*/
        len += intlen(static_cast<int>(val), 10, false);

        //If we're using an exponent AND we're counting symbols.
        if(useExp && count_sym)
        {
            /* We add the intlen of the absolute value of the magnitude,
                * plus one for the sign and one for the 'e'.*/
            len += (intlen(m, 10, false) + 2);
        }

        //If we're counting symbols, add one for the decimal point.
        if(count_sym){len++;}

        //Return the finished maximum length.
        return len;
    }

    template int floatlen<float>(float, int, int, bool);
    template int floatlen<double>(double, int, int, bool);
    template int floatlen<long double>(long double, int, int, bool);


    /**Convert an integer to a C-string.
     * \param the C-string to write to
     * \param the integer to convert
     * \param the base to convert in, default 10.
     * \param the length of the C-string, not counting the null
     * terminator. If 0 or omitted, this will be automatically
     * calculated.
     * \param whether to use capital letters for base > 10*/
    template<typename T>
    inline void itoa(char* str, T val, int base, int len, bool use_caps)
    {
        /*Adapted from comp.lang.c++, Re: Integer to Ustring
        Christian Gollwitzer <auriocus@gmx.de>
        Wed, 17 Jun 2015 09:20:13 +0200

        ...
        Formatting integers is usually done by
        the divide and mod algorithm, which works like this

        if (i<0) { print "-"; i=-i }

        i=12345678
        while i {
            print i % 10
            i = i/10
        }

        Afterwards you reverse the digits.
        */

        /* If no length was provided, get the length of the integer,
            * including negative sign.*/
        len = (len == 0) ? intlen(val, base, true) + 1 : len;

        char* pos = str;

        bool neg = false;
        if (val < 0)
        {
            //Make the number positive;
            val *= -1;
            neg = true;
        }

        if (val == 0)
        {
            //Write out a literal 0
            *(pos++) = '0';
        }

        while (val)
        {
            int digit = val % base;

            /*To get the corresponding character, just add 48 for a numeral,
            or 55 for a letter. We don't have to worry about verifying that
            the character is appropriate for the base, as the `i % base` will
            never yield a value more than the base.
            */

            //48 [0] - 57 [9] (+48)
            if(digit >= 0 && digit <= 9)
            {
                *(pos++) = static_cast<char>(digit+48);
            }
            //65 [A] - 90 [Z] (+55) or 97 [a] - 122 [z] (+87)
            else if(digit >= 10 && digit <= 35)
            {
                int offset = (use_caps ? 55 : 87);
                *(pos++) = static_cast<char>(digit+offset);
            }

            //Divide the number by the base to remove the last parsed digit.
            val /= base;
        }

        if(neg)
        {
            //Place the symbol at the start of the string.
            *(pos++) = '-';
        }

        //Reverse the string. Remember to pass the 0-based start/end indices.
        arev(str, 0, len-2);
    }

    template void itoa<char>(char*, char, int, int, bool);
    template void itoa<unsigned char>(char*, unsigned char, int, int, bool);
    template void itoa<int>(char*, int, int, int, bool);
    template void itoa<unsigned int>(char*, unsigned int, int, int, bool);
    template void itoa<short int>(char*, short int, int, int, bool);
    template void itoa<unsigned short int>(char*, unsigned short int, int, int, bool);
    template void itoa<long int>(char*, long int, int, int, bool);
    template void itoa<unsigned long int>(char*, unsigned long int, int, int, bool);
    template void itoa<long long int>(char*, long long int, int, int, bool);
    template void itoa<unsigned long long int>(char*, unsigned long long int, int, int, bool);

    /**Convert a byte to a C-string.
     * \param the C-string to write to
     * \param the byte (unsigned integer) to convert
     * \param the base to convert in, default 10.
     * \param the length of the C-string, not counting the null
     * terminator. If 0 or omitted, this will be automatically
     * calculated.
     * \param whether to use capital letters for base > 10*/
    inline void btoa(char* str, unsigned int val, int len=0, bool use_caps=false)
    {
        len = (len == 0) ? intlen(val, 16, false) : len;

        char* pos = str;

        bool leadZero = false;
        if(val < 16)
        {
            leadZero = true;
        }

        if (val == 0)
        {
            //Write out a literal 0
            *(pos++) = '0';
        }

        while (val)
        {
            unsigned int digit = val % 16;

            /*To get the corresponding character, just add 48 for a numeral,
            or 55 for a letter. We don't have to worry about verifying that
            the character is appropriate for the base, as the `i % base` will
            never yield a value more than the base.
            */

            //48 [0] - 57 [9] (+48)
            if(/*digit >= 0 && */digit <= 9)
            {
                *(pos++) = static_cast<char>(digit+48);
            }
            //65 [A] - 90 [Z] (+55) or 97 [a] - 122 [z] (+87)
            else if(digit >= 10 && digit <= 35)
            {
                unsigned int offset = (use_caps ? 55 : 87);
                *(pos++) = static_cast<char>(digit+offset);
            }

            //Divide the number by the base to remove the last parsed digit.
            val /= 16;
        }

        if(leadZero)
        {
            *(pos++) = '0';
        }

        //Reverse the string.
        //char temp[strlen(str)];
        char* temp = new char[strlen(str)];
        temp[0] = '\0';

        strcpy(temp, str);
        strcpy(str, strrev(temp));

        delete[] temp;
    }

    /**Convert a floating point number to a C-string.
     * \param the C-string to write to
     * \param the number to convert
     * \param the number of significands (an integer)
     * \param whether to use scientific notation. 0=none, 1=automatic,
     * 2=force scientific notation.*/
    template<typename T>
    inline void ftoa(char* str, T val, int significand=14, int sci=1)
    {
        //If the value is not a number...
        if (isnan(val))
        {
            //The string is "NaN".
            strcpy(str, "NaN");
        }
        //If the value is infinite.
        else if (isinf(val))
        {
            //The string is "Inf".
            strcpy(str, "Inf");
            //TODO: What about negative infinity?
        }
        //If the value is 0.
        else if (val == 0.0)
        {
            //Just print out 0.
            strcpy(str, "0");
        }
        else
        {
            //A single digit.
            int digit;
            int m1;
            char* c = str;

            //Convert "significand" to significand decimal.
            T p = 1 / pow(10, significand);

            //Determine whether the number is negative.
            bool neg = (val < 0);
            //If negative...
            if (neg)
            {
                //Invert the value.
                val = -val;
                //Start the string with a negative sign.
                *(c++) = '-';
            }
            //That last step was important, because we can't log10 a negative.

            /* Calculate magnitude (distance from highest digit to decimal).
                * We can truncate the decimal, as it isn't import here.
                * Thus, we just use int.*/
            int m = log10(val);

            /* Flag whether we use scientific notation. In auto-mode (sci=1),
                * we'll want to use it when our magnitude is over 14 or
                less than -9. Otherwise, we force its use when (sci=2).*/
            //NOTE: Swapped from int to bool for efficiency.
            bool useExp = (
                ((sci == 1) && (m >= 14 || (neg && m >= 9) || m <= -9)) ||
                (sci == 2)
            );

            //If we are using scientific notation...
            if (useExp)
            {
                //If the magnitude is negative.
                if (m < 0)
                {
                    //Set the initial magnitude to -1.
                    m -= 1;
                    //NOTE: Why are we doing this? What about 0.000024 (m=-4)
                }
                //Convert the value to scientific notation.
                val = val / pow(10.0, m);
                //Store the magnitude for later reference.
                m1 = m;
                //We are now at magnitude 0 after conversion.
                m = 0;
            }

            //If the magnitude is less than 1...
            if (m < 1)
            {
                //Reset the magnitude to 0.
                m = 0;
            }

            //Convert the number.
            /* While the value is greater than our significand and magnitude is
                * not negative.*/
            while (val > p || m >= 0)
            {
                //Set our weight to 10^m.
                T weight = pow(10.0, m);

                //Weight (place value) is greater than 0 and not infinite.
                if (weight > 0 && !isinf(weight))
                {
                    //Digit is floor of value / place value.
                    digit = floor(val / weight);

                    //Remove the number we just parsed out.
                    val -= (digit * weight);

                    //Append the digit to the string.
                    *(c++) = '0' + digit;
                }
                //If our magnitude is 0 and our value is > 0.
                if (m == 0 && val > 0)
                {
                    //Append a decimal point.
                    *(c++) = '.';
                }

                //Decrement the magnitude.
                m--;
            }
            //If we are using scientific notation (exponent)...
            if (useExp)
            {
                //Append the 'e' symbol.
                *(c++) = 'e';
                //if the exponent is positive
                if (m1 >= 0)
                {
                    //Append the '+' symbol.
                    *(c++) = '+';
                }
                //else if the exponent is negative
                else
                {
                    //Append the '-' symbol.
                    *(c++) = '-';
                    //Invert the exponent.
                    m1 = -m1;
                }
                //Reset our magnitude to 0, so we can reuse it as an iterator.
                m = 0;

                //While the exponent is > 0.
                while (m1 > 0)
                {
                    //Convert and append digits.
                    *(c++) = '0' + m1 % 10;
                    m1 /= 10;
                    //Increment m.
                    m++;
                }

                //Move back in the string by m.
                c -= m;

                //Reverse the string.
                //char temp[strlen(c)];
                char* temp = new char[strlen(c)];
                temp[0] = '\0';
                strcpy(temp, c);
                strcpy(c, strrev(temp));
                delete[] temp;

                /* We don't use the XOR method because it is generally less
                    * efficient than just swapping with temporary.*/
                /*
                for (int i = 0, j = m-1; i<j; i++, j--)
                {
                    c[i] ^= c[j];
                    c[j] ^= c[i];
                    c[i] ^= c[j];
                }*/
                //Move to the end of the string.
                c += m;
            }
            //Append the null terminator.
            *(c) = '\0';
        }
    }
    template void ftoa<float>(char*, float, int, int);
    template void ftoa<double>(char*, double, int, int);
    template void ftoa<long double>(char*, long double, int, int);

    /**Convert an unsigned pointer integer to a Cstring.
     * Does not presently export the flag value, just the
     * memory address.
     * \param the C-string to write to
     * \param the pointer integer to convert
         * \param whether to use capital letters for base > 10*/
    inline void ptrtoa(char* str, uintptr_t val, bool use_caps)
    {
        /* 64-bits, plus sign and 0x.*/
        ////int len = 67;
        //TODO: Use 32-bit on 32-bit system.

        /*Declare a new C-string.*/
        //char str[len+1] = {'\0'};

        //The current position is length - 1 (to offset)
        char* pos = str;

        /*if (val < 0)
        {
            //Make the number positive;
            val *= -1;
            //Place the symbol at the start of the string.
            //// *pos = '-';
        }*/


        if (val == 0)
        {
            //Write out a literal 0
            *(pos++) = '0';
        }

        while (val)
        {
            uintptr_t digit = val % 16;

            /*To get the corresponding character, just add 48 for a numeral,
            or 55 for a letter. We don't have to worry about verifying that
            the character is appropriate for the base, as the `i % base` will
            never yield a value more than the base.
            */

            //48 [0] - 57 [9] (+48)
            if(/*digit >= 0 && */digit <= 9)
            {
                *(pos++) = static_cast<char>(digit+48);
            }
            //65 [A] - 90 [Z] (+55) or 97 [a] - 122 [z] (+87)
            else if(digit >= 10 && digit <= 35)
            {
                int offset = (use_caps ? 55 : 87);
                *(pos++) = static_cast<char>(digit+offset);
            }

            //Divide the number by the base to remove the last parsed digit.
            val /= 16;
        }

        //Add the hexadecimal indicator 0x".
        *(pos++) = 'x';
        *(pos++) = '0';

        //Reverse the string.
        //char temp[strlen(str)];
        char* temp = new char[67];
        temp[0] = '\0';

        strcpy(temp, str);
        strcpy(str, strrev(temp));

        delete[] temp;
    }


    /**Convert an int to a std::string.
     * \param the integer to convert
     * \param the base to convert
     * \param whether to use capital letters for base > 10
     * \return the string representing the integer.*/
    template<typename T>
    inline std::string itos(T val, int base=10, bool use_caps=false)
    {
        int len = intlen(val, base, true) + 1;

        //char cstr[len];
        char* cstr = new char[len];
        /* We have to initialize after the fact to keep Clang happy,
            * but we must fill the array to avoid memory issues.
            * Thus, we'll use the std::fill_n function to fill the array
            * with null characters. */
        std::fill_n(cstr, len, '\0');

        itoa(cstr, val, base, len, use_caps);
        std::string str = cstr;
        delete[] cstr;
        return str;
    }
    template std::string itos<char>(char, int, bool);
    template std::string itos<unsigned char>(unsigned char, int, bool);
    template std::string itos<int>(int, int, bool);
    template std::string itos<unsigned int>(unsigned int, int, bool);
    template std::string itos<short int>(short int, int, bool);
    template std::string itos<unsigned short int>(unsigned short int, int, bool);
    template std::string itos<long int>(long int, int, bool);
    template std::string itos<unsigned long int>(unsigned long int, int, bool);
    template std::string itos<long long int>(long long int, int, bool);
    template std::string itos<unsigned long long int>(unsigned long long int, int, bool);

    /**Convert a float to a std::string.
     * \param the float to convert
     * \param the number of significands (an integer)
     * \param whether to use scientific notation. 0=none, 1=automatic,
     * 2=force scientific notation.
     * \return the string representing the integer.*/
    template<typename T>
    inline std::string ftos(T val, int significand=14, int sci=1)
    {
        /* Get the estimated maximum number of characters in the float, plus
            * 1 for the null terminator.
            */
        int len = floatlen(val, significand, sci, true) + 1;

        /* Declare a new C-string with the size equal to the estimated
            * maximum number of characters in the float, plus 1 for the null
            * terminator.*/
        //char cstr[len];
        char* cstr = new char[len];

        /* We have to initialize after the fact to keep Clang happy,
            * but we must fill the array to avoid memory issues.
            * Thus, we'll use the std::fill_n function to fill the array
            * with null characters. */
        std::fill_n(cstr, len, '\0');

        //Convert the float to a cstring, and dump into cstr.
        ftoa(cstr, val, significand, sci);
        //Convert the cstring into a string.
        std::string str = cstr;
        delete[] cstr;
        //Return the string.
        return str;
    }
    template std::string ftos<float>(float, int, int);
    template std::string ftos<double>(double, int, int);
    template std::string ftos<long double>(long double, int, int);

    /**Convert an unsigned pointer integer to a std::string.
     * \param the pointer integer to convert
     * \param whether to use capital letters for base > 10
     * \return the string representing the pointer.*/
    //std::string ptrtos(uintptr_t);
    inline std::string ptrtos(uintptr_t val, bool use_caps=false)
    {
        //TODO: Allow for 32-bit systems as well.
        char cstr[67] = {'\0'};
        ptrtoa(cstr, val, use_caps);
        std::string str = cstr;
        return str;
    }

    inline void memdump(char* str, const void* mem, unsigned int bytes, bool use_caps=false, char memformat=0)
    {
        bool bytespacing = memformat & (1 << 0);
        bool wordspacing = memformat & (1 << 1);
        char byte[3] = {'\0'};
        const uint8_t* p = reinterpret_cast<const uint8_t*>(mem);
        for (unsigned int i = 0; i < bytes; i++)
        {
            btoa(byte, p[i], 2, use_caps);
            strcat(str, byte);
            //If we use byte spacing and we're not on the last byte...
            if(bytespacing && i != (bytes-1))
            {
                strcat(str, " ");
            }

            //If we use word spacing and we're between words (and not on last byte).
            if(wordspacing && (i+1)%8 == 0 && i != (bytes-1))
            {
                strcat(str, "|");
                if(bytespacing)
                {
                    strcat(str, " ");
                }
            }
        }
    }
};

/*
//This is the pointer conversion used by printf("%p", ...)
//https://sourceware.org/git/?p=glibc.git;a=blob;f=stdio-common/vfprintf.c
LABEL (form_pointer):
{
    const void *ptr;
    if (fspec == NULL)
        ptr = va_arg (ap, void *);
    else
        ptr = args_value[fspec->data_arg].pa_pointer;
    if (ptr != NULL)
    {
        // If the pointer is not NULL, write it as a %#x spec.
        base = 16;
        number.word = (unsigned long int) ptr;
        is_negative = 0;
        alt = 1;
        group = 0;
        spec = L_('x');
        goto LABEL (number);
    }
    else
    {
        // Write "(nil)" for a nil pointer.
        string = (CHAR_T *) L_("(nil)");
        // Make sure the full string "(nil)" is printed.
        if (prec < 5)
            prec = 5;
        is_long = 0;        // This is no wide-char string.
        goto LABEL (print_string);
    }
 }

LABEL (number):
{
    if (prec < 0)
     // Supply a default precision if none was given.
        prec = 1;
    else
    {
        // We have to take care for the '0' flag.  If a precision
        // is given it must be ignored.
        pad = L_(' ');
    }

    // If the precision is 0 and the number is 0 nothing has to
    // be written for the number, except for the 'o' format in
    // alternate form.
    if (prec == 0 && number.word == 0)
    {
        string = workend;
        if (base == 8 && alt)
        *--string = L_('0');
    }
    else
    {
        // Put the number in WORK.
        string = _itoa_word (number.word, workend, base,
                             spec == L_('X'));
        if (group && grouping)
            string = group_number (string, workend, grouping,
                                   thousands_sep);

        if (use_outdigits && base == 10)
            string = _i18n_number_rewrite (string, workend, workend);
    }
 }
 */

#endif // IOSQUEAK_UTILS_HPP
