#ifndef TABLE_H
#define TABLE_H

#include "common.h"

class Table: public QWidget
{
Q_OBJECT
public:
  Table ();
  void setHeaderList (QStringList headerList);
  void addRow (QStringList rowContents, bool autoAdjust = false);
  void addCell (uint32_t cellIndex, QString value);
  void incrRowCount ();
  void removeAllRows ();
  void adjust ();
  void clear ();
  QString stringListToRowString (QStringList strList);
private:
  QTableWidget * m_table;
  QVBoxLayout * m_vLayout;
  QPushButton * m_exportTableButton;
  QStringList m_headerList;
  QHBoxLayout *m_hLayout;
  QPushButton *m_showTerminalButton;

protected slots:
  void exportButtonClickedSlot ();
  void showTermianlSlot();

};


#endif // TABLE_H
