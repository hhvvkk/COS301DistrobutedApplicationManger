#ifndef BUILD_H
#define BUILD_H

#include <QString>

class Build
{
private:
    QString buildDescription;
    QString buildName;
public:
    Build();
    ~Build();
    Build(QString bn, QString bd = 0);
};

#endif // BUILD_H
