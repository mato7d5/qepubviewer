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

#include "preferences.h"

Preferences::Preferences()
{
    loadGeneralSettings();
    loadRecentDocuments();
}

//methods
void Preferences::commit(bool full) {
    mSettings.setValue(GENERAL_REMEMBER_RECENT, mGeneral.rememberRecent);
    mSettings.setValue(GENERAL_NUMBER_OF_RECENT, mGeneral.numberOfRecent);

    if (full)
        writeRecentFiles();
}

void Preferences::loadGeneralSettings() {
    mGeneral.numberOfRecent = mSettings.value(GENERAL_NUMBER_OF_RECENT, 5).toInt();
    mGeneral.rememberRecent = mSettings.value(GENERAL_REMEMBER_RECENT, false).toBool();
}

void Preferences::loadRecentDocuments() {
    int size = mSettings.beginReadArray(RECENT_DOCUMENTS);

    if (!mRecentFiles.isEmpty())
        mRecentFiles.clear();

    if (size > mGeneral.numberOfRecent)
        size = mGeneral.numberOfRecent;

    for (int i = 0; i < size; ++i) {
        mSettings.setArrayIndex(i);
        mRecentFiles.push_back(mSettings.value(RECENT_DOCUMENTS_FILE).toString());
    }

    mSettings.endArray();
}

void Preferences::writeRecentFiles() {
    mSettings.remove(RECENT_DOCUMENTS);
    mSettings.beginWriteArray(RECENT_DOCUMENTS);
    for (int i = 0; i < mRecentFiles.size(); ++i) {
        mSettings.setArrayIndex(i);
        mSettings.setValue(RECENT_DOCUMENTS_FILE, mRecentFiles.at(i));
    }

    mSettings.endArray();
}

void Preferences::addRecentFile(const QString& file) {
    while (mGeneral.numberOfRecent <= mRecentFiles.size())
        mRecentFiles.removeFirst();

    mRecentFiles.push_back(file);
}
