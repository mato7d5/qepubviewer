/***************************************************************************
 *   Copyright (C) 2015 by Martin Mancuska                                 *
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWebPage>
#include <QPushButton>
#include <QLineEdit>
#include <QString>
#include <QToolBar>
#include <QTreeWidget>
#include <QDockWidget>
#include <epubdocument.h>
#include <memory>
#include "epubnetworkmanager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QString mFileName;
    std::unique_ptr<EpubDocument> mEpubDocument;
    QWebPage mWebPage;
    int mCurrentPage;
    QPushButton* mPrevPageBtn;
    QPushButton* mNextPageBtn;
    QPushButton* mFirstPageBtn;
    QPushButton* mLastPageBtn;
    QPushButton* mContentLeftBtn;
    QLineEdit* mPagesEdit;
    std::unique_ptr<EpubNetworkManager> mNetworkManager;
    QToolBar* mLeftToolBar;
    QDockWidget* mContentDock;
    bool mContentDockShow;

    //content
    QTreeWidget* mContentTree;
    void contentControl(bool show);

    void enableStatusBarControls();
    void jumpToPage(int page);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_action_Open_triggered();
    void on_actionQuit_triggered();
    void on_action_First_Page_triggered();
    void on_action_Last_Page_triggered();
    void on_action_Next_Page_triggered();
    void on_action_Previous_Page_triggered();
    void pageNumber_entered();
    void urlClicked(const QUrl& url);
    void on_action_About_triggered();
    void on_action_Content_triggered();
    void contentItemClick(QTreeWidgetItem* item, int column);
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
