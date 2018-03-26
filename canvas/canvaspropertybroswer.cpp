#include "canvasnode.h"
#include "canvasscene.h"
#include "canvaspropertybroswer.h"
#include <QDebug>
#include <mainwindow.h>
canvasPropertyBroswer * pcanvasPropertyBrowser = 0;

canvasPropertyBroswer::canvasPropertyBroswer ():
  m_vboxLayout (0),
  m_nodeBrowser (0),
  m_backgroundBrowser (0),
  m_nodePosTable (0),
  m_mode (0),
  m_nodeIdSelector (0),
  m_nodeIdProperty (0),
  m_nodeSysIdProperty (0),
  m_nodeDescriptionProperty (0),
  m_nodeXProperty (0),
  m_nodeYProperty (0),
  m_nodeColorProperty (0),
  m_nodeSizeProperty (0),
  m_fileEditProperty (0),
  m_ipv4AddressGroupProperty (0),
  m_macAddressGroupProperty (0),
  m_nodePositionGroupProperty (0),
  m_showNodeTrajectoryProperty (0),
  m_backgroundFileEditProperty (0),
  m_backgroundX (0),
  m_backgroundY (0),
  m_backgroundScaleX (0),
  m_backgroundScaleY (0),
  m_backgroundOpacity (0),
  m_currentNodeId (0),
  m_intManager (0),
  m_stringManager (0),
  m_doubleManager (0),
  m_backgroundDoubleManager (0),
  m_colorManager (0),
  m_filePathManager (0),
  m_nodePositionManager (0),
  m_ipv4AddressManager (0),
  m_macAddressManager (0),
  m_staticStringManager (0),
  m_boolManager (0),
  m_doubleSpinBoxFactory (0),
  m_spinBoxFactory (0),
  m_fileEditFactory (0),
  m_lineEditFactory (0),
  m_checkBoxFactory (0),
  terminalId(0)
{
  m_vboxLayout = new QVBoxLayout;

  setLayout (m_vboxLayout);
  m_nodeBrowser = new QtTreePropertyBrowser;
  m_backgroundBrowser = new QtTreePropertyBrowser;
  m_nodePosTable = new Table;
  m_mode = new QComboBox;
  m_nodeIdSelector = new QComboBox;
  m_mode->addItem ("Node");
//  m_mode->addItem ("Background");
  m_vboxLayout->addWidget (m_mode);

  m_vboxLayout->addWidget (m_nodeIdSelector);
  m_vboxLayout->addWidget (m_nodeBrowser);
  m_vboxLayout->addWidget (m_nodePosTable);
  m_vboxLayout->addWidget (m_backgroundBrowser);
  m_backgroundBrowser->setVisible (false);
  m_nodePosTable->setVisible (true);
  //m_nodePosTable->setColumnCount (3);
  connect (m_mode, SIGNAL(currentIndexChanged(QString)), this, SLOT(modeChangedSlot(QString)));

}


void
canvasPropertyBroswer::show (bool show)
{
  setVisible (show);
}

canvasPropertyBroswer *
canvasPropertyBroswer::getInstance ()
{
  if (!pcanvasPropertyBrowser)
    {
      pcanvasPropertyBrowser = new canvasPropertyBroswer;
    }
  return pcanvasPropertyBrowser;
}

void
canvasPropertyBroswer::systemReset ()
{
  reset ();
}


void
canvasPropertyBroswer::setCurrentNodeId (uint32_t currentNodeId)
{
  m_nodeIdSelector->setCurrentIndex (currentNodeId);
  nodeIdSelectorSlot (QString::number (currentNodeId));
}

void
canvasPropertyBroswer::postParse ()
{
  reset ();
  uint32_t count = canvasNodeMgr::getInstance ()->getCount ();
  for (uint32_t i = 0; i < count; ++i)
    {
      m_nodeIdSelector->addItem (QString::number (i));
    }
  setupManagers ();
  setupFactories ();
  setupNodeProperties ();
//  setupBackgroundProperties ();
  connect (m_nodeIdSelector, SIGNAL(currentIndexChanged(QString)), this, SLOT (nodeIdSelectorSlot(QString)));

}

