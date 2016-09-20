/***************************************************************************
 *   Copyright (C) 2015 - 2016 by Martin Mancuska                          *
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

#include <QString>
#include <QStringList>
#include <QFileDialog>
#include <QMessageBox>
#include <QWebFrame>
#include <QIcon>
#include <QAction>
#include <QLabel>
#include <QIntValidator>
#include <QRegExpValidator>
#include <QDesktopServices>
#include <QStandardPaths>
#include <QDockWidget>
#include <QTreeWidgetItem>
#include <QList>
#include <QMenu>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "preferencesdialog.h"
#include <epubdocument.h>
#include <epubpage.h>
#include <epubexception.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    mCurrentPage(0),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QCoreApplication::setOrganizationName("QEpubViewer");
    QCoreApplication::setApplicationName("QEpubViewer");

    //navigation buttons
    mPrevPageBtn = new QPushButton(this);
    mNextPageBtn = new QPushButton(this);
    mFirstPageBtn = new QPushButton(this);
    mLastPageBtn = new QPushButton(this);
    mPagesEdit = new QLineEdit(this);

    mPrevPageBtn->setIcon(QIcon(":/icons/arrow-left.png"));
    mPrevPageBtn->setIconSize(QSize(22, 22));
    mNextPageBtn->setIcon(QIcon(":/icons/arrow-right.png"));
    mNextPageBtn->setIconSize(QSize(22, 22));
    mFirstPageBtn->setIcon(QIcon(":/icons/arrow-left-double.png"));
    mFirstPageBtn->setIconSize(QSize(22, 22));
    mLastPageBtn->setIcon(QIcon(":/icons/arrow-right-double.png"));
    mLastPageBtn->setIconSize(QSize(22, 22));

    connect(mFirstPageBtn, SIGNAL(clicked()), this, SLOT(on_action_First_Page_triggered()));
    connect(mPrevPageBtn, SIGNAL(clicked()), this, SLOT(on_action_Previous_Page_triggered()));
    connect(mNextPageBtn, SIGNAL(clicked()), this, SLOT(on_action_Next_Page_triggered()));
    connect(mLastPageBtn, SIGNAL(clicked()), this, SLOT(on_action_Last_Page_triggered()));

    mPagesEdit->setAlignment(Qt::AlignHCenter);
    connect(mPagesEdit, SIGNAL(editingFinished()), this, SLOT(pageNumber_entered()));

    auto bar = statusBar();
    bar->addPermanentWidget(new QLabel(this), 1);
    bar->addPermanentWidget(mFirstPageBtn);
    bar->addPermanentWidget(mPrevPageBtn);
    bar->addPermanentWidget(mPagesEdit);
    bar->addPermanentWidget(mNextPageBtn);
    bar->addPermanentWidget(mLastPageBtn);
    bar->addPermanentWidget(new QLabel(this), 1);

    mNetworkManager = std::make_unique<EpubNetworkManager> (this);
    mWebPage.setNetworkAccessManager(mNetworkManager.get());

    mWebPage.setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    connect(&mWebPage, SIGNAL(linkClicked(const QUrl&)), this, SLOT(urlClicked(const QUrl&)));

    ui->mainToolBar->setMovable(false);
    ui->mainToolBar->setFloatable(false);

    mLeftToolBar = new QToolBar(this);
    mLeftToolBar->setFloatable(false);
    mLeftToolBar->setMovable(false);

    mContentLeftBtn = new QPushButton(mLeftToolBar);
    mContentLeftBtn->setIcon(QIcon(":/icons/content-icon.png"));
    mContentLeftBtn->setIconSize(QSize(22, 22));
    mContentLeftBtn->setEnabled(false);
    connect(mContentLeftBtn, SIGNAL(clicked()), this, SLOT(on_action_Content_triggered()));

    mLeftToolBar->addWidget(mContentLeftBtn);
    addToolBar(Qt::LeftToolBarArea, mLeftToolBar);

    mContentTree = nullptr;
    mContentDock = nullptr;
    mContentDockShow = false;

    enableStatusBarControls();

    //recent files
    QMenu* recentFilesMenu = new QMenu(ui->menu_File);
    recentFilesMenu->setTitle(tr("&Recent Files"));
    ui->menu_File->insertMenu(ui->action_Close, recentFilesMenu);

    if (mPreferences.generalRememberRecent()) {
        for (const QString& file : mPreferences.recentFiles()) {
            QAction* action = recentFilesMenu->addAction(file);
            connect(recentFilesMenu, SIGNAL(triggered(QAction*)), this, SLOT(recentFilesActionSlot(QAction*)));
        }
    }
    else
        recentFilesMenu->setEnabled(false);

    showMaximized();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openFile(const QString& fileName) {
    if (!fileName.isEmpty()) {
        mFileName = fileName;
        try {
            if (mEpubDocument.get())
                mEpubDocument.reset(new EpubDocument(mFileName));
            else
                mEpubDocument = std::make_unique<EpubDocument> (mFileName);

            int pagesNum = mEpubDocument->numOfPages();
            if (pagesNum > 0) {
                mNetworkManager->setEpub(mEpubDocument->epubdata());

                jumpToPage(1);

                setWindowTitle(QString("QEpubViewer - %1").arg(mFileName));
                mContentLeftBtn->setEnabled(true);
                ui->action_Content->setEnabled(true);
                ui->action_Close->setEnabled(true);
            }
        }
        catch (EpubException& ex) {
            setWindowTitle("QEpubViewer");
            mPagesEdit->setText("/");
            mPagesEdit->setEnabled(false);
            mContentLeftBtn->setEnabled(false);
            ui->action_Content->setEnabled(false);
            ui->action_Close->setEnabled(false);

            QMessageBox::critical(this, tr("Error"), ex.what(), QMessageBox::Ok);
            return;
        }

        if (mPreferences.generalRememberRecent())
            mPreferences.addRecentFile(mFileName);
    }
}

void MainWindow::enableStatusBarControls() {
    if (mEpubDocument.get() == nullptr) {
        mFirstPageBtn->setEnabled(false);
        mPrevPageBtn->setEnabled(false);
        mPagesEdit->setEnabled(false);
        mNextPageBtn->setEnabled(false);
        mLastPageBtn->setEnabled(false);
        ui->action_First_Page->setEnabled(false);
        ui->action_Previous_Page->setEnabled(false);
        ui->action_Last_Page->setEnabled(false);
        ui->action_Next_Page->setEnabled(false);
    }
    else {
        mPagesEdit->setEnabled(true);
        int numOfPages = mEpubDocument->numOfPages();

        if (mCurrentPage == 1) {
            mFirstPageBtn->setEnabled(false);
            mPrevPageBtn->setEnabled(false);
            mNextPageBtn->setEnabled(true);
            mLastPageBtn->setEnabled(true);
            ui->action_First_Page->setEnabled(false);
            ui->action_Previous_Page->setEnabled(false);
            ui->action_Last_Page->setEnabled(true);
            ui->action_Next_Page->setEnabled(true);
        }
        else if (mCurrentPage < numOfPages) {
            mFirstPageBtn->setEnabled(true);
            mPrevPageBtn->setEnabled(true);
            mNextPageBtn->setEnabled(true);
            mLastPageBtn->setEnabled(true);
            ui->action_First_Page->setEnabled(true);
            ui->action_Previous_Page->setEnabled(true);
            ui->action_Last_Page->setEnabled(true);
            ui->action_Next_Page->setEnabled(true);
        }
        else if (mCurrentPage == numOfPages) {
            mFirstPageBtn->setEnabled(true);
            mPrevPageBtn->setEnabled(true);
            mNextPageBtn->setEnabled(false);
            mLastPageBtn->setEnabled(false);
            ui->action_First_Page->setEnabled(true);
            ui->action_Previous_Page->setEnabled(true);
            ui->action_Last_Page->setEnabled(false);
            ui->action_Next_Page->setEnabled(false);
        }
        else {
            mFirstPageBtn->setEnabled(false);
            mPrevPageBtn->setEnabled(false);
            mNextPageBtn->setEnabled(false);
            mLastPageBtn->setEnabled(false);
            mPagesEdit->setEnabled(false);
            ui->action_First_Page->setEnabled(false);
            ui->action_Previous_Page->setEnabled(false);
            ui->action_Last_Page->setEnabled(false);
            ui->action_Next_Page->setEnabled(false);
        }
    }
}

void MainWindow::jumpToPage(int page) {
    if (page <= 0)
        return;

    if (page == mCurrentPage)
        return;

    int numOfPages = mEpubDocument->numOfPages();
    if ((page - 1) >= numOfPages)
        return;

    const EpubPage&  epubPage = mEpubDocument->page(page - 1);
    mCurrentPage = page;

    mWebPage.mainFrame()->setHtml(epubPage.htmlContent());
    ui->mWebView->setPage(&mWebPage);
    ui->mWebView->show();

    enableStatusBarControls();
    mPagesEdit->setText(QString("%1 / %2").arg(mCurrentPage).arg(numOfPages));
}

void MainWindow::on_action_Open_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open epub file"), \
                                                    QStandardPaths::writableLocation(QStandardPaths::HomeLocation), \
                                                    tr("Epub files (*.epub)"));

    openFile(fileName);
}

void MainWindow::on_actionQuit_triggered()
{
    qApp->exit();
}

void MainWindow::on_action_First_Page_triggered()
{
    jumpToPage(1);
}

void MainWindow::on_action_Last_Page_triggered()
{
    jumpToPage(mEpubDocument->numOfPages());
}

void MainWindow::on_action_Next_Page_triggered()
{
    jumpToPage(mCurrentPage + 1);
}

void MainWindow::on_action_Previous_Page_triggered()
{
    jumpToPage(mCurrentPage - 1);
}

void MainWindow::pageNumber_entered() {
    int pageNum = 0;
    int pagesCount = mEpubDocument->numOfPages();
    int pos = 0;
    QString str = mPagesEdit->text();
    QIntValidator intValidator(1, pagesCount);

    if (intValidator.validate(str, pos) == QValidator::Acceptable) {
        pageNum = str.toInt();
    }
    else {
        QRegExp re("\\s*\\d+\\s*/\\s*\\d+\\s*");
        QRegExpValidator reValidator(re);

        if (reValidator.validate(str, pos) == QValidator::Acceptable) {
            QStringList parts = str.split("/");
            pageNum = parts[0].toInt();
        }
    }

    if (pageNum > 0 && pageNum <= pagesCount) {
        jumpToPage(pageNum);
    }
}

