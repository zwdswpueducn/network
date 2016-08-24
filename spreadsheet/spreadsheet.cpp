#include <QtGui>

#include "cell.h"
#include "spreadsheet.h"

Spreadsheet::Spreadsheet(QWidget *parent)
    : QTableWidget(parent)
{
    autoRecalc = true;

    setItemPrototype(new Cell);
    setSelectionMode(ContiguousSelection);

    connect(this, SIGNAL(itemChanged(QTableWidgetItem *)),
            this, SLOT(somethingChanged()));

    clear();
}

QString Spreadsheet::currentLocation() const
{
    return QChar('A' + currentColumn())
           + QString::number(currentRow() + 1);
}

QString Spreadsheet::currentFormula() const
{
    return formula(currentRow(), currentColumn());
}

QTableWidgetSelectionRange Spreadsheet::selectedRange() const
{
    QList<QTableWidgetSelectionRange> ranges = selectedRanges();
    if (ranges.isEmpty())
        return QTableWidgetSelectionRange();
    return ranges.first();
}

void Spreadsheet::clear()    //清除表格中的内容，将表格初始化
{
    setRowCount(0);
    setColumnCount(0);
    setRowCount(RowCount);
    setColumnCount(ColumnCount);

    for (int i = 0; i < RowCount; ++i) {
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setText(QString(QChar('A' + i)));
        setHorizontalHeaderItem(i, item);
    }

    setCurrentCell(0, 0);


}

bool Spreadsheet::readsampleFile(const QString &fileName)  //读取data文件，并将数据存入public数组alldata中
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, tr("Spreadsheet"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(file.fileName())
                             .arg(file.errorString()));
        return false;
    }
    QTextStream in(&file);
    int k=0;
    int i =1;
    int j =0;
    double fldata;
    QString str;
    QApplication::setOverrideCursor(Qt::WaitCursor);
    while(!in.atEnd())
                   {
                       /*QString str=in.readLine();
                       QStringList words=str.simplified().split(" ");
                       QString str1;
                       for(i=0;i<4;i++){
                           str1=words.at(i);

                           fldata=str1.toDouble();
                           alldata[j][i]=fldata;
                           }
                       j++;*/
                       in>>str;
                       k=i%4;
                       fldata =str.toDouble();
                       if (k==1){
                                    alldata[j][0]=fldata;
                                    }
                                else if(k==2){
                                    alldata[j][1]=fldata;
                                    }
                                else if(k==3){
                                    alldata[j][2]=fldata;
                                    }
                                else if(k==0){
                                    alldata[j][3]=fldata;
                                    j=j+1;
                                    }
                        i=i+1;
                       }  
	zonghang = j-1;
	QApplication::restoreOverrideCursor();
    return true;
}

bool Spreadsheet::readstdsampleFile(const QString &fileName)  //读取data文件，并将数据存入public数组stdsample中
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, tr("Spreadsheet"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(file.fileName())
                             .arg(file.errorString()));
        return false;
    }
    QTextStream in(&file);
    int i =1;
    int j =0;
    double fldata;
    QString str;
    QApplication::setOverrideCursor(Qt::WaitCursor);
    while(!in.atEnd())
                   {
                       QString str=in.readLine();
                       QStringList words=str.simplified().split(" ");
                       QString str1;
                       for(i=0;i<4;i++){
                           str1=words.at(i);
						   fldata=str1.toDouble();
                           stdsample[j][i]=fldata;
                           }
					   j++;}
                       /*in>>str;
                       k=i%4;
                       fldata =str.toDouble();
                       if (k==1){
                                    alldata[j][0]=fldata;
                                    }
                                else if(k==2){
                                    alldata[j][1]=fldata;
                                    }
                                else if(k==3){
                                    alldata[j][2]=fldata;
                                    }
                                else if(k==0){
                                    alldata[j][3]=fldata;
                                    j=j+1;
                                    }
                        i=i+1;
                       } */ 
	stdzonghang = j-1;
	QApplication::restoreOverrideCursor();
    return true;
}

