#include <qtaiv.h>

N::SelectionListFilterEditorPopup:: SelectionListFilterEditorPopup (QWidget * parent,Plan * p)
                                  : FilterEditorPopupWidget        (          parent,       p)
{
        m_mode = 0; // 0 = Selected values 1 = empty 2 = not empty
    m_model = new QStandardItemModel(this);
    m_checkStateProxy = new CheckStateProxyModel(this);
        connect(m_checkStateProxy, SIGNAL(dataChanged(QModelIndex, QModelIndex)), this, SLOT(checkStateProxyDataChanged(QModelIndex, QModelIndex)));
    m_checkStateProxy->setSourceModel(m_model);
        m_checkStateProxy->setColumnCheckable(0);

    QVBoxLayout* l = new QVBoxLayout();
    l->setContentsMargins(6, 6, 6, 6);

    QVBoxLayout* lb = new QVBoxLayout();
        lb->setSpacing(3);
        m_emptyToolButton = new QToolButton(this);
        m_emptyToolButton->setText(tr("Empty"));
        m_emptyToolButton->setAutoRaise(true);
        m_emptyToolButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        lb->addWidget(m_emptyToolButton);
        connect(m_emptyToolButton, SIGNAL(clicked()), this, SLOT(emptyToolButtonClicked()));

	m_notEmptyToolButton = new QToolButton(this);
	m_notEmptyToolButton->setText(tr("Not Empty"));
	m_notEmptyToolButton->setAutoRaise(true);
	m_notEmptyToolButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	lb->addWidget(m_notEmptyToolButton);
	l->addLayout(lb);
	connect(m_notEmptyToolButton, SIGNAL(clicked()), this, SLOT(notEmptyToolButtonClicked()));

    m_lineEdit = new LineEdit(this,plan);
    m_lineEdit->setMinimumWidth(200);
    m_lineEdit->setPlaceholderText(tr("Search pattern"));
        m_lineEdit->installEventFilter(parent);
    connect(m_lineEdit, SIGNAL(textEdited(QString)), this, SLOT(searchForTextEdited(QString)));

    l->addWidget(m_lineEdit);
    m_listView = new ListView(this,plan);
    l->addWidget(m_listView);

	m_selectCheckBox = new QCheckBox(this);
	m_selectCheckBox->setText(tr("Select/Deselect all"));
	m_selectCheckBox->setTristate(true);
	m_selectCheckBox->installEventFilter(parent);
	connect(m_selectCheckBox, SIGNAL(stateChanged(int)), this, SLOT(selectCheckBoxStateChanged(int)));

	l->addWidget(m_selectCheckBox);

    setLayout(l);

    m_listView->setModel(m_checkStateProxy);
        setFocusProxy(m_listView);
        m_listView->setFocus();
        m_listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        m_listView->installEventFilter(parent);
}

N::SelectionListFilterEditorPopup::~SelectionListFilterEditorPopup()
{
}

void N::SelectionListFilterEditorPopup::checkStateProxyDataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight)
{
	Q_UNUSED(topLeft);
	Q_UNUSED(bottomRight);
	m_selectCheckBox->blockSignals(true);
	if (m_checkStateProxy->checkedIndexes().size() == 0){
		m_selectCheckBox->setCheckState(Qt::Unchecked);
	} else if(m_checkStateProxy->checkedIndexes().size() == (m_checkStateProxy->checkableColumnsCount() * m_checkStateProxy->rowCount())){
		m_selectCheckBox->setCheckState(Qt::Checked);
	} else {
		m_selectCheckBox->setCheckState(Qt::PartiallyChecked);
	}
	m_selectCheckBox->blockSignals(false);
}

bool N::SelectionListFilterEditorPopup::cancelAndClose(QObject* obj, int key) const
{
	Q_UNUSED(obj);
	if (key == Qt::Key_Escape){
		return true;
	}
	return false;
}

bool N::SelectionListFilterEditorPopup::commitAndClose(QObject* obj, int key) const
{
	if (key == Qt::Key_Return){
		if (obj == m_listView){
			return true;
		}
	}
	return false;
}

void N::SelectionListFilterEditorPopup::emptyToolButtonClicked()
{
	m_mode = 1;
	emit modeChanged();
}

int N::SelectionListFilterEditorPopup::mode() const
{
	return m_mode;
}

void N::SelectionListFilterEditorPopup::notEmptyToolButtonClicked()
{
	m_mode = 2;
	emit modeChanged();
}

void N::SelectionListFilterEditorPopup::searchForTextEdited(const QString & text)
{
    QModelIndexList i = m_checkStateProxy->match(m_checkStateProxy->index(0,0), Qt::DisplayRole, text);
    if (!i.isEmpty()){
        m_listView->setCurrentIndex(i.first());
    }
}

void N::SelectionListFilterEditorPopup::selectCheckBoxStateChanged(int state)
{
	if (state == Qt::Checked){
		m_checkStateProxy->setAllChecked(true);
	} else if (state == Qt::Unchecked){
		m_checkStateProxy->setAllChecked(false);
	} else if (state == Qt::PartiallyChecked){
		m_selectCheckBox->setChecked(true);
	}
}

QVariantList N::SelectionListFilterEditorPopup::selectedValues() const
{
    QVariantList v;
    Q_FOREACH(QModelIndex i, m_checkStateProxy->checkedIndexes()){
        v.append(i.data());
    }
    return v;
}

void N::SelectionListFilterEditorPopup::setValues(const QVariantList & values)
{
    m_model->clear();
    m_model->setRowCount(values.size());
    m_model->setColumnCount(1);
    for (int iValue = 0; iValue < values.size(); iValue++){
        m_model->setItem(iValue, 0, new QStandardItem(values.at(iValue).toString()));
    }
}

void N::SelectionListFilterEditorPopup::setSelectedValues(const QVariantList & selectedValues)
{
    m_checkStateProxy->setCheckedValues(0, selectedValues);
}
