/****************************************************************************
**
** This file is part of the Qt Extended Opensource Package.
**
** Copyright (C) 2009 Trolltech ASA.
**
** Contact: Qt Extended Information (info@qtextended.org)
**
** This file may be used under the terms of the GNU General Public License
** version 2.0 as published by the Free Software Foundation and appearing
** in the file LICENSE.GPL included in the packaging of this file.
**
** Please review the following information to ensure GNU General Public
** Licensing requirements will be met:
**     http://www.fsf.org/licensing/licenses/info/GPLv2.html.
**
**
****************************************************************************/

#ifndef MBKBDHANDLER_H
#define MBKBDHANDLER_H

#include <QObject>
#include <QWSKeyboardHandler>

class QSocketNotifier;
class MbKbdHandler : public QObject, public QWSKeyboardHandler
{
    Q_OBJECT
public:
    MbKbdHandler(const QString &device = QString("/dev/input/event0"));
    ~MbKbdHandler();

private:
    QSocketNotifier *m_notify;
    int  kbdFd;
    bool shift;

private Q_SLOTS:
    void readKbdData();
};

#endif
