#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QAction;
class QLabel;
class FindDialog;
class Spreadsheet;
class QCustomPlot;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();	

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void newFile();
    void opensample();
    bool save();
    bool saveAs();
	void showdata();
    void find();
    void goToCell();
	void grapher();
	void showsamplecurve();
	void showstdsamplecurve();
    void sort();
    void about();
    void openRecentFile();
    void updateStatusBar();
    void spreadsheetModified();

private:
    void createActions();
    void createMenus();
    void createContextMenu();
    void createToolBars();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    bool okToContinue();
    bool loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    void updateRecentFileActions();
    QString strippedName(const QString &fullFileName);
	
    Spreadsheet *spreadsheet;
	QCustomPlot *widget1;
	FindDialog *findDialog;
    QLabel *locationLabel;
    QLabel *formulaLabel;
    QStringList recentFiles;
    QString curFile;
	

    enum { MaxRecentFiles = 5 };
    QAction *recentFileActions[MaxRecentFiles];
    QAction *separatorAction;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *selectSubMenu;
    QMenu *toolsMenu;
	QMenu *graphMenu;
    QMenu *optionsMenu;
    QMenu *helpMenu;
    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QAction *newAction;
    QAction *opensampleAction;
    QAction *saveAction;
    QAction *saveAsAction;
	QAction *showdataAction;
    QAction *exitAction;
    QAction *cutAction;
    QAction *copyAction;
    QAction *pasteAction;
    QAction *deleteAction;
    QAction *selectRowAction;
    QAction *selectColumnAction;
    QAction *selectAllAction;
    QAction *findAction;
    QAction *goToCellAction;
	QAction *graphAction;
	QAction *samplecurveAction;
	QAction *stdsamplecurveAction;
    QAction *recalculateAction;
    QAction *sortAction;
    QAction *showGridAction;
    QAction *autoRecalcAction;
    QAction *aboutAction;
    QAction *aboutQtAction;
};

#endif
