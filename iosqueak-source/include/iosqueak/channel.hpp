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

/* WHAT IS IOCHANNEL?
 * Channel is intended both as a replacement and wrapper for `std::iostream` and
 * `stdio.h/printf`. It allows for messages and errors to be output to multiple
 * sources simultaneously and asynchronously using signals. New
 * messages from any source are added to the back of the queue, and arbitrary
 * outputs can read them asynchronously from the front, either destructively or
 * non-destructively.
 *
 * Each output is also able to individually timing, as well as which messages it is
 * interested in and how it reads them, without interfering with the behavior
 * of other outputs.
 *
 * EXTERNAL OUTPUTS
 * An external output waits for a signal to be dispatched before it collects its
 * messages. Different signals are dispatched for different levels of verbosity
 * and categories.
 *
 * INTERNAL OUTPUTS
 * Optionally, channel can output to the terminal automatically via either
 * `std::iostream` or `stdio.h/printf`. This output can be controlled externally.
 * For example, a developer might choose to create pseudocommands in their
 * command-line that allow them to change verbosity on-the-fly while the program
 * is running.
 *
 * VERBOSITY
 * The concept of verbosity allows for developers to write and leave all manner
 * of useful output data, including debug information, detailed error messages,
 * and outright snark. Verbosity can be toggled globally for a channel,
 * or on a connection-by-connection basis.
 *
 * Verbosity ranges from 0 (only essential messages) to 3 (literally all messages).
 *
 * CATEGORY
 * Messages can be assigned a category, which makes it easier to different messages
 * to be sent to different outputs, or otherwise be handled differently. At the
 * moment, the categories are...
 * - Normal Messages
 * - Warnings
 * - Errors
 * - Debug Output
 *
 * CROSS-PLATFORM FORMATTING
 * Channel offers coloring and basic formatting on both UNIX and Windows systems
 * via the same interface.
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
#include "pawlib/onechar.hpp"
#include "pawlib/onestring.hpp"

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
        channel();

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
        channel& operator<<(const bool&);
        channel& operator<<(const tril&);
        channel& operator<<(const char&);
        channel& operator<<(const std::string&);
        channel& operator<<(const onechar&);
        channel& operator<<(const onestring&);

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

        template <typename T>
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
        inline channel& operator<<(const onechar* rhs){return resolve_pointer(rhs);}
        inline channel& operator<<(const onestring* rhs){return resolve_pointer(rhs);}

        channel& operator<<(const std::exception& rhs);

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

        channel& operator<<(const IOFormat&);
        channel& operator<<(const IOCursor&);
        channel& operator<<(const IOMemReadSize&);
        channel& operator<<(const IOVrb&);
        channel& operator<<(const IOCat&);
        channel& operator<<(const IOCtrl&);

        void configure_echo(IOEchoMode, IOVrb = IOVrb::tmi, IOCat = IOCat::all);

        /** Suppress a category from broadcasting at all.
             * \param the category to suppress
             */
        void shutup(IOCat);

        /** Suppress verbosities higher than the one given.
             * This is also mainly used to reset verbosity back to "normal"
             * (the default).
             * \param the maximum verbosity to permit (default=IOVrb::normal)
             */
        void shutup(IOVrb = IOVrb::normal);

        /** Permit a category to broadcast.
             * \param the category to broadcast
             */
        void speakup(IOCat);

        /** Permit a verbosity (and all below it) to broadcast.
             * Functionally identical to `shutup(IOVrb)`.
             * \param the maximum verbosity to permit
             */
        void speakup(IOVrb);

        /** Permit all messages (full verbosity and all categories)
             * to be broadcast.
             */
        void speakup();

        ~channel();
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

        template <typename T>
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
             * \param the pointer to resolve
             */
        template <typename T>
        channel& resolve_pointer(const T*);

        /** Resolves a void pointer, which allows
             * for raw data dumps from any pointer
             * imaginable.
             * \param the pointer to resolve
             */
        channel& resolve_pointer(const void*);

        /** Resolves a char pointer.
             * \param the pointer to resolve
             */
        channel& resolve_pointer(const char*);

        /** Resolves any integer.
             * \param the integer variable to resolve
             */
        template <typename T>
        channel& resolve_integer(const T&);

        /** Resolves any floating point number variable.
             * \param the floating point variable to resolve
             */
        template <typename T>
        channel& resolve_float(const T&);

        //The string containing the format.
        std::string format = "";

        /**Flush the standard output.*/
        void flush();

        /**Move the cursor given the command.
             * \param the cursor command
             */
        void move_cursor(const IOCursor&);

        /**Insert a single character without need for null terminator.
        * \param the character to insert
        */
        void inject(char);

        /**Insert a C string into the output stream. Automatically applies
        * unapplied attributes before inserting text.
        * \param the C-string to insert.
        * \param whether the call was recursive. (Internal use only!)
        */
        void inject(const char*, bool=false);

        /**Insert a memory address or its raw contents into the output
         * stream.
         * \param the address to insert
         * \param the size of the object referenced
         * \param whether to print literal address (false) or memory
         * dump (true); default false*/
        void inject(const void*, unsigned int len, bool=false);

        /**Transmit the current pending output stream and reset in
        * preparation for the next message.
        * \param whether to retain the flags
        */
        void transmit(bool=false);

        ///Dirty flag raised when attributes are changed and not yet applied.
        bool dirty_attributes = false;

        /**Apply formatting attributes (usually ANSI) that are pending.
        * \param true if new attributes were applied
        */
        bool apply_attributes();

        /**Returns whether the vrb and cat match parsing (shutup) rules.*/
        bool can_parse();

        /**Clear the channel's message substring array.*/
        void clear_msg();

        /**Reset all attributes.*/
        void reset_attributes();

        /**Reset all flags.*/
        void reset_flags();

};

///Global external (static) instance of channel.
extern channel ioc;

#endif // IOSQUEAK_BROADCAST_HPP
