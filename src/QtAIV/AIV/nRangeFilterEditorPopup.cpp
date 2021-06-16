#include <qtaiv.h>

N::RangeFilterEditorPopup:: RangeFilterEditorPopup  ( QWidget * parent , Plan * p )
                          : FilterEditorPopupWidget (           parent ,        p )
{
    m_rangeFrom = new LineEdit(this,plan);
    m_rangeTo   = new LineEdit(this,plan);

    QFormLayout* l = new QFormLayout(this);
    l->addRow(tr("From:"), m_rangeFrom);
    l->addRow(tr("To:"), m_rangeTo);
    setLayout(l);

	m_rangeFrom->installEventFilter(parent);
	m_rangeTo->installEventFilter(parent);

	m_rangeFrom->setFocus();
}

N::RangeFilterEditorPopup::~RangeFilterEditorPopup()
{
}

QString N::RangeFilterEditorPopup::rangeFrom() const
{
	return m_rangeFrom->text();
}

QString N::RangeFilterEditorPopup::rangeTo() const
{
	return m_rangeTo->text();
}

void N::RangeFilterEditorPopup::setRangeFrom( const QString & text )
{
	m_rangeFrom->setText(text);
}

void N::RangeFilterEditorPopup::setRangeTo( const QString & text )
{
	m_rangeTo->setText(text);
}
