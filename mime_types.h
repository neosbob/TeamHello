#ifndef HTTP_MIME_TYPES_H
#define HTTP_MIME_TYPES_H

#include <string>

class mime_types {
	public:
	/// Convert a file extension into a MIME type.
	std::string extension_to_type(const std::string& extension);

}; // namespace mime_types


#endif // HTTP_MIME_TYPES_HPP

