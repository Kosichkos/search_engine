#include "../include/EngineExceptions.h"

const char* ConfigFileMissingException::what() const noexcept
{
    return "Config file is missing.";
}

const char *ConfigFileEmptyException::what() const noexcept
{
    return "Config file is empty.";
}

const char *IncorrectVersionException::what() const noexcept
{
    return "\"config.json\" has incorrect file version.";
}

const char *RequestsFileMissingException::what() const noexcept
{
    return "Requests file is missing.";
}