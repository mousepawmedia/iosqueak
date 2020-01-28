/** Channel [IOSqueak]
  * Version: 2.0.0
  *
  * Allows managed, custom output to any console or terminal.
  *
  * Author: Jason C. McDonald
  */

/* LICENSE (BSD-3-Clause)
 * Copyright (c) 2016-2020 MousePaw Media.
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

#ifndef IOSQUEAK_CHANNEL_HPP
#define IOSQUEAK_CHANNEL_HPP

//Needed for output sources.
#include <stdio.h>
#include <iostream>

//Needed for the `intptr_t` type
#include <cstdint>

//Needed for `ceil()`
#include <cmath>

//Bitset
#include <bitset>

//Needed for checking types.
#include <typeinfo>
#include <type_traits>

//Needed for handling passed-in exceptions.
#include <exception>

//Signals and callbacks.
#include "eventpp/callbacklist.h"

/*We are only using std::string and std::queue temporarily.
These need to be swapped out for pawlib alternatives ASAP.*/
#include <string>

//We use C's classes often.
#include <cstdio>

#include "pawlib/core_types.hpp"

#include "iosqueak/ioformat.hpp"
#include "iosqueak/stringy.hpp"

class pure_tril;
class tril;

/**An channel allows console output to be custom routed to one or more
 * text-based output channels, including the terminal. It supports various
 * advanced functions, formatting and colors, and message priority.*/
class channel
{
public:
    /**Declares a new channel instance.*/
    channel()
    : msg(""),
      process_cat(IOCat::all),
      process_vrb(IOVrb::tmi),
      echo_mode(IOEchoMode::cout),
      echo_cat(IOCat::all),
      echo_vrb(IOVrb::tmi),
      fmt(IOFormat()),
      readsize(IOMemReadSize(1)),
      vrb(IOVrb::normal),
      cat(IOCat::normal),
      parse(maybe),
      dirty_attributes(false)
    {}

    /** Eventpp signal (callback list) for categories. */
    typedef eventpp::CallbackList<void (std::string,
        IOCat)> IOSignalCat;

    /** Eventpp signal (callback list) for verbosities. */
    typedef eventpp::CallbackList<void (std::string,
        IOVrb)> IOSignalVrb;

    /** Eventpp signal (callback list) for everything,
      * transmitting the message, the verbosity, and the category. */
    typedef eventpp::CallbackList<void (std::string,
        IOVrb, IOCat)> IOSignalFull;

    /** Eventpp signal (callback list) for everything,
         * transmitting only the message. */
    typedef eventpp::CallbackList<void (std::string)> IOSignalAll;


    /* NOTE: In the examples below, the verbosity-related signals must
     * transmit what category the message is (since verbosity is
     * inherent and assumed). The inverse is true of category-related
     * signals. */

    /** Emitted when a message with verbosity 0 (quiet) is broadcast.
     * Callback must be of form 'void callback(string,
     * IOCat){}'
     */
    IOSignalCat signal_v_quiet;

    /** Emitted when a message with verbosity <= 1 (normal) is broadcast.
     * Callback must be of form 'void callback(string,
     * IOCat){}'
     */
    IOSignalCat signal_v_normal;

    /** Emitted when a message with verbosity <=2 (chatty) is broadcast.
     * Callback must be of form 'void callback(string,
     * IOCat){}'
     */
    IOSignalCat signal_v_chatty;

    /** Emitted when a message with verbosity <=3 (tmi) is broadcast.
     * Callback must be of form 'void callback(string,
     * IOCat){}'
     */
    IOSignalCat signal_v_tmi;

    /** Emitted when a message with category "normal" is broadcast.
     * Callback must be of form 'void callback(string,
     * IOVrb){}'
     */
    IOSignalVrb signal_c_normal;

    /** Emitted when a message with category "warning" is broadcast.
     * Callback must be of form 'void callback(string,
     * IOVrb){}'
     */
    IOSignalVrb signal_c_warning;

    /** Emitted when a message with category "error" is broadcast.
     * Callback must be of form 'void callback(string,
     * IOVrb){}'
     */
    IOSignalVrb signal_c_error;

