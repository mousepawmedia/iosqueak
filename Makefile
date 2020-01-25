# Top Level Makefile (MousePaw Media Build System)
# Version: 2.1.0
# Tailored For: IOSqueak
#
# Author(s): Jason C. McDonald

# LICENSE (BSD-3-Clause)
# Copyright (c) 2018 MousePaw Media.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
# this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following disclaimer in the documentation
# and/or other materials provided with the distribution.
#
# 3. Neither the name of the copyright holder nor the names of its contributors
# may be used to endorse or promote products derived from this software without
# specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
# THE POSSIBILITY OF SUCH DAMAGE.
#
# CONTRIBUTING
# See https://www.mousepawmedia.com/developers for information
# on how to contribute to our projects.

MK_DIR = @cmake -E make_directory
CH_DIR = @cmake -E chdir
CP = @cmake -E copy
CP_DIR = @cmake -E copy_directory
RM = @cmake -E remove
RM_DIR = @cmake -E remove_directory
ECHO = @cmake -E echo
LN = @cmake -E create_symlink

none: help

help:
	$(ECHO) "=== IOSqueak 2.0 ==="
	$(ECHO) "Select a build target:"
	$(ECHO) "  make ready           Build IOSqueak and bundles it for distribution."
	$(ECHO) "  make clean           Clean up IOSqueak and Tester."
	$(ECHO) "  make cleandebug      Clean up IOSqueak and Tester Debug."
	$(ECHO) "  make cleanrelease    Clean up IOSqueak and Tester Release."
	$(ECHO) "  make docs            Generate HTML docs."
	$(ECHO) "  make docs_pdf        Generate PDF docs."
	$(ECHO) "  make iosqueak        Build IOSqueak as release."
	$(ECHO) "  make iosqueak_debug  Build IOSqueak as debug."
	$(ECHO) "  make tester          Build IOSqueak Tester (+IOSqueak) as release."
	$(ECHO) "  make tester_debug    Build IOSqueak Tester (+IOSqueak) as debug."
	$(ECHO) "  make all             Build everything."
	$(ECHO) "  make allfresh        Clean and rebuild everything."
	$(ECHO)
	$(ECHO) "Clang Sanitizers (requires Debug build and Clang.)"
	$(ECHO) "  SAN=address     Use AddressSanitizer"
	$(ECHO) "  SAN=leak        Use LeakSanitizer w/o AddressSanitizer (Linux only)"
	$(ECHO) "  SAN=memory      Use MemorySanitizer"
	$(ECHO) "  SAN=thread      Use ThreadSanitizer"
	$(ECHO) "  SAN=undefined   Use UndefiniedBehaviorSanitizer"
	$(ECHO)
	$(ECHO) "Optional Architecture"
	$(ECHO) "  ARCH=32         Make x86 build (-m32)"
	$(ECHO) "  ARCH=64         Make x64 build (-m64)"
	$(ECHO)
	$(ECHO) "Use Configuration File"
	$(ECHO) "  CONFIG=foo      Uses the configuration file 'foo.config'"
	$(ECHO) "                  in the root of this repository."
	$(ECHO) "  When unspecified, default.config will be used."
	$(ECHO)
	$(ECHO) "For other build options, see the 'make' command in 'docs/', 'iosqueak-source/', and 'iosqueak-tester/'."
.PHONY: help

clean:
	$(MAKE) clean -C iosqueak-source
	$(MAKE) clean -C iosqueak-tester
	$(RM) tester_debug
	$(RM) tester
.PHONY: clean

cleanall: clean
	$(MAKE) clean -C docs
.PHONY: cleanall

cleandebug:
	$(MAKE) cleandebug -C iosqueak-source
	$(MAKE) cleandebug -C iosqueak-tester
	$(RM) tester_debug
.PHONY: cleandebug

cleanrelease:
	$(MAKE) cleanrelease -C iosqueak-source
	$(MAKE) cleanrelease -C iosqueak-tester
	$(RM) tester
.PHONY: cleanrelease

docs:
	$(RM_DIR) docs/build/html
	$(MAKE) html -C docs
	$(ECHO) "-------------"
	$(ECHO) "<<<<<<< FINISHED >>>>>>>"
	$(ECHO) "View docs at 'docs/build/html/index.html'."
	$(ECHO) "-------------"
.PHONY: docs

docs_pdf:
	$(MAKE) latexpdf -C docs
	$(ECHO) "-------------"
	$(ECHO) "<<<<<<< FINISHED >>>>>>>"
	$(ECHO) "View docs at 'docs/build/latex/IOSqueak.pdf'."
	$(ECHO) "-------------"
.PHONY: docs_pdf

iosqueak:
	$(ECHO) "-------------"
	$(ECHO) "<<<<<<< FINISHED >>>>>>>"
	$(ECHO) "IOSqueak is in 'iosqueak-source/lib/Release'."
	$(ECHO) "-------------"
.PHONY: iosqueak

iosqueak_debug:
	$(ECHO) "-------------"
	$(ECHO) "<<<<<<< FINISHED >>>>>>>"
	$(ECHO)  on "IOSqueak is in 'iosqueak-source/lib/Debug'."
	$(ECHO) "-------------"
.PHONY: iosqueak_debug

ready: iosqueak
	$(RM_DIR) iosqueak
	$(ECHO) "Creating file structure..."
	$(MK_DIR) iosqueak
	$(ECHO) "Copying IOSqueak..."
	$(CP_DIR) iosqueak-source/include/ iosqueak/include/
	$(ECHO) "Copying README and LICENSE..."
	$(CP) README.md iosqueak/README.md
	$(CP) LICENSE.md iosqueak/LICENSE.md
	$(ECHO) "-------------"
	$(ECHO) "<<<<<<< FINISHED >>>>>>>"
	$(ECHO) "The library is in 'iosqueak'."
	$(ECHO) "-------------"
.PHONY: ready

tester: iosqueak
	$(MAKE) release -C iosqueak-tester
	$(RM) tester
	$(LN) iosqueak-tester/bin/Release/iosqueak-tester tester
	$(ECHO) "-------------"
	$(ECHO) "<<<<<<< FINISHED >>>>>>>"
	$(ECHO) "IOSqueak Tester is in 'iosqueak-tester/bin/Release'."
	$(ECHO) "The link './tester' has been created for convenience."
	$(ECHO) "-------------"
.PHONY: tester

tester_debug: iosqueak_debug
	$(MAKE) debug -C iosqueak-tester
	$(RM) tester_debug
	$(LN) iosqueak-tester/bin/Debug/iosqueak-tester tester_debug
	$(ECHO) "-------------"
	$(ECHO) "<<<<<<< FINISHED >>>>>>>"
	$(ECHO) "IOSqueak Tester is in 'iosqueak-tester/bin/Debug'."
	$(ECHO) "The link './tester_debug' has been created for convenience."
	$(ECHO) "-------------"
.PHONY: tester_debug

all: docs tester
.PHONY: all

allfresh: cleanall all
.PHONY: allfresh
