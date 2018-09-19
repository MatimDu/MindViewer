#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>

QT_BEGIN_NAMESPACE
class QTextDocument;
QT_END_NAMESPACE

class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    Highlighter(QTextDocument *parent = 0);
protected:
    void highlightBlock(const QString &text) Q_DECL_OVERRIDE;
private:
    struct HighlightingRule{
        QRegExp pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;
    QTextCharFormat keywordFormat;
    QTextCharFormat startFormat;
    QTextCharFormat endFormat;
    QTextCharFormat checksumFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;
};

#endif // HIGHLIGHTER_H
