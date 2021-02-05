#include "iosqueak/stringy.hpp"

using namespace stringy;

void stringy::split_string(std::string str,
								  std::string splitBy,
								  std::vector<std::string>& tokens)
{
	/* Store the original string in the array, so we can loop the rest
	 * of the algorithm. */
	tokens.push_back(str);

	// Store the size of what we're splicing out.
	size_t splitLen = splitBy.size();
	// Create a string for temporarily storing the fragment we're processing.
	std::string frag;

	// Loop infinitely - break is internal.
	while (true) {
		// Store the split index in a 'size_t' (unsigned integer) type.
		size_t splitAt;

		/* Store the last string in the vector, which is the only logical
		 * candidate for processing. */
		frag = tokens.back();
		/* The index where the split is. */
		splitAt = frag.find(splitBy);
		// If we didn't find a new split point...
		if (splitAt == std::string::npos) {
			// Break the loop and (implicitly) return.
			break;
		}
		/* Put everything from the left side of the split where the string
		 * being processed used to be. */
		tokens.back() = frag.substr(0, splitAt);
		/* Push everything from the right side of the split to the next empty
		 * index in the vector. */
		tokens.push_back(frag.substr(splitAt + splitLen,
									 frag.size() - (splitAt + splitLen)));
	}
}

char* stringy::reverse_c_string(char* str)
{
	if (str != NULL) {
		char* src;
		size_t len;

		len = strlen(str);
		if (len > 1) {
			char* dst;
			src = str;
			dst = src + len - 1;
			while (src < dst) {
				char tmp = *src;
				*src++ = *dst;
				*dst-- = tmp;
			}
		}
	}
	return str;
}
// TODO: Move this to onestring!
