#ifndef CORE_H
#define CORE_H

#include <iostream>
#include <sstream>

#include <QtGlobal>
#include <QDebug>


#if (QT_VERSION < QT_VERSION_CHECK(5,5,0))
class qInfo
{
    std::stringstream stream;

    public:
        ~qInfo();

        inline qInfo &operator<<(bool t);

        template <typename T>
        qInfo &operator<<(T t) { stream << t << " "; return *this; }
};
#endif


#define LOG_DEBUG    qDebug()
#define LOG_INFO     qInfo()
#define LOG_WARNING  qWarning()
#define LOG_CRITICAL qCritical()


#endif // CORE_H
