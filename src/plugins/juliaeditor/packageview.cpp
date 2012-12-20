#include "packageview.h"
#include "packagemodel.h"

#include <QHeaderView>

using namespace JuliaPlugin;

PackageView::PackageView(QWidget *parent) :
  QWidget(parent)
{
  setWindowTitle( "Packages" );

  // layout -----
  grid_layout = new QGridLayout(this);
  grid_layout->setObjectName( QString::fromUtf8("package_grid_layout") );

  grid_layout->setSpacing(0);
  grid_layout->setContentsMargins(0, 0, 0, 0);
  // -----

  // list customization -----
  list_view = new QTreeView(this);
  list_view->setObjectName( QString::fromUtf8("package_list_view") );
  list_view->setModel( package_model = new PackageModel );
  connect( list_view, SIGNAL(doubleClicked(QModelIndex)), package_model, SLOT(AddPackage(const QModelIndex&)) );

  list_view->setItemDelegate( new PackageDelegate(this) );
  list_view->setIndentation(0);
  list_view->setFrameStyle(QFrame::NoFrame);
  list_view->setUniformRowHeights(true);
  list_view->setAttribute(Qt::WA_MacShowFocusRect, false);
  list_view->setTextElideMode(Qt::ElideMiddle);
  list_view->setSelectionMode(QAbstractItemView::SingleSelection);
  list_view->setSelectionBehavior(QAbstractItemView::SelectRows);
  list_view->setContextMenuPolicy(Qt::CustomContextMenu);
  list_view->installEventFilter(this);

  list_view->header()->hide();
  list_view->header()->setStretchLastSection(false);
  list_view->header()->setResizeMode(0, QHeaderView::Stretch);
  list_view->header()->setResizeMode(1, QHeaderView::Fixed);
  list_view->header()->resizeSection(1, 16);

  list_view->viewport()->setAttribute(Qt::WA_Hover);
  list_view->viewport()->installEventFilter(this);
  // -----

  grid_layout->addWidget(list_view, 0, 0, 1, 1);
}


Core::NavigationView PackageViewFactory::createWidget()
{
  Core::NavigationView view;
  view.widget = new PackageView;
  return view;
}
