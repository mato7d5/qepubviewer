/***************************************************************************
 *   Copyright (C) 2016 by Martin Mancuska                                 *
 *   <martin@borg.sk>                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QSettings>
#include <QStringList>

#define GENERAL_REMEMBER_RECENT     "General/RemberRecent"
#define GENERAL_NUMBER_OF_RECENT    "General/NumberOfRecent"
#define RECENT_DOCUMENTS            "RecentDocuments"
#define RECENT_DOCUMENTS_FILE       "File"

class Preferences
{
private:
    QSettings mSettings;

    struct {
        int numberOfRecent;
        bool rememberRecent;
    } mGeneral;

    QStringList mRecentFiles;

    void loadGeneralSettings();
    void loadRecentDocuments();

public:
    Preferences();

    // General settings
    bool generalRememberRecent() const { return mGeneral.rememberRecent; }
    void generalRememberRecent(bool remember) { mGeneral.rememberRecent = remember; }
    int generalNumberOfRecent() const { return mGeneral.numberOfRecent; }
    void generalNumberOfRecent(int num) { mGeneral.numberOfRecent = num; }

    //Recent files
    QStringList recentFiles() const { return mRecentFiles; }
    void addRecentFile(const QString& file);
};

#endif // PREFERENCES_H
