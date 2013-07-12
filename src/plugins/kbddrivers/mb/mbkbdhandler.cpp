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

#include "mbkbdhandler.h"

#include <QFile>
#include <QTextStream>
#include <QScreen>
#include <QSocketNotifier>
#include <QDebug>

#include <iostream>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

struct ExampleInput {
    unsigned int   dummy1;
    unsigned int   dummy2;
    unsigned short type;
    unsigned short code;
    unsigned int   value;
};

MbKbdHandler::MbKbdHandler(const QString &device)
{
    std::cout << "Loaded MB keyboard plugin!" << std::endl;
    setObjectName("Mb Keypad Handler");
    kbdFd = ::open(device.toLocal8Bit().constData(), O_RDONLY, 0);
    if (kbdFd >= 0) {
        qLog(Input) << "Opened" << device << "as keyboard input";
        m_notify = new QSocketNotifier(kbdFd, QSocketNotifier::Read, this);
        connect(m_notify, SIGNAL(activated(int)), this, SLOT(readKbdData()));
    } else {
        qWarning("Cannot open %s for keyboard input (%s)",
                 device.toLocal8Bit().constData(), strerror(errno));
        return;
    }
    shift = false;
}

MbKbdHandler::~MbKbdHandler()
{
    if (kbdFd >= 0)
        ::close(kbdFd);
}

void MbKbdHandler::readKbdData()
{
    ExampleInput event;

    int n = read(kbdFd, &event, sizeof(ExampleInput));
    if (n != 16) {
        qLog(Input) << "keypressed: n=" << n;
        return;
    }

    qLog(Input) << "keypressed: type=" << event.type
                << "code=" << event.code
                << "value=" << event.value
                << ((event.value != 0) ? "(Down)" : "(Up)");

    Qt::KeyboardModifiers modifiers = Qt::NoModifier;
    int unicode = 0xffff;
    int key_code = 0;

    /*
    switch (event.code) {
    case 0x110:
        key_code = Qt::Key_Context1;
        unicode  = 0xffff;
        break;
    case 0x111:
        key_code = Qt::Key_Back;
        unicode  = 0xffff;
        break;
    }
    */

    std::cout << "EVENT! " << std::endl;
    std::cout << "event type: " << event.type << std::endl;
    std::cout << "event code: " << event.code << std::endl;
    std::cout << "event value: " << event.value << std::endl;

    processKeyEvent(unicode, key_code, modifiers, event.value!=0, false);
}

