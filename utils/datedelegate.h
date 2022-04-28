#ifndef DATEDELEGATE_H
#define DATEDELEGATE_H

#include <QDateTime>
#include <QStyledItemDelegate>


class DateDelegate: public QStyledItemDelegate {
  Q_OBJECT
public:
  DateDelegate(QObject * parent = 0):
    QStyledItemDelegate(parent)
  { }

  QString displayText(const QVariant &value, const QLocale &locale) const {
    if (value.type() == QVariant::DateTime) {
      return value.toDateTime().toString("dd.MM.yyyy hh:mm");
    }
    return QStyledItemDelegate::displayText(value, locale);
  }
};
#endif // DATEDELEGATE_H
