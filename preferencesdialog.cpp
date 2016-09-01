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

#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"
#include "preferences.h"

PreferencesDialog::PreferencesDialog(Preferences& prefs, QWidget *parent) :
    QDialog(parent),
    mPreferences(prefs),
    ui(new Ui::PreferencesDialog)
{
    ui->setupUi(this);

    // load dialog's state
    ui->mRecentlyUsedCB->setChecked(mPreferences.generalRememberRecent());
    ui->mNumberRecentlySB->setValue(mPreferences.generalNumberOfRecent());
}

PreferencesDialog::~PreferencesDialog()
{
    delete ui;
}

void PreferencesDialog::on_mRecentlyUsedCB_stateChanged(int state)
{
    mPreferences.generalRememberRecent(state == Qt::Checked ? true : false);
}

void PreferencesDialog::on_buttonBox_accepted()
{
    mPreferences.commit();
}

void PreferencesDialog::on_mNumberRecentlySB_valueChanged(int num)
{
    mPreferences.generalNumberOfRecent(num);
}