    /** Emitted when a message with category "debug" is broadcast.
     * Callback must be of form 'void callback(string,
     * IOVrb){}'
     */
    IOSignalVrb signal_c_debug;

    /** Emitted when a message with category "testing" is broadcast.
     * Callback must be of form 'void callback(string,
     * IOVrb){}'
     */
    IOSignalVrb signal_c_testing;

    /** Emitted when any message is broadcast.
     * Callback must be of form 'void callback(string,
     * IOVrb, IOCat){}'
     */
    IOSignalFull signal_full;

    /** Emitted when any message is broadcast, but only transmits the
     * message, and omits the verbosity and category.
     * Callback must be of form 'void callback(string){}'
     */
    IOSignalAll signal_all;

    // Overloaded stream insertion operators for Channel.

    //These need custom implementations.
    channel& operator<<(const bool& rhs)
    {
        //If we cannot parse because of `shutup()` settings, abort.
        if(!can_parse()){return *this;}
        switch(fmt.fmt_bool)
        {
            case IOFormatBool::lower:
            {
                inject(rhs ? "true" : "false");
                break;
            }
            case IOFormatBool::upper:
            {
                inject(rhs ? "True" : "False");
                break;
            }
            case IOFormatBool::caps:
            {
                inject(rhs ? "TRUE" : "FALSE");
                break;
            }
            case IOFormatBool::numeral:
            {
                inject(rhs ? "1" : "0");
                break;
            }
            case IOFormatBool::scott:
            {
                inject(rhs ? "Yea" : "Nay");
                break;
            }
        }
        return *this;
    }
    channel& operator<<(const tril& rhs)
    {
        //If we cannot parse because of `shutup()` settings, abort.
        if(!can_parse()){return *this;}
        switch(fmt.fmt_bool)
        {
            case IOFormatBool::lower:
            {
                inject( (~rhs) ? "maybe"
                : (rhs ? "true" : "false"));
                break;
            }
            case IOFormatBool::upper:
            {
                inject( (~rhs) ? "Maybe"
                : (rhs ? "True" : "False"));
                break;
            }
            case IOFormatBool::caps:
            {
                inject( (~rhs) ? "MAYBE"
                : (rhs ? "TRUE" : "FALSE"));
                break;
            }
            case IOFormatBool::numeral:
            {
                inject( (~rhs) ? "2"
                : (rhs ? "1" : "0"));
                break;
            }
            case IOFormatBool::scott:
            {
                inject( (~rhs) ? "Maybe"
                : (rhs ? "Yea" : "Nay"));
                break;
            }
        }
        return *this;
    }

    channel& operator<<(const char& rhs)
    {
        //If we cannot parse because of `shutup()` settings, abort.
        if(!can_parse()){return *this;}

        switch(fmt.fmt_char_value)
        {
            //Output as character.
            case IOFormatCharValue::as_char:
            {
                //This will call the inject(char) function.
                inject(rhs);
                break;
            }
            //Output as integer.
            case IOFormatCharValue::as_int:
            {
                resolve_integer(rhs);
                break;
            }
        }
        return *this;
    }

    channel& operator<<(const std::string& rhs)
    {
        //If we cannot parse because of `shutup()` settings, abort.
        if(!can_parse()){return *this;}

        //Inject the C string form of the string into the channel.
        inject(rhs.c_str());
        return *this;
    }

    inline channel& operator<<(const unsigned char& rhs){return resolve_integer(rhs);}
    inline channel& operator<<(const int& rhs){return resolve_integer(rhs);}
    inline channel& operator<<(const unsigned int& rhs){return resolve_integer(rhs);}
    inline channel& operator<<(const short int& rhs){return resolve_integer(rhs);}
    inline channel& operator<<(const unsigned short int& rhs){return resolve_integer(rhs);}
    inline channel& operator<<(const long int& rhs){return resolve_integer(rhs);}
    inline channel& operator<<(const unsigned long int& rhs){return resolve_integer(rhs);}
    inline channel& operator<<(const long long int& rhs){return resolve_integer(rhs);}
    inline channel& operator<<(const unsigned long long int& rhs){return resolve_integer(rhs);}

