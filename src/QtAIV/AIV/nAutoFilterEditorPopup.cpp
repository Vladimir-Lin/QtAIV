#include <qtaiv.h>

N::AutoFilterEditorPopup:: AutoFilterEditorPopup   (QWidget * parent,Plan * p)
			 : FilterEditorPopupWidget (          parent,       p)
{
	m_progress = new QProgressDialog(this);
	m_progress->setMaximum(100);
	m_mode = 0; // 0 = Selected values 1 = empty 2 = not empty
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

	QFrame* f = new QFrame(this);
	f->setFrameShape(QFrame::HLine);
    f->setFrameShadow(QFrame::Sunken);
        l->addWidget(f);

    m_lineEdit = new LineEdit(this,plan);
    m_lineEdit->setMinimumWidth(200);
    m_lineEdit->setPlaceholderText(tr("Search for"));
    connect(m_lineEdit, SIGNAL(textEdited(QString)), this, SLOT(searchForTextEdited(QString)));

    l->addWidget(m_lineEdit);
    m_listView = new ListView(this,plan);
    l->addWidget(m_listView);
    setLayout(l);

    m_checkStateProxy = new CheckStateProxyModel(this);
        connect(m_checkStateProxy, SIGNAL(dataChanged(QModelIndex, QModelIndex)), this, SLOT(checkStateProxyDataChanged(QModelIndex, QModelIndex)));

        m_singleColumnProxy = new SingleColumnProxyModel(this);
    m_singleValueProxy = new UniqueValuesProxyModel(this);
        connect(m_singleValueProxy, SIGNAL(progressChanged(int)), this, SLOT(uniqueValueModelProgressChanged(int)));
        m_singleValueProxy->setEmptyItemsAllowed(false);

    m_singleColumnProxy->setSourceModel(m_singleValueProxy);
    m_checkStateProxy->setSourceModel(m_singleColumnProxy);
        m_listView->setModel(m_checkStateProxy);

	m_selectCheckBox = new QCheckBox(this);
	m_selectCheckBox->setText(tr("Select/Deselect all"));
	m_selectCheckBox->setTristate(true);
	m_selectCheckBox->installEventFilter(parent);
	connect(m_selectCheckBox, SIGNAL(stateChanged(int)), this, SLOT(selectCheckBoxStateChanged(int)));

	l->addWidget(m_selectCheckBox);
	m_listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	m_listView->installEventFilter(parent);
}

N::AutoFilterEditorPopup::~AutoFilterEditorPopup()
{
}

void N::AutoFilterEditorPopup::checkStateProxyDataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight)
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

void N::AutoFilterEditorPopup::emptyToolButtonClicked()
{
	m_mode = 1;
	emit modeChanged();
}

int N::AutoFilterEditorPopup::mode() const
{
	return m_mode;
}

void N::AutoFilterEditorPopup::notEmptyToolButtonClicked()
{
	m_mode = 2;
	emit modeChanged();
}

void N::AutoFilterEditorPopup::searchForTextEdited(const QString & text)
{
    QModelIndexList mIndexes = m_checkStateProxy->match(m_checkStateProxy->index(0,0), Qt::DisplayRole, text);
    if (!mIndexes.isEmpty()){
        m_listView->setCurrentIndex(mIndexes.first());
    }
}

void N::AutoFilterEditorPopup::selectCheckBoxStateChanged(int state)
{
	if (state == Qt::Checked){
		m_checkStateProxy->setAllChecked(true);
	} else if (state == Qt::Unchecked){
		m_checkStateProxy->setAllChecked(false);
	} else if (state == Qt::PartiallyChecked){
		m_selectCheckBox->setChecked(true);
	}
}

QVariantList N::AutoFilterEditorPopup::selectedValues(int role) const
{
    QVariantList v;
    Q_FOREACH(QModelIndex mIndex, m_checkStateProxy->checkedIndexes()){
        v.append(mIndex.data(role));
    }
    return v;
}

void N::AutoFilterEditorPopup::setSelectedValues(const QVariantList & values)
{
    m_checkStateProxy->setCheckedValues(0, values);
}

void N::AutoFilterEditorPopup::setSourceModel(QAbstractItemModel *model, int column)
{
    m_singleValueProxy->setModelColumn(column);
    m_singleValueProxy->setSourceModel(model);
    m_singleColumnProxy->setSourceModelColumn(column);
    m_singleColumnProxy->sort(0);
        m_checkStateProxy->setColumnCheckable(0);
}

void N::AutoFilterEditorPopup::uniqueValueModelProgressChanged(int progress)
{
        m_progress->setValue(progress);
}
