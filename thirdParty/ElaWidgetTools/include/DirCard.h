#ifndef DIRCARD_H
#define DIRCARD_H

#include <QPushButton>

#include "Def.h"
#include "stdafx.h"
class DirCardPrivate;
class ELA_EXPORT DirCard : public QPushButton
{
    Q_OBJECT
    Q_Q_CREATE(DirCard)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(QString, Title);
    Q_PROPERTY_CREATE_Q_H(QString, SubTitle);
    Q_PROPERTY_CREATE_Q_H(int, TitlePixelSize);
    Q_PROPERTY_CREATE_Q_H(int, SubTitlePixelSize);
    Q_PROPERTY_CREATE_Q_H(int, TitleSpacing);
    Q_PROPERTY_CREATE_Q_H(QPixmap, CardPixmap);
    Q_PROPERTY_CREATE_Q_H(QSize, CardPixmapSize);
    Q_PROPERTY_CREATE_Q_H(int, CardPixmapBorderRadius)
    Q_PROPERTY_CREATE_Q_H(ElaCardPixType::PixMode, CardPixMode);

public:
    explicit DirCard(QWidget* parent = nullptr);
    ~DirCard();
    void setCardPixmapSize(int width, int height);

protected:
    void paintEvent(QPaintEvent* event) override;
};

#endif // ELAREMINDERCARD_H
