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

template<typename T>
T operator|(const T& lhs, const T& rhs);

template<typename T>
T operator&(const T& lhs, const T& rhs);

template<typename T>
T operator^(const T& lhs, const T& rhs);

template<typename T>
T operator~(const T& rhs);

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