    inline channel& operator<<(const float& rhs){return resolve_float(rhs);}
    inline channel& operator<<(const double& rhs){return resolve_float(rhs);}
    inline channel& operator<<(const long double& rhs){return resolve_float(rhs);}

    template<typename T>
    inline channel& operator<<(const T* rhs){return resolve_pointer_smart(rhs);}

    inline channel& operator<<(const void* rhs){return resolve_pointer(rhs);}
    inline channel& operator<<(const bool* rhs){return resolve_pointer(rhs);}
    inline channel& operator<<(const char* rhs){return resolve_pointer(rhs);}
    inline channel& operator<<(const unsigned char* rhs){return resolve_pointer(rhs);}
    inline channel& operator<<(const int* rhs){return resolve_pointer(rhs);}
    inline channel& operator<<(const unsigned int* rhs){return resolve_pointer(rhs);}
    inline channel& operator<<(const short int* rhs){return resolve_pointer(rhs);}
    inline channel& operator<<(const unsigned short int* rhs){return resolve_pointer(rhs);}
    inline channel& operator<<(const long int* rhs){return resolve_pointer(rhs);}
    inline channel& operator<<(const unsigned long int* rhs){return resolve_pointer(rhs);}
    inline channel& operator<<(const long long int* rhs){return resolve_pointer(rhs);}
    inline channel& operator<<(const unsigned long long int* rhs){return resolve_pointer(rhs);}
    inline channel& operator<<(const float* rhs){return resolve_pointer(rhs);}
    inline channel& operator<<(const double* rhs){return resolve_pointer(rhs);}
    inline channel& operator<<(const long double* rhs){return resolve_pointer(rhs);}
    inline channel& operator<<(const std::string* rhs){return resolve_pointer(rhs);}

    channel& operator<<(const std::exception& rhs)
    {
        //If we cannot parse because of `shutup()` settings, abort.
        if(!can_parse()){return *this;}

        /* Inject the exception message into the channel. Assume that
            * the exception itself has been handled externally. */
        inject(rhs.what());
        return *this;
    }

    //All of these need custom implementations.
    template<const long unsigned int T>
    channel& operator<<(const std::bitset<T>& rhs)
    {
        // Store the old values for the two flags we use.
        IOMemReadSize old_readsize = readsize;
        IOFormatPtr old_ptr = fmt.fmt_ptr;

        /* The readsize (in bytes) is the bitset size (bits) divided
            * by 8 and rounded to the nearest integer. */
        readsize = static_cast<unsigned int>(ceil((T/8)));
        // We want a memory dump.
        fmt << IOFormatPtr::memory;

        // Remove the const-ness from the pointer.
        std::bitset<T>* rhs_ptr = const_cast<std::bitset<T>*>(&rhs);
        /* Convert to a void pointer for easier resolution.
            * We must store the result as a pointer for returning shortly.
            */
        channel* r = &resolve_pointer(static_cast<void*>(rhs_ptr));

        // Restore our prior flag values.
        fmt << old_ptr;
        readsize = old_readsize;

        /* Return the value at the pointer, which will be implicitly
            * converted to reference by the function return. */
        return *r;
    }

    channel& operator<<(const IOFormatBase& rhs) {process_fmt(rhs); return *this;}
    channel& operator<<(const IOFormatBool& rhs) {process_fmt(rhs); return *this;}
    channel& operator<<(const IOFormatCharValue& rhs) {process_fmt(rhs); return *this;}
    channel& operator<<(const IOFormatMemSep& rhs) {process_fmt(rhs); return *this;}
    channel& operator<<(const IOFormatNumCase& rhs) {process_fmt(rhs); return *this;}
    channel& operator<<(const IOFormatPtr& rhs) {process_fmt(rhs); return *this;}
    channel& operator<<(const IOFormatSciNotation& rhs) {process_fmt(rhs); return *this;}
    channel& operator<<(const IOFormatSignificands& rhs) {process_fmt(rhs); return *this;}

