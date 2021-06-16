#include <qtaiv.h>

N::SelectionListFilterEditor:: SelectionListFilterEditor (QWidget * parent,Plan * p)
                             : FilterEditorWidget        (          parent,       p)
{
  setPopup(new SelectionListFilterEditorPopup(this));
  setFocusProxy(popup());
  connect(popup(), SIGNAL(modeChanged()), this, SLOT(modeSelected()));
}

N::SelectionListFilterEditor::~SelectionListFilterEditor()
{
}

void N::SelectionListFilterEditor::modeSelected()
{
  emit commitAndClose();
}