void
canvasPropertyBroswer::reset ()
{
  m_currentNodeId = 0;
  m_nodeIdSelector->clear ();
  disconnect (m_nodeIdSelector, SIGNAL(currentIndexChanged(QString)), this, SLOT (nodeIdSelectorSlot(QString)));
  if (m_intManager)
    delete m_intManager;
  if (m_stringManager)
    delete m_stringManager;
  if (m_doubleManager)
    delete m_doubleManager;
  if (m_colorManager)
    delete m_colorManager;
  if (m_filePathManager)
    delete m_filePathManager;
  if (m_nodePositionManager)
    delete m_nodePositionManager;
  if (m_ipv4AddressManager)
    delete m_ipv4AddressManager;
  if (m_macAddressManager)
    delete m_macAddressManager;
  if (m_staticStringManager)
    delete m_staticStringManager;
  if (m_boolManager)
    delete m_boolManager;
  if (m_backgroundDoubleManager)
    delete m_backgroundDoubleManager;
  m_intManager = 0;
  m_stringManager = 0;
  m_doubleManager = 0;
  m_colorManager = 0;
  m_filePathManager = 0;
  m_nodePositionManager = 0;
  m_ipv4AddressManager = 0;
  m_macAddressManager = 0;
  m_staticStringManager = 0;
  m_boolManager = 0;
  m_backgroundDoubleManager = 0;

  if (m_doubleSpinBoxFactory)
    delete m_doubleSpinBoxFactory;
  if (m_spinBoxFactory)
    delete m_spinBoxFactory;
  if (m_fileEditFactory)
    delete m_fileEditFactory;
  if (m_lineEditFactory)
    delete m_lineEditFactory;
  if (m_checkBoxFactory)
    delete m_checkBoxFactory;

  m_doubleSpinBoxFactory = 0;
  m_spinBoxFactory = 0;
  m_fileEditFactory = 0;
  m_lineEditFactory = 0;
  m_checkBoxFactory = 0;

}

void
canvasPropertyBroswer::setupManagers ()
{
  m_intManager = new QtIntPropertyManager;
  m_stringManager = new QtStringPropertyManager;
  m_doubleManager = new QtDoublePropertyManager;
  m_colorManager = new QtColorPropertyManager;
  m_filePathManager = new FilePathManager;
  m_nodePositionManager = new QtGroupPropertyManager;
  m_ipv4AddressManager = new QtGroupPropertyManager;
  m_macAddressManager = new QtGroupPropertyManager;
  m_staticStringManager = new QtStringPropertyManager;
  m_boolManager = new QtBoolPropertyManager;
  m_backgroundDoubleManager = new QtDoublePropertyManager;

}

void
canvasPropertyBroswer::setupFactories ()
{
  m_doubleSpinBoxFactory = new QtDoubleSpinBoxFactory;
  m_spinBoxFactory = new QtSpinBoxFactory;
  m_fileEditFactory = new FileEditFactory;
  m_lineEditFactory = new QtLineEditFactory;
  m_checkBoxFactory = new QtCheckBoxFactory;
}


//void
//canvasPropertyBroswer::refreshBackgroundProperties ()
//{
//  BackgroudImageProperties_t prop = canvasatorMode::getInstance ()->getBackgroundProperties ();
//  m_backgroundDoubleManager->setValue (m_backgroundX, prop.x);
//  m_backgroundDoubleManager->setValue (m_backgroundY, prop.y);
//  //m_backgroundDoubleManager->setValue (m_backgroundScaleX, prop.scaleX);
//  //m_backgroundDoubleManager->setValue (m_backgroundScaleY, prop.scaleY);
//  m_backgroundDoubleManager->setValue (m_backgroundOpacity, prop.opacity);
//  m_backgroundBrowser->setFactoryForManager (m_backgroundDoubleManager, m_doubleSpinBoxFactory);

