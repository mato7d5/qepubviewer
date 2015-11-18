#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWebPage>
#include <QPushButton>
#include <QLineEdit>
#include <QString>
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
    QLineEdit* mPagesEdit;
    std::unique_ptr<EpubNetworkManager> mNetworkManager;

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

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
