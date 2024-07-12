#include <stdexcept>
#include "version.hpp"

// if given version is compatible
bool is_version_compatible(const std::string_view comparator, const size_t version_spec, const size_t revision_spec)
{
    if (comparator.empty())
    {
        if (version_spec != VERSION or revision_spec != REVISION)
            return false;
    } else if (comparator == ">=")
    {
        if (version_spec != VERSION or revision_spec < REVISION)
            return false;
    } else if (comparator == "<=")
    {
        if (version_spec != VERSION or revision_spec > REVISION)
            return false;
    } else if (comparator == "!>=")
    {
        if (version_spec < VERSION or (version_spec == VERSION and revision_spec < REVISION))
            return false;
    } else if (comparator == "!<=")
    {
        if (version_spec > VERSION or (version_spec == VERSION and revision_spec > REVISION))
            return false;
    } else
    {
        throw std::runtime_error("Invalid comparator");
    }
    return true;
}