//}
//void
//canvasPropertyBroswer::setupBackgroundProperties ()
//{
//  m_backgroundX = m_backgroundDoubleManager->addProperty ("X");
//  m_backgroundY = m_backgroundDoubleManager->addProperty ("Y");
//  m_backgroundScaleX = m_backgroundDoubleManager->addProperty ("Scale X By");
//  m_backgroundScaleY = m_backgroundDoubleManager->addProperty ("Scale Y By");
//  m_backgroundOpacity = m_backgroundDoubleManager->addProperty ("Opacity (0.0 to 1.0)");

//  m_backgroundBrowser->addProperty (m_backgroundX);
//  m_backgroundBrowser->addProperty (m_backgroundY);
//  m_backgroundBrowser->addProperty (m_backgroundScaleX);
//  m_backgroundBrowser->addProperty (m_backgroundScaleY);
//  m_backgroundBrowser->addProperty (m_backgroundOpacity);

//  BackgroudImageProperties_t prop = canvasatorMode::getInstance ()->getBackgroundProperties ();
//  m_backgroundDoubleManager->setValue (m_backgroundX, prop.x);
//  m_backgroundDoubleManager->setValue (m_backgroundY, prop.y);
//  m_backgroundDoubleManager->setValue (m_backgroundScaleX, prop.scaleX);
//  m_backgroundDoubleManager->setValue (m_backgroundScaleY, prop.scaleY);
//  m_backgroundDoubleManager->setValue (m_backgroundOpacity, prop.opacity);
//  m_backgroundBrowser->setFactoryForManager (m_backgroundDoubleManager, m_doubleSpinBoxFactory);
//  connect (m_backgroundDoubleManager, SIGNAL(valueChanged(QtProperty*,double)), this, SLOT(valueChangedSlot(QtProperty*,double)));


//}