    channel& operator<<(const IOFormatTextAttr& rhs)
    {
        if (process_fmt(rhs))
        {
            dirty_attributes = true;
        }
        return *this;
    }

    channel& operator<<(const IOFormatTextBG& rhs)
    {
        if (process_fmt(rhs))
        {
            dirty_attributes = true;
        }
        return *this;
    }

    channel& operator<<(const IOFormatTextFG& rhs)
    {
        if (process_fmt(rhs))
        {
            dirty_attributes = true;
        }
        return *this;
    }

    channel& operator<<(const IOFormat& rhs)
    {
        fmt = rhs;
        return *this;
    }
    channel& operator<<(const IOCursor& rhs)
    {
        //If we cannot parse because of `shutup()` settings, abort.
        if(!can_parse()){return *this;}

        //Run the command (ANSI or whatever).
        move_cursor(rhs);
        return *this;
    }
    channel& operator<<(const IOMemReadSize& rhs)
    {
        //If we cannot parse because of `shutup()` settings, abort.
        if(!can_parse()){return *this;}

        readsize = rhs;

        return *this;
    }
    channel& operator<<(const IOVrb& rhs)
    {
        //Set the verbosity.
        vrb = rhs;
        //Revalidate parsing.
        parse = maybe;
        return *this;
    }
    channel& operator<<(const IOCat& rhs)
    {
        //Set the category.
        cat = rhs;
        //Revalidate parsing.
        parse = maybe;
        return *this;
    }
    channel& operator<<(const IOCtrl& rhs)
    {
        /* We'll need to track whether we're supposed to ask the transmission
         * to clear the flags. Doing it ourselves in this function
         * was the cause of T1081. */
        bool keep = true;

        if(static_cast<bool>(rhs & IOCtrl::clear))
        {
            reset_attributes();
            keep = false;
        }

        if(static_cast<bool>(rhs & IOCtrl::r))
        {
            can_parse() ? inject("\r") : inject("");
        }

        if(static_cast<bool>(rhs & IOCtrl::n))
        {
            can_parse() ? inject("\n") : inject("");
        }

        if(static_cast<bool>(rhs & IOCtrl::send))
        {
            transmit(keep);
        }

        if(static_cast<bool>(rhs & IOCtrl::flush))
        {
            flush();
        }

        return *this;
    }

    void configure_echo(IOEchoMode mode, IOVrb vrb = IOVrb::tmi, IOCat cat = IOCat::all)
    {
        echo_mode = mode;
        echo_vrb = vrb;
        echo_cat = cat;
    }

    /** Suppress a category from broadcasting at all.
     * \param the category to suppress
     */
    void shutup(IOCat cat)
    {
        process_cat = process_cat & ~cat;
        if(process_cat == IOCat::none)
        {
            printf("WARNING: All message categories have been turned off!\n");
        }
        //Revalidate parsing.
        parse = maybe;
    }

    /** Suppress verbosities higher than the one given.
     * This is also mainly used to reset verbosity back to "normal"
     * (the default).
     * \param the maximum verbosity to permit (default=IOVrb::normal)
     */
    void shutup(IOVrb vrb = IOVrb::normal)
    {
        //Set the processing verbosity.
        process_vrb = vrb;
        //Revalidate parsing.
        parse = maybe;
    }

    /** Permit a category to broadcast.
     * \param the category to broadcast
     */
    void speakup(IOCat cat)
    {
        //Allow the category through by turning on its bit.
        process_cat = process_cat | cat;
        //Revalidate parsing.
        parse = maybe;
    }

    /** Permit a verbosity (and all below it) to broadcast.
     * Functionally identical to `shutup(IOVrb)`.
     * \param the maximum verbosity to permit
     */
    void speakup(IOVrb vrb)
    {
        //Allow verbosity through by turning on its bit.
        if(process_vrb < vrb)
        {
            process_vrb = vrb;
        }
        //Revalidate parsing.
        parse = maybe;
    }

    /** Permit all messages (full verbosity and all categories)
     * to be broadcast.
     */
    void speakup()
    {
        process_vrb = IOVrb::tmi;
        process_cat = IOCat::all;
        //Revalidate parsing.
        parse = maybe;
    }