void MainWindow::urlClicked(const QUrl& url)
{
    auto scheme = url.scheme();
    if (!scheme.isEmpty()) {
        if (scheme.contains("file", Qt::CaseInsensitive)) {
            auto pageUrls = mEpubDocument->pageUrlMap();
            auto host = url.host();
            int pageNum = -1;

            for (const auto& pageUrl : pageUrls) {
                if (host.compare(pageUrl.first, Qt::CaseInsensitive) == 0) {
                    pageNum = pageUrl.second;
                    break;
                }
            }

            if (pageNum > 0)
                jumpToPage(pageNum);

            if (url.hasFragment())
                mWebPage.mainFrame()->scrollToAnchor(url.fragment());
        }
        else {
            QDesktopServices::openUrl(url);
        }
    }
}

void MainWindow::on_action_About_triggered()
{
    AboutDialog dlg(this);
    dlg.exec();
}

void MainWindow::on_action_Content_triggered()
{
    contentControl(!mContentDockShow);
}

void MainWindow::contentControl(bool show) {
    if (mContentDock == nullptr) {
        mContentDock = new QDockWidget(QString::fromUtf8("Content"), this);
        QList<QTreeWidgetItem*> contentItems;
        mContentTree = new QTreeWidget(mContentDock);

        mContentDock->setAllowedAreas(Qt::LeftDockWidgetArea);
        mContentTree->setColumnCount(1);
        mContentTree->setSortingEnabled(false);

        QTreeWidgetItem* contentItem = nullptr;
        QString parentUrl;

        for (const auto& item : mEpubDocument->contentMap()) {
            QString title = item.first;
            QString url = item.second;
            bool isChild = url.contains("#");

            if (!isChild && parentUrl.length() > 0) {
                int dotIdx = parentUrl.indexOf('.');
                if (dotIdx > 0) {
                    QString parentChapter = parentUrl.mid(0, dotIdx);
                    if (url.startsWith(parentChapter))
                        isChild = true;
                 }
            }

            if (!isChild) {
                contentItem = new QTreeWidgetItem(mContentTree, QStringList(title));
                contentItems.append(contentItem);
                parentUrl = std::move(url);
            }
            else {
                if (!contentItems.isEmpty()) {
                    QTreeWidgetItem* subContentItem = new QTreeWidgetItem(contentItem, QStringList(title));
                    contentItems.append(subContentItem);
                }
            }
        }

        mContentTree->insertTopLevelItems(0, contentItems);
        mContentTree->setHeaderLabel("Chapters");
        connect(mContentTree, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(contentItemClick(QTreeWidgetItem*,int)));
        mContentDock->setWidget(mContentTree);
        mContentDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
        addDockWidget(Qt::LeftDockWidgetArea, mContentDock);
    }

    if (show) {
        mContentDock->show();
        ui->action_Content->setChecked(true);
        mContentDockShow = true;
    }
    else {
        mContentDock->hide();
        ui->action_Content->setChecked(false);
        mContentDockShow = false;
    }
}

