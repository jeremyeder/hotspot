/*
  util.cpp

  This file is part of Hotspot, the Qt GUI for performance analysis.

  Copyright (C) 2016-2017 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
  Author: Milian Wolff <milian.wolff@kdab.com>

  Licensees holding valid commercial KDAB Hotspot licenses may use this file in
  accordance with Hotspot Commercial License Agreement provided with the Software.

  Contact info@kdab.com if any conditions of this licensing are not clear to you.

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "util.h"

#include "hotspot-config.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QDebug>

QString Util::findLibexecBinary(const QString& name)
{
    QDir dir(qApp->applicationDirPath());
    qDebug() << qApp->applicationDirPath() << name << HOTSPOT_LIBEXEC_REL_PATH;
    if (!dir.cd(QStringLiteral(HOTSPOT_LIBEXEC_REL_PATH))) {
        qDebug() << "cd failed";
        return {};
    }
    QFileInfo info(dir.filePath(name));
    if (!info.exists() || !info.isFile() || !info.isExecutable()) {
        return {};
    }
    return info.absoluteFilePath();
}

QString Util::formatString(const QString& input)
{
    return input.isEmpty() ? QObject::tr("??") : input;
}

QString Util::formatCost(quint32 cost)
{
    return QString::number(cost);
}

QString Util::formatCostRelative(quint64 selfCost, quint64 totalCost)
{
    return QString::number(double(selfCost) * 100. / totalCost, 'g', 3);
}

QString Util::formatTime(quint64 nanoseconds)
{
    quint64 totalSeconds = nanoseconds / 1000000000;
    quint64 days = totalSeconds / 60 / 60 / 24;
    quint64 hours = (totalSeconds / 60 / 60) % 24;
    quint64 minutes = (totalSeconds / 60) % 60;
    quint64 seconds = totalSeconds % 60;
    quint64 milliseconds = (nanoseconds / 1000000) % 1000;

    auto format = [] (quint64 fragment, int precision) -> QString {
        return QString::number(fragment).rightJustified(precision, QLatin1Char('0'));
    };
    auto optional = [format] (quint64 fragment) -> QString {
        return fragment > 0 ? format(fragment, 2) + QLatin1Char(':') : QString();
    };
    return optional(days) + optional(hours) + optional(minutes)
            + format(seconds, 2) + QLatin1Char('.') + format(milliseconds, 3) + QLatin1Char('s');
}