    ~channel() = default;
protected:
    std::string msg;

    // Which categories are permitted.
    IOCat process_cat = IOCat::all;
    // The maximum verbosity to permit.
    IOVrb process_vrb = IOVrb::tmi;

    // Which method should be used for Channel's default stdout echo?
    IOEchoMode echo_mode = IOEchoMode::printf;
    // The category to echo.
    IOCat echo_cat = IOCat::all;
    // The maximum verbosity to echo.
    IOVrb echo_vrb = IOVrb::tmi;

    /* MESSAGE ATTRIBUTES
     * These are set by enum and flags,
     * and should be reset after each message (unless
     * the user ends the message with an EoM stream
     * control with a "keep" flag.
     */

    IOFormat fmt;

    // How many bytes of data to read from a pointer.
    IOMemReadSize readsize = IOMemReadSize(1);

    /* Message verbosity. */
    IOVrb vrb = IOVrb::normal;
    /* Message category. */
    IOCat cat = IOCat::normal;
    /* Message parsable */
    tril parse = maybe;

    template<typename T>
    bool process_fmt(T val)
    {
        //If we cannot parse because of `shutup()` settings, abort.
        if(!can_parse()){return false;}

        fmt << val;
        return true;
    }

    template<typename T>
    channel& resolve_pointer_smart(const T* rhs)
    {
        //If we cannot parse because of `shutup()` settings, abort.
        if(!can_parse()){return *this;}

        switch(fmt.fmt_ptr)
        {
            //If we are to print as value...
            case IOFormatPtr::value:
            {
                *this << "[channel cannot interpret value at pointer of this type.]";
                break;
            }
            //If we are to print as address...
            case IOFormatPtr::address:
            {
                //Inject raw address, overriding with byte read size 1.
                inject(rhs, 1, false);
                break;
            }
            //If we are to print the raw memory...
            case IOFormatPtr::memory:
            {
                //Inject raw address with the object size as read_size.
                inject(rhs, sizeof(T), true);
                break;
            }
        }
        return *this;
    }

    /** Resolves any common pointer. This can handle
     * all atomic and numeric C and C++ types.
     * Since this is the least specialized version, it's the fallback.
     * \param the pointer to resolve
     */
    template<typename T>
    channel& resolve_pointer(const T* rhs)
    {
        //If we cannot parse because of `shutup()` settings, abort.
        if(!can_parse()){return *this;}

        switch(fmt.fmt_ptr)
        {
            //If we are to print as value...
            case IOFormatPtr::value:
            {
                *this << *rhs;
                break;
            }
            //If we are to print as address...
            case IOFormatPtr::address:
            {
                //Inject raw address.
                inject(rhs, sizeof(*rhs), false);
                break;
            }
            //If we are to print the raw memory...
            case IOFormatPtr::memory:
            {
                //Inject address for memory dump.
                inject(rhs, sizeof(typeid(*rhs)), true);
                break;
            }
        }
        return *this;
    }

    /** Resolves a void pointer, which allows
     * for raw data dumps from any pointer
     * imaginable.
     * \param the pointer to resolve
     */
    channel& resolve_pointer(const void* rhs)
    {
        //If we cannot parse because of `shutup()` settings, abort.
        if(!can_parse()){return *this;}

        switch(fmt.fmt_ptr)
        {
            //If we are to print as value...
            case IOFormatPtr::value:
            {
                *this << "[channel cannot interpret value at pointer of this type.]";
                break;
            }
            //If we are to print as address...
            case IOFormatPtr::address:
            {
                //Inject raw address, overriding with byte read size 1.
                inject(rhs, 1, false);
                break;
            }
            //If we are to print the raw memory...
            case IOFormatPtr::memory:
            {
                //Inject raw address with the read_size() given by the user.
                inject(rhs, readsize.readsize, true);
                break;
            }
        }
        return *this;
    }


