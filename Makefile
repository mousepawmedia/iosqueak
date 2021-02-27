# Top Level Makefile (MousePaw Media Build System)
# Version: 3.1.0

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

# CHANGE: Project name
PROJECT = IOSqueak
# CHANGE: Project filename
NAME = iosqueak

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
	$(ECHO) "=== $(PROJECT) ==="
	$(ECHO) "Select a build target:"
	$(ECHO) "  make ready         Build $(PROJECT) and bundles it for distribution."
	$(ECHO) "  make clean         Clean up $(PROJECT) and Tester."
	$(ECHO) "  make cleandebug    Clean up $(PROJECT) and Tester Debug."
	$(ECHO) "  make cleanrelease  Clean up $(PROJECT) and Tester Release."
	$(ECHO) "  make docs          Generate HTML docs."
	$(ECHO) "  make docs_pdf      Generate PDF docs."
	$(ECHO) "  make $(NAME)         Build $(PROJECT) as release."
	$(ECHO) "  make $(NAME)_debug   Build $(PROJECT) as debug."
	$(ECHO) "  make tester        Build $(PROJECT) Tester (+$(PROJECT)) as release."
	$(ECHO) "  make tester_debug  Build $(PROJECT) Tester (+$(PROJECT)) as debug."
	$(ECHO) "  make all           Build everything."
	$(ECHO) "  make allfresh      Clean and rebuild everything."
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
	$(ECHO) "For other build options, see the 'make' command in 'docs/', '$(NAME)-source/', '$(NAME)-tester/', and 'cpgf/build/'."

clean:
	$(MAKE) clean -C $(NAME)-source
	$(MAKE) clean -C $(NAME)-tester
	$(RM) tester
	$(RM) tester_debug
	$(RM_DIR) $(NAME)

cleanall: clean
	$(MAKE) clean -C docs

cleandebug:
	$(MAKE) cleandebug -C $(NAME)-source
	$(MAKE) cleandebug -C $(NAME)-tester
	$(RM) tester_debug

cleanrelease:
	$(MAKE) cleanrelease -C $(NAME)-source
	$(MAKE) cleanrelease -C $(NAME)-tester
	$(RM) tester

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
	$(ECHO) "View docs at 'docs/build/latex/$(PROJECT).pdf'."
	$(ECHO) "-------------"

$(NAME):
	$(MAKE) release -C $(NAME)-source
	$(ECHO) "-------------"
	$(ECHO) "<<<<<<< FINISHED >>>>>>>"
	$(ECHO) "$(PROJECT) is in '$(NAME)-source/lib/Release'."
	$(ECHO) "-------------"

$(NAME)_debug:
	$(MAKE) debug -C $(NAME)-source
	$(ECHO) "-------------"
	$(ECHO) "<<<<<<< FINISHED >>>>>>>"
	$(ECHO) "$(PROJECT) is in '$(NAME)-source/lib/Debug'."
	$(ECHO) "-------------"

ready: $(NAME)
	$(RM_DIR) $(NAME)
	$(ECHO) "Creating file structure..."
	$(MK_DIR) $(NAME)/lib
	$(ECHO) "Copying $(PROJECT)..."
	$(CP_DIR) $(NAME)-source/include/ $(NAME)/include/
	$(CP) $(NAME)-source/lib/Release/lib$(NAME).a $(NAME)/lib/lib$(NAME).a
	$(ECHO) "Copying README and LICENSE..."
	$(CP) README.md $(NAME)/README.md
	$(CP) LICENSE.md $(NAME)/LICENSE.md
	$(ECHO) "-------------"
	$(ECHO) "<<<<<<< FINISHED >>>>>>>"
	$(ECHO) "The libraries are in '$(NAME)'."
	$(ECHO) "-------------"

tester: $(NAME)
	$(MAKE) release -C $(NAME)-tester
	$(RM) tester
	$(LN) $(NAME)-tester/bin/Release/$(NAME)-tester tester
	$(ECHO) "-------------"
	$(ECHO) "<<<<<<< FINISHED >>>>>>>"
	$(ECHO) "$(PROJECT) Tester is in '$(NAME)-tester/bin/Release'."
	$(ECHO) "The link './tester' has been created for convenience."
	$(ECHO) "-------------"


tester_debug: $(NAME)_debug
	$(MAKE) debug -C $(NAME)-tester
	$(RM) tester_debug
	$(LN) $(NAME)-tester/bin/Debug/$(NAME)-tester tester_debug
	$(ECHO) "-------------"
	$(ECHO) "<<<<<<< FINISHED >>>>>>>"
	$(ECHO) "$(PROJECT) Tester is in '$(NAME)-tester/bin/Debug'."
	$(ECHO) "The link './tester_debug' has been created for convenience."
	$(ECHO) "-------------"

all: docs tester

allfresh: cleanall all

.PHONY: all allfresh clean cleanall cleandebug cleanrelease docs docs_pdf $(NAME) $(NAME)_debug ready tester tester_debug
