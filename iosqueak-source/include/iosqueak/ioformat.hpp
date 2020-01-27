/** IOFormat [IOSqueak]
  * Version: 2.0.0
  *
  * Control and formatting flags for input and output.
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

#ifndef IOSQUEAK_IOFORMAT_HPP
#define IOSQUEAK_IOFORMAT_HPP

class channel;

/** The category of the message. */
enum class IOCat
{
    /// No category. Internal use only; no correlating signal.
    none = 0,
    /// The default value - anything that doesn't fit elsewhere.
    normal = 1,
    /// Warnings, but not necessarily errors.
    warning = 2,
    /// Error messages.
    error = 4,
    /// Debug messages, such as variable outputs.
    debug = 8,
    /// Testing messages that we may want shut off during benchmarking.
    testing = 16,
    /// All message categories. Internal use only; no correlating signal.
    all = 31
};

/** Controls the output of the Channel. */
enum class IOCtrl
{
    /// Send, keep formatting
    send = 1,
    clear = 2,
    r = 4,
    n = 8,
    flush = 16,
    /// Send with carriage return (\r), keep formatting
    sendc = 1 | 4 | 16,
    /// Send with line feed (\n), keep formatting
    sendl = 1 | 8 | 16,
    /// Send, clear formatting
    end = 1 | 2,
    /// End with carriage return (\r), clear formatting
    endc = 1 | 2 | 4 | 16,
    /// End with line feed (\n), clear formatting
    endl = 1 | 2 | 8 | 16,
};

/** Basic cursor movement. */
enum class IOCursor
{
    // Move cursor left ('\[1D]')
    left,
    // Move cursor right ('\[1C]')
    right
};

enum class IOEchoMode
{
    /** Don't automatically output messages via the stdout echo. */
    none = 0,
    /** Output messages to stdout via C-style `printf`. */
    printf = 1,
    /** Output messages to stdout via C++-style `std::cout`. */
    cout = 2
};

enum class IOFormatBase
{
    bin = 2,
    b2 = 2,
    ter = 3,
    b3 = 3,
    quat = 4,
    b4 = 4,
    quin = 5,
    b5 = 5,
    sen = 6,
    b6 = 6,
    sep = 7,
    b7 = 7,
    oct = 8,
    b8 = 8,
    b9 = 9,
    dec = 10,
    b10 = 10,
    und = 11,
    b11 = 11,
    duo = 12,
    doz = 12,
    b12 = 12,
    tri = 13,
    b13 = 13,
    tetra = 14,
    b14 = 14,
    pent = 15,
    b15 = 15,
    hex = 16,
    b16 = 16,
    b17 = 17,
    b18 = 18,
    b19 = 19,
    vig = 20,
    b20 = 20,
    b21 = 21,
    b22 = 22,
    b23 = 23,
    b24 = 24,
    b25 = 25,
    b26 = 26,
    b27 = 27,
    b28 = 28,
    b29 = 29,
    b30 = 30,
    b31 = 31,
    b32 = 32,
    b33 = 33,
    b34 = 34,
    b35 = 35,
    b36 = 36
};

enum class IOFormatBool
{
    lower = 0,
    upper = 1,
    caps = 2,
    numeral = 3,
    scott = 4
};

enum class IOFormatCharValue
{
    as_char = 0,
    as_int = 1
};

enum class IOFormatMemSep
{
    ///Output as one long string.
    none = 0,
    ///Output with spaces between bytes.
    byte = (1 << 0),
    ///Output with bars between words (8 bytes).
    word = (1 << 1),
    ///Output with spaces between bytes and bars between words.
    all = 3
};

enum class IOFormatNumCase
{
    ///Print all letter digits as lowercase.
    lower = 0,
    ///Print all letter digits as uppercase.
    upper = 1
};

enum class IOFormatPtr
{
    ///Print the value at the address.
    value = 0,
    ///Print the actual memory address.
    address = 1,
    ///Dump the hexadecimal representation of the memory at address.
    memory = 2
};

enum class IOFormatSciNotation
{
    ///Turn off all scientific notation.
    none = 0,
    ///Automatically select the best option.
    automatic = 1,
    ///Turn on all scientific notation.
    on = 2
};

