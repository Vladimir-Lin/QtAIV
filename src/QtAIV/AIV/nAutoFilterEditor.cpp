#include <qtaiv.h>

N::AutoFilterEditor:: AutoFilterEditor   (QWidget * parent,Plan * p)
                    : FilterEditorWidget (          parent,       p)
{
  setPopup(new AutoFilterEditorPopup(this));
  setFocusProxy(popup());
  connect(popup(), SIGNAL(modeChanged()), this, SLOT(modeSelected()));
  setFocusPolicy(Qt::StrongFocus);
}

N::AutoFilterEditor::~AutoFilterEditor(void)
{
}

void N::AutoFilterEditor::modeSelected()
{
	emit commitAndClose();
}

void N::AutoFilterEditor::setSourceModel(QAbstractItemModel *model, int column)
{
	AutoFilterEditorPopup* e = qobject_cast<AutoFilterEditorPopup*>(popup());
	e->setSourceModel(model, column);
}
