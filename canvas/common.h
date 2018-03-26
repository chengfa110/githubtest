#ifndef COMMON_H
#define COMMON_H
#include <stdint.h>
#include <math.h>

#include <QWidget>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QToolButton>
#include <QSpinBox>
#include <QSlider>
#include <QLCDNumber>
#else
#include <QtGui/QToolButton>
#include <QtGui/QSpinBox>
#include <QtGui/QSlider>
#include <QtGui/QLCDNumber>
#endif
#include <QVBoxLayout>
#include <QSplitter>
#include <QToolBar>
#include <QStatusBar>
#include <QTime>
#include <QThread>
#include <QLabel>
#include <QComboBox>
#include <QProgressBar>
#include <QGraphicsPixmapItem>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QtCore/QXmlStreamReader>
#include <QFile>
#include <QTimer>
#include <QDialog>
#include <QApplication>
#include <QMessageBox>
#include <QFileDialog>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsProxyWidget>
#include <QRegExp>
#include <QFontMetrics>
#include <QDesktopWidget>
#include <QLineEdit>
#include <QScrollBar>
#include <QTableWidget>
#include <QCheckBox>
#include <QRegExp>

// Utilities to support porting to Qt5
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#  define GET_ASCII(x)  x.toLatin1 ()
#  define GET_DATA(x)  x.toLatin1 ().data ()
#  define GET_DATA_PTR(x)  x->toLatin1 ().data ()

#else
#  define GET_ASCII(x)  x.toAscii ()
#  define GET_DATA(x)  x.toAscii ().data ()
#  define GET_DATA_PTR(x)  x->toAscii ().data ()
#endif

#endif // COMMON_H