struct IOFormatSignificands
{
    explicit IOFormatSignificands(unsigned int s):significands(s){}
    int significands = 14;
};

/**The standard ANSI text attributes.*/
enum class IOFormatTextAttr
{
    ///Turn of all attributes.
    none = 0,
    ///Bold text.
    bold = 1,
    ///Underlined text.
    underline = 4,
    ///Inverted text colors, also known as "reverse video".
    invert = 6
};
// TODO: Change `IOFormatTextAttr` to a bitfield, to allow multiple format flags.

/** The standard ANSI text background colors. */
enum class IOFormatTextBG
{
    //None.
    none = 0,
    ///Black text background.
    black = 40,
    ///Red text background.
    red = 41,
    ///Green text background.
    green = 42,
    ///Yellow text background.
    yellow = 43,
    ///Blue text background.
    blue = 44,
    ///Magenta text background.
    magenta = 45,
    ///Cyan text background.
    cyan = 46,
    ///White text background.
    white = 47
};

/** The standard ANSI text foreground colors. */
enum class IOFormatTextFG
{
    //None.
    none = 0,
    ///Black text.
    black = 30,
    ///Red text.
    red = 31,
    ///Green text
    green = 32,
    ///Yellow text.
    yellow = 33,
    ///Blue text.
    blue = 34,
    ///Magenta text.
    magenta = 35,
    ///Cyan text.
    cyan = 36,
    ///White text.
    white = 37
};

/**Indicate how many bytes to read from any pointer that isn't
 * recognized explicitly by channel, including void pointers.
 * This will not override the memory dump read size of built-in types.*/
struct IOMemReadSize
{
    /**Indicate how many bytes to read from any pointer that isn't
     * recognized explicitly by channel, including void pointers.
     * This will not override the memory dump read size of built-in
     * types.
     * CAUTION: Misuse can cause SEGFAULT or other memory errors.
     * \param the number of bytes to read*/
    explicit IOMemReadSize(unsigned int i):readsize(i){}
    unsigned int readsize = 1;

    void operator=(unsigned int rhs)
    {
        readsize = rhs;
    }
};

/** The level of verbosity necessary for the message to display. */
enum class IOVrb
{
    /**Only essential messages and errors. For normal end-use.
    Shipping default.*/
    quiet = 0,
    /**Common messages and errors. For common and normal end-user
    testing.*/
    normal = 1,
    /**Most messages and errors. For detailed testing and
    debugging.*/
    chatty = 2,
    /**Absolutely everything. For intense testing, detailed
    debugging, and driving the developers crazy.*/
    tmi = 3
};

class IOFormat
{
    friend channel;

    private:
        IOFormatBase fmt_base;
        IOFormatBool fmt_bool;
        IOFormatCharValue fmt_char_value;
        IOFormatMemSep fmt_mem_sep;
        IOFormatNumCase fmt_numeral_case;
        IOFormatPtr fmt_ptr;
        IOFormatSciNotation fmt_sci_notation;
        IOFormatSignificands fmt_significands;
        IOFormatTextAttr fmt_text_attr;
        IOFormatTextBG fmt_text_bg;
        IOFormatTextFG fmt_text_fg;

    public:
        IOFormat()
        : fmt_base(IOFormatBase::b10),
            fmt_bool(IOFormatBool::lower),
            fmt_char_value(IOFormatCharValue::as_char),
            fmt_mem_sep(IOFormatMemSep::all),
            fmt_numeral_case(IOFormatNumCase::upper),
            fmt_ptr(IOFormatPtr::value),
            fmt_sci_notation(IOFormatSciNotation::automatic),
            fmt_significands(IOFormatSignificands(14)),
            fmt_text_attr(IOFormatTextAttr::none),
            fmt_text_bg(IOFormatTextBG::none),
            fmt_text_fg(IOFormatTextFG::none)
            {}

        IOFormat(const IOFormat& cpy)
        : fmt_base(cpy.fmt_base),
            fmt_bool(cpy.fmt_bool),
            fmt_char_value(cpy.fmt_char_value),
            fmt_mem_sep(cpy.fmt_mem_sep),
            fmt_numeral_case(cpy.fmt_numeral_case),
            fmt_ptr(cpy.fmt_ptr),
            fmt_sci_notation(cpy.fmt_sci_notation),
            fmt_significands(cpy.fmt_significands),
            fmt_text_attr(cpy.fmt_text_attr),
            fmt_text_bg(cpy.fmt_text_bg),
            fmt_text_fg(cpy.fmt_text_fg)
            {}

