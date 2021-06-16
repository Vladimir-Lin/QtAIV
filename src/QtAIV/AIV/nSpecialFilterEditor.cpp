#include <qtaiv.h>

N::SpecialFilterEditor:: SpecialFilterEditor(QWidget * parent,Plan * p)
                     : FilterEditorWidget(parent,p)
{
    QHBoxLayout* mLayout = new QHBoxLayout(this);
    mLayout->setSpacing(0);
    mLayout->setContentsMargins(0, 0, 0, 0);

    cValueLineEdit = new LineEdit(this);
    mLayout->addWidget(cValueLineEdit);

    cSensitivityLabel = new ClickableLabel(this);
    connect(cSensitivityLabel, SIGNAL(clicked(Qt::MouseButtons)), this, SLOT(sensitivityLabelClicked(Qt::MouseButtons)));
    mLayout->addWidget(cSensitivityLabel);

    cMatchFlagsLabel = new ClickableLabel(this);
    connect(cMatchFlagsLabel, SIGNAL(clicked(Qt::MouseButtons)), this, SLOT(matchFlagsLabelClicked(Qt::MouseButtons)));
    mLayout->addWidget(cMatchFlagsLabel);

    setFocusProxy(cValueLineEdit);

    setMatchFlag(Qt::MatchStartsWith);
    setCaseSensitivity(Qt::CaseInsensitive);

    setFocusPolicy(Qt::StrongFocus);
}

N::SpecialFilterEditor::~SpecialFilterEditor()
{
}

Qt::CaseSensitivity N::SpecialFilterEditor::caseSensitivity() const
{
    return cSensitivity;
}

Qt::MatchFlag N::SpecialFilterEditor::matchFlag() const
{
    return cMatchFlag;
}

void N::SpecialFilterEditor::matchFlagsLabelClicked(Qt::MouseButtons buttons)
{
    if (buttons.testFlag(Qt::LeftButton)){
        if (cMatchFlag == Qt::MatchStartsWith){
            setMatchFlag(Qt::MatchEndsWith);
        } else if (cMatchFlag == Qt::MatchEndsWith){
            setMatchFlag(Qt::MatchContains);
        } else if (cMatchFlag == Qt::MatchContains){
            setMatchFlag(Qt::MatchStartsWith);
        }
    }
}

void N::SpecialFilterEditor::sensitivityLabelClicked(Qt::MouseButtons buttons)
{
    if (buttons.testFlag(Qt::LeftButton)){
        if (cSensitivity == Qt::CaseInsensitive){
            setCaseSensitivity(Qt::CaseSensitive);
        } else {
            setCaseSensitivity(Qt::CaseInsensitive);
        }
    }
}
void N::SpecialFilterEditor::setCaseSensitivity(Qt::CaseSensitivity sensitivity)
{
    if (sensitivity == Qt::CaseSensitive){
        cSensitivityLabel->setPixmap(QPixmap(QString::fromUtf8(":/qaiv/filter/case_sensitive")));
        cSensitivityLabel->setToolTip(tr("The filter is case sensitive"));
    } else {
        cSensitivityLabel->setPixmap(QPixmap(QString::fromUtf8(":/qaiv/filter/case_insensitive")));
        cSensitivityLabel->setToolTip(tr("The filter is case insensitive"));
    }
    cSensitivity = sensitivity;
}

void N::SpecialFilterEditor::setMatchFlag(Qt::MatchFlag flag)
{
    if (flag == Qt::MatchEndsWith){
        cMatchFlagsLabel->setPixmap(QPixmap(QString::fromUtf8(":/qaiv/filter/ends_with")));
        cMatchFlagsLabel->setToolTip(tr("The filter value matches the end of the item"));
    } else if (flag == Qt::MatchContains){
        cMatchFlagsLabel->setPixmap(QPixmap(QString::fromUtf8(":/qaiv/filter/contains")));
        cMatchFlagsLabel->setToolTip(tr("The filter value is contained in the item"));
    } else if (flag == Qt::MatchStartsWith){
        cMatchFlagsLabel->setPixmap(QPixmap(QString::fromUtf8(":/qaiv/filter/starts_with")));
        cMatchFlagsLabel->setToolTip(tr("The filter value matches the start of the item"));
    }
    cMatchFlag = flag;
}

void N::SpecialFilterEditor::setText(const QString & text)
{
    cValueLineEdit->setText(text);
}

QString N::SpecialFilterEditor::text() const
{
    return cValueLineEdit->text();
}
