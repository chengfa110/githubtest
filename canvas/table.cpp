#include "table.h"
#include <canvaspropertybroswer.h>

Table::Table ()
{
  m_exportTableButton = new QPushButton ("Export Table");
  m_exportTableButton->setMaximumWidth (120);
  m_showTerminalButton = new QPushButton("Show Terminal");
  m_showTerminalButton->setMaximumWidth(120);
  m_table = new QTableWidget;
  m_hLayout = new QHBoxLayout;
  m_vLayout = new QVBoxLayout;
  m_hLayout->addWidget(m_exportTableButton);
  m_hLayout->addWidget(m_showTerminalButton);
  m_vLayout->addLayout(m_hLayout);
  m_vLayout->addWidget (m_table);
  connect (m_exportTableButton, SIGNAL(clicked()), this, SLOT(exportButtonClickedSlot()));
  connect(m_showTerminalButton,SIGNAL(clicked()),this,SLOT(showTermianlSlot()));
  setLayout (m_vLayout);
}

void
Table::setHeaderList (QStringList headerList)
{
  m_table->setColumnCount (headerList.size ());
  m_table->setHorizontalHeaderLabels (headerList);
  m_headerList = headerList;

}

void
Table::addCell (uint32_t cellIndex, QString value)
{
  uint32_t rows = m_table->rowCount ();
  uint32_t column = cellIndex;
  QTableWidgetItem * item = new QTableWidgetItem (value);
  m_table->setItem (rows-1, column, item);
}

void
Table::incrRowCount ()
{
  uint32_t rows = m_table->rowCount ();
  m_table->setRowCount (rows + 1);
}

void
Table::addRow (QStringList rowContents, bool autoAdjust)
{
  uint32_t rows = m_table->rowCount ();
  m_table->setRowCount (rows + 1);
  uint32_t column = 0;
  foreach (QString st, rowContents)
    {
      QTableWidgetItem * item = new QTableWidgetItem (st);
      m_table->setItem (rows, column++, item);
    }
  if (autoAdjust)
    {
      m_table->resizeRowsToContents();
      m_table->resizeColumnsToContents ();
    }
}


void
Table::removeAllRows ()
{
  for (int i = 0; i < m_table->rowCount (); ++i)
    {
      m_table->removeRow (i);
    }
  m_table->setRowCount (0);
}

void
Table::clear ()
{
  removeAllRows ();
  //clearContents ();
}

void
Table::adjust ()
{
  m_table->resizeRowsToContents();
  m_table->resizeColumnsToContents ();
}

QString
Table::stringListToRowString (QStringList strList)
{
  QString str = "";
  for (int i = 0; i < strList.count (); ++i)
    {
      str += strList.at (i);
      if (i < (strList.count () - 1))
        {
          str += "\t";
        }
    }
  str += "\n";
  return str;
}

void
Table::exportButtonClickedSlot ()
{
  QFileDialog fd;
  QString exportFileName = QFileDialog::getSaveFileName ();
  QFile f (exportFileName);
  if (f.open (QIODevice::WriteOnly))
    {
      QString headerString = stringListToRowString (m_headerList);
      f.write (GET_DATA (headerString));
      int rowCount = m_table->rowCount ();
      int columnCount = m_table->columnCount ();
      for (int i = 0; i < rowCount; ++i)
        {
          QStringList rowStringList;
          for (int j = 0; j < columnCount; ++j)
            {
              QTableWidgetItem * item = m_table->item (i, j);
              if (!item)
                {
                  rowStringList << "N/A";
                }
              else
                {
                  rowStringList << item->text ();
                }
            }
          f.write (GET_DATA (stringListToRowString (rowStringList)));
        }
    }
  f.close();
}

void
Table::showTermianlSlot()
{
    qDebug()<<"gnome";
    QString terminal = QString("gnome-terminal -x bash -c \"ssh ubuntu@node%1;exec bash;\"").arg(canvasPropertyBroswer::getInstance()->getTerminalString());
    QProcess::execute(terminal);
}
