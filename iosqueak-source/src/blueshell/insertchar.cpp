#include "../include/iosqueak/blueshell/blueshell.hpp"

void Blueshell::insert_char(std::string& sent_command, int keypress)
{
	bool prev_check = sent_command.back() == '\\';

	/* Set outer_quote or outer_single to true when
	 * when " or ' is first typed. Set to false when
	 * the closing one is typed. */
	if (keypress == 34 && (!outer_quote && !outer_single) && !prev_check) {
		outer_quote = true;
	} else if (keypress == 34 && outer_quote && !prev_check) {
		outer_quote = false;
	} else if (keypress == 39 && (!outer_single && !outer_quote) &&
			   !prev_check) {
		outer_single = true;
	} else if (keypress == 39 && outer_single && !prev_check) {
		outer_single = false;
	}

	/* Set inner_quote or inner_single to true when
	 * when " or ' is first typed. Set to false when
	 * the closing one is typed. */
	if ((outer_quote || outer_single) && prev_check && !inner_quote &&
		keypress == 34) {
		inner_quote = true;
	} else if ((outer_quote || outer_single) && prev_check && inner_quote &&
			   keypress == 34) {
		inner_quote = false;
	} else if ((outer_quote || outer_single) && prev_check && !inner_single &&
			   keypress == 39) {
		inner_single = true;
	} else if ((outer_quote || outer_single) && prev_check && inner_single &&
			   keypress == 39) {
		inner_single = false;
	}

	if (cursor_moves <= static_cast<int>(sent_command.size())) {
		sent_command.insert(sent_command.size() - cursor_moves,
							1,
							static_cast<char>(keypress));
	}
}
