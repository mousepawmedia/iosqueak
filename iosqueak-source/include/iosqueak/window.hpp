/** Window [IOSqueak]
 *
 * Retrieves information about the terminal window.
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

#ifndef IOSQUEAK_WINDOW_HPP
#define IOSQUEAK_WINDOW_HPP

#ifdef _WIN32
#include <windows.h>
#else
#include <csignal>
#include <cstdio>
#include <sys/ioctl.h>
#include <unistd.h>
#endif

class Window
{
protected:
	static unsigned short width;

	/// Retrieve the width of the terminal according to the native system
	static void poll_width(int = 0)
	{
		// Based on
		// https://www.linuxquestions.org/questions/programming-9/get-width-height-of-a-terminal-window-in-c-810739/
#ifdef _WIN32
		// Get terminal width on Windows.
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
#else
		// Handle two ways of retrieving terminal width on UNIX.
#ifdef TIOCGSIZE
		struct ttysize ts;
		auto _ = ioctl(STDIN_FILENO, TIOGSIZE, &ts);
		(void)_;
		width = ts.ts_cols;
#elif defined(TIOCGWINSZ)
		struct winsize ts;
		auto _ = ioctl(STDIN_FILENO, TIOCGWINSZ, &ts);
		(void)_;
		width = ts.ws_col;
#endif  // ifdef TIOCGSIZE
#endif  // ifdef _WIN32
	}

public:
	/** Retrieves the present terminal width.
	 * Attempts to minimize direct polling of terminal width;
	 * on UNIX, will only poll directly once, and will attach to the
	 * SIGWINCH signal to monitor terminal resizing.
	 */
	static unsigned short get_width()
	{
#ifdef _WIN32
		// On Windows systems, we can't use the SIGWINCH signal, so always poll.
		poll_width();
#else
#ifndef WINDOW_SIGNAL_SETUP
#define WINDOW_SIGNAL_SETUP
		// on UNIX systems, poll the width whenever terminal resizes
		std::signal(SIGWINCH, poll_width);
		poll_width();
#endif  // ifndef WINDOW_SIGNAL_SETUP
#endif  // ifndef _WIN32
		return width;
	}
};

#endif  // IOSQUEAK_WINDOW_HPP
