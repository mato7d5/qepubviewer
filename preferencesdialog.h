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

#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>

class Preferences;

namespace Ui {
class PreferencesDialog;
}

class PreferencesDialog : public QDialog
{
    Q_OBJECT

private:
    Preferences& mPreferences;

public:
    explicit PreferencesDialog(Preferences& prefs, QWidget *parent = 0);
    ~PreferencesDialog();

private slots:
    void on_mRecentlyUsedCB_stateChanged(int state);
    void on_buttonBox_accepted();
    void on_mNumberRecentlySB_valueChanged(int num);

private:
    Ui::PreferencesDialog *ui;
};

#endif // PREFERENCESDIALOG_H
