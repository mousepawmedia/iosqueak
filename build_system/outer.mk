# Outer Makefile (MousePaw Media Build System)
# Version: 3.2.1

# LICENSE
# Copyright (c) 2021 MousePaw Media.
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
	$(ECHO) "=== $(LIB_PROJECT) ==="
	$(ECHO) "Select a build target:"
	$(ECHO) "  make ready               Build $(LIB_PROJECT) and bundle it for distribution."
	$(ECHO) "  make clean"
	$(ECHO) "  make cleandebug"
	$(ECHO) "  make cleanrelease"
	$(ECHO) "  make docs"
	$(ECHO) "  make docs_pdf"
	$(ECHO) "  make $(LIB_NAME)"
	$(ECHO) "  make $(LIB_NAME)_debug"
	$(ECHO) "  make $(EXE_NICKNAME)"
	$(ECHO) "  make $(EXE_NICKNAME)_debug"
	$(ECHO) "  make all"
	$(ECHO) "  make allfresh"
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
	$(ECHO) "For other build options, see the 'make' command in '$(LIB_SRC)/, '$(EXE_SRC)/', and 'docs/'."

clean:
ifneq ($(HEADER_ONLY),1)
	$(MAKE) clean -C $(LIB_SRC)
endif
	$(MAKE) clean -C $(EXE_SRC)
	$(RM) $(EXE_NICKNAME)
	$(RM) $(EXE_NICKNAME)_debug
	$(RM_DIR) $(READY_DIR)

cleanall: clean
	$(MAKE) clean -C docs

cleandebug:
ifneq ($(HEADER_ONLY),1)
	$(MAKE) cleandebug -C $(LIB_SRC)
endif
	$(MAKE) cleandebug -C $(EXE_SRC)
	$(RM) $(EXE_NICKNAME)_debug

cleanrelease:
ifneq ($(HEADER_ONLY),1)
	$(MAKE) cleanrelease -C $(LIB_SRC)
endif
	$(MAKE) cleanrelease -C $(EXE_SRC)
	$(RM) $(EXE_NICKNAME)

docs:
	$(MAKE) html -C docs
	$(ECHO) "-------------"
	$(ECHO) "<<<<<<< FINISHED >>>>>>>"
	$(ECHO) "View docs at 'docs/build/html/index.html'."
	$(ECHO) "-------------"

docs_pdf:
	$(MAKE) latexpdf -C docs
	$(ECHO) "-------------"
	$(ECHO) "<<<<<<< FINISHED >>>>>>>"
	$(ECHO) "View docs at 'docs/build/latex/$(LIB_PROJECT).pdf'."
	$(ECHO) "-------------"

$(LIB_NAME):
ifneq ($(HEADER_ONLY),1)
	$(MAKE) release -C $(LIB_SRC)
endif

	$(ECHO) "-------------"
	$(ECHO) "<<<<<<< FINISHED >>>>>>>"

ifneq ($(HEADER_ONLY),1)
	$(ECHO) "$(LIB_PROJECT) is in '$(LIB_SRC)/lib/Release'."
else
	$(ECHO) "$(LIB_PROJECT) is a header-only library in '$(LIB_SRC)/include."
endif

	$(ECHO) "-------------"

$(LIB_NAME)_debug:
ifneq ($(HEADER_ONLY),1)
	$(MAKE) debug -C $(LIB_SRC)
endif

	$(ECHO) "-------------"
	$(ECHO) "<<<<<<< FINISHED >>>>>>>"

ifneq ($(HEADER_ONLY),1)
	$(ECHO) "$(LIB_PROJECT) is in '$(LIB_SRC)/lib/Debug'."
else
	$(ECHO) "$(LIB_PROJECT) is a header-only library in '$(LIB_SRC)/include."
endif

	$(ECHO) "-------------"

ready: $(LIB_NAME)
	$(RM_DIR) $(READY_DIR)
	$(ECHO) "Creating file structure..."
ifneq ($(HEADER_ONLY),1)
	$(MK_DIR) $(READY_DIR)/lib
endif
	$(ECHO) "Copying $(LIB_PROJECT)..."
	$(CP_DIR) $(LIB_SRC)/include/ $(LIB_NAME)/include/
ifneq ($(HEADER_ONLY),1)
	$(CP) $(LIB_SRC)/lib/Release/lib$(LIB_NAME).a $(LIB_NAME)/lib/lib$(LIB_NAME).a
endif

	$(ECHO) "Copying README and LICENSE..."
	$(CP) README.md $(READY_DIR)/README.md
	$(CP) LICENSE.md $(READY_DIR)/LICENSE.md
	$(ECHO) "-------------"
	$(ECHO) "<<<<<<< FINISHED >>>>>>>"
	$(ECHO) "The library is in '$(READY_DIR)'."
	$(ECHO) "-------------"

$(EXE_NICKNAME): $(LIB_NAME)
	$(MAKE) release -C $(EXE_SRC)
	$(RM) $(EXE_NICKNAME)
	$(LN) $(EXE_SRC)/bin/Release/$(EXE_SRC) $(EXE_NICKNAME)
	$(ECHO) "-------------"
	$(ECHO) "<<<<<<< FINISHED >>>>>>>"
	$(ECHO) "$(EXE_PROJECT) is in '$(EXE_SRC)/bin/Release'."
	$(ECHO) "The link './$(EXE_NICKNAME)' has been created for convenience."
	$(ECHO) "-------------"


$(EXE_NICKNAME)_debug: $(LIB_NAME)_debug
	$(MAKE) debug -C $(EXE_SRC)
	$(RM) $(EXE_NICKNAME)_debug
	$(LN) $(EXE_SRC)/bin/Debug/$(EXE_SRC) $(EXE_NICKNAME)_debug
	$(ECHO) "-------------"
	$(ECHO) "<<<<<<< FINISHED >>>>>>>"
	$(ECHO) "$(EXE_PROJECT) is in $(EXE_SRC)/bin/Debug'."
	$(ECHO) "The link './$(EXE_NICKNAME)_debug' has been created for convenience."
	$(ECHO) "-------------"

all: docs tester

allfresh: cleanall all

.PHONY: all allfresh clean cleanall cleandebug cleanrelease docs docs_pdf $(LIB_NAME) $(LIB_NAME)_debug ready $(EXE_NAME) $(EXE_NAME)_debug
