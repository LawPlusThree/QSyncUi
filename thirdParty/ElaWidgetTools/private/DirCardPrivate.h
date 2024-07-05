#ifndef DIRCARDPRIVATE_H
#define DIRCARDPRIVATE_H

#include <QImage>
#include <QObject>
#include <QPixmap>
#include <QSize>

#include "Def.h"
#include "stdafx.h"
class DirCard;
class DirCardPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(DirCard)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(QString, Title);
    Q_PROPERTY_CREATE_D(QString, SubTitle);
    Q_PROPERTY_CREATE_D(int, TitlePixelSize);
    Q_PROPERTY_CREATE_D(int, SubTitlePixelSize);
    Q_PROPERTY_CREATE_D(int, TitleSpacing);
    Q_PROPERTY_CREATE_D(QPixmap, CardPixmap);
    Q_PROPERTY_CREATE_D(QSize, CardPixmapSize);
    Q_PROPERTY_CREATE_D(int, CardPixmapBorderRadius)
    Q_PROPERTY_CREATE_D(ElaCardPixType::PixMode, CardPixMode);

public:
    explicit DirCardPrivate(QObject* parent = nullptr);
    ~DirCardPrivate();

private:
    int _shadowBorderWidth{6};
    ElaApplicationType::ThemeMode _themeMode;
};

#endif // ELAREMINDERCARDPRIVATE_H
