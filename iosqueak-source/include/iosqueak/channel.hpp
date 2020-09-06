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

// Needed for output sources.
#include <iostream>
#include <stdio.h>

// Signals and callbacks.
#include <string>

#include "eventpp/callbacklist.h"

// We use C's classes often.
#include <cstdio>

#include "iosqueak/ioformat.hpp"
#include "iosqueak/stringify.hpp"
#include "pawlib/core_types.hpp"

class channel
{
protected:
	std::string msg;

	/// Which categories are permitted.
	IOCat process_cat = IOCat::all;
	/// The maximum verbosity to permit.
	IOVrb process_vrb = IOVrb::tmi;

	// Which method should be used for Channel's default standard stream echo?
	IOEchoMode echo_mode = IOEchoMode::printf;
	// The categories to echo.
	IOCat echo_cat = IOCat::all;
	// The maximum verbosity to echo.
	IOVrb echo_vrb = IOVrb::tmi;

	// Message attributes.
	IOFormat fmt;
	IOVrb vrb = IOVrb::normal;
	IOCat cat = IOCat::normal;

    /// The string containing the formatting codes.
    std::string format_str = "";

	// Message parsable?
	tril parse = maybe;

	/** Returns whether the verbosity and category match parsing rules. */
	bool can_parse()
	{
		// If we aren't sure about the parsing condition...
		if (~parse)
		{
			/* Proceed if the verbosity is in range
			 * and the category is set to parse. */
			parse =
				((vrb <= process_vrb) && static_cast<bool>(process_cat & cat))
					? true
					: false;
		}
        return parse;
	}

	template<typename T>
	bool process_fmt(T val)
	{
		// If we cannot parse, abort.
		if (!can_parse())
		{
			return false;
		}

		fmt << val;
		return true;
	}

    /** Flush the standard output. */
    void flush()
    {
        /* TODO: For non-standard outputs, this should attach a bytecode to
         * tell external outputs to flush.
         */

        // Flush is essential for progress-style outputs (after \r and no \n)
        switch (echo_mode)
        {
            case IOEchoMode::cout:
                std::cout << std::flush;
                break;
            case IOEchoMode::printf:
                fflush(stdout);
                break;
            case IOEchoMode::none:
                break;
        }
    }

    /** Move the cursor given the command.
     * \param the cursor command
     */
    void move_cursor(const IOCursor& rhs)
    {
        // TODO: Support non-ANSI terminals.
        switch (rhs)
        {
            case IOCursor::left:
                // NOTE: Watch this. \x1B is allegedly equal to \e, check it.
                inject("\x1B[1D");
                break;
            case IOCursor::right:
                // NOTE: Watch this \x1B is allegedly equal to \e, check it.
                inject("\x1B[1C");
                break;
            default:
                /* Can't happen unless you forgot to implement an
                 * IOCursor option! */
                assert false;
        }
    }

    /** Insert a single character without need for null terminator.
     * \param the character to insert
     */
    void inject(char ch)
    {
        // If we just applied attributes, push them now.
        if (apply_attributes())
        {
            inject(format.c_str());
        }
        msg.push_base(ch);
    }

    /** Insert a C string into the output stream. Automatically applies
     * unapplied attributes before inserting text.
     * \param the C string to insert
     * \param whether the call was recursive. (Internal use only!)
     */
    void inject(const char* str, bool recursive = false)
    {
        // If we just applied attributes, push them now.
        if (!recursive && apply_attributes())
        {
            inject(format.c_str(), true);
        }

        // Append to the message.
        msg.append(str);
    }

