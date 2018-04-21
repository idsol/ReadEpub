#ifndef ERROR_H
#define ERROR_H

#include <exception>
#include <QString>

class Error : public std::exception
{
public:
    explicit Error(const QString& message) throw()
        : _message(message) {
    }
    ~Error() throw() {
    }

    QString message() const throw() {
        return _message;
    }

private:
    QString _message;
};

#endif // ERROR_H
