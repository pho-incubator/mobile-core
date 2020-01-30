#ifndef SCREENMODEL_ANDROID_P_H
#define SCREENMODEL_ANDROID_P_H

#include <QObject>
#include "gmvc_global.h"
#include "gscreenmodel_p.h"

class Q_MVC_EXPORT ScreenModelAndroidPrivate : public ScreenModelPrivate
{
    Q_OBJECT
public:
    ScreenModelAndroidPrivate(ScreenModel *q);
    void init();
    void showStatusBar(bool animated);
    void hideStatusBar(bool animated);

public slots:
    void calculateDPI();
};

#endif // SCREENMODEL_ANDROID_P_H