    /** Resolves a char pointer.
     * \param the pointer to resolve
     */
    channel& resolve_pointer(const char* rhs)
    {
        //If we cannot parse because of `shutup()` settings, abort.
        if(!can_parse()){return *this;}

        switch(fmt.fmt_ptr)
        {
            //If we are to print as value...
            case IOFormatPtr::value:
            {
                //We can just inject the C-string.
                inject(rhs);
                break;
            }
            //If we are to print as address...
            case IOFormatPtr::address:
            {
                //Inject raw address.
                inject(rhs, sizeof(*rhs), false);
                break;
            }
            //If we are to print the raw memory...
            case IOFormatPtr::memory:
            {
                //Inject address for memory dump.
                inject(rhs, ((strlen(rhs) + 1) * sizeof(char)), true);
                break;
            }
        }
        return *this;
    }

    /** Resolves any integer.
     * \param the integer variable to resolve
     */
    template<typename T>
    channel& resolve_integer(const T& rhs)
    {
        // Only accept floating point types; else, compiler error
        static_assert(std::is_integral<T>::value,
            "resolve_integer() only accepts integral types.");

        //If we cannot parse because of `shutup()` settings, abort.
        if(!can_parse()){return *this;}

        int len = stringy::intlen(rhs, static_cast<int>(fmt.fmt_base), true) + 1;

        // Accepting defeat on VLAs - we'll just have to dynamically allocate.
        char* cstr = new char[len];
        std::fill_n(cstr, len, '\0');

        stringy::itoa(cstr, rhs, static_cast<int>(fmt.fmt_base), len, static_cast<bool>(fmt.fmt_numeral_case));
        inject(cstr);

        delete[] cstr;

        return *this;
    }

    /** Resolves any floating point number variable.
     * \param the floating point variable to resolve
     */
    template<typename T>
    channel& resolve_float(const T& rhs)
    {
        // Only accept floating point types; else, compiler error
        static_assert(std::is_floating_point<T>::value,
            "resolve_float() only accepts floating point types.");

        //If we cannot parse because of `shutup()` settings, abort.
        if(!can_parse()){return *this;}

        /*char cstr[stringy::floatlen(rhs, significands, sci, true) + 1];
        cstr[0] = '\0';*/

        // Accepting defeat on VLAs - we'll just have to dynamically allocate.
        uint32_t len = stringy::floatlen(rhs, fmt.fmt_significands.significands,
                                            static_cast<bool>(fmt.fmt_sci_notation), true) + 1;
        char* cstr = new char[len];
        std::fill_n(cstr, len, '\0');

        //Convert the float to a cstring, and dump into cstr.
        stringy::ftoa(cstr, rhs, fmt.fmt_significands.significands, static_cast<bool>(fmt.fmt_sci_notation));
        inject(cstr);

        delete[] cstr;

        return *this;
    }

    //The string containing the format.
    std::string format = "";

    /**Flush the standard output.*/
    void flush()
    {
        /* TODO: For non-standard outputs, this should attach a bytecode to
         * tell external outputs to flush.
         */

        //Flush is essential for progress-style outputs (after \r and no \n).
        switch(echo_mode)
        {
            case IOEchoMode::cout:
            {
                std::cout << std::flush;
                break;
            }
            case IOEchoMode::printf:
            {
                fflush(stdout);
                break;
            }
            case IOEchoMode::none:
            {
                break;
            }
        }
    }

    /**Move the cursor given the command.
     * \param the cursor command
     */
    void move_cursor(const IOCursor& rhs)
    {
        //TODO: We will need to switch formats. For now, just ANSI.
        switch(rhs)
        {
            case IOCursor::left:
                //NOTE: Watch this. \x1B is allegedly equal to \e, check it.
                inject("\x1B[1D");
                break;
            case IOCursor::right:
                //NOTE: Watch this. \x1B is allegedly equal to \e, check it.
                inject("\x1B[1C");
                break;
            default:
                // Can't happen unless you forgot to implement an IOCursor option!
                break;
        }
    }

    /**Insert a single character without need for null terminator.
     * \param the character to insert
     */
    void inject(char ch)
    {
        //If we just applied attributes, push them now.
        if(apply_attributes())
        {
            inject(format.c_str());
        }

        msg.push_back(ch);
    }