    /** Transmit the current pending output stream and reset in
     * preparation for the next message.
     * \param whether to retain the flags
     */
    void transmit(bool keep = false)
    {
        if (msg != "")
        {
            switch (vrb)
            {
                case IOVrb::quiet:
                    // Dispatch on the "quiet" verbosity signal.
                    signal_v_quiet(msg, cat);
                    /* Fall through, so the lower signals get emitted too.
                     * This allows outputs to connect to the HIGHEST
                     * verbosity they will allow, and get the lower verbosity
                     * messages regardless.
                     */
                    [[fallthrough]];
                case IOVrb::normal:
                    // Dispatch on the "normal" verbosity signal.
                    signal_v_normal(msg, cat);
                    [[fallthrough]]
                case IOVrb::chatty:
                    // Dispatch on the "chatty" verbosity signal.
                    signal_v_chatty(msg, cat);
                    [[fallthrough]];
                case IOVrb::tmi:
                    // Dispatch on the "TMI" verbosity signal.
                    signal_v_tmi(msg, cat);
                    break;
            }

            if (static_cast<bool>(cat & IOCat::normal))
            {
                // Dispatch on the "normal" category signal.
                signal_c_normal(msg, vrb);
            }
            if (static_cast<bool>(cat & IOCat::debug))
            {
                // Dispatch on the "debug" category signal.
                signal_c_debug(msg, vrb);
            }
            if (static_cast<bool>(cat & IOCat::warning))
            {
                // Dispatch on the "warning" category signal.
                signal_c_warning(msg, vrb);
            }
            if (static_cast<bool>(cat & IOCat::error))
            {
                // Dispatch on the "error" category signal.
                signal_c_error(msg, vrb);
            }
            if (static_cast<bool>(cat & IOCat::testing))
            {
                // Dispatch on the "testing" category signal.
                signal_c_testing(msg, vrb);
            }

            // Dispatch the general purpose signals.
            signal_full(msg, vrb, cat);
            signal_all(msg);

            // If we are supposed to be echoing...
            if (echo_mode != IOEchoMode::none)
            {
                // If the verbosity and category is correct...
                if (vrb <= echo_vrb && static_cast<bool>(cat | echo_cat))
                {
                    switch (echo_mode)
                    {
                        // If we're supposed to use `printf`...
                        case IOEchoMode::printf:
                            // For error messages, echo to stderr instead.
                            if (static_cast<bool>(cat & IOCat::error))
                            {
                                fprintf(stderr, "%s", msg.c_str());
                            }
                            // For all other messages, echo to stdout.
                            else
                            {
                                printf("%s", msg.c_str());
                            }
                            break;
                        // If we're supposed to use std::cout...
                        case IOEchoMode::cout:
                            // For error messages, echo to stderr instead.
                            if (static_cast<bool>(cat & IOCat::error))
                            {
                                std::cerr << msg.c_str();
                            }
                            // For all other messages, echo to stdout.
                            else
                            {
                                std::cout << msg.c_str();
                            }
                            break;
                    }
                    // This case is here for completeness...
                    case IOEchoMode::none:
                        // ...we should never reach this point!
                        assert false;
                        break;
                }
            }

            /* If we aren't flagged to keep formatting,
             * reset the system in prep for the next message.
             */
            if (!keep)
            {
                reset_flags();
            }

            // Clear the message out in preparation for the next.
            clear_msg();
        }
    }

    /// Dirty flag raised when attributes are changed and not yet applied.
    bool dirty_attributes = false;

    bool apply_attributes()
    {
        bool r = false;
        // If we have unapplied attributes.
        if (dirty_attributes)
        {
            // TODO: Support non-ANSI formats.
            // TODO: Add stringify functions to handle this!
        }
    }

public:
	channel() {}

	/// Signal for categories.
	typedef eventpp::CallbackList<void(std::string, IOCat)> IOSignalCat;

	/// Signal for verbosities.
	typedef eventpp::CallbackList<void(std::string, IOVrb)> IOSignalVrb;

	/// Signal for everything.
	typedef eventpp::CallbackList<void(std::string, IOVrb, IOCat)> IOSignalFull;

	/// Signal for everything, sans verbosity and category.
	typedef eventpp::CallbackList<void(std::string)> IOSignalAll;

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
};

#endif