        IOFormat& operator=(const IOFormat& cpy)
        {
            fmt_base = cpy.fmt_base;
            fmt_bool = cpy.fmt_bool;
            fmt_char_value = cpy.fmt_char_value;
            fmt_mem_sep = cpy.fmt_mem_sep;
            fmt_numeral_case = cpy.fmt_numeral_case;
            fmt_ptr = cpy.fmt_ptr;
            fmt_sci_notation = cpy.fmt_sci_notation;
            fmt_significands = cpy.fmt_significands;
            fmt_text_attr = cpy.fmt_text_attr;
            fmt_text_bg = cpy.fmt_text_bg;
            fmt_text_fg = cpy.fmt_text_fg;
            return *this;
        }

        IOFormat& operator<<(const IOFormatBase& rhs){fmt_base = rhs; return *this;}
        IOFormat& operator<<(const IOFormatBool& rhs){fmt_bool = rhs; return *this;}
        IOFormat& operator<<(const IOFormatCharValue& rhs){fmt_char_value = rhs; return *this;}
        IOFormat& operator<<(const IOFormatMemSep& rhs){fmt_mem_sep = rhs; return *this;}
        IOFormat& operator<<(const IOFormatNumCase& rhs){fmt_numeral_case = rhs; return *this;}
        IOFormat& operator<<(const IOFormatPtr& rhs){fmt_ptr = rhs; return *this;}
        IOFormat& operator<<(const IOFormatSciNotation& rhs){fmt_sci_notation = rhs; return *this;}
        IOFormat& operator<<(const IOFormatSignificands& rhs){fmt_significands = rhs; return *this;}
        IOFormat& operator<<(const IOFormatTextAttr& rhs){fmt_text_attr = rhs; return *this;}
        IOFormat& operator<<(const IOFormatTextBG& rhs){fmt_text_bg = rhs; return *this;}
        IOFormat& operator<<(const IOFormatTextFG& rhs){fmt_text_fg = rhs; return *this;}
};

template<typename T>
T operator&(const T& lhs, const T& rhs)
{
    return static_cast<T>
        (static_cast<int>(lhs) & static_cast<int>(rhs));
}
template<> IOCat operator&<IOCat>(const IOCat&, const IOCat&);
template<> IOCtrl operator&<IOCtrl>(const IOCtrl&, const IOCtrl&);
template<> IOFormatMemSep operator&<IOFormatMemSep>(const IOFormatMemSep&, const IOFormatMemSep&);

template<typename T>
T operator|(const T& lhs, const T& rhs)
{
    return static_cast<T>
        (static_cast<int>(lhs) | static_cast<int>(rhs));
}
template<> IOCat operator|<IOCat>(const IOCat&, const IOCat&);
template<> IOCtrl operator|<IOCtrl>(const IOCtrl&, const IOCtrl&);
template<> IOFormatMemSep operator|<IOFormatMemSep>(const IOFormatMemSep&, const IOFormatMemSep&);

template<typename T>
T operator^(const T& lhs, const T& rhs)
{
    return static_cast<T>
        (static_cast<int>(lhs) ^ static_cast<int>(rhs));
}
// The following linter error is from misparsing of the file.
// cppcheck-suppress internalAstError
template<> IOCat operator^<IOCat>(const IOCat&, const IOCat&);
template<> IOCtrl operator^<IOCtrl>(const IOCtrl&, const IOCtrl&);
template<> IOFormatMemSep operator^<IOFormatMemSep>(const IOFormatMemSep&, const IOFormatMemSep&);

template<typename T>
T operator~(const T& rhs)
{
    return static_cast<T>(~static_cast<int>(rhs));
}
template<> IOCat operator~<IOCat>(const IOCat&);
template<> IOCtrl operator~<IOCtrl>(const IOCtrl&);
template<> IOFormatMemSep operator~<IOFormatMemSep>(const IOFormatMemSep&);

#endif //IOSQUEAK_IOFORMAT_HPP