void
canvasPropertyBroswer::setupNodeProperties ()
{

  // Properties

  canvasNode * canvasNode = canvasNodeMgr::getInstance ()->getNode (m_currentNodeId);

  // Node Id, Node System Id
  m_nodeIdProperty = m_intManager->addProperty ("Node Id");
  m_intManager->setValue (m_nodeIdProperty, m_currentNodeId);
  m_nodeBrowser->addProperty (m_nodeIdProperty);
  m_nodeSysIdProperty = m_intManager->addProperty ("Node MAC Address");
  m_intManager->setValue (m_nodeSysIdProperty, canvasNode ->getNodeSysId ());
  m_nodeBrowser->addProperty (m_nodeSysIdProperty);


//   Node Description
  m_nodeDescriptionProperty = m_stringManager->addProperty ("Node Description");
  m_nodeBrowser->setFactoryForManager (m_stringManager, m_lineEditFactory);
  connect (m_stringManager, SIGNAL (valueChanged (QtProperty*,QString)), this, SLOT (valueChangedSlot (QtProperty*,QString)));
  m_nodeBrowser->addProperty (m_nodeDescriptionProperty);
  m_stringManager->setValue (m_nodeDescriptionProperty, canvasNode->getDescription ()->toPlainText ());


  // Node Position
  m_nodePositionGroupProperty = m_nodePositionManager->addProperty ("Node Position");
  m_nodeXProperty = m_doubleManager->addProperty ("Node X");
  m_nodeYProperty = m_doubleManager->addProperty ("Node Y");
  m_nodePositionGroupProperty->addSubProperty (m_nodeXProperty);
  m_nodePositionGroupProperty->addSubProperty (m_nodeYProperty);
  m_doubleManager->setMinimum (m_nodeXProperty, 0);
  m_doubleManager->setMinimum (m_nodeYProperty, 0);
  m_doubleManager->setValue (m_nodeXProperty, canvasNode->getX ());
  m_doubleManager->setValue (m_nodeYProperty, canvasNode->getY ());
  connect (m_doubleManager, SIGNAL(valueChanged(QtProperty*,double)), this, SLOT(valueChangedSlot(QtProperty*,double)));
  m_nodeBrowser->addProperty (m_nodePositionGroupProperty);
  m_nodeBrowser->setFactoryForManager (m_doubleManager, m_doubleSpinBoxFactory);


  // Node Color
  m_nodeColorProperty = m_colorManager->addProperty ("Node Color");
  connect(m_colorManager, SIGNAL(valueChanged(QtProperty*,QColor)), this, SLOT(valueChangedSlot(QtProperty*,QColor)));
  m_nodeBrowser->addProperty (m_nodeColorProperty);
  m_nodeBrowser->setFactoryForManager (m_colorManager->subIntPropertyManager (), m_spinBoxFactory);
  QColor c = canvasNode->getColor ();
  m_colorManager->setValue (m_nodeColorProperty, c);


  // Node Size
  m_nodeSizeProperty = m_doubleManager->addProperty ("Node Size");
  m_doubleManager->setValue (m_nodeSizeProperty, canvasNode->getWidth ());
  m_doubleManager->setMinimum (m_nodeSizeProperty, 0.1);
  m_nodeBrowser->addProperty (m_nodeSizeProperty);


  // Node Resource
  int resourceId = canvasNode->getResourceId ();
  QString resourcePath = "";
//  if (resourceId != -1)
//    {
//      resourcePath = canvasResourceManager::getInstance ()->get (resourceId);
//    }
   m_fileEditProperty = m_filePathManager->addProperty ("Node Resource");
   m_filePathManager->setValue (m_fileEditProperty, resourcePath);
   m_nodeBrowser->addProperty (m_fileEditProperty);
   m_nodeBrowser->setFactoryForManager (m_filePathManager, m_fileEditFactory);
   connect (m_filePathManager, SIGNAL(valueChanged(QtProperty*,QString)), this, SLOT(valueChangedSlot(QtProperty*,QString)));


//    Node Trajectory
   m_showNodeTrajectoryProperty = m_boolManager->addProperty ("Show Node Trajectory");
   m_nodeBrowser->setFactoryForManager  (m_boolManager, m_checkBoxFactory);
   m_boolManager->setValue (m_showNodeTrajectoryProperty, canvasNode->getShowNodeTrajectory ());
//   m_nodeBrowser->addProperty (m_showNodeTrajectoryProperty);
   connect (m_boolManager, SIGNAL(valueChanged(QtProperty*,bool)), this, SLOT(valueChangedSlot(QtProperty*,bool)));


   // IPv4 and Mac
   m_ipv4AddressGroupProperty = m_ipv4AddressManager->addProperty ("Ipv4 Addresses");
   m_macAddressGroupProperty = m_macAddressManager->addProperty ("Mac Addresses");
   canvasNode::Ipv4Vector_t ipv4Addresses = canvasNode->getIpv4Addresses ();
   for (canvasNode::Ipv4Vector_t::const_iterator i = ipv4Addresses.begin ();
        i != ipv4Addresses.end ();
        ++i)
     {
       QtProperty * property = m_staticStringManager->addProperty (*i);
       m_ipv4AddressGroupProperty->addSubProperty (property);
       m_ipv4AddressVectorProperty.push_back (property);
     }
   canvasNode::MacVector_t macAddresses = canvasNode->getMacAddresses ();
   for (canvasNode::MacVector_t::const_iterator i = macAddresses.begin ();
        i != macAddresses.end ();
        ++i)
    {
      QtProperty * property = m_staticStringManager->addProperty (*i);
      m_macAddressGroupProperty->addSubProperty (property);
      m_macAddressVectorProperty.push_back (property);
    }
   m_nodeBrowser->addProperty (m_ipv4AddressGroupProperty);
   m_nodeBrowser->addProperty (m_macAddressGroupProperty);

   canvasNodeMgr::CounterIdName_t doubleCounterNames = canvasNodeMgr::getInstance ()->getDoubleCounterNames ();
   canvasNodeMgr::CounterIdName_t uint32CounterNames = canvasNodeMgr::getInstance ()->getUint32CounterNames ();
   for (canvasNodeMgr::CounterIdName_t::const_iterator i = doubleCounterNames.begin ();
        i != doubleCounterNames.end ();
        ++i)
     {
       QString counterName = i->second;
       bool result = false;
       qreal counterValue = canvasNode->getDoubleCounterValue (i->first, result);
       if (!result)
         continue;
       QtProperty * prop = m_doubleManager->addProperty (counterName);
       m_doubleManager->setValue (prop, counterValue);
       m_nodeBrowser->addProperty (prop);
       m_nodeCounterDoubleProperty.push_back (prop);
     }

   for (canvasNodeMgr::CounterIdName_t::const_iterator i = uint32CounterNames.begin ();
        i != uint32CounterNames.end ();
        ++i)
     {
       QString counterName = i->second;
       bool result = false;
       qreal counterValue = canvasNode->getUint32CounterValue (i->first, result);
       if (!result)
         continue;
       QtProperty * prop = m_intManager->addProperty (counterName);
       m_intManager->setValue (prop, counterValue);
       m_nodeBrowser->addProperty (prop);
       m_nodeCounterUint32Property.push_back (prop);
     }

}

