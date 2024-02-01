#ifndef SEARCH_ENGINE_ENGINEEXCEPTIONS_H
#define SEARCH_ENGINE_ENGINEEXCEPTIONS_H
#include <exception>

class ConfigFileMissingException : public std::exception
{
public:
    ConfigFileMissingException() = default;

    [[nodiscard]] const char* what() const noexcept override;
};

class ConfigFileEmptyException : public std::exception
{
public:
    ConfigFileEmptyException() = default;

    [[nodiscard]] const char* what() const noexcept override;
};

class IncorrectVersionException : public std::exception
{
public:
    IncorrectVersionException() = default;

    [[nodiscard]] const char* what() const noexcept override;
};

class RequestsFileMissingException : public std::exception
{
public:
    RequestsFileMissingException() = default;

    [[nodiscard]] const char* what() const noexcept override;
};

#endif //SEARCH_ENGINE_ENGINEEXCEPTIONS_H