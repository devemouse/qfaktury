#ifndef DATEFILTERPROXYMODELWIDGET_H
#define DATEFILTERPROXYMODELWIDGET_H

#include <QWidget>
#include <QDate>

namespace Ui {
    class DateFilterProxyModelWidget;
}

class DateFilterProxyModelWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DateFilterProxyModelWidget(QWidget *parent = 0);
    ~DateFilterProxyModelWidget();

public slots:
    void goToToday(void);

signals:
    void startDateChanged(QDate);
    void endDateChanged(QDate);

private slots:
    void on_prev_month_button_clicked();
    void on_next_month_button_clicked();


private:
    Ui::DateFilterProxyModelWidget *ui;
    void addMonth(int month);
};

#endif // DATEFILTERPROXYMODELWIDGET_H