void MainWindow::contentItemClick(QTreeWidgetItem* item, int column) {
    QString title = item->text(0);
    QUrl url;

    auto contentMap = mEpubDocument->contentMap();
    for (const auto& i : contentMap) {
        if (i.first.compare(title, Qt::CaseInsensitive) == 0) {
            url = "file://" + i.second;
            break;
        }
    }

    emit mWebPage.linkClicked(url);
}

void MainWindow::on_action_Close_triggered()
{
    ui->mWebView->close();
    if (mContentDock) {
        mContentDock->hide();
        delete mContentDock;
        mContentDock = nullptr;
    }

    mEpubDocument.reset();
    mEpubDocument = nullptr;
    setWindowTitle("QEpubViewer");
    enableStatusBarControls();
    ui->action_Close->setEnabled(false);
    ui->action_Content->setChecked(false);
    ui->action_Content->setEnabled(false);
    mContentLeftBtn->setEnabled(false);
    mCurrentPage = 0;
    mContentDockShow = false;
}

void MainWindow::on_action_Preferences_triggered()
{
    PreferencesDialog dlg(mPreferences);
    dlg.exec();
}

void MainWindow::recentFilesActionSlot(QAction* action) {
    if (action) {
        const QString& fileName = action->text();
        openFile(fileName);
    }
}