void
canvasPropertyBroswer::valueChangedSlot (QtProperty * property, QColor c)
{
  if (m_nodeColorProperty == property)
    {
      canvasNode * canvasNode = canvasNodeMgr::getInstance ()->getNode (m_currentNodeId);
      canvasNode->setColor (c.red (), c.green (), c.blue (), c.alpha ());
    }
}

void
canvasPropertyBroswer::valueChangedSlot(QtProperty * property, double value)
{
  if (m_nodeXProperty == property)
    {
      canvasNode * canvasNode = canvasNodeMgr::getInstance ()->getNode (m_currentNodeId);
      canvasNode->setX (value);
      qreal x = canvasNode->getX ();
      qreal y = canvasNode->getY ();
      canvasscene::getInstance ()->setNodePos (canvasNode, x, y);
    }
  if (m_nodeYProperty == property)
    {
      canvasNode * canvasNode = canvasNodeMgr::getInstance ()->getNode (m_currentNodeId);
      canvasNode->setY (value);
      qreal x = canvasNode->getX ();
      qreal y = canvasNode->getY ();
      canvasscene::getInstance ()->setNodePos (canvasNode, x, y);

    }
  if (m_nodeSizeProperty == property)
    {
      canvasNode * canvasNode = canvasNodeMgr::getInstance ()->getNode (m_currentNodeId);
      MainWindow::getInstance ()->setNodeSize (canvasNode, value);
    }
//  canvasscene * scene = canvasscene ::getInstance();
//  if (m_backgroundX == property)
//    {
//      scene->setBackgroundX (value);
//    }
//  if (m_backgroundY == property)
//    {
//      scene->setBackgroundY (value);
//    }

//  if (m_backgroundScaleX == property)
//    {
//      scene->setBackgroundScaleX (value);
//    }
//  if (m_backgroundScaleY == property)
//    {
//      scene->setBackgroundScaleY (value);
//    }
//  if (m_backgroundOpacity == property)
//    {
//      scene->setBackgroundOpacity (value);
//    }
}


void
canvasPropertyBroswer::valueChangedSlot(QtProperty * property, QString description)
{
  if (m_nodeDescriptionProperty == property)
    {
      canvasNode * canvasNode = canvasNodeMgr::getInstance ()->getNode (m_currentNodeId);
      canvasNode->setNodeDescription (description);
    }
//  if (m_fileEditProperty == property)
//    {
//      if (description == "")
//        return;
//      uint32_t newResourceId = canvasResourceManager::getInstance ()->getNewResourceId ();
//      canvasResourceManager::getInstance ()->add (newResourceId, description);
//      canvasNode * canvasNode = canvasNodeMgr::getInstance ()->getNode (m_currentNodeId);
//      canvasNode->setResource (newResourceId);
//    }
}

