#include <qtaiv.h>

N::RegExpFilterEditor:: RegExpFilterEditor (QWidget * parent,Plan * p)
                      : Widget             (          parent,       p)
{
    QHBoxLayout* l = new QHBoxLayout(this);
    l->setSpacing(0);
    l->setContentsMargins(0, 0, 0, 0);
        l->setSpacing(6);

    m_patternEdit = new LineEdit(this,plan);
    l->addWidget(m_patternEdit);

    m_sensitivityLabel = new ClickableLabel(this,plan);
    connect(m_sensitivityLabel, SIGNAL(clicked(Qt::MouseButtons)), this, SLOT(sensitivityLabelClicked(Qt::MouseButtons)));
    l->addWidget(m_sensitivityLabel);

	m_wildcardCheckBox = new QCheckBox(this);
	m_wildcardCheckBox->setText(tr("W"));
    l->addWidget(m_wildcardCheckBox);

        setFocusProxy(m_patternEdit);

    setCaseSensitivity(Qt::CaseInsensitive);
    setFocusPolicy(Qt::StrongFocus);
        setAutoFillBackground(true);
}

N::RegExpFilterEditor::~RegExpFilterEditor()
{
}

Qt::CaseSensitivity N::RegExpFilterEditor::caseSenstivity() const
{
    return m_sensitivity;
}

void N::RegExpFilterEditor::sensitivityLabelClicked(Qt::MouseButtons buttons)
{
    if (buttons.testFlag(Qt::LeftButton)){
        if (m_sensitivity == Qt::CaseInsensitive){
            setCaseSensitivity(Qt::CaseSensitive);
        } else {
            setCaseSensitivity(Qt::CaseInsensitive);
        }
    }
}

QString N::RegExpFilterEditor::pattern() const
{
    return m_patternEdit->text();
}

QRegExp::PatternSyntax N::RegExpFilterEditor::patternSyntax() const
{
    //if (m_wildcardCheckBox->isChecked()){
    //    return QRegExp::Wildcard;
    //}
    return QRegExp::RegExp;
}

void N::RegExpFilterEditor::setCaseSensitivity(Qt::CaseSensitivity sensitivity)
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

void N::RegExpFilterEditor::setPattern(const QString &pattern)
{
    m_patternEdit->setText(pattern);
}

void N::RegExpFilterEditor::setPatternSyntax(QRegExp::PatternSyntax patternSyntax)
{
    //m_wildcardCheckBox->setChecked(patternSyntax == QRegExp::Wildcard);
}