    /**Insert a C string into the output stream. Automatically applies
     * unapplied attributes before inserting text.
     * \param the C-string to insert.
     * \param whether the call was recursive. (Internal use only!)
     */
    void inject(const char* str, bool recursive=false)
    {
        //If we just applied attributes, push them now.
        if(!recursive && apply_attributes())
        {
            inject(format.c_str());
        }

        //Append to the message.
        msg.append(str);
    }

    /**Insert a memory address or its raw contents into the output
     * stream.
     * \param the address to insert
     * \param the size of the object referenced
     * \param whether to print literal address (false) or memory
     * dump (true); default false*/
    void inject(const void* ptr, unsigned int len, bool dump=false)
    {
        if(!dump)
        {
            uintptr_t address = reinterpret_cast<uintptr_t>(ptr);
            inject(stringy::ptrtos(address, static_cast<bool>(fmt.fmt_numeral_case)).c_str());
        }
        else if(dump)
        {
            unsigned int memsize = (len * 2) + 1;
            bool bytespacing = static_cast<bool>(fmt.fmt_mem_sep & IOFormatMemSep::byte);
            bool wordspacing = static_cast<bool>(fmt.fmt_mem_sep & IOFormatMemSep::word);
            if(bytespacing)
            {
                //Add a space for each byte except the last.
                memsize += len-1;
            }
            if(wordspacing)
            {
                //Add a bar for each word.
                memsize += (len / 8);

                if(bytespacing)
                {
                    memsize += (len / 8);
                }

                if(len % 8 == 0)
                {
                    //Remove one bar.
                    memsize--;

                    if(bytespacing)
                    {
                        //Remove one space.
                        memsize--;
                    }
                }
            }

            /*char mem[memsize];
            mem[0] = '\0';*/
            // Accepting defeat on VLAs - we'll just have to dynamically allocate.
            char* mem = new char[memsize];
            std::fill_n(mem, memsize, '\0');

            stringy::memdump(mem, ptr, len, false, static_cast<int>(fmt.fmt_mem_sep));
            inject(mem);
        }
    }

    /**Transmit the current pending output stream and reset in
    * preparation for the next message.
    * \param whether to retain the flags
    */
    void transmit(bool keep=false)
    {
        if(msg != "")
        {
            switch(vrb)
            {
                case IOVrb::quiet:
                {
                    // Dispatch the "quiet" verbosity signal.
                    signal_v_quiet(msg, cat);
                    /* Fall through, so the lower signals get emitted to.
                        * This allows outputs to connect to the HIGHEST
                        * verbosity they will allow, and get the rest regardless.
                        */
                    [[fallthrough]];
                }
                case IOVrb::normal:
                {
                    // Dispatch the "normal" verbosity signal.
                    signal_v_normal(msg, cat);
                    [[fallthrough]];
                }
                case IOVrb::chatty:
                {
                    // Dispatch the "chatty" verbosity signal.
                    signal_v_chatty(msg, cat);
                    [[fallthrough]];
                }
                case IOVrb::tmi:
                {
                    // Dispatch the "TMI" verbosity signal.
                    signal_v_tmi(msg, cat);
                    break;
                }
            }

            if(static_cast<bool>(cat & IOCat::normal))
            {
                // Dispatch the "normal" category signal.
                signal_c_normal(msg, vrb);
            }
            if(static_cast<bool>(cat & IOCat::debug))
            {
                // Dispatch the "debug" category signal.
                signal_c_debug(msg, vrb);
            }
            if(static_cast<bool>(cat & IOCat::warning))
            {
                // Dispatch the "warning" category signal.
                signal_c_warning(msg, vrb);
            }
            if(static_cast<bool>(cat & IOCat::error))
            {
                // Dispatch the "error" category signal.
                signal_c_error(msg, vrb);
            }
            if(static_cast<bool>(cat & IOCat::testing))
            {
                // Dispatch the "testing" category signal.
                signal_c_testing(msg, vrb);
            }

            // Dispatch the general purpose signals.
            signal_full(msg, vrb, cat);
            signal_all(msg);

            //If we are supposed to be echoing
            if(echo_mode != IOEchoMode::none)
            {
                //If the verbosity and category is correct...
                if(vrb <= echo_vrb && static_cast<bool>(cat | echo_cat))
                {
                    switch(echo_mode)
                    {
                        // If we're supposed to use `printf`...
                        case IOEchoMode::printf:
                        {
                            if(static_cast<bool>(cat & IOCat::error))
                            {
                                /* Route the message through stderr
                                    * instead of stdout.*/
                                fprintf(stderr, "%s", msg.c_str());
                            }
                            else
                            {
                                /* Route the message through stdout. */
                                printf("%s", msg.c_str());
                            }
                            break;
                        }
                        // If we're supposed to use `std::cout`...
                        case IOEchoMode::cout:
                        {
                            if(static_cast<bool>(cat & IOCat::error))
                            {
                                /* Route the message through stderr
                                    * instead of stdout.*/
                                std::cerr << msg.c_str();
                            }
                            else
                            {
                                /* Route the message through stdout. */
                                std::cout << msg.c_str();
                            }

                            break;
                        }
                        // This case is here for completeness...
                        case IOEchoMode::none:
                        {
                            // ...we should never reach this point!
                            break;
                        }
                    }
                }
            }

            /* If we aren't flagged to keep formatting,
                * reset the system in prep for the next message. */
            if(!keep)
            {
                reset_flags();
            }

            // Clear the message out in preparation for the next.
            clear_msg();

        }
    }

