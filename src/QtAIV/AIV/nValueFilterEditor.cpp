#include <qtaiv.h>

N::ValueFilterEditor:: ValueFilterEditor (QWidget * parent,Plan * p)
                     : Widget            (          parent,       p)
{
    QHBoxLayout* mLayout = new QHBoxLayout(this);
    mLayout->setSpacing(0);
    mLayout->setContentsMargins(0, 0, 0, 0);

    m_valueLineEdit = new LineEdit(this,plan);
    mLayout->addWidget(m_valueLineEdit);

    m_sensitivityLabel = new ClickableLabel(this,plan);
    connect(m_sensitivityLabel, SIGNAL(clicked(Qt::MouseButtons)), this, SLOT(sensitivityLabelClicked(Qt::MouseButtons)));
    mLayout->addWidget(m_sensitivityLabel);

    m_matchFlagsLabel = new ClickableLabel(this,plan);
    connect(m_matchFlagsLabel, SIGNAL(clicked(Qt::MouseButtons)), this, SLOT(matchFlagsLabelClicked(Qt::MouseButtons)));
    mLayout->addWidget(m_matchFlagsLabel);

    setFocusProxy(m_valueLineEdit);

    setMatchFlag(Qt::MatchContains);
    setCaseSensitivity(Qt::CaseInsensitive);

    setFocusPolicy(Qt::StrongFocus);
}

N::ValueFilterEditor::~ValueFilterEditor(void)
{
}

Qt::CaseSensitivity N::ValueFilterEditor::caseSensitivity() const
{
    return m_sensitivity;
}

Qt::MatchFlag N::ValueFilterEditor::matchFlag() const
{
    return m_matchFlag;
}

void N::ValueFilterEditor::matchFlagsLabelClicked(Qt::MouseButtons buttons)
{
    if (buttons.testFlag(Qt::LeftButton)){
        if (m_matchFlag == Qt::MatchStartsWith){
            setMatchFlag(Qt::MatchEndsWith);
        } else if (m_matchFlag == Qt::MatchEndsWith){
            setMatchFlag(Qt::MatchContains);
        } else if (m_matchFlag == Qt::MatchContains){
            setMatchFlag(Qt::MatchStartsWith);
        }
    }
}

void N::ValueFilterEditor::sensitivityLabelClicked(Qt::MouseButtons buttons)
{
    if (buttons.testFlag(Qt::LeftButton)){
        if (m_sensitivity == Qt::CaseInsensitive){
            setCaseSensitivity(Qt::CaseSensitive);
        } else {
            setCaseSensitivity(Qt::CaseInsensitive);
        }
    }
}
void N::ValueFilterEditor::setCaseSensitivity(Qt::CaseSensitivity sensitivity)
{
    if (sensitivity == Qt::CaseSensitive){
        m_sensitivityLabel->setPixmap(QPixmap(QString::fromUtf8(":/qaiv/filter/case_sensitive")));
        m_sensitivityLabel->setToolTip(tr("The filter is case sensitive"));
    } else {
        m_sensitivityLabel->setPixmap(QPixmap(QString::fromUtf8(":/qaiv/filter/case_insensitive")));
        m_sensitivityLabel->setToolTip(tr("The filter is case insensitive"));
    }
    m_sensitivity = sensitivity;
}

void N::ValueFilterEditor::setMatchFlag(Qt::MatchFlag flag)
{
    if (flag == Qt::MatchEndsWith){
        m_matchFlagsLabel->setPixmap(QPixmap(QString::fromUtf8(":/qaiv/filter/ends_with")));
        m_matchFlagsLabel->setToolTip(tr("The filter value matches the end of the item"));
    } else if (flag == Qt::MatchContains){
        m_matchFlagsLabel->setPixmap(QPixmap(QString::fromUtf8(":/qaiv/filter/contains")));
        m_matchFlagsLabel->setToolTip(tr("The filter value is contained in the item"));
    } else if (flag == Qt::MatchStartsWith){
        m_matchFlagsLabel->setPixmap(QPixmap(QString::fromUtf8(":/qaiv/filter/starts_with")));
        m_matchFlagsLabel->setToolTip(tr("The filter value matches the start of the item"));
    }
    m_matchFlag = flag;
}

void N::ValueFilterEditor::setText(const QString & text)
{
    m_valueLineEdit->setText(text);
}

QString N::ValueFilterEditor::text() const
{
    return m_valueLineEdit->text();
}