bool Spreadsheet::writeFile(const QString &fileName)  //把电子表格中的数据写入文件
    {QFile file(fileName);
    if (!file.open(QIODevice::Text |QIODevice::WriteOnly)) {//写入的文件标记换行
        QMessageBox::warning(this, tr("Spreadsheet"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(file.fileName())
                             .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);

    QApplication::setOverrideCursor(Qt::WaitCursor);
    for (int row = 0; row < RowCount; ++row) {
		for (int column = 0; column < ColumnCount; ++column) {
            QString str =formula(row, column);
            if (!str.isEmpty() && column <3)
			{out <<str<<"    ";}
			else if(!str.isEmpty() && column == 3)
			{out<<str<<endl;}
        }
    }
    QApplication::restoreOverrideCursor();
    return true;
}

void Spreadsheet::sort(const SpreadsheetCompare &compare)
{
    QList<QStringList> rows;
    QTableWidgetSelectionRange range = selectedRange();
    int i;

    for (i = 0; i < range.rowCount(); ++i) {
        QStringList row;
        for (int j = 0; j < range.columnCount(); ++j)
            row.append(formula(range.topRow() + i,
                               range.leftColumn() + j));
        rows.append(row);
    }

    qStableSort(rows.begin(), rows.end(), compare);

    for (i = 0; i < range.rowCount(); ++i) {
        for (int j = 0; j < range.columnCount(); ++j)
            setFormula(range.topRow() + i, range.leftColumn() + j,
                       rows[i][j]);
    }

    clearSelection();
    somethingChanged();
}

void Spreadsheet::cut()
{
    copy();
    del();
}

void Spreadsheet::copy()
{
    QTableWidgetSelectionRange range = selectedRange();
    QString str;

    for (int i = 0; i < range.rowCount(); ++i) {
        if (i > 0)
            str += "\n";
        for (int j = 0; j < range.columnCount(); ++j) {
            if (j > 0)
                str += "\t";
            str += formula(range.topRow() + i, range.leftColumn() + j);
        }
    }
    QApplication::clipboard()->setText(str);
}

void Spreadsheet::paste()
{
    QTableWidgetSelectionRange range = selectedRange();
    QString str = QApplication::clipboard()->text();
    QStringList rows = str.split('\n');
    int numRows = rows.count();
    int numColumns = rows.first().count('\t') + 1;

    if (range.rowCount() * range.columnCount() != 1
            && (range.rowCount() != numRows
                || range.columnCount() != numColumns)) {
        QMessageBox::information(this, tr("Spreadsheet"),
                tr("The information cannot be pasted because the copy "
                   "and paste areas aren't the same size."));
        return;
    }

    for (int i = 0; i < numRows; ++i) {
        QStringList columns = rows[i].split('\t');
        for (int j = 0; j < numColumns; ++j) {
            int row = range.topRow() + i;
            int column = range.leftColumn() + j;
            if (row < RowCount && column < ColumnCount)
                setFormula(row, column, columns[j]);
        }
    }
    somethingChanged();
}

void Spreadsheet::del()
{
    QList<QTableWidgetItem *> items = selectedItems();
    if (!items.isEmpty()) {
        foreach (QTableWidgetItem *item, items)
            delete item;
        somethingChanged();
    }
}

void Spreadsheet::selectCurrentRow()
{
    selectRow(currentRow());
}

void Spreadsheet::selectCurrentColumn()
{
    selectColumn(currentColumn());
}

void Spreadsheet::recalculate()
{
    for (int row = 0; row < RowCount; ++row) {
        for (int column = 0; column < ColumnCount; ++column) {
            if (cell(row, column))
                cell(row, column)->setDirty();
        }
    }
    viewport()->update();
}

void Spreadsheet::setAutoRecalculate(bool recalc)
{
    autoRecalc = recalc;
    if (autoRecalc)
        recalculate();
}

void Spreadsheet::findNext(const QString &str, Qt::CaseSensitivity cs)
{
    int row = currentRow();
    int column = currentColumn() + 1;

    while (row < RowCount) {
        while (column < ColumnCount) {
            if (text(row, column).contains(str, cs)) {
                clearSelection();
                setCurrentCell(row, column);
                activateWindow();
                return;
            }
            ++column;
        }
        column = 0;
        ++row;
    }
    QApplication::beep();
}

void Spreadsheet::findPrevious(const QString &str,
                               Qt::CaseSensitivity cs)
{
    int row = currentRow();
    int column = currentColumn() - 1;

    while (row >= 0) {
        while (column >= 0) {
            if (text(row, column).contains(str, cs)) {
                clearSelection();
                setCurrentCell(row, column);
                activateWindow();
                return;
            }
            --column;
        }
        column = ColumnCount - 1;
        --row;
    }
    QApplication::beep();
}

void Spreadsheet::somethingChanged()
{
    if (autoRecalc)
        recalculate();
    emit modified();
}

Cell *Spreadsheet::cell(int row, int column) const
{
    return static_cast<Cell *>(item(row, column));
}

void Spreadsheet::setFormula(int row, int column,
                             const QString &formula)
{
    Cell *c = cell(row, column);
    if (!c) {
        c = new Cell;
        setItem(row, column, c);
    }
    c->setFormula(formula);
}

QString Spreadsheet::formula(int row, int column) const
{
    Cell *c = cell(row, column);
    if (c) {
        return c->formula();
    } else {
        return "";
    }
}

QString Spreadsheet::text(int row, int column) const
{
    Cell *c = cell(row, column);
    if (c) {
        return c->text();
    } else {
        return "";
    }
}

bool SpreadsheetCompare::operator()(const QStringList &row1,
                                    const QStringList &row2) const
{
    for (int i = 0; i < KeyCount; ++i) {
        int column = keys[i];
        if (column != -1) {
            if (row1[column] != row2[column]) {
                if (ascending[i]) {
                    return row1[column] < row2[column];
                } else {
                    return row1[column] > row2[column];
                }
            }
        }
    }
    return false;
}

void Spreadsheet::showdata1()   //显示最近一次导入的数据 
{
    if (issampledata){
	int i,j;
	bool ok;
    int startline = QInputDialog::getInt(this, tr("QInputDialog::getInt()"),
                                          tr("Which Line would U like to show as the first Line:"), 1,100,zonghang,1,
                                          &ok);//输入开始显示的行数
     if (ok){
		QProgressDialog progress("Showing data...", "Cancel", 0, 100, this);
        progress.setWindowModality(Qt::WindowModal);

     for (int i = 0; i < 100; i++) {
         progress.setValue(i);

         if (progress.wasCanceled())
             break;
        
    /*for (k=0;k<=3;k++){
        QTableWidgetItem *item[k] = new QTableWidgetItem;
        item[k]->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        this->setItem(i, k, item[k]);
        this->item(i,k)->setText(QString::number(alldata[i][k],'g',6)) ;
    }*/
		QTableWidgetItem *item0 = new QTableWidgetItem;
        item0->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        this->setItem(i, 0, item0);

        QTableWidgetItem *item1 = new QTableWidgetItem;
        item1->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        this->setItem(i, 1, item1);

        QTableWidgetItem *item2 = new QTableWidgetItem;
        item2->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        this->setItem(i, 2, item2);

        QTableWidgetItem *item3 = new QTableWidgetItem;
        item3->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        this->setItem(i, 3, item3);

      for (j=0;j<=3;j++){
		  
		  this->item(i,j)->setText(QString::number(alldata[i+startline][j],'g',6)) ;}
		 
		}
     progress.setValue(100);
     	
}
	 else {return;}
}
	else 
	{
		int i,j;
	bool ok;
    int startline = QInputDialog::getInt(this, tr("QInputDialog::getInt()"),
                                          tr("Which Line would U like to show as the first Line:"), 1,100,zonghang,1,
                                          &ok);//输入开始显示的行数
     if (ok){
		QProgressDialog progress("Showing data...", "Cancel", 0, 100, this);
        progress.setWindowModality(Qt::WindowModal);

     for (int i = 0; i < 100; i++) {
         progress.setValue(i);

         if (progress.wasCanceled())
             break;
        
    /*for (k=0;k<=3;k++){
        QTableWidgetItem *item[k] = new QTableWidgetItem;
        item[k]->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        this->setItem(i, k, item[k]);
        this->item(i,k)->setText(QString::number(alldata[i][k],'g',6)) ;
    }*/
		QTableWidgetItem *item4 = new QTableWidgetItem;
        item4->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        this->setItem(i, 5, item4);

        QTableWidgetItem *item5 = new QTableWidgetItem;
        item5->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        this->setItem(i, 6, item5);

        QTableWidgetItem *item6 = new QTableWidgetItem;
        item6->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        this->setItem(i, 7, item6);

        QTableWidgetItem *item7 = new QTableWidgetItem;
        item7->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        this->setItem(i, 8, item7);

      for (j=0;j<=3;j++){
		  
		  this->item(i,j+5)->setText(QString::number(stdsample[i+startline][j],'g',6)) ;}
		 
		}
     progress.setValue(100);
     	
}
	 else {return;}
}
}

		
