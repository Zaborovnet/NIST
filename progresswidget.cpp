#include "progresswidget.h"
#include <QProgressBar>
#include <QLabel>
#include <QVBoxLayout>

ProgressWidget::ProgressWidget(QWidget *parent)
    : QWidget(parent)
{
  // Настройка виджета
  QVBoxLayout* layout = new QVBoxLayout(this);

  _statusLabel_po = new QLabel("Прогресс процедур:", this);
  _progressBar_po = new QProgressBar(this);

  layout->addWidget(_statusLabel_po);
  layout->addWidget(_progressBar_po);
}

void ProgressWidget::setData(int completed, int prescribed)
{
  // Рассчитываем процент выполнения
  int percentage = (prescribed > 0) ? (completed * 100) / prescribed : 0;

  // Обновляем элементы интерфейса
  _progressBar_po->setValue(percentage);
  _statusLabel_po->setText(
      QString("Выполнено: %1 из %2 (%3%)")
          .arg(completed)
          .arg(prescribed)
          .arg(percentage)
      );
}
