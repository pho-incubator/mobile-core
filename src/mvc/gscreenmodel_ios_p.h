#ifndef SCREENMODEL_IOS_P_H
#define SCREENMODEL_IOS_P_H

#include <QObject>
#include "gmvc_global.h"
#include "gscreenmodel_p.h"

class ScreenModel;

class Q_MVC_EXPORT ScreenModelIosPrivate : public ScreenModelPrivate
{
    Q_OBJECT
public:
    ScreenModelIosPrivate(ScreenModel *q);
    void init();
    void showStatusBar(bool animated);
    void hideStatusBar(bool animated);

public slots:
    void calculateDPI();
};

#endif // SCREENMODEL_IOS_P_H
