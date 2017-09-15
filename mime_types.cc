#include "mime_types.h"


struct mapping
{
    const char* extension;
    const char* mime_type;
} mappings[] =
{
    { "gif", "image/gif" },
    { "htm", "text/html" },
    { "html", "text/html" },
    { "jpg", "image/jpeg" },
    { "png", "image/png" },
    { "jpeg", "image/jpeg"},
    { "md", "text/markdown"},
    { "xml", "application/xml"},
    { "php", "text/html; charset=UTF-8"},
    { "css", "text/css"}
};

std::string mime_types::extension_to_type(const std::string& extension)
{
    for (mapping m: mappings)
    {
        if (m.extension == extension)
        {
            return m.mime_type;
        }
    }
    return "text/plain";
}

