#ifndef canvasPROPERTYBROWSER_H
#define canvasPROPERTYBROWSER_H
#include "common.h"
#include <QtTreePropertyBrowser>
#include <QtIntPropertyManager>
#include <QtSpinBoxFactory>
#include <QtLineEditFactory>
#include <QtDoubleSpinBoxFactory>
#include <QTableWidget>
#include "filepathmanager.h"
#include "fileeditfactory.h"
#include "table.h"
#include <mainwindow.h>


class canvasPropertyBroswer: public QWidget
{
Q_OBJECT
public:
  static canvasPropertyBroswer * getInstance ();
  void postParse ();
  void systemReset ();
  void show (bool show);
  void setCurrentNodeId (uint32_t currentNodeId);
  void showNodePositionTable (bool show);
  void refresh ();
  int  getTerminalString();
private:
  typedef QVector <QtProperty *> QtPropertyVector_t;
  canvasPropertyBroswer ();
  void reset ();
  void setupNodeProperties ();
//  void setupBackgroundProperties ();
  void setupManagers ();
  void setupFactories ();
//  void refreshBackgroundProperties ();
  QVBoxLayout * m_vboxLayout;
  QtAbstractPropertyBrowser * m_nodeBrowser;
  QtAbstractPropertyBrowser * m_backgroundBrowser;
  Table * m_nodePosTable;
  QComboBox * m_mode;
  QComboBox * m_nodeIdSelector;

  QtProperty * m_nodeIdProperty;
  QtProperty * m_nodeSysIdProperty;
  QtProperty * m_nodeDescriptionProperty;
  QtProperty * m_nodeXProperty;
  QtProperty * m_nodeYProperty;
  QtProperty * m_nodeColorProperty;
  QtProperty * m_nodeSizeProperty;
  QtProperty * m_fileEditProperty;
  QtProperty * m_ipv4AddressGroupProperty;
  QtProperty * m_macAddressGroupProperty;
  QtProperty * m_nodePositionGroupProperty;
  QtPropertyVector_t m_ipv4AddressVectorProperty;
  QtPropertyVector_t m_macAddressVectorProperty;
  QtProperty * m_showNodeTrajectoryProperty;

  QtPropertyVector_t m_nodeCounterDoubleProperty;
  QtPropertyVector_t m_nodeCounterUint32Property;


  // Background
  QtProperty * m_backgroundFileEditProperty;
  QtProperty * m_backgroundX;
  QtProperty * m_backgroundY;
  QtProperty * m_backgroundScaleX;
  QtProperty * m_backgroundScaleY;
  QtProperty * m_backgroundOpacity;

  uint32_t m_currentNodeId;

  QtIntPropertyManager * m_intManager;
  QtStringPropertyManager * m_stringManager;
  QtDoublePropertyManager * m_doubleManager;
  QtDoublePropertyManager * m_backgroundDoubleManager;

  QtColorPropertyManager * m_colorManager;
  FilePathManager * m_filePathManager;
  QtGroupPropertyManager * m_nodePositionManager;
  QtGroupPropertyManager * m_ipv4AddressManager;
  QtGroupPropertyManager * m_macAddressManager;
  QtStringPropertyManager * m_staticStringManager;
  QtBoolPropertyManager * m_boolManager;

  QtDoubleSpinBoxFactory * m_doubleSpinBoxFactory;
  QtSpinBoxFactory * m_spinBoxFactory;
  FileEditFactory * m_fileEditFactory;
  QtLineEditFactory * m_lineEditFactory;
  QtCheckBoxFactory * m_checkBoxFactory;



  typedef std::map <QtProperty *, QString> PropertyIdMap_t;
  PropertyIdMap_t m_propertyId;

  int terminalId;
private slots:
  void nodeIdSelectorSlot (QString newIndex);
  void valueChangedSlot (QtProperty*, QString);
  void valueChangedSlot (QtProperty*, double);
  void valueChangedSlot (QtProperty*, QColor);
  void valueChangedSlot (QtProperty* ,bool);
  void modeChangedSlot (QString mode);

};

#endif // canvasPROPERTYBROWSER_H