void
canvasPropertyBroswer::modeChangedSlot (QString mode)
{
  if (mode == "Node")
    {
      m_nodeIdSelector->setVisible (true);
      m_nodeBrowser->setVisible (true);
      m_nodePosTable->setVisible (true);
      m_backgroundBrowser->setVisible (false);

    }
//  else if (mode == "Background")
//    {
//      if (canvasscene::getInstance ()->getBackgroundImage())
//        {
//          m_nodeIdSelector->setVisible (false);
//          m_nodeBrowser->setVisible (false);
//          m_nodePosTable->setVisible (false);
//          m_backgroundBrowser->setVisible (true);
//          refreshBackgroundProperties ();
//        }
//      else
//        {
//          canvasatorMode::getInstance ()->showPopup ("No Background image set in the xml file");
//        }
//    }
}

void
canvasPropertyBroswer::valueChangedSlot (QtProperty * property, bool showNodeTrajectory)
{
  if (m_showNodeTrajectoryProperty == property)
    {
      canvasNode * m_canvasNode = canvasNodeMgr::getInstance ()->getNode (m_currentNodeId);
      m_canvasNode->setShowNodeTrajectory (showNodeTrajectory);
      MainWindow::getInstance ()->setShowNodeTrajectory (m_canvasNode);
      showNodePositionTable (showNodeTrajectory);
    }
}

void
canvasPropertyBroswer::refresh ()
{
  nodeIdSelectorSlot (m_nodeIdSelector->currentText ());
}


void
canvasPropertyBroswer::showNodePositionTable (bool show)
{
  m_nodePosTable->setVisible (show);
  m_nodePosTable->clear ();
  canvasNodeMgr::TimePosVector_t tpv = canvasNodeMgr::getInstance ()->getPositions (m_currentNodeId);
  QStringList headerList;
  headerList << "Time" << "X-Coord" << "Y-Coord";
  m_nodePosTable->setHeaderList (headerList);
  foreach (TimePosition_t tp, tpv)
    {
      QStringList rowStringList;
      rowStringList << QString::number (tp.t)
                    << QString::number (tp.p.x ())
                    << QString::number (tp.p.y ());
      m_nodePosTable->addRow (rowStringList);
    }
}

