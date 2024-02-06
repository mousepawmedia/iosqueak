#include "../include/iosqueak/blueshell/blueshell.hpp"

bool Blueshell::check_skip_list(const std::string& sent_cmd)
{;
    return (std::find_if(skipped_words.begin(), skipped_words.end(), [&sent_cmd](auto& word){return word == sent_cmd;}) != skipped_words.end());
}