    ///Dirty flag raised when attributes are changed and not yet applied.
    bool dirty_attributes = false;

    /**Apply formatting attributes (usually ANSI) that are pending.
    * \param true if new attributes were applied
    */
    bool apply_attributes()
    {
        bool r = false;
        //If we have unapplied attributes.
        if(dirty_attributes)
        {
            //TODO: We will need to switch formats. For now, just ANSI.
            format = "\033[";
            format.append(stringy::itos(static_cast<int>(fmt.fmt_text_attr)));

            if(fmt.fmt_text_bg != IOFormatTextBG::none)
            {
                format.append(";");
                format.append(stringy::itos(static_cast<int>(fmt.fmt_text_bg)));
            }

            if(fmt.fmt_text_fg != IOFormatTextFG::none)
            {
                format.append(";");
                format.append(stringy::itos(static_cast<int>(fmt.fmt_text_fg)));
            }

            format.append("m");

            //We took care of the attributes. Lower the dirty flag.
            dirty_attributes = false;
            r = true;
        }
        return r;
    }

    /**Returns whether the vrb and cat match parsing (shutup) rules.*/
    bool can_parse()
    {
        /* If we aren't sure about the parsing condition... */
        if(~parse)
        {
            /* If the verbosity is in range or the category is set to parse,
                * then set false. Otherwise, set true. */
            parse = ((vrb <= process_vrb) && static_cast<bool>(process_cat & cat)) ? true : false;
        }
        return parse;
    }

    /**Clear the channel's message substring array.*/
    void clear_msg()
    {
        msg.clear();
        msg = "";
    }

    /**Reset all attributes.*/
    void reset_attributes()
    {
        if(fmt.fmt_text_attr != IOFormatTextAttr::none
            || fmt.fmt_text_fg != IOFormatTextFG::none
            || fmt.fmt_text_bg != IOFormatTextBG::none)
        {
            fmt << IOFormatTextAttr::none;
            fmt << IOFormatTextFG::none;
            fmt << IOFormatTextBG::none;
            dirty_attributes = true;
            /* We must leave calling `apply_attributes()` to `inject()`,
                * otherwise the reset attributes will never get injected
                * into the broadcast stream.
                */
        }
    }

    /**Reset all flags.*/
    void reset_flags()
    {
        //Reset all the flags.
        fmt = IOFormat();
        readsize = IOMemReadSize(1);

        //We reset the verbosity and category.
        vrb = IOVrb::normal;
        cat = IOCat::normal;
    }

};

///Global external (static) instance of channel.
inline channel out;

#endif // IOSQUEAK_BROADCAST_HPP
