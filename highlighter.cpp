#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    keywordPatterns << "\\bclass\\b" << "\\bconst\\b";
    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }
    startFormat.setFontWeight(QFont::Bold);
    startFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegExp("AA AA");//同步开始
    rule.format = startFormat;
    highlightingRules.append(rule);

    endFormat.setFontWeight(QFont::Bold);
    endFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegExp("55[\\s]$");//传输结束
    rule.format = endFormat;
    highlightingRules.append(rule);

    checksumFormat.setForeground(Qt::red);
    rule.pattern = QRegExp("\\s\\w\\w\\s$");//校验值
    rule.format = checksumFormat;
    highlightingRules.append(rule);
}

void Highlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
    setCurrentBlockState(0);
}