void
canvasPropertyBroswer::nodeIdSelectorSlot (QString newIndex)
{
  //NS_LOG_DEBUG (newIndex.toUInt());
  m_currentNodeId = newIndex.toUInt ();
  terminalId = newIndex.toInt();
  canvasNode * canvasNode = canvasNodeMgr::getInstance ()->getNode (m_currentNodeId);

  // Node Id
  m_intManager->setValue (m_nodeIdProperty, m_currentNodeId);


  if (!canvasNode)
    return;

  // Node System Id
  m_intManager->setValue (m_nodeSysIdProperty, canvasNode->getNodeSysId ());


  // Node Description
  m_stringManager->setValue (m_nodeDescriptionProperty, canvasNode->getDescription ()->toPlainText ());


  // Node Position
  m_doubleManager->setValue (m_nodeXProperty, canvasNode->getX ());
  m_doubleManager->setValue (m_nodeYProperty, canvasNode->getY ());


  // Node Color
  QColor c = canvasNode->getColor ();
  m_colorManager->setValue (m_nodeColorProperty, c);


  // Node Size
  m_doubleManager->setValue (m_nodeSizeProperty, canvasNode->getWidth ());
  m_doubleManager->setMinimum (m_nodeSizeProperty, 0.1);


  // Node Resource
//  int resourceId = canvasNode->getResourceId ();
//  QString resourcePath = "";
//  if (resourceId != -1)
//    {
//      resourcePath = canvasResourceManager::getInstance ()->get (resourceId);
//    }
//   m_filePathManager->setValue (m_fileEditProperty, resourcePath);


//  m_boolManager->setValue (m_showNodeTrajectoryProperty, canvasNode->getShowNodeTrajectory ());
//  canvasatorMode::getInstance ()->setShowNodeTrajectory (canvasNode);


   // IPv4 and Mac
   canvasNode::Ipv4Vector_t ipv4Addresses = canvasNode->getIpv4Addresses ();
   for (canvasPropertyBroswer::QtPropertyVector_t::const_iterator i = m_ipv4AddressVectorProperty.begin ();
        i != m_ipv4AddressVectorProperty.end ();
        ++i)
     {
       QtProperty * property = *i;
       m_ipv4AddressGroupProperty->removeSubProperty (property);
     }
   m_ipv4AddressVectorProperty.clear ();
   for (canvasNode::Ipv4Vector_t::const_iterator i = ipv4Addresses.begin ();
        i != ipv4Addresses.end ();
        ++i)
     {
       QtProperty * property = m_staticStringManager->addProperty (*i);
       m_ipv4AddressGroupProperty->addSubProperty (property);
       m_ipv4AddressVectorProperty.push_back (property);
     }
   canvasNode::MacVector_t macAddresses = canvasNode->getMacAddresses ();

   for (canvasPropertyBroswer::QtPropertyVector_t::const_iterator i = m_macAddressVectorProperty.begin ();
        i != m_macAddressVectorProperty.end ();
        ++i)
     {
       QtProperty * property = *i;
       m_macAddressGroupProperty->removeSubProperty (property);
     }
   m_macAddressVectorProperty.clear ();

   for (canvasNode::MacVector_t::const_iterator i = macAddresses.begin ();
        i != macAddresses.end ();
        ++i)
    {
      QtProperty * property = m_staticStringManager->addProperty (*i);
      m_macAddressGroupProperty->addSubProperty (property);
      m_macAddressVectorProperty.push_back (property);
    }


   for (canvasPropertyBroswer::QtPropertyVector_t::const_iterator i = m_nodeCounterDoubleProperty.begin ();
        i != m_nodeCounterDoubleProperty.end ();
        ++i)
     {
       QtProperty * property = *i;
       m_nodeBrowser->removeProperty (property);
     }
   m_nodeCounterDoubleProperty.clear ();

   for (canvasPropertyBroswer::QtPropertyVector_t::const_iterator i = m_nodeCounterUint32Property.begin ();
        i != m_nodeCounterUint32Property.end ();
        ++i)
     {
       QtProperty * property = *i;
       m_nodeBrowser->removeProperty (property);
     }
   m_nodeCounterUint32Property.clear ();
   canvasNodeMgr::CounterIdName_t doubleCounterNames = canvasNodeMgr::getInstance ()->getDoubleCounterNames ();
   canvasNodeMgr::CounterIdName_t uint32CounterNames = canvasNodeMgr::getInstance ()->getUint32CounterNames ();
   for (canvasNodeMgr::CounterIdName_t::const_iterator i = doubleCounterNames.begin ();
        i != doubleCounterNames.end ();
        ++i)
     {
       QString counterName = i->second;
       bool result = false;
       qreal counterValue = canvasNode->getDoubleCounterValue (i->first, result);
       if (!result)
         continue;
       QtProperty * prop = m_doubleManager->addProperty (counterName);
       m_doubleManager->setValue (prop, counterValue);
       m_nodeBrowser->addProperty (prop);
       m_nodeCounterDoubleProperty.push_back (prop);
     }

   for (canvasNodeMgr::CounterIdName_t::const_iterator i = uint32CounterNames.begin ();
        i != uint32CounterNames.end ();
        ++i)
     {
       QString counterName = i->second;
       bool result = false;
       qreal counterValue = canvasNode->getUint32CounterValue (i->first, result);
       if (!result)
         continue;
       QtProperty * prop = m_intManager->addProperty (counterName);
       m_intManager->setValue (prop, counterValue);
       m_nodeBrowser->addProperty (prop);
       m_nodeCounterUint32Property.push_back (prop);
     }

}

int canvasPropertyBroswer::getTerminalString()
{
    qDebug()<<terminalId;
        return terminalId;
}

